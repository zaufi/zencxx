/**
 * \file
 *
 * \brief \c empty implementation for variadic \c seq
 *
 * \date Mon Jul  2 14:59:32 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__DETAILS__SEQ__EMPTY_HH__
# define __ZENCXX__MPL__DETAILS__SEQ__EMPTY_HH__

// Project specific includes
# include <zencxx/mpl/details/variadic_sequence_tag.hh>
# include <zencxx/mpl/seq_fwd.hh>

// Standard includes
# include <boost/mpl/empty.hpp>

namespace boost { namespace mpl {

/**
 * \brief \c boost::mpl::empty specialization
 *
 * \note Generic metaclass has no implementation
 */
template <>
struct empty_impl<zencxx::mpl::details::variadic_sequence_tag>
{
    template <typename Seq>
    struct apply;
};

/**
 * \brief \c boost::mpl::empty specialization for non empty sequence
 */
template <>
template <typename... Types>
struct empty_impl<zencxx::mpl::details::variadic_sequence_tag>::apply<zencxx::mpl::seq<Types...>>
  : public boost::mpl::false_
{};

/**
 * \brief \c boost::mpl::empty specialization for empty sequence
 */
template <>
template <>
struct empty_impl<zencxx::mpl::details::variadic_sequence_tag>::apply<zencxx::mpl::seq<>>
  : public boost::mpl::true_
{};

}}                                                          // namespace mpl, boost
#endif                                                      // __ZENCXX__MPL__DETAILS__SEQ__EMPTY_HH__
