/**
 * \file
 *
 * \brief Debug printing for \c std::chrono types
 *
 * \date Thu Jul 18 08:42:27 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__DEBUG__PRINT__STD_CHRONO_HH__
# define __ZENCXX__DEBUG__PRINT__STD_CHRONO_HH__

// Project specific includes
# include <zencxx/debug/print/any_fwd.hh>
# include <zencxx/debug/print/any_manip.hh>
# include <zencxx/debug/print/any_wrapper.hh>
# include <zencxx/details/export.hh>
# include <zencxx/type_traits/is_std_chrono_duration.hh>
# include <zencxx/type_traits/is_std_chrono_time_point.hh>

// Standard includes
# include <ostream>
# include <chrono>

namespace zencxx { namespace debug { namespace print { namespace details {

ZENCXX_EXPORT extern const int s_print_localtime_idx;
ZENCXX_EXPORT extern const int s_time_format_idx;

class time_format_holder
{
    const char* const m_ptr;

public:
    explicit time_format_holder(const char* const fmt) : m_ptr(fmt) {}
    const char* ptr() const
    {
        return m_ptr;
    }
};

/// Make \c time_format_holder streamable
inline std::ostream& operator<<(std::ostream& os, const time_format_holder& fmt)
{
    os.pword(s_time_format_idx) = const_cast<char*>(fmt.ptr());
    return os;
}

template <typename T>
struct any_duration : public any_wrapper<T>
{
    static_assert(
        is_std_chrono_duration<typename std::decay<T>::type>::value
      , "Type T must be an instance of std::chrono::duration"
      );
    using any_wrapper<T>::any_wrapper;
};

/**
 * \brief Make \c std::chrono::system_clock::duration printable w/
 * \c zencxx::debug::print::any I/O manupulator.
 */
ZENCXX_EXPORT std::ostream& operator<<(
    std::ostream&
  , const any_duration<const std::chrono::system_clock::duration&>&
  );

template <typename T>
struct any_time_point : public any_wrapper<T>
{
    static_assert(
        is_std_chrono_time_point<typename std::decay<T>::type>::value
      , "Type T must be an instance of std::chrono::time_point"
      );
    using any_wrapper<T>::any_wrapper;
};

/**
 * \brief Make \c std::chrono::system_clock::time_point printable w/
 * \c zencxx::debug::print::any I/O manupulator.
 */
ZENCXX_EXPORT std::ostream& operator<<(
    std::ostream&
  , const any_time_point<const std::chrono::system_clock::time_point&>&
  );


/**
 * \brief Generic template to print any duration values
 */
template <typename Rep, typename Period>
inline std::ostream& operator<<(
    std::ostream& os
  , const any_duration<const std::chrono::duration<Rep, Period>&>& dw
  )
{
    os << any(std::chrono::duration_cast<std::chrono::system_clock::duration>(dw.data()));
    // Show type info if needed
    details::show_type_info_impl<std::chrono::duration<Rep, Period>>(os);
    return os;
}

}                                                           // namespace details

/**
 * \brief Manipulator to force printing all following time points as localtime
 *
 * Example:
 * \code
 *  namespace print = zencxx::debug::print;
 *  auto now = std::chrono::system_clock::now();
 *  std::cout << print::localtime << print::any(now) << std::endl;
 * \endcode
 */
template<typename CharT, typename Traits>
inline std::basic_ostream<CharT, Traits>& localtime(std::basic_ostream<CharT, Traits>& os)
{
    os.iword(details::s_print_localtime_idx) = 1;
    return os;
}

/**
 * \brief Manipulator to force printing all following time points as gmtime
 *
 * Example:
 * \code
 *  namespace print = zencxx::debug::print;
 *  auto now = std::chrono::system_clock::now();
 *  std::cout << print::gmtime << print::any(now) << std::endl;
 * \endcode
 */
template<typename CharT, typename Traits>
inline std::basic_ostream<CharT, Traits>& gmtime(std::basic_ostream<CharT, Traits>& os)
{
    os.iword(details::s_print_localtime_idx) = 0;
    return os;
}

/**
 * \brief Manipulator to set date/time format
 *
 * Default format is <code>%d/%m/%y %H:%M:%S</code>
 *
 * Example:
 * \code
 *  namespace print = zencxx::debug::print;
 *  auto now = std::chrono::system_clock::now();
 *  std::cout << print::set_format("%Y-%m-%d %H:%M:%S") << print::any(now) << std::endl;
 * \endcode
 */
inline details::time_format_holder set_date_time_format(const char* const fmt)
{
    return details::time_format_holder(fmt);
}

}}}                                                         // namespace print, debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__PRINT__STD_CHRONO_HH__
