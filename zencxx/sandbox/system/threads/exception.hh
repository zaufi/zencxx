/**
 * \file
 *
 * \brief Class exception (interface)
 *
 * \date Mon Nov  9 03:26:24 MSK 2009 -- Initial design
 */

#ifndef __SYSTEM__THREADS__EXCEPTION_HH__
#  define __SYSTEM__THREADS__EXCEPTION_HH__

// Project specific includes
#  include <config.h>
#  ifndef ZENCXX_USE_STD_EXCEPTIONS_4_TESTS
#    include <system/exception.hh>
#  endif                                                    // ZENCXX_USE_STD_EXCEPTIONS_4_TESTS

// Standard includes
#  include <cerrno>

#  ifdef ZENCXX_USE_STD_EXCEPTIONS_4_TESTS
#    define THIS_LOCATION() __FILE__, __LINE__
#  endif                                                    // ZENCXX_USE_STD_EXCEPTIONS_4_TESTS

namespace sys { namespace threads {

#  ifndef ZENCXX_USE_STD_EXCEPTIONS_4_TESTS
/**
 * \brief Threads library specific exceptions group
 */
struct exception : virtual public ::sys::exception
{
    struct critical_failure;
    struct thread_canceled;
    struct bad_alloc;
};

struct exception::critical_failure
  : public os_exception
  , private ::sys::critical_failure
{
    critical_failure(const debug::location& loc, int code) : os_exception(loc, code) {}
};

struct exception::bad_alloc
  : public runtime_error
  , private ::sys::critical_failure
{
    bad_alloc(const debug::location& loc) : runtime_error(loc) {}
};

struct threads::exception::thread_canceled : public threads::exception, private internal {};

#  else

/**
 * \brief Replacement for full featured exceptions for tests purposes
 */
struct exception
{
    struct critical_failure;
    struct thread_canceled;
    struct bad_alloc;
};

struct exception::critical_failure
{
    critical_failure(const char* const /* file */, int /* line */, int /*code*/)
    {}
};

struct exception::bad_alloc
{
    bad_alloc(const char* const /* file */, int /* line */) {}
};

struct threads::exception::thread_canceled : public threads::exception {};

#  endif
}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__EXCEPTION_HH__
