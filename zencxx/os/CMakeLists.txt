if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    add_compiler_export_flags()

    add_library(
        zencxxos
        ${ZENCXX_LIB_TYPE}
        charset/convert.cc
        color.cc
        daemon_application.cc
        escape_sequences.cc
        exception.cc
        signal/mask.cc
        term/utils.cc
        utils.cc
      )

    target_link_libraries(
        zencxxos
        PUBLIC
            zencxx
            ${Boost_SYSTEM_LIBRARY}
      )

    generate_export_header(zencxxos EXPORT_FILE_NAME details/export.hh)

    #BEGIN Export Component
    set_target_properties(
        zencxxos
        PROPERTIES EXPORT_NAME "os"
      )
    export(
        TARGETS zencxxos zencxx
        NAMESPACE zencxx::
        FILE zencxx-os.cmake
        EXPORT_LINK_INTERFACE_LIBRARIES
     )
    write_export_dependencies(TARGET zencxxos DEPENDENCIES shared)
    #END Export Component

    # Install target library
    install(
        TARGETS zencxxos
        EXPORT zencxx-os
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${LIBZENCXX_PACKAGE}
      )

    # Install cmake files
    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/zencxx-os-dependencies.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKE_MODULESDIR}
        COMPONENT ${LIBZENCXX_DEV_PACKAGE}
      )

    install(
        EXPORT zencxx-os
        FILE zencxx-os.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKE_MODULESDIR}
        NAMESPACE zencxx::
        EXPORT_LINK_INTERFACE_LIBRARIES
        COMPONENT ${LIBZENCXX_DEV_PACKAGE}
      )

    # Install header files
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
        FILES_MATCHING PATTERN *.hh
      )
    install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/details
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zencxx/os
        COMPONENT ${LIBZENCXX_DEV_PACKAGE}
        FILES_MATCHING PATTERN *.hh
      )

    # Compile tests only if needed
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()

endif()
