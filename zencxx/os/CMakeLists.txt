if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    add_compiler_export_flags()

    add_library(
        zencxxos
        charset/convert.cc
        color.cc
        daemon_application.cc
        escape_sequences.cc
        exception.cc
        signal/mask.cc
        term/utils.cc
        utils.cc
      )

    generate_export_header(
        zencxxos
        EXPORT_FILE_NAME details/export.hh
      )

    target_link_libraries(
        zencxxos
        zencxx
        ${Boost_SYSTEM_LIBRARY}
      )

    export(
        TARGETS zencxx zencxxos
        NAMESPACE zencxx::
        FILE zencxxos.cmake
        EXPORT_LINK_INTERFACE_LIBRARIES
      )

    set_target_properties(
        zencxxos
        PROPERTIES
            VERSION "0.0"
            EXPORT_NAME "os"
      )

    install(
        TARGETS zencxxos
        EXPORT zencxxos
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${LIBZENCXX_PACKAGE}
      )

    install(
        EXPORT zencxxos
        DESTINATION ${CMAKE_INSTALL_CMAKE_MODULESDIR}
        NAMESPACE zencxx::
        EXPORT_LINK_INTERFACE_LIBRARIES
      )

    file(GLOB HEADERS *.hh)
    install(
        FILES ${HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zencxx/os
        COMPONENT ${LIBZENCXX_DEV_PACKAGE}
      )
    install(
        DIRECTORY charset signal term
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zencxx/os
        COMPONENT ${LIBZENCXX_DEV_PACKAGE}
        FILES_MATCHING PATTERN "*.hh"
      )
    install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/details
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zencxx/os
        COMPONENT ${LIBZENCXX_DEV_PACKAGE}
        FILES_MATCHING PATTERN "*.hh"
      )

    # Compile tests only if needed
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()

endif()
