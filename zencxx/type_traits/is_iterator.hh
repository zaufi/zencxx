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
#include <zencxx/type_traits/details/expression_validity_checker.hh>
#include <zencxx/type_traits/is_dereferenceable.hh>
#include <zencxx/type_traits/is_incrementable.hh>
#include <zencxx/type_traits/is_swappable.hh>
#include <zencxx/mpl/v_and.hh>

// Standard includes
#include <boost/mpl/apply.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/eval_if.hpp>
#include <iterator>
#include <type_traits>
#include <utility>

namespace zencxx {

ZENCXX_TT_EXPR_CHECKER(
    has_value_type
  , (typename T)
  , (T)
  , (std::declval<typename std::iterator_traits<T>::value_type>())
  );

ZENCXX_TT_EXPR_CHECKER(
    has_difference_type
  , (typename T)
  , (T)
  , (std::declval<typename std::iterator_traits<T>::difference_type>())
  );

ZENCXX_TT_EXPR_CHECKER(
    has_iterator_category_type
  , (typename T)
  , (T)
  , (std::declval<typename std::iterator_traits<T>::iterator_category>())
  );

ZENCXX_TT_EXPR_CHECKER(
    has_pointer_type
  , (typename T)
  , (T)
  , (std::declval<typename std::iterator_traits<T>::pointer>())
  );

ZENCXX_TT_EXPR_CHECKER(
    has_reference_type
  , (typename T)
  , (T)
  , (std::declval<typename std::iterator_traits<T>::reference>())
  );

/**
 * \struct is_iterator
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
  >::type
{};


}                                                           // namespace zencxx
