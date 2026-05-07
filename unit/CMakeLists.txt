cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(CheckCCompilerFlag)

add_executable(v8equiv src/v8equiv.c)
target_include_directories(v8equiv PRIVATE ${PROJECT_SOURCE_DIR}/source)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  check_c_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
  if(COMPILER_SUPPORTS_MARCH_NATIVE)
    target_compile_options(v8equiv PRIVATE -march=native)
  endif()
endif()

target_compile_options(v8equiv PRIVATE -O3 -Wall -Wextra -Wshadow)

add_test(NAME v8equiv COMMAND v8equiv)
