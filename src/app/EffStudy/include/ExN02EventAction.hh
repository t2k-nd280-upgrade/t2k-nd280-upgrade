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
#include <TVector3.h>

class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02EventAction : public G4UserEventAction
{
public:
  ExN02EventAction();
  virtual ~ExN02EventAction();
  
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);

  void AddAbsForwTPC1(G4double de, G4double dl);
  void AddAbsTPCup1(G4double de, G4double dl);
  void AddAbsTPCdown1(G4double de, G4double dl);
  void AddAbsTarget1(G4double de, G4double dl);
  void AddAbsTPCup2(G4double de, G4double dl);
  void AddAbsTPCdown2(G4double de, G4double dl);
  void AddAbsTarget2(G4double de, G4double dl);
  void AddAbsTot(G4double de, G4double dl);
  
  void SetTrack(const G4Step *step);

  // Global informations
  std::vector<G4int>   & GetVecTrackID  () {return fVecTrackID;};
  std::vector<G4int>   & GetVecParentID () {return fVecParentID;};
  std::vector<G4int>   & GetVecTrackPDG () {return fVecTrackPDG;};
  std::vector<G4double>& GetVecTrackE   () {return fVecTrackE;};
  std::vector<G4double>& GetVecTrackMomX() {return fVecTrackMomX;};
  std::vector<G4double>& GetVecTrackMomY() {return fVecTrackMomY;};
  std::vector<G4double>& GetVecTrackMomZ() {return fVecTrackMomZ;};
  std::vector<G4double>& GetVecTrackMomMag() {return fVecTrackMomMag;};
  //std::vector<G4bool>& GetVecTrackIsTPCUp() {return fVecTrackIsTPCUp;};
  //std::vector<G4bool>& GetVecTrackIsTPCDown() {return fVecTrackIsTPCDown;};
  //std::vector<G4bool>& GetVecTrackIsTarget() {return fVecTrackIsTarget;};

  // Forward TPC 1 informations
  G4int               GetForwTPC1_NTracks() { return fForwTPC1_NTracks;};          
  std::vector<G4int>   & GetVecForwTPC1_TrackID()  {return fVecForwTPC1_TrackID;}; 
  std::vector<G4int>   & GetVecForwTPC1_ParentID() {return fVecForwTPC1_ParentID;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackMomX(){return fVecForwTPC1_TrackMomX;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackMomY(){return fVecForwTPC1_TrackMomY;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackMomZ(){return fVecForwTPC1_TrackMomZ;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackLength(){return fVecForwTPC1_TrackLength;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackDeltaLyz(){return fVecForwTPC1_TrackDeltaLyz;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackEdep(){return fVecForwTPC1_TrackEdep;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackCharge(){return fVecForwTPC1_TrackCharge;}; 
  std::vector<G4int>   & GetVecForwTPC1_TrackPDG(){return fVecForwTPC1_TrackPDG;};
  std::vector<G4double>& GetVecForwTPC1_TrackStepFirstX(){return fVecForwTPC1_TrackStepFirstX;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackStepFirstY(){return fVecForwTPC1_TrackStepFirstY;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackStepFirstZ(){return fVecForwTPC1_TrackStepFirstZ;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackStepLastX(){return fVecForwTPC1_TrackStepLastX;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackStepLastY(){return fVecForwTPC1_TrackStepLastY;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackStepLastZ(){return fVecForwTPC1_TrackStepLastZ;}; 
  std::vector<G4double>& GetVecForwTPC1_TrackCosTheta(){return fVecForwTPC1_TrackCosTheta;}; 

  // TPC Up 1 informations
  G4int               GetTPCUp1_NTracks() { return fTPCUp1_NTracks;};          
  std::vector<G4int>   & GetVecTPCUp1_TrackID()  {return fVecTPCUp1_TrackID;}; 
  std::vector<G4int>   & GetVecTPCUp1_ParentID() {return fVecTPCUp1_ParentID;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackMomX(){return fVecTPCUp1_TrackMomX;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackMomY(){return fVecTPCUp1_TrackMomY;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackMomZ(){return fVecTPCUp1_TrackMomZ;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackLength(){return fVecTPCUp1_TrackLength;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackDeltaLyz(){return fVecTPCUp1_TrackDeltaLyz;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackEdep(){return fVecTPCUp1_TrackEdep;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackCharge(){return fVecTPCUp1_TrackCharge;}; 
  std::vector<G4int>   & GetVecTPCUp1_TrackPDG(){return fVecTPCUp1_TrackPDG;};
  std::vector<G4double>& GetVecTPCUp1_TrackStepFirstX(){return fVecTPCUp1_TrackStepFirstX;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackStepFirstY(){return fVecTPCUp1_TrackStepFirstY;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackStepFirstZ(){return fVecTPCUp1_TrackStepFirstZ;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackStepLastX(){return fVecTPCUp1_TrackStepLastX;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackStepLastY(){return fVecTPCUp1_TrackStepLastY;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackStepLastZ(){return fVecTPCUp1_TrackStepLastZ;}; 
  std::vector<G4double>& GetVecTPCUp1_TrackCosTheta(){return fVecTPCUp1_TrackCosTheta;}; 

  // TPC Down 1 informations
  G4int               GetTPCDown1_NTracks() { return fTPCDown1_NTracks;};          
  std::vector<G4int>   & GetVecTPCDown1_TrackID() {return fVecTPCDown1_TrackID;}; 
  std::vector<G4int>   & GetVecTPCDown1_ParentID() {return fVecTPCDown1_ParentID;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackMomX(){return fVecTPCDown1_TrackMomX;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackMomY(){return fVecTPCDown1_TrackMomY;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackMomZ(){return fVecTPCDown1_TrackMomZ;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackLength(){return fVecTPCDown1_TrackLength;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackDeltaLyz(){return fVecTPCDown1_TrackDeltaLyz;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackEdep(){return fVecTPCDown1_TrackEdep;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackCharge(){return fVecTPCDown1_TrackCharge;}; 
  std::vector<G4int>   & GetVecTPCDown1_TrackPDG(){return fVecTPCDown1_TrackPDG;};
  std::vector<G4double>& GetVecTPCDown1_TrackStepFirstX(){return fVecTPCDown1_TrackStepFirstX;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackStepFirstY(){return fVecTPCDown1_TrackStepFirstY;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackStepFirstZ(){return fVecTPCDown1_TrackStepFirstZ;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackStepLastX(){return fVecTPCDown1_TrackStepLastX;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackStepLastY(){return fVecTPCDown1_TrackStepLastY;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackStepLastZ(){return fVecTPCDown1_TrackStepLastZ;}; 
  std::vector<G4double>& GetVecTPCDown1_TrackCosTheta(){return fVecTPCDown1_TrackCosTheta;}; 

  // TPC Up 2 informations
  G4int               GetTPCUp2_NTracks() { return fTPCUp2_NTracks;};          
  std::vector<G4int>   & GetVecTPCUp2_TrackID()  {return fVecTPCUp2_TrackID;}; 
  std::vector<G4int>   & GetVecTPCUp2_ParentID() {return fVecTPCUp2_ParentID;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackMomX(){return fVecTPCUp2_TrackMomX;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackMomY(){return fVecTPCUp2_TrackMomY;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackMomZ(){return fVecTPCUp2_TrackMomZ;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackLength(){return fVecTPCUp2_TrackLength;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackDeltaLyz(){return fVecTPCUp2_TrackDeltaLyz;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackEdep(){return fVecTPCUp2_TrackEdep;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackCharge(){return fVecTPCUp2_TrackCharge;}; 
  std::vector<G4int>   & GetVecTPCUp2_TrackPDG(){return fVecTPCUp2_TrackPDG;};
  std::vector<G4double>& GetVecTPCUp2_TrackStepFirstX(){return fVecTPCUp2_TrackStepFirstX;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackStepFirstY(){return fVecTPCUp2_TrackStepFirstY;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackStepFirstZ(){return fVecTPCUp2_TrackStepFirstZ;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackStepLastX(){return fVecTPCUp2_TrackStepLastX;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackStepLastY(){return fVecTPCUp2_TrackStepLastY;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackStepLastZ(){return fVecTPCUp2_TrackStepLastZ;}; 
  std::vector<G4double>& GetVecTPCUp2_TrackCosTheta(){return fVecTPCUp2_TrackCosTheta;}; 

  // TPC Down 2 informations
  G4int               GetTPCDown2_NTracks() { return fTPCDown2_NTracks;};          
  std::vector<G4int>   & GetVecTPCDown2_TrackID() {return fVecTPCDown2_TrackID;}; 
  std::vector<G4int>   & GetVecTPCDown2_ParentID() {return fVecTPCDown2_ParentID;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackMomX(){return fVecTPCDown2_TrackMomX;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackMomY(){return fVecTPCDown2_TrackMomY;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackMomZ(){return fVecTPCDown2_TrackMomZ;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackLength(){return fVecTPCDown2_TrackLength;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackDeltaLyz(){return fVecTPCDown2_TrackDeltaLyz;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackEdep(){return fVecTPCDown2_TrackEdep;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackCharge(){return fVecTPCDown2_TrackCharge;}; 
  std::vector<G4int>   & GetVecTPCDown2_TrackPDG(){return fVecTPCDown2_TrackPDG;};
  std::vector<G4double>& GetVecTPCDown2_TrackStepFirstX(){return fVecTPCDown2_TrackStepFirstX;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackStepFirstY(){return fVecTPCDown2_TrackStepFirstY;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackStepFirstZ(){return fVecTPCDown2_TrackStepFirstZ;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackStepLastX(){return fVecTPCDown2_TrackStepLastX;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackStepLastY(){return fVecTPCDown2_TrackStepLastY;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackStepLastZ(){return fVecTPCDown2_TrackStepLastZ;}; 
  std::vector<G4double>& GetVecTPCDown2_TrackCosTheta(){return fVecTPCDown2_TrackCosTheta;}; 

  // Target 1 informations
  G4int               GetTarget1_NTracks() { return fTarget1_NTracks;};          
  std::vector<G4int>   & GetVecTarget1_TrackID() {return fVecTarget1_TrackID;}; 
  std::vector<G4int>   & GetVecTarget1_ParentID() {return fVecTarget1_ParentID;}; 
  std::vector<G4double>& GetVecTarget1_TrackMomX(){return fVecTarget1_TrackMomX;}; 
  std::vector<G4double>& GetVecTarget1_TrackMomY(){return fVecTarget1_TrackMomY;}; 
  std::vector<G4double>& GetVecTarget1_TrackMomZ(){return fVecTarget1_TrackMomZ;}; 
  std::vector<G4double>& GetVecTarget1_TrackLength(){return fVecTarget1_TrackLength;}; 
  std::vector<G4double>& GetVecTarget1_TrackDeltaLyz(){return fVecTarget1_TrackDeltaLyz;}; 
  std::vector<G4double>& GetVecTarget1_TrackEdep(){return fVecTarget1_TrackEdep;}; 
  std::vector<G4double>& GetVecTarget1_TrackCharge(){return fVecTarget1_TrackCharge;}; 
  std::vector<G4int>   & GetVecTarget1_TrackPDG(){return fVecTarget1_TrackPDG;};
  std::vector<G4double>& GetVecTarget1_TrackStepFirstX(){return fVecTarget1_TrackStepFirstX;}; 
  std::vector<G4double>& GetVecTarget1_TrackStepFirstY(){return fVecTarget1_TrackStepFirstY;}; 
  std::vector<G4double>& GetVecTarget1_TrackStepFirstZ(){return fVecTarget1_TrackStepFirstZ;}; 
  std::vector<G4double>& GetVecTarget1_TrackStepLastX(){return fVecTarget1_TrackStepLastX;}; 
  std::vector<G4double>& GetVecTarget1_TrackStepLastY(){return fVecTarget1_TrackStepLastY;}; 
  std::vector<G4double>& GetVecTarget1_TrackStepLastZ(){return fVecTarget1_TrackStepLastZ;}; 
  std::vector<G4double>& GetVecTarget1_TrackCosTheta(){return fVecTarget1_TrackCosTheta;}; 

  // Target 2 informations
  G4int               GetTarget2_NTracks() { return fTarget2_NTracks;};          
  std::vector<G4int>   & GetVecTarget2_TrackID() {return fVecTarget2_TrackID;}; 
  std::vector<G4int>   & GetVecTarget2_ParentID() {return fVecTarget2_ParentID;}; 
  std::vector<G4double>& GetVecTarget2_TrackMomX(){return fVecTarget2_TrackMomX;}; 
  std::vector<G4double>& GetVecTarget2_TrackMomY(){return fVecTarget2_TrackMomY;}; 
  std::vector<G4double>& GetVecTarget2_TrackMomZ(){return fVecTarget2_TrackMomZ;}; 
  std::vector<G4double>& GetVecTarget2_TrackLength(){return fVecTarget2_TrackLength;}; 
  std::vector<G4double>& GetVecTarget2_TrackDeltaLyz(){return fVecTarget2_TrackDeltaLyz;}; 
  std::vector<G4double>& GetVecTarget2_TrackEdep(){return fVecTarget2_TrackEdep;}; 
  std::vector<G4double>& GetVecTarget2_TrackCharge(){return fVecTarget2_TrackCharge;}; 
  std::vector<G4int>   & GetVecTarget2_TrackPDG(){return fVecTarget2_TrackPDG;};
  std::vector<G4double>& GetVecTarget2_TrackStepFirstX(){return fVecTarget2_TrackStepFirstX;}; 
  std::vector<G4double>& GetVecTarget2_TrackStepFirstY(){return fVecTarget2_TrackStepFirstY;}; 
  std::vector<G4double>& GetVecTarget2_TrackStepFirstZ(){return fVecTarget2_TrackStepFirstZ;}; 
  std::vector<G4double>& GetVecTarget2_TrackStepLastX(){return fVecTarget2_TrackStepLastX;}; 
  std::vector<G4double>& GetVecTarget2_TrackStepLastY(){return fVecTarget2_TrackStepLastY;}; 
  std::vector<G4double>& GetVecTarget2_TrackStepLastZ(){return fVecTarget2_TrackStepLastZ;}; 
  std::vector<G4double>& GetVecTarget2_TrackCosTheta(){return fVecTarget2_TrackCosTheta;}; 

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
  G4double  fEnergyAbsForwTPC1;   // Total energy absorbed by Forward TPC 1
  G4double  fEnergyAbsTPCup1;   // Total energy absorbed by TPC up 1
  G4double  fEnergyAbsTPCdown1; // Total energy absorbed by TPC down 1
  G4double  fEnergyAbsTarget1;  // Total energy absorbed by Target 1
  G4double  fEnergyAbsTPCup2;   // Total energy absorbed by TPC up 2
  G4double  fEnergyAbsTPCdown2; // Total energy absorbed by TPC down 2
  G4double  fEnergyAbsTarget2;  // Total energy absorbed by Target 2
  G4double  fEnergyAbsTot;     // Total energy absorbed in the detector
  G4double  fTrackLAbsForwTPC1;   // Length of track absorbed in Forward TPC 1
  G4double  fTrackLAbsTPCup1;   // Length of track absorbed in TPC up 1
  G4double  fTrackLAbsTPCdown1; // Length of track absorbed in TPC down 1
  G4double  fTrackLAbsTarget1;  // Length of track absorbed in Target 1
  G4double  fTrackLAbsTPCup2;   // Length of track absorbed in TPC up 2
  G4double  fTrackLAbsTPCdown2; // Length of track absorbed in TPC down 2
  G4double  fTrackLAbsTarget2;  // Length of track absorbed in Target 2
  G4double  fTrackLAbsTot;     // Total length of track absorbed in the detector
  G4int     fNTracks;          // # of tracks in the event
  
  std::vector<G4int>    fVecTrackID;   // Vector of ID of each track
  std::vector<G4int>    fVecParentID;   // Vector of ID of each parent
  std::vector<G4int>    fVecTrackPDG;  // Vector of PDG of each track
  std::vector<G4double> fVecTrackE;    // Vector of initial energy of each track
  std::vector<G4double> fVecTrackMomX; // Vector of initial momentum X
  std::vector<G4double> fVecTrackMomY; // Vector of initial momentum Y
  std::vector<G4double> fVecTrackMomZ; // Vector of initial momentum Z
  std::vector<G4double> fVecTrackMomMag; // Vector of initial momentum Z
  //std::vector<G4bool>   fVecTrackIsTPCUp; // Vector of flag "is contained in TPC Up"
  //std::vector<G4bool>   fVecTrackIsTPCDown; // Vector of flag "is contained in TPC Down"
  //std::vector<G4bool>   fVecTrackIsTarget; // Vector of flag "is contained in Target"
  
  // Forward TPC 1 informations
  G4int     fForwTPC1_NTracks;          // # of tracks in Forward TPC 1
  std::vector<G4int>    fVecForwTPC1_TrackID; // Vector of trackID in Forward TPC 1
  std::vector<G4int>    fVecForwTPC1_ParentID; // Vector of parentID in Forward TPC 1
  std::vector<G4double> fVecForwTPC1_TrackMomX; // Vector of initial mom in Forward TPC 1
  std::vector<G4double> fVecForwTPC1_TrackMomY; // Vector of initial mom in Forward TPC 1
  std::vector<G4double> fVecForwTPC1_TrackMomZ; // Vector of initial mom in Forward TPC 1
  std::vector<G4double> fVecForwTPC1_TrackLength; // Vector of corrected (mult. scatt.) track length in Forward TPC 1
  std::vector<G4double> fVecForwTPC1_TrackDeltaLyz; // Vector of geometrical track length in Forward TPC 1
  std::vector<G4double> fVecForwTPC1_TrackEdep; // Vector of track edep in Forward TPC 1
  std::vector<G4double> fVecForwTPC1_TrackCharge; // Vector of track charge in Forward TPC 1
  std::vector<G4int>    fVecForwTPC1_TrackPDG; // Vector of track pdg in Forward TPC 1
  std::vector<G4double> fVecForwTPC1_TrackStepFirstX; // Vector of first step position 
  std::vector<G4double> fVecForwTPC1_TrackStepFirstY; // Vector of first step position
  std::vector<G4double> fVecForwTPC1_TrackStepFirstZ; // Vector of first step position
  std::vector<G4double> fVecForwTPC1_TrackStepLastX; // Vector of last step position
  std::vector<G4double> fVecForwTPC1_TrackStepLastY; // Vector of last step position
  std::vector<G4double> fVecForwTPC1_TrackStepLastZ; // Vector of last step position
  std::vector<G4double> fVecForwTPC1_TrackCosTheta; // Vector of cos(theta) angle wrt nu direction (z)

  // TPC up 1 informations
  G4int     fTPCUp1_NTracks;          // # of tracks in TPC Up 1
  std::vector<G4int>    fVecTPCUp1_TrackID; // Vector of trackID in TPC up 1
  std::vector<G4int>    fVecTPCUp1_ParentID; // Vector of parentID in TPC up 1
  std::vector<G4double> fVecTPCUp1_TrackMomX; // Vector of initial mom in TPC Up 1
  std::vector<G4double> fVecTPCUp1_TrackMomY; // Vector of initial mom in TPC Up 1
  std::vector<G4double> fVecTPCUp1_TrackMomZ; // Vector of initial mom in TPC Up 1
  std::vector<G4double> fVecTPCUp1_TrackLength; // Vector of corrected (mult. scatt.) track length in TPC Up 1
  std::vector<G4double> fVecTPCUp1_TrackDeltaLyz; // Vector of geometrical track length in TPC Up 1
  std::vector<G4double> fVecTPCUp1_TrackEdep; // Vector of track edep in TPC Up 1
  std::vector<G4double> fVecTPCUp1_TrackCharge; // Vector of track charge in TPC Up 1
  std::vector<G4int>    fVecTPCUp1_TrackPDG; // Vector of track pdg in TPC Up 1
  std::vector<G4double> fVecTPCUp1_TrackStepFirstX; // Vector of first step position 
  std::vector<G4double> fVecTPCUp1_TrackStepFirstY; // Vector of first step position
  std::vector<G4double> fVecTPCUp1_TrackStepFirstZ; // Vector of first step position
  std::vector<G4double> fVecTPCUp1_TrackStepLastX; // Vector of last step position
  std::vector<G4double> fVecTPCUp1_TrackStepLastY; // Vector of last step position
  std::vector<G4double> fVecTPCUp1_TrackStepLastZ; // Vector of last step position
  std::vector<G4double> fVecTPCUp1_TrackCosTheta; // Vector of cos(theta) angle wrt nu direction (z)

  // TPC down 1 informations
  G4int     fTPCDown1_NTracks;          // # of tracks in TPC Down 1
  std::vector<G4int>    fVecTPCDown1_TrackID; // Vector of trackID in TPCDown 1
  std::vector<G4int>    fVecTPCDown1_ParentID; // Vector of parentID in TPCDown 1
  std::vector<G4double> fVecTPCDown1_TrackMomX; // Vector of initial mom in TPC Down 1
  std::vector<G4double> fVecTPCDown1_TrackMomY; // Vector of initial mom in TPC Down 1
  std::vector<G4double> fVecTPCDown1_TrackMomZ; // Vector of initial mom in TPC Down 1
  std::vector<G4double> fVecTPCDown1_TrackLength; // Vector of corrected (mult. scatt.) track length in TPC Down 1
  std::vector<G4double> fVecTPCDown1_TrackDeltaLyz; // Vector of geometrical track length in TPC Down 1
  std::vector<G4double> fVecTPCDown1_TrackEdep; // Vector of track edep in TPC Down 1
  std::vector<G4double> fVecTPCDown1_TrackCharge; // Vector of track charge in TPC Down 1
  std::vector<G4int>    fVecTPCDown1_TrackPDG; // Vector of track pdg in TPC Down 1
  std::vector<G4double> fVecTPCDown1_TrackStepFirstX; // Vector of first step position
  std::vector<G4double> fVecTPCDown1_TrackStepFirstY; // Vector of first step position
  std::vector<G4double> fVecTPCDown1_TrackStepFirstZ; // Vector of first step position
  std::vector<G4double> fVecTPCDown1_TrackStepLastX; // Vector of last step position
  std::vector<G4double> fVecTPCDown1_TrackStepLastY; // Vector of last step position
  std::vector<G4double> fVecTPCDown1_TrackStepLastZ; // Vector of last step position
  std::vector<G4double> fVecTPCDown1_TrackCosTheta; // Vector of cos(theta) angle wrt nu direction (z)

  // TPC up 2 informations
  G4int     fTPCUp2_NTracks;          // # of tracks in TPC Up 2
  std::vector<G4int>    fVecTPCUp2_TrackID; // Vector of trackID in TPC up 2
  std::vector<G4int>    fVecTPCUp2_ParentID; // Vector of parentID in TPC up 2
  std::vector<G4double> fVecTPCUp2_TrackMomX; // Vector of initial mom in TPC Up 2
  std::vector<G4double> fVecTPCUp2_TrackMomY; // Vector of initial mom in TPC Up 2
  std::vector<G4double> fVecTPCUp2_TrackMomZ; // Vector of initial mom in TPC Up 2
  std::vector<G4double> fVecTPCUp2_TrackLength; // Vector of corrected (mult. scatt.) track length in TPC Up 2
  std::vector<G4double> fVecTPCUp2_TrackDeltaLyz; // Vector of geometrical track length in TPC Up 2
  std::vector<G4double> fVecTPCUp2_TrackEdep; // Vector of track edep in TPC Up 2
  std::vector<G4double> fVecTPCUp2_TrackCharge; // Vector of track charge in TPC Up 2
  std::vector<G4int>    fVecTPCUp2_TrackPDG; // Vector of track pdg in TPC Up 2
  std::vector<G4double> fVecTPCUp2_TrackStepFirstX; // Vector of first step position 
  std::vector<G4double> fVecTPCUp2_TrackStepFirstY; // Vector of first step position
  std::vector<G4double> fVecTPCUp2_TrackStepFirstZ; // Vector of first step position
  std::vector<G4double> fVecTPCUp2_TrackStepLastX; // Vector of last step position
  std::vector<G4double> fVecTPCUp2_TrackStepLastY; // Vector of last step position
  std::vector<G4double> fVecTPCUp2_TrackStepLastZ; // Vector of last step position
  std::vector<G4double> fVecTPCUp2_TrackCosTheta; // Vector of cos(theta) angle wrt nu direction (z)

  // TPC down 2 informations
  G4int     fTPCDown2_NTracks;          // # of tracks in TPC Down 2
  std::vector<G4int>    fVecTPCDown2_TrackID; // Vector of trackID in TPCDown 2
  std::vector<G4int>    fVecTPCDown2_ParentID; // Vector of parentID in TPCDown 2
  std::vector<G4double> fVecTPCDown2_TrackMomX; // Vector of initial mom in TPC Down 2
  std::vector<G4double> fVecTPCDown2_TrackMomY; // Vector of initial mom in TPC Down 2
  std::vector<G4double> fVecTPCDown2_TrackMomZ; // Vector of initial mom in TPC Down 2
  std::vector<G4double> fVecTPCDown2_TrackLength; // Vector of corrected (mult. scatt.) track length in TPC Down 2
  std::vector<G4double> fVecTPCDown2_TrackDeltaLyz; // Vector of geometrical track length in TPC Down 2
  std::vector<G4double> fVecTPCDown2_TrackEdep; // Vector of track edep in TPC Down 2
  std::vector<G4double> fVecTPCDown2_TrackCharge; // Vector of track charge in TPC Down 2
  std::vector<G4int>    fVecTPCDown2_TrackPDG; // Vector of track pdg in TPC Down 2
  std::vector<G4double> fVecTPCDown2_TrackStepFirstX; // Vector of first step position
  std::vector<G4double> fVecTPCDown2_TrackStepFirstY; // Vector of first step position
  std::vector<G4double> fVecTPCDown2_TrackStepFirstZ; // Vector of first step position
  std::vector<G4double> fVecTPCDown2_TrackStepLastX; // Vector of last step position
  std::vector<G4double> fVecTPCDown2_TrackStepLastY; // Vector of last step position
  std::vector<G4double> fVecTPCDown2_TrackStepLastZ; // Vector of last step position
  std::vector<G4double> fVecTPCDown2_TrackCosTheta; // Vector of cos(theta) angle wrt nu direction (z)

  // Target 1 informations
  G4int     fTarget1_NTracks;          // # of tracks in Target
  std::vector<G4int>    fVecTarget1_TrackID; // Vector of trackID in Target
  std::vector<G4int>    fVecTarget1_ParentID; // Vector of parentID in Target
  std::vector<G4double> fVecTarget1_TrackMomX; // Vector of initial mom in Target
  std::vector<G4double> fVecTarget1_TrackMomY; // Vector of initial mom in Target 
  std::vector<G4double> fVecTarget1_TrackMomZ; // Vector of initial mom in Target
  std::vector<G4double> fVecTarget1_TrackLength; // Vector of corrected (mult. scatt.) track length in Target
  std::vector<G4double> fVecTarget1_TrackDeltaLyz; // Vector of geometrical track length in Target
  std::vector<G4double> fVecTarget1_TrackEdep; // Vector of track edep in Target
  std::vector<G4double> fVecTarget1_TrackCharge; // Vector of track charge in Target
  std::vector<G4int>    fVecTarget1_TrackPDG; // Vector of track pdg in Target 
  std::vector<G4double> fVecTarget1_TrackStepFirstX; // Vector of first step position
  std::vector<G4double> fVecTarget1_TrackStepFirstY; // Vector of first step position
  std::vector<G4double> fVecTarget1_TrackStepFirstZ; // Vector of first step position
  std::vector<G4double> fVecTarget1_TrackStepLastX; // Vector of last step position
  std::vector<G4double> fVecTarget1_TrackStepLastY; // Vector of last step position
  std::vector<G4double> fVecTarget1_TrackStepLastZ; // Vector of last step position
  std::vector<G4double> fVecTarget1_TrackCosTheta; // Vector of cos(theta) angle wrt nu direction (z)

  // Target 2 informations
  G4int     fTarget2_NTracks;          // # of tracks in Target
  std::vector<G4int>    fVecTarget2_TrackID; // Vector of trackID in Target
  std::vector<G4int>    fVecTarget2_ParentID; // Vector of parentID in Target
  std::vector<G4double> fVecTarget2_TrackMomX; // Vector of initial mom in Target
  std::vector<G4double> fVecTarget2_TrackMomY; // Vector of initial mom in Target 
  std::vector<G4double> fVecTarget2_TrackMomZ; // Vector of initial mom in Target
  std::vector<G4double> fVecTarget2_TrackLength; // Vector of corrected (mult. scatt.) track length in Target
  std::vector<G4double> fVecTarget2_TrackDeltaLyz; // Vector of geometrical track length in Target
  std::vector<G4double> fVecTarget2_TrackEdep; // Vector of track edep in Target
  std::vector<G4double> fVecTarget2_TrackCharge; // Vector of track charge in Target
  std::vector<G4int>    fVecTarget2_TrackPDG; // Vector of track pdg in Target 
  std::vector<G4double> fVecTarget2_TrackStepFirstX; // Vector of first step position
  std::vector<G4double> fVecTarget2_TrackStepFirstY; // Vector of first step position
  std::vector<G4double> fVecTarget2_TrackStepFirstZ; // Vector of first step position
  std::vector<G4double> fVecTarget2_TrackStepLastX; // Vector of last step position
  std::vector<G4double> fVecTarget2_TrackStepLastY; // Vector of last step position
  std::vector<G4double> fVecTarget2_TrackStepLastZ; // Vector of last step position
  std::vector<G4double> fVecTarget2_TrackCosTheta; // Vector of cos(theta) angle wrt nu direction (z)

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
inline void ExN02EventAction::AddAbsForwTPC1(G4double de, G4double dl) {
  fEnergyAbsForwTPC1 += de;
  fTrackLAbsForwTPC1 += dl;
}
inline void ExN02EventAction::AddAbsTPCup1(G4double de, G4double dl) {
  fEnergyAbsTPCup1 += de;
  fTrackLAbsTPCup1 += dl;
}
inline void ExN02EventAction::AddAbsTPCdown1(G4double de, G4double dl) {
  fEnergyAbsTPCdown1 += de;
  fTrackLAbsTPCdown1 += dl;
}
inline void ExN02EventAction::AddAbsTarget1(G4double de, G4double dl) {
  fEnergyAbsTarget1 += de;
  fTrackLAbsTarget1 += dl;
}

inline void ExN02EventAction::AddAbsTPCup2(G4double de, G4double dl) {
  fEnergyAbsTPCup2 += de;
  fTrackLAbsTPCup2 += dl;
}
inline void ExN02EventAction::AddAbsTPCdown2(G4double de, G4double dl) {
  fEnergyAbsTPCdown2 += de;
  fTrackLAbsTPCdown2 += dl;
}
inline void ExN02EventAction::AddAbsTarget2(G4double de, G4double dl) {
  fEnergyAbsTarget2 += de;
  fTrackLAbsTarget2 += dl;
}

inline void ExN02EventAction::AddAbsTot(G4double de, G4double dl) {
  fEnergyAbsTot += de;
  fTrackLAbsTot += dl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
