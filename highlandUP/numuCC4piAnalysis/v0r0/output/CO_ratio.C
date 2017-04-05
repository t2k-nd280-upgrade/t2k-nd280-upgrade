#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <list>
#include <vector>
#include <string>
#include <cstdlib>

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TString.h"
#include "TLatex.h"

double sizeXYZ[4][3][3] = {{{1864.3, 1864.3,  303.0}, {1864.3, 1864.3,  303.0}, {     0,      0,      0}},
			   {{1864.0,  600.0, 1300.0}, {1864.0,  600.0, 1300.0}, {     0,      0,      0}},
			   {{1864.3, 1864.3,  303.0}, {1864.3, 1864.3,  303.0}, {1864.0,  600.0, 1994.0}},
			   {{1864.3, 1864.3,  303.0}, {1864.3, 1864.3,  303.0}, {1864.0,  600.0, 1994.0}}}; // mm

double density[4][3] = {{1.032, 1.027,      0},
			{1.000, 0.3096,     0},
			{1.032, 1.027,  1.000},
			{1.032, 1.027,  1.000}}; // g/cm3

double massfraction_C_O[4][3][2] = {{{92.2577,             0}, {50.1400, 40.7400}, {         0,          0}},
				    {{30*92/100., 70*88/100.}, {91.4708,       0}, {         0,          0}},
				    //{{      0, 88.8102}, {91.4708,       0}, {      0,       0}},
				    {{92.2577,             0}, {50.1400, 40.7400}, {30*92/100., 70*88/100.}},
				    {{92.2577,             0}, {50.1400, 40.7400}, {30*92/100., 70*88/100.}}};

const double N_A = 6.0222e23; // /mol
const double M_O = 15.9994;   // g/mol
const double M_C = 12.0107;   // g/mol

TString topo_name[4] = {"CC-0#pi", "CC-1#pi^{+}", "CC-other", "CC-inclusive"};
TString conf_name[4] = {"current-like", "reference", "alt. Target #rightarrow TPC", "alt. TPC #rightarrow Target"};

int NTargets[4] = {2, 2, 3, 3};

TH1F* computeRatioSelSimNorm(int nbins, double *xbins, TString var, TString var_title, float POT=1e21, bool flag_cutC_FGD2only = false) {

  int pdg_nucleus[2] = {1000060120, 1000080160};

  gStyle->SetOptTitle(0);
  double N_X_Target[4][3][2];
  double N_X[4][2];

  for (int config=0; config<4; config++) {
    N_X[config][0]=N_X[config][1]=0;
    for (int target=0; target<NTargets[config]; target++) {
      N_X_Target[config][target][0] = 6*sizeXYZ[config][target][0]*sizeXYZ[config][target][1]*sizeXYZ[config][target][2]*density[config][target]*massfraction_C_O[config][target][0]*N_A/M_C/100.;
      N_X_Target[config][target][1] = 8*sizeXYZ[config][target][0]*sizeXYZ[config][target][1]*sizeXYZ[config][target][2]*density[config][target]*massfraction_C_O[config][target][1]*N_A/M_O/100.;
      
      N_X[config][0] += N_X_Target[config][target][0];
      N_X[config][1] += N_X_Target[config][target][1];

    }
  }

  TH1F* h_CCinc[4][3][2];
  TH1F* h_CC0pi[4][3][2];

  TH1F* h_ratio_CCinc[4];
  TH1F* h_ratio_CC0pi[4];
  
  for (int config=0; config<4; config++) {
    for (int target=0; target<NTargets[config]; target++) {
      TFile* f = new TFile(TString::Format("jobs/files/config%i_Target%i.root", config, target+1));
      TTree* t = (TTree*)f->Get("truth");

      // fill the histograms
      for (int i=0; i<2; i++) {

	h_CCinc[config][target][i] = new TH1F(TString::Format("hinc%i%i%i",config,target,i), TString::Format("hinc%i%i%i",config,target,i), nbins, xbins);
	h_CC0pi[config][target][i] = new TH1F(TString::Format("h0pi%i%i%i",config,target,i), TString::Format("h0pi%i%i%i",config,target,i), nbins, xbins);
	t->Project(TString::Format("hinc%i%i%i",config,target,i),var,TString::Format("(accum_level[12]>4 || accum_level[13]>4 || accum_level[14]>4 || accum_level[15]>4) && true_Target_pdg==%i", pdg_nucleus[i]));
	t->Project(TString::Format("h0pi%i%i%i",config,target,i),var,TString::Format("(accum_level[0]>5 || accum_level[1]>5 || accum_level[2]>5 || accum_level[3]>5) && true_Target_pdg==%i", pdg_nucleus[i]));
	h_CCinc[config][target][i]->Sumw2();
	h_CC0pi[config][target][i]->Sumw2();

      }

      //rescale Carbon-Oxygen components for water target
      if ((config==1 && target==0) || (config==2 && target==2) || (config==3 && target==2)) {
	h_CCinc[config][target][1]->Scale(0.7); // scale water component done
	h_CC0pi[config][target][1]->Scale(0.7); 
	h_CCinc[config][target][0]->Add(h_CCinc[config][target][1], 0.3/0.7); // scale carbon component up
	h_CC0pi[config][target][0]->Add(h_CC0pi[config][target][1], 0.3/0.7);	
      }

      
    }

    // if asked, kill FG1 contribution
    if (flag_cutC_FGD2only && config==0) {
      h_CCinc[config][0][0]->Scale(0.);
      h_CCinc[config][0][1]->Scale(0.);
      h_CC0pi[config][0][0]->Scale(0.);
      h_CC0pi[config][0][1]->Scale(0.);
      N_X[config][0] = N_X_Target[config][1][0];
      N_X[config][1] = N_X_Target[config][1][1];
    }    

    for (int i=0; i<2; i++) {
      // sum carbon/oxygen contributions from different targets
      for (int target=1; target<NTargets[config]; target++) {
	h_CCinc[config][0][i]->Add(h_CCinc[config][target][i]);
	h_CC0pi[config][0][i]->Add(h_CC0pi[config][target][i]);
      }
    }

    float num_C = h_CCinc[config][0][0]->Integral(0, nbins+1);
    float num_O = h_CCinc[config][0][1]->Integral(0, nbins+1);

    float R = num_C/num_O*(N_X[config][1]/N_X[config][0]);
    float err = 100*sqrt(1e21/POT)*sqrt(1/num_C+1/num_O);

    if (flag_cutC_FGD2only && config==0)
      err = 100*sqrt(1e21/POT)*sqrt(4/num_C+1/num_O);

    // print information
    cout << conf_name[config] << " & ";
    cout << (int)(num_C*POT/1e21) << " & " << (int)(num_O*POT/1e21) << " & ";
    cout << setprecision(3);
    cout << err << " & " << 100*sqrt(1e21/POT)*sqrt(1/num_O) << " & " << 100*sqrt(1e21/POT)*sqrt(1/num_C) << " \\\\" << endl;
    
    // rescale to the number of neutrons
    for (int i=0; i<2; i++) {
      h_CCinc[config][0][i]->Scale(1./(N_X[config][i]));
      h_CC0pi[config][0][i]->Scale(1./(N_X[config][i])); 
    }

    // compute ratio histogram
    h_ratio_CCinc[config] = new TH1F(TString::Format("hratio_inc_%i", config), TString::Format("hratio_inc_%i", config), nbins, xbins);
    h_ratio_CCinc[config]->Divide(h_CCinc[config][0][0], h_CCinc[config][0][1]);
    h_ratio_CC0pi[config] = new TH1F(TString::Format("hratio_0pi_%i", config), TString::Format("hratio_0pi_%i", config), nbins, xbins);
    h_ratio_CC0pi[config]->Divide(h_CC0pi[config][0][0], h_CC0pi[config][0][1]);    

    // change histogram from plotting the ratio to plot the error on the ratio instead
    for (int ibin=1; ibin<=nbins; ibin++) {
      h_ratio_CCinc[config]->SetBinContent(ibin, sqrt(1e21/POT)*h_ratio_CCinc[config]->GetBinError(ibin));
      h_ratio_CCinc[config]->SetBinError(ibin, 0);
      h_ratio_CC0pi[config]->SetBinContent(ibin, sqrt(1e21/POT)*h_ratio_CC0pi[config]->GetBinError(ibin));
      h_ratio_CC0pi[config]->SetBinError(ibin, 0);
    }
      
  }

  TCanvas *c = new TCanvas("c", "c");
  TH1F* h_dummy = c->DrawFrame(xbins[0], 0, xbins[nbins], 0.08);
  h_dummy->GetXaxis()->SetTitle(var_title);
  h_dummy->GetXaxis()->SetTitleSize(0.05);
  h_dummy->GetXaxis()->SetTitleOffset(0.9);
  h_dummy->GetYaxis()->SetTitleSize(0.05);
  h_dummy->GetYaxis()->SetTitle("stat. error on C/O ratio");

  TLegend *leg = new TLegend(0.1, 0.85, 0.9, 0.98);
  leg->SetNColumns(2);
  
  int color=0;
  for (int config=0; config<4; config++) {
    h_ratio_CC0pi[config]->SetLineColor(++color);
    h_ratio_CC0pi[config]->SetLineStyle(2);
    h_ratio_CC0pi[config]->Draw("same");
    h_ratio_CCinc[config]->SetLineColor(color);
    h_ratio_CCinc[config]->Draw("same");
    leg->AddEntry(h_ratio_CCinc[config], TString::Format("%s, CC-inc",  conf_name[config].Data()));
    leg->AddEntry(h_ratio_CC0pi[config], TString::Format("%s, CC-0#pi", conf_name[config].Data()));
  }
  leg->Draw("same");
  
  
  c->Update();
  c->SaveAs(TString::Format("fig/CO/CO_ratio_%s_%.0ePOT.eps", var.Data(), POT));

}


void plotRatioRecoSim(float POT=1e21, bool flag_cutC_FGD2only = false) {

  const int NBins_CosTh = 10;
  double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.8,-0.6,-0.4,-0.2,
					  0.0,0.2,0.4,0.6,0.8,1.0};

  const int NBins_Mom = 13;
  double BinEdges_Mom[NBins_Mom+1] = {0,100,200,300,400,500,750,
				      1000,1500,2000,2500,3000,4000,5000};

  const int NBins_Q2 = 12;
  double BinEdges_Q2[NBins_Q2+1] = {0, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1, 1.5};

  computeRatioSelSimNorm(NBins_CosTh, BinEdges_CosTh, "true_costheta", "true cos#theta",             POT, flag_cutC_FGD2only);
  computeRatioSelSimNorm(NBins_Mom,   BinEdges_Mom,   "true_mom",      "true p_{#mu} [MeV/c]",       POT, flag_cutC_FGD2only);
  computeRatioSelSimNorm(NBins_Q2,    BinEdges_Q2,    "true_Q2",       "true Q^{2} [GeV^{2}/c^{2}]", POT, flag_cutC_FGD2only);

}
