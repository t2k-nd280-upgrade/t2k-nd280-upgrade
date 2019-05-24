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

#include "ND280VolumeID.hh"

#include "TND280UpHit.hh" 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Step;
class G4Track;

//class ExN02TrackerHit : public G4VHit
class ExN02TrackerHit : public TND280UpHit, public G4VHit
{
public:

  ExN02TrackerHit(double, double, G4int, G4int, G4int, G4int, G4double, G4double, G4ThreeVector, G4double); // NEW ND280 HitSegment
  ExN02TrackerHit(double, double); // NEW ND280 HitSegment     

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
  //void Draw(const bool empty = false);
  //void Print(const bool empty = false);


  
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

  G4double fEdep_min;
  G4double fEdep_max;

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

  inline void AddEdep(G4double e, G4double eq) { (void) eq; fEdep += e;}
  inline void SetEdep(G4double e) { fEdep = e; }
  inline G4double GetEdep() { return fEdep; }

  inline void SetCharge(G4double ch) { fCharge = ch; }
  inline G4double GetCharge() { return fCharge; }

  inline void SetPosition     (G4ThreeVector pos)      {fPosition = pos; };
  inline G4ThreeVector GetPosition() { return fPosition; }

  inline void SetTime(G4double t) { fTime = t; }
  inline G4double GetTime() { return fTime; }

  //
  // NEW ND280 HitSegment
  //
  
  // The following code is added to use the ND280HitSegment class
  // in this framework
  
public:

  /*
    
  // Now this is declared in TND280UpEvent, but accessible
  // in the same way (TND280UpEvent is inherited by TrackerHit
  
  inline void SetMaxSagitta(double sag){fMaxSagitta = sag;} 
  inline double GetMaxSagitta(){return fMaxSagitta;}

  inline void SetMaxLength(double length){fMaxLength = length;} 
  inline double GetMaxLength(){return fMaxLength;}

  std::vector<int> fContributors;

  int fPrimaryId;
  double fStartX;
  double fStartY;
  double fStartZ;
  double fStartT;
  double fStopX;
  double fStopY;
  double fStopZ;
  double fStopT;
  double fEnergyDeposit;
  double fTrackLength;

  inline int GetPrimaryId(){return fPrimaryId;}
  inline double GetStartX(){return fStartX;}
  inline double GetStartY(){return fStartY;}
  inline double GetStartZ(){return fStartZ;}
  inline double GetStopX(){return fStopX;}
  inline double GetStopY(){return fStopY;}
  inline double GetStopZ(){return fStopZ;}
  inline double GetEnergyDeposit(){return fEnergyDeposit;}
  inline double GetTrackLength(){return fTrackLength;}

  */


  // All the following code is either taken from nd280mc ND280HitSegment class
  // or adapted in order to use the ND280HitSegment class
  
public:
   
  /// Add the effects of a part of a step to this hit.
  // (same as ND280HitSegment)
  virtual void AddStep(G4Step* theStep, double start=0.0, double end=1.0);
  
  /// Hits for the same primary particle, in the same physical volume belong
  /// in the same hit.
  // (same as ND280HitSegment)
  virtual bool SameHit(G4Step* theStep); 
  
  /// Provide public access to the contributors for internal G4 classes.
  std::vector<int>& GetContributors() {return fContributors;}
    
  /// Find the distance from the starting point to stoping point of the
  /// track.
  virtual double GetLength() const;

protected:
  /// Find the primary track ID for the current track.  This is the primary
  /// that is the ultimate parent of the current track.
  int FindPrimaryId(G4Track* theTrack);
  
  /// Find the maximum separation (the sagitta) between the current hit
  /// segment path points, and the straight line connecting the start and
  /// proposed new stop point.
  double FindSagitta(G4Step* theStep);
  
  /// Find the maximum distance from the hit segment to the new step that is
  /// proposed to be added to the hit segment. This is used to
  /// combine secondaries with a parent track.
  double FindSeparation(G4Step* theStep);
  
  /// The sagitta tolerance for the segment.
  double fMaxSagitta;
  
  /// The maximum length between the start and stop points of the segment.
  double fMaxLength;

private:
  /// The G4 physical volume that contains the hit.
  ND280VolumeID fHitVolume;
  
  /// The end points of the steps that make up this hit.  This is used to
  /// make sure that the current hit stays inside of it's allowed
  /// tolerances.
  std::vector<G4ThreeVector> fPath;

  /////
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
