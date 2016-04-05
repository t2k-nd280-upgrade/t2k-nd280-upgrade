message(STATUS "app ...")

AUX_SOURCE_DIRECTORY(EffStudy/src sourcefile) ###

message(STATUS "sourcefile ${sourcefile}") ###

ADD_EXECUTABLE(EffStudy.exe          EffStudy/Main.cc ${sourcefile}) ###

message(STATUS "root libraries ${ROOT_LIBRARIES}") 
message(STATUS "geant4 libraries ${GEANT4_LIBRARIES}") 
message(STATUS "xerces libraries ${XERCES_LIBRARIES}") 
message(STATUS "vgm libraries ${VGM_LIBRARIES}") 

TARGET_LINK_LIBRARIES(EffStudy.exe   ${ROOT_LIBRARIES}
                                         ${GEANT4_LIBRARIES}
                                         #${XERCES_LIBRARIES}
                                         ${VGM_LIBRARIES}
					 #BaseVGM
                                         #ClhepVGM 
                                         #Geant4GM
                                         #RootGM
                                         #VGM
                                         #XmlVGM
                                         #${INGRIDUTILS_LIBRARIES}
                                         #${OAEVENT_LIBRARIES}
                                         #${ND280MC_LIBRARIES}
                                         )
