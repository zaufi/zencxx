/**
 * \file
 *
 * \author See AUTHORS file from $(top_srcdir)
 *
 * \brief Class Specific module (interface)
 *
 * \date Tue Feb 10 12:45:25 MSK 2004 -- Initial design
 */

#ifndef __SYSTEM__THREADS__SPECIFIC_HH__
#  define __SYSTEM__THREADS__SPECIFIC_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/details/specific.hh>

// Standard includes
#  include <cassert>

namespace sys { namespace threads {

/// Owner indicator tag
struct is_owner {};
/// Non-owner indicator tag
struct not_owner {};

/**
 * \brief Thread local storage.
 *
 * This class stores a pointer value, specific to each thread.
 * If the second argument of the template is specified as
 * \c is_owner, then a specific pointer is deleted after thread
 * exits.
 */
template <typename T, typename OwnerTag = not_owner>
class specific_ptr
{
    details::specific m_specific;
    //
    void set(T* const p)
    {
        m_specific.set(p);
    }

public:
    specific_ptr() {}
    specific_ptr(T* const p)
    {
        set(p);
    }
    T* get()
    {
        T* p = static_cast<T*>(m_specific.get());
        return p;
    }
    //
    T* operator->()
    {
        return get();
    }
    T& operator*()
    {
        return *get();
    }
    //
    T* operator=(T* const p)
    {
        set(p);
        return p;
    }
    operator T*()
    {
        return get();
    }
};

template <typename T>
class specific_ptr<T, is_owner>
{
    details::specific m_specific;
    //
    void set(T* const p)
    {
        m_specific.set(p);
    }

    static void ptr_destructor(void* p)
    {
        if (p) { delete static_cast<T*>(p); }
    }

public:
    specific_ptr() : m_specific(&specific_ptr::ptr_destructor) {}
    specific_ptr(T* const p) : m_specific(&specific_ptr::ptr_destructor)
    {
        set(p);
    }
    virtual ~specific_ptr()
    {
        specific_ptr<T, is_owner>::ptr_destructor(get());
    }

    T* get()
    {
        T* p = static_cast<T*>(m_specific.get());
        return p;
    }
    //
    T* operator->()
    {
        return get();
    }
    T& operator*()
    {
        return *get();
    }
    //
    T* operator=(T* const p)
    {
        set(p);
        return p;
    }
    operator T*()
    {
        return get();
    }
};

/**
 * \brief Thread-specific value.
 *
 * Object is created at the first assignment and destroyed when thread exits.
 */
template <typename T>
class specific_val
{
    mutable details::specific m_specific;

    static void ptr_destructor(void* p)
    {
        if (p) { delete static_cast<T*>(p); }
    }
    T m_default_val;

public:
    specific_val() : m_specific(&specific_val::ptr_destructor) {}
    specific_val(const T& val) : m_specific(&specific_val::ptr_destructor), m_default_val(val) {}
    ~specific_val()
    {
        specific_val<T>::ptr_destructor(m_specific.get());
    }
    T& operator=(const T& p)
    {
        if (!m_specific.get())
            m_specific.set(new T(p));
        else
            *(static_cast<T*>(m_specific.get())) = p;
        assert("TLS pointer expected to be valid" && m_specific.get());
        return *(static_cast<T*>(m_specific.get()));
    }
    T& value()
    {
        if (!m_specific.get())
            m_specific.set(new T(m_default_val));
        assert("TLS pointer expected to be valid" && m_specific.get());
        return *(static_cast<T*>(m_specific.get()));
    }
    const T& value() const
    {
        if (!m_specific.get())
            m_specific.set(new T(m_default_val));
        assert("TLS pointer expected to be valid" && m_specific.get());
        return *(static_cast<T*>(m_specific.get()));
    }
};

}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__SPECIFIC_HH__
