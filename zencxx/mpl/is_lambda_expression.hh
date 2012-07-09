/**
 * \file
 *
 * \brief Metafunction to check if given expression is a lambda one
 *
 * \date Wed Jul  4 07:04:54 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__IS_LAMBDA_EXPRESSION_HH__
# define __ZENCXX__MPL__IS_LAMBDA_EXPRESSION_HH__

// Project specific includes
# include <zencxx/mpl/arg.hh>
# include <zencxx/mpl/v_or.hh>
# include <zencxx/mpl/placeholders.hh>

// Standard includes
# include <boost/mpl/bool.hpp>

namespace zencxx { namespace mpl {

/**
 * \brief Metafunction to check if given expression is a lambda one
 */
template <typename Expr>
struct is_lambda_expression : public boost::mpl::false_
{};

template <int N>
struct is_lambda_expression<arg<N>> : public boost::mpl::true_
{};

template <typename R, typename... Args>
struct is_lambda_expression<R(Args...)>
  : v_or<is_lambda_expression<R>, is_lambda_expression<Args>...>
{
};

template <typename R, typename... Args>
struct is_lambda_expression<R(*)(Args...)>
  : v_or<is_lambda_expression<R>, is_lambda_expression<Args>...>
{
};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__IS_LAMBDA_EXPRESSION_HH__
