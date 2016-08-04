

# setup GEANT4
#source /afs/cern.ch/user/d/dsgalabe/GEANT4/v10r1p03/geant4.10.01.p03-build/geant4make.sh # env var of G4 (e.g. data variables...)
source /afs/cern.ch/user/d/dsgalabe/GEANT4/v10r1p03/geant4.10.01.p03-install/bin/geant4.sh

export GEANT4_INCLUDE=/afs/cern.ch/user/d/dsgalabe/GEANT4/v10r1p03/geant4.10.01.p03-install/include/Geant4/
export GEANT4_INSTALL_BIN=/afs/cern.ch/user/d/dsgalabe/GEANT4/v10r1p03/geant4.10.01.p03-install/bin/
export GEANT4_LIBRARY=/afs/cern.ch/user/d/dsgalabe/GEANT4/v10r1p03/geant4.10.01.p03-install/lib64

# setup ROOT
#source ~/T2K/work/v11r31/ROOT/v5r34p09n04/amd64_linux26/bin/thisroot.sh # root ND280 software
source /afs/cern.ch/user/d/dsgalabe/ROOT/root/bin/thisroot.sh

# setup gcc-xml
export PATH=/afs/cern.ch/user/d/dsgalabe/GCCXML/gccxml-install/bin:$PATH # gcc-xml

# setup Xerces-C++
export XERCESC_INSTALL_DIR=/afs/cern.ch/user/d/dsgalabe/XERCES-C++/xerces-c-3.1.3-install
export LD_LIBRARY_PATH=$XERCESC_INSTALL_DIR/lib:$LD_LIBRARY_PATH # Xerces-C++

# setup VGM
export VGM_INSTALL_DIR=/afs/cern.ch/user/d/dsgalabe/VGM/vgm.4.3-install

# setup t2k-nd280-upgrade
export T2KND280UP=$1
export T2KND280UP_SRC=$T2KND280UP/src/app/EffStudy 
export T2KND280UP_EVT=$T2KND280UP/src/app/nd280UpEvent