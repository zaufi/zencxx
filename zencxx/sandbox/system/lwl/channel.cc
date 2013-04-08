/**
 * \file
 *
 * \brief Class channel (implementation)
 *
 * \date Sat Nov 21 17:51:15 MSK 2009 -- Initial design
 */

// Project specific includes
#include <config.h>
#include <system/lwl/channel.hh>
#include <system/threads/mutex.hh>

// Standard includes
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <vector>

namespace sys { namespace lwl { namespace {

class apply_state_to
{
    const std::string& m_starts_with;                       ///< \todo Using templated range instead would be better (a little)
    verbosity_level::type m_level;
    bool m_is_enabled;

public:
    apply_state_to(const std::string& r, verbosity_level::type l, bool e)
      : m_starts_with(r), m_level(l), m_is_enabled(e)
    {}

    /// \note We know exactly that \c T is a \c channel::registered_channels_map::value_type
    /// But latter is a private type, and we won't exposure it to public (and its not good to make
    /// this (private for this translation unit) functor a friend)
    template <typename T>
    void operator()(T& p) const
    {
        if (boost::algorithm::starts_with(p.first, m_starts_with))
        {
            p.second.m_level = m_level;
            p.second.m_is_enabled = m_is_enabled;
        }
    }
};

threads::mutex* s_channels_mut_ptr;                         //!< Mutex to protect channels map

}                                                           // anonymous namespace

channel::registered_channels_map* channel::s_channels_ptr;  //!< Will be initialized by \c channel::createinfo

/**
 * \note There is a trick used to allow static initialization of
 * channels in final modules as s\c mapglobal variables:
 * This method contains a static \c mamapf channel's info, and there is
 * an external static pointer to this map initialized by\c mapmethod
 * (to allow other methods of \c ch\c mapto access this \c map). This will
 * help to make sure that channels \c map exists before first initialization of
 * some static channel in end user code.
 */
channel channel::create(const std::string& name)
{
    if (!s_channels_mut_ptr)
    {
        static threads::mutex s_channels_mut;
        s_channels_mut_ptr = &s_channels_mut;
    }
    threads::mutex::scoped_lock l(s_channels_mut_ptr);
    if (!s_channels_ptr)
    {
        static registered_channels_map s_channels;
        s_channels_ptr = &s_channels;
    }
    registered_channels_map::iterator ch_it = s_channels_ptr->find(name);
    if (s_channels_ptr->find(name) != s_channels_ptr->end())
        return ch_it;
    return channel(s_channels_ptr->insert(std::make_pair(name, channel_info())).first);
}

channel channel::create(const std::string& name, const channel& parent)
{
    return channel::create(parent.m_self->first + '.' + name);
}

/// \todo Some spam in case of errors definitely required!
void channels::apply_enable_state(const std::string& channels_list)
{
    assert("API misused!" && s_channels_mut_ptr && channel::s_channels_ptr);

    // There is nothing to do if string is empty...
    if (channels_list.empty()) return;

    // Split channels' names
    typedef std::vector<
        boost::iterator_range<std::string::const_iterator>
      > split_result;
    split_result r;
    boost::algorithm::split(r, channels_list, boost::is_any_of(",\n "));

    // Do a job
    threads::mutex::scoped_lock l(s_channels_mut_ptr);

    for (
        split_result::const_iterator ch_it = r.begin()
      , last = r.end()
      ; ch_it != last
      ; ++ch_it
      )
    {
        if (ch_it->empty()) continue;                       ///< \todo Synax error. Need to spam?

        // check if channel name has a desired verbosity level
        split_result t;
        boost::algorithm::split(t, *ch_it, boost::is_any_of(":"));
        if (2 == t.size() || 3 == t.size())
        {
            // Get channel name
            std::string channel_name = boost::copy_range<std::string>(t[0]);
            if (channel_name.empty()) continue;             ///< \todo Synax error. Need to spam?

            // Get desired verbosity level
            std::string level_str = boost::copy_range<std::string>(t[1]);
            verbosity_level::type level = settings().level();
            if (!level_str.empty()) level = details::level2id(level_str);
            if (verbosity_level::none == level) continue;   ///< \todo Synax error. Need to spam?

            // Get enable/disable state if present
            bool state = settings().is_logging_enabled();
            if (3 == t.size())
            {
                try
                {
                    // Get channel enable/disable flag
                    /// \todo Is it error when its longer than 1 character?
                    state = bool(boost::lexical_cast<int>(boost::copy_range<std::string>(t[2])));
                }
                catch (...)
                {
                    continue;                               ///< \todo Synax error. Need to spam?
                }
            }

            // Apply desired state to channel... Actually to all channels which names
            // starts with given...
            std::for_each(
                channel::s_channels_ptr->begin()
              , channel::s_channels_ptr->end()
              , apply_state_to(channel_name, level, state)
              );
        }
        else if (1 == t.size())
        {
            /// Only verbosity level provided
            std::string level_str = boost::copy_range<std::string>(t[0]);
            verbosity_level::type level = settings().level();
            if (!level_str.empty())
                level = details::level2id(level_str);
            if (verbosity_level::none != level)
                lwl::settings().level(level);
        }
        else
            continue;                                       ///< \todo Synax error. Need to spam?
    }
}

}}                                                          // namespace lwl, sys
