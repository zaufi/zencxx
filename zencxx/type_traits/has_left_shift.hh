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

#ifndef __ZENCXX__TYPE_TRAITS__HAS_LEFT_SHIFT_HH__
# define __ZENCXX__TYPE_TRAITS__HAS_LEFT_SHIFT_HH__

// Project specific includes
# include <zencxx/type_traits/details/expression_validity_checker.hh>

// Standard includes

namespace zencxx {
/// Generate \c has_left_shift metafunction
ZENCXX_TT_EXPR_CHECKER(has_left_shift, (typename L, typename R), (L, R), (std::declval<L>() << std::declval<R>()));

/**
 * \struct has_left_shift
 * \brief Metafunction to check if expression <tt>l << r</tt> is valid
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
}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__TYPE_TRAITS__HAS_LEFT_SHIFT_HH__
