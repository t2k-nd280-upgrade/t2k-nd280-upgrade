
// 
// Class to define an ND280 upgrade track point (not GEANT4 based)
// 
// 27/7/16 Davide Sgalaberna
//

#ifndef TND280UpTrackPoint_h
#define TND280UpTrackPoint_h 1

#include <TTree.h>
#include <TFile.h>
#include <TBits.h>
#include <TObjString.h>
#include <TObject.h>
#include <TVector3.h>

#include <iostream>
#include <vector>

using namespace std;

const double kUndefTrackPoint = -9999999;

class TND280UpTrackPoint : public TObject
{
public :
     
  TND280UpTrackPoint();
  ~TND280UpTrackPoint();
  
  void SetPointID(int id){fPointID=id;};
  void SetTime(double time){fTime=time;};
  void SetMomentum(double x,double y,double z){fMomentum.SetXYZ(x,y,z);};
  void SetEdep(double edep){fEdep=edep;};
  void SetStepLength(double steplength){fStepLength=steplength;};
  void SetStepDeltaLyz(double stepdeltalyz){fStepDeltaLyz=stepdeltalyz;};
  void SetStepStatus(int status){fStepStatus=status;};
  void SetPhysVolName(string detname){fPhysVolName=detname;};
  void SetPrePosition(double x,double y,double z){fPrePosition.SetXYZ(x,y,z);};
  void SetPostPosition(double x,double y,double z){fPostPosition.SetXYZ(x,y,z);};
  void SetIsOnBoundary(bool is){fIsOnBoundary=is;};
  void SaveIt(bool save){fSaveIt=save;};

  int GetPointID() {return fPointID;};
  double GetTime() {return fTime;};
  TVector3 GetMomentum() {return fMomentum;};
  double GetEdep() {return fEdep;};
  double GetStepLength() { return fStepLength;};
  double GetStepDeltaLyz() {return fStepDeltaLyz;};
  int GetStepStatus() {return fStepStatus;};
  string GetPhysVolName() {return fPhysVolName;};
  TVector3 GetPrePosition() {return fPrePosition;};
  TVector3 GetPostPosition() {return fPostPosition;};
  bool IsOnBoundary() {return fIsOnBoundary;};
  bool GetSaveIt() {return fSaveIt;};

  void PrintTrackPoint();
  
private :
  
  int fPointID;
  double fTime;
  TVector3 fMomentum;
  double fEdep;
  double fStepLength;
  double fStepDeltaLyz;
  int fStepStatus;
  string fPhysVolName;
  TVector3 fPrePosition;
  TVector3 fPostPosition;
  bool fIsOnBoundary;
  bool fSaveIt;

  ClassDef(TND280UpTrackPoint,3)
};

#endif
