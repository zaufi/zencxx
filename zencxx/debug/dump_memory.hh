/**
 * \file
 *
 * \brief Class \c zencxx::debug::dump_memory (interface)
 *
 * \date Fri Sep 10 12:43:19 MSD 2010 -- Initial design
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

#ifndef __ZENCXX__DEBUG__DUMP_MEMORY_HH__
# define __ZENCXX__DEBUG__DUMP_MEMORY_HH__

// Project specific includes

// Standard includes
#  include <iosfwd>
#  include <string>
#  include <vector>

namespace zencxx { namespace debug {

/**
 * \brief Class to print hex dumped memory
 *
 * The main purpose of this class, as one probably already guessed, is to
 * print a hex dump of a memory region, given by a start address and a size
 * to the constructor, to any C++ stream.
 *
 * Typical usage of this class look like a usual I/O stream manipulator :)
 *
 * Examples of usage:
 * \code
 * char buffer[BUFFER_SIZE];
 * some_structure my = ...;
 * std::string str = ...;
 * cout << debug::dump_memory(buffer, BUFFER_SIZE) << endl;
 * cout << debug::dump_memory(&my) << endl;  // sizeof(my) used as size
 * cout << debug::dump_memory(&str) << endl; // prints string object content
 * // print string contens
 * cout << debug::dump_memory(str.data(), str.size()) << endl;
 * \endcode
 *
 * \todo Add manipulator or smth to print offsets instead of address in a dump or mix of them
 *
 * \todo Add manipulator to set width of hex-dump area (default 16 bytes)
 *
 * \todo Detect dump width dynamically depending on terminal size. Use manipulator
 *       to turn ON/OFF this behaviour.
 */
class dump_memory
{
public:
    /// General constructor
    constexpr dump_memory(const void* ptr, size_t sz) noexcept
      : m_ptr(ptr), m_size(sz)
    {}
    /// Accept \c std::string to dump contents
    explicit dump_memory(const std::string& s) noexcept
      : m_ptr(s.data()), m_size(s.size()) {}
    /// Accept \c std::vector to dump contents
    template <typename T, typename Alloc>
    explicit dump_memory(const std::vector<T, Alloc>& v) noexcept
      : m_ptr(v.data()), m_size(v.size()) {}
    /// Constructor to dump any object
    template <typename T>
    explicit dump_memory(const T* ptr) noexcept
      : m_ptr(ptr), m_size(sizeof(T)) {}
    /**
     * \brief Make \c dump_memory looks like a stream's manipulator
     * \todo Is there some useful manipulators possible? I mean
     * to change formatting/style or smth of printed dumps...
     */
    friend std::ostream& operator<<(std::ostream&, const dump_memory&);

private:
    const void* const m_ptr;                                ///< Pointer to memory block
    const std::size_t m_size;                               ///< Size of block to dump
};

}}                                                          // namespace debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__DUMP_MEMORY_HH__
