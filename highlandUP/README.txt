
Since HighLAND depends on ROOT we should tell the system where ROOT is by setting the ROOTSYS environment variable. So if it has not been set yet do it by hand

export ROOTSYS=FOLDER WHERE ROOT include, bin and lib directories are (In tcsh or csh shells, use setenv instead of export.)



One by one instructions to do the installation

If you don't have CMT available in your system get it from gitLab. First create a folder, where you will put CMT . Go inside that directory and get the CMT package:

git clone http://next.ific.uv.es:8888/sw/CMT.git

To install CMT follow these three instructions

cd CMT
./install_cmt
source setup.sh

The latest instruction tells the system where CMT has been installed.

Then go to the t2k-nd280-upgrade folder 

Before compiling we must indicate which are the folders in which CMT should find the code. This is done setting the CMTPATH environment variable. Since HighLAND depends on ROOT we should indicate both the path for HighLAND and the path for ROOT. The ROOTSYS environment variable is needed by ROOT, so if it has not been set yet do it by hand

export ROOTSYS=FOLDER WHERE ROOT include, bin and lib directories are   (In tcsh or csh shells, use setenv instead of export.)

As explained in root_path page there are several ways of telling CMT where ROOT is, but the simplest one is to download from git a CMT package than bundles ROOT:

git clone http://next.ific.uv.es:8888/sw/ROOT_CMT.git ROOT/v0r0

and then specify a single path for CMT, which corresponds to HIGHLANDUP_PATH=/Path/t2k-nd280-upgrade, or since we are already there, just type

export CMTPATH=$PWD   (In tcsh or csh shells, use setenv instead of export.)

Then go inside the cmt folder of the top level package:

cd highlandUP/numuCCAnalysis/v0r0/cmt

create the makefiles recursively in all packages

cmt broadcast cmt config

and compile recursively

cmt broadcast make

This will compile all packages and produce the executables that we can run. Finally just source the setup.sh(.csh) script such that few environtment variables are set

source setup.sh(.csh)

Now you are ready to run the example and make your first HighLAND plots
