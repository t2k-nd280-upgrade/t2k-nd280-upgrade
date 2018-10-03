#!bin/sh

# Specify all the environment variables here!
export GEANT4_DIR=/Users/suvorov/SOFT/CERN/GEANT4/geant4.10.01.p03-install/
export CLHEP_DIR=/Users/suvorov/SOFT/CERN/CLHEP/2.4.0.4-install/
export CMT_DIR=/Users/suvorov/SOFT/OTHER/CMT/
export ROOT_DIR=/Users/suvorov/SOFT/CERN/ROOT/root-6.12.06-build/

# setup GEANT4
source $GEANT4_DIR/bin/geant4.sh
export GEANT4_INCLUDE=$GEANT4_DIR/include/Geant4/
export GEANT4_INSTALL_BIN=$GEANT4_DIR/bin/
export GEANT4_LIBRARY=$GEANT4_DIR/lib/
export GEANT4_LIBRARY_DIR=$GEANT4_DIR/lib/

# setup ROOT
source $ROOT_DIR/bin/thisroot.sh

# setup CMT
source $CMT_DIR/setup.sh
export CMTPATH=$1

# setup Xerces-C++
#export XERCESC_INSTALL_DIR=/atlas/users/dsgalabe/XERCES-C++/xerces-c-3.1.3-install
##export XERCESC_INSTALL_DIR=/atlas/users/dsgalabe/xerces-c-3.1.3/xerces-c-3.1.3-install
#export XERCESC_INCLUDE_DIR=/atlas/users/dsgalabe/xerces-c-3.1.3/xerces-c-3.1.3-install/include
#export XERCESC_LIBRARY_DIR=/atlas/users/dsgalabe/xerces-c-3.1.3/xerces-c-3.1.3-install/lib
#export LD_LIBRARY_PATH=$XERCESC_LIBRARY_DIR:$XERCESC_INCLUDE_DIR:$XERCESC_INSTALL_DIR/lib:$LD_LIBRARY_PATH # Xerces-C++
##export LD_LIBRARY_PATH=$XERCESC_INSTALL_DIR/lib:$LD_LIBRARY_PATH # Xerces-C++

# setup CLHEP
#export CLHEP_INCLUDE_DIR=/atlas/users/dsgalabe/CLHEP/clhep_install/include
export CLHEP_BASE_DIR=$CLHEP_DIR/
export CLHEP_INCLUDE_DIR=$CLHEP_DIR/include/
export CLHEP_LIBRARY_DIR=$CLHEP_DIR/lib/
export CLHEP_LIB_DIR=$CLHEP_DIR/lib/
export LD_LIBRARY_PATH=$CLHEP_INCLUDE_DIR:$CLHEP_LIBRARY_DIR:$LD_LIBRARY_PATH

# # setup VGM
# export VGM_INSTALL_DIR=/atlas/users/dsgalabe/VGM/vgm.4.3-install

# setup t2k-nd280-upgrade
export T2KND280UP=$1
export T2KND280UP_SRC=$T2KND280UP/src/app/EffStudy
export T2KND280UP_EVT=$T2KND280UP/src/app/nd280UpEvent
export T2KND280UP_RUN=$2