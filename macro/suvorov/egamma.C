#include "TString.h"
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"

void egamma(){
	//TString prefix = "~/node_output/ndUP/SuperFGD/tracking/";
	TString prefix = "$bars/AnalysisResults/ndUP/SuperFGD/tracking/";

	TFile* file_muon_close 	= new TFile((prefix + "SuperFGD-ele_close_uni_len-UseXY-UseXZ-UseYZ-Separate10_tr_200000.root").Data(), "READ");
	TFile* file_muon_far 	 	= new TFile((prefix + "SuperFGD-ele_far_uni_len-UseXY-UseXZ-UseYZ-Separate10_tr_200000.root").Data(), "READ");
	TFile* file_proton_close = new TFile((prefix + "SuperFGD-gamma_close_uni_len-UseXY-UseXZ-UseYZ-Separate10_tr_200000.root").Data(), "READ");
	TFile* file_proton_far 	= new TFile((prefix + "SuperFGD-gamma_far_uni_len-UseXY-UseXZ-UseYZ-Separate10_tr_200000.root").Data(), "READ");

	TH1F* h_energy_e_c = (TH1F*)file_muon_close->Get("hETrack_HiLoRatio_Prod");
	h_energy_e_c->SetName("hEle_llh_close");
	TH1F* h_energy_e_f = (TH1F*)file_muon_far->Get("hETrack_HiLoRatio_Prod");
	h_energy_e_f->SetName("hEle_llh_far");
	TH1F* h_energy_g_c = (TH1F*)file_proton_close->Get("hETrack_HiLoRatio_Prod");
	h_energy_g_c->SetName("hGamma_llh_close");
	TH1F* h_energy_g_f = (TH1F*)file_proton_far->Get("hETrack_HiLoRatio_Prod");
	h_energy_g_f->SetName("hGamma_llh_far");

	TFile* outfile = new TFile((prefix + "output_eg.root").Data(), "RECREATE");

	TCanvas c1("canva","",50,50,1000,800);

	h_energy_e_c->Scale(1./h_energy_e_c->Integral());
	h_energy_g_c->Scale(1./h_energy_g_c->Integral());
	h_energy_e_c->Draw();
	h_energy_g_c->SetLineColor(kRed);
	h_energy_g_c->Draw("same");

	c1.Print((prefix+"/Egamma_close.pdf").Data());

	h_energy_e_f->Scale(1./h_energy_e_f->Integral());
	h_energy_g_f->Scale(1./h_energy_g_f->Integral());
	h_energy_e_f->Draw();
	h_energy_g_f->SetLineColor(kRed);
	h_energy_g_f->Draw("same");

	c1.Print((prefix+"/Egamma_far.pdf").Data());

}