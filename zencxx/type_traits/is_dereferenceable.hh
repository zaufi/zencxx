/**
 * \file
 *
 * \brief Metafunction to check if expression \c *var is valid
 *
 * \date Tue Jul 23 08:58:06 MSK 2013 -- Initial design
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
using dereference_t = decltype(*std::declval<T>());

}                                                           // namespace details
/**
 * \brief Check if variables of type \c T can be dereferenced
 */
template <typename T>
struct is_dereferenceable : details::is_valid<details::dereference_t, T>
{};

}                                                           // namespace zencxx
