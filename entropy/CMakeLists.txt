cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  add_compile_options(-march=native)
endif()

add_compile_options(-O3 -Wall -Wextra -Wshadow -Wno-implicit-function-declaration)

# Entropy tests reuse practrand source files
set(ENTROPY_TESTS
  testwidynski
  testlehmer64
  testsplitmix64
  testxoroshiro128plus
  testxorshift128plus
  testxorshift32
  testpcg32
  testpcg64
  testmitchellmoore
  testmersennetwister
  testxorshift-k4
  testxorshift-k5
)

# AES-based tests only on x86
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  list(APPEND ENTROPY_TESTS testaesctr)
endif()

foreach(test ${ENTROPY_TESTS})
  add_executable(entropy_${test} ${PROJECT_SOURCE_DIR}/practrand/src/${test}.c)
  target_include_directories(entropy_${test} PRIVATE ${SOURCE_DIR})
  set_target_properties(entropy_${test} PROPERTIES
    OUTPUT_NAME ${test}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
endforeach()
