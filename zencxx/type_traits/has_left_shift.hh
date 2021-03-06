/**
 * \file
 *
 * \brief Metafunction to check if expression <tt>l << r</tt> is valid
 *
 * \date Wed Jun 27 02:11:39 MSK 2012 -- Initial design
 */
/*
 * ZenCxx is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libzencxx is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

#pragma once

// Project specific includes
#include <zencxx/type_traits/details/is_valid.hh>

// Standard includes
#include <utility>

namespace zencxx {namespace details {

template <typename L, typename R>
using left_shift_t = decltype(std::declval<L>() << std::declval<R>());

}                                                           // namespace details

/**
 * \struct has_left_shift
 * \brief Metafunction to check if expression `l << r` is valid
 *
 * ... where \c l is a variable of type \c L and \c r has type of \c R.
 *
 * \tparam L left side type
 * \tparam R right side type
 *
 * Example:
 * \code
 *  static_assert(has_left_shift<int, int>::value, "int can be shifted by int");
 *  static_assert(
 *      has_left_shift<std::ostream&, const std::string&>::value
 *    , "string expected to be streamable"
 *    );
 *  static_assert(has_left_shift<int, std::string>::value, "int can't be shifted by std::string");
 * \endcode
 */
template <typename L, typename R>
struct has_left_shift : details::is_valid<details::left_shift_t, L, R>
{};

}                                                           // namespace zencxx
