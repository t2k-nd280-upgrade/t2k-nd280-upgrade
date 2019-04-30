#include <TCanvas.h>
#include <TH3.h>
#include <TROOT.h>

#include "ND280SFGDVoxelSet.hh"

//********************************************************************
void ND280SFGDVoxelSet::DrawVoxels(std::string name="same", Bool_t all=kFALSE){
//********************************************************************

        if(!fVoxels.size()) return;

        std::vector <Double_t> x = this->GetX();
        std::vector <Double_t> y = this->GetY();
        std::vector <Double_t> z = this->GetZ();

        gStyle->SetCanvasColor(0);
        gStyle->SetMarkerStyle(21);
        gStyle->SetMarkerSize(1.05);
        fData = new TNtuple("fData", "fData", "x:y:z:color");

        if(name == "same") fData = (TNtuple*) gROOT->FindObject("fData"); 
        else if (name == "clean"){
            fData = (TNtuple*) gROOT->FindObject("fData");
            fData->Reset();
            return;
        }

        float lastColor = 0;

        if(all){
            TTree *t1 = (TTree*) fData->CloneTree();
            t1->SetBranchAddress("color", &lastColor);
            t1->GetEntry(t1->GetEntries()-1);
        }

        for(UInt_t i=0; i<x.size(); i++){
            std::cout << "x: " << x[i] << ", y: " << y[i] << ", z: " << z[i]  << "PDG: " << this->GetVoxels()[i]->GetPDG() << std::endl;
            fData->Fill(x[i], y[i], z[i], this->GetVoxels()[i]->GetPDG());
        }

        if((TCanvas*) gROOT->FindObject("canv")){
            TCanvas *oldCanvas = (TCanvas*) gROOT->FindObject("canv"); 
            oldCanvas->Close();
            TCanvas *canv = new TCanvas("canv", "canv", 800, 600, 800, 600); canv->cd();
        }
        else {TCanvas *canv = new TCanvas("canv", "canv", 800, 600, 800, 600); canv->cd();}

        fData->Draw("x:y:z:color","","box2");
        TH3F *htemp = (TH3F*)gPad->GetPrimitive("htemp");
        htemp->GetZaxis()->SetLimits(-100,100);
        htemp->GetYaxis()->SetLimits(-100,100);
        htemp->GetXaxis()->SetLimits(-100,100);
        htemp->SetTitle("");

        gPad->Update();
        gPad->WaitPrimitive();
}
