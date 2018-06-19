#ifndef ND280UP_TARGET_READ_OUT
#define ND280UP_TARGET_READ_OUT

//#include <TString.h>

#include "ND280UpConst.hh"
#include <TRandom3.h>

// 0 - Full size SuperFGD 1 - 5x5x5 2 - 24x8x48
#define PROTO 0
#define ELECSIM
#define CROSSTALK 3

using namespace nd280upconv;

class ND280UpTargReadOut {
public:
  ND280UpTargReadOut();
  ~ND280UpTargReadOut();

  void SetTargType(TargetType_t targetid){fTargetID=targetid;};
  double GetTargType(){return fTargetID;};
  TString AsStringTargType(TargetType_t targetid);
  
  double ApplyScintiResponse(double &edep,double steplength,double charge);  
  void ApplyFiberResponse(double &nphot,double &time,double x, double DetSize = 0.);
  void ApplyMPPCResponse(double &npe);

private:
 
  void BirksSaturation(double &edeposit,double steplength,double charge);
  double EdepToPhot(double edep);
  
  void ApplyFiberAttenuation(double &nphot,double x, double DetSize = 0.);
  void ApplyFiberTime(double &time,double x);
  double GetPhotAtt_FGD(double Nphot0,double x, double DetSize = 0.);
  double GetPhotAtt_SciFi(double Nphot0,double x);

  double fAttLength;
  TargetType_t fTargetID;
  TRandom3 *fRndm;
};

#endif
