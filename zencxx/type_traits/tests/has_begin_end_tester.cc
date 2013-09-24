/**
 * \file
 *
 * \brief Test begin/end metafunction checkers
 *
 * \date Tue Jul 23 05:45:04 MSK 2013 -- Initial design
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
#include <zencxx/type_traits/begin_end.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

namespace {
struct has_no_begin {};
struct has_begin
{
    void begin() {}
};
struct has_begin_const
{
    void begin() const {}
};
struct has_begin_inherited : public has_begin {};
struct has_begin_const_inherited : public has_begin_const {};

struct has_no_end {};
struct has_end
{
    void end() {}
};
struct has_end_const
{
    void end() const {}
};
struct has_end_inherited : public has_end {};
}                                                           // anonymous namespace


BOOST_AUTO_TEST_CASE(has_begin_member_test)
{
    BOOST_CHECK_EQUAL(zencxx::has_begin_member<int>::value, false);
    BOOST_CHECK_EQUAL(zencxx::has_begin_member<has_no_begin>::value, false);

    BOOST_CHECK_EQUAL(zencxx::has_begin_member<has_begin_const>::value, true);
    BOOST_CHECK_EQUAL(zencxx::has_begin_member<has_begin>::value, true);
    BOOST_CHECK_EQUAL(zencxx::has_begin_member<has_begin_inherited>::value, true);
}

BOOST_AUTO_TEST_CASE(has_end_member_test)
{
    BOOST_CHECK_EQUAL(zencxx::has_end_member<int>::value, false);
    BOOST_CHECK_EQUAL(zencxx::has_end_member<has_no_end>::value, false);

    BOOST_CHECK_EQUAL(zencxx::has_end_member<has_end_const>::value, true);
    BOOST_CHECK_EQUAL(zencxx::has_end_member<has_end>::value, true);
    BOOST_CHECK_EQUAL(zencxx::has_end_member<has_end_inherited>::value, true);
}

namespace adl {
struct iterable {};
int* begin(iterable&);
int* end(iterable&);
}                                                           // namespace adl

BOOST_AUTO_TEST_CASE(has_begin_adl_test)
{
    BOOST_CHECK_EQUAL(zencxx::has_begin_adl<int>::value, false);
    BOOST_CHECK_EQUAL(zencxx::has_begin_adl<char*>::value, false);

    BOOST_CHECK_EQUAL((zencxx::has_begin_adl<adl::iterable>::value), true);
    BOOST_CHECK_EQUAL((zencxx::has_begin_adl<std::array<char, 10>>::value), true);
    BOOST_CHECK_EQUAL((zencxx::has_begin_adl<std::vector<int>>::value), true);
}

BOOST_AUTO_TEST_CASE(has_end_adl_test)
{
    BOOST_CHECK_EQUAL(zencxx::has_end_adl<int>::value, false);
    BOOST_CHECK_EQUAL(zencxx::has_end_adl<char*>::value, false);

    BOOST_CHECK_EQUAL((zencxx::has_end_adl<adl::iterable>::value), true);
    BOOST_CHECK_EQUAL((zencxx::has_end_adl<std::vector<int>>::value), true);
}
