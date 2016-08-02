
// 
// Class to define an ND280 upgrade track (not GEANT4 based)
// 
// 27/7/16 Davide Sgalaberna
//

#ifndef TND280UpTrack_h
#define TND280UpTrack_h 1

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

#include "TND280UpTrackPoint.hh"

using namespace std;

const double kUndefTrack = -9999999;

class TND280UpTrack : public TObject
{
public :
     
  TND280UpTrack();
  ~TND280UpTrack();
  
  void AddPoint(TND280UpTrackPoint *point);
  TND280UpTrackPoint *GetPoint(int ptid);
  int GetNPoints(){return fNPoints;};
  //void SetNPoints(int npt){fNPoints=npt;};

  void SetTrackID(int id){fTrackID=id;};
  void SetParentID(int id){fParentID=id;};
  void SetProcessName(string name){fProcessName=name;};
  void SetPDG(int pdg){fPDG=pdg;};
  void SetInitKinEnergy(double ene){fInitKinEnergy=ene;};
  void SetInitMom(double x,double y, double z){fInitMom.SetXYZ(x,y,z);};
  void SetInitCosTheta(double costheta){fInitCosTheta=costheta;};
  void SetCharge(double charge){fCharge=charge;};
  void SetRange(double range){fRange=range;};
  void SaveIt(bool saveit){fSaveIt=saveit;};
  void SetSDTotalEnergyDeposit(double sdedep){fSDTotalEnergyDeposit=sdedep;};
  void SetSDLength(double length){fSDLength=length;};

  int GetTrackID() {return fTrackID;};
  int GetParentID() {return fParentID;};
  string GetProcessName(){return fProcessName;};
  int GetPDG(){return fPDG;};
  double GetInitKinEnergy(){return fInitKinEnergy;};
  TVector3 GetInitMom() {return fInitMom;};
  double GetInitCosTheta(){return fInitCosTheta;};
  double GetCharge(){return fCharge;};
  double GetRange(){return fRange;};
  double GetSaveIt(){return fSaveIt;};
  double GetSDTotalEnergyDeposit(){return fSDTotalEnergyDeposit;};
  double GetSDLength(){return fSDLength;};
  
  void PrintTrack();

private :

  TList *fListOfPoints;
  int fNPoints;
  TList *GetListOfPoints(){return fListOfPoints;};

  int fTrackID;
  int fParentID;
  string fProcessName;
  int fPDG;
  double fInitKinEnergy;
  TVector3 fInitMom;
  double fInitCosTheta;
  double fCharge;
  double fRange; 
  bool fSaveIt;
  double fSDTotalEnergyDeposit;
  double fSDLength;

  ClassDef(TND280UpTrack,2)
};

#endif
