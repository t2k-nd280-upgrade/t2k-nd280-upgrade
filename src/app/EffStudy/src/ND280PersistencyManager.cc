//
// Implementation for concrete G4PersistencyManager.
//

#include <G4ios.hh>
#include <G4Event.hh>
#include <G4PrimaryVertex.hh>
#include <G4PrimaryParticle.hh>
#include <G4StepStatus.hh>

#include <G4SystemOfUnits.hh>

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

// Creator for the persistency manager.  This is NOT a public routine
// since it is only used by this class to create a singleton object.
ND280PersistencyManager::ND280PersistencyManager() 
  : G4VPersistencyManager(), fFilename("/dev/null"),
    fLengthThreshold(2*cm),
    fGammaThreshold(50*MeV), fNeutronThreshold(50*MeV),
    fTrajectoryPointAccuracy(1*cm), fSaveAllPrimaryTrajectories(false) {
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

void ND280PersistencyManager::MarkTrajectories(const G4Event* event) {

    const G4TrajectoryContainer* trajectories = event->GetTrajectoryContainer();
    if (!trajectories) {
      //ND280Verbose("No Trajectories ");
      return;
    }
    
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
    for (TrajectoryVector::iterator t = trajectories->GetVector()->begin();
         t != trajectories->GetVector()->end();
         ++t) {
        ND280Trajectory* ndTraj = dynamic_cast<ND280Trajectory*>(*t);
        std::string particleName = ndTraj->GetParticleName();
        std::string processName = ndTraj->GetProcessName();
        double initialMomentum = ndTraj->GetInitialMomentum().mag();

        // Check if all primary particle trajectories should be saved.  The
        // primary particle should always be saved if it, or any of it's
        // children, deposited energy in a sensitive detector.  If the primary
        // didn't deposit energy in a sensitive detector, then it will only be
        // saved if SaveAllPrimaryTrajectories is true.
        if (ndTraj->GetParentID() == 0) {
            if (ndTraj->GetSDTotalEnergyDeposit()>1*eV 
                || GetSaveAllPrimaryTrajectories()) {
                ndTraj->MarkTrajectory();
                continue;
            }
        }

        // Don't save the neutrinos
        if (particleName == "anti_nu_e") continue;
        if (particleName == "anti_nu_mu") continue;
        if (particleName == "anti_nu_tau") continue;
        if (particleName == "nu_e") continue;
        if (particleName == "nu_mu") continue;
        if (particleName == "nu_tau") continue;

        // Save any decay product
        if (processName == "Decay") {
            ndTraj->MarkTrajectory(false);
            continue;
        }

        // Save particles that produce a signal in a sensitive detector.  This
        // doesn't automatically save, but the parents will be automatically
        // considered for saving by the next bit of code.
        if (ndTraj->GetSDLength() > GetLengthThreshold()) {
            ndTraj->MarkTrajectory(false);
            continue;
        }

        // For the next part, only consider particles where the children have
        // deposited energy in a sensitive detector.
        if (ndTraj->GetSDTotalEnergyDeposit()<1*eV) continue;

        // Save higher energy gamma rays that have descendents depositing
        // energy in a sensitive detector.
        if (particleName == "gamma" && initialMomentum > GetGammaThreshold()) {
            ndTraj->MarkTrajectory(false);
            continue;
        }

        // Save higher energy neutrons that have descendents depositing energy
        // in a sensitive detector.
        if (particleName == "neutron" 
            && initialMomentum > GetNeutronThreshold()) {
            ndTraj->MarkTrajectory(false);
            continue;
        }
    }

    // Go through all of the event hit collections and make sure that all
    // primary trajectories and trajectories contributing to a hit are saved.
    // These are mostly a sub-set of the trajectories marked in the previous
    // step, but there are a few corner cases where trajectories are not saved
    // because of theshold issues.
    
    // G4HCofThisEvent* hitCollections = event->GetHCofThisEvent();
    // if (!hitCollections) return;
    // for (int i=0; i < hitCollections->GetNumberOfCollections(); ++i) {
    //     G4VHitsCollection* g4Hits = hitCollections->GetHC(i);
    //     if (g4Hits->GetSize()<1) continue;
    //     for (unsigned int h=0; h<g4Hits->GetSize(); ++h) {
    //         ND280HitSegment* g4Hit
    //             = dynamic_cast<ND280HitSegment*>(g4Hits->GetHit(h));
    //         if (!g4Hit) {
    //             ND280Severe("Not a hit segment");
    //             continue;
    //         }

    G4HCofThisEvent* hitCollections = event->GetHCofThisEvent();
    if (!hitCollections) return;
    
    //for (int i=0; i < hitCollections->GetNumberOfCollections(); ++i) {
    //G4VHitsCollection* g4Hits = hitCollections->GetHC(i);

    // Get hits collections 
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    G4int fHHC1ID = sdManager->GetCollectionID("trackerCollection");
    
    ExN02TrackerHitsCollection* hHC1 
      = static_cast<ExN02TrackerHitsCollection*>(hitCollections->GetHC(fHHC1ID));
    if ( (!hHC1) ) {
      G4ExceptionDescription msg;
      msg << "Some of hits collections of this event not found." << G4endl; 
      G4Exception("ND280Persistency::MarkTrajectories()",
		  "ExN02Code001", FatalException, msg);
      return;
    }   

    G4int n_hit = hHC1->entries();
    for (G4int i=0;i<n_hit;i++){
      ExN02TrackerHit* hit = (*hHC1)[i];
      
      // Make sure that the primary trajectory associated with this hit
      // is saved.  The primary trajectories are defined by
      // ND280TrajectoryMap::FindPrimaryId().
      int primaryId = hit->GetTrackID();
      ND280Trajectory* ndTraj 
	= dynamic_cast<ND280Trajectory*>(
					 ND280TrajectoryMap::Get(primaryId));
      // primaryId is the track ID (see nd280mc ND280TrajectoryMap)
      
      if (ndTraj) ndTraj->MarkTrajectory(false);
      else{
	G4ExceptionDescription msg;
	msg << "Primary trajectory not found" << G4endl; 
	G4Exception("ND280Persistency::MarkTrajectories()",
		    "ExN02Code001", FatalException, msg);
      } 
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
