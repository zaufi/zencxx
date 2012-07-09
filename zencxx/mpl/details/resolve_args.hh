/**
 * \file
 *
 * \brief Metafunction \c zencxx::mpl::details::resolve_args
 *
 * \date Mon Jul  9 05:49:23 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__DETAILS__RESOLVE_ARGS_HH__
# define __ZENCXX__MPL__DETAILS__RESOLVE_ARGS_HH__

// Project specific includes
# include <zencxx/mpl/arg.hh>
# include <zencxx/mpl/apply.hh>

// Standard includes
# include <boost/mpl/identity.hpp>

namespace zencxx { namespace mpl { namespace details {

template <typename R, typename... Args>
struct make_function_pointer
{
    using type = R(*)(Args...);
};

template <typename T>
struct resolve_args
{
    template <typename... Params>
    struct apply : boost::mpl::identity<T>
    {};
};

template <int N>
struct resolve_args<arg<N>>
{
    template <typename... Params>
    struct apply : mpl::apply<arg<N>, Params...>
    {};
};

template <typename R, typename... Args>
struct resolve_args<R(Args...)>
{
    template <typename... Params>
    struct apply
      : make_function_pointer<
            typename mpl::apply<resolve_args<R>, Params...>::type
          , typename mpl::apply<resolve_args<Args>, Params...>::type...
          >
    {};
};

template <typename R, typename... Args>
struct resolve_args<R(*)(Args...)>
{
    template <typename... Params>
    struct apply
      : make_function_pointer<
            typename mpl::apply<resolve_args<R>, Params...>::type
          , typename mpl::apply<resolve_args<Args>, Params...>::type...
          >
    {};
};

}}}                                                         // namespace details, mpl, zencxx
#endif                                                      // __ZENCXX__MPL__DETAILS__RESOLVE_ARGS_HH__
