/**
 * \file
 *
 * \brief Class \c zencxx::mpl::bind (interface)
 *
 * \date Sun Jul  8 20:33:35 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__BIND_HH__
# define __ZENCXX__MPL__BIND_HH__

// Project specific includes
# include <zencxx/mpl/details/bind_impl.hh>

// Standard includes

namespace zencxx { namespace mpl {

/**
 * \brief Metafunction class to produce argument binding expressions
 *
 * \c bind can be used for function composition and/or argument binding.
 * There is two forms of \c bind calls possible:
 * \li to make a lambda expression:
 * \code
 *  using push_expr = bind(push_back, _1, _2);
 * \endcode
 */
struct bind
{
    template <typename F, typename... Params>
    struct apply : details::bind_impl<F, Params...>
    {};
};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__BIND_HH__
