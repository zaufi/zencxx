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
#include <zencxx/mpl/bind.hh>
#include <zencxx/mpl/eval.hh>
#include <zencxx/mpl/eval_if.hh>
#include <zencxx/mpl/iter_fold.hh>
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
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/lambda.hpp>
#include <iostream>
#include <utility>

namespace dbg = zencxx::debug;
namespace mpl = zencxx::mpl;

namespace zencxx { namespace mpl {
using identity = boost::mpl::quote1<boost::mpl::identity>;
using push_back = boost::mpl::quote2<boost::mpl::push_back>;
}}                                                          // namespace mpl, zencxx

#define mpl_eval(Expr) mpl::eval<Expr>::type

BOOST_AUTO_TEST_CASE(simple_eval_test)
{
    typedef mpl::seq<> seq_t;
    using expr = mpl::push_back(seq_t, char);
    using result = mpl::eval<expr>::type;
    static_assert(std::is_same<result, mpl::seq<char>>::value, "mpl::seq<char> expected");
}

BOOST_AUTO_TEST_CASE(eval_test2)
{
    typedef mpl::seq<> seq_t;
    using expr = mpl::push_back(mpl::push_back(seq_t, int), char);
    using result = mpl_eval(expr);
    static_assert(std::is_same<result, mpl::seq<int, char>>::value, "mpl::seq<int, char> expected");
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
        static_assert(
            std::is_same<result, int>::value
          , "int expected"
          );
    }
    {
        using expr = mpl::eval_if(
            boost::mpl::true_
          , not_for_integrals<complete>
          , not_for_integrals<int>
          );
        using result = mpl::eval<expr>::type;
        static_assert(
            std::is_same<result, not_for_integrals<complete>>::value
          , "not_for_integrals<complete> expected"
          );
    }
}

BOOST_AUTO_TEST_CASE(boost_eval_if_test)
{
    {
        typedef boost::mpl::eval_if<
            boost::mpl::true_
          , boost::mpl::identity<int>
          , boost::mpl::identity<incomplete>
          >::type result;
        static_assert(
            std::is_same<result, int>::value
          , "int expected"
          );
    }
    {
        typedef boost::mpl::eval_if<
            boost::mpl::true_
          , boost::mpl::identity<not_for_integrals<complete>>
          , boost::mpl::identity<not_for_integrals<int>>
          >::type result;
        static_assert(
            std::is_same<result, not_for_integrals<complete>>::value
          , "not_for_integrals<complete> expected"
          );
    }
}

BOOST_AUTO_TEST_CASE(bind_form_1_test)
{
    // Use bind to make a lambda
    using push_expr = mpl::bind(mpl::push_back, mpl::_1, mpl::_2);
    using result = mpl::is_lambda_expression<push_expr>::type;
    static_assert(result::value, "lambda expression expected");
}

BOOST_AUTO_TEST_CASE(lambda_eval_test)
{
    using push_expr = mpl::push_back(mpl::_1, mpl::_2);
    {
        using result = mpl::is_lambda_expression<push_expr>::type;
        static_assert(result::value, "lambda expression expected");
    }

    {
        using nulary_mf = mpl::bind(push_expr, mpl::seq<>, char);
        static_assert(mpl::is_lambda_expression<nulary_mf>::value, "lambda expression expected");

        using result = mpl::eval<nulary_mf>::type;
        static_assert(
            std::is_same<result, mpl::seq<char>>::value
          , "seq<char> expected"
          );
    }

    {
        using more_push_expr = mpl::push_back(push_expr, char);
        static_assert(mpl::is_lambda_expression<more_push_expr>::value, "lambda expression expected");

        using nulary_mf = mpl::bind(more_push_expr, mpl::seq<>, char);

        using result = mpl::eval<nulary_mf>::type;
#if 0
        std::cout << __PRETTY_FUNCTION__ << ": " << dbg::type_name(result()) << std::endl;
#endif
        static_assert(
            std::is_same<result, mpl::seq<char, char>>::value
          , "seq<char, char> expected"
          );
    }
}

// namespace {
/**
 * \brief Class \c test
 */
struct test
{
    template <typename State, typename Iter>
    struct apply
    {
        struct type
        {
            static void spam()
            {
                State::spam();
                std::cout << "Current type: " << dbg::type_name(Iter()) << std::endl;
            }
        };
    };
    static void spam()
    {
        std::cout << "Initial state" << std::endl;
    }
};
// }                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(iter_fold_test)
{
    typedef mpl::seq<char, short, int> seq_t;
    using expr = mpl::iter_fold(seq_t, test, test(mpl::_1, mpl::_2));
    using result = mpl::eval<expr>::type;
    std::cout << dbg::type_name(result()) << std::endl;
    std::cout << "Invoke spam chain:" << std::endl;
    result::spam();
}

#if 0
BOOST_AUTO_TEST_CASE(fold_test)
{
    typedef mpl::seq<char, short, int> seq_t;
    using expr = mpl::fold(seq_t, test, test(mpl::_1, mpl::_2));
    using result = mpl::eval<expr>::type;
    std::cout << dbg::type_name(result()) << std::endl;
    std::cout << "Invoke spam chain:" << std::endl;
    result::spam();
}
#endif

#ifdef ZENCXX_PLAYGROUND

using r = fold(vec_of_vec, seq<>, fold(_2, _1, protect(push_back(_1, _2))));
using z = bind(push_back, _1, char);
using nf = mpl::bind(push_expr, mpl::seq<>, char);

using f = mpl::bind(mpl::_1, mpl::seq<>, char);
using nf = mpl::bind(f, mpl::push_back);

using f = mpl::bind(mpl::_1, mpl::_2, mpl::_3);
using uf = mpl::bind(f, mpl::pair(mpl::_1, mpl::push_back), mpl::pair(mpl::_2, mpl::seq<>));

// given: _1, _3
// rest: _2, _4

// out: unary fn w/ _1, _2
#endif                                                      // ZENCXX_PLAYGROUND
namespace {
/**
 * \brief Metafunction class \c test
 */
struct sample_mfc
{
    template <typename T1, typename T2, typename T3>
    struct apply
    {
        typedef std::tuple<T1, T2, T3> type;
    };
};

template <typename T1, typename T2>
struct sample_mf
{
    typedef std::pair<T1, T2> type;
};

}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(boost_mpl_test)
{
#if 0
    typedef boost::mpl::bind<sample_mfc, boost::mpl::_1, boost::mpl::_2, char> bind1;
    typedef boost::mpl::bind<boost::mpl::protect<bind1>::type, char, boost::mpl::_1> bind2;
    typedef boost::mpl::bind<boost::mpl::protect<bind2>::type, int> bind3;
    typedef boost::mpl::apply<bind3>::type result;
#endif
    using pe = mpl::push_back(mpl::seq<>, mpl::_1);
    using result = mpl::push_back(pe, int);
    std::cout << "*** " << dbg::type_name(sample_mf<result, void>()) << std::endl;
    //
#if 0
    std::cout << dbg::type_name(boost::mpl::lambda<sample_mf<boost::mpl::_1, boost::mpl::_2>>::type()) << std::endl;
#endif
#if 0

#endif
#if 0
    typedef boost::mpl::bind<boost::mpl::lambda<bind1>::type, int, mpl::_1> bind2;
    typedef boost::mpl::bind<boost::mpl::lambda<bind2>::type, char> bind3;
    typedef boost::mpl::apply<bind3>::type result;
    std::cout << dbg::type_name(result()) << std::endl;
#endif
}
