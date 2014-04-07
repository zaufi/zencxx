/**
 * \file
 *
 * \brief Class \c zencxx::debug::print::std_chrono (implementation)
 *
 * \date Thu Jul 18 08:42:27 MSK 2013 -- Initial design
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

// Project specific includes
#include <zencxx/debug/print/std_chrono.hh>
#include <zencxx/debug/print/any_manip.hh>

// Standard includes
#include <boost/io/ios_state.hpp>
#include <cassert>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <locale>

namespace zencxx { namespace debug { namespace print { namespace details {
const int s_print_localtime_idx = std::ios_base::xalloc();
const int s_time_format_idx = std::ios_base::xalloc();
const char* const DEFAULT_TIME_FORMAT = "%d/%m/%y %H:%M:%S";
namespace chrono {
typedef std::chrono::duration<int, std::ratio<86400>> days;
}                                                           // namespace chrono

/**
 * This function may set a given stream into an error state (or throw) in case
 * of time conversion failure.
 *
 * \throw std::ios_base::failure in case of time conversion error and if
 * exceptions are turned ON for a given stream.
 *
 * \todo Need refactoring when gcc gets support for \c std::put_time
 */
std::ostream& operator<<(
    std::ostream& os
  , const any_time_point<const std::chrono::system_clock::time_point&>& tp
  )
{
    std::ostream::sentry cerberos(os);
    if (cerberos)
    {
        typedef std::time_put<char, std::ostreambuf_iterator<char>> time_facet;
        auto err = std::ios_base::goodbit;
        const auto& locale = os.getloc();
        if (std::has_facet<time_facet>(locale))
        {
            const auto seconds_since_epoch = std::chrono::system_clock::to_time_t(tp.data());
            std::tm tm;
            std::tm* r;
            if (os.iword(details::s_print_localtime_idx))
                r = localtime_r(&seconds_since_epoch, &tm);
            else
                r = gmtime_r(&seconds_since_epoch, &tm);
            // Is conversion Ok?
            if (r)
            {
                // Get format to use
                const auto* fmt = static_cast<char*>(os.pword(details::s_time_format_idx));
                if (fmt == nullptr)
                    fmt = details::DEFAULT_TIME_FORMAT;
                std::use_facet<time_facet>(locale).put(
                    std::ostreambuf_iterator<char>(os)
                  , os
                  , ' '
                  , &tm
                  , fmt
                  , fmt + std::strlen(fmt)
                  );
            }
            else
            {
                err |= std::ios_base::badbit;
            }
        }
        else
        {
            err |= std::ios_base::badbit;
        }
        os.setstate(err);
    }
    // Show type info if needed
    details::show_type_info_impl<std::chrono::system_clock::time_point>(os);
    return os;
}

std::ostream& operator<<(std::ostream& os, const any_duration<const std::chrono::system_clock::duration&>& dw)
{
    std::ostream::sentry cerberos(os);
    if (cerberos)
    {
        // Save I/O stream state, will be restored at scope exit
        boost::io::ios_all_saver ifs(os);
        // Get a copy of duration value (to be able to modify it)
        auto duration = dw.data();
        // Print days (if any)
        const auto days = std::chrono::duration_cast<chrono::days>(duration);
        if (days.count())
        {
            os << days.count() << std::setw(6) << ((days.count() == 1) ? "day," : "days,") << ' ';
            duration %= days;
        }
        // Print hours:minutes:seconds.msec
        const auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
        if (hours.count())
            duration %= hours;
        assert("Sanity check" && 0 <= hours.count() && hours.count() < 24);
        const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
        if (minutes.count())
            duration %= minutes;
        assert("Sanity check" && 0 <= minutes.count() && minutes.count() < 60);
        const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
        if (seconds.count())
            duration %= seconds;
        const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        if (seconds.count())
            duration %= seconds;
        assert("Sanity check" && 0 <= seconds.count() && seconds.count() < 60);

        os << std::setfill('0')
           << std::setw(2) << hours.count() << ':'
           << std::setw(2) << minutes.count() << ':'
           << std::setw(2) << seconds.count() << '.'
           << std::setw(3) << milliseconds.count()
           ;
    }
    // Show type info if needed
    details::show_type_info_impl<std::chrono::system_clock::duration>(os);
    return os;
}

}}}}                                                        // namespace details, print, debug, zencxx
