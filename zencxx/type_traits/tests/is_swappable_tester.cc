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
#include <zencxx/debug/type_name.hh>

// Standard includes
#include <boost/noncopyable.hpp>
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

namespace test {
struct not_swappable
{
    /// Delete copy ctor
    not_swappable(const not_swappable&) = delete;
    /// Delete copy-assign operator
    not_swappable& operator=(const not_swappable&) = delete;
    /// Delete move ctor
    not_swappable(not_swappable&&) = delete;
    /// Delete move-assign operator
    not_swappable& operator=(not_swappable&&) = delete;
};

struct foo {};
void swap(foo&, foo&) {}

struct bar {};
}                                                           // namespace test

namespace {
struct foo
{
    boost::noncopyable m_member;
};
struct bar {};
void swap(bar&, bar&) {}
}                                                           // anonymous namespace


BOOST_AUTO_TEST_CASE(is_swappable_test)
{
    // Trivial types should be swappable w/ std::swap
    BOOST_CHECK_EQUAL((zencxx::is_swappable<int>::value), true);
    BOOST_CHECK_EQUAL((zencxx::is_swappable<char*>::value), true);

    // No corresponsing swap()! But move constructible/assignable by default.
    BOOST_CHECK_EQUAL((zencxx::is_swappable<test::bar>::value), true);

    // Not a move constructible/assignable, so can't be swapped
    BOOST_CHECK_EQUAL((zencxx::is_swappable<test::not_swappable>::value), false);

    // Swappable due ADL
    BOOST_CHECK_EQUAL((zencxx::is_swappable<test::foo>::value), true);
    BOOST_CHECK_EQUAL((zencxx::is_swappable<bar>::value), true);

    // Not swappable due a "bad" member
    BOOST_CHECK_EQUAL((zencxx::is_swappable<foo>::value), false);
}
