/**
 * \file
 *
 * \brief Class \c zencxx::debug::details::linux_impl::m_backtraceimpl_linux (interface)
 *
 * \date Thu Mar 14 03:00:17 MSK 2013 -- Initial design
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
#include <zencxx/details/export.hh>

// Standard includes
#include <iosfwd>
#include <string>
#include <vector>

namespace zencxx { namespace debug { namespace details { inline namespace linux_impl {
/// \ingroup DEBUG
//@{

/**
 * \brief Class to collect an execution backtrace
 *
 * This class uses GNU libc's call to \c backtrace to obtain a current
 * program's stack trace.
 *
 * \todo It would be better to refactoring this class to be more
 * like an STL container. Particularly replace \c stack() method
 * with pair of \c begin and \c end methods to access collected info
 * and do 'transformation' on the fly (on iterator dereference).
 *
 */
class ZENCXX_EXPORT backtrace
{
public:
    static constexpr auto DEFAULT_STACK_DEPTH = 100u;
    static constexpr auto DEFAULT_EXCLUDE_FIRST_FRAMES_COUNT = 1u;

    typedef std::vector<std::string> stack_type;            ///< Result type for \c stack() method
    typedef stack_type::size_type size_type;                ///< Type for size of collected trace

    /// Collect \c N backtraces, exclude first \e E frames
    explicit backtrace(
        const unsigned depth = DEFAULT_STACK_DEPTH
      , const unsigned exclude_cnt = DEFAULT_EXCLUDE_FIRST_FRAMES_COUNT
      );

    /// Default copy ctor
    backtrace(const backtrace&) = default;
    /// Default copy-assign operator
    backtrace& operator=(const backtrace&) = default;
    /// Default move ctor
    backtrace(backtrace&&) = default;
    /// Default move-assign operator
    backtrace& operator=(backtrace&&) = default;

    /// Check if instnance is empty
    bool empty() const noexcept
    {
        return m_backtrace.empty();
    }
    /// Get size of collected trace info
    size_type size() const noexcept
    {
        return m_backtrace.size();
    }
    /// Obtain a collected trace as a vector of strings
    stack_type stack() const;

private:
    std::vector<void*> m_backtrace;                         ///< Stack trace: array of void pointers
};

/// Make \c backtrace printable to C++ output streams
ZENCXX_EXPORT std::ostream& operator<<(std::ostream&, const backtrace&);

//@}
}}}}                                                        // namespace linux_impl, details, debug, zencxx
