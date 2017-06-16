#ifndef APPLY_RESPONSE
#define APPLY_RESPONSE

#include "G4ThreeVector.hh"
//#include "G4EmCalculator.hh"
#include "G4Track.hh"

#include "ExN02TrackerResponse.hh"
#include "ExN02ND280XML.hh"
#include "ND280RootPersistencyManager.hh"

class ExN02ApplyResponse {
public:
  ExN02ApplyResponse();
  ~ExN02ApplyResponse();

  void CalcResponse(G4ThreeVector lightPos,int trkid,int parid,double charge,double time,double steplength, double edep, G4String touch_namedet);

  G4ThreeVector GetHitPos(){return fHitPos;}
  G4ThreeVector GetHitPE(){return fHitPE;}
  G4ThreeVector GetHitTime(){return fHitTime;}
  G4double GetHitTrkID(){return fHitTrkID;}

private:

  void Init();
  void Delete();
  
  // Define the XML input parser
  ExN02ND280XML *ND280XMLInput;
  ND280RootPersistencyManager* persistencyManager;

  G4ThreeVector fHitPos;
  G4ThreeVector fHitPE;
  G4ThreeVector fHitTime;

  G4double fHitTrkID;
};

#endif
