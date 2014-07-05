# kate: hl cmake;
#
# Copyright 2011-2014 by Alex Turbov <i.zaufi@gmail.com>
#

#---------------------------------------------------------
# Section: Init cmake
#---------------------------------------------------------
cmake_minimum_required(VERSION 2.8.12)

# Enable non unique target names in different subdirs
# (used for unit tests)
cmake_policy(SET CMP0002 OLD)
cmake_policy(SET CMP0022 NEW)

project(zencxx CXX)

set(ZENCXX_MAJOR 0)
set(ZENCXX_MINOR 2)
set(ZENCXX_PATCH 1)
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
# Fix some paths to allow this package to be multislotted (in terms of Gentoo)
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/zencxx-${ZENCXX_VERSION})
set(CMAKE_INSTALL_PKGLIBDIR ${CMAKE_INSTALL_LIBDIR}/zencxx-${ZENCXX_VERSION})
set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DOCDIR}-${ZENCXX_VERSION})
set(CMAKE_INSTALL_CMAKE_MODULESDIR ${CMAKE_INSTALL_LIBDIR}/zencxx-${ZENCXX_VERSION}/cmake)

# Custom (from package's cmake/modules/ directory)
include(AddBoostTests)
# Allow testing using CTest
enable_testing()

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(WriteExportDependencies)

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

if(WIN32)
    set(_ZENCXX_DEFAULT_USE_CURSES OFF)
else()
    set(_ZENCXX_DEFAULT_USE_CURSES ON)
endif()
option(ENABLE_CURSES "Use curses library to get some terminal capabilities (like colors)" ${_ZENCXX_DEFAULT_USE_CURSES})
if(ENABLE_CURSES)
    find_package(Curses)
endif()

#---------------------------------------------------------
# Section: Configure builing process
#---------------------------------------------------------

option(ZENCXX_BUILD_SHARED_LIBS "Build shared libraries" OFF)
if(ZENCXX_BUILD_SHARED_LIBS)
    set(ZENCXX_LIB_TYPE SHARED)
    message(STATUS "Build shared libraries: enabled")
endif()

# NOTE Order is important!
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${Boost_INCLUDE_DIRS})

# Tell to boost::result_of to use decltype to decay return type of callable.
# NOTE This would enabel to use C++11 labmda expressions w/ boost::rage adaptors
add_definitions(-DBOOST_RESULT_OF_USE_DECLTYPE)
# Force the boost::asio to use std::chrono instead of boost::chrono
add_definitions(-DBOOST_ASIO_DISABLE_BOOST_CHRONO)
add_definitions(-DBOOST_ASIO_HAS_STD_CHRONO)
# Don't want to use any deprecated API
add_definitions(-DBOOST_SYSTEM_NO_DEPRECATED)
add_definitions(-DBOOST_FILESYSTEM_NO_DEPRECATED)
# Use latest available Boost.Thread library
# (stay as close as possible to the C++11 Standard)
add_definitions(-DBOOST_THREAD_VERSION=4)
# TODO Move that defines above to an internal config.h file!

# If CMAKE_BUILD_TYPE is not set, try to guess it now
include(GuessBuildType)

# Do compiler dependent options tuning
message(STATUS "Configuring options for C++ compiler: ${CMAKE_CXX_COMPILER_ID} on ${CMAKE_SYSTEM_NAME}")
if(UNIX AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Allow GNU extensions
    add_definitions(-D_GNU_SOURCE)

    # BOOST_PP (boost <= 1.55.0) do not recognize clang as a compiler w/ variadic macros support,
    # so lets force it!
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_definitions(-DBOOST_PP_VARIADICS=1)
    endif()

    # TODO Add STL debug mode (-D_GLIBCXX_DEBUG) in debug builds, but
    # unfortunately this compiler option must be synchronized with boost
    # binary libraries.
    #
    # NOTE One may use GLIBCXX_FORCE_NEW to disable any memory caching in STL allocators.
    #
    # TODO Also it would be nice to enable STL profiling mode (-D_GLIBCXX_PROFILE)
    # (read more http://gcc.gnu.org/onlinedocs/libstdc++/manual/profile_mode.html)

    # Include declaration of use_compiler_option()
    include(UseCompilerOption)

    # Set some compiler options
    use_compiler_option(-pipe OUTPUT COMPILER_HAS_PIPE_OPTION)
    use_compiler_option(-Wall OUTPUT COMPILER_HAS_WALL_OPTION)
    use_compiler_option(-std=c++11 OUTPUT CXX_COMPILER_HAS_CPP11_OPTION)
    if(NOT CXX_COMPILER_HAS_CPP11_OPTION)
        message(FATAL_ERROR "C++11 compatible compiler required to build this project (gcc >= 4.8 or clang >= 3.4)")
    endif()

    if(WITH_SANITIZER)
        set(_asan_used FALSE)
        set(_tsan_used FALSE)
        foreach(_san ${WITH_SANITIZER})
            string(TOUPPER "${_san}" WITH_SANITIZER_UP)
            if(_san STREQUAL thread)
                if(_asan_used)
                    message(FATAL_ERROR "Thread and address sanitizers couldn't be used both at the same time")
                endif()
                if(NOT ZENCXX_BUILD_SHARED_LIBS)
                    use_compiler_option(-fPIC OUTPUT COMPILER_HAS_FPIC_OPTION)
                    use_compiler_option(-pie OUTPUT CXX_COMPILER_HAS_PIE_OPTION)
                    if(NOT (CXX_COMPILER_HAS_PIE_OPTION AND COMPILER_HAS_FPIC_OPTION))
                        message(FATAL_ERROR "Thread sanitizer require -pie and -fPIC options for static libraries variant")
                    endif()
                endif()
                set(_tsan_used TRUE)
            elseif(_san STREQUAL address)
                if(_tsan_used)
                    message(FATAL_ERROR "Thread and address sanitizers couldn't be used both at the same time")
                endif()
                set(_asan_used TRUE)
            endif()
            use_compiler_option(
                -fsanitize=${_san}
                OUTPUT CXX_COMPILER_HAS_FSANITIZE_${WITH_SANITIZER_UP}_OPTION
                ALSO_PASS_TO_LINKER
            )
        endforeach()
    endif()

    # Do not link w/ libraries which isn't provide undefined symbols.
    # (they are specified as dependencies for other targets from this
    # project, but listed as unused by `ldd -u binary`)
    use_compiler_option("-Wl,--as-needed" OUTPUT LINKER_HAS_AS_NEEDED)
    use_compiler_option("-Wl,--no-undefined" OUTPUT LINKER_HAS_NO_UNDEFINED)

    # Setup compiler options depending on build type
    message(STATUS "Setting options for ${CMAKE_BUILD_TYPE} build type")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DebugFull")
        # Show even more warnings in debug mode
        use_compiler_option(-Wextra OUTPUT COMPILER_HAS_WALL_OPTION)
        use_compiler_option(-ggdb3 OUTPUT COMPILER_HAS_GGDB3_OPTION)
        if(NOT COMPILER_HAS_GGDB3_OPTION)
            use_compiler_option(-g3 OUTPUT COMPILER_HAS_G3_OPTION)
        endif()
    else()
        # More linker optimizations in release mode
        use_compiler_option(-g3 OUTPUT COMPILER_HAS_G3_OPTION)
        use_compiler_option("-Wl,-O1" OUTPUT LINKER_HAS_O1)
        use_compiler_option("-Wl,--sort-common" OUTPUT LINKER_HAS_SORT_COMMON)
        # TODO Provide more compiler options (like -march or -ftree-vectorize)
        # TODO Use LTO
    endif()
endif()

# Add -fvisibility option if needed
add_compiler_export_flags()

#---------------------------------------------------------
# Section: Misc actions
#---------------------------------------------------------

# Bring add_compile_tests() into a scope
include(AddCompileTests)

# Set a project-wide name for unit tests executable
# (a target to be build in every tests/ directories)
set(UNIT_TESTS unit_tests)

# Render a CMake finder
configure_package_config_file(
    zencxx-config.cmake.in
    zencxx-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKE_MODULESDIR}
    PATH_VARS
        CMAKE_INSTALL_PREFIX
        CMAKE_INSTALL_CMAKE_MODULESDIR
        CMAKE_INSTALL_INCLUDEDIR
        CMAKE_INSTALL_LIBDIR
        CMAKE_INSTALL_PKGLIBDIR
  )
write_basic_package_version_file(
    zencxx-config-version.cmake
    VERSION ${ZENCXX_VERSION}
    COMPATIBILITY AnyNewerVersion
  )

#---------------------------------------------------------
# Section: Package options
# ALERT It should be after subdirs traversal
#       to make sure that all vars r defined!
#---------------------------------------------------------

# Obtain a distribution codename according LSB
include(GetDistribInfo)

# Specifying precise versions of some packages which are distinct
# in different distributions.
if(DISTRIB_CODENAME STREQUAL "precise")
    set(BOOST_DEV_VERSION_PKG_DEP "1.48.0")
elseif(DISTRIB_CODENAME STREQUAL "quantal")
    set(BOOST_DEV_VERSION_PKG_DEP "1.50.0")
elseif(DISTRIB_CODENAME STREQUAL "raring")
    set(BOOST_DEV_VERSION_PKG_DEP "1.53.0")
elseif(DISTRIB_CODENAME STREQUAL "saucy")
    set(BOOST_DEV_VERSION_PKG_DEP "1.54.0")
elseif(DISTRIB_CODENAME STREQUAL "gentoo")
    # Do nothing
else()
    message(STATUS "WARNING: Target distribution codename is unknown! Will not build binary packages!")
endif()

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
set(CPACK_SOURCE_GENERATOR TBZ2)
include(CPack)

if(DISTRIB_PKG_FMT STREQUAL "DEB")
    set(CPACK_GENERATOR ${DISTRIB_PKG_FMT})
    set(REPREPRO_CUSTOM_OPTIONS "--ask-passphrase")
    include(AddPackage)

    # Make sign options available to set from the CLI/GUI
    set(
        ZENCXX_SIGN_KEY_ID
        "37925FE2"
        CACHE STRING "GPG key ID to be used to sign result packages"
      )
    set(
        ZENCXX_SIGN_USER_NAME
        "builder"
        CACHE STRING "User name to be used to sign packages (builder)"
      )
    set(
        ZENCXX_DEB_REPOSITORY
        "$ENV{HOME}/ubuntu.repo"
        CACHE FILEPATH "Path to repository to deploy .deb packages to"
      )

    set_common_package_options(
        PROJECT_VERSION "${ZENCXX_VERSION}"
        VENDOR_NAME "Alex Turbov"
        VENDOR_CONTACT "Alex Turbov <i.zaufi@gmail.com>"
        BUILD_FLAVOUR "${CPACK_BUILD_FLAVOUR}"
        PACKAGE_INSTALL_PREFIX "/usr"
        README_FILE ${PROJECT_SOURCE_DIR}/README.md
        LICENSE_FILE ${PROJECT_SOURCE_DIR}/LICENSE
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

# Setup doxygen
set(DOXYGEN_PROJECT_BRIEF "\"ZenCXX: C++11 reusable code compilation\"")
if(EXISTS $ENV{HOME}/.cache/doxygen/libstdc++.tag)
    set(DOXYGEN_TAGFILES "$ENV{HOME}/.cache/doxygen/libstdc++.tag=http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/")
endif()
set(DOXYGEN_PREDEFINED "DOXYGEN_RUNNING ZENCXX_EXPORT ZENCXX_NO_EXPORT ZENCXXOS_EXPORT ZENCXXOS_NO_EXPORT")
set(DOXYGEN_PROJECT_NUMBER ${ZENCXX_VERSION})
set(DOXYGEN_EXCLUDE_PATTERNS zencxx-pch.hh)
include(DefineDoxyDocsTargetIfPossible)

include(DefineSkeletonGenerationTargetsIfPossible)
define_skeleton_generation_targets(
    PROJECT_LICENSE LGPL
    PROJECT_NAMESPACE "zencxx"
    PROJECT_PREFIX "ZenCxx"
    PROJECT_OWNER "Alex Turbov and contributors"
    PROJECT_YEARS "2010-2014"
    ENABLE_TESTS
    USE_PRAGMA_ONCE
  )

include(AddOpenTarget)
add_open_target(
    show-api-documentation
    ${CMAKE_BINARY_DIR}/doc/html/index.html
    DEPENDS doxygen
    COMMENT "Open API documentation"
  )

include(UsePCHFile)
use_pch_file(
    PCH_FILE ${PROJECT_BINARY_DIR}/zencxx-pch.hh
    EXCLUDE_DIRS cmake docs tests
    # EXCLUDE_HEADERS ext/hash_set ext/hash_map
  )

#---------------------------------------------------------
# Section: Top level installs
#---------------------------------------------------------

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

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/zencxx-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/zencxx-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_CMAKE_MODULESDIR}
    COMPONENT ${LIBZENCXX_DEV_PACKAGE}
  )
