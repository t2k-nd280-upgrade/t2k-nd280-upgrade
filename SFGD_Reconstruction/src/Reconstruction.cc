
/////// ALGORITHMS TO PERFORM SFGD RECONSTRUCTION //////////

#include "MergeHits.cc"
#include "FlagCrosstalk.cc"
#include "HitsToVoxels.cc"
#include "ComputeVoxelCharge.cc"

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

//***********************************************************************************************
ND280SFGDEvent* Reconstruct(vector <ND280SFGDHit*> listOfHits){
//***********************************************************************************************

    ND280SFGDEvent* RecoEvent = new ND280SFGDEvent();

    ///// All the functions for reconstruction should contain as the last input the 'version' number.
    ///// In this way, if someone wants, can latter on add a new algorithm version for a particular task.

    vector <ND280SFGDHit*> newListOfHits;

    newListOfHits = MergeHits(listOfHits,0);

    ND280SFGDEvent* OriginalEvent = new ND280SFGDEvent();
    OriginalEvent->SetHits(newListOfHits);

    OriginalEvent->DrawHits(kTRUE,kFALSE,"cc1");


    // MergeHits
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

    //FlagCrosstalk(newListOfHits,0);

    // FlagCrosstalk:
    //
    // The goal of this algorithm is to flag the hits as crosstalk.
    // This flag helps reducing ambiguities when going from 2D hits to voxels.
    // 
    // version0: Algorithm by [DD].   Check 2D hits in Z-layers, search first main hits, afterwards flag as crosstalk
    //                                those hits with a main hit in the same Z-layer and less than 20p.e.                                  
    // version1: Algorithm by [CJ].   First call MergeHitsToVoxels, then 3D fits reconstructed voxels with a straight
    //                                line and flags as crosstalk those hits further away than a distance 'D' to the fit.
    // version2: ...

    vector <ND280SFGDVoxel*> listOfVoxels = HitsToVoxels(newListOfHits,1);

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

    //--------
            //Keep only those hits that are associated to reconstructed voxels:
            newListOfHits.clear();

            for(UInt_t i=0; i<listOfVoxels.size(); i++){
                newListOfHits.push_back(listOfVoxels[i]->GetHits()[0]);
                newListOfHits.push_back(listOfVoxels[i]->GetHits()[1]);
                newListOfHits.push_back(listOfVoxels[i]->GetHits()[2]);
            }

            sort(newListOfHits.begin(),newListOfHits.end());

            for(UInt_t i=0; i<newListOfHits.size(); i++){
                newListOfHits[i]->SetMultiplicity(newListOfHits[i]->GetMultiplicity()+1);
            }

            ND280SFGDHit* prevHit = NULL;

            vector <ND280SFGDHit*> cleanListOfHits;

            for(UInt_t i=0; i<newListOfHits.size(); i++){
                if(newListOfHits[i] != prevHit){
                    cleanListOfHits.push_back(newListOfHits[i]);
                }
                prevHit = newListOfHits[i];
            }
            cout << "# of reco hits: " << cleanListOfHits.size() << endl;
    //---------

    ComputeVoxelCharge(listOfVoxels,0);    

    // ComputeVoxelCharge
    //
    // Algorithm to distribute the charge measured on each fiber among the
    // cubes that are sharing the fiber. 
    //
    // version0: Algorithm by [CJ] & [SM].  
    //
    // version1: ...
    //

    RecoEvent->SetVoxels(listOfVoxels);
    RecoEvent->SetHits(cleanListOfHits);

    RecoEvent->DrawHits(kTRUE,kTRUE,"cc2");

    return RecoEvent;
}

// INSTRUCTIONS TO ADD A NEW ALGORITM:
//   If the algorithm has as a goal the same goal as an existing algorithm, add your new algorithm in the same function
//   as a new version. Please, document it inside the 'Reconstruct' main function.
//
//   If the algorithm has a new goal, create a new file inside /SFGD_Reconstruction/src/, include it in this file,
//   call it in Reconstruct and also document it in Reconstruct.
