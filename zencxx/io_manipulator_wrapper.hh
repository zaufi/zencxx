/**
 * \file
 *
 * \brief Class \c zencxx::io_manipulator_wrapper (interface)
 *
 * \date Sun Mar 31 15:26:28 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__IO_MANIPULATOR_WRAPPER_HH__
# define __ZENCXX__IO_MANIPULATOR_WRAPPER_HH__

// Project specific includes

// Standard includes

namespace zencxx {

/**
 * Helper class to produce a wrappers used to create manipulators like
 * behaviour for different types.
 *
 * The idea is to provide an \b unique type w/ binded reference inside to some type \c T.
 * Then one may provide an \c operator<< for such a wrapper(s) in different ways.
 * For example, if there is some struct w/ lot of fields. And we want to have a short
 * output when streamming it, but sometimes long.
 *
 * \code
 *  struct some
 *  {
 *    // a lot of members here
 *  };
 *
 *  // define unique type to overload operator<<
 *  typedef zencxx::io_manipulator_wrapper<__LINE__, some> short_some_io_manip;
 *
 *  // define a helper function to produce such wrappers using ADL
 *  short_some_io_manip output_short(const some& s) {
 *    return short_some_io_manip(s);
 *  }
 *
 *  // and finally overload to print short version of some struct
 *  std::ostream& operator<<(std::ostream& os, const short_some_io_manip& sw) {
 *    os << sw.ref().m_some_member << ... ;
 *    return os;
 *  }
 *
 *  // Now it can be used like this:
 *  some s = ...;
 *  std::cout << output_short(s) << ...;
 * \endcode
 *
 */
template <int Dummy, typename T>
class io_manipulator_wrapper
{
    const T& m_ref;

public:
    /// Default constructor
    explicit io_manipulator_wrapper(const T& r) : m_ref(r) {}
    /// Give a storef reference to outer world
    const T& ref() const
    {
        return m_ref;
    }
};

}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__IO_MANIPULATOR_WRAPPER_HH__
