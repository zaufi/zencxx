/**
 * \file
 *
 * \brief \c zencxx::mpl::has_type metafunction
 *
 * \date Wed Jul  4 05:07:08 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__HAS_TYPE_HH__
# define __ZENCXX__MPL__HAS_TYPE_HH__

// Project specific includes

// Standard includes
# include <boost/mpl/has_xxx.hpp>

namespace zencxx { namespace mpl {
#if 1
/**
 * \attention Nowadays (boost <= 1.50) \c has_type metafunction placed in the
 * \c boost::mpl::aux, so we can't use it here (yep, it's undocumented
 * implementation details), so we have to define our own... yep, just
 * copy'n'paste it :)
 */
BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_type, type, true)
#else
/**
 * \brief Class \c has_type
 */
template <typename T>
class has_type
{
    typedef char yes_type;
    typedef char (&no_type)[2];

    struct checker
    {
        template <typename U>
        static no_type test(...);

        template <typename U>
        static yes_type test(typename std::add_pointer<typename U::type>::type);
    };

public:
    constexpr static bool value = std::is_same<
        decltype(checker::template test<T>(0))
      , yes_type
      >::value;
    typedef std::integral_constant<bool, value> type;
};
#endif

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__HAS_TYPE_HH__
