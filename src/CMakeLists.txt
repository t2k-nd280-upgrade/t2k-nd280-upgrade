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
#include(${CMAKE_BINARY_DIR}/FindND280MC.cmake)
#include(${CMAKE_BINARY_DIR}/FindOAEVENT.cmake)
#include(${CMAKE_BINARY_DIR}/FindINGRIDUTILS.cmake)
#include(${CMAKE_BINARY_DIR}/FindXERCESC.cmake)
#include(${CMAKE_BINARY_DIR}/FindVGM.cmake)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DG4VIS_USE -DWITHOUT_INGRID")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DG4VIS_USE")

if    (_USE_CPP11 MATCHES "ON")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
endif (_USE_CPP11 MATCHES "ON")

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ggdb" )

if    (ROOT_FOUND)
  INCLUDE_DIRECTORIES(${ROOT_INCLUDE_DIR})
  LINK_DIRECTORIES   (${ROOT_LIBRARY_DIR})
endif (ROOT_FOUND)


#if    (VGM_FOUND)
#  INCLUDE_DIRECTORIES(${VGM_INCLUDE_DIR})
#  LINK_DIRECTORIES   (${VGM_LIBRARY_DIR})
#endif (VGM_FOUND)


##### NEW
#
#if    (XERCESC_FOUND)
#  INCLUDE_DIRECTORIES(${XERCESC_INCLUDE_DIR})
#  LINK_DIRECTORIES   (${XERCESC_LIBRARY_DIR})
#endif (XERCESC_FOUND)
#
#if    (CLHEP_FOUND)
#  INCLUDE_DIRECTORIES(${CLHEP_INCLUDE_DIR})
#  LINK_DIRECTORIES   (${CLHEP_LIBRARY_DIR})
#endif (CLHEP_FOUND)
#
#####

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/../src/app/EffStudy/include) ### include GEANT4 simulation
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/../src/app/EffStudy/BlineTracer/include) ### include GEANT4 package to visualize magnetic field
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/../src/app/nd280UpEvent) ### include nd280UpEvent class

message(STATUS "C flags      : " ${CMAKE_C_FLAGS})
message(STATUS "CXX flags    : " ${CMAKE_CXX_FLAGS})
message(STATUS "Linker flags : " ${CMAKE_EXE_LINKER_FLAGS}\n)

add_subdirectory(${CMAKE_SOURCE_DIR}/app)
