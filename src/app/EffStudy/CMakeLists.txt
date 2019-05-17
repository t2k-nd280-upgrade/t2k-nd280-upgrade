message(STATUS "app ...")

#message( STATUS "installing libs: ${ARGN}" )
set( LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib" )
MESSAGE(STATUS ${LIB_INSTALL_DIR})

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/nd280UpEvent sourcefile_event) ###
file (GLOB EFFSTUDY_SOURCEFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
file (GLOB EFFSTUDY_SOURCEFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cc)
file (GLOB EFFSTUDY_SOURCEFILES ${CMAKE_CURRENT_SOURCE_DIR}/BlineTracer/src/*.cc)
file (GLOB EFFSTUDY_SOURCEFILES ${CMAKE_CURRENT_SOURCE_DIR}/nd280mc/*.cc)
file (GLOB EFFSTUDY_SOURCEFILES ${CMAKE_CURRENT_SOURCE_DIR}/nd280mc/*/*.cc)

file (GLOB EFFSTUDY_HEADERFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh)
file (GLOB EFFSTUDY_HEADERFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.hh)
file (GLOB EFFSTUDY_HEADERFILES ${CMAKE_CURRENT_SOURCE_DIR}/BlineTracer/src/*.hh)
file (GLOB EFFSTUDY_HEADERFILES ${CMAKE_CURRENT_SOURCE_DIR}/nd280mc/*.hh)
file (GLOB EFFSTUDY_HEADERFILES ${CMAKE_CURRENT_SOURCE_DIR}/nd280mc/*/*.hh)

set (EFFSTUDY_DEP_LIBRARIES
    ND280Event
)

#if    (RECO_ONLY MATCHES "OFF")
# AUX_SOURCE_DIRECTORY(EffStudy/src sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/BlineTracer/src sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/nd280mc sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/nd280mc/ecal sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/nd280mc/p0d sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/src/ToF sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/src/SuperFGD sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/src/HATPC sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/src/CFBox sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/src/PCB sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/src/FlatCable sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/src/FGDlike sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/src/WAGASCI sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/src/FGD3D sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/src/SciFi sourcefile_sim) ###
# AUX_SOURCE_DIRECTORY(EffStudy/src/ReadOutSim sourcefile_sim) ###
#endif (RECO_ONLY MATCHES "OFF")

set( BUILD_SHARED_LIBS ON )

##################################################
# ND280 DICT

# set( EFFSTUDY_LINKDEF_HEADERFILE LinkDef/LinkDef.h )
# set( EFFSTUDY_SOURCEFILES ${EFFSTUDY_SOURCEFILES} ${CMAKE_CURRENT_BINARY_DIR}/EventDict.cxx )
# set( EFFSTUDY_DICT_PCMFILE ${CMAKE_CURRENT_BINARY_DIR}/EventDict_rdict.pcm )

# ##################################################

# ROOT_GENERATE_DICTIONARY(  EventDict ${EFFSTUDY_DICT_HEADERFILES} LINKDEF ${EFFSTUDY_LINKDEF_HEADERFILE} )
# pbuilder_install_files( ${LIB_INSTALL_DIR} ${EFFSTUDY_DICT_PCMFILE} )

pbuilder_library( EffStudy EFFSTUDY_SOURCEFILES EFFSTUDY_DEP_LIBRARIES )
pbuilder_install_headers (${EFFSTUDY_HEADERFILES})

##################################################
# ND280 LIBRARY

# add_library( ND280Event ${EVENT_SOURCEFILES} )
# target_compile_options( ND280Event INTERFACE ${GLOBAL_COMPILE_OPTIONS} )
# target_link_libraries( ND280Event EventDict ${ROOT_LIBRARIES} )
# install( TARGETS ND280Event EXPORT ${PROJECT_NAME}Targets DESTINATION ${LIB_INSTALL_DIR} )
# #list( APPEND ${PROJECT_NAME}_LIBRARIES ${ARGN} )
# set_property( GLOBAL APPEND PROPERTY ${PROJECT_NAME}_LIBRARIES ${ARGN} )
# set_target_properties( ND280Event PROPERTIES INSTALL_NAME_DIR ${LIB_INSTALL_DIR} )

# # macro( make_library LIBNAME SOURCE LINKED_LIBRARIES EXT_LIBRARIES)
# #     add_library( ${LIBNAME} ${SOURCE} )
# #     target_compile_options( ${LIBNAME} INTERFACE ${GLOBAL_COMPILE_OPTIONS} )
# #     target_link_libraries( ${LIBNAME} ${LINKED_LIBRARIES} ${EXT_LIBRARIES} )
# #     install( TARGETS ${LIBNAME} EXPORT ${PROJECT_NAME}Targets DESTINATION ${LIB_INSTALL_DIR} )
# #     #list( APPEND ${PROJECT_NAME}_LIBRARIES ${ARGN} )
# #     set_property( GLOBAL APPEND PROPERTY ${PROJECT_NAME}_LIBRARIES ${ARGN} )
# #     set_target_properties( ${LIBNAME} PROPERTIES INSTALL_NAME_DIR ${LIB_INSTALL_DIR} )
# # endmacro()
# # make_library( ND280Event ${sourcefile_event} "" ${ROOT_LIBRARIES})


# message(STATUS "sourcefile simulation ${sourcefile_sim}") ###
# message(STATUS "sourcefile events ${sourcefile_event}") ###

# if    (RECO_ONLY MATCHES "OFF")
#   ADD_EXECUTABLE(EffStudy.exe EffStudy/Main.cc ${sourcefile_sim}) ###
# endif (RECO_ONLY MATCHES "OFF")

# message(STATUS "root libraries ${ROOT_LIBRARIES}") 
# message(STATUS "geant4 libraries ${GEANT4_LIBRARIES}") 
# #message(STATUS "xerces libraries ${XERCESC_LIBRARIES}") 
# #message(STATUS "vgm libraries ${VGM_LIBRARIES}") 
# message(STATUS "clhep libraries ${CLHEP_LIBRARIES}") 

# #message("XERCESC_LIBRARIES : ${XERCESC_LIBRARIES}  ")

# if    (RECO_ONLY MATCHES "OFF")
# TARGET_LINK_LIBRARIES(EffStudy.exe   ND280Event
#                                     ${ROOT_LIBRARIES}
#                                     ${GEANT4_LIBRARIES}
#                                     #${XERCESC_LIBRARIES}
#                                     #${VGM_LIBRARIES}
# 					                ${CLHEP_LIBRARIES}
#                                     #BaseVGM
#                                     #ClhepVGM 
#                                     #Geant4GM
#                                     #RootGM
#                                     #VGM
#                                     #XmlVGM
#                                     #${INGRIDUTILS_LIBRARIES}
#                                     )
# endif (RECO_ONLY MATCHES "OFF")


# ############ FOR RECONSTRUCTION FRAMEWORK #############


# AUX_SOURCE_DIRECTORY(Reconstruction/src sourcefile_reco) ###
# AUX_SOURCE_DIRECTORY(Reconstruction/src/ReadOutSim sourcefile_reco) ### 

# ADD_EXECUTABLE(Reconstruction.exe    Reconstruction/Main.cc ${sourcefile_reco}) ###

# TARGET_LINK_LIBRARIES(Reconstruction.exe   ${ROOT_LIBRARIES}
#                                          ${GEANT4_LIBRARIES}
#                                          ${CLHEP_LIBRARIES}
#                                          ND280Event
#                                          )

		
