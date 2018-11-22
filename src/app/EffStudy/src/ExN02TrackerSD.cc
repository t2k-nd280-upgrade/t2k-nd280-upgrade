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

#include "G4Navigator.hh"
#include "G4NavigationHistory.hh"

#include "ND280RootPersistencyManager.hh"

#include <TH2F.h>

//#define DEBUG

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerSD::ExN02TrackerSD(G4String name)
 :G4VSensitiveDetector(name),
  trackerCollection(NULL), HCID(-1),
  fMaximumHitSagitta(1*CLHEP::mm), fMaximumHitLength(10*CLHEP::mm),
  fLastHit(0) {
  
  G4String HCname;
  collectionName.insert(HCname="trackerCollection");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerSD::~ExN02TrackerSD(){ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerSD::Initialize(G4HCofThisEvent* HCE)
{
  trackerCollection = new ExN02TrackerHitsCollection
                          (SensitiveDetectorName,collectionName[0]); 
  //static G4int HCID = -1; // NEW HitSegment
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( HCID, trackerCollection ); 
  
  NSteps_ = 0;

  SetMaximumHitSagitta(1*CLHEP::mm);
  SetMaximumHitLength(10*CLHEP::mm);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ExN02TrackerSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{

  // Take inputs
  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  ND280XMLInput = InputPersistencyManager->GetXMLInput();
  //

  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;

  
  //G4cout << "ExN02TrackerSD::ProcessHits" << G4endl;

  //return false;

  // COMMENT this line to store all the hits
  if(aStep->GetTrack()->GetParentID()>0) return false; // Look only at Primary Tracks!!!
  
  //if(aStep->GetTrack()->GetParentID()==0){
  
      
  
  
  //NSteps_++;
  
  ///// NEW ND280 HitSegment

  ExN02TrackerHit* currentHit = NULL;

  // Find the hit corresponding to the current step
  if (0<=fLastHit && fLastHit < trackerCollection->entries()) {
    ExN02TrackerHit *tmpHit = (*trackerCollection)[fLastHit];
    if (tmpHit->SameHit(aStep)) {
      currentHit = tmpHit;
    }
  }


  /////

  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

#ifdef DEBUG 

  G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

  // Calculate step length in each direction
  double postposX = postStepPoint->GetPosition().x();
  double postposY = postStepPoint->GetPosition().y();
  double postposZ = postStepPoint->GetPosition().z();

  double preposX = preStepPoint->GetPosition().x();
  double preposY = preStepPoint->GetPosition().y();
  double preposZ = preStepPoint->GetPosition().z();

  double deltaX = (postposX - preposX)/2.;
  double deltaY = (postposY - preposY)/2.;
  double deltaZ = (postposZ - preposZ)/2.;

  G4cout << "pre: " << preposX << ", "<< preposY << ", " << preposZ<< G4endl;
  G4cout << "post: " << postposX << ", " << postposY << ", " << postposZ << G4endl;
  G4cout << "delta: " << deltaX << ", " << deltaY << ", " << deltaZ << G4endl;
#endif
  
  // Volume information must be extracted from Touchable of "PreStepPoint"

  G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
  G4String touch_namedet = theTouchable->GetVolume()->GetLogicalVolume()->GetName();
  G4VPhysicalVolume * worldVolumePointer = theTouchable->GetHistory()->GetVolume(0);
 
  
  // Take the position in the local coordinate system (defined in DetectorConstruction)

  ND280RootPersistencyManager* persistencyManager
    = ND280RootPersistencyManager::GetInstance();

  if( persistencyManager->doNavigDetExist() ){

#ifdef DEBUG
    G4cout << "Persistency manager exists " << G4endl ;
#endif
    
    if( !persistencyManager->GetNavigTarg1() ){  // initialize only once   
      persistencyManager->InitNavigator(worldVolumePointer,preStepPoint->GetPosition()); 
    }
    
    if( !persistencyManager->GetHistoMovedTarg1() ){
      G4Exception("ExN02TrackerSD::ProcessHits",
		  "MyCode0002",FatalException,
		  "Navigator is initialized but no movement through history has been done!");    
    }

    ///// NEW ND280 HitSegment    
    
    // #define SEPARATE_STEPS
#ifndef SEPARATE_STEPS
    // Look through the list of available hits and see the new hit should be
    // added to an existing one.
    do {
      // We have already found a hit to add this one too, so short-circuit
      // the search.
      if (currentHit) break;
      //G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
      const G4VProcess* preProcess = preStepPoint->GetProcessDefinedStep();
      G4ProcessType preProcessType = fNotDefined;
      if (preProcess) preProcessType = preProcess->GetProcessType();
      //G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
      //const G4VProcess* postProcess = postStepPoint->GetProcessDefinedStep();
      //G4ProcessType postProcessType = fNotDefined;
      //if (postProcess) postProcessType = postProcess->GetProcessType();
      // If the prestep is of type fTransporation, then we need a new step
      // since we are crossing a geometry boundary.
      if (preProcessType == fTransportation) break;
      // Try and find the hit in list of existing hits.
      for (int hitNumber = trackerCollection->entries()-1; 
	   0 <= hitNumber;
	   --hitNumber) {
	ExN02TrackerHit *tmpHit = (*trackerCollection)[hitNumber];
	if (tmpHit->SameHit(aStep)) {
	  currentHit = tmpHit;
	  fLastHit = hitNumber;
	  //ND280NamedVerbose("hit","Add step to " << hitNumber);
	  break;
	}
      }
    } while (false);
#endif
    
    // If a hit wasn't found, create one.
    if (!currentHit) {
      currentHit = new ExN02TrackerHit(fMaximumHitSagitta,fMaximumHitLength);
      fLastHit = trackerCollection->entries();
      trackerCollection->insert(currentHit);
    }
  
    currentHit->AddStep(aStep);

  } 
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

