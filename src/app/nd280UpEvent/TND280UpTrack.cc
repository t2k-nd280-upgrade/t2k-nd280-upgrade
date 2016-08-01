
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
  
  fNPoints = 0;
  fListOfPoints = new TList;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TND280UpTrack::~TND280UpTrack()
{
  //delete ...;
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
  cout << "Save track: " << ((fSaveIt) ? "YES " : "NO") << endl;
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
