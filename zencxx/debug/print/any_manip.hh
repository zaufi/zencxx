/**
 * \file
 *
 * \brief Debug printing manipulators
 *
 * \date Mon Jul 22 18:39:16 MSK 2013 -- Initial design
 */
/*
 * Copyright (C) 2010-2014 Alex Turbov and contributors, all rights reserved.
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

#pragma once

// Project specific includes
#include <zencxx/details/export.hh>
#include <zencxx/debug/type_name.hh>

// Standard includes
#include <boost/io/ios_state.hpp>
#include <ostream>

namespace zencxx { namespace debug { namespace print { namespace details {
ZENCXX_EXPORT extern const int s_show_type_idx;

class show_type_info_saver : public boost::io::ios_iword_saver
{
    using boost::io::ios_iword_saver::state_type;

public:
    show_type_info_saver(state_type& s)
      : boost::io::ios_iword_saver(s, s_show_type_idx)
    {}
};

/// \internal Show type info depending on manipulator's state
template <typename T, typename CharT, typename Traits>
inline void show_type_info_impl(std::basic_ostream<CharT, Traits>& os)
{
    if (os.iword(s_show_type_idx))
        os << " [" << type_name<T>() << ']';
}

}                                                           // namespace details

/**
 * \brief Manipulator to print type for parameter of \c zencxx::debug::print::any
 *
 * Example:
 * \code
 *  namespace print = zencxx::debug::print;
 *  auto smth = ...;
 *  std::cout << print::show_type_info << print::any(smth) << std::endl;
 *  print::no_show_type_info(std::cout);
 * \endcode
 *
 * \sa \c no_show_type_info
 */
template<typename CharT, typename Traits>
inline std::basic_ostream<CharT, Traits>& show_type_info(std::basic_ostream<CharT, Traits>& os)
{
    os.iword(details::s_show_type_idx) = 1;
    return os;
}

/**
 * \brief Manipulator to disable \c show_type_info
 *
 * \sa \c show_type_info
 */
template<typename CharT, typename Traits>
inline std::basic_ostream<CharT, Traits>& no_show_type_info(std::basic_ostream<CharT, Traits>& os)
{
    os.iword(details::s_show_type_idx) = 0;
    return os;
}

}}}                                                         // namespace print, debug, zencxx
