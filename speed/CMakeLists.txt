cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckCXXCompilerFlag)
include(../cmake/CPM.cmake)

CPMAddPackage("gh:lemire/counters#v3.1.0")

add_executable(rng src/rng.cpp)
target_include_directories(rng PRIVATE ${PROJECT_SOURCE_DIR}/source)
target_link_libraries(rng PRIVATE counters::counters)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
  if(COMPILER_SUPPORTS_MARCH_NATIVE)
    target_compile_options(rng PRIVATE -march=native)
  endif()
endif()
