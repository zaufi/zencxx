file(GLOB TT_HEADERS *.hh)
file(GLOB TT_DETAILS_HEADERS details/*.hh)

install(
    FILES ${TT_HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/zencxx/type_traits"
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )
install(
    FILES ${TT_DETAILS_HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/zencxx/type_traits/details"
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )

# Build test if requested
if(BUILD_TESTING)
    add_subdirectory(tests)

    # Check if headers are self-contained
    file(GLOB_RECURSE ALL_HEADERS *.hh)
    add_compile_tests(
        STRIP_LEADING_PATH "${CMAKE_SOURCE_DIR}"
        SOURCES ${ALL_HEADERS}
      )
endif()
