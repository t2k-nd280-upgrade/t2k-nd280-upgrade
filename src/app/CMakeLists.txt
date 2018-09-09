message(STATUS "app ...")

AUX_SOURCE_DIRECTORY(nd280UpEvent/ sourcefile_event) ###

#if    (RECO_ONLY MATCHES "OFF")
AUX_SOURCE_DIRECTORY(EffStudy/src sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/BlineTracer/src sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/nd280mc sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/nd280mc/ecal sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/nd280mc/p0d sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/ToF sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/SuperFGD sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/HATPC sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/CFBox sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/PCB sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/FlatCable sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/FGDlike sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/WAGASCI sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/FGD3D sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/SciFi sourcefile_sim) ###
AUX_SOURCE_DIRECTORY(EffStudy/src/ReadOutSim sourcefile_sim) ###
#endif (RECO_ONLY MATCHES "OFF")

add_library( ND280UPEVENT_LIB ${sourcefile_event} )

message(STATUS "sourcefile ${sourcefile_sim}") ###

if    (RECO_ONLY MATCHES "OFF")
  ADD_EXECUTABLE(EffStudy.exe          EffStudy/Main.cc ${sourcefile_sim}) ###
endif (RECO_ONLY MATCHES "OFF")


message(STATUS "root libraries ${ROOT_LIBRARIES}") 
message(STATUS "geant4 libraries ${GEANT4_LIBRARIES}") 
#message(STATUS "xerces libraries ${XERCESC_LIBRARIES}") 
#message(STATUS "vgm libraries ${VGM_LIBRARIES}") 
message(STATUS "clhep libraries ${CLHEP_LIBRARIES}") 

#message("XERCESC_LIBRARIES : ${XERCESC_LIBRARIES}  ")

if    (RECO_ONLY MATCHES "OFF")
TARGET_LINK_LIBRARIES(EffStudy.exe   ND280UPEVENT_LIB
                                    ${ROOT_LIBRARIES}
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
                                    )
endif (RECO_ONLY MATCHES "OFF")


############ FOR RECONSTRUCTION FRAMEWORK #############


AUX_SOURCE_DIRECTORY(Reconstruction/src sourcefile_reco) ###
AUX_SOURCE_DIRECTORY(Reconstruction/src/ReadOutSim sourcefile_reco) ### 

ADD_EXECUTABLE(Reconstruction.exe    Reconstruction/Main.cc ${sourcefile_reco}) ###

TARGET_LINK_LIBRARIES(Reconstruction.exe   ${ROOT_LIBRARIES}
                                         ${GEANT4_LIBRARIES}
                                         ${CLHEP_LIBRARIES}
                                         ND280UPEVENT_LIB
                                         )
		
