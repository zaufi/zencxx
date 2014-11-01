/**
 * \file
 *
 * \brief Class tester for \c is_valid
 *
 * \date Sun Nov  2 00:41:46 MSK 2014 -- Initial design
 */
/*
 * Copyright (C) 2010-2014 Alex Turbov and contributors, all rights reserved.
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
#include <zencxx/type_traits/details/is_valid.hh>
#include <zencxx/debug/type_name.hh>
#include <zencxx/mpl/v_and.hh>

// Standard includes
#include <boost/noncopyable.hpp>
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>
#include <type_traits>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

namespace {

template <typename T>
using copy_assignable_t = decltype(std::declval<T&>() = std::declval<const T&>());

template <typename T>
using is_copy_assignable = zencxx::details::is_valid<T, copy_assignable_t>;

struct almost_test
{
    almost_test(int) {}
};

struct test : almost_test
{
    test(int a) : almost_test(a) {}
    void foo(const std::string&) {}
};

template <typename T>
using construct_from_int_t = decltype(T(int()));

template <typename T>
using has_foo_string_method_t = decltype(std::declval<T>().foo(std::declval<const std::string&>()));

template <typename T>
using is_looks_like_test = zencxx::mpl::v_and<
    zencxx::details::is_valid<T, construct_from_int_t>
  , zencxx::details::is_valid<T, has_foo_string_method_t>
  >;
}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(is_valid_test)
{
    BOOST_CHECK_EQUAL(is_copy_assignable<int>::value, true);
    BOOST_CHECK_EQUAL(is_copy_assignable<boost::noncopyable>::value, false);

    BOOST_CHECK_EQUAL(is_looks_like_test<test>::value, true);
    BOOST_CHECK_EQUAL(is_looks_like_test<almost_test>::value, false);
}
