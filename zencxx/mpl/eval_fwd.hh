/**
 * \file
 *
 * \brief Forward declarations for \c zencxx::mpl::eval and \c zencxx::mpl::eval_impl
 *
 * \date Wed Jul  4 06:15:03 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__EVAL_FWD_HH__
# define __ZENCXX__MPL__EVAL_FWD_HH__

// Project specific includes

// Standard includes

namespace zencxx { namespace mpl {

template <typename Expr>
struct eval;

template <typename Tag>
struct eval_impl;


}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__EVAL_FWD_HH__
