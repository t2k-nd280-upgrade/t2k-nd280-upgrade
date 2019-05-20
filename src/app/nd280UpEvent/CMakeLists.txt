message(STATUS "ND280Event ...")

file (GLOB EVENT_SOURCEFILES *.cc)

# REQUIRED TO GIVE RELATIVE PATHS TO ROOT_GENERATE
set (EVENT_DICT_HEADERFILES 
    Event.hh
    Hit.hh
    ND280SFGDEvent.hh
    ND280SFGDHit.hh
    ND280SFGDTrack.hh
    ND280SFGDVoxel.hh
    ND280SFGDVoxelSet.hh
    TND280UpEvent.hh
    TND280UpHit.hh
    TND280UpRecoEvent.hh
    TND280UpRecoTrack.hh
    TND280UpTrack.hh
    TND280UpTrackPoint.hh
    TND280UpVertex.hh
)

set( EVENT_HEADERFILES
    ${EVENT_DICT_HEADERFILES}
)

##################################################
# ND280 DICT

set( EVENT_LINKDEF_HEADERFILE LinkDef/LinkDef.h )
set( EVENT_SOURCEFILES ${EVENT_SOURCEFILES} ${CMAKE_CURRENT_BINARY_DIR}/EventDict.cxx )
set( EVENT_DICT_PCMFILE ${CMAKE_CURRENT_BINARY_DIR}/EventDict_rdict.pcm )

ROOT_GENERATE_DICTIONARY(  EventDict ${EVENT_DICT_HEADERFILES} LINKDEF ${EVENT_LINKDEF_HEADERFILE} )
pbuilder_install_files( ${LIB_INSTALL_DIR} ${EVENT_DICT_PCMFILE} )

##################################################

pbuilder_library( ND280Event EVENT_SOURCEFILES "" )
pbuilder_install_headers (${EVENT_HEADERFILES})
