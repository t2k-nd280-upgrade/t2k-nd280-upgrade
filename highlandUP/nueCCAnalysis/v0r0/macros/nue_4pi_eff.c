void nue_4pi_eff(){
  std::string mc_path   = "/t2k/users/suvorov/AnalysisResults/ndUP/nue/";
  std::string prefix    = "/t2k/users/suvorov/dev/t2k-nd280-upgrade/highlandUP/figures/";

  std::string conf;
  //conf = "Current/";
  conf = "Alternative_Target_TPC/";

  mc_path += conf;
  prefix  += conf;

  mc_path           += "v13/";
  std::string timing = "600/";

  if (conf != "Current/") {
    mc_path += timing;
    prefix  += timing;
  }

  std::string file_tmp = mc_path;

  // what to plot
  bool TargetToF = false;
  bool UseNumu   = true;
  bool RHC       = false;

  file_tmp += "All_";

  std::string det = "";
  // target 1
  //det = "truelepton_pos[2] < 0.";
  // FGD 1
  //det = "truelepton_pos[2] > 0. && truelepton_pos[2] < 1000";
  // FGD2
  //det = "truelepton_pos[2] > 1000.";

  if (RHC) {
   prefix   += "RHC/";
   file_tmp += "RHC_"; 
  }

  file_tmp += "ToF_";

  if (TargetToF) {
    if (conf != "Current/")
      prefix   += "TarToF/";
    file_tmp += "TarToF_";
  } else {
    if (conf != "Current/")
      prefix   += "noTarToF/";
    file_tmp += "noTarToF_";
  }

  Experiment* exp = new Experiment("nd280up");

  AddSamples(exp, UseNumu, RHC, file_tmp);  

  // 8 - all cuts, - before ToF
  std::string cut_base = "accum_level[][0] > 8";
  if (det != "")
    cut_base += " && " + det;
  std::string signal;
  signal = "nueCC < 3";
  //signal = "particle == 11 && nu_pdg == 12";
  std::string opt = "OVER";

  //dump(*exp, prefix, cut_base, opt, mc_path, signal);
  //SelectionKinem(*exp, prefix, cut_base, opt, mc_path, signal);
  //IsoTargetStudy(*exp, prefix, cut_base, opt, mc_path, signal, RHC, (conf == "Current/"), det);
  EffStudy(*exp, prefix, cut_base, opt, mc_path, signal, RHC, (conf == "Current/"), det);
  //PlotToF(*exp, prefix, cut_base, opt, mc_path, signal);
  //ScanCut(*exp, prefix, cut_base, opt, mc_path, signal, RHC, (conf == "Current/"));
  //TopoStudy(*exp, prefix, cut_base, opt, mc_path, signal, RHC, (conf == "Current/"), det);
}

// ********************************************************
void PlotToF(Experiment exp, std::string prefix, std::string cut_base, std::string opt, std::string mc_path, std::string signal) {
  // ********************************************************
  DrawingTools  draw((mc_path + "All_RHC_ToF_TarToF_nue.root").c_str(), 1);
  TCanvas c1("canva","",50,50,1000,800);

  //draw.SetMinY(3);
  //draw.SetLogY();

  // ToF
  std::string cut1 = cut_base + " && selelec_mom > 200 && selelec_likeel > 0.5";
  draw.SetTitleX("L_{ToF ele}");
  draw.Draw(exp, "selelec_ToF_lkl_electron",  50, 0., 1., "particle", cut1, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_ele.png").c_str()); 

  draw.SetTitleX("L_{ToF #mu}");
  draw.Draw(exp, "selelec_ToF_lkl_muon",  50, 0., 1., "particle", cut1, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_mu.png").c_str());

  draw.SetTitleX("L_{ToF p}");
  draw.Draw(exp, "selelec_ToF_lkl_proton",  50, 0., 1., "particle", cut1, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_proton.png").c_str());

  draw.Draw(exp, "selelec_ToF_lkl_proton",  50, 0., 1., "nueCC", cut1, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_proton_top.png").c_str());

  draw.SetTitleX("L_{ToF #pi}");
  draw.Draw(exp, "selelec_ToF_lkl_pion",  50, 0., 1., "particle", cut1, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_pion.png").c_str());

  draw.SetTitleX("ToF mass, MeV");
  draw.Draw(exp, "selelec_ToF_mass",  50, 0., 7000., "particle", cut1, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_mass.png").c_str());

  std::string cut_tmp; 

  draw.SetTitleX("First ToF det");
  draw.SetTitleY("Seconf ToF det");
  cut_tmp = cut1 + " && selelec_second_ToF < 21 && selelec_first_ToF < 21";
  c1->SetGrid(1);
  draw.Draw(exp, "selelec_second_ToF:selelec_first_ToF",  11, 7., 18., 11, 7., 18., "all", cut_tmp, "colz TEXT0", "");
  TH2D* histo(draw.GetLastStackTotal2D());
  histo->SetMarkerSize(1.7);
  histo->GetXaxis()->SetBinLabel(1,"Target1");
  histo->GetXaxis()->SetBinLabel(2,"Target2");
  histo->GetXaxis()->SetBinLabel(3,"FGD1");
  histo->GetXaxis()->SetBinLabel(4,"FGD2");
  histo->GetXaxis()->SetBinLabel(5,"P0D");
  histo->GetXaxis()->SetBinLabel(6,"DsECal");
  histo->GetXaxis()->SetBinLabel(7,"P0D ECal");
  histo->GetXaxis()->SetBinLabel(8,"BrECal");
  histo->GetXaxis()->SetBinLabel(9,"ToF");
  histo->GetXaxis()->SetBinLabel(10,"Invalid");
  histo->GetXaxis()->SetBinLabel(11,"Target ToF");
  histo->GetXaxis()->SetBinLabel(12,"Target");
  histo->GetXaxis()->SetBinLabel(13,"FGD");
  histo->GetXaxis()->SetBinLabel(14,"ECal");
  histo->GetXaxis()->SetBinLabel(15,"Invalid");

  histo->GetYaxis()->SetBinLabel(1,"Target1");
  histo->GetYaxis()->SetBinLabel(2,"Target2");
  histo->GetYaxis()->SetBinLabel(3,"FGD1");
  histo->GetYaxis()->SetBinLabel(4,"FGD2");
  histo->GetYaxis()->SetBinLabel(5,"P0D");
  histo->GetYaxis()->SetBinLabel(6,"DsECal");
  histo->GetYaxis()->SetBinLabel(7,"P0D ECal");
  histo->GetYaxis()->SetBinLabel(8,"BrECal");
  histo->GetYaxis()->SetBinLabel(9,"ToF");
  histo->GetYaxis()->SetBinLabel(10,"Invalid");
  histo->GetYaxis()->SetBinLabel(11,"Target ToF");
  histo->GetYaxis()->SetBinLabel(12,"Target");
  histo->GetYaxis()->SetBinLabel(13,"FGD");
  histo->GetYaxis()->SetBinLabel(14,"ECal");
  histo->GetYaxis()->SetBinLabel(15,"Invalid");
  
  histo->Draw("colz text0");
  c1.Print((prefix+"mc_ele_ToF_det.png").c_str());


  draw.SetFillStyle(1001); 
  draw.SetFillColor(kMagenta);
  //cut1 += " && selelec_ToF_mass != 0";
  draw.SetTitleX("ToF mass, MeV");
  draw.SetTitleY("Momentum, MeV");
  cut_tmp = cut1 + " && abs(particle) == 11";
  draw.Draw(exp, "selelec_mom:selelec_ToF_mass",  50, 0., 5000., 50, 0., 5000., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_mass_momentum.png").c_str());

  draw.SetFillColor(kGreen);
  cut_tmp = cut1 + " && abs(particle) == 2212";
  draw.Draw(exp, "selelec_mom:selelec_ToF_mass",  50, 0., 5000., 50, 0., 5000., "all", cut_tmp, "same", "OVER");
  c1.Print((prefix+"mc_ele_ToF_mass_momentum1.png").c_str());

  draw.SetFillColor(kMagenta);
  draw.SetTitleX("Ele Llh");
  draw.SetTitleY("Momentum, MeV");
  cut_tmp = cut1 + " && (abs(particle) == 11 || abs(particle) == 2212)";
  draw.Draw(exp, "selelec_mom:selelec_ToF_lkl_electron",  50, 0., 1., 50, 0., 5000., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_Mom_ele_lkl.png").c_str());

  draw.SetFillColor(kGreen);
  cut_tmp = cut1 + " && abs(particle) == 2212";
  draw.Draw(exp, "selelec_mom:selelec_ToF_lkl_electron",  50, 0., 1., 50, 0., 5000., "all", cut_tmp, "same", "OVER");
  c1.Print((prefix+"mc_ele_ToF_Mom_ele_lkl1.png").c_str());

  draw.SetFillColor(kMagenta);
  draw.SetTitleX("Proton Llh");
  draw.SetTitleY("Momentum, MeV");
  cut_tmp = cut1 + " && abs(particle) == 11";
  draw.Draw(exp, "selelec_mom:selelec_ToF_lkl_proton",  50, 0., 1., 50, 0., 5000., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_Mom_pr_lkl.png").c_str());

  draw.SetFillColor(kGreen);
  cut_tmp = cut1 + " && abs(particle) == 2212";
  draw.Draw(exp, "selelec_mom:selelec_ToF_lkl_proton",  50, 0., 1., 50, 0., 5000., "all", cut_tmp, "same", "OVER");
  c1.Print((prefix+"mc_ele_ToF_Mom_pr_lkl1.png").c_str());

  draw.SetFillColor(kMagenta);
  draw.SetTitleX("Ele TPC Llh");
  draw.SetTitleY("Momentum, MeV");
  cut_tmp = cut1 + " && abs(particle) == 11";
  draw.Draw(exp, "selelec_mom:selelec_likeel",  50, 0., 1., 50, 0., 5000., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_Mom_ele_tpc_lkl.png").c_str());

  draw.SetFillColor(kGreen);
  cut_tmp = cut1 + " && abs(particle) == 2212";
  draw.Draw(exp, "selelec_mom:selelec_likeel",  50, 0., 1., 50, 0., 5000., "all", cut_tmp, "same", "OVER");
  c1.Print((prefix+"mc_ele_ToF_Mom_ele_tpc_lkl1.png").c_str());

  draw.SetFillStyle(0);


  draw.SetOptStat(1001110);
  gStyle->SetOptStat(1001110);
  cut_tmp = cut1 + " && selelec_mom > 800 && selelec_mom < 1500";
  draw.SetTitleX("Momentum resolution");
  draw.Draw(exp, "(selelec_mom-selelec_truemom)/selelec_truemom",  50, -10, 10., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_mom_smear.png").c_str());

  cut_tmp = cut1 + " && selelec_mom > 800 && selelec_mom < 1500";
  draw.SetTitleX("ToF resolution");
  draw.Draw(exp, "(selelec_ToF-selelec_ToF_true)/selelec_ToF_true",  50, -10, 10., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_ToF_smear.png").c_str()); 

  draw.SetOptStat(0);
  gStyle->SetOptStat(0);


  std::string cut_tof = cut1;

  cut_tmp = cut1 + " && abs(particle) == 11";
  draw.SetTitleX("ToF mass, MeV");
  draw.Draw(exp, "selelec_ToF_mass",  50, 0., 10000., "particle", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_mass_before_PID_ele.png").c_str()); 

  cut_tmp = cut1 + " && " + signal;
  draw.SetTitleX("ToF mass, MeV");
  draw.Draw(exp, "selelec_ToF_mass",  50, 0., 10000., "particle", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_ToF_mass_before_PID_signal.png").c_str()); 

  draw.SetTitleX("L_{e, ToF}");
  draw.SetTitleY("L_{e, TPC}");

  draw.SetFillStyle(1001);

  draw.SetFillColor(kMagenta);
  cut_tmp = cut_tof + " && abs(particle) == 11";
  draw.Draw(exp, "selelec_likeel:selelec_ToF_lkl_electron",  50, 0., 1., 50, 0., 1., "all", cut_tmp, "box", "");
  c1.Print((prefix+"mc_ele_ToF_TPC_ele_ele.png").c_str()); 
  draw.SetFillColor(kGreen);
  cut_tmp = cut_tof + " && abs(particle) == 2212";
  draw.Draw(exp, "selelec_likeel:selelec_ToF_lkl_electron",  50, 0., 1., 50, 0., 1., "all", cut_tmp, "box", "");
  c1.Print((prefix+"mc_ele_ToF_TPC_ele_p.png").c_str()); 

  cut_tmp = cut_tof + " && abs(particle) == 2212 && selelec_likeel > 0.5";
  draw.Draw(exp, "selelec_likeel:selelec_ToF_lkl_electron",  50, 0., 1., 50, 0., 1., "all", cut_tmp, "box", "");
  c1.Print((prefix+"mc_ele_ToF_TPC_ele_p_0.5.png").c_str()); 

  draw.SetFillColor(kMagenta);
  cut_tmp = cut_tof + " && abs(particle) == 11";
  draw.Draw(exp, "selelec_likeel:selelec_ToF_lkl_electron",  50, 0., 1., 50, 0., 1., "all", cut_tmp, "box", "");
  draw.SetFillColor(kGreen);
  cut_tmp = cut_tof + " && abs(particle) == 2212";
  draw.Draw(exp, "selelec_likeel:selelec_ToF_lkl_electron",  50, 0., 1., 50, 0., 1., "all", cut_tmp, "box same", "");
  c1.Print((prefix+"mc_ele_ToF_TPC_ele_p_same.png").c_str()); 

  draw.SetTitleX("L_{p, ToF}");
  draw.SetTitleY("L_{p, TPC}");

  draw.SetFillColor(kMagenta);
  cut_tmp = cut_tof + " && abs(particle) == 11";
  draw.Draw(exp, "selelec_likepr:selelec_ToF_lkl_proton",  50, 0., 1., 50, 0., 1., "all", cut_tmp, "box", "");
  c1.Print((prefix+"mc_ele_ToF_TPC_proton_ele.png").c_str());
  draw.SetFillColor(kGreen);
  cut_tmp = cut_tof + " && abs(particle) == 2212";
  draw.Draw(exp, "selelec_likepr:selelec_ToF_lkl_proton",  50, 0., 1., 50, 0., 1., "all", cut_tmp, "box", "");
  c1.Print((prefix+"mc_ele_ToF_TPC_proton_proton.png").c_str());

}

// ********************************************************
void SelectionKinem(Experiment exp, std::string prefix, std::string cut_base, std::string opt, std::string mc_path, std::string signal) {
  // ********************************************************
  DrawingTools  draw((mc_path + "All_ToF_noTarToF_nue.root").c_str(), 1);
  TCanvas c1("canva","",50,50,1000,800);

  //const int NBins_Mom2 = 15;
  //double BinEdges_Mom2[NBins_Mom2 + 1] = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1400, 2000, 3000, 5000, 10000};

  const int NBins_Mom2 = 19;
  double BinEdges_Mom2[NBins_Mom2 + 1] = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1700, 1800, 2000, 5000};

  const int NBins_Mom1 = 14;
  double BinEdges_Mom1[NBins_Mom1 + 1] = {0, 250, 500, 750, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 5000, 6000, 8000, 12000};

  const int NBins_CosTh = 20;
  double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

  std::string cut1 = cut_base;// + " && selelec_mom > 200";

  draw.SetTitleX("P_{HMN}, MeV/c");
  std::string cut_tmp = cut1 +  " && truelepton_pos[2] > 1000. && selelec_dir[2] > 0";
  draw.Draw(exp, "selelec_mom",  50, 0., 5000., "nueCC", cut_tmp , "", "OVER");
  //draw.DrawCutLineVertical(200, true, "r");
  c1.Print((prefix+"mc_ele_mom_topology.png").c_str()); 

  draw.SetTitleX("P_{HMN, reco}, MeV/c");
  draw.Draw(exp, "selelec_mom",  50, 0., 5000., "particle", cut1, "", "OVER");
  c1.Print((prefix+"mc_ele_mom_particle.png").c_str()); 

  std::string cut_tmp = "accum_level[][0] > 2 && abs(particle) == 11";
  draw.SetTitleX("P_{HMN}, MeV/c");
  draw.Draw(exp, "selelec_mom",  50, 0., 5000., "particle", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_mom_before_PID_ele.png").c_str());

  std::string cut_tmp = "accum_level[][0] > 2 && abs(particle) == 11 && selelec_mom > 200";
  draw.SetTitleX("P_{HMN}, MeV/c");
  draw.Draw(exp, "selelec_mom",  50, 0., 5000., "particle", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_mom_before_PID_ele_200.png").c_str());

  cut_tmp = "accum_level[][0] > 2";
  draw.SetTitleX("P_{HMN}, MeV/c");
  draw.Draw(exp, "selelec_mom",  50, 0., 5000., "particle", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_mom_before_TPC_PID.png").c_str());

  cut_tmp = "accum_level[][0] > 3";
  draw.SetTitleX("P_{HMN}, MeV/c");
  draw.Draw(exp, "selelec_mom",  50, 0., 5000., "particle", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_mom_before_ECal_PID.png").c_str());

  cut_tmp = "accum_level[][0] > 4";
  draw.SetTitleX("P_{HMN}, MeV/c");
  draw.Draw(exp, "selelec_mom",  50, 0., 5000., "particle", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_mom_after_ECal_PID.png").c_str());

  cut_tmp = "accum_level[][0] > 5";
  draw.SetTitleX("P_{HMN}, MeV/c");
  draw.Draw(exp, "selelec_mom",  50, 0., 5000., "particle", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_mom_after_ECal_PID_PID2.png").c_str());

  draw.SetLegendPos("lt"); 

  draw.SetTitleX("cos(#theta_{HMN})");
  draw.Draw(exp, "selelec_costheta",  50, -1., 1., "nueCC", cut1, "", "OVER");
  c1.Print((prefix+"mc_ele_theta.png").c_str()); 

  draw.SetTitleX("cos(#theta_{HMN})");
  cut_tmp = cut1 + " && selelec_pos[2] < 0 && " + signal;
  draw.Draw(exp, "selelec_truepos[2]",  1000, -3000., 3500., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_Z.png").c_str()); 

  draw.SetTitleX("cos(#theta_{HMN})");
  cut_tmp = cut1 + " && selelec_pos[2] < 0";
  draw.Draw(exp, "selelec_truepos[2]",  1000, -3000., 3500., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_Z_TAR.png").c_str()); 

  cut_tmp = "accum_level[][0]>8";
  draw.SetTitleX("#beta_{reco}");
  draw.SetMinY(1.);
  draw.SetMaxY(700.);
  draw.SetLogY(1);
  draw.Draw(exp, "Beta_ToF",  50, 0., 1., "particle", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_beta_ToF.png").c_str()); 

  draw.SetTitleX("#beta_{true}");
  draw.Draw(exp, "Beta_true_ToF",  50, 0., 1., "particle", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_beta_ToF_true.png").c_str()); 

  cut_tmp = "accum_level[][0]>8 && particle == 2212";
  draw.SetTitleX("#beta_{reco}");
  draw.Draw(exp, "Beta_ToF",  50, -1., 5., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_beta_pr.png").c_str());

  draw.SetTitleX("#beta_{true}");
  draw.Draw(exp, "Beta_true_ToF",  50, -1., 5., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_beta_pr_true.png").c_str()); 

  draw.SetLogY(0);
  draw.SetMinY(0);

  draw.SetLegendPos("rt"); 

  cut_tmp = "1";
  draw.SetTitleX("Daughter PDG");
  draw.Draw(exp, "selelec_daughterPDG",  2000, -1., 2500., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_daughter.png").c_str());

  draw.SetTitleY("dE/dx");
  draw.SetTitleX("P_{e, true}, MeV/c");
  cut_tmp = cut1 + " && truelepton_pos[2]<200.";
  draw.Draw(exp, "((sqrt(selelec_truemom*selelec_truemom + 0.2611) - sqrt(true_target_end_mom*true_target_end_mom + 0.2611)) / true_target_length):selelec_truemom", 40, 0., 4000, 40, 0., 10., "all", cut_tmp, "colz", "OVER");
  c1.Print((prefix+"mc_ele_dEdx.png").c_str());

  exp.SetCurrentTree("truth");

  // energy loss in target
  draw.SetTitleY("P_{e, true}, MeV/c");
  //draw.SetMaxY(2500.);
  draw.Draw(exp, "truelepton_mom", 50, 0., 5000., "all", "1", "", "OVER");
  c1.Print((prefix+"mc_ele_mom_true.png").c_str());

  cut_tmp = cut_base;
  draw.SetTitleX("cos(#theta_{true})");
  draw.SetTitleY("P_{e, true}, MeV/c");
  draw.SetLogZ(1);
  draw.Draw(exp, "truelepton_mom:true_costheta", 20, -1., 1., 40, 0., 2000., "all", cut_tmp, "colz", "OVER");
  draw.DrawCutLineHorizontal(200, true, "U");
  c1.Print((prefix+"mc_ele_mom_theta_true.png").c_str());
  draw.SetLogZ(0);

  cut_tmp = "true_costheta > 0.8 && true_position[2] > 200.";
  draw.SetTitleX("Start-End distance, mm");
  draw.Draw(exp, "sqrt(pow(true_end[0]-true_position[0],2) + pow(true_end[1]-true_position[1],2) + pow(true_end[2]-true_position[2],2))",  50, 0., 6000., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_length_FGDs.png").c_str());

  std::string cut_tmp = "true_costheta > 0.8 && true_position[2] < 200.";
  draw.SetTitleX("Start-End distance, mm");
  draw.Draw(exp, "sqrt(pow(true_end[0]-true_position[0],2) + pow(true_end[1]-true_position[1],2) + pow(true_end[2]-true_position[2],2))",  50, 0., 6000., "all", cut_tmp, "", "OVER");
  c1.Print((prefix+"mc_ele_length_Tar.png").c_str());   

  //DataSample ds(mc_path + "All_ToF_noTarToF_nue.root");
  cut_tmp = "1";
  draw.SetTitleX("Z, mm");
  draw.SetTitleY("X, mm");
  draw.Draw(exp, "true_end[0]:true_end[2]", 150, -3000., 3500., 100, -2000., 2000., "all", cut_tmp, "colz", "OVER");
  c1.Print((prefix+"mc_ele_stopXZ.png").c_str());
  draw.SetTitleY("Y, mm");
  draw.Draw(exp, "true_end[1]:true_end[2]", 150, -3000., 3500., 100, -2000., 2000., "all", cut_tmp, "colz", "OVER");
  //draw.Draw(ds.GetTree("truth"), "true_end[2]", 100, -1500., 3500., "all", cut_tmp, "colz", "OVER");
  c1.Print((prefix+"mc_ele_stopYZ.png").c_str());
  exp.SetCurrentTree("default");

  
  cut_tmp = cut_base + " && selelec_stopped == 0 && abs(selelec_trueendpos[1]) < 1690.";
  draw.SetTitleX("Z, mm");
  draw.SetTitleY("X, mm");
  draw.Draw(exp, "selelec_trueendpos[0]:selelec_trueendpos[2]", 150, -3000., 3500., 100, -2000., 2000., "all", cut_tmp, "colz", "OVER");
  c1.Print((prefix+"mc_ele_stopXZ.png").c_str());
  draw.SetTitleY("Y, mm");
  cut_tmp = cut_base + " && selelec_stopped == 0 && abs(selelec_trueendpos[0]) < 1600.";
  draw.Draw(exp, "selelec_trueendpos[1]:selelec_trueendpos[2]", 150, -3000., 3500., 100, -2000., 2000., "all", cut_tmp, "colz", "OVER");
  c1.Print((prefix+"mc_ele_stopYZ.png").c_str());

  std::string target_end = "true_end[0] > -932. && true_end[0] < 932. && true_end[1] > -316. && true_end[1] < 316. && true_end[2] > -2642. && true_end[2] < -748.";
  cut_tmp = target_end + " && truelepton_pos[2] < 200. && true_stopped != 1";
  exp.SetCurrentTree("truth");
  draw.Draw(exp, "1", 4, 0., 4., "all", cut_tmp, "colz", "OVER");


/*
  // PID plotting
  cut1 = "accum_level[][0] > 3";
  draw.SetMinY(3);
  draw.SetLogY();
  draw.SetLegendPos("ct");

  draw.SetTitleX("L_{e}");
  draw.Draw(exp, "selelec_likeel",  50, 0., 1., "particle", cut1, "", "OVER");
  draw.DrawCutLineVertical(0.5, true, "r");
  c1.Print((prefix+"mc_ele_llh.png").c_str()); 

  draw.SetTitleX("L_{#pi}");
  draw.Draw(exp, "selelec_likepi",  50, 0., 1., "particle", cut1, "", "OVER");
  c1.Print((prefix+"mc_prot_llh.png").c_str());  

  draw.SetTitleX("L_{p}");
  draw.Draw(exp, "selelec_likepr",  50, 0., 1., "particle", cut1, "", "OVER");
  c1.Print((prefix+"mc_pion_llh.png").c_str());  

  draw.SetTitleX("L_{#mu}");
  draw.Draw(exp, "selelec_likemu",  50, 0., 1., "particle", cut1, "", "OVER");
  c1.Print((prefix+"mc_mu_llh.png").c_str());  
  

  draw.SetTitleX("L_{#mu}");
  cut1 = "accum_level[][0] > 4 ";
  draw.Draw(exp, "selelec_likemu",  50, 0., 1., "particle", cut1, "", "OVER");
  draw.DrawCutLineVertical(0.03, true, "l");
  c1.Print((prefix+"mc_ele_mu_llh_5.png").c_str()); 
  
  draw.SetLegendPos("rt");

  cut1 = "accum_level[][0] > 5";
  draw.SetTitleX("ECal MIP EM");  
  draw.Draw(exp, "selelec_ecal_mipem",  50, -40., 60., "particle", cut1, "", "OVER");
  draw.DrawCutLineVertical(0., true, "r");
  c1.Print((prefix+"mc_ele_MIPEM.png").c_str()); 

  draw.SetLogY(0);
  draw.SetMinY();  
*/
  //exp.SetCurrentTree("truth");
  c1.Clear();
  draw.SetTitleX("E_{#nu}, MeV");
  draw.Draw(exp, "true_Nu_mom",  200, 0., 10000., "all", "", "", "");
  c1.Print((prefix+"mc_ele_Nu_mom_truth.png").c_str());
  exp.SetCurrentTree("default");  

}

// ********************************************************
void EffStudy(Experiment exp, std::string prefix, std::string cut_base, std::string opt, std::string mc_path, std::string signal, bool RHC, bool Current, std::string det) {
  // ********************************************************
  bool use_file    = true;
  bool include_iso = false;

  DrawingTools  draw((mc_path + "All_ToF_noTarToF_nue.root").c_str(), 1);
  TCanvas c1("canva","",50,50,1000,800);

  //const int NBins_Mom2 = 8;
  //double BinEdges_Mom2[NBins_Mom2 + 1] = {0, 300, 600, 900, 1200, 1600, 2000, 5000, 10000};

  const int NBins_Mom2 = 1;
  double BinEdges_Mom2[NBins_Mom2 + 1] = {0, 1e6};

  const int NBins_Mom1 = 17;
  double BinEdges_Mom1[NBins_Mom1 + 1]   = {0, 200, 300., 400, 500., 600, 700., 800, 900., 1000, 1100., 1200, 1400, 1600, 1800, 2000, 2500, 3000};

  /*const int NBins_CosTh = 20;
  double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};*/

  const int NBins_CosTh = 12;
  double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.7,-0.4,-0.2,0.0,0.2,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

  std::string cut1 = "accum_level[0]>8";
  std::string cutE = "accum_level[0]>8";// + " && truelepton_mom > 200 ";
  std::string cutP = "accum_level[][0]>8";// + " && selelec_mom > 200 ";

  std::string cut_eff_before = signal;
  std::string cut_eff_after  = cutE + " && " + signal;

  std::string cut_pur_before = cutP;
  std::string cut_pur_after  = cutP + " && " + signal;

  /*draw.DrawEffPurVSCut(exp, signal, "", -1, -1, "");
  c1.Print((prefix+"EffvsCuts.png").c_str());
  draw.DrawEventsVSCut(exp);
  draw.DrawEffVSCut(exp, signal, det, -1, -1, "");*/

  TFile* file;
  if (use_file) {
    std::cout << "1" << std::endl;
    if (Current) {
      file = new TFile("/t2k/users/suvorov/data/ndUpCurrentEff.root", "RECREATE");
    } else {
      file = new TFile("/t2k/users/suvorov/data/ndUpCurrentEff.root", "READ");
    }
    file->cd();
  }


  exp.SetCurrentTree("truth");
  std::string cut_eff_after_FGD  = cut_eff_after + " && truelepton_pos[2] > 0.";
  std::string cut_eff_before_FGD = cut_eff_before + " && truelepton_pos[2] > 0.";
  std::string cut_eff_after_TAR  = cut_eff_after + " && truelepton_pos[2] < 0.";
  std::string cut_eff_before_TAR = cut_eff_before + " && truelepton_pos[2] < 0.";


  std::string cut_eff_after_FGD1  = cut_eff_after + " && truelepton_pos[2] > 0. && truelepton_pos[2] < 1000";
  std::string cut_eff_before_FGD1 = cut_eff_before + " && truelepton_pos[2] > 0. && truelepton_pos[2] < 1000";
  std::string cut_eff_after_FGD2  = cut_eff_after + " && truelepton_pos[2] > 1000.";
  std::string cut_eff_before_FGD2 = cut_eff_before + " && truelepton_pos[2] > 1000.";

  std::string cut_pur_after_FGD  = cut_pur_after + " && selelec_pos[2] > 0.";
  std::string cut_pur_before_FGD = cut_pur_before + " && selelec_pos[2] > 0.";
  std::string cut_pur_after_TAR  = cut_pur_after + " && selelec_pos[2] < 0.";
  std::string cut_pur_before_TAR = cut_pur_before + " && selelec_pos[2] < 0.";

  std::string cut_pur_after_FGD1  = cut_pur_after + " && selelec_pos[2] > 0. && selelec_pos[2] < 1000";
  std::string cut_pur_before_FGD1 = cut_pur_before + " && selelec_pos[2] > 0. && selelec_pos[2] < 1000";
  std::string cut_pur_after_FGD2  = cut_pur_after + " && selelec_pos[2] > 1000.";
  std::string cut_pur_before_FGD2 = cut_pur_before + " && selelec_pos[2] > 1000.";

  if (include_iso) {
    cut_eff_after_TAR.erase(0,16);
    cut_eff_after_TAR = "(accum_level[0] > 8 || accum_level[1] > 1)" + cut_eff_after_TAR;

    cut_pur_after_TAR.erase(0,18);
    cut_pur_before_TAR.erase(0,18);
    cut_pur_after_TAR = "(accum_level[][0] > 8 || accum_level[][1] > 1)" + cut_pur_after_TAR;
    cut_pur_before_TAR = "(accum_level[][0] > 8 || accum_level[][1] > 1)" + cut_pur_before_TAR;
  }

  std::cout << cut_eff_after_TAR << std::endl;
  std::cout << cut_pur_after_TAR << std::endl;
  std::cout << cut_pur_before_TAR << std::endl;

  draw.SetTitleX("cos(#theta)");  
  draw.DrawPur(exp, "true_costheta",  NBins_CosTh, BinEdges_CosTh, cut_eff_after_FGD, cut_eff_before_FGD, "", "OVER");
  draw.DrawPur(exp, "true_costheta",  NBins_CosTh, BinEdges_CosTh, cut_eff_after_TAR, cut_eff_before_TAR, "same", "OVER");
  c1.Print((prefix+"mc_ele_theta_eff_true_FGD_TAR.png").c_str());

  exp.SetCurrentTree("truth");

  draw.SetTitleX("cos(#theta)");  
  draw.DrawPur(exp, "true_costheta",  NBins_CosTh, BinEdges_CosTh, cut_eff_after, cut_eff_before, "", "OVER");
  c1.Print((prefix+"mc_ele_theta_eff_true.png").c_str()); 

  if (use_file) {
    if (Current) {
      std::string name = "";
      if (RHC)
        name += "effRHCCurrent_theta";
      else
        name += "effCurrent_theta";
      draw.DrawPur(exp, "true_costheta",  NBins_CosTh, BinEdges_CosTh, cut_eff_after_FGD1, cut_eff_before_FGD1, "", "OVER");
      TGraphAsymmErrors* gr(draw.GetLastGraph());

      gr->SetName((name + "FGD1").c_str());
      gr->SetLineColor(kBlack);
      gr->SetMarkerColor(kBlack);

      draw.DrawPur(exp, "true_costheta",  NBins_CosTh, BinEdges_CosTh, cut_eff_after_FGD2, cut_eff_before_FGD2, "", "OVER");
      TGraphAsymmErrors* gr1(draw.GetLastGraph());

      gr1->SetName((name + "FGD2").c_str());
      gr1->SetLineColor(kRed);
      gr1->SetMarkerColor(kRed);
      gr->SetLineStyle(2);
      gr1->SetLineStyle(2);

      gr->Write();
      gr1->Write();
    } else {
      TGraphAsymmErrors* gr;
      TGraphAsymmErrors* gr1;
      if (RHC) {
        gr = (TGraphAsymmErrors*)file->Get("effRHCCurrent_thetaFGD1");
        gr1 = (TGraphAsymmErrors*)file->Get("effRHCCurrent_thetaFGD2");
      } else { 
        gr = (TGraphAsymmErrors*)file->Get("effCurrent_thetaFGD1");
        gr1 = (TGraphAsymmErrors*)file->Get("effCurrent_thetaFGD2");
      }
    }
  }
      
  draw.DrawPur(exp, "true_costheta",  NBins_CosTh, BinEdges_CosTh, cut_eff_after_TAR, cut_eff_before_TAR, "", "OVER");
  TGraphAsymmErrors* grTAR(draw.GetLastGraph());
    
  draw.DrawPur(exp, "true_costheta",  NBins_CosTh, BinEdges_CosTh, cut_eff_after_FGD2, cut_eff_before_FGD2, "same", "OVER");
  TGraphAsymmErrors* grFGD2(draw.GetLastGraph());
  draw.DrawPur(exp, "true_costheta",  NBins_CosTh, BinEdges_CosTh, cut_eff_after_FGD1, cut_eff_before_FGD1, "same", "OVER");
  TGraphAsymmErrors* grFGD1(draw.GetLastGraph());
    
  grTAR->SetLineColor(4);
  grTAR->SetLineWidth(3);
  grTAR->SetMarkerColor(4);
  grTAR->SetMarkerStyle(1);
  grTAR->GetYaxis()->SetTitle("Efficiency");
  grTAR->Draw("ap Z");
  grFGD1->SetLineColor(kBlack);
  grFGD1->SetLineWidth(3);
  grFGD2->SetLineWidth(3);
  grFGD1->SetMarkerColor(kBlack);
  grFGD2->SetLineColor(kRed);
  grFGD2->SetMarkerColor(kRed);
  grFGD1->SetLineStyle(1);
  grFGD2->SetLineStyle(1);
  grFGD1->SetMarkerStyle(1);
  grFGD2->SetMarkerStyle(1);
  grFGD1->Draw("same p Z");
  grFGD2->Draw("same p Z");

  if (!Current && use_file) {   

    gr->SetMarkerStyle(1);
    gr->SetLineWidth(3);
    
    gr1->SetLineWidth(3);
    gr1->SetMarkerStyle(1);
    gr->SetLineStyle(2);
    gr1->SetLineStyle(2);
    gr->Draw("same p Z");
    gr1->Draw("same p Z");

    TLegend* leg1 = new TLegend(c1.GetLeftMargin(),   1 - c1.GetTopMargin() - 0.15,
      1 - c1.GetRightMargin(), 1. - c1.GetTopMargin());

    leg1->SetNColumns(3);
    leg1->AddEntry(grFGD1, "upgrade, FGD1", "lp");
    leg1->AddEntry(grFGD2, "upgrade, FGD2", "lp");
    leg1->AddEntry(grTAR, "upgrade, Horiz. Target", "lp");
    leg1->AddEntry(gr, "current, FGD1", "lp");
    leg1->AddEntry(gr1, "current, FGD2", "lp");
    leg1->SetFillColor(kWhite);

    leg1->Draw();
    
    
  }

  c1.Print((prefix+"mc_ele_theta_eff_true_to_current.png").c_str());  
   
  draw.SetTitleX("P_{e, true}, MeV/c");
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom1, BinEdges_Mom1, cut_eff_after, cut_eff_before, "", "OVER");
  exp.SetCurrentTree("default");
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom1, BinEdges_Mom1, cut_pur_after, cut_pur_before, "same", "OVER");
  c1.Print((prefix+"mc_ele_mom_eff_true.png").c_str());  

  exp.SetCurrentTree("truth");

  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom1, BinEdges_Mom1, cut_eff_after_FGD, cut_eff_before_FGD, "", "OVER");
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom1, BinEdges_Mom1, cut_eff_after_TAR, cut_eff_before_TAR, "same", "OVER");
  c1.Print((prefix+"mc_ele_mom_eff_true_FGD_TAR.png").c_str());

  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom1, BinEdges_Mom1, cut_eff_after, cut_eff_before, "", "OVER");
  if (use_file) {
    if (Current) {
      std::string name = "";
      if (RHC)
        name += "effRHCCurrent";
      else
        name += "effCurrent";
      draw.DrawPur(exp, "truelepton_mom",  NBins_Mom1, BinEdges_Mom1, cut_eff_after_FGD1, cut_eff_before_FGD1, "", "OVER");
      TGraphAsymmErrors* gr_1(draw.GetLastGraph());

      gr_1->SetName((name + "FGD1").c_str());
      gr_1->SetLineColor(kBlack);
      gr_1->SetMarkerColor(kBlack);

      draw.DrawPur(exp, "truelepton_mom",  NBins_Mom1, BinEdges_Mom1, cut_eff_after_FGD2, cut_eff_before_FGD2, "", "OVER");
      TGraphAsymmErrors* gr1_1(draw.GetLastGraph());

      gr1_1->SetName((name + "FGD2").c_str());
      gr1_1->SetLineColor(kRed);
      gr1_1->SetMarkerColor(kRed);
      gr1_1->SetLineStyle(2);
      gr_1->SetLineStyle(2);

      gr_1->Write();
      gr1_1->Write();
    } else {
      TGraphAsymmErrors* gr_1;
      TGraphAsymmErrors* gr1_1;
      if (RHC) {
        gr_1 = (TGraphAsymmErrors*)file->Get("effRHCCurrentFGD1");
        gr1_1 = (TGraphAsymmErrors*)file->Get("effRHCCurrentFGD2");
      } else { 
        gr_1 = (TGraphAsymmErrors*)file->Get("effCurrentFGD1");
        gr1_1 = (TGraphAsymmErrors*)file->Get("effCurrentFGD2");
      }
    }
  }

  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom1, BinEdges_Mom1, cut_eff_after_TAR, cut_eff_before_TAR, "", "OVER");
  TGraphAsymmErrors* grTAR_1(draw.GetLastGraph());
      
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom1, BinEdges_Mom1, cut_eff_after_FGD2, cut_eff_before_FGD2, "same", "OVER");
  TGraphAsymmErrors* grFGD2_1(draw.GetLastGraph());

  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom1, BinEdges_Mom1, cut_eff_after_FGD1, cut_eff_before_FGD1, "same", "OVER");
  TGraphAsymmErrors* grFGD1_1(draw.GetLastGraph());

  grTAR_1->SetLineColor(4);
  grTAR_1->SetLineWidth(3);
  grTAR_1->SetMarkerColor(4);
  grTAR_1->SetMarkerStyle(1);
  grTAR_1->GetYaxis()->SetTitle("Efficiency");
  grTAR_1->Draw("ap Z");
  grFGD1_1->SetLineColor(kBlack);
  grFGD1_1->SetLineWidth(3);
  grFGD2_1->SetLineWidth(3);
  grFGD1_1->SetMarkerColor(kBlack);
  grFGD2_1->SetLineColor(kRed);
  grFGD2_1->SetMarkerColor(kRed);
  grFGD1_1->SetLineStyle(1);
  grFGD2_1->SetLineStyle(1);
  grFGD1_1->SetMarkerStyle(1);
  grFGD2_1->SetMarkerStyle(1);

  grFGD1_1->SetMarkerStyle(1);
  grFGD2_1->SetMarkerStyle(1);

  grFGD1_1->Draw("same p Z");
  grFGD2_1->Draw("same p Z"); 

  if (!Current && use_file) {     

    gr_1->SetMarkerStyle(1);
    gr_1->SetLineWidth(3);
    gr_1->SetLineStyle(2);
    gr1_1->SetLineStyle(2);
    gr1_1->SetLineWidth(3);
    gr1_1->SetMarkerStyle(1);
    gr_1->Draw("same p Z");
    gr1_1->Draw("same p Z");  

    TLegend* leg = new TLegend(c1.GetLeftMargin(),   1 - c1.GetTopMargin() - 0.15,
      1 - c1.GetRightMargin(), 1. - c1.GetTopMargin());

    leg->SetNColumns(3);
    leg->AddEntry(grFGD1_1, "upgrade, FGD1", "lp");
    leg->AddEntry(grFGD2_1, "upgrade, FGD2", "lp");
    leg->AddEntry(grTAR_1, "upgrade, Horiz. Target", "lp");
    leg->AddEntry(gr_1, "current, FGD1", "lp");
    leg->AddEntry(gr1_1, "current, FGD2", "lp");

    leg->SetFillColor(kWhite);

    leg->Draw();
  
    file->Close();
  }

  c1.Print((prefix+"mc_ele_mom_eff_true_to_current.png").c_str());
  

  draw.SetTitleX("E_{#nu}, MeV");
  draw.DrawPur(exp, "true_Nu_mom",  NBins_Mom1, BinEdges_Mom1, cut_eff_after, cut_eff_before, "", "OVER");
  c1.Print((prefix+"mc_nu_mom_eff_true.png").c_str()); 

  exp.SetCurrentTree("default");

  draw.SetTitleX("P_{HMN}, MeV/c");

  draw.DrawPur(exp, "selelec_mom",  NBins_Mom1, BinEdges_Mom1, cut_pur_after_TAR, cut_pur_before_TAR, "", "OVER");
  TGraphAsymmErrors* grTARpur(draw.GetLastGraph());
  draw.DrawPur(exp, "selelec_mom",  NBins_Mom1, BinEdges_Mom1, cut_pur_after_FGD1, cut_pur_before_FGD1, "same", "OVER");
  TGraphAsymmErrors* grFGD1pur(draw.GetLastGraph());
  draw.DrawPur(exp, "selelec_mom",  NBins_Mom1, BinEdges_Mom1, cut_pur_after_FGD2, cut_pur_before_FGD2, "same", "OVER");
  TGraphAsymmErrors* grFGD2pur(draw.GetLastGraph());
  grTARpur->SetLineColor(4);
  grTARpur->SetLineWidth(3);
  grTARpur->SetMarkerColor(4);
  grTARpur->SetMarkerStyle(1);
  grTARpur->Draw("ap Z");
  grFGD1pur->SetLineColor(kBlack);
  grFGD1pur->SetLineWidth(3);
  grFGD2pur->SetLineWidth(3);
  grFGD1pur->SetMarkerColor(kBlack);
  grFGD2pur->SetLineColor(kRed);
  grFGD2pur->SetMarkerColor(kRed);
  grFGD1pur->SetLineStyle(1);
  grFGD2pur->SetLineStyle(1);
  grFGD1pur->SetMarkerStyle(1);
  grFGD2pur->SetMarkerStyle(1);
  grFGD1pur->Draw("same p Z");
  grFGD2pur->Draw("same p Z");
  c1.Print((prefix+"mc_ele_mom_pur.png").c_str());

  draw.SetTitleX("cos(#theta_{HMN})");
  draw.DrawPur(exp, "selelec_costheta",  NBins_CosTh, BinEdges_CosTh, cut_pur_after_TAR, cut_pur_before_TAR, "", "OVER");
  TGraphAsymmErrors* grTARpur1(draw.GetLastGraph());
  draw.DrawPur(exp, "selelec_costheta",  NBins_CosTh, BinEdges_CosTh, cut_pur_after_FGD1, cut_pur_before_FGD1, "same", "OVER");
  TGraphAsymmErrors* grFGD1pur1(draw.GetLastGraph());
  draw.DrawPur(exp, "selelec_costheta",  NBins_CosTh, BinEdges_CosTh, cut_pur_after_FGD2, cut_pur_before_FGD2, "same", "OVER");
  TGraphAsymmErrors* grFGD2pur1(draw.GetLastGraph());
  grTARpur1->SetLineColor(4);
  grTARpur1->SetLineWidth(3);
  grTARpur1->SetMarkerColor(4);
  grTARpur1->SetMarkerStyle(1);
  grTARpur1->Draw("ap Z");
  grFGD1pur1->SetLineColor(kBlack);
  grFGD1pur1->SetLineWidth(3);
  grFGD2pur1->SetLineWidth(3);
  grFGD1pur1->SetMarkerColor(kBlack);
  grFGD2pur1->SetLineColor(kRed);
  grFGD2pur1->SetMarkerColor(kRed);
  grFGD1pur1->SetLineStyle(1);
  grFGD2pur1->SetLineStyle(1);
  grFGD1pur1->SetMarkerStyle(1);
  grFGD2pur1->SetMarkerStyle(1);
  //grFGD1pur1->Draw("same p Z");
  //grFGD2pur1->Draw("same p Z");
  c1.Print((prefix+"mc_ele_theta_pur.png").c_str());    

  std::cout << "------- Target Eff Pur --------" << std::endl;
  exp.SetCurrentTree("truth");
  draw.DrawPur(exp, "truelepton_mom", NBins_Mom2, BinEdges_Mom2, cut_eff_after_TAR, cut_eff_before_TAR, "", "OVER");
  Float_t Eff_TAR = draw.GetLastGraph()->GetY()[0];
  std::cout << Eff_TAR << "       ";
  exp.SetCurrentTree("default");
  draw.DrawPur(exp, "selelec_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_TAR, cut_pur_before_TAR, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;
  std::cout << "------- FGD1 Eff Pur --------" << std::endl;
  exp.SetCurrentTree("truth");
  draw.DrawPur(exp, "truelepton_mom", NBins_Mom2, BinEdges_Mom2, cut_eff_after_FGD1, cut_eff_before_FGD1, "", "OVER");
  Float_t Eff_FGD1 = draw.GetLastGraph()->GetY()[0];
  std::cout << Eff_FGD1 << "       ";
  exp.SetCurrentTree("default");
  draw.DrawPur(exp, "selelec_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_FGD1, cut_pur_before_FGD1, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;
  std::cout << "------- FGD2 Eff Pur --------" << std::endl;
  exp.SetCurrentTree("truth");
  draw.DrawPur(exp, "truelepton_mom", NBins_Mom2, BinEdges_Mom2, cut_eff_after_FGD2, cut_eff_before_FGD2, "", "OVER");
  Float_t Eff_FGD2 = draw.GetLastGraph()->GetY()[0];
  std::cout << Eff_FGD2 << "       ";
  exp.SetCurrentTree("default");
  draw.DrawPur(exp, "selelec_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_FGD2, cut_pur_before_FGD2, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;
  std::cout << "------- TOTAL Eff Pur --------" << std::endl;
  exp.SetCurrentTree("truth");
  draw.DrawPur(exp, "truelepton_mom", NBins_Mom2, BinEdges_Mom2, cut_eff_after, cut_eff_before, "", "OVER");
  Float_t Eff_ALL = draw.GetLastGraph()->GetY()[0];
  std::cout << Eff_ALL << "       ";
  exp.SetCurrentTree("default");
  draw.DrawPur(exp, "selelec_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after, cut_pur_before, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;

  exp.SetCurrentTree("default");

  std::string signalP = "abs(selelec_PDG) == 11";
  std::string signalE = "abs(true_pdg) == 11";
  
  cut_pur_before  = "accum_level[][0]>3";
  cut_pur_after   = cut_pur_before + " && " + signalP;

  cut_eff_before  = "accum_level[0]>2 && " + signalE;
  cut_eff_after   = "accum_level[0]>3 && " + signalE;
  
  exp.SetCurrentTree("truth");
  //draw.DrawPur(exp, "truelepton_mom", NBins_Mom2, BinEdges_Mom2, cut_eff_after, cut_eff_before, "", "OVER");
  exp.SetCurrentTree("default");
  Eff_FGD1 = draw.GetLastGraph()->GetY()[0];
  //std::cout << Eff_FGD1 << "       ";
  draw.DrawPur(exp, "selelec_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after, cut_pur_before, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;

  cut_pur_before  = "accum_level[][0]>4";
  cut_pur_after   = cut_pur_before + " && " + signalP;

  cut_eff_before  = "accum_level[0]>2 && " + signalE;
  cut_eff_after   = "accum_level[0]>4 && " + signalE;
  
  exp.SetCurrentTree("truth");
  //draw.DrawPur(exp, "truelepton_mom", NBins_Mom2, BinEdges_Mom2, cut_eff_after, cut_eff_before, "", "OVER");
  exp.SetCurrentTree("default");
  //std::cout <<" TPC dE/dx + ECal    "  << draw.GetLastGraph()->GetY()[0] << "          ";
  draw.DrawPur(exp, "selelec_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after, cut_pur_before, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;

 cut_pur_before  = "accum_level[][0]>5";
  cut_pur_after   = cut_pur_before + " && " + signalP;

  cut_eff_before  = "accum_level[0]>2 && " + signalE;
  cut_eff_after   = "accum_level[0]>5 && " + signalE;
  
  exp.SetCurrentTree("truth");
  //draw.DrawPur(exp, "truelepton_mom", NBins_Mom2, BinEdges_Mom2, cut_eff_after, cut_eff_before, "", "OVER");
  exp.SetCurrentTree("default");
  //std::cout <<" TPC dE/dx + ECal + TPC2    "  << draw.GetLastGraph()->GetY()[0] << "          ";
  draw.DrawPur(exp, "selelec_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after, cut_pur_before, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;

}

// ********************************************************
void IsoTargetStudy(Experiment exp, std::string prefix, std::string cut_base, std::string opt, std::string mc_path, std::string signal, bool RHC, bool Current, std::string det) {
  // ********************************************************
  DrawingTools  draw((mc_path + "All_ToF_noTarToF_nue.root").c_str(), 1);
  TCanvas c1("canva","",50,50,1000,800);

  const int NBins_Mom1 = 17;
  double BinEdges_Mom1[NBins_Mom1 + 1]   = {0, 200, 300., 400, 500., 600, 700., 800, 900., 1000, 1100., 1200, 1400, 1600, 1800, 2000, 2500, 3000};
  const int NBins_CosTh = 12;
  double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.7,-0.4,-0.2,0.0,0.2,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

  signal = "nueCC < 3 && selelec_true_isoTar == 1";
  
  std::string cut_eff_after_TAR = "(accum_level[0] < 2 && accum_level[1] > 1) && " + signal;
  std::string cut_eff_before_TAR = signal;

  std::string cut_pur_after_TAR = "(accum_level[][0] < 2 && accum_level[][1] > 1) && " + signal;
  std::string cut_pur_before_TAR = "(accum_level[][0] < 2 && accum_level[][1] > 1)";

  draw.SetTitleX("cos(#theta)");  
  exp.SetCurrentTree("truth");
  draw.DrawPur(exp, "truelepton_dir[2]",  NBins_CosTh, BinEdges_CosTh, cut_eff_after_TAR, cut_eff_before_TAR, "", "OVER");
  c1.Print((prefix+"mc_ele_theta_eff_iso.png").c_str());

   draw.SetTitleX("cos(#theta)");  
  exp.SetCurrentTree("default");
  draw.DrawPur(exp, "truelepton_dir[2]",  NBins_CosTh, BinEdges_CosTh, cut_pur_after_TAR, cut_pur_before_TAR, "", "OVER");
  c1.Print((prefix+"mc_ele_theta_pur_iso.png").c_str());

  draw.SetTitleX("Length");
  draw.Draw(exp, "true_target_ele_length",  50, 0., 2000., "all", cut_pur_before_TAR, "", "OVER");
  draw.Draw(exp, "true_target_pr_length",  50, 0., 2000., "all", cut_pur_before_TAR, "same", "OVER");
  draw.Draw(exp, "true_target_mu_length",  50, 0., 2000., "all", cut_pur_before_TAR, "same", "OVER");
  draw.Draw(exp, "true_target_pi_length",  50, 0., 2000., "all", cut_pur_before_TAR, "same", "OVER");
  c1.Print((prefix+"mc_ele_iso_length.png").c_str()); 

  draw.SetTitleX("P, MeV/c");
  draw.Draw(exp, "selelec_mom",  50, 0., 1000., "nueCC", cut_pur_before_TAR, "", "OVER");
  c1.Print((prefix+"mc_ele_mom_iso.png").c_str());

  draw.SetTitleX("P, MeV/c");
  draw.Draw(exp, "selelec_mom",  50, 0., 1000., "particle", cut_pur_before_TAR, "", "OVER");
  c1.Print((prefix+"mc_ele_mom_iso_particle.png").c_str());

  draw.SetTitleX("P, MeV/c");
  //exp.SetCurrentTree("truth");
  draw.Draw(exp, "selelec_mom",  50, 0., 1000., "reactionnofv", cut_pur_before_TAR, "", "UNDER OVER");
  c1.Print((prefix+"mc_ele_mom_iso_reaction.png").c_str());

  
}

// ********************************************************
void TopoStudy(Experiment exp, std::string prefix, std::string cut_base, std::string opt, std::string mc_path, std::string signal, bool RHC, bool Current, std::string det) {
  // ********************************************************
  DrawingTools  draw((mc_path + "All_ToF_noTarToF_nue.root").c_str(), 1);
  TCanvas c1("canva","",50,50,1000,800);

  const int NBins_Mom2 = 1;
  double BinEdges_Mom2[NBins_Mom2 + 1] = {0, 1e6};

  const int NBins_Mom1 = 17;
  double BinEdges_Mom1[NBins_Mom1 + 1]   = {0, 200, 300., 400, 500., 600, 700., 800, 1000, 1200, 1400, 1600, 1800, 2000, 2500, 3000, 4000, 5000};

  std::string inc     = "accum_level[0] > 8";

  std::string Opi_cut_true = "nueCC == 0";
  std::string Ipi_cut_true = "nueCC == 1";
  std::string Npi_cut_true = "nueCC == 2";

  std::string Opi_cut = "accum_level[][0] > 9";
  std::string Ipi_cut = "accum_level[][1] > 9";
  std::string Npi_cut = "accum_level[][2] > 9";

  std::string TAR     = "truelepton_pos[2] < 0.";
  std::string FGD1    = "truelepton_pos[2] > 0. && truelepton_pos[2] < 1000";
  std::string FGD2    = "truelepton_pos[2] > 1000.";

  std::string cut_pur_after_TAR_Opi   = Opi_cut + " && " + Opi_cut_true + " && " + TAR;
  std::string cut_pur_before_TAR_Opi  = Opi_cut + " && " + TAR;

  std::string cut_pur_after_FGD1_Opi   = Opi_cut + " && " + Opi_cut_true + " && " + FGD1;
  std::string cut_pur_before_FGD1_Opi  = Opi_cut + " && " + FGD1;

  std::string cut_pur_after_FGD2_Opi   = Opi_cut + " && " + Opi_cut_true + " && " + FGD2;
  std::string cut_pur_before_FGD2_Opi  = Opi_cut + " && " + FGD2;

  std::string cut_pur_after_TAR_Ipi   = Ipi_cut + " && " + Ipi_cut_true + " && " + TAR;
  std::string cut_pur_before_TAR_Ipi  = Ipi_cut + " && " + TAR;

  std::string cut_pur_after_FGD1_Ipi   = Ipi_cut + " && " + Ipi_cut_true + " && " + FGD1;
  std::string cut_pur_before_FGD1_Ipi  = Ipi_cut + " && " + FGD1;

  std::string cut_pur_after_FGD2_Ipi   = Ipi_cut + " && " + Ipi_cut_true + " && " + FGD2;
  std::string cut_pur_before_FGD2_Ipi  = Ipi_cut + " && " + FGD2;

  std::string cut_pur_after_TAR_Npi   = Npi_cut + " && " + Npi_cut_true + " && " + TAR;
  std::string cut_pur_before_TAR_Npi  = Npi_cut + " && " + TAR;

  std::string cut_pur_after_FGD1_Npi   = Npi_cut + " && " + Npi_cut_true + " && " + FGD1;
  std::string cut_pur_before_FGD1_Npi  = Npi_cut + " && " + FGD1;

  std::string cut_pur_after_FGD2_Npi   = Npi_cut + " && " + Npi_cut_true + " && " + FGD2;
  std::string cut_pur_before_FGD2_Npi  = Npi_cut + " && " + FGD2;

  std::string cut_pur_after_Opi   = Opi_cut + " && " + Opi_cut_true ;
  std::string cut_pur_before_Opi  = Opi_cut;

  std::string cut_pur_after_Ipi   = Ipi_cut + " && " + Ipi_cut_true ;
  std::string cut_pur_before_Ipi  = Ipi_cut;

  std::string cut_pur_after_Npi   = Npi_cut + " && " + Npi_cut_true ;
  std::string cut_pur_before_Npi  = Npi_cut;

  exp.SetCurrentTree("default");

  std::string cut1 = cut_base + " && selelec_mom > 200";
  draw.SetTitleX("P, MeV/c");
  std::string cut_TAR = "accum_level[][0] > 9  && selelec_mom > 200 && selelec_pos[2] < 0.";
  draw.Draw(exp, "selelec_mom", 50, 0., 5000., "nueCC", cut_TAR, "", "OVER");
  c1.Print((prefix + "mc_ele_top_CC0pi_TAR.png").c_str());

  cut_TAR = "accum_level[][1] > 9  && selelec_mom > 200 && selelec_pos[2] < 0.";
  draw.Draw(exp, "selelec_mom", 50, 0., 5000., "nueCC", cut_TAR, "", "OVER");
  c1.Print((prefix + "mc_ele_top_CC1pi_TAR.png").c_str());

  cut_TAR = "accum_level[][2] > 9  && selelec_mom > 200 && selelec_pos[2] < 0.";
  draw.Draw(exp, "selelec_mom", 50, 0., 5000., "nueCC", cut_TAR, "", "OVER");
  c1.Print((prefix + "mc_ele_top_CCOther_TAR.png").c_str());

  std::cout << "------- Target Eff Pur Opi --------" << std::endl;
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_TAR_Opi, cut_pur_before_TAR_Opi, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;
  std::cout << "------- FGD1 Eff Pur Opi --------" << std::endl;
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_FGD1_Opi, cut_pur_before_FGD1_Opi, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;
  std::cout << "------- FGD2 Eff Pur Opi --------" << std::endl;
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_FGD2_Opi, cut_pur_before_FGD2_Opi, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;
  std::cout << "------- TOTAL Eff Pur Opi --------" << std::endl;
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_Opi, cut_pur_before_Opi, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;

  std::cout << "------- Target Eff Pur Ipi --------" << std::endl;
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_TAR_Ipi, cut_pur_before_TAR_Ipi, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;
  std::cout << "------- FGD1 Eff Pur Ipi --------" << std::endl;
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_FGD1_Ipi, cut_pur_before_FGD1_Ipi, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;
  std::cout << "------- FGD2 Eff Pur Ipi --------" << std::endl;
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_FGD2_Ipi, cut_pur_before_FGD2_Ipi, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;
  std::cout << "------- TOTAL Eff Pur Ipi --------" << std::endl;
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_Ipi, cut_pur_before_Ipi, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;

  std::cout << "------- Target Eff Pur Npi --------" << std::endl;
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_TAR_Npi, cut_pur_before_TAR_Npi, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;
  std::cout << "------- FGD1 Eff Pur Npi --------" << std::endl;
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_FGD1_Npi, cut_pur_before_FGD1_Npi, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;
  std::cout << "------- FGD2 Eff Pur Npi --------" << std::endl;
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_FGD2_Npi, cut_pur_before_FGD2_Npi, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;
  std::cout << "------- TOTAL Eff Pur Npi --------" << std::endl;
  draw.DrawPur(exp, "truelepton_mom",  NBins_Mom2, BinEdges_Mom2, cut_pur_after_Npi, cut_pur_before_Npi, "", "OVER");
  std::cout << draw.GetLastGraph()->GetY()[0] << std::endl;




  exp.SetCurrentTree("default");
}

// ********************************************************
void dump(Experiment exp, std::string prefix, std::string cut_base, std::string opt, std::string mc_path, std::string signal) {
  // ********************************************************
  DrawingTools  draw((mc_path + "All_ToF_noTarToF_nue.root").c_str(), 1);
  TCanvas c1("canva","",50,50,1000,800);

  draw.DumpSteps("kTrackerNueCC");
  draw.DumpCuts();

  std::string cut = cut_base + " && truelepton_mom > 200 && (selelec_ToF_mass == 0)";
  std::string cut2 = cut + " && " + signal;

  draw.Draw(exp, "1", 11, 0, 11, "all",  cut2.c_str(), "", "UNDER OVER");
  Float_t S = draw.GetLastStackTotal()->Integral();

  draw.Draw(exp, "1", 11, 0, 11, "all",  cut.c_str(), "", "UNDER OVER");
  Float_t N = draw.GetLastStackTotal()->Integral();
  //pur /= N;

  std::cout << "N = " << N << "   S = " << S << "  pur = " << S/N << std::endl;
  std::cout << "S = " << S/N*sqrt(N) << std::endl;

}

// ********************************************************
void ScanCut(Experiment exp, std::string prefix, std::string cut_base, std::string opt, std::string mc_path, std::string signal, bool RHC, bool Current) {
  // ********************************************************
  DrawingTools  draw((mc_path + "All_RHC_ToF_TarToF_nue.root").c_str(), 1);
  TCanvas c1("canva","",50,50,1000,800);

  Float_t min_val1 = 700.;
  Float_t max_val1 = 900.;
  Int_t   it1      = 4;
  Float_t step1    = (max_val1 - min_val1)/it1;

  Float_t min_val2 = 1400;
  Float_t max_val2 = 1700;
  Int_t   it2      = 6;
  Float_t step2    = (max_val2 - min_val2)/it2;

  Float_t min_val3 = 200.;
  Float_t max_val3 = 700.;
  Int_t   it3      = 10;
  Float_t step3    = (max_val3 - min_val3)/it3;

  Float_t min_val4 = 0.5;
  Float_t max_val4 = 0.9;
  Int_t   it4      = 10;
  Float_t step4    = (max_val4 - min_val4)/it4;

  Float_t bestCut1 = -999.;
  Float_t bestCut2 = -999.;
  Float_t bestCut3 = -999.;
  Float_t bestCut4 = -999.;
  Float_t SN       = 0.;
  Float_t bestPur  = 0.;  
  Float_t bestN    = 0.;

  std::string cut_base1 = cut_base + " && selelec_mom > 200 && selelec_likeel > 0.5";
  std::string cut0 = cut_base1;
  std::stringstream str;
  Float_t test4;
  
  /*for (Int_t z = 0; z < it4; ++z) {
  test4 = min_val4 + step4*z;
  str.str("");
  str << test4;
  cut0 = cut_base1 + " && selelec_likeel > " + str.str();
*/
  for (Int_t i = 0; i <= it1; ++i) {

    Float_t test1 = min_val1 + step1*i;
    str.str("");
    str << test1;
    std::string cut = cut0 + " &&  (selelec_mom < " + str.str();
    

    for (Int_t j = 0; j <= it2; ++j) {
      Float_t test2 = min_val2 + step2*j;
      str.str("");
      str << test2;
      std::string cut1 = cut + " || selelec_mom > " + str.str();

      for (Int_t k = 0; k <= it3; ++k) {
        Float_t test3 = min_val3 + step3*k;
        str.str("");
        str << test3;
        std::string cut3 = cut1 + " || selelec_ToF_mass < " + str.str() + ")";

        //std::string cut2 = cut1 + " && abs(particle) == 11";
        std::string cut2 = cut3 + " && " + signal;

        std::cout << cut2 << std::endl;

        draw.Draw(exp, "1", 11, 0, 11, "all",  cut2.c_str(), "", "");
        Float_t pur = draw.GetLastStackTotal()->Integral();    
  
        draw.Draw(exp, "1", 11, 0, 11, "all",  cut3.c_str(), "", "");
        Float_t N = draw.GetLastStackTotal()->Integral();
        pur /= N;
  
        std::cout << "Cut = " << test1 << "     " << test2 << std::endl;
        std::cout << "Pur = " << pur << std::endl;
        std::cout << "N = " <<  N << std::endl;
  
        Float_t SN_temp = pur * sqrt(N);
        std::cout << "SN = " << SN_temp << std::endl;
        if (SN < SN_temp) {
          SN      = SN_temp;
          bestCut1 = test1;
          bestCut2 = test2;
          bestCut3 = test3;
          bestCut4 = test4;
          bestPur = pur;
          bestN   = N;
  
        }       
      }
    }  
  }
  //} // 4D scan

  std::cout << "Best cut = " << bestCut1 << "   " << bestCut2 << "     " << bestCut3 << "    " << bestCut4 << "     SN = " << SN << std::endl;
  std::cout << "Best pur = " << bestPur  << "      N  = " << bestN << std::endl;
  //std::cout << "N before = " << pur << std::endl;
 
/*


  Float_t min_val = 0.5;
  Float_t max_val = 0.5;
  Int_t   it1     = 1;
  Float_t step    = (max_val - min_val)/it1;

  Float_t bestCut1= -999.;
  Float_t SN      = 0.;
  Float_t bestPur = 0.;
  Float_t bestN   = 0.;

  //std::string cut_base = cut_base1 + " && selelec_mom > 200 && selelec_likeel > 0.5 && (selelec_mom < 800 || selelec_mom > 1500 || selelec_ToF_mass < 300) ";
  std::string cut_base1 = cut_base + " && selelec_mom > 200 && selelec_likeel > 0.5";

  for (Int_t i = 0; i < it1; ++i) {
    Float_t test1 = min_val + step*i;
    std::stringstream str;
    str.str("");
    str << test1;
    //std::string cut = cut_base1 + " && (selelec_ToF_mass == 0 || selelec_ToF_lkl_proton < " + str.str() + ")";
    std::string cut = cut_base1 + " && selelec_likeel > " + str.str();
    std::string cut2 = cut + " && " + signal;

    std::cout << cut << std::endl;
    std::cout << cut2 << std::endl;

    draw.Draw(exp, "1", 11, 0, 11, "all",  cut2.c_str(), "", "");
    Float_t pur = draw.GetLastStackTotal()->Integral();    
  
    draw.Draw(exp, "1", 11, 0, 11, "all",  cut.c_str(), "", "");
    Float_t N = draw.GetLastStackTotal()->Integral();
    pur /= N;
  
    std::cout << "Cut = " << test1 << std::endl;
    std::cout << "Pur = " << pur << std::endl;
    std::cout << "N = " <<  N << std::endl;
  
    Float_t SN_temp = pur * sqrt(N);
    std::cout << "SN = " << SN_temp << std::endl;
    if (SN < SN_temp) {
      SN      = SN_temp;
      bestCut1 = test1;
      bestPur = pur;
      bestN   = N;
    }
  }



  std::cout << "Best cut = " << bestCut1 << "     SN = " << SN << std::endl;
  std::cout << "Best pur = " << bestPur << "      N  =" << bestN << std::endl;
  //std::cout << "N before = " << pur << std::endl;
 */
}


void AddSamples(Experiment* exp, bool UseNumu, bool RHC, std::string file_tmp) {
  SampleGroup nue("nue");
  SampleGroup numu("numu");

  DataSample *NueDS, *NumuDS;
  DataSample *NumuBarDS;

  // nue reference ToF has normalisation to 1e22. Norm to 1e21
  if (RHC)
    NueDS  = new DataSample((file_tmp + "nue.root").c_str(), 0.1);
  else 
    NueDS  = new DataSample((file_tmp + "nue.root").c_str(), 0.1);
  nue.AddMCSample("NueDS", NueDS);

  if (UseNumu) {
    NumuDS  = new DataSample((file_tmp + "numu.root").c_str(), 1.);
    numu.AddMCSample("numuDS", NumuDS);
    if (RHC) {
      NumuBarDS  = new DataSample((file_tmp + "numubar.root").c_str(), 1.);
      numu.AddMCSample("NumuBarDS", NumuBarDS);
    }
  }

  exp->AddSampleGroup("nue",  nue);
  exp->AddSampleGroup("numu", numu);
}
