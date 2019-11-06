#ifndef _SFGD_TRACK_H_
#define _SFGD_TRACK_H_

#include "ND280SFGDVoxelSet.hh"
#include "ND280SFGDVoxel.hh"
#include "ND280SFGDTrack.hh"
#include "ND280SFGDHit.hh"
#include <TObject.h>
#include <Math/Vector3D.h>

class ND280SFGDTrack: public ND280SFGDVoxelSet{

private:
    int       fPDG;          // 
    int       fTrackID;      // 
    int       fParentID;     // 
    double    fRange;        // 
    double    fCosTheta;     // Initial CosTheta
    double    fCosPhi;       // Initial CosPhi
    double    fMom;          // Initial Momentum  
    std::vector <double>  fFitPar; // best fit parameters.
    bool      fIsReco;       // true if reconstructed.
    ND280SFGDTrack* fTrueTrack;
    ND280SFGDTrack* fSegment;
    double    fPrecision;    // voxels with correct trkID / (total of voxels in the reco track)
    double    fRecall;       // voxels with correct trkID / (total of voxels in the true track)

public:

    // it is public to be able to work with XYZVector operations easily.
    XYZVector fMomVec;       // MomVector      

    //constructors
    ND280SFGDTrack(){
        this->Init();
    };

    ND280SFGDTrack(std::vector <ND280SFGDVoxel*> listOfVoxels){
        this->Init();
        this->SetVoxels(listOfVoxels);        
    };

    virtual ~ND280SFGDTrack(){
    };

    //-----Setters------

    void SetPDG         (int       p_PDG)                   { fPDG       = p_PDG; }
    void SetTrackID     (int       p_ID )                   { fTrackID   = p_ID;  }
    void SetParentID    (int       p_prt)                   { fParentID  = p_prt; }
    void SetCosTheta    (double    p_cst)                   { fCosTheta  = p_cst; }
    void SetCosPhi      (double    p_csp)                   { fCosPhi    = p_csp; }
    void SetRange       (double    p_rng)                   { fRange     = p_rng; }
    void SetMomentum    (double    p_mom)                   { fMom       = p_mom; }
    void SetFitParams   (std::vector <double>  p_fit)       { fFitPar    = p_fit; }
    void SetTrueTrack   (ND280SFGDTrack* p_true)            { fTrueTrack = p_true;}
    void SetNextSegment (ND280SFGDTrack* p_seg)             { fSegment   = p_seg; }
    void SetPrecision   (double    p_pre)                   { fPrecision = p_pre; }
    void SetRecall      (double    p_rec)                   { fRecall    = p_rec; }
    void SetIsReco      (bool      p_IsR)                   { fIsReco    = p_IsR; }

    //------------------

    //-----Getters------

    int     GetPDG()                     { return fPDG;       }
    int     GetTrackID()                 { return fTrackID;   }
    int     GetParentID()                { return fParentID;  }
    double  GetCosTheta()                { return fCosTheta;  }
    double  GetRange()                   { return fRange;     }
    double  GetMomentum()                { return fMom;       }
    std::vector <double>  GetFitParams() { return fFitPar;    }
    ND280SFGDTrack* GetNextSegment()     { return fSegment;   }
    ND280SFGDTrack* GetTrueTrack()       { return fTrueTrack; }
    double GetRecall()                   { return fRecall;    }
    double GetPrecision()                { return fPrecision; }
    bool   IsReco()                      { return fIsReco;    }
    XYZVector GetMomVec()                { return fMomVec;    }
    
    double GetF1Score(){
        double f1score = 0; 
        if (fRecall && fPrecision) f1score = 2.*fRecall*fPrecision/(fRecall+fPrecision);
        return f1score;
    }

    void Init(Option_t* /*option*/="")
    {
        fPDG       = -999;          
        fTrackID   = -999;      
        fParentID  = -999;     
        fRange     = -999;        
        fCosTheta  = -999;  
        fCosPhi    = -999;   
        fMom       = -999;          
        fFitPar.clear();
        fIsReco    = false;
        fTrueTrack = nullptr;
        fSegment   = nullptr;
        fPrecision = 0;  
        fRecall    = 0;
        fMomVec.SetXYZ(0,0,0);
    }

    //------------------

    ClassDef(ND280SFGDTrack,1);
};

#endif
