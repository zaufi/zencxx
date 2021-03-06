/**
 * \file
 *
 * \brief Class tester for \c mpl_traits
 *
 * \date Wed Jul  4 08:13:33 MSK 2012 -- Initial design
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
#include <zencxx/mpl/has_apply.hh>
#include <zencxx/mpl/has_type.hh>
#include <zencxx/mpl/pair.hh>
#include <zencxx/mpl/seq.hh>
// Debugging helpers from zencxx::debug namespace
#include <zencxx/debug/type_name.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/quote.hpp>
#include <iostream>
#include <type_traits>

namespace dbg = zencxx::debug;
namespace mpl = zencxx::mpl;

// Uncomment if u want to use boost test output streams.
//  Then just output smth to it and valida an output by
//  BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

namespace {
struct empty {};

struct has_type
{
    typedef void type;
};

struct nulary
{
    struct apply
    {
        typedef void type;
    };
};

struct unary
{
    template <typename T>
    struct apply
    {
        typedef T type;
    };
};

struct binary
{
    template <typename T1, typename T2>
    struct apply
    {
        typedef std::pair<T1, T2> type;
    };
};

}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(has_type_test)
{
    static_assert(mpl::has_type<int>::value == false, "no type expected");
    static_assert(mpl::has_type<empty>::value == false, "no type expected");
    static_assert(mpl::has_type<boost::mpl::quote2<boost::mpl::push_back>>::value == false, "no type expected");

    static_assert(mpl::has_type<has_type>::value == true, "type expected");
    static_assert(mpl::has_type<mpl::seq<>>::value == true, "type expected");
    static_assert(mpl::has_type<mpl::seq<char>>::value == true, "type expected");

    // Check a metafunction
    static_assert(mpl::has_type<std::is_same<char, int>>::value == true, "type expected");
    static_assert(mpl::has_type<boost::mpl::push_back<mpl::seq<>, int>>::value == true, "type expected");
}

BOOST_AUTO_TEST_CASE(has_apply_test)
{
    static_assert(mpl::has_apply<int>::value == false, "no apply expected");
    static_assert(mpl::has_apply<empty>::value == false, "no apply expected");

    static_assert(mpl::has_apply<nulary>::value == true, "apply expected");
    static_assert(mpl::has_apply<unary, int>::value == true, "apply expected");
    static_assert(mpl::has_apply<binary, int, long>::value == true, "apply expected");

    // Make and check a real metafunction class
    using pb = boost::mpl::quote2<boost::mpl::push_back>;
    static_assert(mpl::has_apply<pb, mpl::seq<>, int>::value == true, "apply expected");
}

BOOST_AUTO_TEST_CASE(another_has_apply_test)
{
    BOOST_CHECK(!zencxx::mpl::has_apply<empty>::value);
}
