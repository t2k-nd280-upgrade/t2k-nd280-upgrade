////////////////////////////////////////////////////////////
// $Id: ND280RootPersistencyManager.cc,v 1.93 2012/03/23 14:09:24 mcgrew Exp $
//
 
#include "ND280RootPersistencyManager.hh"

//#include "ND280UserRunAction.hh"
#include "ND280Trajectory.hh"
#include "ND280TrajectoryPoint.hh"

#include <memory>
#include <cmath>
#include <algorithm>

#include <globals.hh>

#include <G4Event.hh>
#include <G4Run.hh>
#include <G4RunManager.hh>
#include <G4UnitsTable.hh>
#include <G4ParticleTable.hh>
#include <G4PrimaryVertex.hh>
#include <G4PrimaryParticle.hh>
#include <G4SDManager.hh>
#include <G4HCtable.hh>
#include <G4VisAttributes.hh>
#include <G4VSolid.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Sphere.hh>
#include <G4Polyhedra.hh>
#include <G4Trap.hh>
#include <G4StepStatus.hh>
#include <G4TransportationManager.hh>
#include <G4FieldManager.hh>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TGeoMedium.h>
#include <TGeoPgon.h>

TROOT root("ROOT","Root of Everything");

ND280RootPersistencyManager::ND280RootPersistencyManager() 
    : ND280PersistencyManager(), fOutput(NULL),
      fEventTree(NULL), fND280UpEvent(NULL), 
      fEventsNotSaved(0) {}

ND280RootPersistencyManager* ND280RootPersistencyManager::GetInstance() {
    ND280RootPersistencyManager *current 
        = dynamic_cast<ND280RootPersistencyManager*>(
            G4VPersistencyManager::GetPersistencyManager());
    if (!current) current = new ND280RootPersistencyManager();
    return current;
}

ND280RootPersistencyManager::~ND280RootPersistencyManager() {
    if (fOutput) delete fOutput; fOutput = NULL;
}

bool ND280RootPersistencyManager::IsOpen() {
  if (fOutput && fOutput->IsOpen()) {
    fOutput->cd();
    return true;
  }
  return false;
}

bool ND280RootPersistencyManager::Open(G4String filename) {
  if (fOutput) {
    G4cout << "ND280RootPersistencyManager::Open "
	   << "-- Delete current file pointer" 
	   << G4endl;
  }
  
  SetFilename(filename +".root");
  
  G4cout << "ND280RootPersistencyManager::Open " << GetFilename() << G4endl;
  
  //
  // TFile::Open() crashes!!!
  //
  //fOutput = TFile::Open(GetFilename(),"RECREATE","T2K-ND280 upgrade Root Output");
  //
  fOutput = new TFile(GetFilename().c_str(),"RECREATE");
  
  fEventTree = new TTree("ND280upEvents","ND280 upgrade Event Tree");
  
  //fND280UpEvent = new TND280UpEvent();    
  fEventTree->Branch("Event","TND280UpEvent",&fND280UpEvent,128000,0);
  
  fEventsNotSaved = 0;
  
  return true;
}

bool ND280RootPersistencyManager::Close() {
  if (!fOutput) {
    G4ExceptionDescription msg;
    msg << "No Output File" << G4endl; 
    G4Exception("ND280RootPersistencyManager::Close",
		"ExN02Code001",FatalException, msg);
    return false;
  }
  
  fOutput->cd();
  fOutput->Write();
  fOutput->Close();
  
  G4cout << "Output file " << GetFilename() << " closed." << G4endl;
  
  fEventTree = NULL;
  fND280UpEvent = NULL;

  delete fEventTree;
  delete fND280UpEvent;

  return true;
}

bool ND280RootPersistencyManager::Store(const G4Event* anEvent) {
  if (!fOutput) {
    G4ExceptionDescription msg;
    msg << "No Output File" << G4endl; 
    G4Exception("ND280RootPersistencyManager::Store",
   		"ExN02Code001",FatalException, msg);
    return false;
  }
  
  //const G4Run* runInfo = G4RunManager::GetRunManager()->GetCurrentRun();
  //const ND280UserRunAction* runAction 
  //= dynamic_cast<const ND280UserRunAction*>(
  //G4RunManager::GetRunManager()->GetUserRunAction());
  
  //ND280UserEventInformation* eventInfo 
  //= dynamic_cast<ND280UserEventInformation*>(
  //anEvent->GetUserInformation());
  //if (!eventInfo) {
  //ND280Error("  %%% Event without user information.");
  //G4Exception("Trying to write an event without user information");
  //}
  
  //int runId = runInfo->GetRunID();
  //int subId = runAction->GetT2KSubrunId();
  //int eventId = anEvent->GetEventID();
  //if (subId>0) eventId += 100000*subId;
  
  // // Build the event context.
  // ND::TND280Context context;
  // context.SetRun(runId);
  // if (subId >= 0) context.SetSubRun(subId);
  // context.SetEvent(eventId);
  // context.SetPartition(ND::TND280Context::kMCData);
  
  // // The event is constructed using an auto ptr since we must delete it
  // // before leaving this method.
  // std::auto_ptr<ND::TND280Event> event(new ND::TND280Event(context));
  
  // if (!event->FindDatum("truth")) {
  //   event->push_back(new ND::TDataVector("truth"));
  // }
  
  // // Mark the trajectories to save.
  // MarkTrajectories(anEvent);
  
  // // Save the trajectories.
  // CopyTrajectories(*event,anEvent->GetTrajectoryContainer());
  
  // // Save the primary particles.
  // ND::TG4PrimaryVertexContainer* vertexContainer
  //   = MakePrimary("G4PrimVertex00",
  // 		  "G4 Primary Particle Vertex",
  // 		  anEvent->GetPrimaryVertex());
  // if (vertexContainer) {
  //   // Sort the vertex container by time.
  //   std::stable_sort(vertexContainer->begin(), vertexContainer->end(), 
  // 		     lt_prim_vertex);
  //   ND::THandle<ND::TDataVector> vect
  //     = event->Get<ND::TDataVector>("truth");
  //   vect->push_back(vertexContainer);
  // }
  // else {
  //   ND280Verbose("%% No Primary Particles to save");
  // }
  
  
  
  fND280UpEvent = new TND280UpEvent();
  // The event is constructed using an auto ptr since we must delete it
  // before leaving this method.                            
  //std::auto_ptr<TND280UpEvent> fND280UpEvent(new TND280UpEvent());
  
  fND280UpEvent->SetEventID(anEvent->GetEventID());
  
  //                                          
  // Store the track in the ND280 event 
  //                                          
  
  const G4TrajectoryContainer* trajectories = anEvent->GetTrajectoryContainer();
  if (!trajectories) {
    G4ExceptionDescription msg;
    msg << "No Trajectories" << G4endl; 
    G4Exception("ExN02EventAction::EndOfEventAction()",
   		"ExN02Code001", JustWarning, msg);
    return false;
  }
  
  // loop over the trajectories
  for (TrajectoryVector::iterator t = trajectories->GetVector()->begin();
       t != trajectories->GetVector()->end();
       ++t) { 
    
    ND280Trajectory* ndTraj = dynamic_cast<ND280Trajectory*>(*t);
    //   G4VTrajectory* g4Traj = dynamic_cast<G4VTrajectory*>(*t);
    
    G4String particleName = ndTraj->GetParticleName();
    G4int NptTraj = ndTraj->GetPointEntries();
    G4int TrajTrkId = ndTraj->GetTrackID(); 

    TND280UpTrack *nd280Track = new TND280UpTrack();
    nd280Track->SetTrackID(ndTraj->GetTrackID());
    nd280Track->SetParentID(ndTraj->GetParentID());
    nd280Track->SetPDG(ndTraj->GetPDGEncoding());
    nd280Track->SetProcessName(ndTraj->GetProcessName());
    nd280Track->SetInitKinEnergy(ndTraj->GetInitialKineticEnergy());
    
    double momX = ndTraj->GetInitialMomentum().x();
    double momY = ndTraj->GetInitialMomentum().y();
    double momZ = ndTraj->GetInitialMomentum().z();
    nd280Track->SetInitMom(momX,momY,momZ);
    
    nd280Track->SetInitCosTheta(ndTraj->GetInitialCosTheta());
    nd280Track->SetCharge(ndTraj->GetCharge());
    nd280Track->SetRange(ndTraj->GetRange());
    
    int NPoints = ndTraj->GetPointEntries();
    nd280Track->SetNPoints(NPoints);
    
    //
    // Store the points of the track 
    //
    
    for(int itp=0;itp<NPoints;itp++){ // loop over all the points
      
      ND280TrajectoryPoint* ndPoint = dynamic_cast<ND280TrajectoryPoint*>(ndTraj->GetPoint(itp));
      
      if (!ndPoint) 
	{
	  G4ExceptionDescription msg;
	  msg << "No Points in the Trajectory" << G4endl; 
	  G4Exception("ExN02EventAction::EndOfEventAction()",
		      "ExN02Code001", JustWarning, msg);
	  return false;
	}
      
      TND280UpTrackPoint *nd280TrackPoint = new TND280UpTrackPoint();
      
      nd280TrackPoint->SetPointID(itp);
      nd280TrackPoint->SetTime(ndPoint->GetTime());
      
      // momentum 
      double momPtX = ndPoint->GetMomentum().x();
      double momPtY = ndPoint->GetMomentum().y();
      double momPtZ = ndPoint->GetMomentum().z();
      nd280TrackPoint->SetMomentum(momPtX,momPtY,momPtZ);
      
      nd280TrackPoint->SetEdep(ndPoint->GetEdep());
      nd280TrackPoint->SetStepLength(ndPoint->GetStepLength());
      nd280TrackPoint->SetStepDeltaLyz(ndPoint->GetStepDeltaLyz());
      nd280TrackPoint->SetStepStatus(ndPoint->GetStepStatus());
      nd280TrackPoint->SetPhysVolName(ndPoint->GetPhysVolName());
      
      // preStep position 
      double prevX = ndPoint->GetPrevPosition().x();
      double prevY = ndPoint->GetPrevPosition().y();
      double prevZ = ndPoint->GetPrevPosition().z();
      nd280TrackPoint->SetPrePosition(prevX,prevY,prevZ);
      
      // postStep position
      double postX = ndPoint->GetPostPosition().x();
      double postY = ndPoint->GetPostPosition().y();
      double postZ = ndPoint->GetPostPosition().z();
      nd280TrackPoint->SetPostPosition(postX,postY,postZ);
      
      nd280Track->AddPoint(nd280TrackPoint);
      
    } // end loop over the points
    
    //nd280Track->SaveIt(true); // TO DEFINE IT    
    fND280UpEvent->AddTrack(nd280Track);
    
    // 
    // ND280Trajectory::ShowTrajectory()
    // and 
    // G4VTrajectory::ShowTrajectory(os)
    // don't work:
    //
    //  G4BestUnit: the category Momentum does not exist.
    //
    // -------- EEEE ------- G4Exception-START -------- EEEE -------
    // *** G4Exception : InvalidCall
    //       issued by : G4BestUnit::G4BestUnit()
    // Missing unit category !
    // *** Fatal Exception *** core dump ***
    // -------- EEEE -------- G4Exception-END --------- EEEE -------
    //
    //ndTraj->ShowTrajectory(G4cout);
    //
    
  } // end loop over Trajectories
  
  // // Print and store the ND280 event and 1 ND280 track
  // fND280UpEvent->PrintEvent();
  // fND280UpEvent->GetTrack(1)->PrintTrack();
  // fND280UpEvent->GetTrack(1)->GetPoint(1)->PrintTrackPoint();
  
  // // Save the informational particles.
  // if (eventInfo) {
  //   // No info to be saved (be sure and change this comment if some is
  //   // added.
  // }
  
  fOutput->cd();
  
  // Get the address of the event from the auto_ptr to the fEventTree branch
  // pointer,, and  then fill the event  tree.  This copies the  data to the
  // output  file.  After the  data is  saved, I  reset the  the fND280Event
  // pointer (the fEventTree branch pointer).
  //fND280Event = event.get(); 
  fEventTree->Fill();
  fND280UpEvent = NULL;
  delete fND280UpEvent;

  // ++fEventsNotSaved;
  // if (fEventsNotSaved>100) {
  //   ND280Info("AutoSave Event Tree");
  //   fEventTree->AutoSave("SaveSelf");
  //   fEventsNotSaved = 0;
  // }
    
  return true;
}




// bool ND280RootPersistencyManager::Store(const G4Run* aRun) {
//     return false;
// }

//bool ND280RootPersistencyManager::Store(const G4VPhysicalVolume* aWorld) {
//     if (!gGeoManager) {
  //         ND280Error("ND280RootPersistencyManage::Store(world) run before /t2k/update");
  //         ND280RootGeometryManager::Get()->Update(aWorld,true);
  //     }
  //     if (!fOutput) {
  //         ND280Error("ND280RootPersistencyManager::Store "
  //                    << "-- No Output File");
  //         return false;
  //     }
  //     fOutput->cd();
  //     gGeoManager->Write();
  //     return true;
  //}
  
  // std::vector<int>::iterator ND280RootPersistencyManager::CleanHitContributors(
  //     std::map<int,int>& parentMap,
  //     const ND::TG4TrajectoryContainer& trajCon,
  //     std::vector<int>::iterator start, 
  //     std::vector<int>::iterator finish) {
  
  //     for (std::vector<int>::iterator c=start; c != finish; ++c) {
  //         // Check each contributor to make sure that it is a valid
  //         // trajectory.  If it isn't in the trajectory map, then set it
  //         // to a parent that is.
  //         int loopTrap = 100000;
  //         while (!trajCon.GetTrajectory(*c)) {
  //             std::map<int,int>::iterator t = parentMap.find(*c);
  //             if (t == parentMap.end()) {
  //                 ND280Warn("% Contributing trajectory without parent");
  //                 break;
  //             }
//             *c = t->second;
//             if ( 0 > --loopTrap) {
//                 ND280Warn("Break loop in "
//                           << "ND280RootPersistencyManager::"
//                           << "CleanHitContributors");
//                 break;
//             }
//         }
//     }

//     // Remove the duplicate entries.
//     std::sort(start,finish);
//     finish = std::unique(start,finish);
        
//     return finish;
// }

// void ND280RootPersistencyManager::FillParentMap(std::map<int,int>& parentMap,
//                                                     const G4Event* src) {
//     parentMap.clear();

//     const G4TrajectoryContainer* g4Traj = src->GetTrajectoryContainer();
//     if (g4Traj) {
//         for (TrajectoryVector::iterator t = g4Traj->GetVector()->begin();
//              t != g4Traj->GetVector()->end();
//              ++t) {
//             parentMap[(*t)->GetTrackID()] = (*t)->GetParentID();
//         }
//     }
//     else {
//         ND280Warn("%%% Missing trajectories for Hit Contributors");
//     }
// }
                                                       
// ND::TG4HitContainer* ND280RootPersistencyManager::CreateHitSegmentContainer(
//     std::map<int,int>& parentMap,
//     const ND::TG4TrajectoryContainer& trajCon,
//     G4VHitsCollection* g4Hits,
//     std::string hitsName,
//     ND::TND280Event& dest) {

//     if (!g4Hits) return NULL;
//     if (g4Hits->GetSize()<1) return NULL;
//     // Check to make sure that this hit collection contains ND280HitSegment
//     // objects.
//     ND280HitSegment* g4Hit = dynamic_cast<ND280HitSegment*>(g4Hits->GetHit(0));
//     if (!g4Hit) return NULL;

//     ND::TG4HitContainer* hits = new ND::TG4HitContainer(hitsName.c_str());
//     for (unsigned int h=0; h<g4Hits->GetSize(); ++h) {
//         ND280HitSegment* g4Hit
//             = dynamic_cast<ND280HitSegment*>(g4Hits->GetHit(h));
//         std::vector<int>::iterator e = 
//             CleanHitContributors(parentMap,trajCon,
//                                  g4Hit->GetContributors().begin(),
//                                  g4Hit->GetContributors().end());
//         g4Hit->GetContributors().erase(e,g4Hit->GetContributors().end());
//         ND::TG4HitSegment* hit = dynamic_cast<ND::TG4HitSegment*>(g4Hit);
//         hits->push_back(new ND::TG4HitSegment(*hit));
//     }
    
//     return hits;
// }
                                                       
// void ND280RootPersistencyManager::CopyHitCollections(ND::TND280Event& dest, 
//                                                      const G4Event* src) {
//     G4HCofThisEvent* HCofEvent = src->GetHCofThisEvent();
//     G4SDManager *sdM = G4SDManager::GetSDMpointer();
//     G4HCtable *hcT = sdM->GetHCtable();
//     if (!HCofEvent) return;

//     // Make sure that the directory for G4 hits exists in the output event.
//     if (!dest.FindDatum("truth/g4Hits")) {
//         if (!dest.FindDatum("truth")) {
//             dest.push_back(new ND::TDataVector("truth"));
//         }
//         dest.Get<ND::TDataVector>("truth")->push_back(
//             new ND::TDataVector("g4Hits"));
//     }

//     //Keep track of all trajectories and which ones are saved.
//     std::map<int,int> parentMap;
//     FillParentMap(parentMap,src);

//     // Get the trajectories that have been saved in the event.
//     ND::THandle<ND::TG4TrajectoryContainer> trajCon 
//         = dest.Get<ND::TG4TrajectoryContainer>("truth/G4Trajectories");

//     // Copy each of the hit categories into the output event.
//     for (int i=0; i<hcT->entries(); ++i) {
//         G4String SDname = hcT->GetSDname(i);
//         G4String HCname = hcT->GetHCname(i);
//         int HCId = sdM->GetCollectionID(SDname+"/"+HCname);
//         G4VHitsCollection* g4Hits = HCofEvent->GetHC(HCId);
//         if (g4Hits->GetSize()<1) continue;
//         std::string hitsName = SDname;
//         ND::TDatum* hits = NULL;
//         if (!hits) hits = CreateHitSegmentContainer(parentMap,*trajCon,
//                                                     g4Hits, hitsName, dest);
//         if (hits) dest.Get<ND::TDataVector>("truth/g4Hits")->push_back(hits);
//     }
// }

// ND::TG4PrimaryVertexContainer* ND280RootPersistencyManager::MakePrimary(
//     const char* name, const char* title, const G4PrimaryVertex* src) {
//     if (!src) return NULL;

//     ND::TG4PrimaryVertexContainer* vertexContainer = NULL;
//     while (src) {
//         ND::TG4PrimaryVertex vtx;
        
//         vtx.GetPosition().SetX(src->GetX0());
//         vtx.GetPosition().SetY(src->GetY0());
//         vtx.GetPosition().SetZ(src->GetZ0());
//         vtx.GetPosition().SetT(src->GetT0());
        
//         for (int i=0; i< src->GetNumberOfParticle(); ++i) {
//             ND::TG4PrimaryParticle prim;
//             G4PrimaryParticle *g4Prim = src->GetPrimary(i);
//             prim.SetTrackId(g4Prim->GetTrackID());
//             prim.SetPDGCode(g4Prim->GetPDGcode());
//             prim.SetTitle(g4Prim->GetG4code()->GetParticleName());
//             prim.GetMomentum().SetPx(g4Prim->GetPx());
//             prim.GetMomentum().SetPy(g4Prim->GetPy());
//             prim.GetMomentum().SetPz(g4Prim->GetPz());
//             double E = pow(prim.GetMomentum().P(),2) 
//                 + pow(g4Prim->GetG4code()->GetPDGMass(),2);
//             if (E>0) E = sqrt(E);
//             else E = 0;
//             prim.GetMomentum().SetE(E);
//             vtx.GetPrimaryParticles().push_back(prim);
//             ND280Verbose("   "
//                       << " id " << g4Prim->GetTrackID()
//                       << " " << g4Prim->GetG4code()->GetParticleName()
//                       << " with " << G4BestUnit(prim.GetMomentum().E(),"Energy")
//                       << " (from " << G4BestUnit(g4Prim->GetMomentum().mag(), 
//                                                  "Energy") << "/c" 
//                       << " + " << G4BestUnit(prim.GetMomentum().M(), 
//                                              "Energy") << "/c^2"
//                       << ")");
//         }

//         ND280VertexInfo* srcInfo 
//             = dynamic_cast<ND280VertexInfo*>(src->GetUserInformation());
//         if (srcInfo) {
//             vtx.SetGeneratorName(srcInfo->GetName());
//             vtx.SetReaction(srcInfo->GetReaction());
// #if oaEvent_MAJOR_VERSION>7 ||				
//     (oaEvent_MAJOR_VERSION==7 && oaEvent_MINOR_VERSION>1) || 
//     (oaEvent_MAJOR_VERSION==6 &&				
//      oaEvent_MINOR_VERSION>2 && 
//      oaEvent_PATCH_VERSION>5)
//             vtx.SetFilename(srcInfo->GetFilename());
// #else
// #warning Not saving input file name, tree, or entry.
// #endif
//             vtx.SetInteractionNumber(srcInfo->GetInteractionNumber());
//             vtx.SetCrossSection(srcInfo->GetCrossSection());
//             vtx.SetDiffCrossSection(srcInfo->GetDiffCrossSection());
//             vtx.SetWeight(srcInfo->GetWeight());
//             vtx.SetProbability(srcInfo->GetProbability());
//             ND::TG4PrimaryVertexContainer *vtxInfo
//                 = MakePrimary("Informational","Informational Vertex",
//                               srcInfo->GetInformationalVertex());
//             if (vtxInfo) {
//                 vtx.SetInfoVertex(*vtxInfo);
//                 delete vtxInfo;
//             }
//         }

//         if (!vertexContainer) {
//             vertexContainer = new ND::TG4PrimaryVertexContainer(name,title);
//         }
//         vertexContainer->push_back(vtx);
//         src = src->GetNext();
//     }
    
//     return vertexContainer;
// }


// void ND280RootPersistencyManager::CopyTrajectoryPoints(ND::TG4Trajectory& traj, 
//                                                        G4VTrajectory* g4Traj) {
//     std::vector<int> selected;

//     // Choose the trajectory points that are going to be saved.
//     SelectTrajectoryPoints(selected, g4Traj);

//     // Make sure the selected trajectory points are in order and unique.
//     std::sort(selected.begin(),selected.end());
//     selected.erase(std::unique(selected.begin(), selected.end()),
//                    selected.end());

//     ////////////////////////////////////
//     // Save the trajectories.
//     ////////////////////////////////////
//     for (std::vector<int>::iterator tp = selected.begin(); 
//          tp != selected.end(); ++tp) {
//         ND280TrajectoryPoint* nd280Point 
//             = dynamic_cast<ND280TrajectoryPoint*>(g4Traj->GetPoint(*tp));
//         ND::TG4TrajectoryPoint point;
//         point.SetPosition(nd280Point->GetPosition().x(),
//                           nd280Point->GetPosition().y(),
//                           nd280Point->GetPosition().z(),
//                           nd280Point->GetTime());
//         point.SetMomentum(nd280Point->GetMomentum().x(),
//                           nd280Point->GetMomentum().y(),
//                           nd280Point->GetMomentum().z());
//         point.SetVolumeNode(nd280Point->GetVolumeNode());
//         traj.GetTrajectoryPoints().push_back(point);
//     }

// }

// void ND280RootPersistencyManager::CopyTrajectories(
//     ND::TND280Event& dest,
//     const G4TrajectoryContainer* src) {
//     if (!src) {
//         ND280Verbose("No Trajectories ");
//         return;
//     }

//     ND::TG4TrajectoryContainer* traj = new ND::TG4TrajectoryContainer();

//     // Keep track of all trajectories and which ones are saved.  The first
//     // field is the track id, and the second is whether it is saved.
//     std::map<int,std::pair<int,bool> > parentMap;
//     for (TrajectoryVector::iterator t = src->GetVector()->begin();
//          t != src->GetVector()->end();
//          ++t) {
//         ND280Trajectory* ndTraj = dynamic_cast<ND280Trajectory*>(*t);

//         parentMap[ndTraj->GetTrackID()] 
//             = std::pair<int,bool>(ndTraj->GetParentID(),false);

//         // Check if the trajectory should be saved.
//         if (!ndTraj->SaveTrajectory()) continue;
//         parentMap[(*t)->GetTrackID()].second = true;

//         std::string particleName = ndTraj->GetParticleName();

//         ND::TG4Trajectory copy;
//         copy.SetTrackId((*t)->GetTrackID());
//         // Set the particle type information.
//         G4ParticleDefinition* part 
//             = G4ParticleTable::GetParticleTable()->FindParticle(particleName);
//         if (!part) {
//             G4Exception(std::string("ND280RootPersistencyManager::")
//                         + "No particle information for "
//                         + (*t)->GetParticleName());
//         }
//         copy.SetParticleName(particleName);
//         copy.SetPDGEncoding((*t)->GetPDGEncoding());
//         // Check through the list of saved trajectories to find the first
//         // parent that is written to the output file.
//         int parentId = (*t)->GetTrackID();
//         do {
//             parentId = parentMap[parentId].first;
//         } while (parentId != 0 && !parentMap[parentId].second);
//         copy.SetParentId(parentId);
//         // Set the particle momentum.
//         TLorentzVector mom;
//         mom.SetXYZM((*t)->GetInitialMomentum().x(),
//                     (*t)->GetInitialMomentum().y(),
//                     (*t)->GetInitialMomentum().z(),
//                     part->GetPDGMass());
//         copy.SetInitialMomentum(mom);
//         CopyTrajectoryPoints(copy, (*t));
//         (*traj)[copy.GetTrackId()] = copy;
//     }

//     ND::THandle<ND::TDataVector> vect = dest.Get<ND::TDataVector>("truth");
//     vect->push_back(traj);
// }

// void ND280RootPersistencyManager::MakeMCHeader(ND::TND280Event& dest,
//                                                const G4Event* src) {
// #if oaEvent_MAJOR_VERSION>7 ||                                  
//     (oaEvent_MAJOR_VERSION==7 && oaEvent_MINOR_VERSION>26)
//     // create the MC Header.
//     ND::TND280MCHeader* mcHeader = new ND::TND280MCHeader();
//     ND::THandle<ND::TDataVector> vect = dest.Get<ND::TDataVector>("truth");
//     vect->AddDatum(mcHeader);

//     // Add the off-axis detector magnetic field.
//     G4FieldManager* fieldMgr 
//         = G4TransportationManager::GetTransportationManager()
//         ->GetFieldManager();
//     const ND280MagneticField* field
//         = dynamic_cast<const ND280MagneticField*>(fieldMgr->GetDetectorField());
//     if (field) mcHeader->SetOffAxisField(field->GetFieldStrength());

// #if oaEvent_MAJOR_VERSION>7
//     // Save the hash code of the geometry for this event.
//     mcHeader->SetGeometryHash(ND::TOADatabase::Get().GeomId().GetHash());
// #else
// #warning Not saving geometry hash in MC Header.
// #endif

//     const G4RunManager* runMan = G4RunManager::GetRunManager();
//     const ND280UserPrimaryGeneratorAction* userGenerator
//         = dynamic_cast<const ND280UserPrimaryGeneratorAction*>(
//             runMan->GetUserPrimaryGeneratorAction());
//     if (!userGenerator) G4Exception("Missing ND280UserPrimaryGeneratorAction");
//     const ND280PrimaryGenerator* primGenerator 
//         = dynamic_cast<const ND280PrimaryGenerator*>(
//             userGenerator->GetGenerator(0));
//     if (!primGenerator) G4Exception("Missing ND280UserPrimaryGeneratorAction");
//     double intensity = primGenerator->GetCountGenerator()->GetIntensity();

// #if not BEFORE_oaEvent(8,6,0)
//     mcHeader->SetIntensity(intensity);
// #else
// #warning Not saving intensity in MC Header.
// #endif

// #else
//     return;
// #warning Not creating MC Header.
// #endif
// }
