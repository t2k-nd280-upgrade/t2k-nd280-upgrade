# Check if cmake has the required version
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

# Set name of our project
project(t2k_nd280_upgrade)

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake )
MESSAGE(STATUS ${PROJECT_SOURCE_DIR}/../cmake)
include( PackageBuilder )


pbuilder_prepare_project()

option(RECO_ONLY "Build only reconstruction pcages, omit G4" OFF)

if    (_USE_CPP11 MATCHES "ON")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread ")
endif (_USE_CPP11 MATCHES "ON")

find_package (ROOT COMPONENTS MathCoreProut RIO Hist Tree Net TreePlayer Geom)
if (ROOT_FOUND)
    pbuilder_add_ext_libraries (${ROOT_LIBRARIES})
    #message(STATUS "${ROOT_LIBRARIES}")
else (ROOT_FOUND)
    message(FATAL "Unable to find ROOT")
endif (ROOT_FOUND)
include_directories (${ROOT_INCLUDE_DIR})

find_package(GEANT4 REQUIRED ui_all vis_all)
pbuilder_add_ext_libraries( ${GEANT4_LIBRARIES} )

if (NOT ${GEANT_VERSION} VERSION_LESS 10.02.03)
	MESSAGE(STATUS "Geant4 version > 10.02p03 detected. Special flag is added for compilation")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNEW_G4")
endif (NOT ${GEANT_VERSION} VERSION_LESS 10.02.03)

# find_package(CLHEP)
# pbuilder_add_ext_libraries( ${CHLEP_LIBRARIES} )

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

#####

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/nd280UpEvent) ### include nd280UpEvent class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/BlineTracer/include) ### include GEANT4 package to visualize magnetic field
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/nd280mc) ### include nd280mc class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/nd280mc/ecal) ### include nd280mc/ecal class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/nd280mc/p0d) ### include nd280mc/p0d class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/src) ### include GEANT4 simulation
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/src/CFBox) ### include CFBox class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/src/FGD3D) ### include FGD3D class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/src/FGDlike) ### include FGDlike class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/src/FlatCable) ### include FlatCable class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/src/HATPC) ### include SuperFGD class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/src/PCB) ### include PCB class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/src/SuperFGD) ### include SuperFGD class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/src/SciFi) ### include SciFi class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/src/ToF) ### include ToF class
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/src/WAGASCI) ### include WAGASCI class
# INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/EffStudy/src/ReadOutSim) ### include ReadOutSim class


########### FOR RECONSTRUCTION ##########

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/Reconstruction/src/) ### include general classes
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..//src/app/Reconstruction/src/ReadOutSim) ### include ReadOutSim class

#########################################

# remove plenty of warnings from ROOT source code
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual -Wno-shadow -Wno-unused-const-variable")
if (APPLE)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-private-field -Wno-pointer-bool-conversion")
endif (APPLE)

message(STATUS "C flags      : " ${CMAKE_C_FLAGS})
message(STATUS "CXX flags    : " ${CMAKE_CXX_FLAGS})
message(STATUS "Linker flags : " ${CMAKE_EXE_LINKER_FLAGS})

add_subdirectory(${CMAKE_SOURCE_DIR}/app/nd280UpEvent)
add_subdirectory(${CMAKE_SOURCE_DIR}/app/EffStudy)
add_subdirectory(${CMAKE_SOURCE_DIR}/app/Reconstruction)
