// #ifndef _SFGD_EVENT_H_
// #define _SFGD_EVENT_H_

// #include "ND280SFGDVoxelSet.hh"
// #include "ND280SFGDVoxel.hh"
// #include "ND280SFGDTrack.hh"
// #include "ND280SFGDHit.hh"

// class ND280SFGDEvent: public ND280SFGDVoxelSet{

// private:
//     std::vector  <ND280SFGDTrack*>  fTrueTracks;
//     std::vector  <ND280SFGDTrack*>  fRecoTracks;
//     ND280SFGDVoxel* fTrueVertex = nullptr;
//     ND280SFGDVoxel* fRecoVertex = nullptr;


// public:

//     ND280SFGDEvent(){
//     };

//     ND280SFGDEvent(std::vector <ND280SFGDVoxel*> listOfVoxels){
//         this->SetVoxels(listOfVoxels);
//     };

//     ND280SFGDEvent(std::vector <ND280SFGDTrack*> listOfTracks){
//         this->SetTrueTracks(listOfTracks);
//         std::vector <ND280SFGDVoxel*> listOfVoxels;
//         Int_t numOfVoxels = 0;
//         for(UInt_t i=0; i<listOfTracks.size(); i++){
//             numOfVoxels += listOfTracks[i]->GetVoxels().size();
//         }
//         listOfVoxels.reserve(numOfVoxels);
//         for(UInt_t i=0; i<listOfTracks.size(); i++){
//             for(UInt_t j=0; j<listOfTracks[i]->GetVoxels().size(); j++){
//                 listOfVoxels.push_back(listOfTracks[i]->GetVoxel(j));
//             } 
//         }
//         this->SetVoxels(listOfVoxels);
//     };

//     virtual ~ND280SFGDEvent(){
//         this->ResetEvent();
//     };

//     //-----Setters------

//     void SetTrueTracks   (std::vector <ND280SFGDTrack*>     p_true_Tracks  ) { fTrueTracks = p_true_Tracks;              }
//     void AddTrueTrack    (ND280SFGDTrack*                   p_true_Track   ) { fTrueTracks.push_back(p_true_Track);      }
//     void SetTrueVertex   (ND280SFGDVoxel*                   p_true_Vrtx    ) { fTrueVertex= p_true_Vrtx;                 }
//     void SetRecoVertex   (ND280SFGDVoxel*                   p_reco_Vrtx    ) { fRecoVertex= p_reco_Vrtx;                 }

//     void SetRecoTracks   (std::vector <ND280SFGDTrack*>     p_reco_Tracks  ) { fRecoTracks = p_reco_Tracks;              }
//     void AddRecoTrack    (ND280SFGDTrack*                   p_reco_Track   ) { fRecoTracks.push_back(p_reco_Track);      }
    
//     //------------------

//     //-----Getters------

//     std::vector <ND280SFGDTrack*>     GetTrueTracks()     { return fTrueTracks;   }
//     std::vector <ND280SFGDTrack*>     GetRecoTracks()     { return fRecoTracks;   }
//     ND280SFGDVoxel* GetRecoVertex() { return fRecoVertex;}  
//     ND280SFGDVoxel* GetTrueVertex() { return fTrueVertex;}  

//     //------------------

//     ND280SFGDHit* AddHit(){
//         ND280SFGDHit *hit = new ND280SFGDHit();
//         return hit;
//     }

//     void ResetEvent(Option_t* /*option*/="")
//     {
//         for ( UInt_t i=0; i<fHits.size(); i++)        delete fHits[i];
//         for ( UInt_t i=0; i<fVoxels.size(); i++)      delete fVoxels[i];
//         for ( UInt_t i=0; i<fTrueTracks.size(); i++ ) delete fTrueTracks[i];
//         for ( UInt_t i=0; i<fRecoTracks.size(); i++ ) delete fRecoTracks[i];
//         fHits.clear();
//         fVoxels.clear();
//         fTrueTracks.clear();
//         fRecoTracks.clear();
//     } 

//     ClassDef(ND280SFGDEvent,1);
// };

// #endif



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
    ND280SFGDVoxel* fTrueVertex = nullptr;
    ND280SFGDVoxel* fRecoVertex = nullptr;
    double fNuMom;

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
    void SetTrueVertex   (ND280SFGDVoxel*                   p_true_Vrtx    ) { fTrueVertex= p_true_Vrtx;                 }
    void SetRecoVertex   (ND280SFGDVoxel*                   p_reco_Vrtx    ) { fRecoVertex= p_reco_Vrtx;                 }

    void SetRecoTracks   (std::vector <ND280SFGDTrack*>     p_reco_Tracks  ) { fRecoTracks = p_reco_Tracks;              }
    void AddRecoTrack    (ND280SFGDTrack*                   p_reco_Track   ) { fRecoTracks.push_back(p_reco_Track);      }

    void SetNuMom    (double                            p_nu_mom           ) { fNuMom = p_nu_mom;                        }

    //------------------

    //-----Getters------

    std::vector <ND280SFGDTrack*>     GetTrueTracks()     { return fTrueTracks;   }
    std::vector <ND280SFGDTrack*>     GetRecoTracks()     { return fRecoTracks;   }

    ND280SFGDVoxel* GetRecoVertex() { return fRecoVertex;}  
    ND280SFGDVoxel* GetTrueVertex() { return fTrueVertex;}  

    double          GetNuMom()      { return fNuMom;     }

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
        fTrueVertex = nullptr;
        fRecoVertex = nullptr;
        fNuMom      = -999;
    } 

    ClassDef(ND280SFGDEvent,1);
};

#endif
