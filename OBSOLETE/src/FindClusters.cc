// #include "extraClasses/CHit2D.hxx"
// #include "extraClasses/CHit3D.hxx"
// #include "extraClasses/CCluster3DHits.hxx"

// //***********************************************************************************************
// std::vector<ND280SFGDTrack*> FindClusters_version0(vector <ND280SFGDVoxel*> listOfVoxels){
// //***********************************************************************************************

//     std::vector<CHit3D>* gHits    = new std::vector<CHit3D>();
//     std::vector<CHit2D>* gHitsYZ  = new std::vector<CHit2D>();
//     std::vector<CHit2D>* gHitsXZ  = new std::vector<CHit2D>();
//     std::vector<CHit2D>* gHitsXY  = new std::vector<CHit2D>();
//     std::vector<CHit2D>* unused2D  = new std::vector<CHit2D>();

//     std::vector<ND280SFGDTrack*> listOfTracks;

//     Int_t ID = 0;
//     Int_t voxelID = 0;

//         for(UInt_t i=0; i<listOfVoxels.size(); i++){
//             CHit3D voxel = CHit3D();
//             voxel.SetId(voxelID);
//             TVector3 Pos;
//             Pos.SetXYZ(listOfVoxels[i]->GetX(),listOfVoxels[i]->GetY(),listOfVoxels[i]->GetZ());
//             voxel.SetPosition(Pos);
//             voxelID++;
//             for(UInt_t hit=0; hit<3; hit++){
//                 ND280SFGDHit* auxHit = listOfVoxels[i]->GetHits()[hit];
//                 if(auxHit->GetView() == 0){
//                     CHit2D v0hit = CHit2D();
//                     v0hit.SetRow(auxHit->GetY());
//                     v0hit.SetColumn(auxHit->GetX());
//                     v0hit.SetTime(0);
//                     v0hit.SetCharge(auxHit->GetCharge());
//                     //v0hit.SetCharge(0);
//                     v0hit.SetPlane(2);
//                     v0hit.SetId(ID);
//                     gHitsXY->push_back(v0hit);
//                     voxel.AddConstituent(ID,2);
//                     voxel.SetFiberCharge(auxHit->GetCharge(),2);
//                     ID++;
//                 }
//                 if(auxHit->GetView() == 1){
//                     CHit2D v1hit = CHit2D();
//                     v1hit.SetRow(auxHit->GetZ());
//                     v1hit.SetColumn(auxHit->GetX());
//                     v1hit.SetTime(0);
//                     v1hit.SetCharge(auxHit->GetCharge());
//                     v1hit.SetPlane(1);
//                     v1hit.SetId(ID);
//                     gHitsXZ->push_back(v1hit);
//                     voxel.AddConstituent(ID,1);
//                     voxel.SetFiberCharge(auxHit->GetCharge(),1);
//                     ID++;
//                 }
//                 if(auxHit->GetView() == 2){
//                     CHit2D v2hit = CHit2D();
//                     v2hit.SetRow(auxHit->GetZ());
//                     v2hit.SetColumn(auxHit->GetY());
//                     v2hit.SetTime(0);
//                     v2hit.SetCharge(auxHit->GetCharge());
//                     v2hit.SetPlane(0);
//                     v2hit.SetId(ID);
//                     gHitsYZ->push_back(v2hit);
//                     voxel.AddConstituent(ID,0);
//                     voxel.SetFiberCharge(auxHit->GetCharge(),0);
//                     ID++;
//                 }
//             }
//             gHits->push_back(voxel);
//         }
//     listOfTracks = CCluster3DHits(unused2D,gHits,listOfVoxels);
//     return listOfTracks;
// }

//***********************************************************************************************
std::vector<ND280SFGDTrack*> FindClusters_version0(vector <ND280SFGDVoxel*> listOfVoxels){
//***********************************************************************************************

std::vector<ND280SFGDTrack*> listOfTracks;
return listOfTracks;

}

//***********************************************************************************************
vector <ND280SFGDTrack*> FindClusters(vector <ND280SFGDVoxel*> listOfVoxels, Int_t version){
//***********************************************************************************************

    vector <ND280SFGDTrack*> listOfTracks;

    if( version == 0 ){
        listOfTracks = FindClusters_version0(listOfVoxels);
    }
    else{
        cerr << "Version" << version << "does not exist in HitsToVoxels function!" << endl;
        exit(1);
    }

    return listOfTracks;

}
