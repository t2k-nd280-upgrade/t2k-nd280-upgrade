
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
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(i, e);
  if (i < 4)
    analysisManager->FillH1(16, e);
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
  	   << " (" << vtx->GetX0() / CLHEP::mm
  	   << ", " << vtx->GetY0() / CLHEP::mm
  	   << ", " << vtx->GetZ0() / CLHEP::mm 
  	   << ", " << vtx->GetT0() / CLHEP::second << ")"
  	   << G4endl;

    ExN02VertexInfo* vInfo 
      = dynamic_cast<ExN02VertexInfo*>(vtx->GetUserInformation());
    if (vInfo) {
      G4cout << "  Generator: " << vInfo->GetName() << G4endl;
      G4cout << "  Reaction:  " << vInfo->GetReaction() << G4endl;
      G4cout << "  Reac Num:  " << vInfo->GetReactionNum() << G4endl;
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

    if(vInfo){
      
      G4cout << "Incoming Vertex:" << vtxNumber << G4endl;
      
      const G4PrimaryVertex *incvtx = vInfo->GetInformationalVertex();
      
      G4cout << " w/ " << incvtx->GetNumberOfParticle() << " primaries" 
	     << " at " 
	// << " (" << G4BestUnit(vtx->GetX0(),"Length") 
	// << ", " << G4BestUnit(vtx->GetY0(),"Length") 
	// << ", " << G4BestUnit(vtx->GetZ0(),"Length") 
	// << ", " << G4BestUnit(vtx->GetT0(),"Time") << ")"
	     << " (" << incvtx->GetX0() / CLHEP::mm
	     << ", " << incvtx->GetY0() / CLHEP::mm
	     << ", " << incvtx->GetZ0() / CLHEP::mm 
	     << ", " << incvtx->GetT0() / CLHEP::second << ")"
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
	  //fVecVtx_NuPDG.push_back(prim->GetPDGcode());
	}
      }
    }
    // Get the Decay Vertex
    // Get the Production Vertex
    
  }
  
  



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
  

  

  /*

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


   for (G4int i=0;i<n_hit;i++){
     ExN02TrackerHit* hit = (*hHC1)[i];

     G4String detname     = hit->GetNameDet();
     G4ThreeVector trkmom = hit->GetMom();
     G4double trkcharge   = hit->GetCharge();
     G4int trkpdg         = hit->GetTrackPDG();
     G4double costh       = hit->GetTrackCosTheta();

   } // end loop over the hits
   
   // get analysis manager  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  // // fill histograms
  //analysisManager->FillH1(1, fEnergyAbsTPCup);

  //
  // Fill Ntuple: be careful to the column ID. Check with RunAction where the Ntuple is created                 
  //

  //analysisManager->FillNtupleIColumn(18,  fNTracks);
  //analysisManager->AddNtupleRow();

  */


  // Print per event (modulo n)
  //  
  G4int eventID = event->GetEventID();
  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;
  }

  // // Print out the informations from SteppingAction
  //std::cout << std::endl;
  //std::cout << "Print out the informations from SteppingAction:" << std::endl;
  //std::cout << std::endl;
  //std::cout << "# of Tracks = " << fNTracks << std::endl;
  //for(G4int itrk=0;itrk<fNTracks;itrk++){
  //std::cout << "Track ID: = " << fVecTrackID[itrk] 
  //	      << " - Track PDG: = " << fVecTrackPDG[itrk]   
  //   	      << " - Track E: = " << fVecTrackE[itrk]   
  //	      << " - Track Mom: = " << fVecTrackMomMag[itrk]   
  //	      << std::endl;
  //}
  //std::cout << std::endl;
  



  

  // get number of stored trajectories
  //
  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  
  // periodic printing
  //
  //if (eventID < 100 || eventID%100 == 0) {
  G4cout << ">>> Event " << event->GetEventID() << G4endl;
  G4cout << "    " << n_trajectories 
	 << " trajectories stored in this event." << G4endl;
  //}
  G4cout << G4endl;

  



  //
  // Fill the trajectories with the amount of energy deposited into
  // sensitive detectors (code from nd280mc adapted)
  // 
  // The code below is taken from nd280mc and adapted 
  // 
  
  //G4HCofThisEvent* HCofEvent = evt->GetHCofThisEvent(); // used in nd280mc
  //if (!HCofEvent) return;
  if (!hce) return;
  G4SDManager *sdM = G4SDManager::GetSDMpointer();
  G4HCtable *hcT = sdM->GetHCtable();

  for (int i=0; i<hcT->entries(); ++i) {                                          
                                            
    G4String SDname = hcT->GetSDname(i);
    G4String HCname = hcT->GetHCname(i);
    
    // The collection name is given by <detector name>/<Primitive Scorer name>.    

    G4cout << "Sensitive detector: " << SDname+"/"+HCname << G4endl;
    
    int HCId = sdM->GetCollectionID(SDname+"/"+HCname);
    G4VHitsCollection* g4Hits = hce->GetHC(HCId);
    if (g4Hits->GetSize()<1) continue;
    
    G4int n_hit = g4Hits->GetSize();
    std::cout << "# of hits = " << n_hit << std::endl;
    
    for (unsigned int h=0; h<g4Hits->GetSize(); ++h) {                 
      
      //ND280HitSegment* g4Hit
      //= dynamic_cast<ND280HitSegment*>(g4Hits->GetHit(h)); // used in nd280mc      
      ExN02TrackerHit* g4Hit = dynamic_cast<ExN02TrackerHit*>(g4Hits->GetHit(h));
      
      double energy = g4Hit->GetEnergyDeposit(); // used in nd280mc
      //double energy = g4Hit->GetEdep();

      int trackId = g4Hit->GetContributors().front(); // used in nd280mc
      //int trackId = g4Hit->GetTrackID();
      
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
      
      
      // // Print results
      //if(g4Hit->GetParentID()==0){	
      //g4Hit->Print();
      //}

      
      traj->AddSDEnergyDeposit(energy);      
      //traj->AddSDLength(g4Hit->GetLength()); // used in nd280mc
      traj->AddSDLength(g4Hit->GetStepLength());
      

   

      //
      // THE FOLLOWING PIECE OF CODE OF nd280mc IS NOT IMPLEMENTED YET
      //
      
      // for (int loopCount = 0; ; ++loopCount) {
      //  	int parentId = traj->GetParentID();
      // 	if (!parentId) break;
      //  	g4Traj = ND280TrajectoryMap::Get(parentId);
      //  	if (!g4Traj) {
      // 	  G4ExceptionDescription msg;
      // 	  msg << "Missing parentId " << G4endl; 
      // 	  G4Exception("ExN02EventAction::EndOfEventAction()",
      // 		      "ExN02Code001", FatalException, msg);
      // 	  break;
      // 	}
      //  	traj = dynamic_cast<ND280Trajectory*>(g4Traj);
      //  	if (!traj) {
      // 	  G4ExceptionDescription msg;
      // 	  msg << "Not a ND280Trajectory " << G4endl; 
      // 	  G4Exception("ExN02EventAction::EndOfEventAction()",
      // 		      "ExN02Code001", FatalException, msg);
      // 	  break;
      // 	}
      //  	traj->AddSDDaughterEnergyDeposit(energy);
      //  	if (loopCount>9999) {
      // 	  G4ExceptionDescription msg;
      // 	  msg << "Infinite loop for trajectory id. Infinite loop trap " << G4endl; 
      // 	  G4Exception("ExN02EventAction::EndOfEventAction()",
      // 		      "ExN02Code001", FatalException, msg);
      // 	  break;
      //    }
      // }
      
    } // end loop over hits    
  } // end loop over hit containers



  // Store the seed
  //G4RunManager::GetRunManager()->rndmSaveThisEvent();


  G4cout << "End of EndOfEvent" << G4endl;

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*
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

}
*/
