cmake_minimum_required(VERSION 3.21)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
  AES
  VERSION 1.0.0
  DESCRIPTION "AES Project for Network Security"
  LANGUAGES CXX
  )

# Generate compile_commands.json to make it easier to work with clang based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set build type
set(CMAKE_BUILD_TYPE
      Debug
      CACHE STRING "Choose the type of build." FORCE)

# Option that sets if warnings are treated as errors
option(WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)

# Set the possible values of build type for cmake-gui, ccmake
set_property(
CACHE CMAKE_BUILD_TYPE
PROPERTY STRINGS
         "Debug"
         "Release"
         "MinSizeRel"
         "RelWithDebInfo")

# Create target for compiler options
add_library(aes_warnings INTERFACE)

set(CLANG_WARNINGS
    -Wall
    -Wextra # reasonable and standard
    -Wshadow # warn the user if a variable declaration shadows one from a parent context
    -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps
    # catch hard to track down memory errors
    -Wold-style-cast # warn for c-style casts
    -Wcast-align # warn for potential performance problem casts
    -Wunused # warn on anything being unused
    -Woverloaded-virtual # warn if you overload (not override) a virtual function
    -Wpedantic # warn if non-standard C++ is used
    -Wconversion # warn on type conversions that may lose data
    -Wsign-conversion # warn on sign conversions
    -Wnull-dereference # warn if a null dereference is detected
    -Wdouble-promotion # warn if float is implicit promoted to double
    -Wformat=2 # warn on security issues around functions that format output (ie printf)
    -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
)
set(GCC_WARNINGS
    ${CLANG_WARNINGS}
    -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
    -Wduplicated-cond # warn if if / else chain has duplicated conditions
    -Wduplicated-branches # warn if if / else branches have duplicated code
    -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
    -Wuseless-cast # warn if you perform a cast to the same type
    -Wsuggest-override # warn if an overridden member function is not marked 'override' or 'final'
)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(PROJECT_WARNINGS_CXX ${CLANG_WARNINGS})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNINGS_CXX ${GCC_WARNINGS})
endif()

target_compile_options(
    aes_warnings
    INTERFACE 
    # C++ warnings
    $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_WARNINGS_CXX}>
)

# enable clang tidy macro
macro(enable_clang_tidy target WARNINGS_AS_ERRORS)
  find_program(CLANGTIDY clang-tidy)
  if(CLANGTIDY)
    # construct the clang-tidy command line
    set(CLANG_TIDY_OPTIONS
        ${CLANGTIDY}
        -extra-arg=-Wno-unknown-warning-option
        -extra-arg=-Wno-ignored-optimization-argument
        -extra-arg=-Wno-unused-command-line-argument
        -p
        -fix)
    # set standard
    if(NOT
       "${CMAKE_CXX_STANDARD}"
       STREQUAL
       "")
      if("${CLANG_TIDY_OPTIONS_DRIVER_MODE}" STREQUAL "cl")
        set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=/std:c++${CMAKE_CXX_STANDARD})
      else()
        set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=-std=c++${CMAKE_CXX_STANDARD})
      endif()
    endif()

    # set warnings as errors
    if(${WARNINGS_AS_ERRORS})
      list(APPEND CLANG_TIDY_OPTIONS -warnings-as-errors=*)
    endif()

    message("Also setting clang-tidy globally")
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS})
  else()
    message(${WARNING_MESSAGE} "clang-tidy requested but executable not found")
  endif()
endmacro()
enable_clang_tidy(options ${WARNINGS_AS_ERRORS})

# Makes output colorful
add_compile_options(-fdiagnostics-color=always)

# Set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Add our src folder to build
add_subdirectory(src)

# Enable tests
enable_testing()
add_subdirectory(test)
add_test(NAME AES_UNIT_TEST COMMAND AES_UNIT_TEST)
set_tests_properties(AES_UNIT_TEST PROPERTIES TIMEOUT 30) 

