/**
 * \file
 *
 * \brief Define MPL placeholer symbols
 *
 * \date Sun Jul  8 06:59:21 MSK 2012 -- Initial design
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

#if !BOOST_PP_IS_ITERATING
#  ifndef __ZENCXX__MPL__PLACEHOLDERS_HH__
#    define __ZENCXX__MPL__PLACEHOLDERS_HH__

// Project specific includes
#    include <zencxx/mpl/arg.hh>

// Standard includes
#    include <boost/preprocessor/iterate.hpp>
namespace zencxx { namespace mpl {
typedef arg<-1> _;
}}                                                          // namespace mpl, zencxx
// Specify iteration params
#    ifndef ZENCXX_MPL_MAX_ARGS
#      define ZENCXX_MPL_MAX_ARGS 5
#    endif
#    define BOOST_PP_ITERATION_PARAMS_1 (3, (1, ZENCXX_MPL_MAX_ARGS, <zencxx/mpl/placeholders.hh>))
// Include self...
#    include BOOST_PP_ITERATE()
#    undef BOOST_PP_ITERATION_PARAMS_1
#  endif                                                    // __ZENCXX__MPL__PLACEHOLDERS_HH__
#else                                                       // !BOOST_PP_IS_ITERATING
#  define N BOOST_PP_ITERATION()
namespace zencxx { namespace mpl {
typedef arg<N> BOOST_PP_CAT(_, N);
}}                                                          // namespace mpl, zencxx
#  undef N
#endif                                                      // BOOST_PP_IS_ITERATING
