////////////////////////////////////////////////////////////
// $Id: ND280UserTrackingAction.cc,v 1.6 2007/01/01 05:32:49 mcgrew Exp $
//

#include "globals.hh"
#include "G4Track.hh"
#include "G4TrackingManager.hh"

#include "ND280UserTrackingAction.hh"
#include "ND280Trajectory.hh"
#include "ND280TrajectoryMap.hh"

ND280UserTrackingAction::ND280UserTrackingAction() {}

ND280UserTrackingAction::~ND280UserTrackingAction() {}

void ND280UserTrackingAction::PreUserTrackingAction(const G4Track* trk) {
    G4VTrajectory* traj = new ND280Trajectory(trk);
    fpTrackingManager->SetTrajectory(traj);
    fpTrackingManager->SetStoreTrajectory(true);
    ND280TrajectoryMap::Add(traj);
    
    //G4cout << traj->GetPointEntries() << G4endl; //GetRange();
}

