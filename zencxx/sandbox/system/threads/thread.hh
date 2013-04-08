/**
 * \file
 *
 * \brief Class thread
 *
 * \date Fri Sep  5 01:58:53 MSD 2008 -- Initial design
 */

#ifndef __SYSTEM__THREADS__THREAD_HH__
#  define __SYSTEM__THREADS__THREAD_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/cancel.hh>
#  include <system/threads/details/thread.hh>
#  include <system/threads/details/thread_data.hh>

// Standard includes
#  include <boost/function.hpp>

namespace sys { namespace threads {

template <typename ResultType = void>
class thread
{
    typedef boost::function<ResultType()> thread_function_type;
    typedef typename thread_function_type::result_type result_type;

    typename details::thread m_thr;
    thread_function_type m_functor;
    mutable mutex m_mut;
    details::thread_data* m_thr_data;
    details::return_value_wrapper<result_type> m_ret;
    bool m_can_join;
    bool m_is_running;

public:
    explicit thread(const thread_function_type& f)
      : m_functor(f)
      , m_thr_data(0)
      , m_can_join(false)
      , m_is_running(false) {}
    //
    void spawn()
    {
        m_thr.spawn(&thread::thread_, this);
    }
    /// \todo Is this reasonable to cancel thread in destructor??
    ~thread()
    {
        assert("Thread shouldn't be running" && !is_running());
        mutex::scoped_lock l(&m_mut);
        if (m_thr_data) delete m_thr_data;
    }
    bool is_running() const
    {
        mutex::scoped_lock l(&m_mut);
        return m_is_running;
    }
    result_type join()
    {
        /// \todo Check that thread is running! (Need new exception type)
        m_thr.join();
        mutex::scoped_lock l(&m_mut);
        m_can_join = false;
        return m_ret.return_result();
    }
    bool can_join() const
    {
        mutex::scoped_lock l(&m_mut);
        return m_can_join;
    }
    bool is_cancel() const
    {
        mutex::scoped_lock l(&m_mut);
        if (m_thr_data)
        {
            mutex::scoped_lock g(&m_thr_data->m_mut);
            return m_thr_data->m_is_canceled;
        }
        return false;
    }
    void cancel()
    {
        mutex::scoped_lock l(&m_mut);
        if (m_thr_data)
        {
            mutex::scoped_lock g(&m_thr_data->m_mut);
            m_thr_data->m_is_canceled = true;
            if (m_thr_data->m_cur_cond)
            {
                int IGNORE_UNUSED rc = m_thr_data->m_cur_cond->broadcast();
                assert("condition broadcast failure" && 0 == rc);
            }
        }
        else assert(!"Trying to cancel non running thread?");
    }

private:
    /// \todo Review exceptions safety of this function required
    static void* thread_(void* p)
    {
        int tmp;
        int IGNORE_UNUSED result = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &tmp);
        assert("Smth really wrong with pthread implementation!" && 0 == result);

        thread* const this_ = static_cast<thread* const>(p);
        //
        {
            mutex::scoped_lock l(&this_->m_mut);
            this_->m_thr_data = details::get_specific_thread_data();
            this_->m_is_running = true;
        }
        //
        this_->m_ret.assign_result_of(this_->m_functor);
        {
            mutex::scoped_lock l(&this_->m_mut);
            this_->m_can_join = true;
            this_->m_is_running = false;
        }
        pthread_exit(0);                                    // this is preferred way to exit from thread.
        return 0;                                           // this is needed to suppress gcc warning...
    }
};

}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__THREAD_HH__
