cmake_minimum_required(VERSION 3.14)

include(ExternalProject)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  add_compile_options(-march=native)
endif()

add_compile_options(-O3 -Wall -Wextra -Wshadow -Wno-implicit-function-declaration)

# Check for unzip to build PractRand's RNG_test
find_program(UNZIP unzip)

if(UNZIP)
  # Build PractRand's RNG_test from the checked-in archive
  add_custom_target(RNG_test
    COMMAND unzip -qo PractRand_0.93.zip -d build && cd build && cat ../practrand-0.93-bigbuffer.patch | patch -p0 && ${CMAKE_CXX_COMPILER} -std=c++14 -c src/*.cpp src/RNGs/*.cpp src/RNGs/other/*.cpp -O3 -Iinclude -pthread && ar rcs libPractRand.a *.o && ${CMAKE_CXX_COMPILER} -std=c++14 -o RNG_test tools/RNG_test.cpp libPractRand.a -O3 -Iinclude -pthread && cp RNG_test ..
    DEPENDS PractRand_0.93.zip practrand-0.93-bigbuffer.patch
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    BYPRODUCTS RNG_test
  )
else()
  message(STATUS "unzip not found, skipping RNG_test build")
endif()

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

# Copy scripts into the build directory
file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/runtests.sh
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/results)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/results
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
  )
endif()
