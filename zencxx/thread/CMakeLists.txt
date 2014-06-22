add_library(
    zencxxthread
    ${ZENCXX_LIB_TYPE}
    predefined_lock_types.cc
    et/details/lock_matrix.cc
    et/details/thread_lock_tracker.cc
    et/default_scheduler.cc
    et/fifo_adaptor.cc
    et/parameterized_adaptor.cc
    et/priority_queue_adaptor.cc
    et/unilock.cc
  )

target_link_libraries(
    zencxxthread
    PUBLIC
        zencxxos
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
  )

generate_export_header(zencxxthread EXPORT_FILE_NAME details/export.hh)

#BEGIN Export Component
set_target_properties(
    zencxxthread
    PROPERTIES EXPORT_NAME "thread"
  )
export(
    TARGETS zencxxthread zencxxos zencxx
    NAMESPACE zencxx::
    FILE zencxx-thread.cmake
    EXPORT_LINK_INTERFACE_LIBRARIES
  )
write_export_dependencies(TARGET zencxxthread DEPENDENCIES os)
#END Export Component

# Install target library
install(
    TARGETS zencxxthread
    EXPORT zencxx-thread
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    COMPONENT ${LIBZENCXX_PACKAGE}
  )

# Install cmake files
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/zencxx-thread-dependencies.cmake"
    DESTINATION "${CMAKE_INSTALL_CMAKE_MODULESDIR}"
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )

install(
    EXPORT zencxx-thread
    FILE zencxx-thread.cmake
    DESTINATION "${CMAKE_INSTALL_CMAKE_MODULESDIR}"
    NAMESPACE zencxx::
    EXPORT_LINK_INTERFACE_LIBRARIES
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )

# Install header files
file(GLOB HEADERS *.hh)
install(
    FILES ${HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/zencxx/thread"
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )

install(
    DIRECTORY details et "${CMAKE_CURRENT_BINARY_DIR}/details"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/zencxx/thread"
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
    FILES_MATCHING PATTERN "*.hh"
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
