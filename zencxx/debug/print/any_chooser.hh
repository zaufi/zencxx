/**
 * \file
 *
 * \brief Class \c zencxx::debug::print::details::any_chooser (interface)
 *
 * \date Sun Jul 28 13:32:45 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__DEBUG__PRINT__ANY_CHOOSER_HH__
# define __ZENCXX__DEBUG__PRINT__ANY_CHOOSER_HH__

// Project specific includes
# include <zencxx/debug/print/any_convertible_to_string.hh>
# include <zencxx/debug/print/any_streamable.hh>
# include <zencxx/debug/print/any_stub.hh>
# include <zencxx/debug/print/builtins.hh>
# include <zencxx/debug/print/containers.hh>
# include <zencxx/debug/print/std_chrono.hh>
# include <zencxx/debug/print/std_pair.hh>
# include <zencxx/debug/print/std_tuple.hh>
# include <zencxx/debug/print/system_error_class.hh>
# include <zencxx/mpl/seq.hh>
# include <zencxx/mpl/v_or.hh>
# include <zencxx/type_traits/has_left_shift.hh>
# include <zencxx/type_traits/is_range_iterable.hh>
# include <zencxx/type_traits/is_std_basic_string.hh>
# include <zencxx/type_traits/is_std_chrono_duration.hh>
# include <zencxx/type_traits/is_std_chrono_time_point.hh>
# include <zencxx/type_traits/is_std_pair.hh>
# include <zencxx/type_traits/is_std_tuple.hh>
# include <zencxx/type_traits/to_string.hh>

// Standard includes
# include <boost/mpl/apply.hpp>
# include <boost/mpl/bind.hpp>
# include <boost/mpl/deref.hpp>
# include <boost/mpl/find_if.hpp>
# include <boost/mpl/pair.hpp>
# include <boost/mpl/quote.hpp>
# include <ostream>

namespace zencxx { namespace debug { namespace print { namespace details {

/**
 * Metafunction to find an appropriate outputer for given type \c T.
 */
template <typename T>
class any_chooser
{
    typedef typename std::decay<T>::type pure_type;
    typedef mpl::seq<
        // pretty print for booleans
        boost::mpl::pair<
            std::is_same<T, bool>
          , boost::mpl::quote1<any_bool>
          >
        // pretty print character types
      , boost::mpl::pair<
            mpl::v_or<
                std::is_same<T, char>
              , std::is_same<T, wchar_t>
              , std::is_same<T, char16_t>
              , std::is_same<T, char32_t>
              >
          , boost::mpl::quote1<any_char>
          >
        // generic raw pointers printer
      , boost::mpl::pair<
            std::is_pointer<T>
          , boost::mpl::quote1<any_pointer>
          >
        // pretty print for std::strings
      , boost::mpl::pair<
            is_std_basic_string<pure_type>
          , boost::mpl::quote1<any_string>
          >
        // pretty print for std::chrono::duration
      , boost::mpl::pair<
            is_std_chrono_duration<pure_type>
          , boost::mpl::quote1<any_duration>
          >
        // pretty print for std::chrono::time_point
      , boost::mpl::pair<
            is_std_chrono_time_point<pure_type>
          , boost::mpl::quote1<any_time_point>
          >
        // pretty print for system error_code classes (boost or std)
      , boost::mpl::pair<
            is_system_error_class<pure_type>
          , boost::mpl::quote1<any_error_code_class>
          >
        // check if given type can be printed w/ operator<<(ostream&, ...)
      , boost::mpl::pair<
            has_left_shift<std::ostream&, T>
          , boost::mpl::quote1<any_streamable>
          >
        // check if given type can be converted to string
      , boost::mpl::pair<
            /// \todo Replace w/ true variadic \c v_or, but
            /// need to solve some problem before ;-)
            mpl::v_or<
                has_to_string_member<T>
              , has_to_string_adl<T>
              >
          , boost::mpl::quote1<any_convertible_to_string>
          >
        // pretty print for std::pair
      , boost::mpl::pair<
            is_std_pair<pure_type>
          , boost::mpl::quote1<any_pair>
          >
        // pretty print for std::tuple
      , boost::mpl::pair<
            is_std_tuple<pure_type>
          , boost::mpl::quote1<any_tuple>
          >
        // print any container
      , boost::mpl::pair<
            is_range_iterable<T>
          , boost::mpl::quote1<any_container>
          >
        // default case: print that type is not printable ;-)
      , boost::mpl::pair<
            boost::mpl::true_
          , boost::mpl::quote1<any_stub>
          >
      > any_type_dispatch_list;

    typedef typename boost::mpl::find_if<
        any_type_dispatch_list
      , boost::mpl::bind<
            boost::mpl::quote1<boost::mpl::first>
          , boost::mpl::_
          >
      >::type match_iter;

    typedef typename boost::mpl::second<
        typename boost::mpl::deref<match_iter>::type
      >::type meta;

public:
    typedef typename boost::mpl::apply1<meta, T>::type type;
};

}}}}                                                        // namespace details, print, debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__PRINT__ANY_CHOOSER_HH__
