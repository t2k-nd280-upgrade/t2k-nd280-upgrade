#ifndef TARGET_READ_OUT
#define TARGET_READ_OUT

#include "G4ThreeVector.hh"
//#include "G4EmCalculator.hh"
#include "G4Track.hh"


namespace conv {
  // Combined or single analysis 
  typedef enum ETargetType
    {
      kUndefined = -999999,
      kSuperFGD,
      kFGD,
      kFGD3D,
      kWAGASCI,
      kSciFi,
      kFGDlike
    }
    TargetType_t;
} // conv



using namespace conv;

class ExN02TargReadOut {
public:
  ExN02TargReadOut();
  ~ExN02TargReadOut();

  void SetTargType(TargetType_t targetid){fTargetID=targetid;};
  G4double GetTargType(){return fTargetID;};
  G4String AsStringTargType(TargetType_t targetid);

  G4double ApplyScintiResponse(G4double &edep,G4double steplength,G4Track *aTrack);  
  G4double ApplyScintiResponse(G4double &edep,G4double steplength,G4double charge);  
  void ApplyFiberResponse(G4double &nphot, G4double &time, G4double x);
  void ApplyMPPCResponse(G4double &npe);

private:
 
  void BirksSaturation(G4double &edeposit, G4double steplength, G4Track *aTrack);
  void BirksSaturation(G4double &edeposit, G4double steplength, G4double charge);
  G4double EdepToPhot(G4double edep);
  
  void ApplyFiberAttenuation(G4double &nphot, G4double x);
  void ApplyFiberTime(G4double &time,G4double x);
  G4double GetPhotAtt_FGD(G4double Nphot0,G4double x);
  G4double GetPhotAtt_SciFi(G4double Nphot0,G4double x);

  G4double fAttLength;
  TargetType_t fTargetID;
};

#endif
