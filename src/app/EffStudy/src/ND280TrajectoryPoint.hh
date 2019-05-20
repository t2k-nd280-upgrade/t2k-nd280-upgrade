#ifndef ND280TrajectoryPoint_hh_seen
#define ND280TrajectoryPoint_hh_seen

#include <globals.hh>
#include <G4TrajectoryPoint.hh>
#include <G4ThreeVector.hh>
#include <G4Allocator.hh>
#include <G4StepStatus.hh>

class G4Track;
class G4Step;
class G4VProcess;

/// Store a point along a particle trajectory.  This is used internally for
/// some bookkeeping, and then is translated into an output class when the
/// event is saved.
class ND280TrajectoryPoint : public G4TrajectoryPoint {
public:
  ND280TrajectoryPoint();
  ND280TrajectoryPoint(const G4Track* aTrack);
  ND280TrajectoryPoint(const G4Step* aStep);
  ND280TrajectoryPoint(const ND280TrajectoryPoint &right);
  virtual ~ND280TrajectoryPoint();
  
  inline void *operator new(size_t);
  inline void operator delete(void *aND280TrajectoryPoint);
  inline int operator==(const ND280TrajectoryPoint& right) const
  { return (this==&right); };
  
  /// Get the time that the particle passed this trajectory point.
  G4double GetTime() const { return fTime; }
  
  /// Get the 3-momentum of the particle at this trajectory point.
  const G4ThreeVector GetMomentum() const { return fMomentum; }
  
  /// Get the deposited energy of the particle at this trajectory point.
  G4double GetEdep() const { return fEdep; }

  /// Get the step length at this trajectory point.
  G4double GetStepLength() const { return fStepLength; }

  /// Get the step length on yz plane (MicroMegas) at this trajectory point.
  G4double GetStepDeltaLyz() const { return fStepDeltaLyz; }

  /// Get the G4 stepping status of the interaction that instigated this
  /// trajectory point.
  G4StepStatus GetStepStatus() const { return fStepStatus; }
  
  /// The name of the physical volume containing the stopping point of the
  /// current step.  This may (often) be a different volume than the volume
  /// referenced by GetVolumeNode().
  G4String GetPhysVolName() const { return fPhysVolName; }
  
  // /// Get the node for the volume containing the stopping point.  If the
  // /// stopping point is on a geometric boundary, this is the volume that the
  // /// track is just exiting. 
  // int GetVolumeNode() const; // NOT IMPLEMENTED
  
  // Get method for HEPRep style attributes
  
  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;
  

  // MY METHODS
  G4ThreeVector GetPrevPosition() const { return fPrevPosition;};
  G4ThreeVector GetPostPosition() const { return fPostPosition;};
  G4bool IsOnBoundary() const { return fIsOnBoundary;};
  G4bool SavePoint() const { return fSavePoint;};
  G4String GetLogVolName() const { return fLogVolName; }

  /// Mark this point as one that should be saved in the output.  
  void MarkPoint();
  
  
  //---------
private:
  //---------
  
  // Member data
  
  G4double fTime;
  G4ThreeVector fMomentum;
  G4double fEdep;
  G4double fStepLength;
  G4double fStepDeltaLyz;
  G4StepStatus fStepStatus;
  G4bool fIsOnBoundary;
  G4String fPhysVolName;
  G4String fLogVolName;
  G4ThreeVector fPrevPosition;

  // MY MEMBERS
  G4ThreeVector fPostPosition;
  G4bool fSavePoint;
  
};

#if defined G4TRACKING_ALLOC_EXPORT
extern G4DLLEXPORT G4Allocator<ND280TrajectoryPoint> aND280TrajPointAllocator;
#else
extern G4DLLIMPORT G4Allocator<ND280TrajectoryPoint> aND280TrajPointAllocator;
#endif

inline void* ND280TrajectoryPoint::operator new(size_t) {
  void *aTrajectoryPoint = (void *) aND280TrajPointAllocator.MallocSingle();
  return aTrajectoryPoint;
}

inline void ND280TrajectoryPoint::operator delete(void *aTrajectoryPoint) {
  aND280TrajPointAllocator.FreeSingle(
			       (ND280TrajectoryPoint *) aTrajectoryPoint);
}
#endif

