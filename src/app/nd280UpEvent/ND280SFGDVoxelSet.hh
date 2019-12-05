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
#include <cstring>

// apparently, including this libraries in global_header is not enough,
// so they needd to explicitely be add here to build succesfully the method DumpToCSVfile.
#include <fstream>
#include <iostream>

class ND280SFGDVoxelSet : public TObject
{

protected:

    std::vector <ND280SFGDVoxel*> fVoxels; // All the voxels in this voxel set.
    std::vector <ND280SFGDHit*> fHits;     // All the 2D hits associated to this voxel set.
    TNtuple *fData;

    void DrawSet(bool wait, TString canvName, int color, int opt);
    void Draw3D (bool wait, int color);

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

    void SetVoxels(std::vector<ND280SFGDVoxel*> p_fVoxels) { fVoxels = p_fVoxels; }
    void SetHits(std::vector<ND280SFGDHit*> p_fHits) { fHits = p_fHits; }
    void AddVoxel(ND280SFGDVoxel* p_vxl) {fVoxels.push_back(p_vxl);}

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

    void DrawHits            (bool wait=true, TString canvName = "cc", int color=0);
    void DrawVoxels          (bool wait=true, TString canvName = "cc", int color=0);
    void DrawHitsAndVoxels   (bool wait=true, TString canvName = "cc", int color=0);
    void DrawVoxelsTruePE    (bool wait=true, TString canvName = "cc");
    void DrawVoxelsRecoPE    (bool wait=true, TString canvName = "cc");            
    void DrawClusters        (bool wait=true, TString canvName = "cc");           
    void DrawTrueTracks      (bool wait=true, TString canvName = "cc");            
    void DrawRecoTracks      (bool wait=true, TString canvName = "cc");
    void DrawGraphDistance   (bool wait=true, TString canvName = "cc");  
    void DrawTrueType        (bool wait=true, TString canvName = "cc");    
    void DrawTimeSeparation  (bool wait=true, TString canvName = "cc"); 
    void DrawBranchings      (bool wait=true, TString canvName = "cc"); 
    void DrawKinks           (bool wait=true, TString canvName = "cc");  
    void DrawBreakingPoints  (bool wait=true, TString canvName = "cc"); 

    double GetMaxEuclDist(int type=-1){
        double dist = 0;
        if (type == -1){
        for (int i=0; i<(int) fVoxels.size();++i)
            for (int j=i+1; j<(int) fVoxels.size();++j){
                if(fVoxels[i]->DistToVoxel(fVoxels[j])>dist) dist = fVoxels[i]->DistToVoxel(fVoxels[j]);}
        }
        else{
            for (int i=0; i<(int) fVoxels.size();++i)
                for (int j=i+1; j<(int) fVoxels.size();++j){
                    if (fVoxels[i]->GetTrueType() == type and fVoxels[j]->GetTrueType() == type) if(fVoxels[i]->DistToVoxel(fVoxels[j])>dist) dist = fVoxels[i]->DistToVoxel(fVoxels[j]);
            }       
        }
        return dist;
    }        

    void Reset(Option_t * /*option*/ = "")
    {
    }

    ClassDef(ND280SFGDVoxelSet, 1);
};

#endif
