#include <TCanvas.h>
#include <TH3.h>
#include <TROOT.h>

#include <cstring>

#include "ND280SFGDVoxelSet.hh"
#include "ND280SFGDHit.hh"
#include <TH2.h>

//******************************************************************************
void ND280SFGDVoxelSet::DrawHits(Bool_t p_Wait = kFALSE, Bool_t p_All = kFALSE, TString canvName = "cc"){
//******************************************************************************

    this->Init();

    TString hXYname = "viewXY";
    TString hXZname = "viewXZ";
    TString hYZname = "viewYZ";

    if(std::strncmp(canvName.Data(),"cc",3)){
        hXYname+="_"+canvName;
        hXZname+="_"+canvName;
        hYZname+="_"+canvName;
        if(gROOT->FindObject(hXYname.Data())) delete gROOT->FindObject(hXYname.Data());
        if(gROOT->FindObject(hXZname.Data())) delete gROOT->FindObject(hXZname.Data());
        if(gROOT->FindObject(hYZname.Data())) delete gROOT->FindObject(hYZname.Data());
    }

    TH2F *hXY = new TH2F(hXYname.Data(),hXYname.Data(), 220, -110., 110, 220, -110., 110);      
    TH2F *hXZ = new TH2F(hXZname.Data(),hXZname.Data(), 220, -110., 110, 220, -110., 110);
    TH2F *hYZ = new TH2F(hYZname.Data(),hYZname.Data(), 220, -110., 110, 220, -110., 110);

    Double_t Qmax1 = 0;
    Double_t Qmax2 = 0;
    Double_t Qmax3 = 0;

    Int_t maxX = -999;
    Int_t maxY = -999;
    Int_t maxZ = -999;
    Int_t minX = 999;
    Int_t minY = 999;
    Int_t minZ = 999;

    Int_t tolerance = 10;

    gStyle->SetOptStat(0);

    for(UInt_t i=0; i<fHits.size(); i++){
        ND280SFGDHit* hit = fHits[i];

        if(hit->GetView() == 0){
            hXY->Fill(hit->GetX(),hit->GetY(),hit->GetCharge());
            if(hit->GetCharge()>Qmax1) Qmax1 = hit->GetCharge();
            if(hit->GetY()>maxY) maxY = hit->GetY();
            if(hit->GetY()<minY) minY = hit->GetY();
        }
        if(hit->GetView() == 1){
            hXZ->Fill(hit->GetX(),hit->GetZ(),hit->GetCharge());
            if(hit->GetCharge()>Qmax2) Qmax2 = hit->GetCharge();
            if(hit->GetX()>maxX) maxX = hit->GetX();
            if(hit->GetX()<minX) minX = hit->GetX();
        }
        if(hit->GetView() == 2){
            hYZ->Fill(hit->GetY(),hit->GetZ(),hit->GetCharge());
            if(hit->GetCharge()>Qmax3) Qmax3 = hit->GetCharge();
            if(hit->GetZ()>maxZ) maxZ = hit->GetZ();
            if(hit->GetZ()<minZ) minZ = hit->GetZ();
        }
    }

    if(std::strncmp(canvName.Data(),"cc",3)){
        if(gROOT->FindObject(canvName.Data())) delete gROOT->FindObject(canvName.Data());
    }

    TCanvas *cc = new TCanvas(canvName.Data(),canvName.Data(),0, 0, 600,600);
    cc->Divide(2,2);

    hXY->GetZaxis()->SetRangeUser(0.1,1.05*Qmax1);
    hXZ->GetZaxis()->SetRangeUser(0.1,1.05*Qmax2);
    hYZ->GetZaxis()->SetRangeUser(0.1,1.05*Qmax3);

    cc->cd(1);
    hXY->Draw("COLZ");
    hXY->GetXaxis()->SetRangeUser(minX-tolerance,maxX+tolerance);
    hXY->GetYaxis()->SetRangeUser(minY-tolerance,maxY+tolerance);
    cc->cd(2);
    hXZ->Draw("COLZ");
    hXZ->GetXaxis()->SetRangeUser(minX-tolerance,maxX+tolerance);
    hXZ->GetYaxis()->SetRangeUser(minZ-tolerance,maxZ+tolerance);
    cc->cd(3);
    hYZ->Draw("COLZ");
    hYZ->GetXaxis()->SetRangeUser(minY-tolerance,maxY+tolerance);
    hYZ->GetYaxis()->SetRangeUser(minZ-tolerance,maxZ+tolerance);

    if(p_Wait && !p_All){            
        cc->Update();
        cc->WaitPrimitive();
    }

    if(p_All){
        cc->cd(4);
        cc->Update();
        this->DrawVoxels(kTRUE,kTRUE,0,0);
    }


    if(!std::strncmp(canvName.Data(),"cc",3)){
        delete hXY;
        delete hXZ;
        delete hYZ;
        delete cc;
    }


}


//********************************************************************
void ND280SFGDVoxelSet::DrawVoxels(Bool_t p_Wait = kFALSE, Bool_t p_All = kFALSE, Int_t color=0, Int_t mode = 0){
//********************************************************************

    if(!mode || mode == 3) {this->Init(); fData = (TNtuple*) gROOT->FindObject("fData"); fData->Reset();}
    if(mode){
        fData = (TNtuple*) gROOT->FindObject("fData");
        std::cout << "Entries in fData: " << fData->GetEntries() << std::endl;
        if(!fData) {
            std::cout << "fData not found!!!" << std::endl;
            return;
        }
    }

    if(!fVoxels.size()) return;

    std::vector <Double_t> x = this->GetX();
    std::vector <Double_t> y = this->GetY();
    std::vector <Double_t> z = this->GetZ();

    Int_t maxX = -999;
    Int_t maxY = -999;
    Int_t maxZ = -999;
    Int_t minX = 999;
    Int_t minY = 999;
    Int_t minZ = 999;

    Int_t tolerance = 10;

    for(UInt_t i=0; i<x.size(); i++){
        //fData->Fill(x[i], y[i], z[i], this->GetVoxels()[i]->GetTrackID());
        if(!color) fData->Fill(x[i], y[i], z[i], this->GetVoxels()[i]->GetEdep());
        else       fData->Fill(x[i], y[i], z[i], color);

        if(x[i]>maxX) maxX = x[i];
        if(x[i]<minX) minX = x[i];
        if(y[i]>maxY) maxY = y[i];
        if(y[i]<minY) minY = y[i];
        if(z[i]>maxZ) maxZ = z[i];
        if(z[i]<minZ) minZ = z[i];
    }

    if(mode == 1 || mode == 3) {return;}

    if(color){
        minX = -100;
        minY = -100;
        minZ = -100;
        maxX =  100;
        maxY =  100;
        maxZ =  100;
    }

    if(p_All){    
        fData->Draw("x:y:z:color","","box2");
        TH3F *htemp = (TH3F*)gPad->GetPrimitive("htemp");
        htemp->GetZaxis()->SetLimits(minX-tolerance,maxX+tolerance);
        htemp->GetYaxis()->SetLimits(minY-tolerance,maxY+tolerance);
        htemp->GetXaxis()->SetLimits(minZ-tolerance,maxZ+tolerance);
        htemp->SetTitle("");
        if(p_Wait){            
            gPad->Update();
            gPad->WaitPrimitive();
        }
    }
    else{
        TCanvas *canv = new TCanvas("canv", "canv", 800, 600, 800, 600);
        fData->Draw("x:y:z:color","","box2");
        TH3F *htemp = (TH3F*)gPad->GetPrimitive("htemp");
        htemp->GetZaxis()->SetLimits(minX-tolerance,maxX+tolerance);
        htemp->GetYaxis()->SetLimits(minY-tolerance,maxY+tolerance);
        htemp->GetXaxis()->SetLimits(minZ-tolerance,maxZ+tolerance);
        htemp->SetTitle("");
        if(p_Wait){            
            gPad->Update();
            gPad->WaitPrimitive();
        }
        if(p_Wait){            
            gPad->Update();
            gPad->WaitPrimitive();
        }
        delete canv;
    }
}

//********************************************************************
void ND280SFGDVoxelSet::DrawHitsAndVoxels(){
//********************************************************************
    DrawHits(kFALSE,kTRUE);
}
