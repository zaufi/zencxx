/**
 * \file
 *
 * \brief Class \c zencxx::mpl::details::bind_impl (interface)
 *
 * \date Tue Jul 10 06:54:51 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__DETAILS__BIND_IMPL_HH__
# define __ZENCXX__MPL__DETAILS__BIND_IMPL_HH__

// Project specific includes
# include <zencxx/mpl/details/resolve_args.hh>
# include <zencxx/mpl/eval.hh>
# include <zencxx/mpl/is_lambda_expression.hh>
# include <zencxx/mpl/seq.hh>
# include <zencxx/mpl/v_apply.hh>

// Standard includes

namespace zencxx { namespace mpl { namespace details {

template <typename, typename...>
struct bind_impl;

template <typename R, typename... Args, typename... Params>
struct bind_impl<R(Args...), Params...>
{
    static_assert(
        is_lambda_expression<R(Args...)>::value
      , "MPL lambda expression expected as first parameter of mpl::bind"
      );

    typedef typename v_apply<details::resolve_args<R(Args...)>, Params...>::type expr;
    typedef typename eval<expr>::type type;
};

template <typename R, typename... Args, typename... Params>
struct bind_impl<R(*)(Args...), Params...>
{
    static_assert(
        is_lambda_expression<R(Args...)>::value
      , "MPL lambda expression expected as first parameter of mpl::bind"
      );

    typedef typename v_apply<details::resolve_args<R(Args...)>, Params...>::type expr;
    typedef typename eval<expr>::type type;
};

}}}                                                         // namespace details, mpl, zencxx
#endif                                                      // __ZENCXX__MPL__DETAILS__BIND_IMPL_HH__
