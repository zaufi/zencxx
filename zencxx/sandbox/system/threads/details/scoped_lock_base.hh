/**
 * \file
 *
 * \brief Class scoped_lock_base (interface)
 *
 * \date Sun Nov  8 21:03:46 MSK 2009 -- Initial design
 */

#ifndef __SYSTEM__THREADS__DETAILS__SCOPED_LOCK_BASE_HH__
#  define __SYSTEM__THREADS__DETAILS__SCOPED_LOCK_BASE_HH__

// Project specific includes
#  include <config.h>

// Standard includes
#  include <boost/function.hpp>
#  include <boost/noncopyable.hpp>

namespace sys { namespace threads { namespace details {

/**
 * \brief Base class for all locable's internal \c scoped_locks
 *
 */
class scoped_lock_base : boost::noncopyable
{
    boost::function<void(void)> m_unlock;

protected:
    template <typename LockFunction, typename UnlockFunction>
    scoped_lock_base(
        LockFunction lf
      , UnlockFunction uf
      )
      : m_unlock(uf)
    {
        lf();
    }
    ~scoped_lock_base()
    {
        m_unlock();
    }
};

}}}                                                         // namespace details, threads, sys
#endif                                                      // __SYSTEM__THREADS__DETAILS__SCOPED_LOCK_BASE_HH__
