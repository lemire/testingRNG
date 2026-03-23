cmake_minimum_required(VERSION 3.14)

project(testingRNG C CXX)

option(ENABLE_ASAN "Enable Address Sanitizer" OFF)

if(ENABLE_ASAN)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

enable_testing()

enable_testing()

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)

option(BUILD_SPEED "Build speed benchmarks" ON)
option(BUILD_UNIT "Build unit tests" ON)
option(BUILD_PRACTRAND "Build PractRand test harnesses" ON)
option(BUILD_TESTU01 "Build TestU01 test harnesses" ON)
option(BUILD_ENTROPY "Build entropy test harnesses" ON)

if(BUILD_SPEED)
  add_subdirectory(speed)
endif()

if(BUILD_UNIT)
  add_subdirectory(unit)
endif()

if(BUILD_PRACTRAND)
  add_subdirectory(practrand)
endif()

if(BUILD_TESTU01)
  add_subdirectory(testu01)
endif()

if(BUILD_ENTROPY)
  add_subdirectory(entropy)
endif()
