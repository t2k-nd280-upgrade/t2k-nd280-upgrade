
// 
// Class to define an ND280 upgrade track
// 
// 27/7/16 Davide Sgalaberna
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "TND280UpTrack.hh"

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
  fSDLength      = kUndefTrack;
  fLengthTarget1 = kUndefTrack;
  fLengthTarget2 = kUndefTrack;
  fLyzTPCUp1     = kUndefTrack;
  fLyzTPCUp2     = kUndefTrack;
  fLyzTPCDown1   = kUndefTrack;
  fLyzTPCDown2   = kUndefTrack;
  fLyzForwTPC1   = kUndefTrack;
  fLyzForwTPC2   = kUndefTrack;
  fLyzForwTPC3   = kUndefTrack;

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
  fSDLength      = kUndefTrack;

  fLengthTarget1 = kUndefTrack;
  fLengthTarget2 = kUndefTrack;
  fLyzTPCUp1     = kUndefTrack;
  fLyzTPCUp2     = kUndefTrack;
  fLyzTPCDown1   = kUndefTrack;
  fLyzTPCDown2   = kUndefTrack;
  fLyzForwTPC1   = kUndefTrack;
  fLyzForwTPC2   = kUndefTrack;
  fLyzForwTPC3   = kUndefTrack;

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
  if(fLengthTarget1>0.){cout << "Length in Target1: " << fLengthTarget1 << endl;}
  if(fLengthTarget2>0.){cout << "Length in Target2: " << fLengthTarget2 << endl;}
  if(fLyzTPCUp1>0.){cout << "Lyz in TPCUp1: "         << fLyzTPCUp1     << endl;}
  if(fLyzTPCUp2>0.){cout << "Lyz in TPCUp2: "         << fLyzTPCUp2     << endl;}
  if(fLyzTPCDown1>0.){cout << "Lyz in TPCDown1: "     << fLyzTPCDown1   << endl;}
  if(fLyzTPCDown2>0.){cout << "Lyz in TPCDown2: "     << fLyzTPCDown2   << endl;}
  if(fLyzForwTPC1>0.){cout << "Lyz in ForwTPC1: "     << fLyzForwTPC1   << endl;}
  if(fLyzForwTPC2>0.){cout << "Lyz in ForwTPC2: "     << fLyzForwTPC2   << endl;}
  if(fLyzForwTPC3>0.){cout << "Lyz in ForwTPC3: "     << fLyzForwTPC3   << endl;}

  cout << "Save track: " << ((fSaveIt) ? "YES " : "NO") << endl;
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
