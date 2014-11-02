/**
 * \file
 *
 * \brief Metafunctions to check for \c to_string() member or free functions availability
 *
 * \date Thu Jul 25 09:31:27 MSK 2013 -- Initial design
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
#include <string>
#include <utility>

namespace zencxx { namespace details {

template <typename T>
using call_to_string_member_t = decltype(std::declval<const T&>().to_string());

template <typename T>
using call_to_string_adl_t = decltype(to_string(std::declval<const T&>()));

}                                                           // namespace details

/**
 * \brief Metafunciton to check if given type \c T has \c to_string member function.
 */
template <typename T>
struct has_to_string_member : public details::is_valid<details::call_to_string_member_t, T>
{};

/**
 * \brief Metafunciton to check if given type \c T has \c to_string as a free function.
 */
template <typename T>
struct has_to_string_adl : public details::is_valid<details::call_to_string_adl_t, T>
{};

}                                                           // namespace zencxx
