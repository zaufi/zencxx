set(HEADERS has_left_shift.hh)

install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/type_traits
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )

# Build test if requested
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
