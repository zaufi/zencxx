add_boost_tests(
    TARGET ${UNIT_TESTS}
    MODULE ThreadTests
    DYN_LINK
    SOURCES
        has_default_lock_param_tester.cc
        lock_matrix_tester.cc
        default_scheduler_tester.cc
        fifo_adaptor_tester.cc
        priority_queue_adaptor_tester.cc
        parameterized_adaptor_tester.cc
    NO_CATCH_SYSTEM_ERRORS
  )

target_link_libraries(
    ${UNIT_TESTS}
    zencxxthread
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
