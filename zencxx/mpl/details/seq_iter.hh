/**
 * \file
 *
 * \brief Class \c zencxx::mpl::details::seq_iter (interface)
 *
 * \date Mon Jul  2 13:45:22 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__DETAILS__SEQ_ITER_HH__
# define __ZENCXX__MPL__DETAILS__SEQ_ITER_HH__

// Project specific includes
# include <zencxx/mpl/is_seq.hh>
# include <zencxx/mpl/v_at.hh>

// Standard includes
# include <boost/mpl/iterator_tags.hpp>
# include <boost/mpl/next.hpp>
# include <boost/mpl/prior.hpp>

namespace zencxx { namespace mpl { namespace details {

// Generic template (nothing 2 do)
template <typename Pos, typename Seq>
struct item_at;

// Use variadic `at' to get an item
template <typename Pos, typename... Types>
struct item_at<Pos, seq<Types...>> : v_at<Pos, Types...>
{};

/**
 * \brief Generic template for variadic sequence iterator
 *
 * \note It actually do nothing 'cept report an error in compile time,
 * cuz all work done in specializations...
 */
template <typename Pos, typename Seq>
struct seq_iter
{
    static_assert(is_seq<Seq>::type::value, "Sequence type must be a zencxx::mpl::seq<...>");
};

/**
 * \brief <em>Bidirectional iterator</em> concept implementation
 *
 * ... iterator to the middle of non empty sequence
 */
template <typename Pos, typename... Types>
struct seq_iter<Pos, seq<Types...>>
{
    typedef boost::mpl::bidirectional_iterator_tag category;
    typedef Pos position;
    typedef seq<Types...> sequence_type;

    /// \name next/prior implementation details
    //@{
    typedef seq_iter<typename boost::mpl::next<position>::type, sequence_type> next;
    typedef seq_iter<typename boost::mpl::prior<position>::type, sequence_type> prior;
    //@}

    /// \c deref implementation
    typedef typename item_at<position, sequence_type>::type type;
};

/**
 * \brief Specialization for \c begin<Seq> iterator
 *
 * ... iterator to the begin of non-empty sequence
 *
 * \note No \c prior type here
 */
template <typename... Types>
struct seq_iter<boost::mpl::long_<0>, seq<Types...>>
{
    typedef boost::mpl::bidirectional_iterator_tag category;
    typedef boost::mpl::long_<0> position;
    typedef seq<Types...> sequence_type;

    /// \c next implementation
    typedef seq_iter<typename boost::mpl::next<position>::type, sequence_type> next;

    /// \c deref implementation
    typedef typename item_at<position, sequence_type>::type type;
};

/**
 * \brief Specialization for \c end<Seq> iterator
 *
 * ... iterator to the end of non-empty sequence
 *
 * \note No \c next and \c type type here
 */
template <typename... Types>
struct seq_iter<typename seq<Types...>::size, seq<Types...>>
{
    typedef boost::mpl::bidirectional_iterator_tag category;
    typedef seq<Types...> sequence_type;
    typedef typename sequence_type::size position;

    /// \c prior implementation
    typedef seq_iter<typename boost::mpl::prior<position>::type, sequence_type> prior;
};

/**
 * \brief Specialization for begin/end iterator of an empty sequence
 *
 * \note No \c next, \c prior and \c type type here
 */
template <>
struct seq_iter<boost::mpl::long_<0>, seq<>>
{
    typedef boost::mpl::bidirectional_iterator_tag category;
    typedef seq<> sequence_type;
    typedef boost::mpl::long_<0> position;
};

}}}                                                         // namespace details, mpl, zencxx
#endif                                                      // __ZENCXX__MPL__DETAILS__SEQ_ITER_HH__
