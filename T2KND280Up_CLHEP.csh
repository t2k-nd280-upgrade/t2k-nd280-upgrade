

# setup GEANT4
source /sps/t2k/mlamoure/dev/Upgrade/geant4.10.01.p03-install/bin/geant4.csh /sps/t2k/mlamoure/dev/Upgrade/geant4.10.01.p03-install/bin

setenv GEANT4_INCLUDE /sps/t2k/mlamoure/dev/Upgrade/geant4.10.01.p03-install/include/Geant4/
setenv GEANT4_INCLUDE_DIR /sps/t2k/mlamoure/dev/Upgrade/geant4.10.01.p03-install/include/Geant4/
setenv GEANT4_INSTALL_BIN /sps/t2k/mlamoure/dev/Upgrade/geant4.10.01.p03-install/bin/
setenv GEANT4_LIBRARY /sps/t2k/mlamoure/dev/Upgrade/geant4.10.01.p03-install/lib64
setenv GEANT4_LIBRARY_DIR /sps/t2k/mlamoure/dev/Upgrade/geant4.10.01.p03-install/lib64

# setup ROOT
source /sps/t2k/mlamoure/repository/v12r1/ROOT/v5r34p34n00/Linux-x86_64/root/bin/thisroot.csh

# setup CMT
source /sps/t2k/mlamoure/CMT/setup.csh
setenv CMTPATH ${PWD}:/sps/t2k/mlamoure/repository/v12r1

# setup highlandUp
source /sps/t2k/mlamoure/dev/Upgrade/t2k-nd280-upgrade/highlandUP/ndupAnalysis/v0r0/cmt/setup.csh

# setup Xerces-C++
setenv XERCESC_INSTALL_DIR /sps/t2k/mlamoure/dev/Upgrade/xerces-c-3.1.3/xerces-c-3.1.3-install
setenv LD_LIBRARY_PATH ${XERCESC_INSTALL_DIR}/lib:${LD_LIBRARY_PATH}

# setup CLHEP
setenv CLHEP_INCLUDE_DIR /sps/t2k/mlamoure/dev/Upgrade/CLHEP2.2.0.4-install/include
setenv CLHEP_LIBRARY_DIR /sps/t2k/mlamoure/dev/Upgrade/CLHEP2.2.0.4-install/lib
setenv LD_LIBRARY_PATH ${CLHEP_INCLUDE_DIR}:${CLHEP_LIBRARY_DIR}:${LD_LIBRARY_PATH}     

# setup VGM
setenv VGM_INSTALL_DIR /sps/t2k/mlamoure/dev/Upgrade/vgm.4.3-install

setenv GENIE /sps/t2k/mlamoure/dev/Upgrade/GENIE

# setup t2k-nd280-upgrade
setenv T2KND280UP ${PWD}
setenv T2KND280UP_SRC ${T2KND280UP}/src/app/EffStudy 
setenv T2KND280UP_EVT ${T2KND280UP}/src/app/nd280UpEvent
