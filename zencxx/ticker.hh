/**
 * \file
 *
 * \brief Class \c zencxx::ticker (interface)
 *
 * \date Tue Jun 29 09:29:40 MSD 2010 -- Initial design
 * \date Tue Sep 20 00:15:14 MSK 2011 -- Rewrite \c ticker from scratch to use \c boost::asio::io_service
 *
 * \todo Research the best way to pass \c std::chrono::system_clock::duration instances to functions.
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

#ifndef __ZENCXX__TICKER_HH__
# define __ZENCXX__TICKER_HH__

// Project specific includes
# include <zencxx/details/export.hh>
# include <zencxx/details/registered_job.hh>
# include <zencxx/exception.hh>

// Standard includes
# include <boost/asio/io_service.hpp>
# include <boost/thread/lock_types.hpp>
# include <boost/thread/mutex.hpp>
# include <cassert>
# include <functional>
# include <memory>
# include <unordered_map>
# include <utility>

namespace zencxx {
/**
 * \brief Manage execution of one-time and/or periodic jobs
 *
 * To schedule a job one can use an overloaded method \c call() which can
 * be used to
 * - call a single run job immediately (w/o particular time)
 * - call a single run job at specified time. If scheduled time already in
 *   the past at moment of a \c call(), the job will be executed immediately.
 * - call a periodic job. In this case the next run occurs at \f$ now + period \f$,
 *   the next one at \f$ now+2*period \f$, & etc.
 * - call a periodic job starting at specified time -- i.e. the first call
 *   occurs at \f$ time \f$, the next one at \f$ time+period \f$, & etc. If specified
 *   time already in the past, the next call take place at \f$ now + (now-time) \bmod{period} \f$
 *
 * \todo Add \c call() overload with initial start time and period.
 * \todo Need to rethink and redesign this class to support system and steady clock depending on
 * requirements.
 */
class ZENCXX_EXPORT ticker : public std::enable_shared_from_this<ticker>
{
    /// Type to hold all registered jobs
    typedef std::unordered_map<unsigned, details::registered_job> registered_jobs;

public:
    /// Class to represent a registered job to the "external world"
    class ZENCXX_EXPORT job
    {
        friend class ticker;

        std::weak_ptr<ticker> m_ticker_wptr;                ///< Weak pointer to parent ticker
        unsigned m_job_id;                                  ///< ID of the registered job (key in jobs container)

        /// Make an instance of job from pointer to parent
        /// ticker and registered job iterator
        job(
            std::weak_ptr<ticker>&& parent_ticker           ///< Pointer to parent ticker
          , const unsigned job_id                           ///< ID of the registered job
          ) ZENCXX_NO_EXPORT;

    public:
        /// Allows default initialization for STL containers compatibility
        job() : m_job_id(0) {}
        job(const job&) = default;                          ///< Default copy ctor
        job& operator=(const job&) = default;               ///< Default copy-assign operator
        job(job&&) noexcept;                                ///< Move ctor
        job& operator=(job&&) noexcept;                     ///< Move-assign operator

        enum struct start_type : unsigned
        {
            auto_start                                      ///< Schedule job to start automatically after registration
          , manual_start                                    ///< Task required explicit start
        };
        enum struct State : unsigned
        {
            scheduled                                       ///< The job is scheduled to execute
          , running                                         ///< The job is running right now
          , stopped                                         ///< The job is stopped for scheduling
          , stale                                           ///< The job is invalid
        };
        void start();                                       ///< Manual start
        void stop();                                        ///< Suspend given job from schedule
        void cancel();                                      ///< Remove job from scheduler
    };
    friend class job;
    /// Ticker's exceptions group
    struct ZENCXX_EXPORT exception : public zencxx::exception
    {
        struct resource_error;
        struct stale_job;
    };

    /// The only way to make an instance of \c ticker class
    static std::shared_ptr<ticker> create(boost::asio::io_service&);
    /// Stop ticker, cancel all jobs still waiting
    ~ticker();

    /// Register callback to execute periodically
    template <typename Functor>
    job call(
        Functor
      , const std::chrono::system_clock::duration
      , job::start_type = job::start_type::auto_start
      );
    /// Register callback to execute once at given time
    template <typename Functor>
    job call(
        Functor
      , const std::chrono::system_clock::time_point
      );
    /// Register a callback to execute once ASAP
    template <typename Functor>
    void call(Functor);

    /// Get I/O underlaid service
    boost::asio::io_service& service();

  private:
    /// Use I/O service
    ZENCXX_NO_EXPORT ticker(boost::asio::io_service&);
    /// Delete copy ctor
    ticker(const ticker&) = delete;
    /// Delete copy-assign operator
    ticker& operator=(const ticker&) = delete;

    /// Append a job to the list of registered jobs
    job append_job(details::registered_job&&);
    /// Put a given job into a schedule (w/ lock acquired)
    ZENCXX_NO_EXPORT void reschedule_job(
        registered_jobs::iterator
      , const boost::unique_lock<boost::mutex>&
      );
    ZENCXX_NO_EXPORT void one_time_job_handler(
        registered_jobs::iterator
      , const boost::system::error_code&
      );
    ZENCXX_NO_EXPORT void periodic_job_handler(
        registered_jobs::iterator
      , const boost::system::error_code&
      );
    ZENCXX_NO_EXPORT void remove_if_needed(registered_jobs::iterator);
    ZENCXX_NO_EXPORT void remove_if_needed(
        registered_jobs::iterator
      , boost::unique_lock<boost::mutex>&
      );
    void remove_job(const unsigned);
    void remove_job(registered_jobs::iterator, boost::unique_lock<boost::mutex>&);
    void pause_job(const unsigned);
    void resume_job(const unsigned);

    /// \name MT-safe members
    //@{
    boost::asio::io_service& m_srv;                         ///< I/O service to use
    //@}

    /// MT-unsafe members w/ their guards
    //@{
    mutable boost::mutex m_jobs_mut;                        ///< Mutex to guard access to everything :)
    registered_jobs m_jobs;                                 ///< Holder for registered jobs
    //@}
};

struct ZENCXX_EXPORT ticker::exception::resource_error : public ticker::exception {};

struct ZENCXX_EXPORT ticker::exception::stale_job : public ticker::exception
{
    stale_job() : ticker::exception()
    {
        *this << exception::reason("No such job registered in a ticker or latter is dead");
    }
};

inline ticker::job::job(
    std::weak_ptr<ticker>&& parent_ticker
  , const unsigned job_id
  )
  : m_ticker_wptr(std::move(parent_ticker))
  , m_job_id(job_id)
{
}

inline ticker::job::job(job&& other) noexcept
  : m_ticker_wptr(std::move(other.m_ticker_wptr))
  , m_job_id(other.m_job_id)
{
}

inline ticker::job& ticker::job::operator=(job&& other) noexcept
{
    if (this != &other)
    {
        new(this) job(std::move(other));
    }
    return *this;
}

inline void ticker::job::start()
{
    if (std::shared_ptr<ticker> tkr = m_ticker_wptr.lock())
        tkr->resume_job(m_job_id);
    else
        ZENCXX_THROW(exception::stale_job());
}

inline void ticker::job::stop()
{
    if (std::shared_ptr<ticker> tkr = m_ticker_wptr.lock())
        tkr->pause_job(m_job_id);
    else
        ZENCXX_THROW(exception::stale_job());
}

inline void ticker::job::cancel()
{
    if (std::shared_ptr<ticker> tkr = m_ticker_wptr.lock())
    {
        tkr->remove_job(m_job_id);
        // Disallow future operations using removed job externally sotred
        m_ticker_wptr.reset();
    }
}

inline std::shared_ptr<ticker> ticker::create(boost::asio::io_service& srv)
{
    return std::shared_ptr<ticker>(new ticker(srv));
}

inline ticker::ticker(boost::asio::io_service& srv)
  : m_srv(srv)
{
}

template <typename Functor>
inline void ticker::call(Functor nulary_functor)
{
    m_srv.post(std::move(nulary_functor));
}

template <typename Functor>
inline ticker::job ticker::call(
    Functor nulary_functor
  , const std::chrono::system_clock::time_point at_time
  )
{
    return append_job(
        details::registered_job(
            std::unique_ptr<boost::asio::system_timer>(
                // set absolute expiration time
                new boost::asio::system_timer(m_srv, at_time)
              )
          , std::chrono::system_clock::duration::zero()     // Ignored parameter
          , std::move(nulary_functor)                       // Function to execute
          , details::registered_job::type::once             // No periodic runs required
          , details::registered_job::state::running         // Immediate schedule required
          )
      );
}

template <typename Functor>
inline ticker::job ticker::call(
    Functor nulary_functor
  , const std::chrono::system_clock::duration interval
  , job::start_type how2start
  )
{
    return append_job(
        details::registered_job(
            std::unique_ptr<boost::asio::system_timer>(
                // set expiration time relative to now
                new boost::asio::system_timer(m_srv, interval)
              )
          , interval
          , std::move(nulary_functor)                       // Function to execute
          , details::registered_job::type::periodic         // Periodic runs required
          , (
                job::start_type::auto_start == how2start    // Decide how to start
                  ? details::registered_job::state::running
                  : details::registered_job::state::stopped
              )
          )
      );
}

inline boost::asio::io_service& ticker::service()
{
    return m_srv;
}

inline void ticker::remove_job(const unsigned job_id)
{
    boost::unique_lock<boost::mutex> l(m_jobs_mut);         // Guard iterator access under the lock
    auto job_it = m_jobs.find(job_id);
    if (job_it != end(m_jobs))
        remove_job(job_it, l);
}

}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__TICKER_HH__
