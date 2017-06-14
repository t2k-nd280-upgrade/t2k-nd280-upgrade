#ifndef INGRID_RESPONSE
#define INGRID_RESPONSE

#include "G4ThreeVector.hh"
#include "G4EmCalculator.hh"
#include "G4Track.hh"

class IngridResponse {
public:
  IngridResponse();
  ~IngridResponse();

  void ApplyScintiResponse(G4double* edep, G4Track* aTrack);
  void ApplyLightCollection(G4double* edep, G4int mod, G4int view, G4ThreeVector pos);//added
  void ApplyLightCollection(G4double* edep, G4int mod, G4int view, G4ThreeVector pos, G4int pln);//added B2 2014/1/27
  void ApplyFiberResponse(G4double* edep, G4double* time, G4int view, G4ThreeVector pos);
  void ApplyFiberResponse(G4double* edep,G4int mod, G4double* time, G4int view, G4ThreeVector pos);//added B2
  void ApplyFiberResponse(G4double* edep,G4int mod, G4double* time, G4int view, G4ThreeVector pos, G4int pln);//added B2 2014/1/27
  void ApplyFiberResponseV(G4double* edep, G4double* time, G4int pln, G4ThreeVector pos);
  void ApplyMPPCResponse(G4double edep, G4double* pe);
  void ApplyADCResponse(G4double* pe, G4double* lope, G4int* adc, G4int* loadc);
  void ApplyTDCResponse(G4double time, G4int* tdc);
  //void teeest(){return;};
private:
  G4EmCalculator emcal;

  void BirksSaturation(G4double* edeposit, G4Track* aTrack);

};

#endif
