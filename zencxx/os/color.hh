/**
 * \file
 *
 * \brief Class \c zencxx::os::color (interface)
 *
 * This file also include the \c zencxx/os/escape_sequences.hh header to provide
 * colorful ESC sequences usually used w/ \c zencxx::os::color class.
 *
 * \date Fri Oct 22 09:30:45 MSD 2010 -- Initial design
 * \date Sun Mar 31 15:01:35 MSK 2013 -- Add enable/disable management members
 * \date Mon Apr  7 15:32:29 MSK 2014 -- Add support for 256 and 16M colors
 *
 * \todo What about 16 color terminals? ;-)
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

#pragma once

// Project specific includes
#include <zencxx/os/details/export.hh>
#include <zencxx/os/details/color.hh>
#include <zencxx/os/escape_sequences.hh>

// Standard includes
#include <cassert>
#include <ostream>

namespace zencxx { namespace os {

/**
 * \brief Stream \e manipulator to output colors
 *
 * This manipulator will check is color enabled,
 * and suppress output of ESC sequences if it doesn't.
 */
class ZENCXXOS_EXPORT color : public details::color_enabler_base
{
    const char* const m_color;

public:
    explicit color(const char* const c)
      : m_color(c)
    {
        assert("Sanity check" && c);
    }
    /// Get stored color esc sequence
    const char* get() const
    {
        return m_color;
    }
    friend std::ostream& operator<<(std::ostream& os, const color& c)
    {
        if (color::is_enabled())
            os << c.m_color;
        return os;
    }

    /// \name Nested types to use different color spaces
    //@{
    struct fg
    {
        using rgb = details::rgb<details::foreground_tag>;
        using indexed_rgb = details::indexed_rgb<details::foreground_tag>;
        using grayscale = details::grayscale<details::foreground_tag>;
    };
    struct bg
    {
        using rgb = details::rgb<details::background_tag>;
        using indexed_rgb = details::indexed_rgb<details::background_tag>;
        using grayscale = details::grayscale<details::background_tag>;
    };
    //@}
};

}}                                                          // namespace os, zencxx
