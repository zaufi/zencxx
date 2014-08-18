/**
 * \file
 *
 * \brief \c O1_size implementation for variadic \c seq
 *
 * \date Mon Jul  2 14:35:23 MSK 2012 -- Initial design
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
#include <zencxx/mpl/details/variadic_sequence_tag.hh>

// Standard includes
#include <boost/mpl/O1_size_fwd.hpp>

namespace boost { namespace mpl {

/**
 * \brief \c boost::mpl::O1_size implementation
 */
template <>
struct O1_size_impl<zencxx::mpl::details::variadic_sequence_tag>
{
    template <typename Seq>
    struct apply
    {
        static constexpr long value = Seq::size::value;
        typedef typename Seq::size type;
    };
};

}}                                                          // namespace mpl, boost
