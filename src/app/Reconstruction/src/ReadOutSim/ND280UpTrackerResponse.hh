#ifndef ND280UP_TRACKER_RESPONSE
#define ND280UP_TRACKER_RESPONSE

#include <TVector3.h>

#include "G4Track.hh"
#include "G4EmCalculator.hh"

class ND280UpTrackerResponse {
public:
  ND280UpTrackerResponse();
  ~ND280UpTrackerResponse();

  void ApplyScintiResponse(G4double* edep, G4Track* aTrack);
  void ApplyLightCollection(G4double* edep, G4int mod, G4int view, G4int pln, G4int ch, TVector3 pos);//added 2014/12/16
  void ApplyFiberResponse(G4double* edep, G4double* time, G4int view, TVector3 pos);
  void ApplyFiberResponse(G4double* edep, G4double* time, G4int view, TVector3 pos, G4int mod);
  void ApplyFiberResponseV(G4double* edep, G4double* time, G4int pln, TVector3 pos);
  //void ApplyMPPCResponse(G4double edep, G4double* pe);
  void ApplyMPPCResponse(G4double edep, G4double* pe, G4int mod);
  void ApplyADCResponse(G4double* pe, G4double* lope, G4int* adc, G4int* loadc);
  void ApplyTDCResponse(G4double time, G4int* tdc);
  float peratio[17][2][18][80];//2014/12/16

  void ApplyScintiResponse2(G4double* edep, G4double* steplength, G4Track* aTrack);
private:
  G4EmCalculator emcal;
  void BirksSaturation(G4double* edeposit, G4Track* aTrack);
  void BirksSaturation2(G4double* edeposit, G4double * steplength, G4Track* aTrack);

};

#endif
