
/////// ALGORITHMS TO PERFORM SFGD RECONSTRUCTION //////////

#include "MergeHits.cc"
#include "FlagCrosstalk.cc"
#include "HitsToVoxels.cc"
#include "ComputeVoxelCharge.cc"
#include "FindClusters.cc"
#include "../utils/global_variables.h"
    
// DEVELOPERS
// To ask specific questions, please, address them directly to the developer of the algorithm:
//
// César Jesús-Valls [CJ], contact cjesus@ifae.es
//
// Dana Douqa [DD],        contact dana.douqa@etu.unige.ch
//
// Davide Sgalaberna [DS], contact davide.sgalaberna@cern.ch
//
// Sergey Martynenko [SM], contact sergey.martynenko@stonybrook.edu
//
// ...


// GENERAL INFORMATION 
// The main function (the entrance point) is 'Reconstruct'. 

// The idea is to take a list of hits, as we are going to have in the final SFGD detector,
// and to use them to fill as much information as possible in a ND280SFGDEvent object.

// Latter on, the reconstructed information and the true information coming from MC can be compared
// in order to evaluate the performance of the algorithm. 

double FA(double ell) {
    const double LongCompFrac_FGD = 0.77;
    const double LongAtt_FGD = 4634.;//*CLHEP::mm;
    const double ShortAtt_FGD = 332.;//*CLHEP::mm;

    return 0.35*( LongCompFrac_FGD*exp((-ell)/LongAtt_FGD) + (1-LongCompFrac_FGD)*exp((-ell)/ShortAtt_FGD) );
}


//***********************************************************************************************
ND280SFGDEvent* Reconstruct(vector <ND280SFGDHit*> listOfHits, vector <ND280SFGDHit*> &listOfHitsCopy, vector <ND280SFGDVoxel*> listOfTrueVoxels){
//***********************************************************************************************

    ND280SFGDEvent* RecoEvent = new ND280SFGDEvent();
 
    for(size_t nhits=0; nhits<listOfHits.size(); nhits++){
        ND280SFGDHit* auxHit = new ND280SFGDHit();
        *auxHit = *listOfHits[nhits];

        // cout << listOfHits[nhits] << endl;
        // cout << auxHit << endl;
        // cout << listOfHits[nhits]->GetCharge() << endl;
        // cout << auxHit->GetCharge() << endl << endl;
        listOfHitsCopy.push_back(auxHit);
    }

    ///// All the functions for reconstruction should contain as the last input the 'version' number.
    ///// In this way, if someone wants, can latter on add a new algorithm version for a particular task.

    vector <ND280SFGDHit*> newListOfHits;

    newListOfHits = MergeHits(listOfHitsCopy,0);

    // for(uint iii=0; iii<listOfHits.size(); iii++){
    //     cout << listOfHits[iii]->GetCharge() << endl;
    // }

    ND280SFGDEvent* OriginalEvent = new ND280SFGDEvent();
    OriginalEvent->SetHits(listOfHits);

    if(SHOW_TRUE) OriginalEvent->DrawHits(kTRUE,kFALSE,"cc1");
    // MergeHits summary:
    //
    // Hits, either coming from data or MC are merged (Summed) if they are in the same fiber and not well separated
    // in time. In reality, we can not distinguish two hits if they happen closer than a time 'T'.  
    //
    // Hits in the same fiber but separated by a time larger than 'T' must be stored as separated hits as they are 
    // often related to relevant physics, as Michel electron decay.
    //
    // version0: Algorithm by [CJ].   Sums all hits in the same fiber irrespectively of its time. Time is still not stored
    //                                in the MC. It returns a list of the merged hits.
    //
    // version1: ...
    //    

    FlagCrosstalk(newListOfHits,0);

    // FlagCrosstalk summary:
    //
    // The goal of this algorithm is to flag the hits as crosstalk.
    // This flag helps reducing ambiguities when going from 2D hits to voxels.
    // 
    // version0: Algorithm by [DD].   Check 2D hits in Z-layers, search first main hits, afterwards flag as crosstalk
    //                                those hits with a main hit in the same Z-layer and less than 20p.e.                                  
    // version1: Algorithm by [CJ].   First call MergeHitsToVoxels, then 3D fits reconstructed voxels with a straight
    //                                line and flags as crosstalk those hits further away than a distance 'D' to the fit.
    // version2: ...

    vector <ND280SFGDVoxel*> listOfVoxels;
    if (USE_CROSSTALK_FLAG)  listOfVoxels = HitsToVoxels(newListOfHits,1);
    else listOfVoxels = HitsToVoxels(newListOfHits,0);

    // HitsToVoxels
    //
    // 2D Hits, are matches to form 3D voxels. Multiplicity of each hit is also computed.
    //
    // version0: Algorithm by [CJ].   Compared the 2D views. A XYZ voxel is created if it can be formed from 2
    //                                different 2D views. The number of reconstructed voxels is equal or larger 
    //                                than the real one.
    //
    // version1: ...
    //

    // vector <ND280SFGDVoxel*> newListOfVoxels;
    // for(size_t ivox=0; ivox<listOfVoxels.size(); ivox++){        
    //     for(size_t jvox=0; jvox<listOfTrueVoxels.size(); jvox++){
    //         if( listOfVoxels[ivox] == listOfVoxels[jvox]){
    //             newListOfVoxels.push_back(listOfVoxels[ivox]);
    //         }
    //     }
    // }

    // listOfVoxels = newListOfVoxels;
    // newListOfVoxels.clear();
    
    //--------
            //Keep only those hits that are associated to reconstructed voxels:
            // newListOfHits.clear();

            // for(UInt_t i=0; i<listOfVoxels.size(); i++){
            //     newListOfHits.push_back(listOfVoxels[i]->GetHits()[0]);
            //     newListOfHits.push_back(listOfVoxels[i]->GetHits()[1]);
            //     newListOfHits.push_back(listOfVoxels[i]->GetHits()[2]);
            // }

            // sort(newListOfHits.begin(),newListOfHits.end());

            // for(UInt_t i=0; i<newListOfHits.size(); i++){
            //     newListOfHits[i]->SetMultiplicity(0);
            // }

            // ND280SFGDHit* prevHit = NULL;

            // vector <ND280SFGDHit*> cleanListOfHits;

            // for(UInt_t i=0; i<newListOfHits.size(); i++){
            //     if(newListOfHits[i] != prevHit){
            //         cleanListOfHits.push_back(newListOfHits[i]);
            //     }
            //     prevHit = newListOfHits[i];
            // }

            // newListOfHits.clear();
            // newListOfHits = cleanListOfHits;
    //---------

    // Int_t uniqueID = 0;
    // for(size_t ihit = 0; ihit < newListOfHits.size(); ihit++){
    //     newListOfHits[ihit]->SetCh(uniqueID);
    //     uniqueID++;
    // }

    Int_t uniqueID = 0;
    for(size_t ivox = 0; ivox < listOfVoxels.size(); ivox++){
        listOfVoxels[ivox]->GetHits()[0]->SetCh(uniqueID);
        uniqueID++;
        listOfVoxels[ivox]->GetHits()[1]->SetCh(uniqueID);
        uniqueID++;
        listOfVoxels[ivox]->GetHits()[2]->SetCh(uniqueID);
        uniqueID++;
    }

    //cout << "# OF RECO VOXELS: " << listOfVoxels.size() << endl;
    //ComputeVoxelCharge(listOfVoxels,0);    

    // Int_t CNT = 0;
    // for(size_t ivox=0; ivox<listOfVoxels.size(); ivox++){        
    //     CNT = 0;
    //     for(size_t jvox=0; jvox<listOfTrueVoxels.size(); jvox++){
    //         cout << "X: " << listOfTrueVoxels[jvox]->GetX() <<  endl;
    //         cout << "Y: " << listOfTrueVoxels[jvox]->GetY() <<  endl;
    //         cout << "Z: " << listOfTrueVoxels[jvox]->GetZ() <<  endl << endl;
    //         if( listOfVoxels[ivox]->GetX() == listOfTrueVoxels[jvox]->GetX() && listOfVoxels[ivox]->GetY() == listOfTrueVoxels[jvox]->GetY()  && listOfVoxels[ivox]->GetZ() == listOfTrueVoxels[jvox]->GetZ() ){
    //             cout << "True Edep: " << listOfTrueVoxels[jvox]->GetEdep() << ", RecoEdep: " << listOfVoxels[ivox]->GetEdep() << endl;
    //             cout << "multiplicity hit0: " << listOfVoxels[ivox]->GetHits()[0]->GetMultiplicity() << ", local reco charge: " << listOfVoxels[ivox]->GetLocalHitsQ()[0] << ", local true charge: " << listOfTrueVoxels[jvox]->GetLocalHitsQ()[0] << ", merged charge: " << listOfTrueVoxels[jvox]->GetHits()[0]->GetCharge() / FA((listOfTrueVoxels[jvox]->GetZ()+94)*10) <<  endl;
    //             cout << "multiplicity hit1: " << listOfVoxels[ivox]->GetHits()[1]->GetMultiplicity() << ", local reco charge: " << listOfVoxels[ivox]->GetLocalHitsQ()[1] << ", local true charge: " << listOfTrueVoxels[jvox]->GetLocalHitsQ()[1] << ", merged charge: " << listOfTrueVoxels[jvox]->GetHits()[1]->GetCharge() / FA((listOfTrueVoxels[jvox]->GetY()+26)*10) <<  endl;
    //             cout << "multiplicity hit2: " << listOfVoxels[ivox]->GetHits()[2]->GetMultiplicity() << ", local reco charge: " << listOfVoxels[ivox]->GetLocalHitsQ()[2] << ", local true charge: " << listOfTrueVoxels[jvox]->GetLocalHitsQ()[2] << ", merged charge: " << listOfTrueVoxels[jvox]->GetHits()[2]->GetCharge() / FA((listOfTrueVoxels[jvox]->GetX()+102)*10) <<  endl;
    //             CNT++;
    //         }
    //     }
    //     if(!CNT){
    //             cout << "RecoEdep: " << listOfVoxels[ivox]->GetEdep() << endl;
    //             cout << "multiplicity hit0: " << listOfVoxels[ivox]->GetHits()[0]->GetMultiplicity() << ", local reco charge: " << listOfVoxels[ivox]->GetLocalHitsQ()[0] << ", X: " << listOfVoxels[ivox]->GetX() <<  endl;
    //             cout << "multiplicity hit1: " << listOfVoxels[ivox]->GetHits()[1]->GetMultiplicity() << ", local reco charge: " << listOfVoxels[ivox]->GetLocalHitsQ()[1] << ", Y: " << listOfVoxels[ivox]->GetY() <<  endl;
    //             cout << "multiplicity hit2: " << listOfVoxels[ivox]->GetHits()[2]->GetMultiplicity() << ", local reco charge: " << listOfVoxels[ivox]->GetLocalHitsQ()[2] << ", Z: " << listOfVoxels[ivox]->GetZ() <<  endl;
    //     }
    // }
    // cout << "Total Expected: " << listOfTrueVoxels.size() << endl;
    // cout << "Total Seen: " << CNT << endl;


    
    // ComputeVoxelCharge summary:
    //
    // Algorithm to distribute the charge measured on each fiber among the
    // cubes that are sharing the fiber. 
    //
    // version0: Algorithm by [CJ] & [SM].  
    //
    // version1: ...
    //

    //vector <ND280SFGDTrack*> listOfTracks;

    //listOfTracks = FindClusters(listOfVoxels,0);    

    // Find clusters summary:
    //
    //
    //
    //
    //
    //
    //
    //

    RecoEvent->SetVoxels(listOfVoxels);
    RecoEvent->SetHits(newListOfHits);

    if (SHOW_RECO) RecoEvent->DrawHits(kTRUE,kTRUE,"cc2");

    //RecoEvent->SetTracks(listOfTracks);
    //RecoEvent->DrawVoxels(kTRUE,kFALSE,1,0);
    // for(std::size_t itrk=0;itrk<listOfTracks.size();++itrk){
    //     if(listOfTracks.size() == 1) {cout << "single cluster: " << itrk << endl; RecoEvent->DrawVoxels(kTRUE,kFALSE,1,0);}   //mode 0 == first and print
    //     else{
    //         if(itrk == 0) {cout << "first: " << itrk << endl; listOfTracks[itrk]->DrawVoxels(kTRUE,kFALSE,itrk+1,3);}         //mode 3 == first but not print
    //         else if(itrk+1 == listOfTracks.size()) {cout << "last: " << itrk << endl; listOfTracks[itrk]->DrawVoxels(kTRUE,kFALSE,itrk+1,2);} // mode 2 == not first and print
    //         else {cout << "middle: " << itrk << endl; listOfTracks[itrk]->DrawVoxels(kTRUE,kFALSE,itrk+1,1);}                // mode 1 == not first, not print
    //     }
    // }
    //RecoEvent->DrawHits(kTRUE,kTRUE,"cc2");
    // if(listOfTracks.size() == 1)
    //     if(listOfTracks[0]->GetVoxels().size() > 30 )// {RecoEvent->DrawHits(kFALSE,kFALSE,"cc1"); RecoEvent->DrawTracks();}
    
    return RecoEvent;
}

// INSTRUCTIONS TO ADD A NEW ALGORITM:
//   If the algorithm has as a goal the same goal as an existing algorithm, add your new algorithm in the same function
//   as a new version. Please, document it inside the 'Reconstruct' main function.
//
//   If the algorithm has a new goal, create a new file inside /SFGD_Reconstruction/src/, include it in this file,
//   call it in Reconstruct and also document it in Reconstruct.
