# Copyright 2011 by Alex Turbov <i.zaufi@gmail.com>
#
# Command to add a target to make a specified package.
#
# Example:
#
#   add_package(
#       NAME libzencxx
#       SUMMARY "C++11 reusable code collection"
#       DESCRIPTION "Header only libraries"
#       PACKAGE_VERSION 0ubuntu1
#       DEPENDS boost
#       SET_DEFAULT_CONFIG_CPACK
#     )
#
# TODO Add `fakeroot' and `cpack' programs detection
#


include(CMakeParseArguments)

function(add_package)
    set(options SET_DEFAULT_CONFIG_CPACK)
    set(one_value_args NAME SUMMARY DESCRIPTION PACKAGE_VERSION)
    set(multi_value_args DEPENDS)
    cmake_parse_arguments(add_package "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    # Check mandatory parameters
    # 0) package name
    if(NOT add_package_NAME)
        message(FATAL_ERROR "Package name is not given")
    else()
        set(PACKAGE_NAME "${add_package_NAME}")
        string(TOUPPER "${add_package_NAME}" pkg_name)
        string(REPLACE "-" "_" pkg_name "${pkg_name}")
        set(${pkg_name}_PACKAGE "${add_package_NAME}" PARENT_SCOPE)
    endif()
    # 1) package description
    if(NOT add_package_SUMMARY OR NOT add_package_DESCRIPTION)
        message(FATAL_ERROR "Package description and/or summary is not provided")
    else()
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${add_package_SUMMARY}")
        set(CPACK_PACKAGE_DESCRIPTION "${add_package_DESCRIPTION}")
    endif()

    # Check optional parameters
    if (add_package_SET_DEFAULT_CONFIG_CPACK)
        set(config_file CPackConfig.cmake)
    else()
        set(config_file CPack-${add_package_NAME}.cmake)
    endif()
    # package version
    if(NOT add_package_PACKAGE_VERSION)
        set(CPACK_PACKAGE_VERSION "0ubuntu1")
    else()
        set(CPACK_PACKAGE_VERSION "${add_package_PACKAGE_VERSION}")
    endif()
    # dependencies list
    if(add_package_DEPENDS)
        # Form a comma separated list of dependencies from a cmake's list
        string(REPLACE ";" ", " CPACK_DEBIAN_PACKAGE_DEPENDS "${${NAME}_PACKAGE_DEPENDS}")
    endif()

    # Generate a package specific cpack's config file to be used
    # at custom execution command...
    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/cpack/CPackConfig.cmake.in
        ${CMAKE_BINARY_DIR}/${config_file}
      )
    # Add target to create the specified package using just the generated config file
    add_custom_target(
        ${add_package_NAME}-package
        COMMAND cpack --config ${config_file}
        COMMENT "Makeing package ${add_package_NAME}"
      )
endfunction()

# kate: hl cmake;
