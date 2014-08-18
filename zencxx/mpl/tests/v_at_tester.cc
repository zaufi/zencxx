/**
 * \file
 *
 * \brief Class tester for \c v_at
 *
 * \date Sat Jun 30 22:34:13 MSK 2012 -- Initial design
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
#include <zencxx/mpl/v_at.hh>
// Debugging helpers from zencxx::debug namespace
#include <zencxx/debug/type_name.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
#include <iostream>

namespace dbg = zencxx::debug;
namespace mpl = zencxx::mpl;

BOOST_AUTO_TEST_CASE(v_at_test)
{
    static_assert(std::is_same<char, mpl::v_at_c<0, char>::type>::value, "char expected");
    static_assert(std::is_same<short, mpl::v_at_c<1, char, short>::type>::value, "short expected");
    static_assert(std::is_same<int, mpl::v_at_c<2, char, short, int>::type>::value, "int expected");
    static_assert(std::is_same<short, mpl::v_at_c<1, char, short, int>::type>::value, "short expected");
    static_assert(std::is_same<char, mpl::v_at_c<0, char, short, int>::type>::value, "char expected");
}
