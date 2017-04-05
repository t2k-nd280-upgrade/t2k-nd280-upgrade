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
#include "TH2F.h"
#include "THStack.h"
#include "TChain.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TString.h"
#include "TLatex.h"

using namespace std;

int NTargets[6] = {2, 2, 3, 3, 3, 3};
TString topoName[4] = {"CC-0#pi", "CC-1#pi^{+}", "CC-other", "CC-inclusive"};
TString confName[6] = {"ND280 current-like", 
		       "ND280 upgrade-like ref.", 
		       "ND280 upgrade-like alt. Target(60cm)->TPC",
		       "ND280 upgrade-like alt. TPC->Target(60cm)",		       
		       "ND280 upgrade-like alt. Target(40cm)->TPC",
		       "ND280 upgrade-like alt. TPC->Target(40cm)"};

int cut_beforeTopo = 4;

TH1F* computeEfficiency(int config, int target, int branch, int cut1, int cut2, 
			TString var, TString var_title, 
			int nbins, double* xbins, TString suffix="") {
  
  TFile f(TString::Format("jobs/files/config%i_Target%i%s.root",
			  config, target, suffix.Data()));
  TTree* t = (TTree*)f.Get("truth");
  TH1::AddDirectory(kFALSE);

  TH1F* h_beforeCut = new TH1F(TString::Format("before_%s_%i_%i_%i",var.Data(),config,target,branch),
			       TString::Format("before_%s_%i_%i_%i",var.Data(),config,target,branch),
			       nbins, xbins);
  TH1F* h_afterCut = new TH1F(TString::Format("after_%s_%i_%i_%i",var.Data(),config,target,branch),
			      TString::Format("after_%s_%i_%i_%i",var.Data(),config,target,branch),
			      nbins, xbins);
  
  Int_t accum_level[16];
  Float_t variable;
  Int_t topo;
  
  t->SetBranchAddress("accum_level",     &accum_level);
  t->SetBranchAddress(var,               &variable);
  t->SetBranchAddress("topology",        &topo);

  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
    t->GetEntry(ient);
    
    bool topoCut = (branch>=12 || topo==branch/4);
    if (accum_level[branch]>cut1 && topoCut)
      h_beforeCut->Fill(variable);
    if (accum_level[branch]>cut2 && topoCut)
      h_afterCut->Fill(variable);

  }

  h_beforeCut->Sumw2();
  h_afterCut->Sumw2();

  TH1F *h_eff = new TH1F(TString::Format("h_eff_%s_%i_%i_%i",var.Data(),config,target,branch),
			 TString::Format("h_eff_%s_%i_%i_%i",var.Data(),config,target,branch),
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

TH1F* computeEffTotal(int config, int target, int topology, int cut1, int cut2,
		      TString var, TString var_title, 
		      int nbins, double* xbins, TString suffix="") {
  
  TString configName(TString::Format("config%i", config)); 
 
  TH1::AddDirectory(kFALSE);

  TFile f(TString::Format("jobs/files/config%i_Target%i%s.root",
			  config, target, suffix.Data()));
  TTree* t = (TTree*)f.Get("truth");	

  TH1F* h_beforeCut = new TH1F(TString::Format("before_%s_%i_%i",var.Data(),config,target),
			       TString::Format("before_%s_%i_%i",var.Data(),config,target),
			       nbins, xbins);
  TH1F* h_afterCut = new TH1F(TString::Format("after_%s_%i_%i",var.Data(),config,target),
			      TString::Format("after_%s_%i_%i",var.Data(),config,target),
			      nbins, xbins);

  Int_t accum_level[16];
  Float_t variable;
  Int_t topo, reac;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(var,           &variable);
  t->SetBranchAddress("topology",    &topo);
  t->SetBranchAddress("reaction",    &reac);
 
  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
	  
    t->GetEntry(ient);
	
    bool topoCut = (topology==3 || topo==topology);

    if (accum_level[4*topology]>cut1 && topoCut)
      h_beforeCut->Fill(variable);

    bool cut_passed=false;	  
    for (int i=0; i<4; i++)
      cut_passed = (cut_passed || accum_level[4*topology+i]>cut2);
    if (cut_passed && topoCut)
      h_afterCut->Fill(variable);
    
  }


  TH1F *h_eff = new TH1F(TString::Format("h_eff_%s_%i_%i",var.Data(),config,target),
			 TString::Format("h_eff_%s_%i_%i",var.Data(),config,target),
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

void plotEfficiency(int config, int target, int topology, int cut1, int cut2,
		    TString var, TString var_title, 
		    int nbins, double* xbins, TString suffix="") {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleAlign(23);

  TH1F* h_eff_Fwd  = computeEfficiency(config, target, 4*topology,
				       cut1, cut2,
				       var, var_title,
				       nbins, xbins, suffix);
  TH1F* h_eff_Bwd  = computeEfficiency(config, target, 4*topology+1,
				       cut1, cut2,
				       var, var_title,
				       nbins, xbins, suffix);
  TH1F* h_eff_HA   = computeEfficiency(config, target, 4*topology+2,
				       cut1, cut2,
				       var, var_title,
				       nbins, xbins, suffix);
  TH1F* h_eff_ECal = computeEfficiency(config, target, 4*topology+3,
				       cut1, cut2,
				       var, var_title,
				       nbins, xbins, suffix);
  TH1F* h_eff_total = computeEffTotal(config, target, topology,
				      cut1, cut2,
				      var, var_title,
				      nbins, xbins, suffix);

  TCanvas *c;
  c = new TCanvas("c", "c");
  
  h_eff_Fwd->SetTitle(TString::Format("%s, %s %i, %s", 
				      confName[config].Data(), 
				      (config!=1 && target!=3) ? "FGD":"Target",
				      (target!=3) ? target : 1, topoName[topology].Data()));

  h_eff_Fwd->SetMinimum(0.);
  h_eff_Fwd->SetMaximum(1.02);
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

  TLegend *leg = new TLegend(0.1, 0.89, 0.9, 0.94);
  leg->SetNColumns(5);

  leg->AddEntry(h_eff_Fwd,  "Fwd TPC", "elp");
  leg->AddEntry(h_eff_Bwd,  "Bwd TPC", "elp");
  leg->AddEntry(h_eff_HA,   "HA TPC", "elp");
  leg->AddEntry(h_eff_ECal, "ECal", "elp");
  leg->AddEntry(h_eff_total,"Total", "elp");
  leg->Draw("same");

  c->Update();
  c->SaveAs(TString::Format("fig/eff/eff_config%i_sep_Target%i_%s_topo%i.eps", 
			    config, target, var.Data(), topology));

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

  h_stack->SetMinimum(0.);
  h_stack->SetMaximum(1.02);
  h_stack->Draw("");
  h_stack->GetXaxis()->SetTitle(var_title);
  h_stack->GetXaxis()->SetTitleSize(0.05);
  h_stack->GetXaxis()->SetTitleOffset(0.9);
  h_stack->GetYaxis()->SetTitleSize(0.05);
  h_stack->GetYaxis()->SetTitle("efficiency");
  
  h_stack->SetTitle(TString::Format("%s, %s %i, %s", 
				    confName[config].Data(), 
				    (config!=1 && target!=3) ? "FGD":"Target",
				      (target!=3) ? target : 1, topoName[topology].Data()));

  TLegend *leg2 = new TLegend(0.1, 0.89, 0.9, 0.94);
  leg2->SetNColumns(5);

  leg2->AddEntry(h_eff_Fwd,  "Fwd TPC", "f");
  leg2->AddEntry(h_eff_Bwd,  "Bwd TPC", "f");
  leg2->AddEntry(h_eff_HA,   "HA TPC", "f");
  leg2->AddEntry(h_eff_ECal, "ECal", "f");
  leg2->Draw("same");

  c2->Update();
  c2->SaveAs(TString::Format("fig/eff/eff_config%i_stacked_Target%i_%s_topo%i.png", 
			     config, target, var.Data(), topology));
 
}


void plotTotal(int topology, int cut1, int cut2, 
	       TString var, TString var_title, 
	       int nbins, double* xbins, TString suffix="") {

  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  vector<TH1F*> histos;
  
  TCanvas *c = new TCanvas("c", "c");
  TH1F* h_dummy = c->DrawFrame(xbins[0], 0, xbins[nbins], 1.02);
  h_dummy->GetXaxis()->SetTitle(var_title);
  h_dummy->GetXaxis()->SetTitleSize(0.05);
  h_dummy->GetXaxis()->SetTitleOffset(0.9);
  h_dummy->GetYaxis()->SetTitleSize(0.05);
  h_dummy->GetYaxis()->SetTitle("efficiency");

  TLegend *leg = new TLegend(0.1, 0.89, 0.9, 0.97);
  leg->SetNColumns(3);

  int col=0;
  int color[7]={1,2,3,4,6,7,35};
  for (int ic=0; ic<2; ic++) {
	  
    TString configName;
    if (ic == 0) configName = "current";
    if (ic == 1) configName = "upgrade ref.";	
    for (int it=1; it<=NTargets[ic]; it++) {
		
      TH1F* h = computeEffTotal(ic, it, topology, cut1, cut2, var, var_title, nbins, xbins, suffix);
      h->SetLineColor(color[col++]);
      h->Draw("esame");
      leg->AddEntry(h, TString::Format("%s, %s %i", configName.Data(), (ic!=1 && it<=2) ? "FGD":"Target", it<=2 ? it:1), "elp");

      histos.push_back(h);
    }
  }

  TH1F* hAlt1 = computeEffTotal(2, 3, topology, cut1, cut2, var, var_title, nbins, xbins, suffix);
  hAlt1->SetLineColor(color[col++]);
  hAlt1->Draw("esame");
  leg->AddEntry(hAlt1, "upgrade alt. (Target->TPC), Target", "elp");
  histos.push_back(hAlt1);
  
  TH1F* hAlt2 = computeEffTotal(3, 3, topology, cut1, cut2, var, var_title, nbins, xbins, suffix);
  hAlt2->SetLineColor(color[col++]);
  hAlt2->Draw("esame");
  leg->AddEntry(hAlt2, "upgrade alt. (TPC->Target), Target", "elp");
  histos.push_back(hAlt2);
  
  leg->Draw("same");
  c->Update();
  c->SaveAs(TString::Format("fig/eff/eff_total_%s_topo%i.eps", var.Data(), topology));

  TFile *fW = new TFile(TString::Format("root/total_%s.root", var.Data()), "RECREATE");
  for (unsigned int i=0; i<histos.size(); i++)
    histos[i]->Write();
  c->Write();
  
  //====================

}


void plot2D(int config, int target, int topology, int cut1, int cut2, 
	    TString varX, TString var_titleX, TString varY, TString var_titleY, 
	    int nbinsX, double* xbinsX, int nbinsY, double* xbinsY, TString suffix="") {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleAlign(23);
  TH1::AddDirectory(kFALSE);

  TFile f(TString::Format("jobs/files/config%i_Target%i%s.root",
			  config, target, suffix.Data()));
  TTree* t = (TTree*)f.Get("truth");

  TH2F* h1 = new TH2F("h1", "h2", nbinsX, xbinsX, nbinsY, xbinsY);
  TH2F* h2 = new TH2F("h1", "h2", nbinsX, xbinsX, nbinsY, xbinsY);

  Int_t accum_level[16];
  Float_t variableX, variableY;
  Int_t nPosPi, nOthPi;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(varX,          &variableX);
  t->SetBranchAddress(varY,          &variableY);

  for (Int_t ient=0; ient < t->GetEntries(); ient++) {

    t->GetEntry(ient);

    if (accum_level[4*topology]>cut1)
      h1->Fill(variableX, variableY);

    bool cut_passed=false;	  
    for (int i=0; i<4; i++)
      cut_passed = (cut_passed || accum_level[4*topology+i]>cut2);
    if (cut_passed)
      h2->Fill(variableX, variableY);

  }

 
  TH2F *h_eff = new TH2F("h_eff", "h_eff",
			 nbinsX, xbinsX, nbinsY, xbinsY);
  h_eff->Divide(h2, h1);
  
  TCanvas *c = new TCanvas("c", "c");
  h_eff->SetTitle(TString::Format("%s, %s %i, %s", 
				  confName[config].Data(), 
				  (config!=1 && target!=3) ? "FGD":"Target",
				  (target!=3) ? target : 1, topoName[topology].Data()));

  h_eff->GetXaxis()->SetTitle(var_titleX);
  h_eff->GetXaxis()->SetTitleSize(0.05);
  h_eff->GetXaxis()->SetTitleOffset(0.9);
  h_eff->GetYaxis()->SetTitle(var_titleY);
  h_eff->GetYaxis()->SetTitleSize(0.05);
  h_eff->GetYaxis()->SetTitleOffset(0.9);
  h_eff->GetZaxis()->SetRangeUser(0.,1.);
  
  h_eff->Draw("colz");
  c->Update();
  c->SaveAs(TString::Format("fig/eff/2D/eff_config%i_Target%i_total_%s_%s_topo%i.eps", 
			    config, target, varX.Data(), varY.Data(), topology));


}


void plotAll(bool applyToF=false, TString suffix="") {

  const int NBins_CosTh = 20;
  double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

  const int NBins_Mom = 29;
  double BinEdges_Mom[NBins_Mom+1] = {0,50,100,150,200,250,300,350,400,450,500,550, 600,650,700,750,800,850,900,950,1000,1200, 1400,1600,1800,2000,2500,3000,4000,5000};

  const int NBins_Q2 = 15;
  double BinEdges_Q2[NBins_Q2+1] = {0, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1, 1.5, 2, 3, 5};

   const int NBins_Enu = 14;
  double BinEdges_Enu[NBins_Enu+1] = {0, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1200, 1500, 2000, 3000, 5000};
 


  int ToF_applied = applyToF ? 1:0;
  
  for (int ic=0; ic<4; ic++) {
    for (int it=1; it<=NTargets[ic]; it++) {
      for (int topo=3; topo<4; topo++) {
	plotEfficiency(ic, it, topo, -1, (topo==3 ? 4:5) + ToF_applied,
		       "true_costheta", "true cos #theta",
		       NBins_CosTh, BinEdges_CosTh, suffix);
	plotEfficiency(ic, it, topo, -1, (topo==3 ? 4:5) + ToF_applied,
		       "true_mom", "true p_{#mu} [MeV/c]",
		       NBins_Mom, BinEdges_Mom, suffix);
	plotEfficiency(ic, it, topo, -1, (topo==3 ? 4:5) + ToF_applied,
		       "true_Q2", "true Q^{2} [GeV^{2}/c^{2}]",
		       NBins_Q2, BinEdges_Q2, suffix);
	plot2D(ic, it, topo, -1, (topo==3 ? 4:5) + ToF_applied,
	       "true_costheta", "true cos #theta", "true_mom", "true p_{#mu} [MeV/c]",
	       NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom, suffix);
	plot2D(ic, it, topo, -1, (topo==3 ? 4:5) + ToF_applied,
	       "true_Nu_mom", "true neutrino energy [MeV/c]", "true_Q2", "true Q^{2} [GeV^{2}/c^{2}]",
	       NBins_Enu, BinEdges_Enu, NBins_Q2, BinEdges_Q2, suffix);
      }
    }
  }
  
  
 
  for (int topo=0; topo<4; topo++) {
    plotTotal(topo, -1, (topo==3 ? 4:5) + ToF_applied, "true_costheta", "true cos #theta",
	      NBins_CosTh, BinEdges_CosTh, suffix);
    plotTotal(topo, -1, (topo==3 ? 4:5) + ToF_applied, "true_mom", "true p_{#mu} [MeV/c]",
	      NBins_Mom, BinEdges_Mom, suffix);
    plotTotal(topo, -1, (topo==3 ? 4:5) + ToF_applied, "true_Q2", "true Q^{2} [GeV^{2}/c^{2}]",
	      NBins_Q2, BinEdges_Q2, suffix);
  }

}
