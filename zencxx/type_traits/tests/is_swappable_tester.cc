/**
 * \file
 *
 * \brief Class tester for \c is_swappable
 *
 * \date Tue Jul 23 10:03:37 MSK 2013 -- Initial design
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

// Project specific includes
#include <zencxx/type_traits/is_swappable.hh>

// Standard includes
// ALERT The following #define must be enabled only in one translation unit
// per unit test binary (which may consists of several such modules)
// #define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

namespace test {
struct not_swappable {};
struct foo {};
struct bar {};
void swap(foo&, bar&) {}
void swap(bar&, foo&) {}
}                                                           // namespace test

namespace {
struct foo {};
struct bar {};
void swap(foo&, bar&) {}
}                                                           // anonymous namespace


BOOST_AUTO_TEST_CASE(is_swappable_adl_test)
{
    BOOST_CHECK_EQUAL((zencxx::is_swappable_adl<test::foo, test::bar>::value), true);
    BOOST_CHECK_EQUAL((zencxx::is_swappable_adl<foo, bar>::value), true);
}

BOOST_AUTO_TEST_CASE(is_swappable_test)
{
    BOOST_CHECK_EQUAL((zencxx::is_swappable<int, char*>::value), false);
    BOOST_CHECK_EQUAL((zencxx::is_swappable<test::not_swappable, test::foo>::value), false);

    // Should be swappable w/ std::swap
    BOOST_CHECK_EQUAL((zencxx::is_swappable<int, int>::value), true);
    BOOST_CHECK_EQUAL((zencxx::is_swappable<char*, char*>::value), true);

    // Swappable due ADL
    BOOST_CHECK_EQUAL((zencxx::is_swappable<test::foo, test::bar>::value), true);
    BOOST_CHECK_EQUAL((zencxx::is_swappable<foo, bar>::value), true);
    // NOTE No corresponsing swap()!
    BOOST_CHECK_EQUAL((zencxx::is_swappable<bar, foo>::value), false);
}
