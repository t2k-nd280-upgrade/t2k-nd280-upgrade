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
    Int_t     fPDG;          // PDG from GEANT4
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

    void SetPDG     (Int_t     p_PDG)     { fPDG = p_PDG; }

    //------------------

    //-----Getters------

    Int_t     GetPDG()      { return fPDG; }

    //------------------

    ClassDef(ND280SFGDTrack,1);
};

#endif
