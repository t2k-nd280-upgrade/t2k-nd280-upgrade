#!/bin/sh
hostname
echo job 10
date
export CMTPATH=/atlas/users/scantamb/logbook/2016/05/h2_v2r4/:/atlas/users/scantamb/nd280software/nd280v11r33/
source /atlas/users/scantamb/logbook/2016/05/h2_v2r4/nd280Highland2/v2r4/cmt/setup.sh
echo CMTPATH is $CMTPATH
TMPDIR=`mktemp -d`
echo Running RunNumuCCAnalysis.exe in $TMPDIR for FlatTree_nd280Highland2v1r3p1_Prod6_FileList_90210032-0012.txt.root
RunNumuCCAnalysis.exe /ams/data4/neutrino/iRODS_files/QMULZone1/home/asg/asg2015oa_jointfit/ND280/numu/FlatTrees/MC/Run2//WaterIn/FlatTree_nd280Highland2v1r3p1_Prod6_FileList_90210032-0012.txt.root -p /atlas/users/scantamb/logbook/2016/05/baseTrackerAnalysis.parameters.dat.my -o $TMPDIR/numuCC_h2v2r4_FlatTree_nd280Highland2v1r3p1_Prod6_FileList_90210032-0012.txt.root_.root &>/neutrino/data7/enrico/2016_05/v2r3/Run2_Neut/microtrees_v2r4/outfiles/numuCC_h2v2r4_FlatTree_nd280Highland2v1r3p1_Prod6_FileList_90210032-0012.txt.root_.out
echo -n Finished RunNumuCCAnalysis.exe on:
date
echo Sleeping 44 seconds
sleep 44
echo Moving $TMPDIR/numuCC_h2v2r4_FlatTree_nd280Highland2v1r3p1_Prod6_FileList_90210032-0012.txt.root_.root
echo into /neutrino/data7/enrico/2016_05/v2r3/Run2_Neut/microtrees_v2r4/outfiles/
echo as 10/
mv $TMPDIR/numuCC_h2v2r4_FlatTree_nd280Highland2v1r3p1_Prod6_FileList_90210032-0012.txt.root_.root /neutrino/data7/enrico/2016_05/v2r3/Run2_Neut/microtrees_v2r4/outfiles/
cp -u /atlas/data2/undelivered-logs/scantamb/* /neutrino/data6/scantamb//undelivered-logs/
echo Cleanup done!
date
#qsub -q veryshort -l mem=6000mb -l vmem=6000mb -N numuCC_h2v2r410 /neutrino/data7/enrico/2016_05/v2r3/Run2_Neut/microtrees_v2r4/batchjobs/batch_script_10.sh -e /neutrino/data7/enrico/2016_05/v2r3/Run2_Neut/microtrees_v2r4/logfiles/err_10.log -o /neutrino/data7/enrico/2016_05/v2r3/Run2_Neut/microtrees_v2r4/logfiles/log_10.log
