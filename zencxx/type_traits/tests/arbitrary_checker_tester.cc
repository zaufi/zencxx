/**
 * \file
 *
 * \brief Tester for \c ZENCXX_TT_EXPR_CHECKER_EX
 *
 * \date Wed Jul 24 08:20:12 MSK 2013 -- Initial design
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
#define ZENCXX_TYPE_TRAITS_ENABLE_DIAGNOSTIC
#include <zencxx/type_traits/details/expression_validity_checker.hh>

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
struct not_a_test {};

struct test
{
    test(int) {}
    void bar(const std::string&) const;
};

ZENCXX_TT_EXPR_CHECKER_EX(
    is_looks_like_test
  , (typename T)
  , (const T& t)
  , (T)
  , (std::declval<const T&>())
  , T(123)                                                  // check that `T' can be constructed from `int'
  , t.bar(std::declval<const std::string&>())               // check that instance of `T' has a `bar' member-function accepting a string
  );
}                                                           // anonymous namespace


BOOST_AUTO_TEST_CASE(arbitrary_checker_test)
{
    BOOST_CHECK_EQUAL(is_looks_like_test<not_a_test>::value, false);
    BOOST_CHECK_EQUAL(is_looks_like_test<test>::value, true);
}
