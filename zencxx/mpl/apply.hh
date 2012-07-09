/**
 * \file
 *
 * \brief Metafunction to invoke a metafunction class and apply parameters
 *
 * \date Mon Jul  9 05:56:25 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__APPLY_HH__
# define __ZENCXX__MPL__APPLY_HH__

// Project specific includes
# include <zencxx/mpl/has_apply.hh>

// Standard includes

namespace zencxx { namespace mpl {

/**
 * \brief Variadic analog of \c boost::mpl::apply
 *
 * \note Actually here is one difference: it assumes that given
 * template parameter \c F is already a metafunction class and
 * \b must have a nested \c apply template.
 *
 * \tparam F metafunction class to invoke
 * \tparam Args parameters pack to apply
 */
template <typename F, typename... Args>
struct apply
{
    static_assert(
        has_apply<F, Args...>::value
      , "F isn't a metafunction class! Nested apply<> template missed"
      );
    typedef typename F::template apply<Args...>::type type;
};

/**
 * \brief Specialization for nulary metafunction classes
 *
 * \sa \c zencxx::mpl::apply
 */
template <typename F>
struct apply<F>
{
    static_assert(
        has_apply<F>::value
      , "F isn't a metafunction class! Nested apply<> template missed"
      );
    typedef typename F::apply::type type;
};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__APPLY_HH__
