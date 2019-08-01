#include "TFile.h"
#include "TChain.h"
#include "TVector3.h"
#include "TH2F.h"

#include "TRandom3.h"

#include <iostream>

using namespace std;

const double PileUp_proc = 0.02133;
const double Dist_cut = 20.;

void OOFV_signal_merge_v2() {

	TFile* file_bg = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/reco/SuperFGD-neutron_OOFV_v3-UseXY-UseXZ-UseYZ-Separate10_na_1368000.root", "READ");

  TTree* tree_bg = (TTree*)file_bg->Get("neutron"); 

  Int_t n_fibers_cut;
  Double_t hit_pos[3];
  Double_t first_hit_time;
  Double_t light_tot;

  tree_bg->SetBranchAddress("First_hit_time",     &first_hit_time);
  tree_bg->SetBranchAddress("Hit_pos",            &hit_pos);
  tree_bg->SetBranchAddress("N_fibers",       &n_fibers_cut);
  //tree_bg->SetBranchAddress("N_fibers_cut",       &n_fibers_cut);
  tree_bg->SetBranchAddress("Light_tot",          &light_tot);

  TFile* signal_file = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/reco/SuperFGD-neutron_v30-UseXY-UseXZ-UseYZ-Separate10_na_1000000.root", "READ");
  TTree* tree       = (TTree*)signal_file->Get("neutron");

  Double_t ekin, costheta, LA_s, dist, dt_s, neutron_time, neutron_dist_true;
  Double_t vertex_pos[3];
  Int_t n_fibers_s;
  Double_t light_tot_s;

  tree->SetBranchAddress("First_hit_time",     &dt_s);
  tree->SetBranchAddress("Vertex_pos",					vertex_pos);
  tree->SetBranchAddress("Distance_cubes",		 &LA_s);
  tree->SetBranchAddress("N_fibers",     &n_fibers_s);
  tree->SetBranchAddress("Light_tot",     &light_tot_s);

  TRandom3* gen = new TRandom3(0);

  TFile* out_f = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/plot/plot_pur_merge_v2.root", "RECREATE");

  TH2F* LeverArm_time_s     = new TH2F("LAvsTime_s", "Time vs Lever arm signal", 300, 0., 300., 500, -200., 300.);
  TH2F* LeverArm_time_bg     = new TH2F("LAvsTime_bg", "Time vs Lever arm signal", 300, 0., 300., 500, -200., 300.);
  TH2F* LeverArm_time_bg_signal_free     = new TH2F("LAvsTime_bg_signal_free", "Time vs Lever arm signal", 300, 0., 300., 500, -200., 300.);


  // Strategy:
  // Loop over 1e6 signal events (w/ and w/o hits from neutron)
  //
  // 1) take "neutrino" vertex position from signal
  // 2) is the signal (neutron) reconstructed?
  //    2.1 yes
  //      2.1.1 signal or BG hit close to vertex (< 20cm)     --> skip
  //      2.1.2 first hit in time (AFTER THE VERTEX) is low light (< 40 p.e.) --> skip
  //      2.1.3 first time hit from signal                    --> fill signal histo
  //      2.2.4 first time hit from BG (After the vetrex)     --> fill BG histo
  //
  //    2.2 no  --> Fill BG histo, fil BG_signal_free_histo
  //
  // 3) Normolise BG and BG_signal_free with the pile up factor (2.3%)

  // What is missing: 
  // 1st BG happens before the vertex, more BG clusters happens after the vertex
  // happens quite rarely


  Int_t BG_spoiled 			= 0;
  Int_t BG_pileup 			= 0;
  Int_t Signal_ok				= 0;

  float vertex_time;
  float first_hit_time_signal;
  float LA_bg, dt_bg;
  float res;

  Int_t Nev_bg = tree_bg->GetEntries();
  Int_t Nev_s = 1000000;
  int it_s = 0;
  int it_bg = 0;

  int Nsignal = 0;
  int Total_signal = 0;

  while (true) {
    if (it_bg >= Nev_bg || it_s >= 1000000)
      break;

    tree->GetEntry(it_s);
    ++it_s;

    tree_bg->GetEntry(it_bg);
    ++it_bg;

    vertex_time = gen->Gaus(200., 18.);

    TVector3 vertex(vertex_pos[0], vertex_pos[1], vertex_pos[2]);
    TVector3 hit(hit_pos[0], hit_pos[1], hit_pos[2]);
    LA_bg = (hit-vertex).Mag() / 10;    

    // 2.2
    if (dt_s == 1.e9) {
      if (LA_bg > Dist_cut && light_tot / 3 > 40 && n_fibers_cut && first_hit_time > vertex_time) {
        res = 0.95 / sqrt(n_fibers_cut);
        first_hit_time = gen->Gaus(first_hit_time, res);

        dt_bg = first_hit_time - vertex_time;

        LeverArm_time_bg_signal_free->Fill(LA_bg, dt_bg);
        LeverArm_time_bg->Fill(LA_bg, dt_bg);
      }
      continue;
    }

    // 2.1.1
    if (LA_bg < Dist_cut || LA_s < Dist_cut)
      continue;

    // 2.1.2
    // both low light
    if (!n_fibers_s && !n_fibers_cut)
      continue;

    // signal low light but signal first
    if (!n_fibers_s && dt_s + vertex_time < first_hit_time)
      continue;

    // bg low light but bg first and AFTER the vertex
    if (!n_fibers_cut && dt_s + vertex_time > first_hit_time && first_hit_time > vertex_time)
      continue;

    res = 0.95 / sqrt(n_fibers_cut);
    first_hit_time = gen->Gaus(first_hit_time, res);
    dt_bg = first_hit_time - vertex_time;

    res = 0.95;
    res /= sqrt(n_fibers_s);
    dt_s = gen->Gaus(dt_s, res);
    first_hit_time_signal = dt_s + vertex_time;

    // 2.1.3
    // signal first
    if (first_hit_time > first_hit_time_signal) {
      LeverArm_time_s->Fill(LA_s, dt_s);
      continue;
    }

    // 2.1.4
    // BG first
    if (first_hit_time < first_hit_time_signal && first_hit_time > vertex_time) {
      LeverArm_time_bg->Fill(LA_bg, dt_bg);
      continue;
    }
  }

  LeverArm_time_bg->Scale(PileUp_proc);
  LeverArm_time_bg_signal_free->Scale(PileUp_proc);

  LeverArm_time_s->Write();
  LeverArm_time_bg->Write();
  LeverArm_time_bg_signal_free->Write();

  TH2F* pur = (TH2F*)LeverArm_time_s->Clone("Purity");
	TH2F* tot = (TH2F*)LeverArm_time_s->Clone("tot");
	tot->Add(LeverArm_time_bg);

	pur->Divide(tot);

	LeverArm_time_s->RebinY(2);
	LeverArm_time_s->RebinX(2);
	LeverArm_time_bg->RebinY(2);
	LeverArm_time_bg->RebinX(2);

	TH2F* pur2 = (TH2F*)LeverArm_time_s->Clone("Pur_scaled");
	tot = (TH2F*)LeverArm_time_s->Clone("tot");
	tot->Add(LeverArm_time_bg);
	pur2->Divide(tot);

	pur->Write();
	pur2->Write();

	out_f->Close();

  cout << "Total:\t" << it_s << endl;
  cout << "signal ok:\t" << Signal_ok << "\t" << 1. * Signal_ok / it_s * 100 << "%" << endl;
  cout << "BG spoiled:\t" << BG_spoiled << "\t" << 1. * BG_spoiled / it_s * 100 << "%" << endl;
  cout << "BG pileup:\t" << BG_pileup << "\t" << 1. * BG_pileup / it_s * 100 << "%" << endl;

  exit(1);
}