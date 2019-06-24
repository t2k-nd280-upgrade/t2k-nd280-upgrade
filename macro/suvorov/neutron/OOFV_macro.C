#include "TFile.h"
#include "TChain.h"

#include "TH2F.h"

#include "TRandom3.h"

#include <iostream>

//#include "TND280UpEvent.hh"
//#include "TND280UpHit.hh"

using namespace std;

int OOFV_macro() {

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

  TFile* signal_f = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/spectra.root", "READ");
  TH2F* signal_h = (TH2F*)signal_f->Get("muon_2d");

  TRandom3* gen = new TRandom3(0);

  TFile* file_out = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/plot/plot_OOFV.root", "RECREATE");
  TH2F* LeverArm_time     = new TH2F("LAvsTime", "Time vs Lever arm", 300, 0., 300., 500, -200., 300.);
  TH2F* LeverArm_time_a   = new TH2F("LAvsTime_a", "Time after vertex vs Lever arm", 300, 0., 300., 300, 0., 300.);

  TH1F* pt = new TH1F("pt", "Transverse momentum", 500, 0., 500.);

  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  ch->SetBranchAddress("Event", &nd280UpEvent);

  Int_t Nevents = ch->GetEntries();

  //Nevents = 20;

  cout << "[                     ] N = " << Nevents << "\r[";

  for (Int_t i = 0; i < Nevents; ++i) {

    if (i%(int(Nevents/20)) == 0)
      cout << "." << flush;

    ch->GetEntry(i);

    // generate random point for vertex and random time
    Float_t v_time  = gen->Gaus(200., 18.);
    Float_t v_x     = (gen->Uniform() - 0.5)* 2040.;
    Float_t v_y     = (gen->Uniform() - 0.5)* 520.;
    Float_t v_z     = (gen->Uniform() - 0.5)* 1880.;

    v_y -= 18.;
    v_z -= 1707.;

    Float_t first_time = 1e9;
    Int_t first_hit = -1;

    Float_t first_time_a = 1e9;
    Int_t first_hit_after = -1;

    for(int ihit = 0; ihit < nd280UpEvent->GetNHits(); ++ihit) { // get last entry
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);
      double time = nd280UpHit->GetStartT();

      if (first_time > time) {
        first_time = time;
        first_hit = ihit;
      }

      if (time > v_time && time < first_time_a) {
      	first_time_a 	= time;
      	first_hit_after 	= ihit;
      }
    } // loop over hits

    if (first_hit == -1)
      continue;

    TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(first_hit);

    double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
    double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2. - 18.; // middle step Y 
    double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2. - 1707.; // middle step Z

    TVector3 vertex(v_x, v_y, v_z);
    TVector3 hit(posX, posY, posZ);

    Float_t LA = (hit - vertex).Mag() / 10.;
    Float_t T  = first_time - v_time;

    LeverArm_time->Fill(LA, T);

    if (first_hit_after == -1)
      continue;

    nd280UpHit = nd280UpEvent->GetHit(first_hit_after);

    posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
    posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2. - 18.; // middle step Y 
    posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2. - 1707.; // middle step Z

    vertex = TVector3(v_x, v_y, v_z);
    hit = TVector3(posX, posY, posZ);

    LA = (hit - vertex).Mag() / 10.;
    T  = first_time - v_time;

    LeverArm_time_a->Fill(LA, T);

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
  LeverArm_time_a->Write();
  pt->Write();

  file_out->Close();

  return 1;
}