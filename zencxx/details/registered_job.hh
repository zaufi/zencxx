/**
 * \file
 *
 * \brief Class \c zencxx::details::registered_job (interface)
 *
 * \date Tue Jun 29 21:28:50 MSD 2010 -- Split \c ticker.hh into details and public parts
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

#ifndef __ZENCXX__DETAILS__REGISTERED_JOB_HH__
# define __ZENCXX__DETAILS__REGISTERED_JOB_HH__

// Project specific includes

// Standard includes
# include <boost/asio/deadline_timer.hpp>
# include <boost/date_time/posix_time/posix_time.hpp>
# include <functional>
# include <memory>
# include <string>
# include <utility>

namespace zencxx { namespace details {
/**
 * \todo Need to rewrite this class using \c boost::msm cuz jobs lifetime can be
 * really complicated. Nowadays we just have no such cases... but potentially we
 * may do...
 *
 * \todo Use \c std::chrono instead of \c boost::posix_time. After that, move ctor
 * and assign can be defaulted.
 *
 * \note This class contains non-copyable member, so the whole class non-copyable
 * as well! (I.e. no need to inherit from noncopyable explicitly)
 */
struct registered_job
{
    enum struct state : unsigned
    {
        /// Manually made job is stopped as well as the job after \c stop() call
        stopped
      , running                                             ///< The job is about to run right now
      , scheduled                                           ///< The job is waits on timer
      , scheduled_for_removal                               ///< The job scheduled for removal
    };

    enum struct type : unsigned
    {
        once                                                ///< One time running job
      , periodic                                            ///< Periodically running job
    };

    /// \warning Unfortunately \c boost::asio::deadline_timer isn't movable
    /// as well as \c boost::scoped_ptr (and \c shared_ptr is too heavy to use here),
    /// so \c std::unique_ptr comes into scene :)
    std::unique_ptr<boost::asio::deadline_timer> m_timer;   ///< Timer to wait on
    boost::posix_time::time_duration m_interval;            ///< In case of periodic job it contains period
    std::function<void()> m_functor;                        ///< Functor to execute
    std::string m_description;                              ///< Job's description
    type m_type;
    state m_state;

    registered_job(
        std::unique_ptr<boost::asio::deadline_timer>&& timer
      , const boost::posix_time::time_duration interval
      , std::function<void()>&& functor
      , std::string&& description
      , const type t
      , const state s
      )
      : m_timer(std::move(timer))
      , m_interval(interval)
      , m_functor(std::move(functor))
      , m_description(std::move(description))
      , m_type(t)
      , m_state(s)
    {
        assert(
            "Functor instance expected to be non empty to have some sense"
          && bool(m_functor)
          );
    }
    /// \name Move semantic support
    //@{
    /// Move ctor can't be defaulted (thanx to \c boost::posix_time::time_duration)
    registered_job(registered_job&& other) noexcept
      : m_timer(std::move(other.m_timer))
      , m_interval(other.m_interval)
      , m_functor(std::move(other.m_functor))
      , m_description(std::move(other.m_description))
      , m_type(other.m_type)
      , m_state(other.m_state)
    {
    }
    /// Move assign can't be defaulted (thanx to \c boost::posix_time::time_duration)
    registered_job& operator=(registered_job&& other) noexcept
    {
        if (this != &other)
            new(this) registered_job(std::move(other));
        return *this;
    }
    //@}
    /// Delete copy ctor
    registered_job(const registered_job&) = delete;
    /// Delete copy-assign operator
    registered_job& operator=(const registered_job&) = delete;
};
}}                                                          // namespace details, zencxx
#endif                                                      // __ZENCXX__DETAILS__REGISTERED_JOB_HH__
