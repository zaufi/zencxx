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
# include <boost/asio/steady_timer.hpp>
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
 * \todo It seems there is a different timer must be used depending on scheduled
 * call type:
 * \li steady clock for relative time calls
 * \li wall clock for absolute time calls
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
    std::unique_ptr<boost::asio::steady_timer> m_timer;     ///< Timer to wait on
    std::chrono::steady_clock::duration m_interval;         ///< In case of periodic job it contains period
    std::function<void()> m_functor;                        ///< Functor to execute
    type m_type;
    state m_state;

    registered_job(
        std::unique_ptr<boost::asio::steady_timer>&& timer
      , const std::chrono::steady_clock::duration interval
      , std::function<void()>&& functor
      , const type t
      , const state s
      )
      : m_timer(std::move(timer))
      , m_interval(interval)
      , m_functor(std::move(functor))
      , m_type(t)
      , m_state(s)
    {
        assert(
            "Functor instance expected to be non empty to have some sense"
          && bool(m_functor)
          );
    }
    /// Default move ctor
    registered_job(registered_job&&) = default;
    /// Default move-assign operator
    registered_job& operator=(registered_job&&) = default;
    /// Delete copy ctor
    registered_job(const registered_job&) = delete;
    /// Delete copy-assign operator
    registered_job& operator=(const registered_job&) = delete;
};
}}                                                          // namespace details, zencxx
#endif                                                      // __ZENCXX__DETAILS__REGISTERED_JOB_HH__
