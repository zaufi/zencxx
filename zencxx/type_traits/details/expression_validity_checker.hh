/**
 * \file
 *
 * \brief Macros to generate has_xxx metafunctions based on C++11 features
 *
 * \date Wed Jul  4 17:17:13 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__TYPE_TRAITS__DETAILS__EXPRESSION_VALIDITY_CHECKER_HH__
# define __ZENCXX__TYPE_TRAITS__DETAILS__EXPRESSION_VALIDITY_CHECKER_HH__

// Project specific includes

// Standard includes
# include <boost/preprocessor/tuple/enum.hpp>
# if !BOOST_PP_VARIADICS
#   error this file require variadic macros support
# endif
# include <type_traits>

/**
 * \brief Macro to generate type checkers
 *
 * Generate expression validity checkers using modern C++11 features
 * like \c decltype and variadic macros.
 */
# define ZEN_TT_EXPR_CHECKER(Name, ParamsTuple, TestCallParamsTuple, Expr) \
namespace details {                                                        \
struct BOOST_PP_CAT(Name, _checker)                                        \
{                                                                          \
    typedef char yes_type;                                                 \
    typedef char (&no_type)[2];                                            \
    template <BOOST_PP_TUPLE_ENUM(ParamsTuple)>                            \
    static no_type test(...);                                              \
    template <BOOST_PP_TUPLE_ENUM(ParamsTuple)>                            \
    static yes_type test(decltype((Expr), void())*);                       \
};                                                                         \
}                                                                          \
template <BOOST_PP_TUPLE_ENUM(ParamsTuple)>                                \
struct Name : std::is_same<                                                \
        decltype(details::BOOST_PP_CAT(Name, _checker)::template test<     \
            BOOST_PP_TUPLE_ENUM(TestCallParamsTuple)                       \
          >(0))                                                            \
      , details::BOOST_PP_CAT(Name, _checker)::yes_type                    \
      > {}

#endif                                                      // __ZENCXX__TYPE_TRAITS__DETAILS__EXPRESSION_VALIDITY_CHECKER_HH__
