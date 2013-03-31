/**
 * \file
 *
 * \brief Class \c zencxx::os::color (implementation)
 *
 * \date Sun Mar 31 15:01:35 MSK 2013 -- Initial design
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

// Project specific includes
#include <zencxx/os/color.hh>
#include <zencxx/os/term/utils.hh>

// Standard includes

namespace zencxx { namespace os {
/// Try to autodetect color support
bool color::m_is_color_enabled = term::is_color_term();
}}                                                          // namespace os, zencxx
