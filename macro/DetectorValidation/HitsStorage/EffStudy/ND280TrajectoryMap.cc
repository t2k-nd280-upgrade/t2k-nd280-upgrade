#include <G4VTrajectory.hh>
#include <G4VTrajectoryPoint.hh>
#include <G4ThreeVector.hh>
#include <ND280Trajectory.hh>

//#include <TND280Log.hxx>

#include "ND280TrajectoryMap.hh"

std::map<int, G4VTrajectory*> ND280TrajectoryMap::fMap;

void ND280TrajectoryMap::Clear() {
    fMap.clear();
}

void ND280TrajectoryMap::Add(G4VTrajectory* traj) {
        int trackId = traj->GetTrackID();
        fMap[trackId] = traj;
}

int ND280TrajectoryMap::FindPrimaryId(int trackId) {
    int currentId = trackId;
    int parentId = trackId;
    int loopCount=0;
    for (loopCount=0;loopCount<10000;++loopCount) {
        G4VTrajectory* t = Get(currentId);
        if (!t) break;
        parentId = t->GetParentID();
        // Check to see the search loop should terminate.
        G4VTrajectory* p = Get(parentId);
        // There is no parent so break so this is a primary trajectory.
        if (!p) break;
        // Decay products are primary trajectories since they should be
        // independently reconstructed
        ND280Trajectory * nd280Traj = dynamic_cast<ND280Trajectory*>(t);
        if (!nd280Traj){
	  //G4Exception("Invalid Trajectory");
	  G4ExceptionDescription msg;
	  msg << "Invalid Trajectory" << G4endl;
	  G4Exception("ND280TrajectoryMap::FindPrimaryId",
		      "MyCode0002",FatalException, msg);
	}
        if (nd280Traj->GetProcessName() == "Decay") break;
        // A parent ID of zero means that this particle is a primary particle,
        // so that makes it a primary trajectory too.
        if (parentId == 0) break;
        currentId = parentId;
    }
    if (loopCount>9999) {
      //ND280Log("Infinite Loop in ND280TrajectoryMap::FindPrimaryId(): "
      //<< "Track Id: " << trackId);
    }
    
    return currentId;
}

G4VTrajectory* ND280TrajectoryMap::Get(int trackId) {
    std::map<int,G4VTrajectory*>::iterator t = fMap.find(trackId);
    if (t == fMap.end()) {
        return NULL;
    }
    return t->second;
}

