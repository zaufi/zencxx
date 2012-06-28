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

// Standard includes
# include <boost/mpl/bool.hpp>

namespace zencxx { namespace details {
struct has_left_shift_tag {};

struct has_left_shift_any
{
    template <typename T>
    has_left_shift_any(const T&);
};

has_left_shift_tag operator<<(const has_left_shift_any&, const has_left_shift_any&);

template <typename L, typename R>
struct has_left_shift_impl
{
  typedef char no_type;
  struct yes_type { char m_padding[8]; };

  static no_type check(has_left_shift_tag);
  template <typename... T>
  static yes_type check(T&&...);

  static L* l;
  static R* r;

  constexpr static bool value = sizeof(yes_type) == sizeof(check(*l << *r));
  typedef boost::mpl::bool_<value> type;
};

template <typename L, typename R>
constexpr bool has_left_shift_impl<L, R>::value;
}                                                           // namespace details

/**
 * \brief Metafunction to determine is \c operator<< available for given types
 */
template <typename L, typename R>
struct has_left_shift : public details::has_left_shift_impl<L, R> {};

}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__TYPE_TRAITS__HAS_LEFT_SHIFT_HH__
