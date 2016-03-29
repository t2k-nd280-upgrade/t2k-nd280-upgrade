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
  void ReachDetectPrim(G4String detname);
  void EvtxPrim(G4double Etrue);
  
  void PDGPrimTPCup(G4int pdg);
  void PDGPrimTPCdown(G4int pdg);
  void PDGPrimTarget(G4int pdg);
  void PDGPrim(G4int pdg);

private:
  G4double  fEnergyAbsTPCup;
  G4double  fEnergyAbsTPCdown;
  G4double  fEnergyAbsTarget;
  G4double  fEnergyAbsTot;
  G4double  fTrackLAbsTPCup;
  G4double  fTrackLAbsTPCdown;
  G4double  fTrackLAbsTarget;
  G4double  fTrackLAbsTot;

  G4double  fTruePrimMom;   // true momentum at vertex of the primary track
  G4double  fTruePrimE;     // true energy at vertex of the primary track
  G4double  fPDGPrim;     // PDG of primary particle
  G4int     fPrimInTPCup;   // check if the primary track reached the TPCup
  G4int     fPrimInTPCdown; // check if the primary track reached the TPCdown
  G4int     fPrimInTarget;  // check if the primary track reached the target

  G4int  fPDGPrimTPCup;    // PDG of primary particle in TPC up  
  G4int  fPDGPrimTPCdown;  // PDG of primary particle in TPC down
  G4int  fPDGPrimTarget;   // PDG of primary particle in Target

};

// inline functions 


inline void ExN02EventAction::PDGPrim(G4int pdg) {
  fPDGPrim = pdg; // fill it only the first time
}

inline void ExN02EventAction::PDGPrimTPCup(G4int pdg) {
  if(fPDGPrim==pdg) fPDGPrimTPCup = pdg; 
  //G4cout << "pdg = " << pdg << " - pdg_prim = " << fPDGPrim << G4endl;
  //exit(1);
}

inline void ExN02EventAction::PDGPrimTPCdown(G4int pdg) {
  if(fPDGPrim==pdg) fPDGPrimTPCdown = pdg;
}

inline void ExN02EventAction::PDGPrimTarget(G4int pdg) {
  if(fPDGPrimTarget<0. && fPDGPrim==pdg)
    fPDGPrimTarget = pdg;
}

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

inline void ExN02EventAction::ReachDetectPrim(G4String detname) {
  if(detname=="TPC Up")   fPrimInTPCup   = 1.;
  if(detname=="TPC Down") fPrimInTPCdown = 1.;
  if(detname=="Target")  fPrimInTarget  = 1.; 
}

inline void ExN02EventAction::EvtxPrim(G4double Etrue) {
  if(fTruePrimE<=0.) fTruePrimE = Etrue; // fill it only the first time
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
