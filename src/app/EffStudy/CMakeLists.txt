message(STATUS "EffStudy ...")

#message( STATUS "installing libs: ${ARGN}" )
# set( LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib" )
# MESSAGE(STATUS ${LIB_INSTALL_DIR})

# AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/nd280UpEvent sourcefile_event) ###
file (GLOB_RECURSE EFFSTUDY_SOURCEFILES 
    src/*.cc
    src/*/*.cc
    BlineTracer/src/*.cc
    nd280mc/*.cc
    nd280mc/*/*.cc
)

MESSAGE(STATUS ${EFFSTUDY_SOURCEFILES})

file (GLOB EFFSTUDY_HEADERFILES 
    src/*.hh
    src/*/*.hh
    BlineTracer/src/*.hh
    nd280mc/*.hh
    nd280mc/*/*.hh
)
# file (GLOB EFFSTUDY_HEADERFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.hh)
# file (GLOB EFFSTUDY_HEADERFILES ${CMAKE_CURRENT_SOURCE_DIR}/BlineTracer/src/*.hh)
# file (GLOB EFFSTUDY_HEADERFILES ${CMAKE_CURRENT_SOURCE_DIR}/nd280mc/*.hh)
# file (GLOB EFFSTUDY_HEADERFILES ${CMAKE_CURRENT_SOURCE_DIR}/nd280mc/*/*.hh)

set (EFFSTUDY_DEP_LIBRARIES
    ND280Event
    Reconstruction
)


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


# ############ FOR RECONSTRUCTION FRAMEWORK #############

set (EFFSTUDY_PROG_DEP_LIBRARIES
    ND280Event
    EffStudy
)

set (EFFSTUDY_PROGRAMS
    EffStudyApp
)

pbuilder_executables( EFFSTUDY_PROGRAMS EFFSTUDY_PROG_DEP_LIBRARIES )