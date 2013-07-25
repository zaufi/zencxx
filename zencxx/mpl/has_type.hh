/**
 * \file
 *
 * \brief \c zencxx::mpl::has_type metafunction
 *
 * \date Wed Jul  4 05:07:08 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__HAS_TYPE_HH__
# define __ZENCXX__MPL__HAS_TYPE_HH__

// Project specific includes
# include <zencxx/type_traits/details/expression_validity_checker.hh>

// Standard includes
# include <utility>

namespace zencxx { namespace mpl {
/**
 * \attention Nowadays (boost <= 1.50) \c has_type metafunction placed in the
 * \c boost::mpl::aux, so we can't use it here (yep, it's undocumented
 * implementation details), so we have to define our own... and better one
 * because of new features of C++11 :))
 */
ZENCXX_TT_EXPR_CHECKER(has_type, (typename T), (T), std::declval<typename T::type>());
}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__HAS_TYPE_HH__
