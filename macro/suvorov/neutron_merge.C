#include "TFile.h"
#include "TH2F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TPad.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"

#include <iostream>

void neutron_merge() {
	TFile* file = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v3-UseXY-UseXZ-UseYZ-Separate10_na_200000.root", "READ");

	TH2F* init_e_cos  = (TH2F*)file->Get("ini_ET");
	TH2F* eff_e_cos   = (TH2F*)file->Get("eff_ET");
	TH2F* pe_e_cos    = (TH2F*)file->Get("pe_ET");
	TH2F* hits_energy = (TH2F*)file->Get("hitsE");
	TH2F* hits_number = (TH2F*)file->Get("hitsN");
	TH2F* e_dist      = (TH2F*)file->Get("e_dist");
  TH2F* beta_ekin   = (TH2F*)file->Get("beta_ekin");
  TH2F* beta_ekin_sm= (TH2F*)file->Get("beta_ekin_sm");

  TH1F* mom_forward = (TH1F*)file->Get("mom");
  TH1F* mom_norm    = (TH1F*)file->Get("mom1");

  TH2F* pe_e        = (TH2F*)file->Get("pe_E");

  Int_t rebin_Y = 5;

  TH2F* energy_resol[6];
  TH2F* energy_resol_rebin[6];

  TGraphAsymmErrors* graph[6];

  const Int_t Nbins = 28;
  Double_t bins[Nbins] = {0., 5., 10., 15., 20., 30., 40., 50., 60., 70., 80., 90., 100., 110., 120., 130., 140., 150., 170., 190., 210., 230., 250., 300., 350., 400., 450., 500.};

  TString histo_name;
  for (Int_t i = 1; i <= 6; ++i) {
    histo_name = Form("energy_resol%i", i);
    energy_resol[i-1] = (TH2F*)file->Get(histo_name);
    //energy_resol[i-1]->RebinY(2);
    //energy_resol[i-1]->RebinX(2);
    energy_resol_rebin[i-1] = new TH2F(Form("energy_resol_rebin%i", i), "", Nbins-1, bins, energy_resol[i-1]->GetYaxis()->GetNbins(), 0., 500.);
    for (Int_t xbin = 0; xbin <= energy_resol[i-1]->GetXaxis()->GetNbins(); ++xbin) {
      for (Int_t ybin = 0; ybin <= energy_resol[i-1]->GetYaxis()->GetNbins(); ++ybin) {
        Double_t x_center = energy_resol[i-1]->GetXaxis()->GetBinCenter(xbin);
        Double_t y_center = energy_resol[i-1]->GetYaxis()->GetBinCenter(ybin);
        energy_resol_rebin[i-1]->Fill(x_center, y_center, energy_resol[i-1]->GetBinContent(xbin, ybin));
      }
    }
  }

  TFile* file_out = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/plot3_gamma_VA3.root", "RECREATE");
  file_out->cd();

  TF1* f1 = new TF1("f1", "( (x < [1] ) ? [0] * TMath::Gaus(x, [1], [2]) : [0] * TMath::Gaus(x, [1], [3]))", 0, 500);
  f1->SetParName(0, "Const");
  f1->SetParName(1, "Mean");
  f1->SetParName(2, "Left sigma");
  f1->SetParName(3, "Right sigma");
  /*

  for (Int_t resID = 1; resID <= 6; ++resID ) {
    TH1D* fitted_histo[Nbins];
    graph[resID-1] = new TGraphAsymmErrors(); 
    gStyle->SetOptFit(1);
    for (Int_t i = 0; i < Nbins -1; ++i) {
      fitted_histo[i] = energy_resol_rebin[resID-1]-> ProjectionY(Form("projections%i_%i", resID, i), i+1, i+1);
      if (i > 8)
        fitted_histo[i]->Rebin(2);
      if (i > 18 && i != 27)
        fitted_histo[i]->Rebin(2);
  
      f1->SetParameters(200, 70, 10, 80);
      fitted_histo[i]->Fit("f1");
      fitted_histo[i]->Draw();
      gPad->Update();
      if (resID == 1)
        fitted_histo[i]->Write();
  
      TF1 *fit = fitted_histo[i]->GetFunction("f1");
  
      if (!fit || i == 0) continue;
  
      Float_t mean      = fit->GetParameter(1);
      Float_t sigmaL    = fit->GetParameter(2);
      Float_t sigmaR    = fit->GetParameter(3);

      if (sigmaL < 0)
        sigmaL = 0;
      if (sigmaR > 500)
        sigmaR = 0;

      if (mean < 0)
        continue;
  
      Double_t x_center = energy_resol_rebin[resID-1]->GetXaxis()->GetBinCenter(i);
  
      graph[resID-1]->SetPoint(graph[resID-1]->GetN(), x_center, mean);
      graph[resID-1]->SetPointError(graph[resID-1]->GetN()-1, 0., 0., sigmaL, sigmaR);
    }
  
    graph[resID-1]->SetMaximum(500.);
    graph[resID-1]->SetMinimum(0.);
    graph[resID-1]->Draw("ap");
    gPad->Update();
    graph[resID-1]->Write(Form("graph%i", resID));
  }
  */

	// do rebinning
	
	init_e_cos->RebinY(rebin_Y);
	pe_e_cos->RebinY(rebin_Y);
	eff_e_cos->RebinY(rebin_Y);
  mom_forward->Rebin(rebin_Y);
  mom_norm->Rebin(rebin_Y);

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

  // energy distance study
  // TH2F* e_dist      = new TH2F("e_dist", "Energy vs distance", 300, 0., 1500, 250, 0., 500.);
  TH2F* e_d_c = (TH2F*)e_dist->Clone("e_d_c");
  TH1D* dist = e_d_c->ProjectionX("dist", 13, -1);
  dist->Rebin(5);
  dist->SetTitle("First hit distance. Ekin > 26 MeV");
  dist->Write();
  TH1D* dist_eff = (TH1D*)dist->Clone("Energy distance");
  dist_eff->SetTitle("Efficiency vs distance cut");
  for (Int_t i = 1; i <= dist->GetNbinsX(); ++i) {
    dist_eff->SetBinContent(i, dist->Integral(i, -1));
  }
  dist_eff->Scale(1./dist->Integral());
  dist_eff->Write();

  mom_forward->Divide(mom_norm);

  TF1* beta = new TF1("beta", "sqrt(x*x + 2*x*939)/(x+939)", 0., 500.);
  (void)beta;
  TCanvas* c1 = new TCanvas("canva","",50,50,1000,800);
  TF1* resol_100_05 = new TF1("resol_100_09", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/100*0.5*30", 0., 500.);
  resol_100_05->Draw();
  c1->Write();
  TF1* resol_20_09 = new TF1("resol_20_09", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/20*0.9*30", 0., 500.);
  resol_20_09->Draw();
  c1->Write();

  TF1* resol_50_09 = new TF1("resol_50_09", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/50*0.9*30", 0., 500.);
  TF1* resol_50_07 = new TF1("resol_50_07", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/50*0.7*30", 0., 500.);
  resol_50_07->SetLineColor(kGreen);
  TF1* resol_50_05 = new TF1("resol_50_05", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/50*0.5*30", 0., 500.);
  resol_50_05->SetLineColor(kBlue);
  TF1* resol_50_03 = new TF1("resol_50_03", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/50*0.3*30", 0., 500.);
  resol_50_03->SetLineColor(kYellow);
  resol_50_09->Draw();
  resol_50_07->Draw("same");
  resol_50_05->Draw("same");
  resol_50_03->Draw("same");
  c1->Write();

  //TF1* resol_20_09 = new TF1("resol_20_09", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/20*0.9*30", 0., 500.);
  TF1* resol_20_07 = new TF1("resol_20_07", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/20*0.7*30", 0., 500.);
  resol_20_07->SetLineColor(kGreen);
  TF1* resol_20_05 = new TF1("resol_20_05", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/20*0.5*30", 0., 500.);
  resol_20_05->SetLineColor(kBlue);
  TF1* resol_20_03 = new TF1("resol_20_03", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/20*0.3*30", 0., 500.);
  resol_20_03->SetLineColor(kYellow);
  resol_20_09->Draw();
  resol_20_07->Draw("same");
  resol_20_05->Draw("same");
  resol_20_03->Draw("same");
  c1->Write();

  init_e_cos->Write();
  eff_e_cos->Write();
  eff_e_cos_2->Write();
  pe_e_cos->Write();
  pe_e->Write();
  hits_energy->Write();
  hits_number->Write();
  e_dist->Write();
  beta_ekin->Write();
  beta_ekin_sm->Write();

  mom_forward->Draw("E1");
  gPad->Modified();
  gPad->Update();
  mom_forward->Write();

  for (Int_t i = 1; i <= 6; ++i) {
    energy_resol[i-1]->Write();
    energy_resol_rebin[i-1]->Write();
  }

  file_out->Close();
  file->Close();
}