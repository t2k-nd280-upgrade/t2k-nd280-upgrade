# - Try to find CLHEP
# Once done this will define
#
#  CLHEP_FOUND - system has CLHEP
#  CLHEP_INCLUDE_DIR - the CLHEP include directory
#  CLHEP_LIBRARIES - The libraries needed to use CLHEP
#  CLHEP_DEFINITIONS - Compiler switches required for using CLHEP
#

MESSAGE("\n Looking for CLHEP...")
                   # Find CLHEP_DIR
#message("CLHEP_LIBRARIES : ${CLHEP_LIBRARIES}")

if (UNIX AND NOT APPLE)
  set(LIBNAME  libCLHEP.so)
elseif (APPLE)
  set(LIBNAME  libCLHEP.dylib)
endif (UNIX AND NOT APPLE)

FIND_PATH(CLHEP_INCLUDE_DIR NAMES CLHEP                    PATHS
                                                           $ENV{CLHEP_BASE_DIR}/include/
                                                           NO_DEFAULT_PATH)

FIND_PATH(CLHEP_LIBRARY_DIR NAMES ${LIBNAME}               PATHS
                                                           $ENV{CLHEP_BASE_DIR}/lib/
                                                           NO_DEFAULT_PATH)

FIND_PROGRAM(CLHEP_CONFIG_EXECUTABLE NAMES clhep-config    PATHS
                                                           $ENV{CLHEP_BASE_DIR}/bin/
                                                           NO_DEFAULT_PATH)

#message("CLHEP_CONFIG_EXECUTABLE : ${CLHEP_CONFIG_EXECUTABLE}  ")

if (CLHEP_CONFIG_EXECUTABLE)

  set(CLHEP_FOUND TRUE)
  set(CLHEP_LIBRARIES ${LIBNAME})
  EXEC_PROGRAM(${CLHEP_CONFIG_EXECUTABLE} ARGS "--version" OUTPUT_VARIABLE CLHEP_VERSION)
  message(STATUS "found in  ${CLHEP_LIBRARY_DIR}")
  message(STATUS "version : ${CLHEP_VERSION}\n")
  # message(STATUS "libraries : ${CLHEP_LIBRARIES}")
  # message(STATUS "definitions : ${CLHEP_DEFINITIONS}")

  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${CLHEP_LIBRARY_DIR})

  INCLUDE_DIRECTORIES(${CLHEP_INCLUDE_DIR})
  LINK_DIRECTORIES   (${CLHEP_LIBRARY_DIR})

else (CLHEP_CONFIG_EXECUTABLE)

  message(STATUS "Could NOT find CLHEP!\n")

endif (CLHEP_CONFIG_EXECUTABLE)

