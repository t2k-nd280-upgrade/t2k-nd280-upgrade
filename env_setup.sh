# Setup Geant4 environment variables not defined by geant4.sh but needed by cmake
export GEANT4_INCLUDE=$(geant4-config --prefix)/include/Geant4
export GEANT4_INSTALL_BIN=$(geant4-config --prefix)/bin
export GEANT4_LIBRARY=$(geant4-config --prefix)/lib
export GEANT4_LIBRARY_DIR=$(geant4-config --prefix)/lib

# Setup CLHEP environment variables needed by cmake
# Activate below if Geant4 doesn't have CLHEP built-in
# export CLHEP_BASE_DIR=$(clhep-config --prefix | sed s/\"//g)
# export CLHEP_INCLUDE_DIR=$CLHEP_BASE_DIR/include/
# export CLHEP_LIBRARY_DIR=$CLHEP_BASE_DIR/lib64/
# export CLHEP_LIB_DIR=$CLHEP_BASE_DIR/lib64/
# export LD_LIBRARY_PATH=$CLHEP_INCLUDE_DIR:$CLHEP_LIBRARY_DIR:$LD_LIBRARY_PATH