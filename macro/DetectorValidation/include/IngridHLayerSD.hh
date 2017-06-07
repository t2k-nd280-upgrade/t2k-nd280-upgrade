#ifndef IngridHLayerSD_H
#define IngridHLayerSD_H
 
#include "G4VSensitiveDetector.hh"

#include "IngridHLayerHit.hh"
#include "IngridResponse.hh"

class G4HCofThisEvent;
class G4Step;
//class G4TouchableHistory;

class IngridHLayerSD : public G4VSensitiveDetector {
  G4THitsCollection<IngridHLayerHit>* hlayerHitCollection;
  IngridResponse *ingresp;
  G4double TotalhlayerDep;
 
public:
  IngridHLayerSD(const G4String& name);
  virtual ~IngridHLayerSD();

  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  virtual void Initialize(G4HCofThisEvent* HCTE);
  virtual void EndOfEvent(G4HCofThisEvent* HCTE);

  virtual void DrawAll();
  virtual void PrintAll(); 

  
  inline G4int GetNhit()
  { return hlayerHitCollection->entries(); }
  inline G4double GetTotalDep()
  { return TotalhlayerDep; }
  

};

#endif
