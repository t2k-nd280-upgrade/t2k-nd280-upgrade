#include "TFile.h"
#include "TChain.h"
#include "TVector3.h"
#include "TH2F.h"

#include "TRandom3.h"

#include <iostream>

using namespace std;

const double PileUp_proc = 0.02133;

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

  TFile* signal_file = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/reco/SuperFGD-neutron_v23-UseXY-UseXZ-UseYZ-Separate10_na_1000000.root", "READ");
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

  TFile* out_f = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/plot/plot_pur_merge.root", "RECREATE");

  TH2F* LeverArm_time_s     = new TH2F("LAvsTime_s", "Time vs Lever arm signal", 300, 0., 300., 500, -200., 300.);
  TH2F* LeverArm_time_bg     = new TH2F("LAvsTime_bg", "Time vs Lever arm signal", 300, 0., 300., 500, -200., 300.);


  // Strategy:
  // 1) take vertex position from signal
  // 2) Bias time and position for signal
  // 3) take first hit in time from both signal && BG
  //		3.1 first time from signal 		--> 'ok'
  //		3.2 first timw for BG
  //			3.2.1 < 50 cm
  //				BG 			--> 'spoiled' event
  //			3.2.1 > 50 cm
  //				BG 			--> pile up


  // BG < Signal???

  Int_t BG_spoiled 			= 0;
  Int_t BG_pileup 			= 0;
  Int_t Signal_ok				= 0;

  float vertex_time;
  float first_hit_time_signal;
  float LA_bg, dt_bg;

  Int_t Nev_bg = tree_bg->GetEntries();
  Int_t Nev_s = 1000000;
  cout << "[                     ] N = " << Nev_bg << "\r[";
  int it_s = 0;
  int it_bg = 0;

  while (true) {
    if (it_bg >= Nev_bg || it_s >= 1000000)
      break;
  	//if (i%(int(Nev_bg/20)) == 0)
    //  cout << "." << flush;
  	tree_bg->GetEntry(it_bg);
    if (!n_fibers_cut || light_tot / 3 < 40) {
      ++it_bg;
      continue;
    }

    tree->GetEntry(it_s);
    if (!n_fibers_s || light_tot_s / 3 < 40) {
      ++it_s;
      continue;
    }

  	// time count from 0, so through the real time around 200 +-18 ns
  	vertex_time = gen->Gaus(200., 18.);
    float res = 0.95;
    res /= sqrt(n_fibers_s);
  	first_hit_time_signal = gen->Gaus(dt_s, res) + vertex_time;

  	//vertex_pos[1] -= 18.;
    //vertex_pos[2] -= 1707.;

    Double_t Fst_time = first_hit_time;
    res = 0.95 / sqrt(n_fibers_cut);
    first_hit_time = gen->Gaus(first_hit_time, res);
    /*for(int ihit = 0; ihit < nd280UpEvent->GetNHits(); ++ihit) { // get last entry
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);
      double time = nd280UpHit->GetStartT();

      if (time < Fst_time) {
      	Fst_time = time;
      	first_in_time_id = ihit;
      }
    }*/

    if (Fst_time > first_hit_time_signal) {
    	++Signal_ok;
    	LeverArm_time_s->Fill(LA_s, dt_s);
      ++it_s;
      ++it_bg;
    	continue;
    }

    TVector3 vertex(vertex_pos[0], vertex_pos[1], vertex_pos[2]);
    TVector3 hit(hit_pos[0], hit_pos[1], hit_pos[2]);

    dt_bg = Fst_time - vertex_time;
    LA_bg = (hit-vertex).Mag() / 10;
    cout << "LA bg " << LA_bg << endl;

    /*if (LA_bg < 500.) {
    	++BG_spoiled;
    	continue;     
  	}*/

  	++BG_pileup;
  	LeverArm_time_bg->Fill(LA_bg, dt_bg, PileUp_proc);
    ++it_bg;
    ++it_s;
  } // loop over signal events

  cout << "]" << endl;

  LeverArm_time_s->Write();
  LeverArm_time_bg->Write();

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