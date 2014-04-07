/**
 * \file
 *
 * \brief Forward declarations for \c zencxx::debug::print::any
 *
 * \date Sun Jul 28 09:08:27 MSK 2013 -- Initial design
 */
/*
 * Copyright (C) 2010-2014 Alex Turbov and contributors, all rights reserved.
 * This is free software. It is licensed for use, modification and
 * redistribution under the terms of the GNU Lesser General Public License,
 * version 3 or later <http://gnu.org/licenses/lgpl.html>
 *
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

#ifndef __ZENCXX__DEBUG__PRINT__ANY_FWD_HH__
# define __ZENCXX__DEBUG__PRINT__ANY_FWD_HH__

// Project specific includes

// Standard includes
# include <type_traits>
# include <utility>

namespace zencxx { namespace debug { namespace print { namespace details {
template <typename>
class any_chooser;
}                                                           // namespace details

template <typename T>
typename std::enable_if<
    std::is_arithmetic<T>::value || std::is_pointer<T>::value
  , typename details::any_chooser<T>::type
  >::type any(T);

template <typename T>
typename std::enable_if<
    !(std::is_arithmetic<T>::value || std::is_pointer<T>::value)
  , typename details::any_chooser<const T&>::type
  >::type any(const T&);

}}}                                                         // namespace print, debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__PRINT__ANY_FWD_HH__
