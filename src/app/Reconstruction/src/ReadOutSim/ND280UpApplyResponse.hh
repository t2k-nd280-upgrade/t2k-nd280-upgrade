#ifndef ND280UP_APPLY_RESPONSE
#define ND280UP_APPLY_RESPONSE

#include <TH2F.h>
#include <TVector3.h>

#include "ND280UpTrackerResponse.hh"
#include "ND280UpTargReadOut.hh"
#include "ND280UpConst.hh"

class ND280UpApplyResponse {
public:
  ND280UpApplyResponse();
  ~ND280UpApplyResponse();

  void CalcResponse(TVector3 lightPos,int trkid,int parid,double charge,double time,double steplength, double edep, TString touch_namedet);

  TVector3 GetHitPos(){return fHitPos;}
  TVector3 GetHitPE(){return fHitPhot;}
  TVector3 GetHitTime(){return fHitTime;}
  double GetHitTrkID(){return fHitTrkID;}

  void SetTargetID(nd280upconv::TargetType_t id){ftargetid = id;}
  nd280upconv::TargetType_t GetTargetID(){return ftargetid;}

  //////////////////////////////////////
  //                                  //
  // From ND280RootPersistencyManager //
  //                                  //
  //////////////////////////////////////

  //
  // Define the position of the Target MPPCs  
  // Same reference system as the navigator 
  // i.e. set in DetectorConstruction
  //
  
  void SetMPPCProj2D_XY(TH2F *h2d);
  void SetMPPCProj2D_XZ(TH2F *h2d);
  void SetMPPCProj2D_YZ(TH2F *h2d);

  TH2F *GetMPPCProj2D_XY();
  TH2F *GetMPPCProj2D_XZ();
  TH2F *GetMPPCProj2D_YZ();

  // Assume the MPPC read-out plane is at x,y,z<0
  // Assume at least 2 projections exist
  double GetMPPCPosX(double Zpos);
  double GetMPPCPosY(double Zpos);
  double GetMPPCPosZ(double Xpos);

  // return the MPPC type in the XZ plane based on its Z postion
  int GetMPPCType(double Z);

  // Position of the MPPC hits
  void GetHitPosXY(double lightX, double lightY, double &mppcX, double &mppcY);
  void GetHitPosXZ(double lightX, double lightZ, double &mppcX, double &mppcZ);
  void GetHitPosYZ(double lightY, double lightZ, double &mppcY, double &mppcZ);
  
  void SetDetNameAlongX(TString name){fDetNameAlongX=name;};
  void SetDetNameAlongY(TString name){fDetNameAlongY=name;};
  void SetDetNameAlongZ(TString name){fDetNameAlongZ=name;};
  TString GetDetNameAlongX(){return fDetNameAlongX;};
  TString GetDetNameAlongY(){return fDetNameAlongY;};
  TString GetDetNameAlongZ(){return fDetNameAlongZ;};

  /////////////

private:

  void Init();
  void Delete();
  
  // Define the XML input parser
  // ND280RootPersistencyManager* persistencyManager;

  TVector3 fHitPos;
  TVector3 fHitPhot;
  TVector3 fHitTime;

  nd280upconv::TargetType_t ftargetid;

  double fHitTrkID;



  //////////////////////////////////////
  //                                  //
  // From ND280RootPersistencyManager //
  //                                  //
  //////////////////////////////////////

  TH2F *fMPPCProj2D_XY;
  TH2F *fMPPCProj2D_XZ;
  TH2F *fMPPCProj2D_YZ;

  bool fIsMPPCProjXY;  
  bool fIsMPPCProjXZ;  
  bool fIsMPPCProjYZ;  

  TString fDetNameAlongX;
  TString fDetNameAlongY;
  TString fDetNameAlongZ;

  Float_t fDetectorXsize;
  Float_t fDetectorYsize;
  Float_t fDetectorZsize;
};

#endif
