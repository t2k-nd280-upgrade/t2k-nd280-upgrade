#include "TString.h"
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"

void tracking(){
	//TString prefix = "~/node_output/ndUP/SuperFGD/tracking/";
	TString prefix = "$bars/AnalysisResults/ndUP/SuperFGD/tracking/";

	TFile* file_muon_close 	= new TFile((prefix + "SuperFGD-muon_close_real_len-UseXY-UseXZ-UseYZ-Separate10_tr_200000.root").Data(), "READ");
	TFile* file_muon_far 	 	= new TFile((prefix + "SuperFGD-muon_far_real_len-UseXY-UseXZ-UseYZ-Separate10_tr_200000.root").Data(), "READ");
	TFile* file_proton_close = new TFile((prefix + "SuperFGD-proton_close_real_len-UseXY-UseXZ-UseYZ-Separate10_tr_200000.root").Data(), "READ");
	TFile* file_proton_far 	= new TFile((prefix + "SuperFGD-proton_far_real_len-UseXY-UseXZ-UseYZ-Separate10_tr_200000.root").Data(), "READ");

	TFile* outfile = new TFile((prefix + "output.root").Data(), "RECREATE");

	// draw e
	TH1F* h_energy_m_c = (TH1F*)file_muon_close->Get("hMuon_Stopped_Edep");
	h_energy_m_c->SetName("hMuon_Stopped_Edep_close");
	TH1F* h_energy_m_f = (TH1F*)file_muon_far->Get("hMuon_Stopped_Edep");
	h_energy_m_f->SetName("hMuon_Stopped_Edep_far");
	TH1F* h_energy_p_c = (TH1F*)file_proton_close->Get("hProt_Stopped_Edep");
	h_energy_p_c->SetName("hProt_Stopped_Edep_close");
	TH1F* h_energy_p_f = (TH1F*)file_proton_far->Get("hProt_Stopped_Edep");
	h_energy_p_f->SetName("hProt_Stopped_Edep_far");

	Int_t rebin = 4000;

	// draw e over dist
	TH1F* h_energy_dist_m_c_mb = (TH1F*)file_muon_close->Get("hMuon_Stopped_EdepOverLen_ManyBins");
	h_energy_dist_m_c_mb->Rebin(rebin);
	h_energy_dist_m_c_mb->SetName("hMuon_Stopped_EdepOverLen_close_mb");
	TH1F* h_energy_dist_m_f_mb = (TH1F*)file_muon_far->Get("hMuon_Stopped_EdepOverLen_ManyBins");
	h_energy_dist_m_f_mb->Rebin(rebin);
	h_energy_dist_m_f_mb->SetName("hMuon_Stopped_EdepOverLen_far_mb");
	TH1F* h_energy_dist_p_c_mb = (TH1F*)file_proton_close->Get("hProt_Stopped_EdepOverLen_ManyBins");
	h_energy_dist_p_c_mb->Rebin(rebin);
	h_energy_dist_p_c_mb->SetName("hProt_Stopped_EdepOverLen_close_mb");
	TH1F* h_energy_dist_p_f_mb = (TH1F*)file_proton_far->Get("hProt_Stopped_EdepOverLen_ManyBins");
	h_energy_dist_p_f_mb->Rebin(rebin);
	h_energy_dist_p_f_mb->SetName("hProt_Stopped_EdepOverLen_far_mb");

	TH1F* h_energy_dist_m_c_mb_0_300 = (TH1F*)file_muon_close->Get("hMuon_Stopped_EdepOverLen_Mom_ManyBins_0_300");
	h_energy_dist_m_c_mb_0_300->Rebin(rebin);
	h_energy_dist_m_c_mb_0_300->SetName("hMuon_Stopped_EdepOverLen_close_mb_0_300");
	TH1F* h_energy_dist_m_f_mb_0_300 = (TH1F*)file_muon_far->Get("hMuon_Stopped_EdepOverLen_Mom_ManyBins_0_300");
	h_energy_dist_m_f_mb_0_300->Rebin(rebin);
	h_energy_dist_m_f_mb_0_300->SetName("hMuon_Stopped_EdepOverLen_far_mb_0_300");
	TH1F* h_energy_dist_p_c_mb_0_300 = (TH1F*)file_proton_close->Get("hProt_Stopped_EdepOverLen_Mom_ManyBins_0_300");
	h_energy_dist_p_c_mb_0_300->Rebin(rebin);
	h_energy_dist_p_c_mb_0_300->SetName("hProt_Stopped_EdepOverLen_close_mb_0_300");
	TH1F* h_energy_dist_p_f_mb_0_300 = (TH1F*)file_proton_far->Get("hProt_Stopped_EdepOverLen_Mom_ManyBins_0_300");
	h_energy_dist_p_f_mb_0_300->Rebin(rebin);
	h_energy_dist_p_f_mb_0_300->SetName("hProt_Stopped_EdepOverLen_far_mb_0_300");

	TH1F* h_energy_dist_m_c_mb_300_600 = (TH1F*)file_muon_close->Get("hMuon_Stopped_EdepOverLen_Mom_ManyBins_300_600");
	h_energy_dist_m_c_mb_300_600->Rebin(rebin);
	h_energy_dist_m_c_mb_300_600->SetName("hMuon_Stopped_EdepOverLen_close_mb_300_600");
	TH1F* h_energy_dist_m_f_mb_300_600 = (TH1F*)file_muon_far->Get("hMuon_Stopped_EdepOverLen_Mom_ManyBins_300_600");
	h_energy_dist_m_f_mb_300_600->Rebin(rebin);
	h_energy_dist_m_f_mb_300_600->SetName("hMuon_Stopped_EdepOverLen_far_mb_300_600");
	TH1F* h_energy_dist_p_c_mb_300_600 = (TH1F*)file_proton_close->Get("hProt_Stopped_EdepOverLen_Mom_ManyBins_300_600");
	h_energy_dist_p_c_mb_300_600->Rebin(rebin);
	h_energy_dist_p_c_mb_300_600->SetName("hProt_Stopped_EdepOverLen_close_mb_300_600");
	TH1F* h_energy_dist_p_f_mb_300_600 = (TH1F*)file_proton_far->Get("hProt_Stopped_EdepOverLen_Mom_ManyBins_300_600");
	h_energy_dist_p_f_mb_300_600->Rebin(rebin);
	h_energy_dist_p_f_mb_300_600->SetName("hProt_Stopped_EdepOverLen_far_mb_300_600");

	TH1F* h_energy_dist_m_c_mb_600_800 = (TH1F*)file_muon_close->Get("hMuon_Stopped_EdepOverLen_Mom_ManyBins_600_800");
	h_energy_dist_m_c_mb_600_800->Rebin(rebin);
	h_energy_dist_m_c_mb_600_800->SetName("hMuon_Stopped_EdepOverLen_close_mb_600_800");
	TH1F* h_energy_dist_m_f_mb_600_800 = (TH1F*)file_muon_far->Get("hMuon_Stopped_EdepOverLen_Mom_ManyBins_600_800");
	h_energy_dist_m_f_mb_600_800->Rebin(rebin);
	h_energy_dist_m_f_mb_600_800->SetName("hMuon_Stopped_EdepOverLen_far_mb_600_800");
	TH1F* h_energy_dist_p_c_mb_600_800 = (TH1F*)file_proton_close->Get("hProt_Stopped_EdepOverLen_Mom_ManyBins_600_800");
	h_energy_dist_p_c_mb_600_800->Rebin(rebin);
	h_energy_dist_p_c_mb_600_800->SetName("hProt_Stopped_EdepOverLen_close_mb_600_800");
	TH1F* h_energy_dist_p_f_mb_600_800 = (TH1F*)file_proton_far->Get("hProt_Stopped_EdepOverLen_Mom_ManyBins_600_800");
	h_energy_dist_p_f_mb_600_800->Rebin(rebin);
	h_energy_dist_p_f_mb_600_800->SetName("hProt_Stopped_EdepOverLen_far_mb_600_800");

	TH1F* h_energy_dist_m_c_mb_800_1000 = (TH1F*)file_muon_close->Get("hMuon_Stopped_EdepOverLen_Mom_ManyBins_800_1000");
	h_energy_dist_m_c_mb_800_1000->Rebin(rebin);
	h_energy_dist_m_c_mb_800_1000->SetName("hMuon_Stopped_EdepOverLen_close_mb_800_1000");
	TH1F* h_energy_dist_m_f_mb_800_1000 = (TH1F*)file_muon_far->Get("hMuon_Stopped_EdepOverLen_Mom_ManyBins_800_1000");
	h_energy_dist_m_f_mb_800_1000->Rebin(rebin);
	h_energy_dist_m_f_mb_800_1000->SetName("hMuon_Stopped_EdepOverLen_far_mb_800_1000");
	TH1F* h_energy_dist_p_c_mb_800_1000 = (TH1F*)file_proton_close->Get("hProt_Stopped_EdepOverLen_Mom_ManyBins_800_1000");
	h_energy_dist_p_c_mb_800_1000->Rebin(rebin);
	h_energy_dist_p_c_mb_800_1000->SetName("hProt_Stopped_EdepOverLen_close_mb_800_1000");
	TH1F* h_energy_dist_p_f_mb_800_1000 = (TH1F*)file_proton_far->Get("hProt_Stopped_EdepOverLen_Mom_ManyBins_800_1000");
	h_energy_dist_p_f_mb_800_1000->Rebin(rebin);
	h_energy_dist_p_f_mb_800_1000->SetName("hProt_Stopped_EdepOverLen_far_mb_800_1000");

	//
	//
	// over length
	//
	//
	TH1F* h_energy_dist_m_c_mb_len_0_50 = (TH1F*)file_muon_close->Get("hMuon_Stopped_EdepOverLen_Len_ManyBins_0_50");
	h_energy_dist_m_c_mb_len_0_50->Rebin(rebin);
	h_energy_dist_m_c_mb_len_0_50->SetName("hMuon_Stopped_EdepOverLen_close_mb_len_0_50");
	TH1F* h_energy_dist_m_f_mb_len_0_50 = (TH1F*)file_muon_far->Get("hMuon_Stopped_EdepOverLen_Len_ManyBins_0_50");
	h_energy_dist_m_f_mb_len_0_50->Rebin(rebin);
	h_energy_dist_m_f_mb_len_0_50->SetName("hMuon_Stopped_EdepOverLen_far_mb_len_0_50");
	TH1F* h_energy_dist_p_c_mb_len_0_50 = (TH1F*)file_proton_close->Get("hProt_Stopped_EdepOverLen_Len_ManyBins_0_50");
	h_energy_dist_p_c_mb_len_0_50->Rebin(rebin);
	h_energy_dist_p_c_mb_len_0_50->SetName("hProt_Stopped_EdepOverLen_close_mb_len_0_50");
	TH1F* h_energy_dist_p_f_mb_len_0_50 = (TH1F*)file_proton_far->Get("hProt_Stopped_EdepOverLen_Len_ManyBins_0_50");
	h_energy_dist_p_f_mb_len_0_50->Rebin(rebin);
	h_energy_dist_p_f_mb_len_0_50->SetName("hProt_Stopped_EdepOverLen_far_mb_len_0_50");

	TH1F* h_energy_dist_m_c_mb_len_50_100 = (TH1F*)file_muon_close->Get("hMuon_Stopped_EdepOverLen_Len_ManyBins_50_100");
	h_energy_dist_m_c_mb_len_50_100->Rebin(rebin);
	h_energy_dist_m_c_mb_len_50_100->SetName("hMuon_Stopped_EdepOverLen_close_mb_len_50_100");
	TH1F* h_energy_dist_m_f_mb_len_50_100 = (TH1F*)file_muon_far->Get("hMuon_Stopped_EdepOverLen_Len_ManyBins_50_100");
	h_energy_dist_m_f_mb_len_50_100->Rebin(rebin);
	h_energy_dist_m_f_mb_len_50_100->SetName("hMuon_Stopped_EdepOverLen_far_mb_len_50_100");
	TH1F* h_energy_dist_p_c_mb_len_50_100 = (TH1F*)file_proton_close->Get("hProt_Stopped_EdepOverLen_Len_ManyBins_50_100");
	h_energy_dist_p_c_mb_len_50_100->Rebin(rebin);
	h_energy_dist_p_c_mb_len_50_100->SetName("hProt_Stopped_EdepOverLen_close_mb_len_50_100");
	TH1F* h_energy_dist_p_f_mb_len_50_100 = (TH1F*)file_proton_far->Get("hProt_Stopped_EdepOverLen_Len_ManyBins_50_100");
	h_energy_dist_p_f_mb_len_50_100->Rebin(rebin);
	h_energy_dist_p_f_mb_len_50_100->SetName("hProt_Stopped_EdepOverLen_far_mb_len_50_100");

	TH1F* h_energy_dist_m_c_mb_len_100_200 = (TH1F*)file_muon_close->Get("hMuon_Stopped_EdepOverLen_Len_ManyBins_100_200");
	h_energy_dist_m_c_mb_len_100_200->Rebin(rebin);
	h_energy_dist_m_c_mb_len_100_200->SetName("hMuon_Stopped_EdepOverLen_close_mb_len_100_200");
	TH1F* h_energy_dist_m_f_mb_len_100_200 = (TH1F*)file_muon_far->Get("hMuon_Stopped_EdepOverLen_Len_ManyBins_100_200");
	h_energy_dist_m_f_mb_len_100_200->Rebin(rebin);
	h_energy_dist_m_f_mb_len_100_200->SetName("hMuon_Stopped_EdepOverLen_far_mb_len_100_200");
	TH1F* h_energy_dist_p_c_mb_len_100_200 = (TH1F*)file_proton_close->Get("hProt_Stopped_EdepOverLen_Len_ManyBins_100_200");
	h_energy_dist_p_c_mb_len_100_200->Rebin(rebin);
	h_energy_dist_p_c_mb_len_100_200->SetName("hProt_Stopped_EdepOverLen_close_mb_len_100_200");
	TH1F* h_energy_dist_p_f_mb_len_100_200 = (TH1F*)file_proton_far->Get("hProt_Stopped_EdepOverLen_Len_ManyBins_100_200");
	h_energy_dist_p_f_mb_len_100_200->Rebin(rebin);
	h_energy_dist_p_f_mb_len_100_200->SetName("hProt_Stopped_EdepOverLen_far_mb_len_100_200");

	TH1F* h_energy_dist_m_c_mb_len_200_300 = (TH1F*)file_muon_close->Get("hMuon_Stopped_EdepOverLen_Len_ManyBins_200_300");
	h_energy_dist_m_c_mb_len_200_300->Rebin(rebin);
	h_energy_dist_m_c_mb_len_200_300->SetName("hMuon_Stopped_EdepOverLen_close_mb_len_200_300");
	TH1F* h_energy_dist_m_f_mb_len_200_300 = (TH1F*)file_muon_far->Get("hMuon_Stopped_EdepOverLen_Len_ManyBins_200_300");
	h_energy_dist_m_f_mb_len_200_300->Rebin(rebin);
	h_energy_dist_m_f_mb_len_200_300->SetName("hMuon_Stopped_EdepOverLen_far_mb_len_200_300");
	TH1F* h_energy_dist_p_c_mb_len_200_300 = (TH1F*)file_proton_close->Get("hProt_Stopped_EdepOverLen_Len_ManyBins_200_300");
	h_energy_dist_p_c_mb_len_200_300->Rebin(rebin);
	h_energy_dist_p_c_mb_len_200_300->SetName("hProt_Stopped_EdepOverLen_close_mb_len_200_300");
	TH1F* h_energy_dist_p_f_mb_len_200_300 = (TH1F*)file_proton_far->Get("hProt_Stopped_EdepOverLen_Len_ManyBins_200_300");
	h_energy_dist_p_f_mb_len_200_300->Rebin(rebin);
	h_energy_dist_p_f_mb_len_200_300->SetName("hProt_Stopped_EdepOverLen_far_mb_len_200_300");

	TH1F* h_energy_dist_m_c_mb_len_300_400 = (TH1F*)file_muon_close->Get("hMuon_Stopped_EdepOverLen_Len_ManyBins_300_400");
	h_energy_dist_m_c_mb_len_300_400->Rebin(rebin);
	h_energy_dist_m_c_mb_len_300_400->SetName("hMuon_Stopped_EdepOverLen_close_mb_len_300_400");
	TH1F* h_energy_dist_m_f_mb_len_300_400 = (TH1F*)file_muon_far->Get("hMuon_Stopped_EdepOverLen_Len_ManyBins_300_400");
	h_energy_dist_m_f_mb_len_300_400->Rebin(rebin);
	h_energy_dist_m_f_mb_len_300_400->SetName("hMuon_Stopped_EdepOverLen_far_mb_len_300_400");
	TH1F* h_energy_dist_p_c_mb_len_300_400 = (TH1F*)file_proton_close->Get("hProt_Stopped_EdepOverLen_Len_ManyBins_300_400");
	h_energy_dist_p_c_mb_len_300_400->Rebin(rebin);
	h_energy_dist_p_c_mb_len_300_400->SetName("hProt_Stopped_EdepOverLen_close_mb_len_300_400");
	TH1F* h_energy_dist_p_f_mb_len_300_400 = (TH1F*)file_proton_far->Get("hProt_Stopped_EdepOverLen_Len_ManyBins_300_400");
	h_energy_dist_p_f_mb_len_300_400->Rebin(rebin);
	h_energy_dist_p_f_mb_len_300_400->SetName("hProt_Stopped_EdepOverLen_far_mb_len_300_400");

	TH1F* h_energy_dist_m_c_mb_len_400_500 = (TH1F*)file_muon_close->Get("hMuon_Stopped_EdepOverLen_Len_ManyBins_400_500");
	h_energy_dist_m_c_mb_len_400_500->Rebin(rebin);
	h_energy_dist_m_c_mb_len_400_500->SetName("hMuon_Stopped_EdepOverLen_close_mb_len_400_500");
	TH1F* h_energy_dist_m_f_mb_len_400_500 = (TH1F*)file_muon_far->Get("hMuon_Stopped_EdepOverLen_Len_ManyBins_400_500");
	h_energy_dist_m_f_mb_len_400_500->Rebin(rebin);
	h_energy_dist_m_f_mb_len_400_500->SetName("hMuon_Stopped_EdepOverLen_far_mb_len_400_500");
	TH1F* h_energy_dist_p_c_mb_len_400_500 = (TH1F*)file_proton_close->Get("hProt_Stopped_EdepOverLen_Len_ManyBins_400_500");
	h_energy_dist_p_c_mb_len_400_500->Rebin(rebin);
	h_energy_dist_p_c_mb_len_400_500->SetName("hProt_Stopped_EdepOverLen_close_mb_len_400_500");
	TH1F* h_energy_dist_p_f_mb_len_400_500 = (TH1F*)file_proton_far->Get("hProt_Stopped_EdepOverLen_Len_ManyBins_400_500");
	h_energy_dist_p_f_mb_len_400_500->Rebin(rebin);
	h_energy_dist_p_f_mb_len_400_500->SetName("hProt_Stopped_EdepOverLen_far_mb_len_400_500");



	TH1F* h_energy_dist_m_c = (TH1F*)file_muon_close->Get("hMuon_Stopped_EdepOverLen");
	h_energy_dist_m_c->SetName("hMuon_Stopped_EdepOverLen_close");
	TH1F* h_energy_dist_m_f = (TH1F*)file_muon_far->Get("hMuon_Stopped_EdepOverLen");
	h_energy_dist_m_f->SetName("hMuon_Stopped_EdepOverLen_far");
	TH1F* h_energy_dist_p_c = (TH1F*)file_proton_close->Get("hProt_Stopped_EdepOverLen");
	h_energy_dist_p_c->SetName("hProt_Stopped_EdepOverLen_close");
	TH1F* h_energy_dist_p_f = (TH1F*)file_proton_far->Get("hProt_Stopped_EdepOverLen");
	h_energy_dist_p_f->SetName("hProt_Stopped_EdepOverLen_far");

	// draw LLH
	/*TH1F* h_llh_m_c = (TH1F*)file_muon_close->Get("hMuon_Stopped_Edep");
	TH1F* h_llh_m_f = (TH1F*)file_muon_far->Get("hMuon_Stopped_Edep");
	TH1F* h_llh_p_c = (TH1F*)file_proton_close->Get("hProt_Stopped_Edep");
	TH1F* h_llh_p_f = (TH1F*)file_proton_far->Get("hProt_Stopped_Edep");*/

	outfile->cd();

	h_energy_m_c->Write();
	h_energy_m_f->Write();
	h_energy_p_c->Write();
	h_energy_p_f->Write();

	// draw e over dist
	h_energy_dist_m_c->Write();
	h_energy_dist_m_f->Write();
	h_energy_dist_p_c->Write();
	h_energy_dist_p_f->Write();


	TCanvas c1("canva","",50,50,1000,800);
	h_energy_dist_m_f_mb->Draw();
	h_energy_dist_m_c_mb->SetLineColor(kRed);
	h_energy_dist_m_c_mb->Draw("same");
	c1.SetName("Muon_far_vs_close");
	c1.Write(); c1.Print((prefix+"/Muon_far_vs_close.pdf").Data());

	h_energy_dist_p_f_mb->Draw();
	h_energy_dist_p_c_mb->SetLineColor(kRed);
	h_energy_dist_p_c_mb->Draw("same");
	c1.SetName("Proton_far_vs_close");
	c1.Write(); c1.Print((prefix+"/Proton_far_vs_close.pdf").Data());

	// HERE NORMALISATION COMES
	h_energy_dist_m_f_mb->Scale(1./h_energy_dist_m_f_mb->Integral());
	h_energy_dist_p_f_mb->Scale(1./h_energy_dist_p_f_mb->Integral());
	h_energy_dist_m_f_mb->SetLineColor(kBlue);
	h_energy_dist_p_f_mb->SetLineColor(kRed);
	h_energy_dist_m_f_mb->Draw(" HIST");
	h_energy_dist_p_f_mb->Draw("same HIST");
	c1.SetName("Far_muon_vs_proton");
	c1.SetTitle("Far_muon_vs_proton");
	c1.Write(); c1.Print((prefix+"/Far_muon_vs_proton.pdf").Data());

	h_energy_dist_m_c_mb->Scale(1./h_energy_dist_m_c_mb->Integral());
	h_energy_dist_p_c_mb->Scale(1./h_energy_dist_p_c_mb->Integral());
	h_energy_dist_m_c_mb->SetLineColor(kBlue);
	h_energy_dist_p_c_mb->SetLineColor(kRed);
	h_energy_dist_m_c_mb->Draw(" HIST");
	h_energy_dist_p_c_mb->Draw("same HIST");
	c1.SetName("Close_muon_vs_proton");
	c1.SetTitle("Close_muon_vs_proton");
	c1.Write(); c1.Print((prefix+"/Close_muon_vs_proton.pdf").Data());

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	// Close muon vs proton
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	h_energy_dist_m_c_mb_0_300->Scale(1./h_energy_dist_m_c_mb_0_300->Integral());
	h_energy_dist_p_c_mb_0_300->Scale(1./h_energy_dist_p_c_mb_0_300->Integral());
	h_energy_dist_m_c_mb_0_300->SetLineColor(kBlue);
	h_energy_dist_p_c_mb_0_300->SetLineColor(kRed);
	h_energy_dist_m_c_mb_0_300->Draw(" HIST");
	h_energy_dist_p_c_mb_0_300->Draw("same HIST");
	c1.SetName("Close_muon_vs_proton_0_300");
	c1.SetTitle("Close_muon_vs_proton_0_300");
	c1.Write(); c1.Print((prefix+"/Close_muon_vs_proton_0_300.pdf").Data());

	h_energy_dist_m_c_mb_300_600->Scale(1./h_energy_dist_m_c_mb_300_600->Integral());
	h_energy_dist_p_c_mb_300_600->Scale(1./h_energy_dist_p_c_mb_300_600->Integral());
	h_energy_dist_m_c_mb_300_600->SetLineColor(kBlue);
	h_energy_dist_p_c_mb_300_600->SetLineColor(kRed);
	h_energy_dist_m_c_mb_300_600->Draw(" HIST");
	h_energy_dist_p_c_mb_300_600->Draw("same HIST");
	c1.SetName("Close_muon_vs_proton_300_600");
	c1.SetTitle("Close_muon_vs_proton_300_600");
	c1.Write(); c1.Print((prefix+"/Close_muon_vs_proton_300_600.pdf").Data());

	h_energy_dist_m_c_mb_600_800->Scale(1./h_energy_dist_m_c_mb_600_800->Integral());
	h_energy_dist_p_c_mb_600_800->Scale(1./h_energy_dist_p_c_mb_600_800->Integral());
	h_energy_dist_m_c_mb_600_800->SetLineColor(kBlue);
	h_energy_dist_p_c_mb_600_800->SetLineColor(kRed);
	h_energy_dist_m_c_mb_600_800->Draw(" HIST");
	h_energy_dist_p_c_mb_600_800->Draw("same HIST");
	c1.SetName("Close_muon_vs_proton_600_800");
	c1.SetTitle("Close_muon_vs_proton_600_800");
	c1.Write(); c1.Print((prefix+"/Close_muon_vs_proton_600_800.pdf").Data());

	h_energy_dist_m_c_mb_800_1000->Scale(1./h_energy_dist_m_c_mb_800_1000->Integral());
	h_energy_dist_p_c_mb_800_1000->Scale(1./h_energy_dist_p_c_mb_800_1000->Integral());
	h_energy_dist_m_c_mb_800_1000->SetLineColor(kBlue);
	h_energy_dist_p_c_mb_800_1000->SetLineColor(kRed);
	h_energy_dist_m_c_mb_800_1000->Draw(" HIST");
	h_energy_dist_p_c_mb_800_1000->Draw("same HIST");
	c1.SetName("Close_muon_vs_proton_800_1000");
	c1.SetTitle("Close_muon_vs_proton_800_1000");
	c1.Write(); c1.Print((prefix+"/Close_muon_vs_proton_800_1000.pdf").Data());

	//
	//
	// length
	//
	//
	h_energy_dist_m_c_mb_len_0_50->Scale(1./h_energy_dist_m_c_mb_len_0_50->Integral());
	h_energy_dist_p_c_mb_len_0_50->Scale(1./h_energy_dist_p_c_mb_len_0_50->Integral());
	h_energy_dist_m_c_mb_len_0_50->SetLineColor(kBlue);
	h_energy_dist_p_c_mb_len_0_50->SetLineColor(kRed);
	h_energy_dist_m_c_mb_len_0_50->Draw(" HIST");
	h_energy_dist_p_c_mb_len_0_50->Draw("same HIST");
	c1.SetName("Close_muon_vs_proton_len_0_50");
	c1.SetTitle("Close_muon_vs_proton_len_0_50");
	c1.Write(); c1.Print((prefix+"/Close_muon_vs_proton_len_0_50.pdf").Data());

	h_energy_dist_m_c_mb_len_50_100->Scale(1./h_energy_dist_m_c_mb_len_50_100->Integral());
	h_energy_dist_p_c_mb_len_50_100->Scale(1./h_energy_dist_p_c_mb_len_50_100->Integral());
	h_energy_dist_m_c_mb_len_50_100->SetLineColor(kBlue);
	h_energy_dist_p_c_mb_len_50_100->SetLineColor(kRed);
	h_energy_dist_m_c_mb_len_50_100->Draw(" HIST");
	h_energy_dist_p_c_mb_len_50_100->Draw("same HIST");
	c1.SetName("Close_muon_vs_proton_len_50_100");
	c1.SetTitle("Close_muon_vs_proton_len_50_100");
	c1.Write(); c1.Print((prefix+"/Close_muon_vs_proton_len_50_100.pdf").Data());

	h_energy_dist_m_c_mb_len_100_200->Scale(1./h_energy_dist_m_c_mb_len_100_200->Integral());
	h_energy_dist_p_c_mb_len_100_200->Scale(1./h_energy_dist_p_c_mb_len_100_200->Integral());
	h_energy_dist_m_c_mb_len_100_200->SetLineColor(kBlue);
	h_energy_dist_p_c_mb_len_100_200->SetLineColor(kRed);
	h_energy_dist_m_c_mb_len_100_200->Draw(" HIST");
	h_energy_dist_p_c_mb_len_100_200->Draw("same HIST");
	c1.SetName("Close_muon_vs_proton_len_100_200");
	c1.SetTitle("Close_muon_vs_proton_len_100_200");
	c1.Write(); c1.Print((prefix+"/Close_muon_vs_proton_len_100_200.pdf").Data());

	h_energy_dist_m_c_mb_len_200_300->Scale(1./h_energy_dist_m_c_mb_len_200_300->Integral());
	h_energy_dist_p_c_mb_len_200_300->Scale(1./h_energy_dist_p_c_mb_len_200_300->Integral());
	h_energy_dist_m_c_mb_len_200_300->SetLineColor(kBlue);
	h_energy_dist_p_c_mb_len_200_300->SetLineColor(kRed);
	h_energy_dist_m_c_mb_len_200_300->Draw(" HIST");
	h_energy_dist_p_c_mb_len_200_300->Draw("same HIST");
	c1.SetName("Close_muon_vs_proton_len_200_300");
	c1.SetTitle("Close_muon_vs_proton_len_200_300");
	c1.Write(); c1.Print((prefix+"/Close_muon_vs_proton_len_200_300.pdf").Data());

	h_energy_dist_m_c_mb_len_300_400->Scale(1./h_energy_dist_m_c_mb_len_300_400->Integral());
	h_energy_dist_p_c_mb_len_300_400->Scale(1./h_energy_dist_p_c_mb_len_300_400->Integral());
	h_energy_dist_m_c_mb_len_300_400->SetLineColor(kBlue);
	h_energy_dist_p_c_mb_len_300_400->SetLineColor(kRed);
	h_energy_dist_m_c_mb_len_300_400->Draw(" HIST");
	h_energy_dist_p_c_mb_len_300_400->Draw("same HIST");
	c1.SetName("Close_muon_vs_proton_len_300_400");
	c1.SetTitle("Close_muon_vs_proton_len_300_400");
	c1.Write(); c1.Print((prefix+"/Close_muon_vs_proton_len_300_400.pdf").Data());

	h_energy_dist_m_c_mb_len_400_500->Scale(1./h_energy_dist_m_c_mb_len_400_500->Integral());
	h_energy_dist_p_c_mb_len_400_500->Scale(1./h_energy_dist_p_c_mb_len_400_500->Integral());
	h_energy_dist_m_c_mb_len_400_500->SetLineColor(kBlue);
	h_energy_dist_p_c_mb_len_400_500->SetLineColor(kRed);
	h_energy_dist_m_c_mb_len_400_500->Draw(" HIST");
	h_energy_dist_p_c_mb_len_400_500->Draw("same HIST");
	c1.SetName("Close_muon_vs_proton_len_400_500");
	c1.SetTitle("Close_muon_vs_proton_len_400_500");
	c1.Write(); c1.Print((prefix+"/Close_muon_vs_proton_len_400_500.pdf").Data());

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	// Far muon vs proton
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	h_energy_dist_m_f_mb_0_300->Scale(1./h_energy_dist_m_f_mb_0_300->Integral());
	h_energy_dist_p_f_mb_0_300->Scale(1./h_energy_dist_p_f_mb_0_300->Integral());
	h_energy_dist_m_f_mb_0_300->SetLineColor(kBlue);
	h_energy_dist_p_f_mb_0_300->SetLineColor(kRed);
	h_energy_dist_m_f_mb_0_300->Draw(" HIST");
	h_energy_dist_p_f_mb_0_300->Draw("same HIST");
	c1.SetName("Far_muon_vs_proton_0_300");
	c1.SetTitle("Far_muon_vs_proton_0_300");
	c1.Write(); c1.Print((prefix+"/Far_muon_vs_proton_0_300.pdf").Data());

	h_energy_dist_m_f_mb_300_600->Scale(1./h_energy_dist_m_f_mb_300_600->Integral());
	h_energy_dist_p_f_mb_300_600->Scale(1./h_energy_dist_p_f_mb_300_600->Integral());
	h_energy_dist_m_f_mb_300_600->SetLineColor(kBlue);
	h_energy_dist_p_f_mb_300_600->SetLineColor(kRed);
	h_energy_dist_m_f_mb_300_600->Draw(" HIST");
	h_energy_dist_p_f_mb_300_600->Draw("same HIST");
	c1.SetName("Far_muon_vs_proton_300_600");
	c1.SetTitle("Far_muon_vs_proton_300_600");
	c1.Write(); c1.Print((prefix+"/Far_muon_vs_proton_300_600.pdf").Data());

	h_energy_dist_m_f_mb_600_800->Scale(1./h_energy_dist_m_f_mb_600_800->Integral());
	h_energy_dist_p_f_mb_600_800->Scale(1./h_energy_dist_p_f_mb_600_800->Integral());
	h_energy_dist_m_f_mb_600_800->SetLineColor(kBlue);
	h_energy_dist_p_f_mb_600_800->SetLineColor(kRed);
	h_energy_dist_m_f_mb_600_800->Draw(" HIST");
	h_energy_dist_p_f_mb_600_800->Draw("same HIST");
	c1.SetName("Far_muon_vs_proton_600_800");
	c1.SetTitle("Far_muon_vs_proton_600_800");
	c1.Write(); c1.Print((prefix+"/Far_muon_vs_proton_600_800.pdf").Data());

	h_energy_dist_m_f_mb_800_1000->Scale(1./h_energy_dist_m_f_mb_800_1000->Integral());
	h_energy_dist_p_f_mb_800_1000->Scale(1./h_energy_dist_p_f_mb_800_1000->Integral());
	h_energy_dist_m_f_mb_800_1000->SetLineColor(kBlue);
	h_energy_dist_p_f_mb_800_1000->SetLineColor(kRed);
	h_energy_dist_m_f_mb_800_1000->Draw(" HIST");
	h_energy_dist_p_f_mb_800_1000->Draw("same HIST");
	c1.SetName("Far_muon_vs_proton_800_1000");
	c1.SetTitle("Far_muon_vs_proton_800_1000");
	c1.Write(); c1.Print((prefix+"/Far_muon_vs_proton_800_1000.pdf").Data());

	//
	//
	// length
	//
	//
	h_energy_dist_m_f_mb_len_0_50->Scale(1./h_energy_dist_m_f_mb_len_0_50->Integral());
	h_energy_dist_p_f_mb_len_0_50->Scale(1./h_energy_dist_p_f_mb_len_0_50->Integral());
	h_energy_dist_m_f_mb_len_0_50->SetLineColor(kBlue);
	h_energy_dist_p_f_mb_len_0_50->SetLineColor(kRed);
	h_energy_dist_m_f_mb_len_0_50->Draw(" HIST");
	h_energy_dist_p_f_mb_len_0_50->Draw("same HIST");
	c1.SetName("Far_muon_vs_proton_len_0_50");
	c1.SetTitle("Far_muon_vs_proton_len_0_50");
	c1.Write(); c1.Print((prefix+"/Far_muon_vs_proton_len_0_50.pdf").Data());

	h_energy_dist_m_f_mb_len_50_100->Scale(1./h_energy_dist_m_f_mb_len_50_100->Integral());
	h_energy_dist_p_f_mb_len_50_100->Scale(1./h_energy_dist_p_f_mb_len_50_100->Integral());
	h_energy_dist_m_f_mb_len_50_100->SetLineColor(kBlue);
	h_energy_dist_p_f_mb_len_50_100->SetLineColor(kRed);
	h_energy_dist_m_f_mb_len_50_100->Draw(" HIST");
	h_energy_dist_p_f_mb_len_50_100->Draw("same HIST");
	c1.SetName("Far_muon_vs_proton_len_50_100");
	c1.SetTitle("Far_muon_vs_proton_len_50_100");
	c1.Write(); c1.Print((prefix+"/Far_muon_vs_proton_len_50_100.pdf").Data());

	h_energy_dist_m_f_mb_len_100_200->Scale(1./h_energy_dist_m_f_mb_len_100_200->Integral());
	h_energy_dist_p_f_mb_len_100_200->Scale(1./h_energy_dist_p_f_mb_len_100_200->Integral());
	h_energy_dist_m_f_mb_len_100_200->SetLineColor(kBlue);
	h_energy_dist_p_f_mb_len_100_200->SetLineColor(kRed);
	h_energy_dist_m_f_mb_len_100_200->Draw(" HIST");
	h_energy_dist_p_f_mb_len_100_200->Draw("same HIST");
	c1.SetName("Far_muon_vs_proton_len_100_200");
	c1.SetTitle("Far_muon_vs_proton_len_100_200");
	c1.Write(); c1.Print((prefix+"/Far_muon_vs_proton_len_100_200.pdf").Data());

	h_energy_dist_m_f_mb_len_200_300->Scale(1./h_energy_dist_m_f_mb_len_200_300->Integral());
	h_energy_dist_p_f_mb_len_200_300->Scale(1./h_energy_dist_p_f_mb_len_200_300->Integral());
	h_energy_dist_m_f_mb_len_200_300->SetLineColor(kBlue);
	h_energy_dist_p_f_mb_len_200_300->SetLineColor(kRed);
	h_energy_dist_m_f_mb_len_200_300->Draw(" HIST");
	h_energy_dist_p_f_mb_len_200_300->Draw("same HIST");
	c1.SetName("Far_muon_vs_proton_len_200_300");
	c1.SetTitle("Far_muon_vs_proton_len_200_300");
	c1.Write(); c1.Print((prefix+"/Far_muon_vs_proton_len_200_300.pdf").Data());

	h_energy_dist_m_f_mb_len_300_400->Scale(1./h_energy_dist_m_f_mb_len_300_400->Integral());
	h_energy_dist_p_f_mb_len_300_400->Scale(1./h_energy_dist_p_f_mb_len_300_400->Integral());
	h_energy_dist_m_f_mb_len_300_400->SetLineColor(kBlue);
	h_energy_dist_p_f_mb_len_300_400->SetLineColor(kRed);
	h_energy_dist_m_f_mb_len_300_400->Draw(" HIST");
	h_energy_dist_p_f_mb_len_300_400->Draw("same HIST");
	c1.SetName("Far_muon_vs_proton_len_300_400");
	c1.SetTitle("Far_muon_vs_proton_len_300_400");
	c1.Write(); c1.Print((prefix+"/Far_muon_vs_proton_len_300_400.pdf").Data());

	h_energy_dist_m_f_mb_len_400_500->Scale(1./h_energy_dist_m_f_mb_len_400_500->Integral());
	h_energy_dist_p_f_mb_len_400_500->Scale(1./h_energy_dist_p_f_mb_len_400_500->Integral());
	h_energy_dist_m_f_mb_len_400_500->SetLineColor(kBlue);
	h_energy_dist_p_f_mb_len_400_500->SetLineColor(kRed);
	h_energy_dist_m_f_mb_len_400_500->Draw(" HIST");
	h_energy_dist_p_f_mb_len_400_500->Draw("same HIST");
	c1.SetName("Far_muon_vs_proton_len_400_500");
	c1.SetTitle("Far_muon_vs_proton_len_400_500");
	c1.Write(); c1.Print((prefix+"/Far_muon_vs_proton_len_400_500.pdf").Data());

	h_energy_m_f->Draw();
	h_energy_m_c->SetLineColor(kRed);
	h_energy_m_c->Draw("same");
	c1.SetName("Close_vs_far_muon");
	c1.SetTitle("Close_vs_far_muon");
	c1.Write(); c1.Print((prefix+"/Close_vs_far_muon.pdf").Data());

	h_energy_p_f->Draw();
	h_energy_p_c->SetLineColor(kRed);
	h_energy_p_c->Draw("same");
	c1.SetName("Close_vs_far_proton");
	c1.SetTitle("Close_vs_far_proton");
	c1.Write(); c1.Print((prefix+"/Close_vs_far_proton.pdf").Data());

	h_energy_dist_m_c_mb_len_0_50->Write();
	h_energy_dist_m_c_mb_len_50_100->Write();
	h_energy_dist_m_c_mb_len_100_200->Write();
	h_energy_dist_m_c_mb_len_200_300->Write();
	h_energy_dist_m_c_mb_len_300_400->Write();
	h_energy_dist_m_c_mb_len_400_500->Write();

	h_energy_dist_m_f_mb_len_0_50->Write();
	h_energy_dist_m_f_mb_len_50_100->Write();
	h_energy_dist_m_f_mb_len_100_200->Write();
	h_energy_dist_m_f_mb_len_200_300->Write();
	h_energy_dist_m_f_mb_len_300_400->Write();
	h_energy_dist_m_f_mb_len_400_500->Write();

	h_energy_dist_p_c_mb_len_0_50->Write();
	h_energy_dist_p_c_mb_len_50_100->Write();
	h_energy_dist_p_c_mb_len_100_200->Write();
	h_energy_dist_p_c_mb_len_200_300->Write();
	h_energy_dist_p_c_mb_len_300_400->Write();
	h_energy_dist_p_c_mb_len_400_500->Write();

	h_energy_dist_p_f_mb_len_0_50->Write();
	h_energy_dist_p_f_mb_len_50_100->Write();
	h_energy_dist_p_f_mb_len_100_200->Write();
	h_energy_dist_p_f_mb_len_200_300->Write();
	h_energy_dist_p_f_mb_len_300_400->Write();
	h_energy_dist_p_f_mb_len_400_500->Write();

	cout << "Eff:" << h_energy_dist_p_c_mb_len_0_50->Integral(25, 50) / h_energy_dist_p_c_mb_len_0_50->Integral() << endl;
	cout << "MisID:" << h_energy_dist_p_c_mb_len_0_50->Integral(0, 25) / h_energy_dist_p_c_mb_len_0_50->Integral() << endl;

	cout << "Eff:" << h_energy_dist_p_c_mb_len_50_100->Integral(15, 50) / h_energy_dist_p_c_mb_len_50_100->Integral() << endl;
	cout << "MisID:" << h_energy_dist_p_c_mb_len_50_100->Integral(0, 15) / h_energy_dist_p_c_mb_len_50_100->Integral() << endl;

	cout << "Eff:" << h_energy_dist_p_c_mb_len_100_200->Integral(12, 50) / h_energy_dist_p_c_mb_len_100_200->Integral() << endl;
	cout << "MisID:" << h_energy_dist_p_c_mb_len_100_200->Integral(0, 12) / h_energy_dist_p_c_mb_len_100_200->Integral() << endl;

	cout << "Eff:" << h_energy_dist_p_c_mb_len_200_300->Integral(9, 50) / h_energy_dist_p_c_mb_len_200_300->Integral() << endl;
	cout << "MisID:" << h_energy_dist_p_c_mb_len_200_300->Integral(0, 9) / h_energy_dist_p_c_mb_len_200_300->Integral() << endl;

	cout << "Eff:" << h_energy_dist_p_c_mb_len_300_400->Integral(8, 50) / h_energy_dist_p_c_mb_len_300_400->Integral() << endl;
	cout << "MisID:" << h_energy_dist_p_c_mb_len_300_400->Integral(0, 8) / h_energy_dist_p_c_mb_len_300_400->Integral() << endl;

	cout << "Eff:" << h_energy_dist_p_c_mb_len_400_500->Integral(8, 50) / h_energy_dist_p_c_mb_len_400_500->Integral() << endl;
	cout << "MisID:" << h_energy_dist_p_c_mb_len_400_500->Integral(0, 8) / h_energy_dist_p_c_mb_len_400_500->Integral() << endl;


cout << endl;


	cout << "Eff:" << h_energy_dist_p_f_mb_len_0_50->Integral(16, 50) / h_energy_dist_p_f_mb_len_0_50->Integral() << endl;
	cout << "MisID:" << h_energy_dist_p_f_mb_len_0_50->Integral(0, 16) / h_energy_dist_p_f_mb_len_0_50->Integral() << endl;

	cout << "Eff:" << h_energy_dist_p_f_mb_len_50_100->Integral(10, 50) / h_energy_dist_p_f_mb_len_50_100->Integral() << endl;
	cout << "MisID:" << h_energy_dist_p_f_mb_len_50_100->Integral(0, 10) / h_energy_dist_p_f_mb_len_50_100->Integral() << endl;

	cout << "Eff:" << h_energy_dist_p_f_mb_len_100_200->Integral(7, 50) / h_energy_dist_p_f_mb_len_100_200->Integral() << endl;
	cout << "MisID:" << h_energy_dist_p_f_mb_len_100_200->Integral(0, 7) / h_energy_dist_p_f_mb_len_100_200->Integral() << endl;

	cout << "Eff:" << h_energy_dist_p_f_mb_len_200_300->Integral(6, 50) / h_energy_dist_p_f_mb_len_200_300->Integral() << endl;
	cout << "MisID:" << h_energy_dist_p_f_mb_len_200_300->Integral(0, 6) / h_energy_dist_p_f_mb_len_200_300->Integral() << endl;

	cout << "Eff:" << h_energy_dist_p_f_mb_len_300_400->Integral(5, 50) / h_energy_dist_p_f_mb_len_300_400->Integral() << endl;
	cout << "MisID:" << h_energy_dist_p_f_mb_len_300_400->Integral(0, 5) / h_energy_dist_p_f_mb_len_300_400->Integral() << endl;

	cout << "Eff:" << h_energy_dist_p_f_mb_len_400_500->Integral(5, 50) / h_energy_dist_p_f_mb_len_400_500->Integral() << endl;
	cout << "MisID:" << h_energy_dist_p_f_mb_len_400_500->Integral(0, 5) / h_energy_dist_p_f_mb_len_400_500->Integral() << endl;


	outfile->Close();

}