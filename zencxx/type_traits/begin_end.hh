/**
 * \file
 *
 * \brief Metafunctions to check for \c begin() and \c end() member or free functions availability
 *
 * \date Wed Jul 24 12:25:28 MSK 2013 -- Initial design
 *
 * \todo Check that begin/end returns an iterator type
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
#include <utility>                                          // for std::declval

namespace zencxx { namespace details {

template <typename T>
using call_begin_member_t = decltype(std::declval<T&>().begin());

template <typename T>
using call_begin_adl_t = decltype(begin(std::declval<T&>()));

template <typename T>
using call_end_member_t = decltype(std::declval<T&>().end());

template <typename T>
using call_end_adl_t = decltype(end(std::declval<T&>()));

}                                                           // namespace details

/**
 * \brief Metafunction to check if given type \c T has member-function \c begin()
 *
 * \tparam T type to check
 */
template <typename T>
struct has_begin_member : details::is_valid<details::call_begin_member_t, T>
{};

/**
 * \brief Metafunction to check if given type \c T has member-function \c end()
 *
 * \tparam T type to check
 */
template <typename T>
struct has_end_member : details::is_valid<details::call_end_member_t, T>
{};

/**
 * \brief Metafunction to check if there is a free function \c begin(T&) exists
 *
 * \tparam T type to check
 */
template <typename T>
struct has_begin_adl : details::is_valid<details::call_begin_adl_t, T>
{};

/**
 * \brief Metafunction to check if there is a free function \c end(T&) exists
 *
 * \tparam T type to check
 */
template <typename T>
struct has_end_adl : details::is_valid<details::call_end_adl_t, T>
{};

}                                                           // namespace zencxx
