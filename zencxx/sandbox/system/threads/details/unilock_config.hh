/**
 * \file
 *
 * \brief Configurable parameters for unilock
 *
 * \date Thu Jun 10 11:23:28 MSD 2010 -- Initial design
 */

#ifndef __SYSTEM__THREADS__DETAILS__UNILOCK_CONFIG_HH__
#  define __SYSTEM__THREADS__DETAILS__UNILOCK_CONFIG_HH__

#  ifndef UNILOCK_MAX_LOCK_MATRIX_SIZE
#    define UNILOCK_MAX_LOCK_MATRIX_SIZE 5
#  endif                                                    // UNILOCK_MAX_LOCK_MATRIX_SIZE

#  ifndef UNILOCK_MAX_LOCK_ARGS
#    define UNILOCK_MAX_LOCK_ARGS 5
#  endif                                                    // UNILOCK_MAX_LOCK_ARGS

#endif                                                      // __SYSTEM__THREADS__DETAILS__UNILOCK_CONFIG_HH__
