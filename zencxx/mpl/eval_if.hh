/**
 * \file
 *
 * \brief Class \c zencxx::mpl::eval_if (interface)
 *
 * \date Sun Jul  8 07:22:06 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__MPL__EVAL_IF_HH__
# define __ZENCXX__MPL__EVAL_IF_HH__

// Project specific includes
# include <zencxx/mpl/eval.hh>

// Standard includes
# include <boost/mpl/eval_if.hpp>

namespace zencxx { namespace mpl {

/**
 * \brief Metafunction class to evaluate <em>meta condition expressions</em>
 *
 * We can't use \c boost::mpl::eval_if just defining a metafunction class for
 * it as for other metafunctions, cuz \c mpl::eval would evaluate all parts
 * what is definitelty wrong...
 *
 * So this metafunction class will evaluate only \e then or \e else part
 * depending on condition.
 *
 */
struct eval_if
{
    template <typename Cond, typename Then, typename Otherwise>
    struct apply
    {
        typedef typename boost::mpl::eval_if<
            typename eval<Cond>::type
          , eval<Then>
          , eval<Otherwise>
          >::type type;
    };
};

/// \name Specializations for \c zencxx::mpl::eval to provide lazy evaluation
//@{
template <>
template <typename... Args>
struct eval_impl<call_meta_function_class_tag>::apply<eval_if(Args...)>
{
    typedef typename eval_if::template apply<Args...>::type type;
};

template <>
template <typename... Args>
struct eval_impl<call_meta_function_class_tag>::apply<eval_if(*)(Args...)>
{
    typedef typename eval_if::template apply<Args...>::type type;
};
//@}
}}                                                          // namespace mpl, zencxx
#endif                                                      // __ZENCXX__MPL__EVAL_IF_HH__
