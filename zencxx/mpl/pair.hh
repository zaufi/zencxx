/**
 * \file
 *
 * \brief Class \c zencxx::mpl::pair (interface)
 *
 * \date Sun Jul  8 20:43:05 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__PAIR_HH__
# define __ZENCXX__MPL__PAIR_HH__

// Project specific includes

// Standard includes
# include <boost/mpl/pair.hpp>
# include <boost/mpl/quote.hpp>

namespace zencxx { namespace mpl {

/**
 * \brief Metafunction class to produce MPL pairs
 */
struct pair
{
    template <typename T1, typename T2>
    struct apply
    {
        typedef boost::mpl::pair<T1, T2> type;
    };
};

/// Metafunction class to access first element of pair
typedef boost::mpl::quote1<boost::mpl::first> first;

/// Metafunction class to access second element of pair
typedef boost::mpl::quote1<boost::mpl::second> second;

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__PAIR_HH__
