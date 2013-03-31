/**
 * \file
 *
 * \brief Class \c zencxx::exception (interface)
 *
 * \date Sat Sep 11 21:05:57 MSD 2010 -- Initial design
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

#ifndef __ZENCXX__EXCEPTION_HH__
# define __ZENCXX__EXCEPTION_HH__

// Project specific includes
# include <zencxx/debug/backtrace.hh>
# include <zencxx/debug/location.hh>
# include <zencxx/debug/type_name.hh>
# include <zencxx/details/export.hh>

// Standard includes
# include <boost/exception/diagnostic_information.hpp>
# include <boost/exception/error_info.hpp>
# include <boost/exception/exception.hpp>
# include <boost/exception/get_error_info.hpp>
# include <boost/exception/info.hpp>
# include <boost/format/format_fwd.hpp>

/**
 * Macro to throw exceptions with attached location in a convenient way.
 * This will attach location info "transparently" for user (programmer)
 * so later it can be retrieved in a catch block...
 */
#  define ZENCXX_THROW(Ex)                                        \
    throw Ex << zencxx::exception::location_info(ZEN_THIS_LOCATION()) \
      << zencxx::exception::original_type_info(typeid((Ex)).name())

namespace zencxx {

/**
 * \brief Base class for all exceptions
 *
 * <a herf="http://www.boost.org/doc/libs/release/libs/exception/doc/boost-exception.html">
 * boost::exception</a> would give this class ability to attach an arbitrary data to
 * exception instances. Particularly all exceptions would have a backtrace and original
 * exception \c typeid attached. Also there is couple of \e predefined types that can be
 * attached:
 * \li \c location_info to indicate a source code point where exception came from
 * \li \c reason_info a human readable error description. This would replace necessity to
 * define a constructor with a \c std::string for every exception class we would like to
 * define.
 *
 * \note See unit tests for usage examples
 * \todo More elaborate description and code examples.
 */
class ZENCXX_EXPORT exception
  : virtual public std::exception
  , virtual public boost::exception
{
public:
    /// Type of backtrace data attached to exception
    typedef boost::error_info<struct tag_backtrace, debug::backtrace> trace_info;
    /// Type of location data attached to exception
    typedef boost::error_info<struct tag_location, debug::location> location_info;
    /// Type of reason message attached to exception
    typedef boost::error_info<struct tag_reason, std::string> reason_info_string;
    /// Type of \b static reason message attached to exception
    typedef boost::error_info<struct tag_reason, const char*> reason_info_literal;
    /// Original exception type attached as pointer to \c std::type_info::name() result
    /// \note According 5.8.2.1 lifetime of all \c type_info instances is the program end,
    /// so it is safe to refer pointers to that instances anytime/anywhere.
    typedef boost::error_info<struct tag_original_type_info, const char* const> original_type_info;
    /// Type of nested exception attched to this exception
    typedef boost::error_info<struct tag_exception, std::exception_ptr> nested_exception_info;

    /// Default constructor would attach backtrace to \c this instance
    /// Drop 2 frames by default: backtrace and this exception class ctors
    explicit exception(const unsigned drop_frames = 2)
    {
        *this << trace_info(debug::backtrace(debug::backtrace::DEFAULT_STACK_DEPTH, drop_frames));
    }

    /// \name Helper functions to attach strings (various type of) to exception
    //@{
    /// Helper function to produce \c reason_info_literal from literal string
    template <std::size_t N>
    static reason_info_literal reason(const char* (&str)[N])
    {
        return reason_info_literal(str);
    }
    /// Helper function to produce \c ReasonInfoStr from \c std::string
    static reason_info_string reason(const std::string& str)
    {
        return reason_info_string(str);
    }
    /// Helper function to produce \c reason_info_string from a movable \c std::string
    static reason_info_string reason(std::string&& str)
    {
        return reason_info_string(std::move(str));
    }
    /// Helper function to produce \c reason_info_string from \c boots::format
    static reason_info_string reason(const boost::format&);
    //@}

    static nested_exception_info wrap(std::exception_ptr e)
    {
        return nested_exception_info(e);
    }

    /**
     * \brief Get current exception pointer
     *
     * This function can be used to attach currently caught exception to another one.
     * The typical usage as following:
     * \code
     *  try
     *  {
     *  }
     *  catch (const some::exception&)
     *  {
     *      ZENCXX_THROW(other::exception()) << zencxx::exception::current_exception();
     *  }
     * \endcode
     *
     * To check if just caught exception has some nested one use \c has_nested_exception
     * member of this class. Then you may rethrow it if needed.
     *
     * \sa \c has_nested_exception
     * \sa \c rethrow_nested_exception
     */
    static nested_exception_info current_exception()
    {
        assert("This function must be called in catch() block only!" && std::uncaught_exception());
        return nested_exception_info(std::current_exception());
    }

    /**
     * \brief Attach a copy of exception \c E as a nested exception
     */
    template <typename E>
    static nested_exception_info attach_exception(const E& e)
    {
        return std::make_exception_ptr(std::forward<E>(e));
    }

    /// \name Functions to get various details attached to exception
    //@{
    /// Generic getter of attached data
    template <typename T>
    const typename T::value_type* get() const
    {
        return boost::get_error_info<T>(*this);
    }

    /// Get attached backtrace
    const debug::backtrace& trace() const;

    /// Get attached name of the real/original exception type
    std::string origin_type_info() const;

    /// Get attached reason
    virtual std::string reason() const;

    /// Get attached location in source code
    const debug::location& where() const;

    /// Check if nested exception present
    bool has_nested_exception() const;

    /// Rethrow nested exception if any
    /*[[noreturn]]*/ void rethrow_nested_exception() const;
    //@}

    /**
     * \brief Override standard virtual method
     *
     * Actually, in the real world there is no need to call this method :)
     * But it maybe helpful anyway, for example when your program terminates
     * cuz uncaught exception (it is OK for actively developed programs) u may
     * register <em>verbose termination handler</em> and be able to see
     * more info (including \c what() string) before your binary flush a core
     * file. It will help u to realize where is a problem. To register a
     * <a href="http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt02ch06s02.html">
     * verbose termination handler</a> just do the following somewhere close to \c main's
     * begin:
     * \code
     * std::set_terminate(__gnu_cxx::__verbose_terminate_handler);
     * \endcode
     *
     * But the \b normal way to handle exceptions supposed to be like this:
     * \li there is one (or maybe couple) <em>exceptions filter</em> functions (c below)
     * \li \c try / \c catch code usualy looks like this:
     * \code
     *  try {
     *    do_smth_that_may_throw();
     *  }
     *  catch (const SomeSpecialException&) {
     *    // hadnle this exception in a special way...
     *  }
     *  catch (...) {
     *    // all other types of exceptions would be handled in
     *    // a generic exceptions filter function. Latter usually can
     *    // distinct few types of well known system exceptions (meaning
     *    // used within a given project) and can show as much info
     *    // as it possible (by current verbosity level and debugging options)
     *    report_problem();
     *  }
     * \endcode
     * \li <em>exceptions filter</em> function usually look like this:
     * \code
     *  void report_problem() {
     *    try {
     *      // rethrow current exception!
     *      trhow;
     *    }
     *    catch (const zencxx::os::exception& e) {
     *      // We know that this type of exceptions contain errno code
     *      // and failed API function name... lets show this info also
     *      // in a log...
     *    }
     *    catch (const zencxx::exception& e) {
     *      // This is our generic type of exception.
     *      // Usually it contains backtrace, source code location and
     *      // reason info (human readable string). Lets log all what we can
     *      // extract from this exception type. Possible, depending on
     *      // current verbosity level, we may (or may not) print some
     *      // developer related info (like a backtrace I mean)...
     *    }
     *    catch (const std::exception& e) {
     *      // Oops! It is not one of ours exception!
     *      // It seems code review required and this exception must be
     *      // 'translated' into smth what our system can understand...
     *    }
     *    catch (...) {
     *      // Oops! This usually means some lack in a code design!!
     *    }
     *  }
     * \endcode
     */
    virtual const char* what() const throw() override
    {
        return boost::diagnostic_information_what(*this);
    }
};

}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__EXCEPTION_HH__
