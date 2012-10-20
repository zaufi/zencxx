/**
 * \file
 *
 * \brief Class \c zencxx::mpl::details::iter_fold_impl (interface)
 *
 * \date Mon Jul  9 10:59:45 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__DETAILS__ITER_FOLD_IMPL_HH__
# define __ZENCXX__MPL__DETAILS__ITER_FOLD_IMPL_HH__

// Project specific includes
# include <zencxx/mpl/details/resolve_args.hh>
# include <zencxx/mpl/v_apply.hh>

// Standard includes
# include <boost/mpl/next.hpp>

namespace zencxx { namespace mpl { namespace details {

template <
    typename First
  , typename Last
  , typename State
  , typename ForwardOp
  >
struct iter_fold_impl
{
    typedef typename iter_fold_impl<
        typename boost::mpl::next<First>::type
      , Last
      , typename v_apply<resolve_args<ForwardOp>, State, First>::type
      , ForwardOp
      >::type type;
};

template <
    typename Last
  , typename State
  , typename ForwardOp
  >
struct iter_fold_impl<Last, Last, State, ForwardOp>
{
    typedef State type;
};

}}}                                                         // namespace details, mpl, zencxx
#endif                                                      // __ZENCXX__MPL__DETAILS__ITER_FOLD_IMPL_HH__
