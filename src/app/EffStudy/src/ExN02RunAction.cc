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

#include "ExN02RunAction.hh"
#include "ExN02EventAction.hh"
#include "ExN02Analysis.hh" // TODO

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//ExN02RunAction::ExN02RunAction()
//  : G4UserRunAction()
ExN02RunAction::ExN02RunAction(ExN02EventAction *eventAction)
  : G4UserRunAction(),
    fEventAction(eventAction)
{
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
  analysisManager->CreateH1("Edep_abs_tpcup","Edep in TPC Up", 100, 0., 800*MeV);
  // analysisManager->CreateH1("Edep_abs_tpcdown","Edep in TPC Down", 100, 0., 800*MeV);
  // analysisManager->CreateH1("Edep_abs_target","Edep in Target", 100, 0., 800*MeV);
  // analysisManager->CreateH1("TrkLght_abs_tpcup","trackL in TPC Up", 100, 0., 2*m);
  // analysisManager->CreateH1("TrkLght_abs_tpcdown","trackL in TPC Down", 100, 0., 2*m);
  // analysisManager->CreateH1("TrkLght_abs_target","trackL in Target", 100, 0., 2*m);
  
  // Creating ntuple
  //             
  if(fEventAction){
    analysisManager->CreateNtuple("EffStudy","Primary particle and total absorbed energy");
    analysisManager->CreateNtupleDColumn("EabsTPCup");      // column Id = 0
    analysisManager->CreateNtupleDColumn("EabsTPCdown");    // column Id = 1
    analysisManager->CreateNtupleDColumn("EabsTarget");     // column Id = 2
    analysisManager->CreateNtupleDColumn("LabsTPCup");      // column Id = 3
    analysisManager->CreateNtupleDColumn("LabsTPCdown");    // column Id = 4
    analysisManager->CreateNtupleDColumn("LabsTarget");     // column Id = 5
    analysisManager->CreateNtupleIColumn("NTracks");        // column Id = 6
    
    analysisManager->CreateNtupleIColumn("TPCUp_NTracks");   // column Id = 7
    analysisManager->CreateNtupleIColumn("TPCDown_NTracks"); // column Id = 8
    analysisManager->CreateNtupleIColumn("Target_NTracks");  // column Id = 9   


    //
    // WARNING concerning vectors: HARMLESS
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
    
    analysisManager->CreateNtupleIColumn("VecTrackID" ,fEventAction->GetVecTrackID());        // column Id = 10
    analysisManager->CreateNtupleIColumn("VecTrackPDG",fEventAction->GetVecTrackPDG());       // column Id = 11
    analysisManager->CreateNtupleDColumn("VecTrackE",  fEventAction->GetVecTrackE());         // column Id = 12
    analysisManager->CreateNtupleDColumn("VecTrackMomX",  fEventAction->GetVecTrackMomX());   // column Id = 13
    analysisManager->CreateNtupleDColumn("VecTrackMomY",  fEventAction->GetVecTrackMomY());   // column Id = 14
    analysisManager->CreateNtupleDColumn("VecTrackMomZ",  fEventAction->GetVecTrackMomZ());   // column Id = 15
    analysisManager->CreateNtupleDColumn("VecTrackMomMag",fEventAction->GetVecTrackMomMag()); // column Id = 16
    
    analysisManager->CreateNtupleIColumn("VecTPCUp_TrackID",fEventAction->GetVecTPCUp_TrackID());     // column Id = 17
    analysisManager->CreateNtupleDColumn("VecTPCUp_TrackMomX",fEventAction->GetVecTPCUp_TrackMomZ()); // column Id = 18
    analysisManager->CreateNtupleDColumn("VecTPCUp_TrackMomY",fEventAction->GetVecTPCUp_TrackMomY()); // column Id = 19
    analysisManager->CreateNtupleDColumn("VecTPCUp_TrackMomZ",fEventAction->GetVecTPCUp_TrackMomZ()); // column Id = 20
    
    analysisManager->CreateNtupleIColumn("VecTPCDown_TrackID",fEventAction->GetVecTPCDown_TrackID());     // column Id = 21
    analysisManager->CreateNtupleDColumn("VecTPCDown_TrackMomX",fEventAction->GetVecTPCDown_TrackMomZ()); // column Id = 22
    analysisManager->CreateNtupleDColumn("VecTPCDown_TrackMomY",fEventAction->GetVecTPCDown_TrackMomY()); // column Id = 23
    analysisManager->CreateNtupleDColumn("VecTPCDown_TrackMomZ",fEventAction->GetVecTPCDown_TrackMomZ()); // column Id = 24

    analysisManager->CreateNtupleIColumn("VecTarget_TrackID",fEventAction->GetVecTarget_TrackID());     // column Id = 25
    analysisManager->CreateNtupleDColumn("VecTarget_TrackMomX",fEventAction->GetVecTarget_TrackMomZ()); // column Id = 26
    analysisManager->CreateNtupleDColumn("VecTarget_TrackMomY",fEventAction->GetVecTarget_TrackMomY()); // column Id = 27
    analysisManager->CreateNtupleDColumn("VecTarget_TrackMomZ",fEventAction->GetVecTarget_TrackMomZ()); // column Id = 28
    
    analysisManager->FinishNtuple();
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

  //inform the runManager to save random number seed  
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);      

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file     
  //
  G4String fileName = "EffStudy";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::EndOfRunAction(const G4Run*)
{ 
  // print histogram statistics         
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl;
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl;
    }
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
    
  }
  
  // save histograms & ntuple   
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



