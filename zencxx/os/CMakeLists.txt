if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    add_compiler_export_flags()

    add_library(
        zencxxos
        exception.cc
        signal/mask.cc
        utils.cc
      )

    generate_export_header(
        zencxxos
        EXPORT_FILE_NAME details/export.hh
      )

    target_link_libraries(
        zencxxos
        ${Boost_SYSTEM_LIBRARY}
      )

    set_target_properties(
        zencxxos
        PROPERTIES VERSION "0.0"
      )

    install(
        TARGETS zencxxos
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${LIBZENCXX_PACKAGE}
      )


    file(GLOB HEADERS *.hh)
    install(
        FILES ${HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT ${LIBZENCXX_DEV_PACKAGE}
      )

    install(
        DIRECTORY signal
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/os
        COMPONENT ${LIBZENCXX_DEV_PACKAGE}
        FILES_MATCHING PATTERN "*.hh"
      )

# Compile tests only if needed
# if(BUILD_TESTING)
#     add_subdirectory(tests)
# endif()
endif()
