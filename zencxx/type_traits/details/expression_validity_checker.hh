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

#pragma once

// Project specific includes
#include <zencxx/type_traits/details/yes_no_type.hh>

// Standard includes
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#if !BOOST_PP_VARIADICS
#  error this file require variadic macros support
#endif
#include <type_traits>

/**
 * \brief Macro to generate type checkers
 *
 * Generate expression validity checkers using modern C++11 features
 * like \c decltype and variadic macros.
 *
 * This macro eventually produce a checker which would looks like the following:
 * \code
 *  namespace details {
 *  struct UNSPECIFIED_STRUCT {
 *      // NOTE: Other not important members are missed...
 *
 *      template <TplParamsT>
 *      static UNSPECIFIED_TYPE test(TestTplArgsT, decltype(EXPRESSION-TO-CHECK, ... unspecified ...));
 *  };
 *  template <>
 *  struct checker-name : std::is_same<
 *      decltype(UNSPECIFIED_STRUCT::template test<TestInstParamsT>(TestInstArgsT, ... unspecified ... ) )
 *    , UNSPECIFIED_TYPE
 *    >
 *  {};
 * \endcode
 *
 * \param Name name of the type checker to generate
 * \param TplParamsT a tuple with template parameters list of the generated type checker
 * \param TestTplArgsT a tuple with arguments (possible declared as template parameters) which
 * should be passed as arguments of a testing function, so they can be used, via names, in a
 * \c decltype \e expression
 * \param TestInstParamsT a tuple of template parameters which will be passed to an internal
 * checker function at instantiation point
 * \param TestInstArgsT a tuple of arguments (possible containing checker's template parameters),
 * which will be passed to an internal checker function
 * \param ... the rest is an expression to test. It may use variables previously passed to a checker
 * function.
 *
 * Lets suppose we have an user defined type:
 * \code
 * struct test
 * {
 *     test(int) {}
 *     void bar(const std::string&) const;
 * };
 * \endcode
 * and we want to write a type-checker which will test arbirtary type \c T for some requirements:
 * - is it constructible from \c int
 * - is instance of \c T has a public member-function `bar(const std::string&)`
 *
 * to do so we can use \c ZENCXX_TT_EXPR_CHECKER_EX as the following:
 * \code
 * ZENCXX_TT_EXPR_CHECKER_EX(
 *     is_looks_like_test                        // set a name for a type-checker,
 *   , (typename T)                              // it will accept the only template parameter
 *   , (const T& t)                              // our checker need an instance of T (cref to)
 *   , (T)                                       // parameter(s) needed to instantiate a checker
 *   , (std::declval<const T&>())                // pass a fake cref to test to our checkers
 *     // Make sure T conforms to requirements:
 *   , T(123)                                    // check that `T' can be constructed from `int'
 *   , t.bar(std::declval<const std::string&>()) // check that instance of `T' has a `bar' member-function
 *   );
 * \endcode
 *
 * note that last two parameters is C++ expressions to be validated for given type \c T and
 * a fake parameter \c t, which is of <tt>const T&</tt> type.
 *
 * \todo Rewrite using _Template Aliases_ (\c using keyword and separate type for
 * \c decltype expressions).
 */
#define ZENCXX_TT_EXPR_CHECKER_EX(                                \
    Name                                                          \
  , TplParamsT                                                    \
  , TestTplArgsT                                                  \
  , TestInstParamsT                                               \
  , TestInstArgsT                                                 \
  , ...                                                           \
  )                                                               \
namespace details {                                               \
struct BOOST_PP_CAT(Name, _checker)                               \
{                                                                 \
    template <typename...>                                        \
    static ::zencxx::type_traits::details::no_type test(...);     \
    template <BOOST_PP_TUPLE_ENUM(TplParamsT)>                    \
    static ::zencxx::type_traits::details::yes_type test(         \
        int                                                       \
        BOOST_PP_COMMA_IF(BOOST_PP_TUPLE_SIZE(TestTplArgsT))      \
        BOOST_PP_TUPLE_ENUM(TestTplArgsT)                         \
      , decltype(__VA_ARGS__, true)*                              \
      );                                                          \
};                                                                \
}                                                                 \
template <BOOST_PP_TUPLE_ENUM(TplParamsT)>                        \
struct Name : public std::is_same<                                \
    decltype(                                                     \
        details::BOOST_PP_CAT(Name, _checker)::template test<     \
            BOOST_PP_TUPLE_ENUM(TestInstParamsT)                  \
          >(                                                      \
            0                                                     \
            BOOST_PP_COMMA_IF(BOOST_PP_TUPLE_SIZE(TestInstArgsT)) \
            BOOST_PP_TUPLE_ENUM(TestInstArgsT)                    \
          , nullptr                                               \
          )                                                       \
      )                                                           \
  , ::zencxx::type_traits::details::yes_type                      \
  >::type {}

/**
 * \brief Macro to generate simple type checkers
 *
 * This is reduced version of \c ZENCXX_TT_EXPR_CHECKER_EX, it doesn't use
 * anything except template parameters to validate some C++ expression.
 */
#define ZENCXX_TT_EXPR_CHECKER(Name, TplParamsT, TestInstParamsT, ...) \
  ZENCXX_TT_EXPR_CHECKER_EX(                                           \
      Name                                                             \
    , TplParamsT                                                       \
    , (int)                                                            \
    , TestInstParamsT                                                  \
    , (0)                                                              \
    , __VA_ARGS__                                                      \
    )

