#include "TFile.h"
#include "TChain.h"

#include "TH2F.h"

#include "TRandom3.h"

#include <iostream>

//#include "TND280UpEvent.hh"
//#include "TND280UpHit.hh"

using namespace std;

void OOFV_signal_merge() {
	TChain* ch = new TChain("ND280upEvents");

  cout << "Creating chain" << endl;

  TString name_base = "/t2k/users/suvorov/AnalysisResults/ndUP/PileUp/G4/neutron_7b_v2/g4_neutron_7b-Exp";
  for (Int_t i = 0; i < 1263; ++i) {
    Int_t iter = i * 1000;
    TString name = name_base;
    name += iter;
    name += "-Nexpt1000.root";
    ch->AddFile(name.Data());
  }

  cout << "Files added" << endl;

  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  ch->SetBranchAddress("Event", &nd280UpEvent);

  TFile* signal_file = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/reco/SuperFGD-neutron_v22-UseXY-UseXZ-UseYZ-Separate10_na_1000000.root", "READ");
  TTree* tree       = (TTree*)signal_file->Get("neutron");

  Double_t ekin, costheta, LA_s, dist, dt_s, neutron_time, neutron_dist_true;
  Double_t vertex_pos[3];

  tree->SetBranchAddress("First_hit_time",     &dt_s);
  tree->SetBranchAddress("Vertex_pos",					vertex_pos);
  tree->SetBranchAddress("Distance_cubes",		 &LA_s);

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

  Int_t Nev = ch->GetEntries();
  Nev = 1000000;
  cout << "[                     ] N = " << Nev << "\r[";
  for (Int_t i = 0; i < Nev; ++i) {
  	if (i%(int(Nev/20)) == 0)
      cout << "." << flush;
  	tree->GetEntry(i);
  	ch->GetEntry(i);
  	// time count from 0, so through the real time around 200 +-18 ns
  	vertex_time = gen->Gaus(200., 18.);
  	first_hit_time_signal = dt_s + vertex_time;

  	vertex_pos[1] -= 18.;
    vertex_pos[2] -= 1707.;

    Int_t first_in_time_id = -1;
    Double_t Fst_time = 1e9;
    for(int ihit = 0; ihit < nd280UpEvent->GetNHits(); ++ihit) { // get last entry
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);
      double time = nd280UpHit->GetStartT();

      if (time < Fst_time) {
      	Fst_time = time;
      	first_in_time_id = ihit;
      }
    }

    if (Fst_time > first_hit_time_signal) {
    	++Signal_ok;
    	LeverArm_time_s->Fill(LA_s, dt_s);
    	continue;
    }

    double posX = (nd280UpEvent->GetHit(first_in_time_id)->GetStartX() + nd280UpEvent->GetHit(first_in_time_id)->GetStopX())/2.;
    double posY = (nd280UpEvent->GetHit(first_in_time_id)->GetStartY() + nd280UpEvent->GetHit(first_in_time_id)->GetStopY())/2. - 18.; 
    double posZ = (nd280UpEvent->GetHit(first_in_time_id)->GetStartZ() + nd280UpEvent->GetHit(first_in_time_id)->GetStopZ())/2. - 1707.; 

    TVector3 vertex(vertex_pos[0], vertex_pos[1], vertex_pos[2]);
    TVector3 hit(posX, posY, posZ);

    dt_bg = Fst_time - vertex_time;
    LA_bg = (hit-vertex).Mag();

    /*if (LA_bg < 500.) {
    	++BG_spoiled;
    	continue;     
  	}*/

  	++BG_pileup;
  	LeverArm_time_bg->Fill(LA_bg, dt_bg, 0.046);
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

  cout << "Total:\t" << Nev << endl;
  cout << "signal ok:\t" << Signal_ok << "\t" << 1. * Signal_ok / Nev * 100 << "%" << endl;
  cout << "BG spoiled:\t" << BG_spoiled << "\t" << 1. * BG_spoiled / Nev * 100 << "%" << endl;
  cout << "BG pileup:\t" << BG_pileup << "\t" << 1. * BG_pileup / Nev * 100 << "%" << endl;
}