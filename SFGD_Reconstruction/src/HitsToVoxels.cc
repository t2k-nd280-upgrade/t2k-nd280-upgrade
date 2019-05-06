
/////// ALGORITHMS TO MERGE 2D HITS INTO 3D VOXELS //////////

//***********************************************************************************************
vector <ND280SFGDVoxel*> HitsToVoxels_version0(vector <ND280SFGDHit*> listOfHits, vector <ND280SFGDVoxel*> listOfVoxels, Bool_t removeCrosstalk=kFALSE){
//***********************************************************************************************

    Bool_t DEBUG = kFALSE;

    struct pseudoVoxel{
        Int_t         View;
        Double_t      X;
        Double_t      Y;
        Double_t      Z;
        ND280SFGDHit* hit1;
        ND280SFGDHit* hit2;
        ND280SFGDHit* hit3;
        Int_t         found;
    };

    vector <pseudoVoxel> vXYZ;
    vXYZ.clear();

    listOfVoxels.clear();

     vector <Int_t> analyzedHits(listOfHits.size(),0);

    for(UInt_t ihit=0; ihit<listOfHits.size(); ihit++){
        //cout << "crosstalk flag: " << listOfHits[ihit]->GetxTalkFlag() << endl;
        if(removeCrosstalk && listOfHits[ihit]->GetxTalkFlag()) analyzedHits[ihit] = 1;
        if(analyzedHits[ihit]) continue;
        else analyzedHits[ihit] = 1;
        if(DEBUG){            
            cout << "ihit: " << ihit << endl;
            cout << listOfHits[ihit]->GetX() << ", " << listOfHits[ihit]->GetY() << ", " << listOfHits[ihit]->GetZ() << endl;
        }
        for(UInt_t jhit=0; jhit<listOfHits.size(); jhit++){
            if(removeCrosstalk && listOfHits[jhit]->GetxTalkFlag()){ analyzedHits[ihit] = 1; continue; }
            if(ihit == jhit) continue;
            if(listOfHits[ihit]->GetView() == listOfHits[jhit]->GetView()) continue;

            if(listOfHits[ihit]->GetView() == 0 && listOfHits[jhit]->GetView() == 1 && listOfHits[ihit]->GetX() == listOfHits[jhit]->GetX()){
                pseudoVoxel Vox;
                Vox.X = listOfHits[ihit]->GetX();
                Vox.Y = listOfHits[ihit]->GetY();
                Vox.Z = listOfHits[jhit]->GetZ();
                Vox.View = 0;
                Vox.hit1 = listOfHits[ihit];
                Vox.hit2 = listOfHits[jhit];
                vXYZ.push_back(Vox);
                if(DEBUG){                    
                    cout << "jhit: " << jhit << endl;
                    cout << listOfHits[jhit]->GetX() << ", " << listOfHits[jhit]->GetY() << ", " << listOfHits[jhit]->GetZ() << endl;
                }
            }
            if(listOfHits[ihit]->GetView() == 0 && listOfHits[jhit]->GetView() == 2  && listOfHits[ihit]->GetY() == listOfHits[jhit]->GetY()){
                pseudoVoxel Vox;
                Vox.X = listOfHits[ihit]->GetX();
                Vox.Y = listOfHits[ihit]->GetY();
                Vox.Z = listOfHits[jhit]->GetZ();
                Vox.View = 1;
                Vox.hit1 = listOfHits[ihit];
                Vox.hit3 = listOfHits[jhit];
                vXYZ.push_back(Vox);
                if(DEBUG){
                    cout << "jhit: " << jhit << endl;
                    cout << listOfHits[jhit]->GetX() << ", " << listOfHits[jhit]->GetY() << ", " << listOfHits[jhit]->GetZ() << endl;    
                }

            }
            if(listOfHits[ihit]->GetView() == 1 && listOfHits[jhit]->GetView() == 2  && listOfHits[ihit]->GetZ() == listOfHits[jhit]->GetZ()){
                pseudoVoxel Vox;
                Vox.X = listOfHits[ihit]->GetX();
                Vox.Y = listOfHits[jhit]->GetY();
                Vox.Z = listOfHits[jhit]->GetZ();
                Vox.View = 2;
                Vox.hit2 = listOfHits[ihit];
                Vox.hit3 = listOfHits[jhit];
                vXYZ.push_back(Vox);
                if(DEBUG){
                    cout << "jhit: " << jhit << endl;
                    cout << listOfHits[jhit]->GetX() << ", " << listOfHits[jhit]->GetY() << ", " << listOfHits[jhit]->GetZ() << endl;
                }
            }
        }
    }

    if(DEBUG) cout << "# of 2D matches: " << vXYZ.size() << endl;

    if(DEBUG){        
        Int_t CNT = 0;
        for(UInt_t ivox=0; ivox<vXYZ.size(); ivox++){
            for(UInt_t jvox=0; jvox<vXYZ.size(); jvox++){
                if(ivox == jvox) continue;
                if(vXYZ[ivox].X == vXYZ[jvox].X && vXYZ[ivox].Y == vXYZ[jvox].Y && vXYZ[ivox].Z == vXYZ[jvox].Z && vXYZ[ivox].View == vXYZ[jvox].View){
                    CNT++;
                }
            }     
        }
        cout << "# of duplicated 2D matches: " << CNT << endl;
    }

    if(DEBUG){        
        for(UInt_t ivox=0; ivox<vXYZ.size(); ivox++){
            cout << "XYZ:  " << vXYZ[ivox].X << ", " << vXYZ[ivox].Y<< ", "  << vXYZ[ivox].Z << ", " << vXYZ[ivox].View << endl; 
        }
    }

    vector <Int_t> analyzedVoxels(vXYZ.size(),0);

    for(UInt_t ivox=0; ivox<vXYZ.size(); ivox++) vXYZ[ivox].found = 0;
    
    for(UInt_t ivox=0; ivox<vXYZ.size(); ivox++){
        Int_t found = 0;
        if(analyzedVoxels[ivox]) continue;
        analyzedVoxels[ivox] = 1;
        for(UInt_t jvox=0; jvox<vXYZ.size(); jvox++){
            if(analyzedVoxels[jvox]) continue;
            if(vXYZ[ivox].X == vXYZ[jvox].X && vXYZ[ivox].Y == vXYZ[jvox].Y && vXYZ[ivox].Z == vXYZ[jvox].Z && vXYZ[ivox].View != vXYZ[jvox].View){
                analyzedVoxels[jvox] = 1;
                found++;
                if(vXYZ[ivox].found){
                    vXYZ[ivox].found = found;
                    continue;
                }
                ND280SFGDVoxel* newVoxel = new ND280SFGDVoxel(vXYZ[ivox].X,vXYZ[ivox].Y,vXYZ[ivox].Z);
                vector <ND280SFGDHit*> hitsInVoxel;
                hitsInVoxel.resize(3);
                if(vXYZ[ivox].View == 0){              
                    hitsInVoxel[0] = vXYZ[ivox].hit1;
                    hitsInVoxel[1] = vXYZ[ivox].hit2;
                    hitsInVoxel[2] = vXYZ[jvox].hit3;
                }
                else if(vXYZ[ivox].View == 1){                    
                    hitsInVoxel[0] = vXYZ[ivox].hit1;
                    hitsInVoxel[1] = vXYZ[jvox].hit2;
                    hitsInVoxel[2] = vXYZ[ivox].hit3;
                }
                else if(vXYZ[ivox].View == 2){                    
                    hitsInVoxel[0] = vXYZ[jvox].hit1;
                    hitsInVoxel[1] = vXYZ[ivox].hit2;
                    hitsInVoxel[2] = vXYZ[ivox].hit3;
                }
                else{
                    cerr << "View must be 0,1 or 2!" << endl;
                    exit(1);
                }
                newVoxel->SetHits(hitsInVoxel);
                listOfVoxels.push_back(newVoxel);
                vXYZ[ivox].found = found;
            }
        }
    }

    cout << "# of recoVoxels: " << listOfVoxels.size() << endl;

    if(DEBUG){        
        cout << "Voxels:" << endl;
        for(UInt_t ivox=0; ivox<listOfVoxels.size(); ivox++){
            cout << ivox << "| XYZ: " << listOfVoxels[ivox]->GetX() << ", " << listOfVoxels[ivox]->GetY() << ", " <<  listOfVoxels[ivox]->GetZ() << endl;   
        }
    }

    if(DEBUG){        
        Int_t CNT = 0;
        for(UInt_t ivox=0; ivox<listOfVoxels.size(); ivox++){
            for(UInt_t jvox=0; jvox<listOfVoxels.size(); jvox++){
                if(ivox == jvox) continue;
                if(listOfVoxels[ivox]->GetX() == listOfVoxels[jvox]->GetX() && listOfVoxels[ivox]->GetY() == listOfVoxels[jvox]->GetY() && listOfVoxels[ivox]->GetZ() == listOfVoxels[jvox]->GetZ() ){
                    CNT++;
                }
            }     
        }
        cout << "# of duplicated voxels: " << CNT << endl;
    }

    // for(UInt_t ivox=0; ivox<listOfVoxels.size(); ivox++){
    //     std::cout << "multiplicity hit1: " << listOfVoxels[ivox]->GetHits()[0]->GetMultiplicity() << std::endl;
    // }

    // for(UInt_t ivox=0; ivox<listOfVoxels.size(); ivox++){
    //     cout << "XYZ:  " << listOfVoxels[ivox]->GetX() << ", " << listOfVoxels[ivox]->GetY() << ", "  << listOfVoxels[ivox]->GetZ() << endl;
    //     cout << "hits: " << listOfVoxels[ivox]->GetHits()[0] << ", " << listOfVoxels[ivox]->GetHits()[1]  << ", "  << listOfVoxels[ivox]->GetHits()[2] << endl;  
    // }

    return listOfVoxels;

}


//***********************************************************************************************
vector <ND280SFGDVoxel*> HitsToVoxels(vector <ND280SFGDHit*> listOfHits, Int_t version){
//***********************************************************************************************

    vector <ND280SFGDVoxel*> listOfVoxels;

    if( version == 0 ){
        listOfVoxels = HitsToVoxels_version0(listOfHits,listOfVoxels);
    }
    else if ( version == 1){
        listOfVoxels = HitsToVoxels_version0(listOfHits,listOfVoxels,kTRUE);
    }
    else{
        cerr << "Version" << version << "does not exist in HitsToVoxels function!" << endl;
        exit(1);
    }

    return listOfVoxels;
}
