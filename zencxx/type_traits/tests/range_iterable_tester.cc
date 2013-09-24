/**
 * \file
 *
 * \brief Class tester for \c range_iterable
 *
 * \date Thu Jul 25 06:04:52 MSK 2013 -- Initial design
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
#include <zencxx/type_traits/is_range_iterable.hh>
#include <zencxx/debug/type_name.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>
#include <string>
#include <vector>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

namespace debug = zencxx::debug;

namespace adl {
struct iterable
{
    int m_arr[123];
};
int* begin(iterable& it)
{
    return it.m_arr;
}
int* end(iterable& it)
{
    return it.m_arr + 123;
}
}                                                           // namespace adl

namespace member {
struct iterable
{
    int m_arr[123];
    int* begin()
    {
        return m_arr;
    }
    int* end()
    {
        return m_arr + 123;
    }
};
struct const_iterable : iterable
{
    int m_arr[123];
    const int* begin() const
    {
        return m_arr;
    }
    const int* end() const
    {
        return m_arr + 123;
    }
};
}                                                           // namespace member

BOOST_AUTO_TEST_CASE(range_iterable_test)
{
    // Check some non-iterable types
    BOOST_CHECK_EQUAL(zencxx::is_range_iterable<int>::value, false);
    BOOST_CHECK_EQUAL(zencxx::is_range_iterable<char*>::value, false);

    // Check some standard types
    BOOST_CHECK_EQUAL(zencxx::is_range_iterable<char[123]>::value, true);
    BOOST_CHECK_EQUAL(zencxx::is_range_iterable<std::vector<int>>::value, true);
    BOOST_CHECK_EQUAL(zencxx::is_range_iterable<std::string>::value, true);

    // User defined types
    BOOST_CHECK_EQUAL(zencxx::is_range_iterable<adl::iterable>::value, true);
    BOOST_CHECK_EQUAL(zencxx::is_range_iterable<member::iterable>::value, true);
    BOOST_CHECK_EQUAL(zencxx::is_range_iterable<const member::iterable>::value, false);
    BOOST_CHECK_EQUAL(zencxx::is_range_iterable<const member::const_iterable>::value, true);
}
