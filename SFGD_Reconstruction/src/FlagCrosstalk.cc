
/////// ALGORITHMS TO MERGE 2D HITS INTO 3D VOXELS //////////

void Flagger(Int_t delta1, Int_t delta2, TH2F* h){

    vector <Float_t> hits;
    hits.resize(delta2);
    
    for(Int_t i=0; i<delta1; i++){
        Int_t maxQ = 0;
        for(Int_t j=0; j<delta2; j++){
            hits[j] = (h->GetBinContent(i+1,j+1));
            if(h->GetBinContent(i+1,j+1)>maxQ) maxQ = h->GetBinContent(i+1,j+1);
        }
        if(maxQ){            
            for(Int_t j=0; j<delta2; j++){
                if(!hits[j]) continue;
                if( (Float_t) hits[j]/maxQ < 0.2 && hits[j]<20) h->SetBinContent(i+1,j+1,0);
            }
        }
    }
}

void InvertedFlagger(Int_t delta1, Int_t delta2, TH2F* h){

    vector <Float_t> hits;
    hits.resize(delta1);
    
    for(Int_t i=0; i<delta2; i++){
        Int_t maxQ = 0;
        for(Int_t j=0; j<delta1; j++){
            hits[j] = (h->GetBinContent(i+1,j+1));
            if(h->GetBinContent(i+1,j+1)>maxQ) maxQ = h->GetBinContent(i+1,j+1);
        }
        if(maxQ){            
            for(Int_t j=0; j<delta1; j++){
                if(!hits[j]) continue;
                if( (Float_t) hits[j]/maxQ < 0.2 && hits[j]<20) h->SetBinContent(i+1,j+1,0);
            }
        }
    }
}


//***********************************************************************************************
void FlagCrosstalk_version0(vector <ND280SFGDHit*> listOfHits){
//***********************************************************************************************
    
    Float_t maxX = -999;
    Float_t maxY = -999;
    Float_t maxZ = -999;
    Float_t minX = 999;
    Float_t minY = 999;
    Float_t minZ = 999;

    gStyle->SetOptStat(0);

    for(UInt_t i=0; i<listOfHits.size(); i++){
        if(listOfHits[i]->GetView() == 0){            
            if(listOfHits[i]->GetX()>maxX) maxX = listOfHits[i]->GetX();
            if(listOfHits[i]->GetX()<minX) minX = listOfHits[i]->GetX();
            if(listOfHits[i]->GetY()>maxY) maxY = listOfHits[i]->GetY();
            if(listOfHits[i]->GetY()<minY) minY = listOfHits[i]->GetY();
        }
        if(listOfHits[i]->GetView() == 1){            
            if(listOfHits[i]->GetX()>maxX) maxX = listOfHits[i]->GetX();
            if(listOfHits[i]->GetX()<minX) minX = listOfHits[i]->GetX();
            if(listOfHits[i]->GetZ()>maxZ) maxZ = listOfHits[i]->GetZ();
            if(listOfHits[i]->GetZ()<minZ) minZ = listOfHits[i]->GetZ();
        }
        if(listOfHits[i]->GetView() == 2){            
            if(listOfHits[i]->GetY()>maxY) maxY = listOfHits[i]->GetY();
            if(listOfHits[i]->GetY()<minY) minY = listOfHits[i]->GetY();
            if(listOfHits[i]->GetZ()>maxZ) maxZ = listOfHits[i]->GetZ();
            if(listOfHits[i]->GetZ()<minZ) minZ = listOfHits[i]->GetZ();
        }

    }

    Float_t deltaX = maxX-minX+5;
    Float_t deltaY = maxY-minY+5;
    Float_t deltaZ = maxZ-minZ+5;

    Float_t widthX = abs(minX)+abs(maxX)+2;

    TH2F* hXY = new TH2F("XY","XY",deltaX,minX-2.5,maxX+2.5,deltaY,minY-2.5,maxY+2.5);
    TH2F* hXZ = new TH2F("XZ","XZ",deltaX,minX-2.5,maxX+2.5,deltaZ,minZ-2.5,maxZ+2.5);
    TH2F* hYZ = new TH2F("YZ","YZ",deltaY,minY-2.5,maxY+2.5,deltaZ,minZ-2.5,maxZ+2.5);

    for(UInt_t i=0; i<listOfHits.size(); i++){
        if(listOfHits[i]->GetView() == 0) hXY->Fill(listOfHits[i]->GetX(),listOfHits[i]->GetY(),listOfHits[i]->GetCharge());
        if(listOfHits[i]->GetView() == 1) hXZ->Fill(listOfHits[i]->GetX(),listOfHits[i]->GetZ(),listOfHits[i]->GetCharge());
        if(listOfHits[i]->GetView() == 2) hYZ->Fill(listOfHits[i]->GetY(),listOfHits[i]->GetZ(),listOfHits[i]->GetCharge());
    }

    deltaX < deltaY ? Flagger(deltaX,deltaY,hXY) : InvertedFlagger(deltaX,deltaY,hXY);
    deltaX < deltaZ ? Flagger(deltaX,deltaZ,hXZ) : InvertedFlagger(deltaX,deltaZ,hXZ);
    deltaY < deltaZ ? Flagger(deltaY,deltaZ,hYZ) : InvertedFlagger(deltaY,deltaZ,hYZ);

    for(UInt_t ihit=0; ihit<listOfHits.size(); ihit++){
        if(listOfHits[ihit]->GetView() == 0){
            // cout << "X: " <<  listOfHits[ihit]->GetX() << ", Y: " << listOfHits[ihit]->GetY() << endl;
            // cout << "Content: " << hXY->GetBinContent(listOfHits[ihit]->GetX()-(minX-2.5)+1,listOfHits[ihit]->GetY()-(minY-2.5)+1) << endl << endl;
            // hXY->SetBinContent(listOfHits[ihit]->GetX()-(minX-2.5)+1,listOfHits[ihit]->GetY()-(minY-2.5)+1,10000);
            if(! hXY->GetBinContent(listOfHits[ihit]->GetX()-(minX-2.5)+1,listOfHits[ihit]->GetY()-(minY-2.5)+1)) listOfHits[ihit]->SetxTalkFlag(1);
            if(! hXY->GetBinContent(listOfHits[ihit]->GetX()-(minX-2.5)+1,listOfHits[ihit]->GetY()-(minY-2.5)+1)) hXY->SetBinContent(listOfHits[ihit]->GetX()-(minX-2.5)+1,listOfHits[ihit]->GetY()-(minY-2.5)+1,10000);
        }
        else if(listOfHits[ihit]->GetView() == 1){
            if(! hXZ->GetBinContent(listOfHits[ihit]->GetX()-(minX-2.5)+1,listOfHits[ihit]->GetZ()-(minZ-2.5)+1)) listOfHits[ihit]->SetxTalkFlag(1);
            if(! hXZ->GetBinContent(listOfHits[ihit]->GetX()-(minX-2.5)+1,listOfHits[ihit]->GetZ()-(minZ-2.5)+1)) hXZ->SetBinContent(listOfHits[ihit]->GetX()-(minX-2.5)+1,listOfHits[ihit]->GetZ()-(minZ-2.5)+1,10000);
        }
        else if(listOfHits[ihit]->GetView() == 2){
            if(! hYZ->GetBinContent(listOfHits[ihit]->GetY()-(minY-2.5)+1,listOfHits[ihit]->GetZ()-(minZ-2.5)+1)) listOfHits[ihit]->SetxTalkFlag(1);
            if(! hYZ->GetBinContent(listOfHits[ihit]->GetY()-(minY-2.5)+1,listOfHits[ihit]->GetZ()-(minZ-2.5)+1)) hYZ->SetBinContent(listOfHits[ihit]->GetY()-(minY-2.5)+1,listOfHits[ihit]->GetZ()-(minZ-2.5)+1,10000);
        }
    }

    TCanvas *cc = new TCanvas("c","c",0, 0, 600,600);
    cc->Divide(2,2);

    cc->cd(1);
    hXY->Draw("COLZ");
    cc->cd(2);
    hXZ->Draw("COLZ");
    cc->cd(3);
    hYZ->Draw("COLZ");

    cc->Update();
    cc->WaitPrimitive();

    delete hXY;
    delete hXZ;
    delete hYZ;
    delete cc;
}


//***********************************************************************************************
void FlagCrosstalk_version1(vector <ND280SFGDHit*> listOfHits){
//***********************************************************************************************
}


//***********************************************************************************************
void FlagCrosstalk(vector <ND280SFGDHit*> listOfHits, Int_t version){
//***********************************************************************************************

    if( version == 0 ){
        FlagCrosstalk_version0(listOfHits);
    }
    else if ( version == 1){
        cout << "FlagCrosstalk version 1 is not implemented yet!" << endl;
    }
    else{
        cerr << "Version" << version << "does not exist in FlagCrosstalk function!" << endl;
        exit(1);
    }
}
