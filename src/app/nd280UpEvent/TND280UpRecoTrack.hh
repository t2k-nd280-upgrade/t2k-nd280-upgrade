
// 
// Class to define an ND280 upgrade reco track (not GEANT4 based)
// 
// 4/7/17 Davide Sgalaberna
//

#ifndef TND280UpRecoTrack_h
#define TND280UpRecoTrack_h 1

#include <TTree.h>
#include <TFile.h>
#include <TBits.h>
#include <TObjString.h>
#include <TVector3.h>
#include <TObject.h>
#include <TList.h>
#include <TCollection.h>
#include <TIterator.h>
#include <TH2F.h>

#include <iostream>
#include <vector>

using namespace std;

class TND280UpRecoTrack : public TObject
{
public :
     
  TND280UpRecoTrack();
  ~TND280UpRecoTrack();
  
  void SetTrackID(int id){fTrackID=id;};
  void SetParentID(int id){fParentID=id;};
  void SetPDG(int pdg){fPDG=pdg;};
  void SetInitMom(double x,double y, double z){fInitMom.SetXYZ(x,y,z);};
  void SetInitCosTheta(double costheta){fInitCosTheta=costheta;};
  void SetCharge(double charge){fCharge=charge;};
  void SetRange(double range){fRange=range;};

  int GetTrackID() {return fTrackID;};
  int GetParentID() {return fParentID;};
  int GetPDG(){return fPDG;};
  TVector3 GetInitMom() {return fInitMom;};
  double GetInitCosTheta(){return fInitCosTheta;};
  double GetCharge(){return fCharge;};
  double GetRange(){return fRange;};

  void SetMPPCHit_XY(TH2F* h2d){fMPPCHit_XY=new TH2F(*h2d);}
  void SetMPPCHit_XZ(TH2F* h2d){fMPPCHit_XZ=new TH2F(*h2d);}
  void SetMPPCHit_YZ(TH2F* h2d){fMPPCHit_YZ=new TH2F(*h2d);}

  TH2F *GetMPPCHit_XY(){return fMPPCHit_XY;}
  TH2F *GetMPPCHit_XZ(){return fMPPCHit_XZ;}
  TH2F *GetMPPCHit_YZ(){return fMPPCHit_YZ;}

  void PrintTrack();

private :

  int fTrackID;
  int fParentID;
  int fPDG;
  TVector3 fInitMom;
  double fInitCosTheta;
  double fCharge;
  double fRange; 

  TH2F *fMPPCHit_XY; 
  TH2F *fMPPCHit_XZ; 
  TH2F *fMPPCHit_YZ; 

  ClassDef(TND280UpRecoTrack,2)
};

#endif
