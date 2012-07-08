/**
 * \file
 *
 * \brief Class \c zencxx::mpl::eval (interface)
 *
 * \date Sat Jun 30 17:49:07 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__EVAL_HH__
# define __ZENCXX__MPL__EVAL_HH__

// Project specific includes
# include <zencxx/mpl/details/eval_impl.hh>

// Standard includes

namespace zencxx { namespace mpl {
/**
 * \page ctexpr Compile-time expressions
 *
 * There are few types of <em>compile-time expressions</em> possible:
 * - <tt>fn(param1[, ...[, paramN]])</tt> -- call to metafunction class w/ given parameter list.
 * Being a metafunction class, \c fn must have a nested \c apply metafunction.
 * Example:
 * \code
 *  typedef seq<char, short, int, long, long long> integrals;
 *  using arithmetic = join(integrals, make_seq(float, double, long double));
 * \endcode
 *
 * To evaluate an expression, one have to "call" the \c eval metafunction like this:
 * \code
 *  using result = eval<expression>::type;
 * \endcode
 *
 * It is possible to create a lambda expressions w/ usual syntax of placeholders:
 * \code
 *  using append_if_absent = eval_if(is_same(find_if(_1, _2), end(_1)), push_back(_1, _2), _1);
 * \endcode
 *
 * ... or even smth like this:
 * \code
 *  using binrary_action = _1(_2, _3);
 * \endcode
 *
 * After construction one may bind parameters:
 * \code
 *  using expr = bind(append_if_absent, integrals, __int128);
 *  using result = eval<expr>::type;
 *  // result is `seq<char, short, int, long, long long, __int128>'
 * \endcode
 *
 * \todo to make a partial application expression:
 * \code
 *  using expr = bind(binrary_action, pair<_2, integrals>, pair<_3, __int128>);
 *  using result = eval<expr>::type;
 *  // result is `seq<char, short, int, long, long long, __int128>'
 * \endcode
 *
 * \todo TBD
 *
 * \p notes Specializations for function pointers
 *
 * If u have a <em>compile-time expression</em> w/ nested "calls" some of parameters
 * will be passed as function pointers (due conversion). In fact only outer "call"
 * will be a function type. So library require specializations to handle inner "calls"
 * correctly.
 */

/**
 * \brief Primary template to evaluate <em>compile-time expressions</em>
 *
 * Use tag based dispatching to delegate the job to an appropriate specialization.
 *
 */
template <typename Expr>
struct eval : eval_impl<
    typename expression_tag<Expr>::type
  >::template apply<Expr>
{
};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__EVAL_HH__
