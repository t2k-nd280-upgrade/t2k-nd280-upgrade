
// 
// Class to define an ND280 upgrade reco track
// 
// 4/7/17 Davide Sgalaberna
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

#include "ND280UpConst.hh"

#include "TND280UpRecoTrack.hh"

using namespace std;

ClassImp(TND280UpRecoTrack)

TND280UpRecoTrack::TND280UpRecoTrack()
{  
  fTrackID       = 0;
  fParentID      = 0;
  fPDG           = 0;
  fInitMom.SetXYZ(nd280upconv::kBadNum,
		  nd280upconv::kBadNum,
		  nd280upconv::kBadNum);
  fInitCosTheta  = nd280upconv::kBadNum;
  fCharge        = nd280upconv::kBadNum;
  fRange         = nd280upconv::kBadNum;

  fMPPCHit_XY=0; 
  fMPPCHit_XZ=0; 
  fMPPCHit_YZ=0; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TND280UpRecoTrack::~TND280UpRecoTrack()
{
  //delete ...;

  fTrackID       = 0;
  fParentID      = 0;
  fPDG           = 0;
  fInitMom.SetXYZ(nd280upconv::kBadNum,
		  nd280upconv::kBadNum,
		  nd280upconv::kBadNum);
  fInitCosTheta  = nd280upconv::kBadNum;
  fCharge        = nd280upconv::kBadNum;
  fRange         = nd280upconv::kBadNum;

  if(fMPPCHit_XY) fMPPCHit_XY->Delete(); fMPPCHit_XY=0; 
  if(fMPPCHit_XZ) fMPPCHit_XZ->Delete(); fMPPCHit_XZ=0; 
  if(fMPPCHit_YZ) fMPPCHit_YZ->Delete(); fMPPCHit_YZ=0; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......   

void TND280UpRecoTrack::PrintTrack()
{
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
  cout << "Track ID: " << fTrackID << endl;
  cout << "Parent ID: " << fParentID << endl;
  cout << "PDG: " << fPDG << endl;
  cout << "Initial Momentum: " << fInitMom.Mag() << endl;
  cout << "Initial CosTheta: " << fInitCosTheta << endl;
  cout << "Charge: " << fCharge << endl;
  cout << "Range: " << fRange << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
