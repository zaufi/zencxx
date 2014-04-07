add_boost_tests(
    TARGET ${UNIT_TESTS}
    SOURCES
        charset_conversion_tester.cc
        color_tester.cc
    NO_CATCH_SYSTEM_ERRORS
  )

target_link_libraries(
    ${UNIT_TESTS}
    zencxxos
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
