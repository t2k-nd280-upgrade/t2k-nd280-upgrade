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
// $Id: ExN02TrackerHit.hh,v 1.8 2006-06-29 17:47:53 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN02TrackerHit_h
#define ExN02TrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02TrackerHit : public G4VHit
{
public:

  ExN02TrackerHit(G4int, G4int, G4int, G4int, G4double, G4double, G4ThreeVector, G4double);
  ExN02TrackerHit(G4int, G4double, G4int, const G4ThreeVector&);
  ExN02TrackerHit(G4int, G4double, G4int);
  ExN02TrackerHit(G4int, G4double);

  ExN02TrackerHit();
  ~ExN02TrackerHit();
  ExN02TrackerHit(const ExN02TrackerHit&);
  const ExN02TrackerHit& operator=(const ExN02TrackerHit&);
  G4int operator==(const ExN02TrackerHit&) const;

  G4int CompareID(const ExN02TrackerHit right) ;
  G4int CompareP(const ExN02TrackerHit right);
  G4int isFaster(const ExN02TrackerHit right);
  G4int LargerEdep(const ExN02TrackerHit right);

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  void Draw();
  void Print();
  void Print_Hit();

private:
  G4String fNameDet;
  G4double fStepLength;
  G4int    fParentID;
  G4int fDetID;
  G4int fTrackID;
  G4double fEdep;
  G4double fCharge;
  G4int fParticle;
  G4ThreeVector fPosition;
  G4double fTime;

public:

  void SetNameDet     (G4String name)    { fNameDet = name; };
  G4String GetNameDet()    { return fNameDet; }; 

  G4double GetStepLength() { return fStepLength; }; 
  void SetStepLength  (G4double length ) { fStepLength = length; };

  G4int GetParentID()      { return fParentID; };
  void SetParentID    (G4int parent)     { fParentID = parent; };
  
  inline void SetDetID     (G4int detid)      {fDetID = detid; };
  inline G4int GetDetID() { return fDetID; }// ID of scintillator

  inline void SetTrackID     (G4int track)      { fTrackID = track; };
  inline G4int GetTrackID() { return fTrackID; }

  inline void SetParticle(G4int p) { fParticle = p; }
  inline G4int GetParticle() { return fParticle; }

  inline void AddEdep(G4double e, G4double eq) { fEdep += e;}
  inline void SetEdep(G4double e) { fEdep = e; }
  inline G4double GetEdep() { return fEdep; }

  inline void SetCharge(G4double ch) { fCharge = ch; }
  inline G4double GetCharge() { return fCharge; }

  inline void SetPosition     (G4ThreeVector pos)      {fPosition = pos; };
  inline G4ThreeVector GetPosition() { return fPosition; }

  inline void SetTime(G4double t) { fTime = t; }
  inline G4double GetTime() { return fTime; }
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<ExN02TrackerHit> ExN02TrackerHitsCollection;

extern G4Allocator<ExN02TrackerHit> ExN02TrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* ExN02TrackerHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) ExN02TrackerHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void ExN02TrackerHit::operator delete(void *aHit)
{
  ExN02TrackerHitAllocator.FreeSingle((ExN02TrackerHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
