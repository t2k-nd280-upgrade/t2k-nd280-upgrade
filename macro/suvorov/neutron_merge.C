#include "TFile.h"
#include "TH2F.h"
#include <iostream>

void neutron_merge() {
	TFile* file = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/SuperFGD-neutron-UseXY-UseXZ-UseYZ-Separate10_na_200000.root", "READ");

	TH2F* init_e_cos  = (TH2F*)file->Get("ini_ET");
	TH2F* eff_e_cos   = (TH2F*)file->Get("eff_ET");
	TH2F* pe_e_cos    = (TH2F*)file->Get("pe_ET");
	TH2F* hits_energy = (TH2F*)file->Get("hitsE");
	TH2F* hits_number = (TH2F*)file->Get("hitsN");
	TH2F* e_dist      = (TH2F*)file->Get("e_dist");

	// do rebinning
	Int_t rebin_Y = 5;
	init_e_cos->RebinY(rebin_Y);
	pe_e_cos->RebinY(rebin_Y);
	eff_e_cos->RebinY(rebin_Y);

	TH2F* eff_e_cos_2 = (TH2F*)eff_e_cos->Clone("eff_ecos2");

	std::cout << "cos theta bins \t" << init_e_cos->GetXaxis()->GetNbins() << std::endl;
	std::cout << "energy bins \t" << init_e_cos->GetYaxis()->GetNbins() << std::endl;

	for (Int_t i = 1; i <= init_e_cos->GetXaxis()->GetNbins(); ++i) {
    for (Int_t j = 1; j <= init_e_cos->GetYaxis()->GetNbins(); ++j) {
      Float_t pre = eff_e_cos->GetBinContent(i, j);
      Float_t scale = init_e_cos->GetBinContent(i, j);
      eff_e_cos_2->SetBinContent(i, j, pre / scale);
    }
  }

  for (Int_t i = 1; i <= pe_e_cos->GetXaxis()->GetNbins(); ++i) {
    for (Int_t j = 1; j <= pe_e_cos->GetYaxis()->GetNbins(); ++j) {
      Float_t pre = pe_e_cos->GetBinContent(i, j);
      Float_t scale = eff_e_cos->GetBinContent(i, j);
      pe_e_cos->SetBinContent(i, j, pre / scale);
    }
  }

  TFile* file_out = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/plot.root", "RECREATE");
  file_out->cd();

  init_e_cos->Write();
  eff_e_cos->Write();
  eff_e_cos_2->Write();
  pe_e_cos->Write();
  hits_energy->Write();
  hits_number->Write();
  e_dist->Write();

  file_out->Close();
  file->Close();
}