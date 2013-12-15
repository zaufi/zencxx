/**
 * \file
 *
 * \brief Metafunction to check is given type is a some \c error_code class
 *
 * \date Tue Oct 15 03:39:59 MSK 2013 -- Initial design
 */
/*
 * Copyright (C) 2010-2013 Alex Turbov and contributors, all rights reserved.
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
#include <boost/system/error_code.hpp>
#include <system_error>
#include <type_traits>

namespace zencxx {

/**
 * \brief Metafunction to check is given type is a some \c error_code class
 *
 * Tries to handle both: boost and standard error codes.
 *
 */
template <typename T>
struct is_system_error_class : public std::integral_constant<
    bool
  , std::is_base_of<std::error_code, T>::value || std::is_base_of<boost::system::error_code, T>::value
  >
{};

}                                                           // namespace zencxx