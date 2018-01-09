
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
  
  void SetTrackID(int id){fTrackID=id;}
  void SetParentID(int id){fParentID=id;}
  void SetPDG(int pdg){fPDG=pdg;}
  //void SetTruthMom(double x,double y, double z){fTruthMom.SetXYZ(x,y,z);}
  void SetTruthMom(double mom){fTruthMom=mom;}
  void SetTruthLength(double len){fTruthLength=len;}
  void SetTruthCosTheta(double costheta){fTruthCosTheta=costheta;}
  void SetTruthPhi(double phi){fTruthPhi=phi;}
  void SetCharge(double charge){fCharge=charge;}
  void SetRange(double range){fRange=range;}
  void SetEdep(double ene){fEdep=ene;}
  void SetOutFV(bool isoutfv){fIsOutFV=isoutfv;}
  void SetIsReco(bool isreco){fIsReco=isreco;}
  void SetIsSeparated(bool issepa){fIsSeparated=issepa;}

  int GetTrackID() {return fTrackID;}
  int GetParentID() {return fParentID;}
  int GetPDG(){return fPDG;}
  //TVector3 GetTruthMom() {return fTruthMom;}
  double GetTruthMom(){return fTruthMom;}
  double GetTruthLength(){return fTruthLength;}
  double GetTruthCosTheta(){return fTruthCosTheta;}
  double GetTruthPhi(){return fTruthPhi;}
  double GetCharge(){return fCharge;}
  double GetRange(){return fRange;}
  double GetEdep(){return fEdep;}
  bool IsOutFV(){return fIsOutFV;}
  bool IsReco(){return fIsReco;}
  bool IsSeparated(){return fIsSeparated;}

  void PrintTrack();

private :

  int fTrackID;
  int fParentID;
  int fPDG;
  //TVector3 fTruthMom;
  double fTruthMom;
  double fTruthLength;
  double fTruthCosTheta;
  double fTruthPhi;
  double fCharge;
  double fRange; 
  double fEdep;
  bool fIsOutFV;
  bool fIsReco;
  bool fIsSeparated;

  TH2F *fMPPCHit_XY; 
  TH2F *fMPPCHit_XZ; 
  TH2F *fMPPCHit_YZ;

  ClassDef(TND280UpRecoTrack,2)
};

#endif
