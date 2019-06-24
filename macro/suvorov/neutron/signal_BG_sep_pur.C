#include "TFile.h"
#include "TH2F.h"

#include <iostream>

const double PileUp_proc = 0.02133;

int signal_BG_sep_pur() {
	// signal. neutron spectrum from NEUT, position unifirm
	TFile* f_signal = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/plot/plot_neutron_v29.root", "READ");
	TH2F* h_signal  = (TH2F*)f_signal->Get("LAvsTime");
	h_signal->SetName("LAvsTime_c");

	TFile* f_BG = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/plot/plot_OOFV_v3.root", "READ");
	TH2F* h_BG  = (TH2F*)f_BG->Get("LAvsTime");
	h_BG->SetName("LAvsTime_BG");

	// norm BG to 4.6% of the pile up
	// this value was estimated for 1MW beam
	h_BG->Scale(PileUp_proc * h_signal->Integral() / h_BG->Integral());

	TH2F* pur = (TH2F*)h_signal->Clone("Purity");
	TH2F* tot = (TH2F*)h_signal->Clone("tot");
	tot->Add(h_BG);

	pur->Divide(tot);

	h_signal->RebinY(2);
	h_signal->RebinX(2);
	h_BG->RebinY(2);
	h_BG->RebinX(2);

	TH2F* pur2 = (TH2F*)h_signal->Clone("Pur_scaled");
	tot = (TH2F*)h_signal->Clone("tot");
	tot->Add(h_BG);
	pur2->Divide(tot);

	TFile* out = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/plot/plot_pur.root", "RECREATE");
	pur->Write();
	pur2->Write();
	h_signal->Write();
	h_BG->Write();
	out->Close();

	exit(1);
	return 1;
}