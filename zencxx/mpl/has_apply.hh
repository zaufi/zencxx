/**
 * \file
 *
 * \brief \c zencxx::mpl::has_apply metafunction
 *
 * \date Wed Jul  4 05:39:08 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__HAS_APPLY_HH__
# define __ZENCXX__MPL__HAS_APPLY_HH__

// Project specific includes
# include <zencxx/type_traits/details/yes_no_type.hh>

// Standard includes
# include <boost/mpl/has_xxx.hpp>
# include <type_traits>

namespace zencxx { namespace mpl { namespace details {
/**
 * Define \c has_apply as it defined in boost internals
 * It will be used to detect nulary apply (it can't do anything else!)
 */
BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_apply, apply, false)
}                                                           // namespace details

/**
 * \brief Metafunction to check presence of apply template w/ given args inside
 * of given type \c T.
 *
 * In other words, this metafunction check that given type \c T is a metafunction
 * class...
 *
 * \attention Nowadays (boost <= 1.50) \c has_apply metafunction placed in the
 * \c boost::mpl::aux, so we can't use it here (yep, it's undocumented
 * implementation details)... And, anyways, it really suxx! It's unable to
 * detect templated \c apply :( So we it would be better to rewrite it!
 */
template <typename T, typename... Args>
class has_apply
{
    struct checker
    {
        template <typename U, typename... Params>
        static zencxx::type_traits::details::no_type test(...);

        template <typename U, typename... Params>
        static zencxx::type_traits::details::yes_type test(
            typename std::add_pointer<
                typename U::template apply<Params...>::type
              >::type
          );
    };

public:
    constexpr static bool value = std::is_same<
        decltype(checker::template test<T, Args...>(0))
      , zencxx::type_traits::details::yes_type
      >::value;
    typedef std::integral_constant<bool, value> type;
};

/**
 * \brief Specialization for nulary \c apply
 */
template <typename T>
class has_apply<T> : public details::has_apply<T>
{};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__HAS_APPLY_HH__
