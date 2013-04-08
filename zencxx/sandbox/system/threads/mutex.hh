/**
 * \file
 *
 * \author See AUTHORS file from $(top_srcdir)
 *
 * \brief Class mutex module (interface)
 *
 * \date Mon Apr 19 14:56:08 MSD 2004 -- Initial design
 */

#ifndef __SYSTEM__THREADS__MUTEX_HH__
#  define __SYSTEM__THREADS__MUTEX_HH__

// Project specific includes
#  include <config.h>
#  include <system/gnu_extensions.hh>
#  include <system/config.hh>
#  include <system/threads/exception.hh>
#  include <system/threads/details/mutex.hh>

// Standard includes
#  include <cassert>
#  include <cerrno>

namespace sys { namespace threads {

class condition;                                            // fwd decl

/// Implementation of \c mutex primitive using POSIX API
class mutex
{
    friend class condition;
    details::mutex m_mut;                                   ///< Underlayed OS specific  mutex

public:

    mutex()
    {
        if ( int rc = m_mut.initialize() )
            throw threads::exception::critical_failure(THIS_LOCATION(), rc);
        // paranoid checks in debug mode
        assert(
            "Check if internal mutex construction Ok"
          && 0 == m_mut.try_lock()
          && 0 == m_mut.unlock()
          );
    }
    ~mutex()
    {
        // we don't care about real error code here
        int IGNORE_UNUSED rc = m_mut.destroy();
        assert("sys::threads::mutex destroy failure" && 0 == rc);
    }
    /**
     * Accure a mutex
     *
     * \throw threads::exception::critical_failure if unable to lock underlayed
     *  mutex or underlayed condition variable's waiting function returns an error.
     * Latter almost impossible (only if smbd corrupt this instance's memory)
     */
    void lock()
    {
        int IGNORE_UNUSED rc = m_mut.lock();
        assert("sys::threads::mutex::lock() failure" && 0 == rc);
    }
    bool try_lock()
    {
        int IGNORE_UNUSED rc = m_mut.try_lock();
        if (rc && EBUSY != rc)
            assert(!"sys::threads::mutex::try_lock() failure");
        return 0 == rc;
    }
    void unlock()
    {
        int IGNORE_UNUSED rc = m_mut.unlock();
        assert("sys::threads::mutex::unlock() failure" && 0 == rc);
    }
    /**
     * \brief Wrapper for mutex object that obtains lock in constructor and releases it in destructor.
     *
     * It is recommended to use \c scoped_lock to manipulate mutex whenever is possible,
     * except specific cases when it is required to access lock and unlock functions
     * explicitly. The main reason is that it is always guaranteed that mutex will be
     * unlocked on exit, regardless of the way the procedure ends (it can be either normal
     * termination or exception).
     */
    class scoped_lock
    {
        mutex* m_mut;

    public:
        explicit scoped_lock(mutex* m) : m_mut(m)
        {
            m_mut->lock();
        }
        ~scoped_lock()
        {
            m_mut->unlock();
        }
    };
};

}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__MUTEX_HH__
