////////////////////////////////////////////////////////////
// $Id: ND280RootPersistencyManager.hh,v 1.31 2011/09/06 18:58:35 mcgrew Exp $
// 
#ifndef ND280RootPersistencyManager_hh_seen
#define ND280RootPersistencyManager_hh_seen
 
#include <string>
#include <vector>

class TFile;
class TTree;
class TGeoManager;
class TGeoVolume;
class TGeoMedium;

class G4TrajectoryContainer;
class G4PrimaryVertex;
class G4VHitsCollection;

#include "ND280PersistencyManager.hh"

#include <TND280UpEvent.hh>
#include <TND280UpVertex.hh>
#include <TND280UpTrack.hh>
#include <TND280UpTrackPoint.hh>

/// Provide a root output for the geant 4 events.
class ND280RootPersistencyManager : public ND280PersistencyManager {
public:
  virtual ~ND280RootPersistencyManager();
  
  /// If a persistency manager has not been created, create one.
  static ND280RootPersistencyManager* GetInstance(void);
  
  /// Return true if the ROOT output file is active.  This means that the
  /// output file is open and ready to accept data.
  bool IsOpen();
  
  /// Return a pointer to the current TFile.
  TFile* GetTFile() const {return fOutput;}
  
  /// Return a pointer to the TTree
  TTree* GetTTree() const {return fEventTree;}

  /// Stores an event to the output file.
  virtual G4bool Store(const G4Event* anEvent);
  virtual G4bool Store(const G4Run* aRun);
  virtual G4bool Store(const G4VPhysicalVolume* aWorld);

  /// Retrieve information from a file.  This is not implemented.
  virtual G4bool Retrieve(G4Event *&aEvent) {return false;}
  virtual G4bool Retrieve(G4Run* &aRun) {return false;}
  virtual G4bool Retrieve(G4VPhysicalVolume* &aWorld) {return false;};
  
  /// Interface with PersistencyMessenger (open and close the
  /// database).
  virtual G4bool Open(G4String dbname);
  virtual G4bool Close(void);
  
  /// Copy the G4 Trajectories into an output event.
  //void CopyTrajectories(ND::TND280Event& dest, 
  //const G4TrajectoryContainer* src);
  
  /// Copy the G4 Trajectory points into an output event.
  //void CopyTrajectoryPoints(ND::TG4Trajectory& points,
  //G4VTrajectory* g4Traj);
  
  /// Copy the G4 Trajectories into an output event.
  //ND::TG4PrimaryVertexContainer* MakePrimary(const char* name,
  //const char* title,
  //const G4PrimaryVertex* src);
  
  /// Copy the off axis tracker hits to the output object.
  //void CopyHitCollections(ND::TND280Event& dest, const G4Event* src);
  
protected:
  /// use GetPersistencyManager() instead
  ND280RootPersistencyManager();
  
  /// Find the right color for a logical volume.  This won't necessarily be
  /// the same color as set in G4, but should be a reasonable choice for a
  /// detector picture.  This will return a negative number if the volume
  /// should be invisible.
  //int GetVolumeColor(const G4VPhysicalVolume* vol);
  
  /// Clean out the hit contributor list of contributors that don't have an
  /// associated trajectory in the trajectory container.  This takes a of
  /// trajectories parents (indexed by trajectory number), the container of
  /// trajectories saved in the output event, and iterators to the start and
  /// finish of a current contributor list.  It returns an iterator to one
  /// past the new end of the contributor list.  This gets used like this:
  /// \code
  /// std::vector<Int_t>::iterator e =
  ///        CleanHitContributors(parentMap,trajCon,
  ///                             contrib.begin(), contrib.end());
  /// contrib.erase(e,contrib.end());
  /// \endcode
  //std::vector<Int_t>::iterator CleanHitContributors(
  //std::map<int,int>& parentMap,
  //const ND::TG4TrajectoryContainer& trajCon,
  //std::vector<Int_t>::iterator start, 
  //std::vector<Int_t>::iterator finish);
  
  /// Make a map between each trajectory and it's parent trajectory.  If a
  /// trajectory came from a primary particle, it's parent has a track id of
  /// zero.  The map is "std::map<trackId,parentId>" so that 
  /// \code
  /// int parentId = parentMap[trackId];
  /// \endcode
  //void FillParentMap(std::map<int,int>& parentMap, const G4Event* src);
  
  /// Copy the hits from a G4VHitCollection into an output
  /// ND::TG4HitContainer object, but only if the g4Hits contain objects of
  /// type ND280HitSegment.  If the G4VHitCollection has the wrong type of
  /// hits, this will return NULL.
  //ND::TG4HitContainer* CreateHitSegmentContainer(
  //std::map<int,int>& parentMap,
  //const ND::TG4TrajectoryContainer& trajCon,
  //G4VHitsCollection* g4Hits,
  //std::string hitsName,
  //ND::TND280Event& dest);

  /// Make the MC Header and add it to truth.
  //void MakeMCHeader(ND::TND280Event& dest, const G4Event* src);

private:
  /// The ROOT output file that events are saved into.
  TFile *fOutput;
  
  /// The event tree that contains the output events.
  TTree *fEventTree;
  
  /// The TTree branches
  TND280UpEvent *fND280UpEvent; // The ND280 Upgrade Event class

  G4int fEventID; 
  
  /// The number of events saved to the output file since the last write.
  int fEventsNotSaved;
  
};
#endif
