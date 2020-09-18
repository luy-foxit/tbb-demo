cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
set(CMAKE_VERBOSE_MAKEFILE  ON)

project(tbb-demo C CXX)

set(TARGET_OS linux)
set(TARGET_ARCH x86_64)
set(PROJECT_BUILD_ROOT ${PROJECT_BINARY_DIR})
set(TBB_CMAKE_BUILD_DIR tbb_cmake_build)

set(CMAKE_BUILD_TYPE Debug)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "Current CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

set(TBB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/oneTBB)

if(TARGET_ARCH STREQUAL "x86_64")
     set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -march=x86-64")
elseif(TARGET_OS STREQUAL "android")
     set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DANDROID -fexceptions -frtti -Wno-unused-private-field")
     if(TARGET_ARCH STREQUAL "armv8")
          set(ANDROID_ABI  "arm64-v8a")
     endif()
     message("ANDROID_ABI: ${ANDROID_ABI}")
else()
     message(FATAL_ERROR "unsupported TARGET_OS ${TARGET_OS}, TARGET_ARCH ${TARGET_ARCH}")
endif()

message(STATUS "Current CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
     set(TBB_BUILD_COMPILER "clang")
else()
     set(TBB_BUILD_COMPILER "gcc")
endif()

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-unused-variable")
include(${TBB_ROOT}/cmake/TBBBuild.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("CMAKE_BUILD_TYPE_DEBUG: ${CMAKE_BUILD_TYPE}")
     set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DDEBUG -g -O0")
     set(TBB_FIND_RELEASE_ONLY 0)
     set(TBB_GENERATE_CFG "_debug")
     set(TBB_GENERATE_SUFFIX "_debug")
     set(TBB_MAKE_ARGS tbb_build_dir=${PROJECT_BUILD_ROOT}/${TBB_CMAKE_BUILD_DIR} tbb_build_prefix=${TBB_CMAKE_BUILD_DIR} TBB_DEBUG_DIR=${PROJECT_BUILD_ROOT}/${TBB_CMAKE_BUILD_DIR}/${TBB_CMAKE_BUILD_DIR}_debug cfg=debug)
else()
    message("CMAKE_BUILD_TYPE_RELEASE: ${CMAKE_BUILD_TYPE}")
     set(TBB_GENERATE_CFG "_release")
     set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DNDEBUG")
     set(TBB_FIND_RELEASE_ONLY 1)
     set(TBB_MAKE_ARGS tbb_build_dir=${PROJECT_BUILD_ROOT}/${TBB_CMAKE_BUILD_DIR} tbb_build_prefix=${TBB_CMAKE_BUILD_DIR} TBB_RELEASE_DIR=${PROJECT_BUILD_ROOT}/${TBB_CMAKE_BUILD_DIR}/${TBB_CMAKE_BUILD_DIR}_release cfg=release)
endif()

if(TARGET_OS STREQUAL "android")
     set(TBB_MAKE_ARGS ${TBB_MAKE_ARGS} arch=${ANDROID_ABI} compiler=${TBB_BUILD_COMPILER} api_version=${ANDROID_API_VERSION} target=android)
endif()

tbb_build(TBB_ROOT ${TBB_ROOT} CONFIG_DIR TBB_DIR MAKE_ARGS ${TBB_MAKE_ARGS})
find_package(TBB REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

message("TBB_IMPORTED_TARGETS: ${TBB_IMPORTED_TARGETS}")
add_subdirectory(src)

file(GLOB TBB_LIB               "${PROJECT_BUILD_ROOT}/${TBB_CMAKE_BUILD_DIR}/${TBB_CMAKE_BUILD_DIR}${TBB_GENERATE_CFG}/libtbb*${TBB_GENERATE_SUFFIX}.so*")
install (FILES     ${TBB_LIB}      DESTINATION lib)

