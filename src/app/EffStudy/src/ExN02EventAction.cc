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


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02EventAction::ExN02EventAction()
  : G4UserEventAction(),
    fEnergyAbsTPCup(0.),
    fEnergyAbsTPCdown(0.),
    fEnergyAbsTarget(0.),
    fTrackLAbsTPCup(0.),
    fTrackLAbsTPCdown(0.),
    fTrackLAbsTarget(0.),
    fNTracks(0),
    fHHC1ID(-1),  
    fVecTrackID(),
    fVecTrackPDG(),
    fVecTrackE(),
    fVecTrackMomX(),
    fVecTrackMomY(),
    fVecTrackMomZ(),
    fVecTrackMomMag(),
    
    fTPCUp_NTracks(0),
    fVecTPCUp_TrackID(),
    fVecTPCUp_TrackMomX(),
    fVecTPCUp_TrackMomY(),
    fVecTPCUp_TrackMomZ(),

    fTPCDown_NTracks(0),
    fVecTPCDown_TrackID(),
    fVecTPCDown_TrackMomX(),
    fVecTPCDown_TrackMomY(),
    fVecTPCDown_TrackMomZ(),

    fTarget_NTracks(0),
    fVecTarget_TrackID(),
    fVecTarget_TrackMomX(),
    fVecTarget_TrackMomY(),
    fVecTarget_TrackMomZ()

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
  fEnergyAbsTPCup   = 0.;
  fEnergyAbsTPCdown = 0.;
  fEnergyAbsTarget  = 0.;
  fTrackLAbsTPCup   = 0.;
  fTrackLAbsTPCdown = 0.;
  fTrackLAbsTarget  = 0.;
  fNTracks          = 0; // It's a counter. It must be 0 at the begin of the event.

  fTPCUp_NTracks = 0;
  fTPCDown_NTracks = 0;
  fTarget_NTracks = 0;

  // vector initialisation per event
  fVecTrackID.clear(); // clear the vector
  fVecTrackPDG.clear(); // clear the vector
  fVecTrackE.clear(); // clear the vector
  fVecTrackMomX.clear(); // clear the vector
  fVecTrackMomY.clear(); // clear the vector
  fVecTrackMomZ.clear(); // clear the vector
  fVecTrackMomMag.clear(); // clear the vector

  fVecTPCUp_TrackID.clear(); 
  fVecTPCUp_TrackMomX.clear(); 
  fVecTPCUp_TrackMomY.clear(); 
  fVecTPCUp_TrackMomZ.clear(); 

  fVecTPCDown_TrackID.clear(); 
  fVecTPCDown_TrackMomX.clear(); 
  fVecTPCDown_TrackMomY.clear(); 
  fVecTPCDown_TrackMomZ.clear(); 

  fVecTarget_TrackID.clear(); 
  fVecTarget_TrackMomX.clear(); 
  fVecTarget_TrackMomY.clear(); 
  fVecTarget_TrackMomZ.clear(); 
  
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

   G4int currtrkid_TPCUp = kBadNum;
   G4int prevtrkid_TPCUp = kBadNum;
   G4int currtrkid_TPCDown = kBadNum;
   G4int prevtrkid_TPCDown = kBadNum;
   G4int currtrkid_Target = kBadNum;
   G4int prevtrkid_Target = kBadNum;
   
   for (G4int i=0;i<n_hit;i++){
     ExN02TrackerHit* hit = (*hHC1)[i];
     
     G4String detname = hit->GetNameDet();
     //std::cout << detname << std::endl;
     
     if(detname=="TPCUp"){     
	 
       // Get Track initial informations (first step of the track)
       currtrkid_TPCUp = hit->GetTrackID();
       G4ThreeVector trkmom = hit->GetMom();

       if(prevtrkid_TPCUp==kBadNum){
	 fVecTPCUp_TrackID.push_back(currtrkid_TPCUp);
	 
	 fVecTPCUp_TrackMomX.push_back(trkmom.x());
	 fVecTPCUp_TrackMomY.push_back(trkmom.y());
	 fVecTPCUp_TrackMomZ.push_back(trkmom.z());
 	 
	 prevtrkid_TPCUp = currtrkid_TPCUp;
	 fTPCUp_NTracks++;
	 //std::cout << "First:" << prevtrkid_TPCUp << " - " << currtrkid_TPCUp << std::endl;
       }
       else if(currtrkid_TPCUp!=prevtrkid_TPCUp && prevtrkid_TPCUp!=kBadNum){
	 fVecTPCUp_TrackID.push_back(currtrkid_TPCUp);
	 
	 fVecTPCUp_TrackMomX.push_back((G4double)trkmom.x());
	 fVecTPCUp_TrackMomY.push_back((G4double)trkmom.y());
	 fVecTPCUp_TrackMomZ.push_back((G4double)trkmom.z());

	 prevtrkid_TPCUp = currtrkid_TPCUp;
	 fTPCUp_NTracks++;
	 //std::cout << prevtrkid_TPCUp << " - " << currtrkid_TPCUp << std::endl;
       }       
     }

     else if(detname=="TPCDown"){     
	 
       // Get Track initial informations (first step of the track)
       currtrkid_TPCDown = hit->GetTrackID();
       G4ThreeVector trkmom = hit->GetMom();

       if(prevtrkid_TPCDown==kBadNum){
	 fVecTPCDown_TrackID.push_back(currtrkid_TPCDown);
	 
	 fVecTPCDown_TrackMomX.push_back(trkmom.x());
	 fVecTPCDown_TrackMomY.push_back(trkmom.y());
	 fVecTPCDown_TrackMomZ.push_back(trkmom.z());
 	 
	 prevtrkid_TPCDown = currtrkid_TPCDown;
	 fTPCDown_NTracks++;
	 //std::cout << "First:" << prevtrkid_TPCDown << " - " << currtrkid_TPCDown << std::endl;
       }
       else if(currtrkid_TPCDown!=prevtrkid_TPCDown && prevtrkid_TPCDown!=kBadNum){
	 fVecTPCDown_TrackID.push_back(currtrkid_TPCDown);
	 
	 fVecTPCDown_TrackMomX.push_back((G4double)trkmom.x());
	 fVecTPCDown_TrackMomY.push_back((G4double)trkmom.y());
	 fVecTPCDown_TrackMomZ.push_back((G4double)trkmom.z());

	 prevtrkid_TPCDown = currtrkid_TPCDown;
	 fTPCDown_NTracks++;
	 //std::cout << prevtrkid_TPCDown << " - " << currtrkid_TPCDown << std::endl;
       }       
     }

     else if(detname=="Target"){     
	 
       // Get Track initial informations (first step of the track)
       currtrkid_Target = hit->GetTrackID();
       G4ThreeVector trkmom = hit->GetMom();

       if(prevtrkid_Target==kBadNum){
	 fVecTarget_TrackID.push_back(currtrkid_Target);
	 
	 fVecTarget_TrackMomX.push_back(trkmom.x());
	 fVecTarget_TrackMomY.push_back(trkmom.y());
	 fVecTarget_TrackMomZ.push_back(trkmom.z());
 	 
	 prevtrkid_Target = currtrkid_Target;
	 fTarget_NTracks++;
	 //std::cout << "First:" << prevtrkid_Target << " - " << currtrkid_Target << std::endl;
       }
       else if(currtrkid_Target!=prevtrkid_Target && prevtrkid_Target!=kBadNum){
	 fVecTarget_TrackID.push_back(currtrkid_Target);
	 
	 fVecTarget_TrackMomX.push_back((G4double)trkmom.x());
	 fVecTarget_TrackMomY.push_back((G4double)trkmom.y());
	 fVecTarget_TrackMomZ.push_back((G4double)trkmom.z());

	 prevtrkid_Target = currtrkid_Target;
	 fTarget_NTracks++;
	 //std::cout << prevtrkid_Target << " - " << currtrkid_Target << std::endl;
       }       
     }


     
   }
   
  

  // get analysis manager  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // // fill histograms
  analysisManager->FillH1(1, fEnergyAbsTPCup);
  // analysisManager->FillH1(2, fEnergyAbsTPCdown);
  // analysisManager->FillH1(3, fEnergyAbsTarget);
  // analysisManager->FillH1(4, fTrackLAbsTPCup);
  // analysisManager->FillH1(5, fTrackLAbsTPCdown);
  // analysisManager->FillH1(6, fTrackLAbsTarget);

  //
  // Fill Ntuple: be careful to the column ID. Check with RunAction where the Ntuple is created                 
  //

  analysisManager->FillNtupleDColumn(0,  fEnergyAbsTPCup);
  analysisManager->FillNtupleDColumn(1,  fEnergyAbsTPCdown);
  analysisManager->FillNtupleDColumn(2,  fEnergyAbsTarget);
  analysisManager->FillNtupleDColumn(3,  fTrackLAbsTPCup);
  analysisManager->FillNtupleDColumn(4,  fTrackLAbsTPCdown);
  analysisManager->FillNtupleDColumn(5,  fTrackLAbsTarget); 
  analysisManager->FillNtupleIColumn(6,  fNTracks);

  analysisManager->FillNtupleIColumn(7,  fTPCUp_NTracks);
  analysisManager->FillNtupleIColumn(8,  fTPCDown_NTracks);
  analysisManager->FillNtupleIColumn(9,  fTarget_NTracks);

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

  // Print out the informations from SteppingAction
  std::cout << std::endl;
  std::cout << "Print out the informations from SteppingAction:" << std::endl;
  std::cout << std::endl;
  std::cout << "# of Tracks = " << fNTracks << std::endl;
  for(int itrk=0;itrk<fNTracks;itrk++){
    std::cout << "Track ID: = " << fVecTrackID[itrk] 
	      << " - Track PDG: = " << fVecTrackPDG[itrk]   
	      << " - Track E: = " << fVecTrackE[itrk]   
	      << " - Track Mom: = " << fVecTrackMomMag[itrk]   
	      << std::endl;
  }
  std::cout << std::endl;
  
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

void ExN02EventAction::SetTrack(G4Track *track) { // Used in SteppingAction

  // Take track informations from its first step
  G4int newtrkid      = track->GetTrackID();
  G4int newtrkpdg     = track->GetDefinition()->GetPDGEncoding();

  // Get the total initial energy of the track
  G4double newtrkmass        = track->GetDefinition()->GetPDGMass();
  G4double newtrkEkinVtx     = track->GetVertexKineticEnergy();  
  G4double newtrkEtotVtx     = newtrkEkinVtx + newtrkmass;             
  
  // Get the total initial momentum
  G4ThreeVector newVecMomVtx = track->GetMomentum(); //Direction(); 
  G4double newtrkMomX   = newVecMomVtx.x();
  G4double newtrkMomY   = newVecMomVtx.y();
  G4double newtrkMomZ   = newVecMomVtx.z();
  G4double newtrkMomMag = newVecMomVtx.mag();

  G4int vecel = fNTracks-1;

  //G4String detname = track->GetVolume()->GetName();
  //std::vector < G4String > vecstr;  
  //if(detname!="TPCUp") return;

  if(fNTracks==0){
    fVecTrackID .push_back(newtrkid);
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
    fVecTrackPDG.push_back(newtrkpdg);
    fVecTrackE  .push_back(newtrkEtotVtx);

    fVecTrackMomX.push_back(newtrkMomX);
    fVecTrackMomY.push_back(newtrkMomY);
    fVecTrackMomZ.push_back(newtrkMomZ);
    fVecTrackMomMag.push_back(newtrkMomMag);

    fNTracks++;
  }
}
