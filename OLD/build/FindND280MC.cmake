MESSAGE(" Looking for nd280mc...")

if (_MY_ND280MC_INCLUDE_DIR)
FIND_PATH(ND280MC_INCLUDE_DIR NAMES ND280Constructor.hh          PATHS
                                                                 ${_MY_ND280MC_INCLUDE_DIR}/
                                                                 NO_DEFAULT_PATH)
else  (_MY_ND280MC_INCLUDE_DIR)
FIND_PATH(ND280MC_INCLUDE_DIR NAMES ND280Constructor.hh          PATHS
                                                                 $ENV{ND280MCROOT}/src/
                                                                 NO_DEFAULT_PATH)
endif (_MY_ND280MC_INCLUDE_DIR)
# message("ND280MC_INCLUDE_DIRS : ${ND280MC_INCLUDE_DIR}")

if    (_MY_ND280MC_LIBRARY_DIR)
FIND_PATH(ND280MC_LIBRARY_DIR NAMES libnd280mc.so libnd280mc.a   PATHS
                                                                 ${_MY_ND280MC_LIBRARY_DIR}/
                                                                 NO_DEFAULT_PATH)
else  (_MY_ND280MC_LIBRARY_DIR)
FIND_PATH(ND280MC_LIBRARY_DIR NAMES   libnd280mc.so              PATHS
                                                                 $ENV{ND280MCROOT}/$ENV{ND280MCCONFIG}
                                                                 NO_DEFAULT_PATH)
endif (_MY_ND280MC_LIBRARY_DIR)
# message("ND280MC_LIBRARY_DIR : ${ND280MC_LIBRARY_DIR}")

if    (ND280MC_INCLUDE_DIR AND ND280MC_LIBRARY_DIR)
   set(ND280MC_FOUND TRUE)
endif (ND280MC_INCLUDE_DIR AND ND280MC_LIBRARY_DIR)

if (ND280MC_FOUND)

  SET(ND280MC_LIBRARIES nd280mc)
  message(STATUS "found in ${ND280MC_INCLUDE_DIR}")
  message(STATUS "libraries : ${ND280MC_LIBRARIES}\n")

  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${ND280MC_LIBRARY_DIR})

  INCLUDE_DIRECTORIES(${ND280MC_INCLUDE_DIR})
  LINK_DIRECTORIES   (${ND280MC_LIBRARY_DIR})

else (ND280MC_FOUND)

  message(FATAL_ERROR "Could NOT find nd280mc!\n")

endif (ND280MC_FOUND)


