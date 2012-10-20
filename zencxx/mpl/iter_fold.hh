/**
 * \file
 *
 * \brief Class \c zencxx::mpl::iter_fold (interface)
 *
 * \date Mon Jul  9 07:43:34 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__ITER_FOLD_HH__
# define __ZENCXX__MPL__ITER_FOLD_HH__

// Project specific includes
# include <zencxx/mpl/details/iter_fold_impl.hh>
# include <zencxx/mpl/eval.hh>
# include <zencxx/mpl/bind.hh>

// Standard includes
# include <boost/mpl/begin.hpp>
# include <boost/mpl/end.hpp>
# include <boost/mpl/quote.hpp>
# include <boost/mpl/deref.hpp>

namespace zencxx { namespace mpl {

/**
 * \brief Metafunction class to iterate over sequence
 *
 * Returns the result of the successive application of binary \c ForwardOp
 * to the result of the previous \c ForwardOp invocation (\c State if it's the
 * first call) and each iterator in the range <tt>[begin<Sequence>::type,
 * end<Sequence>::type)</tt> in order.
 *
 * \note We can't just use \c boost::mpl::quote to produce metafucntion class
 * just like it has been done for other metafucntions, cuz result must be \e evaluated.
 */
struct iter_fold
{
    template <typename Seq, typename State, typename ForwardOp>
    struct apply
      : eval<
            typename details::iter_fold_impl<
                typename boost::mpl::begin<Seq>::type
              , typename boost::mpl::end<Seq>::type
              , State
              , ForwardOp
              >::type
          >
    {};
};

#if 0
typedef boost::mpl::quote1<boost::mpl::deref> deref;

struct fold
{
    template <typename Seq, typename State, typename ForwardOp>
    struct apply
      : eval<
            typename details::iter_fold_impl<
                typename boost::mpl::begin<Seq>::type
              , typename boost::mpl::end<Seq>::type
              , State
              , bind(ForwardOp, _1, deref(_2))
              >::type
          >
    {};
};
#endif

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__ITER_FOLD_HH__
