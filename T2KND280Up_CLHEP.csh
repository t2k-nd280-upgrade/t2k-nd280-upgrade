#!/usr/bin/csh

setenv SOFTWARE_DIR /gpfs/home/t2k/kiwamoto/ND280Upgrade/Software

# setup GEANT4
source ${SOFTWARE_DIR}/GEANT4-install/bin/geant4.csh ${SOFTWARE_DIR}/GEANT4-install/bin

setenv G4INCLUDE ${SOFTWARE_DIR}/GEANT4-install/include/Geant4/
setenv GEANT4_INCLUDE ${SOFTWARE_DIR}/GEANT4-install/include/Geant4/
setenv GEANT4_INCLUDE_DIR ${SOFTWARE_DIR}/GEANT4-install/include/Geant4/
setenv GEANT4_INSTALL_BIN ${SOFTWARE_DIR}/GEANT4-install/bin/

setenv G4LIB ${SOFTWARE_DIR}/GEANT4-install/lib64
setenv G4SYSTEM ${SOFTWARE_DIR}/GEANT4-install/lib64
setenv GEANT4_LIBRARY ${SOFTWARE_DIR}/GEANT4-install/lib64
setenv GEANT4_LIBRARY_DIR ${SOFTWARE_DIR}/GEANT4-install/lib64

# setup ROOT
source /gpfs/home/t2k/kiwamoto/ND280Upgrade/Software/root_v5.34.36/bin/thisroot.csh

# setup CMT
source /gpfs/home/t2k/kiwamoto/ND280Upgrade/Software/CMT/setup.csh
setenv CMTPATH ${SOFTWARE_DIR}/t2k-nd280-upgrade:${SOFTWARE_DIR}/CMT

#setup NEUT

setenv CERN ${SOFTWARE_DIR}/CERNLIB
setenv CERN_LEVEL 2005
setenv CERN_ROOT ${CERN}/${CERN_LEVEL}
setenv CERNLIB ${CERN_ROOT}/lib

if ($?LD_LIBRARY_PATH) then
setenv LD_LIBRARY_PATH ${CERNLIB}:${LD_LIBRARY_PATH}
else
setenv LD_LIBRARY_PATH ${CERNLIB}
endif

setenv NEUT_ROOT ${SOFTWARE_DIR}/neut_5.3.6
setenv LD_LIBRARY_PATH ${NEUT_ROOT}/src/reweight:${LD_LIBRARY_PATH}

# setup highlandUp
#source ${SOFTWARE_DIR}/t2k-nd280-upgrade/highlandUP/run_at_start_t2k.csh
setenv CVSROOT :ext:anoncvs@repo.nd280.org:/home/trt2kmgr/ND280Repository
setenv CVS_RSH ssh
unset CVS_SERVER

# setup Xerces-C++
#setenv XERCESC_INSTALL_DIR ${SOFTWARE_DIR}/xerces-c-3.1.3/xerces-c-3.1.3-install
#setenv LD_LIBRARY_PATH ${XERCESC_INSTALL_DIR}/lib:${LD_LIBRARY_PATH}

# setup CLHEP
setenv CLHEP_BASE_DIR ${SOFTWARE_DIR}/CLHEP-install/
setenv CLHEP_INCLUDE_DIR ${SOFTWARE_DIR}/CLHEP-install/include
setenv CLHEP_LIBRARY_DIR ${SOFTWARE_DIR}/CLHEP-install/lib
setenv CLHEP_LIB_DIR ${SOFTWARE_DIR}/CLHEP-install/lib

setenv LD_LIBRARY_PATH ${CLHEP_INCLUDE_DIR}:${CLHEP_LIBRARY_DIR}:${LD_LIBRARY_PATH}     

# setup VGM
#setenv VGM_INSTALL_DIR ${SOFTWARE_DIR}/vgm.4.3-install

# setup t2k-nd280-upgrade
setenv T2KND280UP ${SOFTWARE_DIR}/t2k-nd280-upgrade
setenv T2KND280UP_SRC ${T2KND280UP}/src/app/EffStudy 
setenv T2KND280UP_EVT ${T2KND280UP}/src/app/nd280UpEvent
setenv T2KND280UP_RUN /gpfs/fs03/t2k/beam/work/kiwamoto/ND280Upgrade/Run
setenv T2KND280UP_REC ${T2KND280UP}/src/app/Reconstruction
setenv T2KND280UP_ELPH /gpfs/fs03/t2k/beam/work/kiwamoto/ELPH_11122018

echo "DONE T2KND280Up_CLHEP.csh"
