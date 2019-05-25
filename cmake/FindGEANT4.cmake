MESSAGE(" Looking for Geant4...")

if (UNIX AND NOT APPLE)
  set(libname  libG4event.so)
elseif (APPLE)
  set(libname  libG4event.dylib)
endif (UNIX AND NOT APPLE)

FIND_PATH(GEANT4_INCLUDE_DIR NAMES G4PVPlacement.hh          PATHS
                                                             $ENV{G4INCLUDE}/
                                                             #/afs/cern.ch/user/d/dsgalabe/GEANT4/v10r1p03/geant4.10.01.p03-install/include/Geant4/
							     $ENV{GEANT4_INCLUDE}
							     NO_DEFAULT_PATH) 

message("GEANT4_INCLUDE_DIRS : ${GEANT4_INCLUDE_DIR}")

FIND_PATH(GEANT4_LIBRARY_DIR NAMES ${libname}                PATHS
                                                             $ENV{G4LIB}/$ENV{G4SYSTEM}/
							     $ENV{GEANT4_LIBRARY}
							     NO_DEFAULT_PATH)
							     
message("GEANT4_LIBRARY_DIR : ${GEANT4_LIBRARY_DIR}")

#FIND_PROGRAM(GEANT4_CONFIG_EXECUTABLE  NAMES  geant4-config  PATHS
#                                                             $ENV{G4INCLUDE}/../../bin
#                                                             NO_DEFAULT_PATH)

#FIND_PROGRAM(GEANT4_CONFIG_EXECUTABLE_LOAD NAMES  geant-load-flags  PATHS
#                                                                 $ENV{G4BIN}/
#								 NO_DEFAULT_PATH)

#FIND_PROGRAM(GEANT4_CONFIG_EXECUTABLE_CPP NAMES  geant-cpp-flags  PATHS
FIND_PROGRAM(GEANT4_CONFIG_EXECUTABLE NAMES  geant4-config    PATHS
                                                                 $ENV{G4BIN}/ 
								 $ENV{GEANT4_INSTALL_BIN}
								 #/afs/cern.ch/user/d/dsgalabe/GEANT4/v10r1p03/geant4.10.01.p03-install/bin/
								 NO_DEFAULT_PATH)

execute_process(
    COMMAND ${GEANT4_CONFIG_EXECUTABLE} --version
    OUTPUT_VARIABLE GEANT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)

message("GEANT4_CONFIG_EXECUTABLES : ${GEANT4_CONFIG_EXECUTABLE_CPP}")

#if (GEANT4_INCLUDE_DIR AND GEANT4_LIBRARY_DIR AND GEANT4_CONFIG_EXECUTABLE_LOAD)
if (GEANT4_INCLUDE_DIR AND GEANT4_LIBRARY_DIR)
   set(GEANT4_FOUND TRUE)
#endif (GEANT4_INCLUDE_DIR AND GEANT4_LIBRARY_DIR AND GEANT4_CONFIG_EXECUTABLE_LOAD)
endif (GEANT4_INCLUDE_DIR AND GEANT4_LIBRARY_DIR)

if (GEANT4_FOUND)

#  EXEC_PROGRAM(${GEANT4_CONFIG_EXECUTABLE} ARGS "--version" OUTPUT_VARIABLE GEANT4VERSION)

  # ask geant4-config for the library varaibles

  EXEC_PROGRAM(${GEANT4_CONFIG_EXECUTABLE} ARGS "--libs" OUTPUT_VARIABLE GEANT4_LIBRARIES)
  EXEC_PROGRAM(${GEANT4_CONFIG_EXECUTABLE} ARGS "--cflags" OUTPUT_VARIABLE GEANT4_FLAGS)
  
  message(STATUS "found in ${GEANT4_INCLUDE_DIR}\n")
  message(STATUS "libraries : ${GEANT4_LIBRARIES}")
  message(STATUS "flags : ${GEANT4_FLAGS}\n")
  
  
  # message(STATUS "definitions : ${Gean4_DEFINITIONS} \n")

  SET(LD_LIBRARY_PATH "${LD_LIBRARY_PATH} ${GEANT4_LIBRARY_DIR}")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GEANT4_FLAGS}")

  INCLUDE_DIRECTORIES(${GEANT4_INCLUDE_DIR})
  LINK_DIRECTORIES   (${GEANT4_LIBRARY_DIR})

else (GEANT4_FOUND)

  message(FATAL_ERROR "Could NOT find Gean4!\n")

endif (GEANT4_FOUND)
