# - Check if given sources are compilable
# To do check just `make test`
#
# TODO Handle additional CMAKE_REQUIRED_INCLUDES
#

#=============================================================================
# Copyright 2014 by Alex Turbov <i.zaufi@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file LICENSE for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of this repository, substitute the full
#  License text for the above reference.)

include(CMakeParseArguments)

set(_ADD_COMPILE_TESTS_BASE_DIR "${CMAKE_CURRENT_LIST_DIR}")

function(add_compile_tests)
    set(options DEBUG)
    set(one_value_args STRIP_LEADING_PATH LANGUAGE PREFIX)
    set(multi_value_args SOURCES)
    cmake_parse_arguments(add_compile_tests "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    # Prevent running w/o source files
    if(NOT add_compile_tests_SOURCES)
        message(FATAL_ERROR "No source files given to `add_compile_tests'")
    endif()
    if(NOT add_compile_tests_LANGUAGE)
        set(_lang "CXX")
        set(_test_ext "cc")
        set(_compiler "${CMAKE_CXX_COMPILER}")
    elseif(add_compile_tests_LANGUAGE STREQUAL "CXX")
        set(_lang "CXX")
        set(_test_ext "cc")
        set(_compiler "${CMAKE_CXX_COMPILER}")
    elseif(add_compile_tests_LANGUAGE STREQUAL "C")
        set(_lang "C")
        set(_test_ext "c")
        set(_compiler "${CMAKE_C_COMPILER}")
    else()
        # TODO Handle other languages if needed...
        message(FATAL_ERROR "Unsupported language given to `add_compile_tests'")
    endif()

    if(NOT add_compile_tests_PREFIX)
        set(add_compile_tests_PREFIX "check_compile")
    endif()

    # Prepare compiler command
    # 0. Remove compiler executable
    string(
        REPLACE
            "<CMAKE_${_lang}_COMPILER>"
            ""
            _compile_options
            "${CMAKE_${_lang}_COMPILE_OBJECT}"
      )
    # 1. Get defines
    get_directory_property(_defines_list COMPILE_DEFINITIONS)
    foreach(_def ${_defines_list})
        # TODO Handle -D option for other compilers
        set(_defines "${_defines} -D${_def}")
    endforeach()
    string(REPLACE "<DEFINES>" "${_defines}" _compile_options "${_compile_options}")
    # 2. Get #include paths
    get_directory_property(_includes_list INCLUDE_DIRECTORIES)
    foreach(_inc ${_includes_list})
        # TODO Handle -I option for other compilers
        set(_includes "${_includes} -I${_inc}")
    endforeach()
    # 3. Get other compiler options
    # Take a build type specific options as well
    if(CMAKE_BUILD_TYPE)
        set(_extra_flags "${CMAKE_${_lang}_FLAGS_${CMAKE_BUILD_TYPE}}")
    endif()
    # Form a compiler command w/o particular sources/objects
    string(
        REPLACE "<FLAGS>" "${CMAKE_${_lang}_FLAGS} ${_extra_flags} ${_includes}" _compile_options "${_compile_options}"
      )
    if(add_compile_tests_DEBUG)
        message(STATUS "Prepared compiler command: '${_compile_options}'")
    endif()

    # Create an output directory for tests
    set(_output_dir "${CMAKE_CURRENT_BINARY_DIR}/CompileTests")
    if(NOT EXISTS "${_output_dir}")
        file(MAKE_DIRECTORY "${_output_dir}")
    endif()

    # Register individual tests
    foreach(_src ${add_compile_tests_SOURCES})
        # Split filename into components
        get_filename_component(_src_dir ${_src} DIRECTORY)
        get_filename_component(_src_name ${_src} NAME)

        # Strip base source path
        string(REPLACE "${add_compile_tests_STRIP_LEADING_PATH}/" "" _src_rel_dir "${_src_dir}")

        # Prepare some variables to be rendered (hardcoded)
        set(_fake_cc "${_output_dir}/check_${_src_name}.${_test_ext}")

        # Render cmake code for tests
        configure_file(
            "${_ADD_COMPILE_TESTS_BASE_DIR}/compile_test.cmake.in"
            "${_output_dir}/compile_check_${_src_name}.cmake"
            @ONLY
          )

        # Register a new test
        string(MAKE_C_IDENTIFIER "${_src_rel_dir}/${_src_name}" _src_test_name)
        if(add_compile_tests_DEBUG)
            message(STATUS "Registering test: '${add_compile_tests_PREFIX}_${_src_test_name}'")
        endif()
        add_test(
            NAME "${add_compile_tests_PREFIX}_${_src_test_name}"
            COMMAND ${CMAKE_COMMAND} -P "${_output_dir}/compile_check_${_src_name}.cmake"
          )
    endforeach()
endfunction()

# X-Chewy-RepoBase: https://raw.githubusercontent.com/mutanabbi/chewy-cmake-rep/master/
# X-Chewy-Path: AddCompileTests.cmake
# X-Chewy-Version: 1.0
# X-Chewy-Description: Check if source(s) can be compiled well (or not)
# X-Chewy-AddonFile: check_compile.cmake.in
