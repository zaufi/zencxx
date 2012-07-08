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
# include <boost/mpl/bool.hpp>
# include <boost/mpl/eval_if.hpp>

namespace zencxx { namespace mpl {

/**
 * \brief True variadic analog of \c boost::mpl::and_
 *
 * Generic template accept at least two parameters
 */
template <typename T, typename... Tail>
struct v_and : boost::mpl::eval_if<T, v_and<Tail...>, boost::mpl::false_>
{
};

/**
 * \brief True variadic analog of \c boost::mpl::and_
 *
 * Single parameter specialization
 */
template <typename T>
struct v_and<> : boost::bool_<T::type::value>
{
};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__V_AND_HH__
