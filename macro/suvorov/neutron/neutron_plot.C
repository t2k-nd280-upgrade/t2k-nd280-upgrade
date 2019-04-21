#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TPad.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TRandom3.h"

#include <iostream>

using namespace std;

void neutron_plot() {
  TFile* file = new TFile("~/node_output/ndUP/SuperFGD/neutron/plot_neutron_v25.root", "READ");

  const Int_t time_size = 7;

  TString prefix = "~/FIGURE/neutron/";

  const Int_t Ndist = 9;
  float distance_cut[Ndist];
  distance_cut[0] = 10.;
  distance_cut[1] = 20.;
  distance_cut[2] = 30.;
  distance_cut[3] = 40.;
  distance_cut[4] = 50.;
  distance_cut[5] = 60.;
  distance_cut[6] = 70.;
  distance_cut[7] = 0.;
  // 50 - 100 cm
  distance_cut[8] = 100.;

  TCanvas* c1 = new TCanvas("canva","",50,50,1000,800);

  TH2F* energy_resol[time_size];
  TH1F* energy_acc[time_size];
  TGraphAsymmErrors* graph_resol[time_size];

  for (Int_t i = 0; i < time_size; ++i) {
    cout << "Working on " << i << endl;
    energy_resol[i]       = (TH2F*)file->Get((Form("CutValue_50/energy_smearng_%i", i)));
    energy_acc[i]         = (TH1F*)file->Get((Form("CutValue_50/energy_acc_%i", i)));
    graph_resol[i]        = (TGraphAsymmErrors*)file->Get(Form("CutValue_50/resol_graph_%i", i));

    energy_resol[i]->Draw("colz");
    energy_resol[i]->GetYaxis()->SetRangeUser(0., 300);
    energy_resol[i]->GetXaxis()->SetRangeUser(0., 300);
    gPad->Update();
    c1->Print((prefix + Form("Smearing_%i.pdf", i)).Data());

    energy_acc[i]->Draw();
    c1->Print((prefix + Form("Acc_%i.pdf", i)).Data());

    graph_resol[i]->Draw("ap");
    graph_resol[i]->GetYaxis()->SetRangeUser(0., 1.);
    graph_resol[i]->GetXaxis()->SetRangeUser(0., 500.);
    gPad->Update();
    c1->Print((prefix + Form("Resol_%i.pdf", i)).Data());
  }
}