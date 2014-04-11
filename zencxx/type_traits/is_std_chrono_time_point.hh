/**
 * \file
 *
 * \brief Metafunction to check is given type is an instance of \c std::chrono::time_point
 *
 * \date Sun Jul 28 21:47:33 MSK 2013 -- Initial design
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

#pragma once

// Project specific includes

// Standard includes
#include <chrono>
#include <type_traits>

namespace zencxx {

template <typename T>
struct is_std_chrono_time_point : public std::false_type
{};

template <typename Clock, typename Duration>
struct is_std_chrono_time_point<std::chrono::time_point<Clock, Duration>>
  : public std::true_type
{};

}                                                           // namespace zencxx
