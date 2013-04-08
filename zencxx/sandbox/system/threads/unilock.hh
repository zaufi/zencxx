/**
 * \file
 *
 * \brief Class unilock (interface)
 *
 * \date Thu Jun 10 11:30:44 MSD 2010 -- Initial design
 */

#ifndef __SYSTEM__THREADS__UNILOCK_HH__
#  define __SYSTEM__THREADS__UNILOCK_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/details/unilock_interface_impl.hh>
#  include <system/threads/details/unilock_interface_wrapper.hh>
#  include <system/threads/default_scheduler.hh>
#  include <system/threads/fifo_scheduler.hh>
#  include <system/threads/priority_scheduler.hh>
#  include <system/threads/parameterized_scheduler.hh>

// Standard includes
#  include <boost/mpl/integral_c.hpp>
#  include <boost/mpl/list_c.hpp>
#  include <boost/mpl/list.hpp>
#  include <boost/mpl/pair.hpp>

namespace sys { namespace threads {

/**
 * \brief This matrix (1x1 size) defines the only type of lock.
 *
 * \c unilock instantiated with this matrix (and default scheduler) becomes equivalent to mutex.
 *
 * \sa default_scheduler
 */
struct exclusive_lock
{
    enum type
    {
        lock
    };
    typedef boost::mpl::list<
        boost::mpl::pair<
            boost::mpl::integral_c<type, lock>
          , boost::mpl::list_c<bool, false>
          >
      > matrix_type;
};

/**
 * \brief This matrix defines read/write locking policy.
 */
struct rw_lock
{
    enum type
    {
        read
      , write
    };
    typedef boost::mpl::list<                               //      | Read | Write
        boost::mpl::pair<                                   // -------------------
            boost::mpl::integral_c<type, read>              //      |      |
          , boost::mpl::list_c<bool, true,  false>          // Read |  1   | 0
          >                                                 //      |      |
      , boost::mpl::pair<                                   // -------------------
            boost::mpl::integral_c<type, write>             //      |      |
          , boost::mpl::list_c<bool, false, false>          // Write|  0   | 0
          >                                                 //      |      |
      > matrix_type;
};

template <typename Scheduler>
class unilock
  : public boost::mpl::apply<details::lock_interface_chooser, Scheduler, unilock<Scheduler> >::type
  , public boost::mpl::apply<details::unlock_interface_chooser, Scheduler, unilock<Scheduler> >::type
{
public:
    typedef Scheduler scheduler_type;
    typedef typename scheduler_type::lock_params_type lock_params_type;
    typedef typename scheduler_type::unlock_params_type unlock_params_type;

private:
    typedef details::unilock_interface_impl<scheduler_type> impl_type;

    friend class boost::mpl::apply<details::lock_interface_chooser, Scheduler, unilock<Scheduler> >::type;
    friend class boost::mpl::apply<details::unlock_interface_chooser, Scheduler, unilock<Scheduler> >::type;

    impl_type m_impl;

    void do_lock(typename boost::call_traits<lock_params_type>::param_type p)
    {
        m_impl.lock(p);
    }
    bool do_try_lock(typename boost::call_traits<lock_params_type>::param_type p)
    {
        return m_impl.try_lock(p);
    }
    void do_unlock(typename boost::call_traits<unlock_params_type>::param_type p)
    {
        m_impl.unlock(p);
    }
};

}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__UNILOCK_HH__
