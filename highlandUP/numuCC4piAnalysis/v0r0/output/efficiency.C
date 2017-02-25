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

float Z_bins[2][2][4] = { {{-1787, -1354, -920, -487}, { 487,  920, 1354, 1787}}, 
			 {{  276,   377,  478,  579}, {1554, 1664, 1775, 1885}} };


TH1F* computeEfficiency(int target, bool current, int mode, int branch, int cut1, int cut2, 
			TString var, TString var_title, 
			int nbins, double* xbins, int Z_bin=-1) {
  
  TFile* f = new TFile(TString::Format("jobs/files/%s_Target%i_%i.root",
				       current ? "current":"upgrade",
				       target, mode));
  TTree* t = (TTree*)f->Get("truth");
  
  TH1F* h_beforeCut = new TH1F(TString::Format("before_%s_%i",var.Data(),branch),
			       TString::Format("before_%s_%i",var.Data(),branch),
			       nbins, xbins);
  TH1F* h_afterCut = new TH1F(TString::Format("after_%s_%i",var.Data(),branch),
			      TString::Format("after_%s_%i",var.Data(),branch),
			      nbins, xbins);
  


  Int_t accum_level[1][1];
  Float_t variable, pos[4];
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(var,           &variable);
  t->SetBranchAddress("true_vertex_position", &pos);

  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
    t->GetEntry(ient);
    
    if (accum_level[0][branch]>cut1 && 
	(Z_bin==-1 || (pos[2]>Z_bins[current][target-1][Z_bin] && pos[2]<Z_bins[current][target-1][Z_bin+1])))
      h_beforeCut->Fill(variable);
    if (accum_level[0][branch]>cut2 && 
	(Z_bin==-1 || (pos[2]>Z_bins[current][target-1][Z_bin] && pos[2]<Z_bins[current][target-1][Z_bin+1])))
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

TH1F* computeEffTotal(int target, bool current, int branch, int cut1, int cut2, 
		      TString var, TString var_title, 
		      int nbins, double* xbins, bool drawECal=true, int Z_bin=-1) {
  
  // use one file to get the number of events before cuts
  TFile* f0 = new TFile(TString::Format("jobs/files/%s_Target%i_%i.root",
				       current ? "current":"upgrade",
				       target, 0));
  TTree* t0 = (TTree*)f0->Get("truth");

  // merge all branches in one TChain for after cuts
  // CAREFUL: the truth tree contains 4 times the same events but with exclusive selected events
  TChain* t = new TChain("truth");

  int last = drawECal ? 4:3;
  for (int i=0; i<last; i++)
    t->Add( TString::Format("jobs/files/%s_Target%i_%i.root",
			    current ? "current":"upgrade",
			    target, i) );


  TH1F* h_beforeCut = new TH1F(TString::Format("before_%s_%i",var.Data(),branch),
			       TString::Format("before_%s_%i",var.Data(),branch),
			       nbins, xbins);
  TH1F* h_afterCut = new TH1F(TString::Format("after_%s_%i",var.Data(),branch),
			      TString::Format("after_%s_%i",var.Data(),branch),
			      nbins, xbins);

  Int_t accum_level[1][1];
  Float_t variable, pos[4];
  
  t0->SetBranchAddress("accum_level", &accum_level);
  t0->SetBranchAddress(var,           &variable);
  t0->SetBranchAddress("true_vertex_position", &pos);

  for (Int_t ient=0; ient < t0->GetEntries(); ient++) {
    t0->GetEntry(ient);
    if (accum_level[0][branch]>cut1 && 
	(Z_bin==-1 || (pos[2]>Z_bins[current][target-1][Z_bin] && pos[2]<Z_bins[current][target-1][Z_bin+1])))
      h_beforeCut->Fill(variable);
  }
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(var,           &variable);
  t->SetBranchAddress("true_vertex_position", &pos);

  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
    t->GetEntry(ient);
    if (accum_level[0][branch]>cut2 &&
	(Z_bin==-1 || (pos[2]>Z_bins[current][target-1][Z_bin] && pos[2]<Z_bins[current][target-1][Z_bin+1])))
      h_afterCut->Fill(variable);
  }


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
  
  return h_eff;

}

void plotEfficiency(int cut1, int cut2, 
		    TString var, TString var_title, 
		    int nbins, double* xbins, int target, bool current=false, bool drawECal=true, 
		    int Z_bin=-1) {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  TH1F* h_eff_Fwd  = computeEfficiency(target, current, 0,
				       0, cut1, cut2,
				       var, var_title,
				       nbins, xbins,
				       Z_bin);
  TH1F* h_eff_Bwd  = computeEfficiency(target, current, 1,
				       0, cut1, cut2,
				       var, var_title,
				       nbins, xbins,
				       Z_bin);
  TH1F* h_eff_HA   = computeEfficiency(target, current, 2,
				       0, cut1, cut2,
				       var, var_title,
				       nbins, xbins,
				       Z_bin);
  TH1F* h_eff_ECal = computeEfficiency(target, current, 3,
				       0, cut1, cut2,
				       var, var_title,
				       nbins, xbins,
				       Z_bin);
  

  TH1F* h_eff_total = computeEffTotal(target, current,
				      0, cut1, cut2,
				      var, var_title,
				      nbins, xbins, drawECal,
				      Z_bin);

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
  if (drawECal)
    h_eff_total->Draw("esame");

  TLegend *leg = new TLegend(0.1, 0.90, 0.9, 0.96);
  leg->SetNColumns(5);

  leg->AddEntry(h_eff_Fwd,  "Fwd TPC", "elp");
  leg->AddEntry(h_eff_Bwd,  "Bwd TPC", "elp");
  leg->AddEntry(h_eff_HA,   "HA TPC", "elp");
  if (drawECal)
    leg->AddEntry(h_eff_ECal, "ECal", "elp");
  leg->AddEntry(h_eff_total,"Total", "elp");
  leg->Draw("same");

  c->Update();
  if (Z_bin==-1)
    c->SaveAs(TString::Format("fig/eff/eff_%s_sep_Target%i_%s.eps", 
			      current ? "current":"upgrade", target, var.Data()));
  else
    c->SaveAs(TString::Format("fig/eff/binZ/eff_%s_sep_Target%i_%s_Z%i.eps", 
			      current ? "current":"upgrade", target, var.Data(), Z_bin));


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
  if (drawECal)
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

  TLegend *leg2 = new TLegend(0.1, 0.90, 0.9, 0.96);
  leg2->SetNColumns(5);

  leg2->AddEntry(h_eff_Fwd,  "Fwd TPC", "f");
  leg2->AddEntry(h_eff_Bwd,  "Bwd TPC", "f");
  leg2->AddEntry(h_eff_HA,   "HA TPC", "f");
  if (drawECal)
    leg2->AddEntry(h_eff_ECal, "ECal", "f");
  leg2->Draw("same");

  c2->Update();
  if (Z_bin==-1)
    c2->SaveAs(TString::Format("fig/eff/eff_%s_stacked_Target%i_%s.png", 
			       current ? "current":"upgrade", target, var.Data()));
  else
    c2->SaveAs(TString::Format("fig/eff/binZ/eff_%s_stacked_Target%i_%s_Z%i.png", 
			     current ? "current":"upgrade", target, var.Data(), Z_bin));
}


void plotTotal(int cut1, int cut2, 
	       TString var, TString var_title, 
	       int nbins, double* xbins, bool drawECal=true, int Z_bin=-1) {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TCanvas *c;
  c = new TCanvas("c", "c");
  TH1F* h_dummy = c->DrawFrame(xbins[0], 0, xbins[nbins-1], 1);
  h_dummy->GetXaxis()->SetTitle(var_title);
  h_dummy->GetXaxis()->SetTitleSize(0.05);
  h_dummy->GetXaxis()->SetTitleOffset(0.9);
  h_dummy->GetYaxis()->SetTitleSize(0.05);
  h_dummy->GetYaxis()->SetTitle("efficiency");

  TLegend *leg = new TLegend(0.1, 0.90, 0.9, 0.96);
  leg->SetNColumns(4);
  
  int color[4]={3,4,1,2};

  int i=0;
  for (int ic=0; ic<2; ic++) {
    for (int it=1; it<=2; it++) {

      TH1F* h_eff_total = computeEffTotal(it, ic,
					  0, cut1, cut2,
					  var, var_title,
					  nbins, xbins, drawECal,
					  Z_bin);

      h_eff_total->SetLineColor(color[i++]);
      h_eff_total->Draw("esame");

      leg->AddEntry(h_eff_total, TString::Format("ND280 %s %i", ic==1 ? "current, FGD":"upgrade, Target", it), "elp");

    }
  }

  leg->Draw("same");
  if (Z_bin == -1)
    c->SaveAs(TString::Format("fig/eff/eff_total_%s.eps", var.Data()));
  else 
    c->SaveAs(TString::Format("fig/eff/binZ/eff_total_%s_Z%i.eps", var.Data(), Z_bin));
}


void plotAll(bool drawECal=true) {

  const int NBins_CosTh = 20;
  double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,
					  0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

  const int NBins_Mom = 29;
  double BinEdges_Mom[NBins_Mom+1] = {0,50,100,150,200,250,300,350,400,450,500,550,
				      600,650,700,750,800,850,900,950,1000,1200,
				      1400,1600,1800,2000,2500,3000,4000,5000};

  const int NBins_Q2 = 15;
  double BinEdges_Q2[NBins_Q2+1] = {0, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1, 1.5, 2, 3, 5};

  int cut=4;

  for (int ic=0; ic<=1; ic++) {
    for (int it=1; it<=2; it++) {
      plotEfficiency(0, cut, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh, it, ic, drawECal);
      plotEfficiency(0, cut, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom, it, ic, drawECal);
     plotEfficiency(0, cut, "true_Q2", "true Q^{2} [GeV^{2}/c^{2}]", NBins_Q2, BinEdges_Q2, it, ic, drawECal);
    }
  }

  plotTotal(0, cut, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh, drawECal);
  plotTotal(0, cut, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom, drawECal);
  plotTotal(0, cut, "true_Q2", "true Q^{2} [GeV^{2}/c^{2}]", NBins_Q2, BinEdges_Q2, drawECal);


  // binned in Z

  for (int iz=0; iz<3; iz++) {
    for (int ic=0; ic<=1; ic++) {
      for (int it=1; it<=2; it++) {
	plotEfficiency(0, cut, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh, it, ic, drawECal, iz);
	plotEfficiency(0, cut, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom, it, ic, drawECal, iz);
	plotEfficiency(0, cut, "true_Q2", "true Q^{2} [GeV^{2}/c^{2}]", NBins_Q2, BinEdges_Q2, it, ic, drawECal, iz);
      }
    }
    plotTotal(0, cut, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh, drawECal, iz);
    plotTotal(0, cut, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom, drawECal, iz);
    plotTotal(0, cut, "true_Q2", "true Q^{2} [GeV^{2}/c^{2}]", NBins_Q2, BinEdges_Q2, drawECal, iz);
  }
}

