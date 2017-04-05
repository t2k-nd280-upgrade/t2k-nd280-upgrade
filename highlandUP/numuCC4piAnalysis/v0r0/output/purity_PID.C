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

TString confName[6] = {"ND280 current", 
		       "ND280 upgrade ref.", 
		       "ND280 upgrade alt. Target(60cm)->TPC",
		       "ND280 upgrade alt. TPC->Target(60cm)",		       
		       "ND280 upgrade alt. Target(40cm)->TPC",
		       "ND280 upgrade alt. TPC->Target(40cm)"};

TString modeName[4] = {"FwdTPC", "BwdTPC", "HATPC", "ECal"};

int NTargets[6] = {2, 2, 3, 3, 3, 3};

void computeEffPurVSCut(int target, int config, int branch, int cut_level, 
			TString var, TString var_title, 
			int ncuts=100, float cut_min=0, float cut_max=1, bool below=false) {
    
  TFile* f = new TFile(TString::Format("jobs/files/config%i_Target%i.root",
				       config, target));
  TTree* t = (TTree*)f->Get("default");

  float* cut = new float[ncuts];
  float* eff = new float[ncuts];
  float* pur = new float[ncuts];
  float* nTrueSel = new float[ncuts];
  float* nTrue    = new float[ncuts];
  float* nSel     = new float[ncuts];

  for (int i=0; i<ncuts; i++) {
    cut[i] = cut_min + (cut_max-cut_min)*(float)i/(ncuts);
    eff[i] = pur[i] = 0;
    nTrueSel[i] = nTrue[i] = nSel[i] = 0;
  }

  Int_t accum_level[1][12];
  Float_t variable;
  int reaction, particle;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(var,           &variable);
  t->SetBranchAddress("reaction",    &reaction);
  t->SetBranchAddress("particle",    &particle);

  for (int i=0; i<ncuts; i++) {
	  
    for (Int_t ient=0; ient < t->GetEntries(); ient++) {
      t->GetEntry(ient);
    
      bool cutPassed=false;
      if (below)
	cutPassed = (variable<cut[i]);
      else
	cutPassed = (variable>cut[i]);

      if (accum_level[0][branch]>cut_level && cutPassed)
	nSel[i]++;
      if (accum_level[0][branch]>cut_level && reaction>=0 && reaction<=3)
	nTrue[i]++;
      if (accum_level[0][branch]>cut_level && cutPassed && reaction>=0 && reaction<=3)
	nTrueSel[i]++;
    }
	
  }


  for (int i=0; i<ncuts; i++) {
    if (nTrue[i]==0) eff[i] = 1;
    else             eff[i] = (float)nTrueSel[i]/(float)nTrue[i];
    if (nSel[i]==0)  pur[i] = 0;
    else             pur[i] = (float)nTrueSel[i]/(float)nSel[i];
	  
    cout << nSel[i] << " " << nTrueSel[i] << " " << nTrue[i] << endl;
    cout << eff[i] << " " << pur[i] << endl;
  }
  
  TCanvas* c = new TCanvas("c");
  TGraph* gE = new TGraph(ncuts, cut, eff);
  TGraph* gP = new TGraph(ncuts, cut, pur);

  gStyle->SetOptTitle(1);
  gStyle->SetTitleAlign(23);

  gE->SetTitle(TString::Format("%s, %s %i, %s", 
			       confName[config].Data(), 
			       (config!=1 && target!=3) ? "FGD":"Target",
			       (target!=3) ? target : 1,
			       modeName[branch].Data()));
  gE->GetXaxis()->SetTitle(TString::Format("cut on %s", var_title.Data()));
  gE->GetXaxis()->SetTitleSize(0.05);
  gE->GetXaxis()->SetTitleOffset(0.9);
  gE->GetYaxis()->SetTitleSize(0.05);

  gE->Draw("AL");

  gP->SetLineColor(kRed);
  gP->Draw("L same");

  TLegend *leg = new TLegend(0.1, 0.9, 0.9, 0.94);
  leg->SetNColumns(2);
  leg->AddEntry(gE, "efficiency", "l");
  leg->AddEntry(gP, "purity", "l");
  leg->Draw("same");

  c->SaveAs(TString::Format("fig/lkl/purOpt/opt_%s_config%i_Target%i_%s.eps", 
			    var.Data(), config, target, modeName[branch].Data()));

}


void plotEffPurVSCut() {

  for (int ic=0; ic<6; ic++)
    for (int it=1; it<=NTargets[ic]; it++) {
      for (int im=0; im<3; im++) 
	computeEffPurVSCut(it, ic, im, 3, "selmu_likemu", "L_{#mu}", 100, 0, 1);
      
      computeEffPurVSCut(it, ic, 3, 3, "selmu_ecal_mipem", "MipEM", 100, -40, 40, true);
      computeEffPurVSCut(it, ic, 3, 3, "selmu_ecal_EneOnL", "E/L", 100, 0, 2, true);
    }
}
