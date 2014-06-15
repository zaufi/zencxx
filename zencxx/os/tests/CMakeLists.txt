add_boost_tests(
    TARGET ${UNIT_TESTS}
    MODULE OSTests
    DYN_LINK
    SOURCES
        color_tester.cc
        charset_conversion_tester.cc
    NO_CATCH_SYSTEM_ERRORS
  )

target_link_libraries(
    ${UNIT_TESTS}
    zencxxos
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
