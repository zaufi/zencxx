/**
 * \file
 *
 * \brief Class channel (interface)
 *
 * \date Sat Nov 21 17:51:15 MSK 2009 -- Split initial version of logger.cc/logger.hh into details
 */

#ifndef __SYSTEM__LWL__CHANNEL_HH__
#  define __SYSTEM__LWL__CHANNEL_HH__

// Project specific includes
#  include <config.h>
#  include <system/lwl/logger.hh>
#  include <system/lwl/settings.hh>

// Standard includes
#  include <map>

namespace sys { namespace lwl {

/// Functions to manage all registered channels
namespace channels {

/**
  * \brief Set given state to all channels specified in a string
  * \note Empty string means ALL channels
  * \param channels_list comma separated list of channels to set state of
  *
  * The only parameter is a string which describes desired channel state and verbosity level.
  * It has the following format:
  * \code
  * channel-name:verbosity-level:state[,...]
  * \endcode
  * where
  * \li \c cnahhel-name is a name of channel
  * \li \c verbosity-level desired verbosity level. If ommited current logger settings are used.
  * \li \c state \c 0 (disabled) or \c 1 (enabled). If ommited current logger settings are used.
  */
void apply_enable_state(const std::string& channels_list);

}                                                           // namespace channels

/**
 * \brief Channel for some subsystem
 *
 * \todo Need to have a real tree to better implement parent-child relation
 * of channels. Nowadays some algorithms are far from perfect due this lack,
 * but small number of channels makes this not so tight bottleneck.
 */
class channel
{
    friend void channels::apply_enable_state(const std::string&);

    /// Structure to hold a per-channel settings
    struct channel_info
    {
        verbosity_level::type m_level;                      ///< Per channel verbosity level
        bool m_is_enabled;                                  ///< Is channel enabled?

        /// \note Setting channels' verbosity level to \c none would take in account
        /// a global one when \c log_proxy would be constructed and configured.
        channel_info(
            verbosity_level::type l = verbosity_level::none // set default level to none
          , bool e = true                                   // allow spam by default
          )
          : m_level(l)
          , m_is_enabled(e)
        {}
    };
    /// Type to hold info about all channels in a system
    typedef std::map<std::string, channel_info> registered_channels_map;

    static registered_channels_map* s_channels_ptr;         ///< Pivate collection of registered channels
    /// Pointer to info about this channel in a private
    /// collection of registered channels
    registered_channels_map::iterator m_self;

    /// Private constructor called by \c create member only
    channel(registered_channels_map::iterator it) : m_self(it) {}

public:
    details::log_proxy operator()() const                   ///< Accure a \c log_proxy instance to make a SPAM
    {
        return details::log_proxy(
            m_self->first
          , m_self->second.m_level
          , m_self->second.m_is_enabled && settings().is_logging_enabled()
          );
    }

    static channel create(const std::string&);              ///< Create channel with given name
    /// Create channel with given name connected to parent
    static channel create(const std::string&, const channel&);
};

}}                                                          // namespace lwl, sys
#endif                                                      // __SYSTEM__LWL__CHANNEL_HH__
