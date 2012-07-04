/**
 * \file
 *
 * \brief Tags to classify <em>compile-time expressions</em>
 *
 * \date Wed Jul  4 06:07:18 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__EXPRESSION_TAGS_HH__
# define __ZENCXX__MPL__EXPRESSION_TAGS_HH__

// Project specific includes
# include <zencxx/mpl/has_apply.hh>
# include <zencxx/mpl/has_type.hh>
# include <zencxx/mpl/is_lambda_expression.hh>

// Standard includes
# include <type_traits>

namespace zencxx { namespace mpl {

struct call_meta_function_class_tag;
struct call_meta_function_tag;
struct lambda_expression_tag;
struct meta_function_class_tag;
struct meta_function_tag;
struct ordinal_type_tag;
struct unsupported_meta_expressoin;

namespace details {
template <
    typename Expr
  , bool = std::is_function<Expr>::value
       || (
            std::is_pointer<Expr>::value
         && std::is_function<typename std::remove_pointer<Expr>::type>::value
          )
  , bool = has_type<Expr>::value
  >
struct tagger;
#if 0
{
    typedef unsupported_meta_expressoin type;
};
#endif

template <typename Expr>
struct tagger<Expr, true, false>
{
    typedef call_meta_function_class_tag type;
};

template <typename Expr>
struct tagger<Expr, true, true>
{
    typedef call_meta_function_tag type;
};

template <typename Expr>
struct tagger<Expr, false, true>
{
    typedef meta_function_tag type;
};

template <typename Expr>
struct tagger<Expr, false, false>
{
    typedef ordinal_type_tag type;
};

}                                                           // namespace details

/**
 * \brief Metafunction to get \e expression_tag by type
 */
template <typename Expr>
struct expression_tag : details::tagger<Expr>
{
};

}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__EXPRESSION_TAGS_HH__
