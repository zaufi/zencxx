add_boost_tests(
    TARGET ${UNIT_TESTS}
    SOURCES lock_matrix_tester.cc unilock_tester.cc
    NO_CATCH_SYSTEM_ERRORS
  )

target_link_libraries(
    ${UNIT_TESTS}
    zencxx
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

# kate: hl cmake;
