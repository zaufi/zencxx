/**
 * \file
 *
 * \brief Metafunction to check if given type \c T has \c default_lock member
 *
 * \date Wed Aug 21 00:37:34 MSK 2013 -- Initial design
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
#include <zencxx/type_traits/details/is_valid.hh>

// Standard includes

namespace zencxx { namespace thread { namespace details {

/**
 * \struct has_default_lock_param
 * \brief Metafunction to check if given type \c T has \c default_lock member
 *
 * \tparam T type to check
 */
template <typename T>
using default_lock_param_t = decltype(T::default_lock);

/**
 * \brief Metafunction to check if given type \c T has \c default_lock member
 *
 * \tparam T type to check
 */
template <typename T>
struct has_default_lock_param : zencxx::details::is_valid<default_lock_param_t, T>
{};

}}}                                                         // namespace details, thread, zencxx
