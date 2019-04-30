#ifndef _SFGD_VOXELSET_H_
#define _SFGD_VOXELSET_H_ 1

#include "ND280SFGDHit.hh"
#include "ND280SFGDVoxel.hh"
#include <TObject.h>
#include <TMath.h>
#include <Math/Vector3D.h>
#include <TNtuple.h>
#include <TStyle.h>

using namespace ROOT::Math;

class ND280SFGDVoxelSet: public TObject{

protected:
    std::vector  <ND280SFGDVoxel*>          fVoxels;    // All the voxels in this voxel set.
    std::vector  <ND280SFGDHit*>            fHits;      // All the 2D hits associated to this voxel set. 
    TNtuple *fData;

public:

    ND280SFGDVoxelSet(){
        gStyle->SetCanvasColor(0);
        gStyle->SetMarkerStyle(21);
        gStyle->SetMarkerSize(1.05);
        fData = new TNtuple("fData", "fData", "x:y:z:color");
    };

    virtual ~ND280SFGDVoxelSet(){
        this->Reset();
    };

    //-----Setters------

    void SetVoxels (std::vector <ND280SFGDVoxel*> p_fVoxels) { fVoxels = p_fVoxels; }
    void SetHits   (std::vector <ND280SFGDHit*> p_fHits)     { fHits = p_fHits;    }

    //------------------

    //-----Getters------

    std::vector  <ND280SFGDVoxel*>  GetVoxels()         { return fVoxels;      } 
    ND280SFGDVoxel*            GetVoxel(Int_t p_N)      { return fVoxels[p_N]; }
    std::vector  <ND280SFGDHit*>    GetHits()           { return fHits;        }

    std::vector <Double_t> GetX(){
        std::vector <Double_t> auxVec;
        auxVec.reserve(fVoxels.size());
        for(UInt_t i=0; i<fVoxels.size(); i++){
            auxVec.push_back(fVoxels[i]->GetX());
        }
        return auxVec;
    }
    std::vector <Double_t> GetY(){
        std::vector <Double_t> auxVec;
        auxVec.reserve(fVoxels.size());
        for(UInt_t i=0; i<fVoxels.size(); i++){
            auxVec.push_back(fVoxels[i]->GetY());
        }
        return auxVec;
    }
    std::vector <Double_t> GetZ(){
        std::vector <Double_t> auxVec;
        auxVec.reserve(fVoxels.size());
        for(UInt_t i=0; i<fVoxels.size(); i++){
            auxVec.push_back(fVoxels[i]->GetZ());
        }
        return auxVec;
    }

    //------------------

    void DrawVoxels(std::string name, Bool_t all);

    void Reset(Option_t* /*option*/="")
    {
        for(UInt_t i=0; i<fHits.size(); i++)   delete fHits[i];
        for(UInt_t i=0; i<fVoxels.size(); i++) delete fVoxels[i];
        fHits.clear();
        fVoxels.clear();
    } 

    ClassDef(ND280SFGDVoxelSet,1);
};

#endif
