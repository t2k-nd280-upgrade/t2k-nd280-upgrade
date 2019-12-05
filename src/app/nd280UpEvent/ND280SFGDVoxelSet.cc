#include <TCanvas.h>
#include <TH3.h>
#include <TROOT.h>

#include <cstring>

#include "ND280SFGDVoxelSet.hh"
#include "ND280SFGDHit.hh"
#include <TH2.h>

//******************************************************************************
void ND280SFGDVoxelSet::DrawSet(bool wait = false, TString canvName = "cc", int color = 0, int opt = 0){
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

    TH2F *hXY = new TH2F(hXYname.Data(),hXYname.Data(), 220, -0., 220, 220, 0., 220);      
    TH2F *hXZ = new TH2F(hXZname.Data(),hXZname.Data(), 220, -0., 220, 220, 0., 220);
    TH2F *hYZ = new TH2F(hYZname.Data(),hYZname.Data(), 220, -0., 220, 220, 0., 220);

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
            hXY->Fill(hit->GetX(),hit->GetY(),hit->GetPE());
            if(hit->GetPE()>Qmax1) Qmax1 = hit->GetPE();
            if(hit->GetY()>maxY) maxY = hit->GetY();
            if(hit->GetY()<minY) minY = hit->GetY();
        }
        if(hit->GetView() == 1){
            hXZ->Fill(hit->GetX(),hit->GetZ(),hit->GetPE());
            if(hit->GetPE()>Qmax2) Qmax2 = hit->GetPE();
            if(hit->GetX()>maxX) maxX = hit->GetX();
            if(hit->GetX()<minX) minX = hit->GetX();
        }
        if(hit->GetView() == 2){
            hYZ->Fill(hit->GetY(),hit->GetZ(),hit->GetPE());
            if(hit->GetPE()>Qmax3) Qmax3 = hit->GetPE();
            if(hit->GetZ()>maxZ) maxZ = hit->GetZ();
            if(hit->GetZ()<minZ) minZ = hit->GetZ();
        }
    }

    if(std::strncmp(canvName.Data(),"cc",3)){
        if(gROOT->FindObject(canvName.Data())) delete gROOT->FindObject(canvName.Data());
    }

    TCanvas *cc = new TCanvas(canvName.Data(),canvName.Data(),0, 0, 600,600);

    if(opt!=1){
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
    }

    // draw only the 2D hits
    if(wait && !opt){            
        cc->Update();
        cc->WaitPrimitive();
    }
    // draw only the voxels
    else if(opt==1){
        cc->Divide(1);
        cc->cd(1);
        this->Draw3D(wait,color);
    }
    else if(opt==2){
        cc->cd(4);
        cc->Update();
        this->Draw3D(wait,color);
    }

    if(!std::strncmp(canvName.Data(),"cc",3)){
        delete hXY;
        delete hXZ;
        delete hYZ;
        delete cc;
    }
}

//********************************************************************
void ND280SFGDVoxelSet::Draw3D(bool wait = false, int color = 0){
//********************************************************************

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

    std::map<int,int> ID_to_color;
    

    std::vector<int> listOfTrackID;
    for (auto v:this->GetVoxels())
        for(auto t:v->GetTrueTrackIDs())
            listOfTrackID.push_back(t);

    if(listOfTrackID.size()){
        std::sort   (listOfTrackID.begin(), listOfTrackID.end());
        listOfTrackID.erase(std::unique (listOfTrackID.begin(), listOfTrackID.end()), listOfTrackID.end()); 
    }

    int color_cnt = 0;
    if(color == 4){
        for(auto t:listOfTrackID)
            ID_to_color[t] = color_cnt++;

        // for(auto t:listOfTrackID)
        //     std::cout << "trackID-color: " << t << "," << ID_to_color.find(t)->second << std::endl;


        // for(auto v:this->GetVoxels()){
        //     if(v->GetTrueTrackIDs().size()){
        //         std::cout << "id-color: ";
        //         for(auto d:v->GetTrueTrackIDs())
        //             std::cout << d << "-" << ID_to_color.find(d)->second << " || ";
        //         std::cout << std::endl;
        //     }
        // }
    }


    for(UInt_t i=0; i<x.size(); i++){
        if(!color) fData->Fill(x[i], y[i], z[i], this->GetVoxels()[i]->GetRecoPE());
        else if(color ==1) fData->Fill(x[i], y[i], z[i], this->GetVoxels()[i]->GetClusterID()); 
        else if(color ==2) fData->Fill(x[i], y[i], z[i], this->GetVoxels()[i]->GetRecoTrackIDs()[0]); 
        else if(color ==3) fData->Fill(x[i], y[i], z[i], this->GetVoxels()[i]->GetTruePE());
        else if(color ==4) {if(this->GetVoxels()[i]->GetTrueTrackIDs().size()) fData->Fill(x[i], y[i], z[i], ID_to_color.find(this->GetVoxels()[i]->GetTrueTrackIDs()[0])->second); } 
        else if(color ==5) fData->Fill(x[i], y[i], z[i], this->GetVoxels()[i]->GetTrueType()); 
        else if(color ==6) fData->Fill(x[i], y[i], z[i], this->GetVoxels()[i]->GetDistance()); 
        else if(color ==7) fData->Fill(x[i], y[i], z[i], this->GetVoxels()[i]->GetTime()); 
        else if(color ==8) fData->Fill(x[i], y[i], z[i], this->GetVoxels()[i]->IsBranching()); 
        else if(color ==9) fData->Fill(x[i], y[i], z[i], this->GetVoxels()[i]->IsKink()); 
        else if(color ==10){
            if(this->GetVoxels()[i]->IsBranching())   fData->Fill(x[i], y[i], z[i], 2);
            else if(this->GetVoxels()[i]->IsKink())   fData->Fill(x[i], y[i], z[i], 1);
            else fData->Fill(x[i], y[i], z[i], 0);
        }
        else fData->Fill(x[i], y[i], z[i], color);

        if(x[i]>maxX) maxX = x[i];
        if(x[i]<minX) minX = x[i];
        if(y[i]>maxY) maxY = y[i];
        if(y[i]<minY) minY = y[i];
        if(z[i]>maxZ) maxZ = z[i];
        if(z[i]<minZ) minZ = z[i];
    }
  
    fData->Draw("x:y:z:color","","box2");
    TH3F *htemp = (TH3F*)gPad->GetPrimitive("htemp");
    htemp->GetZaxis()->SetLimits(minX-tolerance,maxX+tolerance);
    htemp->GetYaxis()->SetLimits(minY-tolerance,maxY+tolerance);
    htemp->GetXaxis()->SetLimits(minZ-tolerance,maxZ+tolerance);
    htemp->SetTitle("");
    if(wait){            
        gPad->Update();
        gPad->WaitPrimitive();
    }
}


//********************************************************************
void ND280SFGDVoxelSet::DrawHits            (bool wait, TString canvName, int color)   { this->DrawSet(wait,canvName,color,0); }
void ND280SFGDVoxelSet::DrawVoxels          (bool wait, TString canvName, int color)   { this->DrawSet(wait,canvName,color,1); }
void ND280SFGDVoxelSet::DrawHitsAndVoxels   (bool wait, TString canvName, int color)   { this->DrawSet(wait,canvName,color,2); }
void ND280SFGDVoxelSet::DrawVoxelsRecoPE    (bool wait, TString canvName)              { this->DrawSet(wait,canvName,0,1);     }
void ND280SFGDVoxelSet::DrawClusters        (bool wait, TString canvName)              { this->DrawSet(wait,canvName,1,1);     }
void ND280SFGDVoxelSet::DrawRecoTracks      (bool wait, TString canvName)              { this->DrawSet(wait,canvName,2,1);     }
void ND280SFGDVoxelSet::DrawVoxelsTruePE    (bool wait, TString canvName)              { this->DrawSet(wait,canvName,3,1);     }
void ND280SFGDVoxelSet::DrawTrueTracks      (bool wait, TString canvName)              { this->DrawSet(wait,canvName,4,1);     }
void ND280SFGDVoxelSet::DrawTrueType        (bool wait, TString canvName)              { this->DrawSet(wait,canvName,5,1);     }
void ND280SFGDVoxelSet::DrawGraphDistance   (bool wait, TString canvName)              { this->DrawSet(wait,canvName,6,1);     }
void ND280SFGDVoxelSet::DrawTimeSeparation  (bool wait, TString canvName)              { this->DrawSet(wait,canvName,7,1);     }
void ND280SFGDVoxelSet::DrawBranchings      (bool wait, TString canvName)              { this->DrawSet(wait,canvName,8,1);     }
void ND280SFGDVoxelSet::DrawKinks           (bool wait, TString canvName)              { this->DrawSet(wait,canvName,9,1);     }
void ND280SFGDVoxelSet::DrawBreakingPoints  (bool wait, TString canvName)              { this->DrawSet(wait,canvName,10,1);     }
//********************************************************************


