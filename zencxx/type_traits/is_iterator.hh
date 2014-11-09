/**
 * \file
 *
 * \brief Metafunction to check is given type \c T conforms to \e Iterator concept
 *
 * \date Tue Jul 23 06:01:19 MSK 2013 -- Initial design
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
#include <zencxx/type_traits/is_dereferenceable.hh>
#include <zencxx/type_traits/is_incrementable.hh>
#include <zencxx/type_traits/is_swappable.hh>
#include <zencxx/mpl/v_and.hh>

// Standard includes
#include <boost/mpl/eval_if.hpp>
#include <iterator>
#include <type_traits>

namespace zencxx { namespace details {

/// \name Helper to get iterator nested types via traits
//@{
template <typename T>
using iterator_value_type_t =
    decltype(std::declval<typename std::iterator_traits<T>::value_type>());

template <typename T>
using iterator_difference_type_t =
    decltype(std::declval<typename std::iterator_traits<T>::difference_type>());

template <typename T>
using iterator_iterator_category_type_t =
    decltype(std::declval<typename std::iterator_traits<T>::iterator_category>());

template <typename T>
using iterator_pointer_type_t =
    decltype(std::declval<typename std::iterator_traits<T>::pointer>());

template <typename T>
using iterator_reference_type_t =
    decltype(std::declval<typename std::iterator_traits<T>::reference>());
//@}

/**
 * \internal Check if \c std::iterator_traits<T> has nested type \c reference
 */
template <typename T>
struct has_reference_type : is_valid<iterator_reference_type_t, T>
{};

/**
 * \internal Metafunction to make sure \c std::iterator_traits<T>::reference is a valid
 * and same as dereference type of lvalue of type \c T.
 */
template <typename T>
struct check_dereference_type : boost::mpl::eval_if<
    has_reference_type<T>
  , std::is_same<iterator_reference_type_t<T>, dereference_t<T>>
  , std::false_type
  >
{};

}                                                           // namespace details

/**
 * \brief Metafunction to check is given type \c T conforms to \e Iterator concept
 *
 * The Standard require that \e Iterator concept must have the following
 * sementic:
 *  - \c T satisfies the \e CopyConstructible, \e CopyAssignable, and \e Destructible
 * requirements (17.6.3.1) and lvalues of type \c T are swappable (17.6.3.2), and
 *  - the expressions in Table 106 are valid and have the indicated semantics.
 *
 * The valid expressions are:
 * - \c *r w/ return type \e reference
 * - \c ++r w/ return type \c T&
 *
 * \todo Some types that can be considered as \e Iterator (like raw pointers) do not
 * respect this concept in all ways. For example, return type of \c ++r can be a pointer.
 * So, stricter checking is not recommended for generic use.
 */
template <typename T>
struct is_iterator : mpl::v_and<
    std::is_copy_constructible<T>
  , std::is_copy_assignable<T>
  , std::is_destructible<T>
  , is_swappable<T>
  , is_dereferenceable<T>
  , is_incrementable<T>
  , details::check_dereference_type<T>
  >::type
{};

}                                                           // namespace zencxx
