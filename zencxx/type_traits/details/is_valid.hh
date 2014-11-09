/**
 * \file
 *
 * \brief Class \c zencxx::details::is_valid (interface)
 *
 * \date Sun Nov  2 00:34:45 MSK 2014 -- Initial design
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
#include <zencxx/type_traits/details/void_t.hh>

// Standard includes
#include <type_traits>

namespace zencxx { namespace details {

/**
 * \brief Helper class for type_traits library
 *
 * \sa "Modern Template Metaprogramming - A Compendium" presentation from CppCon2014
 *
 * \note In addition to an original presentation, there is a type named
 * \c expression_type defined. For generic (false) template it is equal
 * to \c std::false_type, but if expression provided by \c Op is valid,
 * it would be equal to the result type of that expression (which is
 * usually \c decltype). That is used if additional checks need to
 * be done (except the whole expression validity) for result type.
 * For example \c is_iterator use it to check if result of \c ++
 * operator the same as \c T&, after checking that \c T is incrementable.
 * That is needed to be "lazy" to avoid compile time errors.
 *
 */
template <
    template <typename...>
    class Op
  , typename T1
  , typename T2 = T1
  , typename = void
  >
struct is_valid : public std::false_type
{
    using expression_type = std::false_type;
};

/// Specialize \c is_valid for unary \c Op
template <
    template <typename>
    class Op
  , typename T
  >
struct is_valid<Op, T, T, void_t<Op<T>>> : public std::true_type
{
    using expression_type = Op<T>;
};

/// Specialize \c is_valid for binary \c Op
template <
    template <typename, typename>
    class Op
  , typename T1
  , typename T2
  >
struct is_valid<Op, T1, T2, void_t<Op<T1, T2>>> : public std::true_type
{
    using expression_type = Op<T1, T2>;
};

}}                                                          // namespace details, zencxx
