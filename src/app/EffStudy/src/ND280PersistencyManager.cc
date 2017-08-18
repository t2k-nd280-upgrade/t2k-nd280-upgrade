//
// Implementation for concrete G4PersistencyManager.
//

#include <G4ios.hh>
#include <G4Event.hh>
#include <G4PrimaryVertex.hh>
#include <G4PrimaryParticle.hh>
#include <G4StepStatus.hh>
#include <G4Region.hh>
#include <G4RegionStore.hh>
#include <G4LogicalVolume.hh>

#include <TPRegexp.h>

//#include <TND280Log.hxx>

#include "ND280PersistencyManager.hh"
#include "ND280PersistencyMessenger.hh"
#include "ND280Trajectory.hh"
#include "ND280TrajectoryPoint.hh"
#include "ND280TrajectoryMap.hh"
//#include "ND280HitSegment.hh" // I use ExN02TrackerHit

#include "ExN02TrackerHit.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"

#include <G4SystemOfUnits.hh>

// Creator for the persistency manager.  This is NOT a public routine
// since it is only used by this class to create a singleton object.
ND280PersistencyManager::ND280PersistencyManager() 
  : G4VPersistencyManager(), fFilename("/dev/null"),
    fLengthThreshold(1*cm), // default nd280mc is 2*cm (NOT APPLIED!!!)
    fGammaThreshold(20*MeV), // default nd280mc is 50*MeV 
    fNeutronThreshold(50*MeV),
    fTrajectoryPointAccuracy(1*cm), 
    fSaveAllPrimaryTrajectories(true) //default nd280mc is false
{
  fPersistencyMessenger = new ND280PersistencyMessenger(this);
}

// When should this be called???  Probably the user should never call
// this directly and it will be called automatically in the exit phase.
ND280PersistencyManager::~ND280PersistencyManager() {
    ClearTrajectoryBoundaries();
    delete fPersistencyMessenger;
}

G4bool ND280PersistencyManager::Store(const G4Event* anEvent) {
  //ND280Severe(" -- Event store called without a save method " 
  //<< GetFilename());
  return false;
}

G4bool ND280PersistencyManager::Store(const G4Run* aRun) {
  //ND280Severe(" -- Run store called without a save method " 
  //<< GetFilename());
  return false;
}

G4bool ND280PersistencyManager::Store(const G4VPhysicalVolume* aWorld) {
  //ND280Severe(" -- Geometry store called without a save method " 
  //<< GetFilename());
  return false;
}

void ND280PersistencyManager::AddTrajectoryBoundary(const G4String& b) {
    TPRegexp* bound = new TPRegexp(b.c_str());
    fTrajectoryBoundaries.push_back(bound);
}

void ND280PersistencyManager::ClearTrajectoryBoundaries() {
    for (std::vector<TPRegexp*>::iterator r = fTrajectoryBoundaries.begin();
         r != fTrajectoryBoundaries.end();
         ++r) {
        delete (*r);
    }
    fTrajectoryBoundaries.clear();
}

bool ND280PersistencyManager::SaveTrajectoryBoundary(G4VTrajectory* g4Traj,
                                                     G4StepStatus status,
                                                     G4String currentVolume,
                                                     G4String prevVolume) {
    if (status != fGeomBoundary) return false;
    std::string particleInfo = ":" + g4Traj->GetParticleName();
    if (std::abs(g4Traj->GetCharge())<0.1) particleInfo += ":neutral";
    else particleInfo += ":charged";
    std::string current = particleInfo + ":" + currentVolume;
    std::string previous = particleInfo + ":" + prevVolume;
    for (std::vector<TPRegexp*>::iterator r = fTrajectoryBoundaries.begin();
         r != fTrajectoryBoundaries.end();
         ++r) {
        // Check if a watched volume is being entered.
        if ((*r)->Match(current)>0 && (*r)->Match(previous)<1) {
	  //ND280NamedDebug("boundary","Entering " << current);
	  return true;
        }
        // Check if a watched volume is being exited.
        if ((*r)->Match(current)<1 && (*r)->Match(previous)>0) {
	  //ND280NamedDebug("boundary","Exiting " << current);
	  return true;
        }
    }
    return false;
}



void ND280PersistencyManager::MarkPoint(ND280TrajectoryPoint* ndPoint) {
  
  G4Region* SDRegion = G4RegionStore::GetInstance()->
    GetRegion("SDRegion",false);
  int NSDRootVolumes =  SDRegion->GetNumberOfRootVolumes();
  std::vector<G4LogicalVolume*>::iterator it_logicalVolumeInRegion =
    SDRegion->GetRootLogicalVolumeIterator();

  //G4String detname_curr = ndPoint->GetPhysVolName();
  G4String detname_curr = ndPoint->GetLogVolName();

  // G4cout << "Point in Phys Vol: " << detname_curr 
  // 	 << " --> Log Vol: " << ndPoint->GetLogVolName()
  // 	 << G4endl;
  // // G4cout << "     Eloss = " << ndPoint->GetEdep() / MeV << G4endl;
  
  for(int i = 0; i < NSDRootVolumes ; i++, it_logicalVolumeInRegion++){  
    G4LogicalVolume *logVolumeCurr = (*it_logicalVolumeInRegion);
    G4String SDLogVolName = logVolumeCurr->GetName();
    
    //
    // Names of Physical and Logical volumes contained in the SDRegion
    // are identical --> the check is done in ExN02DetectorConstruction!!!
    //
    
    //if(detname_curr == SDLogVolName){
    if(detname_curr.contains(SDLogVolName)){	  
      //G4cout << " --> SDLogVolName = " << SDLogVolName << G4endl;
      ndPoint->MarkPoint();
      break;
    }
  }
}



void ND280PersistencyManager::MarkTrajectory(ND280Trajectory* ndTraj,const G4Event *event) {
  
  //G4cout << "ND280PersistencyManager::MarkTrajectory" << G4endl;
  
  // Go through all of the trajectories and save:
  //
  //   ** Trajectories from primary particles if
  //       1) a daughter depeosited energy in a sensitive detector
  //       2) or, SaveAllPrimaryTrajectories() is true
  //
  //   ** Trajectories created by a particle decay.
  //
  //   ** Charged particle trajectories that pass through a sensitive
  //         detector.
  // 
  //   ** Gamma-rays and neutrons above a threshold which have daughters
  //         depositing energy in a sensitive detector.
  //    
  
  std::string particleName = ndTraj->GetParticleName();
  std::string processName = ndTraj->GetProcessName();
  double initialMomentum = ndTraj->GetInitialMomentum().mag();
  
  // Check if all primary particle trajectories should be saved.  The
  // primary particle should always be saved if it, or any of it's
  // children, deposited energy in a sensitive detector.  If the primary
  // didn't deposit energy in a sensitive detector, then it will only be
  // saved if SaveAllPrimaryTrajectories is true.
  
  // N.B. The variable GetSDTotalEnergyDeposit() is filled in ExN02EventAction
  //      with the energy deposited by the G4Hits in sensitive detectors 

  if (ndTraj->GetParentID() == 0) {

    //
    // DON'T APPLY ANY CUT ON THE SD ENERGY DEPOSIT
    // Applied in nd280mc!!!
    // This is not needed because it's always 0 w/o storing hits, 
    // i.e. when using a uniform target!!!
    //
    //if (ndTraj->GetSDTotalEnergyDeposit()>1*eV 
    //|| GetSaveAllPrimaryTrajectories()) {
    //
    
    if (GetSaveAllPrimaryTrajectories()) {
      
      ndTraj->MarkTrajectory();
      return;
    }
  }
    
  // Don't save the neutrinos
  if (particleName == "anti_nu_e") return;
  if (particleName == "anti_nu_mu") return;
  if (particleName == "anti_nu_tau") return;
  if (particleName == "nu_e") return;
  if (particleName == "nu_mu") return;
  if (particleName == "nu_tau") return;

  // Don't save low momentum charged particles (My cuts)
  if (particleName == "gamma") return;
  if (particleName == "neutron") return;
  if (particleName == "proton" && initialMomentum<200*MeV) return;
  //
  
  // Save any decay product
  if (processName == "Decay") { // useful for Michel electrons
    ndTraj->MarkTrajectory(false);
    return;
  }

  //
  // DON'T APPLY ANY CUT ON THE SD TRACK LENGTH
  //
  // Save particles that produce a signal in a sensitive detector.  This
  // doesn't automatically save, but the parents will be automatically
  // considered for saving by the next bit of code.  
  //if (ndTraj->GetSDLength() > GetLengthThreshold()) {
  //ndTraj->MarkTrajectory(false);
  //return;
  //}
  
  //
  // DON'T APPLY ANY CUT ON THE SD ENERGY DEPOSIT
  // Applied in nd280mc!!!
  // This is not needed because it's always 0 w/o storing hits, 
  // i.e. when using a uniform target!!!
  //
  // For the next part, only consider particles where the children have
  // deposited energy in a sensitive detector.
  //if (ndTraj->GetSDTotalEnergyDeposit()<1*eV) return; 
  

  // Save higher energy gamma rays that have descendents depositing
  // energy in a sensitive detector.
  // if (particleName == "gamma" && initialMomentum > GetGammaThreshold()) {
  //   ndTraj->MarkTrajectory(false);
  //   return;
  // }
  
  // Save higher energy neutrons that have descendents depositing energy
  // in a sensitive detector.
  // if (particleName == "neutron" 
  //     && initialMomentum > GetNeutronThreshold()) {
  //   ndTraj->MarkTrajectory(false);
  //   return;
  // }
  
  // Since I don't apply any cut on track length
  // I store all the tracks that were not rejected at this point.
  // Not done in nd280mc!!!
  //ndTraj->MarkTrajectory(false);


  //
  // TODO:
  // - comment  ndTraj->MarkTrajectory(false); --> don't store all the tracks
  // - store all the following particles 
  //    pi+/- (+-211)
  //    mu+/- (+-13) (see whether to make cuts on mom, length for those coming from pi+ -> mu+ 
  //    prot (2212) (apply cut on momentum)
  //    e+/- (+-11) (apply cut on momentum --> avoid electrons from ionization etc...)
  // - store tracks that passed pdg cuts but with some physical processes (elastic, inelastic,...)
  // - don't store gammas ???
  //
  // Look at process names here:
  // http://geant4.web.cern.ch/geant4/support/proc_mod_catalog/processes/
  //




  // My cuts

  // gamma conversion
  if (particleName == "e+" || particleName == "e-"){
    if (processName == "conv"){
      if(initialMomentum > 100*MeV){
	ndTraj->MarkTrajectory(false); 
     	return;
      }
    }
  }

  // electron multiple scattering
  if ( (particleName == "e-" || particleName == "e+") &&
       processName == "msc"){ 
    ndTraj->MarkTrajectory(false);
  }

  // muon multiple scattering
  if ( (particleName == "mu-" || particleName == "mu+") &&
       processName == "msc"){ // multiple scattering
    ndTraj->MarkTrajectory(false);
  }
    
  // pion inelastic 
  if (processName == "pi+Inelastic"){
    if (particleName == "pi+"){
      ndTraj->MarkTrajectory(false);
      return;
    }
  }
  if (processName == "pi-Inelastic"){
    if (particleName == "pi-"){
      ndTraj->MarkTrajectory(false);
      return;
    }
  }

  // proton inelastic 
  if (processName == "protonInelastic"){
    if (particleName == "proton"){
      ndTraj->MarkTrajectory(false);
      return;
    }
  }  

  // hadronic elastic processes
  if (processName == "hadElastic"){
    if ( particleName == "proton" ||
	 particleName == "pi-"    ||
	 particleName == "pi+"
	 ){
      ndTraj->MarkTrajectory(false);
      return;    
    }
  }
  
  return;


  //
  // THE LAST CUT MUST BE RECONSIDERED!!!
  // USE TRUE INFORMATIONS INSTEAD OF STORING EVERYTHING,
  // I.E. PDG, INITIALKINETICENERGY, MOMENTUM, RANGE, ETC...
  //


  //
  // SHADOWED, BECAUSE ND280Trajectory ALREADY DECLARED
  // NEEDS REWRITING!!!
  //
  //
  // // Go through all of the event hit collections and make sure that all
  // // primary trajectories and trajectories contributing to a hit are saved.
  // // These are mostly a sub-set of the trajectories marked in the previous
  // // step, but there are a few corner cases where trajectories are not saved
  // // because of theshold issues.
  //
  // G4HCofThisEvent* hitCollections = event->GetHCofThisEvent();
  // if (!hitCollections) return;
  // for (int i=0; i < hitCollections->GetNumberOfCollections(); ++i) {
  //   G4VHitsCollection* g4Hits = hitCollections->GetHC(i);
  //   if (g4Hits->GetSize()<1) continue;  
  //   for (unsigned int h=0; h<g4Hits->GetSize(); ++h) {
  //     ExN02TrackerHit* g4Hit
  // 	= dynamic_cast<ExN02TrackerHit*>(g4Hits->GetHit(h));      
  //     if (!g4Hit) {
  // 	G4ExceptionDescription msg;
  // 	msg << "Not a hit." << G4endl; 
  // 	G4Exception("ND280Persistency::MarkTrajectory()",
  // 		    "ExN02Code001", FatalException, msg);
  // 	continue;
  //     }      
  //     // Make sure that the primary trajectory associated with this hit
  //     // is saved.  The primary trajectories are defined by
  //     // ND280TrajectoryMap::FindPrimaryId(      int primaryId = g4Hit->GetTrackID();
  //     ND280Trajectory* ndTraj 
  // 	= dynamic_cast<ND280Trajectory*>(
  // 					 ND280TrajectoryMap::Get(primaryId));
  //     // primaryId is the track ID (see nd280mc ND280TrajectoryMap)      
  //     if (ndTraj) ndTraj->MarkTrajectory(false);
  //     else{
  // 	G4ExceptionDescription msg;
  // 	msg << "Primary trajectory not found" << G4endl; 
  // 	G4Exception("ND280Persistency::MarkTrajectories()",
  // 		    "ExN02Code001", FatalException, msg);
  //     } 
  //   }
  // } 
}



void ND280PersistencyManager::MarkTrajectories(const G4Event* event) {

  //G4cout << "ND280PersistencyManager::MarkTrajectories" << endl;
 

  const G4TrajectoryContainer* trajectories = event->GetTrajectoryContainer();
  if (!trajectories) {
    //ND280Verbose("No Trajectories ");
    return;
  }
  
  for (TrajectoryVector::iterator t = trajectories->GetVector()->begin();
       t != trajectories->GetVector()->end();
       ++t) {      
    ND280Trajectory* ndTraj = dynamic_cast<ND280Trajectory*>(*t);
    MarkTrajectory(ndTraj,event);
  }
}




double ND280PersistencyManager::FindTrajectoryAccuracy(
    G4VTrajectory* g4Traj, int point1, int point2) {
    if ((point2-point1) < 2) return 0;
    
    G4ThreeVector p1 = g4Traj->GetPoint(point1)->GetPosition();
    G4ThreeVector p2 = g4Traj->GetPoint(point2)->GetPosition();

    if ( (p2-p1).mag() < 0.1*mm) return 0;

    G4ThreeVector dir = (p2-p1).unit();

    double approach = 0.0;
    for (int p = point1+1; p<point2; ++p) {
        p2 = g4Traj->GetPoint(p)->GetPosition() - p1;
        approach = std::max((p2 - (dir*p2)*dir).mag(), approach);
    }

    return approach;
}



int ND280PersistencyManager::SplitTrajectory(
    G4VTrajectory* g4Traj, int point1, int point2) {

  //G4cout << "ND280PersistencyManager::SplitTrajectory" << G4endl;

    int point3 = 0.5*(point1 + point2);
    if (point3 <= point1){
	//G4Exception("Points too close to split");
	G4ExceptionDescription msg;
	msg << "Points too close to split" << G4endl; 
	G4Exception("ND280Persistency::SplitTrajectory()",
		    "ExN02Code001", JustWarning, msg);
    } 
    
    if (point2 <= point3){
      //G4Exception("Points too close to split");
      G4ExceptionDescription msg;
      msg << "Points too close to split" << G4endl; 
      G4Exception("ND280Persistency::SplitTrajectory()",
		  "ExN02Code001", JustWarning, msg);
    }
    double bestAccuracy = FindTrajectoryAccuracy(g4Traj, point1, point3);

    bestAccuracy = std::max(FindTrajectoryAccuracy(g4Traj, point3, point2),
                            bestAccuracy);
    for (int p = point1+1; p<point2-1; ++p) {
        double a1 = FindTrajectoryAccuracy(g4Traj,point1, p);
        double a2 = FindTrajectoryAccuracy(g4Traj,p, point2);
        double accuracy = std::max(a1,a2);
        if (accuracy < bestAccuracy) {
            point3 = p;
            bestAccuracy  = accuracy;
        }
    }

    return point3;
}

void ND280PersistencyManager::SelectTrajectoryPoints(
    std::vector<int>& selected, G4VTrajectory* g4Traj) {

  //G4cout << "ND280PersistencyManager::SelectTrajectoryPoints" << G4endl;
    
    selected.clear();
    if (g4Traj->GetPointEntries() < 1) return;

    ////////////////////////////////////
    // Save the first point of the trajectory.
    ////////////////////////////////////
    selected.push_back(0);

    /////////////////////////////////////
    // Save the last point of the trajectory.
    /////////////////////////////////////
    int lastIndex = g4Traj->GetPointEntries()-1;
    if (lastIndex < 1) return;
    selected.push_back(lastIndex);

    //////////////////////////////////////////////
    // Short out trajectories that don't deposit any energy. 
    //
    // This eliminates neutrals, and we may need to add some special handling
    // for photons and possibly neutrons.
    // 
    //////////////////////////////////////////////
    ND280Trajectory* ndTraj = dynamic_cast<ND280Trajectory*>(g4Traj);    
    if (ndTraj->GetSDEnergyDeposit() < 1*eV) return;

    // Find the trajectory points where particles are entering and leaving the
    // detectors.
    ND280TrajectoryPoint* nd280Point 
        = dynamic_cast<ND280TrajectoryPoint*>(g4Traj->GetPoint(0));
    
    G4String prevVolumeName = nd280Point->GetPhysVolName();
    
    for (int tp = 1; tp < lastIndex; ++tp) {
        nd280Point = dynamic_cast<ND280TrajectoryPoint*>(g4Traj->GetPoint(tp));
        
	G4String volumeName = nd280Point->GetPhysVolName();
	
        // Save the point on a boundary crossing for volumes where we are
        // saving the entry and exit points.
        if (SaveTrajectoryBoundary(g4Traj,nd280Point->GetStepStatus(),
                                   volumeName,prevVolumeName)) {
            selected.push_back(tp);
        }

        prevVolumeName = volumeName;
    }

    double desiredAccuracy = GetTrajectoryPointAccuracy();
    // Make sure that the trajectory accuracy stays in tolerance.
    for (int throttle = 0; throttle < 1000; ++throttle) {
        bool addPoint = false;
        for (std::vector<int>::iterator p1 = selected.begin();
             p1 != selected.end();
             ++p1) {
            std::vector<int>::iterator p2 = p1+1;
            if (p2==selected.end()) break;
            double trajectoryAccuracy = FindTrajectoryAccuracy(g4Traj,*p1,*p2);
            if (trajectoryAccuracy <= desiredAccuracy) continue;
            int split = SplitTrajectory(g4Traj,*p1,*p2);
            if (split < 0) continue;
            selected.push_back(split);
            addPoint = true;
            break;
        }
        std::sort(selected.begin(), selected.end());
        selected.erase(std::unique(selected.begin(), selected.end()), 
                       selected.end());
        if (!addPoint) break;
    }
}
