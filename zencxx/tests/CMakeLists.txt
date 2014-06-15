add_boost_tests(
    TARGET ${UNIT_TESTS}
    DYN_LINK
    SOURCES
        exception_tester.cc
        random_tester.cc
        ticker_tester.cc
  )

target_link_libraries(
    ${UNIT_TESTS}
    zencxx
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
