add_compiler_export_flags()

set(
    ZENCXX_SOURCES
    debug/dump_memory.cc
    debug/print/any_manip.cc
    debug/print/builtins.cc
    debug/print/containers.cc
    debug/print/std_chrono.cc
    exception.cc
    thread/predefined_lock_types.cc
    thread/et/details/lock_matrix.cc
    thread/et/details/thread_lock_tracker.cc
    thread/et/default_scheduler.cc
    thread/et/fifo_adaptor.cc
    thread/et/parameterized_adaptor.cc
    thread/et/priority_queue_adaptor.cc
    thread/et/unilock.cc
    ticker.cc
    utility/random.cc
  )

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(ZENCXX_EXTRA_SOURCES debug/details/backtrace_impl_linux.cc)
endif()

add_library(
    zencxx
    ${ZENCXX_SOURCES}
    ${ZENCXX_EXTRA_SOURCES}
  )

generate_export_header(
    zencxx
    EXPORT_FILE_NAME details/export.hh
  )

target_link_libraries(
    zencxx
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
  )

export(
    TARGETS zencxx
    NAMESPACE zencxx::
    FILE zencxxshared.cmake
    EXPORT_LINK_INTERFACE_LIBRARIES
  )

set_target_properties(
    zencxx
    PROPERTIES
        VERSION "0.0"
        EXPORT_NAME "shared"
  )

install(
    TARGETS zencxx
    EXPORT zencxxshared
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT ${LIBZENCXX_PACKAGE}
  )

install(
    EXPORT zencxxshared
    DESTINATION ${CMAKE_INSTALL_CMAKE_MODULESDIR}
    NAMESPACE zencxx::
    EXPORT_LINK_INTERFACE_LIBRARIES
  )

file(GLOB HEADERS *.hh)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zencxx
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )

install(
    DIRECTORY debug details utility ${CMAKE_CURRENT_BINARY_DIR}/details
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zencxx
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
    FILES_MATCHING PATTERN "*.hh"
  )

add_subdirectory(mpl)
add_subdirectory(os)
add_subdirectory(type_traits)

# Compile tests only if needed
if(BUILD_TESTING)
    add_subdirectory(debug/tests)
    add_subdirectory(tests)
    add_subdirectory(thread/tests)
endif()
