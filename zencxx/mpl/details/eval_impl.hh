/**
 * \file
 *
 * \brief Class \c zencxx::mpl::details::eval_impl (interface)
 *
 * \date Wed Jul  4 06:34:23 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__DETAILS__EVAL_IMPL_HH__
# define __ZENCXX__MPL__DETAILS__EVAL_IMPL_HH__

// Project specific includes
# include <zencxx/mpl/eval_fwd.hh>
# include <zencxx/mpl/expression_tags.hh>

// Standard includes

namespace zencxx { namespace mpl {

template <>
struct eval_impl<call_meta_function_class_tag>
{
    template <typename Expr>
    struct apply;
};

template <>
template <typename R, typename... Args>
struct eval_impl<call_meta_function_class_tag>::apply<R(Args...)>
{
    typedef typename R::template apply<typename eval<Args>::type...>::type type;
};

template <>
template <typename R, typename... Args>
struct eval_impl<call_meta_function_class_tag>::apply<R(*)(Args...)>
{
    typedef typename R::template apply<typename eval<Args>::type...>::type type;
};


template <>
struct eval_impl<lambda_expression_tag>
{
    template <typename...>
    struct apply;
};

template <>
template <typename R, typename... Args, typename... Params>
struct eval_impl<lambda_expression_tag>::apply<R(Args...), Params...>
  : R::template apply<Args..., Params...>
{
};

template <>
template <typename R, typename... Args, typename... Params>
struct eval_impl<lambda_expression_tag>::apply<R(*)(Args...), Params...>
  : R::template apply<Args..., Params...>
{
};


template <>
struct eval_impl<ordinal_type_tag>
{
    template <typename Expr>
    struct apply
    {
        typedef Expr type;
    };
};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__DETAILS__EVAL_IMPL_HH__
