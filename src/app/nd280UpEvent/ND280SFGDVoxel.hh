#ifndef _SFGD_VOXEL_H_
#define _SFGD_VOXEL_H_

#include "ND280SFGDHit.hh"
#include <TObject.h>
#include <TMath.h>
#include <Math/Vector3D.h>

using namespace ROOT::Math;

class ND280SFGDVoxel: public TObject{

// One voxel is 1 cube. --> Its XYZ coordinates are well defined.
// One voxel has a unique track PDG. If there is more than 1 particle in the same XYZ coordinates more than 1 voxel is stored.

// All the GEANT4 hits that contribute to the Edep are stored in a vector. To distinguish them to 2D MPPC hits they are called G4Hits.
// The reconstructed values consider the attenuation and conversion to photoelectrons over G4Hits with same TRUE pdg.

private:
    Int_t     fPDG;          // PDG from GEANT4
    Int_t     fTrackID;      // Track ID from GEANT4
    Int_t     fParentID;     // Parent ID from GEANT4
    Double_t  fEdep;         // The energy deposit (Edep) corresponds only to the energy deposition by the track with the PDG associated to the voxel.   
    //XYZVector fPos;          // Position of the Cube XYZ
    Double_t  fPosX;
    Double_t  fPosY;
    Double_t  fPosZ;
    Bool_t    fTrueXTalk;     // xTalk from MC
    std::vector    <ND280SFGDHit*> fHits; //MPPCHits
    std::vector    <Double_t> fLocalHitsQ; //Hits charge before merging
    std::vector    <Int_t>  fContributorsID;

public:
    //constructors
    ND280SFGDVoxel(){
        this->Reset();
    };

    ND280SFGDVoxel(Double_t p_X,Double_t p_Y,Double_t p_Z){
        this->Reset();
        SetXYZ(p_X,p_Y,p_Z);
    };

    virtual ~ND280SFGDVoxel(){
    };

    //-----Setters------

    void SetHits       (std::vector <ND280SFGDHit*> p_Hits) { fHits = p_Hits; }
    void SetLocalHitsQ (std::vector <Double_t> p_VoxelHits) { fLocalHitsQ = p_VoxelHits; }


    void SetPDG       (Int_t     p_PDG)     { fPDG       = p_PDG;     }
    void SetTrackID   (Int_t     p_TrackID) { fTrackID   = p_TrackID; }
    void SetParentID  (Int_t     p_ParentID){ fParentID  = p_ParentID; }
    void SetEdep      (Double_t  p_Edep)    { fEdep      = p_Edep;    }
    // void SetPos       (XYZVector p_Pos)     { fPos       = p_Pos;     }
    void SetTrueXTalk (Bool_t p_Xtalk)      { fTrueXTalk = p_Xtalk;   }
    // void SetX         (Double_t  p_N)       { fPos.SetX(p_N); }
    // void SetY         (Double_t  p_N)       { fPos.SetY(p_N); }
    // void SetZ         (Double_t  p_N)       { fPos.SetZ(p_N); }
    void SetX         (Double_t  p_PosX)       { fPosX = p_PosX; }
    void SetY         (Double_t  p_PosY)       { fPosY = p_PosY; }
    void SetZ         (Double_t  p_PosZ)       { fPosZ = p_PosZ; }
    // void SetXYZ       (Double_t p_X, Double_t p_Y, Double_t p_Z) {fPos.SetX(p_X); fPos.SetY(p_Y); fPos.SetZ(p_Z);}
    void SetXYZ (Double_t p_X, Double_t p_Y, Double_t p_Z)  {this->SetX(p_X); this->SetY(p_Y); this->SetZ(p_Z);}
    void AddContributor(Int_t p_Cont) {fContributorsID.push_back(p_Cont);}
    //------------------

    //-----Getters------

    std::vector <ND280SFGDHit*> &GetHits()  { return fHits; }
    std::vector <Double_t> &GetLocalHitsQ() { return fLocalHitsQ; }

    Double_t  GetEdep()      { return fEdep;      } 
    Int_t     GetPDG()       { return fPDG;       }
    Int_t     GetTrackID()   { return fTrackID;   }
    Int_t     GetParentID()  { return fParentID;  }
    // XYZVector GetPos()       { return fPos;       }
    Bool_t    GetTrueXTalk() { return fTrueXTalk; }
    // Double_t  GetX()         { return fPos.X();   }
    // Double_t  GetY()         { return fPos.Y();   }
    // Double_t  GetZ()         { return fPos.Z();   }
    Double_t  GetX()         { return fPosX;   }
    Double_t  GetY()         { return fPosY;   }
    Double_t  GetZ()         { return fPosZ;   }
    std::vector<Int_t> GetContributors() {return fContributorsID;} 
    //------------------

    bool operator==(ND280SFGDVoxel* rhs)
    { 
        bool equal = this->GetX() == rhs->GetX() && this->GetY() == rhs->GetY() && this->GetZ() == rhs->GetZ();
        return equal;
    }

    void Reset(Option_t* /*option*/="")
    {
        fHits.clear();
        fHits.reserve(3);
        fLocalHitsQ.resize(3,0);
        fPDG     = -999;
        fTrackID = -999;
        fEdep    = -999;
        this->SetXYZ(-999,-999,-999);
    }

    ClassDef(ND280SFGDVoxel,1);
};

#endif
