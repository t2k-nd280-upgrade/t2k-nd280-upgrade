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
// $Id: ExN02EventAction.hh,v 1.8 2006-06-29 17:47:35 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#ifndef ExN02EventAction_h
#define ExN02EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "G4Step.hh"
#include "G4Track.hh"

#include <vector>

class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02EventAction : public G4UserEventAction
{
public:
  ExN02EventAction();
  virtual ~ExN02EventAction();
  
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);
  
  void AddAbsTPCup(G4double de, G4double dl);
  void AddAbsTPCdown(G4double de, G4double dl);
  void AddAbsTarget(G4double de, G4double dl);
  void AddAbsTot(G4double de, G4double dl);
  
  void SetTrack(G4Track *track);
  
  // Global informations
  std::vector<G4int>   & GetVecTrackID  () {return fVecTrackID;};
  std::vector<G4int>   & GetVecTrackPDG () {return fVecTrackPDG;};
  std::vector<G4double>& GetVecTrackE   () {return fVecTrackE;};
  std::vector<G4double>& GetVecTrackMomX() {return fVecTrackMomX;};
  std::vector<G4double>& GetVecTrackMomY() {return fVecTrackMomY;};
  std::vector<G4double>& GetVecTrackMomZ() {return fVecTrackMomZ;};
  std::vector<G4double>& GetVecTrackMomMag() {return fVecTrackMomMag;};

  // TPC Up informations
  G4int               GetTPCUp_NTracks() { return fTPCUp_NTracks;};          
  std::vector<G4int>   & GetVecTPCUp_TrackID() {return fVecTPCUp_TrackID;}; 
  std::vector<G4double>& GetVecTPCUp_TrackMomX(){return fVecTPCUp_TrackMomX;}; 
  std::vector<G4double>& GetVecTPCUp_TrackMomY(){return fVecTPCUp_TrackMomY;}; 
  std::vector<G4double>& GetVecTPCUp_TrackMomZ(){return fVecTPCUp_TrackMomZ;}; 
  std::vector<G4double>& GetVecTPCUp_TrackLength(){return fVecTPCUp_TrackLength;}; 
  std::vector<G4double>& GetVecTPCUp_TrackEdep(){return fVecTPCUp_TrackEdep;}; 
  std::vector<G4double>& GetVecTPCUp_TrackCharge(){return fVecTPCUp_TrackCharge;}; 

  // TPC Down informations
  G4int               GetTPCDown_NTracks() { return fTPCDown_NTracks;};          
  std::vector<G4int>   & GetVecTPCDown_TrackID() {return fVecTPCDown_TrackID;}; 
  std::vector<G4double>& GetVecTPCDown_TrackMomX(){return fVecTPCDown_TrackMomX;}; 
  std::vector<G4double>& GetVecTPCDown_TrackMomY(){return fVecTPCDown_TrackMomY;}; 
  std::vector<G4double>& GetVecTPCDown_TrackMomZ(){return fVecTPCDown_TrackMomZ;}; 
  std::vector<G4double>& GetVecTPCDown_TrackLength(){return fVecTPCDown_TrackLength;}; 
  std::vector<G4double>& GetVecTPCDown_TrackEdep(){return fVecTPCDown_TrackEdep;}; 
  std::vector<G4double>& GetVecTPCDown_TrackCharge(){return fVecTPCDown_TrackCharge;}; 

  // Target informations
  G4int               GetTarget_NTracks() { return fTarget_NTracks;};          
  std::vector<G4int>   & GetVecTarget_TrackID() {return fVecTarget_TrackID;}; 
  std::vector<G4double>& GetVecTarget_TrackMomX(){return fVecTarget_TrackMomX;}; 
  std::vector<G4double>& GetVecTarget_TrackMomY(){return fVecTarget_TrackMomY;}; 
  std::vector<G4double>& GetVecTarget_TrackMomZ(){return fVecTarget_TrackMomZ;}; 
  std::vector<G4double>& GetVecTarget_TrackLength(){return fVecTarget_TrackLength;}; 
  std::vector<G4double>& GetVecTarget_TrackEdep(){return fVecTarget_TrackEdep;}; 
  std::vector<G4double>& GetVecTarget_TrackCharge(){return fVecTarget_TrackCharge;}; 

  // Vertex informations
  G4int               GetNVtx() { return fNVtx;};
  std::vector<G4double>& GetVecVtx_X(){return fVecVtx_X;}; 
  std::vector<G4double>& GetVecVtx_Y(){return fVecVtx_Y;}; 
  std::vector<G4double>& GetVecVtx_Z(){return fVecVtx_Z;}; 
  std::vector<G4int>   & GetVecVtx_NuPDG()    {return fVecVtx_NuPDG;}; 
  std::vector<G4int>   & GetVecVtx_ReacMode() {return fVecVtx_ReacMode;}; 
  std::vector<G4double>& GetVecVtx_EvtProb()  {return fVecVtx_EvtProb;}; 
  std::vector<G4double>& GetVecVtx_EvtWeight(){return fVecVtx_EvtWeight;}; 

private:

  // Hits
  G4int     fHHC1ID;           // Hit Collection 1: Full Tracker

  // Global informations
  G4double  fEnergyAbsTPCup;   // Total energy absorbed by TPC up
  G4double  fEnergyAbsTPCdown; // Total energy absorbed by TPC down
  G4double  fEnergyAbsTarget;  // Total energy absorbed by Target
  G4double  fEnergyAbsTot;     // Total energy absorbed in the detector
  G4double  fTrackLAbsTPCup;   // Length of track absorbed in TPC up
  G4double  fTrackLAbsTPCdown; // Length of track absorbed in TPC down
  G4double  fTrackLAbsTarget;  // Length of track absorbed in Target
  G4double  fTrackLAbsTot;     // Total length of track absorbed in the detector
  G4int     fNTracks;          // # of tracks in the event
  
  std::vector<G4int>    fVecTrackID;   // Vector of ID of each track
  std::vector<G4int>    fVecTrackPDG;  // Vector of PDG of each track
  std::vector<G4double> fVecTrackE;    // Vector of initial energy of each track
  std::vector<G4double> fVecTrackMomX; // Vector of initial momentum X
  std::vector<G4double> fVecTrackMomY; // Vector of initial momentum Y
  std::vector<G4double> fVecTrackMomZ; // Vector of initial momentum Z
  std::vector<G4double> fVecTrackMomMag; // Vector of initial momentum Z
  
  // TPC up informations
  G4int     fTPCUp_NTracks;          // # of tracks in TPC Up
  std::vector<G4int>    fVecTPCUp_TrackID; // Vector of trackID in TPCup
  std::vector<G4double> fVecTPCUp_TrackMomX; // Vector of initial mom in TPC Up
  std::vector<G4double> fVecTPCUp_TrackMomY; // Vector of initial mom in TPC Up
  std::vector<G4double> fVecTPCUp_TrackMomZ; // Vector of initial mom in TPC Up
  std::vector<G4double> fVecTPCUp_TrackLength; // Vector of track length in TPC Up
  std::vector<G4double> fVecTPCUp_TrackEdep; // Vector of track edep in TPC Up
  std::vector<G4double> fVecTPCUp_TrackCharge; // Vector of track charge in TPC Up
  
  // TPC down informations
  G4int     fTPCDown_NTracks;          // # of tracks in TPC Down
  std::vector<G4int>    fVecTPCDown_TrackID; // Vector of trackID in TPCDown
  std::vector<G4double> fVecTPCDown_TrackMomX; // Vector of initial mom in TPC Down
  std::vector<G4double> fVecTPCDown_TrackMomY; // Vector of initial mom in TPC Down
  std::vector<G4double> fVecTPCDown_TrackMomZ; // Vector of initial mom in TPC Down
  std::vector<G4double> fVecTPCDown_TrackLength; // Vector of track length in TPC Down
  std::vector<G4double> fVecTPCDown_TrackEdep; // Vector of track edep in TPC Down
  std::vector<G4double> fVecTPCDown_TrackCharge; // Vector of track charge in TPC Down

  // Target informations
  G4int     fTarget_NTracks;          // # of tracks in Target
  std::vector<G4int>    fVecTarget_TrackID; // Vector of trackID in Target
  std::vector<G4double> fVecTarget_TrackMomX; // Vector of initial mom in Target
  std::vector<G4double> fVecTarget_TrackMomY; // Vector of initial mom in Target 
  std::vector<G4double> fVecTarget_TrackMomZ; // Vector of initial mom in Target
  std::vector<G4double> fVecTarget_TrackLength; // Vector of track length in Target
  std::vector<G4double> fVecTarget_TrackEdep; // Vector of track edep in Target
  std::vector<G4double> fVecTarget_TrackCharge; // Vector of track charge in Target

  // Vertex informations
  G4int     fNVtx;
  std::vector<G4double> fVecVtx_X; // Vector of vertex X position
  std::vector<G4double> fVecVtx_Y; // Vector of vertex Y position
  std::vector<G4double> fVecVtx_Z; // Vector of vertex Z position
  std::vector<G4int>    fVecVtx_NuPDG;     // Vector of vertex Nu PDG
  std::vector<G4int>    fVecVtx_ReacMode;  // Vector of vertex reaction mode
  std::vector<G4double> fVecVtx_EvtProb;   // Vector of vertex event prob
  std::vector<G4double> fVecVtx_EvtWeight; // Vector of vertex event weight
};


// inline functions 
inline void ExN02EventAction::AddAbsTPCup(G4double de, G4double dl) {
  fEnergyAbsTPCup += de;
  fTrackLAbsTPCup += dl;
}

inline void ExN02EventAction::AddAbsTPCdown(G4double de, G4double dl) {
  fEnergyAbsTPCdown += de;
  fTrackLAbsTPCdown += dl;
}

inline void ExN02EventAction::AddAbsTarget(G4double de, G4double dl) {
  fEnergyAbsTarget += de;
  fTrackLAbsTarget += dl;
}

inline void ExN02EventAction::AddAbsTot(G4double de, G4double dl) {
  fEnergyAbsTot += de;
  fTrackLAbsTot += dl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
