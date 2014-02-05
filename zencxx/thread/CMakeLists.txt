add_compiler_export_flags()

set(
    ZENCXX_THREAD_SOURCES
    predefined_lock_types.cc
    et/details/lock_matrix.cc
    et/details/thread_lock_tracker.cc
    et/default_scheduler.cc
    et/fifo_adaptor.cc
    et/parameterized_adaptor.cc
    et/priority_queue_adaptor.cc
    et/unilock.cc
  )

add_library(
    zencxxthread
    ${ZENCXX_LIB_TYPE}
    ${ZENCXX_THREAD_SOURCES}
  )

target_link_libraries(
    zencxxthread
    zencxxos
  )

generate_export_header(zencxxthread EXPORT_FILE_NAME details/export.hh)

install(
    TARGETS zencxxthread
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT ${LIBZENCXX_PACKAGE}
  )

file(GLOB HEADERS *.hh)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zencxx/thread
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )

install(
    DIRECTORY details et ${CMAKE_CURRENT_BINARY_DIR}/details
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zencxx/thread
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
    FILES_MATCHING PATTERN "*.hh"
  )

# Compile tests only if needed
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
