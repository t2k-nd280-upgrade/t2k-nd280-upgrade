
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
  void SetLengthFGD1(double length)    {fLengthFGD1=length;};
  void SetLengthFGD2(double length)    {fLengthFGD2=length;};
  void SetLengthTPCUp1(double length)  {fLengthTPCUp1=length;};
  void SetLengthTPCUp2(double length)  {fLengthTPCUp2=length;};
  void SetLengthTPCDown1(double length){fLengthTPCDown1=length;};
  void SetLengthTPCDown2(double length){fLengthTPCDown2=length;};
  void SetLengthForwTPC1(double length){fLengthForwTPC1=length;};
  void SetLengthForwTPC2(double length){fLengthForwTPC2=length;};
  void SetLengthForwTPC3(double length){fLengthForwTPC3=length;};
  void SetLengthDsECal(double length)  {fLengthDsECal=length;};
  void SetLengthBrlECal(double length) {fLengthBrlECal=length;};
  void SetLengthP0DECal(double length) {fLengthP0DECal=length;};
  void SetLengthUSECalP0D(double length) {fLengthUSECalP0D=length;};

  void SetLyzTPCUp1(double length)    {fLyzTPCUp1=length;};
  void SetLyzTPCUp2(double length)    {fLyzTPCUp2=length;};
  void SetLyzTPCDown1(double length)  {fLyzTPCDown1=length;};
  void SetLyzTPCDown2(double length)  {fLyzTPCDown2=length;};
  void SetLyzForwTPC1(double length)  {fLyzForwTPC1=length;};
  void SetLyzForwTPC2(double length)  {fLyzForwTPC2=length;};
  void SetLyzForwTPC3(double length)  {fLyzForwTPC3=length;};

  void SetEdepTarget1(double edep)  {fEdepTarget1=edep;};
  void SetEdepTarget2(double edep)  {fEdepTarget2=edep;};
  void SetEdepFGD1(double edep)     {fEdepFGD1=edep;};
  void SetEdepFGD2(double edep)     {fEdepFGD2=edep;};
  void SetEdepTPCUp1(double edep)   {fEdepTPCUp1=edep;};
  void SetEdepTPCUp2(double edep)   {fEdepTPCUp2=edep;};
  void SetEdepTPCDown1(double edep) {fEdepTPCDown1=edep;};
  void SetEdepTPCDown2(double edep) {fEdepTPCDown2=edep;};
  void SetEdepForwTPC1(double edep) {fEdepForwTPC1=edep;};
  void SetEdepForwTPC2(double edep) {fEdepForwTPC2=edep;};
  void SetEdepForwTPC3(double edep) {fEdepForwTPC3=edep;};
  void SetEdepDsECal(double edep)   {fEdepDsECal=edep;};
  void SetEdepBrlECal(double edep)  {fEdepBrlECal=edep;};
  void SetEdepP0DECal(double edep)  {fEdepP0DECal=edep;};
  void SetEdepUSECalP0D(double edep){fEdepUSECalP0D=edep;};

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
  double GetLengthFGD1()    {return fLengthFGD1;};
  double GetLengthFGD2()    {return fLengthFGD2;};
  double GetLengthTPCUp1()  {return fLengthTPCUp1;};
  double GetLengthTPCUp2()  {return fLengthTPCUp2;};
  double GetLengthTPCDown1(){return fLengthTPCDown1;};
  double GetLengthTPCDown2(){return fLengthTPCDown2;};
  double GetLengthForwTPC1(){return fLengthForwTPC1;};
  double GetLengthForwTPC2(){return fLengthForwTPC2;};
  double GetLengthForwTPC3(){return fLengthForwTPC3;};
  double GetLengthDsECal()  {return fLengthDsECal;};
  double GetLengthBrlECal() {return fLengthBrlECal;};
  double GetLengthP0DECal() {return fLengthP0DECal;};
  double GetLengthUSECalP0D(){return fLengthUSECalP0D;};

  double GetLyzTPCUp1()    {return fLyzTPCUp1;};
  double GetLyzTPCUp2()    {return fLyzTPCUp2;};
  double GetLyzTPCDown1()  {return fLyzTPCDown1;};
  double GetLyzTPCDown2()  {return fLyzTPCDown2;};
  double GetLyzForwTPC1()  {return fLyzForwTPC1;};
  double GetLyzForwTPC2()  {return fLyzForwTPC2;};
  double GetLyzForwTPC3()  {return fLyzForwTPC3;};
  
  double GetEdepTarget1()  {return fEdepTarget1;};
  double GetEdepTarget2()  {return fEdepTarget2;};
  double GetEdepFGD1()     {return fEdepFGD1;};
  double GetEdepFGD2()     {return fEdepFGD2;};
  double GetEdepTPCUp1()   {return fEdepTPCUp1;};
  double GetEdepTPCUp2()   {return fEdepTPCUp2;};
  double GetEdepTPCDown1() {return fEdepTPCDown1;};
  double GetEdepTPCDown2() {return fEdepTPCDown2;};
  double GetEdepForwTPC1() {return fEdepForwTPC1;};
  double GetEdepForwTPC2() {return fEdepForwTPC2;};
  double GetEdepForwTPC3() {return fEdepForwTPC3;};  
  double GetEdepDsECal()   {return fEdepDsECal;};
  double GetEdepBrlECal()  {return fEdepBrlECal;};
  double GetEdepP0DECal()  {return fEdepP0DECal;};
  double GetEdepUSECalP0D(){return fEdepUSECalP0D;};
  
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
  double fLengthFGD1;
  double fLengthFGD2;
  double fLengthTPCUp1;
  double fLengthTPCUp2;
  double fLengthTPCDown1;
  double fLengthTPCDown2 ;
  double fLengthForwTPC1;
  double fLengthForwTPC2;
  double fLengthForwTPC3;
  double fLengthDsECal;
  double fLengthBrlECal;
  double fLengthP0DECal;
  double fLengthUSECalP0D;
  double fLyzTPCUp1;
  double fLyzTPCUp2;
  double fLyzTPCDown1;
  double fLyzTPCDown2 ;
  double fLyzForwTPC1;
  double fLyzForwTPC2;
  double fLyzForwTPC3;
  double fEdepTarget1;
  double fEdepTarget2;
  double fEdepFGD1;
  double fEdepFGD2;
  double fEdepTPCUp1;
  double fEdepTPCUp2;
  double fEdepTPCDown1;
  double fEdepTPCDown2 ;
  double fEdepForwTPC1;
  double fEdepForwTPC2;
  double fEdepForwTPC3;
  double fEdepDsECal;
  double fEdepBrlECal;
  double fEdepP0DECal;
  double fEdepUSECalP0D;

  ClassDef(TND280UpTrack,2)
};

#endif
