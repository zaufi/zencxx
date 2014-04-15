/**
 * \file
 *
 * \brief Class \c zencxx::debug::details::linux_impl::backtrace_impl_dummy (interface)
 *
 * \date Thu Mar 14 05:05:45 MSK 2013 -- Initial design
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
#include <boost/config.hpp>
#include <ostream>
#include <string>
#include <vector>

#ifdef BOOST_NO_CXX11_INLINE_NAMESPACES
# define ZENCXX_INLINE_NAMESPACE
#else                                                       // BOOST_NO_CXX11_INLINE_NAMESPACES
# define ZENCXX_INLINE_NAMESPACE inline
#endif                                                      // !BOOST_NO_CXX11_INLINE_NAMESPACES

namespace zencxx { namespace debug { namespace details { ZENCXX_INLINE_NAMESPACE namespace dummy_impl {

/**
 * \brief Dummy implementation of \c backtrace class for platforms
 * which doesn't supports for backtraces or still waiting for implementation =)
 */
class backtrace
{
public:
  public:
    BOOST_STATIC_CONSTEXPR auto DEFAULT_STACK_DEPTH = 100u;
    BOOST_STATIC_CONSTEXPR auto DEFAULT_EXCLUDE_FIRST_FRAMES_COUNT = 1u;

    typedef size_t size_type;                               ///< Type for size of collected trace
    typedef std::vector<std::string> stack_type;            ///< Result type for \c stack() method

    explicit backtrace(
        const unsigned depth = DEFAULT_STACK_DEPTH
      , const unsigned exclude_cnt = DEFAULT_EXCLUDE_FIRST_FRAMES_COUNT
      )
    {}

    bool empty() const BOOST_NOEXCEPT
    {
        return true;
    }
    size_type size() const BOOST_NOEXCEPT
    {
        return 0;
    }
    stack_type stack() const
    {
        return stack_type();
    }
};

/**
 * \brief Make \c Backtrace printable to C++ output streams
 */
inline std::ostream& operator<<(std::ostream& os, const backtrace&)
{
    os << "<no-backtrace-available>";
    return os;
}

}}}}                                                        // namespace dummy_impl, details, debug, zencxx
