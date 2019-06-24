#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TVector3.h"

#include <iostream>

using namespace std;

void GENIE_analyser() {
	

	TFile* in_file = new TFile("/t2k/users/suvorov/data/GENIE/v3/GENIE_G18_10b_1e21_numu_FHC.root", "READ");

	TTree* tree = (TTree*)in_file->Get("gRooTracker");

	TFile* out_file = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/spectra.root", "RECREATE");

	TH1F* prot_h = new TH1F("pr_h", "", 200, 0., 5000.);
	TH1F* muon_h = new TH1F("mu_h", "", 200, 0., 5000.);
	TH2F* muon_2h = new TH2F("muon_2d", "", 100, -1., 1., 200, 0., 5000.);

	const Int_t StdHepN_max = 300;

	Int_t StdHepN;
	Int_t StdHepPdg[StdHepN_max];
	Double_t StdHepP4[StdHepN_max][4];

	tree->SetBranchAddress("StdHepN",  		&StdHepN);
	tree->SetBranchAddress("StdHepPdg",		&StdHepPdg);
	tree->SetBranchAddress("StdHepP4",  	&StdHepP4);

	Int_t N_entries = tree->GetEntries();

	cout << "Working [          ] N = " << N_entries << "\rWorking [";

	for (Int_t i = 0; i < N_entries; ++i) {
		tree->GetEntry(i);

		if (i%(N_entries/10)==0)
       cout <<"."<<flush;

    Double_t pr_max = 0.;
    Double_t mu_max = 0.;
    Double_t mu_cos = 0.;

		for (Int_t id = 0; id < StdHepN; ++id) {
			TVector3 vec(StdHepP4[id][0], StdHepP4[id][1], StdHepP4[id][2]);
			if (StdHepPdg[id] == 2212 && vec.Mag() > pr_max) {
				pr_max = vec.Mag();
			}
			if (StdHepPdg[id] == 13 && vec.Mag() > mu_max) {
				mu_max = vec.Mag();
				mu_cos = cos(vec.Theta());
			}
		}

		//cout << pr_max << "  " << mu_max << endl;

		if (pr_max)
			prot_h->Fill(pr_max * 1000.);
		if (mu_max) {
			muon_h->Fill(mu_max * 1000.);
			muon_2h->Fill(mu_cos, mu_max * 1000.);
		}
	}

	cout << endl;

	out_file->cd();
	prot_h->Write();
	muon_h->Write();
	muon_2h->Write();
	out_file->Close();
}
