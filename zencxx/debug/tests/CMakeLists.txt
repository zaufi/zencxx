add_boost_tests(
    TARGET ${UNIT_TESTS}
    SOURCES debug_printing_tester.cc
    NO_CATCH_SYSTEM_ERRORS
  )

target_link_libraries(
    ${UNIT_TESTS}
    zencxx
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
