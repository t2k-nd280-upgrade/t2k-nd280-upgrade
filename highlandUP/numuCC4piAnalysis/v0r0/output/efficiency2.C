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

//TString INPDIR = "/nfs/neutrinos/cjesus/work/jobs/files/CompEff";
//TString OUTDIR = "/nfs/neutrinos/cjesus/work/jobs/plots";
TString INPDIR = "/nfs/neutrinos/cjesus/work/jobs/files/UniTarget-FGD12";
TString OUTDIR = "/nfs/neutrinos/cjesus/work/jobs/plots";

int NTargets[4] = {2, 2, 3, 3};
TString branchNames[8] = {"CC0pi", "CC1pi", "CCoth", "FWD", "BWD", "HA", "Target", "ECal"};

const int NBins_CosTh = 20;
double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

const int NBins_Mom = 29;
double BinEdges_Mom[NBins_Mom+1] = {0,50,100,150,200,250,300,350,400,450,500,550, 600,650,700,750,800,850,900,950,1000,1200, 1400,1600,1800,2000,2500,3000,4000,5000};

const int NBins_Q2 = 15;
double BinEdges_Q2[NBins_Q2+1] = {0, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1, 1.5, 2, 3, 5};

const int NBins_Enu = 14;
double BinEdges_Enu[NBins_Enu+1] = {0, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1200, 1500, 2000, 3000, 5000};
 
double PSRes_muonMom = 400;
double PSRes_muonCos = 0.4;


// plot the efficiency for a given file, configuration, target, event category
//   config: 0 = Current detector ; 2 = Upgrade detector
//   target: 1 = FGD-1 ; 2 = FGD-2 ; 3 = Super-FGD
//   categ:  0 = CC-0pi; 1 = CC-1pi; 2 = CC-other; 3 = FWD TPC; 4 = BWD TPC; 5 = HA TPC; 6 = iso-target; 7 = ECal;
//          -2 = inclusive except iso-target; -1 = inclusive
// files should be named as : config{confignumber)_Target(targetnumber)_suffix.root
TH1F* computeEff(TString suffix, int config, int target, int categ, int cut1, int cut2,
		 TString var, TString var_title, 
		 int nbins, double* xbins, bool PSRes=false) {
  
  TH1::AddDirectory(kFALSE);

 // config = 2;
 // target = 3;

  TFile f(TString::Format("%s/Target%i/config%i_Target%i%s.root",
        INPDIR.Data(),target, config, target, suffix.Data()));
  TTree* t = (TTree*)f.Get("truth");	

  TH1F* h_beforeCut = new TH1F(TString::Format("before_%s_%i_%i",var.Data(),config,target),
			       TString::Format("before_%s_%i_%i",var.Data(),config,target),
			       nbins, xbins);
  TH1F* h_afterCut = new TH1F(TString::Format("after_%s_%i_%i",var.Data(),config,target),
			      TString::Format("after_%s_%i_%i",var.Data(),config,target),
			      nbins, xbins);

  Int_t accum_level[8];
  Float_t variable;
  int topology;
  Float_t res_var;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(var,           &variable);
  t->SetBranchAddress("topology",    &topology);

  if (PSRes && var=="true_costheta")
    t->SetBranchAddress("true_mom", &res_var);
  if (PSRes && var=="true_mom")
    t->SetBranchAddress("true_costheta", &res_var);
 
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

    // do we apply some phase space restriction for the efficiency computation
    if (PSRes && var=="true_costheta")
      if (res_var < PSRes_muonMom)
	continue;
    if (PSRes && var=="true_mom")
      if (res_var < PSRes_muonCos)
	continue;
      
    bool sel1 = false, sel2 = false;
    for (vector<int>::iterator it = branches.begin(); it != branches.end(); ++it) {
      bool topoCut = ((*it) >= 3) || (topology==(*it));
      sel1 = sel1 || (accum_level[*it] > cut1 && topoCut);
      sel2 = sel2 || (accum_level[*it] > cut2 && topoCut);
    }

    if (sel1)
      h_beforeCut->Fill(variable);
    if (sel2)
      h_afterCut->Fill(variable);
    
  }

  // prepare the efficiency histogram
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


TH1F* computeDisBef(TString suffix, int config, int target, int categ, int cut1, int cut2,
     TString var, TString var_title, 
     int nbins, double* xbins, bool PSRes=false) {
  
  TH1::AddDirectory(kFALSE);

 // config = 2;
 // target = 3;

  TFile f(TString::Format("%s/Target%i/config%i_Target%i%s.root",
        INPDIR.Data(),target, config, target, suffix.Data()));
  TTree* t = (TTree*)f.Get("truth");  

  TH1F* h_beforeCut = new TH1F(TString::Format("before_%s_%i_%i",var.Data(),config,target),
             TString::Format("before_%s_%i_%i",var.Data(),config,target),
             nbins, xbins);
  TH1F* h_afterCut = new TH1F(TString::Format("after_%s_%i_%i",var.Data(),config,target),
            TString::Format("after_%s_%i_%i",var.Data(),config,target),
            nbins, xbins);

  Int_t accum_level[8];
  Float_t variable;
  int topology;
  Float_t res_var;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(var,           &variable);
  t->SetBranchAddress("topology",    &topology);

  if (PSRes && var=="true_costheta")
    t->SetBranchAddress("true_mom", &res_var);
  if (PSRes && var=="true_mom")
    t->SetBranchAddress("true_costheta", &res_var);
 
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

    // do we apply some phase space restriction for the efficiency computation
    if (PSRes && var=="true_costheta")
      if (res_var < PSRes_muonMom)
  continue;
    if (PSRes && var=="true_mom")
      if (res_var < PSRes_muonCos)
  continue;
      
    bool sel1 = false, sel2 = false;
    for (vector<int>::iterator it = branches.begin(); it != branches.end(); ++it) {
      bool topoCut = ((*it) >= 3) || (topology==(*it));
      sel1 = sel1 || (accum_level[*it] > cut1 && topoCut);
      sel2 = sel2 || (accum_level[*it] > cut2 && topoCut);
    }

    if (sel1)
      h_beforeCut->Fill(variable);
    if (sel2)
      h_afterCut->Fill(variable);
    
  }

  // prepare the efficiency histogram
  // TH1F *h_eff = new TH1F(TString::Format("h_eff_%s_%i_%i",var.Data(),config,target),
  //      TString::Format("h_eff_%s_%i_%i",var.Data(),config,target),
  //      nbins, xbins);
  // h_eff->Divide(h_afterCut, h_beforeCut);

  // for (int i=0; i<nbins; i++) {
  //   float eff = h_eff->GetBinContent(i+1);
  //   h_eff->SetBinError(i+1, sqrt(eff*(1-eff)/h_beforeCut->GetBinContent(i+1)));
  // }
  
  h_beforeCut->GetXaxis()->SetTitle(var_title);
  h_beforeCut->GetXaxis()->SetTitleSize(0.05);
  h_beforeCut->GetXaxis()->SetTitleOffset(0.9);
  h_beforeCut->GetYaxis()->SetTitleSize(0.05);
  h_beforeCut->GetYaxis()->SetTitle("before selection");
  
  return h_beforeCut;

}

TH1F* computeDisAft(TString suffix, int config, int target, int categ, int cut1, int cut2,
     TString var, TString var_title, 
     int nbins, double* xbins, bool PSRes=false) {
  
  TH1::AddDirectory(kFALSE);

 // config = 2;
 // target = 3;

  TFile f(TString::Format("%s/Target%i/config%i_Target%i%s.root",
        INPDIR.Data(),target, config, target, suffix.Data()));
  TTree* t = (TTree*)f.Get("truth");  

  TH1F* h_beforeCut = new TH1F(TString::Format("before_%s_%i_%i",var.Data(),config,target),
             TString::Format("before_%s_%i_%i",var.Data(),config,target),
             nbins, xbins);
  TH1F* h_afterCut = new TH1F(TString::Format("after_%s_%i_%i",var.Data(),config,target),
            TString::Format("after_%s_%i_%i",var.Data(),config,target),
            nbins, xbins);

  Int_t accum_level[8];
  Float_t variable;
  int topology;
  Float_t res_var;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(var,           &variable);
  t->SetBranchAddress("topology",    &topology);

  if (PSRes && var=="true_costheta")
    t->SetBranchAddress("true_mom", &res_var);
  if (PSRes && var=="true_mom")
    t->SetBranchAddress("true_costheta", &res_var);
 
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

    // do we apply some phase space restriction for the efficiency computation
    if (PSRes && var=="true_costheta")
      if (res_var < PSRes_muonMom)
  continue;
    if (PSRes && var=="true_mom")
      if (res_var < PSRes_muonCos)
  continue;
      
    bool sel1 = false, sel2 = false;
    for (vector<int>::iterator it = branches.begin(); it != branches.end(); ++it) {
      bool topoCut = ((*it) >= 3) || (topology==(*it));
      sel1 = sel1 || (accum_level[*it] > cut1 && topoCut);
      sel2 = sel2 || (accum_level[*it] > cut2 && topoCut);
    }

    if (sel1)
      h_beforeCut->Fill(variable);
    if (sel2)
      h_afterCut->Fill(variable);
    
  }

  // prepare the efficiency histogram
  // TH1F *h_eff = new TH1F(TString::Format("h_eff_%s_%i_%i",var.Data(),config,target),
  //      TString::Format("h_eff_%s_%i_%i",var.Data(),config,target),
  //      nbins, xbins);
  // h_eff->Divide(h_afterCut, h_beforeCut);

  // for (int i=0; i<nbins; i++) {
  //   float eff = h_eff->GetBinContent(i+1);
  //   h_eff->SetBinError(i+1, sqrt(eff*(1-eff)/h_beforeCut->GetBinContent(i+1)));
  // }
  
  h_afterCut->GetXaxis()->SetTitle(var_title);
  h_afterCut->GetXaxis()->SetTitleSize(0.05);
  h_afterCut->GetXaxis()->SetTitleOffset(0.9);
  h_afterCut->GetYaxis()->SetTitleSize(0.05);
  h_afterCut->GetYaxis()->SetTitle("selected");
  
  return h_afterCut;

}


// TH1F* computeDis(TString suffix, int config, int target, int categ, int cut,
//      TString var, TString var_title, 
//      int nbins, double* xbins, bool PSRes=false) {
  
//   TH1::AddDirectory(kFALSE);

//   TFile f(TString::Format("%s/Target%i/config%i_Target%i%s.root",
//         INPDIR.Data(),target, config, target, suffix.Data()));
//   TTree* t = (TTree*)f.Get("truth");  

//   TH1F* h = new TH1F(TString::Format("after_%s_%i_%i",var.Data(),config,target),
//             TString::Format("after_%s_%i_%i",var.Data(),config,target),
//             nbins, xbins);

//   Int_t accum_level[8];
//   Float_t variable;
//   int topology;
//   Float_t res_var;
  
//   t->SetBranchAddress("accum_level", &accum_level);
//   t->SetBranchAddress(var,           &variable);
//   t->SetBranchAddress("topology",    &topology);

//   if (PSRes && var=="true_costheta")
//     t->SetBranchAddress("true_mom", &res_var);
//   if (PSRes && var=="true_mom")
//     t->SetBranchAddress("true_costheta", &res_var);
 
//   vector<int> branches;
//   if (categ <= -1) {
//     branches.push_back(3); 
//     branches.push_back(4);
//     branches.push_back(5);
//     branches.push_back(7);
//     if (categ == -1)
//       branches.push_back(6);
//   }
//   else
//     branches.push_back(categ);
    

//   for (Int_t ient=0; ient < t->GetEntries(); ient++) {
    
//     t->GetEntry(ient);
      
//     bool sel = false
//     for (vector<int>::iterator it = branches.begin(); it != branches.end(); ++it) {
//       bool topoCut = ((*it) >= 3) || (topology==(*it));
//       sel = sel || (accum_level[*it] > cut && topoCut);
//     }
//     if (sel)
//       h->Fill(variable);
    
//   }
  
//   h->GetXaxis()->SetTitle(var_title);
//   h->GetXaxis()->SetTitleSize(0.05);
//   h->GetXaxis()->SetTitleOffset(0.9);
//   return h;

// }


// superimpose the efficiencies for a given set of files, category and variable
void plotTotal(TString suffix, int categ, int cut1, int cut2, 
	       TString var, TString var_title, 
	       int nbins, double* xbins, bool PSRes=false) {
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  vector<TH1F*> histos;
  
  TCanvas *c = new TCanvas("c");
  TH1F* h_dummy = c->DrawFrame(xbins[0], 0, xbins[nbins], 1.02);
  h_dummy->GetXaxis()->SetTitle(var_title);
  h_dummy->GetXaxis()->SetTitleSize(0.05);
  h_dummy->GetXaxis()->SetTitleOffset(0.9);
  h_dummy->GetYaxis()->SetTitleSize(0.05);
  h_dummy->GetYaxis()->SetTitle("efficiency");

  TLegend *leg = new TLegend(0.1, 0.905, 0.9, 0.98);
  leg->SetBorderSize(0);
  leg->SetNColumns(3);

  int configs[2] = {0,2};
  int linestyles[2] = {2,1};
  TString configNames[2] = {"current", "upgrade"};
//  TString configNames[2] = {"OLD_G4", "NEW_G4"};

  int colors[3] = {kBlack, kRed, kBlue};

  for (int ic=0; ic<2; ic++) { //ALL
//  for (int ic=1; ic<2; ic++) { //ONLY UPGRADE
//  for (int ic=0; ic<1; ic++) { //ONLY BEFORE UPGRADE
	  
  for (int it=1; it<=NTargets[configs[ic]]; it++) {
//		    for (int it=1; it<=2; it++) {
      cout << it << endl;
      TH1F* h = computeEff(suffix, configs[ic], it, categ, cut1, cut2, 
			   var, var_title, nbins, xbins, PSRes);
      h->SetLineColor(colors[it-1]);
      h->SetLineStyle(linestyles[ic]);
      h->SetLineWidth(3);
      h->Draw("esame");

      TString targetName="";
      if (ic==0 || it<=2)
	targetName = TString::Format("FGD %i", it);
      else
	targetName = "Horiz.Target";
      leg->AddEntry(h, TString::Format("%s, %s", configNames[ic].Data(), targetName.Data()), "el");

      histos.push_back(h);
    }

    if (ic==0)
      leg->AddEntry((TObject*)0, "#bf{Preliminary}", "");
//      leg->AddEntry((TObject*)0, "#bf{NEW_hUp}", "");
  }

  leg->Draw("same");

  TString suffixName = suffix;
  if (suffix == "")
    suffixName = "_nu";


  TString categName = "";
  if (categ == -1)
    categName = "wTarget";
  else if (categ == -2)
    categName = "woTarget";
  else
    categName = branchNames[categ];

  c->Update();
  c->SaveAs(TString::Format("%s/eff_total%s_%s_%s%s.eps", 
			    OUTDIR.Data(), suffixName.Data(), var.Data(), categName.Data(), PSRes ? "_PSRes":""));

  TFile fW(TString::Format("%s/eff_total%s_%s_%s%s.root", 
          OUTDIR.Data(), suffixName.Data(), var.Data(), categName.Data(), PSRes ? "_PSRes":""), "RECREATE");

  cout << endl << "Writing: " << TString::Format("%s/eff_total%s_%s_%s%s.root", 
          OUTDIR.Data(), suffixName.Data(), var.Data(), categName.Data(), PSRes ? "_PSRes":"") << endl;
  
  for(int w=0; w<histos.size(); w++){
  histos[w]->Write();
  }

}

// compate two sets of file (for a given detector configuration)
void plotTotalComp(TString suffix1, TString suffix2, int config, int categ, int cut1, int cut2, 
	       TString var, TString var_title, 
	       int nbins, double* xbins) {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  vector<TH1F*> histos;
  
  TCanvas *c = new TCanvas("c");
  // TH1F* h_dummy = c->DrawFrame(xbins[0], 0, xbins[nbins], 1.02);
  // h_dummy->GetXaxis()->SetTitle(var_title);
  // h_dummy->GetXaxis()->SetTitleSize(0.05);
  // h_dummy->GetXaxis()->SetTitleOffset(0.9);
  // h_dummy->GetYaxis()->SetTitleSize(0.05);
  // h_dummy->GetYaxis()->SetTitle("efficiency");

  TLegend *leg = new TLegend(0.1, 0.905, 0.9, 0.98);
  leg->SetBorderSize(0);
  leg->SetNColumns(NTargets[config]);

  int linestyles[2] = {2,1};
  TString suffixes[2] = {suffix1, suffix2};

  int colors[3] = {kBlack, kRed, kBlue};

  for (int ic=1; ic>=0; ic--) {
	  
    for (int it=1; it<=NTargets[config]; it++) {
		
    //   TH1F* h = computeEff(suffixes[ic], config, it, categ, cut1, cut2, 
				// var, var_title, nbins, xbins);
      TH1F* h = computeDisBef(suffixes[ic], config, it, categ, cut1, cut2,
        var, var_title, nbins, xbins);
      h->SetLineColor(colors[it-1]);
      h->SetLineStyle(linestyles[ic]);
      h->SetLineWidth(3);
      h->Draw("esame");

      TString targetName="";
      if (config==0 || it<=2)
	targetName = TString::Format("FGD %i", it);
      else
	targetName = "Horiz.Target";

      TString suffixName="";
      if (suffixes[ic]!="") {
	suffixName = suffixes[ic];
	suffixName.Replace(0,1,", ");
      }

      leg->AddEntry(h, TString::Format("%s%s", targetName.Data(), suffixName.Data()), "el");

      histos.push_back(h);
    }

  }

  leg->Draw("same");

  TString categName = "";
  if (categ == -1)
    categName = "wTarget";
  else if (categ == -2)
    categName = "woTarget";
  else
    categName = branchNames[categ];

  c->Update();
  c->SaveAs(TString::Format("%s/eff_total_%s_comp_config%i%s_VS%s_%s.eps", 
			    OUTDIR.Data(), var.Data(), config, suffix1.Data(), suffix2.Data(), categName.Data()));


}


void plot2D(TString suffix, int config, int target, int categ, int cut1, int cut2, 
	    TString varX, TString var_titleX, TString varY, TString var_titleY, 
	    int nbinsX, double* xbinsX, int nbinsY, double* xbinsY) {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleAlign(23);
  TH1::AddDirectory(kFALSE);

  TFile f(TString::Format("%s/config%i_Target%i%s.root",
        INPDIR.Data(),config, target, suffix.Data()));
  TTree* t = (TTree*)f.Get("truth");

  TH2F* hSig    = new TH2F("h1", "h2", nbinsX, xbinsX, nbinsY, xbinsY);
  TH2F* hSelSig = new TH2F("h2", "h2", nbinsX, xbinsX, nbinsY, xbinsY);
  TH2F* hSel    = new TH2F("h3", "h3", nbinsX, xbinsX, nbinsY, xbinsY);

  Int_t accum_level[8];
  Float_t variableX, variableY;
  int topology;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(varX,          &variableX);
  t->SetBranchAddress(varY,          &variableY);
  t->SetBranchAddress("topology",    &topology);

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

    bool sel1 = false, sel2 = false, sel3 = false;
    for (vector<int>::iterator it = branches.begin(); it != branches.end(); ++it) {
      bool topoCut = ((*it) >= 3) || (topology==(*it));
      sel1 = sel1 || (accum_level[*it] > cut1 && topoCut);
      sel2 = sel2 || (accum_level[*it] > cut2 && topoCut);
      sel3 = sel3 || (accum_level[*it] > cut2);
    }

    if (sel1)
      hSig->Fill(variableX, variableY);
    if (sel2)
      hSelSig->Fill(variableX, variableY);
    if (sel3)
      hSel->Fill(variableX, variableY);

  }
  f.Close();

  TCanvas *c = new TCanvas("c", "c");
  TH2F *h_eff = new TH2F("h_eff", "h_eff", nbinsX, xbinsX, nbinsY, xbinsY);
  h_eff->Divide(hSelSig, hSig);
  TH2F *h_pur = new TH2F("h_pur", "h_pur", nbinsX, xbinsX, nbinsY, xbinsY);
  h_pur->Divide(hSelSig, hSel);

  TString suffixName = suffix;
  if (suffix == "")
    suffixName = "_nu";

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

  h_eff->SetTitle(TString::Format("%s, %s, %s", 
				  config==0 ? "Current":"Upgrade",
				  targetName.Data(),
				  categName.Data()));

  h_eff->GetXaxis()->SetTitle(var_titleX);
  h_eff->GetXaxis()->SetTitleSize(0.05);
  h_eff->GetXaxis()->SetTitleOffset(0.9);
  h_eff->GetYaxis()->SetTitle(var_titleY);
  h_eff->GetYaxis()->SetTitleSize(0.05);
  h_eff->GetYaxis()->SetTitleOffset(0.9);
  h_eff->GetZaxis()->SetRangeUser(0.,1.);
  
  h_eff->Draw("colz");
  c->Update();
  c->SaveAs(TString::Format("%s/eff_config%i_target%i_total_%s_%s_%s%s.eps", 
			    OUTDIR.Data(), config, target, varX.Data(), varY.Data(), categName.Data(), suffixName.Data()));

  TFile fW(TString::Format("%s/eff_config%i_target%i_total_%s_%s_%s%s.root", 
			   OUTDIR.Data(), config, target, varX.Data(), varY.Data(), categName.Data(), suffixName.Data()), "RECREATE");

  h_eff->Write();
  h_pur->Write();


}



void efficiency2() {

  TString suffixes[1] = {"_FHC_numu_OLD"};
  int conf[2] = {0,2};

 //  for (int c=-2; c<8; c++) {
 //    for (int s=0; s<3; s++) {
 //      plotTotal(suffixes[s], c, -1, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh);
 //      plotTotal(suffixes[s], c, -1, 4, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom);
 //      plotTotal(suffixes[s], c, -1, 4, "true_Q2", "true Q^{2} [GeV^{2}/c^{2}]", NBins_Q2, BinEdges_Q2);

 //      for (int cc=0; cc<2; cc++) {
	// for (int t=1; t<=NTargets[conf[cc]]; t++) {
	//   plot2D(suffixes[s], conf[cc], t, c, -1, 4, 
	// 	 "true_costheta", "true cos #theta", "true_mom", "true p_{#mu} [MeV/c]", 
	// 	 NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);
	// }
 //      }
 //    }
 //  }

 //  for (int s=0; s<3; s++) {
 //    plotTotal(suffixes[s], -2, -1, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh, true);
 //    plotTotal(suffixes[s], -2, -1, 4, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom, true);
 //  }

 //  plotTotalComp("", "_old", 0, -2, -1, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh);
 //  plotTotalComp("", "_old", 2, -2, -1, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh);

     // plot2D("_FHC_numu_OLD", 2, 3, -2, -1, 4, 
     // "true_costheta", "true cos #theta", "true_mom", "true p_{#mu} [MeV/c]", 
     // NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);


   // plotTotal("_FHC_numu_OLD", -2, -1, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh);
   // plotTotal("_FHC_numu_NEW", -2, -1, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh);

// for (int c=-2; c<8; c++) {
//    plotTotal("_FHC_numu_EcalMod_NEW_hUp_OLD_G4", c, -1, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh);
//  }

//  for (int c=-2; c<8; c++) {
//    plotTotal("_FHC_numu_EcalMod_NEW_hUp_OLD_G4", c, -1, 4, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom);
//  }

//  for (int c=-2; c<8; c++) {
//    plotTotal("_FHC_numu_EcalMod_NEW_hUp_OLD_G4", c, -1, 4, "true_Q2", "true Q^{2} [GeV^{2}/c^{2}]", NBins_Q2, BinEdges_Q2);
//  }

// for (int c=-2; c<8; c++) {
//    plotTotal("_FHC_numu_EcalMod_NEW_hUp_NEW_G4", c, -1, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh);
//  }

//  for (int c=-2; c<8; c++) {
//    plotTotal("_FHC_numu_EcalMod_NEW_hUp_NEW_G4", c, -1, 4, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom);
//  }

//  for (int c=-2; c<8; c++) {
//    plotTotal("_FHC_numu_EcalMod_NEW_hUp_NEW_G4", c, -1, 4, "true_Q2", "true Q^{2} [GeV^{2}/c^{2}]", NBins_Q2, BinEdges_Q2);
//  }


for (int c=-2; c<8; c++) {
     plotTotalComp("_FHC_numu_EcalMod_NEW_hUp_OLD_G4", "_FHC_numu_EcalMod_NEW_hUp_NEW_G4", 2, 
           c, -1, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh);
   }

for (int c=-2; c<8; c++) {
     plotTotalComp("_FHC_numu_EcalMod_NEW_hUp_OLD_G4", "_FHC_numu_EcalMod_NEW_hUp_NEW_G4", 2, 
           c, -1, 4, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom);
   }

  for (int c=-2; c<8; c++) {
     plotTotalComp("_FHC_numu_EcalMod_NEW_hUp_OLD_G4", "_FHC_numu_EcalMod_NEW_hUp_NEW_G4", 2, 
           c, -1, 4, "true_Q2", "true Q^{2} [GeV^{2}/c^{2}]", NBins_Q2, BinEdges_Q2);
 }


}
