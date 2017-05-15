#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <cstdlib>

#include "TRandom3.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TString.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TMath.h"

// number of targets for each configuration
int NTargets[6] = {2, 2, 3, 3, 3, 3};

/// Returns the distribution of ToF mass for a given config/target/true PDG
/// bool truth:  true = plot the true ToF mass ; false = plot the reconstructed ToF mass
/// bool onlyTB: true = plot only tracks reaching Top or Bottom ToF ; false = plot all tracks
TH1F* compute_ToF_mass(int config, int target, int selPDG, TString suffix="", bool truth=false, bool onlyTB=false, float min_mom=50, float max_mom=30000) {
	
  // open file
  TChain* t = new TChain("default");
	
  t->Add(TString::Format("jobs/files/config%i_Target%i%s.root",
			 config, target, suffix.Data()));
	
  // which ToF counters to consider for the Top/Bottom only plots
  int ToF_Top=16, ToF_Bot=18;
  if (target==2)
    { ToF_Top=15; ToF_Bot=17; }
	
  // prepare the histogram
  TH1F* h = new TH1F(TString::Format("h_ToF_config%i_target%i_pdg%i",config,target,selPDG),
		     TString::Format("h_ToF_config%i_target%i_pdg%i",config,target,selPDG),
		     200, 0, 2000);
	
  // initialize the tree	
  Int_t accum_level[1][16];
  Float_t mass[100], mom[100];
  Int_t PDG[100], det1[100], det2[100], nTracks;
  t->SetBranchAddress("accum_level",    &accum_level);
  t->SetBranchAddress(truth ? "selAll_ToF_true_mass":"selAll_ToF_mass", &mass);
  t->SetBranchAddress("selAll_PDG",     &PDG);
  t->SetBranchAddress("selAll_mom",     &mom);
  t->SetBranchAddress("selAll_nTracks", &nTracks);
  t->SetBranchAddress("selAll_ToF_det_used1", &det1);
  t->SetBranchAddress("selAll_ToF_det_used2", &det2);
	
  // loop on events
  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
		
    t->GetEntry(ient);
    // only consider preselected events
    if (accum_level[0][0]<=2)
      continue;
		
    // loop on tracks
    for (Int_t i=0; i<nTracks; i++) {
      // cut on momentum
      if (mom[i] < min_mom || mom[i] > max_mom) continue;
      // take only particles of the PDG we want
      if (abs(PDG[i]) != selPDG) continue;
      // if onlyTB is ON, we consider the events only if the Top or Bottom ToF was used
      if (onlyTB && !(det1[i]==ToF_Bot || det1[i]==ToF_Top) && !(det2[i]==ToF_Bot || det2[i]==ToF_Top)) continue;
      h->Fill(mass[i]);
    }
		
  }
	
	
  h->GetXaxis()->SetTitle(truth ? "true ToF mass [MeV/c^{2}]":"reco ToF mass [MeV/c^{2}]");
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetXaxis()->SetTitleOffset(0.9);
	

	
  return h;
	
}

/// Plot the ToF mass, where the different true particle types are superimposed
/// bool truth:  true = plot the true ToF mass ; false = plot the reconstructed ToF mass
/// bool onlyTB: true = plot only tracks reaching Top or Bottom ToF ; false = plot all tracks
/// bool norm:   true = normalize the distribution to 1 ; false = do not normalize
void plot_ToF_mass(int config, int target, TString suffix="", bool truth=false, bool onlyTB=false, bool norm=true, float min_mom=50, float max_mom=30000) {
	
  // different particles types to be considered
  int PDG[4] = {13, 211, 2212, 11};
  int col[4] = { 2,  28,    4,  8};
  TString leg[4] = {"#mu", "#pi", "p", "e"};
	
  TH1F* h[4];
  TCanvas *c = new TCanvas("c");
	
  TH1F* h_dummy = c->DrawFrame(0, 0, 2000, 0.3);
  h_dummy->GetXaxis()->SetTitle(truth ? "true ToF mass [MeV/c^{2}]":"reco ToF mass [MeV/c^{2}]");
  h_dummy->GetXaxis()->SetTitleSize(0.05);
  h_dummy->GetXaxis()->SetTitleOffset(0.9);
	
  TLegend *legend = new TLegend(0.1, 0.89, 0.9, 0.97);
  legend->SetNColumns(4);
	
  float max=0;
  for (int i=0; i<4; i++) {
    h[i] = compute_ToF_mass(config, target, PDG[i], suffix, truth, onlyTB, min_mom, max_mom);
    h[i]->SetLineColor(col[i]);
    h[i]->SetFillColor(col[i]);
    h[i]->SetFillStyle(3001);
		
    if (norm) h[i]->Scale(1./h[i]->Integral());
    h[i]->Draw("same");
    legend->AddEntry(h[i], leg[i].Data(), "f");
		
    if (h[i]->GetMaximum() > max)
      max = h[i]->GetMaximum();
  }
	
  h_dummy->GetYaxis()->SetRangeUser(0, 1.1*max);
	
  legend->Draw("same");
  c->Update();

  TString mombin="";
  if (min_mom!=50 || max_mom!=30000)
    mombin = TString::Format("_mom_%i_%i", (int)min_mom, (int)max_mom);

  c->SaveAs(TString::Format("fig/ToF_mass/%s_ToF_mass_config%i_target%i%s%s%s%s.eps", truth ? "true":"reco", config, target, suffix.Data(), onlyTB ? "_onlyTB":"", norm ? "_norm":"", mombin.Data()));
	
}


/// plot the ToF PID variables
/// int pullPDG: plot variable with which hypothesis (between 11,13,211,2212)
/// int var:     which variable to plot (0: ToF pull, 1: ToF likelihood, 2: product of ToF likelihood and TPC likelihood
void plot_ToF_pull(int config, int target, int pullPDG, TString suffix="", bool onlyTB=false, int var=0, float min_mom=50, float max_mom=30000) {
	
  // open the file
  TChain* t = new TChain("default");
	
  t->Add(TString::Format("jobs/files/config%i_Target%i%s.root",
			 config, target, suffix.Data()));
  TH1::AddDirectory(kFALSE);
	
  // which ToF counters to consider for the Top/Bottom only plots
  int ToF_Top=16, ToF_Bot=18;
  if (target==2)
    { ToF_Top=15; ToF_Bot=17; }

  // different particles/variables to be considered

  int selPDG[4]      = {13, 211, 11, 2212};
  TString selName[4] = {"#mu", "#pi", "e", "p"};
  int selColor[4]    = {2, 28, 8, 4};
	
  TString varName[3]  = {"pull", "L", "prodL"};
  TString varTitle[3] = {"pull", "L^{ToF}", "L^{TPC}L^{ToF}"};
	
  // prepare the histogram
  int nbins=100;
  float xmin=-10, xmax=10;
  if (var>0) {xmin=0; xmax=1.01;}
	
  TH1F* h[4];
  for (int j=0; j<4; j++)
    h[j] = new TH1F(TString::Format("h_ToF_pull_config%i_target%i_pdg%i_%i",config,target,pullPDG,j),
		    TString::Format("h_ToF_pull_config%i_target%i_pdg%i_%i",config,target,pullPDG,j),
		    nbins, xmin, xmax);
	
  // initialize the tree	
	
  Int_t accum_level[1][16];
  Float_t mom[100];
  Int_t PDG[100], det1[100], det2[100], nTracks;
	
  Float_t pullmu[100], pulle[100], pullpi[100], pullp[100];
  Float_t lklmu[100], lkle[100], lklpi[100], lklp[100];
  Float_t TPC_pullmu[100], TPC_pulle[100], TPC_pullpi[100], TPC_pullp[100];
  Float_t TPC_lklmu[100], TPC_lkle[100], TPC_lklpi[100], TPC_lklp[100];
	
  t->SetBranchAddress("accum_level",    &accum_level);
  t->SetBranchAddress("selAll_PDG",     &PDG);
  t->SetBranchAddress("selAll_mom",     &mom);
  t->SetBranchAddress("selAll_nTracks", &nTracks);
  t->SetBranchAddress("selAll_ToF_det_used1", &det1);
  t->SetBranchAddress("selAll_ToF_det_used2", &det2);
	
  t->SetBranchAddress("selAll_ToF_pull_muon",     &pullmu);
  t->SetBranchAddress("selAll_ToF_pull_pion",     &pullpi);
  t->SetBranchAddress("selAll_ToF_pull_electron", &pulle);
  t->SetBranchAddress("selAll_ToF_pull_proton",   &pullp);
	
  t->SetBranchAddress("selAll_ToF_lkl_muon",     &lklmu);
  t->SetBranchAddress("selAll_ToF_lkl_pion",     &lklpi);
  t->SetBranchAddress("selAll_ToF_lkl_electron", &lkle);
  t->SetBranchAddress("selAll_ToF_lkl_proton",   &lklp);
	
  t->SetBranchAddress("selAll_TPC_pull_muon",     &TPC_pullmu);
  t->SetBranchAddress("selAll_TPC_pull_pion",     &TPC_pullpi);
  t->SetBranchAddress("selAll_TPC_pull_electron", &TPC_pulle);
  t->SetBranchAddress("selAll_TPC_pull_proton",   &TPC_pullp);
	
  t->SetBranchAddress("selAll_TPC_lkl_muon",     &TPC_lklmu);
  t->SetBranchAddress("selAll_TPC_lkl_pion",     &TPC_lklpi);
  t->SetBranchAddress("selAll_TPC_lkl_electron", &TPC_lkle);
  t->SetBranchAddress("selAll_TPC_lkl_proton",   &TPC_lklp);
	
	
  // loop on events
  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
		
    t->GetEntry(ient);
    if (accum_level[0][0]<=2)
      continue;
		
    // loop on tracks
    for (Int_t i=0; i<nTracks; i++) {
      if (mom[i] < min_mom || mom[i] > max_mom) continue;
      if (onlyTB && !(det1[i]==ToF_Bot || det1[i]==ToF_Top) && !(det2[i]==ToF_Bot || det2[i]==ToF_Top)) continue;
			
      // what is the true charge of the particle
      int charge = -1;
      if (PDG[i]==211)
	charge = 1;
      if (PDG[i]==2212)
	charge = 1;
      if (PDG[i]==-11)
	charge = 1;

      // loop on the different true particles
      for (int j=0; j<4; j++) {
				
	// what is the charge we want to select
	int chargeToSel = -1;
	if (pullPDG==211)
	  chargeToSel = 1;
	if (pullPDG==2212)
	  chargeToSel = 1;
	if (pullPDG==-11)
	  chargeToSel = -1;
				
	// only select tracks with correct charge
	if (charge != chargeToSel) continue;
				
	// fill the histograms
	if (var==0 && abs(PDG[i]) == selPDG[j]) {
	  if (pullPDG==13)   h[j]->Fill(pullmu[i]);
	  if (pullPDG==211)  h[j]->Fill(pullpi[i]);
	  if (pullPDG==11)   h[j]->Fill(pulle[i]);
	  if (pullPDG==2212) h[j]->Fill(pullp[i]);
	}
	if (var==1 && abs(PDG[i]) == selPDG[j]) {
	  if (pullPDG==13)   h[j]->Fill(lklmu[i]);
	  if (pullPDG==211)  h[j]->Fill(lklpi[i]);
	  if (pullPDG==11)   h[j]->Fill(lkle[i]);
	  if (pullPDG==2212) h[j]->Fill(lklp[i]);
	}
	if (var==2 && abs(PDG[i]) == selPDG[j]) {
	  if (pullPDG==13)   h[j]->Fill(lklmu[i]*TPC_lklmu[i]);
	  if (pullPDG==211)  h[j]->Fill(lklpi[i]*TPC_lklpi[i]);
	  if (pullPDG==11)   h[j]->Fill(lkle[i]*TPC_lkle[i]);
	  if (pullPDG==2212) h[j]->Fill(lklp[i]*TPC_lklp[i]);
	}
      }
			
    }
		
  }
	
  // plot the stacked histograms with all the true particles types superimposed
  TLegend *leg = new TLegend(0.1, 0.89, 0.9, 0.94);
  leg->SetNColumns(4);
  THStack *hs = new THStack("hs", "");
  for (int j=3; j>=0; j--) {
    h[j]->SetLineColor(selColor[j]);
    h[j]->SetFillColor(selColor[j]);
    h[j]->SetFillStyle(3001);
    hs->Add(h[j]);
  }
	
  for (int j=0; j<4; j++)
    leg->AddEntry(h[j], selName[j], "f");
	
  TCanvas *c = new TCanvas();
  hs->Draw();
  leg->Draw("same");
	
  TString name;
  if (pullPDG==13)   name="#mu";
  if (pullPDG==211)  name="#pi";
  if (pullPDG==11)   name="e";
  if (pullPDG==2212) name="p";
	
  hs->GetHistogram()->GetXaxis()->SetTitle(TString::Format("%s|_{%s}", varTitle[var].Data(), name.Data()));
  hs->GetHistogram()->GetXaxis()->SetTitleSize(0.045);
  hs->GetHistogram()->GetXaxis()->SetTitleOffset(0.9);
	
  hs->GetHistogram()->SetMinimum(0.5);
  c->SetLogy();
	
  c->Update();

  TString mombin="";
  if (min_mom!=50 || max_mom!=30000)
    mombin = TString::Format("_mom_%i_%i", (int)min_mom, (int)max_mom);

  c->SaveAs(TString::Format("fig/ToF_mass/pull_%i_config%i_target%i%s%s_%s%s.eps", pullPDG, config, target, suffix.Data(), onlyTB ? "_onlyTB":"", varName[var].Data(), mombin.Data()));
	
}

/// plot a 2D graph efficiency versus purity where each point corresponds to a different cut on likelihood variables
void plot_ToF_EffPur(int config, int target, int particleToSel, TString suffix, bool onlyTB=false){
	
  gStyle->SetOptTitle(0);	
  TString selName[4] = {"#mu", "#pi", "e", "p"};	
	
  // open the file
  TChain* t = new TChain("default");
  t->Add(TString::Format("jobs/files/config%i_Target%i%s.root",
			 config, target, suffix.Data()));
  TH1::AddDirectory(kFALSE);
	
  // which ToF counters to consider for the Top/Bottom only plots
  int ToF_Top=16, ToF_Bot=18;
  if (target>=2)
    { ToF_Top=15; ToF_Bot=17; }
	
  // initialize the tree
	
  Int_t accum_level[1][16];
  Float_t mom[100];
  Int_t PDG[100], det1[100], det2[100], nTracks;
  Float_t ToF_lklmu[100], ToF_lkle[100], ToF_lklpi[100], ToF_lklp[100];
  Float_t TPC_lklmu[100], TPC_lkle[100], TPC_lklpi[100], TPC_lklp[100];
	
  t->SetBranchAddress("accum_level",    &accum_level);
  t->SetBranchAddress("selAll_PDG",     &PDG);
  t->SetBranchAddress("selAll_mom",     &mom);
  t->SetBranchAddress("selAll_nTracks", &nTracks);
  t->SetBranchAddress("selAll_ToF_det_used1", &det1);
  t->SetBranchAddress("selAll_ToF_det_used2", &det2);
	
  t->SetBranchAddress("selAll_ToF_lkl_muon",     &ToF_lklmu);
  t->SetBranchAddress("selAll_ToF_lkl_pion",     &ToF_lklpi);
  t->SetBranchAddress("selAll_ToF_lkl_electron", &ToF_lkle);
  t->SetBranchAddress("selAll_ToF_lkl_proton",   &ToF_lklp);
  t->SetBranchAddress("selAll_TPC_lkl_muon",     &TPC_lklmu);
  t->SetBranchAddress("selAll_TPC_lkl_pion",     &TPC_lklpi);
  t->SetBranchAddress("selAll_TPC_lkl_electron", &TPC_lkle);
  t->SetBranchAddress("selAll_TPC_lkl_proton",   &TPC_lklp);
	
  // in order to 
		
  int Ncuts=10000;
  float **cut2 = new float*[4];
  float **a = new float*[4];
  for (int i=0; i<4; i++){
	cut2[i] = new float[Ncuts];
	a[i] = new float[Ncuts];
  }
  float *cut1 = new float[Ncuts];
       
  float N_sig=0, *N_sel = new float[Ncuts], *N_sigsel = new float[Ncuts];
  float *eff = new float[Ncuts], *pur = new float[Ncuts];
  float *N_sel_TPC = new float[Ncuts], *N_sigsel_TPC = new float[Ncuts];
  float *eff_TPC = new float[Ncuts], *pur_TPC = new float[Ncuts];
  float *sign = new float[Ncuts], *sign_TPC = new float[Ncuts];
	
  TRandom3 r;
	
  // throw randomly the values of the cuts
  for (int k=0; k<Ncuts; k++){
    cut1[k] = r.Rndm();
    for (int i=0; i<4; i++){
      cut2[i][k] = r.Rndm();
      a[i][k]    = r.Rndm();
    }
    N_sel[k] = N_sigsel[k] = 0;
    N_sel_TPC[k] = N_sigsel_TPC[k] = 0;
  }
	
  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
		
    t->GetEntry(ient);
    if (accum_level[0][0]<=2)
      continue;
		
    for (Int_t i=0; i<nTracks; i++) {
      if (mom[i] < 50) continue;
      if (onlyTB && !(det1[i]==ToF_Bot || det1[i]==ToF_Top) && !(det2[i]==ToF_Bot || det2[i]==ToF_Top)) continue;
	  
      int bin_mom=0;
      if (mom[i]>50 && mom[i]<200) bin_mom=0;
      if (mom[i]>200 && mom[i]<400) bin_mom=1;
      if (mom[i]>400 && mom[i]<1000) bin_mom=2;
      if (mom[i]>1000) bin_mom=3;
			
      // what is the charge we want to select?
      int chargeToSel = -1;
      if (particleToSel==211)
	chargeToSel = 1;
      if (particleToSel==2212)
	chargeToSel = 1;
      if (particleToSel==-11)
	chargeToSel = -1;
			
      // what is the charge of the selected track?
      int charge = -1;
      if (PDG[i]==211)
	charge = 1;
      if (PDG[i]==2212)
	charge = 1;
      if (PDG[i]==-11)
	charge = 1;
			
      // signal is the particles with correct true PDG
      bool signal = (PDG[i] == particleToSel);
      if (signal) N_sig++;
			
      // loop on cuts
      for (int k=0; k<Ncuts; k++) {
	bool selected = false, selected_TPC = false;
				
	bool noToF = TMath::IsNaN(ToF_lklmu[i]);
				
	/*
	  if (particleToSel==13) 
	  selected = (!noToF && ToF_lklmu[i]*TPC_lklmu[i] >= cut1[k]) || (noToF && TPC_lklmu[i] >= cut2[k]);
	  if (particleToSel==211)
	  selected = (!noToF && ToF_lklpi[i]*TPC_lklpi[i] >= cut1[k]) || (noToF && TPC_lklpi[i] >= cut2[k]);
	  if (particleToSel==11)
	  selected = (!noToF && ToF_lkle[i]*TPC_lkle[i] >= cut1[k]) || (noToF && TPC_lkle[i] >= cut2[k]);
	  if (particleToSel==2212)
	  selected = (!noToF && ToF_lklp[i]*TPC_lklp[i] >= cut1[k]) || (noToF && TPC_lklp[i] >= cut2[k]);
	*/
				
	// we cut on a combination of TPC and ToF likelihood
	if (particleToSel==13) 
	  selected = (noToF && TPC_lklmu[i] >= cut1[k]) || (!noToF && a[bin_mom][k]*ToF_lklmu[i]+(1-a[bin_mom][k])*TPC_lklmu[i] >= cut2[bin_mom][k]);
	if (particleToSel==211)
	  selected = (noToF && TPC_lklpi[i] >= cut1[k]) || (!noToF && a[bin_mom][k]*ToF_lklpi[i]+(1-a[bin_mom][k])*TPC_lklpi[i] >= cut2[bin_mom][k]);
	if (particleToSel==11)
	  selected = (noToF && TPC_lkle[i] >= cut1[k]) || (!noToF && a[bin_mom][k]*ToF_lkle[i]+(1-a[bin_mom][k])*TPC_lkle[i] >= cut2[bin_mom][k]);
	if (particleToSel==2212)
	  selected = (noToF && TPC_lklp[i] >= cut1[k]) || (!noToF && a[bin_mom][k]*ToF_lklp[i]+(1-a[bin_mom][k])*TPC_lklp[i] >= cut2[bin_mom][k]);
				
	// to be compared with cutting on TPC likelihood
	if (particleToSel==13) 
	  selected_TPC = (TPC_lklmu[i] >= cut1[k]);
	if (particleToSel==211)
	  selected_TPC = (TPC_lklpi[i] >= cut1[k]);
	if (particleToSel==11)
	  selected_TPC = (TPC_lkle[i] >= cut1[k]);
	if (particleToSel==2212)
	  selected_TPC = (TPC_lklp[i] >= cut1[k]);
				
	selected = selected && charge==chargeToSel;
	selected_TPC = selected_TPC && charge==chargeToSel;
				
	if (selected) 
	  N_sel[k]++;
	if (selected && signal)
	  N_sigsel[k]++;
				
	if (selected_TPC) 
	  N_sel_TPC[k]++;
	if (selected_TPC && signal)
	  N_sigsel_TPC[k]++;
				
      }
    }
		
  }
       
  // compute efficiency/purity/significance
  int best_k=-1, best_k_TPC=-1;
  float best_sign=-1, best_sign_TPC=-1;

  for (int k=0; k<Ncuts; k++) {
    eff[k] = (float)N_sigsel[k]/N_sig;
    pur[k] = (float)N_sigsel[k]/N_sel[k];
    eff_TPC[k] = (float)N_sigsel_TPC[k]/N_sig;
    pur_TPC[k] = (float)N_sigsel_TPC[k]/N_sel_TPC[k];
    sign[k] = (float)N_sigsel[k]/sqrt(N_sel[k]);
    sign_TPC[k] = (float)N_sigsel_TPC[k]/sqrt(N_sel_TPC[k]);

    if (sign[k] > best_sign){
      best_sign = sign[k];
      best_k = k;
    }
    if (sign_TPC[k] > best_sign_TPC){
      best_sign_TPC = sign_TPC[k];
      best_k_TPC = k;
    }
  }

  cout << "===============================================" << endl;
  cout << "Selection of " << particleToSel << " on file " << suffix.Data() << endl;
  cout << endl;

  cout << "best working point with ToF+TPC:" << endl;
  cout << "- significance = " << best_sign << endl;
  cout << "- efficiency   = " << eff[best_k] << endl;
  cout << "- purity       = " << pur[best_k] << endl;
  for (int bin=0; bin<4; bin++){
    cout << "-- mom bin " << bin+1 << endl;
    cout << "--- a = " << a[bin][best_k] << endl;
    cout << "--- cut on a*ToF+(1-a)*TPC = " << cut2[bin][best_k] << endl;
    cout << "--- cut on TPC (if no ToF) = " << cut1[best_k] << endl;
  }

  cout << endl;

  cout << "best working point with TPC only:" << endl;
  cout << "- significance = " << best_sign_TPC << endl;
  cout << "- efficiency   = " << eff_TPC[best_k] << endl;
  cout << "- purity       = " << pur_TPC[best_k] << endl;
  cout << "--- cut on TPC = " << cut1[best_k_TPC] << endl;
  cout << endl << "===============================================" << endl; 

  TGraph *g     = new TGraph(Ncuts, pur, eff);
  TGraph *gTPC  = new TGraph(Ncuts, pur_TPC, eff_TPC);
	
  float xmin=0;
  if (particleToSel==13) 
    xmin=0.8;
  if (particleToSel==211)
    xmin=0.4;
  if (particleToSel==11)
    xmin=0.1;
  if (particleToSel==2212)
    xmin=0.75;

  TCanvas *c = new TCanvas("c");
  TH1F *h_dummy = c->DrawFrame(xmin,0,1,1);

  h_dummy->GetXaxis()->SetTitle("purity");
  h_dummy->GetXaxis()->SetTitleSize(0.045);
  h_dummy->GetYaxis()->SetTitle("efficiency");
  h_dummy->GetYaxis()->SetTitleSize(0.045);
  h_dummy->GetYaxis()->SetRangeUser(0,1.05);
	
  g->SetMarkerColor(kBlue);
  g->SetLineColor(kBlue);
  g->Draw("P same");
	
  gTPC->SetMarkerColor(kRed);
  gTPC->SetLineColor(kRed);
  gTPC->Draw("P same");
	
  TLegend *leg = new TLegend(0.1, 0.92, 0.9, 0.97);
  leg->SetNColumns(2);
  leg->AddEntry(g, "cuts on TPC and ToF lkl", "l");
  leg->AddEntry(gTPC, "cuts on TPC lkl", "l");
  leg->Draw("same");
	
  c->Update();
  c->SaveAs(TString::Format("fig/ToF_mass/effpur2D_config%i_target%i%s_pdg%i%s_mom.eps", config, target, suffix.Data(), particleToSel, onlyTB ? "_onlyTB":""));
	
}



    void plot2D(int config, int target, TString suffix, bool onlyTB) {
	
    TChain* t = new TChain("default");
    t->Add(TString::Format("jobs/files/config%i_Target%i%s.root",
			   config, target, suffix.Data()));
    TH1::AddDirectory(kFALSE);
	
    int ToF_Top=16, ToF_Bot=18;
    if (target==2)
      { ToF_Top=15; ToF_Bot=17; }
	
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
	
    int PDGs[4] = {13, 211, 2212, 11};
    int col[4] = { 2,  28,    4,  8};
    TString leg[4] = {"muons", "pions", "protons", "electrons"};
	
    TH2F* hAll_Mom1[4];
    TH2F* hAll_dedx1[4];
    TH2F* hAll_Mom2[4];
    TH2F* hAll_dedx2[4];
    TH2F* hAll_Mom3[4];
    TH2F* hAll_Mom4[4];
    TH2F* hAll_LLe[4][5];
    TH2F* hAll_LLmu[4][5];
    TH2F* hAll_LLp[4][5];
    TH2F* hAll_LLpi[4][5];
	
    for (int j=0; j<4; j++){
      hAll_Mom1[j] = new TH2F(TString::Format("h_LLe_mom1_config%i_target%i_%i",config,target,j),
			      TString::Format("h_LLe_mom1_config%i_target%i_%i",config,target,j),
			      40, 0, 1200, 20, 0, 1.01);
      hAll_dedx1[j] = new TH2F(TString::Format("h_LLe_dedx1_config%i_target%i_%i",config,target,j),
			       TString::Format("h_LLe_dedx1_config%i_target%i_%i",config,target,j),
			       40, 0, 1500, 20, 0, 1.01);
      hAll_Mom2[j] = new TH2F(TString::Format("h_LLe_mom2_config%i_target%i_%i",config,target,j),
			      TString::Format("h_LLe_mom2_config%i_target%i_%i",config,target,j),
			      40, 0, 1200, 20, 0, 1.01);
      hAll_dedx2[j] = new TH2F(TString::Format("h_LLe_dedx2_config%i_target%i_%i",config,target,j),
			       TString::Format("h_LLe_dedx2_config%i_target%i_%i",config,target,j),
			       40, 0, 1500, 20, 0, 1.01);
      hAll_Mom3[j] = new TH2F(TString::Format("h_LLe_mom3_config%i_target%i_%i",config,target,j),
			      TString::Format("h_LLe_mom3_config%i_target%i_%i",config,target,j),
			      40, 0, 1200, 20, 0, 1.01);
      hAll_Mom4[j] = new TH2F(TString::Format("h_LLe_mom4_config%i_target%i_%i",config,target,j),
			      TString::Format("h_LLe_mom4_config%i_target%i_%i",config,target,j),
			      40, 0, 1200, 20, 0, 1.01);
      for (int k=0; k<5; k++){
	hAll_LLe[j][k] = new TH2F(TString::Format("h_ToF_LLe_config%i_target%i_%i%i",config,target,j,k),
				  TString::Format("h_ToF_LLe_config%i_target%i_%i",config,target,j),
				  10, 0, 1.01, 10, 0, 1.01);
	hAll_LLmu[j][k] = new TH2F(TString::Format("h_ToF_LLmu_config%i_target%i_%i%i",config,target,j,k),
				   TString::Format("h_ToF_LLmu_config%i_target%i_%i",config,target,j),
				   10, 0, 1.01, 10, 0, 1.01);
	hAll_LLp[j][k] = new TH2F(TString::Format("h_ToF_LLp_config%i_target%i_%i%i",config,target,j,k),
				  TString::Format("h_ToF_LLp_config%i_target%i_%i",config,target,j),
				  10, 0, 1.01, 10, 0, 1.01);
	hAll_LLpi[j][k] = new TH2F(TString::Format("h_ToF_LLpi_config%i_target%i_%i%i",config,target,j,k),
				   TString::Format("h_ToF_LLpi_config%i_target%i_%i",config,target,j),
				   10, 0, 1.01, 10, 0, 1.01);
      }
    }
	
    TH2F *hPull  = new TH2F("h1", "h1", 40, 0, 1200, 50, -5, 8);
    TH2F *hMass  = new TH2F("h2", "h2", 40, 0, 1200, 40, 0, 400);
    TH2F *hLLe   = new TH2F("h3", "h3", 20, 0, 1.01, 20, 0, 1.01);
    TH2F *hLLmu  = new TH2F("h4", "h4", 20, 0, 1.01, 20, 0, 1.01);
    TH2F *hLe    = new TH2F("h5", "h5", 40, 0, 1200, 20, 0, 1.01);
    TH2F *hLmu   = new TH2F("h6", "h6", 40, 0, 1200, 20, 0, 1.01);
	
    Int_t accum_level[1][16];
    Float_t mu_mom, mom[100], mass[100], dedx[100];
    Int_t PDG[100], nTracks, det1[100], det2[100];
    Float_t ToF_pullmu[100], ToF_pulle[100], ToF_pullpi[100], ToF_pullp[100];
    Float_t ToF_lklmu[100], ToF_lkle[100], ToF_lklpi[100], ToF_lklp[100];
    Float_t TPC_lklmu[100], TPC_lkle[100], TPC_lklpi[100], TPC_lklp[100];
	
    t->SetBranchAddress("accum_level",    &accum_level);
    t->SetBranchAddress("selmu_mom",      &mu_mom);
	
    t->SetBranchAddress("selAll_PDG",     &PDG);
    t->SetBranchAddress("selAll_mom",     &mom);
    t->SetBranchAddress("selAll_nTracks", &nTracks);
    t->SetBranchAddress("selAll_ToF_det_used1", &det1);
    t->SetBranchAddress("selAll_ToF_det_used2", &det2);
    t->SetBranchAddress("selAll_ToF_mass",&mass);
	
    t->SetBranchAddress("selAll_ToF_pull_muon",     &ToF_pullmu);
    t->SetBranchAddress("selAll_ToF_pull_pion",     &ToF_pullpi);
    t->SetBranchAddress("selAll_ToF_pull_electron", &ToF_pulle);
    t->SetBranchAddress("selAll_ToF_pull_proton",   &ToF_pullp);
	
    t->SetBranchAddress("selAll_ToF_lkl_muon",      &ToF_lklmu);
    t->SetBranchAddress("selAll_ToF_lkl_pion",      &ToF_lklpi);
    t->SetBranchAddress("selAll_ToF_lkl_electron",  &ToF_lkle);
    t->SetBranchAddress("selAll_ToF_lkl_proton",    &ToF_lklp);
	
    t->SetBranchAddress("selAll_TPC_lkl_muon",      &TPC_lklmu);
    t->SetBranchAddress("selAll_TPC_lkl_pion",      &TPC_lklpi);
    t->SetBranchAddress("selAll_TPC_lkl_electron",  &TPC_lkle);
    t->SetBranchAddress("selAll_TPC_lkl_proton",    &TPC_lklp);
	
    t->SetBranchAddress("selAll_TPC_dedx",          &dedx);
	
	
    for (Int_t ient=0; ient < t->GetEntries(); ient++) {
		
      t->GetEntry(ient);
      if (accum_level[0][0]<=2)
	continue;
		
      for (Int_t i=0; i<nTracks; i++) {
			
	if (onlyTB && 
	    !(det1[i]==ToF_Bot || det1[i]==ToF_Top) && 
	    !(det2[i]==ToF_Bot || det2[i]==ToF_Top) ) 
	  continue;
			
	if (mom[i] == mu_mom){
	  hPull->Fill(mom[i],  ToF_pulle[i]);
	  hMass->Fill(mom[i],  mass[i]);
	  hLLe->Fill(ToF_lkle[i], TPC_lkle[i]);
	  hLLmu->Fill(ToF_lklmu[i], TPC_lklmu[i]);
	  hLe->Fill(mom[i],  ToF_lkle[i]);
	  hLmu->Fill(mom[i], ToF_lklmu[i]);
	}
			
	if (mom[i] > 50){
	  for (int j=0; j<4; j++){
	    if (abs(PDG[i]) != PDGs[j]) continue;
					
	    hAll_Mom1[j]->Fill(mom[i], ToF_lkle[i]);
	    hAll_dedx1[j]->Fill(dedx[i], ToF_lkle[i]);
	    hAll_Mom2[j]->Fill(mom[i], ToF_lklmu[i]);
	    hAll_dedx2[j]->Fill(dedx[i], ToF_lklmu[i]);
	    hAll_Mom3[j]->Fill(mom[i], ToF_lkle[i]*TPC_lkle[i]);
	    hAll_Mom4[j]->Fill(mom[i], ToF_lklmu[i]*TPC_lklmu[i]);
	    hAll_LLe[j][0]->Fill(ToF_lkle[i], TPC_lkle[i]);
	    hAll_LLmu[j][0]->Fill(ToF_lklmu[i], TPC_lklmu[i]);
	    hAll_LLp[j][0]->Fill(ToF_lklp[i], TPC_lklp[i]);
	    hAll_LLpi[j][0]->Fill(ToF_lklpi[i], TPC_lklpi[i]);
		
		int bin=1;
		if (mom[i]>50 && mom[i]<200) bin=1;
		if (mom[i]>200 && mom[i]<400) bin=2;
		if (mom[i]>400 && mom[i]<1000) bin=3;
		if (mom[i]>1000 && mom[i]<30000) bin=4;
	    hAll_LLe[j][bin]->Fill(ToF_lkle[i], TPC_lkle[i]);
	    hAll_LLmu[j][bin]->Fill(ToF_lklmu[i], TPC_lklmu[i]);
	    hAll_LLp[j][bin]->Fill(ToF_lklp[i], TPC_lklp[i]);
	    hAll_LLpi[j][bin]->Fill(ToF_lklpi[i], TPC_lklpi[i]);
		
	  }
	}
			
      }
		
    }
	
    TCanvas *cPull = new TCanvas();
    hPull->GetXaxis()->SetTitle("muon candidate momentum [MeV/c]");
    hPull->GetYaxis()->SetTitle("electron pull");
    hPull->Draw("colz");
    cPull->SaveAs(TString::Format("fig/ToF_mass/2D_mu/mom_vs_pullele%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));
	
    TCanvas *cMass = new TCanvas();
    hMass->GetXaxis()->SetTitle("muon candidate momentum [MeV/c]");
    hMass->GetYaxis()->SetTitle("ToF mass [MeV/c^{2}]");
    hMass->Draw("colz");
    cMass->SaveAs(TString::Format("fig/ToF_mass/2D_mu/mom_vs_mass%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));
	
    TCanvas *cLLe = new TCanvas();
    hLLe->GetXaxis()->SetTitle("L_{e}^{ToF}");
    hLLe->GetYaxis()->SetTitle("L_{e}^{TPC}");
    hLLe->Draw("colz");
    cLLe->SaveAs(TString::Format("fig/ToF_mass/2D_mu/ToF_vs_TPC_Le%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));
	
    TCanvas *cLLmu = new TCanvas();
    hLLmu->GetXaxis()->SetTitle("L_{#mu}^{ToF}");
    hLLmu->GetYaxis()->SetTitle("L_{#mu}^{TPC}");
    hLLmu->Draw("colz");
    cLLmu->SaveAs(TString::Format("fig/ToF_mass/2D_mu/ToF_vs_TPC_Lmu%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));
	
    TCanvas *cLe = new TCanvas();
    hLe->GetXaxis()->SetTitle("muon candidate momentum [MeV/c]");
    hLe->GetYaxis()->SetTitle("L_{e}^{ToF}");
    hLe->Draw("colz");
    cLe->SaveAs(TString::Format("fig/ToF_mass/2D_mu/mom_vs_Le%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));
	
    TCanvas *cLmu = new TCanvas();
    hLmu->GetXaxis()->SetTitle("muon candidate momentum [MeV/c]");
    hLmu->GetYaxis()->SetTitle("L_{#mu}^{ToF}");
    hLmu->Draw("colz");
    cLmu->SaveAs(TString::Format("fig/ToF_mass/2D_mu/mom_vs_Lmu%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));
	

    // normal 2D plots

	TString binName[5]={"","_mom1","_mom2","_mom3","_mom4"};
	
    for (int j=0; j<4; j++){
		for (int k=0; k<5; k++){
      TCanvas *c1 = new TCanvas("c1");
	hAll_LLe[j][k]->GetXaxis()->SetTitleSize(0.045);
	hAll_LLe[j][k]->GetYaxis()->SetTitleSize(0.045);
	hAll_LLe[j][k]->GetYaxis()->SetTitleOffset(1.1);
      hAll_LLe[j][k]->GetXaxis()->SetTitle("L_{e}^{ToF}");
      hAll_LLe[j][k]->GetYaxis()->SetTitle("L_{e}^{TPC}");
	  hAll_LLe[j][k]->SetContour(100);
      hAll_LLe[j][k]->Draw("colz");
      c1->SaveAs(TString::Format("fig/ToF_mass/2D_all/ToF_vs_TPC_Le_true%i%s%s%s.eps", PDGs[j], suffix.Data(), onlyTB ? "_onlyTB":"", binName[k].Data()));
		
      TCanvas *c2 = new TCanvas("c2");
	hAll_LLmu[j][k]->GetXaxis()->SetTitleSize(0.045);
	hAll_LLmu[j][k]->GetYaxis()->SetTitleSize(0.045);
	hAll_LLmu[j][k]->GetYaxis()->SetTitleOffset(1.1);
      hAll_LLmu[j][k]->GetXaxis()->SetTitle("L_{#mu}^{ToF}");
      hAll_LLmu[j][k]->GetYaxis()->SetTitle("L_{#mu}^{TPC}");
	  hAll_LLmu[j][k]->SetContour(100);
      hAll_LLmu[j][k]->Draw("colz");
      c2->SaveAs(TString::Format("fig/ToF_mass/2D_all/ToF_vs_TPC_Lmu_true%i%s%s%s.eps", PDGs[j], suffix.Data(), onlyTB ? "_onlyTB":"", binName[k].Data()));
		
      TCanvas *c3 = new TCanvas("c3");
	hAll_LLp[j][k]->GetXaxis()->SetTitleSize(0.045);
	hAll_LLp[j][k]->GetYaxis()->SetTitleSize(0.045);
	hAll_LLp[j][k]->GetYaxis()->SetTitleOffset(1.1);
      hAll_LLp[j][k]->GetXaxis()->SetTitle("L_{p}^{ToF}");
      hAll_LLp[j][k]->GetYaxis()->SetTitle("L_{p}^{TPC}");
	  hAll_LLp[j][k]->SetContour(100);
      hAll_LLp[j][k]->Draw("colz");
      c3->SaveAs(TString::Format("fig/ToF_mass/2D_all/ToF_vs_TPC_Lp_true%i%s%s%s.eps", PDGs[j], suffix.Data(), onlyTB ? "_onlyTB":"", binName[k].Data()));
		
      TCanvas *c4 = new TCanvas("c4");
	hAll_LLpi[j][k]->GetXaxis()->SetTitleSize(0.045);
	hAll_LLpi[j][k]->GetYaxis()->SetTitleSize(0.045);
	hAll_LLpi[j][k]->GetYaxis()->SetTitleOffset(1.1);
      hAll_LLpi[j][k]->GetXaxis()->SetTitle("L_{#pi}^{ToF}");
      hAll_LLpi[j][k]->GetYaxis()->SetTitle("L_{#pi}^{TPC}");
	  hAll_LLpi[j][k]->SetContour(100);
      hAll_LLpi[j][k]->Draw("colz");
      c4->SaveAs(TString::Format("fig/ToF_mass/2D_all/ToF_vs_TPC_Lpi_true%i%s%s%s.eps", PDGs[j], suffix.Data(), onlyTB ? "_onlyTB":"", binName[k].Data()));
    }
	}

	
    // superimposed 2D plots
	
    float max=0;
    for (int j=0; j<4; j++){
      if (hAll_Mom1[j]->GetMaximum() > max)
	max = hAll_Mom1[j]->GetMaximum();
    }
	
    TLegend *legendMom = new TLegend(0.1, 0.9, 0.9, 0.95);
    legendMom->SetNColumns(4);
	
    TCanvas *cAll_Mom1 = new TCanvas("c2D_Mom1");
    for (int j=0; j<4; j++){
		
      legendMom->AddEntry(hAll_Mom1[j], leg[j].Data(), "f");
      hAll_Mom1[j]->SetMarkerColor(col[j]);
      hAll_Mom1[j]->SetLineColor(col[j]);
      hAll_Mom1[j]->SetFillColor(col[j]);
      hAll_Mom1[j]->GetXaxis()->SetTitle("momentum [MeV/c]");
      hAll_Mom1[j]->GetXaxis()->SetTitleSize(0.045);
      hAll_Mom1[j]->GetYaxis()->SetTitle("L_{e}^{ToF}");
      hAll_Mom1[j]->GetYaxis()->SetTitleSize(0.045);
      hAll_Mom1[j]->GetYaxis()->SetTitleOffset(1.1);
		
      TCanvas *c = new TCanvas("c");
      hAll_Mom1[j]->SetContour(100);
      hAll_Mom1[j]->Draw("colz");
      c->SaveAs(TString::Format("fig/ToF_mass/2D_all/mom_vs_Le_pdg%i%s%s.eps", PDGs[j], suffix.Data(), onlyTB ? "_onlyTB":""));		
		
      hAll_Mom1[j]->SetMaximum(1.1*max);	
      cAll_Mom1->cd();
		
      if (j==0)
	hAll_Mom1[j]->Draw("box");
      else
	hAll_Mom1[j]->Draw("box same");
		
    }
    legendMom->Draw("same");
    cAll_Mom1->SaveAs(TString::Format("fig/ToF_mass/2D_all/mom_vs_Le%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));
	
	
    max=0;
    for (int j=0; j<4; j++){
      if (hAll_dedx1[j]->GetMaximum() > max)
	max = hAll_dedx1[j]->GetMaximum();
    }
	
    TLegend *legenddedx = new TLegend(0.1, 0.9, 0.9, 0.95);
    legenddedx->SetNColumns(4);
	
    TCanvas *cAll_dedx1 = new TCanvas("c2D_dedx1");
    for (int j=0; j<4; j++){
		
      legenddedx->AddEntry(hAll_dedx1[j], leg[j].Data(), "f");
      hAll_dedx1[j]->SetMarkerColor(col[j]);
      hAll_dedx1[j]->SetLineColor(col[j]);
      hAll_dedx1[j]->SetFillColor(col[j]);
      hAll_dedx1[j]->SetMaximum(1.1*max);
		
		
      hAll_dedx1[j]->GetXaxis()->SetTitle("dE/dx [ADC]");
      hAll_dedx1[j]->GetXaxis()->SetTitleSize(0.045);
      hAll_dedx1[j]->GetYaxis()->SetTitle("L_{e}^{ToF}");
      hAll_dedx1[j]->GetYaxis()->SetTitleSize(0.045);
      hAll_dedx1[j]->GetYaxis()->SetTitleOffset(1.1);
		
      if (j==0)	
	hAll_dedx1[j]->Draw("box");
      else
	hAll_dedx1[j]->Draw("box same");
		
    } 
    legenddedx->Draw("same");
    cAll_dedx1->SaveAs(TString::Format("fig/ToF_mass/2D_all/dedx_vs_Le%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));
	
	
    max=0;
    for (int j=0; j<4; j++){
      if (hAll_Mom2[j]->GetMaximum() > max)
	max = hAll_Mom2[j]->GetMaximum();
    }
	
    TCanvas *cAll_Mom2 = new TCanvas("c2D_Mom2");
    for (int j=0; j<4; j++){
		
      hAll_Mom2[j]->SetMarkerColor(col[j]);
      hAll_Mom2[j]->SetLineColor(col[j]);
      hAll_Mom2[j]->SetFillColor(col[j]);
      hAll_Mom2[j]->GetXaxis()->SetTitle("momentum [MeV/c]");
      hAll_Mom2[j]->GetXaxis()->SetTitleSize(0.045);
      hAll_Mom2[j]->GetYaxis()->SetTitle("L_{#mu}^{ToF}");
      hAll_Mom2[j]->GetYaxis()->SetTitleSize(0.045);
      hAll_Mom2[j]->GetYaxis()->SetTitleOffset(1.1);
		
      TCanvas *c = new TCanvas("c");
      hAll_Mom2[j]->SetContour(100);
      hAll_Mom2[j]->Draw("colz");
      c->SaveAs(TString::Format("fig/ToF_mass/2D_all/mom_vs_Lmu_pdg%i%s%s.eps", PDGs[j], suffix.Data(), onlyTB ? "_onlyTB":""));
		
      hAll_Mom2[j]->SetMaximum(1.1*max);	
      cAll_Mom2->cd();
		
      if (j==0)			
	hAll_Mom2[j]->Draw("box");
      else
	hAll_Mom2[j]->Draw("box same");		
		
    }
    legendMom->Draw("same");
    cAll_Mom2->SaveAs(TString::Format("fig/ToF_mass/2D_all/mom_vs_Lmu%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));
	
	
    max=0;
    for (int j=0; j<4; j++){
      if (hAll_dedx2[j]->GetMaximum() > max)
	max = hAll_dedx2[j]->GetMaximum();
    }
	
    TCanvas *cAll_dedx2 = new TCanvas("c2D_dedx2");
    for (int j=0; j<4; j++){
		
      hAll_dedx2[j]->SetMarkerColor(col[j]);
      hAll_dedx2[j]->SetLineColor(col[j]);
      hAll_dedx2[j]->SetFillColor(col[j]);
      hAll_dedx2[j]->SetMaximum(1.1*max);
		
      if (j==0){
	hAll_dedx2[j]->GetXaxis()->SetTitle("dE/dx [ADC]");
	hAll_dedx2[j]->GetXaxis()->SetTitleSize(0.045);
	hAll_dedx2[j]->GetYaxis()->SetTitle("L_{#mu}^{ToF}");
	hAll_dedx2[j]->GetYaxis()->SetTitleSize(0.045);
	hAll_dedx2[j]->GetYaxis()->SetTitleOffset(1.1);
	hAll_dedx2[j]->Draw("box");
      }
      else
	hAll_dedx2[j]->Draw("box same");
		
    } 
    legenddedx->Draw("same");
    cAll_dedx2->SaveAs(TString::Format("fig/ToF_mass/2D_all/dedx_vs_Lmu%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));
	
	
	
    max=0;
    for (int j=0; j<4; j++){
      if (hAll_Mom3[j]->GetMaximum() > max)
	max = hAll_Mom3[j]->GetMaximum();
    }
		
    TCanvas *cAll_Mom3 = new TCanvas("c2D_Mom3");
    for (int j=0; j<4; j++){
		
      hAll_Mom3[j]->SetMarkerColor(col[j]);
      hAll_Mom3[j]->SetLineColor(col[j]);
      hAll_Mom3[j]->SetFillColor(col[j]);
      hAll_Mom3[j]->GetXaxis()->SetTitle("momentum [MeV/c]");
      hAll_Mom3[j]->GetXaxis()->SetTitleSize(0.045);
      hAll_Mom3[j]->GetYaxis()->SetTitle("L_{e}^{ToF} L_{e}^{TPC}");
      hAll_Mom3[j]->GetYaxis()->SetTitleSize(0.045);
      hAll_Mom3[j]->GetYaxis()->SetTitleOffset(1.1);
		
      TCanvas *c = new TCanvas("c");
      hAll_Mom3[j]->SetContour(100);
      hAll_Mom3[j]->Draw("colz");
      c->SaveAs(TString::Format("fig/ToF_mass/2D_all/mom_vs_LLe_pdg%i%s%s.eps", PDGs[j], suffix.Data(), onlyTB ? "_onlyTB":""));		
		
      hAll_Mom3[j]->SetMaximum(1.1*max);	
      cAll_Mom3->cd();
		
      if (j==0)
	hAll_Mom3[j]->Draw("box");
      else
	hAll_Mom3[j]->Draw("box same");
		
    }
    legendMom->Draw("same");
    cAll_Mom3->SaveAs(TString::Format("fig/ToF_mass/2D_all/mom_vs_LLe%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));



    max=0;
    for (int j=0; j<4; j++){
      if (hAll_Mom4[j]->GetMaximum() > max)
	max = hAll_Mom4[j]->GetMaximum();
    }

    TCanvas *cAll_Mom4 = new TCanvas("c2D_Mom4");
    for (int j=0; j<4; j++){
		
      hAll_Mom4[j]->SetMarkerColor(col[j]);
      hAll_Mom4[j]->SetLineColor(col[j]);
      hAll_Mom4[j]->SetFillColor(col[j]);
      hAll_Mom4[j]->GetXaxis()->SetTitle("momentum [MeV/c]");
      hAll_Mom4[j]->GetXaxis()->SetTitleSize(0.045);
      hAll_Mom4[j]->GetYaxis()->SetTitle("L_{#mu}^{ToF} L_{#mu}^{TPC}");
      hAll_Mom4[j]->GetYaxis()->SetTitleSize(0.045);
      hAll_Mom4[j]->GetYaxis()->SetTitleOffset(1.1);
		
      TCanvas *c = new TCanvas("c");
      hAll_Mom4[j]->SetContour(100);
      hAll_Mom4[j]->Draw("colz");
      c->SaveAs(TString::Format("fig/ToF_mass/2D_all/mom_vs_LLmu_pdg%i%s%s.eps", PDGs[j], suffix.Data(), onlyTB ? "_onlyTB":""));		
		
      hAll_Mom4[j]->SetMaximum(1.1*max);	
      cAll_Mom4->cd();
		
      if (j==0)
	hAll_Mom4[j]->Draw("box");
      else
	hAll_Mom4[j]->Draw("box same");
		
    }
    legendMom->Draw("same");
    cAll_Mom4->SaveAs(TString::Format("fig/ToF_mass/2D_all/mom_vs_LLmu%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));

	
	
    TLegend *legendLLe = new TLegend(0.1, 0.9, 0.9, 0.95);
    legendLLe->SetNColumns(4);
	
    TCanvas *cAll_LLe = new TCanvas("c2D_LLe");
    for (int j=0; j<4; j++){
		
      legendLLe->AddEntry(hAll_LLe[j][0], leg[j].Data(), "f");
      hAll_LLe[j][0]->SetMarkerColor(col[j]);
      hAll_LLe[j][0]->SetLineColor(col[j]);
      hAll_LLe[j][0]->SetFillColor(col[j]);
      hAll_LLe[j][0]->SetMaximum(1.1*max);
		
      if (j==0)
	hAll_LLe[j][0]->Draw("box");
      else
	hAll_LLe[j][0]->Draw("box same");
		
    } 
    legendLLe->Draw("same");
    cAll_LLe->SaveAs(TString::Format("fig/ToF_mass/2D_all/ToF_vs_TPC_Le_all%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));
	
    TLegend *legendLLmu = new TLegend(0.1, 0.9, 0.9, 0.95);
    legendLLmu->SetNColumns(4);
	
    TCanvas *cAll_LLmu = new TCanvas("c2D_LLmu");
    for (int j=0; j<4; j++){
		
      legendLLmu->AddEntry(hAll_LLmu[j][0], leg[j].Data(), "f");
      hAll_LLmu[j][0]->SetMarkerColor(col[j]);
      hAll_LLmu[j][0]->SetLineColor(col[j]);
      hAll_LLmu[j][0]->SetFillColor(col[j]);
      hAll_LLmu[j][0]->SetMaximum(1.1*max);
		
      if (j==0)
	hAll_LLmu[j][0]->Draw("box");
      else
	hAll_LLmu[j][0]->Draw("box same");
		
    } 
    legendLLmu->Draw("same");
    cAll_LLmu->SaveAs(TString::Format("fig/ToF_mass/2D_all/ToF_vs_TPC_Lmu_all%s%s.eps", suffix.Data(), onlyTB ? "_onlyTB":""));
	
	
      
	
  }


  void plotAll_time(int ic=1, int it=1) {
	
    TString suffix[6] = {"_600ps", "_150ps", "_50ps", "_600ps_newToF", "_150ps_newToF", "_50ps_newToF"};
    for (int s=0; s<6; s++) {
      for (int onlyTB=0; onlyTB<=1; onlyTB++) {
	plot_ToF_mass(ic, it, suffix[s], false, onlyTB, false);
	plot_ToF_mass(ic, it, suffix[s], true, onlyTB, false);
	plot_ToF_mass(ic, it, suffix[s], false, onlyTB, true);
	plot_ToF_mass(ic, it, suffix[s], true, onlyTB, true);

	plot_ToF_mass(ic, it, suffix[s], false, onlyTB, false, 50, 200);
	plot_ToF_mass(ic, it, suffix[s], false, onlyTB, false, 200, 400);
	plot_ToF_mass(ic, it, suffix[s], false, onlyTB, false, 400, 1000);
	plot_ToF_mass(ic, it, suffix[s], false, onlyTB, false, 1000, 30000);
      }
    }
	
  }

  void plotAll_pull(int ic=1, int it=1) {
	
    int pullPDG[4] = {13, 211, 11, 2212};
	
    TString suffix[6] = {"_600ps", "_150ps", "_50ps", "_600ps_newToF", "_150ps_newToF", "_50ps_newToF"};
    for (int s=0; s<6; s++) {
      for (int i=0; i<4; i++) {
	/*
	for (int j=0; j<3; j++) {
	  plot_ToF_pull(ic, it, pullPDG[i], suffix[s], false, j);
	  plot_ToF_pull(ic, it, pullPDG[i], suffix[s], false, j, 50, 200);
	  plot_ToF_pull(ic, it, pullPDG[i], suffix[s], false, j, 200, 400);
	  plot_ToF_pull(ic, it, pullPDG[i], suffix[s], false, j, 400, 1000);
	  plot_ToF_pull(ic, it, pullPDG[i], suffix[s], false, j, 1000, 30000);
	}
	*/		
	plot_ToF_EffPur(ic, it, pullPDG[i], suffix[s], false);
			
      }
    }
	
  }


  void plotAll_2D(int ic=1, int it=1){
	
    TString suffix[6] = {"_600ps", "_150ps", "_50ps", "_600ps_newToF", "_150ps_newToF", "_50ps_newToF"};
    for (int s=0; s<6; s++) {
      plot2D(ic, it, suffix[s], false);
      plot2D(ic, it, suffix[s], true);
    }
	
  }


  void plotAll(int ic=1, int it=1) {
	
    plotAll_time(ic, it);
    plotAll_pull(ic, it);
    plotAll_2D(ic, it);
	
  }
