#ifndef _SFGD_EVENT_H_
#define _SFGD_EVENT_H_

#include "ND280SFGDVoxelSet.hh"
#include "ND280SFGDVoxel.hh"
#include "ND280SFGDTrack.hh"
#include "ND280SFGDHit.hh"

class ND280SFGDEvent: public ND280SFGDVoxelSet{

private:
    std::vector  <ND280SFGDTrack*>  fTracks;

public:

    ND280SFGDEvent(){
    };

    ND280SFGDEvent(std::vector <ND280SFGDVoxel*> listOfVoxels){
        this->SetVoxels(listOfVoxels);
    };

    ND280SFGDEvent(std::vector <ND280SFGDTrack*> listOfTracks){
        this->SetTracks(listOfTracks);
        std::vector <ND280SFGDVoxel*> listOfVoxels;
        Int_t numOfVoxels = 0;
        for(UInt_t i=0; i<listOfTracks.size(); i++){
            numOfVoxels += listOfTracks[i]->GetVoxels().size();
        }
        listOfVoxels.reserve(numOfVoxels);
        for(UInt_t i=0; i<listOfTracks.size(); i++){
            for(UInt_t j=0; j<listOfTracks[i]->GetVoxels().size(); j++){
                listOfVoxels.push_back(listOfTracks[i]->GetVoxel(j));
            } 
        }
        this->SetVoxels(listOfVoxels);
    };

    virtual ~ND280SFGDEvent(){
        this->ResetEvent();
    };

    //-----Setters------

    void SetTracks(std::vector <ND280SFGDTrack*> p_fTracks) { fTracks = p_fTracks; }

    //------------------

    //-----Getters------

    std::vector <ND280SFGDTrack*>  GetTracks()     { return fTracks; }

    //------------------

    ND280SFGDHit* AddHit(){
        ND280SFGDHit *hit = new ND280SFGDHit();
        return hit;
    }

    void DrawTracks();

    void ResetEvent(Option_t* /*option*/="")
    {
        for ( UInt_t i=0; i<fHits.size(); i++)   delete fHits[i];
        for ( UInt_t i=0; i<fVoxels.size(); i++) delete fVoxels[i];
        for ( UInt_t i=0; i<fTracks.size(); i++ ) delete fTracks[i];
        fHits.clear();
        fVoxels.clear();
        fTracks.clear();
    } 

    ClassDef(ND280SFGDEvent,1);
};

#endif
