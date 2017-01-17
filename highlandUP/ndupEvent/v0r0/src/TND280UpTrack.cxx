
// 
// Class to define an ND280 upgrade track
// 
// 27/7/16 Davide Sgalaberna
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

#include "TND280UpTrack.hxx"

using namespace std;

ClassImp(TND280UpTrack)

TND280UpTrack::TND280UpTrack()
{  
  fTrackID       = kUndefTrack;
  fParentID      = kUndefTrack;
  fProcessName   = "";
  fPDG           = kUndefTrack;
  fInitKinEnergy = kUndefTrack;
  fInitMom.SetXYZ(kUndefTrack,
		  kUndefTrack,
		  kUndefTrack);
  fInitCosTheta  = kUndefTrack;
  fCharge        = kUndefTrack;
  fRange         = kUndefTrack;
  fSaveIt        = true;
  fSDTotalEnergyDeposit = kUndefTrack;
  fSDLength       = kUndefTrack;

  fLengthTarget1  = kUndefTrack;
  fLengthTarget2  = kUndefTrack;
  fLengthFGD1     = kUndefTrack;
  fLengthFGD2  = kUndefTrack;
  fLengthTPCUp1   = kUndefTrack;
  fLengthTPCUp2   = kUndefTrack;
  fLengthTPCDown1 = kUndefTrack;
  fLengthTPCDown2 = kUndefTrack;
  fLengthForwTPC1 = kUndefTrack;
  fLengthForwTPC2 = kUndefTrack;
  fLengthForwTPC3 = kUndefTrack;
  fLengthDsECal   = kUndefTrack;
  fLengthBrlECal  = kUndefTrack;
  fLengthP0DECal  = kUndefTrack;

  fLyzTPCUp1     = kUndefTrack;
  fLyzTPCUp2     = kUndefTrack;
  fLyzTPCDown1   = kUndefTrack;
  fLyzTPCDown2   = kUndefTrack;
  fLyzForwTPC1   = kUndefTrack;
  fLyzForwTPC2   = kUndefTrack;
  fLyzForwTPC3   = kUndefTrack;
 
  fEdepTarget1   = kUndefTrack;
  fEdepTarget2   = kUndefTrack;
  fEdepFGD1      = kUndefTrack;
  fEdepFGD2      = kUndefTrack;
  fEdepTPCUp1     = kUndefTrack;
  fEdepTPCUp2     = kUndefTrack;
  fEdepTPCDown1   = kUndefTrack;
  fEdepTPCDown2   = kUndefTrack;
  fEdepForwTPC1   = kUndefTrack;
  fEdepForwTPC2   = kUndefTrack;
  fEdepForwTPC3   = kUndefTrack;
  fEdepDsECal     = kUndefTrack;
  fEdepBrlECal    = kUndefTrack;
  fEdepP0DECal    = kUndefTrack;

  fNPoints = 0;
  fListOfPoints = new TList;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TND280UpTrack::~TND280UpTrack()
{
  //delete ...;


  fTrackID       = kUndefTrack;
  fParentID      = kUndefTrack;
  fProcessName   = "";
  fPDG           = kUndefTrack;
  fInitKinEnergy = kUndefTrack;
  fInitMom.SetXYZ(kUndefTrack,
		  kUndefTrack,
		  kUndefTrack);
  fInitCosTheta  = kUndefTrack;
  fCharge        = kUndefTrack;
  fRange         = kUndefTrack;
  fSaveIt        = true;
  fSDTotalEnergyDeposit = kUndefTrack;
  fSDLength       = kUndefTrack;

  fLengthTarget1  = kUndefTrack;
  fLengthTarget2  = kUndefTrack;
  fLengthFGD1     = kUndefTrack;
  fLengthFGD2  = kUndefTrack;
  fLengthTPCUp1   = kUndefTrack;
  fLengthTPCUp2   = kUndefTrack;
  fLengthTPCDown1 = kUndefTrack;
  fLengthTPCDown2 = kUndefTrack;
  fLengthForwTPC1 = kUndefTrack;
  fLengthForwTPC2 = kUndefTrack;
  fLengthForwTPC3 = kUndefTrack;
  fLengthDsECal   = kUndefTrack;
  fLengthBrlECal  = kUndefTrack;
  fLengthP0DECal  = kUndefTrack;

  fLyzTPCUp1     = kUndefTrack;
  fLyzTPCUp2     = kUndefTrack;
  fLyzTPCDown1   = kUndefTrack;
  fLyzTPCDown2   = kUndefTrack;
  fLyzForwTPC1   = kUndefTrack;
  fLyzForwTPC2   = kUndefTrack;
  fLyzForwTPC3   = kUndefTrack;
 
  fEdepTarget1   = kUndefTrack;
  fEdepTarget2   = kUndefTrack;
  fEdepFGD1      = kUndefTrack;
  fEdepFGD2      = kUndefTrack;
  fEdepTPCUp1     = kUndefTrack;
  fEdepTPCUp2     = kUndefTrack;
  fEdepTPCDown1   = kUndefTrack;
  fEdepTPCDown2   = kUndefTrack;
  fEdepForwTPC1   = kUndefTrack;
  fEdepForwTPC2   = kUndefTrack;
  fEdepForwTPC3   = kUndefTrack;
  fEdepDsECal     = kUndefTrack;
  fEdepBrlECal    = kUndefTrack;
  fEdepP0DECal    = kUndefTrack;

  fNPoints = 0;

  fListOfPoints->Delete();
  delete fListOfPoints;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......   

void TND280UpTrack::AddPoint(TND280UpTrackPoint *point)
{
  fListOfPoints->Add(point);
  fNPoints = fListOfPoints->GetEntries(); // Update # of point in the track
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

TND280UpTrackPoint * TND280UpTrack::GetPoint(int ptid)
{
  TObject *objpoint = fListOfPoints->At(ptid);
  if(!objpoint){
    cerr << endl;
    cerr << "TND280UpTrack::GetPoint" << endl;
    cerr << "The index " << ptid << " is out of range" << endl;
    cerr << endl;
    exit(1);
  }
  TND280UpTrackPoint *point = dynamic_cast<TND280UpTrackPoint*>(objpoint); 
  return point;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

void TND280UpTrack::PrintTrack()
{
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
  cout << "Track ID: " << fTrackID << endl;
  cout << "Parent ID: " << fParentID << endl;
  cout << "# of points: " << fNPoints << endl;
  cout << "Process name: " << fProcessName << endl;
  cout << "PDG: " << fPDG << endl;
  cout << "Initial Kinetic Energy: " << fInitKinEnergy << endl;
  cout << "Initial Momentum: " << fInitMom.Mag() << endl;
  cout << "Initial CosTheta: " << fInitCosTheta << endl;
  cout << "Charge: " << fCharge << endl;
  cout << "Range: " << fRange << endl;
  cout << "SD total energy deposited: " << fSDTotalEnergyDeposit << endl;
  cout << "SD length: " << fSDLength << endl;

  if(fLengthTarget1>0.){cout << "Length in Target1: "   << fLengthTarget1 << endl;}
  if(fLengthTarget2>0.){cout << "Length in Target2: "   << fLengthTarget2 << endl;}
  if(fLengthFGD1>0.){cout << "Length in FGD1: "   << fLengthFGD1 << endl;}
  if(fLengthFGD2>0.){cout << "Length in FGD2: "   << fLengthFGD2 << endl;}
  if(fLengthTPCUp1>0.){cout << "Length in TPCUp1: "     << fLengthTPCUp1     << endl;}
  if(fLengthTPCUp2>0.){cout << "Length in TPCUp2: "     << fLengthTPCUp2     << endl;}
  if(fLengthTPCDown1>0.){cout << "Length in TPCDown1: " << fLengthTPCDown1   << endl;}
  if(fLengthTPCDown2>0.){cout << "Length in TPCDown2: " << fLengthTPCDown2   << endl;}
  if(fLengthForwTPC1>0.){cout << "Length in ForwTPC1: " << fLengthForwTPC1   << endl;}
  if(fLengthForwTPC2>0.){cout << "Length in ForwTPC2: " << fLengthForwTPC2   << endl;}
  if(fLengthForwTPC3>0.){cout << "Length in ForwTPC3: " << fLengthForwTPC3   << endl;}
  if(fLengthDsECal>0.) {cout << "Length in DsECal: "  << fLengthDsECal   << endl;}
  if(fLengthBrlECal>0.){cout << "Length in BrlECal: " << fLengthBrlECal   << endl;}
  if(fLengthP0DECal>0.){cout << "Length in P0DECal: " << fLengthP0DECal   << endl;}

  if(fLyzTPCUp1>0.){cout << "Lyz in TPCUp1: "         << fLyzTPCUp1     << endl;}
  if(fLyzTPCUp2>0.){cout << "Lyz in TPCUp2: "         << fLyzTPCUp2     << endl;}
  if(fLyzTPCDown1>0.){cout << "Lyz in TPCDown1: "     << fLyzTPCDown1   << endl;}
  if(fLyzTPCDown2>0.){cout << "Lyz in TPCDown2: "     << fLyzTPCDown2   << endl;}
  if(fLyzForwTPC1>0.){cout << "Lyz in ForwTPC1: "     << fLyzForwTPC1   << endl;}
  if(fLyzForwTPC2>0.){cout << "Lyz in ForwTPC2: "     << fLyzForwTPC2   << endl;}
  if(fLyzForwTPC3>0.){cout << "Lyz in ForwTPC3: "     << fLyzForwTPC3   << endl;}

  if(fEdepTarget1>0.){cout << "Edep in Target1: "     << fEdepTarget1 << endl;}
  if(fEdepTarget2>0.){cout << "Edep in Target2: "     << fEdepTarget2 << endl;}
  if(fEdepFGD1>0.){cout << "Edep in FGD1: "     << fEdepFGD1 << endl;}
  if(fEdepFGD2>0.){cout << "Edep in FGD2: "     << fEdepFGD2 << endl;}
  if(fEdepTPCUp1>0.){cout << "Edep in TPCUp1: "       << fEdepTPCUp1     << endl;}
  if(fEdepTPCUp2>0.){cout << "Edep in TPCUp2: "       << fEdepTPCUp2     << endl;}
  if(fEdepTPCDown1>0.){cout << "Edep in TPCDown1: "   << fEdepTPCDown1   << endl;}
  if(fEdepTPCDown2>0.){cout << "Edep in TPCDown2: "   << fEdepTPCDown2   << endl;}
  if(fEdepForwTPC1>0.){cout << "Edep in ForwTPC1: "   << fEdepForwTPC1   << endl;}
  if(fEdepForwTPC2>0.){cout << "Edep in ForwTPC2: "   << fEdepForwTPC2   << endl;}
  if(fEdepForwTPC3>0.){cout << "Edep in ForwTPC3: "   << fEdepForwTPC3   << endl;}
  if(fEdepDsECal>0.) {cout << "Edep in DsECal: "    << fEdepDsECal    << endl;}
  if(fEdepBrlECal>0.){cout << "Edep in BrlECal: "   << fEdepBrlECal   << endl;}
  if(fEdepP0DECal>0.){cout << "Edep in P0DECal: "   << fEdepP0DECal   << endl;}

  cout << "Save track: " << ((fSaveIt) ? "YES " : "NO") << endl;
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
