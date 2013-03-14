set(
    ZENCXX_SOURCES
    exception.cc
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
    DIRECTORY debug
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
