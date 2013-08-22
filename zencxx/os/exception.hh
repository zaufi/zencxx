/**
 * \file
 *
 * \brief Class \c zencxx::os::exception (interface)
 *
 * \date Mon Sep 13 10:18:46 MSD 2010 -- Initial design
 */
/*
 * ZenCxx is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ZenCxx is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

#ifndef __ZENCXX__OS__EXCEPTION_HH__
# define __ZENCXX__OS__EXCEPTION_HH__

// Project specific includes
# include <zencxx/exception.hh>
# include <zencxx/os/details/export.hh>

// Standard includes
# include <boost/exception/errinfo_api_function.hpp>
# include <boost/system/error_code.hpp>
# include <cerrno>

namespace zencxx { namespace os {
/**
 * \brief A class to represent system and/or OS errors
 *
 * In addition to base functions this class provide an error code
 * and API function name (which caused a failure) attached to all instances.
 *
 * You have to provide an API function name caused a failure to \b constructor
 * (yes it just to make that param mandatory). Also it would attach an OS error
 * code taken from well known \c errno variable or one may provide them explicitly
 * to constructor. This class also provide a set of functions that intented to
 * simplify an access to attached data.
 *
 * \c os::exception uses a new way to deal with error codes in modern C++, so
 * if u unfamiliar with it, please read the following article and boost manual:
 *
 * \sa <a href="http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-1.html">
 *     System error support in C++0x
 *     </a>
 * \sa <a href="http://www.boost.org/doc/libs/release/libs/system/doc/index.html">
 *      Boost System library: Operating system support, including the diagnostics
 *      support that will be part of the C++0x standard library.
 *     </a>
 *
 * Example:
 * \code
 *  void do_system_work()
 *  {
 *    int result = some_posix_call(...);
 *    if (result == -1)
 *      ZENCXX_THROW(os::exception("some_posix_call"));
 *  }
 *  //
 *  try
 *  {
 *    do_system_work();
 *  }
 *  catch (const os::Exception& e)
 *  {
 *    // Is this smth really serious? (not EAGAIN)
 *    if (e.error_code() != boost::system::errc::resource_unavailable_try_again)
 *    {
 *      boost::system::error_code code = e.error_code();
 *      spam::log() << "Unable to do a job: " << code.message() << " (code " << code.value() << ")";
 *    }
 *  }
 * \endcode
 *
 * \todo Make it possible to work w/ \c std::error_code as well
 */
class ZENCXXOS_EXPORT exception : virtual public zencxx::exception
{
    typedef boost::error_info<
        struct tag_error_code
      , boost::system::error_code
      > error_code_info;

public:
    /**
     * \brief Attach the current error code to \c this instance
     *
     * \param[in] api_function_name name of a failed function. Must be non null
     *  string literal.
     *
     * \throw \c std::bad_alloc
     *
     * \invariant API function name can't be NULL
     */
    explicit exception(const char* const api_function_name)
      : exception(errno, api_function_name)
    {}

    /**
     * \brief Make an exception instance with given code
     *
     * \param[in] code explicit error code (one of \c errno value)
     * \param[in] api_function_name name of a failed function. Must be non null
     *  string literal.
     *
     * \throw \c std::bad_alloc
     *
     * \invariant API function name can't be NULL
     */
    exception(const int code, const char* const api_function_name)
      : exception({code, boost::system::system_category()}, api_function_name)
    {}

    /**
     * \brief Make an exception instance with given error code
     *
     * \param[in] code explicit error code
     * \param[in] api_function_name name of a failed function. Must be non null
     *  string literal.
     *
     * \throw \c std::bad_alloc
     *
     * \invariant API function name can't be NULL
     */
    exception(const boost::system::error_code& code, const char* const api_function_name)
    {
        assert("API function name must be provided" && api_function_name);
        *this << error_code_info(code) << boost::errinfo_api_function(api_function_name);
    }

    /**
     * \brief Get attached function name
     * \invariant API function name expected to be \b always attached!
     */
    const char* failed_function() const
    {
        const char* const* result = get<boost::errinfo_api_function>();
        assert("API function name expected to be here" && result && *result);
        return *result;
    }

    /**
     * \brief Get attached error code
     * \invariant Error code expected to be \b always attached!
     */
    const boost::system::error_code& error_code() const
    {
        // Check if there is boost::system::error_code attached...
        const boost::system::error_code* const code = get<error_code_info>();
        assert("Error code expected to be here" && code);
        return *code;
    }

    // Inject some helpful members from the base class
    using zencxx::exception::reason;

    /// Provide more human readable info about system error
    virtual std::string reason() const override;
};

}}                                                          // namespace os, zencxx
#endif                                                      // __ZENCXX__OS__EXCEPTION_HH__
