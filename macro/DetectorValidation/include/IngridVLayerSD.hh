#ifndef IngridVLayerSD_H
#define IngridVLayerSD_H
 
#include "G4VSensitiveDetector.hh"

#include "IngridVLayerHit.hh"
#include "IngridResponse.hh"

class G4HCofThisEvent;
class G4Step;
//class G4TouchableHistory;

class IngridVLayerSD : public G4VSensitiveDetector {
  G4THitsCollection<IngridVLayerHit>* vlayerHitCollection;
  G4double TotalvlayerDep;
  IngridResponse *ingresp;
 
public:
  IngridVLayerSD(const G4String& name);
  virtual ~IngridVLayerSD();

  // virtual methods
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  virtual void Initialize(G4HCofThisEvent* HCTE);
  virtual void EndOfEvent(G4HCofThisEvent* HCTE);

  virtual void DrawAll();
  virtual void PrintAll(); 

   
  inline G4int GetNhit()
  { return vlayerHitCollection->entries(); }
  inline G4double GetTotalDep()
  { return TotalvlayerDep; }

};

#endif
