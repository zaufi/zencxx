file(GLOB HEADERS *.hh)

install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )

add_subdirectory(debug)
add_subdirectory(mpl)
add_subdirectory(type_traits)
