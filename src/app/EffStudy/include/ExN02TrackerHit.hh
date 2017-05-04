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

  /*
public:
  
  // ND280 UPGRADE
  void SetTrackID     (G4int track)      { trackID = track; };
  void SetParentID    (G4int parent)     { parentID = parent; };
  void SetChamberNb   (G4int chamb)      { chamberNb = chamb; };  
  void SetEdep        (G4double de)      { edep = de; };
  void SetPreStepPos  (G4ThreeVector xyz){ prestepPos = xyz; };  
  void SetPostStepPos (G4ThreeVector xyz){ poststepPos = xyz; };  
  void SetMom         (G4ThreeVector xyz){ mom = xyz; }; 
  void SetNameDet     (G4String name)    { namedet = name; };
  void SetCharge      (G4int mycharge)   { charge = mycharge; };
  void SetTrackPDG    (G4int pdg)        { trackpdg = pdg; };
  void SetTrackName   (G4String name)    { trackname = name; };
  void SetTrackLength (G4double length ) { tracklength = length; };
  void SetStepLength  (G4double length ) { steplength = length; };
  void SetStepDeltaLyz(G4double deltalyz ) { stepdeltalyz = deltalyz; };
  void SetNSteps      (G4int num)        { nsteps = num; };
  void SetTrackCosTheta(G4double costh)  { trackcostheta = costh; };
 
  //G4int GetTrackID()       { return trackID; };
  G4int GetParentID()      { return parentID; };
  G4int GetChamberNb()     { return chamberNb; };
  //G4double GetEdep()       { return edep; };      
  G4ThreeVector GetPreStepPos()   { return prestepPos; };
  G4ThreeVector GetPostStepPos() { return poststepPos; };
  G4ThreeVector GetMom()   { return mom; }; 
  G4String GetNameDet()    { return namedet; }; 
  G4int GetCharge()        { return charge; }; 
  G4int GetTrackPDG()      { return trackpdg; }; 
  G4String GetTrackName()  { return trackname; }; 
  G4double GetTrackLength(){ return tracklength; }; 
  G4double GetStepLength() { return steplength; }; 
  G4double GetStepDeltaLyz() { return stepdeltalyz; }; 
  G4int GetNSteps()        { return nsteps; }; 
  G4double GetTrackCosTheta(){ return trackcostheta; };
  ///// 
  */

private:
  // My new
  G4String fNameDet;
  G4double fStepLength;
  G4int    fParentID;
  //
  G4int fDetID;
  G4int fTrackID;
  G4int fMod;
  G4int fView;
  G4int fPln;
  G4int fCh;
  G4double fEdep;
  G4double fEdep_q;
  G4double fPE;
  G4double fPEX;
  G4double fPEY;
  G4double fPEZ;
  G4double fLope;
  G4int fParticle;
  G4ThreeVector fPosition;
  G4ThreeVector fPosInMod;
  G4int fEventID;
  G4double fTime;
  G4double fDelayTime;
  G4double fDelayTimeX;
  G4double fDelayTimeY;
  G4double fDelayTimeZ;
  G4int fGridCell_id_x1;//added by koga 2016/1/7
  G4int fGridCell_id_x2;//added by koga 2016/1/7
  G4int fGridCell_id_y1;//added by koga 2016/1/7
  G4int fGridCell_id_y2;//added by koga 2016/1/7

  /*
  // ND280 UPGRADE
  //G4int         trackID;
  G4int         parentID;
  G4int         chamberNb;
  //G4double      edep;
  G4ThreeVector prestepPos;
  G4ThreeVector poststepPos;
  G4String      namedet;
  G4int         charge;
  G4ThreeVector mom; 
  G4int         trackpdg;
  G4String      trackname;  
  G4double      tracklength;
  G4double      steplength;
  G4double      stepdeltalyz;
  G4int         nsteps;
  G4double      trackcostheta;
  /////
  */


public:

  // My new
  void SetNameDet     (G4String name)    { fNameDet = name; };
  G4String GetNameDet()    { return fNameDet; }; 

  G4double GetStepLength() { return fStepLength; }; 
  void SetStepLength  (G4double length ) { fStepLength = length; };

  G4int GetParentID()      { return fParentID; };
  void SetParentID    (G4int parent)     { fParentID = parent; };
  //
  
  inline void SetDetID     (G4int detid)      {fDetID = detid; };
  inline G4int GetDetID() { return fDetID; }// ID of scintillator

  inline void SetTrackID     (G4int track)      { fTrackID = track; };
  inline G4int GetTrackID() { return fTrackID; }

  inline void SetParticle(G4int p) { fParticle = p; }
  inline G4int GetParticle() { return fParticle; }

  inline void AddEdep(G4double e, G4double eq) { fEdep += e; fEdep_q += eq; }
  inline void SetEdep(G4double e) { fEdep = e; }
  inline void SetEdepQ(G4double eq) { fEdep_q = eq; } 
  inline G4double GetEdep() { return fEdep; }
  inline G4double GetEdepQ() { return fEdep_q; }

  inline void SetPosition     (G4ThreeVector pos)      {fPosition = pos; };
  inline G4ThreeVector GetPosition() { return fPosition; }

  inline void SetPosInMod     (G4ThreeVector pos)      {fPosInMod = pos; };
  inline G4ThreeVector GetPosInMod() { return fPosInMod; }

  inline void SetTime(G4double t) { fTime = t; }
  inline G4double GetTime() { return fTime; }

  inline void SetDelayTime(G4double dt) { fDelayTime = dt; }
  inline void SetDelayTimeX(G4double dt) { fDelayTimeX = dt; }
  inline void SetDelayTimeY(G4double dt) { fDelayTimeY = dt; }
  inline void SetDelayTimeZ(G4double dt) { fDelayTimeZ = dt; }
  inline G4double GetDelayTime() { return fDelayTime; }
  inline G4double GetDelayTimeX() { return fDelayTimeX; }
  inline G4double GetDelayTimeY() { return fDelayTimeY; }
  inline G4double GetDelayTimeZ() { return fDelayTimeZ; }

  inline void SetEventID(G4int evtid) { fEventID = evtid;}
  inline G4int GetEventID() { return fEventID; }

  inline void SetMod     (G4int mod)      {fMod = mod; };
  inline G4int GetMod() { return fMod; }

  inline void SetView     (G4int view)      {fView = view; };
  inline G4int GetView() { return fView; }

  inline void SetPln     (G4int pln)      {fPln = pln; };
  inline G4int GetPln() { return fPln; }

  inline void SetCh     (G4int ch)      {fCh = ch; };
  inline G4int GetCh() { return fCh; }

  inline void SetPE(G4double p) { fPE = p; }
  inline void SetPEX(G4double p) { fPEX = p; }
  inline void SetPEY(G4double p) { fPEY = p; }
  inline void SetPEZ(G4double p) { fPEZ = p; }
  inline G4double GetPE() { return fPE; }
  inline G4double GetPEX() { return fPEX; }
  inline G4double GetPEY() { return fPEY; }
  inline G4double GetPEZ() { return fPEZ; }

  inline void SetLOPE(G4double p) { fLope = p; }
  inline G4double GetLOPE() { return fLope; }

  inline void SetCellidX1(G4int id) { fGridCell_id_x1 = id; }//added by koga 2016/1/7
  inline G4int GetCellidX1() { return fGridCell_id_x1; }//added by koga 2016/1/7

  inline void SetCellidX2(G4int id) { fGridCell_id_x2 = id; }//added by koga 2016/1/7
  inline G4int GetCellidX2() { return fGridCell_id_x2; }//added by koga 2016/1/7

  inline void SetCellidY1(G4int id) { fGridCell_id_y1 = id; }//added by koga 2016/1/7
  inline G4int GetCellidY1() { return fGridCell_id_y1; }//added by koga 2016/1/7

  inline void SetCellidY2(G4int id) { fGridCell_id_y2 = id; }//added by koga 2016/1/7
  inline G4int GetCellidY2() { return fGridCell_id_y2; }//added by koga 2016/1/7
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
