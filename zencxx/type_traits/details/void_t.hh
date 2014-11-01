/**
 * \file
 *
 * \brief Class \c zencxx::details::void_t (interface)
 *
 * \date Sun Nov  2 00:26:51 MSK 2014 -- Initial design
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

namespace zencxx { namespace details {

/**
 * \brief Helper class for type_traits library
 *
 * \sa "Modern Template Metaprogramming - A Compendium" presentation from CppCon2014
 *
 */
template <typename... T>
struct voider
{
    typedef void type;
};

template <typename... T>
using void_t = typename voider<T...>::type;

}}                                                          // namespace details, zencxx
