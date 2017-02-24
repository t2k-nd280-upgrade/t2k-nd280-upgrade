setenv SOFTWARE_DIR  /sps/t2k/mlamoure/dev/Upgrade

# setup GEANT4
source ${SOFTWARE_DIR}/geant4.10.01.p03-install/bin/geant4.csh ${SOFTWARE_DIR}/geant4.10.01.p03-install/bin

setenv G4INCLUDE ${SOFTWARE_DIR}/geant4.10.01.p03-install/include/Geant4/
setenv GEANT4_INCLUDE ${SOFTWARE_DIR}/geant4.10.01.p03-install/include/Geant4/
setenv GEANT4_INCLUDE_DIR ${SOFTWARE_DIR}/geant4.10.01.p03-install/include/Geant4/
setenv GEANT4_INSTALL_BIN ${SOFTWARE_DIR}/geant4.10.01.p03-install/bin/

setenv G4LIB ${SOFTWARE_DIR}/geant4.10.01.p03-install/lib64
setenv G4SYSTEM ${SOFTWARE_DIR}/geant4.10.01.p03-install/lib64
setenv GEANT4_LIBRARY ${SOFTWARE_DIR}/geant4.10.01.p03-install/lib64
setenv GEANT4_LIBRARY_DIR ${SOFTWARE_DIR}/geant4.10.01.p03-install/lib64

# setup ROOT
source /sps/t2k/mlamoure/repository/v12r1/ROOT/v5r34p34n00/Linux-x86_64/root/bin/thisroot.csh

# setup CMT
source /sps/t2k/mlamoure/CMT/setup.csh
setenv CMTPATH ${SOFTWARE_DIR}/t2k-nd280-upgrade:/sps/t2k/mlamoure/repository/v12r1

# setup highlandUp
source ${SOFTWARE_DIR}/t2k-nd280-upgrade/highlandUP/run_at_start_t2k.csh

# setup Xerces-C++
setenv XERCESC_INSTALL_DIR ${SOFTWARE_DIR}/xerces-c-3.1.3/xerces-c-3.1.3-install
setenv LD_LIBRARY_PATH ${XERCESC_INSTALL_DIR}/lib:${LD_LIBRARY_PATH}

# setup CLHEP
setenv CLHEP_BASE_DIR ${SOFTWARE_DIR}/CLHEP2.2.0.4-install/
setenv CLHEP_INCLUDE_DIR ${SOFTWARE_DIR}/CLHEP2.2.0.4-install/include
setenv CLHEP_LIBRARY_DIR ${SOFTWARE_DIR}/CLHEP2.2.0.4-install/lib
setenv CLHEP_LIB_DIR ${SOFTWARE_DIR}/CLHEP2.2.0.4-install/lib


setenv LD_LIBRARY_PATH ${CLHEP_INCLUDE_DIR}:${CLHEP_LIBRARY_DIR}:${LD_LIBRARY_PATH}     

# setup VGM
setenv VGM_INSTALL_DIR ${SOFTWARE_DIR}/vgm.4.3-install

# setup t2k-nd280-upgrade
setenv T2KND280UP ${SOFTWARE_DIR}/t2k-nd280-upgrade
setenv T2KND280UP_SRC ${T2KND280UP}/src/app/EffStudy 
setenv T2KND280UP_EVT ${T2KND280UP}/src/app/nd280UpEvent
setenv T2KND280UP_RUN=$1
