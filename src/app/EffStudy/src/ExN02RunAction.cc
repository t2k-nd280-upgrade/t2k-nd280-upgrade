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
// $Id: ExN02RunAction.cc,v 1.9 2006-06-29 17:48:16 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#include <ctime>
#include <sys/time.h>


#include "ExN02RunAction.hh"
#include "ExN02EventAction.hh"
#include "ExN02Analysis.hh" // TODO

#include "ND280PersistencyManager.hh"
#include "ND280RootPersistencyManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

//#include "G4SystemOfUnits.hh" // NEW GLOBAL
#include <CLHEP/Units/SystemOfUnits.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//ExN02RunAction::ExN02RunAction()
//  : G4UserRunAction()
ExN02RunAction::ExN02RunAction(ExN02EventAction *eventAction)
  : G4UserRunAction(),
    fEventAction(eventAction)
{
  //set the seed engine
  //CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine); // NEW SEED

  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  // Create analysis manager                                        
  // The choice of analysis technology is done via selectin of a namespace       
  // in B4Analysis.hh                                                              
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories                                            
  //analysisManager->SetHistoDirectoryName("histograms");                            
  //analysisManager->SetNtupleDirectoryName("ntuple");

  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);

  // Book histograms, ntuple
  //

  // // Creating histograms 
  analysisManager->CreateH1("Edep_abs_tpcup","Edep in TPC Up", 100, 0., 800*CLHEP::MeV);
  
  // Creating ntuple
  //             
  if(fEventAction){
    //
    // Here the numbering MUST match the one in EventAction, where Ntuple is filled!!
    //
    /*
    analysisManager->CreateNtuple("EffStudy","Primary particle and total absorbed energy");
    analysisManager->CreateNtupleDColumn("EabsForwTPC1");      // column Id = 0
    analysisManager->CreateNtupleDColumn("EabsForwTPC2");      // column Id = 1
    analysisManager->CreateNtupleDColumn("EabsForwTPC3");      // column Id = 2
    analysisManager->CreateNtupleDColumn("EabsTPCup1");      // column Id = 3
    analysisManager->CreateNtupleDColumn("EabsTPCdown1");    // column Id = 4
    analysisManager->CreateNtupleDColumn("EabsTarget1");     // column Id = 5
    analysisManager->CreateNtupleDColumn("LabsForwTPC1");      // column Id = 6
    analysisManager->CreateNtupleDColumn("LabsForwTPC2");      // column Id = 7
    analysisManager->CreateNtupleDColumn("LabsForwTPC3");      // column Id = 8
    analysisManager->CreateNtupleDColumn("LabsTPCup1");      // column Id = 9
    analysisManager->CreateNtupleDColumn("LabsTPCdown1");    // column Id = 10
    analysisManager->CreateNtupleDColumn("LabsTarget1");     // column Id = 11
    analysisManager->CreateNtupleDColumn("EabsTPCup2");      // column Id = 12
    analysisManager->CreateNtupleDColumn("EabsTPCdown2");    // column Id = 13
    analysisManager->CreateNtupleDColumn("EabsTarget2");     // column Id = 14
    analysisManager->CreateNtupleDColumn("LabsTPCup2");      // column Id = 15
    analysisManager->CreateNtupleDColumn("LabsTPCdown2");    // column Id = 16
    analysisManager->CreateNtupleDColumn("LabsTarget2");     // column Id = 17
    analysisManager->CreateNtupleIColumn("NTracks");        // column Id = 18
    analysisManager->CreateNtupleIColumn("ForwTPC1_NTracks");   // column Id = 19
    analysisManager->CreateNtupleIColumn("ForwTPC2_NTracks");   // column Id = 20
    analysisManager->CreateNtupleIColumn("ForwTPC3_NTracks");   // column Id = 21
    analysisManager->CreateNtupleIColumn("TPCUp1_NTracks");   // column Id = 22
    analysisManager->CreateNtupleIColumn("TPCDown1_NTracks"); // column Id = 23
    analysisManager->CreateNtupleIColumn("Target1_NTracks");  // column Id = 24  
    analysisManager->CreateNtupleIColumn("TPCUp2_NTracks");   // column Id = 25
    analysisManager->CreateNtupleIColumn("TPCDown2_NTracks"); // column Id = 26
    analysisManager->CreateNtupleIColumn("Target2_NTracks");  // column Id = 27  
    analysisManager->CreateNtupleIColumn("NVtx");            // column Id = 28

    //
    // WARNING concerning vectors: HARMLESS 
    // (see http://hypernews.slac.stanford.edu/HyperNews/geant4/cindex)
    //
    // -------- WWWW ------- G4Exception-START -------- WWWW -------
    // *** G4Exception : Analysis_W002
    // issued by : G4RootNtupleManager::CreateNtupleFromBooking()
    // Unsupported column type VecTarget_TrackMomX
    // *** This is just a warning message. ***
    // -------- WWWW -------- G4Exception-END --------- WWWW -------
    //

    //
    // IMPORTANT: Vector Ntuple columns must be created after the variables in order
    //            to not break the order and Ntuple ID
    //

    analysisManager->CreateNtupleIColumn("VecTrackID" ,fEventAction->GetVecTrackID());        // column Id = 15
    analysisManager->CreateNtupleIColumn("VecParentID" ,fEventAction->GetVecParentID());        // column Id = 15
    analysisManager->CreateNtupleIColumn("VecTrackPDG",fEventAction->GetVecTrackPDG());       // column Id = 16
    analysisManager->CreateNtupleDColumn("VecTrackE",  fEventAction->GetVecTrackE());         // column Id = 17
    analysisManager->CreateNtupleDColumn("VecTrackMomX",  fEventAction->GetVecTrackMomX());   // column Id = 18
    analysisManager->CreateNtupleDColumn("VecTrackMomY",  fEventAction->GetVecTrackMomY());   // column Id = 19
    analysisManager->CreateNtupleDColumn("VecTrackMomZ",  fEventAction->GetVecTrackMomZ());   // column Id = 20
    analysisManager->CreateNtupleDColumn("VecTrackMomMag",fEventAction->GetVecTrackMomMag()); // column Id = 21

    analysisManager->CreateNtupleIColumn("VecForwTPC1_TrackID",fEventAction->GetVecForwTPC1_TrackID());     // column Id = 22
    analysisManager->CreateNtupleIColumn("VecForwTPC1_ParentID",fEventAction->GetVecForwTPC1_ParentID());     // column Id = 22
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackMomX",fEventAction->GetVecForwTPC1_TrackMomX()); // column Id = 23
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackMomY",fEventAction->GetVecForwTPC1_TrackMomY()); // column Id = 24
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackMomZ",fEventAction->GetVecForwTPC1_TrackMomZ()); // column Id = 25
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackLength",fEventAction->GetVecForwTPC1_TrackLength()); // column Id = 26
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackDeltaLyz",fEventAction->GetVecForwTPC1_TrackDeltaLyz()); // column Id = 26
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackEdep",fEventAction->GetVecForwTPC1_TrackEdep()); // column Id = 27
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackCharge",fEventAction->GetVecForwTPC1_TrackCharge()); // column Id = 28
    analysisManager->CreateNtupleIColumn("VecForwTPC1_TrackPDG",fEventAction->GetVecForwTPC1_TrackPDG()); // column Id = 29  
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackStepFirstX",fEventAction->GetVecForwTPC1_TrackStepFirstX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackStepFirstY",fEventAction->GetVecForwTPC1_TrackStepFirstY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackStepFirstZ",fEventAction->GetVecForwTPC1_TrackStepFirstZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackStepLastX",fEventAction->GetVecForwTPC1_TrackStepLastX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackStepLastY",fEventAction->GetVecForwTPC1_TrackStepLastY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackStepLastZ",fEventAction->GetVecForwTPC1_TrackStepLastZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC1_TrackCosTheta",fEventAction->GetVecForwTPC1_TrackCosTheta()); // column Id = 30 
 
    analysisManager->CreateNtupleIColumn("VecForwTPC2_TrackID",fEventAction->GetVecForwTPC2_TrackID());     // column Id = 22
    analysisManager->CreateNtupleIColumn("VecForwTPC2_ParentID",fEventAction->GetVecForwTPC2_ParentID());     // column Id = 22
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackMomX",fEventAction->GetVecForwTPC2_TrackMomX()); // column Id = 23
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackMomY",fEventAction->GetVecForwTPC2_TrackMomY()); // column Id = 24
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackMomZ",fEventAction->GetVecForwTPC2_TrackMomZ()); // column Id = 25
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackLength",fEventAction->GetVecForwTPC2_TrackLength()); // column Id = 26
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackDeltaLyz",fEventAction->GetVecForwTPC2_TrackDeltaLyz()); // column Id = 26
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackEdep",fEventAction->GetVecForwTPC2_TrackEdep()); // column Id = 27
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackCharge",fEventAction->GetVecForwTPC2_TrackCharge()); // column Id = 28
    analysisManager->CreateNtupleIColumn("VecForwTPC2_TrackPDG",fEventAction->GetVecForwTPC2_TrackPDG()); // column Id = 29  
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackStepFirstX",fEventAction->GetVecForwTPC2_TrackStepFirstX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackStepFirstY",fEventAction->GetVecForwTPC2_TrackStepFirstY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackStepFirstZ",fEventAction->GetVecForwTPC2_TrackStepFirstZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackStepLastX",fEventAction->GetVecForwTPC2_TrackStepLastX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackStepLastY",fEventAction->GetVecForwTPC2_TrackStepLastY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackStepLastZ",fEventAction->GetVecForwTPC2_TrackStepLastZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC2_TrackCosTheta",fEventAction->GetVecForwTPC2_TrackCosTheta()); // column Id = 30 
 
    analysisManager->CreateNtupleIColumn("VecForwTPC3_TrackID",fEventAction->GetVecForwTPC3_TrackID());     // column Id = 22
    analysisManager->CreateNtupleIColumn("VecForwTPC3_ParentID",fEventAction->GetVecForwTPC3_ParentID());     // column Id = 22
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackMomX",fEventAction->GetVecForwTPC3_TrackMomX()); // column Id = 23
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackMomY",fEventAction->GetVecForwTPC3_TrackMomY()); // column Id = 24
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackMomZ",fEventAction->GetVecForwTPC3_TrackMomZ()); // column Id = 25
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackLength",fEventAction->GetVecForwTPC3_TrackLength()); // column Id = 26
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackDeltaLyz",fEventAction->GetVecForwTPC3_TrackDeltaLyz()); // column Id = 26
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackEdep",fEventAction->GetVecForwTPC3_TrackEdep()); // column Id = 27
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackCharge",fEventAction->GetVecForwTPC3_TrackCharge()); // column Id = 28
    analysisManager->CreateNtupleIColumn("VecForwTPC3_TrackPDG",fEventAction->GetVecForwTPC3_TrackPDG()); // column Id = 29  
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackStepFirstX",fEventAction->GetVecForwTPC3_TrackStepFirstX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackStepFirstY",fEventAction->GetVecForwTPC3_TrackStepFirstY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackStepFirstZ",fEventAction->GetVecForwTPC3_TrackStepFirstZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackStepLastX",fEventAction->GetVecForwTPC3_TrackStepLastX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackStepLastY",fEventAction->GetVecForwTPC3_TrackStepLastY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackStepLastZ",fEventAction->GetVecForwTPC3_TrackStepLastZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecForwTPC3_TrackCosTheta",fEventAction->GetVecForwTPC3_TrackCosTheta()); // column Id = 30 
    
    analysisManager->CreateNtupleIColumn("VecTPCUp1_TrackID",fEventAction->GetVecTPCUp1_TrackID());     // column Id = 22
    analysisManager->CreateNtupleIColumn("VecTPCUp1_ParentID",fEventAction->GetVecTPCUp1_ParentID());     // column Id = 22
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackMomX",fEventAction->GetVecTPCUp1_TrackMomX()); // column Id = 23
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackMomY",fEventAction->GetVecTPCUp1_TrackMomY()); // column Id = 24
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackMomZ",fEventAction->GetVecTPCUp1_TrackMomZ()); // column Id = 25
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackLength",fEventAction->GetVecTPCUp1_TrackLength()); // column Id = 26
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackDeltaLyz",fEventAction->GetVecTPCUp1_TrackDeltaLyz()); // column Id = 26
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackEdep",fEventAction->GetVecTPCUp1_TrackEdep()); // column Id = 27
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackCharge",fEventAction->GetVecTPCUp1_TrackCharge()); // column Id = 28
    analysisManager->CreateNtupleIColumn("VecTPCUp1_TrackPDG",fEventAction->GetVecTPCUp1_TrackPDG()); // column Id = 29  
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackStepFirstX",fEventAction->GetVecTPCUp1_TrackStepFirstX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackStepFirstY",fEventAction->GetVecTPCUp1_TrackStepFirstY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackStepFirstZ",fEventAction->GetVecTPCUp1_TrackStepFirstZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackStepLastX",fEventAction->GetVecTPCUp1_TrackStepLastX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackStepLastY",fEventAction->GetVecTPCUp1_TrackStepLastY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackStepLastZ",fEventAction->GetVecTPCUp1_TrackStepLastZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCUp1_TrackCosTheta",fEventAction->GetVecTPCUp1_TrackCosTheta()); // column Id = 30 

    analysisManager->CreateNtupleIColumn("VecTPCUp2_TrackID",fEventAction->GetVecTPCUp2_TrackID());     // column Id = 22
    analysisManager->CreateNtupleIColumn("VecTPCUp2_ParentID",fEventAction->GetVecTPCUp2_ParentID());     // column Id = 22
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackMomX",fEventAction->GetVecTPCUp2_TrackMomX()); // column Id = 23
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackMomY",fEventAction->GetVecTPCUp2_TrackMomY()); // column Id = 24
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackMomZ",fEventAction->GetVecTPCUp2_TrackMomZ()); // column Id = 25
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackLength",fEventAction->GetVecTPCUp2_TrackLength()); // column Id = 26
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackDeltaLyz",fEventAction->GetVecTPCUp2_TrackDeltaLyz()); // column Id = 26
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackEdep",fEventAction->GetVecTPCUp2_TrackEdep()); // column Id = 27
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackCharge",fEventAction->GetVecTPCUp2_TrackCharge()); // column Id = 28
    analysisManager->CreateNtupleIColumn("VecTPCUp2_TrackPDG",fEventAction->GetVecTPCUp2_TrackPDG()); // column Id = 29  
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackStepFirstX",fEventAction->GetVecTPCUp2_TrackStepFirstX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackStepFirstY",fEventAction->GetVecTPCUp2_TrackStepFirstY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackStepFirstZ",fEventAction->GetVecTPCUp2_TrackStepFirstZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackStepLastX",fEventAction->GetVecTPCUp2_TrackStepLastX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackStepLastY",fEventAction->GetVecTPCUp2_TrackStepLastY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackStepLastZ",fEventAction->GetVecTPCUp2_TrackStepLastZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCUp2_TrackCosTheta",fEventAction->GetVecTPCUp2_TrackCosTheta()); // column Id = 30 

    analysisManager->CreateNtupleIColumn("VecTPCDown1_TrackID",fEventAction->GetVecTPCDown1_TrackID());     // column Id = 31
    analysisManager->CreateNtupleIColumn("VecTPCDown1_ParentID",fEventAction->GetVecTPCDown1_ParentID());     // column Id = 31
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackMomX",fEventAction->GetVecTPCDown1_TrackMomX()); // column Id = 32
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackMomY",fEventAction->GetVecTPCDown1_TrackMomY()); // column Id = 33
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackMomZ",fEventAction->GetVecTPCDown1_TrackMomZ()); // column Id = 34
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackLength",fEventAction->GetVecTPCDown1_TrackLength()); // column Id = 35 
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackDeltaLyz",fEventAction->GetVecTPCDown1_TrackDeltaLyz()); // column Id = 35 
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackEdep",fEventAction->GetVecTPCDown1_TrackEdep()); // column Id = 36
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackCharge",fEventAction->GetVecTPCDown1_TrackCharge()); // column Id = 36
    analysisManager->CreateNtupleIColumn("VecTPCDown1_TrackPDG",fEventAction->GetVecTPCDown1_TrackPDG()); // column Id = 37 
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackStepFirstX",fEventAction->GetVecTPCDown1_TrackStepFirstX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackStepFirstY",fEventAction->GetVecTPCDown1_TrackStepFirstY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackStepFirstZ",fEventAction->GetVecTPCDown1_TrackStepFirstZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackStepLastX",fEventAction->GetVecTPCDown1_TrackStepLastX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackStepLastY",fEventAction->GetVecTPCDown1_TrackStepLastY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackStepLastZ",fEventAction->GetVecTPCDown1_TrackStepLastZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCDown1_TrackCosTheta",fEventAction->GetVecTPCDown1_TrackCosTheta()); // column Id = 30 

    analysisManager->CreateNtupleIColumn("VecTPCDown2_TrackID",fEventAction->GetVecTPCDown2_TrackID());     // column Id = 31
    analysisManager->CreateNtupleIColumn("VecTPCDown2_ParentID",fEventAction->GetVecTPCDown2_ParentID());     // column Id = 31
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackMomX",fEventAction->GetVecTPCDown2_TrackMomX()); // column Id = 32
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackMomY",fEventAction->GetVecTPCDown2_TrackMomY()); // column Id = 33
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackMomZ",fEventAction->GetVecTPCDown2_TrackMomZ()); // column Id = 34
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackLength",fEventAction->GetVecTPCDown2_TrackLength()); // column Id = 35 
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackDeltaLyz",fEventAction->GetVecTPCDown2_TrackDeltaLyz()); // column Id = 35 
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackEdep",fEventAction->GetVecTPCDown2_TrackEdep()); // column Id = 36
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackCharge",fEventAction->GetVecTPCDown2_TrackCharge()); // column Id = 36
    analysisManager->CreateNtupleIColumn("VecTPCDown2_TrackPDG",fEventAction->GetVecTPCDown2_TrackPDG()); // column Id = 37 
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackStepFirstX",fEventAction->GetVecTPCDown2_TrackStepFirstX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackStepFirstY",fEventAction->GetVecTPCDown2_TrackStepFirstY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackStepFirstZ",fEventAction->GetVecTPCDown2_TrackStepFirstZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackStepLastX",fEventAction->GetVecTPCDown2_TrackStepLastX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackStepLastY",fEventAction->GetVecTPCDown2_TrackStepLastY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackStepLastZ",fEventAction->GetVecTPCDown2_TrackStepLastZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTPCDown2_TrackCosTheta",fEventAction->GetVecTPCDown2_TrackCosTheta()); // column Id = 30 

    analysisManager->CreateNtupleIColumn("VecTarget1_TrackID",fEventAction->GetVecTarget1_TrackID());     // column Id = 38
    analysisManager->CreateNtupleIColumn("VecTarget1_ParentID",fEventAction->GetVecTarget1_ParentID());     // column Id = 38
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackMomX",fEventAction->GetVecTarget1_TrackMomX()); // column Id = 39
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackMomY",fEventAction->GetVecTarget1_TrackMomY()); // column Id = 40
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackMomZ",fEventAction->GetVecTarget1_TrackMomZ()); // column Id = 41
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackLength",fEventAction->GetVecTarget1_TrackLength()); // column Id = 42
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackDeltaLyz",fEventAction->GetVecTarget1_TrackDeltaLyz()); // column Id = 42
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackEdep",fEventAction->GetVecTarget1_TrackEdep()); // column Id = 43
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackCharge",fEventAction->GetVecTarget1_TrackCharge()); // column Id = 44
    analysisManager->CreateNtupleIColumn("VecTarget1_TrackPDG",fEventAction->GetVecTarget1_TrackPDG()); // column Id = 44  
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackStepFirstX",fEventAction->GetVecTarget1_TrackStepFirstX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackStepFirstY",fEventAction->GetVecTarget1_TrackStepFirstY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackStepFirstZ",fEventAction->GetVecTarget1_TrackStepFirstZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackStepLastX",fEventAction->GetVecTarget1_TrackStepLastX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackStepLastY",fEventAction->GetVecTarget1_TrackStepLastY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackStepLastZ",fEventAction->GetVecTarget1_TrackStepLastZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTarget1_TrackCosTheta",fEventAction->GetVecTarget1_TrackCosTheta()); // column Id = 30 

    analysisManager->CreateNtupleIColumn("VecTarget2_TrackID",fEventAction->GetVecTarget2_TrackID());     // column Id = 38
    analysisManager->CreateNtupleIColumn("VecTarget2_ParentID",fEventAction->GetVecTarget2_ParentID());     // column Id = 38
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackMomX",fEventAction->GetVecTarget2_TrackMomX()); // column Id = 39
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackMomY",fEventAction->GetVecTarget2_TrackMomY()); // column Id = 40
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackMomZ",fEventAction->GetVecTarget2_TrackMomZ()); // column Id = 41
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackLength",fEventAction->GetVecTarget2_TrackLength()); // column Id = 42
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackDeltaLyz",fEventAction->GetVecTarget2_TrackDeltaLyz()); // column Id = 42
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackEdep",fEventAction->GetVecTarget2_TrackEdep()); // column Id = 43
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackCharge",fEventAction->GetVecTarget2_TrackCharge()); // column Id = 44
    analysisManager->CreateNtupleIColumn("VecTarget2_TrackPDG",fEventAction->GetVecTarget2_TrackPDG()); // column Id = 44  
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackStepFirstX",fEventAction->GetVecTarget2_TrackStepFirstX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackStepFirstY",fEventAction->GetVecTarget2_TrackStepFirstY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackStepFirstZ",fEventAction->GetVecTarget2_TrackStepFirstZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackStepLastX",fEventAction->GetVecTarget2_TrackStepLastX()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackStepLastY",fEventAction->GetVecTarget2_TrackStepLastY()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackStepLastZ",fEventAction->GetVecTarget2_TrackStepLastZ()); // column Id = 30 
    analysisManager->CreateNtupleDColumn("VecTarget2_TrackCosTheta",fEventAction->GetVecTarget2_TrackCosTheta()); // column Id = 30 

    analysisManager->CreateNtupleDColumn("VtxX",fEventAction->GetVecVtx_X()); // column Id = 45   
    analysisManager->CreateNtupleDColumn("VtxY",fEventAction->GetVecVtx_Y()); // column Id = 46   
    analysisManager->CreateNtupleDColumn("VtxZ",fEventAction->GetVecVtx_Z()); // column Id = 47
    
    analysisManager->CreateNtupleIColumn("VecVtx_NuPDG" ,fEventAction->GetVecVtx_NuPDG());     // column Id = 48   
    analysisManager->CreateNtupleIColumn("VecVtx_ReacMode" ,fEventAction->GetVecVtx_ReacMode());  // column Id = 49   
    analysisManager->CreateNtupleDColumn("VecVtx_EvtProb"  ,fEventAction->GetVecVtx_EvtProb());   // column Id = 50   
    analysisManager->CreateNtupleDColumn("VecVtx_EvtWeight",fEventAction->GetVecVtx_EvtWeight()); // column Id = 51   

    analysisManager->FinishNtuple();

    */
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02RunAction::~ExN02RunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  // Create ROOT the persistency manager.                                                                                                                                                            
  ND280RootPersistencyManager* persistencyManager
    = ND280RootPersistencyManager::GetInstance();
  ExN02ND280XML *ND280XMLInput = persistencyManager->GetXMLInput();
  bool IsRandomSeed = ND280XMLInput->GetXMLIsRandomSeed();

  // set the Seed
  if(IsRandomSeed) SetSeed();




  //inform the runManager to save random number seed  
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);      
  // // Store the seed
  //G4RunManager::GetRunManager()->rndmSaveThisEvent();



  
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  // Open an output file     
  //
  //G4String fileName = "EffStudy";
  //analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::EndOfRunAction(const G4Run*)
{ 
  // print histogram statistics         
  //
  //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //if ( analysisManager->GetH1(1) ) {
  //G4cout << G4endl << " ----> print histograms statistic ";
  //if(isMaster) {
  //G4cout << "for the entire run " << G4endl << G4endl;
  //}
  //else {
  //G4cout << "for the local thread " << G4endl << G4endl;
  //}
  // G4cout << " EAbs : mean = "
  // 	   << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy")
  // 	   << " rms = "
  // 	   << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") << G4endl;
  // G4cout << " EGap : mean = "
  // 	   << G4BestUnit(analysisManager->GetH1(2)->mean(), "Energy")
  // 	   << " rms = "
  // 	   << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Energy") << G4endl;
  // G4cout << " LAbs : mean = "
  // 	   << G4BestUnit(analysisManager->GetH1(3)->mean(), "Length")
  // 	   << " rms = "
  // 	   << G4BestUnit(analysisManager->GetH1(3)->rms(),  "Length") << G4endl;
  // G4cout << " LGap : mean = "
  // 	   << G4BestUnit(analysisManager->GetH1(4)->mean(), "Length")
  // 	   << " rms = "
  // 	   << G4BestUnit(analysisManager->GetH1(4)->rms(),  "Length") << G4endl;
  //}
  //
  // save histograms & ntuple   
  //
  //analysisManager->Write();
  //analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::SetSeed() {

  //set random seed with system time
  
  //G4long seed = time(NULL); 
  //if (seed<0) seed = -seed;
  //G4cout << "### Random seed number set to: " << seed<< G4endl;
  //CLHEP::HepRandom::setTheSeed(seed);
  
  // As used in nd280mc
  long seed = 0.;
  struct timeval buffer;
  gettimeofday(&buffer,NULL);
  //ND280Log("### Set seed from local time: " << ctime(&buffer.tv_sec));
  //ND280Log("###      Micro-second offset: " << buffer.tv_usec);
  seed = long(buffer.tv_sec + buffer.tv_usec);
  SetSeed(long(seed));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

void ExN02RunAction::SetSeed(long seed) {
  if (seed<0) seed = -seed;
  G4cout << "### Random seed number set to: "<< seed << G4endl;
  CLHEP::HepRandom::setTheSeed(seed);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

long ExN02RunAction::GetSeed(void) const {
  return CLHEP::HepRandom::getTheSeed();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
