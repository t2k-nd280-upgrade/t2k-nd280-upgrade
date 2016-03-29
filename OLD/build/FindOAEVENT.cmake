MESSAGE(" Looking for oaEvent...")

if (_MY_OAEVENT_INCLUDE_DIR)
FIND_PATH(OAEVENT_INCLUDE_DIR NAMES TG4HitSegment.hxx            PATHS
                                                                 ${_MY_OAEVENT_INCLUDE_DIR}/
                                                                 NO_DEFAULT_PATH)
else  (_MY_OAEVENT_INCLUDE_DIR)
FIND_PATH(OAEVENT_INCLUDE_DIR NAMES TG4HitSegment.hxx            PATHS
                                                                 $ENV{OAEVENTROOT}/src/
                                                                 NO_DEFAULT_PATH)
endif (_MY_OAEVENT_INCLUDE_DIR)
# message("OAEVENT_INCLUDE_DIRS : ${OAEVENT_INCLUDE_DIR}")

if    (_MY_OAEVENT_LIBRARY_DIR)
FIND_PATH(OAEVENT_LIBRARY_DIR NAMES liboaEvent.so liboaEvent.a   PATHS
                                                                 ${_MY_OAEVENT_LIBRARY_DIR}/
                                                                 NO_DEFAULT_PATH)
else  (_MY_OAEVENT_LIBRARY_DIR)
FIND_PATH(OAEVENT_LIBRARY_DIR NAMES   liboaEvent.so              PATHS
                                                                 $ENV{OAEVENTROOT}/$ENV{OAEVENTCONFIG}
                                                                 NO_DEFAULT_PATH)
endif (_MY_OAEVENT_LIBRARY_DIR)
# message("OAEVENT_LIBRARY_DIR : ${OAEVENT_LIBRARY_DIR}")


if    (OAEVENT_INCLUDE_DIR AND OAEVENT_LIBRARY_DIR)
   set(OAEVENT_FOUND TRUE)
endif (OAEVENT_INCLUDE_DIR AND OAEVENT_LIBRARY_DIR)

if (OAEVENT_FOUND)

  SET(OAEVENT_LIBRARIES oaEvent)
  message(STATUS "found in ${OAEVENT_INCLUDE_DIR}")
  message(STATUS "libraries : ${OAEVENT_LIBRARIES}\n")

  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${OAEVENT_LIBRARY_DIR})

  INCLUDE_DIRECTORIES(${OAEVENT_INCLUDE_DIR})
  LINK_DIRECTORIES   (${OAEVENT_LIBRARY_DIR})

else (OAEVENT_FOUND)

  message(FATAL_ERROR "Could NOT find oaEvent!\n")

endif (OAEVENT_FOUND)


