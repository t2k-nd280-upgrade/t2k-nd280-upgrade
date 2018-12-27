#!/usr/bin/sh

export SOFTWARE_DIR=/gpfs/home/t2k/kiwamoto/ND280Upgrade/Software

# setup GEANT4
source ${SOFTWARE_DIR}/GEANT4-install/bin/geant4.sh

export G4INCLUDE=${SOFTWARE_DIR}/GEANT4-install/include/Geant4/
export GEANT4_INCLUDE=${SOFTWARE_DIR}/GEANT4-install/include/Geant4/
export GEANT4_INCLUDE_DIR=${SOFTWARE_DIR}/GEANT4-install/include/Geant4/
export GEANT4_INSTALL_BIN=${SOFTWARE_DIR}/GEANT4-install/bin/

export G4LIB=${SOFTWARE_DIR}/GEANT4-install/lib64
export G4SYSTEM=${SOFTWARE_DIR}/GEANT4-install/lib64
export GEANT4_LIBRARY=${SOFTWARE_DIR}/GEANT4-install/lib64
export GEANT4_LIBRARY_DIR=${SOFTWARE_DIR}/GEANT4-install/lib64

# setup ROOT
source /gpfs/home/t2k/kiwamoto/ND280Upgrade/Software/root_v5.34.36/bin/thisroot.sh

# setup CMT
source /gpfs/home/t2k/kiwamoto/ND280Upgrade/Software/CMT/setup.sh
export CMTPATH=${SOFTWARE_DIR}/t2k-nd280-upgrade:${SOFTWARE_DIR}/CMT

#setup NEUT

export CERN=${SOFTWARE_DIR}/CERNLIB
export CERN_LEVEL=2005
export CERN_ROOT=${CERN}/${CERN_LEVEL}
export CERNLIB=${CERN_ROOT}/lib

if [ $?LD_LIBRARY_PATH ]; then
   export LD_LIBRARY_PATH=${CERNLIB}:${LD_LIBRARY_PATH}
else
   export LD_LIBRARY_PATH=${CERNLIB}
fi

export NEUT_ROOT=${SOFTWARE_DIR}/neut_5.3.6
export LD_LIBRARY_PATH=${NEUT_ROOT}/src/reweight:${LD_LIBRARY_PATH}

# setup highlandUp
#source ${SOFTWARE_DIR}/t2k-nd280-upgrade/highlandUP/run_at_start_t2k.sh
export CVSROOT=:ext:anoncvs@repo.nd280.org:/home/trt2kmgr/ND280Repository
export CVS_RSH=ssh
unset CVS_SERVER

# setup Xerces-C++
#export XERCESC_INSTALL_DIR=${SOFTWARE_DIR}/xerces-c-3.1.3/xerces-c-3.1.3-install
#export LD_LIBRARY_PATH=${XERCESC_INSTALL_DIR}/lib:${LD_LIBRARY_PATH}

# setup CLHEP
export CLHEP_BASE_DIR=${SOFTWARE_DIR}/CLHEP-install/
export CLHEP_INCLUDE_DIR=${SOFTWARE_DIR}/CLHEP-install/include
export CLHEP_LIBRARY_DIR=${SOFTWARE_DIR}/CLHEP-install/lib
export CLHEP_LIB_DIR=${SOFTWARE_DIR}/CLHEP-install/lib

export LD_LIBRARY_PATH=${CLHEP_INCLUDE_DIR}:${CLHEP_LIBRARY_DIR}:${LD_LIBRARY_PATH}     

# setup VGM
#export VGM_INSTALL_DIR=${SOFTWARE_DIR}/vgm.4.3-install

# setup t2k-nd280-upgrade
export T2KND280UP=${SOFTWARE_DIR}/t2k-nd280-upgrade
export T2KND280UP_SRC=${T2KND280UP}/src/app/EffStudy 
export T2KND280UP_EVT=${T2KND280UP}/src/app/nd280UpEvent
export T2KND280UP_RUN=/gpfs/fs03/t2k/beam/work/kiwamoto/ND280Upgrade/Run
export T2KND280UP_REC=${T2KND280UP}/src/app/Reconstruction
export T2KND280UP_ELPH=/gpfs/fs03/t2k/beam/work/kiwamoto/ELPH_11122018

echo "DONE T2KND280Up_CLHEP.sh"
