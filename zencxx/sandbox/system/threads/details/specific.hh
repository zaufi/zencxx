/**
 * \file
 *
 * \brief Class specific (interface)
 *
 * \date Mon Nov  9 03:07:34 MSK 2009 -- Initial design
 */

#ifndef __SYSTEM__THREADS__DETAILS__SPECIFIC_HH__
#  define __SYSTEM__THREADS__DETAILS__SPECIFIC_HH__

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
 * \brief Implementation of Thread Local Storage (TLS) wrapper using POSIX-threads functions.
 * As in case of thread, this system-dependent implementation provides only
 * function call wrappers. High-level TLS primitives like \c specific_ptr and
 * \c specific_val are in specific.hh.
 */
class specific : public boost::noncopyable
{
    pthread_key_t m_key;

public:
    /// \throw exception::critical_failure the system lacked the necessary resources (including memory)
    /// \todo Is \c bad_alloc more suitable when key couldn't be set due run out of memory?
    specific()
    {
        int rc = pthread_key_create(&m_key, 0);
        if (rc)
            throw threads::exception::critical_failure(THIS_LOCATION(), rc);
    }
    /// \throw exception::critical_failure the system lacked the necessary resources (including memory)
    /// \todo Is \c bad_alloc more suitable when key couldn't be set due run out of memory?
    explicit specific(void (*f)(void*))
    {
        int rc = pthread_key_create(&m_key, f);
        if (rc)
            throw threads::exception::critical_failure(THIS_LOCATION(), rc);
    }
    ~specific()
    {
        int IGNORE_UNUSED result = pthread_key_delete(m_key);
        assert("sys::threads::specific destroy failure" && 0 == result);
    }
    void* get()
    {
        return pthread_getspecific(m_key);
    }
    /// \throw exception::critical_failure if internal key is invalid or not
    /// enough memory to create a key
    /// \todo Is \c bad_alloc more suitable when key couldn't be set due run out of memory?
    void set(void* const p)
    {
        /// \warning LynxOS has constless prototype
        int rc = pthread_setspecific(m_key, const_cast<void*>(p));
        if (rc)
            throw threads::exception::critical_failure(THIS_LOCATION(), rc);
    }
};

}}}                                                         // namespace details, threads, sys
#endif                                                      // __SYSTEM__THREADS__DETAILS__SPECIFIC_HH__
