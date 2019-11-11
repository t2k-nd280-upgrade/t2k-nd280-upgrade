#ifndef _SFGD_EVENT_H_
#define _SFGD_EVENT_H_

#include "ND280SFGDVoxelSet.hh"
#include "ND280SFGDVoxel.hh"
#include "ND280SFGDTrack.hh"
#include "ND280SFGDHit.hh"

class ND280SFGDEvent: public ND280SFGDVoxelSet{

private:
    std::vector  <ND280SFGDTrack*>  fTrueTracks;
    std::vector  <ND280SFGDTrack*>  fRecoTracks;
    std::vector  <ND280SFGDVoxelSet*> fTrueVertices;
    std::vector  <ND280SFGDVoxelSet*> fRecoVertices;

public:

    ND280SFGDEvent(){
    };

    ND280SFGDEvent(std::vector <ND280SFGDVoxel*> listOfVoxels){
        this->SetVoxels(listOfVoxels);
    };

    ND280SFGDEvent(std::vector <ND280SFGDTrack*> listOfTracks){
        this->SetTrueTracks(listOfTracks);
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

    void SetTrueTracks   (std::vector <ND280SFGDTrack*>     p_true_Tracks  ) { fTrueTracks = p_true_Tracks;              }
    void AddTrueTrack    (ND280SFGDTrack*                   p_true_Track   ) { fTrueTracks.push_back(p_true_Track);      }
    void SetTrueVertices (std::vector  <ND280SFGDVoxelSet*> p_true_Vertices) { fTrueVertices = p_true_Vertices;          }
    void AddTrueVertex   (ND280SFGDVoxelSet*                p_true_Vertex  ) { fTrueVertices.push_back(p_true_Vertex);   }

    void SetRecoTracks   (std::vector <ND280SFGDTrack*>     p_reco_Tracks  ) { fRecoTracks = p_reco_Tracks;              }
    void AddRecoTrack    (ND280SFGDTrack*                   p_reco_Track   ) { fRecoTracks.push_back(p_reco_Track);      }
    void SetRecoVertices (std::vector  <ND280SFGDVoxelSet*> p_reco_Vertices) { fRecoVertices = p_reco_Vertices;          }
    void AddRecoVertex   (ND280SFGDVoxelSet*                p_reco_Vertex  ) { fRecoVertices.push_back(p_reco_Vertex);   }

    //------------------

    //-----Getters------

    std::vector <ND280SFGDTrack*>     GetTrueTracks()     { return fTrueTracks;   }
    std::vector <ND280SFGDVoxelSet*>  GetTrueVertices()   { return fTrueVertices; }
    std::vector <ND280SFGDTrack*>     GetRecoTracks()     { return fRecoTracks;   }
    std::vector <ND280SFGDVoxelSet*>  GetRecoVertices()   { return fRecoVertices; }

    bool HasSingleTrueVertex()      { return fTrueVertices.size() == 1 && fTrueVertices[0]->GetVoxels().size() == 1;}
    ND280SFGDVoxel* GetTrueVertex() { return fTrueVertices[0]->GetVoxels()[0];}  

    bool HasSingleRecoVertex()      { return fRecoVertices.size() == 1 && fRecoVertices[0]->GetVoxels().size() == 1;}
    ND280SFGDVoxel* GetRecoVertex() { return fRecoVertices[0]->GetVoxels()[0];}  

    //------------------

    ND280SFGDHit* AddHit(){
        ND280SFGDHit *hit = new ND280SFGDHit();
        return hit;
    }

    void ResetEvent(Option_t* /*option*/="")
    {
        for ( UInt_t i=0; i<fHits.size(); i++)        delete fHits[i];
        for ( UInt_t i=0; i<fVoxels.size(); i++)      delete fVoxels[i];
        for ( UInt_t i=0; i<fTrueTracks.size(); i++ ) delete fTrueTracks[i];
        for ( UInt_t i=0; i<fRecoTracks.size(); i++ ) delete fRecoTracks[i];
        fHits.clear();
        fVoxels.clear();
        fTrueTracks.clear();
        fRecoTracks.clear();
    } 

    ClassDef(ND280SFGDEvent,1);
};

#endif
