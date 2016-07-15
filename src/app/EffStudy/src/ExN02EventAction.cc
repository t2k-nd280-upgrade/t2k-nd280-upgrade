
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
    fHHC1ID(-1),  
    fEnergyAbsTPCup(0.),
    fEnergyAbsTPCdown(0.),
    fEnergyAbsTarget(0.),
    fEnergyAbsTot(0.),
    fTrackLAbsTPCup(0.),
    fTrackLAbsTPCdown(0.),
    fTrackLAbsTarget(0.),
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
    
    fTPCUp_NTracks(0),
    fVecTPCUp_TrackID(),
    fVecTPCUp_ParentID(),
    fVecTPCUp_TrackMomX(),
    fVecTPCUp_TrackMomY(),
    fVecTPCUp_TrackMomZ(),
    fVecTPCUp_TrackLength(),
    fVecTPCUp_TrackDeltaLyz(),
    fVecTPCUp_TrackEdep(),
    fVecTPCUp_TrackCharge(),
    fVecTPCUp_TrackPDG(),
    fVecTPCUp_TrackStepFirstX(),
    fVecTPCUp_TrackStepFirstY(),
    fVecTPCUp_TrackStepFirstZ(),
    fVecTPCUp_TrackStepLastX(),
    fVecTPCUp_TrackStepLastY(),
    fVecTPCUp_TrackStepLastZ(),
    fVecTPCUp_TrackCosTheta(),

    fTPCDown_NTracks(0),
    fVecTPCDown_TrackID(),
    fVecTPCDown_ParentID(),
    fVecTPCDown_TrackMomX(),
    fVecTPCDown_TrackMomY(),
    fVecTPCDown_TrackMomZ(),
    fVecTPCDown_TrackLength(),
    fVecTPCDown_TrackDeltaLyz(),
    fVecTPCDown_TrackEdep(),
    fVecTPCDown_TrackCharge(),
    fVecTPCDown_TrackPDG(),
    fVecTPCDown_TrackStepFirstX(),
    fVecTPCDown_TrackStepFirstY(),
    fVecTPCDown_TrackStepFirstZ(),
    fVecTPCDown_TrackStepLastX(),
    fVecTPCDown_TrackStepLastY(),
    fVecTPCDown_TrackStepLastZ(),
    fVecTPCDown_TrackCosTheta(),

    fTarget_NTracks(0),
    fVecTarget_TrackID(),
    fVecTarget_ParentID(),
    fVecTarget_TrackMomX(),
    fVecTarget_TrackMomY(),
    fVecTarget_TrackMomZ(),
    fVecTarget_TrackLength(),
    fVecTarget_TrackDeltaLyz(),
    fVecTarget_TrackEdep(),
    fVecTarget_TrackCharge(),
    fVecTarget_TrackPDG(),
    fVecTarget_TrackStepFirstX(),
    fVecTarget_TrackStepFirstY(),
    fVecTarget_TrackStepFirstZ(),
    fVecTarget_TrackStepLastX(),
    fVecTarget_TrackStepLastY(),
    fVecTarget_TrackStepLastZ(),
    fVecTarget_TrackCosTheta(),

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
  fEnergyAbsTPCup   = 0.;
  fEnergyAbsTPCdown = 0.;
  fEnergyAbsTarget  = 0.;
  fTrackLAbsTPCup   = 0.;
  fTrackLAbsTPCdown = 0.;
  fTrackLAbsTarget  = 0.;
  fNTracks          = 0; // It's a counter. It must be 0 at beginning of event.

  fTPCUp_NTracks = 0;
  fTPCDown_NTracks = 0;
  fTarget_NTracks = 0;

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

  fVecTPCUp_TrackID.clear(); 
  fVecTPCUp_ParentID.clear(); 
  fVecTPCUp_TrackMomX.clear(); 
  fVecTPCUp_TrackMomY.clear(); 
  fVecTPCUp_TrackMomZ.clear(); 
  fVecTPCUp_TrackLength.clear(); 
  fVecTPCUp_TrackDeltaLyz.clear(); 
  fVecTPCUp_TrackEdep.clear(); 
  fVecTPCUp_TrackCharge.clear(); 
  fVecTPCUp_TrackPDG.clear();
  fVecTPCUp_TrackStepFirstX.clear();
  fVecTPCUp_TrackStepFirstY.clear();
  fVecTPCUp_TrackStepFirstZ.clear();
  fVecTPCUp_TrackStepLastX.clear();
  fVecTPCUp_TrackStepLastY.clear();
  fVecTPCUp_TrackStepLastZ.clear();
  fVecTPCUp_TrackCosTheta.clear();

  fVecTPCDown_TrackID.clear(); 
  fVecTPCDown_ParentID.clear(); 
  fVecTPCDown_TrackMomX.clear(); 
  fVecTPCDown_TrackMomY.clear(); 
  fVecTPCDown_TrackMomZ.clear(); 
  fVecTPCDown_TrackLength.clear(); 
  fVecTPCDown_TrackDeltaLyz.clear(); 
  fVecTPCDown_TrackEdep.clear(); 
  fVecTPCDown_TrackCharge.clear(); 
  fVecTPCDown_TrackPDG.clear();
  fVecTPCDown_TrackStepFirstX.clear();
  fVecTPCDown_TrackStepFirstY.clear();
  fVecTPCDown_TrackStepFirstZ.clear();
  fVecTPCDown_TrackStepLastX.clear();
  fVecTPCDown_TrackStepLastY.clear();
  fVecTPCDown_TrackStepLastZ.clear();
  fVecTPCDown_TrackCosTheta.clear();

  fVecTarget_TrackID.clear(); 
  fVecTarget_ParentID.clear(); 
  fVecTarget_TrackMomX.clear(); 
  fVecTarget_TrackMomY.clear(); 
  fVecTarget_TrackMomZ.clear();
  fVecTarget_TrackLength.clear(); 
  fVecTarget_TrackDeltaLyz.clear(); 
  fVecTarget_TrackEdep.clear(); 
  fVecTarget_TrackCharge.clear(); 
  fVecTarget_TrackPDG.clear();
  fVecTarget_TrackStepFirstX.clear();
  fVecTarget_TrackStepFirstY.clear();
  fVecTarget_TrackStepFirstZ.clear();
  fVecTarget_TrackStepLastX.clear();
  fVecTarget_TrackStepLastY.clear();
  fVecTarget_TrackStepLastZ.clear();
  fVecTarget_TrackCosTheta.clear();

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

   G4int currtrkid_TPCUp = kBadNum;
   G4int prevtrkid_TPCUp = kBadNum;
   G4int aftertrkid_TPCUp = kBadNum;
   G4int currtrkid_TPCDown = kBadNum;
   G4int prevtrkid_TPCDown = kBadNum;
   G4int aftertrkid_TPCDown = kBadNum;
   G4int currtrkid_Target = kBadNum;
   G4int prevtrkid_Target = kBadNum;
   G4int aftertrkid_Target = kBadNum;

   G4int currparentid_TPCUp = kBadNum;
   G4int currparentid_TPCDown = kBadNum;
   G4int currparentid_Target = kBadNum;

   for (G4int i=0;i<n_hit;i++){
     ExN02TrackerHit* hit = (*hHC1)[i];

     G4String detname = hit->GetNameDet();
     //std::cout << detname << std::endl;
     
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
     
     // if(detname=="Target"){
     // G4cout << "i = " << i 
     // 	    << " - current ID = " << hit->GetTrackID() 
     // 	    << " - after ID = " << hit_after->GetTrackID() 
     // 	    << " --> detname = " << detname << " -->  detname_after = " << detname_after << G4endl;
     //}
     //continue;


     if(detname=="TPCUp"){     
	 
       currtrkid_TPCUp = hit->GetTrackID(); // current track
       currparentid_TPCUp = hit->GetParentID(); // current track
       
       //G4cout << "currtrkid_TPCUp = " << currtrkid_TPCUp << G4endl;
 
       if(hit_after!=0) aftertrkid_TPCUp = hit_after->GetTrackID(); // following track 
       else aftertrkid_TPCUp = kBadNum;
       
       G4ThreeVector trkmom = hit->GetMom();
       G4double trkcharge = hit->GetCharge();
       G4int trkpdg = hit->GetTrackPDG();
       G4double costh = hit->GetTrackCosTheta();

       // Get Track initial informations (FIRST STEP of the track in detector)

       // It's the first track of event or change track or change detector
       if( (prevtrkid_TPCUp==kBadNum) || (currtrkid_TPCUp!=prevtrkid_TPCUp && prevtrkid_TPCUp!=kBadNum) || (detname_before!=detname) ){
	 
	 fVecTPCUp_TrackID.push_back(currtrkid_TPCUp);
	 fVecTPCUp_ParentID.push_back(currparentid_TPCUp);
	 
	 fVecTPCUp_TrackMomX.push_back((G4double)trkmom.x() / MeV);
	 fVecTPCUp_TrackMomY.push_back((G4double)trkmom.y() / MeV);
	 fVecTPCUp_TrackMomZ.push_back((G4double)trkmom.z() / MeV);
	 fVecTPCUp_TrackCharge.push_back(trkcharge / eplus);
	 fVecTPCUp_TrackPDG.push_back(trkpdg);

	 // get first step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTPCUp_TrackStepFirstX.push_back(x);	 	 
	 fVecTPCUp_TrackStepFirstY.push_back(y);	 	 
	 fVecTPCUp_TrackStepFirstZ.push_back(z);	 	 

	 // get track cos(theta) from first pre-step wrt nu direction (0,0,1)
	 fVecTPCUp_TrackCosTheta.push_back(costh);



	 // if(trkmom.mag() / MeV> 100){
	 //   double mom_oth = sqrt( trkmom.x()*trkmom.x() +  
	 // 			  trkmom.y()*trkmom.y() +
	 // 			  trkmom.z()*trkmom.z() );
	 //   G4cout << "TPCUp first step = "
	 // 	  << "trkid = " << currtrkid_TPCUp
	 // 	  << " pdg = " << trkpdg
	 // 	  << " mom = " << trkmom.mag() / MeV
	 // 	  << " mom_oth = " << mom_oth
	 // 	  << " x = " << x 
	 // 	  << " y = " << y 
	 // 	  << " z = " << z 
	 // 	  << G4endl; 
	 // }

	 prevtrkid_TPCUp = currtrkid_TPCUp;
	 fTPCUp_NTracks++;	
	 
	 //G4cout << "First:" << prevtrkid_TPCUp << " --> " << currtrkid_TPCUp 
	 //<< " - " << detname_before << " --> " << detname << G4endl;
	 
       } // if first step of the track

       
       // Get Track initial informations (LAST STEP of the track in detector)
       
       if( currtrkid_TPCUp!=aftertrkid_TPCUp || detname!=detname_after ){ 

	 // If aftertrkid_TPCUp=kBadNum, still ok as last step
	 // If first step = last step --> store it both as first and last step
	 
	 // get last step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTPCUp_TrackStepLastX.push_back(x);	 	 
	 fVecTPCUp_TrackStepLastY.push_back(y);	 	 
	 fVecTPCUp_TrackStepLastZ.push_back(z);	 	 
	 
	 //std::cout << "Last:" << currtrkid_TPCUp << " --> " << aftertrkid_TPCUp 
	 //<< " - " << detname << " --> " << detname_after << std::endl;     

       } // if last step of the track
       
     } // end TPCUp

     else if(detname=="TPCDown"){     
	 
       currtrkid_TPCDown = hit->GetTrackID();
       currparentid_TPCDown = hit->GetParentID();
       
       if(hit_after!=0) aftertrkid_TPCDown = hit_after->GetTrackID(); // following track 
       else aftertrkid_TPCDown = kBadNum;

       G4ThreeVector trkmom = hit->GetMom();
       G4double trkcharge = hit->GetCharge();
       G4int trkpdg = hit->GetTrackPDG();
       G4double costh = hit->GetTrackCosTheta();

       // Get Track initial informations (FIRST STEP of the track in detector)

       if( (prevtrkid_TPCDown==kBadNum) || (currtrkid_TPCDown!=prevtrkid_TPCDown && prevtrkid_TPCDown!=kBadNum) || (detname_before!=detname) ){
	 
	 fVecTPCDown_TrackID.push_back(currtrkid_TPCDown);
	 fVecTPCDown_ParentID.push_back(currparentid_TPCDown);
	  
	 fVecTPCDown_TrackMomX.push_back((G4double)trkmom.x() / MeV);
	 fVecTPCDown_TrackMomY.push_back((G4double)trkmom.y() / MeV);
	 fVecTPCDown_TrackMomZ.push_back((G4double)trkmom.z() / MeV);
	 fVecTPCDown_TrackCharge.push_back(trkcharge / eplus);
	 fVecTPCDown_TrackPDG.push_back(trkpdg);

	 // get first step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTPCDown_TrackStepFirstX.push_back(x);	 	 
	 fVecTPCDown_TrackStepFirstY.push_back(y);	 	 
	 fVecTPCDown_TrackStepFirstZ.push_back(z);	 	 

	 // get track cos(theta) from first pre-step wrt nu direction (0,0,1)
	 fVecTPCDown_TrackCosTheta.push_back(costh);
	 
	 // if(trkmom.mag() / MeV > 100 ){
	 //   double mom_oth = sqrt( trkmom.x()*trkmom.x() +  
	 // 			  trkmom.y()*trkmom.y() +
	 // 			  trkmom.z()*trkmom.z() );
	 //   G4cout << "TPCDown first step = "
	 // 	  << "trkid = " << currtrkid_TPCDown
	 // 	  << " pdg = " << trkpdg
	 // 	  << " mom = " << trkmom.mag() / MeV
	 // 	  << " mom_oth = " << mom_oth
	 // 	  << " x = " << x 
	 // 	  << " y = " << y 
	 // 	  << " z = " << z 
	 // 	  << G4endl;
	 // }

	 prevtrkid_TPCDown = currtrkid_TPCDown;
	 fTPCDown_NTracks++;
	 
	 //G4cout << "First:" << prevtrkid_TPCDown << " --> " << currtrkid_TPCDown 
	 //<< " - " << detname_before << " --> " << detname << G4endl;
	 
       } // if first step of the track 

       // Get Track initial informations (LAST STEP of the track in detector)
       
       if( currtrkid_TPCDown!=aftertrkid_TPCDown || detname!=detname_after ){ 

	 // If aftertrkid_TPCDown=kBadNum, still ok as last step
	 // If first step = last step --> store it both as first and last step
	 
	 // get last step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTPCDown_TrackStepLastX.push_back(x);	 	 
	 fVecTPCDown_TrackStepLastY.push_back(y);	 	 
	 fVecTPCDown_TrackStepLastZ.push_back(z);	 	 
	 
	 //std::cout << "Last:" << currtrkid_TPCDown << " --> " << aftertrkid_TPCDown
	 //<< " - " << detname << " --> " << detname_after << std::endl;     

       } // if last step of the track

     } // end TPCDown

     else if(detname=="Target"){     
	 
       currtrkid_Target = hit->GetTrackID();
       currparentid_Target = hit->GetParentID();
	      
       if(hit_after!=0) aftertrkid_Target = hit_after->GetTrackID(); // following track 
       else aftertrkid_Target = kBadNum;

       // G4cout << "currtrkid_Target = " << currtrkid_Target
       // 	      << " - prevtrkid_Target = " << prevtrkid_Target
       // 	      << " - aftertrkid_Target = " << aftertrkid_Target 
       // 	      << " - det = " << detname
       // 	      << " - det_before = " << detname_before
       // 	      << " - det_after = " << detname_after
       // 	      << G4endl;
       
       G4ThreeVector trkmom = hit->GetMom();
       G4double trkcharge = hit->GetCharge();
       G4int trkpdg = hit->GetTrackPDG();
       G4double costh = hit->GetTrackCosTheta();
       
       // Get Track initial informations (FIRST STEP of the track)
       
       if( prevtrkid_Target==kBadNum || (currtrkid_Target!=prevtrkid_Target && prevtrkid_Target!=kBadNum) || (detname_before!=detname) ){
	 
	 fVecTarget_TrackID.push_back(currtrkid_Target);
	 fVecTarget_ParentID.push_back(currparentid_Target);
	 
	 fVecTarget_TrackMomX.push_back((G4double)trkmom.x() / MeV);
	 fVecTarget_TrackMomY.push_back((G4double)trkmom.y() / MeV);
	 fVecTarget_TrackMomZ.push_back((G4double)trkmom.z() / MeV);
	 fVecTarget_TrackCharge.push_back(trkcharge / eplus);
 	 fVecTarget_TrackPDG.push_back(trkpdg);

	 // get first step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTarget_TrackStepFirstX.push_back(x);	 	 
	 fVecTarget_TrackStepFirstY.push_back(y);	 	 
	 fVecTarget_TrackStepFirstZ.push_back(z);	 	 

	 // get track cos(theta) from first pre-step wrt nu direction (0,0,1)
	 fVecTarget_TrackCosTheta.push_back(costh);
	 
	 prevtrkid_Target = currtrkid_Target;
	 fTarget_NTracks++;
	 // G4cout << "First:" << prevtrkid_Target << " --> " << currtrkid_Target 
	 //  	<< " - " << detname_before << " --> " << detname << G4endl;
	 // G4cout << " - pos: " << x << ", " << y << ", " << z << G4endl;
	 
	 // if(trkmom.mag() / MeV > 100){	   
	 //   double mom_oth = sqrt( trkmom.x()*trkmom.x() +  
	 // 			  trkmom.y()*trkmom.y() +
	 // 			  trkmom.z()*trkmom.z() );
	 //   G4cout << "Target first step = "
	 // 	  << "trkid = " << currtrkid_Target
	 // 	  << " pdg = " << trkpdg
	 // 	  << " mom = " << trkmom.mag() / MeV
	 // 	  << " mom = " << mom_oth 
	 // 	  << " x = " << x 
	 // 	  << " y = " << y 
	 // 	  << " z = " << z 
	 // 	  << G4endl;
	 // }
       } // if first step of the track

       // Get Track initial informations (LAST STEP of the track in detector)
       
       if( currtrkid_Target!=aftertrkid_Target || detname!=detname_after ){ 

	 // If aftertrkid_Target=kBadNum, still ok as last step
	 // If first step = last step --> store it both as first and last step
	 
	 // get last step position (mm)
	 double x = hit->GetPreStepPos().x() / mm;
	 double y = hit->GetPreStepPos().y() / mm;
	 double z = hit->GetPreStepPos().z() / mm;
	 fVecTarget_TrackStepLastX.push_back(x);	 	 
	 fVecTarget_TrackStepLastY.push_back(y);	 	 
	 fVecTarget_TrackStepLastZ.push_back(z);	 	 
	 
	 //G4cout << "Last:" << currtrkid_Target << " --> " << aftertrkid_Target 
	 //<< " - " << detname << " --> " << detname_after << G4endl;   
	 //G4cout << " - pos: " << x << ", " << y << ", " << z << G4endl;
  
       } // if last step of the track

     } // end Target
     
   } // end loop over the hits
   
   
   // Resize the track vectors to be filled 
      
   fVecTPCUp_TrackLength.resize(fTPCUp_NTracks);
   fVecTPCUp_TrackDeltaLyz.resize(fTPCUp_NTracks);
   fVecTPCUp_TrackEdep.resize(fTPCUp_NTracks);
   
   fVecTPCDown_TrackLength.resize(fTPCDown_NTracks);
   fVecTPCDown_TrackDeltaLyz.resize(fTPCDown_NTracks);
   fVecTPCDown_TrackEdep.resize(fTPCDown_NTracks);
   
   fVecTarget_TrackLength.resize(fTarget_NTracks);
   fVecTarget_TrackDeltaLyz.resize(fTarget_NTracks);
   fVecTarget_TrackEdep.resize(fTarget_NTracks);


   // Loop over all the steps of each track
   
   for (G4int i=0;i<n_hit;i++){ // keep same order as above!!!
     
     ExN02TrackerHit* hit = (*hHC1)[i];
     G4double steplength = hit->GetStepLength();
     G4double stepdeltalyz = hit->GetStepDeltaLyz();
     G4double stepedep = hit->GetEdep();
     //G4double tracklength = hit->GetTrackLength(); // track length at each step --> not needed
     G4int trkID = hit->GetTrackID();
     G4String detname = hit->GetNameDet();
     
     // Loop over the tracks in Target
     if(detname=="Target"){
       for(G4int itrk=0;itrk<fTarget_NTracks;itrk++){
	 // Keep the same order as above!!	 
	 G4int trkOrder = fVecTarget_TrackID.at(itrk);
	 if(trkOrder == trkID){	   
   	   fVecTarget_TrackLength[itrk] += steplength / mm;
   	   fVecTarget_TrackDeltaLyz[itrk] += stepdeltalyz / mm;
   	   fVecTarget_TrackEdep[itrk] += stepedep / MeV;
	 } 
       } // end loop over tracks Target
     } // Target

     // Loop over the tracks in TPC Up

     else if(detname=="TPCUp"){       
       for(G4int itrk=0;itrk<fTPCUp_NTracks;itrk++){
	 // Keep the same order as above!!	 
	 G4int trkOrder = fVecTPCUp_TrackID.at(itrk);
	 if(trkOrder == trkID){
	   fVecTPCUp_TrackLength[itrk] += steplength / mm;
	   fVecTPCUp_TrackDeltaLyz[itrk] += stepdeltalyz / mm;
	   fVecTPCUp_TrackEdep[itrk] += stepedep / MeV;	 
	 } 
       } // end loop over tracks Target
     } // TPC Up
     
     else if(detname=="TPCDown"){       
       for(G4int itrk=0;itrk<fTPCDown_NTracks;itrk++){
	 // Keep the same order as above!!	 
	 G4int trkOrder = fVecTPCDown_TrackID.at(itrk);
	 if(trkOrder == trkID){
	   fVecTPCDown_TrackLength[itrk] += steplength / mm;
	   fVecTPCDown_TrackDeltaLyz[itrk] += stepdeltalyz / mm;
	   fVecTPCDown_TrackEdep[itrk] += stepedep / MeV;	 
	 } 
       } // end loop over tracks Target
     } // TPC Up
   
   } // end loop over step     

   
   // G4cout << G4endl;
   // G4cout << "Target tracks: " << G4endl;
   // G4cout << G4endl;
   // for(G4int itrk=0;itrk<fTarget_NTracks;itrk++){
   //   G4cout << "Track ID: " << fVecTarget_TrackID.at(itrk) 
   // 	    << " - TrackLength = " << fVecTarget_TrackLength[itrk] / mm << " mm" 
   // 	    << " - TrackEdep = " << fVecTarget_TrackEdep[itrk] / MeV << " MeV" 
   // 	    << G4endl;   
   // }
   // G4cout << G4endl;
   // G4cout << "TPC Up tracks: " << G4endl;
   // G4cout << G4endl;
   // for(G4int itrk=0;itrk<fTPCUp_NTracks;itrk++){
   //   G4cout << "Track ID: " << fVecTPCUp_TrackID.at(itrk) 
   // 	    << " - TrackLength = " << fVecTPCUp_TrackLength[itrk] / mm << " mm" 
   // 	    << " - TrackEdep = " << fVecTPCUp_TrackEdep[itrk] / MeV << " MeV" 
   // 	    << G4endl;   
   // }
   // G4cout << G4endl;
   // G4cout << "TPC Down tracks: " << G4endl;
   // G4cout << G4endl;
   // for(G4int itrk=0;itrk<fTPCDown_NTracks;itrk++){
   //   G4cout << "Track ID: " << fVecTPCDown_TrackID.at(itrk) 
   // 	    << " - TrackLength = " << fVecTPCDown_TrackLength[itrk] / mm << " mm" 
   // 	    << " - TrackLength = " << fVecTPCDown_TrackEdep[itrk] / MeV << " MeV" 
   // 	    << G4endl;   
   // }
   

  // get analysis manager  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // // fill histograms
  analysisManager->FillH1(1, fEnergyAbsTPCup);

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
  
  analysisManager->FillNtupleIColumn(10, fNVtx);
  
  analysisManager->AddNtupleRow();


  // Print per event (modulo n)
  //  
  G4int eventID = event->GetEventID();
  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;

    //   G4cout << "   Absorber TPC up: total energy: " << std::setw(7)
    // 	   << G4BestUnit(fEnergyAbsTPCup,"Energy")
    // 	   << "       total track length: " << std::setw(7)
    // 	   << G4BestUnit(fTrackLAbsTPCup,"Length")
    // 	   << G4endl 
    // 	   << "   Absorber TPC down: total energy: " << std::setw(7)
    // 	   << G4BestUnit(fEnergyAbsTPCup,"Energy")
    // 	   << "       total track length: " << std::setw(7)
    // 	   << G4BestUnit(fTrackLAbsTPCup,"Length")
    // 	   << G4endl
    // 	   << "   Absorber Target: total energy: " << std::setw(7)
    // 	   << G4BestUnit(fEnergyAbsTarget,"Energy")
    // 	   << "       total track length: " << std::setw(7)
    // 	   << G4BestUnit(fTrackLAbsTarget,"Length")
    // 	   << G4endl;

  }

  // Print out the informations from SteppingAction
  //std::cout << std::endl;
  //std::cout << "Print out the informations from SteppingAction:" << std::endl;
  //std::cout << std::endl;
  //std::cout << "# of Tracks = " << fNTracks << std::endl;
  //for(G4int itrk=0;itrk<fNTracks;itrk++){
  //std::cout << "Track ID: = " << fVecTrackID[itrk] 
  //	      << " - Track PDG: = " << fVecTrackPDG[itrk]   
  //	      << " - Track E: = " << fVecTrackE[itrk]   
  //  	      << " - Track Mom: = " << fVecTrackMomMag[itrk]   
  //  	      << std::endl;
  //}
  //std::cout << std::endl;
  
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
  
}

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

  // Set flags if track (first prestep) is contained in each detector
  //G4String detname = prestep->GetTouchableHandle()->GetVolume()->GetName();  
  //G4bool newtrkIsTPCUp = false;
  //G4bool newtrkIsTPCDown = false;
  //G4bool newtrkIsTarget = false;
  
  G4int vecel = fNTracks-1;

  //G4String detname = track->GetVolume()->GetName();
  //std::vector < G4String > vecstr;  
  //if(detname!="TPCUp") return;

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
