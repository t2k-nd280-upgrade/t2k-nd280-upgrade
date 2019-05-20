message(STATUS "app ...")

file (GLOB EVENT_SOURCEFILES *.cc)
file (GLOB EVENT_DICT_HEADERFILES *.hh)

set( EVENT_HEADERFILES
    ${EVENT_DICT_HEADERFILES}
)

##################################################
# ND280 DICT

set( EVENT_LINKDEF_HEADERFILE LinkDef/LinkDef.h )
set( EVENT_SOURCEFILES ${EVENT_SOURCEFILES} ${CMAKE_CURRENT_BINARY_DIR}/EventDict.cxx )
set( EVENT_DICT_PCMFILE ${CMAKE_CURRENT_BINARY_DIR}/EventDict_rdict.pcm )

##################################################

MESSAGE(STATUS "lib install dir: ${LIB_INSTALL_DIR}")

ROOT_GENERATE_DICTIONARY(  EventDict ${EVENT_DICT_HEADERFILES} LINKDEF ${EVENT_LINKDEF_HEADERFILE} )
pbuilder_install_files( ${LIB_INSTALL_DIR} ${EVENT_DICT_PCMFILE} )

pbuilder_library( ND280Event EVENT_SOURCEFILES "" )
pbuilder_install_headers (${EVENT_HEADERFILES})
