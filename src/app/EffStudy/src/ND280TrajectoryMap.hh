#ifndef ND280TrajectoryMap_hh_seen
#define ND280TrajectoryMap_hh_seen
////////////////////////////////////////////////////////////
// $Id: ND280TrajectoryMap.hh,v 1.1 2007/01/01 05:32:49 mcgrew Exp $
//

#include <map>

class G4VTrajectory;

/// Maintain a singleton map of track Id to the trajectory in the trajectory
/// container.
class ND280TrajectoryMap {
public:
    ~ND280TrajectoryMap() {}

    /// Provide a map between the track id and the trajectory object.
    static G4VTrajectory* Get(int trackId);

    /// Add a trajectory to the map.
    static void Add(G4VTrajectory* traj);

    /// Clear the trajectory map.  This must be done in the
    /// ND280UserEventAction::BeginOfEventAction() method.
    static void Clear();

    /// Find the primary track ID for the current track.  This is the primary
    /// that is the ultimate parent of the current track.
    static int FindPrimaryId(int trackId);

private:
    /// A map to the trajectories information indexed the the track id. Be
    /// careful since the trajectory information is owned by the event, so if
    /// you try to use this after a trajectory has been deleted... bad things
    /// will happen.
    static std::map<int,G4VTrajectory*> fMap;

    /// The constructor is private so that it can only be created using the
    /// static get method.
    ND280TrajectoryMap() {}
};
#endif
