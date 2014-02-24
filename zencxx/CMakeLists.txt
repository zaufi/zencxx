add_compiler_export_flags()

set(
    ZENCXX_SOURCES
    debug/dump_memory.cc
    debug/print/any_manip.cc
    debug/print/builtins.cc
    debug/print/containers.cc
    debug/print/std_chrono.cc
    exception.cc
    ticker.cc
    utility/random.cc
  )

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(ZENCXX_EXTRA_SOURCES debug/details/backtrace_impl_linux.cc)
endif()

add_library(
    zencxx
    ${ZENCXX_LIB_TYPE}
    ${ZENCXX_SOURCES}
    ${ZENCXX_EXTRA_SOURCES}
  )

target_link_libraries(
    zencxx
    PUBLIC
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
  )

generate_export_header(zencxx EXPORT_FILE_NAME details/export.hh)

#BEGIN Export Component
set_target_properties(
    zencxx
    PROPERTIES EXPORT_NAME "shared"
  )
export(
    TARGETS zencxx
    NAMESPACE zencxx::
    FILE zencxx-shared.cmake
    EXPORT_LINK_INTERFACE_LIBRARIES
  )
#END Export Component

# Install target library
install(
    TARGETS zencxx
    EXPORT zencxx-shared
    COMPONENT ${LIBZENCXX_PACKAGE}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

# Install cmake files
install(
    EXPORT zencxx-shared
    FILE zencxx-shared.cmake
    DESTINATION ${CMAKE_INSTALL_CMAKE_MODULESDIR}
    NAMESPACE zencxx::
    EXPORT_LINK_INTERFACE_LIBRARIES
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )

# Install header files
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
    FILES_MATCHING PATTERN *.hh
  )

add_subdirectory(mpl)
add_subdirectory(os)
add_subdirectory(thread)
add_subdirectory(type_traits)

# Compile tests only if needed
if(BUILD_TESTING)
    add_subdirectory(debug/tests)
    add_subdirectory(tests)
endif()
