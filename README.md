# T2K ND280 upgrade simulation package

## Installation

1. At T2KND280Up_CLHEP.sh specify 4 correct paths at the beginning of the file:
```bash
export GEANT4_DIR=
export CLHEP_DIR=
export CMT_DIR=
export ROOT_DIR=
```
all the other paths will be assigned automatically. CLHEP could be taken from the GEANT4 installation. CMT is really necessary only if you will use highland. For the G4 sim and read out the CMT installation is not obligate.

2. Run 
```bash
source T2KND280Up_CLHEP.sh $PWD
```
3. Configure the cmake installation
```bash
mkdir build_dir; cd builddir;
cmake t2k-nd280-upgrade/src/;
```
4. Run the installation
```bash
make
```
We strongly recomend use multicore compilation for speed up, e.g. `make -J6`

## Run the code

### Run G4 simulation
The example of the command you may run for the G4 simulation of the ND280 upgrade
```bash
./submit/submit_nd280upgrade_sim 
--nruns 40 --nexpts 2500 
--config  config/RealTarget/configuration_cluster_SuperFGD_proto2.xml 
--vis-file mac/vis_cluster_pion1000.mac
--job-dir /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/proto2/ 
--tag proto2_pi_1000 --softw-dir /t2k/users/suvorov/dev/t2k-nd280-upgrade/
```
- ./submit/submit_nd280upgrade_sim - script from github
config/RealTarget/configuration_cluster_SuperFGD_proto2.xml - the detector configuration XML file, where you can specify the subdetectors position and size and switch some of them on/off

Notice1: ND280 basket center is hardcoded 16 mm down due to mechanical compression of the support structure. With this trick the particle gun from Y=0 will start 16 mm down by Y axis. If you want to prevent it, put the target 16 mm up along Y axis.


Notice2: File GENIE_geometry_SuperFGD_1e21_numu.root in the 28 line is obligatory but not used in the sim (I hope we will find time to solve lots of such bugs soon). Can use any genie file or download from IRODS QMULZone1/home/ND280Upgrade/GENIE

- --vis-file mac/vis_cluster_pion1000.mac attached GEANT4 command file. Configure PG here with /gun/*
- --nruns 40 --nexpts 2500 40 runs with 2500 events each
- --job-dir output dir
- --tag output file name

In order to optimize CPU and disk space usage you can change the output details. For example you can store the hits from all particles or from the primaries only. This should be set up at /src/app/EffStudy/src/ExN02TrackerSD.cc:106. Also you can specify which trajectories and which trajectory points you want to save. By default only primary trajectories and start/end point in each subdetector is stored. You can change these settings in /src/app/EffStudy/src/ND280RootPersistencyManager.cc.

### Run the G4 output analysis
Then you can run a macro. The examples could be found in the macro folder. To run the macro you need to tell root how to read ND280 events, so it will be like this:
```bash
root -l -q -b /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+ 
/t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+ 
/t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+ 
/t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.cc+ 
/t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+ 'macro/your_macro.C’
```
The other way to analyse the G4 output is to run the reconstruction code that includes ReadOut simulation. The example of the run command is:
```bash
./submit/Reconstruction/submit_nd280upgrade_targetreco 
--inputfile /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/proto1/proto1_p_1000MeV_c12-50000.root 
--job-dir /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/proto1/ 
--softw-dir /t2k/users/suvorov/dev/t2k-nd280-upgrade/ 
--nruns 40 --nexpts 2500 --cut-truth-fv 0  --debug 0 --debug-plot 0 --tag proto1_p_1000_c12_noCT

```
To run the specific analysis you need to uncomment one in the /src/app/Reconstruction/Main.cc and recompile.

### Run the Highland analysis

For more information see
http://nd280.lancs.ac.uk/devel/invariant/highlandND280Doc/
and 
https://indico.cern.ch/event/724624/contributions/3083209/attachments/1693650/2725603/highlandUp_documentation_v1.pdf
