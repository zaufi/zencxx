/**
 * \file
 *
 * \brief Class mutex (interface)
 *
 * \date Sun Dec 20 19:25:42 MSK 2009 -- Initial design
 */

#ifndef __SYSTEM__THREADS__DETAILS__MUTEX_HH__
#  define __SYSTEM__THREADS__DETAILS__MUTEX_HH__

// Project specific includes
#  include <config.h>

// Standard includes
#  include <boost/noncopyable.hpp>
#  include <pthread.h>

namespace sys { namespace threads { namespace details {

class condition;

/**
 * \brief Wrapper around low level POSIX mutex
 */
class mutex : private boost::noncopyable
{
    friend class condition;
    pthread_mutex_t m_mut;

public:
    int initialize()
    {
        return pthread_mutex_init(&m_mut, 0);
    }
    int destroy()
    {
        return pthread_mutex_destroy(&m_mut);
    }
    int lock()
    {
        return pthread_mutex_lock(&m_mut);
    }
    int try_lock()
    {
        return pthread_mutex_trylock(&m_mut);
    }
    int unlock()
    {
        return pthread_mutex_unlock(&m_mut);
    }
};

}}}                                                         // namespace details, threads, sys
#endif                                                      // __SYSTEM__THREADS__DETAILS__MUTEX_HH__
