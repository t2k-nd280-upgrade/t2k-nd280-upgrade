
// 
// Class to define an ND280 upgrade hit
// 
// 26/4/17 Davide Sgalaberna
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

#include "TND280UpHit.hh"

using namespace std;

ClassImp(TND280UpHit)

TND280UpHit::TND280UpHit()
{  
  fHitID       = kUndefHit;
  fProcessName = "";
  fSaveIt      = true;
  fDetID       = kUndefHit;
  fPDG         = kUndefHit;
  fTrackID     = kUndefHit;
  fParentID    = kUndefHit;
  fEdep        = kUndefHit;
  fEdep_Q      = kUndefHit;
  fPE          = kUndefHit;
  fPEX         = kUndefHit;
  fPEY         = kUndefHit;
  fPEZ         = kUndefHit;
  fLocPosX     = kUndefHit;
  fLocPosY     = kUndefHit;
  fLocPosZ     = kUndefHit;
  fTime        = kUndefHit;
  fTimePE      = kUndefHit;
  fTimePEX     = kUndefHit;
  fTimePEY     = kUndefHit;
  fTimePEZ     = kUndefHit;
  fDetName     = "";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TND280UpHit::~TND280UpHit()
{
  //delete ...;

  fHitID       = kUndefHit;
  fProcessName = "";
  fSaveIt      = true;
  fDetID       = kUndefHit;
  fPDG         = kUndefHit;
  fTrackID     = kUndefHit;
  fParentID    = kUndefHit;
  fEdep        = kUndefHit;
  fEdep_Q      = kUndefHit;
  fPE          = kUndefHit;
  fPEX         = kUndefHit;
  fPEY         = kUndefHit;
  fPEZ         = kUndefHit;
  fLocPosX     = kUndefHit;
  fLocPosY     = kUndefHit;
  fLocPosZ     = kUndefHit;
  fTime        = kUndefHit;
  fTimePE      = kUndefHit;
  fTimePEX     = kUndefHit;
  fTimePEY     = kUndefHit;
  fTimePEZ     = kUndefHit;
  fDetName     = "";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......   
void TND280UpHit::PrintHit()
{
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
  cout << "Hit ID: " << fHitID << endl;
  cout << "Process name: " << fProcessName << endl;
  cout << "Save hit: " << ((fSaveIt) ? "YES " : "NO") << endl;
  cout << "Detector ID: " << fDetID << endl;
  cout << "Detector Name: " << fDetName << endl;
  cout << "PDG: " << fPDG << endl;
  cout << "Track ID: " << fTrackID << endl;
  cout << "Parent ID: " << fParentID << endl;
  cout << "Time: " << fTime << endl;
  cout << "PE Time: " << fTimePE << endl;
  cout << "PE Time X: " << fTimePEX << endl;
  cout << "PE Time Y: " << fTimePEY << endl;
  cout << "PE Time Z: " << fTimePEZ << endl;
  cout << "Edep: " << fEdep << endl;
  cout << "Edep_Q: " << fEdep_Q << endl;
  cout << "# of PE: " << fPE << endl;
  cout << "# of PE X: " << fPEX << endl;
  cout << "# of PE Y: " << fPEY << endl;
  cout << "# of PE Z: " << fPEZ << endl;
  cout << "Local Position: "  
       << fLocPosX << ", "
       << fLocPosY << ", "
       << fLocPosZ << endl;
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
