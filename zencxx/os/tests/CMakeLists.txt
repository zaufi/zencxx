if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(
        PLATFORM_DEPENDED_SOURCES
        charset_conversion_tester.cc
      )
endif()

add_boost_tests(
    TARGET ${UNIT_TESTS}
    SOURCES
        color_tester.cc
        ${PLATFORM_DEPENDED_SOURCES}
    NO_CATCH_SYSTEM_ERRORS
  )

target_link_libraries(
    ${UNIT_TESTS}
    zencxxos
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
