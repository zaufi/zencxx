/**
 * \file
 *
 * \brief Metafunction \c zencxx::mpl::details::v_at_impl
 *
 * \date Sat Jun 30 22:27:47 MSK 2012 -- Initial design
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
#include <boost/mpl/next.hpp>

namespace zencxx { namespace mpl { namespace details {

// Generic template (nothing 2 do)
template <
    typename WantPos
  , typename StartPos
  , typename... Seq
  >
struct v_at_impl;

// Still not found...
template <
    typename WantPos
  , typename CurPos
  , typename T
  , typename... Types
  >
struct v_at_impl<WantPos, CurPos, T, Types...>
{
    typedef typename v_at_impl<
        WantPos
      , typename boost::mpl::next<CurPos>::type
      , Types...
      >::type type;
};

// Got it!
template <
    typename FoundPos
  , typename T
  , typename... Types
  >
struct v_at_impl<
    FoundPos
  , FoundPos
  , T
  , Types...
  >
{
    typedef T type;
};

}}}                                                         // namespace details, mpl, zencxx
