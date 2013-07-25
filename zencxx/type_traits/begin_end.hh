/**
 * \file
 *
 * \brief Metafunctions to check for \c begin() and \c end() member or free functions availability
 *
 * \date Wed Jul 24 12:25:28 MSK 2013 -- Initial design
 *
 * \todo Check that begin/end returns an iterator type
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

#ifndef __ZENCXX__TYPE_TRAITS__BEGIN_END_HH__
# define __ZENCXX__TYPE_TRAITS__BEGIN_END_HH__

// Project specific includes
# include <zencxx/type_traits/details/expression_validity_checker.hh>

// Standard includes

namespace zencxx {

/// Generate \c has_begin_member metafunction
ZENCXX_TT_EXPR_CHECKER(
    has_begin_member
  , (typename T)
  , (T)
  , std::declval<T&>().begin()
  );

/// Generate \c has_begin_adl metafunction
ZENCXX_TT_EXPR_CHECKER(
    has_begin_adl
  , (typename T)
  , (T)
  , begin(std::declval<T&>())
  );

/// Generate \c has_end_member metafunction
ZENCXX_TT_EXPR_CHECKER(
    has_end_member
  , (typename T)
  , (T)
  , std::declval<T&>().end()
  );

/// Generate \c has_end_adl metafunction
ZENCXX_TT_EXPR_CHECKER(
    has_end_adl
  , (typename T)
  , (T)
  , end(std::declval<T&>())
  );

/**
 * \struct has_begin_member
 * \brief Metafunction to check if given type \c T has member-function \c begin()
 *
 * \tparam T type to check
 */

/**
 * \struct has_end_member
 * \brief Metafunction to check if given type \c T has member-function \c end()
 *
 * \tparam T type to check
 */

/**
 * \struct has_begin_adl
 * \brief Metafunction to check if there is a free function \c begin(T&) exists
 *
 * \tparam T type to check
 */

/**
 * \struct has_end_adl
 * \brief Metafunction to check if there is a free function \c end(T&) exists
 *
 * \tparam T type to check
 */

}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__TYPE_TRAITS__BEGIN_END_HH__
