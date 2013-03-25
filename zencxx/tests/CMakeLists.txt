set(
    UNIT_TESTS_SOURCES
    exception_tester.cc
    ticker_tester.cc
  )

add_executable(
    unit_tests
    ${UNIT_TESTS_SOURCES}
  )

target_link_libraries(
    unit_tests
    zencxx
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

add_boost_tests(
    TARGET unit_tests
    SOURCES ${UNIT_TESTS_SOURCES}
  )
