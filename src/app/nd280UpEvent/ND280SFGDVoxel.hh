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
    Double_t  fTrueTime;     // yet not clear how to fill this variable.
    Double_t  fRecoTime;     // yet not clear how to fill this variable.
    Double_t  fTrueEdep;     // yet not clear how to fill this variable.
    Double_t  fRecoEdep;     // yet not clear how to fill this variable.
    Int_t     fClustID;      // ID of the cluster.


    Double_t  fTime;         // Erase it soon. --> move to True/Reco

    std::vector <Int_t> fHitIDs;       //MPPCHits IDs
    std::vector <ND280SFGDHit*> fHits; //MPPCHits pointers
    std::vector <ND280SFGDVoxel*> fNeighbors;

    double fDistance;

    bool fIsVisited;
    bool fIsEndPoint;

    bool fIsKink;
    bool fIsBranching;
    bool fIsVertex;

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
    void SetTrueType      (Int_t     p_true_Type)               { fTrueType =  p_true_Type; }
    void SetRecoType      (Double_t  p_reco_Type)               { fRecoType =  p_reco_Type; }
    void SetTrueTime      (Double_t  p_true_Time)               { fTrueTime =  p_true_Time;  }
    void SetRecoTime      (Double_t  p_reco_Time)               { fRecoTime =  p_reco_Time;  }
    void SetClusterID     (Double_t  p_cID)                     { fClustID  =  p_cID;        }
    void SetX             (Double_t  p_PosX)                    { fPosX  = p_PosX; }
    void SetY             (Double_t  p_PosY)                    { fPosY  = p_PosY; }
    void SetZ             (Double_t  p_PosZ)                    { fPosZ  = p_PosZ; }
    void SetTime          (Double_t  p_Time)                    { fTime  = p_Time; }
    void SetXYZ           (Double_t  p_PosX, Double_t p_PosY, Double_t p_PosZ)  {this->SetX(p_PosX); this->SetY(p_PosY); this->SetZ(p_PosZ);}
    void SetIsKink        (bool      p_kink)                    { fIsKink      = p_kink; }
    void SetIsBranching   (bool      p_bran)                    { fIsBranching = p_bran; }
    void SetIsVertex      (bool      p_vrtx)                    { fIsVertex    = p_vrtx; }
    

    void AddNeighbor      (ND280SFGDVoxel* p_neighbor) {fNeighbors.push_back(p_neighbor);}
    void SetNeighbors     (std::vector <ND280SFGDVoxel*> p_neighbors) {fNeighbors = p_neighbors;}
    void SetIsEndPoint    (bool p_fIsEndPoint) { fIsEndPoint = p_fIsEndPoint;}
    void SetIsVisited     (bool p_fIsVisited)  { fIsVisited  = p_fIsVisited;}
    void SetDistance      (double  p_distance) { fDistance = p_distance; }
    //------------------

    //-----Getters------

    Int_t     GetID()                       { return fID;            }
    Double_t  GetX()                        { return fPosX;          }
    Double_t  GetY()                        { return fPosY;          }
    Double_t  GetZ()                        { return fPosZ;          }
    Double_t  GetTime()                     { return fTime;          }
    Double_t  GetTrueTime()                 { return fTrueTime;      }
    Double_t  GetRecoTime()                 { return fRecoTime;      }
    Int_t     GetClusterID()                { return fClustID;       }
    std::vector<Int_t> GetTruePDGs()        { return fTruePDGs;      } 
    std::vector<Int_t> GetTrueTrackIDs()    { return fTrueTrackIDs;  } 
    std::vector<Int_t> GetTrueParentIDs()   { return fTrueParentIDs; } 
    std::vector<Int_t> GetRecoPDGs()        { return fRecoPDGs;      } 
    std::vector<Int_t> GetRecoTrackIDs()    { return fRecoTrackIDs;  } 
    std::vector<Int_t> GetRecoParentIDs()   { return fRecoParentIDs; } 
    Int_t    GetTrueType()                  { return fTrueType;      } 
    Double_t GetRecoType()                  { return fRecoType;      } 
    Double_t GetTruePE()                    { return fTruePE;        } 
    Double_t GetRecoPE()                    { return fRecoPE;        } 
    Double_t GetTrueEdep()                  { return fTrueEdep;      } 
    Double_t GetRecoEdep()                  { return fRecoEdep;      }
    std::vector <ND280SFGDHit*>   GetHits() { return fHits;          }
    std::vector <Int_t> GetHitIDs()         { return fHitIDs;        }
    ND280SFGDHit*   GetHit(Int_t p_index)   { if((int) fHits.size()   <= p_index) {printf("GetHit Error! Accessing empty value!\n"); exit(1);}   return fHits[p_index]; } 
    Int_t   GetHitID(Int_t p_index)         { if((int) fHitIDs.size() <= p_index) {printf("GetHit Error! Accessing empty value!\n"); exit(1);}   return fHitIDs[p_index]; } 
    bool IsBranching() { return fIsBranching;  }
    bool IsKink()      { return fIsKink;       }
    bool IsVertex()    { return fIsVertex;     }

    std::vector <ND280SFGDVoxel*> GetNeighbors() {return fNeighbors;}
    bool IsEndPoint() {return fIsEndPoint;}
    bool IsVisited()  {return fIsVisited;}
    double GetDistance() {return fDistance;}
    //------------------

    void ClearRecoTrackIDs () {fRecoTrackIDs.clear();}

    double DistToMPPC(int view){
        double distance = 0;
        if      (view == 2) distance =  this->GetX()*10;       // X is distance to plane YZ 
        else if (view == 1) distance =  this->GetY()*10;       // Y is distance to plane XZ
        else if (view == 0) distance =  this->GetZ()*10;       // Z is distance to plane XY
        if (distance<0)     {
            std::cerr << "Distance can not be negative! case: " << view << " distance: " << distance << std::endl; 
            exit(1);
        }
        return distance;
    }

    double DistToVoxel(ND280SFGDVoxel* vxl){
        double distance = sqrt(pow(this->GetX()-vxl->GetX(),2) + pow(this->GetY()-vxl->GetY(),2) + pow(this->GetZ()-vxl->GetZ(),2));
        return distance;
    }

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
        fNeighbors.clear();
        fID          = -999;
        fTrueType    = -999;
        fRecoType    = -999;
        fTruePE      = -999;
        fRecoPE      = -999;
        fTrueEdep    = -999;
        fRecoEdep    = -999;
        fTrueTime    = -999;
        fRecoTime    = -999;
        fClustID     = -999;
        fTime        = -999;
        fIsEndPoint  = false;
        fIsVisited   = false;
        fDistance    = 0;
        fIsVertex    = false;
        fIsBranching = false;
        fIsKink      = false;
    }

    ClassDef(ND280SFGDVoxel,1);
};

#endif
