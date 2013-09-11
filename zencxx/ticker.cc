/**
 * \file
 *
 * \brief Class \c zencxx::ticker (implementation)
 *
 * \date Tue Jun 29 09:29:40 MSD 2010 -- Initial design
 */
/*
 * ZenCxx is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ZenCxx is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

// Project specific includes
#include <zencxx/ticker.hh>

// Standard includes
#include <atomic>

namespace zencxx { namespace {
std::atomic<unsigned> s_next_job_id = {0};
}                                                           // anonymous namespace

/**
 * Ticker destroy should be really triky cuz there is different tasks could be
 * in different state. First of all we have to lock the \c m_jobs container and
 * check the particular job state:
 * \li for \c stopped jobs it is safe to earese right now
 * \li for \c running jobs we have to mark it as \c scheduled_for_removal
 * and wait 'till user functor finished and would remove the job
 * \li for \c scheduled jobs we have to send \c cancel() to their timer
 * and wait 'till asio handler erase the job
 * \li for \c scheduled_for_removal nothing to do...
 */
ticker::~ticker()
{
    {
        boost::unique_lock<boost::mutex> l(m_jobs_mut);
        for (auto job_it = begin(m_jobs), last = end(m_jobs); job_it != last;)
        {
            switch (job_it->second.m_state)
            {
                case details::registered_job::state::stopped:
                    m_jobs.erase(job_it++);                 // Erase current job and move to next one...
                    break;
                case details::registered_job::state::scheduled:
                    job_it->second.m_state = details::registered_job::state::scheduled_for_removal;
                    job_it->second.m_timer->cancel();
                    job_it++;
                    break;
                case details::registered_job::state::running:
                    job_it->second.m_state = details::registered_job::state::scheduled_for_removal;
                case details::registered_job::state::scheduled_for_removal:
                    ++job_it;                               // Move to next job
                    // Nothing to do...
                    break;
                default:
                    assert(!"Unexpected job state");
                    break;
            }
        }
    }
    // Wait 'till all unfinished jobs are gone...
    /// \todo Is it reliable?
    while (true)
    {
        boost::unique_lock<boost::mutex> l(m_jobs_mut);
        if (m_jobs.empty()) break;
    }
}


/**
 * \pre The initial state of job could be \c running or \c stopped to indicate
 * whether it must be scheduled now (first case) or manually later...
 * In case of \c stopped it would remains the same, otherwise, after rescheduling,
 * it would be changed to \c scheduled.
 *
 * \post At function exit only \c scheduled or \c stopped states are expected!
 */
ticker::job ticker::append_job(details::registered_job&& job_info)
{
#  ifndef NDEBUG
    try
    {
        shared_from_this();
    }
    catch (const boost::bad_weak_ptr&)
    {
        assert(!"Instances of ticker class must be shared pointers!");
    }
    assert(
        "Initail check state failed"
      && (details::registered_job::state::stopped == job_info.m_state
      || details::registered_job::state::running == job_info.m_state
      ));
#  endif                                                    // NDEBUG

    boost::unique_lock<boost::mutex> l(m_jobs_mut);
    registered_jobs::iterator new_job = end(m_jobs);
    unsigned id = 0;
    try
    {
        // Find unique ID for the job
        do
        {
            id = s_next_job_id++;
        }
        while (id && m_jobs.find(id) != end(m_jobs));
        // Move a new job into the container of registered jobs
        new_job = m_jobs.insert(end(m_jobs), std::make_pair(id, std::move(job_info)));

        // Is it required to schedule the task right now?
        if (details::registered_job::state::stopped != new_job->second.m_state)
        {
            reschedule_job(new_job, l);                     // Update schedule
            assert("Sanity check" && details::registered_job::state::scheduled == new_job->second.m_state);
        }
        // No reschedule required...
    }
    catch (const std::bad_alloc&)
    {
        if (new_job != end(m_jobs))                         // Remove registration if it was success
            m_jobs.erase(new_job);
        ZENCXX_THROW(exception::resource_error()) << exception::reason("Not enough memory to register a job");
    }
    assert("Sanity check" && new_job != end(m_jobs));
    assert(
        "Postcondition check state failed"
      && (details::registered_job::state::stopped == new_job->second.m_state
      || details::registered_job::state::scheduled == new_job->second.m_state
      ));
    return job(shared_from_this(), id);
}

void ticker::reschedule_job(registered_jobs::iterator job_it, const boost::unique_lock<boost::mutex>&)
{
    assert("Sanity check" && job_it != end(m_jobs));
    assert(
        "Job doesn't expected to be in schedule aleady"
      && details::registered_job::state::scheduled != job_it->second.m_state
      );

    if (details::registered_job::type::once == job_it->second.m_type)
    {
        job_it->second.m_timer->async_wait(
            [=](const boost::system::error_code& err)
            {
                this->one_time_job_handler(job_it, err);
            }
          );
    }
    else
    {
        job_it->second.m_timer->async_wait(
            [=](const boost::system::error_code& err)
            {
                this->periodic_job_handler(job_it, err);
            }
          );
    }
    // set new state to 'scheduled' meaning that I/O service starts to work
    job_it->second.m_state = details::registered_job::state::scheduled;
}

/**
 * There is 2 cases possibe whan this function will be invoked:
 * \li While we've been scheduled and wait a timer event, someone has remove this job
 *     In that case \c err parameter would be \c boost::asio::error::operation_aborted.
 * \li Otherwise this is normal launch...
 *
 * \note For \b both cases passed iterator <b>still valid</b>!
 */
void ticker::one_time_job_handler(registered_jobs::iterator job_it, const boost::system::error_code& err)
{
    // Check if we were called as timer cancel notification
    if (boost::asio::error::operation_aborted != err)
    {
        // No! We didn't!
        std::function<void()> nulary_function;
        {
            boost::unique_lock<boost::mutex> l(m_jobs_mut); // Guard iterator access under the lock
            // Assert the job type and status
            assert(
                "Type of job expected to be 'one-time'"
              && details::registered_job::type::once == job_it->second.m_type
              );
            assert(
                "Job state expected to be 'scheduled'"
              && details::registered_job::state::scheduled == job_it->second.m_state
              );
            // Yeah! We r still scheduled!
            /// \todo Think about to avoid copying!
            nulary_function = job_it->second.m_functor;     // Make a copy of user functor
            // Change state to 'running' cuz we r intended to execute it finally!
            job_it->second.m_state = details::registered_job::state::running;
        }
        nulary_function();                                  // Ok! Ready to do the job...
        boost::unique_lock<boost::mutex> l(m_jobs_mut);     // Guard iterator access under the lock
        job_it->second.m_state = details::registered_job::state::scheduled_for_removal;
        remove_if_needed(job_it, l);                        // Don't need this job anymore...
    }
    else
    {
        // Check if job needs to be removed
        remove_if_needed(job_it);
    }
}

/**
 * \copydoc ticker::one_time_job_handler
 */
void ticker::periodic_job_handler(registered_jobs::iterator job_it, const boost::system::error_code& err)
{
    // Check if we were called as timer cancel notification
    if (boost::asio::error::operation_aborted != err)
    {
        // No! We didn't!
        std::function<void()> nulary_function;
        {
            boost::unique_lock<boost::mutex> l(m_jobs_mut); // Guard iterator access under the lock
            // Assert the job type and status
            assert(
                "Type of a job expected to be 'periodic'"
              && details::registered_job::type::periodic == job_it->second.m_type
              );
            assert(
                "Job state expected to be 'scheduled'"
              && details::registered_job::state::scheduled == job_it->second.m_state
              );
            // Yeah! We r still scheduled!
            /// \todo Think about to avoid copying!
            nulary_function = job_it->second.m_functor;     // Make a copy of user functor
            // Change state to 'running' cuz we r intended to execute it finally!
            job_it->second.m_state = details::registered_job::state::running;
        }

        nulary_function();                                  // Ok! Ready to do the job...
        boost::unique_lock<boost::mutex> l(m_jobs_mut);     // Guard iterator access under the lock
        // Check if job still needed to reschedule
        if (details::registered_job::state::running == job_it->second.m_state)
        {
            // Reset timer for next event (tick)
            job_it->second.m_timer->expires_from_now(job_it->second.m_interval);
            reschedule_job(job_it, l);
        }
        else
        {
            remove_if_needed(job_it, l);                    // Probably don't need this job anymore...
        }
    }
    else
    {
        // Check if job needs to be removed
        remove_if_needed(job_it);
    }
}

inline void ticker::remove_if_needed(registered_jobs::iterator job_it)
{
    auto l = boost::unique_lock<boost::mutex>{m_jobs_mut};  // Guard iterator access under the lock
    remove_if_needed(job_it, l);
}

void ticker::remove_if_needed(registered_jobs::iterator job_it, boost::unique_lock<boost::mutex>&)
{
    assert(
        "Job couldn't be in a running state right now! Review your code!"
      && details::registered_job::state::running != job_it->second.m_state
      );
    // Check if someone called Cancel() for a given job
    if (details::registered_job::state::scheduled_for_removal == job_it->second.m_state)
    {
        m_jobs.erase(job_it);                               // Ok, remove the job finally
    }                                                       // Otherwise, someone just stopped the job...
}

void ticker::remove_job(registered_jobs::iterator job_it, boost::unique_lock<boost::mutex>& l)
{
    assert("Sanity check" && job_it != end(m_jobs));
    assert(
        "Job has been already marked for removal! Review your code!"
      && details::registered_job::state::scheduled_for_removal != job_it->second.m_state
      );

    // If this job is waiting on timer, we need to cancel it!
    const bool need_to_cancel = details::registered_job::state::scheduled == job_it->second.m_state;
    // If this job is running right now, we have to just mark it, asio completion handler
    // would remove it after user provided functor finished.
    const bool can_remove_right_now = details::registered_job::state::running != job_it->second.m_state;
    // Do not remove it! Just mark for deletion... Handler is responsible to
    // do actual remove...
    job_it->second.m_state = details::registered_job::state::scheduled_for_removal;
    // Send cancel to timer if needed...
    if (need_to_cancel)
    {
        job_it->second.m_timer->cancel();
        // This would lead to call to hander w/ error code, and then
        // the actual remove would happened...
    }
    else if (can_remove_right_now)
    {
        // Ok, the job (was) in stopepd state, so we may remove it right now...
        remove_if_needed(job_it, l);
    }
}

void ticker::pause_job(const unsigned job_id)
{
    auto l = boost::unique_lock<boost::mutex>{m_jobs_mut};  // Guard iterator access under the lock
    auto job_it = m_jobs.find(job_id);
    if (job_it != end(m_jobs))
    {
        assert(
            "Job has been already marked as stopped! Review your code!"
          && details::registered_job::state::stopped != job_it->second.m_state
          );

        // If this job is not currently running, we need to cancel its' timer.
        const bool need_to_cancel = details::registered_job::state::running != job_it->second.m_state;
        // Anyway we must mark it as stopped before handler would be executed (yeah, w/ error code)
        // or user provided functor finished.
        job_it->second.m_state = details::registered_job::state::stopped;
        if (need_to_cancel)
        {
            job_it->second.m_timer->cancel();
            // This would lead to call to hander w/ error code, and then
            // the actual remove would happened...
        }
    }
    else ZENCXX_THROW(exception::stale_job());
}

/// \todo What if timer already in the past?
void ticker::resume_job(const unsigned job_id)
{
    auto l = boost::unique_lock<boost::mutex>{m_jobs_mut};  // Guard iterator access under the lock
    auto job_it = m_jobs.find(job_id);
    if (job_it != end(m_jobs))
    {
        assert(
            "Job has been already marked as scheduled! Review your code!"
          && details::registered_job::state::scheduled != job_it->second.m_state
          );
        reschedule_job(job_it, l);
    }
    else ZENCXX_THROW(exception::stale_job());
}

}                                                           // namespace zencxx
