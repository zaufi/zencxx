add_boost_tests(
    TARGET ${UNIT_TESTS}
    MODULE TypeTraitsTests
    DYN_LINK
    SOURCES
        arbitrary_checker_tester.cc
        has_left_shift_tester.cc
        has_begin_end_tester.cc
        is_dereferenceable_tester.cc
        is_incrementable_tester.cc
        is_swappable_tester.cc
        is_iterator_tester.cc
        is_system_error_class_tester.cc
        range_iterable_tester.cc
        to_string_tester.cc
    NO_CATCH_SYSTEM_ERRORS
  )

target_link_libraries(
    ${UNIT_TESTS}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
