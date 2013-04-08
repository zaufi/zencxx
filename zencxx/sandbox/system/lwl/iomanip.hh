/**
 * \file
 *
 * \brief Couple of manipulators for logging subsystem
 *
 * \date Sat Jul 17 10:52:48 MSD 2010 -- Initial design
 *
 * \note There is no reason to have manipulators \c inline, cuz
 * they r always called indirectly (via pointer).
 */

#ifndef __SYSTEM__LWL__IOMANIP_HH__
#  define __SYSTEM__LWL__IOMANIP_HH__

// Project specific includes
#  include <config.h>

// Standard includes
#  include <ostream>

namespace sys { namespace lwl {

/**
 * \brief Manipulator to incrase indent of a \b next line
 *
 * \b Next means that next line will be indented according current indentation level.
 * One more important thing is that indentation possible \b only \b if \c sys::lwl::endl used
 * to move to new line!
 *
 * It might sounds slightly inconvinient and strange... yes it is! :)
 * <b>But u have to just remember that fact!</b>
 *
 * \c unindent works similar: \b next (after \c endl) line will be unindented!
 *
 * Other behaviour is \b impossible w/o hacks into \c streambufs and lots of assumptions.
 * To illustrate the problem with indentaion and generic (not modified) streams, lets take
 * a look into the following code:
 * \code
 *  cout << "first line\n" << "start 2nd line... " << indent << "... end of 2nd line\n";
 * \endcode
 * In generic C++ streams it is \b impossible to implement desired \c indent manipulator, cuz
 * at time then it would be called, we r already in the middle of the line (2nd in the example).
 * Moreover, start of line already can be on a disk for example (in case of fstreams). So
 * there is no way to unwind current line up to \c '\n' character and insert a bunch of spaces.
 *
 * This is only possible with custom \c streambuf which would know that it designed to output text
 * line by line. Latter can hold up the internal buffer from being flushed 'till \c '\n' occurs.
 * And even in this case u have to attach this custom buffer to any stream where u want to use
 * indentation (\b before any other stream operations)... so it is why we have that we have... %)
 *
 * Anyway u may peek into docs for \c OUT_FIELD macro for example of indent/unindent usage.
 * It is not so ugly as it could :)
 */
std::ostream& indent(std::ostream&);

/**
 * \brief Manipulator to decrase indent of a \b next line
 * \warning Read docs for \c indent to understand how to use both manipulators.
 * \sa sys::lwl::indent
 */
std::ostream& unindent(std::ostream&);

/**
 * \brief 'Overload' for std::endl
 *
 * This version \e knows about indentation (at least \c sys::lwl's implementation :).
 * As expected it outputs a \c '\n' character and do indentation of a \b next line
 * according current (\b per \b thread) state (yeah! just print required amount of spaces :).
 */
std::ostream& endl(std::ostream&);

}}                                                          // namespace lwl, sys
#endif                                                      // __SYSTEM__LWL__IOMANIP_HH__
