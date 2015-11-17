# Check if cmake has the required version
cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

# Set name of our project
project(t2k-nd280-upgrade)

set(LIBRARY_OUTPUT_PATH    "${CMAKE_BINARY_DIR}/../lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/../bin")

# Load some basic macros which are needed later on
include(${CMAKE_BINARY_DIR}/FindROOT.cmake)
include(${CMAKE_BINARY_DIR}/FindCLHEP.cmake)
include(${CMAKE_BINARY_DIR}/FindGEANT4.cmake)
include(${CMAKE_BINARY_DIR}/FindND280MC.cmake)
include(${CMAKE_BINARY_DIR}/FindOAEVENT.cmake)
include(${CMAKE_BINARY_DIR}/FindINGRIDUTILS.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DG4VIS_USE -DWITHOUT_INGRID")

if    (_USE_CPP11 MATCHES "ON")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
endif (_USE_CPP11 MATCHES "ON")

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ggdb" )

if    (ROOT_FOUND)
  INCLUDE_DIRECTORIES(${ROOT_INCLUDE_DIR})
  LINK_DIRECTORIES   (${ROOT_LIBRARY_DIR})
endif (ROOT_FOUND)

message(STATUS "C flags      : " ${CMAKE_C_FLAGS})
message(STATUS "CXX flags    : " ${CMAKE_CXX_FLAGS})
message(STATUS "Linker flags : " ${CMAKE_EXE_LINKER_FLAGS}\n)

add_subdirectory(${CMAKE_SOURCE_DIR}/app)

