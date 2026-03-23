cmake_minimum_required(VERSION 3.14)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

project(testingRNG C CXX)

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
