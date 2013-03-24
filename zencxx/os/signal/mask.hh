/**
 * \file
 *
 * \brief Class \c zencxx::os::signal::mask (interface)
 *
 * \date Sat Nov 28 21:51:03 MSK 2009 -- Initial design
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

#ifndef __ZENCXX__OS__SIGNAL__MASK_HH__
# define __ZENCXX__OS__SIGNAL__MASK_HH__

// Project specific includes
# include <zencxx/os/exception.hh>

// Standard includes
# include <pthread.h>
# include <ostream>
# include <cassert>
# include <csignal>

namespace zencxx { namespace os { namespace signal {

/**
 * \brief Helper class to manage signal masks
 */
class mask
{
    sigset_t mask_;                                         ///< Current signal mask

public:
    enum init_style
    {
        get_current
      , empty
    };
    enum how
    {
        block = SIG_BLOCK
      , unblock = SIG_UNBLOCK
      , set_as_is = SIG_SETMASK
    };
    /// Construct an instance using explicit mask
    explicit mask(const sigset_t& m) : mask_(m)
    {}
    /// Default constructor will obtain the current mask or init to empty
    explicit mask(init_style what = get_current)
    {
        if (get_current == what)
        {
            const int rc = pthread_sigmask(SIG_SETMASK, 0, &mask_);
            if (0 != rc)
                ZENCXX_THROW(os::exception(rc, "pthread_sigmask"));
        }
        else clear();
    }
    mask& clear()                                           ///< Clear all signals
    {
        sigemptyset(&mask_);
        return *this;
    }
    mask& set_all()                                         ///< Set all signals
    {
        sigfillset(&mask_);
        return *this;
    }
    mask& set(int signum)                                   ///< Add signal to set
    {
        int rc = sigaddset(&mask_, signum);
        if (0 != rc)
            ZENCXX_THROW(os::exception(rc, "sigaddset"));
        return *this;
    }
    mask& unset(int signum)                                 ///< Remove signal from set
    {
        int rc = sigdelset(&mask_, signum);
        if (0 != rc)
            ZENCXX_THROW(os::exception(rc, "sigdelset"));
        return *this;
    }
    /// Check is given signal in the mask
    bool is_set(int signum) const
    {
        int rc = sigismember(&mask_, signum);
        if (-1 == rc)
            ZENCXX_THROW(os::exception(rc, "sigismember"));
        return rc;
    }
    operator const sigset_t&() const                        ///< Access to the current mask
    {
        return mask_;
    }
    void apply(how h = set_as_is) const                     ///< Apply mask to a current thread
    {
        assert("Invalid set mask method" && (block == h || unblock == h || set_as_is == h));
        int rc = pthread_sigmask(h, &mask_, 0);
        if (0 != rc)
            ZENCXX_THROW(os::exception(rc, "pthread_sigmask"));
    }
};

/// Make a signal mask class printable
std::ostream& operator<<(std::ostream&, const mask&);

}}}                                                         // namespace signal, os, zencxx
#endif                                                      // __ZENCXX__OS__SIGNAL__MASK_HH__
