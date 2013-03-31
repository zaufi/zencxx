/**
 * \file
 *
 * \brief Terminal utility functions
 *
 * \date Sun Oct  2 00:12:48 MSK 2011 -- Initial design
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

#ifndef __ZENCXX__OS__TERM__UTILS_HH__
# define __ZENCXX__OS__TERM__UTILS_HH__

// Project specific includes
# include <zencxx/os/exception.hh>
# include <zencxx/os/details/export.hh>

// Standard includes
# include <utility>

namespace zencxx { namespace os { namespace term {

/// Get terminal dimensions
ZENCXXOS_EXPORT std::pair<unsigned, unsigned> get_term_size();

/// Check if terminal supports colors
ZENCXXOS_EXPORT bool is_color_term();

}}}                                                         // namespace term, os, zencxx
#endif                                                      // __ZENCXX__OS__TERM__UTILS_HH__
