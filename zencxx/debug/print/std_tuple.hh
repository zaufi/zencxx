/**
 * \file
 *
 * \brief Debug printing for \c std::tuple
 *
 * \date Fri Jul 26 02:46:48 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__DEBUG__PRINT__STD_TUPLE_HH__
# define __ZENCXX__DEBUG__PRINT__STD_TUPLE_HH__

// Project specific includes
# include <zencxx/debug/print/any_manip.hh>
# include <zencxx/debug/print/any_generic.hh>
# include <zencxx/io_manipulator_wrapper.hh>

// Standard includes
# include <boost/mpl/plus.hpp>
# include <boost/mpl/size_t.hpp>
# include <ostream>
# include <tuple>
# include <utility>

namespace zencxx { namespace debug { namespace print { namespace details {
ZENCXX_MAKE_TEMPLATE_TEMPLATE_WRAPPER_CLASS(std_tuple_wrapper, std::tuple);

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

template <typename... T>
inline std::ostream& operator<<(std::ostream& os, const std_tuple_wrapper<T...>& wt)
{
    {
        using tuple_t = typename std_tuple_wrapper<T...>::wrapped_type;
        details::show_type_info_saver s(os);
        no_show_type_info(os);
        os << '(';
        tuple_printer<
            boost::mpl::integral_c<std::size_t, 0>
          , boost::mpl::integral_c<std::size_t, std::tuple_size<tuple_t>::value>
          >::template print_to(os, wt.ref());
        os << ')';
    }
    // Show type info if needed
    details::show_type_info_impl<std::tuple<T...>>(os);
    return os;
}
}                                                           // namespace details

template <typename... T>
details::std_tuple_wrapper<T...> any(const std::tuple<T...>& t)
{
    return details::std_tuple_wrapper<T...>(t);
}

}}}                                                         // namespace print, debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__PRINT__STD_TUPLE_HH__
