MESSAGE(" Looking for ingridUtils...")

if (_MY_INGRIDUTILS_INCLUDE_DIR)
FIND_PATH(INGRIDUTILS_INCLUDE_DIR NAMES ingridUtils_version.h    PATHS
                                                                 ${_MY_INGRIDUTILS_INCLUDE_DIR}/
                                                                 NO_DEFAULT_PATH)
else  (_MY_INGRIDUTILS_INCLUDE_DIR)
FIND_PATH(INGRIDUTILS_INCLUDE_DIR NAMES ingridUtils_version.h    PATHS
                                                                 $ENV{INGRIDUTILSROOT}/src/
                                                                 NO_DEFAULT_PATH)
endif (_MY_INGRIDUTILS_INCLUDE_DIR)
# message("INGRIDUTILS_INCLUDE_DIRS : ${INGRIDUTILS_INCLUDE_DIR}")

if    (_MY_INGRIDUTILS_LIBRARY_DIR)
FIND_PATH(INGRIDUTILS_LIBRARY_DIR NAMES libingridUtils.so libingridUtils.a  PATHS
                                                                            ${_MY_INGRIDUTILS_LIBRARY_DIR}/
                                                                            NO_DEFAULT_PATH)
else  (_MY_INGRIDUTILS_LIBRARY_DIR)
FIND_PATH(INGRIDUTILS_LIBRARY_DIR NAMES   libingridUtils.so      PATHS
                                                                 $ENV{INGRIDUTILSROOT}/$ENV{INGRIDUTILSCONFIG}
                                                                 NO_DEFAULT_PATH)
endif (_MY_INGRIDUTILS_LIBRARY_DIR)
# message("INGRIDUTILS_LIBRARY_DIR : ${INGRIDUTILS_LIBRARY_DIR}")


if    (INGRIDUTILS_INCLUDE_DIR AND INGRIDUTILS_LIBRARY_DIR)
   set(INGRIDUTILS_FOUND TRUE)
endif (INGRIDUTILS_INCLUDE_DIR AND INGRIDUTILS_LIBRARY_DIR)

if (INGRIDUTILS_FOUND)

  SET(INGRIDUTILS_LIBRARIES ingridUtils)
  message(STATUS "found in ${INGRIDUTILS_INCLUDE_DIR}")
  message(STATUS "libraries : ${INGRIDUTILS_LIBRARIES}\n")

  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${INGRIDUTILS_LIBRARY_DIR})

  INCLUDE_DIRECTORIES(${INGRIDUTILS_INCLUDE_DIR})
  LINK_DIRECTORIES   (${INGRIDUTILS_LIBRARY_DIR})

else (INGRIDUTILS_FOUND)

  message(FATAL_ERROR "Could NOT find ingridUtils!\n")

endif (INGRIDUTILS_FOUND)


