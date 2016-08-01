

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
// $Id: ExN02EventAction.cc,v 1.11 2006-06-29 17:48:05 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#include "ExN02EventAction.hh"
#include "ExN02Constants.hh"
#include "ExN02RunAction.hh"
#include "ExN02Analysis.hh"
#include "ExN02TrackerHit.hh"
#include "ExN02VertexInfo.hh"
#include "ND280Trajectory.hh"
#include "ND280RootPersistencyManager.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4UnitsTable.hh"

#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"

#include "Randomize.hh"
#include <iomanip>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TObject.h>

#include <TND280UpEvent.hh>
#include <TND280UpTrack.hh>
#include <TND280UpTrackPoint.hh>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02EventAction::ExN02EventAction()
  : G4UserEventAction(),
    fHHC1ID(-1),  
    fEnergyAbsTPCup1(0.),
    fEnergyAbsTPCdown1(0.),
    fEnergyAbsTarget1(0.),
    fEnergyAbsTPCup2(0.),
    fEnergyAbsTPCdown2(0.),
    fEnergyAbsTarget2(0.),
    fEnergyAbsForwTPC1(0.),
    fEnergyAbsForwTPC2(0.),
    fEnergyAbsForwTPC3(0.),
    fEnergyAbsTot(0.),
    fTrackLAbsTPCup1(0.),
    fTrackLAbsTPCdown1(0.),
    fTrackLAbsTarget1(0.),
    fTrackLAbsTPCup2(0.),
    fTrackLAbsTPCdown2(0.),
    fTrackLAbsTarget2(0.),
    fTrackLAbsForwTPC1(0.),
    fTrackLAbsForwTPC2(0.),
    fTrackLAbsForwTPC3(0.),
    fTrackLAbsTot(0.),
    fNTracks(0),
    
    fVecTrackID(),
    fVecParentID(),
    fVecTrackPDG(),
    fVecTrackE(),
    fVecTrackMomX(),
    fVecTrackMomY(),
    fVecTrackMomZ(),
    fVecTrackMomMag(),
    
    fTPCUp1_NTracks(0),
    fVecTPCUp1_TrackID(),
    fVecTPCUp1_ParentID(),
    fVecTPCUp1_TrackMomX(),
    fVecTPCUp1_TrackMomY(),
    fVecTPCUp1_TrackMomZ(),
    fVecTPCUp1_TrackLength(),
    fVecTPCUp1_TrackDeltaLyz(),
    fVecTPCUp1_TrackEdep(),
    fVecTPCUp1_TrackCharge(),
    fVecTPCUp1_TrackPDG(),
    fVecTPCUp1_TrackStepFirstX(),
    fVecTPCUp1_TrackStepFirstY(),
    fVecTPCUp1_TrackStepFirstZ(),
    fVecTPCUp1_TrackStepLastX(),
    fVecTPCUp1_TrackStepLastY(),
    fVecTPCUp1_TrackStepLastZ(),
    fVecTPCUp1_TrackCosTheta(),

    fTPCDown1_NTracks(0),
    fVecTPCDown1_TrackID(),
    fVecTPCDown1_ParentID(),
    fVecTPCDown1_TrackMomX(),
    fVecTPCDown1_TrackMomY(),
    fVecTPCDown1_TrackMomZ(),
    fVecTPCDown1_TrackLength(),
    fVecTPCDown1_TrackDeltaLyz(),
    fVecTPCDown1_TrackEdep(),
    fVecTPCDown1_TrackCharge(),
    fVecTPCDown1_TrackPDG(),
    fVecTPCDown1_TrackStepFirstX(),
    fVecTPCDown1_TrackStepFirstY(),
    fVecTPCDown1_TrackStepFirstZ(),
    fVecTPCDown1_TrackStepLastX(),
    fVecTPCDown1_TrackStepLastY(),
    fVecTPCDown1_TrackStepLastZ(),
    fVecTPCDown1_TrackCosTheta(),

    fTPCUp2_NTracks(0),
    fVecTPCUp2_TrackID(),
    fVecTPCUp2_ParentID(),
    fVecTPCUp2_TrackMomX(),
    fVecTPCUp2_TrackMomY(),
    fVecTPCUp2_TrackMomZ(),
    fVecTPCUp2_TrackLength(),
    fVecTPCUp2_TrackDeltaLyz(),
    fVecTPCUp2_TrackEdep(),
    fVecTPCUp2_TrackCharge(),
    fVecTPCUp2_TrackPDG(),
    fVecTPCUp2_TrackStepFirstX(),
    fVecTPCUp2_TrackStepFirstY(),
    fVecTPCUp2_TrackStepFirstZ(),
    fVecTPCUp2_TrackStepLastX(),
    fVecTPCUp2_TrackStepLastY(),
    fVecTPCUp2_TrackStepLastZ(),
    fVecTPCUp2_TrackCosTheta(),

    fTPCDown2_NTracks(0),
    fVecTPCDown2_TrackID(),
    fVecTPCDown2_ParentID(),
    fVecTPCDown2_TrackMomX(),
    fVecTPCDown2_TrackMomY(),
    fVecTPCDown2_TrackMomZ(),
    fVecTPCDown2_TrackLength(),
    fVecTPCDown2_TrackDeltaLyz(),
    fVecTPCDown2_TrackEdep(),
    fVecTPCDown2_TrackCharge(),
    fVecTPCDown2_TrackPDG(),
    fVecTPCDown2_TrackStepFirstX(),
    fVecTPCDown2_TrackStepFirstY(),
    fVecTPCDown2_TrackStepFirstZ(),
    fVecTPCDown2_TrackStepLastX(),
    fVecTPCDown2_TrackStepLastY(),
    fVecTPCDown2_TrackStepLastZ(),
    fVecTPCDown2_TrackCosTheta(),

    fForwTPC1_NTracks(0),
    fVecForwTPC1_TrackID(),
    fVecForwTPC1_ParentID(),
    fVecForwTPC1_TrackMomX(),
    fVecForwTPC1_TrackMomY(),
    fVecForwTPC1_TrackMomZ(),
    fVecForwTPC1_TrackLength(),
    fVecForwTPC1_TrackDeltaLyz(),
    fVecForwTPC1_TrackEdep(),
    fVecForwTPC1_TrackCharge(),
    fVecForwTPC1_TrackPDG(),
    fVecForwTPC1_TrackStepFirstX(),
    fVecForwTPC1_TrackStepFirstY(),
    fVecForwTPC1_TrackStepFirstZ(),
    fVecForwTPC1_TrackStepLastX(),
    fVecForwTPC1_TrackStepLastY(),
    fVecForwTPC1_TrackStepLastZ(),
    fVecForwTPC1_TrackCosTheta(),

    fForwTPC2_NTracks(0),
    fVecForwTPC2_TrackID(),
    fVecForwTPC2_ParentID(),
    fVecForwTPC2_TrackMomX(),
    fVecForwTPC2_TrackMomY(),
    fVecForwTPC2_TrackMomZ(),
    fVecForwTPC2_TrackLength(),
    fVecForwTPC2_TrackDeltaLyz(),
    fVecForwTPC2_TrackEdep(),
    fVecForwTPC2_TrackCharge(),
    fVecForwTPC2_TrackPDG(),
    fVecForwTPC2_TrackStepFirstX(),
    fVecForwTPC2_TrackStepFirstY(),
    fVecForwTPC2_TrackStepFirstZ(),
    fVecForwTPC2_TrackStepLastX(),
    fVecForwTPC2_TrackStepLastY(),
    fVecForwTPC2_TrackStepLastZ(),
    fVecForwTPC2_TrackCosTheta(),

    fForwTPC3_NTracks(0),
    fVecForwTPC3_TrackID(),
    fVecForwTPC3_ParentID(),
    fVecForwTPC3_TrackMomX(),
    fVecForwTPC3_TrackMomY(),
    fVecForwTPC3_TrackMomZ(),
    fVecForwTPC3_TrackLength(),
    fVecForwTPC3_TrackDeltaLyz(),
    fVecForwTPC3_TrackEdep(),
    fVecForwTPC3_TrackCharge(),
    fVecForwTPC3_TrackPDG(),
    fVecForwTPC3_TrackStepFirstX(),
    fVecForwTPC3_TrackStepFirstY(),
    fVecForwTPC3_TrackStepFirstZ(),
    fVecForwTPC3_TrackStepLastX(),
    fVecForwTPC3_TrackStepLastY(),
    fVecForwTPC3_TrackStepLastZ(),
    fVecForwTPC3_TrackCosTheta(),

    fTarget1_NTracks(0),
    fVecTarget1_TrackID(),
    fVecTarget1_ParentID(),
    fVecTarget1_TrackMomX(),
    fVecTarget1_TrackMomY(),
    fVecTarget1_TrackMomZ(),
    fVecTarget1_TrackLength(),
    fVecTarget1_TrackDeltaLyz(),
    fVecTarget1_TrackEdep(),
    fVecTarget1_TrackCharge(),
    fVecTarget1_TrackPDG(),
    fVecTarget1_TrackStepFirstX(),
    fVecTarget1_TrackStepFirstY(),
    fVecTarget1_TrackStepFirstZ(),
    fVecTarget1_TrackStepLastX(),
    fVecTarget1_TrackStepLastY(),
    fVecTarget1_TrackStepLastZ(),
    fVecTarget1_TrackCosTheta(),

    fTarget2_NTracks(0),
    fVecTarget2_TrackID(),
    fVecTarget2_ParentID(),
    fVecTarget2_TrackMomX(),
    fVecTarget2_TrackMomY(),
    fVecTarget2_TrackMomZ(),
    fVecTarget2_TrackLength(),
    fVecTarget2_TrackDeltaLyz(),
    fVecTarget2_TrackEdep(),
    fVecTarget2_TrackCharge(),
    fVecTarget2_TrackPDG(),
    fVecTarget2_TrackStepFirstX(),
    fVecTarget2_TrackStepFirstY(),
    fVecTarget2_TrackStepFirstZ(),
    fVecTarget2_TrackStepLastX(),
    fVecTarget2_TrackStepLastY(),
    fVecTarget2_TrackStepLastZ(),
    fVecTarget2_TrackCosTheta(),

    fNVtx(0),
    fVecVtx_X(), 
    fVecVtx_Y(), 
    fVecVtx_Z(), 
    fVecVtx_NuPDG(),     
    fVecVtx_ReacMode(),  
    fVecVtx_EvtProb(),   
    fVecVtx_EvtWeight() 
    
{
  // // set printing per each event
  // G4RunManager::GetRunManager()->SetPrintProgress(1);

  // initialize the vectors (don't use it)
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02EventAction::~ExN02EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void ExN02EventAction::BeginOfEventAction(const G4Event*)
{
  // variable initialisation per event
  fEnergyAbsForwTPC1 = 0.;
  fEnergyAbsForwTPC2 = 0.;
  fEnergyAbsForwTPC3 = 0.;
  fTrackLAbsForwTPC1 = 0.;
  fTrackLAbsForwTPC2 = 0.;
  fTrackLAbsForwTPC3 = 0.;
  fEnergyAbsTPCup1   = 0.;
  fEnergyAbsTPCdown1 = 0.;
  fEnergyAbsTarget1  = 0.;
  fTrackLAbsTPCup1   = 0.;
  fTrackLAbsTPCdown1 = 0.;
  fTrackLAbsTarget1  = 0.;
  fEnergyAbsTPCup2   = 0.;
  fEnergyAbsTPCdown2 = 0.;
  fEnergyAbsTarget2  = 0.;
  fTrackLAbsTPCup2   = 0.;
  fTrackLAbsTPCdown2 = 0.;
  fTrackLAbsTarget2  = 0.;
  fNTracks          = 0; // It's a counter. It must be 0 at beginning of event.

  fForwTPC1_NTracks = 0;
  fForwTPC2_NTracks = 0;
  fForwTPC3_NTracks = 0;
  fTPCUp1_NTracks = 0;
  fTPCDown1_NTracks = 0;
  fTPCUp2_NTracks = 0;
  fTPCDown2_NTracks = 0;
  fTarget1_NTracks = 0;
  fTarget2_NTracks = 0;

  fNVtx = 0; // It's a counter. It must be 0 at  beginning of event

  // vector initialisation per event
  fVecTrackID.clear(); // clear the vector
  fVecParentID.clear(); // clear the vector
  fVecTrackPDG.clear(); // clear the vector
  fVecTrackE.clear(); // clear the vector
  fVecTrackMomX.clear(); // clear the vector
  fVecTrackMomY.clear(); // clear the vector
  fVecTrackMomZ.clear(); // clear the vector
  fVecTrackMomMag.clear(); // clear the vector

  fVecForwTPC1_TrackID.clear(); 
  fVecForwTPC1_ParentID.clear(); 
  fVecForwTPC1_TrackMomX.clear(); 
  fVecForwTPC1_TrackMomY.clear(); 
  fVecForwTPC1_TrackMomZ.clear(); 
  fVecForwTPC1_TrackLength.clear(); 
  fVecForwTPC1_TrackDeltaLyz.clear(); 
  fVecForwTPC1_TrackEdep.clear(); 
  fVecForwTPC1_TrackCharge.clear(); 
  fVecForwTPC1_TrackPDG.clear();
  fVecForwTPC1_TrackStepFirstX.clear();
  fVecForwTPC1_TrackStepFirstY.clear();
  fVecForwTPC1_TrackStepFirstZ.clear();
  fVecForwTPC1_TrackStepLastX.clear();
  fVecForwTPC1_TrackStepLastY.clear();
  fVecForwTPC1_TrackStepLastZ.clear();
  fVecForwTPC1_TrackCosTheta.clear();

  fVecForwTPC2_TrackID.clear(); 
  fVecForwTPC2_ParentID.clear(); 
  fVecForwTPC2_TrackMomX.clear(); 
  fVecForwTPC2_TrackMomY.clear(); 
  fVecForwTPC2_TrackMomZ.clear(); 
  fVecForwTPC2_TrackLength.clear(); 
  fVecForwTPC2_TrackDeltaLyz.clear(); 
  fVecForwTPC2_TrackEdep.clear(); 
  fVecForwTPC2_TrackCharge.clear(); 
  fVecForwTPC2_TrackPDG.clear();
  fVecForwTPC2_TrackStepFirstX.clear();
  fVecForwTPC2_TrackStepFirstY.clear();
  fVecForwTPC2_TrackStepFirstZ.clear();
  fVecForwTPC2_TrackStepLastX.clear();
  fVecForwTPC2_TrackStepLastY.clear();
  fVecForwTPC2_TrackStepLastZ.clear();
  fVecForwTPC2_TrackCosTheta.clear();

  fVecForwTPC3_TrackID.clear(); 
  fVecForwTPC3_ParentID.clear(); 
  fVecForwTPC3_TrackMomX.clear(); 
  fVecForwTPC3_TrackMomY.clear(); 
  fVecForwTPC3_TrackMomZ.clear(); 
  fVecForwTPC3_TrackLength.clear(); 
  fVecForwTPC3_TrackDeltaLyz.clear(); 
  fVecForwTPC3_TrackEdep.clear(); 
  fVecForwTPC3_TrackCharge.clear(); 
  fVecForwTPC3_TrackPDG.clear();
  fVecForwTPC3_TrackStepFirstX.clear();
  fVecForwTPC3_TrackStepFirstY.clear();
  fVecForwTPC3_TrackStepFirstZ.clear();
  fVecForwTPC3_TrackStepLastX.clear();
  fVecForwTPC3_TrackStepLastY.clear();
  fVecForwTPC3_TrackStepLastZ.clear();
  fVecForwTPC3_TrackCosTheta.clear();

  fVecTPCUp1_TrackID.clear(); 
  fVecTPCUp1_ParentID.clear(); 
  fVecTPCUp1_TrackMomX.clear(); 
  fVecTPCUp1_TrackMomY.clear(); 
  fVecTPCUp1_TrackMomZ.clear(); 
  fVecTPCUp1_TrackLength.clear(); 
  fVecTPCUp1_TrackDeltaLyz.clear(); 
  fVecTPCUp1_TrackEdep.clear(); 
  fVecTPCUp1_TrackCharge.clear(); 
  fVecTPCUp1_TrackPDG.clear();
  fVecTPCUp1_TrackStepFirstX.clear();
  fVecTPCUp1_TrackStepFirstY.clear();
  fVecTPCUp1_TrackStepFirstZ.clear();
  fVecTPCUp1_TrackStepLastX.clear();
  fVecTPCUp1_TrackStepLastY.clear();
  fVecTPCUp1_TrackStepLastZ.clear();
  fVecTPCUp1_TrackCosTheta.clear();

  fVecTPCUp2_TrackID.clear(); 
  fVecTPCUp2_ParentID.clear(); 
  fVecTPCUp2_TrackMomX.clear(); 
  fVecTPCUp2_TrackMomY.clear(); 
  fVecTPCUp2_TrackMomZ.clear(); 
  fVecTPCUp2_TrackLength.clear(); 
  fVecTPCUp2_TrackDeltaLyz.clear(); 
  fVecTPCUp2_TrackEdep.clear(); 
  fVecTPCUp2_TrackCharge.clear(); 
  fVecTPCUp2_TrackPDG.clear();
  fVecTPCUp2_TrackStepFirstX.clear();
  fVecTPCUp2_TrackStepFirstY.clear();
  fVecTPCUp2_TrackStepFirstZ.clear();
  fVecTPCUp2_TrackStepLastX.clear();
  fVecTPCUp2_TrackStepLastY.clear();
  fVecTPCUp2_TrackStepLastZ.clear();
  fVecTPCUp2_TrackCosTheta.clear();

  fVecTPCDown1_TrackID.clear(); 
  fVecTPCDown1_ParentID.clear(); 
  fVecTPCDown1_TrackMomX.clear(); 
  fVecTPCDown1_TrackMomY.clear(); 
  fVecTPCDown1_TrackMomZ.clear(); 
  fVecTPCDown1_TrackLength.clear(); 
  fVecTPCDown1_TrackDeltaLyz.clear(); 
  fVecTPCDown1_TrackEdep.clear(); 
  fVecTPCDown1_TrackCharge.clear(); 
  fVecTPCDown1_TrackPDG.clear();
  fVecTPCDown1_TrackStepFirstX.clear();
  fVecTPCDown1_TrackStepFirstY.clear();
  fVecTPCDown1_TrackStepFirstZ.clear();
  fVecTPCDown1_TrackStepLastX.clear();
  fVecTPCDown1_TrackStepLastY.clear();
  fVecTPCDown1_TrackStepLastZ.clear();
  fVecTPCDown1_TrackCosTheta.clear();

  fVecTPCDown2_TrackID.clear(); 
  fVecTPCDown2_ParentID.clear(); 
  fVecTPCDown2_TrackMomX.clear(); 
  fVecTPCDown2_TrackMomY.clear(); 
  fVecTPCDown2_TrackMomZ.clear(); 
  fVecTPCDown2_TrackLength.clear(); 
  fVecTPCDown2_TrackDeltaLyz.clear(); 
  fVecTPCDown2_TrackEdep.clear(); 
  fVecTPCDown2_TrackCharge.clear(); 
  fVecTPCDown2_TrackPDG.clear();
  fVecTPCDown2_TrackStepFirstX.clear();
  fVecTPCDown2_TrackStepFirstY.clear();
  fVecTPCDown2_TrackStepFirstZ.clear();
  fVecTPCDown2_TrackStepLastX.clear();
  fVecTPCDown2_TrackStepLastY.clear();
  fVecTPCDown2_TrackStepLastZ.clear();
  fVecTPCDown2_TrackCosTheta.clear();

  fVecTarget1_TrackID.clear(); 
  fVecTarget1_ParentID.clear(); 
  fVecTarget1_TrackMomX.clear(); 
  fVecTarget1_TrackMomY.clear(); 
  fVecTarget1_TrackMomZ.clear();
  fVecTarget1_TrackLength.clear(); 
  fVecTarget1_TrackDeltaLyz.clear(); 
  fVecTarget1_TrackEdep.clear(); 
  fVecTarget1_TrackCharge.clear(); 
  fVecTarget1_TrackPDG.clear();
  fVecTarget1_TrackStepFirstX.clear();
  fVecTarget1_TrackStepFirstY.clear();
  fVecTarget1_TrackStepFirstZ.clear();
  fVecTarget1_TrackStepLastX.clear();
  fVecTarget1_TrackStepLastY.clear();
  fVecTarget1_TrackStepLastZ.clear();
  fVecTarget1_TrackCosTheta.clear();

  fVecTarget2_TrackID.clear(); 
  fVecTarget2_ParentID.clear(); 
  fVecTarget2_TrackMomX.clear(); 
  fVecTarget2_TrackMomY.clear(); 
  fVecTarget2_TrackMomZ.clear();
  fVecTarget2_TrackLength.clear(); 
  fVecTarget2_TrackDeltaLyz.clear(); 
  fVecTarget2_TrackEdep.clear(); 
  fVecTarget2_TrackCharge.clear(); 
  fVecTarget2_TrackPDG.clear();
  fVecTarget2_TrackStepFirstX.clear();
  fVecTarget2_TrackStepFirstY.clear();
  fVecTarget2_TrackStepFirstZ.clear();
  fVecTarget2_TrackStepLastX.clear();
  fVecTarget2_TrackStepLastY.clear();
  fVecTarget2_TrackStepLastZ.clear();
  fVecTarget2_TrackCosTheta.clear();

  fVecVtx_X.clear(); 
  fVecVtx_Y.clear(); 
  fVecVtx_Z.clear(); 
  fVecVtx_NuPDG.clear();     
  fVecVtx_ReacMode.clear();  
  fVecVtx_EvtProb.clear();   
  fVecVtx_EvtWeight.clear(); 
  
  // Initialize the hit collections
  if (fHHC1ID==-1) {
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    fHHC1ID = sdManager->GetCollectionID("trackerCollection");
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void ExN02EventAction::EndOfEventAction(const G4Event* event)
{

  // Accumulate statistics
  //  

  ///////////////////////////////////////////////////////
  //                                                   //
  //   Access the primary and informational vertices   //
  //                                                   //
  ///////////////////////////////////////////////////////

  
  // Get the Primary Vertex
  
  G4int vtxNumber=0;
  for (G4PrimaryVertex* vtx = event->GetPrimaryVertex();vtx;vtx = vtx->GetNext()) {
    ++vtxNumber;
    
    // G4int vtxNumber=0;
    // for (G4int ivtx=0; ivtx<event->GetNumberOfPrimaryVertex(); ivtx++){  
    // ++vtxNumber;
    // G4PrimaryVertex* vtx = event->GetPrimaryVertex();
  
    G4cout << "Vertex: " << vtxNumber  
  	   << " w/ " << vtx->GetNumberOfParticle() << " primaries" 
  	   << " at " 
      // << " (" << G4BestUnit(vtx->GetX0(),"Length") 
      // << ", " << G4BestUnit(vtx->GetY0(),"Length") 
      // << ", " << G4BestUnit(vtx->GetZ0(),"Length") 
      // << ", " << G4BestUnit(vtx->GetT0(),"Time") << ")"
  	   << " (" << vtx->GetX0() / mm
  	   << ", " << vtx->GetY0() / mm
  	   << ", " << vtx->GetZ0() / mm 
  	   << ", " << vtx->GetT0() / second << ")"
  	   << G4endl;

    ExN02VertexInfo* vInfo 
      = dynamic_cast<ExN02VertexInfo*>(vtx->GetUserInformation());
    if (vInfo) {
      G4cout << "  Generator: " << vInfo->GetName() << G4endl;
      G4cout << "  Reaction:  " << vInfo->GetReaction() << G4endl;
      G4cout << "  Weight:    " << vInfo->GetWeight() << G4endl;
      G4cout << "  Prob:      " << vInfo->GetProbability() << G4endl;
    }
    for (G4int p=0; p<vtx->GetNumberOfParticle(); ++p) {
      G4PrimaryParticle* prim = vtx->GetPrimary(p);
      G4ParticleDefinition* partDef = prim->GetG4code();
      G4ThreeVector dir = prim->GetMomentum().unit();
      G4cout << "  " << partDef->GetParticleName() << " "
    	     << prim->GetPDGcode()
    	     << " w/ "
    	     << G4BestUnit(prim->GetMomentum().mag(),"Energy")
    	     << "  Direction: (" << dir.x()
    	     << ", " << dir.y()
    	     << ", " << dir.z() << ")"
    	     << G4endl;
    }
   
    // Get the Incoming Vertex
    G4cout << "Incoming Vertex:" << vtxNumber << G4endl;
    
    const G4PrimaryVertex *incvtx = vInfo->GetInformationalVertex();

    G4cout << " w/ " << incvtx->GetNumberOfParticle() << " primaries" 
  	   << " at " 
  	   // << " (" << G4BestUnit(vtx->GetX0(),"Length") 
  	   // << ", " << G4BestUnit(vtx->GetY0(),"Length") 
  	   // << ", " << G4BestUnit(vtx->GetZ0(),"Length") 
  	   // << ", " << G4BestUnit(vtx->GetT0(),"Time") << ")"
  	   << " (" << incvtx->GetX0() / mm
  	   << ", " << incvtx->GetY0() / mm
  	   << ", " << incvtx->GetZ0() / mm 
  	   << ", " << incvtx->GetT0() / second << ")"
  	   << G4endl;

    for (G4int nu=0; nu<incvtx->GetNumberOfParticle(); ++nu) {
      G4PrimaryParticle* prim = incvtx->GetPrimary(nu);
      G4ParticleDefinition* partDef = prim->GetG4code();
      G4ThreeVector dir = prim->GetMomentum().unit();
      G4cout << "  " << partDef->GetParticleName() << " "
    	     << prim->GetPDGcode()
    	     << " w/ "
    	     << G4BestUnit(prim->GetMomentum().mag(),"Energy")
    	     << "  Direction: (" << dir.x()
    	     << ", " << dir.y()
    	     << ", " << dir.z() << ")"
    	     << G4endl;	
      G4int pdg = prim->GetPDGcode();
    
      //G4cout << pdg << " " << fabs(pdg) << G4endl;

      if( fabs(pdg)==12 ||
    	  fabs(pdg)==14 ||
    	  fabs(pdg)==16  ){	
    	fVecVtx_NuPDG.push_back(prim->GetPDGcode());
      }
    }
    // Get the Decay Vertex
    // Get the Production Vertex

    // Fill the Ntuple
    
    fVecVtx_X.push_back(vtx->GetX0() / mm);
    fVecVtx_Y.push_back(vtx->GetY0() / mm);
    fVecVtx_Z.push_back(vtx->GetZ0() / mm); 
    G4int mode = G4UIcommand::ConvertToInt(vInfo->GetReaction());
    fVecVtx_ReacMode .push_back(mode);
    fVecVtx_EvtProb  .push_back(vInfo->GetWeight());
    fVecVtx_EvtWeight.push_back(vInfo->GetProbability()); 
    
    G4cout << "mode = " << mode << G4endl;
  }

  fNVtx = vtxNumber; // get # of vertices in the event
     
  


  ///////////////////////////////////////////////////////
  //                                                   //
  // Access hit collections and handle the stored hits //
  //                                                   //
  ///////////////////////////////////////////////////////
    
  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if (!hce) 
    {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found." << G4endl; 
      G4Exception("ExN02EventAction::EndOfEventAction()",
		  "ExN02Code001", JustWarning, msg);
      return;
    }   
  
  // Get hits collections 
  ExN02TrackerHitsCollection* hHC1 
    = static_cast<ExN02TrackerHitsCollection*>(hce->GetHC(fHHC1ID));
         
  if ( (!hHC1) ) {
    G4ExceptionDescription msg;
    msg << "Some of hits collections of this event not found." << G4endl; 
    G4Exception("ExN02EventAction::EndOfEventAction()",
		"ExN02Code001", JustWarning, msg);
    return;
  }   
  
   G4int n_hit = hHC1->entries();
   std::cout << "# of hits = " << n_hit << std::endl;

   G4int currtrkid_ForwTPC1 = kBadNum;
   G4int prevtrkid_ForwTPC1 = kBadNum;
   G4int aftertrkid_ForwTPC1 = kBadNum;
   G4int currparentid_ForwTPC1 = kBadNum;

   G4int currtrkid_ForwTPC2 = kBadNum;
   G4int prevtrkid_ForwTPC2 = kBadNum;
   G4int aftertrkid_ForwTPC2 = kBadNum;
   G4int currparentid_ForwTPC2 = kBadNum;

   G4int currtrkid_ForwTPC3 = kBadNum;
   G4int prevtrkid_ForwTPC3 = kBadNum;
   G4int aftertrkid_ForwTPC3 = kBadNum;
   G4int currparentid_ForwTPC3 = kBadNum;

   G4int currtrkid_TPCUp1 = kBadNum;
   G4int prevtrkid_TPCUp1 = kBadNum;
   G4int aftertrkid_TPCUp1 = kBadNum;
   G4int currparentid_TPCUp1 = kBadNum;

   G4int currtrkid_TPCDown1 = kBadNum;
   G4int prevtrkid_TPCDown1 = kBadNum;
   G4int aftertrkid_TPCDown1 = kBadNum;
   G4int currparentid_TPCDown1 = kBadNum;

   G4int currtrkid_Target1 = kBadNum;
   G4int prevtrkid_Target1 = kBadNum;
   G4int aftertrkid_Target1 = kBadNum;
   G4int currparentid_Target1 = kBadNum;

   G4int currtrkid_TPCUp2 = kBadNum;
   G4int prevtrkid_TPCUp2 = kBadNum;
   G4int aftertrkid_TPCUp2 = kBadNum;
   G4int currparentid_TPCUp2 = kBadNum;

   G4int currtrkid_TPCDown2 = kBadNum;
   G4int prevtrkid_TPCDown2 = kBadNum;
   G4int aftertrkid_TPCDown2 = kBadNum;
   G4int currparentid_TPCDown2 = kBadNum;

   G4int currtrkid_Target2 = kBadNum;
   G4int prevtrkid_Target2 = kBadNum;
   G4int aftertrkid_Target2 = kBadNum;
   G4int currparentid_Target2 = kBadNum;

   for (G4int i=0;i<n_hit;i++){
     ExN02TrackerHit* hit = (*hHC1)[i];

     G4String detname     = hit->GetNameDet();
     G4ThreeVector trkmom = hit->GetMom();
     G4double trkcharge   = hit->GetCharge();
     G4int trkpdg         = hit->GetTrackPDG();
     G4double costh       = hit->GetTrackCosTheta();
          
     ExN02TrackerHit* hit_after;
     if(i < (n_hit-1)) hit_after = (*hHC1)[i+1];
     else              hit_after = 0;

     G4String detname_after = "";
     if(hit_after!=0) detname_after = hit_after->GetNameDet();
     //std::cout << detname_after << std::endl;

     ExN02TrackerHit* hit_before;
     if(i > 0) hit_before = (*hHC1)[i-1];
     else      hit_before = 0;

     G4String detname_before = "";
     if(hit_before!=0) detname_before = hit_before->GetNameDet();
     //std::cout << detname_after << std::endl;
     
     // if(detname=="Target1"){
     // G4cout << "i = " << i 
     // 	    << " - current ID = " << hit->GetTrackID() 
     // 	    << " - after ID = " << hit_after->GetTrackID() 
     // 	    << " --> detname = " << detname << " -->  detname_after = " << detname_after << G4endl;
     //}
     //continue;
     

     if(detname=="TPCUp1"){     
	 
       currtrkid_TPCUp1 = hit->GetTrackID(); // current track
       currparentid_TPCUp1 = hit->GetParentID(); // current track
       
       //G4cout << "currtrkid_TPCUp1 = " << currtrkid_TPCUp1 << G4endl;
 
       if(hit_after!=0) aftertrkid_TPCUp1 = hit_after->GetTrackID(); // following track 
       else aftertrkid_TPCUp1 = kBadNum;
              
       // Get Track initial informations (FIRST STEP of the track in detector)
       
       // It's the first track of event or change track or change detector
       if( (prevtrkid_TPCUp1==kBadNum) || (currtrkid_TPCUp1!=prevtrkid_TPCUp1 && prevtrkid_TPCUp1!=kBadNum) || (detname_before!=detname) ){
	 
	 fVecTPCUp1_TrackID.push_back(currtrkid_TPCUp1);
	 fVecTPCUp1_ParentID.push_back(currparentid_TPCUp1);
	 
	 fVecTPCUp1_TrackMomX.push_back((G4double)trkmom.x() / MeV);
	 fVecTPCUp1_TrackMomY.push_back((G4double)trkmom.y() / MeV);
	 fVecTPCUp1_TrackMomZ.push_back((G4double)trkmom.z() / MeV);
	 fVecTPCUp1_TrackCharge.push_back(trkcharge / eplus);
	 fVecTPCUp1_TrackPDG.push_back(trkpdg);

	 // get first step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTPCUp1_TrackStepFirstX.push_back(x);	 	 
	 fVecTPCUp1_TrackStepFirstY.push_back(y);	 	 
	 fVecTPCUp1_TrackStepFirstZ.push_back(z);	 	 

	 // get track cos(theta) from first pre-step wrt nu direction (0,0,1)
	 fVecTPCUp1_TrackCosTheta.push_back(costh);

	 prevtrkid_TPCUp1 = currtrkid_TPCUp1;
	 fTPCUp1_NTracks++;	
	 
	 //G4cout << "First:" << prevtrkid_TPCUp1 << " --> " << currtrkid_TPCUp1 
	 //<< " - " << detname_before << " --> " << detname << G4endl;
	 
       } // if first step of the track

       
       // Get Track initial informations (LAST STEP of the track in detector)
       
       if( currtrkid_TPCUp1!=aftertrkid_TPCUp1 || detname!=detname_after ){ 

	 // If aftertrkid_TPCUp1=kBadNum, still ok as last step
	 // If first step = last step --> store it both as first and last step
	 
	 // get last step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTPCUp1_TrackStepLastX.push_back(x);	 	 
	 fVecTPCUp1_TrackStepLastY.push_back(y);	 	 
	 fVecTPCUp1_TrackStepLastZ.push_back(z);	 	 
	 
	 //std::cout << "Last:" << currtrkid_TPCUp1 << " --> " << aftertrkid_TPCUp1 
	 //<< " - " << detname << " --> " << detname_after << std::endl;     

       } // if last step of the track
       
     } // end TPCUp1

     else if(detname=="TPCUp2"){     
	 
       currtrkid_TPCUp2 = hit->GetTrackID(); // current track
       currparentid_TPCUp2 = hit->GetParentID(); // current track
       
       //G4cout << "currtrkid_TPCUp2 = " << currtrkid_TPCUp2 << G4endl;
 
       if(hit_after!=0) aftertrkid_TPCUp2 = hit_after->GetTrackID(); // following track 
       else aftertrkid_TPCUp2 = kBadNum;
       
       // Get Track initial informations (FIRST STEP of the track in detector)

       // It's the first track of event or change track or change detector
       if( (prevtrkid_TPCUp2==kBadNum) || (currtrkid_TPCUp2!=prevtrkid_TPCUp2 && prevtrkid_TPCUp2!=kBadNum) || (detname_before!=detname) ){
	 
	 fVecTPCUp2_TrackID.push_back(currtrkid_TPCUp2);
	 fVecTPCUp2_ParentID.push_back(currparentid_TPCUp2);
	 
	 fVecTPCUp2_TrackMomX.push_back((G4double)trkmom.x() / MeV);
	 fVecTPCUp2_TrackMomY.push_back((G4double)trkmom.y() / MeV);
	 fVecTPCUp2_TrackMomZ.push_back((G4double)trkmom.z() / MeV);
	 fVecTPCUp2_TrackCharge.push_back(trkcharge / eplus);
	 fVecTPCUp2_TrackPDG.push_back(trkpdg);

	 // get first step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTPCUp2_TrackStepFirstX.push_back(x);	 	 
	 fVecTPCUp2_TrackStepFirstY.push_back(y);	 	 
	 fVecTPCUp2_TrackStepFirstZ.push_back(z);	 	 

	 // get track cos(theta) from first pre-step wrt nu direction (0,0,1)
	 fVecTPCUp2_TrackCosTheta.push_back(costh);

	 prevtrkid_TPCUp2 = currtrkid_TPCUp2;
	 fTPCUp2_NTracks++;	
	 
	 //G4cout << "First:" << prevtrkid_TPCUp2 << " --> " << currtrkid_TPCUp2 
	 //<< " - " << detname_before << " --> " << detname << G4endl;
	 
       } // if first step of the track

       
       // Get Track initial informations (LAST STEP of the track in detector)
       
       if( currtrkid_TPCUp2!=aftertrkid_TPCUp2 || detname!=detname_after ){ 

	 // If aftertrkid_TPCUp2=kBadNum, still ok as last step
	 // If first step = last step --> store it both as first and last step
	 
	 // get last step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTPCUp2_TrackStepLastX.push_back(x);	 	 
	 fVecTPCUp2_TrackStepLastY.push_back(y);	 	 
	 fVecTPCUp2_TrackStepLastZ.push_back(z);	 	 
	 
	 //std::cout << "Last:" << currtrkid_TPCUp2 << " --> " << aftertrkid_TPCUp2 
	 //<< " - " << detname << " --> " << detname_after << std::endl;     

       } // if last step of the track
       
     } // end TPCUp2


     else if(detname=="TPCDown1"){     
	 
       currtrkid_TPCDown1 = hit->GetTrackID();
       currparentid_TPCDown1 = hit->GetParentID();
       
       if(hit_after!=0) aftertrkid_TPCDown1 = hit_after->GetTrackID(); // following track 
       else aftertrkid_TPCDown1 = kBadNum;

       // Get Track initial informations (FIRST STEP of the track in detector)

       if( (prevtrkid_TPCDown1==kBadNum) || (currtrkid_TPCDown1!=prevtrkid_TPCDown1 && prevtrkid_TPCDown1!=kBadNum) || (detname_before!=detname) ){
	 
	 fVecTPCDown1_TrackID.push_back(currtrkid_TPCDown1);
	 fVecTPCDown1_ParentID.push_back(currparentid_TPCDown1);
	  
	 fVecTPCDown1_TrackMomX.push_back((G4double)trkmom.x() / MeV);
	 fVecTPCDown1_TrackMomY.push_back((G4double)trkmom.y() / MeV);
	 fVecTPCDown1_TrackMomZ.push_back((G4double)trkmom.z() / MeV);
	 fVecTPCDown1_TrackCharge.push_back(trkcharge / eplus);
	 fVecTPCDown1_TrackPDG.push_back(trkpdg);

	 // get first step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTPCDown1_TrackStepFirstX.push_back(x);	 	 
	 fVecTPCDown1_TrackStepFirstY.push_back(y);	 	 
	 fVecTPCDown1_TrackStepFirstZ.push_back(z);	 	 

	 // get track cos(theta) from first pre-step wrt nu direction (0,0,1)
	 fVecTPCDown1_TrackCosTheta.push_back(costh);

	 prevtrkid_TPCDown1 = currtrkid_TPCDown1;
	 fTPCDown1_NTracks++;
	 
	 //G4cout << "First:" << prevtrkid_TPCDown1 << " --> " << currtrkid_TPCDown1 
	 //<< " - " << detname_before << " --> " << detname << G4endl;
	 
       } // if first step of the track 

       // Get Track initial informations (LAST STEP of the track in detector)
       
       if( currtrkid_TPCDown1!=aftertrkid_TPCDown1 || detname!=detname_after ){ 

	 // If aftertrkid_TPCDown1=kBadNum, still ok as last step
	 // If first step = last step --> store it both as first and last step
	 
	 // get last step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTPCDown1_TrackStepLastX.push_back(x);	 	 
	 fVecTPCDown1_TrackStepLastY.push_back(y);	 	 
	 fVecTPCDown1_TrackStepLastZ.push_back(z);	 	 
	 
	 //std::cout << "Last:" << currtrkid_TPCDown1 << " --> " << aftertrkid_TPCDown1
	 //<< " - " << detname << " --> " << detname_after << std::endl;     

       } // if last step of the track

     } // end TPCDown1

     else if(detname=="TPCDown2"){     
	 
       currtrkid_TPCDown2 = hit->GetTrackID();
       currparentid_TPCDown2 = hit->GetParentID();
       
       if(hit_after!=0) aftertrkid_TPCDown2 = hit_after->GetTrackID(); // following track 
       else aftertrkid_TPCDown2 = kBadNum;

       // Get Track initial informations (FIRST STEP of the track in detector)

       if( (prevtrkid_TPCDown2==kBadNum) || (currtrkid_TPCDown2!=prevtrkid_TPCDown2 && prevtrkid_TPCDown2!=kBadNum) || (detname_before!=detname) ){
	 
	 fVecTPCDown2_TrackID.push_back(currtrkid_TPCDown2);
	 fVecTPCDown2_ParentID.push_back(currparentid_TPCDown2);
	  
	 fVecTPCDown2_TrackMomX.push_back((G4double)trkmom.x() / MeV);
	 fVecTPCDown2_TrackMomY.push_back((G4double)trkmom.y() / MeV);
	 fVecTPCDown2_TrackMomZ.push_back((G4double)trkmom.z() / MeV);
	 fVecTPCDown2_TrackCharge.push_back(trkcharge / eplus);
	 fVecTPCDown2_TrackPDG.push_back(trkpdg);

	 // get first step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTPCDown2_TrackStepFirstX.push_back(x);	 	 
	 fVecTPCDown2_TrackStepFirstY.push_back(y);	 	 
	 fVecTPCDown2_TrackStepFirstZ.push_back(z);	 	 

	 // get track cos(theta) from first pre-step wrt nu direction (0,0,1)
	 fVecTPCDown2_TrackCosTheta.push_back(costh);

	 prevtrkid_TPCDown2 = currtrkid_TPCDown2;
	 fTPCDown2_NTracks++;
	 
	 //G4cout << "First:" << prevtrkid_TPCDown2 << " --> " << currtrkid_TPCDown2 
	 //<< " - " << detname_before << " --> " << detname << G4endl;
	 
       } // if first step of the track 

       // Get Track initial informations (LAST STEP of the track in detector)
       
       if( currtrkid_TPCDown2!=aftertrkid_TPCDown2 || detname!=detname_after ){ 

	 // If aftertrkid_TPCDown2=kBadNum, still ok as last step
	 // If first step = last step --> store it both as first and last step
	 
	 // get last step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTPCDown2_TrackStepLastX.push_back(x);	 	 
	 fVecTPCDown2_TrackStepLastY.push_back(y);	 	 
	 fVecTPCDown2_TrackStepLastZ.push_back(z);	 	 
	 
	 //std::cout << "Last:" << currtrkid_TPCDown2 << " --> " << aftertrkid_TPCDown2
	 //<< " - " << detname << " --> " << detname_after << std::endl;     

       } // if last step of the track

     } // end TPCDown2

     else if(detname=="Target1"){     
	 
       currtrkid_Target1 = hit->GetTrackID();
       currparentid_Target1 = hit->GetParentID();
	      
       if(hit_after!=0) aftertrkid_Target1 = hit_after->GetTrackID(); // following track 
       else aftertrkid_Target1 = kBadNum;

       // G4cout << "currtrkid_Target1 = " << currtrkid_Target1
       // 	      << " - prevtrkid_Target1 = " << prevtrkid_Target1
       // 	      << " - aftertrkid_Target1 = " << aftertrkid_Target1 
       // 	      << " - det = " << detname
       // 	      << " - det_before = " << detname_before
       // 	      << " - det_after = " << detname_after
       // 	      << G4endl;
        
       // Get Track initial informations (FIRST STEP of the track)
       
       if( prevtrkid_Target1==kBadNum || (currtrkid_Target1!=prevtrkid_Target1 && prevtrkid_Target1!=kBadNum) || (detname_before!=detname) ){
	 
	 fVecTarget1_TrackID.push_back(currtrkid_Target1);
	 fVecTarget1_ParentID.push_back(currparentid_Target1);
	 
	 fVecTarget1_TrackMomX.push_back((G4double)trkmom.x() / MeV);
	 fVecTarget1_TrackMomY.push_back((G4double)trkmom.y() / MeV);
	 fVecTarget1_TrackMomZ.push_back((G4double)trkmom.z() / MeV);
	 fVecTarget1_TrackCharge.push_back(trkcharge / eplus);
 	 fVecTarget1_TrackPDG.push_back(trkpdg);

	 // get first step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTarget1_TrackStepFirstX.push_back(x);	 	 
	 fVecTarget1_TrackStepFirstY.push_back(y);	 	 
	 fVecTarget1_TrackStepFirstZ.push_back(z);	 	 

	 // get track cos(theta) from first pre-step wrt nu direction (0,0,1)
	 fVecTarget1_TrackCosTheta.push_back(costh);
	 
	 prevtrkid_Target1 = currtrkid_Target1;
	 fTarget1_NTracks++;
	 // G4cout << "First:" << prevtrkid_Target1 << " --> " << currtrkid_Target1 
	 //  	<< " - " << detname_before << " --> " << detname << G4endl;
	 // G4cout << " - pos: " << x << ", " << y << ", " << z << G4endl;
	 
       } // if first step of the track

       // Get Track initial informations (LAST STEP of the track in detector)
       
       if( currtrkid_Target1!=aftertrkid_Target1 || detname!=detname_after ){ 

	 // If aftertrkid_Target1=kBadNum, still ok as last step
	 // If first step = last step --> store it both as first and last step
	 
	 // get last step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTarget1_TrackStepLastX.push_back(x);	 	 
	 fVecTarget1_TrackStepLastY.push_back(y);	 	 
	 fVecTarget1_TrackStepLastZ.push_back(z);	 	 
	 
	 //G4cout << "Last:" << currtrkid_Target1 << " --> " << aftertrkid_Target1 
	 //<< " - " << detname << " --> " << detname_after << G4endl;   
	 //G4cout << " - pos: " << x << ", " << y << ", " << z << G4endl;
  
       } // if last step of the track

     } // end Target1

     else if(detname=="Target2"){     
	 
       currtrkid_Target2 = hit->GetTrackID();
       currparentid_Target2 = hit->GetParentID();
	      
       if(hit_after!=0) aftertrkid_Target2 = hit_after->GetTrackID(); // following track 
       else aftertrkid_Target2 = kBadNum;

       // G4cout << "currtrkid_Target2 = " << currtrkid_Target2
       // 	      << " - prevtrkid_Target2 = " << prevtrkid_Target2
       // 	      << " - aftertrkid_Target2 = " << aftertrkid_Target2 
       // 	      << " - det = " << detname
       // 	      << " - det_before = " << detname_before
       // 	      << " - det_after = " << detname_after
       // 	      << G4endl;
       
       // Get Track initial informations (FIRST STEP of the track)
       
       if( prevtrkid_Target2==kBadNum || (currtrkid_Target2!=prevtrkid_Target2 && prevtrkid_Target2!=kBadNum) || (detname_before!=detname) ){
	 
	 fVecTarget2_TrackID.push_back(currtrkid_Target2);
	 fVecTarget2_ParentID.push_back(currparentid_Target2);
	 
	 fVecTarget2_TrackMomX.push_back((G4double)trkmom.x() / MeV);
	 fVecTarget2_TrackMomY.push_back((G4double)trkmom.y() / MeV);
	 fVecTarget2_TrackMomZ.push_back((G4double)trkmom.z() / MeV);
	 fVecTarget2_TrackCharge.push_back(trkcharge / eplus);
 	 fVecTarget2_TrackPDG.push_back(trkpdg);

	 // get first step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTarget2_TrackStepFirstX.push_back(x);	 	 
	 fVecTarget2_TrackStepFirstY.push_back(y);	 	 
	 fVecTarget2_TrackStepFirstZ.push_back(z);	 	 

	 // get track cos(theta) from first pre-step wrt nu direction (0,0,1)
	 fVecTarget2_TrackCosTheta.push_back(costh);
	 
	 prevtrkid_Target2 = currtrkid_Target2;
	 fTarget2_NTracks++;
	 // G4cout << "First:" << prevtrkid_Target2 << " --> " << currtrkid_Target2 
	 //  	<< " - " << detname_before << " --> " << detname << G4endl;
	 // G4cout << " - pos: " << x << ", " << y << ", " << z << G4endl;

       } // if first step of the track

       // Get Track initial informations (LAST STEP of the track in detector)
       
       if( currtrkid_Target2!=aftertrkid_Target2 || detname!=detname_after ){ 

	 // If aftertrkid_Target2=kBadNum, still ok as last step
	 // If first step = last step --> store it both as first and last step
	 
	 // get last step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTarget2_TrackStepLastX.push_back(x);	 	 
	 fVecTarget2_TrackStepLastY.push_back(y);	 	 
	 fVecTarget2_TrackStepLastZ.push_back(z);	 	 
	 
	 //G4cout << "Last:" << currtrkid_Target2 << " --> " << aftertrkid_Target2 
	 //<< " - " << detname << " --> " << detname_after << G4endl;   
	 //G4cout << " - pos: " << x << ", " << y << ", " << z << G4endl;
  
       } // if last step of the track

     } // end Target2


     


     // Forward TPC 1 (drift region: two halves left/right to cathode)
     else if(detname=="ForwTPC1/Half"){     	 
       currtrkid_ForwTPC1 = hit->GetTrackID(); // current track
       currparentid_ForwTPC1 = hit->GetParentID(); // current track
       
       //G4cout << "currtrkid_ForwTPC1 = " << currtrkid_ForwTPC1 << G4endl;
 
       if(hit_after!=0) aftertrkid_ForwTPC1 = hit_after->GetTrackID(); // following track 
       else aftertrkid_ForwTPC1 = kBadNum;

       // Get Track initial informations (FIRST STEP of the track in detector)

       // It's the first track of event or change track or change detector
       if( (prevtrkid_ForwTPC1==kBadNum) || (currtrkid_ForwTPC1!=prevtrkid_ForwTPC1 && prevtrkid_ForwTPC1!=kBadNum) || (detname_before!=detname) ){
	 
     	 fVecForwTPC1_TrackID.push_back(currtrkid_ForwTPC1);
     	 fVecForwTPC1_ParentID.push_back(currparentid_ForwTPC1);
	 fVecForwTPC1_TrackMomX.push_back((G4double)trkmom.x() / MeV);
     	 fVecForwTPC1_TrackMomY.push_back((G4double)trkmom.y() / MeV);
     	 fVecForwTPC1_TrackMomZ.push_back((G4double)trkmom.z() / MeV);
     	 fVecForwTPC1_TrackCharge.push_back(trkcharge / eplus);
     	 fVecForwTPC1_TrackPDG.push_back(trkpdg);

     	 // get first step position (mm)
     	 double x = hit->GetPreStepPos().x() / mm;
     	 double y = hit->GetPreStepPos().y() / mm;
     	 double z = hit->GetPreStepPos().z() / mm;
     	 fVecForwTPC1_TrackStepFirstX.push_back(x);	 	 
     	 fVecForwTPC1_TrackStepFirstY.push_back(y);	 	 
     	 fVecForwTPC1_TrackStepFirstZ.push_back(z);	 	 
     	 // get track cos(theta) from first pre-step wrt nu direction (0,0,1)
     	 fVecForwTPC1_TrackCosTheta.push_back(costh);

     	 prevtrkid_ForwTPC1 = currtrkid_ForwTPC1;
     	 fForwTPC1_NTracks++;	
	 
     	 //G4cout << "First:" << prevtrkid_ForwTPC1 << " --> " << currtrkid_ForwTPC1 
     	 //<< " - " << detname_before << " --> " << detname << G4endl;	 
       } // if first step of the track
       
       // Get Track initial informations (LAST STEP of the track in detector)       
       if( currtrkid_ForwTPC1!=aftertrkid_ForwTPC1 || detname!=detname_after ){ 

     	 // If aftertrkid_ForwTPC1=kBadNum, still ok as last step
     	 // If first step = last step --> store it both as first and last step
	 
     	 // get last step position (mm)
     	 double x = hit->GetPreStepPos().x() / mm;
     	 double y = hit->GetPreStepPos().y() / mm;
     	 double z = hit->GetPreStepPos().z() / mm;
     	 fVecForwTPC1_TrackStepLastX.push_back(x);	 	 
     	 fVecForwTPC1_TrackStepLastY.push_back(y);	 	 
     	 fVecForwTPC1_TrackStepLastZ.push_back(z);	 	 
     	 //std::cout << "Last:" << currtrkid_ForwTPC1 << " --> " << aftertrkid_ForwTPC1 
     	 //<< " - " << detname << " --> " << detname_after << std::endl;     
       } // if last step of the track       
     } // end ForwTPC1




     // Forward TPC 2 (drift region: two halves left/right to cathode)
     else if(detname=="ForwTPC2/Half"){     	 
       currtrkid_ForwTPC2 = hit->GetTrackID(); // current track
       currparentid_ForwTPC2 = hit->GetParentID(); // current track
       
       //G4cout << "currtrkid_ForwTPC2 = " << currtrkid_ForwTPC2 << G4endl;
 
       if(hit_after!=0) aftertrkid_ForwTPC2 = hit_after->GetTrackID(); // following track 
       else aftertrkid_ForwTPC2 = kBadNum;

       // Get Track initial informations (FIRST STEP of the track in detector)

       // It's the first track of event or change track or change detector
       if( (prevtrkid_ForwTPC2==kBadNum) || (currtrkid_ForwTPC2!=prevtrkid_ForwTPC2 && prevtrkid_ForwTPC2!=kBadNum) || (detname_before!=detname) ){
	 
     	 fVecForwTPC2_TrackID.push_back(currtrkid_ForwTPC2);
     	 fVecForwTPC2_ParentID.push_back(currparentid_ForwTPC2);
	 fVecForwTPC2_TrackMomX.push_back((G4double)trkmom.x() / MeV);
     	 fVecForwTPC2_TrackMomY.push_back((G4double)trkmom.y() / MeV);
     	 fVecForwTPC2_TrackMomZ.push_back((G4double)trkmom.z() / MeV);
     	 fVecForwTPC2_TrackCharge.push_back(trkcharge / eplus);
     	 fVecForwTPC2_TrackPDG.push_back(trkpdg);

     	 // get first step position (mm)
     	 double x = hit->GetPreStepPos().x() / mm;
     	 double y = hit->GetPreStepPos().y() / mm;
     	 double z = hit->GetPreStepPos().z() / mm;
     	 fVecForwTPC2_TrackStepFirstX.push_back(x);	 	 
     	 fVecForwTPC2_TrackStepFirstY.push_back(y);	 	 
     	 fVecForwTPC2_TrackStepFirstZ.push_back(z);	 	 
     	 // get track cos(theta) from first pre-step wrt nu direction (0,0,1)
     	 fVecForwTPC2_TrackCosTheta.push_back(costh);

     	 prevtrkid_ForwTPC2 = currtrkid_ForwTPC2;
     	 fForwTPC2_NTracks++;	
	 
     	 //G4cout << "First:" << prevtrkid_ForwTPC2 << " --> " << currtrkid_ForwTPC2 
     	 //<< " - " << detname_before << " --> " << detname << G4endl;	 
       } // if first step of the track
       
       // Get Track initial informations (LAST STEP of the track in detector)       
       if( currtrkid_ForwTPC2!=aftertrkid_ForwTPC2 || detname!=detname_after ){ 

     	 // If aftertrkid_ForwTPC2=kBadNum, still ok as last step
     	 // If first step = last step --> store it both as first and last step
	 
     	 // get last step position (mm)
     	 double x = hit->GetPreStepPos().x() / mm;
     	 double y = hit->GetPreStepPos().y() / mm;
     	 double z = hit->GetPreStepPos().z() / mm;
     	 fVecForwTPC2_TrackStepLastX.push_back(x);	 	 
     	 fVecForwTPC2_TrackStepLastY.push_back(y);	 	 
     	 fVecForwTPC2_TrackStepLastZ.push_back(z);	 	 
     	 //std::cout << "Last:" << currtrkid_ForwTPC2 << " --> " << aftertrkid_ForwTPC2 
     	 //<< " - " << detname << " --> " << detname_after << std::endl;     
       } // if last step of the track       
     } // end ForwTPC2



     // Forward TPC 3 (drift region: two halves left/right to cathode)
     else if(detname=="ForwTPC3/Half"){     	 
       currtrkid_ForwTPC3 = hit->GetTrackID(); // current track
       currparentid_ForwTPC3 = hit->GetParentID(); // current track
       
       //G4cout << "currtrkid_ForwTPC3 = " << currtrkid_ForwTPC3 << G4endl;
 
       if(hit_after!=0) aftertrkid_ForwTPC3 = hit_after->GetTrackID(); // following track 
       else aftertrkid_ForwTPC3 = kBadNum;

       // Get Track initial informations (FIRST STEP of the track in detector)

       // It's the first track of event or change track or change detector
       if( (prevtrkid_ForwTPC3==kBadNum) || (currtrkid_ForwTPC3!=prevtrkid_ForwTPC3 && prevtrkid_ForwTPC3!=kBadNum) || (detname_before!=detname) ){
	 
     	 fVecForwTPC3_TrackID.push_back(currtrkid_ForwTPC3);
     	 fVecForwTPC3_ParentID.push_back(currparentid_ForwTPC3);
	 fVecForwTPC3_TrackMomX.push_back((G4double)trkmom.x() / MeV);
     	 fVecForwTPC3_TrackMomY.push_back((G4double)trkmom.y() / MeV);
     	 fVecForwTPC3_TrackMomZ.push_back((G4double)trkmom.z() / MeV);
     	 fVecForwTPC3_TrackCharge.push_back(trkcharge / eplus);
     	 fVecForwTPC3_TrackPDG.push_back(trkpdg);

     	 // get first step position (mm)
     	 double x = hit->GetPreStepPos().x() / mm;
     	 double y = hit->GetPreStepPos().y() / mm;
     	 double z = hit->GetPreStepPos().z() / mm;
     	 fVecForwTPC3_TrackStepFirstX.push_back(x);	 	 
     	 fVecForwTPC3_TrackStepFirstY.push_back(y);	 	 
     	 fVecForwTPC3_TrackStepFirstZ.push_back(z);	 	 
     	 // get track cos(theta) from first pre-step wrt nu direction (0,0,1)
     	 fVecForwTPC3_TrackCosTheta.push_back(costh);

     	 prevtrkid_ForwTPC3 = currtrkid_ForwTPC3;
     	 fForwTPC3_NTracks++;	
	 
     	 //G4cout << "First:" << prevtrkid_ForwTPC3 << " --> " << currtrkid_ForwTPC3 
     	 //<< " - " << detname_before << " --> " << detname << G4endl;	 
       } // if first step of the track
       
       // Get Track initial informations (LAST STEP of the track in detector)       
       if( currtrkid_ForwTPC3!=aftertrkid_ForwTPC3 || detname!=detname_after ){ 

     	 // If aftertrkid_ForwTPC3=kBadNum, still ok as last step
     	 // If first step = last step --> store it both as first and last step
	 
     	 // get last step position (mm)
     	 double x = hit->GetPreStepPos().x() / mm;
     	 double y = hit->GetPreStepPos().y() / mm;
     	 double z = hit->GetPreStepPos().z() / mm;
     	 fVecForwTPC3_TrackStepLastX.push_back(x);	 	 
     	 fVecForwTPC3_TrackStepLastY.push_back(y);	 	 
     	 fVecForwTPC3_TrackStepLastZ.push_back(z);	 	 
     	 //std::cout << "Last:" << currtrkid_ForwTPC3 << " --> " << aftertrkid_ForwTPC3 
     	 //<< " - " << detname << " --> " << detname_after << std::endl;     
       } // if last step of the track       
     } // end ForwTPC3




     
   } // end loop over the hits
   
   
   // Resize the track vectors to be filled 

   fVecForwTPC1_TrackLength.resize(fForwTPC1_NTracks);
   fVecForwTPC1_TrackDeltaLyz.resize(fForwTPC1_NTracks);
   fVecForwTPC1_TrackEdep.resize(fForwTPC1_NTracks);

   fVecForwTPC2_TrackLength.resize(fForwTPC2_NTracks);
   fVecForwTPC2_TrackDeltaLyz.resize(fForwTPC2_NTracks);
   fVecForwTPC2_TrackEdep.resize(fForwTPC2_NTracks);

   fVecForwTPC3_TrackLength.resize(fForwTPC3_NTracks);
   fVecForwTPC3_TrackDeltaLyz.resize(fForwTPC3_NTracks);
   fVecForwTPC3_TrackEdep.resize(fForwTPC3_NTracks);
      
   fVecTPCUp1_TrackLength.resize(fTPCUp1_NTracks);
   fVecTPCUp1_TrackDeltaLyz.resize(fTPCUp1_NTracks);
   fVecTPCUp1_TrackEdep.resize(fTPCUp1_NTracks);
   
   fVecTPCUp2_TrackLength.resize(fTPCUp2_NTracks);
   fVecTPCUp2_TrackDeltaLyz.resize(fTPCUp2_NTracks);
   fVecTPCUp2_TrackEdep.resize(fTPCUp2_NTracks);

   fVecTPCDown1_TrackLength.resize(fTPCDown1_NTracks);
   fVecTPCDown1_TrackDeltaLyz.resize(fTPCDown1_NTracks);
   fVecTPCDown1_TrackEdep.resize(fTPCDown1_NTracks);

   fVecTPCDown2_TrackLength.resize(fTPCDown2_NTracks);
   fVecTPCDown2_TrackDeltaLyz.resize(fTPCDown2_NTracks);
   fVecTPCDown2_TrackEdep.resize(fTPCDown2_NTracks);
   
   fVecTarget1_TrackLength.resize(fTarget1_NTracks);
   fVecTarget1_TrackDeltaLyz.resize(fTarget1_NTracks);
   fVecTarget1_TrackEdep.resize(fTarget1_NTracks);

   fVecTarget2_TrackLength.resize(fTarget2_NTracks);
   fVecTarget2_TrackDeltaLyz.resize(fTarget2_NTracks);
   fVecTarget2_TrackEdep.resize(fTarget2_NTracks);

   // Loop over all the steps of each track

   for (G4int i=0;i<n_hit;i++){ // keep same order as above!!!
     
     ExN02TrackerHit* hit = (*hHC1)[i];
     G4double steplength = hit->GetStepLength();
     G4double stepdeltalyz = hit->GetStepDeltaLyz();
     G4double stepedep = hit->GetEdep();
     //G4double tracklength = hit->GetTrackLength(); // track length at each step --> not needed
     G4int trkID = hit->GetTrackID();
     G4String detname = hit->GetNameDet();

     // Loop over the tracks in Target 1
     if(detname=="Target1"){
       for(G4int itrk=0;itrk<fTarget1_NTracks;itrk++){
	 // Keep the same order as above!!	 
	 G4int trkOrder = fVecTarget1_TrackID.at(itrk);
	 if(trkOrder == trkID){	   
   	   fVecTarget1_TrackLength[itrk] += steplength / mm;
   	   fVecTarget1_TrackDeltaLyz[itrk] += stepdeltalyz / mm;
   	   fVecTarget1_TrackEdep[itrk] += stepedep / MeV;
	 } 
       } // end loop over tracks Target 1
     } // Target 1

     // Loop over the tracks in Target 2
     else if(detname=="Target2"){
       for(G4int itrk=0;itrk<fTarget2_NTracks;itrk++){
	 // Keep the same order as above!!	 
	 G4int trkOrder = fVecTarget2_TrackID.at(itrk);
	 if(trkOrder == trkID){	   
   	   fVecTarget2_TrackLength[itrk] += steplength / mm;
   	   fVecTarget2_TrackDeltaLyz[itrk] += stepdeltalyz / mm;
   	   fVecTarget2_TrackEdep[itrk] += stepedep / MeV;
	 } 
       } // end loop over tracks Target 2
     } // Target 2

     // Loop over the tracks in TPC Up 1
     else if(detname=="TPCUp1"){       
       for(G4int itrk=0;itrk<fTPCUp1_NTracks;itrk++){
	 // Keep the same order as above!!	 
	 G4int trkOrder = fVecTPCUp1_TrackID.at(itrk);
	 if(trkOrder == trkID){
	   fVecTPCUp1_TrackLength[itrk] += steplength / mm;
	   fVecTPCUp1_TrackDeltaLyz[itrk] += stepdeltalyz / mm;
	   fVecTPCUp1_TrackEdep[itrk] += stepedep / MeV;	 
	 } 
       } // end loop over tracks TPC Up 1
     } // TPC Up 1

     // Loop over the tracks in TPC Up 2
     else if(detname=="TPCUp2"){       
       for(G4int itrk=0;itrk<fTPCUp2_NTracks;itrk++){
	 // Keep the same order as above!!	 
	 G4int trkOrder = fVecTPCUp2_TrackID.at(itrk);
	 if(trkOrder == trkID){
	   fVecTPCUp2_TrackLength[itrk] += steplength / mm;
	   fVecTPCUp2_TrackDeltaLyz[itrk] += stepdeltalyz / mm;
	   fVecTPCUp2_TrackEdep[itrk] += stepedep / MeV;	 
	 } 
       } // end loop over tracks TPC Up 2
     } // TPC Up 2
     
     else if(detname=="TPCDown1"){       
       for(G4int itrk=0;itrk<fTPCDown1_NTracks;itrk++){
	 // Keep the same order as above!!	 
	 G4int trkOrder = fVecTPCDown1_TrackID.at(itrk);
	 if(trkOrder == trkID){
	   fVecTPCDown1_TrackLength[itrk] += steplength / mm;
	   fVecTPCDown1_TrackDeltaLyz[itrk] += stepdeltalyz / mm;
	   fVecTPCDown1_TrackEdep[itrk] += stepedep / MeV;	 
	 } 
       } // end loop over tracks TPC Down 1
     } // TPC Down 1

     else if(detname=="TPCDown2"){       
       for(G4int itrk=0;itrk<fTPCDown2_NTracks;itrk++){
	 // Keep the same order as above!!	 
	 G4int trkOrder = fVecTPCDown2_TrackID.at(itrk);
	 if(trkOrder == trkID){
	   fVecTPCDown2_TrackLength[itrk] += steplength / mm;
	   fVecTPCDown2_TrackDeltaLyz[itrk] += stepdeltalyz / mm;
	   fVecTPCDown2_TrackEdep[itrk] += stepedep / MeV;	 
	 } 
       } // end loop over tracks TPC Down 2
     } // TPC Down 2

     else if(detname=="ForwTPC1/Half"){       
       for(G4int itrk=0;itrk<fForwTPC1_NTracks;itrk++){
	 // Keep the same order as above!!	 
	 G4int trkOrder = fVecForwTPC1_TrackID.at(itrk);
	 if(trkOrder == trkID){
	   fVecForwTPC1_TrackLength[itrk] += steplength / mm;
	   fVecForwTPC1_TrackDeltaLyz[itrk] += stepdeltalyz / mm;
	   fVecForwTPC1_TrackEdep[itrk] += stepedep / MeV;	 
	 } 
       } // end loop over tracks Forward TPC 1
     } // Forward TPC 1   

     else if(detname=="ForwTPC2/Half"){       
       for(G4int itrk=0;itrk<fForwTPC2_NTracks;itrk++){
	 // Keep the same order as above!!	 
	 G4int trkOrder = fVecForwTPC2_TrackID.at(itrk);
	 if(trkOrder == trkID){
	   fVecForwTPC2_TrackLength[itrk] += steplength / mm;
	   fVecForwTPC2_TrackDeltaLyz[itrk] += stepdeltalyz / mm;
	   fVecForwTPC2_TrackEdep[itrk] += stepedep / MeV;	 
	 } 
       } // end loop over tracks Forward TPC 2
     } // Forward TPC 2  
     
     else if(detname=="ForwTPC3/Half"){       
       for(G4int itrk=0;itrk<fForwTPC3_NTracks;itrk++){
	 // Keep the same order as above!!	 
	 G4int trkOrder = fVecForwTPC3_TrackID.at(itrk);
	 if(trkOrder == trkID){
	   fVecForwTPC3_TrackLength[itrk] += steplength / mm;
	   fVecForwTPC3_TrackDeltaLyz[itrk] += stepdeltalyz / mm;
	   fVecForwTPC3_TrackEdep[itrk] += stepedep / MeV;	 
	 } 
       } // end loop over tracks Forward TPC 3
     } // Forward TPC 3  

   } // end loop over step     

  

  // get analysis manager  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  
  // // fill histograms
  //analysisManager->FillH1(1, fEnergyAbsTPCup);


  //
  // Fill Ntuple: be careful to the column ID. Check with RunAction where the Ntuple is created                 
  //

  analysisManager->FillNtupleDColumn(0,  fEnergyAbsForwTPC1);
  analysisManager->FillNtupleDColumn(1,  fEnergyAbsForwTPC2);
  analysisManager->FillNtupleDColumn(2,  fEnergyAbsForwTPC3);
  analysisManager->FillNtupleDColumn(3,  fEnergyAbsTPCup1);
  analysisManager->FillNtupleDColumn(4,  fEnergyAbsTPCdown1);
  analysisManager->FillNtupleDColumn(5,  fEnergyAbsTarget1);
  analysisManager->FillNtupleDColumn(6,  fTrackLAbsForwTPC1);
  analysisManager->FillNtupleDColumn(7,  fTrackLAbsForwTPC2);
  analysisManager->FillNtupleDColumn(8,  fTrackLAbsForwTPC3);
  analysisManager->FillNtupleDColumn(9,  fTrackLAbsTPCup1);
  analysisManager->FillNtupleDColumn(10,  fTrackLAbsTPCdown1);
  analysisManager->FillNtupleDColumn(11,  fTrackLAbsTarget1); 
  analysisManager->FillNtupleDColumn(12,  fEnergyAbsTPCup2);
  analysisManager->FillNtupleDColumn(13,  fEnergyAbsTPCdown2);
  analysisManager->FillNtupleDColumn(14,  fEnergyAbsTarget2);
  analysisManager->FillNtupleDColumn(15,  fTrackLAbsTPCup2);
  analysisManager->FillNtupleDColumn(16,  fTrackLAbsTPCdown2);
  analysisManager->FillNtupleDColumn(17,  fTrackLAbsTarget2); 
  analysisManager->FillNtupleIColumn(18,  fNTracks);
  analysisManager->FillNtupleIColumn(19,  fForwTPC1_NTracks);
  analysisManager->FillNtupleIColumn(20,  fForwTPC2_NTracks);
  analysisManager->FillNtupleIColumn(21,  fForwTPC3_NTracks);
  analysisManager->FillNtupleIColumn(22,  fTPCUp1_NTracks);
  analysisManager->FillNtupleIColumn(23,  fTPCDown1_NTracks);
  analysisManager->FillNtupleIColumn(24,  fTarget1_NTracks);
  analysisManager->FillNtupleIColumn(25,  fTPCUp2_NTracks);
  analysisManager->FillNtupleIColumn(26,  fTPCDown2_NTracks);
  analysisManager->FillNtupleIColumn(27,  fTarget2_NTracks);
  analysisManager->FillNtupleIColumn(28, fNVtx);
  
  analysisManager->AddNtupleRow();


  // Print per event (modulo n)
  //  
  G4int eventID = event->GetEventID();
  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;

    //   G4cout << "   Absorber TPC up 1: total energy: " << std::setw(7)
    // 	   << G4BestUnit(fEnergyAbsTPCup1,"Energy")
    // 	   << "       total track length: " << std::setw(7)
    // 	   << G4BestUnit(fTrackLAbsTPCup1,"Length")
    // 	   << G4endl 
    // 	   << "   Absorber TPC down 1: total energy: " << std::setw(7)
    // 	   << G4BestUnit(fEnergyAbsTPCdown,"Energy")
    // 	   << "       total track length: " << std::setw(7)
    // 	   << G4BestUnit(fTrackLAbsTPCdown,"Length")
    // 	   << G4endl
    //   G4cout << "   Absorber TPC up 2: total energy: " << std::setw(7)
    // 	   << G4BestUnit(fEnergyAbsTPCup2,"Energy")
    // 	   << "       total track length: " << std::setw(7)
    // 	   << G4BestUnit(fTrackLAbsTPCup2,"Length")
    // 	   << G4endl 
    // 	   << "   Absorber TPC down 2: total energy: " << std::setw(7)
    // 	   << G4BestUnit(fEnergyAbsTPCdown,"Energy")
    // 	   << "       total track length: " << std::setw(7)
    // 	   << G4BestUnit(fTrackLAbsTPCdown,"Length")
    // 	   << G4endl
    // 	   << "   Absorber Target: total energy: " << std::setw(7)
    // 	   << G4BestUnit(fEnergyAbsTarget,"Energy")
    // 	   << "       total track length: " << std::setw(7)
    // 	   << G4BestUnit(fTrackLAbsTarget,"Length")
    // 	   << G4endl;

  }

  // // Print out the informations from SteppingAction
  // std::cout << std::endl;
  // std::cout << "Print out the informations from SteppingAction:" << std::endl;
  // std::cout << std::endl;
  // std::cout << "# of Tracks = " << fNTracks << std::endl;
  // for(G4int itrk=0;itrk<fNTracks;itrk++){
  // std::cout << "Track ID: = " << fVecTrackID[itrk] 
  // 	      << " - Track PDG: = " << fVecTrackPDG[itrk]   
  // 	      << " - Track E: = " << fVecTrackE[itrk]   
  //  	      << " - Track Mom: = " << fVecTrackMomMag[itrk]   
  //  	      << std::endl;
  // }
  // std::cout << std::endl;


  
  // get number of stored trajectories
  //
  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  
  // periodic printing
  //
  if (eventID < 100 || eventID%100 == 0) {
    G4cout << ">>> Event " << event->GetEventID() << G4endl;
    G4cout << "    " << n_trajectories 
	   << " trajectories stored in this event." << G4endl;
  }
  G4cout << G4endl;

  


  

  
  //////////////////////////////////////////////
  //                                          //
  // Store the event in persistency ROOT file //
  //                                          //
  //////////////////////////////////////////////

  //G4cout << G4endl;
  //G4cout << " pND280man->Store(event)" << G4endl;
  //G4cout << G4endl;
  
  //ND280RootPersistencyManager* pND280man
  //= ND280RootPersistencyManager::GetInstance();
  //pND280man->Store(event);
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02EventAction::SetTrack(const G4Step *step) { // Used in SteppingAction

  G4Track *track = step->GetTrack();
  G4StepPoint *prestep = step->GetPreStepPoint();
  
  // Take track informations from its first step
  G4int newtrkid      = track->GetTrackID();
  G4int newparentid   = track->GetParentID();
  G4int newtrkpdg     = track->GetDefinition()->GetPDGEncoding();

  // Get the total initial energy of the track
  G4double newtrkmass        = track->GetDefinition()->GetPDGMass();
  G4double newtrkEkinVtx     = track->GetVertexKineticEnergy();  
  G4double newtrkEtotVtx     = newtrkEkinVtx + newtrkmass;             
  
  // Get the total initial momentum (not from GetTrack() that is poststep)  
  G4double newtrkMomX   = prestep->GetMomentum().x();
  G4double newtrkMomY   = prestep->GetMomentum().y();
  G4double newtrkMomZ   = prestep->GetMomentum().z();
  G4double newtrkMomMag = prestep->GetMomentum().mag();

  // G4cout << "Track ID = " << track->GetTrackID()
  // 	 << " - preStep Mom = " << prestep->GetMomentum().mag()
  // 	 << " - track Mom = " << track->GetMomentum().mag()
  // 	 << G4endl;
  
  // Set flags if track (first prestep) is contained in each detector
  //G4String detname = prestep->GetTouchableHandle()->GetVolume()->GetName();  
  //G4bool newtrkIsTPCUp = false;
  //G4bool newtrkIsTPCDown = false;
  //G4bool newtrkIsTarget = false;
  
  G4int vecel = fNTracks-1;

  //G4String detname = track->GetVolume()->GetName();
  //std::vector < G4String > vecstr;  
  //if(detname!="TPCUp1") return;

  if(fNTracks==0){
    fVecTrackID .push_back(newtrkid);
    fVecParentID.push_back(newparentid);
    fVecTrackPDG.push_back(newtrkpdg);
    fVecTrackE  .push_back(newtrkEtotVtx);

    fVecTrackMomX.push_back(newtrkMomX);
    fVecTrackMomY.push_back(newtrkMomY);
    fVecTrackMomZ.push_back(newtrkMomZ);
    fVecTrackMomMag.push_back(newtrkMomMag);

    fNTracks++;
   }
  else if(fNTracks>0 && newtrkid!=fVecTrackID[vecel]){ // Only the first step
    fVecTrackID .push_back(newtrkid);
    fVecParentID.push_back(newparentid);
    fVecTrackPDG.push_back(newtrkpdg);
    fVecTrackE  .push_back(newtrkEtotVtx);

    fVecTrackMomX.push_back(newtrkMomX);
    fVecTrackMomY.push_back(newtrkMomY);
    fVecTrackMomZ.push_back(newtrkMomZ);
    fVecTrackMomMag.push_back(newtrkMomMag);

    fNTracks++;
  }
}
