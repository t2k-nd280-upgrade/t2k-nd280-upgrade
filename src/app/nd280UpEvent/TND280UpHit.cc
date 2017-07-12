
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
  ///// NEW ND280 HitSegment
  fContributors.clear();
  fPrimaryId = kUndefHit;
  fStartX = kUndefHit;
  fStartY = kUndefHit;
  fStartZ = kUndefHit;
  fStartT = kUndefHit;
  fStopX = kUndefHit;
  fStopY = kUndefHit;
  fStopZ = kUndefHit;
  fStopT = kUndefHit;
  fEnergyDeposit = 0.;
  fTrackLength = 0.;
  ////////

  // true
  fHitID       = kUndefHit;
  fPDG         = kUndefHit;
  fTrackID     = kUndefHit; // NOT used
  fParentID    = kUndefHit; // NOT used
  fEdep        = kUndefHit; // NOT used
  fLocPosX     = kUndefHit; // used
  fLocPosY     = kUndefHit; // used
  fLocPosZ     = kUndefHit; // used
  fTime        = kUndefHit; // NOT used
  fDetName     = ""; // used
  // reco
  fPEX         = kUndefHit;
  fPEY         = kUndefHit;
  fPEZ         = kUndefHit;
  fMPPCPosX    = kUndefHit;
  fMPPCPosY    = kUndefHit;
  fMPPCPosZ    = kUndefHit;
  fTimePEX     = kUndefHit;
  fTimePEY     = kUndefHit;
  fTimePEZ     = kUndefHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TND280UpHit::~TND280UpHit()
{
  //delete ...;

  ///// NEW ND280 HitSegment
  fContributors.clear();
  fPrimaryId = kUndefHit;
  fStartX = kUndefHit;
  fStartY = kUndefHit;
  fStartZ = kUndefHit;
  fStartT = kUndefHit;
  fStopX = kUndefHit;
  fStopY = kUndefHit;
  fStopZ = kUndefHit;
  fStopT = kUndefHit;
  fEnergyDeposit = 0.;
  fTrackLength = 0.;
  ////////

  // true
  fHitID       = kUndefHit;
  fPDG         = kUndefHit;
  fTrackID     = kUndefHit;
  fParentID    = kUndefHit;
  fEdep        = kUndefHit;
  fLocPosX     = kUndefHit;
  fLocPosY     = kUndefHit;
  fLocPosZ     = kUndefHit;
  fTime        = kUndefHit;
  fDetName     = "";
  // reco
  fPEX         = kUndefHit;
  fPEY         = kUndefHit;
  fPEZ         = kUndefHit;
  fMPPCPosX    = kUndefHit;
  fMPPCPosY    = kUndefHit;
  fMPPCPosZ    = kUndefHit;
  fTimePEX     = kUndefHit;
  fTimePEY     = kUndefHit;
  fTimePEZ     = kUndefHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......   
void TND280UpHit::PrintHit()
{
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;

  cout << "Truth:" << endl;
  cout << " - Hit ID: " << fHitID << endl;
  cout << " - PDG: " << fPDG << endl;
  cout << " - Track ID: " << fTrackID << endl;
  cout << " - Parent ID: " << fParentID << endl;
  cout << " - Detector Name: " << fDetName << endl;
  cout << " - Edep: " << fEdep << endl;
  cout << " - Time: " << fTime << endl;  
  cout << " - Local Position: "  
       << fLocPosX << ", "
       << fLocPosY << ", "
       << fLocPosZ << endl;

  cout << "Recostructed:" << endl;
  cout << " - MPPC Position: " 
       << fMPPCPosX << ", "
       << fMPPCPosY << ", "
       << fMPPCPosZ << endl;
  cout << " - # of PE X: " << fPEX << endl;
  cout << " - # of PE Y: " << fPEY << endl;
  cout << " - # of PE Z: " << fPEZ << endl;
  cout << " - PE Time X: " << fTimePEX << endl;
  cout << " - PE Time Y: " << fTimePEY << endl;
  cout << " - PE Time Z: " << fTimePEZ << endl;
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
