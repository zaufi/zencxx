/**
 * \file
 *
 * \brief Class unilock_interface_impl (interface)
 *
 * \date Thu Jun 10 11:42:16 MSD 2010 -- Initial design
 */

#ifndef __SYSTEM__THREADS__DETAILS__UNILOCK_INTERFACE_IMPL_HH__
#  define __SYSTEM__THREADS__DETAILS__UNILOCK_INTERFACE_IMPL_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/condition.hh>
#  include <system/threads/mutex.hh>

// Standard includes
#  include <boost/call_traits.hpp>

namespace sys { namespace threads { namespace details {

template <typename Scheduler>
class unilock_interface_impl
{
public:
    typedef Scheduler scheduler_type;
    typedef typename scheduler_type::lock_params_type lock_params_type;
    typedef typename scheduler_type::unlock_params_type unlock_params_type;

private:
    threads::mutex m_mut;
    threads::condition m_cond;
    scheduler_type m_sched;

    typedef bool (unilock_interface_impl::*lock_func_t)(
        typename boost::call_traits<lock_params_type>::param_type
      , int
      , threads::mutex&
      );

    bool lock_impl(
        typename boost::call_traits<lock_params_type>::param_type p
      , int request_id
      , threads::mutex& mut
      )
    {
        while (!m_sched.try_lock(p, request_id))
            m_cond.wait(mut);
        return true;
    }
    bool try_lock_impl(
        typename boost::call_traits<lock_params_type>::param_type p
      , int request_id
      , threads::mutex&
      )
    {
        return m_sched.try_lock(p, request_id);
    }

    bool lock_decorator(typename boost::call_traits<lock_params_type>::param_type p, lock_func_t lf)
    {
        bool result = false;
        {
            threads::mutex::scoped_lock l(&m_mut);          // may throw critical_failure
            int request_id;
            try
            {
                request_id = m_sched.assign_request_id(p);
            }
            catch (...)
            {
                throw threads::exception::critical_failure(THIS_LOCATION(), EACCES);
            }
            try
            {
                result = (this->*lf)(p, request_id, m_mut); // call decorated member function
            }
            catch (const threads::exception::thread_canceled& e)
            {
                try
                {
                    m_sched.unassign_request_id(p, request_id);
                }
                catch (...)
                {
                    throw e;                                // rethrow thread_cancelled anyway
                }
            }
            try
            {
                m_sched.unassign_request_id(p, request_id);
            }
            catch (...)
            {
                throw threads::exception::critical_failure(THIS_LOCATION(), EACCES);
            }
        }
        m_cond.broadcast();
        return result;
    }

public:
    void lock(typename boost::call_traits<lock_params_type>::param_type p)
    {
        lock_decorator(p, &unilock_interface_impl::lock_impl);
    }
    bool try_lock(typename boost::call_traits<lock_params_type>::param_type p)
    {
        return lock_decorator(p, &unilock_interface_impl::try_lock_impl);
    }
    void unlock(typename boost::call_traits<unlock_params_type>::param_type p)
    {
        try
        {
            threads::mutex::scoped_lock l(&m_mut);          // may throw critical_failure
            m_sched.unlock(p);
        }
        catch (...)
        {
            throw threads::exception::critical_failure(THIS_LOCATION(), EACCES);
        }
        m_cond.broadcast();
    }
};

}}}                                                         // namespace details, threads, sys
#endif                                                      // __SYSTEM__THREADS__DETAILS__UNILOCK_INTERFACE_IMPL_HH__
