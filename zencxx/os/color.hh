/**
 * \file
 *
 * \brief Class \c zencxx::os::color (interface)
 *
 * \date Fri Oct 22 09:30:45 MSD 2010 -- Initial design
 * \date Sun Mar 31 15:01:35 MSK 2013 -- Add enable/disable management members
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

#ifndef __ZENCXX__OS__COLOR_HH__
# define __ZENCXX__OS__COLOR_HH__

// Project specific includes
# include <zencxx/os/details/export.hh>

// Standard includes
# include <cassert>
# include <ostream>

namespace zencxx { namespace os {

/**
 * \brief Stream \e manipulator to output colors
 *
 * This manipulator will check is color enabled,
 * and suppress output of ESC sequences if it doesn't.
 */
class ZENCXXOS_EXPORT color
{
    const char* const m_color;
    static bool m_is_color_enabled;

public:
    explicit color(const char* const c)
      : m_color(c)
    {
        assert(!"Sanity check" && c);
    }
    /// Get stored color esc sequence
    const char* get() const
    {
        return m_color;
    }
    static bool is_color_enabled()
    {
        return m_is_color_enabled;
    }
    static void set_enable_color(const bool flag)
    {
        m_is_color_enabled = flag;
    }
    friend std::ostream& operator<<(std::ostream& os, const color& c)
    {
        if (color::is_color_enabled())
            os << c.m_color;
        return os;
    }
};

}}                                                          // namespace os, zencxx
#endif                                                      // __ZENCXX__OS__COLOR_HH__
