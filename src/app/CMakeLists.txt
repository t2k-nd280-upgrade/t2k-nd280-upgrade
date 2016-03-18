message(STATUS "app ...")

AUX_SOURCE_DIRECTORY(ND280Upgrade/src sourcefile) ###

message(STATUS "sourcefile ${sourcefile}") ###

ADD_EXECUTABLE(ND280Upgrade.exe          ND280Upgrade/Main.cc ${sourcefile}) ###

TARGET_LINK_LIBRARIES(ND280Upgrade.exe   ${ROOT_LIBRARIES}
                                         ${GEANT4_LIBRARIES}
                                         ${INGRIDUTILS_LIBRARIES}
                                         ${OAEVENT_LIBRARIES}
                                         ${ND280MC_LIBRARIES}
                                         )
