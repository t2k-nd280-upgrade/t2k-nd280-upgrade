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

int NTargets[3] = {2, 2, 3};

void check_overlap(int target, int config) {
	  
	TFile* f0 = new TFile(TString::Format("jobs/files/config%i_Target%i_%i.root",
				       config, target, 0));
	TTree* t0 = (TTree*)f0->Get("truth");
	
	TFile* f1 = new TFile(TString::Format("jobs/files/config%i_Target%i_%i.root",
				       config, target, 1));
	TTree* t1 = (TTree*)f1->Get("truth");
	
	TFile* f2 = new TFile(TString::Format("jobs/files/config%i_Target%i_%i.root",
				       config, target, 2));
	TTree* t2 = (TTree*)f2->Get("truth");
	
	TFile* f3 = new TFile(TString::Format("jobs/files/config%i_Target%i_%i.root",
				       config, target, 3));
	TTree* t3 = (TTree*)f3->Get("truth");
	
	int accum_level0[1][1], accum_level1[1][1], accum_level2[1][1], accum_level3[1][1];
	t0->SetBranchAddress("accum_level", &accum_level0);
	t1->SetBranchAddress("accum_level", &accum_level1);
	t2->SetBranchAddress("accum_level", &accum_level2);
	t3->SetBranchAddress("accum_level", &accum_level3);
	
	for (Int_t ient=0; ient < t0->GetEntries(); ient++) {
    t0->GetEntry(ient);
	t1->GetEntry(ient);
    t2->GetEntry(ient);
	t3->GetEntry(ient);
	
    if (accum_level0[0][0]>4 && accum_level1[0][0]>4)
		cout << "Overlap 0 1 !!!" << endl;
    if (accum_level0[0][0]>4 && accum_level2[0][0]>4)
		cout << "Overlap 0 2 !!!" << endl;
	if (accum_level0[0][0]>4 && accum_level3[0][0]>4)
		cout << "Overlap 0 3 !!!" << endl;
    if (accum_level1[0][0]>4 && accum_level2[0][0]>4)
		cout << "Overlap 1 2 !!!" << endl;
	if (accum_level1[0][0]>4 && accum_level3[0][0]>4)
		cout << "Overlap 1 3 !!!" << endl;
    if (accum_level2[0][0]>4 && accum_level3[0][0]>4)
		cout << "Overlap 2 3 !!!" << endl;
	
	}
	
}

void check_overlap_all() {
	
	  for (int ic=0; ic<3; ic++) {
    for (int it=1; it<=NTargets[ic]; it++) {
		cout << "Config = " << ic <<", Target = " << it << endl;
		check_overlap(it, ic);
	}
	  }
}

TH2F* computeCosTheta_vs_SinPhiSinTheta(TString file, int branch, int cut) {

  TFile* f = new TFile(file);
  TTree* t = (TTree*)f->Get("default");
  
  TH2F* h = new TH2F("h", "h", 20, -1, 1, 20, -1, 1);
  t->Project("h", "selmu_truedir[1]:selmu_truedir[2]", TString::Format("accum_level[][%i]>%i", branch, cut));

  return h;
  
}

TH2F* computeCosTheta_vs_startRadius(TString file, int branch, int cut) {

  TFile* f = new TFile(file);
  TTree* t = (TTree*)f->Get("default");
  
  TH2F* h = new TH2F("h", "h", 20, -1, 1, 20, 0, 1500);
  t->Project("h", "sqrt(pow(selmu_truepos[0],2)+pow(selmu_truepos[1],2)):selmu_truedir[2]", TString::Format("accum_level[][%i]>%i", branch, cut));

  return h;
  
}


void plotCosTheta_vs_SinPhiSinTheta() {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  TString samplename[4] = {"FwdTPC", "BwdTPC", "HATPC", "ECal"};

  // loop on configurations
  for (int ic=0; ic<2; ic++) {
    // loop on target
    for (int it=1; it<=2; it++) {
      // loop on branches
      for (int i=0; i<4; i++) {

	TCanvas* c = new TCanvas();
	TH2F* h = computeCosTheta_vs_SinPhiSinTheta(TString::Format("jobs/files/config%i_Target%i_%i.root", 
								    ic, it, i), 
						    0, 4);
	h->GetXaxis()->SetTitle("cos #theta");
	h->GetXaxis()->SetTitleSize(0.045);
	h->GetYaxis()->SetTitle("sin #phi sin #theta");
	h->GetYaxis()->SetTitleSize(0.045);

	h->Draw("colz");
	c->SaveAs(TString::Format("fig/dirY_dirZ/dirY_vs_costheta_%s_Target%i_%s.eps", 
				  ic==0 ? "current":"upgrade", it, samplename[i].Data()));
    
      }
    }
  }


}

void plotCosTheta_vs_startRadius() {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  TString samplename[4] = {"FwdTPC", "BwdTPC", "HATPC", "ECal"};

  // loop on configurations
  for (int ic=0; ic<2; ic++) {
    // loop on target
    for (int it=1; it<=2; it++) {
      // loop on branches
      for (int i=0; i<4; i++) {

	TCanvas* c = new TCanvas();
	TH2F* h = computeCosTheta_vs_startRadius(TString::Format("jobs/files/config%i_Target%i_%i.root", 
								 ic, it, i), 
						 0, 4);
	h->GetXaxis()->SetTitle("cos #theta");
	h->GetXaxis()->SetTitleSize(0.045);
	h->GetYaxis()->SetTitle("radius in (XY)");
	h->GetYaxis()->SetTitleSize(0.045);

	h->Draw("colz");
	c->SaveAs(TString::Format("fig/startR_dirZ/startR_vs_costheta_%s_Target%i_%s.eps", 
				  ic==0 ? "current":"upgrade", it, samplename[i].Data()));
    
      }
    }
  }


}


TH1F* computeTPC(TString file, int branch, int cut) {

  TFile* f = new TFile(file);
  TTree* t = (TTree*)f->Get("default");
  
  TH1F* h = new TH1F("h", "h", 7, 0, 7);
  t->Project("h", "selmu_longestTPC", TString::Format("accum_level[][%i]>%i", branch, cut));

}

void plotTPC() {

  TCanvas* c = new TCanvas();
  
  for (int i=0; i<4; i++) {
    TH1F* h = computeTPC(TString::Format("jobs/files/config1_Target1_%i",i), 0, 4);
  }
 
}
