/**
 * \file
 *
 * \brief Forward declarations of \c copy_range and \c copy_range_impl
 *
 * \date Tue Jul  3 21:48:58 MSK 2012 -- Initial design
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
#include <zencxx/mpl/seq_fwd.hh>

// Standard includes
#include <boost/mpl/back_inserter.hpp>

namespace zencxx { namespace mpl {

// NOTE There is no default implementation! Only specializations...
// (for now at least)
template <typename Tag>
struct copy_range_impl;

template <
    typename Seq
  , typename First
  , typename Last
  , typename Inserter = boost::mpl::back_inserter<seq<>>
  >
struct copy_range;

}}                                                          // namespace mpl, zencxx
