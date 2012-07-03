/**
 * \file
 *
 * \brief \c back and \c front implementation for variadic \c seq
 *
 * \date Mon Jul  2 15:43:06 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__DETAILS__SEQ__BACK_FRONT_HH__
# define __ZENCXX__MPL__DETAILS__SEQ__BACK_FRONT_HH__

// Project specific includes
# include <zencxx/mpl/seq_fwd.hh>

// Standard includes
# include <boost/mpl/back_fwd.hpp>
# include <boost/mpl/front_fwd.hpp>

namespace boost { namespace mpl {
#if 1
/**
 * \c boost::mpl::back specialization
 *
 * \todo I have no idea why \c back_impl (and \c pop_back_impl) specialization
 * can't be found by gcc 4.7.1 :( But this one works! Isn't it shouldn't?
 */
template <typename... Types, typename T>
struct back<zencxx::mpl::seq<Types..., T>>
{
    typedef T type;
};
#else
/**
 * \brief \c boost::mpl::back specialization
 *
 * \note Generic metaclass has no implementation
 */
template <>
struct back_impl<zencxx::mpl::details::variadic_sequence_tag>
{
    template <typename Seq>
    struct apply;
};

/**
 * \brief \c boost::mpl::back specialization for \c zencxx::mpl::seq
 */
template <>
template <typename... Types, typename T>
struct back_impl<zencxx::mpl::details::variadic_sequence_tag>::apply<zencxx::mpl::seq<Types..., T>>
{
    typedef T type;
};
#endif

/**
 * \brief \c boost::mpl::front specialization
 *
 * \note Generic metaclass has no implementation
 */
template <>
struct front_impl<zencxx::mpl::details::variadic_sequence_tag>
{
    template <typename Seq>
    struct apply;
};

/**
 * \brief \c boost::mpl::front specialization for \c zencxx::mpl::seq
 */
template <>
template <typename T, typename... Types>
struct front_impl<zencxx::mpl::details::variadic_sequence_tag>::apply<zencxx::mpl::seq<T, Types...>>
{
    typedef T type;
};

}}                                                          // namespace mpl, boost
#endif                                                      // __ZENCXX__MPL__DETAILS__SEQ__BACK_FRONT_HH__
