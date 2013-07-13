add_boost_tests(
    TARGET ${UNIT_TESTS}
    SOURCES
        copy_range_tester.cc
        mpl_logic_tester.cc
        mpl_traits_tester.cc
        seq_tester.cc
        v_at_tester.cc
    NO_CATCH_SYSTEM_ERRORS
  )

target_link_libraries(
    ${UNIT_TESTS}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
