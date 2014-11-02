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
#include <zencxx/type_traits/details/is_valid.hh>

// Standard includes
#include <utility>

namespace zencxx { namespace details {

template <typename T>
using swap_with_adl_t = decltype(swap(std::declval<T&>(), std::declval<T&>()));

}                                                           // namespace details

/**
 * \brief Metafunction to check is given type \c T conform to \e Swappable concept
 *
 * According 17.6.3.2: An object \c t is swappable with an object \c u if and only if:
 * - the expressions `swap(t, u)` and `swap(u, t)` are valid when evaluated
 * in the context described below, and ...
 *
 * The context in which `swap(t, u)` and `swap(u, t)` are evaluated shall
 * ensure that a binary non-member function named \c swap is selected via overload
 * resolution (13.3) on a candidate set that includes:
 * - the two swap function templates defined in `<utility>` (20.2) and
 * - the lookup set produced by argument-dependent lookup (3.4.2).
 *
 * \attention It is not possible to check if `std::swap(t, u)` valid for a particular
 * type using \c decltype, as it done for ADL based call. The reason of that is a
 * \c swap declaration! It is a \b template function, so always be valid! Due
 * \c decltype will not check the body, it would check only a function signature:
 * i.e. function is found via resolution, so call is possible. It wouldn't instantiate
 * the function body, so no further checks (if type is move constructible/assignable)
 * would take place.
 */
template <typename T>
struct is_swappable : std::integral_constant<
    bool
  , std::is_move_constructible<T>::value && std::is_move_assignable<T>::value
      || details::is_valid<details::swap_with_adl_t, T>::value
  >
{};

}                                                           // namespace zencxx
