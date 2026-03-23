cmake_minimum_required(VERSION 3.14)

include(ExternalProject)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  add_compile_options(-march=native)
endif()

add_compile_options(-O3 -Wall -Wextra -Wshadow -Wno-implicit-function-declaration)

# Build TestU01 from the checked-in archive
set(TESTU01_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/testu01-install)

ExternalProject_Add(testu01_lib
  URL ${CMAKE_CURRENT_SOURCE_DIR}/TestU01.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${TESTU01_INSTALL_DIR}
  BUILD_COMMAND make -j4
  INSTALL_COMMAND make install
  BUILD_IN_SOURCE TRUE
)

set(TESTU01_TESTS
  testmersennetwister
  testlehmer64
  testsplitmix64
  testxoroshiro128plus
  testv8xorshift128plus
  testxorshift128plus
  testxorshift32
  testpcg32
  testpcg64
  testxorshift1024star
  testxorshift1024plus
  testwyhash
  testwyrand
  testw1rand
)

# AES-based tests only on x86
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  list(APPEND TESTU01_TESTS testaesctr testaesdragontamer)
endif()

foreach(test ${TESTU01_TESTS})
  add_executable(testu01_${test} src/${test}.c)
  add_dependencies(testu01_${test} testu01_lib)
  target_include_directories(testu01_${test} PRIVATE
    ${SOURCE_DIR}
    ${TESTU01_INSTALL_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
  target_link_libraries(testu01_${test} PRIVATE
    ${TESTU01_INSTALL_DIR}/lib/libtestu01.a
    ${TESTU01_INSTALL_DIR}/lib/libprobdist.a
    ${TESTU01_INSTALL_DIR}/lib/libmylib.a
    m
  )
  set_target_properties(testu01_${test} PROPERTIES OUTPUT_NAME ${test})
endforeach()

# Copy scripts into the build directory
file(GLOB TESTU01_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/*.sh)
file(COPY ${TESTU01_SCRIPTS}
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
