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
#include "TH2F.h"
#include "TChain.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TString.h"
#include "TLatex.h"

using namespace std;

int NTargets[3] = {2, 2, 3};

TString branchNames[8] = {"CC0pi", "CC1pi", "CCoth", "FWD", "BWD", "HA", "Target", "ECal"};

const int NBins_CosTh = 20;
double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

const int NBins_Mom = 30;
double BinEdges_Mom[NBins_Mom+1] = {0,50,100,150,200,250,300,350,400,450,500,550, 600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000};

const int NBins_Q2 = 15;
double BinEdges_Q2[NBins_Q2+1] = {0, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1, 1.5, 2, 3, 5};

const int NBins_Enu = 14;
double BinEdges_Enu[NBins_Enu+1] = {0, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1200, 1500, 2000, 3000, 5000};
 
TCanvas *c = new TCanvas("c");

TH2F* compute2D_Dis(TString suffix, int config, int target, int categ, int cut_level,
		    TString varX, TString var_titleX, TString varY, TString var_titleY, 
		    int nbinsX, double* xbinsX, int nbinsY, double* xbinsY) {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleAlign(23);
  TH1::AddDirectory(kFALSE);

  TFile f(TString::Format("/nfs/neutrinos/cjesus/work/jobs/files/config%i_Target%i%s.root", config, target, suffix.Data()));
  //  f.ls();
  cout << "Reading the file in: " << TString::Format("/nfs/neutrinos/cjesus/work/jobs/files/config%i_Target%i%s.root", config, target, suffix.Data()) << endl;
  TTree* t = (TTree*)f.Get("truth");

  TString targetName="";
  if (config==0 || target<=2)
    targetName = TString::Format("FGD %i", target);
  else
    targetName = "Horiz.Target";

  TString categName = "";
  if (categ == -1)
    categName = "wTarget";
  else if (categ == -2)
    categName = "woTarget";
  else
    categName = branchNames[categ];

  TString title = TString::Format("%s, %s, %s", 
				  config==0 ? "Current":"Upgrade",
				  targetName.Data(),
				  categName.Data());

  TH2F* h = new TH2F(title, title, nbinsX, xbinsX, nbinsY, xbinsY);

  Int_t accum_level[8];
  Float_t variableX, variableY;
  int topology;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(varX,          &variableX);
  t->SetBranchAddress(varY,          &variableY);

  vector<int> branches;
  if (categ <= -1) {
    branches.push_back(3); 
    branches.push_back(4);
    branches.push_back(5);
    branches.push_back(7);
    if (categ == -1)
      branches.push_back(6);
  }
  else
    branches.push_back(categ);

  for (Int_t ient=0; ient < t->GetEntries(); ient++) {

    t->GetEntry(ient);
    bool sel = false;
    for (vector<int>::iterator it = branches.begin(); it != branches.end(); ++it) {
      bool topoCut = ((*it) >= 3) || (topology==(*it));
      sel = sel || (accum_level[*it] > cut_level);
    }

    if (sel)
      h->Fill(variableX, variableY);
  }
  f.Close();

  h->GetXaxis()->SetTitle(var_titleX);
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetXaxis()->SetTitleOffset(0.9);
  h->GetYaxis()->SetTitle(var_titleY);
  h->GetYaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetTitleOffset(0.9);

  return h;

}


void plot2D(TString suffix, int config, int categ, int cut_level,
	    TString varX, TString var_titleX, TString varY, TString var_titleY, 
	    int nbinsX, double* xbinsX, int nbinsY, double* xbinsY, bool log=false) {

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

  TString categName = "";
  if (categ == -1)
    categName = "wTarget";
  else if (categ == -2)
    categName = "woTarget";
  else
    categName = branchNames[categ];

  TString suffixName = suffix;
  if (suffix == "")
    suffixName = "_nu";

  vector<TH2F*> histos;

  for (int targ=1; targ<=NTargets[config]; targ++){
    histos.push_back(compute2D_Dis(suffix, config, targ, categ, cut_level,
				   varX, var_titleX, varY, var_titleY, 
				   nbinsX, xbinsX, nbinsY, xbinsY));
 
  TH2F *h = (TH2F*)histos[0]->Clone();
  for (UInt_t i=1; i<histos.size(); i++) 
  h->Add(histos[i]);
  h->SetTitle("");

  if (log) {
    c->SetLogz();
    h->GetZaxis()->SetRangeUser(0.5, 3000);
  }
  h->SetContour(100);
  h->Draw("colz");

   c->SaveAs(TString::Format("%s/2D_%s_%s_config%i_target%i_%s%s%s.eps", 
  			    "/nfs/neutrinos/cjesus/work/jobs/plots", varX.Data(), varY.Data(), config, targ, categName.Data(), suffixName.Data(), log ? "_log":""));

}
}


void plotAll() {

  TString suffixes[2] = {"_RHC_numu_antinu","_FHC_numu"};
  int conf[2] = {0,2};
  for (int c=-2; c<-1; c++)
    for (int s=0; s<2; s++)
      for (int cc=0; cc<2; cc++) {
	plot2D(suffixes[s], conf[cc], c, 4, 
	       "true_costheta", "true cos #theta", "true_mom", "true p_{#mu} [MeV/c]", 
	       NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom, false);
      	plot2D(suffixes[s], conf[cc], c, 4, 
	       "true_costheta", "true cos #theta", "true_mom", "true p_{#mu} [MeV/c]", 
	       NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom, true);
      }

}
