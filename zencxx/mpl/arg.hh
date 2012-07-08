/**
 * \file
 *
 * \brief Class \c zencxx::mpl::arg (interface)
 *
 * \date Sun Jul  8 06:10:48 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__ARG_HH__
# define __ZENCXX__MPL__ARG_HH__

// Project specific includes
# include <zencxx/mpl/v_at.hh>

// Standard includes

namespace zencxx { namespace mpl {

/**
 * \brief [Type brief class description here]
 *
 * [More detailed description here]
 *
 */
template <int N>
struct arg
{
    template <typename... Args>
    struct apply : v_at_c<N, Args...>
    {};
};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__ARG_HH__
