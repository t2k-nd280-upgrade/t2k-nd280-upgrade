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

#include "G4Event.hh"
//#include "G4EventManager.hh"

#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

#include "ExN02RunAction.hh"
#include "ExN02Analysis.hh"

#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02EventAction::ExN02EventAction()
  : G4UserEventAction(),
    fEnergyAbsTPCup(0.),
    fEnergyAbsTPCdown(0.),
    fEnergyAbsTarget(0.),
    fTrackLAbsTPCup(0.),
    fTrackLAbsTPCdown(0.),
    fTrackLAbsTarget(0.),
    fPrimInTPCup(0.),
    fPrimInTPCdown(0.),
    fPrimInTarget(0.),
    fPDGPrimTPCup(-999.),
    fPDGPrimTPCdown(-999.),
    fPDGPrimTarget(-999.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02EventAction::~ExN02EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void ExN02EventAction::BeginOfEventAction(const G4Event*)
{
  // initialisation per event
  fEnergyAbsTPCup   = 0.;
  fEnergyAbsTPCdown = 0.;
  fEnergyAbsTarget  = 0.;
  fTrackLAbsTPCup   = 0.;
  fTrackLAbsTPCdown = 0.;
  fTrackLAbsTarget  = 0.;
  fTruePrimMom   = 0.;
  fTruePrimE     = 0.;  
  fPrimInTPCup   = -999.;   
  fPrimInTPCdown = -999.; 
  fPrimInTarget  = -999.;
  fPDGPrimTPCup = -999.;
  fPDGPrimTPCdown  = -999.;
  fPDGPrimTarget = -999.;
  fPDGPrim = -999.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void ExN02EventAction::EndOfEventAction(const G4Event* event)
{
  // Accumulate statistics
  //

  // get analysis manager  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // fill histograms
  analysisManager->FillH1(1, fEnergyAbsTPCup);
  analysisManager->FillH1(2, fEnergyAbsTPCdown);
  analysisManager->FillH1(3, fEnergyAbsTarget);
  analysisManager->FillH1(4, fTrackLAbsTPCup);
  analysisManager->FillH1(5, fTrackLAbsTPCdown);
  analysisManager->FillH1(6, fTrackLAbsTarget);
  analysisManager->FillH1(7, fPrimInTPCup);
  analysisManager->FillH1(8, fPrimInTPCdown);
  analysisManager->FillH1(9, fPrimInTarget);

  // fill ntuple                 
  analysisManager->FillNtupleDColumn(0, fEnergyAbsTPCup);
  analysisManager->FillNtupleDColumn(1, fEnergyAbsTPCdown);
  analysisManager->FillNtupleDColumn(2, fEnergyAbsTarget);
  analysisManager->FillNtupleDColumn(3, fTrackLAbsTPCup);
  analysisManager->FillNtupleDColumn(4, fTrackLAbsTPCdown);
  analysisManager->FillNtupleDColumn(5, fTrackLAbsTarget);
  analysisManager->FillNtupleDColumn(6, fPrimInTPCup);
  analysisManager->FillNtupleDColumn(7, fPrimInTPCdown);
  analysisManager->FillNtupleDColumn(8, fPrimInTarget);
  analysisManager->FillNtupleDColumn(9, fPDGPrimTPCup);
  analysisManager->FillNtupleDColumn(10, fPDGPrimTPCdown);
  analysisManager->FillNtupleDColumn(11, fPDGPrimTarget);
  analysisManager->FillNtupleDColumn(12, fPDGPrim);
  analysisManager->AddNtupleRow();


  // Print per event (modulo n)
  //  
  G4int eventID = event->GetEventID();
  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;

    G4cout
      << "   Absorber TPC up: total energy: " << std::setw(7)
      << G4BestUnit(fEnergyAbsTPCup,"Energy")
      << "       total track length: " << std::setw(7)
      << G4BestUnit(fTrackLAbsTPCup,"Length")
      << G4endl 
      << "   Absorber TPC down: total energy: " << std::setw(7)
      << G4BestUnit(fEnergyAbsTPCup,"Energy")
      << "       total track length: " << std::setw(7)
      << G4BestUnit(fTrackLAbsTPCup,"Length")
      << G4endl
      << "   Absorber Target: total energy: " << std::setw(7)
      << G4BestUnit(fEnergyAbsTarget,"Energy")
      << "       total track length: " << std::setw(7)
      << G4BestUnit(fTrackLAbsTarget,"Length")
      << G4endl;
  }
  


  //G4int event_id = evt->GetEventID();
  
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
