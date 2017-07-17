
#include "ND280UpPID.hh"

using namespace std;


ND280UpPID::ND280UpPID()
{
  this->Init();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ND280UpPID::~ND280UpPID()
{
  this->Delete();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpPID::Init(){
  fMPPCHit_XY=0;
  fMPPCHit_XZ=0;
  fMPPCHit_YZ=0;

  fTrackID=0;
  fPDG=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpPID::Delete(){
  
  if(fMPPCHit_XY)fMPPCHit_XY->Delete(); fMPPCHit_XY=0; 
  if(fMPPCHit_XZ)fMPPCHit_XZ->Delete(); fMPPCHit_XZ=0; 
  if(fMPPCHit_YZ)fMPPCHit_YZ->Delete(); fMPPCHit_YZ=0;

  fTrackID=0;
  fPDG=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpPID::ComputePDFs(){

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......









