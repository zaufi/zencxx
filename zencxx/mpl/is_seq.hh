/**
 * \file
 *
 * \brief Class \c zencxx::mpl::is_seq (interface)
 *
 * \date Tue Jul  3 04:40:36 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__IS_SEQ_HH__
# define __ZENCXX__MPL__IS_SEQ_HH__

// Project specific includes
# include <zencxx/mpl/seq_fwd.hh>

// Standard includes
# include <boost/mpl/bool.hpp>

namespace zencxx { namespace mpl {

/**
 * \brief Metafunction to check is type \c T is a \c mpl::seq
 *
 * ... generic (\c false) case.
 */
template <typename T>
struct is_seq : public boost::mpl::false_ {};

/**
 * \brief Metafunction to check is type \c T is a \c mpl::seq
 *
 * ... \c true specialization.
 */
template <typename... Types>
struct is_seq<seq<Types...>> : public boost::mpl::true_ {};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__IS_SEQ_HH__
