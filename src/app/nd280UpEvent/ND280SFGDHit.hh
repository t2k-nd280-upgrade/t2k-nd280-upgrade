#ifndef _SFGD_HIT_H_
#define _SFGD_HIT_H_

#include <TObject.h>
#include <TMath.h>
#include <Math/Vector3D.h>

using namespace ROOT::Math;

class ND280SFGDHit : public TObject {
private:

  // Information filled by the unpacking on real data (Aug/Sept 2018)

  Int_t         fView;            //(0=XY, 1=XZ, 2=ZY)
  XYZVector     fPos;             // X,Y,Z.  (One of them is -1, depending on the view)
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

  //------------------

  //Extra information from MC or to be filled during reconstruction

  XYZVector     fTruePos;         // X,Y,Z. 
  Bool_t        fxTalkFlag;       //Cross talk (false=not CT, true=CT)
  Int_t         fMulti;           //Number of times the hit is used to reconstruct 3D voxels
  Int_t         fPDG;             //true PDG of track creating the hit
  Int_t         fTrackId;         //true track ID from MC // 1 == primary
  Int_t         fParentId;        //true track parent ID from MC
  Double_t      fEdep;            //true Energy Deposit from MC
  Bool_t        fTrueXTalk;       //True if hit is associated to a crosstalk voxel in MC.

  //------------------

public:

  ND280SFGDHit(){ 
    fMulti = 0; 
  };
  virtual ~ND280SFGDHit() {
  };

  void Clear(Option_t* /*option*/="")
  {
    TObject::Clear();
  }

  //-----Setters------

  void SetX               (Double_t  p_N)              { fPos.SetX(p_N);     }
  void SetY               (Double_t  p_N)              { fPos.SetY(p_N);     }
  void SetZ               (Double_t  p_N)              { fPos.SetZ(p_N);     }
  void SetTrueX           (Double_t  p_N)              { fTruePos.SetX(p_N); }
  void SetTrueY           (Double_t  p_N)              { fTruePos.SetY(p_N); }
  void SetTrueZ           (Double_t  p_N)              { fTruePos.SetZ(p_N); }

  void SetPos             (XYZVector p_Pos)            { fPos            = p_Pos;            }
  void SetView            (Int_t     p_View)           { fView           = p_View;           }
  void SetCharge          (Double_t  p_Charge)         { fCharge         = p_Charge;         }
  void SetHG_pe           (Double_t  p_HG)             { fHG_pe          = p_HG;             }
  void SetLG_pe           (Double_t  p_LG)             { fLG_pe          = p_LG;             }
  void SetToT_pe          (Double_t  p_ToT)            { fToT_pe         = p_ToT;            }
  void SetHitAmpl         (Double_t  p_HitAmpl)        { fHitAmpl        = p_HitAmpl;        }
  void SetHitLGAmpl       (Double_t  p_HitLGAmpl)      { fHitLGAmpl      = p_HitLGAmpl;      }
  void SetRE              (Double_t  p_RE)             { fRE             = p_RE;             }
  void SetFE              (Double_t  p_FE)             { fFE             = p_FE;             }
  void SetToT             (Double_t  p_ToT)            { fToT            = p_ToT;            }
  void SetDt              (Double_t  p_Dt)             { fDt             = p_Dt;             }
  void SetSpillTag        (Double_t  p_SpillTag)       { fSpillTag       = p_SpillTag;       }
  void SetFEB             (Double_t  p_FEB)            { fFEB            = p_FEB;            }
  void SetCh              (Double_t  p_Ch)             { fCh             = p_Ch;             }
  void SetTfromSpill      (Double_t  p_TfromSpill)     { fTfromSpill     = p_TfromSpill;     }
  void SetGTrigTag        (Double_t  p_GTrigTag)       { fGTrigTag       = p_GTrigTag;       }
  void SetGTrigTime       (Double_t  p_GTrigTime)      { fGTrigTime      = p_GTrigTime;      }
  void SetSpillTime       (Double_t  p_SpillTime)      { fSpillTime      = p_SpillTime;      }
  void SetSpillTrailTime  (Double_t  p_SpillTrailTime) { fSpillTrailTime = p_SpillTrailTime; }
  
  void SetTruePos         (XYZVector p_Pos)            { fTruePos        = p_Pos;            }
  void SetxTalkFlag       (Int_t p_CT)                 { fxTalkFlag      = p_CT;             }
  void SetMultiplicity    (Int_t p_fMulti)             { fMulti          = p_fMulti;         }
  void SetPDG             (Int_t p_PDG)                { fPDG            = p_PDG;            }
  void SetTrackID         (Int_t p_ID)                 { fTrackId        = p_ID;             }
  void SetParentID        (Int_t p_ID)                 { fParentId       = p_ID;             }
  void SetEdep            (Double_t p_E)               { fEdep           = p_E;              }
  void SetTrueXTalk       (Bool_t p_B)                 { fTrueXTalk      = p_B;              }

  //------------------

  //-----Getters------

  XYZVector GetPos()            { return fPos;            }
  Int_t     GetView()           { return fView;           }
  Double_t  GetX()              { return fPos.X();        }
  Double_t  GetY()              { return fPos.Y();        }
  Double_t  GetZ()              { return fPos.Z();        }
  Double_t  GetCharge()         { return fCharge;         }
  Double_t  GetHG_pe()          { return fHG_pe;          }
  Double_t  GetLG_pe()          { return fLG_pe;          }
  Double_t  GetToT_pe()         { return fToT_pe;         }
  Double_t  GetHitAmpl()        { return fHitAmpl;        }
  Double_t  GetHitLGAmpl()      { return fHitLGAmpl;      }
  Double_t  GetRE()             { return fRE;             }
  Double_t  GetFE()             { return fFE;             }
  Double_t  GetToT()            { return fToT;            }
  Double_t  GetDt()             { return fDt;             }
  Double_t  GetSpillTag()       { return fSpillTag;       }
  Double_t  GetFEB()            { return fFEB;            }
  Double_t  GetCh()             { return fCh;             }
  Double_t  GetTfromSpill()     { return fTfromSpill;     }
  Double_t  GetGTrigTag()       { return fGTrigTag;       }
  Double_t  GetGTrigTime()      { return fGTrigTime;      }
  Double_t  GetSpillTime()      { return fSpillTime;      }
  Double_t  GetSpillTrailTime() { return fSpillTrailTime; }

  XYZVector GetTruePos()        { return fTruePos;        }
  Bool_t    GetxTalkFlag()      { return fxTalkFlag;      }
  Int_t     GetMultiplicity()   { return fMulti;          }
  Double_t  GetTrueX()          { return fTruePos.X();}
  Double_t  GetTrueY()          { return fTruePos.Y();}
  Double_t  GetTrueZ()          { return fTruePos.Z();}
  Int_t     GetPDG()            { return fPDG;}
  Int_t     GetTrackID()        { return fTrackId;}
  Int_t     GetParentID()       { return fParentId;}
  Double_t  GetEdep()           { return fEdep;}
  Bool_t    GetTrueXTalk()      { return fTrueXTalk;}

  //------------------

  ClassDef(ND280SFGDHit,1);
};

#endif
