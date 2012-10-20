/**
 * \file
 *
 * \brief Variadic metafunction to calculate logic AND expression
 *
 * \date Sun Jul  8 06:36:29 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__V_AND_HH__
# define __ZENCXX__MPL__V_AND_HH__

// Project specific includes

// Standard includes
# include <boost/mpl/eval_if.hpp>

namespace zencxx { namespace mpl {

/**
 * \brief True variadic analog of \c boost::mpl::and_
 *
 * Generic template isn't defined
 */
template <typename...>
struct v_and;

/**
 * \brief True variadic analog of \c boost::mpl::and_
 *
 * Specialization to accept at least two parameters
 */
template <typename T1, typename T2, typename... Tail>
struct v_and<T1, T2, Tail...>
  : boost::mpl::eval_if<
      T1
    , v_and<T2, Tail...>
    , boost::mpl::false_
    >::type
{
};

/**
 * \brief True variadic analog of \c boost::mpl::and_
 *
 * Single parameter specialization
 */
template <typename T>
struct v_and<T> : boost::mpl::bool_<T::type::value>
{
};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__V_AND_HH__
