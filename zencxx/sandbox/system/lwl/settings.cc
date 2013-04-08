/**
 * \file
 *
 * \brief Class settings (implementation)
 *
 * \date Sun Jan 24 23:24:34 MSK 2010 -- Initial design
 */

// Project specific includes
#include <config.h>
#include <system/lwl/settings.hh>
#include <system/lwl/channel.hh>
#include <system/threads/mutex.hh>

// Standard includes
#include <iostream>
#include <cstdlib>
#include <cstring>

namespace sys { namespace lwl { namespace {

threads::mutex* s_settings_mut_ptr;                         ///< Pointer to private mutex to protect settings access
logger_settings* s_settings_ptr;                            ///< Pointer to private settings instance

}                                                           // anonymous namespace

logger_settings::logger_settings()
  : m_normal_stream(&std::cout)
  , m_error_stream(&std::cerr)
  , m_level(level::info)
    /// \note WTF!? The same expression being assigned to static constant evaluated to zero!!
    /// It seems some initialize order issues here!
    /// \todo If we r using gcc only, maybe we can use gcc specific attributes to resolve this issue? :))
  , m_is_color_enabled(getenv("TERM") && strcmp(getenv("TERM"), "dumb"))
  , m_is_logging_enabled(true)
{
}

logger_settings& settings()
{
    if (!s_settings_mut_ptr)
    {
        static threads::mutex s_settings_mut;
        s_settings_mut_ptr = &s_settings_mut;
    }
    if (!s_settings_ptr)
    {
        threads::mutex::scoped_lock g(s_settings_mut_ptr);
        if (!s_settings_ptr)
        {
            static logger_settings s_settings;
            s_settings_ptr = &s_settings;
        }
    }
    return *s_settings_ptr;
}

void logger_settings::apply_channels_state()
{
    channels::apply_enable_state(m_channels_list);
}

}}                                                          // namespace lwl, sys
