/**
 * \file
 *
 * \brief Random utility functions
 *
 * \date Sun Jun 30 18:24:42 MSK 2013 -- Initial design
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

// Project specific includes
#include <zencxx/utility/random.hh>

// Standard includes
#include <ctime>

namespace zencxx { namespace alphabet { namespace details {
const char s_letters[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "~!@#$%^&*()-_+=`[]{}|\\;:'\",<>.?/ "
  ;
}}                                                          // namespace details, alphabet

namespace {
/// Random Engine used by generators
std::default_random_engine s_re;

/**
 * Helper function to initialize \c s_re (default random engine)
 * \todo Investigate why the first test in \c random_test_2 always shows
 * the same value chosen from integers range [0..10)
 */
inline bool init_default_random_engine()
{
    s_re.seed(std::time(nullptr));                          // Init random seed
    return true;
}

/// Make sure random engine is initialized w/ seed
const bool s_default_re_initialized = init_default_random_engine();
}                                                           // anonymous namespace

ZENCXX_EXPORT std::default_random_engine& default_random_engine()
{
    return s_re;
}

}                                                           // namespace zencxx
