/**
 * \file
 *
 * \brief Class tester for \c zencxx::has_left_shift
 *
 * \date Wed Jun 27 02:48:18 MSK 2012 -- Initial design
 */
/*
 * ZenCxx is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libzencxx is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

// Project specific includes
#include <zencxx/type_traits/has_left_shift.hh>

// Standard includes
// ALERT The following #define must be enabled only in one translation unit
// per unit test binary (which may consists of several such modules)
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>

// Uncomment if u want to use boost test output streams.
//  Then just output smth to it and valida an output by
//  BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

// Your first test function :)
BOOST_AUTO_TEST_CASE(has_left_shift_test)
{
    BOOST_CHECK_EQUAL((zencxx::has_left_shift<int, int>::type::value), true);
    BOOST_CHECK_EQUAL((zencxx::has_left_shift<std::ostream, int>::type::value), true);
    BOOST_CHECK_EQUAL((zencxx::has_left_shift<std::ostream, std::string>::type::value), true);

    BOOST_CHECK_EQUAL((zencxx::has_left_shift<std::string, std::string>::type::value), false);
    BOOST_CHECK_EQUAL((zencxx::has_left_shift<int, std::string>::type::value), false);
}
