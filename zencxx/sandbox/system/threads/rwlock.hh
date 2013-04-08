/**
 * \file
 *
 * \brief Class rwlock (interface)
 *
 * \date Thu Nov  5 19:58:55 MSK 2009 -- Initial design
 */

#ifndef __SYSTEM__THREADS__RWLOCK_HH__
#  define __SYSTEM__THREADS__RWLOCK_HH__

// Project specific includes
#  include <config.h>
#  include <system/config.hh>
#  include <system/gnu_extensions.hh>
#  include <system/threads/details/scoped_lock_base.hh>
#  include <system/threads/exception.hh>
#  include <system/os/posix_time/time.hh>

// Standard includes
#  include <boost/function.hpp>
#  include <boost/bind.hpp>
#  include <boost/noncopyable.hpp>
#  include <cassert>
#  include <cerrno>
#  include <pthread.h>

namespace sys { namespace threads {

struct rwlock : public boost::noncopyable
{
    pthread_rwlock_t m_lock;                                //!< Underlayed POSIX rwlock

public:
    enum lock_type
    {
        read, write
    };
    /// Default constructor
    rwlock()
    {
        int rc = pthread_rwlock_init(&m_lock, 0);
        if (rc)
            throw threads::exception::critical_failure(THIS_LOCATION(), rc);
        // paranoid checks in debug mode
        assert(
            "Check if internal rwlock construction Ok"
          && 0 == pthread_rwlock_tryrdlock(&m_lock)
          && 0 == pthread_rwlock_unlock(&m_lock)
          );
    }
    ~rwlock()
    {
        // we don't care about real error code here
        int IGNORE_UNUSED result = pthread_rwlock_destroy(&m_lock);
        assert("sys::threads::rwlock destroy failure" && 0 == result);
    }
    /// Accure a lock
    void lock(lock_type lt)
    {
        // choose appropriate underlayed lock function
        int lock_result = (read == lt ? &pthread_rwlock_rdlock : &pthread_rwlock_wrlock)(&m_lock);

        if (0 == lock_result)
            return;
        if (EDEADLK == lock_result)
            assert(!"Deadlock detected. Calling thread already has an inconsistent lock type!");
        else if (EAGAIN == lock_result)
            assert(!"Insufficient system resources to complete a lock!");
        else if (EINVAL == lock_result)
            assert(!"EINVAL usually means uninitialized or corrupted rwlock");
    }
    bool try_lock(lock_type lt)
    {
        // choose appropriate underlayed lock function
        int lock_result = (read == lt ? &pthread_rwlock_tryrdlock : &pthread_rwlock_trywrlock)(&m_lock);
        if (EDEADLK == lock_result)
            assert(!"Deadlock detected. Calling thread already has an inconsistent lock type!");
        else if (EBUSY != lock_result)
            assert(!"Insufficient system resources to complete a lock!");
        return 0 == lock_result;
    }
    void unlock(lock_type/* lt*/)
    {
        int unlock_result = pthread_rwlock_unlock(&m_lock);
        assert("Current thread doesn't obtain any lock..." && EPERM != unlock_result);
        if (0 != unlock_result)
            throw threads::exception::critical_failure(THIS_LOCATION(), unlock_result);
    }
    /**
     * \brief Mutex \c guard is a wrapper for mutex object that obtains lock in constructor
     *  and releases it in destructor.
     *
     * It is recommended to use \c scoped_lock to manipulate mutex whenever is possible,
     * except specific cases when it is required to access lock and unlock functions
     * explicitly. The main reason is that it is always guaranteed that mutex will be
     * unlocked on exit, regardless of the way the procedure ends (it can be either normal
     * termination or exception).
     */
    class scoped_lock : private details::scoped_lock_base
    {
    public:
        template <typename Lockable>
        explicit scoped_lock(Lockable* l, typename Lockable::lock_type t)
          : details::scoped_lock_base(
                boost::bind(&Lockable::lock, l, t)
              , boost::bind(&Lockable::unlock, l, t)
              )
        {
        }
    };
};

}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__RWLOCK_HH__
