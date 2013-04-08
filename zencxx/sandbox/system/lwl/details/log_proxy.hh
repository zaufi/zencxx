/**
 * \file
 *
 * \brief Class log_proxy (interface)
 *
 * \date Fri Nov 20 20:21:39 MSK 2009 -- Split initial version of logger.cc/logger.hh into details
 */

#ifndef __SYSTEM__LWL__DETAILS__LOG_PROXY_HH__
#  define __SYSTEM__LWL__DETAILS__LOG_PROXY_HH__

// Project specific includes
#  include <config.h>
#  include <system/gnu_extensions.hh>
#  include <system/lwl/color.hh>

// Standard includes
#  include <algorithm>
#  include <cassert>
#  include <memory>
#  include <sstream>
#  include <vector>

namespace sys { namespace lwl {

class channel;                                              // forward declaration (required for friendship)

namespace details {

#  if GCC_VERSION >= 30101
typedef std::ios_base& (*ostream_manip_type)(std::ios_base&);
#  else
typedef std::ios& (*ostream_manip_type)(std::ios&);
#  endif

/// Object used to make a log message
/// \todo Need to disable assign and take address of instance.
/// \todo Think about using a TLS \c stringstream instead of makeing it every time.
class log_proxy
{
    friend class ::sys::lwl::channel;
    friend log_proxy log();                                 ///< Only \c log() function may create \c log_proxy instances

    /// \todo Probably it is not the best way, but fastest in development time.
    /// Think about using \c boost::array with limited size instead!
    typedef std::vector<const char*> colors_stack_type;

    mutable std::auto_ptr<std::stringstream> m_os;          ///< Pointer to string stream used to collect data
    colors_stack_type m_current_color;                      ///< Stack of applied colors
    std::string m_prefix;                                   ///< Prefix to be printed before a message
    verbosity_level::type m_level;                          ///< Verbosity level of current message
    bool m_is_enabled;                                      ///< True if user wants to see output

    /// Default constructor used to make a spam
    log_proxy()
      : m_os(new std::stringstream())
      , m_level(verbosity_level::none)
      , m_is_enabled(settings().is_logging_enabled())
    {
    }
    /// This constructor used privately from \c channel::operator() method to
    /// make an preconfigured instance of \c log_proxy
    log_proxy(const std::string& pfx, verbosity_level::type level, bool is_enabled)
      : m_os(new std::stringstream())
      , m_prefix(pfx)
      , m_level(level)
      , m_is_enabled(is_enabled)
    {
    }

    log_proxy& configure(verbosity_level::type);

    /// \note \c log_proxy is not assignable!
    log_proxy& operator=(const log_proxy&);

public:
    /// "Move constructor" transfers spam ability (move semantic)
    log_proxy(const log_proxy& old)
      : m_os(old.m_os)
      , m_level(old.m_level)
      , m_is_enabled(old.m_is_enabled)
    {
        /// \todo Its DAMN DIRTY HACK! But we have no C++0x yet :(
        /// Currently I c no equal (by efficiency) solution...
        std::swap(m_current_color, const_cast<log_proxy&>(old).m_current_color);
        std::swap(m_prefix, const_cast<log_proxy&>(old).m_prefix);
    }
    /// Flush collected data on destroy
    ~log_proxy();
    /// Make logger looks like stream for end user
    template <typename T>
    log_proxy& operator<<(const T& p)
    {
        if (m_is_enabled && m_os.get())
            (*m_os) << p;
        return *this;
    }
    /// Overload to track current colors applied
    log_proxy& operator<<(color c);
    /// Allow manipulators work as expected
    log_proxy& operator<<(std::ostream& (*manip)(std::ostream&))
    {
        if (m_is_enabled)
            (*m_os) << manip;
        return *this;
    }
    log_proxy& operator<<(ostream_manip_type manip)
    {
        if (m_is_enabled)
            (*m_os) << manip;
        return *this;
    }
    /// Overload to prevent template code bloat-up (when printing string literals)
    log_proxy& operator<<(const char* const p)
    {
        assert("String expected to be valid" && p);
        if (m_is_enabled)
            (*m_os) << p;
        return *this;
    }
    log_proxy& operator[](const std::string& pfx)
    {
        m_prefix = pfx;
        return *this;
    }

    //@{
    /// Verbosity level setter
    log_proxy& fatal()   { return configure(verbosity_level::fatal);   }
    log_proxy& error()   { return configure(verbosity_level::error);   }
    log_proxy& warning() { return configure(verbosity_level::warning); }
    log_proxy& notice()  { return configure(verbosity_level::notice);  }
    log_proxy& info()    { return configure(verbosity_level::info);    }
    log_proxy& trace()   { return configure(verbosity_level::trace);   }
    log_proxy& debug()   { return configure(verbosity_level::debug);   }
    //@}
};

/// Obtain a logger proxy object
inline details::log_proxy log()
{
    return log_proxy();
}

verbosity_level::type level2id(const std::string&);         ///< Get verbosity level by string

void set_thread_id(const std::string&);

}}}                                                         // namespace details, lwl, sys
#endif                                                      // __SYSTEM__LWL__DETAILS__LOG_PROXY_HH__
