/**
 * \file
 *
 * \brief Class tester for \c is_iterator
 *
 * \date Tue Jul 23 08:15:58 MSK 2013 -- Initial design
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
#include <zencxx/type_traits/is_iterator.hh>
#include <zencxx/debug/type_name.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>
#include <list>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

namespace {
struct not_an_iterator {};

struct type_with_value_type
{
    typedef int value_type;
};

struct almost_iterator
{
    typedef int value_type;
    typedef int difference_type;
    typedef int iterator_category;
    typedef int* pointer;
    typedef int& reference;

    int operator*()
    {
        return int();
    }
    // NOTE There is no operator++, so it doesn't
    // conforms Iterator concept
};

struct looks_like_iterator
{
    typedef int value_type;
    typedef int difference_type;
    typedef int iterator_category;
    typedef int* pointer;
    typedef int& reference;

    reference operator*()
    {
        static auto value = 123;
        return value;
    }
    looks_like_iterator& operator++()
    {
        return *this;
    }
};
}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(is_iterator_test)
{
    using namespace zencxx;

    BOOST_CHECK_EQUAL(is_iterator<int>::value, false);
    BOOST_CHECK_EQUAL(is_iterator<not_an_iterator>::value, false);
    BOOST_CHECK_EQUAL(is_iterator<almost_iterator>::value, false);
    BOOST_CHECK_EQUAL(is_iterator<type_with_value_type>::value, false);

    BOOST_CHECK_EQUAL(is_iterator<int*>::value, true);
    BOOST_CHECK_EQUAL(is_iterator<looks_like_iterator>::value, true);
    BOOST_CHECK_EQUAL(is_iterator<decltype(std::declval<std::list<int>>().begin())>::value, true);
    BOOST_CHECK_EQUAL(is_iterator<decltype(std::declval<std::string>().cbegin())>::value, true);
}
