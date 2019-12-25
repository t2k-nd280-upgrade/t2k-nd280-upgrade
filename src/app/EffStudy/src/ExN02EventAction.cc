
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
#include "ND280TrajectoryMap.hh"
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
    fHHC1ID(-1)  
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
  // Initialize the hit collections
  if (fHHC1ID==-1) {
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    fHHC1ID = sdManager->GetCollectionID("trackerCollection");
  }
  
}

void ExN02EventAction::FH(G4int i, G4double e) {
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(i, e);
  if (i < 4)
    analysisManager->FillH1(16, e);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void ExN02EventAction::EndOfEventAction(const G4Event* event)
{

    ///////////////////////////////////////////////////////
    //                                                   //
    //   Access the primary and informational vertices   //
    //                                                   //
    ///////////////////////////////////////////////////////

    // Get the Primary Vertex
    G4int vtxNumber=0;
    for (G4PrimaryVertex* vtx = event->GetPrimaryVertex();vtx;vtx = vtx->GetNext()) {
        ++vtxNumber;
        G4cout << "[ExN02EventAction] Primary vertex: " << vtxNumber  
               << "\tw/ " << vtx->GetNumberOfParticle() << " primaries" 
               << " at " 
               << " (" << vtx->GetX0() / CLHEP::mm
               << ", " << vtx->GetY0() / CLHEP::mm
               << ", " << vtx->GetZ0() / CLHEP::mm 
               << ", " << vtx->GetT0() / CLHEP::second << ")"
               << G4endl;
        ExN02VertexInfo* vInfo = dynamic_cast<ExN02VertexInfo*>(vtx->GetUserInformation());
        
        if (vInfo) {
            G4cout << "[ExN02EventAction]    Generator : " << vInfo->GetName()        << G4endl;
            G4cout << "[ExN02EventAction]    Reaction  : " << vInfo->GetReaction()    << G4endl;
            G4cout << "[ExN02EventAction]    Reac Num  : " << vInfo->GetReactionNum() << G4endl;
            G4cout << "[ExN02EventAction]    Weight    : " << vInfo->GetWeight()      << G4endl;
            G4cout << "[ExN02EventAction]    Prob      : " << vInfo->GetProbability() << G4endl;
        }
        for (G4int p=0; p<vtx->GetNumberOfParticle(); ++p) {
            G4PrimaryParticle*    prim    = vtx->GetPrimary(p);
            G4ParticleDefinition* partDef = prim->GetG4code();
            G4ThreeVector         dir     = prim->GetMomentum().unit();
            G4cout << "[ExN02EventAction]\t— " << partDef->GetParticleName() 
                   << " (PDG:"
                   << prim->GetPDGcode()
                   << ")\tw/ "
                   << G4BestUnit(prim->GetMomentum().mag(),"Energy")
                   << "  Direction: (" << dir.x()
                   << ", " << dir.y()
                   << ", " << dir.z() << ")"
                   << G4endl;
        }
        // Get the Incoming Vertex
        if(vInfo){
            const G4PrimaryVertex *incvtx = vInfo->GetInformationalVertex();
            G4cout << "[ExN02EventAction] Incoming Vertex :" << vtxNumber;
            G4cout << "\tw/ " << incvtx->GetNumberOfParticle() << " primaries" 
                   << " at " 
                   << " (" << incvtx->GetX0() / CLHEP::mm
                   << ", " << incvtx->GetY0() / CLHEP::mm
                   << ", " << incvtx->GetZ0() / CLHEP::mm 
                   << ", " << incvtx->GetT0() / CLHEP::second 
                   << ")"  << G4endl;
            
            for (G4int nu=0; nu<incvtx->GetNumberOfParticle(); ++nu) {
                G4PrimaryParticle* prim = incvtx->GetPrimary(nu);
                G4ParticleDefinition* partDef = prim->GetG4code();
                G4ThreeVector dir = prim->GetMomentum().unit();
                G4cout << "[ExN02EventAction]\t— " << partDef->GetParticleName() 
                       << " (PDG:"
                       << prim->GetPDGcode()
                       << ")\tw/ "
                       << G4BestUnit(prim->GetMomentum().mag(),"Energy")
                       << "  Direction: (" << dir.x()
                       << ", " << dir.y()
                       << ", " << dir.z() << ")"
                       << G4endl;	
                G4int pdg = prim->GetPDGcode();
            }
        }
    }

    ///////////////////////////////////////////////////////
    //                                                   //
    // Access hit collections and handle the stored hits //
    //                                                   //
    ///////////////////////////////////////////////////////
    
    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    if (!hce){
        G4ExceptionDescription msg;
        msg << "No hits collection of this event found." << G4endl; 
        G4Exception("ExN02EventAction::EndOfEventAction()",
                    "ExN02Code001", JustWarning, msg);
        return;
    }   

    // Print per event (modulo n)
    G4int eventID = event->GetEventID();
    G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();

    // get number of stored trajectories
    G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
    G4int n_trajectories = 0;
    if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
    
    // periodic printing
    G4cout << "[ExN02EventAction] >>> Event " << event->GetEventID() << G4endl;
    G4cout << "[ExN02EventAction] " << n_trajectories 
           << " trajectories stored in this event." << G4endl;



    ////////////////////////////////////////////////////////////////////
    //                                                                //
    // Fill the trajectories with the amount of energy deposited into //
    // sensitive detectors (code from nd280mc adapted)                //
    //                                                                //
    // The code below is taken from nd280mc and adapted               //
    //                                                                //
    ////////////////////////////////////////////////////////////////////
    
    if (!hce) return;
    G4SDManager *sdM = G4SDManager::GetSDMpointer();
    G4HCtable *hcT = sdM->GetHCtable();

    for (int i=0; i<hcT->entries(); ++i) {                                          
        
        G4String SDname = hcT->GetSDname(i);
        G4String HCname = hcT->GetHCname(i);
        // The collection name is given by <detector name>/<Primitive Scorer name>.    
        G4cout << "[ExN02EventAction] Sensitive detector: " << SDname+"/"+HCname << G4endl;
        
        int HCId = sdM->GetCollectionID(SDname+"/"+HCname);
        G4VHitsCollection* g4Hits = hce->GetHC(HCId);
        if (g4Hits->GetSize()<1) continue;
        
        G4int n_hit = g4Hits->GetSize();
        std::cout << "[ExN02EventAction] # of hits = " << n_hit << std::endl;
        
        for (unsigned int h=0; h<g4Hits->GetSize(); ++h) {                 
          
            ExN02TrackerHit* g4Hit = dynamic_cast<ExN02TrackerHit*>(g4Hits->GetHit(h));
            double energy  = g4Hit->GetEnergyDeposit();        // used in nd280mc
            int    trackId = g4Hit->GetContributors().front(); // used in nd280mc
            
            G4VTrajectory* g4Traj = ND280TrajectoryMap::Get(trackId);
            if (!g4Traj) {
                G4ExceptionDescription msg;
                msg << "Missing trackId: " << trackId << G4endl; 
                G4Exception("ExN02EventAction::EndOfEventAction()",
                            "ExN02Code001", FatalException, msg);
                continue;
            }
            ND280Trajectory* traj = dynamic_cast<ND280Trajectory*>(g4Traj);
            if (!traj) {
                G4ExceptionDescription msg;
                msg << "Not a ND280Trajectory " << G4endl; 
                G4Exception("ExN02EventAction::EndOfEventAction()",
                	        "ExN02Code001", FatalException, msg);
                continue;
            }
            traj->AddSDEnergyDeposit(energy);      
            traj->AddSDLength(g4Hit->GetStepLength());
        } // end loop over hits    
    } // end loop over hit containers
    G4cout << "[ExN02EventAction] End of EndOfEvent" << G4endl;
}


