/**
 * \file
 *
 * \author See AUTHORS file from $(top_srcdir)
 *
 * \brief Class condition module (interface)
 *
 * \date Mon Apr 19 14:56:16 MSD 2004 -- Initial design
 */

#ifndef __SYSTEM__THREADS__CONDITION_HH__
#  define __SYSTEM__THREADS__CONDITION_HH__

// Project specific includes
#  include <config.h>
#  include <system/gnu_extensions.hh>
#  include <system/threads/exception.hh>
#  include <system/threads/details/cancelation_checker.hh>
#  include <system/threads/details/condition.hh>
#  include <system/os/posix_time/time.hh>

// Standard includes
#  include <cassert>
#  include <cerrno>

namespace sys { namespace threads {

/**
 * \brief Implementation of \c condition primitive using POSIX-threads functions.
 */
class condition
{
    details::condition m_cond;                              //!< We r wrapper over this low level condition

public:
    condition()
    {
        int rc = m_cond.initialize();
        if (rc)
            throw threads::exception::critical_failure(THIS_LOCATION(), rc);
        // paranoid checks in debug mode
        assert(
            "Check if internal condition variable construction Ok"
          && 0 == m_cond.signal()
          );
    }
    ~condition()
    {
        // we don't care about real error code here
        int IGNORE_UNUSED rc = m_cond.destroy();
        assert("condition destroy failure" && 0 == rc);
    }
    void signal()
    {
        int IGNORE_UNUSED rc = m_cond.signal();
        assert("condition signal failure" && 0 == rc);
    }
    void broadcast()
    {
        int IGNORE_UNUSED rc = m_cond.broadcast();
        assert("condition broadcast failure" && 0 == rc);
    }

    /**
     * \attention If wait() throws, external mutex (mut) should be locked
     */
    void wait(mutex& ext_mut)
    {
        int IGNORE_UNUSED rc = 0;
        {
            details::cancelation_checker canceler(&m_cond);
            rc = m_cond.wait(ext_mut.m_mut);
        }
        assert("condition wait failure" && 0 == rc);
    }

    /**
     * \attention If timed_wait() throws, external mutex (mut) should be locked
     */
    bool timed_wait(mutex& ext_mut, const os::posix_time::time& wake_up_time)
    {
        int rc = 0;
        {
            details::cancelation_checker canceler(&m_cond);
            rc = m_cond.timed_wait(ext_mut.m_mut, wake_up_time);
        }
        if (ETIMEDOUT == rc || EINTR == rc)
            return false;
        assert("condition timed wait failure" && 0 == rc);
        return true;
    }
};

}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__CONDITION_HH__
