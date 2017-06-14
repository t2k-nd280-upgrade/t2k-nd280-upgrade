#ifndef IngridVetoSD_H
#define IngridVetoSD_H
 
#include "G4VSensitiveDetector.hh"

#include "IngridVetoHit.hh"
#include "IngridResponse.hh"

class G4HCofThisEvent;
class G4Step;
//class G4TouchableHistory;

class IngridVetoSD : public G4VSensitiveDetector {
  G4THitsCollection<IngridVetoHit>* vetoHitCollection;
  IngridResponse *ingresp;
  G4double TotalvetoDep;
  // veto bar of 12.5cm
  //G4int vetoID[100]; //store only the TrackerID where a Hit is created (<280)
  //G4double DEP[100];
  // veto bar of 5cm
  G4int vetoID[300]; // store only the TrackerID where a Hit is created (<700)
  G4double DEP[300];
  G4int fHIT;
 
public:
  IngridVetoSD(const G4String& name);
  virtual ~IngridVetoSD();

  // virtual methods
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  virtual void Initialize(G4HCofThisEvent* HCTE);
  virtual void EndOfEvent(G4HCofThisEvent* HCTE);

  virtual void DrawAll();
  virtual void PrintAll(); 

  
  inline G4int GetNhit()
  { return vetoHitCollection->entries(); }
  inline G4double GetTotalDep()
  { return TotalvetoDep; }
  


};

#endif
