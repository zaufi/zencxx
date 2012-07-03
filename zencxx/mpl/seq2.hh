/**
 * \file
 *
 * \brief Limitless replacement for \c boost::mpl::seq
 *
 * \date Thu Jun 28 23:14:49 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__VECTOR_HH__
# define __ZENCXX__MPL__VECTOR_HH__

// Project specific includes
# include <zencxx/mpl/details/seq.hh>
# include <zencxx/mpl/details/seq_iter.hh>

// Standard includes
# include <boost/mpl/at_fwd.hpp>
# include <boost/mpl/advance_fwd.hpp>
# include <boost/mpl/back_fwd.hpp>
# include <boost/mpl/begin_end_fwd.hpp>
# include <boost/mpl/clear_fwd.hpp>
# include <boost/mpl/deref.hpp>
# include <boost/mpl/distance_fwd.hpp>
# include <boost/mpl/front_fwd.hpp>
# include <boost/mpl/iterator_tags.hpp>
# include <boost/mpl/long.hpp>
# include <boost/mpl/next.hpp>
# include <boost/mpl/pop_back_fwd.hpp>
# include <boost/mpl/pop_front_fwd.hpp>
# include <boost/mpl/prior.hpp>
# include <boost/mpl/push_back_fwd.hpp>
# include <boost/mpl/push_front_fwd.hpp>
# include <boost/mpl/size_fwd.hpp>

namespace boost { namespace mpl {
/**
 * \c boost::mpl::begin specialization
 */
template <typename... Types>
struct begin<zencxx::mpl::seq<Types...>>
{
  typedef zencxx::mpl::details::seq_iter<
      boost::mpl::long_<0>
    , zencxx::mpl::seq<Types...>
    > type;
};
/**
 * \c boost::mpl::end specialization
 */
template <typename... Types>
struct end<zencxx::mpl::seq<Types...>>
{
  typedef zencxx::mpl::details::seq_iter<
      typename boost::mpl::long_<sizeof...(Types)>::type
    , zencxx::mpl::seq<Types...>
    > type;
};
/**
 * \c boost::mpl::next specialization
 */
template <typename Pos, typename Seq>
struct next<zencxx::mpl::details::seq_iter<Pos, Seq>>
{
  typedef zencxx::mpl::details::seq_iter<typename next<Pos>::type, Seq> type;
};
/**
 * \c boost::mpl::prior specialization
 */
template <typename Pos, typename Seq>
struct prior<zencxx::mpl::details::seq_iter<Pos, Seq>>
{
  typedef zencxx::mpl::details::seq_iter<typename prior<Pos>::type, Seq> type;
};
/**
 * \c boost::mpl::deref specialization
 */
template <typename Pos, typename Seq>
struct deref<zencxx::mpl::details::seq_iter<Pos, Seq>>
{
  typedef typename zencxx::mpl::details::seq_iter<Pos, Seq>::type type;
};
/**
 * \c boost::mpl::size specialization
 *
 * Default implementation will call \c boost::mpl::distance between
 * begin and end iterators, that woule lead to lot of instantiations and better
 * to avoid them using this specialization which calculates size much more faster.
 */
template <typename... Types>
struct size<zencxx::mpl::seq<Types...>>
{
  typedef long_<sizeof...(Types)> type;
};
/**
 * \c boost::mpl::advance specialization
 */
template <typename Pos, typename N, typename Seq>
struct advance<zencxx::mpl::details::seq_iter<Pos, Seq>, N>
{
  enum
  {
      pos = Pos::value
    , n = N::value
  };
  typedef zencxx::mpl::details::seq_iter<long_<pos + n>, Seq> type;
};
/**
 * \c boost::mpl::distance specialization
 */
template <typename First, typename Last, typename Seq>
struct distance<
    zencxx::mpl::details::seq_iter<First, Seq>
  , zencxx::mpl::details::seq_iter<Last, Seq>
  >
{
  enum
  {
      pos1 = First::value
    , pos2 = Last::value
  };
  typedef long_<pos2 - pos1> type;
};
/**
 * \c boost::mpl::push_back specialization
 */
template <typename... Types, typename T>
struct push_back<zencxx::mpl::seq<Types...>, T>
{
  typedef zencxx::mpl::seq<Types..., T> type;
};
/**
 * \c boost::mpl::push_front specialization
 */
template <typename... Types, typename T>
struct push_front<zencxx::mpl::seq<Types...>, T>
{
  typedef zencxx::mpl::seq<T, Types...> type;
};
/**
 * \c boost::mpl::pop_back specialization
 */
template <typename... Types, typename T>
struct pop_back<zencxx::mpl::seq<Types..., T>>
{
  typedef zencxx::mpl::seq<Types...> type;
};
template <typename T>
struct pop_back<zencxx::mpl::seq<T>>
{
  typedef zencxx::mpl::seq<> type;
};
template <>
struct pop_back_impl<zencxx::mpl::details::variadic_sequence_tag>
{
    template <typename Seq>
    struct apply
    {
        typedef float type;
    };
};
/**
 * \c boost::mpl::pop_front specialization
 */
template <typename... Types, typename T>
struct pop_front<zencxx::mpl::seq<T, Types...>>
{
  typedef zencxx::mpl::seq<Types...> type;
};
/**
 * \c boost::mpl::back specialization
 */
template <typename... Types, typename T>
struct back<zencxx::mpl::seq<Types..., T>>
{
  typedef T type;
};
/**
 * \c boost::mpl::back specialization
 */
template <typename T>
struct back<zencxx::mpl::seq<T>>
{
  typedef T type;
};
/**
 * \c boost::mpl::front specialization
 */
template <typename T, typename... Types>
struct front<zencxx::mpl::seq<T, Types...>>
{
  typedef T type;
};
/**
 * \c boost::mpl::clear specialization
 */
template <typename... Types>
struct clear<zencxx::mpl::seq<Types...>>
{
  typedef zencxx::mpl::seq<> type;
};
#if 0
/**
 * \c boost::mpl::at specialization
 */
template <typename... Types, typename N>
struct at<zencxx::mpl::seq<Types...>, N>
{
  typedef typename zencxx::mpl::details::get_at<
      N
    , boost::mpl::long_<0>
    , zencxx::mpl::seq<Types...>
    >::type type;
};
#endif
}}                                                          // namespace mpl, boost
#endif                                                      // __ZENCXX__MPL__VECTOR_HH__
