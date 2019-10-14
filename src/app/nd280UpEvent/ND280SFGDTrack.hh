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
    int      fPDG;          // 
    int      fTrackID;      // 
    int      fParentID;     // 
    double   fRange;        // 
    double   fCosTheta;     // Initial CosTheta
    double   fMom;          // Initial Momentum
public:

    //constructors
    ND280SFGDTrack(){
    };

    ND280SFGDTrack(std::vector <ND280SFGDVoxel*> listOfVoxels){
        this->SetVoxels(listOfVoxels);        
    };

    virtual ~ND280SFGDTrack(){
    };

    //-----Setters------

    void SetPDG         (int     p_PDG)     { fPDG      = p_PDG; }
    void SetTrackID     (int     p_ID )     { fTrackID  = p_ID;  }
    void SetParentID    (int     p_prt)     { fParentID = p_prt; }
    void SetCosTheta    (double  p_cos)     { fCosTheta = p_cos; }
    void SetRange       (double  p_rng)     { fRange    = p_rng; }
    void SetMomentum    (double  p_mom)     { fMom      = p_mom; }

    //------------------

    //-----Getters------

    int     GetPDG()      { return fPDG;      }
    int     GetTrackID()  { return fTrackID;  }
    int     GetParentID() { return fParentID; }
    double  GetCosTheta() { return fCosTheta; }
    double  GetRange()    { return fRange;    }
    double  GetMomentum() { return fMom;      }


    //------------------

    ClassDef(ND280SFGDTrack,1);
};

#endif
