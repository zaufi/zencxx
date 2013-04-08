/**
 * \file
 *
 * \brief Class log_proxy (implementation)
 *
 * \date Fri Nov 20 20:21:39 MSK 2009 -- Initial design
 */

// Project specific includes
#include <config.h>
#include <system/lwl/details/log_proxy.hh>
#include <system/lwl/color.hh>
#include <system/os/posix_time/time.hh>
#include <system/threads/mutex.hh>
#include <system/threads/specific.hh>

// Standard includes
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <unistd.h>

namespace sys { namespace lwl { namespace details { namespace {

const std::string::size_type MAX_THREAD_ID = 8;             //!< Max length of thread if field in log
const std::string::size_type YEAR_WIDTH = 4;                //!< Width of year filed in log
const std::string::size_type MDT_WIDTH = 2;                 //!< Width of month/day/hour/minute/seconds field
const std::string::size_type MICROSEC_WIDTH = 6;            //!< Width of microseconds filed in log

/// Verbosity levels as strings lookup table
const char* const s_level_strings[] =
{
    ""                                                      // error level "none"
  , "fatal"
  , "error"
  , "warning"
  , "notice"
  , "info"
  , "trace"
  , "debug"
};

/// Length of widest string in a \c s_level_strings
/// \note +1 for one more space after level field
const size_t MAX_SEVERITY_LEVEL_WIDTH = 1 + strlen(
    *std::max_element(
        s_level_strings
      , s_level_strings + (sizeof(s_level_strings) / sizeof(const char* const))
      , boost::bind(&strlen, _1) < boost::bind(&strlen, _2)
      )
  );

threads::mutex s_output_mut;                                // Mutex to guard log output
threads::specific_val<std::string> s_thread_id;             // Per thread string with thread name

inline const std::string& get_thread_id()
{
    return s_thread_id.value();
}

/// Print out log entry
int show_message(const std::string& msg, const std::string& pfx, verbosity_level::type level)
{
    // Get current time
    const os::posix_time::time t;                           // "now"
    tm timestamp_gmt = t.gmtime();

    // Select stream to spam into...
    std::ostream& os = settings().select_stream(level);
    std::ios::fmtflags old_flags = os.flags();              // Save stream state (fromatting flags)
    std::string thread_id_str = get_thread_id();
    if (thread_id_str.empty())
        thread_id_str = boost::lexical_cast<std::string>(pthread_self());
    if (thread_id_str.length() > MAX_THREAD_ID)
    {
        std::string::const_iterator first = thread_id_str.begin();
        std::string::const_iterator last = first;
        std::advance(last, MAX_THREAD_ID);
        thread_id_str = std::string(first, last);
    }

    // Accure I/O mutex before spam to avoid garbage on a screen
    threads::mutex::scoped_lock g(&s_output_mut);

    // Make a spam
    os << std::setfill('0') << color(esc::fg::gray)
      // Output YYYY.MM.DD HH:MM:SS.MSEC
      << timestamp_gmt << '.'
      // NOTE microseconds has 6 digits
      << std::setw(int(MICROSEC_WIDTH)) << std::setfill('0') << (t.nanoseconds() / 1000)
      << color(esc::reset) << ' '
      // Output thread name as <thread-id>
      << color(esc::fg::normal)
      << std::left << std::setw(int(MAX_THREAD_ID)) << std::setfill(' ') << thread_id_str << std::right
      << color(esc::reset) << ' '
      // Output severity level
      << color(settings().select_severity_color(level))
      << std::setw(int(MAX_SEVERITY_LEVEL_WIDTH)) << s_level_strings[level]
      // Ready to print a main message...
      << color(esc::reset) << ' '
      ;
    const std::string::size_type DELIMITERS_SIZE = 9;       // Sum of individual chars in the logging prefix above
    if (!pfx.empty())
        os << color(esc::fg::bright_white)
          << '[' << pfx << ']'
          << color(esc::reset) << ' ';

    // Split the buffer into lines
    typedef std::vector<boost::iterator_range<std::string::const_iterator> > lines_vector;
    // NOTE lines_split is quick replacement for boost::algorithm::split
    lines_vector lines;
    boost::algorithm::split(lines, msg, boost::algorithm::is_any_of("\n"));
    if (!lines.empty())
    {
        // Print the first line (prefix already in a buffer)
        os << color(settings().select_severity_color(level));
        lines_vector::const_iterator it = lines.begin();
        std::copy(it->begin(), it->end(), std::ostream_iterator<char>(os));
        os << color(esc::reset) << '\n';
        ++it;                                               // Move current line pointer
        for (
            lines_vector::const_iterator last = lines.end()
          ; it != last
          ; ++it
          )
        {
            // Print remaining lines with padding
            const std::string::size_type PADDING_SIZE = YEAR_WIDTH + 5 * MDT_WIDTH + MICROSEC_WIDTH
              + MAX_THREAD_ID + MAX_SEVERITY_LEVEL_WIDTH + DELIMITERS_SIZE;
            static const std::string PADDING(PADDING_SIZE, ' ');
            os << PADDING << color(settings().select_severity_color(level));
            std::copy(it->begin(), it->end(), std::ostream_iterator<char>(os));
            os << color(esc::reset) << '\n';
        }
    }
    else
    {
        /// \todo Probably it is not good to print empty lines...
        os << color(esc::reset) << '\n';
    }
    os.flush();
    os.flags(old_flags);                                    // Restore stream formatting flags
    return 0;
}

}                                                           // anonymous namespace

verbosity_level::type level2id(const std::string& level_name)
{
    typedef std::map<std::string, verbosity_level::type> str2vl_table;
    /// \todo It would be nice to join this map and global list of strings somehow
    static const str2vl_table s_map = boost::assign::pair_list_of
        (s_level_strings[0], verbosity_level::none)
        (s_level_strings[1], verbosity_level::fatal)
        (s_level_strings[2], verbosity_level::error)
        (s_level_strings[3], verbosity_level::warning)
        (s_level_strings[4], verbosity_level::notice)
        (s_level_strings[5], verbosity_level::info)
        (s_level_strings[6], verbosity_level::trace)
        (s_level_strings[7], verbosity_level::debug)
      ;
    str2vl_table::const_iterator it = s_map.find(level_name);
    return (s_map.end() == it) ? verbosity_level::none : it->second;
}

void set_thread_id(const std::string& id_str)
{
    s_thread_id.value() = id_str;
}

log_proxy::~log_proxy()
{
    if (m_is_enabled && m_os.get())
        show_message(m_os->str(), m_prefix, m_level);
}

log_proxy& log_proxy::operator<<(color c)
{
    if (m_is_enabled)
    {
        if (sys::esc::prev == c.get())
        {
            assert("Color stack expected to be non empty" && !m_current_color.empty());
            if (!m_current_color.empty())                   // safe check for release version
            {
                m_current_color.pop_back();
                if (!m_current_color.empty())               // If it was not last item in a stack, switch color to previous
                    (*m_os) << color(m_current_color.back());
                else                                        // Otherwise switch to default for this level
                    (*m_os) << color(settings().select_severity_color(m_level));
            }
        }
        else if (sys::esc::reset == c.get())                // Drop colors stack on explicit reset request
        {
            m_current_color.clear();
            (*m_os) << c.get();
        }
        else
        {
            m_current_color.push_back(c.get());
            (*m_os) << c.get();
        }
    }
    return *this;
}

/**
 * Here is two cases possible depending on constructor used to
 * make an instance of \c log_proxy.
 *
 * \li User just wants to make a spam w/o channels. In this case \c m_level
 * will be initialized to \c verbosity_level::none.
 *
 * \li \c log_proxy instance was obtained from channel's \c operator(), and
 * \c m_level would be initialized to channel's verbosity level, which is
 * definitely can't be (must assert this to avoid possible future errors)
 * \c verbosity_level::none.
 *
 * In the first case we have to assign desired verbosity level and deside is
 * spam still be enabled.
 *
 * In the second case we have to analyze weather or not desired verbosity level
 * same or less than predefined by channel. If so, spam would be allowed, otherwise
 * disabled.
 */
log_proxy& log_proxy::configure(verbosity_level::type l)
{
    if (verbosity_level::none == m_level)                   // Was it initialized by log() function?
    {                                                       // Yep
        m_level = l;
        m_is_enabled = m_is_enabled && (l <= settings().level());
    }
    else
    {
        m_is_enabled = m_is_enabled && (l <= m_level);
        if (m_is_enabled) m_level = l;
    }
    m_level = l;
    return *this;
}

}}}                                                         // namespace details, lwl, sys
