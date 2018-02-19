#ifndef ND280UP_TARGET_READ_OUT
#define ND280UP_TARGET_READ_OUT

//#include <TString.h>

#include "ND280UpConst.hh"
#include <TRandom3.h>

using namespace nd280upconv;

class ND280UpTargReadOut {
public:
  ND280UpTargReadOut();
  ~ND280UpTargReadOut();

  void SetTargType(TargetType_t targetid){fTargetID=targetid;};
  double GetTargType(){return fTargetID;};
  TString AsStringTargType(TargetType_t targetid);
  
  double ApplyScintiResponse(double &edep,double steplength,double charge);  
  void ApplyFiberResponse(double &nphot,double &time,double x);
  void ApplyMPPCResponse(double &npe);

private:
 
  void BirksSaturation(double &edeposit,double steplength,double charge);
  double EdepToPhot(double edep);
  
  void ApplyFiberAttenuation(double &nphot,double x);
  void ApplyFiberTime(double &time,double x);
  double GetPhotAtt_FGD(double Nphot0,double x);
  double GetPhotAtt_SciFi(double Nphot0,double x);

  double fAttLength;
  TargetType_t fTargetID;
  TRandom3 *fRndm;
};

#endif
