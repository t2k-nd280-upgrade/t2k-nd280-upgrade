
// 
// Class to define an ND280 upgrade hits (not GEANT4 based)
// 
// 26/4/17 Davide Sgalaberna
//

#ifndef TND280UpHit_h
#define TND280UpHit_h 1

#include <TTree.h>
#include <TFile.h>
#include <TBits.h>
#include <TObjString.h>
#include <TVector3.h>
#include <TObject.h>
#include <TList.h>
#include <TCollection.h>
#include <TIterator.h>

#include <iostream>
#include <vector>

using namespace std;

const double kUndefHit = -9999999;

class TND280UpHit : public TObject
{
public :
     
  TND280UpHit();
  ~TND280UpHit();
  
  // true
  void SetHitID(int id){fHitID=id;};
  void SetPDG(int pdg){fPDG=pdg;};
  void SetTrackID(int trkid){fTrackID=trkid;};
  void SetParentID(int id){fParentID=id;};
  void SetEdep(double edep){fEdep=edep;};
  void SetLocPosX(double x){fLocPosX=x;};
  void SetLocPosY(double y){fLocPosY=y;};
  void SetLocPosZ(double z){fLocPosZ=z;};
  void SetTime(double time){fTime=time;};
  void SetDetName(string name){fDetName=name;};
  // reco
  void SetPEX(double pe){fPEX=pe;};
  void SetPEY(double pe){fPEY=pe;};
  void SetPEZ(double pe){fPEZ=pe;};
  void SetMPPCPosX(double x){fMPPCPosX=x;};
  void SetMPPCPosY(double y){fMPPCPosY=y;};
  void SetMPPCPosZ(double z){fMPPCPosZ=z;};
  void SetTimePEX(double time){fTimePEX=time;};
  void SetTimePEY(double time){fTimePEY=time;};
  void SetTimePEZ(double time){fTimePEZ=time;};
  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......   
  
  // true
  int GetHitID() {return fHitID;};
  int GetPDG(){return fPDG;};
  int GetTrackID(){return fTrackID;};
  int GetParentID() {return fParentID;};
  double GetEdep(){return fEdep;};
  double GetLocPosX(){return fLocPosX;};
  double GetLocPosY(){return fLocPosY;};
  double GetLocPosZ(){return fLocPosZ;};
  double GetTime(){return fTime;};
  string GetDetName(){return fDetName;};
  // reco
  double GetPEX(){return fPEX;};
  double GetPEY(){return fPEY;};
  double GetPEZ(){return fPEZ;};
  double GetMPPCPosX(){return fMPPCPosX;};
  double GetMPPCPosY(){return fMPPCPosY;};
  double GetMPPCPosZ(){return fMPPCPosZ;};
  double GetTimePEX(){return fTimePEX;};
  double GetTimePEY(){return fTimePEY;};
  double GetTimePEZ(){return fTimePEZ;};
 
  void PrintHit();



  /////// NEW ND280 HitSegment
  
  //
  // The following variables are same as used in nd280mc in TG4HitSegment
  // They are inherited by ExN02TrackerHit
  //

public:

  std::vector<int> fContributors;
  int fPrimaryId;
  double fStartX;
  double fStartY;
  double fStartZ;
  double fStartT;
  double fStopX;
  double fStopY;
  double fStopZ;
  double fStopT;
  double fEnergyDeposit;
  double fTrackLength;

  inline int GetPrimaryId(){return fPrimaryId;}
  inline double GetStartX(){return fStartX;}
  inline double GetStartY(){return fStartY;}
  inline double GetStartZ(){return fStartZ;}
  inline double GetStartT(){return fStartT;}
  inline double GetStopX(){return fStopX;}
  inline double GetStopY(){return fStopY;}
  inline double GetStopZ(){return fStopZ;}
  inline double GetStopT(){return fStopT;}
  inline double GetEnergyDeposit(){return fEnergyDeposit;}
  inline double GetTrackLength(){return fTrackLength;}

  inline void SetPrimaryId(int id){fPrimaryId = id;}
  inline void SetStartX(double pos){fStartX=pos;}
  inline void SetStartY(double pos){fStartY=pos;}
  inline void SetStartZ(double pos){fStartZ=pos;}
  inline void SetStartT(double time){fStartT=time;}
  inline void SetStopX(double pos){fStopX=pos;}
  inline void SetStopY(double pos){fStopY=pos;}
  inline void SetStopZ(double pos){fStopZ=pos;}
  inline void SetStopT(double time){fStopT=time;}
  inline void SetEnergyDeposit(double edep){fEnergyDeposit=edep;}
  inline void SetTrackLength(double length){fTrackLength=length;}

  ////////////////////




private :
  
  // true
  int fHitID;
  int fPDG;
  int fTrackID;
  int fParentID;
  double fEdep;
  double fLocPosX;
  double fLocPosY;
  double fLocPosZ;
  double fTime;
  string fDetName;
  // reco
  double fPEX;
  double fPEY;
  double fPEZ;
  double fMPPCPosX;
  double fMPPCPosY;
  double fMPPCPosZ;
  double fTimePEX;
  double fTimePEY;
  double fTimePEZ;

  ClassDef(TND280UpHit,5)
};

#endif
