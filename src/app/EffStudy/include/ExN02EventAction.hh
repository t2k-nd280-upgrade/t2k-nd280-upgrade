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
  
  std::vector<G4int>   & GetVecTrackID  () {return fVecTrackID;};
  std::vector<G4int>   & GetVecTrackPDG () {return fVecTrackPDG;};
  std::vector<G4double>& GetVecTrackE   () {return fVecTrackE;};
  std::vector<G4double>& GetVecTrackMomX() {return fVecTrackMomX;};
  std::vector<G4double>& GetVecTrackMomY() {return fVecTrackMomY;};
  std::vector<G4double>& GetVecTrackMomZ() {return fVecTrackMomZ;};
  std::vector<G4double>& GetVecTrackMomMag() {return fVecTrackMomMag;};
  
  std::vector<G4int>   & GetVecTPCUp_TrackID() {return fVecTPCUp_TrackID;}; 
  std::vector<G4double>& GetVecTPCUp_TrackMomX(){return fVecTPCUp_TrackMomX;}; 
  std::vector<G4double>& GetVecTPCUp_TrackMomY(){return fVecTPCUp_TrackMomY;}; 
  std::vector<G4double>& GetVecTPCUp_TrackMomZ(){return fVecTPCUp_TrackMomZ;}; 
  
  G4int               GetTPCUp_NTracks() { return fTPCUp_NTracks;};          

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
  std::vector<G4int>    fVecTPCUp_TrackID; // Vector of trackID in TPCup
  std::vector<G4double> fVecTPCUp_TrackMomX; // Vector of initial mom in TPC Up
  std::vector<G4double> fVecTPCUp_TrackMomY; // Vector of initial mom in TPC Up
  std::vector<G4double> fVecTPCUp_TrackMomZ; // Vector of initial mom in TPC Up
  
  G4int     fTPCUp_NTracks;          // # of tracks in TPC Up
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

    
