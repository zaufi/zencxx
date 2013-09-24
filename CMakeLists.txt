# kate: hl cmake;
#
# Copyright 2011-2013 by Alex Turbov <i.zaufi@gmail.com>
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
# Include build number if specified
# NOTE This feature used by build server to make a packages that
# would differ from previous build.
if (ZENCXX_BUILD_NUMBER)
    set(ZENCXX_VERSION ${ZENCXX_MAJOR}.${ZENCXX_MINOR}.${ZENCXX_PATCH}.${ZENCXX_BUILD_NUMBER})
else()
    set(ZENCXX_VERSION ${ZENCXX_MAJOR}.${ZENCXX_MINOR}.${ZENCXX_PATCH})
endif()
message(STATUS "Configuring ${PROJECT_NAME} ${ZENCXX_VERSION}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

#---------------------------------------------------------
# Section: Include aux cmake functions
#---------------------------------------------------------

include(CTest)
# Define install destination dirs
include(GNUInstallDirs)
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/zencxx-${ZENCXX_VERSION})
set(CMAKE_INSTALL_PKGLIBDIR ${CMAKE_INSTALL_LIBDIR}/zencxx-${ZENCXX_VERSION})
set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DOCDIR}-${ZENCXX_VERSION})

# Custom (from package's cmake/modules/ directory)
include(AddBoostTests)
# Allow testing using CTest
enable_testing()

include(GenerateExportHeader)

#---------------------------------------------------------
# Section: Find used stuff
#---------------------------------------------------------
set(Boost_USE_MULTITHREADED ON)
find_package(
    Boost 1.53 REQUIRED COMPONENTS
    # Lets keep libs list sorted... :)
    system
    thread
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
# Force the boost::asio to use std::chrono
add_definitions(-DBOOST_ASIO_DISABLE_BOOST_CHRONO)
# Don't want to use any deprecated API
add_definitions(-DBOOST_SYSTEM_NO_DEPRECATED)
add_definitions(-DBOOST_FILESYSTEM_NO_DEPRECATED)
# Use latest available Boost.Thread library
# (stay as close as possible to the C++11 Standard)
add_definitions(-DBOOST_THREAD_VERSION=4)

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

# Include declaration of use_compiler_option()
include(MacroUseCompilerOption)
# Set some compiler options
use_compiler_option(OPTION -pipe LANGUAGE CXX OUTPUT COMPILER_HAS_PIPE_OPTION)
use_compiler_option(OPTION -Wall LANGUAGE CXX OUTPUT COMPILER_HAS_WALL_OPTION)
use_compiler_option(OPTION -std=c++11 LANGUAGE CXX OUTPUT CXX_COMPILER_HAS_CPP11_OPTION)
if(NOT CXX_COMPILER_HAS_CPP11_OPTION)
    message(FATAL_ERROR "C++11 compatible compiler required to build this project (gcc >= 4.8)")
endif()

# If CMAKE_BUILD_TYPE is not set, try to guess it
include(GuessBuildType)

# Setup compiler options depending on build type
message(STATUS "Setting options for ${CMAKE_BUILD_TYPE} build type")
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DebugFull")
    # Show even more warnings in debug mode
    use_compiler_option(OPTION -Wextra LANGUAGE CXX OUTPUT COMPILER_HAS_WALL_OPTION)
    use_compiler_option(OPTION -ggdb3 LANGUAGE CXX OUTPUT COMPILER_HAS_GGDB3_OPTION)
    if(NOT COMPILER_HAS_GGDB3_OPTION)
        use_compiler_option(OPTION -g3 LANGUAGE CXX OUTPUT COMPILER_HAS_G3_OPTION)
    endif()
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

# Set a project-wide name for unit tests executable
# (a target to be build in every tests/ directories)
set(UNIT_TESTS unit_tests)
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/modules/ZenCXXConfig.cmake.in
    ${CMAKE_BINARY_DIR}/ZenCXXConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    PATH_VARS CMAKE_INSTALL_PREFIX CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )
write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/ZenCXXConfigVersion.cmake
    VERSION ${ZENCXX_VERSION}
    COMPATIBILITY AnyNewerVersion
  )

#---------------------------------------------------------
# Section: Package options
# ALERT It should be after subdirs traversal
#       to make sure that all vars r defined!
#---------------------------------------------------------

# Obtain distribution codename according LSB
include(GetDistribInfo)

# Specifying precise versions of some packages which are distinct
# in different distributions.
if(DISTRIB_CODENAME STREQUAL "precise")
  set(BOOST_DEV_VERSION_PKG_DEP "1.48")
elseif(DISTRIB_CODENAME STREQUAL "quantal")
  set(BOOST_DEV_VERSION_PKG_DEP "1.50")
elseif(DISTRIB_CODENAME STREQUAL "raring")
  set(BOOST_DEV_VERSION_PKG_DEP "1.53")
else()
  message(STATUS "WARNING: Target distribution codename is unknown! Do not even try to build .deb packages!")
endif()
set(BOOST_VERSION_PKG_DEP "${BOOST_DEV_VERSION_PKG_DEP}.0")

if(
     CMAKE_BUILD_TYPE STREQUAL "Debug"
  OR CMAKE_BUILD_TYPE STREQUAL "DebugFull"
  OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"
  )
  set(CMAKE_INSTALL_DO_STRIP OFF)
  set(CPACK_BUILD_FLAVOUR "-dbg")
else()
  set(CPACK_BUILD_FLAVOUR "")
endif()

# Include CPack support
include(CPack)
set(CPACK_GENERATOR DEB)
set(CPACK_SOURCE_GENERATOR TBZ2)
include(AddPackage)

# Make sign options available to set from the CLI/GUI
set(
    ZENCXX_SIGN_KEY_ID
    "94CF01DC"
    CACHE STRING "GPG key ID to be used to sign result packages"
  )
set(
    ZENCXX_SIGN_USER_NAME
    "builder"
    CACHE STRING "User name to be used to sign packages (builder)"
  )
set(
    ZENCXX_DEB_REPOSITORY
    "$ENV{HOME}/repo"
    CACHE FILEPATH "Path to repository to deploy .deb packages to"
  )

set_common_package_options(
    PROJECT_VERSION "${ZENCXX_VERSION}"
    VENDOR_NAME "Alex Turbov"
    VENDOR_CONTACT "Alex Turbov <i.zaufi@gmail.com>"
    BUILD_FLAVOUR "${CPACK_BUILD_FLAVOUR}"
    SIGN_BY ${ZENCXX_SIGN_USER_NAME}
    SIGN_WITH ${ZENCXX_SIGN_KEY_ID}
    ALLOW_DEPLOY_PACKAGES
    DEPLOY_TO ${ZENCXX_DEB_REPOSITORY}
  )

set(PKG_VERSION "${ZENCXX_VERSION}-0ubuntu2")

add_package(
    NAME libzencxx
    SUMMARY "ZEN C++ library"
    DESCRIPTION "Redistributable library"
    HOMEPAGE "https://github.com/zaufi/zencxx"
    VERSION "${PKG_VERSION}"
    SECTION "libs"
    DEPENDS
        "libstdc++6 (>= 4.6)"
        "libboost-system${BOOST_VERSION_PKG_DEP}"
        "libboost-thread${BOOST_VERSION_PKG_DEP}"
    REPLACES "libzencxx${CPACK_BUILD_FLAVOUR} (<= ${PKG_VERSION})"
    SIGN_WITH
  )

add_package(
    NAME libzencxx-dev
    SUMMARY "Developer files for ZEN C++ library"
    DESCRIPTION "C++ headers and CMake configuration support files"
    HOMEPAGE "https://github.com/zaufi/zencxx"
    VERSION "${PKG_VERSION}"
    SECTION "devel"
    DEPENDS
        "libzencxx${CPACK_BUILD_FLAVOUR} (= ${PKG_VERSION})"
        "libboost${BOOST_DEV_VERSION_PKG_DEP}-all-dev"
    REPLACES "libzencxx-dev (<= ${PKG_VERSION})"
  )

if(NOT NO_DOXY_DOCS OR NOT NO_DOXY_DOCS STREQUAL "ON")
    add_package(
        NAME libzencxx-doc
        SUMMARY "ZEN C++ library API documentation"
        DESCRIPTION "Doxygened library API documentation"
        HOMEPAGE "https://github.com/zaufi/zencxx"
        VERSION "${PKG_VERSION}"
        SECTION "devel"
        REPLACES "libzencxx-doc (<= ${PKG_VERSION})"
        PRE_BUILD doxygen
      )
endif()

#---------------------------------------------------------
# Section: Dive into subdirs after main configuration
#---------------------------------------------------------
add_subdirectory(zencxx)

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
include(DefineCheckHeadersTarget)
# Setup doxygen
set(DOXYGEN_PROJECT_BRIEF "\"ZenCXX: C++11 reusable code compilation\"")
if(EXISTS $ENV{HOME}/.cache/doxygen/libstdc++.tag)
    set(DOXYGEN_TAGFILES "$ENV{HOME}/.cache/doxygen/libstdc++.tag=http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/")
endif()
set(DOXYGEN_PREDEFINED "DOXYGEN_RUNNING ZENCXX_EXPORT ZENCXX_NO_EXPORT ZENCXXOS_EXPORT ZENCXXOS_NO_EXPORT")
set(DOXYGEN_PROJECT_NUMBER ${ZENCXX_VERSION})
include(DefineDoxyDocsTargetIfPossible)

include(DefineSkeletonGenerationTargetsIfPossible)
define_skeleton_generation_targets(
    PROJECT_LICENSE LGPL
    PROJECT_NAMESPACE "zencxx"
    PROJECT_PREFIX "ZenCxx"
    PROJECT_OWNER "Alex Turbov and contributors"
    PROJECT_YEARS "2010-2013"
    ENABLE_TESTS
  )

include(UsePCHFile)
use_pch_file(
    PCH_FILE ${CMAKE_BINARY_DIR}/zencxx-pch.hh
    EXCLUDE_DIRS cmake docs tests
    # EXCLUDE_HEADERS ext/hash_set ext/hash_map
  )


#---------------------------------------------------------
# Section: Top level installs
#---------------------------------------------------------

install(
    FILES ${CMAKE_BINARY_DIR}/ZenCXXConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/ZenCXXConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zencxx-${ZENCXX_VERSION}
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )

if(NOT NO_DOXY_DOCS OR NOT NO_DOXY_DOCS STREQUAL "ON")
    install(
        DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}/html
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT ${LIBZENCXX_DOC_PACKAGE}
        PATTERN "*.html"
        PATTERN "*.svg"
        PATTERN "*.ttf"
        PATTERN "*.png"
        PATTERN "*.css"
        PATTERN "*.map" EXCLUDE
        PATTERN "*.md5" EXCLUDE
        PATTERN "*.dot" EXCLUDE
      )
endif()

install(
    FILES README.md LICENSE
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
    COMPONENT ${LIBZENCXX_PACKAGE}
  )
