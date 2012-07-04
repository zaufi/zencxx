/**
 * \file
 *
 * \brief Class tester for \c zencxx::mpl::eval
 *
 * \date Wed Jul  4 05:30:56 MSK 2012 -- Initial design
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
#include <zencxx/mpl/eval.hh>
#include <zencxx/mpl/seq.hh>
// Debugging helpers from zencxx::debug namespace
#include <zencxx/debug/type_name.hh>

// Standard includes
#define BOOST_TEST_DYN_LINK
#include <boost/test/auto_unit_test.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <iostream>
#include <utility>

namespace dbg = zencxx::debug;
namespace mpl = zencxx::mpl;

namespace zencxx { namespace mpl {
using identity = boost::mpl::quote1<boost::mpl::identity>;
using push_back = boost::mpl::quote2<boost::mpl::push_back>;

struct eval_if
{
    template <typename Cond, typename Then, typename Otherwise>
    struct apply
    {
        typedef typename boost::mpl::eval_if<
            typename eval<Cond>::type
          , eval<Then>
          , eval<Otherwise>
          >::type type;
    };
};

template <>
template <typename... Args>
struct eval_impl<call_meta_function_class_tag>::apply<eval_if(Args...)>
{
    typedef typename eval_if::template apply<Args...>::type type;
};

template <>
template <typename... Args>
struct eval_impl<call_meta_function_class_tag>::apply<eval_if(*)(Args...)>
{
    typedef typename eval_if::template apply<Args...>::type type;
};

}}                                                          // namespace mpl, zencxx


BOOST_AUTO_TEST_CASE(simple_eval_test)
{
    typedef mpl::seq<> seq_t;
    using expr = mpl::push_back(seq_t, char);
    using result = mpl::eval<expr>::type;
    std::cout << dbg::type_name(result()) << std::endl;
}

BOOST_AUTO_TEST_CASE(eval_test2)
{
    typedef mpl::seq<> seq_t;
    using expr = mpl::push_back(mpl::push_back(seq_t, int), char);
    using result = mpl::eval<expr>::type;
    std::cout << dbg::type_name(result()) << std::endl;
}

namespace {
struct incomplete;

struct complete {};

template <typename T>
struct not_for_integrals
{
    typedef typename std::enable_if<!std::is_integral<T>::value, T>::type type;
};
}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(eval_if_test)
{
    {
        using expr = mpl::eval_if(boost::mpl::true_, int, incomplete);
        using result = mpl::eval<expr>::type;
        std::cout << dbg::type_name(result()) << std::endl;
    }
    {
        using expr = mpl::eval_if(
            boost::mpl::true_
          , not_for_integrals<complete>
          , not_for_integrals<int>
          );
        using result = mpl::eval<expr>::type;
        std::cout << dbg::type_name(result()) << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(eval_if_test2)
{
    {
        typedef boost::mpl::eval_if<
            boost::mpl::true_
          , boost::mpl::identity<int>
          , boost::mpl::identity<incomplete>
          >::type result;
        std::cout << dbg::type_name(result()) << std::endl;
    }
    {
        typedef boost::mpl::eval_if<
            boost::mpl::true_
          , boost::mpl::identity<not_for_integrals<complete>>
          , boost::mpl::identity<not_for_integrals<int>>
          >::type result;
        std::cout << dbg::type_name(result()) << std::endl;
    }
}
