/**
 * \file
 *
 * \brief Class \c zencxx::debug::details::basic_location (interface)
 *
 * \date Fri Sep 10 13:05:08 MSD 2010 -- Initial design
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

// Standard includes
#include <boost/call_traits.hpp>
#include <ostream>

namespace zencxx { namespace debug {
/// \ingroup DEBUG
//@{

/**
 *
 * \brief Class to hold a position in source code
 *
 * Most of the time source code location needed for logging purposes.
 * I don't know why a lot of libraries, which has a class similar to this one,
 * wanted to have a line number as \c int. The only reasonable thing possible
 * w/ line number is to convert to string and print. So why not to have it as
 * <tt>const char* const</tt> and avoid runtime (!) conversion?! Moreover, most
 * of the time it will even take less space than \c int.
 *
 * \tparam File internal representation of filename (default is <tt>const char*</tt>),
 * but it is quite possible to have a \c std::string ... the question is "FOR WHAT?"
 * \tparam Line internal representation of a line number
 * \tparam Function internal representation of a function name
 */
template <
    typename File = const char*
  , typename Line = File
  , typename Function = File
  >
class basic_location
{
public:
    typedef File file_type;                                 ///< Type used to store filename
    typedef Line line_type;                                 ///< Type used to store line number
    typedef Function function_type;                         ///< Type used to current function

    /// \note Uninitialized (default) location will contain default constructed
    /// \c m_file , \c m_function and \c m_line
    constexpr basic_location() noexcept
      : m_file(file_type())
      , m_function(function_type())
      , m_line(line_type())
    {}
    constexpr basic_location(
        typename boost::call_traits<file_type>::param_type afile
      , typename boost::call_traits<line_type>::param_type aline
      , typename boost::call_traits<function_type>::param_type afunction = function_type()
      ) noexcept
      : m_file(afile)
      , m_function(afunction)
      , m_line(aline)
    {}
    /// Get source code file
    constexpr typename boost::call_traits<file_type>::param_type file() const noexcept
    {
        return m_file;
    }
    /// Get function name
    constexpr typename boost::call_traits<function_type>::param_type function() const noexcept
    {
        return m_function;
    }
    /// Get source code line
    constexpr typename boost::call_traits<line_type>::param_type line() const noexcept
    {
        return m_line;
    }
    /**
     * \brief Is location was initialized?
     * \note It seems uninitialized \c m_file is quite
     * enough to detect an empty location.
     */
    constexpr bool empty() const noexcept
    {
        return m_file == file_type();
    }

protected:
    file_type m_file;                                       ///< Filename
    function_type m_function;                               ///< Current function name
    line_type m_line;                                       ///< Current line
};

/**
 * \brief Make \c basic_location printable to C++ output streams
 * \relates basic_location
 * \todo It will be nice to make a manipulator(s) to change location string formatting
 */
template <
    typename File
  , typename Line
  , typename Function
  >
inline std::ostream& operator<<(std::ostream& os, const basic_location<File, Line, Function>& l)
{
    if (!l.empty())
    {
        os << l.file() << ':' << l.line();
        if (l.function() != typename basic_location<File, Line, Function>::function_type())
            os << '[' << l.function() << ']';
    }
    else os << "<unknown location>";
    return os;
}

//@}
}}                                                          // namespace debug, zencxx
