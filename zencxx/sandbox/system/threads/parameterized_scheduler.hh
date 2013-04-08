/**
 * \file
 *
 * \brief Class parameterized_scheduler (interface)
 *
 * \date Thu Jun 10 14:06:17 MSD 2010 -- Initial design
 */

#ifndef __SYSTEM__THREADS__PARAMETERIZED_SCHEDULER_HH__
#  define __SYSTEM__THREADS__PARAMETERIZED_SCHEDULER_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/details/lock_matrix.hh>

// Standard includes
#  include <boost/call_traits.hpp>
#  include <boost/fusion/include/at_c.hpp>
#  include <boost/fusion/include/front.hpp>
#  include <boost/fusion/include/vector.hpp>
#  include <boost/fusion/mpl/pop_front.hpp>
#  include <boost/fusion/mpl/push_front.hpp>
#  include <map>

namespace sys { namespace threads {

/// \note This scheduler assume that template parameter \c Scheduler is default constructable
template <typename Scheduler, typename Key>
class parameterized_scheduler
{
public:
    typedef Key key_type;
    typedef Scheduler scheduler_type;
    typedef typename scheduler_type::matrix_type matrix_type;
    typedef typename boost::mpl::push_front<
        typename scheduler_type::lock_params_type
      , key_type
      >::type lock_params_type;
    typedef typename boost::mpl::push_front<
        typename scheduler_type::unlock_params_type
      , key_type
      >::type unlock_params_type;
    typedef typename scheduler_type::request_id_type request_id_type;

private:
    typedef std::map<key_type, scheduler_type> locked_resources_type;
    locked_resources_type m_locked_resources;

public:

    bool try_lock(
        typename boost::call_traits<lock_params_type>::param_type p
      , request_id_type request_id
      )
    {
        return m_locked_resources[boost::fusion::front(p)]
          .try_lock(boost::fusion::pop_front(p), request_id);
    }
    void unlock(typename boost::call_traits<unlock_params_type>::param_type p)
    {
        scheduler_type& s = m_locked_resources[boost::fusion::front(p)];
        s.unlock(boost::fusion::pop_front(p));
        if (!s.is_used())
            m_locked_resources.erase(boost::fusion::front(p));
    }
    //
    bool is_used() const
    {
        return !m_locked_resources.empty();
    }
    request_id_type assign_request_id(typename boost::call_traits<lock_params_type>::param_type p)
    {
        return m_locked_resources[boost::fusion::front(p)]
          .assign_request_id(boost::fusion::pop_front(p));
    }
    void unassign_request_id(
        typename boost::call_traits<lock_params_type>::param_type p
      , request_id_type request_id
      )
    {
        m_locked_resources[boost::fusion::front(p)]
          .unassign_request_id(boost::fusion::pop_front(p), request_id);
    }
};

}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__PARAMETERIZED_SCHEDULER_HH__
