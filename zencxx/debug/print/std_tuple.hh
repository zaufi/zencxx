/**
 * \file
 *
 * \brief Debug printing for \c std::tuple
 *
 * \date Fri Jul 26 02:46:48 MSK 2013 -- Initial design
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
#include <zencxx/debug/print/any_manip.hh>
#include <zencxx/debug/print/any_fwd.hh>
#include <zencxx/debug/print/any_wrapper.hh>
#include <zencxx/type_traits/is_std_tuple.hh>

// Standard includes
#include <boost/mpl/plus.hpp>
#include <boost/mpl/size_t.hpp>
#include <ostream>
#include <tuple>
#include <utility>

namespace zencxx { namespace debug { namespace print { namespace details {

template <typename T>
struct any_tuple : public any_wrapper<T>
{
    static_assert(
        is_std_tuple<typename std::decay<T>::type>::value
      , "Type T must be an instance of std::tuple"
      );
    using any_wrapper<T>::any_wrapper;
};

template <typename First, typename Last>
struct tuple_printer
{
    template <typename... T>
    static void print_to(std::ostream& os, const std::tuple<T...>& t)
    {
        os << ::zencxx::debug::print::any(std::get<First::value>(t)) << ", ";
        tuple_printer<
            typename boost::mpl::plus<First, boost::mpl::integral_c<std::size_t, 1>>::type
          , Last
          >::template print_to(os, t);
    }
};

template <typename First>
struct tuple_printer<First, typename boost::mpl::plus<First, boost::mpl::integral_c<std::size_t, 1>>::type>
{
    template <typename... T>
    static void print_to(std::ostream& os, const std::tuple<T...>& t)
    {
        os << any(std::get<First::value>(t));
    }
};

template <typename Last>
struct tuple_printer<Last, Last>
{
    template <typename... T>
    static void print_to(std::ostream&, const std::tuple<T...>&)
    {
    }
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const any_tuple<const T&>& wt)
{
    using wrapped_tuple_t = typename any_tuple<const T&>::wrapped_type;
    using tuple_t = typename std::decay<wrapped_tuple_t>::type;
    {
        details::show_type_info_saver s(os);
        no_show_type_info(os);
        os << '(';
        tuple_printer<
            boost::mpl::integral_c<std::size_t, 0>
          , boost::mpl::integral_c<std::size_t, std::tuple_size<tuple_t>::value>
          >::template print_to(os, wt.data());
        os << ')';
    }
    // Show type info if needed
    details::show_type_info_impl<tuple_t>(os);
    return os;
}
}}}}                                                        // namespace details, print, debug, zencxx
