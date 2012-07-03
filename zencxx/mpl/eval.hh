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

// Standard includes

namespace zencxx { namespace mpl {

template <typename Expr>
struct eval
{
    typedef Expr type;
};

template <typename R, typename... Args>
struct eval<R(Args...)>
{
    typedef typename R::template apply<typename eval<Args>::type...>::type type;
};

template <typename R, typename... Args>
struct eval<R(*)(Args...)>
{
    typedef typename R::template apply<typename eval<Args>::type...>::type type;
};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__EVAL_HH__
