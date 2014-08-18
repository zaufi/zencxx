/**
 * \file
 *
 * \brief Class tester for \c copy_range
 *
 * \date Tue Jul  3 22:31:32 MSK 2012 -- Initial design
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

// Project specific includes
#include <zencxx/mpl/copy_range.hh>
#include <zencxx/mpl/seq.hh>
// Debugging helpers from zencxx::debug namespace
#include <zencxx/debug/type_name.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
#include <iostream>
#include <type_traits>

namespace dbg = zencxx::debug;
using namespace zencxx::mpl;

BOOST_AUTO_TEST_CASE(copy_range_test)
{
    typedef seq<char, short, int, long> seq_t;
    {
        typedef copy_range_c<seq_t, 0, 0>::type dst_t;
        static_assert(std::is_same<seq<>, dst_t>::value, "Empty seq expected");
    }
    {
        typedef copy_range_c<seq_t, 0, 1>::type dst_t;
        static_assert(std::is_same<seq<char>, dst_t>::value, "seq<char> expected");
    }
    {
        typedef copy_range_c<seq_t, 1, 2>::type dst_t;
        static_assert(std::is_same<seq<short>, dst_t>::value, "seq<short> expected");
    }
    {
        typedef copy_range_c<seq_t, 0, 2>::type dst_t;
        static_assert(std::is_same<seq<char, short>, dst_t>::value, "seq<char, short> expected");
    }
    {
        typedef copy_range_c<seq_t, 1, 3>::type dst_t;
        static_assert(std::is_same<seq<short, int>, dst_t>::value, "seq<short, int> expected");
    }
    {
        typedef copy_range_c<seq_t, 3, 3>::type dst_t;
        static_assert(std::is_same<seq<>, dst_t>::value, "seq<> expected");
    }

    {
        typedef seq<> empty_seq_t;
        typedef copy_range_c<empty_seq_t, 0, 0>::type dst_t;
        static_assert(std::is_same<seq<>, dst_t>::value, "Empty seq expected");
    }
}
