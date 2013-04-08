/**
 * \file
 *
 * \brief Class thread (interface)
 *
 * \date Mon Nov  9 03:07:44 MSK 2009 -- Initial design
 */

#ifndef __SYSTEM__THREADS__DETAILS__THREAD_HH__
#  define __SYSTEM__THREADS__DETAILS__THREAD_HH__

// Project specific includes
#  include <config.h>
#  include <system/gnu_extensions.hh>
#  include <system/threads/exception.hh>

// Standard includes
#  include <boost/noncopyable.hpp>
#  include <cassert>
#  include <pthread.h>

namespace sys { namespace threads { namespace details {

/**
 * \brief Implementation of thread-wrapper using POSIX-threads functions.
 * Just a couple of wrappers. Main \c thread primitive is in thread.hh.
 */

class thread : public boost::noncopyable
{
    pthread_t m_tid;

public:
    void spawn(void* (*f)(void*), void* arg)
    {
        if (int rc = pthread_create(&m_tid, 0, f, arg))
            throw threads::exception::critical_failure(THIS_LOCATION(), rc);
    }
    void join(void)
    {
        int IGNORE_UNUSED result = pthread_join(m_tid, 0);
#  ifndef NDEBUG
        if (result)
        {
            assert("Deadlock detected. U have to fix your code!" && EDEADLK != result);
            assert("Concurrent thread join detected. U have to fix your code!" && EINVAL != result);
            assert("No thread with the ID thread could be found" && ESRCH != result);
            assert("pthread_join() returns with unknown error!" && 0 != result);
        }
#  endif
    }
};

template <typename T>
struct return_value_wrapper
{
    T m_value;
    const T& return_result() const
    {
        return m_value;
    }
    template <typename F>
    void assign_result_of(F f)
    {
        m_value = f();
    }
};

template <>
struct return_value_wrapper<void>
{
    void return_result() const {}
    template <typename F>
    void assign_result_of(F f)
    {
        f();
    }
};

}}}                                                         // namespace details, threads, sys
#endif                                                      // __SYSTEM__THREADS__DETAILS__THREAD_HH__
