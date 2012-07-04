/**
 * \file
 *
 * \brief Class \c zencxx::has_left_shift (interface)
 *
 * \date Wed Jun 27 02:11:39 MSK 2012 -- Initial design
 */
/*
 * ZenCxx is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libzencxx is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

#ifndef __ZENCXX__TYPE_TRAITS__HAS_LEFT_SHIFT_HH__
# define __ZENCXX__TYPE_TRAITS__HAS_LEFT_SHIFT_HH__

// Project specific includes
# include <zencxx/type_traits/details/expression_validity_checker.hh>

// Standard includes

namespace zencxx {
ZEN_TT_EXPR_CHECKER(has_left_shift, (typename L, typename R), (L, R), (std::declval<L>() << std::declval<R>()));

#if 0
template <typename L, typename R>
class has_left_shift
{
    typedef char yes_type;
    typedef char (&no_type)[2];

    struct checker
    {
        template <typename NL, typename NR>
        static no_type test(...);

        template <typename NL, typename NR>
        static yes_type test(decltype(std::declval<NL>() << std::declval<NR>(), void())*);
    };

public:
    constexpr static bool value = std::is_same<
        decltype(checker::template test<L, R>(0))
      , yes_type
      >::value;
    typedef std::integral_constant<bool, value> type;
};
#endif

}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__TYPE_TRAITS__HAS_LEFT_SHIFT_HH__
