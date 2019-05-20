message(STATUS "EffStudy ...")

file (GLOB_RECURSE EFFSTUDY_SOURCEFILES 
    src/*.cc
    src/*/*.cc
    BlineTracer/src/*.cc
    nd280mc/*.cc
    nd280mc/*/*.cc
)

file (GLOB EFFSTUDY_HEADERFILES 
    src/*.hh
    src/*/*.hh
    BlineTracer/src/*.hh
    nd280mc/*.hh
    nd280mc/*/*.hh
)

set (EFFSTUDY_DEP_LIBRARIES
    ND280Event
    Reconstruction
)

pbuilder_library( EffStudy EFFSTUDY_SOURCEFILES EFFSTUDY_DEP_LIBRARIES )
pbuilder_install_headers (${EFFSTUDY_HEADERFILES})


# ############ FOR EffStudy app #############

set (EFFSTUDY_PROG_DEP_LIBRARIES
    ND280Event
    EffStudy
)

set (EFFSTUDY_PROGRAMS
    EffStudyApp
)

pbuilder_executables( EFFSTUDY_PROGRAMS EFFSTUDY_PROG_DEP_LIBRARIES )