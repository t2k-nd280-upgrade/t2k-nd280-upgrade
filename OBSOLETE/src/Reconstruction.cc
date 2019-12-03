
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
 
    // try to improve this... using more elegant way.
    // use copies to not overwritte pointer information! -----
    for(size_t nhits=0; nhits<listOfHits.size(); nhits++){
        ND280SFGDHit* auxHit = new ND280SFGDHit();
        *auxHit = *listOfHits[nhits];
        listOfHitsCopy.push_back(auxHit);
    }

    if (!listOfHitsCopy.size()) return RecoEvent;

    // vector<Voxel*> listOfVoxelsCopy;
    // for(size_t nvoxs=0; nvoxs<listOfTrueVoxels.size(); nvoxs++){
    //     ND280SFGDVoxel* auxVox = new ND280SFGDHit();
    //     *auxVox = *listOfTrueVoxels[nvox];
    //     listOfVoxelsCopy.push_back(auxVox);
    // }
    // ------------------------------------------------------


    // ---- start the reconstruction
    // All the functions for reconstruction should contain as the last input the 'version' number.
    // In this way, if someone wants, can latter on add a new algorithm version for a particular task.
    // ------------------------------------------------------

    // add drawability for the original event:
    if(SHOW_TRUE){
        ND280SFGDEvent* OriginalEvent = new ND280SFGDEvent();
        OriginalEvent->SetHits(listOfHitsCopy);
        OriginalEvent->DrawHits(kTRUE,kFALSE,"cc1");
    }

    // flag the list of hits:
    FlagCrosstalk(listOfHitsCopy,0);

    // vector <ND280SFGDVoxel*> listOfVoxels;
    // if (USE_CROSSTALK_FLAG)  listOfVoxels = HitsToVoxels(newListOfHits,1);
    // else listOfVoxels = HitsToVoxels(newListOfHits,0);

    //vector <ND280SFGDVoxel*> listOfVoxels_wFlag;
    //vector <ND280SFGDVoxel*> listOfVoxels_woFlag;

    //listOfVoxels_wFlag  = HitsToVoxels(listOfHitsCopy,1);
    //listOfVoxels_woFlag = HitsToVoxels(listOfHitsCopy,0);

    // add some member variable to the voxel to store 'core','xtalk','ghost' true info.

    //for(auto woFvox:listOfVoxels_woFlag){}
    
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

    // Int_t uniqueID = 0;
    // for(size_t ivox = 0; ivox < listOfVoxels.size(); ivox++){
    //     listOfVoxels[ivox]->GetHits()[0]->SetCh(uniqueID);
    //     uniqueID++;
    //     listOfVoxels[ivox]->GetHits()[1]->SetCh(uniqueID);
    //     uniqueID++;
    //     listOfVoxels[ivox]->GetHits()[2]->SetCh(uniqueID);
    //     uniqueID++;
    // }

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

    //RecoEvent->SetVoxels(listOfVoxels);
    //cout << "size of newListOfHits: " << listOfHitsCopy.size() << endl;
    RecoEvent->SetHits(listOfHitsCopy);

    //if (SHOW_RECO) RecoEvent->DrawHits(kTRUE,kTRUE,"cc2");

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
