message(STATUS "app ...")

AUX_SOURCE_DIRECTORY(EffStudy/src sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/BlineTracer/src sourcefile) ###
AUX_SOURCE_DIRECTORY(nd280UpEvent/ sourcefile) ###
#AUX_SOURCE_DIRECTORY(nd280UpEvent/Truth sourcefile) ###
#AUX_SOURCE_DIRECTORY(nd280UpEvent/Reco sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/nd280mc sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/nd280mc/ecal sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/nd280mc/p0d sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/ToF sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/SuperFGD sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/HATPC sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/CFBox sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/PCB sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/FGDlike sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/WAGASCI sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/FGD3D sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/SciFi sourcefile) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/ReadOutSim sourcefile) ###

message(STATUS "sourcefile ${sourcefile}") ###

ADD_EXECUTABLE(EffStudy.exe          EffStudy/Main.cc ${sourcefile}) ###

message(STATUS "root libraries ${ROOT_LIBRARIES}") 
message(STATUS "geant4 libraries ${GEANT4_LIBRARIES}") 
#message(STATUS "xerces libraries ${XERCESC_LIBRARIES}") 
#message(STATUS "vgm libraries ${VGM_LIBRARIES}") 
message(STATUS "clhep libraries ${CLHEP_LIBRARIES}") 

#message("XERCESC_LIBRARIES : ${XERCESC_LIBRARIES}  ")

TARGET_LINK_LIBRARIES(EffStudy.exe   ${ROOT_LIBRARIES}
                                         ${GEANT4_LIBRARIES}
                                         #${XERCESC_LIBRARIES}
                                         #${VGM_LIBRARIES}
					 ${CLHEP_LIBRARIES}
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


############ FOR RECONSTRUCTION FRAMEWORK #############

AUX_SOURCE_DIRECTORY(Reconstruction/src sourcefile) ###
AUX_SOURCE_DIRECTORY(Reconstruction/src/ReadOutSim sourcefile) ###

ADD_EXECUTABLE(Reconstruction.exe    Reconstruction/Main.cc ${sourcefile}) ###

TARGET_LINK_LIBRARIES(Reconstruction.exe   ${ROOT_LIBRARIES}
                                         ${GEANT4_LIBRARIES}
                                         ${CLHEP_LIBRARIES}
                                         )

#ADD_LIBRARY(EffStudy    EffStudy/Main.cc ) ###
#TARGET_LINK_LIBRARIES(Reconstruction.exe   EffStudy
#					    )			
