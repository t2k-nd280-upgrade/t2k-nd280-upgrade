#ifndef ND280UP_PID
#define ND280UP_PID

#include <TH2F.h>

class ND280UpPID {
public:
  ND280UpPID();
  ~ND280UpPID();

  void ComputePDFs();

  void SetMPPCHit_XY(TH2F *h2d){fMPPCHit_XY=new TH2F(*h2d);}
  void SetMPPCHit_XZ(TH2F *h2d){fMPPCHit_XZ=new TH2F(*h2d);}
  void SetMPPCHit_YZ(TH2F *h2d){fMPPCHit_YZ=new TH2F(*h2d);}

  TH2F *GetMPPCHit_XY(){return fMPPCHit_XY;}
  TH2F *GetMPPCHit_XZ(){return fMPPCHit_XZ;}
  TH2F *GetMPPCHit_YZ(){return fMPPCHit_YZ;}

  void SetTrackID(int id){fTrackID=id;}
  int GetTrackID(){return fTrackID;}

  void SetPDG(int id){fPDG=id;}
  int GetPDG(){return fPDG;}
  
private:

  void Init();
  void Delete();

  TH2F *fMPPCHit_XY;
  TH2F *fMPPCHit_XZ;
  TH2F *fMPPCHit_YZ;

  TH1F *fEdep_Muon;
  TH1F *fEdep_Pion;
  TH1F *fEdep_Prot;
  TH1F *fEdep_Elec;

  int fTrackID;
  int fPDG;
};

#endif
