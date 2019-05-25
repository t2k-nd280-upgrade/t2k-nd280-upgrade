message(STATUS "Reconstruction ...")


file (GLOB RECONSTRUCTION_SOURCEFILES 
    src/*.cc
    src/ReadOutSim/*.cc
)

file (GLOB RECONSTRUCTION_HEADERFILES 
    src/*.hh
    src/ReadOutSim/*.hh
)

set (RECONSTRUCTION_DEP_LIBRARIES
    ND280Event
)

pbuilder_library( Reconstruction RECONSTRUCTION_SOURCEFILES RECONSTRUCTION_DEP_LIBRARIES )
pbuilder_install_headers (${RECONSTRUCTION_HEADERFILES})

# ############ FOR RECONSTRUCTION FRAMEWORK #############

set (RECONSPROG_DEP_LIBRARIES
    ND280Event
    EffStudy
    Reconstruction
)

set (RECONSTRUCTION_PROGRAMS
    ReconstructionApp
)

pbuilder_executables( RECONSTRUCTION_PROGRAMS RECONSPROG_DEP_LIBRARIES )
