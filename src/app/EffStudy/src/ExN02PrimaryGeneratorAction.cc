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
// $Id: B4PrimaryGeneratorAction.cc 75215 2013-10-29 16:07:06Z gcosmo $
// 
/// \file B4PrimaryGeneratorAction.cc
/// \brief Implementation of the B4PrimaryGeneratorAction class

#include "ExN02PrimaryGeneratorAction.hh"
#include "ExN02PrimaryGeneratorMessenger.hh"
#include "ExN02RooTrackerKinematicsGenerator.hh"

#include "ExN02VertexInfo.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Run.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PrimaryGeneratorAction::ExN02PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0),
   RooTrackerNEUT(),
   fPrimaryGeneratorMessenger(0),
   fGeneratorType("")
{
  // Set the messenger  
  fPrimaryGeneratorMessenger = new ExN02PrimaryGeneratorMessenger(this); 
  
  //
  // default particle kinematic
  //

  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);
  
  G4String particlename = "mu-"; //"pi-" //"pi+" 
  G4double particleenergy = 600 * MeV; // GeV   
  
  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle(particlename);
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.)); 
  fParticleGun->SetParticleEnergy(particleenergy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PrimaryGeneratorAction::~ExN02PrimaryGeneratorAction()
{
  delete fPrimaryGeneratorMessenger;
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  bool doGun = false;
  if(fGeneratorType=="ParticleGun") doGun = true;

  if(doGun){
  
    // In order to avoid dependence of PrimaryGeneratorAction
    // on DetectorConstruction class we get world volume
    // from G4LogicalVolumeStore
    //
    G4double worldZHalfLength = 0;
    G4double targetYHalfLength = 0;
    
    G4LogicalVolume* worlLV
      = G4LogicalVolumeStore::GetInstance()->GetVolume("/t2k");
    G4Box* worldBox = 0;
    if ( worlLV) worldBox = dynamic_cast< G4Box*>(worlLV->GetSolid()); 
    if ( worldBox ) {
      worldZHalfLength = worldBox->GetZHalfLength();  
    }
    else  {
      G4ExceptionDescription msg;
      msg << "World volume of box not found." << G4endl;
      msg << "Perhaps you have changed geometry." << G4endl;
      msg << "The gun will be place in the center.";
      G4Exception("ExN02PrimaryGeneratorAction::GeneratePrimaries()",
		  "MyCode0002",FatalException, msg);
    } 

    /*
    G4LogicalVolume* targetLV
      = G4LogicalVolumeStore::GetInstance()->GetVolume("/t2k/OA/Magnet/Basket/Target1");
    G4Box* targetBox = 0;
    if ( targetLV) targetBox = dynamic_cast< G4Box*>(targetLV->GetSolid()); 
    if ( targetBox ) {
      targetYHalfLength = targetBox->GetYHalfLength();  
    }
    else  {
      G4ExceptionDescription msg;
      msg << "Target volume of box not found." << G4endl;
      msg << "Perhaps you have changed geometry." << G4endl;
      msg << "The gun will be place in the center.";
      G4Exception("ExN02PrimaryGeneratorAction::GeneratePrimaries()",
		  "MyCode0002",FatalException, msg);
    } 
    */
  
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
  
  else if(fGeneratorType=="Generator"){ // Read NEUT or GENIE output (same format)

    //
    // Check that the # of G4 envents is <= # of NEUT events
    // It must be done here because in order to get the tot # of events
    // the G4Run object is needed, but it's initialized only at the beginning of the first event
    // (see http://hypernews.slac.stanford.edu/HyperNews/geant4/get/runmanage/175/1.html)
    //
    // Also use current event ID from NEUT tree object, since the event is not initialized yet
    //
    
    // G4int CurrNEUTEvtID = RooTrackerNEUT.GetCurrNEUTevent();
    // if(CurrNEUTEvtID==0){      
    //   G4int TotG4events = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed(); // Number passed to BeamOn in vis.mac
    //   G4int TotNEUTevents = RooTrackerNEUT.GetTotNEUTevents();      
    //   if(TotG4events > TotNEUTevents){	
    // 	G4cerr << G4endl;
    // 	G4cerr << "The total # of G4 events exceeds the # of available NEUT events" << G4endl;
    // 	G4cerr << " - # G4 events = " << TotG4events << G4endl; 
    // 	G4cerr << " - # NEUT events = " << TotNEUTevents << G4endl;
    // 	G4cerr << G4endl;	
    // 	const char *msg = "The # of G4 events is larger than # of available NEUT events!";
    // 	const char *origin = "ExN02PrimaryGeneratorAction::GeneratePrimaries";
    // 	const char *code = "if(CurrNEUTEvtID==0){";
    // 	G4Exception(origin,code,FatalException,msg);   
    //   }  
    // }

    
    // Generate the primary vertex
    RooTrackerNEUT.GeneratePrimaryVertex(anEvent);   
  }  

  else{
    G4ExceptionDescription msg;
    msg << "Generator type has not been set!!!" << G4endl;
    G4Exception("ExN02PrimaryGeneratorAction::GeneratePrimaries()",
		"MyCode0002",FatalException, msg);        
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

