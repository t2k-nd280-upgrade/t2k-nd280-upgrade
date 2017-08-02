#ifndef ND280UP_RECOTRACK
#define ND280UP_RECOTRACK

#include "ND280UpConst.hh"

#include <TH2F.h>

using namespace std;

//
// Davide Sgalaberna 15/7/17
//
// Container of methods to reconstruct single tracks
//

class ND280UpRecoTrack {
public:
  
  ND280UpRecoTrack();
  ~ND280UpRecoTrack();
  
  void SetMPPCXY(TH2F *h){f2d_xy=h;}
  void SetMPPCXZ(TH2F *h){f2d_xz=h;}
  void SetMPPCYZ(TH2F *h){f2d_yz=h;}

  void DoTracking(nd280upconv::TargetType_t dettype);

  nd280upconv::TargetType_t GetDetectorType(){return fDetType;}

  double GetRecoCosTheta(){return fRecoCosTheta;}
  double GetRecoMom(){return fRecoMom;}

  double GetLength(){return fTrkLength;}
  double GetLengthX(){return fTrkLengthX;}
  double GetLengthY(){return fTrkLengthY;}
  double GetLengthZ(){return fTrkLengthZ;}

  double GetLengthStraight(){return fTrkLengthStraight;}
  double GetLengthStraightX(){return fTrkLengthStraightX;}
  double GetLengthStraightY(){return fTrkLengthStraightY;}
  double GetLengthStraightZ(){return fTrkLengthStraightZ;}

  double GetLengthFit(){return fTrkLengthFit;}
  double GetLengthFitX(){return fTrkLengthFitX;}
  double GetLengthFitY(){return fTrkLengthFitY;}
  double GetLengthFitZ(){return fTrkLengthFitZ;}
  
  double GetEdep(){return fTrkEdep;}
  
  bool IsViewXY(){ return fUseView_xy;}
  bool IsViewXZ(){ return fUseView_xz;}
  bool IsViewYZ(){ return fUseView_yz;}
  int GetNumOfViews(){return fNumView;}

  int GetNHits(){return fNHits;}
  int GetNHitsXY(){return fNHitsXY;}
  int GetNHitsYZ(){return fNHitsYZ;}
  int GetNHitsXZ(){return fNHitsXZ;}

  bool IsOutFV(){return fIsOutFV;}
  bool IsReco(){return fIsReco;}
  
private:

  void Init();
  void Delete();

  // Methods for tracking
  void CleanMPPCHits();
  void CalcLength();
  void CalcLengthFit();
  void CalcLengthStraight();
  void CalcNHits();
  void CalcEdep();
  void CalcOutFV();
  void DefineRecoTrack(); // cuts 
  void CalcRecoVar(); 
  //

  void SetIsReco(bool isreco){fIsReco = isreco;}

  void SetLength(double len){fTrkLength=len;}
  void SetLengthX(double len){fTrkLengthX=len;}
  void SetLengthY(double len){fTrkLengthY=len;}
  void SetLengthZ(double len){fTrkLengthZ=len;}

  // Event display of MPPC hits with single track
  TH2F *f2d_xy; 
  TH2F *f2d_xz;
  TH2F *f2d_yz;
  
  double fTrkLengthStraight; // Track length (farthest hits)
  double fTrkLengthStraightX; // Track length (farthest hits)
  double fTrkLengthStraightY; // Track length (farthest hits)
  double fTrkLengthStraightZ; // Track length (farthest hits)

  double fTrkLengthFit; // from fit of hits
  double fTrkLengthFitX; // from fit of hits
  double fTrkLengthFitY; // from fit of hits
  double fTrkLengthFitZ; // from fit of hits

  double fTrkLength; // from fit of hits
  double fTrkLengthX; // from fit of hits
  double fTrkLengthY; // from fit of hits
  double fTrkLengthZ; // from fit of hits

  double fTrkEdep; // total energy deposited in single track MPPC hits (all views)

  bool fUseView_xy;
  bool fUseView_xz;
  bool fUseView_yz;
  int fNumView;
  bool fIsOutFV;
  bool fIsReco;

  int fNHitsXY;
  int fNHitsYZ;
  int fNHitsXZ;
  int fNHits;

  double fRecoCosTheta;
  double fRecoMom;

  nd280upconv::TargetType_t fDetType;  
};

#endif
