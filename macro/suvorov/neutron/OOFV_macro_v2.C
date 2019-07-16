#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TVector3.h"

#include "TH2F.h"

#include "TRandom3.h"

#include <iostream>

using namespace std;

int OOFV_macro_v2() {

  TFile* file_bg = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/reco/SuperFGD-neutron_OOFV_v3-UseXY-UseXZ-UseYZ-Separate10_na_1368000.root", "READ");

  TTree* tree_bg = (TTree*)file_bg->Get("neutron");

  TFile* signal_f = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/spectra.root", "READ");
  TH2F* signal_h = (TH2F*)signal_f->Get("muon_2d");

  TRandom3* gen = new TRandom3(0);

  TFile* file_out = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/plot/plot_OOFV_v3.root", "RECREATE");
  TH2F* LeverArm_time     = new TH2F("LAvsTime", "Time vs Lever arm", 300, 0., 300., 500, -200., 300.);
  TH2F* LeverArm_time_true= new TH2F("LAvsTime_true", "Time vs Lever arm", 300, 0., 300., 500, -200., 300.);

  TH2F* hits_XY = new TH2F("hits_XY", "", 192, -96, 96., 58, -29, 29.);
  TH2F* hits_XZ = new TH2F("hits_XZ", "", 192, -96, 96., 210, -105, 105.);
  TH2F* hits_YZ = new TH2F("hits_YZ", "", 58, -29, 29., 210, -105, 105.);

  TH1F* pt = new TH1F("pt", "Transverse momentum", 500, 0., 500.);

  Int_t n_fibers_cut;
  Double_t hit_pos[3];
  Double_t first_hit_time;
  Double_t light_tot;

  tree_bg->SetBranchAddress("First_hit_time",     &first_hit_time);
  tree_bg->SetBranchAddress("Hit_pos",            &hit_pos);
  tree_bg->SetBranchAddress("N_fibers",       &n_fibers_cut);
  //tree_bg->SetBranchAddress("N_fibers_cut",       &n_fibers_cut);
  tree_bg->SetBranchAddress("Light_tot",          &light_tot);

  Int_t Nevents = tree_bg->GetEntries();
  //Nevents = 20;

  cout << "[                     ] N = " << Nevents << "\r[";

  for (Int_t i = 0; i < Nevents; ++i) {

    if (i%(int(Nevents/20)) == 0)
      cout << "." << flush;

    tree_bg->GetEntry(i);

    // generate random point for vertex and random time
    Float_t v_time  = gen->Gaus(200., 18.);
    Float_t v_x     = (gen->Uniform() - 0.5)* 2040.;
    Float_t v_y     = (gen->Uniform() - 0.5)* 520.;
    Float_t v_z     = (gen->Uniform() - 0.5)* 1880.;

    TVector3 vertex(v_x, v_y, v_z);
    TVector3 hit(hit_pos[0], hit_pos[1], hit_pos[2]);

    //cout << "v " << v_x << "\t" << v_y << "\t" << v_z << endl;
    //cout << "h " << hit_pos[0] << "\t" << hit_pos[1] << "\t" << hit_pos[2] << endl;

    Float_t LA = (hit - vertex).Mag() / 10.;
    Float_t T  = first_hit_time - v_time;

    LeverArm_time_true->Fill(LA, T);

    if (light_tot / 3 < 40.)
      continue;
    if (!n_fibers_cut)
      continue;

    Float_t res   = 0.95;
    res           *= sqrt(1. / n_fibers_cut);
    Float_t first_time = gen->Gaus(first_hit_time, res);
    T  = first_time - v_time;

    LeverArm_time->Fill(LA, T);

    hits_XY->Fill(hit_pos[0] / 10, (hit_pos[1]) / 10);
    hits_XZ->Fill(hit_pos[0] / 10, (hit_pos[2]) / 10);
    hits_YZ->Fill((hit_pos[1]) / 10, (hit_pos[2]) / 10);

    // STV studies

    Double_t mom, costheta, sintheta;
    signal_h->GetRandom2(costheta, mom);
    sintheta = sqrt(1 - costheta*costheta);

    Float_t phi = (gen->Uniform() - 0.5) * 2*TMath::Pi();

    TVector3 dir(sintheta*std::cos(phi), sintheta*std::sin(phi), costheta);
    // speed of light 30 cm/ns
    Float_t beta = LA / (T * 30.);
    Float_t m = 939.565379;
    Float_t e_tot = sqrt(m*m / (1 - beta*beta));

    Float_t mom_n = sqrt( e_tot*e_tot - m*m );

    TVector3 mom_n_v = (hit - vertex).Unit();
    mom_n_v *= mom_n;
    dir *= mom;

    pt->Fill((dir - mom_n_v).Mag());

  } // loop over events

  cout << "]" << endl;

  LeverArm_time->Write();
  LeverArm_time_true->Write();
  pt->Write();

  hits_XY->Write();
  hits_XZ->Write();
  hits_YZ->Write();

  file_out->Close();

  exit(1);
}