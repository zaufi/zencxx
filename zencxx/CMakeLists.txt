set(
    ZENCXX_SOURCES
    debug/dump_memory.cc
    exception.cc
    os/daemon_application.cc
    os/exception.cc
    os/signal/mask.cc
  )

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(ZENCXX_EXTRA_SOURCES debug/details/backtrace_impl_linux.cc)
endif()

add_library(
    zencxx
    ${ZENCXX_SOURCES}
    ${ZENCXX_EXTRA_SOURCES}
  )

target_link_libraries(
    zencxx
    ${Boost_SYSTEM_LIBRARY}
  )

install(
    TARGETS zencxx
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
    DIRECTORY debug os
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
    FILES_MATCHING PATTERN "*.hh"
  )

add_subdirectory(mpl)
add_subdirectory(type_traits)

# Compile tests only if needed
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
