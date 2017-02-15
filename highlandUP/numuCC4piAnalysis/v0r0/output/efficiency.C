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
#include "TGraphAsymmErrors.h"
#include "TTree.h"
#include "TH1F.h"
#include "THStack.h"
#include "TChain.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TString.h"
#include "TLatex.h"


TH1F* computeEfficiency(TString file, int branch, int cut1, int cut2, 
		     TString var, TString var_title, 
		     int nbins, double* xbins) {
  
  TFile* f = new TFile(file);
  TTree* t = (TTree*)f->Get("truth");
  
  TH1F* h_beforeCut = new TH1F(TString::Format("before_%s_%i",var.Data(),branch),
			       TString::Format("before_%s_%i",var.Data(),branch),
			       nbins, xbins);
  TH1F* h_afterCut = new TH1F(TString::Format("after_%s_%i",var.Data(),branch),
			      TString::Format("after_%s_%i",var.Data(),branch),
			      nbins, xbins);
  


  Int_t accum_level[1][1];
  Float_t variable;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(var,           &variable);

  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
    t->GetEntry(ient);
    
    if (accum_level[0][branch]>cut1)
      h_beforeCut->Fill(variable);
    if (accum_level[0][branch]>cut2)
      h_afterCut->Fill(variable);

  }

  h_beforeCut->Sumw2();
  h_afterCut->Sumw2();

  TH1F *h_eff = new TH1F(TString::Format("h_eff_%s_%i",var.Data(),branch),
			 TString::Format("h_eff_%s_%i",var.Data(),branch),
			 nbins, xbins);
  h_eff->Divide(h_afterCut, h_beforeCut);

  for (int i=0; i<nbins; i++) {
    float eff = h_eff->GetBinContent(i+1);
    h_eff->SetBinError(i+1, sqrt(eff*(1-eff)/h_beforeCut->GetBinContent(i+1)));
  }
  
  h_eff->GetXaxis()->SetTitle(var_title);
  h_eff->GetXaxis()->SetTitleSize(0.05);
  h_eff->GetXaxis()->SetTitleOffset(0.9);
  h_eff->GetYaxis()->SetTitleSize(0.05);
  h_eff->GetYaxis()->SetTitle("efficiency");
  
  //delete t;
  //delete f;

  return h_eff;

}


void plotEfficiency(int cut1, int cut2, 
		    TString var, TString var_title, 
		    int nbins, double* xbins, int target, bool current=false, bool left=false) {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  TH1F* h_eff_Fwd  = computeEfficiency(TString::Format("jobs/files/%s_Target%i_%i.root",
						       current ? "current":"upgrade",
						       target, 0),
				       0, cut1, cut2,
				       var, var_title,
				       nbins, xbins);
  TH1F* h_eff_Bwd  = computeEfficiency(TString::Format("jobs/files/%s_Target%i_%i.root",
						       current ? "current":"upgrade",
						       target, 1),
				       0, cut1, cut2,
				       var, var_title,
				       nbins, xbins);
  TH1F* h_eff_HA   = computeEfficiency(TString::Format("jobs/files/%s_Target%i_%i.root",
						       current ? "current":"upgrade",
						       target, 2),
				       0, cut1, cut2,
				       var, var_title,
				       nbins, xbins);
  TH1F* h_eff_ECal = computeEfficiency(TString::Format("jobs/files/%s_Target%i_%i.root",
						       current ? "current":"upgrade",
						       target, 3),
				       0, cut1, cut2,
				       var, var_title,
				       nbins, xbins);
  

  TH1F* h_eff_total = (TH1F*)h_eff_Fwd->Clone("h_eff_total");
  h_eff_total->Add(h_eff_Bwd);
  h_eff_total->Add(h_eff_HA);
  h_eff_total->Add(h_eff_ECal);

  TCanvas *c;
  c = new TCanvas("c", "c");

  h_eff_Fwd->SetMinimum(0.);
  h_eff_Fwd->SetMaximum(1.);
  h_eff_Fwd->SetLineColor(kBlue);
  h_eff_Bwd->SetLineColor(kRed);
  h_eff_HA->SetLineColor(kViolet);
  h_eff_ECal->SetLineColor(kGreen+2);
  h_eff_total->SetLineColor(kBlack);

  h_eff_Fwd->Draw("e");
  h_eff_Bwd->Draw("esame");
  h_eff_HA->Draw("esame");
  h_eff_ECal->Draw("esame");
  h_eff_total->Draw("esame");

  TLegend *leg;
  if (left) leg = new TLegend(0.1, 0.65, 0.3, 0.9);
  else      leg = new TLegend(0.7, 0.65, 0.9, 0.9);

  leg->AddEntry(h_eff_Fwd,  "Fwd TPC", "elp");
  leg->AddEntry(h_eff_Bwd,  "Bwd TPC", "elp");
  leg->AddEntry(h_eff_HA,   "HA TPC", "elp");
  leg->AddEntry(h_eff_ECal, "ECal", "elp");
  leg->AddEntry(h_eff_total,"Total", "elp");
  leg->Draw("same");

  c->Update();
  c->SaveAs(TString::Format("fig/eff_%s_sep_Target%i_%s.eps", 
			    current ? "current":"upgrade", target, var.Data()));


  // draw stacked histogram

  TCanvas *c2;
  c2 = new TCanvas("c2", "c2");
  
  h_eff_Fwd->SetFillStyle(3001);
  h_eff_Bwd->SetFillStyle(3001);
  h_eff_HA->SetFillStyle(3001);
  h_eff_ECal->SetFillStyle(3001);

  h_eff_Fwd->SetFillColor(kBlue);
  h_eff_Bwd->SetFillColor(kRed);
  h_eff_HA->SetFillColor(kViolet);
  h_eff_ECal->SetFillColor(kGreen+2);

  THStack* h_stack = new THStack("hs","");
  h_stack->Add(h_eff_ECal,"hist");
  h_stack->Add(h_eff_HA,"hist");
  h_stack->Add(h_eff_Bwd,"hist");
  h_stack->Add(h_eff_Fwd,"hist");

  h_stack->Draw("");
  h_stack->GetXaxis()->SetTitle(var_title);
  h_stack->GetXaxis()->SetTitleSize(0.05);
  h_stack->GetXaxis()->SetTitleOffset(0.9);
  h_stack->GetYaxis()->SetTitleSize(0.05);
  h_stack->GetYaxis()->SetTitle("efficiency");

  TLegend *leg2;
  if (left) leg2 = new TLegend(0.1, 0.65, 0.3, 0.9);
  else      leg2 = new TLegend(0.7, 0.65, 0.9, 0.9);

  leg2->AddEntry(h_eff_Fwd,  "Fwd TPC", "f");
  leg2->AddEntry(h_eff_Bwd,  "Bwd TPC", "f");
  leg2->AddEntry(h_eff_HA,   "HA TPC", "f");
  leg2->AddEntry(h_eff_ECal, "ECal", "f");
  leg2->Draw("same");

  c2->Update();
  c2->SaveAs(TString::Format("fig/eff_%s_stacked_Target%i_%s.png", 
			     current ? "current":"upgrade", target, var.Data()));
}


void plotAll() {

  const int NBins_CosTh = 20;
  double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,
					  0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

  plotEfficiency(0, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh, 1, false, true);
  plotEfficiency(0, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh, 2, false, true);
  plotEfficiency(0, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh, 1, true, true);
  plotEfficiency(0, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh, 2, true, true);

  const int NBins_Mom = 29;
  double BinEdges_Mom[NBins_Mom+1] = {0,50,100,150,200,250,300,350,400,450,500,550,
				      600,650,700,750,800,850,900,950,1000,1200,
				      1400,1600,1800,2000,2500,3000,4000,5000};

  plotEfficiency(0, 4, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom, 1, false, false);
  plotEfficiency(0, 4, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom, 2, false, false);
  plotEfficiency(0, 4, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom, 1, true, false);
  plotEfficiency(0, 4, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom, 2, true, false);

}
