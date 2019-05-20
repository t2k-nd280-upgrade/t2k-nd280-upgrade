#ifndef _SFGD_VOXELSET_H_
#define _SFGD_VOXELSET_H_ 1

#include "ND280SFGDHit.hh"
#include "ND280SFGDVoxel.hh"
#include <TObject.h>
#include <TMath.h>
#include <Math/Vector3D.h>
#include <TNtuple.h>
#include <TStyle.h>
#include <TString.h>

using namespace ROOT::Math;

class ND280SFGDVoxelSet : public TObject
{

protected:
    std::vector<ND280SFGDVoxel *> fVoxels; // All the voxels in this voxel set.
    std::vector<ND280SFGDHit *> fHits;     // All the 2D hits associated to this voxel set.
    TNtuple *fData;

public:
    ND280SFGDVoxelSet()
    {
        this->Init();
    };

    virtual ~ND280SFGDVoxelSet()
    {
        this->Reset();
    };

    //-----Setters------

    void SetVoxels(std::vector<ND280SFGDVoxel *> p_fVoxels) { fVoxels = p_fVoxels; }
    void SetHits(std::vector<ND280SFGDHit *> p_fHits) { fHits = p_fHits; }

    //------------------

    //-----Getters------

    TNtuple *GetData() { return fData; }
    std::vector<ND280SFGDVoxel *> GetVoxels() { return fVoxels; }
    ND280SFGDVoxel *GetVoxel(Int_t p_N) { return fVoxels[p_N]; }
    std::vector<ND280SFGDHit *> GetHits() { return fHits; }

    std::vector<Double_t> GetX()
    {
        std::vector<Double_t> auxVec;
        auxVec.reserve(fVoxels.size());
        for (UInt_t i = 0; i < fVoxels.size(); i++)
        {
            auxVec.push_back(fVoxels[i]->GetX());
        }
        return auxVec;
    }
    std::vector<Double_t> GetY()
    {
        std::vector<Double_t> auxVec;
        auxVec.reserve(fVoxels.size());
        for (UInt_t i = 0; i < fVoxels.size(); i++)
        {
            auxVec.push_back(fVoxels[i]->GetY());
        }
        return auxVec;
    }
    std::vector<Double_t> GetZ()
    {
        std::vector<Double_t> auxVec;
        auxVec.reserve(fVoxels.size());
        for (UInt_t i = 0; i < fVoxels.size(); i++)
        {
            auxVec.push_back(fVoxels[i]->GetZ());
        }
        return auxVec;
    }

    //------------------

    void Init()
    {
        gStyle->SetCanvasColor(0);
        gStyle->SetMarkerStyle(21);
        gStyle->SetMarkerSize(1.05);
        fData = new TNtuple("fData", "fData", "x:y:z:color");
    }

    void DrawHits(Bool_t p_Wait, Bool_t p_All, TString canvName); // p_Wait = kTRUE to update canvas and Wait for click, p_All used to define DrawEvent().
    void DrawVoxels(Bool_t p_Wait, Bool_t p_All);                 // p_Wait = kTRUE to update canvas and Wait for click
    void DrawHitsAndVoxels();                                     // Draws both hits and voxels for the event.

    void Reset(Option_t * /*option*/ = "")
    {
    }

    ClassDef(ND280SFGDVoxelSet, 1);
};

#endif
