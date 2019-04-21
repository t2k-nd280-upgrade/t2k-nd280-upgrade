#ifndef _HIT_H_
#define _HIT_H_

#include <TObject.h>
#include <TClonesArray.h>

class Hit : public TObject {
private:
  Int_t         fview;            //(0=XY, 1=XZ, 2=ZY)
  Int_t         fX;               //X coordinate of hit
  Int_t         fY;               //Y coordinate of hit
  Int_t         fZ;               //Z coordinate of hit
  Double_t      fCharge;          //Charge_pe of hit
  Double_t      fHG_pe;           //HG_pe of hit
  Double_t      fLG_pe;           //LG_pe of hit
  Double_t      fToT_pe;          //ToT_pe of hit
  Double_t      fHitAmpl;         //Amplitude of hit in ADC
  Double_t      fHitLGAmpl;       //LG Amplitude of hit in ADC
  Double_t      fRE;              //hit lead time
  Double_t      fFE;              //hit trail time
  Double_t      fToT;             //ToT of hit
  Double_t      fDt;              //time difference between trigger and hit time ( FEB[i].hitLeadTime – FEB[12].hitLeadTime )
  Double_t      fSpillTag;        //Spill tag of hit
  Double_t      fFEB;             //FEB
  Double_t      fCh;              //Channel
  Double_t      fTfromSpill;      //hitTimefromSpill
  Double_t      fGTrigTag;        //GTrigTag
  Double_t      fGTrigTime;       //GTrigTime
  Double_t      fSpillTime;       //Spill time
  Double_t      fSpillTrailTime;  //Spill trail time

  Int_t         fMulti;           //Number of times the hit is used to reconstruct 3D voxels
  bool          fTrueXTalk;       //crosstalk flag from montecarlo (true means it is crosstalk)
  bool          fRecoXTalk;       //crosstalk flag from reconstruction (true means it is crosstalk)
  Double_t      fTrueQxy;         //true Q in the XY plane from MC
  Double_t      fTrueQxz;         //true Q in the XZ plane from MC
  Double_t      fTrueQyz;         //true Q in the YZ plane from MC
  Double_t      fTrueX;           //true X from MC
  Double_t      fTrueY;           //true Y from MC
  Double_t      fTrueZ;           //true Z from MC
  bool          fMC;              //true if hit produced in MC, false otherwise

public:
  Hit(){ fMulti = 0; };
  virtual ~Hit() { };

  void Clear(Option_t* /*option*/="")
  {
    TObject::Clear();
  }

  //functions to store data
  void SetView(Int_t p_view) {fview = p_view;}
  void SetX(Int_t p_X) {fX = p_X;}
  void SetY(Int_t p_Y) {fY = p_Y;}
  void SetZ(Int_t p_Z) {fZ = p_Z;}
  void SetCharge(Double_t p_Charge) {fCharge = p_Charge;}
  void SetHG_pe(Double_t p_HG) {fHG_pe = p_HG;}
  void SetLG_pe(Double_t p_LG) {fLG_pe = p_LG;}
  void SetToT_pe(Double_t p_ToT) {fToT_pe = p_ToT;}
  void SetHitAmpl(Double_t p_HitAmpl) {fHitAmpl = p_HitAmpl;}
  void SetHitLGAmpl(Double_t p_HitLGAmpl) {fHitLGAmpl = p_HitLGAmpl;}
  void SetRE(Double_t p_RE) {fRE = p_RE;}
  void SetFE(Double_t p_FE) {fFE = p_FE;}
  void SetToT(Double_t p_ToT) {fToT = p_ToT;}
  void SetDt(Double_t p_Dt) {fDt = p_Dt;}
  void SetSpillTag(Double_t p_SpillTag) {fSpillTag = p_SpillTag;}
  void SetFEB(Double_t p_FEB) {fFEB = p_FEB;}
  void SetCh(Double_t p_Ch) {fCh = p_Ch;}
  void SetTfromSpill(Double_t p_TfromSpill) {fTfromSpill = p_TfromSpill;}
  void SetGTrigTag(Double_t p_GTrigTag) {fGTrigTag = p_GTrigTag;}
  void SetGTrigTime(Double_t p_GTrigTime) {fGTrigTime = p_GTrigTime;}
  void SetSpillTime(Double_t p_SpillTime) {fSpillTime = p_SpillTime;}
  void SetSpillTrailTime(Double_t p_SpillTrailTime) {fSpillTrailTime = p_SpillTrailTime;}

  void SetMultiplicity(Int_t p_fMulti) {fMulti = p_fMulti;}
  void SetTrueXTalk(bool p_fTrueXTalk) {fTrueXTalk = p_fTrueXTalk;}
  void SetRecoXTalk(bool p_fRecoXTalk) {fRecoXTalk = p_fRecoXTalk;}
  void SetTrueChargeXY(Double_t p_fTrueQxy) {fTrueQxy = p_fTrueQxy;}
  void SetTrueChargeXZ(Double_t p_fTrueQxz) {fTrueQxz = p_fTrueQxz;}
  void SetTrueChargeYZ(Double_t p_fTrueQyz) {fTrueQyz = p_fTrueQyz;}
  void SetTrueX(Double_t p_fTrueX) {fTrueX = p_fTrueX;}
  void SetTrueY(Double_t p_fTrueY) {fTrueY = p_fTrueY;}
  void SetTrueZ(Double_t p_fTrueZ) {fTrueZ = p_fTrueZ;}
  void SetMC(bool p_fMC) {fMC = p_fMC;}


  void SetAll(Hit* in);   //used to copy all hit info from one hit to another in the event selection


  //functions to retrieve data
  Int_t GetView() {return fview;}
  Int_t GetX() {return fX;}
  Int_t GetY() {return fY;}
  Int_t GetZ() {return fZ;}
  Double_t GetCharge() {return fCharge;}
  Double_t GetHG_pe() {return fHG_pe;}
  Double_t GetLG_pe() {return fLG_pe;}
  Double_t GetToT_pe() {return fToT_pe;}
  Double_t GetHitAmpl() {return fHitAmpl;}
  Double_t GetHitLGAmpl() {return fHitLGAmpl;}
  Double_t GetRE() {return fRE;}
  Double_t GetFE() {return fFE;}
  Double_t GetToT() {return fToT;}
  Double_t GetDt() {return fDt;}
  Double_t GetSpillTag() {return fSpillTag;}
  Double_t GetFEB() {return fFEB;}
  Double_t GetCh() {return fCh;}
  Double_t GetTfromSpill() {return fTfromSpill;}
  Double_t GetGTrigTag() {return fGTrigTag;}
  Double_t GetGTrigTime() {return fGTrigTime;}
  Double_t GetSpillTime() {return fSpillTime;}
  Double_t GetSpillTrailTime() {return fSpillTrailTime;}

  Int_t GetMultiplicity() {return fMulti;}
  bool GetTrueXTalk() {return fTrueXTalk;}
  bool GetRecoXTalk() {return fTrueXTalk;}
  Double_t GetTrueChargeXY() {return fTrueQxy;}
  Double_t GetTrueChargeXZ() {return fTrueQxz;}
  Double_t GetTrueChargeYZ() {return fTrueQyz;}
  Double_t GetTrueX() {return fTrueX;}
  Double_t GetTrueY() {return fTrueY;}
  Double_t GetTrueZ() {return fTrueZ;}
  bool  IsMC() {return fMC;}

  ClassDef(Hit,2);
};

#endif