/**
 * \file
 *
 * \brief Metafunction to check if expression <tt>++var</tt> is valid
 *
 * \date Tue Jul 23 09:07:42 MSK 2013 -- Initial design
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
using preincrement_t = decltype(++std::declval<T&>());

}                                                           // namespace details

/**
 * \brief Check if variables of type \c T can be (prefix) incremented
 *
 * According Standard, section 5.3.2:
 *
 * The operand of prefix \c ++ is modified by adding \c 1, or set to \c true if
 * it is \c bool (this use is deprecated). The operand shall be a modifiable \e lvalue.
 * The type of the operand shall be an arithmetic type or a pointer to
 * a completely-defined object type. The result is the updated operand; it is an
 * \e lvalue, and it is a bit-field if the operand is a bit-field. If \c x is not of type
 * \c bool, the expression \c ++x is equivalent to \c x+=1.
 *
 * So, the given type \c T considered to be \e incrementable if:
 * - it is mutable \e lvalue (i.e. not a \c cont) and
 * - it is arithmetic type, or
 * - it is pointer type, or
 * - it is user defined type w/ overloaded \c operator++()
 *
 * \attention The (straightforward) implementation for this checker uses \c ++declval<T&>(),
 * i.e. not the mentioned above (complicated) rules to check. Unit tests (nowadays) are
 * work as expected and I see no reason to make smth more complicated... until some unexpected
 * failure has happened.
 */
template <typename T>
struct is_incrementable : details::is_valid<details::preincrement_t, T>
{};

}                                                           // namespace zencxx
