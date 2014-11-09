/**
 * \file
 *
 * \brief Type checker to test is given type \c T can be iterated w/ range based \c for
 *
 * \date Tue Jul 23 02:56:37 MSK 2013 -- Initial design
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
#include <zencxx/type_traits/details/result_of_expression.hh>
#include <zencxx/type_traits/begin_end.hh>
#include <zencxx/type_traits/is_iterator.hh>
#include <zencxx/mpl/v_and.hh>
#include <zencxx/mpl/v_or.hh>

// Standard includes
#include <boost/mpl/eval_if.hpp>
#include <type_traits>
#include <utility>

namespace zencxx { namespace details {

template <typename T>
using call_begin_member_t = decltype(std::declval<T&>().begin());

template <typename T>
using call_begin_adl_t = decltype(begin(std::declval<T&>()));

template <typename T>
using call_end_member_t = decltype(std::declval<T&>().end());

template <typename T>
using call_end_adl_t = decltype(end(std::declval<T&>()));

ZENCXX_TT_RESULT_OF_EXPR(result_of_begin_member, (typename T), std::declval<T&>().begin());
ZENCXX_TT_RESULT_OF_EXPR(result_of_begin_adl, (typename T), begin(std::declval<T&>()));
ZENCXX_TT_RESULT_OF_EXPR(result_of_end_member, (typename T), std::declval<T&>().end());
ZENCXX_TT_RESULT_OF_EXPR(result_of_end_adl, (typename T), end(std::declval<T&>()));

/// \todo Replace w/ \c boost::mpl
template <typename T, template <typename> class ResultGetter>
struct is_result_type_is_iterator
{
    typedef typename is_iterator<
        typename ResultGetter<T>::type
      >::type type;
};

template <
    typename T
  , template <typename> class Checker
  , template <typename> class ResultGetter
  >
struct ri_validate_part
  : boost::mpl::eval_if<
      Checker<T>
    , is_result_type_is_iterator<T, ResultGetter>
    , std::false_type
    >::type
{};
}                                                           // namespace details

/**
 * \brief Type checker to test is given type \c T can be iterated w/ range based \c for
 *
 * The Standard require the following constraints for \e range-based \e for expression:
 * - if \c _RangeT is an array type, \e begin-expr and \e end-expr are \c __range and
 * `__range + __bound>`, respectively, where \c __bound is the array bound.
 * If \c _RangeT is an array of unknown size or an array of incomplete type, the program is ill-formed;
 * - if \c _RangeT is a class type, the \e unqualified-ids \c begin and \c end are looked up in the
 * scope of class \c _RangeT as if by class member access lookup (3.4.5), and if either (or both)
 * finds at least one declaration, \e begin-expr and \e end-expr are \c __range.begin() and
 * \c __range.end(), respectively;
 * - otherwise, \e begin-expr and \e end-expr are \c begin(__range) and \c end(__range),
 * respectively, where \c begin and \c end are looked up with argument-dependent lookup (3.4.2).
 * For the purposes of this name lookup, namespace std is an associated namespace.
 *
 */
template <typename T>
struct is_range_iterable
  : mpl::v_or<
      mpl::v_and<
          details::ri_validate_part<T, has_begin_member, details::result_of_begin_member>
        , details::ri_validate_part<T, has_end_member, details::result_of_end_member>
        >
    , mpl::v_and<
          details::ri_validate_part<T, has_begin_adl, details::result_of_begin_adl>
        , details::ri_validate_part<T, has_end_adl, details::result_of_end_adl>
        >
    >::type
{};

/**
 * \brief Type checker to test is given type \c T can be iterated w/ range based \c for
 *
 * Specialization for array types.
 */
template <typename T, std::size_t N>
struct is_range_iterable<T[N]> : std::true_type
{};

}                                                           // namespace zencxx
