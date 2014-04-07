/**
 * \file
 *
 * \brief Source code location related stuff
 *
 * \date Fri Sep 10 13:05:12 MSD 2010 -- Initial design
 * \date Thu May 31 22:56:40 MSK 2012 -- Use constexpr to avoid runtime filename string traversal
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

#ifndef __ZENCXX__DEBUG__LOCATION_HH__
# define __ZENCXX__DEBUG__LOCATION_HH__

// Project specific includes
# include <zencxx/debug/basic_location.hh>
# include <zencxx/ctstring.hh>

// Standard includes
# include <boost/current_function.hpp>
# include <boost/preprocessor/if.hpp>
# include <boost/preprocessor/stringize.hpp>

/// \ingroup DEBUG
//@{

/// Macro to get current file (short form)
/// \internal
# define ZENCXX_PP_CURRENT_FILE_SHORT_ \
    ::zencxx::debug::details::file_basename(__FILE__)

/// Macro to get current file (normal form)
/// \internal
# define ZENCXX_PP_CURRENT_FILE_ __FILE__

/// Macro to pass full filename and line number to \c location
/// \internal
# define ZENCXX_PP_MARK_(IsString) \
    ZENCXX_PP_CURRENT_FILE_ \
  , BOOST_PP_IF(IsString, BOOST_PP_STRINGIZE(__LINE__), __LINE__)

/// Macro to pass short filename and line number to \c location
/// \internal
# define ZENCXX_PP_SHORT_MARK_(IsString) \
    ZENCXX_PP_CURRENT_FILE_SHORT_ \
  , BOOST_PP_IF(IsString, BOOST_PP_STRINGIZE(__LINE__), __LINE__)

/// Macro to pass short filename and line number to \c location
/// \internal
# define ZENCXX_PP_FUNC_MARK_(IsString) \
    ZENCXX_PP_CURRENT_FILE_SHORT_ \
  , BOOST_PP_IF(IsString, BOOST_PP_STRINGIZE(__LINE__), __LINE__) \
  , BOOST_CURRENT_FUNCTION

/// \todo Documentation
# define ZEN_THIS_LOCATION()   ::zencxx::debug::make_location(ZENCXX_PP_SHORT_MARK_(1))
/// \todo Documentation
# define ZEN_SRC_LOCATION()    ::zencxx::debug::make_location(ZENCXX_PP_MARK_(1))
/// \todo Documentation
# define ZEN_SOURCE_LOCATION() ::zencxx::debug::make_location(ZENCXX_PP_FUNC_MARK_(1))

namespace zencxx { namespace debug { namespace details {
/// \internal Function to get file name from full path
constexpr inline const char* file_basename(const char* const path)
{
    return ct::strrchr(path, '/') ? ct::strrchr(path, '/') + 1 : path;
}
}                                                           // namespace details

/**
 * \brief Type of most used \c basic_location instances
 *
 * Location helper macros will produce locations of this type.
 *
 * \sa \c ZEN_THIS_LOCATION
 * \sa \c ZEN_SRC_LOCATION
 * \sa \c ZEN_SOURCE_LOCATION
 */
typedef basic_location<const char*, const char*, const char*> location;

/// Produce \c basic_location instance initialized with file name and line number
/// \overload
template <typename File, typename Line>
inline basic_location<File, Line> make_location(File file, Line line)
{
    return basic_location<File, Line>(file, line);
}

/// Produce \c basic_location instance initialized with file name, line number and function name
/// \overload
template <typename File, typename Line, typename Function>
inline basic_location<File, Line, Function> make_location(File file, Line line, Function function)
{
    return basic_location<File, Line, Function>(file, line, function);
}
}}                                                          // namespace debug, zencxx

//@}
#endif                                                      // __ZENCXX__DEBUG__LOCATION_HH__
