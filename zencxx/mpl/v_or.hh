/**
 * \file
 *
 * \brief Class \c zencxx::mpl::v_or (interface)
 *
 * \date Sun Jul  8 06:19:16 MSK 2012 -- Initial design
 */
/*
 * Copyright (C) 2012-2014 Alex Turbov and contributors, all rights reserved.
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
#include <boost/mpl/eval_if.hpp>
#include <type_traits>

namespace zencxx { namespace mpl {

/**
 * \brief True variadic analog of \c boost::mpl::or_
 *
 * Generic template isn't defined
 */
template <typename...>
struct v_or;

/**
 * \brief True variadic analog of \c boost::mpl::or_
 *
 * Specialization to accept at least two parameters
 */
template <typename T1, typename T2, typename... Tail>
struct v_or<T1, T2, Tail...>
  : boost::mpl::eval_if<
      T1
    , std::true_type
    , v_or<T2, Tail...>
    >::type
{
};

/**
 * \brief True variadic analog of \c boost::mpl::or_
 *
 * Single parameter specialization
 */
template <typename T>
struct v_or<T> : std::integral_constant<bool, T::type::value>
{
};

}}                                                          // namespace mpl, zencxx
