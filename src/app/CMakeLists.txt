message(STATUS "app ...")

ADD_EXECUTABLE(g4VisExampe               visExample.cpp)
TARGET_LINK_LIBRARIES(g4VisExampe        ${ROOT_LIBRARIES}
                                         ${GEANT4_LIBRARIES}
                                         ${INGRIDUTILS_LIBRARIES}
                                         ${OAEVENT_LIBRARIES}
                                         ${ND280MC_LIBRARIES})
