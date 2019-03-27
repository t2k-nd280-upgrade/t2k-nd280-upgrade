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

const float MIP_LY_AV3 = 50;

//hadd -f /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_1000000.root /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt0_NEvt1000.root /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt1* /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt2* /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt3* /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt4* /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt5* /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt6* /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt7000_NEvt1000.root /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt8000_NEvt1000.root /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt9000_NEvt1000.root /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt7?000_NEvt1000.root /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt8?000_NEvt1000.root /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v15-UseXY-UseXZ-UseYZ-Separate10_na_Evt9?000_NEvt1000.root

using namespace std;

void neutron_merge() {
	TFile* file = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SuperFGD-neutron_v18-UseXY-UseXZ-UseYZ-Separate10_na_1000000.root", "READ");

  const Int_t Ndist = 9;
  float distance_cut[Ndist];
  distance_cut[0] = 10.;
  distance_cut[1] = 20.;
  distance_cut[2] = 30.;
  distance_cut[3] = 40.;
  distance_cut[4] = 50.;
  distance_cut[5] = 60.;
  distance_cut[6] = 70.;
  distance_cut[7] = 0.;
  // 50 - 100 cm
  distance_cut[8] = 100.;

  TTree* tree       = (TTree*)file->Get("neutron");
  Double_t ekin, costheta, dist_cubes, dist, first_hit_time, neutron_time, neutron_dist_true;
  Double_t dir_true[3], dir_reco[3];
  Double_t light_fst, light_max, light_tot;
  Double_t N_hits_XY, N_hits_XZ, N_hits_YZ; 
  Double_t HM_proton;
  Int_t first_sc_reco;

  tree->SetBranchAddress("KinEnergy_true",     &ekin);
  tree->SetBranchAddress("CosTheta_true",      &costheta);
  tree->SetBranchAddress("Light_fst",          &light_fst);
  tree->SetBranchAddress("Light_max",          &light_max);
  tree->SetBranchAddress("Light_tot",          &light_tot);

  tree->SetBranchAddress("Dir_True",           dir_true);
  tree->SetBranchAddress("Dir_Reco",           dir_reco);

  tree->SetBranchAddress("HM_proton",          &HM_proton);
  // reco info 
  tree->SetBranchAddress("Distance_cubes",     &dist_cubes);
  tree->SetBranchAddress("First_hit_time",     &first_hit_time);
  // true distance (no smearing with cube size)
  tree->SetBranchAddress("Distance_hit_true",  &dist);
  tree->SetBranchAddress("First_sc_reco",      &first_sc_reco);
  
  // true neutron points
  tree->SetBranchAddress("Neutron_time",       &neutron_time);
  tree->SetBranchAddress("Neutron_dist",       &neutron_dist_true);

  tree->SetBranchAddress("N_hits_XY",          &N_hits_XY);
  tree->SetBranchAddress("N_hits_XZ",          &N_hits_XZ);
  tree->SetBranchAddress("N_hits_YZ",          &N_hits_YZ);

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

  TH1F* hits_cl_XY = (TH1F*)file->Get("hitsN_XY");
  TH1F* hits_cl_XZ = (TH1F*)file->Get("hitsN_XZ");
  TH1F* hits_cl_YZ = (TH1F*)file->Get("hitsN_YZ");

  Int_t test_light_n = 0;
  Float_t test_light = 0.;

  // do rebinning
  Int_t rebin_Y = 5;
  init_e_cos->RebinY(rebin_Y);
  pe_e_cos->RebinY(rebin_Y);
  eff_e_cos->RebinY(rebin_Y);
  mom_forward->Rebin(rebin_Y);
  mom_norm->Rebin(rebin_Y);

  TFile* file_out = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/plot_neutron_v24.root", "RECREATE");
  file_out->cd();

  for (Int_t i = 1; i <= pe_e_cos->GetXaxis()->GetNbins(); ++i) {
    for (Int_t j = 1; j <= pe_e_cos->GetYaxis()->GetNbins(); ++j) {
      Float_t pre = pe_e_cos->GetBinContent(i, j);
      Float_t scale = eff_e_cos->GetBinContent(i, j);
      pe_e_cos->SetBinContent(i, j, pre / scale);
    }
  }

  const Int_t time_size = 6;
  Double_t time_sigma[time_size] = {0., 1.5/sqrt(3), 0.6/sqrt(3), 0., 0., 0.};

  // read tree several times in order to check
  for (Int_t distID = 0; distID < Ndist; ++distID) {

    TH2F* energy_resol[time_size];
    TH2F* energy_resol_rebin[time_size];

    TH1F* energy_acc[time_size];

    TH2F* beta_ekin_sm[time_size];

    TH1F* reco_first = new TH1F("recp_first", "Reco 1st scatter", 400, 0., 800.);
    TH1F* reco_first_n = new TH1F("recp_first_n", "Reco 1st scatter", 400, 0., 800.);

    TH1F* test_l_e   = new TH1F("ly_energy", "Averagy l.y.", 250, 0., 500.);
    TH1F* test_l_e_n = new TH1F("test1", "test", 250, 0., 500.);

    TH2F* angle_smearing = new TH2F("angle_smearing", "Angle smearing", 50, -1., 1., 50, -1., 1.);

    TH1F* time_resol  = new TH1F("time_res", "Time resolution", 500, 0., 6000.);
    TH1F* ly_fst     = new TH1F("ly_fst", "Light yield first", 250, 0., 6000.);
    TH1F* ly_tot     = new TH1F("ly_tot", "Light yield total", 250, 0., 6000.);
    TH1F* ly_max     = new TH1F("ly_max", "Light yield max", 250, 0., 6000.);

    TH1F* proton_mom = new TH1F("proton_mom", "Proton momentum", 250, 0., 500.);

    TH1F* test_light_y     = new TH1F("test_light_y", "Light yield", 6000, 0., 6000.);
  
    const Int_t Nbins = 31;
    Double_t bins[Nbins] = {0., 5., 10., 15., 20., 30., 40., 50., 60., 70., 80., 90., 100., 110., 120., 130., 140., 150., 170.  , 190., 210., 230., 250., 300., 350., 400., 450., 500., 600., 700., 800.};

    const Int_t Nbins_cos = 11;
    Double_t bins_cos[Nbins_cos] = {-1., -0.8, -0.6, -0.4, -0.2, 0., 0.2, 0.4, 0.6, 0.8, 1.};
    TH1F* cos_h = new TH1F("cos_h", "", Nbins_cos-1, bins_cos);

    TH2F* energy_resol_cos[time_size][Nbins_cos-1];
  
    for (Int_t i = 0; i < time_size; ++i) {
      energy_resol[i]       = new TH2F(Form("energy_smearng_%i", i), "energy smearing matrix", 400, 0., 800, 600, 0., 1200);
      energy_resol_rebin[i] = new TH2F(Form("energy_resol_rebin_%i", i), "", Nbins-1, bins, 600, 0., 1200.);
      energy_acc[i]         = new TH1F(Form("energy_acc_%i", i), "energy accuracy in the ROI", 120, -1., 1.);
      beta_ekin_sm[i]       = (TH2F*)beta_ekin->Clone(Form("beta_ekin_smeared_%i", i));
      beta_ekin_sm[i]->Reset();

      for (Int_t j = 0; j < Nbins_cos-1; ++j) {
        energy_resol_cos[i][j] = new TH2F(Form("energy_smearng_%i_cos%i", i, j), "energy smearing matrix", 400, 0., 800, 600, 0., 1200);
      }
    }
    eff_e_cos->Reset();
  
    // loop over tree to fill the resolution histoes
    float beta, time, ekin2;
    TRandom3* gen = new TRandom3();
    Int_t NEntries = tree->GetEntries();
    for (Int_t entryID = 0; entryID < NEntries; ++entryID) {
      tree->GetEntry(entryID);
  
      // selection on distance (cm)
      if (distID != 8)
        if (dist_cubes < distance_cut[distID])// || dist_cubes > distance_cut[distID+1])
          continue;

      if (distID == 8 && (dist_cubes < 50. || dist_cubes > 100.))
        continue;

      // selection on light
      if (light_tot / 3. < 50.)
        continue;

      reco_first->Fill(ekin, first_sc_reco);
      reco_first_n->Fill(ekin);

      if (first_sc_reco == 0) 
        continue;
  
      eff_e_cos->Fill(costheta, ekin);
  
      //beta = neutron_dist_true / (neutron_time * 30.);
      beta = dist_cubes / (first_hit_time * 30.);
      ekin2 = sqrt(939.565379*939.565379 / (1 - beta*beta)) - 939.565379;
      energy_resol[0]->Fill(ekin, ekin2);
      energy_resol_rebin[0]->Fill(ekin, ekin2);
      if (ekin > 90. && ekin < 110.)
        energy_acc[0]->Fill((ekin2 - ekin) / ekin);

      angle_smearing->Fill(dir_true[2], dir_reco[2]);

      for (Int_t resID = 1; resID < time_size; ++resID ) {
  
        if (resID > 2 ) {
          if (!light_tot)
            cout << "no light!!!  " << light_fst << endl;
          float light_fiber = light_tot / 3.;
          float res;
          if (resID == 3) {
            res      = 1.5 / sqrt(3);
            res           *= sqrt(MIP_LY_AV3 / light_fiber);
          } if (resID == 4 || resID == 5) {
            res      = 0.92 / sqrt(3);
            res           *= sqrt(50 / light_fiber);
          }

          time_resol->Fill(1000 * res);
          ly_fst->Fill(light_fst);
          ly_max->Fill(light_max);
          ly_tot->Fill(light_tot);
          time_sigma[resID] = res;

          if (ekin > 400) {
            test_light += light_tot;
            ++test_light_n;
            test_light_y->Fill(light_tot);
          }

          test_l_e->Fill(ekin, light_tot);
          test_l_e_n->Fill(ekin);
          proton_mom->Fill(HM_proton);
        }


        float time_vertex = 0.;
        if (resID == 5) {
          time_vertex = gen->Gaus(0., 1.);
        }

        time = gen->Gaus(first_hit_time, time_sigma[resID]);
        time -= time_vertex;
        if (time < 0.)
          time = 0.;
        beta = dist_cubes / (time * 30.);

        if (beta < 0)
          continue;
        ekin2 = sqrt(939.565379*939.565379 / (1 - beta*beta)) - 939.565379;

        energy_resol[resID]->Fill(ekin, ekin2);
        energy_resol_rebin[resID]->Fill(ekin, ekin2);
        Int_t cos_bin = cos_h->FindBin(costheta);
        //energy_resol_cos[resID][cos_bin-1]->Fill(ekin, ekin2);
        beta_ekin_sm[resID]->Fill(ekin, beta);
        if (ekin > 90. && ekin < 110.)
          energy_acc[resID]->Fill((ekin2 - ekin) / ekin);
      }
    }
  
    TGraphAsymmErrors* graph[time_size];
    TGraphAsymmErrors* graph_resol[time_size];
  
    TF1* f1 = new TF1("f1", "( (x < [1] ) ? [0] * TMath::Gaus(x, [1], [2]) : [0] * TMath::Gaus(x, [1], [3]))", 0, 700);
    f1->SetParName(0, "Const");
    f1->SetParName(1, "Mean");
    f1->SetParName(2, "Left sigma");
    f1->SetParName(3, "Right sigma");
    
    TDirectory* dir = file_out->mkdir(Form("CutValue_%i", (int)distance_cut[distID]));
    dir->cd();
    
    // loop over projections to fit
    for (Int_t resID = 0; resID < time_size; ++resID ) {
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
    time_resol->Write();
    ly_fst->Write();
    ly_max->Write();
    ly_tot->Write();
    reco_first->Divide(reco_first_n);
    reco_first->Write();

    TH1F* ly_cut = (TH1F*)ly_tot->Clone("ly_cut");
    ly_cut->Reset();

    for (Int_t i = 1; i <= ly_cut->GetXaxis()->GetNbins(); ++i)
      ly_cut->SetBinContent(i, ly_tot->Integral(i, ly_cut->GetXaxis()->GetNbins()+1));
    ly_cut->Write();

    for (Int_t i = 0; i < time_size; ++i) {
      energy_resol[i]->Write();
      energy_acc[i]->Write();
      beta_ekin_sm[i]->Write();
      //energy_resol_rebin[i]->Write();
    }

    TDirectory* dir2 = dir->mkdir("CosTheta_binning");
    dir2->cd();
    for (Int_t i = 0; i < time_size; ++i) {
      for (Int_t j = 0; j < Nbins_cos-1; ++j) {
        energy_resol_cos[i][j]->Write();
      }
    }
    dir->cd();

    for (Int_t i = 1; i <= test_l_e->GetXaxis()->GetNbins(); ++i) {
      Float_t pre = test_l_e->GetBinContent(i);
      Float_t scale = test_l_e_n->GetBinContent(i);
      test_l_e->SetBinContent(i, pre / scale);
    }

    test_l_e->Write();
    test_light_y->Write();
    proton_mom->Write();

  } // loop over distance cuts

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

  hits_cl_XY->Write();
  hits_cl_XZ->Write();
  hits_cl_YZ->Write();
  //beta_ekin_sm->Write();

  mom_forward->Draw("E1");
  gPad->Modified();
  gPad->Update();
  mom_forward->Write();

  file_out->Close();
  file->Close();
}