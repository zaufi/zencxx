/**
 * \file
 *
 * \brief Class tester for \c seq
 *
 * \date Mon Jul  2 15:05:32 MSK 2012 -- Initial design
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
#include <zencxx/mpl/seq.hh>
#include <zencxx/mpl/copy_range.hh>
// Debugging helpers from zencxx::debug namespace
#include <zencxx/debug/type_name.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
#include <boost/test/output_test_stream.hpp>
#include <boost/mpl/O1_size.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/clear.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/iter_fold.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <iostream>

namespace dbg = zencxx::debug;
namespace mpl = zencxx::mpl;

// Uncomment if u want to use boost test output streams.
//  Then just output smth to it and valida an output by
//  BOOST_CHECK(out_stream.is_equal("Test text"))
using boost::test_tools::output_test_stream;

BOOST_AUTO_TEST_CASE(empty_seq_test)
{
    typedef mpl::seq<> seq_t;

    static_assert(
        boost::mpl::is_sequence<seq_t>::type::value
      , "seq_t expected to be an MPL sequence"
      );

    static_assert(
        boost::mpl::empty<seq_t>::value
      , "empty sequence expected"
      );
    static_assert(
        boost::mpl::O1_size<seq_t>::type::value == 0u
      , "empty sequence expected"
      );
    static_assert(
        boost::mpl::size<seq_t>::type::value == 0u
      , "empty sequence expected"
      );
    static_assert(
        std::is_same<boost::mpl::begin<seq_t>::type, boost::mpl::end<seq_t>::type>::value
      , "begin==end expected"
      );

    // back
    {
        typedef mpl::seq<char> one_char_seq_t;
        typedef boost::mpl::push_back<seq_t, char>::type one_item_seq_t;
        static_assert(
            boost::mpl::empty<one_char_seq_t>::value == false
          , "non empty sequence expected"
          );
        static_assert(
            std::is_same<one_char_seq_t, one_item_seq_t>::value
          , "equal sequences are expected"
          );
        typedef boost::mpl::pop_back<one_char_seq_t>::type empty_seq_t;
        static_assert(
            boost::mpl::empty<empty_seq_t>::value
          , "empty sequence expected"
          );
    }
    // front
    {
        typedef mpl::seq<char> one_char_seq_t;
        typedef boost::mpl::push_front<seq_t, char>::type one_item_seq_t;
        static_assert(
            std::is_same<one_char_seq_t, one_item_seq_t>::value
          , "equal sequences are expected"
          );
        typedef boost::mpl::pop_front<one_char_seq_t>::type empty_seq_t;
        static_assert(
            boost::mpl::empty<empty_seq_t>::value
          , "empty sequence expected"
          );
    }
}

BOOST_AUTO_TEST_CASE(one_item_seq_test)
{
    typedef mpl::seq<char> seq_t;

    static_assert(
        boost::mpl::is_sequence<seq_t>::type::value
      , "seq_t expected to be an MPL sequence"
      );

    static_assert(
        boost::mpl::empty<seq_t>::value == false
      , "non empty sequence expected"
      );
    static_assert(
        boost::mpl::O1_size<seq_t>::type::value == 1u
      , "non empty sequence expected"
      );
    static_assert(
        boost::mpl::size<seq_t>::type::value == 1u
      , "non empty sequence expected"
      );
    static_assert(
        std::is_same<char, boost::mpl::at_c<seq_t, 0>::type>::value
      , "char expected"
      );

    static_assert(
        std::is_same<char, boost::mpl::back<seq_t>::type>::value
      , "char expected"
      );
    static_assert(
        std::is_same<char, boost::mpl::front<seq_t>::type>::value
      , "char expected"
      );

    {
        typedef boost::mpl::begin<seq_t>::type begin_t;
        typedef boost::mpl::advance<begin_t, boost::mpl::long_<1>>::type iter_t;
        static_assert(
            std::is_same<boost::mpl::end<seq_t>::type, iter_t>::value
          , "end iterator expected"
          );
        static_assert(boost::mpl::distance<begin_t, iter_t>::type::value == 1u, "1 expected");
    }

    {
        typedef boost::mpl::pop_front<seq_t>::type empty_seq_t;
        static_assert(
            boost::mpl::empty<empty_seq_t>::value
          , "empty sequence expected"
          );
    }

    {
        typedef mpl::seq<char, short> char_short_seq_t;
        typedef boost::mpl::push_back<seq_t, short>::type two_items_seq_t;
        static_assert(
            std::is_same<char_short_seq_t, two_items_seq_t>::value
          , "equal sequences are expected"
          );
        static_assert(
            std::is_same<short, boost::mpl::back<char_short_seq_t>::type>::value
          , "short expected"
          );
        static_assert(
            std::is_same<char, boost::mpl::front<char_short_seq_t>::type>::value
          , "char expected"
          );
        typedef boost::mpl::pop_back<char_short_seq_t>::type one_item_seq_t;
        static_assert(
            boost::mpl::empty<one_item_seq_t>::value == false
          , "non empty sequence expected"
          );
        static_assert(
            std::is_same<one_item_seq_t, seq_t>::value
          , "equal sequences are expected"
          );
    }
}

BOOST_AUTO_TEST_CASE(two_items_seq_test)
{
    typedef mpl::seq<char, short> seq_t;

    static_assert(
        boost::mpl::is_sequence<seq_t>::type::value
      , "seq_t expected to be an MPL sequence"
      );

    static_assert(
        boost::mpl::empty<seq_t>::value == false
      , "non empty sequence expected"
      );
    static_assert(
        boost::mpl::O1_size<seq_t>::type::value == 2u
      , "non empty sequence expected"
      );
    static_assert(
        boost::mpl::size<seq_t>::type::value == 2u
      , "non empty sequence expected"
      );
    static_assert(
        std::is_same<short, boost::mpl::back<seq_t>::type>::value
      , "short expected"
      );
    static_assert(
        std::is_same<char, boost::mpl::front<seq_t>::type>::value
      , "char expected"
      );

    typedef boost::mpl::begin<seq_t>::type begin_t;
    typedef boost::mpl::advance<begin_t, boost::mpl::long_<1>>::type iter_t;
    static_assert(
        std::is_same<
            boost::mpl::end<seq_t>::type
          , boost::mpl::next<iter_t>::type
          >::value
        , "end iterator expected"
        );
    static_assert(
        boost::mpl::distance<begin_t, iter_t>::type::value == 1u
      , "1 expected"
      );
    static_assert(
        boost::mpl::distance<begin_t, boost::mpl::end<seq_t>::type>::type::value == 2u
      , "2 expected"
      );
}
namespace {

template <typename Next = void, typename T = void>
struct fold_visitor
{
    static void spam()
    {
        Next::spam();
        std::cout << "Current type: " << dbg::type_name(T()) << std::endl;
    }
};
template <>
struct fold_visitor<void, void>
{
    static void spam()
    {
    }
};

}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(seq_fold_test)
{
    typedef mpl::seq<char, short, int, long, long long, float, double, long double> seq_t;
    boost::mpl::fold<
        seq_t
      , fold_visitor<>
      , fold_visitor<boost::mpl::_, boost::mpl::_>
      >::type::spam();
}

BOOST_AUTO_TEST_CASE(seq_iter_fold_test)
{
    typedef mpl::seq<char, short, int, long, long long, float, double, long double> seq_t;
    boost::mpl::iter_fold<
        seq_t
      , fold_visitor<>
      , fold_visitor<boost::mpl::_, boost::mpl::_>
      >::type::spam();
}

BOOST_AUTO_TEST_CASE(seq_inserter_test)
{
    typedef boost::mpl::back_inserter<mpl::seq<>> inserter_t;
    typedef boost::mpl::apply<
        boost::mpl::unpack_args<inserter_t::operation>
      , mpl::seq<mpl::seq<>, char>
      >::type one_char_seq_t;
    static_assert(
        std::is_same<mpl::seq<char>, one_char_seq_t>::value
      , "seq<char> expected"
      );
}
