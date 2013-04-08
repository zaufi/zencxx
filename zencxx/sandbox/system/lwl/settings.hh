/**
 * \file
 *
 * \brief Class settings (interface)
 *
 * \date Sun Jan 24 23:24:34 MSK 2010 -- Initial design
 */

#ifndef __SYSTEM__LWL__SETTINGS_HH__
#  define __SYSTEM__LWL__SETTINGS_HH__

// Project specific includes
#  include <config.h>
#  include <system/lwl/verbosity_level.hh>
#  include <system/lwl/escape_sequences.hh>

// Standard includes
#  include <cassert>
#  include <ostream>

namespace sys { namespace lwl {

class logger_settings;

/// The only way to end user to access a logger settings singleton
extern logger_settings& settings();

/**
 * \brief Singleton with logger settings
 *
 * \note If final module would unable to setup logging device (file stream for example)
 * logging facility will be completely disabled.
 */
class logger_settings
{
    friend logger_settings& settings();
    /// Comma separated list of selected channels to enable/disable at start
    std::string m_channels_list;
    std::ostream* m_normal_stream;                          ///< Stream to be used to output normal messages
    std::ostream* m_error_stream;                           ///< Stream to be used to output error/warning messages
    verbosity_level::type m_level;                          ///< Current verbosity level
    bool m_is_color_enabled;                                ///< @c true if colors are enabled
    bool m_is_logging_enabled;                              ///< @c true if logging required

    /// Only @c settings() function may create an instance of this class
    logger_settings();
    /// Returns true if verbose level is critical, error, of warning...
    bool is_error_message(verbosity_level::type l)
    {
        return (
             verbosity_level::fatal == l
          || verbosity_level::error == l
          || verbosity_level::warning == l
          );
    }

public:
    verbosity_level::type level() const
    {
        return m_level;
    }
    void level(verbosity_level::type l)
    {
        m_level = l;
    }

    void allow_logging(bool c)
    {
        m_is_logging_enabled = c;
    }
    bool is_logging_enabled() const
    {
        return m_is_logging_enabled;
    }

    bool is_color_enabled() const
    {
        return m_is_color_enabled;
    }
    void allow_color(bool c)
    {
        m_is_color_enabled = c;
    }

    const char* select_severity_color(verbosity_level::type l)
    {
        switch (l)
        {
        case verbosity_level::fatal:   return esc::fg::light_red;
        case verbosity_level::error:   return esc::fg::light_red;
        case verbosity_level::warning: return esc::fg::brown;
        case verbosity_level::notice:  return esc::fg::light_cyan;
        case verbosity_level::info:    return esc::fg::green;
        case verbosity_level::trace:   return esc::fg::cyan;
        case verbosity_level::debug:   return esc::fg::gray;
        default: return esc::fg::normal;
        }
    }

    /// Set destination stream for usual messages (i.e. not errors)
    void out_normal_messages_to(std::ostream* s)
    {
        m_normal_stream = s;
    }
    /// Set destination stream for critical messages (i.e. errors)
    void out_error_messages_to(std::ostream* s)
    {
        m_error_stream = s;
    }
    /// Return stream object to spam according current verbose level
    std::ostream& select_stream(verbosity_level::type l)
    {
        assert("Log streams must be valid" && 0 != m_error_stream && 0 != m_normal_stream);
        return is_error_message(l) ? *m_error_stream : *m_normal_stream;
    }

    /// Remember list of selected channels and desired state for future application
    /// (by calling \c apply_channels_state())
    void setup_channels_state(const std::string& list)
    {
        m_channels_list = list;
    }

    /// \note Due crossdepencencies among \c logger_settings and \c channel
    /// function body (even simple) has gone to implementation file.
    void apply_channels_state();
};

}}                                                          // namespace lwl, sys
#endif                                                      // __SYSTEM__LWL__SETTINGS_HH__
