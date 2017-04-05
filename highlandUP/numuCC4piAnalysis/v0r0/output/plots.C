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

using namespace std;

int NTargets[6] = {2, 2, 3, 3, 3, 3};
TString topoName[4] = {"CC-0#pi", "CC-1#pi^{+}", "CC-other", "BKG"};
TString confName[6] = {"ND280 current", 
		       "ND280 upgrade ref.", 
		       "ND280 upgrade alt. Target(60cm)->TPC",
		       "ND280 upgrade alt. TPC->Target(60cm)",		       
		       "ND280 upgrade alt. Target(40cm)->TPC",
		       "ND280 upgrade alt. TPC->Target(40cm)"};

double sizeXYZ[6][3][3] = {{{1864.3, 1864.3,  303.0}, {1864.3, 1864.3,  303.0}, {     0,      0,      0}},
			   {{1864.0,  600.0, 1300.0}, {1864.0,  600.0, 1300.0}, {     0,      0,      0}},
			   {{1864.3, 1864.3,  303.0}, {1864.3, 1864.3,  303.0}, {1864.0,  600.0, 1994.0}},
			   {{1864.3, 1864.3,  303.0}, {1864.3, 1864.3,  303.0}, {1864.0,  600.0, 1994.0}},
			   {{1864.3, 1864.3,  303.0}, {1864.3, 1864.3,  303.0}, {1864.0,  400.0, 1994.0}},
			   {{1864.3, 1864.3,  303.0}, {1864.3, 1864.3,  303.0}, {1864.0,  400.0, 1994.0}}}; // mm

double density[6][3] = {{1.032, 1.027,      0},
			{1.000, 0.3096,     0},
			{1.032, 1.027,  1.000},
			{1.032, 1.027,  1.000},
			{1.032, 1.027,  1.000},
			{1.032, 1.027,  1.000}}; // g/cm3

double massfraction_C_O[6][3][2] = {{{92.2577,       0}, {50.1400, 40.7400}, {      0,       0}},
				    {{      0, 88.8102}, {91.4708,       0}, {      0,       0}},
				    {{92.2577,       0}, {50.1400, 40.7400}, {      0, 88.8102}},
				    {{92.2577,       0}, {50.1400, 40.7400}, {      0, 88.8102}},
				    {{92.2577,       0}, {50.1400, 40.7400}, {      0, 88.8102}},
				    {{92.2577,       0}, {50.1400, 40.7400}, {      0, 88.8102}}};

const double N_A = 6.0222e23; // /mol
const double M_O = 15.9994; // g/mol
const double M_C = 12.0107; // g/mol

int cut_beforeTopo = 4;

double normalization(int config) {

  return 1;

}

void printSummary() {

TString confName_loc[6] = {"current", 
			   "ref.", 
			   "alt. Target(60cm)$\\to$TPC",
			   "alt. TPC$\\to$Target(60cm)",			   
			   "alt. Target(40cm)$\\to$TPC",
			   "alt. TPC$\\to$Target(40cm)"};

 for (int ic=0; ic<4; ic++) {
   cout << confName_loc[ic];
    for (int it=1; it<=NTargets[ic]; it++) {
      TFile* f = TFile::Open(TString::Format("jobs/files/config%i_Target%i.root", ic, it));
      TTree* t = (TTree*)f->Get("truth");
      
      double N = 0;
      for (int ib=0; ib<4; ib++)
	N += t->GetEntries(TString::Format("accum_level[%i]>4", ib))/normalization(ic);
      
      cout << " & " << N;
    }
    cout << "\\\\" << endl;
  }

}

TH1F* computeDistribution(int config, int target, int branch, int cut, 
			  TString var, TString var_title, TString unit,
			  int nbins, double* xbins) {
 
  
  TFile* f = TFile::Open(TString::Format("jobs/files/config%i_Target%i.root",
			  config, target));
  TTree* t = (TTree*)f->Get("truth");
  TH1::AddDirectory(kFALSE);
  
  TH1F* h = new TH1F("h", "h", nbins, xbins);
  
  Int_t accum_level[16];
  Float_t variable;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(var,           &variable);

  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
    t->GetEntry(ient);
    
    if (accum_level[branch]>cut)
      h->Fill(variable);

  }

  h->Sumw2();

  double w_min = 0xABCDEF, w_max=0;
  bool variable_width = false;

  for (int i = 0; i < nbins-1; i++){
    double w = xbins[i+1]-xbins[i];
    if (w<w_min) 
      w_min = w;
    if (w>w_max)
      w_max = w;
  }

  if (w_max-w_min > 1e-3) {
    variable_width = true;
    for (int i = 0; i < h->GetNbinsX(); i++){
      double w = h->GetXaxis()->GetBinWidth(i+1);
      h->SetBinContent(i+1, h->GetBinContent(i+1)*w_min/w);
      h->SetBinError(i+1,   h->GetBinError(i+1)*w_min/w);
    }
  }

  h->GetXaxis()->SetTitleSize(0.05);
  h->GetXaxis()->SetTitleOffset(0.9);

  if (unit.Length() !=0 )
    h->GetXaxis()->SetTitle(var_title+" ["+unit+"]");
  else
    h->GetXaxis()->SetTitle(var_title);

  if (!variable_width) 
    h->GetYaxis()->SetTitle("entries (/10^{20} POT)");
  else if (unit.Length()==0)
    h->GetYaxis()->SetTitle(TString::Format("entries (/%1f /10^{20} POT)", w_min));
  else
    h->GetYaxis()->SetTitle(TString::Format("entries (/%1f %s /10^{20} POT)", w_min, unit.Data()));

  h->GetYaxis()->SetTitleSize(0.05);
  
  f->Close();
  delete f;
  return h;
  

}

TH1F* computeDistributionTopo(int config, int target, int branch, int topology, int cut, 
			      TString var, TString var_title, TString unit,
			      int nbins, double* xbins) {
  
  TFile* f = TFile::Open(TString::Format("jobs/files/config%i_Target%i.root",
			  config, target), "READ");
  TTree* t = (TTree*)f->Get("truth");;
  TH1::AddDirectory(kFALSE);

  TH1F* h = new TH1F("h", "h", nbins, xbins);
  
  Int_t accum_level[16];
  Float_t variable;
  Int_t topo;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(var,           &variable);
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress("topology",    &topo);

  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
    t->GetEntry(ient);
    if (accum_level[branch]>cut && topo==topology)
      h->Fill(variable);
  }

  h->Sumw2();
  h->Scale(1/normalization(config));

  double w_min = 0xABCDEF, w_max=0;
  bool variable_width = false;

  for (int i = 0; i < nbins-1; i++){
    double w = xbins[i+1]-xbins[i];
    if (w<w_min) 
      w_min = w;
    if (w>w_max)
      w_max = w;
  }

  if (w_max-w_min > 1e-3) {
    variable_width = true;
    for (int i = 0; i < h->GetNbinsX(); i++){
      double w = h->GetXaxis()->GetBinWidth(i+1);
      h->SetBinContent(i+1, h->GetBinContent(i+1)*w_min/w);
      h->SetBinError(i+1,   h->GetBinError(i+1)*w_min/w);
    }
  }

  h->GetXaxis()->SetTitleSize(0.05);
  h->GetXaxis()->SetTitleOffset(0.9);

  if (unit.Length() !=0 )
    h->GetXaxis()->SetTitle(var_title+" ["+unit+"]");
  else
    h->GetXaxis()->SetTitle(var_title);

  if (!variable_width) 
    h->GetYaxis()->SetTitle("entries (/10^{20} POT)");
  else if (unit.Length()==0)
    h->GetYaxis()->SetTitle(TString::Format("entries (/%i /10^{20} POT)", (int)w_min));
  else
    h->GetYaxis()->SetTitle(TString::Format("entries (/%i %s /10^{20} POT)", (int)w_min, unit.Data()));

  h->GetYaxis()->SetTitleSize(0.05);
  
  f->Close();
  delete f;
  return h;

}

void plotDistribution(int config, int target, int topology, int cut, 
		      TString var, TString var_title, TString unit, 
		      int nbins, double* xbins, bool log=false) {


  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleAlign(23);

  TH1F* h_Fwd  = computeDistribution(config, target, 4*topology+0, cut, var, var_title, unit, nbins, xbins);
  TH1F* h_Bwd  = computeDistribution(config, target, 4*topology+1, cut, var, var_title, unit, nbins, xbins);
  TH1F* h_HA   = computeDistribution(config, target, 4*topology+2, cut, var, var_title, unit, nbins, xbins);
  TH1F* h_ECal = computeDistribution(config, target, 4*topology+3, cut, var, var_title, unit, nbins, xbins);

  TCanvas *c;
  c = new TCanvas("c", "c");
  
  h_Fwd->SetFillStyle(3001);
  h_Bwd->SetFillStyle(3001);
  h_HA->SetFillStyle(3001);
  h_ECal->SetFillStyle(3001);

  h_Fwd->SetFillColor(kBlue);
  h_Bwd->SetFillColor(kRed);
  h_HA->SetFillColor(kViolet);
  h_ECal->SetFillColor(kGreen+2);

  THStack* h_stack = new THStack("hs","");
  h_stack->Add(h_ECal,"hist");
  h_stack->Add(h_HA,  "hist");
  h_stack->Add(h_Bwd, "hist");
  h_stack->Add(h_Fwd, "hist");

  h_stack->Draw("");
  h_stack->GetXaxis()->SetTitle(h_Fwd->GetXaxis()->GetTitle());
  h_stack->GetYaxis()->SetTitle(h_Fwd->GetYaxis()->GetTitle());
  h_stack->GetXaxis()->SetTitleSize(0.05);
  h_stack->GetXaxis()->SetTitleOffset(0.9);
  h_stack->GetYaxis()->SetTitleSize(0.05);

  if (cut <= cut_beforeTopo)
    h_stack->SetTitle(TString::Format("%s, %s %i", 
				      confName[config].Data(), 
				      (config!=1 && target!=3) ? "FGD":"Target",
				      (target!=3) ? target : 1));
  else
    h_stack->SetTitle(TString::Format("%s, %s %i, %s", 
				      confName[config].Data(), 
				      (config!=1 && target!=3) ? "FGD":"Target",
				      (target!=3) ? target : 1, topoName[topology].Data()));
  
  if (log) {
    h_stack->SetMinimum(0.1);
    c->SetLogy(1);
  }

  TLegend *leg = new TLegend(0.1, 0.89, 0.9, 0.94);
  leg->SetNColumns(5);

  leg->AddEntry(h_Fwd,  "Fwd TPC", "f");
  leg->AddEntry(h_Bwd,  "Bwd TPC", "f");
  leg->AddEntry(h_HA,   "HA TPC", "f");
  leg->AddEntry(h_ECal, "ECal", "f");
  leg->Draw("same");

  c->Update();

  if (cut <= cut_beforeTopo)
    c->SaveAs(TString::Format("fig/dis/branch/dis_config%i_stacked_Target%i_%s.png", 
			      config, target, var.Data()));
  else
    c->SaveAs(TString::Format("fig/dis/branch/dis_config%i_stacked_Target%i_topo%i_%s.png", 
			    config, target, topology, var.Data()));

}


void plotDistributionTopo(int config, int target, int topology, int cut, 
			  TString var, TString var_title, TString unit, 
			  int nbins, double* xbins,  bool log=false) {
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleAlign(23);

  TH1F* h[4];
  int color[4] = {2,3,4,7};

  for (int i=0; i<4; i++) {
    h[i] = computeDistributionTopo(config, target, 4*topology+i, i, cut, var, var_title, unit, nbins, xbins);
    h[i]->SetFillStyle(3001);
    h[i]->SetFillColor(color[i]);
  }

  TCanvas *c;
  c = new TCanvas("c", "c");
  
  THStack* h_stack = new THStack("hs","");
  TLegend *leg = new TLegend(0.1, 0.89, 0.9, 0.94);
  leg->SetNColumns(5);

  for (int i=3; i>=0; i--) {
    h_stack->Add(h[i],"hist");
    leg->AddEntry(h[i], topoName[i], "f");
  }

  h_stack->Draw("");
  h_stack->GetXaxis()->SetTitle(h[0]->GetXaxis()->GetTitle());
  h_stack->GetYaxis()->SetTitle(h[0]->GetYaxis()->GetTitle());
  h_stack->GetXaxis()->SetTitleSize(0.05);
  h_stack->GetXaxis()->SetTitleOffset(0.9);
  h_stack->GetYaxis()->SetTitleSize(0.05);
  if (cut <= cut_beforeTopo)
  h_stack->SetTitle(TString::Format("%s, %s %i", 
		    confName[config].Data(), 
		    (config!=1 && target!=3) ? "FGD":"Target",
				    (target!=3) ? target : 1));
  else
  h_stack->SetTitle(TString::Format("%s, %s %i, %s", 
		    confName[config].Data(), 
		    (config!=1 && target!=3) ? "FGD":"Target",
				    (target!=3) ? target : 1, topoName[topology].Data()));

  if (log) {
    h_stack->SetMinimum(0.1);
    c->SetLogy(1);
  }
  
  c->Update();

  if (cut <= cut_beforeTopo)
  c->SaveAs(TString::Format("fig/dis/topo/dis_config%i_stackedTopo_Target%i_%s.png", 
			    config, target, var.Data()));
  else
 c->SaveAs(TString::Format("fig/dis/topo/dis_config%i_stackedTopo_Target%i_topo%i_%s.png", 
			   config, target, topology, var.Data()));

}


void plotAllDis() {

  const int NBins_CosTh = 20;
  double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,
					  0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

  const int NBins_Mom = 29;
  double BinEdges_Mom[NBins_Mom+1] = {0,50,100,150,200,250,300,350,400,450,500,550,
				      600,650,700,750,800,850,900,950,1000,1200,
				      1400,1600,1800,2000,2500,3000,4000,5000};

  const int NBins_Q2 = 15;
  double BinEdges_Q2[NBins_Q2+1] = {0, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1, 1.5, 2, 3, 5};

  for (int ic=0; ic<4; ic++) {
    for (int it=1; it<=NTargets[ic]; it++) {
		/*
	plotDistribution(ic, it, 0, 4, "true_costheta", "true cos #theta", "", NBins_CosTh, BinEdges_CosTh, true);
	plotDistribution(ic, it, 0, 4, "true_mom", "true p_{#mu}", "MeV/c", NBins_Mom, BinEdges_Mom, false);
	plotDistribution(ic, it, 0, 4,  "true_Q2", "true Q^{2}", "GeV^{2}/c^{2}", NBins_Q2, BinEdges_Q2, false);
	*/
	plotDistributionTopo(ic, it, 0, 4, "true_costheta", "true cos #theta", "", NBins_CosTh, BinEdges_CosTh, true);
	plotDistributionTopo(ic, it, 0, 4, "true_mom", "true p_{#mu}", "MeV/c", NBins_Mom, BinEdges_Mom, false);
	plotDistributionTopo(ic, it, 0, 4, "true_Q2", "true Q^{2}", "GeV^{2}/c^{2}", NBins_Q2, BinEdges_Q2, false);

	for (int topo=0; topo<3; topo++) {
		/*
	  plotDistribution(ic, it, topo, 5, "true_costheta", "true cos #theta", "", NBins_CosTh, BinEdges_CosTh, true);
	  plotDistribution(ic, it, topo, 5, "true_mom", "true p_{#mu}", "MeV/c", NBins_Mom, BinEdges_Mom, false);
	  plotDistribution(ic, it, topo, 5,  "true_Q2", "true Q^{2}", "GeV^{2}/c^{2}", NBins_Q2, BinEdges_Q2, false);
	  */
	  plotDistributionTopo(ic, it, topo, 5, "true_costheta", "true cos #theta", "", NBins_CosTh, BinEdges_CosTh, true);
	  plotDistributionTopo(ic, it, topo, 5, "true_mom", "true p_{#mu}", "MeV/c", NBins_Mom, BinEdges_Mom, false);
	  plotDistributionTopo(ic, it, topo, 5, "true_Q2", "true Q^{2}", "GeV^{2}/c^{2}", NBins_Q2, BinEdges_Q2, false);
	}
    }
  }

}
