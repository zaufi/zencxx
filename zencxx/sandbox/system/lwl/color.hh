/**
 * \file
 *
 * \brief Class color (interface)
 *
 * \date Sun Jan 24 23:58:03 MSK 2010 -- Initial design
 */

#ifndef __SYSTEM__LWL__COLOR_HH__
#  define __SYSTEM__LWL__COLOR_HH__

// Project specific includes
#  include <config.h>
#  include <system/lwl/settings.hh>

// Standard includes

namespace sys { namespace lwl {

/**
 * \brief Stream manipulator to output colors
 *
 * This manipulator will check is color enabled in logger settings,
 * and suppress output of ESC sequences if not.
 */
class color
{
    const char* const m_color;

public:
    color(const char* const c) : m_color(c) {}
    friend std::ostream& operator<<(std::ostream& os, const color& c)
    {
        if (settings().is_color_enabled())
            os << c.m_color;
        return os;
    }
    const char* get() const
    {
        return m_color;
    }
};

}}                                                          // namespace lwl, sys
#endif                                                      // __SYSTEM__LWL__COLOR_HH__
