file(GLOB HEADERS *.hh)

install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mpl
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )

# Build test if requested
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
