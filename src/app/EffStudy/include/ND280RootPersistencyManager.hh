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
#include "ExN02ND280XML.hh"

#include <TND280UpEvent.hh>
#include <TND280UpVertex.hh>
#include <TND280UpTrack.hh>
#include <TND280UpTrackPoint.hh>

#include "G4Navigator.hh"
#include "G4NavigationHistory.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include <ND280PhysicalVolumeInformation.hh>

#include <TH2F.h> 

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
    
  // Open input XML configuration file
  void OpenXML(G4String filename);
  ExN02ND280XML *GetXMLInput(){return fND280XMLInput;};
  /////

  // 
  // Attach the Navigator to this singleton class to reach it from everywhere 
  // Needed to compute the distance of the hit in the target 
  // (WAGASCI, SuperFGD, etc...) in the light read-out reference
  // system, i.e. the mother volume of the Target (see DetectorConstruction)
  //
  void InitNavigator(G4VPhysicalVolume *logvolume,G4ThreeVector position);

  G4Navigator *fNavigTarg1;
  G4Navigator *GetNavigTarg1(){return fNavigTarg1;};  

  const G4NavigationHistory *fNavigHistoTarg1;
  const G4NavigationHistory *GetNavigHistoTarg1(){return fNavigHistoTarg1;};  

  G4String GetNavigHistoVolName();
  
  G4bool fIsHistoMovedTarg1;
  void   SetHistoMovedTarg1(bool val){fIsHistoMovedTarg1=val;};
  G4bool GetHistoMovedTarg1(){return fIsHistoMovedTarg1;};
  
  G4String fNavigDetName_Targ1;
  void SetNavigDetName_Targ1(G4String name);
  G4String GetNavigDetName_Targ1(){return fNavigDetName_Targ1;};
  G4bool fNavigDetExist;
  G4bool doNavigDetExist(){return fNavigDetExist;};

  G4ThreeVector GetLocalPosition(G4ThreeVector WorldPos);

  // Define the position of the Target MPPCs  
  // Same reference system as the navigator 
  // i.e. set in DetectorConstruction
  //

  TH2F *fMPPCProj2D_XY;
  TH2F *fMPPCProj2D_XZ;
  TH2F *fMPPCProj2D_YZ;

  TH2F *GetMPPCProj2D_XY();
  TH2F *GetMPPCProj2D_XZ();
  TH2F *GetMPPCProj2D_YZ();

  // Assume the MPPC read-out plane is at x,y,z<0
  // Assume at least 2 projections exist
  G4double GetMPPCPosX();
  G4double GetMPPCPosY();
  G4double GetMPPCPosZ();

  void InitMPPCProj2D(double width, double height, double length, double numX, double numY, double numZ, bool IsProjXY, bool IsProjXZ, bool IsProjYZ); 

  G4bool fIsMPPCProjXY;  
  void SetIsMPPCProjXY(bool isproj){fIsMPPCProjXY=isproj;}
  G4bool GetIsMPPCProjXY(){return fIsMPPCProjXY;}

  G4bool fIsMPPCProjXZ;  
  void SetIsMPPCProjXZ(bool isproj){fIsMPPCProjXZ=isproj;}
  G4bool GetIsMPPCProjXZ(){return fIsMPPCProjXZ;}

  G4bool fIsMPPCProjYZ;  
  void SetIsMPPCProjYZ(bool isproj){fIsMPPCProjYZ=isproj;}
  G4bool GetIsMPPCProjYZ(){return fIsMPPCProjYZ;}

  // Position of the MPPC hits
  void GetHitPosXY(double lightX, double lightY, double &mppcX, double &mppcY);
  void GetHitPosXZ(double lightX, double lightZ, double &mppcX, double &mppcZ);
  void GetHitPosYZ(double lightY, double lightZ, double &mppcY, double &mppcZ);
  
  G4String fDetNameAlongX;
  G4String fDetNameAlongY;
  G4String fDetNameAlongZ;
  void SetDetNameAlongX(G4String name){fDetNameAlongX=name;};
  void SetDetNameAlongY(G4String name){fDetNameAlongY=name;};
  void SetDetNameAlongZ(G4String name){fDetNameAlongZ=name;};
  G4String GetDetNameAlongX(){return fDetNameAlongX;};
  G4String GetDetNameAlongY(){return fDetNameAlongY;};
  G4String GetDetNameAlongZ(){return fDetNameAlongZ;};

  //B.Q
  ND280PhysicalVolumeInformation * PVInfo;
  vector <ND280PhysicalVolumeInformation> vPVInfo;
  void AddPhysicalVolumeInformation(G4int, G4String, G4ThreeVector);
  vector <ND280PhysicalVolumeInformation> GetPhysicalVolumeInformation(){return vPVInfo;};
  ///

  /////
  
  void SetEventFirst(int first){fEventFirst=first;};
  void SetNEvents(int num){fNEvents=num;};

  int GetEventFirst(){return fEventFirst;};
  int GetNEvents(){return fNEvents;};

  ExN02ND280XML *fND280XMLInput;

  G4int fEventFirst;
  G4int fNEvents;

  /////






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
