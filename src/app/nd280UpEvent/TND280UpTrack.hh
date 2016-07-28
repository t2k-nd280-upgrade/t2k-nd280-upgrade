
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
#include <vector>
#include <TObject.h>
#include <TList.h>
#include <TCollection.h>
#include <TIterator.h>

#include <iostream>

#include "TND280UpTrackPoint.hh"

using namespace std;

class TND280UpTrack : public TObject
{
public :
     
  TND280UpTrack();
  ~TND280UpTrack();
  
  void AddPoint(TND280UpTrackPoint *point);
  TND280UpTrackPoint *GetPoint(int ptid);
  int GetNPoints(){return fNPoints;};

  void SetTrackID(int id){fTrackID=id;};
  void SetParentID(int id){fParentID=id;};
  void SetProcessName(string name){fProcessName=name;};
  void SetPDG(int pdg){fPDG=pdg;};
  void SetInitKinEnergy(double ene){fInitKinEnergy=ene;};
  void SetInitMom(double mom){fInitMom=mom;};
  void SetInitCosTheta(double costheta){fInitCosTheta=costheta;};
  void SetCharge(double charge){fCharge=charge;};
  void SetRange(double range){fRange=range;};
  void SaveIt(bool saveit){fSaveIt=saveit;};

  int GetTrackID() {return fTrackID;};
  int GetParentID() {return fParentID;};
  string GetProcessName(){return fProcessName;};
  int GetPDG(){return fPDG;};
  double GetInitKinEnergy(){return fInitKinEnergy;};
  double GetInitMom()     {return fInitMom;};
  double GetInitCosTheta(){return fInitCosTheta;};
  double GetCharge(){return fCharge;};
  double GetRange(){return fRange;};
  double GetSaveIt(){return fSaveIt;};

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
  double fInitMom;
  double fInitCosTheta;
  double fCharge;
  double fRange; 
  bool fSaveIt;
  
  //const double kUndefinedTrack;
  
  ClassDef(TND280UpTrack,2)
};

#endif
