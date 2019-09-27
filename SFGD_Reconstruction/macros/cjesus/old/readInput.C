#define readTree_cxx
#include "readTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void readTree::Loop()
{
//   In a ROOT session, you can do:
//      root> .L readTree.C
//      root> readTree t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch


    TString fileIn = "/Users/cjesus/Documents/Data/MC_output.root";

    TCanvas *c1 = new TCanvas("c1","c1",0,0,600,600);
    c1->Divide(3,2);
    TCanvas *c2 = new TCanvas("c2","c2",600,0,600,600);

    TFile *FileInput  = new TFile(fileIn.Data(),"update");
    TTree* data = (TTree*) FileInput->Get("AllEvents");
    Int_t nEvents = data->GetEntries();

    TH2F* PE_xy; TH2F* PE_xz; TH2F* PE_yz;
    PE_xy = (TH2F*) FileInput->Get("OutMPPCProj2D_XY");
    PE_xz = (TH2F*) FileInput->Get("OutMPPCProj2D_XZ");
    PE_yz = (TH2F*) FileInput->Get("OutMPPCProj2D_YZ");

    TH2F* CT_xy; TH2F* CT_xz; TH2F* CT_yz;
    CT_xy = (TH2F*) PE_xy->Clone("CT_xy");
    CT_xz = (TH2F*) PE_xz->Clone("CT_xz");
    CT_yz = (TH2F*) PE_yz->Clone("CT_yz");

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if(jentry ==0 ) continue;

        for(int index=0; index<numHits; index++){
            //cout << "PE(YZ,XZ,XY): " << hitPE[index][0] << ", " << hitPE[index][1] << ", " << hitPE[index][2] << endl;
            //cout << "Loc(X,Y,Z): " << hitLocation[index][0] << ", " << hitLocation[index][0] << ", " << hitLocation[index][0] << endl;
            cout << "trajID: " << hitTraj[index] << endl;
            PE_yz->Fill(hitLocation[index][1]*10,hitLocation[index][2]*10,hitPE[index][0]);
            PE_xz->Fill(hitLocation[index][0]*10,hitLocation[index][2]*10,hitPE[index][1]);
            PE_xy->Fill(hitLocation[index][0]*10,hitLocation[index][1]*10,hitPE[index][2]);
            if(!crosstalk[index]){            
                CT_yz->SetBinContent(CT_yz->FindBin(hitLocation[index][1]*10,hitLocation[index][2]*10),2);
                CT_xz->SetBinContent(CT_xz->FindBin(hitLocation[index][0]*10,hitLocation[index][2]*10),2);
                CT_xy->SetBinContent(CT_xy->FindBin(hitLocation[index][0]*10,hitLocation[index][1]*10),2);
            }
            if(crosstalk[index]){            
                if( CT_yz->GetBinContent(CT_yz->FindBin(hitLocation[index][1]*10,hitLocation[index][2]*10)) != 2 )CT_yz->SetBinContent(CT_yz->FindBin(hitLocation[index][1]*10,hitLocation[index][2]*10),1);
                if( CT_xz->GetBinContent(CT_xz->FindBin(hitLocation[index][0]*10,hitLocation[index][2]*10)) != 2 )CT_xz->SetBinContent(CT_xz->FindBin(hitLocation[index][0]*10,hitLocation[index][2]*10),1);
                if( CT_xy->GetBinContent(CT_xy->FindBin(hitLocation[index][0]*10,hitLocation[index][1]*10)) != 2 )CT_xy->SetBinContent(CT_xy->FindBin(hitLocation[index][0]*10,hitLocation[index][1]*10),1);
            }
        }

        gStyle->SetCanvasColor(0);
        gStyle->SetMarkerStyle(21);
        gStyle->SetMarkerSize(1.05);
        TNtuple* evt3D = new TNtuple("fData", "fData", "x:y:z:color");

        int maxX = -999;
        int maxY = -999;
        int maxZ = -999;
        int minX = 999;
        int minY = 999;
        int minZ = 999;
        int tolerance = 10;

        for(int index=0; index<numHits; index++){
            evt3D->Fill(hitLocation[index][0], hitLocation[index][1], hitLocation[index][2], hitTraj[index]);
            if(hitLocation[index][0]>maxX) maxX = hitLocation[index][0];
            if(hitLocation[index][0]<minX) minX = hitLocation[index][0];
            if(hitLocation[index][1]>maxY) maxY = hitLocation[index][1];
            if(hitLocation[index][1]<minY) minY = hitLocation[index][1];
            if(hitLocation[index][2]>maxZ) maxZ = hitLocation[index][2];
            if(hitLocation[index][2]<minZ) minZ = hitLocation[index][2];
        }

        cout << "X max,min: " << maxX << "," << minX << endl;
        cout << "Y max,min: " << maxY << "," << minY << endl;
        cout << "Z max,min: " << maxZ << "," << minZ << endl;

        PE_yz->GetXaxis()->SetRangeUser(10*(minY-tolerance),10*(maxY+tolerance));
        PE_yz->GetYaxis()->SetRangeUser(10*(minZ-tolerance),10*(maxZ+tolerance));
        PE_xz->GetXaxis()->SetRangeUser(10*(minX-tolerance),10*(maxX+tolerance));
        PE_xz->GetYaxis()->SetRangeUser(10*(minZ-tolerance),10*(maxZ+tolerance));
        PE_xy->GetXaxis()->SetRangeUser(10*(minX-tolerance),10*(maxX+tolerance));
        PE_xy->GetYaxis()->SetRangeUser(10*(minZ-tolerance),10*(maxZ+tolerance));
        CT_yz->GetXaxis()->SetRangeUser(10*(minY-tolerance),10*(maxY+tolerance));
        CT_yz->GetYaxis()->SetRangeUser(10*(minZ-tolerance),10*(maxZ+tolerance));
        CT_xz->GetXaxis()->SetRangeUser(10*(minX-tolerance),10*(maxX+tolerance));
        CT_xz->GetYaxis()->SetRangeUser(10*(minZ-tolerance),10*(maxZ+tolerance));
        CT_xy->GetXaxis()->SetRangeUser(10*(minX-tolerance),10*(maxX+tolerance));
        CT_xy->GetYaxis()->SetRangeUser(10*(minZ-tolerance),10*(maxZ+tolerance));

        c1->cd(1);
        PE_yz->Draw("COLZ");
        c1->cd(2);
        PE_xz->Draw("COLZ");
        c1->cd(3);
        PE_xy->Draw("COLZ");
        c1->cd(4);
        CT_yz->Draw("COLZ");
        c1->cd(5);
        CT_xz->Draw("COLZ");
        c1->cd(6);
        CT_xy->Draw("COLZ");
        c1->Update();

        c2->cd();
        evt3D->Draw("x:y:z:color","","box2");
        TH3F *htemp = (TH3F*) gPad->GetPrimitive("htemp");
        if(htemp) htemp->GetZaxis()->SetLimits(minX-tolerance,maxX+tolerance);
        if(htemp) htemp->GetYaxis()->SetLimits(minY-tolerance,maxY+tolerance);
        if(htemp) htemp->GetXaxis()->SetLimits(minZ-tolerance,maxZ+tolerance);
        if(htemp) htemp->SetTitle("");
        c2->Update();
        c2->WaitPrimitive();

        PE_yz->Reset();
        PE_xz->Reset();
        PE_xy->Reset();
        CT_yz->Reset();
        CT_xz->Reset();
        CT_xy->Reset();

      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}

TFile*f = new TFile("MC_output.root");
TTree* tree;f->GetObject("AllEvents",tree);
tree->MakeClass("readTree");
