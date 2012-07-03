/**
 * \file
 *
 * \brief Metafunction \c zencxx::mpl::copy_range
 *
 * \date Tue Jul  3 17:59:37 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__COPY_RANGE_HH__
# define __ZENCXX__MPL__COPY_RANGE_HH__

// Project specific includes
# include <zencxx/mpl/copy_range_fwd.hh>

// Standard includes
# include <boost/mpl/long.hpp>
# include <boost/mpl/sequence_tag.hpp>

namespace zencxx { namespace mpl {

/**
 * \brief Metafunction to copy range of items from one compile-time sequence to another
 *
 * Default tag dispatching based implementation.
 *
 * \tparam Seq initial sequence to copy items from
 * \tparam First index of first element to copy
 * \tparam Last index of the first element when to stop copying
 * \tparam Inserter MPL analog of an output iterator in STL (see boost MPL docs)
 *
 * Example:
 * \code
 *  typedef seq<char, short, int, long> src_t;
 *  typedef copy_range<src_t, long_<1>, long_<3>>::type dst_t;
 *  static_assert(std::is_same<dst_t, seq<short, int>>::value, "Failed");
 * \endcode
 */
template <
    typename Seq
  , typename First
  , typename Last
  , typename Inserter
  >
struct copy_range
  : public copy_range_impl<
      typename boost::mpl::sequence_tag<Seq>::type
    >::template apply<Seq, First, Last, Inserter>
{
};

/**
 * \brief Little more convenient version of the \c copy_range algorithm
 */
template <
    typename Seq
  , long First
  , long Last
  , typename Inserter = boost::mpl::back_inserter<seq<>>
  >
struct copy_range_c
  : public copy_range<
      Seq
    , boost::mpl::long_<First>
    , boost::mpl::long_<Last>
    , Inserter
    >
{
};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__COPY_RANGE_HH__
