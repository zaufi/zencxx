add_compiler_export_flags()

set(
    ZENCXX_SOURCES
    debug/dump_memory.cc
    exception.cc
    ticker.cc
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

set_target_properties(
    zencxx
    PROPERTIES VERSION "0.0"
  )

install(
    TARGETS zencxx
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT ${LIBZENCXX_PACKAGE}
  )

file(GLOB HEADERS *.hh)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zencxx
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )

install(
    DIRECTORY debug os ${CMAKE_CURRENT_BINARY_DIR}/details
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zencxx
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
    FILES_MATCHING PATTERN "*.hh"
  )

add_subdirectory(mpl)
add_subdirectory(os)
add_subdirectory(type_traits)

# Compile tests only if needed
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
