/**
 * \file
 *
 * \brief Class unilock_interface_wrapper (interface)
 *
 * \date Thu Jun 10 11:42:44 MSD 2010 -- Initial design
 */

#if !BOOST_PP_IS_ITERATING
#  ifndef __SYSTEM__THREADS__DETAILS__UNILOCK_INTERFACE_WRAPPER_HH__
#    define __SYSTEM__THREADS__DETAILS__UNILOCK_INTERFACE_WRAPPER_HH__

// Project specific includes
#    include <config.h>
#    include <system/threads/details/unilock_config.hh>
#    include <system/threads/details/unilock_interface_impl.hh>

// Standard includes
#    include <boost/call_traits.hpp>
#    include <boost/fusion/include/size.hpp>
#    include <boost/fusion/include/value_at.hpp>
#    include <boost/mpl/at.hpp>
#    include <boost/mpl/apply.hpp>
#    include <boost/mpl/equal_to.hpp>
#    include <boost/mpl/int.hpp>
#    include <boost/mpl/not.hpp>
#    include <boost/utility/enable_if.hpp>
#    include <boost/preprocessor/cat.hpp>
#    include <boost/preprocessor/comma_if.hpp>
#    include <boost/preprocessor/enum_params.hpp>
#    include <boost/preprocessor/iterate.hpp>
#    include <boost/preprocessor/repeat.hpp>
#    include <boost/preprocessor/repetition/enum_binary_params.hpp>

namespace sys { namespace threads { namespace details {

template <typename Scheduler>
struct is_the_only_lock_type : public boost::mpl::equal_to<
    typename Scheduler::matrix_type::matrix_size
  , boost::mpl::int_<1>
  >
{};

template <
    typename Arity
  , typename IsTheOnlyLockType
  >
struct lock_interface_wrapper;

template <
    typename Arity
  , typename IsTheOnlyLockType
  >
struct unlock_interface_wrapper;

struct lock_interface_chooser
{
    template <
        typename Scheduler
      , typename Derived
      >
    struct apply : public boost::mpl::apply<
        lock_interface_wrapper<
            typename boost::fusion::result_of::size<
                typename Scheduler::lock_params_type
              >::type
          , typename is_the_only_lock_type<Scheduler>::type
          >
      , Scheduler
      , Derived
      >::type
    {};
};

struct unlock_interface_chooser
{
    template <
        typename Scheduler
      , typename Derived
      >
    struct apply : public boost::mpl::apply<
        unlock_interface_wrapper<
            typename boost::fusion::result_of::size<
                typename Scheduler::unlock_params_type
              >::type
          , typename is_the_only_lock_type<Scheduler>::type
          >
      , Scheduler
      , Derived
      >::type
    {};
};

}}}                                                         // namespace details, threads, sys
#    define BOOST_PP_ITERATION_PARAMS_1 (3, (1, UNILOCK_MAX_LOCK_ARGS, "system/threads/details/unilock_interface_wrapper.hh"))
#    include BOOST_PP_ITERATE()

#  endif                                                    // __SYSTEM__THREADS__DETAILS__UNILOCK_INTERFACE_WRAPPER_HH__
#else                                                       // BOOST_PP_IS_ITERATING
#  define N BOOST_PP_ITERATION()

#  define LOCK_ARGS(z, n, text) \
    BOOST_PP_COMMA_IF(n) \
    typename boost::call_traits<\
        typename boost::fusion::result_of::value_at_c<text, n>::type \
      >::param_type BOOST_PP_CAT(p,n)

namespace sys { namespace threads { namespace details {

template <typename IsTheOnlyLockType>
struct lock_interface_wrapper<boost::mpl::int_<N>, IsTheOnlyLockType>
{
    template <
        typename Scheduler
      , typename Derived
      >
    struct apply
    {
        class type
        {
            typedef typename Scheduler::lock_params_type lock_params_type;

        public:
            void lock(BOOST_PP_REPEAT(N, LOCK_ARGS, lock_params_type))
            {
                static_cast<Derived* const>(this)->do_lock(
                    lock_params_type(BOOST_PP_ENUM_PARAMS(N, p))
                  );
            }
            bool try_lock(BOOST_PP_REPEAT(N, LOCK_ARGS, lock_params_type))
            {
                return static_cast<Derived* const>(this)->do_try_lock(
                    lock_params_type(BOOST_PP_ENUM_PARAMS(N, p))
                  );
            }
        };
    };
};

template <>
struct lock_interface_wrapper<boost::mpl::int_<N>, boost::mpl::true_>
{
    template <
        typename Scheduler
      , typename Derived
      >
    struct apply
    {
        class type
        {
            typedef typename Scheduler::lock_params_type lock_params_type;

            typedef typename boost::mpl::at<
                typename Scheduler::matrix_type::matrix_spec_type
              , boost::mpl::int_<0>
              >::type::first lock_value_type;

        public:
            void lock(BOOST_PP_REPEAT(N, LOCK_ARGS, lock_params_type))
            {
                static_cast<Derived* const>(this)->do_lock(
                    lock_params_type(BOOST_PP_ENUM_PARAMS(N, p))
                  );
            }
            // Overload when lock type is the only in given matrix specification
            void lock(BOOST_PP_REPEAT(BOOST_PP_DEC(N), LOCK_ARGS, lock_params_type))
            {
                lock(
                    BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), p)
                    BOOST_PP_COMMA_IF(BOOST_PP_DEC(N))
                    lock_value_type::value
                  );
            }
            bool try_lock(BOOST_PP_REPEAT(N, LOCK_ARGS, lock_params_type))
            {
                return static_cast<Derived* const>(this)->do_try_lock(
                    lock_params_type(BOOST_PP_ENUM_PARAMS(N, p))
                  );
            }
            // Overload when lock type is the only in given matrix specification
            bool try_lock(BOOST_PP_REPEAT(BOOST_PP_DEC(N), LOCK_ARGS, lock_params_type))
            {
                try_lock(
                    BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), p)
                    BOOST_PP_COMMA_IF(BOOST_PP_DEC(N))
                    lock_value_type::value
                  );
             }
        };
    };
};

template <typename IsTheOnlyLockType>
struct unlock_interface_wrapper<boost::mpl::int_<N>, IsTheOnlyLockType>
{
    template <
        typename Scheduler
      , typename Derived
      >
    struct apply
    {
        class type
        {
            typedef typename Scheduler::unlock_params_type unlock_params_type;

        public:
            void unlock(BOOST_PP_REPEAT(N, LOCK_ARGS, unlock_params_type))
            {
                static_cast<Derived* const>(this)->do_unlock(
                    unlock_params_type(BOOST_PP_ENUM_PARAMS(N, p))
                  );
            }
        };
    };
};

template <>
struct unlock_interface_wrapper<boost::mpl::int_<N>, boost::mpl::true_>
{
    template <
        typename Scheduler
      , typename Derived
      >
    struct apply
    {
        class type
        {
            typedef typename Scheduler::unlock_params_type unlock_params_type;

        public:
            void unlock(BOOST_PP_REPEAT(N, LOCK_ARGS, unlock_params_type))
            {
                static_cast<Derived* const>(this)->do_unlock(
                    unlock_params_type(BOOST_PP_ENUM_PARAMS(N, p))
                  );
            }
            // Overload when lock type is the only in given matrix specification
            void unlock(BOOST_PP_REPEAT(BOOST_PP_DEC(N), LOCK_ARGS, unlock_params_type))
            {
                typedef typename boost::mpl::at<
                    typename Scheduler::matrix_type::matrix_spec_type
                  , boost::mpl::int_<0>
                  >::type::first lock_value_type;
                // Call full parameter overloaded unlock()
                unlock(
                    BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), p)
                    BOOST_PP_COMMA_IF(BOOST_PP_DEC(N))
                    lock_value_type::value
                  );
            }
        };
    };
};

}}}                                                         // namespace details, threads, sys

#  undef LOCK_ARGS
#  undef N
#endif                                                      // BOOST_PP_IS_ITERATING
