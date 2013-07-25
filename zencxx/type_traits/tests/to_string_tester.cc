/**
 * \file
 *
 * \brief Class tester for \c to_string
 *
 * \date Thu Jul 25 09:37:57 MSK 2013 -- Initial design
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
#include <zencxx/type_traits/to_string.hh>
#include <zencxx/debug/type_name.hh>

// Standard includes
// ALERT The following #define must be enabled only in one translation unit
// per unit test binary (which may consists of several such modules)
// #define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/auto_unit_test.hpp>
#include <boost/uuid/uuid_io.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>
#include <string>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

namespace debug = zencxx::debug;

namespace adl {
struct non_convertible_to_string_anyway {};
struct has_to_string_member
{
    std::string to_string() const;
};
struct has_to_string_adl
{
};
std::string to_string(const has_to_string_adl&)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return std::string();
}
}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(to_string_adl_test)
{
    BOOST_CHECK_EQUAL(zencxx::has_to_string_adl<char>::value, false);
    BOOST_CHECK_EQUAL(zencxx::has_to_string_adl<int>::value, false);
    BOOST_CHECK_EQUAL(zencxx::has_to_string_adl<int*>::value, false);
    BOOST_CHECK_EQUAL(zencxx::has_to_string_adl<adl::non_convertible_to_string_anyway>::value, false);
    BOOST_CHECK_EQUAL(zencxx::has_to_string_adl<adl::has_to_string_member>::value, false);

    BOOST_CHECK_EQUAL(zencxx::has_to_string_adl<boost::uuids::uuid>::value, true);
    BOOST_CHECK_EQUAL(zencxx::has_to_string_adl<adl::has_to_string_adl>::value, true);
}

BOOST_AUTO_TEST_CASE(to_string_member_test)
{
    BOOST_CHECK_EQUAL(zencxx::has_to_string_member<char>::value, false);
    BOOST_CHECK_EQUAL(zencxx::has_to_string_member<adl::non_convertible_to_string_anyway>::value, false);
    BOOST_CHECK_EQUAL(zencxx::has_to_string_member<adl::has_to_string_adl>::value, false);
    BOOST_CHECK_EQUAL(zencxx::has_to_string_member<boost::uuids::uuid>::value, false);

    BOOST_CHECK_EQUAL(zencxx::has_to_string_member<adl::has_to_string_member>::value, true);
}
