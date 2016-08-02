//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: ExN02TrackerSD.cc,v 1.9 2006-06-29 17:48:27 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN02TrackerSD.hh"
#include "ExN02Constants.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerSD::ExN02TrackerSD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="trackerCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerSD::~ExN02TrackerSD(){ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerSD::Initialize(G4HCofThisEvent* HCE)
{
  trackerCollection = new ExN02TrackerHitsCollection
                          (SensitiveDetectorName,collectionName[0]); 
  static G4int HCID = -1;
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( HCID, trackerCollection ); 
  
  NSteps_ = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ExN02TrackerSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  
  //if(NSteps_==0){

  G4double edep = aStep->GetTotalEnergyDeposit();
  
  if(edep==0.) return false;
  
  // The geometry is stored in the PreStepPoint()
  
  G4StepPoint* prestep  = aStep->GetPreStepPoint();
  G4StepPoint* poststep = aStep->GetPostStepPoint();
  G4Track* track        = aStep->GetTrack();
  
  G4double steplength   = aStep->GetStepLength(); // step length corrected w/ multiple scattering

  // Get geometrical step length on yz plane (transverse). Not corrected w/ multiple scattering

  G4ThreeVector deltapos = aStep->GetDeltaPosition(); // geometrical step length (not corrected)
  G4double deltalength = deltapos.mag();

  G4ThreeVector prestepPos = prestep->GetPosition();
  G4ThreeVector poststepPos = poststep->GetPosition();

  G4double prestep_x = prestepPos.x() / mm;
  G4double prestep_y = prestepPos.y() / mm;
  G4double prestep_z = prestepPos.z() / mm;

  G4double poststep_x = poststepPos.x() / mm;
  G4double poststep_y = poststepPos.y() / mm;
  G4double poststep_z = poststepPos.z() / mm;
  
  G4double stepdeltalyz = sqrt( (prestep_y - poststep_y)*(prestep_y - poststep_y) +
				(prestep_z - poststep_z)*(prestep_z - poststep_z) );

  // G4double steplength_oth = sqrt( (prestep_x - poststep_x)*(prestep_x - poststep_x) +
  //(prestep_y - poststep_y)*(prestep_y - poststep_y) +
  //(prestep_z - poststep_z)*(prestep_z - poststep_z) );

  //if( fabs(steplength_oth-steplength) > 0.0001){
  //G4cout << steplength << ", " 
  //<< deltalength << ", "
  //	   << steplength_oth << ": " 
  //	   << (prestep_x - poststep_x) << ", "
  //	   << (prestep_y - poststep_y) << ", "
  //	   << (prestep_z - poststep_z) << ", "
  //	   << G4endl;
  //}

  //if (prestep->GetStepStatus() == fGeomBoundary)  return false;
  //if (poststep->GetStepStatus() == fGeomBoundary) return false;  

  //
  // Use the preStep because the SDManager is referred to the preStep detector!!!
  //
  G4String namedet     = prestep->GetTouchableHandle()->GetVolume()->GetName();
  //G4String namedet     = poststep->GetTouchableHandle()->GetVolume()->GetName();  
  
  //G4ThreeVector mom    = track->GetMomentum(); // gives poststep momentum --> WRONG!!!
  G4ThreeVector mom    = prestep->GetMomentum(); // gives prestep momentum
  
  G4double charge      = track->GetDefinition()->GetPDGCharge();
  G4int trackid        = track->GetTrackID();
  G4int parentid       = track->GetParentID();
  G4int trackpdg       = track->GetDefinition()->GetPDGEncoding();
  G4String trackname   = track->GetDefinition()->GetParticleName();
  
  //G4double tracklength = track->GetTrackLength(); // use sum of step lengths!!!
  G4double tracklength = 0.;
  
  G4double trackcosth = prestep->GetMomentumDirection().z(); // wrt nu direction (0,0,1)

  // if(aStep->IsFirstStepInVolume()){
  //   G4cout << "It's the first step" << G4endl; 
  // }
  // else if(aStep->IsLastStepInVolume()){
  //   G4cout << "It's the last step" << G4endl;     
  // }  

  ExN02TrackerHit* newHit = new ExN02TrackerHit();
  //newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()
  //->GetCopyNumber());
    
  newHit->SetTrackID    (trackid);
  newHit->SetParentID   (parentid);
  newHit->SetEdep       (edep);
  newHit->SetMom        (mom); 
  newHit->SetPreStepPos (prestepPos);
  newHit->SetPostStepPos(poststepPos);
  newHit->SetNameDet    (namedet);
  newHit->SetCharge     (charge);
  newHit->SetTrackName  (trackname);
  newHit->SetTrackPDG   (trackpdg);
  newHit->SetTrackLength(tracklength);
  newHit->SetNSteps     (NSteps_);
  newHit->SetStepLength (steplength);
  newHit->SetStepDeltaLyz (stepdeltalyz);
  newHit->SetTrackCosTheta (trackcosth);

  trackerCollection->insert( newHit );
  
  //newHit->Print();
  //newHit->Draw();
  
  //}
  
  NSteps_++;
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerSD::EndOfEvent(G4HCofThisEvent*)
{

  if (verboseLevel>0) { 
    G4int NbHits = trackerCollection->entries();
    G4cout << "\n-------->Hits Collection: in this event they are " << NbHits 
	   << " hits in the tracker chambers: " << G4endl;
    //for (G4int i=0;i<NbHits;i++) (*trackerCollection)[i]->Print();
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

