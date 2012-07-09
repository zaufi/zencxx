/**
 * \file
 *
 * \brief Class tester for \c mpl_traits
 *
 * \date Wed Jul  4 08:13:33 MSK 2012 -- Initial design
 */
/*
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
#include <zencxx/mpl/eval_if.hh>
#include <zencxx/mpl/pair.hh>
#include <zencxx/mpl/is_lambda_expression.hh>
#include <zencxx/mpl/placeholders.hh>
#include <zencxx/mpl/seq.hh>
// Debugging helpers from zencxx::debug namespace
#include <zencxx/debug/type_name.hh>

// Standard includes
// ALERT The following #define must be enabled only in one translation unit
// per unit test binary (which may consists of several such modules)
// #define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
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
struct has_apply0
{
    struct apply
    {
        typedef int type;
    };
};
struct has_apply1
{
    template <typename T>
    struct apply
    {
        typedef T type;
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

    static_assert(mpl::has_apply<has_apply0>::value == true, "apply expected");
    static_assert(mpl::has_apply<has_apply1, int>::value == true, "apply expected");

    // Make and check a metafunction class
    using pb = boost::mpl::quote2<boost::mpl::push_back>;
    static_assert(mpl::has_apply<pb, mpl::seq<>, int>::value == true, "apply expected");
}


BOOST_AUTO_TEST_CASE(is_lambda_expression_test)
{
    // Is not lambda!
    static_assert(
        mpl::is_lambda_expression<int>::value == false
      , "no lambda expression expected"
      );
    static_assert(
        mpl::is_lambda_expression<mpl::pair(int, char)>::value == false
      , "no lambda expression expected"
      );

    // Lambda!
    static_assert(
        mpl::is_lambda_expression<mpl::_1>::value == true
      , "lambda expression expected"
      );
    static_assert(
        mpl::is_lambda_expression<mpl::pair(mpl::_1, mpl::_2)>::value == true
      , "lambda expression expected"
      );

    static_assert(
        mpl::is_lambda_expression<mpl::pair(mpl::pair(mpl::_1, mpl::_2), int)>::value == true
      , "lambda expression expected"
      );
}
#if 0
BOOST_AUTO_TEST_CASE(is_function_test)
{
    std::cout << std::is_function<void()>::value << std::endl;
    std::cout << std::is_function<std::remove_pointer<int(*)()>::type>::value << std::endl;
    std::cout << std::is_function<std::remove_reference<int(&)()>::type>::value << std::endl;
}
#endif
