#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <cstdlib>

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TString.h"
#include "TColor.h"

//#include "/sps/t2k/mlamoure/T2KStyle/SetT2KStyle.H"

TString INPDIR = "/nfs/neutrinos/cjesus/work/jobs/files/NoPionInTarget";
TString OUTDIR = "/nfs/neutrinos/cjesus/work/jobs/plots/";

TString confName[4] = {"ND280 current-like", 
		       "ND280 up-like reference", 
		       "ND280 up-like baseline",
		       "ND280 up-like alternative"};

const int NBins_CosTh = 20;
double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

const int NBins_CosTh2 = 13;
double BinEdges_CosTh2[NBins_CosTh+1] = {-1,-0.8,-0.5,-0.3,0.0,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

const int NBins_Mom = 20;
double BinEdges_Mom[NBins_Mom+1] = {0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1400,1600,1800,2000,2500,3000,4000,5000};

double BinEdges_MomGeV[NBins_Mom+1] = {0,0.100,0.200,0.300,0.400,0.500,0.600,0.700,0.800,0.900,1.000,1.100,1.200,1.400,1.600,1.800,2.000,2.500,3.000,4.000,5.000};
  
int NTargets[6] = {2, 2, 3, 3, 3, 3};

//int NTargets[6] = {3, 3, 3, 3, 3, 3};//DO NOT USE THIS

void plotPionEfficiency(int config, int target, TString suffix="") {

  TFile f(TString::Format("%s/Target%i/config%i_Target%i%s.root",
        INPDIR.Data(),target, config, target, suffix.Data()));
  TTree* t = (TTree*)f.Get("default");
  TH1::AddDirectory(kFALSE);

  int col[5] = {1,2,3,4,6};
  TString name[5] = {"#pi^{+}->#pi^{+}", "#pi^{+}->#pi^{-}", "#pi^{-}->#pi^{-}", "#pi^{-}->#pi^{+}", "#pi^{0}->#pi^{0}"};
  // TString name2[5] = {"true_piplus_seen_as_piplus",
		//       "true_piplus_seen_as_piminus",
		//       "true_piminus_seen_as_piminus",
		//       "true_piminus_seen_as_piplus",
		//       "true_pizero_seen_as_pizero"};

  TString name2[5] = {"pipEff",
          "pipMisId",
          "pimEff",
          "pimMisId",
          "pi0Eff"};

          

  TString name_piplus[3] = {"TPC pion", "Isotarget pion", "ME pion"};

  TString sample = "nu";
  if (suffix.Contains("_antinu"))
    sample = "antinu";
  if (suffix.Contains("_nubkg_antinu"))
    sample = "nubkg_antinu";

  TH1F *h1_mom[5], *h2_mom[5], *heff_mom[5], *h1_cos[5], *h2_cos[5], *heff_cos[5];
  TH2F *h1[5], *h2[5], *heff[5];

  int nbinsM = NBins_Mom;
  int nbinsC = NBins_CosTh;
  double* xbinsM = BinEdges_Mom;
  double* xbinsC = BinEdges_CosTh;

  if (suffix.Contains("_antinu") || suffix.Contains("_nubkg_antinu")) {
    nbinsC = NBins_CosTh2;
    xbinsC = BinEdges_CosTh2;
  }

  for (int i=0; i<5; i++){
    h1_mom[i] = new TH1F(TString::Format("beforeM_%s",name2[i].Data()), 
			 TString::Format("beforeM_%s",name2[i].Data()),
			 nbinsM, xbinsM);
    h2_mom[i] = new TH1F(TString::Format("afterM_%s",name2[i].Data()),  
			 TString::Format("afterM_%s",name2[i].Data()),
			 nbinsM, xbinsM);
    heff_mom[i] = new TH1F(TString::Format("effM_%s",name2[i].Data()),  
			   TString::Format("effM_%s",name2[i].Data()), 
			   nbinsM, xbinsM);
    h1_cos[i] = new TH1F(TString::Format("beforeC_%s",name2[i].Data()),
			 TString::Format("beforeC_%s",name2[i].Data()),
			 nbinsC, xbinsC);
    h2_cos[i] = new TH1F(TString::Format("afterC_%s",name2[i].Data()),
			 TString::Format("afterC_%s",name2[i].Data()), 
			 nbinsC, xbinsC);
    heff_cos[i] = new TH1F(TString::Format("effC_%s",name2[i].Data()), 
			   TString::Format("effC_%s",name2[i].Data()), 
			   nbinsC, xbinsC);

    h1[i] = new TH2F(TString::Format("before_%s",name2[i].Data()),
		     TString::Format("before_%s",name2[i].Data()), 
		     nbinsC, xbinsC, nbinsM, xbinsM);
    h2[i] = new TH2F(TString::Format("after_%s",name2[i].Data()), 
		     TString::Format("before_%s",name2[i].Data()), 
		     nbinsC, xbinsC, nbinsM, xbinsM);
    heff[i] = new TH2F(TString::Format("%s_%i",name2[i].Data(),target), 
		       TString::Format("before_%s",name2[i].Data()), 
		       nbinsC, xbinsC, nbinsM, xbinsM);
  }

  TH1F *h2_PiPlus_cos[3], *h2_PiPlus_mom[3], *heff_PiPlus_cos[3], *heff_PiPlus_mom[3];

  for (int i=0; i<3; i++){
    h2_PiPlus_cos[i] = new TH1F(TString::Format("afterC_spec_%i",i),  
				TString::Format("afterC_spec_%i",i), 
				nbinsC, xbinsC);
    h2_PiPlus_mom[i] = new TH1F(TString::Format("afterM_spec_%i",i),  
				    TString::Format("afterM_spec_%i",i), 
				    nbinsM, xbinsM);
    heff_PiPlus_cos[i] = new TH1F(TString::Format("effC_spec_%i",i),  
				  TString::Format("effC_spec_%i",i), 
				  nbinsC, xbinsC);
    heff_PiPlus_mom[i] = new TH1F(TString::Format("effM_spec_%i",i),  
				      TString::Format("effM_spec_%i",i), 
				      nbinsM, xbinsM);
  }

  Int_t accum_level[1][8];
  int   npiplus,          npiminus,          npizero;
  int   piplus_reco[100], piminus_reco[100], pizero_reco[100];
  float piplus_mom[100],  piminus_mom[100],  pizero_mom[100];
  float piplus_cos[100],  piminus_cos[100],  pizero_cos[100];
  
  t->SetBranchAddress("accum_level",          &accum_level);
  t->SetBranchAddress("sel_nTruePiPlus",      &npiplus);
  t->SetBranchAddress("sel_nTruePiMinus",     &npiminus);
  t->SetBranchAddress("sel_nTruePiZero",      &npizero);
  t->SetBranchAddress("sel_TruePiPlus_reco",  &piplus_reco);
  t->SetBranchAddress("sel_TruePiMinus_reco", &piminus_reco);
  t->SetBranchAddress("sel_TruePiZero_reco",  &pizero_reco);
  t->SetBranchAddress("sel_TruePiPlus_mom",   &piplus_mom);
  t->SetBranchAddress("sel_TruePiMinus_mom",  &piminus_mom);
  t->SetBranchAddress("sel_TruePiZero_mom",   &pizero_mom);
  t->SetBranchAddress("sel_TruePiPlus_cos",   &piplus_cos);
  t->SetBranchAddress("sel_TruePiMinus_cos",  &piminus_cos);
  t->SetBranchAddress("sel_TruePiZero_cos",   &pizero_cos);

  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
    t->GetEntry(ient);
    if (accum_level[0][0]<=2) continue;

    for (int j=0; j<npiplus; j++){
      cout << "npiplus: " << npiplus << endl;
      cout << "piplus_reco[j]: " << piplus_reco[j] << endl;
      cout << "piplus_mom[j]: " << piplus_mom[j] << endl; 

      h1_mom[0]->Fill(piplus_mom[j]);
      h1_mom[1]->Fill(piplus_mom[j]);
      if (piplus_reco[j]>0)
	h2_mom[0]->Fill(piplus_mom[j]);
      if (piplus_reco[j]<0)
	h2_mom[1]->Fill(piplus_mom[j]);

      h1_cos[0]->Fill(piplus_cos[j]);
      h1_cos[1]->Fill(piplus_cos[j]);
      if (piplus_reco[j]>0)
	h2_cos[0]->Fill(piplus_cos[j]);
      if (piplus_reco[j]<0)
	h2_cos[1]->Fill(piplus_cos[j]);

      h1[0]->Fill(piplus_cos[j], piplus_mom[j]);
      h1[1]->Fill(piplus_cos[j], piplus_mom[j]);
      if (piplus_reco[j]>0)
	h2[0]->Fill(piplus_cos[j], piplus_mom[j]);
      if (piplus_reco[j]<0)
	h2[1]->Fill(piplus_cos[j], piplus_mom[j]);

      if (piplus_reco[j]>0){
	h2_PiPlus_cos[piplus_reco[j]-1]->Fill(piplus_cos[j]);
	h2_PiPlus_mom[piplus_reco[j]-1]->Fill(piplus_mom[j]);
      }
    }

    for (int j=0; j<npiminus; j++){
      h1_mom[2]->Fill(piminus_mom[j]);
      h1_mom[3]->Fill(piminus_mom[j]);
      if (piminus_reco[j]>0)
	h2_mom[2]->Fill(piminus_mom[j]);
      if (piminus_reco[j]<0)
	h2_mom[3]->Fill(piminus_mom[j]);

      h1_cos[2]->Fill(piminus_cos[j]);
      h1_cos[3]->Fill(piminus_cos[j]);
      if (piminus_reco[j]>0)
	h2_cos[2]->Fill(piminus_cos[j]);
      if (piminus_reco[j]<0)
	h2_cos[3]->Fill(piminus_cos[j]);

      h1[2]->Fill(piminus_cos[j], piminus_mom[j]);
      h1[3]->Fill(piminus_cos[j], piminus_mom[j]);
      if (piminus_reco[j]>0)
	h2[2]->Fill(piminus_cos[j], piminus_mom[j]);
      if (piminus_reco[j]<0)
	h2[3]->Fill(piminus_cos[j], piminus_mom[j]);
    }

    for (int j=0; j<npizero; j++){
      h1_mom[4]->Fill(pizero_mom[j]);
      if (pizero_reco[j]>0)
	h2_mom[4]->Fill(pizero_mom[j]);

      h1_cos[4]->Fill(pizero_cos[j]);
      if (pizero_reco[j]>0)
	h2_cos[4]->Fill(pizero_cos[j]);

      h1[4]->Fill(piminus_cos[j], pizero_mom[j]);
      if (pizero_reco[j]>0)
	h2[4]->Fill(pizero_cos[j], pizero_mom[j]);

    }

  }

  gStyle->SetOptStat(0);
  
  TCanvas *cMom = new TCanvas("cMom");
  TH1F* hM = (TH1F*)cMom->DrawFrame(xbinsM[0], 0, xbinsM[nbinsM], 1);
  TCanvas *cCos = new TCanvas("cCos");
  TH1F* hC = (TH1F*)cCos->DrawFrame(xbinsC[0], 0, xbinsC[nbinsC], 1);

  hM->SetTitle(TString::Format("%s, %s %i", 
			       confName[config].Data(), 
			       (config!=1 && target!=3) ? "FGD":"Target",
			       (target!=3) ? target : 1));
  hC->SetTitle(TString::Format("%s, %s %i", 
			       confName[config].Data(), 
			       (config!=1 && target!=3) ? "FGD":"Target",
			       (target!=3) ? target : 1));

  hM->GetXaxis()->SetTitle("pion momentum [MeV/c]");
  hM->GetXaxis()->SetTitleSize(0.045);
  hM->GetYaxis()->SetTitle("efficiency");
  hM->GetYaxis()->SetTitleSize(0.045);
  hC->GetXaxis()->SetTitle("pion cos#theta");
  hC->GetXaxis()->SetTitleSize(0.045);
  hC->GetYaxis()->SetTitle("efficiency");
  hC->GetYaxis()->SetTitleSize(0.045);

  TLegend *leg = new TLegend(0.1, 0.88, 0.9, 0.94);
  leg->SetNColumns(5);

  TFile *fW = new TFile(TString::Format("%s/effPion_%s_config%i_target%i.root", OUTDIR.Data(), sample.Data(), config, target), "RECREATE");
  //TFile *fW = new TFile(TString::Format("%s/efficiency_pion.root", OUTDIR.Data());

  for (int i=0; i<5; i++){

    heff_mom[i]->Divide(h2_mom[i], h1_mom[i]);
    heff_cos[i]->Divide(h2_cos[i], h1_cos[i]);
    heff[i]->Divide(h2[i], h1[i]);

    for (int b=0; b<heff_mom[i]->GetNbinsX(); b++) {
      float eff = heff_mom[i]->GetBinContent(b+1);
      heff_mom[i]->SetBinError(b+1, sqrt(eff*(1-eff)/h1_mom[i]->GetBinContent(b+1)));
    }
    for (int b=0; b<heff_cos[i]->GetNbinsX(); b++) {
      float eff = heff_cos[i]->GetBinContent(b+1);
      heff_cos[i]->SetBinError(b+1, sqrt(eff*(1-eff)/h1_cos[i]->GetBinContent(b+1)));
    }

    heff_mom[i]->SetLineColor(col[i]);
    heff_cos[i]->SetLineColor(col[i]);


    heff[i]->GetXaxis()->SetTitle("pion cos#theta");
    heff[i]->GetXaxis()->SetTitleSize(0.045);
    heff[i]->GetYaxis()->SetTitle("pion momentum [MeV/c]");
    heff[i]->GetYaxis()->SetTitleSize(0.045);

    leg->AddEntry(heff_mom[i], name[i], "l");

    cMom->cd();
    heff_mom[i]->Draw("same");
    cCos->cd();
    heff_cos[i]->Draw("same");

    TCanvas *c = new TCanvas("c");
    heff[i]->SetTitle(TString::Format("%s, %s %i, eff(%s)", 
				      confName[config].Data(), 
				      (config!=1 && target!=3) ? "FGD":"Target",
				      (target!=3) ? target : 1, name[i].Data()));
    heff[i]->GetZaxis()->SetRangeUser(0.,1.);
    heff[i]->Draw("colz");
    heff[i]->Write();
    c->SaveAs(TString::Format("%s/2D_effPion%i_%s_config%i_Target%i_2D.eps", 
			      OUTDIR.Data(),i, sample.Data(), config, target));

  }

  cMom->cd();
  leg->Draw("same");
  cCos->cd();
  leg->Draw("same");

  cMom->SaveAs(TString::Format("%s/effPion_%s_config%i_Target%i_mom.eps", 
			       OUTDIR.Data(),sample.Data(), config, target));
  cCos->SaveAs(TString::Format("%s/effPion_%s_config%i_Target%i_cos.eps", 
			       OUTDIR.Data(),sample.Data(), config, target));


  //======================================


  TCanvas *cMom2 = new TCanvas("cMom2");
  hM->Draw();
  TCanvas *cCos2 = new TCanvas("cCos2");
  hC->Draw();

  TLegend *leg2 = new TLegend(0.1, 0.88, 0.9, 0.94);
  leg2->SetNColumns(4);

  int col2[4] = {4, 2, 3};

  for (int i=0; i<3; i++) {
    heff_PiPlus_cos[i]->Divide(h2_PiPlus_cos[i], h1_cos[0]);
    heff_PiPlus_mom[i]->Divide(h2_PiPlus_mom[i], h1_mom[0]);

    for (int b=0; b<heff_PiPlus_mom[i]->GetNbinsX(); b++) {
      float eff = heff_PiPlus_mom[i]->GetBinContent(b+1);
      heff_PiPlus_mom[i]->SetBinError(b+1, sqrt(eff*(1-eff)/h1_mom[i]->GetBinContent(b+1)));
    }
    
    for (int b=0; b<heff_PiPlus_cos[i]->GetNbinsX(); b++) {
      float eff = heff_PiPlus_cos[i]->GetBinContent(b+1);
      heff_PiPlus_cos[i]->SetBinError(b+1, sqrt(eff*(1-eff)/h1_cos[i]->GetBinContent(b+1)));
    }

    heff_PiPlus_mom[i]->SetLineColor(col2[i]);
    heff_PiPlus_cos[i]->SetLineColor(col2[i]);

    leg2->AddEntry(heff_PiPlus_mom[i], name_piplus[i], "l");

    cMom2->cd();
    heff_PiPlus_mom[i]->Draw("same");
    cCos2->cd();
    heff_PiPlus_cos[i]->Draw("same");

  }

  TH1F* hAll_Cos = (TH1F*)heff_PiPlus_cos[0]->Clone();
  TH1F* hAll_Mom = (TH1F*)heff_PiPlus_mom[0]->Clone();  
  for (int i=1; i<3; i++) {
    hAll_Cos->Add(heff_PiPlus_cos[i]);
    hAll_Mom->Add(heff_PiPlus_mom[i]);
    hAll_Cos->SetLineColor(kBlack);
    hAll_Mom->SetLineColor(kBlack);
  }

  leg2->AddEntry(hAll_Mom, "Total", "l");
  cCos2->cd();
  hAll_Cos->Draw("same");
  cMom2->cd();
  hAll_Mom->Draw("same");

  cMom2->cd();
  leg2->Draw("same");
  cCos2->cd();
  leg2->Draw("same");

  cMom2->SaveAs(TString::Format("%s/effPionPlus_%s_config%i_Target%i_mom%s.eps", 
				OUTDIR.Data(),sample.Data(), config, target, suffix.Data()));
  cCos2->SaveAs(TString::Format("%s/effPionPlus_%s_config%i_Target%i_cos%s.eps", 
				OUTDIR.Data(),sample.Data(), config, target, suffix.Data()));

}



void plotTotalPionEfficiency(TString suffix="") {

  TH1::AddDirectory(kFALSE);

  /*
  int N=6;
  int col[6] = {1,3,3,2,4,4};
  int line[6] = {1,1,2,1,1,2};
  int config[6] = {0,1,1,0,1,1};
  int target[6] = {1,1,1,2,2,2};
  TString suffix2[6] = {"_nominal", "_nominal", "_1cmTarget_90-3", "_nominal", "_nominal", "_1cmTarget_90-3"};
  TString name[6] = {"", "", ", better target", "", "", ", better target"};
  */

  int N=2;
  int col[3] = {1,4,6};
  int line[3] = {1,1,1};
  int config[3] = {0,2,2};
  int target[3] = {1,1,3};
  TString suffix2[3] = {"", "", "_betterTarget"};
  TString name[3] = {"", "", " (improved)"};

  TString sample = "nu";
  if (suffix.Contains("_antinu"))
    sample = "antinu";
  if (suffix.Contains("_nubkg_antinu"))
    sample = "nubkg_antinu";

  int nbinsM = NBins_Mom;
  int nbinsC = NBins_CosTh;
  double* xbinsM = BinEdges_MomGeV;
  double* xbinsC = BinEdges_CosTh;

  if (suffix.Contains("_antinu") || suffix.Contains("_nubkg_antinu")) {
    nbinsC = NBins_CosTh2;
    xbinsC = BinEdges_CosTh2;
  }

  TH1F **h1_cos = new TH1F*[N], **h1_mom = new TH1F*[N];
  TH1F **h2_cos = new TH1F*[N], **h2_mom = new TH1F*[N];
  TH1F **heff_cos = new TH1F*[N], **heff_mom = new TH1F*[N];

  for (int i=0; i<N; i++){
    h1_cos[i] = new TH1F(TString::Format("beforeC_%i",i),  
				TString::Format("beforeC_%i",i), 
				nbinsC, xbinsC);
    h1_mom[i] = new TH1F(TString::Format("beforeM_%i",i),  
				    TString::Format("beforeM_%i",i), 
				    nbinsM, xbinsM);
    h2_cos[i] = new TH1F(TString::Format("afterC_%i",i),  
				TString::Format("afterC_%i",i), 
				nbinsC, xbinsC);
    h2_mom[i] = new TH1F(TString::Format("afterM_%i",i),  
				    TString::Format("afterM_%i",i), 
				    nbinsM, xbinsM);
    heff_cos[i] = new TH1F(TString::Format("effC_%i",i),  
				  TString::Format("effC_%i",i), 
				  nbinsC, xbinsC);
    heff_mom[i] = new TH1F(TString::Format("effM_%i",i),  
				      TString::Format("effM_%i",i), 
				      nbinsM, xbinsM);
  }


  Int_t accum_level[1][8];
  int   npiplus;
  int   piplus_reco[100];
  float piplus_mom[100];
  float piplus_cos[100];
  

  for (int i=0; i<N; i++) {

  TFile f(TString::Format("%s/Target%i/config%i_Target%i%s%s.root",
        INPDIR.Data(),target[i], config[i], target[i], suffix.Data(), suffix2[i].Data()));

    // TFile f(TString::Format("/config%i_Target%i%s%s.root",
			 //    config[i], target[i], suffix.Data(), suffix2[i].Data()));
    TTree* t = (TTree*)f.Get("default");

    t->SetBranchAddress("accum_level",          &accum_level);
    t->SetBranchAddress("sel_nTruePiPlus",      &npiplus);
    t->SetBranchAddress("sel_TruePiPlus_reco",  &piplus_reco);
    t->SetBranchAddress("sel_TruePiPlus_mom",   &piplus_mom);
    t->SetBranchAddress("sel_TruePiPlus_cos",   &piplus_cos);

    for (Int_t ient=0; ient < t->GetEntries(); ient++) {
      t->GetEntry(ient);
      if (accum_level[0][0]<=2) continue;

      for (int j=0; j<npiplus; j++){
	h1_mom[i]->Fill(piplus_mom[j]/1000);
	if (piplus_reco[j]>0)
	  h2_mom[i]->Fill(piplus_mom[j]/1000);

	h1_cos[i]->Fill(piplus_cos[j]);
	if (piplus_reco[j]>0)
	  h2_cos[i]->Fill(piplus_cos[j]);
      }

    }

  }

  
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  

  TCanvas *cMom = new TCanvas("cMom");
  TH1F* hM = (TH1F*)cMom->DrawFrame(xbinsM[0], 0, xbinsM[nbinsM], 1);
  TCanvas *cCos = new TCanvas("cCos");
  TH1F* hC = (TH1F*)cCos->DrawFrame(xbinsC[0], 0, xbinsC[nbinsC], 1);

  hM->GetXaxis()->SetTitle("true pion momentum [GeV/c]");
  hM->GetXaxis()->SetTitleSize(0.05);
  hM->GetXaxis()->SetTitleOffset(0.9);
  hM->GetXaxis()->SetLabelSize(0.05);
  hM->GetXaxis()->SetLabelFont(132);
  hM->GetYaxis()->SetTitle("efficiency");
  hM->GetYaxis()->SetTitleSize(0.05);
  hM->GetYaxis()->SetLabelSize(0.05);
  hM->GetYaxis()->SetLabelFont(132);
  hC->GetXaxis()->SetTitle("true pion cos#theta");
  hC->GetXaxis()->SetTitleSize(0.05);
  hC->GetXaxis()->SetTitleOffset(0.9);
  hC->GetXaxis()->SetLabelSize(0.05);
  hC->GetXaxis()->SetLabelFont(132);
  hC->GetYaxis()->SetTitle("efficiency");
  hC->GetYaxis()->SetTitleSize(0.05);
  hC->GetYaxis()->SetLabelSize(0.05);
  hC->GetYaxis()->SetLabelFont(132);

  TLegend *leg = new TLegend(0.1, 0.905, 0.9, 0.96);
  leg->SetNColumns(3);

  for (int i=0; i<N; i++){

    heff_mom[i]->Divide(h2_mom[i], h1_mom[i]);
    heff_cos[i]->Divide(h2_cos[i], h1_cos[i]);
    
    for (int b=0; b<heff_mom[i]->GetNbinsX(); b++) {
      float eff = heff_mom[i]->GetBinContent(b+1);
      heff_mom[i]->SetBinError(b+1, sqrt(eff*(1-eff)/h1_mom[i]->GetBinContent(b+1)));
    }
    for (int b=0; b<heff_cos[i]->GetNbinsX(); b++) {
      float eff = heff_cos[i]->GetBinContent(b+1);
      heff_cos[i]->SetBinError(b+1, sqrt(eff*(1-eff)/h1_cos[i]->GetBinContent(b+1)));
    }

    heff_mom[i]->SetLineWidth(2);
    heff_cos[i]->SetLineWidth(2);
    heff_mom[i]->SetLineColor(col[i]);
    heff_cos[i]->SetLineColor(col[i]);
    heff_mom[i]->SetLineStyle(line[i]);
    heff_cos[i]->SetLineStyle(line[i]);
    
    leg->AddEntry(heff_mom[i], TString::Format("%s %i%s", 
					       (config[i]!=1 && target[i]!=3) ? "FGD":"Target",
					       (target[i]!=3) ? target[i] : 1, name[i].Data()), "l");
    
    cMom->cd();
    heff_mom[i]->Draw("same");
    cCos->cd();
    heff_cos[i]->Draw("same");

  }

  cMom->cd();
  leg->Draw("same");
  cCos->cd();
  leg->Draw("same");

  cMom->SaveAs(TString::Format("%s/effPionTot_%s_mom.eps", 
			       OUTDIR.Data(), sample.Data()));
  cCos->SaveAs(TString::Format("%s/effPionTot_%s_cos.eps", 
			       OUTDIR.Data(), sample.Data()));

  //======================================



}



void plotAll(){

  for (int ic=2; ic<=2; ic++){
    for (int it=1; it<=NTargets[ic]; it++){
      if(ic == 1) continue;
      plotPionEfficiency(ic, it, "_FHC_numu");
//      plotPionEfficiency(ic, it, "_antinu");
//      plotPionEfficiency(ic, it, "_nubkg_antinu");
    }
  }

//plotTotalPionEfficiency("_FHC_numu");
//  plotTotalPionEfficiency("_antinu");
//  plotTotalPionEfficiency("_nubkg_antinu");

}
