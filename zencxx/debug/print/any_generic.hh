/**
 * \file
 *
 * \brief Generic template of \c zencxx::debug::print::any()
 *
 * \date Thu Jul 25 09:11:45 MSK 2013 -- Initial design
 */
/*
 * Copyright (C) 2010-2013 Alex Turbov and contributors, all rights reserved.
 * This is free software. It is licensed for use, modification and
 * redistribution under the terms of the GNU Lesser General Public License,
 * version 3 or later <http://gnu.org/licenses/lgpl.html>
 *
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

#ifndef __ZENCXX__DEBUG__PRINT__ANY_GENERIC_HH__
# define __ZENCXX__DEBUG__PRINT__ANY_GENERIC_HH__

// Project specific includes
# include <zencxx/debug/print/any_manip.hh>
# include <zencxx/debug/type_name.hh>
# include <zencxx/type_traits/has_left_shift.hh>
# include <zencxx/type_traits/is_range_iterable.hh>

// Standard includes
# include <ostream>
# include <utility>

namespace zencxx { namespace debug { namespace print { namespace details {

template <typename T, bool IsPrintable>
class generic_any_wrapper
{
    const T& m_ref;

public:
    /// \todo Is it really safe to have the non-explicit constructor here?
    generic_any_wrapper(const T& r) : m_ref(r) {}
    const T& ref() const
    {
        return m_ref;
    }
    void print_to(std::ostream& os) const
    {
        os << "<" << type_name<T>() << " is not printable>";
    }
};

template <typename T>
class generic_any_wrapper<T, true> : public generic_any_wrapper<T, false>
{
public:
    /// \todo Is it really safe to have the non-explicit constructor here?
    generic_any_wrapper(const T& r) : generic_any_wrapper<T, false>(r) {}
    void print_to(std::ostream& os) const
    {
        os << this->ref();                                  // Print a value
        details::show_type_info_impl<T>(os);                // Show type info, if needed
    }
};

template <typename T, bool IsPrintable>
inline std::ostream& operator<<(std::ostream& os, generic_any_wrapper<T, IsPrintable>&& wrp)
{
    wrp.print_to(os);
    return os;
}
}                                                           // namespace details

template <typename T>
inline typename std::enable_if<
    !is_range_iterable<T>::value
  , details::generic_any_wrapper<
        T
      , has_left_shift<std::ostream&, const T&>::value
      >
  >::type any(const T& value)
{
    return {value};
}

}}}                                                         // namespace print, debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__PRINT__ANY_GENERIC_HH__
