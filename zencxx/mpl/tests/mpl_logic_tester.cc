/**
 * \file
 *
 * \brief Class tester for \c mpl_logic
 *
 * \date Wed Jul 11 07:58:13 MSK 2012 -- Initial design
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
#include <zencxx/mpl/v_and.hh>
#include <zencxx/mpl/v_or.hh>
// Debugging helpers from zencxx::debug namespace
#include <zencxx/debug/type_name.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>

namespace dbg = zencxx::debug;
namespace mpl = zencxx::mpl;

// Uncomment if u want to use boost test output streams.
//  Then just output smth to it and valida an output by
//  BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

namespace {
struct incomplete;

template <typename T>
struct not_for_integrals
{
    typedef typename std::enable_if<!std::is_integral<T>::value, T>::type type;
};
}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(mpl_logic_and_test)
{
    {
        using result = mpl::v_and<boost::mpl::false_>;
        static_assert(!result::type::value, "false expected");
    }
    {
        using result = mpl::v_and<boost::mpl::true_>;
        static_assert(result::type::value, "true expected");
    }
    {
        using result = mpl::v_and<boost::mpl::false_, boost::mpl::true_>;
        static_assert(!result::type::value, "false expected");
    }
    {
        using result = mpl::v_and<boost::mpl::true_, boost::mpl::false_, incomplete, not_for_integrals<int>>;
        static_assert(!result::type::value, "false expected");
    }
}

BOOST_AUTO_TEST_CASE(mpl_logic_or_test)
{
    {
        using result = mpl::v_or<boost::mpl::false_>;
        static_assert(!result::type::value, "false expected");
    }
    {
        using result = mpl::v_or<boost::mpl::false_, boost::mpl::false_>;
        static_assert(!result::type::value, "false expected");
    }
    {
        using result = mpl::v_or<boost::mpl::false_, boost::mpl::false_, boost::mpl::true_>;
        static_assert(result::type::value, "true expected");
    }
    {
        using result = mpl::v_or<boost::mpl::false_, boost::mpl::true_, incomplete, not_for_integrals<int>>;
        static_assert(result::type::value, "true expected");
    }
}
