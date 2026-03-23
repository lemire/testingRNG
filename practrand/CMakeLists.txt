cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  add_compile_options(-march=native)
endif()

add_compile_options(-O3 -Wall -Wextra -Wshadow -Wno-implicit-function-declaration)

set(PRACTRAND_TESTS
  testwidynski
  testlehmer64
  testsplitmix64
  testv8xorshift128plus
  testxoroshiro128plus
  testxorshift128plus
  testxorshift32
  testpcg32
  testpcg64
  testmitchellmoore
  testmersennetwister
  testxorshift-k4
  testxorshift-k5
  testxorshift1024star
  testxorshift1024plus
  testwyhash
  testwyrand
  testw1rand
)

# AES-based tests only on x86
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  list(APPEND PRACTRAND_TESTS testaesctr testaesdragontamer)
endif()

foreach(test ${PRACTRAND_TESTS})
  add_executable(practrand_${test} src/${test}.c)
  target_include_directories(practrand_${test} PRIVATE ${SOURCE_DIR})
  set_target_properties(practrand_${test} PROPERTIES OUTPUT_NAME ${test})
endforeach()
