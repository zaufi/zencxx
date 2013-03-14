# kate: hl cmake;
#
# Copyright 2011 by Alex Turbov <i.zaufi@gmail.com>
#

#---------------------------------------------------------
# Section: Init cmake
#---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

# Enable non unique target names in different subdirs
# (used for unit tests)
cmake_policy(SET CMP0002 OLD)

project(libzencxx CXX)

set(ZENCXX_MAJOR 0)
set(ZENCXX_MINOR 1)
set(ZENCXX_PATCH 0)
set(ZENCXX_VERSION ${ZENCXX_MAJOR}.${ZENCXX_MINOR}.${ZENCXX_PATCH})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

#---------------------------------------------------------
# Section: Include aux cmake functions
#---------------------------------------------------------

# Standard
include(CTest)
include(GNUInstallDirs)
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/zencxx-${ZENCXX_VERSION}/zencxx)
set(CMAKE_INSTALL_PKGLIBDIR ${CMAKE_INSTALL_LIBDIR}/zencxx-${ZENCXX_VERSION})

# Custom (from package's cmake/modules/ directory)
include(AddBoostTests)
# Allow testing using CTest
enable_testing()

#---------------------------------------------------------
# Section: Find used stuff
#---------------------------------------------------------
find_package(
    Boost 1.49 REQUIRED COMPONENTS
    # Lets keep libs list sorted... :)
    unit_test_framework
  )

#---------------------------------------------------------
# Section: Configure builing process
#---------------------------------------------------------

# Allow GNU extensions
add_definitions(-D_GNU_SOURCE)

# NOTE Order is important!
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${Boost_INCLUDE_DIRS})
# Tell to boost::result_of to use decltype to decay return type of callable.
# NOTE This would enabel to use C++11 labmda expressions w/ boost::rage adaptors
add_definitions(-DBOOST_RESULT_OF_USE_DECLTYPE)

# TODO Add STL debug mode (-D_GLIBCXX_DEBUG) in debug builds, but
# unfortunately this compiler option must be synchronized with boost
# binary libraries.
#
# NOTE One may use GLIBCXX_FORCE_NEW to disable any memory caching in STL allocators.
#
# TODO Also it would be nice to enable STL profiling mode (-D_GLIBCXX_PROFILE)
# (read more http://gcc.gnu.org/onlinedocs/libstdc++/manual/profile_mode.html)

# Do not link w/ libraries which isn't provide undefined symbols.
# (they are specified as dependencies for other targets from this
# project, but listed as unused by `ldd -u binary`)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_MODULE_LINKER_FLAGS}")
# Set generic compiler flags
set(CMAKE_CXX_FLAGS "-pipe -std=c++0x -Wno-multichar ${CMAKE_CXX_FLAGS}")

# If CMAKE_BUILD_TYPE is not set, assume Debug
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Setup compiler options depending on build type
message(STATUS "Setting options for ${CMAKE_BUILD_TYPE} build type")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Show all warnings in debug mode
    set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -Wall")
    # Allow additional security measures
    add_definitions(-D_FORTIFY_SOURCE=2)
else()
    # More linker optimizations in release mode
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE
        "-Wl,-O1 -Wl,--sort-common ${CMAKE_EXE_LINKER_FLAGS_RELEASE}"
      )
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE
        "-Wl,-O1 -Wl,--sort-common ${CMAKE_MODULE_LINKER_FLAGS_RELEASE}"
      )
endif()

#---------------------------------------------------------
# Section: Misc actions
#---------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CPACK_BUILD_FLAVOUR "-dbg")
else()
    set(CPACK_BUILD_FLAVOUR "")
endif()

# Set a project-wide name for unit tests executable
# (a target to be build in every tests/ directories)
set(UNIT_TESTS unit_tests)
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/modules/LibZenCXXConfig.cmake.in
    ${CMAKE_BINARY_DIR}/LibZenCXXConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    PATH_VARS CMAKE_INSTALL_PREFIX CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
  )
write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/LibZenCXXConfigVersion.cmake
    VERSION ${ZENCXX_VERSION}
    COMPATIBILITY AnyNewerVersion
  )
install(
    FILES ${CMAKE_BINARY_DIR}/LibZenCXXConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/LibZenCXXConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${ZENCXX_VERSION}
  )

#---------------------------------------------------------
# Section: Define some helpful targets (using included)
#          functions.
# ALERT It should be at the bottom!
#       To make sure all vars r defined!
#---------------------------------------------------------

get_directory_property(CHECK_HEADER_DEFINITIONS_LIST COMPILE_DEFINITIONS)
foreach(_define ${CHECK_HEADER_DEFINITIONS_LIST})
    set(CHECK_HEADER_DEFINITIONS "-D${_define} ${CHECK_HEADER_DEFINITIONS}")
endforeach()
include(DefineDoxyDocsTargetIfPossible)
include(DefineCheckHeadersTarget)
include(DefineSkeletonGenerationTargetsIfPossible)

#---------------------------------------------------------
# Section: Package options
# ALERT It should be after subdirs traversal
#       to make sure that all vars r defined!
#---------------------------------------------------------
set(CPACK_GENERATOR DEB)
set(CPACK_SOURCE_GENERATOR TBZ2)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${ZENCXX_VERSION}")

# Include CPack support
include(CPack)
include(AddPackage)

add_package(
    NAME libzencxx-dev
    SUMMARY "ZEN C++ library: Header only libraries"
    DESCRIPTION "Libraries that doesn't require building"
    PACKAGE_VERSION 0ubuntu1
    DEPENDS boost
    SET_DEFAULT_CONFIG_CPACK
  )

#---------------------------------------------------------
# Section: Dive into subdirs after main configuration
#---------------------------------------------------------
add_subdirectory(zencxx)
