/**
 * \file
 *
 * \brief Metafunction to check is given types \c T and \c U conform to \e Swappable concept
 *
 * \date Tue Jul 23 10:03:29 MSK 2013 -- Initial design
 */
/*
 * Copyright (C) 2013 Alex Turbov and contributors, all rights reserved.
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

#pragma once

// Project specific includes
#include <zencxx/type_traits/details/expression_validity_checker.hh>

// Standard includes
#include <utility>

namespace zencxx {

// Generate \c is_swappable_adl metafunction
ZENCXX_TT_EXPR_CHECKER_EX(
    is_swappable_adl
  , (typename T, typename U)
  , (T& t, U& u)
  , (T, U)
  , (std::declval<T&>(), std::declval<U&>())
  , swap(t, u)
  );

// Generate \c is_swappable_with_std_swap metafunction
ZENCXX_TT_EXPR_CHECKER_EX(
    is_swappable_with_std_swap
  , (typename T, typename U)
  , (T& t, U& u)
  , (T, U)
  , (std::declval<T&>(), std::declval<U&>())
  , std::swap(t, u)
  );

namespace details {
template <typename T, typename U>
using is_swappable_impl = std::integral_constant<
    bool
  , is_swappable_with_std_swap<T, U>::value || is_swappable_adl<T, U>::value
  >;
}                                                           // namespace details

/**
 * \brief Metafunction to check is given types \c T and \c U conform to \e Swappable concept
 *
 * According 17.6.3.2: An object \c t is swappable with an object \c u if and only if:
 * - the expressions <tt>swap(t, u)</tt> and <tt>swap(u, t)</tt> are valid when evaluated
 * in the context described below, and ...
 *
 * The context in which <tt>swap(t, u)</tt> and <tt>swap(u, t)</tt> are evaluated shall
 * ensure that a binary non-member function named \c swap is selected via overload
 * resolution (13.3) on a candidate set that includes:
 * - the two swap function templates defined in <utility> (20.2) and
 * - the lookup set produced by argument-dependent lookup (3.4.2).
 */
template <typename... T>
struct is_swappable;

template <typename T, typename U>
struct is_swappable<T, U> : details::is_swappable_impl<T, U> {};

/**
 * \brief Metafunction to check if instances of \c T can be swapped with each other
 * according \e Swappable concept as defined in The Standard.
 */
template <typename T>
struct is_swappable<T> : details::is_swappable_impl<T, T> {};

}                                                           // namespace zencxx
