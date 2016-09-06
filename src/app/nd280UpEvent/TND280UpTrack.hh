
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
  
  void SetLengthTarget1(double length) {fLengthTarget1=length;};
  void SetLengthTarget2(double length) {fLengthTarget2=length;};
  void SetLengthTPCUp1(double length)  {fLengthTPCUp1=length;};
  void SetLengthTPCUp2(double length)  {fLengthTPCUp2=length;};
  void SetLengthTPCDown1(double length){fLengthTPCDown1=length;};
  void SetLengthTPCDown2(double length){fLengthTPCDown2=length;};
  void SetLengthForwTPC1(double length){fLengthForwTPC1=length;};
  void SetLengthForwTPC2(double length){fLengthForwTPC2=length;};
  void SetLengthForwTPC3(double length){fLengthForwTPC3=length;};

  void SetLyzTPCUp1(double length)    {fLyzTPCUp1=length;};
  void SetLyzTPCUp2(double length)    {fLyzTPCUp2=length;};
  void SetLyzTPCDown1(double length)  {fLyzTPCDown1=length;};
  void SetLyzTPCDown2(double length)  {fLyzTPCDown2=length;};
  void SetLyzForwTPC1(double length)  {fLyzForwTPC1=length;};
  void SetLyzForwTPC2(double length)  {fLyzForwTPC2=length;};
  void SetLyzForwTPC3(double length)  {fLyzForwTPC3=length;};

  void SetEdepTarget1(double length)  {fEdepTarget1=length;};
  void SetEdepTarget2(double length)  {fEdepTarget2=length;};
  void SetEdepTPCUp1(double length)   {fEdepTPCUp1=length;};
  void SetEdepTPCUp2(double length)   {fEdepTPCUp2=length;};
  void SetEdepTPCDown1(double length) {fEdepTPCDown1=length;};
  void SetEdepTPCDown2(double length) {fEdepTPCDown2=length;};
  void SetEdepForwTPC1(double length) {fEdepForwTPC1=length;};
  void SetEdepForwTPC2(double length) {fEdepForwTPC2=length;};
  void SetEdepForwTPC3(double length) {fEdepForwTPC3=length;};

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
  
  double GetLengthTarget1() {return fLengthTarget1;};
  double GetLengthTarget2() {return fLengthTarget2;};
  double GetLengthTPCUp1()  {return fLengthTPCUp1;};
  double GetLengthTPCUp2()  {return fLengthTPCUp2;};
  double GetLengthTPCDown1(){return fLengthTPCDown1;};
  double GetLengthTPCDown2(){return fLengthTPCDown2;};
  double GetLengthForwTPC1(){return fLengthForwTPC1;};
  double GetLengthForwTPC2(){return fLengthForwTPC2;};
  double GetLengthForwTPC3(){return fLengthForwTPC3;};

  double GetLyzTPCUp1()    {return fLyzTPCUp1;};
  double GetLyzTPCUp2()    {return fLyzTPCUp2;};
  double GetLyzTPCDown1()  {return fLyzTPCDown1;};
  double GetLyzTPCDown2()  {return fLyzTPCDown2;};
  double GetLyzForwTPC1()  {return fLyzForwTPC1;};
  double GetLyzForwTPC2()  {return fLyzForwTPC2;};
  double GetLyzForwTPC3()  {return fLyzForwTPC3;};
  
  double GetEdepTarget1()  {return fEdepTarget1;};
  double GetEdepTarget2()  {return fEdepTarget2;};
  double GetEdepTPCUp1()   {return fEdepTPCUp1;};
  double GetEdepTPCUp2()   {return fEdepTPCUp2;};
  double GetEdepTPCDown1() {return fEdepTPCDown1;};
  double GetEdepTPCDown2() {return fEdepTPCDown2;};
  double GetEdepForwTPC1() {return fEdepForwTPC1;};
  double GetEdepForwTPC2() {return fEdepForwTPC2;};
  double GetEdepForwTPC3() {return fEdepForwTPC3;};

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
  double fLengthTarget1;
  double fLengthTarget2;
  double fLengthTPCUp1;
  double fLengthTPCUp2;
  double fLengthTPCDown1;
  double fLengthTPCDown2 ;
  double fLengthForwTPC1;
  double fLengthForwTPC2;
  double fLengthForwTPC3;
  double fLyzTPCUp1;
  double fLyzTPCUp2;
  double fLyzTPCDown1;
  double fLyzTPCDown2 ;
  double fLyzForwTPC1;
  double fLyzForwTPC2;
  double fLyzForwTPC3;
  double fEdepTarget1;
  double fEdepTarget2;
  double fEdepTPCUp1;
  double fEdepTPCUp2;
  double fEdepTPCDown1;
  double fEdepTPCDown2 ;
  double fEdepForwTPC1;
  double fEdepForwTPC2;
  double fEdepForwTPC3;

  ClassDef(TND280UpTrack,2)
};

#endif
