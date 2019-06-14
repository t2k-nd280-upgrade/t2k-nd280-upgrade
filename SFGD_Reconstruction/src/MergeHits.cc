
/////// ALGORITHMS TO MERGE 2D HITS INTO 3D VOXELS //////////

//***********************************************************************************************
vector <ND280SFGDHit*> MergeHits_version0(vector <ND280SFGDHit*> listOfHits){
//***********************************************************************************************

    vector <ND280SFGDHit*> newListOfHits;
    vector <Int_t> analyzedHits(listOfHits.size(),0);


    std::cout << "Original # of hits: " << listOfHits.size() << std::endl;

    // TH2F* hTvsQ = new TH2F("TvsQ","TvsQ",40,-100,-60,100,0,300);
    // for(UInt_t ihit=0; ihit<listOfHits.size(); ihit++){
    //     hTvsQ->Fill(listOfHits[ihit]->GetDt(),listOfHits[ihit]->GetCharge());
    // }

    for(UInt_t ihit=0; ihit<listOfHits.size(); ihit++){
        listOfHits[ihit]->SetxTalkFlag(0);
        if(analyzedHits[ihit]) continue;
        newListOfHits.push_back(listOfHits[ihit]);
        analyzedHits[ihit]=1;
        for(UInt_t jhit=0; jhit<listOfHits.size(); jhit++){
            if(analyzedHits[jhit]) continue;
            if(listOfHits[ihit]->GetView() == listOfHits[jhit]->GetView() && listOfHits[ihit]->GetX() == listOfHits[jhit]->GetX() && listOfHits[ihit]->GetY() == listOfHits[jhit]->GetY() && listOfHits[ihit]->GetZ() == listOfHits[jhit]->GetZ() ){
                analyzedHits[jhit] = 1;
                listOfHits[ihit]->SetCharge( listOfHits[ihit]->GetCharge() + listOfHits[jhit]->GetCharge() );
            }
        }
    }

    // TCanvas* canvasTvsQ = new TCanvas("TvsQ");
    // canvasTvsQ->cd();
    // hTvsQ->Draw("COLZ");
    // canvasTvsQ->Update();
    // canvasTvsQ->WaitPrimitive();
    // delete canvasTvsQ;
    // delete hTvsQ;

    std::cout << "# of merged hits: " << newListOfHits.size() << std::endl;

    Double_t Qxy = 0;
    Double_t Qxz = 0;
    Double_t Qyz = 0;


    for(UInt_t ihit=0; ihit<listOfHits.size(); ihit++){
        if(listOfHits[ihit]->GetView() == 0) Qxy+=listOfHits[ihit]->GetCharge();
        if(listOfHits[ihit]->GetView() == 1) Qxz+=listOfHits[ihit]->GetCharge();
        if(listOfHits[ihit]->GetView() == 2) Qyz+=listOfHits[ihit]->GetCharge();
    }

    return newListOfHits;
}


//***********************************************************************************************
vector <ND280SFGDHit*> MergeHits_version1(vector <ND280SFGDHit*> listOfHits){
//***********************************************************************************************

    vector <ND280SFGDHit*> newListOfHits;

    return newListOfHits;
}


//***********************************************************************************************
vector <ND280SFGDHit*> MergeHits(vector <ND280SFGDHit*> listOfHits, Int_t version){
//***********************************************************************************************

    vector <ND280SFGDHit*> newListOfHits;

    if( version == 0 ){
        newListOfHits = MergeHits_version0(listOfHits);
    }
    else if ( version == 1){
        cout << "MergeHits version 1 is not implemented yet!" << endl;
    }
    else{
        cerr << "Version" << version << "does not exist in HitsToVoxels function!" << endl;
        exit(1);
    }

    return newListOfHits;
}
