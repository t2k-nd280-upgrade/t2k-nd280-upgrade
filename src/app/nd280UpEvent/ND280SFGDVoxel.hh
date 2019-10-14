#ifndef _SFGD_VOXEL_H_
#define _SFGD_VOXEL_H_

#include "ND280SFGDHit.hh"
#include <TObject.h>
#include <TMath.h>
#include <Math/Vector3D.h>

using namespace ROOT::Math;

class ND280SFGDVoxel: public TObject{

private:

    Int_t                    fID;               // ID of the voxel

    std::vector <Int_t>      fTruePDGs;          // PDG from GEANT4
    std::vector <Int_t>      fTrueTrackIDs;      // Track ID from GEANT4
    std::vector <Int_t>      fTrueParentIDs;     // Parent ID from GEANT4
    std::vector <Int_t>      fRecoPDGs;          // PDG from GEANT4
    std::vector <Int_t>      fRecoTrackIDs;      // Track ID from GEANT4
    std::vector <Int_t>      fRecoParentIDs;     // Parent ID from GEANT4
    Int_t     fTrueType;     // 0: track || 1: crosstalk || 2: ghost
    Int_t     fRecoType;         
    Double_t  fTruePE;       // true PE before attenuation and efficiency implementation  
    Double_t  fRecoPE;
    Double_t  fPosX;
    Double_t  fPosY;
    Double_t  fPosZ;
    Double_t  fTime;         // yet not clear how to fill this variable.
    Double_t  fTrueEdep;     // yet not clear how to fill this variable.
    Double_t  fRecoEdep;     // yet not clear how to fill this variable.

    std::vector <Int_t> fHitIDs;       //MPPCHits IDs
    std::vector <ND280SFGDHit*> fHits; //MPPCHits pointers

public:
    //constructors
    ND280SFGDVoxel(){
        this->Init();
    };

    ND280SFGDVoxel(Double_t p_X,Double_t p_Y,Double_t p_Z){
        this->Init();
        SetXYZ(p_X,p_Y,p_Z);
    };

    virtual ~ND280SFGDVoxel(){
    };

    //-----Setters------

    void SetID            (Int_t p_ID)                          { fID = p_ID; }
    void SetHitIDs        (std::vector <Int_t> p_HitIDs)        { fHitIDs = p_HitIDs; }
    void SetHitID         (Int_t p_HitID, Int_t p_view)         { if((int) fHitIDs.size() <= p_view) {printf("SetHit Error! Accessing empty value, use AddHit!\n"); exit(1);} else fHitIDs[p_view] = p_HitID;}
    void AddHitID         (Int_t p_HitID)                       { fHitIDs.push_back(p_HitID); }
    void SetHits          (std::vector <ND280SFGDHit*> p_Hits)  { fHits = p_Hits; }
    void SetHit           (ND280SFGDHit* p_Hit, Int_t p_view)   { if((int) fHits.size() <= p_view) {printf("SetHit Error! Accessing empty value, use AddHit!\n"); exit(1);} else fHits[p_view] = p_Hit;}
    void AddHit           (ND280SFGDHit* p_Hit)                 { fHits.push_back(p_Hit); }
    void AddTruePDG       (Int_t     p_true_PDG)                { fTruePDGs.push_back(p_true_PDG);           }
    void AddTrueTrackID   (Int_t     p_true_TrackID)            { fTrueTrackIDs.push_back(p_true_TrackID);   }
    void AddTrueParentID  (Int_t     p_true_ParentID)           { fTrueParentIDs.push_back(p_true_ParentID); }
    void AddRecoPDG       (Int_t     p_reco_PDG)                { fRecoPDGs.push_back(p_reco_PDG);           }
    void AddRecoTrackID   (Int_t     p_reco_TrackID)            { fRecoTrackIDs.push_back(p_reco_TrackID);   }
    void AddRecoParentID  (Int_t     p_reco_ParentID)           { fRecoParentIDs.push_back(p_reco_ParentID); }
    void SetTrueEdep      (Double_t  p_true_Edep)               { fTrueEdep =  p_true_Edep; }
    void SetRecoEdep      (Double_t  p_reco_Edep)               { fRecoEdep =  p_reco_Edep; }
    void SetTruePE        (Double_t  p_true_PE)                 { fTruePE   =  p_true_PE;   }
    void SetRecoPE        (Double_t  p_reco_PE)                 { fRecoPE   =  p_reco_PE;   }
    void SetTrueType      (Double_t  p_true_Type)               { fTrueType =  p_true_Type; }
    void SetRecoType      (Double_t  p_reco_Type)               { fRecoType =  p_reco_Type; }
    void SetX             (Double_t  p_PosX)                    { fPosX  = p_PosX; }
    void SetY             (Double_t  p_PosY)                    { fPosY  = p_PosY; }
    void SetZ             (Double_t  p_PosZ)                    { fPosZ  = p_PosZ; }
    void SetTime          (Double_t  p_Time)                    { fTime  = p_Time; }
    void SetXYZ           (Double_t  p_PosX, Double_t p_PosY, Double_t p_PosZ)  {this->SetX(p_PosX); this->SetY(p_PosY); this->SetZ(p_PosZ);}
    //------------------

    //-----Getters------

    Int_t     GetID()                       { return fID;            }
    Double_t  GetX()                        { return fPosX;          }
    Double_t  GetY()                        { return fPosY;          }
    Double_t  GetZ()                        { return fPosZ;          }
    Double_t  GetTime()                     { return fTime;          }
    std::vector<Int_t> GetTruePDGs()        { return fTruePDGs;      } 
    std::vector<Int_t> GetTrueTrackIDs()    { return fTrueTrackIDs;  } 
    std::vector<Int_t> GetTrueParentIDs()   { return fTrueParentIDs; } 
    std::vector<Int_t> GetRecoPDGs()        { return fRecoPDGs;      } 
    std::vector<Int_t> GetRecoTrackIDs()    { return fRecoTrackIDs;  } 
    std::vector<Int_t> GetRecoParentIDs()   { return fRecoParentIDs; } 
    Double_t GetTrueType()                  { return fTrueType;      } 
    Double_t GetRecoType()                  { return fRecoType;      } 
    Double_t GetTruePE()                    { return fTruePE;        } 
    Double_t GetRecoPE()                    { return fRecoPE;        } 
    Double_t GetTrueEdep()                  { return fTrueEdep;      } 
    Double_t GetRecoEdep()                  { return fRecoEdep;      }
    std::vector <ND280SFGDHit*>   GetHits() { return fHits;          }
    std::vector <Int_t> GetHitIDs()         { return fHitIDs;        }
    ND280SFGDHit*   GetHit(Int_t p_index)   { if((int) fHits.size() <= p_index) {printf("GetHit Error! Accessing empty value!\n"); exit(1);}   return fHits[p_index]; } 
    Int_t   GetHitID(Int_t p_index)           { if((int) fHitIDs.size() <= p_index) {printf("GetHit Error! Accessing empty value!\n"); exit(1);}   return fHitIDs[p_index]; } 
    //------------------

    bool operator==(ND280SFGDVoxel* rhs)
    { 
        bool equal = this->GetX() == rhs->GetX() && this->GetY() == rhs->GetY() && this->GetZ() == rhs->GetZ();
        return equal;
    }

    void Init(Option_t* /*option*/="")
    {
        this->SetXYZ(-999,-999,-999);
        fTruePDGs.clear();
        fTrueTrackIDs.clear();
        fTrueParentIDs.clear();
        fRecoPDGs.clear();
        fRecoTrackIDs.clear();
        fRecoParentIDs.clear();
        fHitIDs.clear();
        fHits.clear();
        fID       = -999;
        fTrueType = -999;
        fRecoType = -999;
        fTruePE   = -999;
        fRecoPE   = -999;
        fTrueEdep = -999;
        fRecoEdep = -999;
        fTime     = -999;
    }

    ClassDef(ND280SFGDVoxel,1);
};

#endif
