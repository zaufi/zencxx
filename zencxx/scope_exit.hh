/**
 * \file
 *
 * \brief Class \c zencxx::scope_exit (interface)
 *
 * \date Fri Apr 19 23:23:10 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__SCOPE_EXIT_HH__
# define __ZENCXX__SCOPE_EXIT_HH__

// Project specific includes

// Standard includes
#  include <boost/preprocessor/cat.hpp>
#  include <functional>

/**
 * \brief Macro to use instead of \c BOOST_SCOPE_EXIT in C++0x code :)
 *
 * Typical usage is to execute arbitrary code when execution flow leave
 * a current scope (normally or due exception).
 * This macro accepts a capture list same as C++0x lambdas do...
 * Yea, native lambda used inside, and "function" body is a lambda expression actually.
 *
 * Usage example:
 * \code
 *  boost::filesystem::path tmp = boost::filesystem::unique_path("/tmp/XXXXXXXX.XXX");
 *  ZENCXX_SCOPE_EXIT(=) {
 *    boost::system::error_code ec;
 *    boost::filesystem::remove(tmp, ec);
 *  }
 *  try {
 *    // work w/ temporary file
 *  }
 *  catch (...) {
 *  }
 *  return smth;
 * \endcode
 *
 * \todo Think about how to avoid \c std::function usage inside of \c on_scope_exit_call
 */
# define ZENCXX_SCOPE_EXIT(...) \
  zencxx::on_scope_exit_call BOOST_PP_CAT(scope_exit_var_, __LINE__) = [__VA_ARGS__]()

namespace zencxx {

/**
 * \brief Exploit RAII to call some functor on scope exit
 *
 * Comparing to \c BOOST_SCOPE_EXIT it can be a member of some class.
 *
 */
class on_scope_exit_call
{
    std::function<void()> m_on_exit;

public:
    /// Constructor to accept any nulary callable object convertible
    /// to \c std::function
    template <typename F>
    on_scope_exit_call(F f) : m_on_exit(f)
    {}

    /// Make a deferred call on destroy
    ~on_scope_exit_call()
    {
        if (m_on_exit) m_on_exit();
    }
};

}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__SCOPE_EXIT_HH__
