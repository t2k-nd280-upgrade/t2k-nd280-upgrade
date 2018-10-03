#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TPad.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TRandom3.h"

#include <iostream>

void neutron_merge() {
	TFile* file = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v8-UseXY-UseXZ-UseYZ-Separate10_na_1000000.root", "READ");

  const Int_t Ndist = 7;
  float distance_cut[Ndist];
  distance_cut[0] = 10.;
  distance_cut[1] = 20.;
  distance_cut[2] = 30.;
  distance_cut[3] = 40.;
  distance_cut[4] = 50.;
  distance_cut[5] = 60.;
  distance_cut[6] = 70.;

  TTree* tree       = (TTree*)file->Get("neutron");
  Double_t ekin, costheta, light, dist_cubes, dist, first_hit_time, neutron_time, neutron_dist_true;
  Double_t dir_true[3], dir_reco[3];
  tree->SetBranchAddress("KinEnergy_true",     &ekin);
  tree->SetBranchAddress("CosTheta_true",      &costheta);
  tree->SetBranchAddress("Light",              &light);

  tree->SetBranchAddress("Dir_True",           dir_true);
  tree->SetBranchAddress("Dir_Reco",           dir_reco);
  // reco info 
  tree->SetBranchAddress("Distance_cubes",     &dist_cubes);
  tree->SetBranchAddress("First_hit_time",     &first_hit_time);
  // true distance (no smearing with cube size)
  tree->SetBranchAddress("Distance_hit_true",  &dist);
  
  // true neutron points
  tree->SetBranchAddress("Neutron_time",       &neutron_time);
  tree->SetBranchAddress("Neutron_dist",       &neutron_dist_true);

	TH2F* init_e_cos  = (TH2F*)file->Get("ini_ET");
	TH2F* eff_e_cos   = (TH2F*)file->Get("eff_ET");
	TH2F* pe_e_cos    = (TH2F*)file->Get("pe_ET");
	TH2F* hits_energy = (TH2F*)file->Get("hitsE");
	TH2F* hits_number = (TH2F*)file->Get("hitsN");
	TH2F* e_dist      = (TH2F*)file->Get("e_dist");
  TH2F* beta_ekin   = (TH2F*)file->Get("beta_ekin");
  //TH2F* beta_ekin_sm= (TH2F*)file->Get("beta_ekin_sm");

  TH1F* mom_forward = (TH1F*)file->Get("mom");
  TH1F* mom_norm    = (TH1F*)file->Get("mom1");

  TH2F* pe_e        = (TH2F*)file->Get("pe_E");

  // do rebinning
  Int_t rebin_Y = 5;
  init_e_cos->RebinY(rebin_Y);
  pe_e_cos->RebinY(rebin_Y);
  eff_e_cos->RebinY(rebin_Y);
  mom_forward->Rebin(rebin_Y);
  mom_norm->Rebin(rebin_Y);

  TFile* file_out = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/plot_neutron_VA3.root", "RECREATE");
  file_out->cd();

  // read tree several times in order to check
  for (Int_t distID = 0; distID < Ndist; ++distID) {

    TH2F* energy_resol[7];
    TH2F* energy_resol_rebin[7];

    TH1F* energy_acc[7];

    TH2F* beta_ekin_sm[7];

    TH2F* angle_smearing = new TH2F("angle_smearing", "Angle smearing", 50, -1., 1., 50, -1., 1.);
  
    const Int_t Nbins = 28;
    Double_t bins[Nbins] = {0., 5., 10., 15., 20., 30., 40., 50., 60., 70., 80., 90., 100., 110., 120., 130., 140., 150., 170.  , 190., 210., 230., 250., 300., 350., 400., 450., 500.};
  
    for (Int_t i = 0; i < 7; ++i) {
      energy_resol[i]       = new TH2F(Form("energy_smearng_%i", i), "energy smearing matrix", 250, 0., 500, 300, 0., 600);
      energy_resol_rebin[i] = new TH2F(Form("energy_resol_rebin_%i", i), "", Nbins-1, bins, 300, 0., 600.);
      energy_acc[i]         = new TH1F(Form("energy_acc_%i", i), "energy accuracy in the ROI", 120, -60., 60.);
      beta_ekin_sm[i]       = (TH2F*)beta_ekin->Clone(Form("beta_ekin_smeared_%i", i));
      beta_ekin_sm[i]->Reset();
    }
    eff_e_cos->Reset();
  
    // loop over tree to fill the resolution histoes
    float beta, time, ekin2;
    TRandom3* gen = new TRandom3();
    Int_t NEntries = tree->GetEntries();
    for (Int_t entryID = 0; entryID < NEntries; ++entryID) {
      tree->GetEntry(entryID);
  
      // selection on distance (cm)
      if (dist_cubes < distance_cut[distID])
        continue;
  
      eff_e_cos->Fill(costheta, ekin);

      Double_t time_sigma[7] = {0., 0.9, 0.9/sqrt(2), 0.9/sqrt(3), 0.6, 0.6/sqrt(2), 0.6/sqrt(3)};
  
      //beta = neutron_dist_true / (neutron_time * 30.);
      beta = dist_cubes / (first_hit_time * 30.);
      ekin2 = sqrt(939.565379*939.565379 / (1 - beta*beta)) - 939.565379;
      energy_resol[0]->Fill(ekin, ekin2);
      energy_resol_rebin[0]->Fill(ekin, ekin2);
      if (ekin > 70. && ekin < 105.)
        energy_acc[0]->Fill(ekin2 - ekin);

      angle_smearing->Fill(dir_true[2], dir_reco[2]);

      for (Int_t resID = 1; resID < 7; ++resID ) {
  
        time = gen->Gaus(first_hit_time, time_sigma[resID]);
        beta = dist_cubes / (time * 30.);
        ekin2 = sqrt(939.565379*939.565379 / (1 - beta*beta)) - 939.565379;
        energy_resol[resID]->Fill(ekin, ekin2);
        energy_resol_rebin[resID]->Fill(ekin, ekin2);
        beta_ekin_sm[resID]->Fill(ekin, beta);
        if (ekin > 70. && ekin < 105.)
          energy_acc[resID]->Fill(ekin2 - ekin);
      }
    }
  
    TGraphAsymmErrors* graph[6];
    TGraphAsymmErrors* graph_resol[6];
  
    TF1* f1 = new TF1("f1", "( (x < [1] ) ? [0] * TMath::Gaus(x, [1], [2]) : [0] * TMath::Gaus(x, [1], [3]))", 0, 700);
    f1->SetParName(0, "Const");
    f1->SetParName(1, "Mean");
    f1->SetParName(2, "Left sigma");
    f1->SetParName(3, "Right sigma");
    
    TDirectory* dir = file_out->mkdir(Form("CutValue_%i", (int)distance_cut[distID]));
    dir->cd();
    
    // loop over projections to fit
    for (Int_t resID = 0; resID < 7; ++resID ) {
      TH1D* fitted_histo[Nbins+5];

      graph[resID] = new TGraphAsymmErrors();
      graph[resID]->SetName("smearing_graph");
      graph_resol[resID] = new TGraphAsymmErrors();
      graph_resol[resID]->SetName("resol_graph");

      gStyle->SetOptFit(1);
      for (Int_t i = 2; i <= Nbins; ++i) {
        fitted_histo[i] = energy_resol_rebin[resID]-> ProjectionY(Form("projections_%i_%i", resID, i), i, i);
        if (i > 8 && i < 19)
          fitted_histo[i]->Rebin(2);
        if (i > 18)
          fitted_histo[i]->Rebin(5);
  
        float pre_mean = energy_resol_rebin[resID]->GetXaxis()->GetBinCenter(i);
    
        f1->SetParameters(100, pre_mean, pre_mean*0.1, pre_mean*0.1);
        fitted_histo[i]->Fit("f1", "Q");
        fitted_histo[i]->Draw();
        gPad->Update();
        //if (resID == 3)
        //  fitted_histo[i]->Write();
    
        TF1 *fit = fitted_histo[i]->GetFunction("f1");
    
        if (!fit) continue;
    
        Float_t mean      = fit->GetParameter(1);
        Float_t sigmaL    = fit->GetParameter(2);
        Float_t sigmaR    = fit->GetParameter(3);
        Float_t mean_e     = fit->GetParError(1);
        Float_t sigma_e    = fit->GetParError(3);
  
        if (sigmaL < 0)
          sigmaL = 0;
        if (sigmaR > 500)
          sigmaR = 0;
  
        if (mean < 0)
          continue;
    
        Double_t x_center = energy_resol_rebin[resID]->GetXaxis()->GetBinCenter(i);
    
        graph[resID]->SetPoint(graph[resID]->GetN(), x_center, mean);
        graph[resID]->SetPointError(graph[resID]->GetN()-1, 0., 0., sigmaL, sigmaR);
        
        graph_resol[resID]->SetPoint(graph_resol[resID]->GetN(), x_center, sigmaR/mean);
        float err = sigmaR/mean * sqrt(mean_e * mean_e / mean /mean + sigma_e * sigma_e / sigmaR / sigmaR);
        graph_resol[resID]->SetPointError(graph_resol[resID]->GetN()-1, 0., 0., err, err);
        
      }
    
      graph[resID]->SetMaximum(600.);
      graph[resID]->SetMinimum(0.);
      graph[resID]->Draw("ap");
      gPad->Update();
      graph[resID]->Write(Form("smearing_graph_%i", resID));
      graph_resol[resID]->Write(Form("resol_graph_%i", resID));
    } // loop over projections to fit
    // draw resolution and efficiency for each distance
    TH2F* eff_e_cos_2 = (TH2F*)eff_e_cos->Clone("eff_ecos2");

    for (Int_t i = 1; i <= init_e_cos->GetXaxis()->GetNbins(); ++i) {
      for (Int_t j = 1; j <= init_e_cos->GetYaxis()->GetNbins(); ++j) {
        Float_t pre = eff_e_cos->GetBinContent(i, j);
        Float_t scale = init_e_cos->GetBinContent(i, j);
        eff_e_cos_2->SetBinContent(i, j, pre / scale);
      }
    }
    eff_e_cos_2->Write("efficiency_vs_Ekin_theta");
    angle_smearing->Write();

    for (Int_t i = 0; i < 7; ++i) {
      energy_resol[i]->Write();
      energy_acc[i]->Write();
      beta_ekin_sm[i]->Write();
      //energy_resol_rebin[i]->Write();
    }

  } // loop over distance cuts

  for (Int_t i = 1; i <= pe_e_cos->GetXaxis()->GetNbins(); ++i) {
    for (Int_t j = 1; j <= pe_e_cos->GetYaxis()->GetNbins(); ++j) {
      Float_t pre = pe_e_cos->GetBinContent(i, j);
      Float_t scale = eff_e_cos->GetBinContent(i, j);
      pe_e_cos->SetBinContent(i, j, pre / scale);
    }
  }
  file_out->cd();
  // energy distance study
  // TH2F* e_dist      = new TH2F("e_dist", "Energy vs distance", 300, 0., 1500, 250, 0., 500.);
  TH2F* e_d_c = (TH2F*)e_dist->Clone("e_d_c");
  TH1D* dist_proj = e_d_c->ProjectionX("dist", 13, -1);
  dist_proj->Rebin(5);
  dist_proj->SetTitle("First hit distance. Ekin > 26 MeV");
  dist_proj->Write();
  TH1D* dist_eff = (TH1D*)dist_proj->Clone("Energy distance");
  dist_eff->SetTitle("Efficiency vs distance cut");
  for (Int_t i = 1; i <= dist_proj->GetNbinsX(); ++i) {
    dist_eff->SetBinContent(i, dist_proj->Integral(i, -1));
  }
  dist_eff->Scale(1./dist_proj->Integral());
  dist_eff->Write();

  mom_forward->Divide(mom_norm);

  TF1* beta_f = new TF1("beta", "sqrt(x*x + 2*x*939)/(x+939)", 0., 500.);
  (void)beta_f;
  TCanvas* c1 = new TCanvas("canva","",50,50,1000,800);
  /*TF1* resol_100_05 = new TF1("resol_100_09", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/100*0.5*30", 0., 500.);
  resol_100_05->Draw();
  c1->Write();
  TF1* resol_20_09 = new TF1("resol_20_09", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/20*0.9*30", 0., 500.);
  resol_20_09->Draw();
  c1->Write();*/

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
  c1->SetName("Dist20");
  c1->SetTitle("Analytical resol for > 50 cm. .9, .7, .5, .3 ns time resol");
  c1->Write();

  TF1* resol_20_09 = new TF1("resol_20_09", "1/x * 939* TMath::Power((1-beta(x)*beta(x)), -1.5) * TMath::Power(beta(x), 3)/20*0.9*30", 0., 500.);
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
  c1->SetName("Dist20");
  c1->SetTitle("Analytical resol for > 20 cm. .9, .7, .5, .3 ns time resol");
  c1->Write();

  init_e_cos->Write();
  eff_e_cos->Write();
  pe_e_cos->Write();
  pe_e->Write();
  hits_energy->Write();
  hits_number->Write();
  e_dist->Write();
  beta_ekin->Write();
  //beta_ekin_sm->Write();

  mom_forward->Draw("E1");
  gPad->Modified();
  gPad->Update();
  mom_forward->Write();

  file_out->Close();
  file->Close();
}