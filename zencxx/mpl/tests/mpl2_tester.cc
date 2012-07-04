/**
 * \file
 *
 * \brief Class tester for \c mpl2
 *
 * \date Fri Jun 29 06:42:21 MSK 2012 -- Initial design
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
#include <boost/mpl/size.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/type_traits.hpp>
#include <iostream>


namespace dbg = zencxx::debug;
namespace mpl = zencxx::mpl;

namespace zencxx { namespace mpl {

using identity = boost::mpl::quote1<boost::mpl::identity>;
using size = boost::mpl::quote1<boost::mpl::size>;
using deref = boost::mpl::quote1<boost::mpl::deref>;
using end = boost::mpl::quote1<boost::mpl::end>;
using not_ = boost::mpl::quote1<boost::mpl::not_>;

using contains = boost::mpl::quote2<boost::mpl::contains>;
using find = boost::mpl::quote2<boost::mpl::find>;
using push_back = boost::mpl::quote2<boost::mpl::push_back>;
using is_same = boost::mpl::quote2<boost::is_same>;


template <
    typename Seq
  , typename Initial
  , typename Expr
  >
struct fold
{

};

#if 0
template <typename N>
struct arg : boost::mpl::int_<N>
{
    template <typename... Args>
    struct apply
    {
    };
}
#endif

template <typename Expr>
struct call_impl
{
    typedef Expr expression_type;
};

template <typename Expr>
struct bind;

template <typename R, typename... Args>
struct bind<R(Args...)>
{
    typedef call_impl<R(Args...)> call;
    template <typename... Z>
    struct apply{};
};



template <typename Expr, typename... Args>
struct eval<call_impl<Expr>(Args...)>
{
    typedef Expr expr;
    typedef int type;
};

#if 0
template <
    template <typename...> class Class
  >
struct unpack_args
{
    template <typename... Args>
    struct apply
    {
        typedef Class<Args...> type;
    };
};

template <template <typename...> class Class>
template <typename... Args>
struct unpack_args<Class>::apply<vector<Args...>>
  : public unpack_args<Class>::apply<Args...>               // reuse generic template
{
};
#endif

}}


using is_exists = mpl::not_(
    mpl::is_same(
        mpl::find(boost::mpl::_1, boost::mpl::_2)
      , mpl::end(boost::mpl::_1)
      )
  );

template <typename... Args>
struct test
{
    typedef test type;
};

template <typename>
struct params;

template <typename R, typename... Args>
struct params<R(*)(Args...)>
{
    typedef mpl::seq<Args...> type;
};

void foo(int, char);
void bar(float, long, ...);

BOOST_AUTO_TEST_CASE(mpl3_test)
{
    std::cout << dbg::type_name(params<&foo>::type()) << std::endl;
}

BOOST_AUTO_TEST_CASE(mpl2_test)
{
    typedef mpl::vector<char, short int> items;
    using integer_types = mpl::push_back(mpl::push_back(items, long), long long);
    using arithmetic_types = mpl::push_back(mpl::push_back(integer_types, float), double);

    {
        using result = mpl::eval<arithmetic_types>::type;
        std::cout << dbg::type_name(result()) << std::endl;
    }
    {
        using types = mpl::eval<arithmetic_types>::type;
        using is_present = call(is_exists, types, long);
        using result = mpl::eval<is_present>::expr;
        std::cout << dbg::type_name(std::add_pointer<result>::type()) << std::endl;
    }
#if 0
    {
        typedef boost::mpl::apply<
            mpl::unpack_args<test>
          , char, short, int, long, long long, float, double
          >::type result;
        std::cout << dbg::type_name(std::add_pointer<result>::type()) << std::endl;
    }
#endif

#if 0
    {
        using expr = mpl::append_if_absent(arithmetic_types, unsigned);
        using result = eval<expr>::type;
        std::cout << dbg::type_name(result()) << std::endl;
    }
#endif

}
