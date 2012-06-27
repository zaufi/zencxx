set(HEADERS has_left_shift.hh)

install(
  FILES ${HEADERS}
  DESTINATION include
  COMPONENT ${HDONLY_PACKAGE_NAME}
)

# Build test if requested
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
