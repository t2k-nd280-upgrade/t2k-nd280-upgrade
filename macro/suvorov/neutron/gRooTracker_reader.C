#include "TFile.h"
#include "TChain.h"
#include "TH2F.h"

#include <iostream>
#include <fstream>

void gRooTracker_reader() {
	TChain* ch = new TChain("gRooTracker");

	ch->Add("/t2k/users/suvorov/AnalysisResults/ndUP/PileUp/gRooTracker_v4/neutron_7b.root");

	Double_t EvtVtx[4];
  Double_t NuParentDecX4[4];
  ch->SetBranchAddress("EvtVtx",        EvtVtx);
  ch->SetBranchAddress("NuParentDecX4", NuParentDecX4);

  TFile* out_file = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/plot/plot_OOFV_in.root", "RECREATE");

  TH2F* hits_XY = new TH2F("hits_XY", "", 196, -98, 98., 60, -30, 30.);
  TH2F* hits_XZ = new TH2F("hits_XZ", "", 196, -98, 98., 210, -105, 105.);
  TH2F* hits_YZ = new TH2F("hits_YZ", "", 60, -30, 30., 210, -105, 105.);

  TH2F* prod_XY = new TH2F("prod_XY", "", 200, -400, 400., 200, -400, 400.);
  TH2F* prod_XZ = new TH2F("prod_XZ", "", 200, -400, 400., 250, -400, 100.);
  TH2F* prod_YZ = new TH2F("prod_YZ", "", 200, -400, 400., 250, -400, 100.);

  auto n_ent = ch->GetEntries();

  for (auto i = 0; i < n_ent; ++i){
  	ch->GetEntry(i);

  	EvtVtx[0] *= 100;
  	EvtVtx[1] *= 100;
  	EvtVtx[2] *= 100;
  	EvtVtx[2] += 170.7;

  	hits_XY->Fill(EvtVtx[0], EvtVtx[1]);
  	hits_XZ->Fill(EvtVtx[0], EvtVtx[2]);
  	hits_YZ->Fill(EvtVtx[1], EvtVtx[2]);

    prod_XY->Fill(NuParentDecX4[0] / 10, NuParentDecX4[1] / 10);
    prod_XZ->Fill(NuParentDecX4[0] / 10, NuParentDecX4[2] / 10);
    prod_YZ->Fill(NuParentDecX4[1] / 10, NuParentDecX4[2] / 10);


  }

  out_file->cd();

  hits_XY->Write();
  hits_XZ->Write();
  hits_YZ->Write();

  prod_XY->Write();
  prod_XZ->Write();
  prod_YZ->Write();

  out_file->Close();
  exit(1);
}