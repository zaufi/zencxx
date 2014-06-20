/**
 * \file
 *
 * \brief Class \c zencxx::debug::details::linux_impl::m_backtraceimpl_linux (implementation)
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

// Project specific includes
#include <zencxx/debug/details/backtrace_impl_linux.hh>

// Standard includes
#include <execinfo.h>
#if defined(__GNUC__)
# include <cxxabi.h>
# include <cstdlib>
# include <cstring>
#endif                                                      // __GNUC__ and gcc version high enough
#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>

namespace zencxx { namespace debug { namespace details { namespace linux_impl {
/**
 * \brief Collect back traces information
 *
 * Remember first \c depth frames from current program's stack
 * excluding first \c exclude_cnt frames. Exclude is useful to drop
 * obvious locations. For example first frame is always points to
 * this constructor, so it mostly useless anyway. Being attached
 * to some exception 2nd frame is always an exception's constructor,
 * in that case it is useless as well...
 *
 * \throw std::bad_alloc in case of failure to allocate enough space for trace.
 */
backtrace::backtrace(const unsigned depth, const unsigned exclude_cnt)
{
    assert("Sanity check" && exclude_cnt < depth);

    void* bt[depth];
    size_type trace_size = static_cast<size_type>(::backtrace(bt, int(depth)));
    if (trace_size > size_type(exclude_cnt))
    {
        m_backtrace.resize(trace_size - exclude_cnt);
        std::copy(bt + exclude_cnt, bt + trace_size, begin(m_backtrace));
    }
}

auto backtrace::stack() const -> stack_type
{
    stack_type result;
    if (!m_backtrace.empty())
    {
        const size_type trace_size = m_backtrace.size();
        std::unique_ptr<char*, decltype(&std::free)> strings = {
            backtrace_symbols(&m_backtrace[0], static_cast<int>(trace_size))
          , &std::free
          };
        if (strings)
        {
            result.reserve(trace_size);                     // Preallocate enough space in a result vector
            for (size_t i = 0; i < trace_size; ++i)
            {
#if __GNUC__
                // Try to demangle current name
                if (char* open_bracket = strrchr(strings.get()[i], '('))
                {
                    if (char* plus_sign = strchr(open_bracket, '+'))
                    {
                        *plus_sign = 0;                     // Insert NULL terminator instead of '+' char
                        // Demangle a symbol starting right after just found open bracket
                        int status;
                        std::unique_ptr<char, decltype(&std::free)> sym = {
                            abi::__cxa_demangle(++open_bracket, 0, 0, &status)
                          , &std::free
                          };
                        *plus_sign = '+';                   // Reinsert a '+' sign back to the string
                        if (!status)
                        {
                            // Inplace construct a string from leading part (before '(' char)
                            auto it = result.emplace(
                                end(result)
                              , strings.get()[i]
                              , open_bracket - strings.get()[i]
                              );
                            it->append(sym.get());          // Append demangled symbol
                            it->append(plus_sign);          // Append tail part from original backtrace string
                            continue;
                        }
                    }
                }
#endif                                                      // __GNUC__
                // Unknown compilator or string format in a backtrace...
                // Probably smth has changed in glibc in last case!
                result.emplace_back(strings.get()[i]);      // Just insert a string w/o demangling it
            }
        }
    }
    return result;
}

/**
 * \relates backtrace
 * \todo Is there any useful manipulators possible?
 * I mean to change formatting or smth...
 */
std::ostream& operator<<(std::ostream& os, const backtrace& bt)
{
    const auto& stack = bt.stack();
    std::copy(begin(stack), end(stack), std::ostream_iterator<std::string>(os, "\n"));
    return os;
}

}}}}                                                        // namespace linux_impl, details, debug, zencxx
