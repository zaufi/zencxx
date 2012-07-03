set(UNIT_TESTS_SOURCES
    copy_range_tester.cc
    seq_tester.cc
    v_at_tester.cc
#    vector_tester.cc
#     mpl2_tester.cc
  )

add_boost_tests(
    TARGET ${UNIT_TESTS}
    SOURCES ${UNIT_TESTS_SOURCES}
    NO_CATCH_SYSTEM_ERRORS
  )

target_link_libraries(
    ${UNIT_TESTS}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
