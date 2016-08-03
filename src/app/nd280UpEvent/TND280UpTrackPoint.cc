
// 
// Class to define an ND280 upgrade track point
// 
// 27/7/16 Davide Sgalaberna
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "TND280UpTrackPoint.hh"

using namespace std;

ClassImp(TND280UpTrackPoint)

TND280UpTrackPoint::TND280UpTrackPoint(){

  fPointID = kUndefTrackPoint;
  fTime = kUndefTrackPoint;
  fMomentum.SetXYZ(kUndefTrackPoint,
		   kUndefTrackPoint,
		   kUndefTrackPoint);
  fEdep = kUndefTrackPoint;
  fStepLength = kUndefTrackPoint;
  fStepDeltaLyz = kUndefTrackPoint;
  fStepStatus = kUndefTrackPoint;
  fPhysVolName = "";
  fPrePosition.SetXYZ(kUndefTrackPoint,
		      kUndefTrackPoint,
		      kUndefTrackPoint);
  fPostPosition.SetXYZ(kUndefTrackPoint,
		       kUndefTrackPoint,
		       kUndefTrackPoint);
  fSaveIt = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
TND280UpTrackPoint::~TND280UpTrackPoint(){
  //delete ...;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

void TND280UpTrackPoint::PrintTrackPoint()
{
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
  cout << "TrackPoint ID: " << fPointID << endl;
  cout << "Time: " << fTime << endl;
  cout << "Momentum: " << fMomentum.Mag() << endl;
  cout << "Deposited Energy: " << fEdep << endl;
  cout << "Step Length: " << fStepLength << endl;
  cout << "Step DeltaLyz: " << fStepDeltaLyz << endl;
  cout << "Step Status: " << fStepStatus << endl;
  cout << "Phys Volume Name: " << fPhysVolName << endl;
  cout << "Is on boundary: " << ((fIsOnBoundary) ? "YES " : "NO") << endl;
  cout << "Save it: " << ((fSaveIt) ? "YES " : "NO") << endl;
  cout << "preStep position: ("  
       << fPrePosition.X() << ", " 
       << fPrePosition.Y() << ", "
       << fPrePosition.Z() << ") "
       << endl;
  cout << "postStep position: (" 
       << fPostPosition.X() << ", "
       << fPostPosition.Y() << ", "
       << fPostPosition.Z() << ") " 
       << endl;
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

