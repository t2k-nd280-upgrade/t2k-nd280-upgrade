#ifndef _COMMON_HEADER
#define _COMMON_HEADER
#ifndef __CINT__

// ROOT libraries
#include "TROOT.h"
#include "TRint.h"
#include "TApplication.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TNtuple.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TObject.h"
#include <TStyle.h>
#include <TTree.h>
#include <TBranch.h>
#include <TObject.h>
#include <TClonesArray.h>
#include <TClass.h>
#include <TChain.h>
#include <TLegend.h>

//C++ libraries
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <array>

//Simplify things using std namespace
using namespace std;

//Add dictionary
#include "../utils/LinkDef.h"

//Reconstruction classes
#include "../../src/app/nd280UpEvent/ND280SFGDVoxelSet.cc"
#include "../../src/app/nd280UpEvent/ND280SFGDVoxel.cc"
#include "../../src/app/nd280UpEvent/ND280SFGDHit.cc"
#include "../../src/app/nd280UpEvent/ND280SFGDTrack.cc"
#include "../../src/app/nd280UpEvent/ND280SFGDEvent.cc"

//Add extra functionalitirs and T2kstyle
//#include "global_tools.C"

#endif //CINT

//try to avoid main in Aclic compiled code
#ifndef __MAKECINT__

#ifndef __CINT__

void THIS_NAME();

//*****************************************************

int main(int argc, char* argv[]){
#ifdef INTERACTIVE_OUTPUT

#ifdef OVERRIDE_OPTIONS
TRint* theApp=new TRint("App",&argc,argv,0,-1);
#else
TRint* theApp=new TRint("App",&argc,argv);
#endif

#else
#ifdef OVERRIDE_OPTIONS
 TApplication* theApp=new TApplication("App",&argc,argv,0,-1);
#else
 TApplication* theApp=new TApplication("App",&argc,argv);
#endif
#endif

THIS_NAME();
theApp->Run();
exit(0);
return 0;
}

//*****************************************************

#endif //CINT
#endif //makecint
#endif // Common header
