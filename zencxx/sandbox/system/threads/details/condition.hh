/**
 * \file
 *
 * \brief Class condition (interface)
 *
 * \date Sun Dec 20 19:25:35 MSK 2009 -- Initial design
 */

#ifndef __SYSTEM__THREADS__DETAILS__CONDITION_HH__
#  define __SYSTEM__THREADS__DETAILS__CONDITION_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/mutex.hh>
#  include <system/os/posix_time/time.hh>

// Standard includes
#  include <boost/noncopyable.hpp>
#  include <pthread.h>

namespace sys { namespace threads { namespace details {

/**
 * \brief Wrapper around low level POSIX condition variable
 */
class condition : private boost::noncopyable
{
    pthread_cond_t m_cond;                                  //!< Underlayed system condition variable

public:
    int initialize()
    {
        return pthread_cond_init(&m_cond, 0);
    }
    int destroy()
    {
        return pthread_cond_destroy(&m_cond);
    }
    int signal()
    {
        return pthread_cond_signal(&m_cond);
    }
    int broadcast()
    {
        return pthread_cond_broadcast(&m_cond);
    }
    int wait(mutex& mut)
    {
        return pthread_cond_wait(&m_cond, &mut.m_mut);
    }
    int timed_wait(mutex& mut, const os::posix_time::time& t)
    {
        timespec ts = t;
        return pthread_cond_timedwait(&m_cond, &mut.m_mut, &ts);
    }
};

}}}                                                         // namespace details, threads, sys
#endif                                                      // __SYSTEM__THREADS__DETAILS__CONDITION_HH__
