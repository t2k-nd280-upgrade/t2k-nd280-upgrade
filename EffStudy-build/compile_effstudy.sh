
#source /afs/cern.ch/user/d/dsgalabe/GEANT4/v10r1p03_dataON/geant4.10.01.p03-build/geant4make.sh
#source ~/T2K/work/v11r31/ROOT/v5r34p09n04/amd64_linux26/bin/thisroot.sh
#echo $ROOTSYS

echo
echo "Use $1 CPU cores" 
echo "Path to code directory: $2"
echo

cmake -DGeant4_DIR=/afs/cern.ch/user/d/dsgalabe/GEANT4/v10r1p03_dataON/geant4.10.01.p03-install/lib64/Geant4-10.1.3/ $2

make -j$1