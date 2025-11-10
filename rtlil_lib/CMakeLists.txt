cmake_minimum_required(VERSION 3.15)
project(RTLILLibrary VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform-specific compiler flags
if(MSVC)
    # Visual Studio specific flags
    add_compile_options(
        /W3                     # Warning level 3
        /wd4267                 # Disable size_t conversion warnings
        /wd4244                 # Disable int conversion warnings
        /wd4996                 # Disable deprecated function warnings
        /wd4100                 # Disable unused parameter warnings
        /D_CRT_SECURE_NO_WARNINGS
        /EHsc                   # Enable C++ exceptions
        /permissive-           # Standards conformance
    )
    # Disable min/max macros in Windows.h
    add_compile_definitions(NOMINMAX)
else()
    # GCC/Clang flags
    add_compile_options(
        -Wall
        -Wextra
        -Wno-unused-parameter
        -O2
    )
endif()

# Define YOSYS namespace and configuration macros
add_compile_definitions(
    _YOSYS_
    YOSYS_NAMESPACE_BEGIN=namespace\ Yosys\ {
    YOSYS_NAMESPACE_END=}
    YOSYS_NAMESPACE_PREFIX=Yosys::
    YOSYS_NO_IDS_REFCNT
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
)

# Kernel source files
set(KERNEL_SOURCES
    src/rtlil.cc
    src/log.cc
    src/binding.cc
    src/init.cc
    src/fmt.cc
    src/io.cc
    src/rtlil_bufnorm.cc
)

# BigInt library sources
set(BIGINT_SOURCES
    libs/bigint/BigUnsigned.cc
    libs/bigint/BigInteger.cc
    libs/bigint/BigIntegerAlgorithms.cc
    libs/bigint/BigUnsignedInABase.cc
    libs/bigint/BigIntegerUtils.cc
)

# SHA1 library sources
set(SHA1_SOURCES
    libs/sha1/sha1.cpp
)

# Create static library
add_library(rtlil STATIC
    ${KERNEL_SOURCES}
    ${BIGINT_SOURCES}
    ${SHA1_SOURCES}
)

# Set library output name
set_target_properties(rtlil PROPERTIES
    OUTPUT_NAME "rtlil"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Add header files to the library target for IDE support (Visual Studio)
file(GLOB_RECURSE HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hh"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/bigint/*.hh"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/sha1/*.h"
)
target_sources(rtlil PRIVATE ${HEADER_FILES})

# Organize files in Visual Studio solution
if(MSVC)
    # Create source groups for better organization in Visual Studio
    source_group("Header Files\\Kernel" REGULAR_EXPRESSION "include/.*\\.h")
    source_group("Header Files\\BigInt" REGULAR_EXPRESSION "libs/bigint/.*\\.hh")
    source_group("Header Files\\SHA1" REGULAR_EXPRESSION "libs/sha1/.*\\.h")
    source_group("Source Files\\Kernel" REGULAR_EXPRESSION "src/.*\\.cc")
    source_group("Source Files\\BigInt" REGULAR_EXPRESSION "libs/bigint/.*\\.cc")
    source_group("Source Files\\SHA1" REGULAR_EXPRESSION "libs/sha1/.*\\.cpp")
endif()

# Test executables
add_executable(simple_example test/simple_example.cc)
target_link_libraries(simple_example PRIVATE rtlil)
set_target_properties(simple_example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_executable(complex_example test/complex_example.cc)
target_link_libraries(complex_example PRIVATE rtlil)
set_target_properties(complex_example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Add custom target to run tests
add_custom_target(run_simple_test
    COMMAND simple_example
    DEPENDS simple_example
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMENT "Running simple example test..."
)

add_custom_target(run_complex_test
    COMMAND complex_example
    DEPENDS complex_example
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMENT "Running complex example test..."
)

add_custom_target(run_all_tests
    DEPENDS run_simple_test run_complex_test
    COMMENT "Running all tests..."
)

# Installation rules (optional)
install(TARGETS rtlil
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include/rtlil
    FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY libs/bigint/
    DESTINATION include/rtlil/bigint
    FILES_MATCHING PATTERN "*.hh"
)

install(TARGETS simple_example complex_example
    RUNTIME DESTINATION bin
)

# Print configuration summary
message(STATUS "==========================================")
message(STATUS "RTLIL Library Configuration")
message(STATUS "==========================================")
message(STATUS "CMake version: ${CMAKE_VERSION}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
if(MSVC)
    message(STATUS "MSVC version: ${MSVC_VERSION}")
    message(STATUS "Visual Studio: ${CMAKE_GENERATOR}")
endif()
message(STATUS "==========================================")
