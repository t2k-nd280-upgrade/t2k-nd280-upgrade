#include "extraClasses/CHit2D.hxx"
#include "extraClasses/CHit3D.hxx"
#include "extraClasses/CSharedCharge.C"

double FibAttenuation(double ell) {

    const double LongCompFrac_FGD = 0.77;
    const double LongAtt_FGD = 4634.;//*CLHEP::mm;
    const double ShortAtt_FGD = 332.;//*CLHEP::mm;

    return ( LongCompFrac_FGD*exp((-ell)/LongAtt_FGD) + (1-LongCompFrac_FGD)*exp((-ell)/ShortAtt_FGD) );
}


//***********************************************************************************************
void ComputeVoxelCharge_version0(vector <ND280SFGDVoxel*> listOfVoxels){
//***********************************************************************************************

    std::vector<CHit3D>* gHits    = new std::vector<CHit3D>();
    std::vector<CHit2D>* gHitsYZ  = new std::vector<CHit2D>();
    std::vector<CHit2D>* gHitsXZ  = new std::vector<CHit2D>();
    std::vector<CHit2D>* gHitsXY  = new std::vector<CHit2D>();

    for(UInt_t i=0; i<listOfVoxels.size(); i++){
        listOfVoxels[i]->SetEdep(-1);
        listOfVoxels[i]->GetHits()[0]->SetMultiplicity(0);
        listOfVoxels[i]->GetHits()[1]->SetMultiplicity(0);
        listOfVoxels[i]->GetHits()[2]->SetMultiplicity(0);
    }

    for(UInt_t i=0; i<listOfVoxels.size(); i++){
        listOfVoxels[i]->GetHits()[0]->SetMultiplicity(listOfVoxels[i]->GetHits()[0]->GetMultiplicity()+1);
        listOfVoxels[i]->GetHits()[1]->SetMultiplicity(listOfVoxels[i]->GetHits()[1]->GetMultiplicity()+1);
        listOfVoxels[i]->GetHits()[2]->SetMultiplicity(listOfVoxels[i]->GetHits()[2]->GetMultiplicity()+1);
    }

    Int_t ID = 0;
    Int_t voxelID = 0;

        for(UInt_t i=0; i<listOfVoxels.size(); i++){
            CHit3D voxel = CHit3D();
            voxel.SetId(voxelID);
            TVector3 Pos;
            Pos.SetXYZ(listOfVoxels[i]->GetX(),listOfVoxels[i]->GetY(),listOfVoxels[i]->GetZ());
            voxel.SetPosition(Pos);
            voxelID++;
            for(UInt_t hit=0; hit<3; hit++){
                ND280SFGDHit* auxHit = listOfVoxels[i]->GetHits()[hit];
                if(auxHit->GetView() == 0){
                    CHit2D v0hit = CHit2D();
                    v0hit.SetRow(auxHit->GetY());
                    v0hit.SetColumn(auxHit->GetX());
                    v0hit.SetTime(0);
                    v0hit.SetCharge(auxHit->GetCharge());
                    v0hit.SetPlane(2);
                    v0hit.SetId(auxHit->GetCh());
                    gHitsXY->push_back(v0hit);
                    voxel.AddConstituent(auxHit->GetCh(),2);
                    voxel.SetFiberCharge(auxHit->GetCharge(),2);
                }
                if(auxHit->GetView() == 1){
                    CHit2D v1hit = CHit2D();
                    v1hit.SetRow(auxHit->GetZ());
                    v1hit.SetColumn(auxHit->GetX());
                    v1hit.SetTime(0);
                    v1hit.SetCharge(auxHit->GetCharge());
                    v1hit.SetPlane(1);
                    v1hit.SetId(auxHit->GetCh());
                    gHitsXZ->push_back(v1hit);
                    voxel.AddConstituent(auxHit->GetCh(),1);
                    voxel.SetFiberCharge(auxHit->GetCharge(),1);
                }
                if(auxHit->GetView() == 2){
                    CHit2D v2hit = CHit2D();
                    v2hit.SetRow(auxHit->GetZ());
                    v2hit.SetColumn(auxHit->GetY());
                    v2hit.SetTime(0);
                    v2hit.SetCharge(auxHit->GetCharge());
                    v2hit.SetPlane(0);
                    v2hit.SetId(auxHit->GetCh());
                    gHitsYZ->push_back(v2hit);
                    voxel.AddConstituent(auxHit->GetCh(),0);
                    voxel.SetFiberCharge(auxHit->GetCharge(),0);
                }
            }
            gHits->push_back(voxel);
        }
    CSharedCharge(gHits,gHitsYZ,gHitsXZ,gHitsXY,listOfVoxels);
    cout << "alive" << endl;

    // for(UInt_t i=0; i<listOfVoxels.size(); i++){
    //     cout << listOfVoxels[i]->GetHits()[0]->GetMultiplicity() << endl;
    //     cout << listOfVoxels[i]->GetHits()[1]->GetMultiplicity() << endl;
    //     cout << listOfVoxels[i]->GetHits()[2]->GetMultiplicity() << endl;
    //     cout << listOfVoxels[i]->GetEdep() << endl << endl;
    // }
}

//***********************************************************************************************
void ComputeVoxelCharge_version1(vector <ND280SFGDVoxel*> listOfVoxels){
//***********************************************************************************************

    for(UInt_t i=0; i<listOfVoxels.size(); i++){
        listOfVoxels[i]->SetEdep(-1);
        listOfVoxels[i]->GetHits()[0]->SetMultiplicity(listOfVoxels[i]->GetHits()[0]->GetMultiplicity()+1);
        listOfVoxels[i]->GetHits()[1]->SetMultiplicity(listOfVoxels[i]->GetHits()[1]->GetMultiplicity()+1);
        listOfVoxels[i]->GetHits()[2]->SetMultiplicity(listOfVoxels[i]->GetHits()[2]->GetMultiplicity()+1);
    }

    for (Int_t m=1; m<=10; m++){
        for(UInt_t i=0; i<listOfVoxels.size(); i++){
            if(listOfVoxels[i]->GetEdep()>0) continue;
            Double_t recoCharge = 0;
            Int_t mHit0 = listOfVoxels[i]->GetHits()[0]->GetMultiplicity();
            Int_t mHit1 = listOfVoxels[i]->GetHits()[1]->GetMultiplicity();
            Int_t mHit2 = listOfVoxels[i]->GetHits()[2]->GetMultiplicity();
            if( mHit0 == m || mHit1 == m || mHit2 == m) {
                Double_t qHit0 = (1/0.35)*listOfVoxels[i]->GetHits()[0]->GetCharge() /FibAttenuation(10*(listOfVoxels[i]->GetZ()+94));
                Double_t qHit1 = (1/0.35)*listOfVoxels[i]->GetHits()[1]->GetCharge() /FibAttenuation(10*(listOfVoxels[i]->GetY()+26));
                Double_t qHit2 = (1/0.35)*listOfVoxels[i]->GetHits()[2]->GetCharge() /FibAttenuation(10*(listOfVoxels[i]->GetX()+102));

                // cout << "m: " << mHit0 << mHit1 << mHit2 << endl;
                // cout<< "Q0: " << qHit0 << endl;
                // cout<< "Q1: " << qHit1 << endl;
                // cout<< "Q2: " << qHit2 << endl;
                if (mHit0 == m && mHit1 == m && mHit2 == m){        
                    recoCharge = (qHit0+qHit1+qHit2) / 3;
                }
                else if (mHit0 == m && mHit1 == m){
                    recoCharge = (qHit0+qHit1) / 2;
                }
                else if (mHit0 == m && mHit2 == m){
                    recoCharge = (qHit0+qHit2) / 2;
                }
                else if (mHit1 == m && mHit2 == m){       
                    recoCharge = (qHit1+qHit2) / 2;
                }
                else{
                    if(mHit0 == m){
                        recoCharge = qHit0;
                    }
                    else if(mHit1 == m){
                        recoCharge = qHit1;
                    }
                    else if(mHit2 == m){
                        recoCharge = qHit2;
                    }
                }
                listOfVoxels[i]->SetEdep(recoCharge);
            }
        }
    }
    for(UInt_t i=0; i<listOfVoxels.size(); i++){
        if(listOfVoxels[i]->GetEdep()>0) continue;
        listOfVoxels[i]->SetEdep(10000);
    }

}


//***********************************************************************************************
void ComputeVoxelCharge(vector <ND280SFGDVoxel*> listOfVoxels, Int_t version){
//***********************************************************************************************

    if( version == 0 ){
        ComputeVoxelCharge_version0(listOfVoxels);
    }
    else if( version == 1 ){
        ComputeVoxelCharge_version1(listOfVoxels);
    }
    else{
        cerr << "Version" << version << "does not exist in ComputeVoxelCharge function!" << endl;
        exit(1);
    }

}
