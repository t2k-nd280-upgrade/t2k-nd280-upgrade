//##################################################################################
//##################################################################################
//##################### Created by B. Quilain on 2017/05/20 ########################
//##################### Read output of DetectorValidation.C ########################
//################### Last modification by B. Quilain on 2017/05/31 ################
//##################################################################################
//##################################################################################
{

  //Step 0. Set the drawing options
  gStyle->SetOptFit();
  gStyle->SetOptStat(false);



  
  //Step 1. Read/output of the muon particle gun
  TFile * fMuon = new TFile("PGmuon2-Targ1-MuM-ParID0-CC_Evt0_NEvt1.root","read");
  TH1D * Muon_TotalEdep_pecorr_plan = (TH1D*) fMuon->Get("TotalEdep_pecorr_plan");
  TH1D * Muon_TotalEdep_pecorr_grid = (TH1D*) fMuon->Get("TotalEdep_pecorr_grid");

  TH1D * Muon_TotalEdep_pecorrfiber_plan = (TH1D*) fMuon->Get("TotalEdep_pecorrfiber_plan");
  TH1D * Muon_TotalEdep_pecorrfiber_grid = (TH1D*) fMuon->Get("TotalEdep_pecorrfiber_grid");

  TH1D * Muon_TotalEdep_pe_plan = (TH1D*) fMuon->Get("TotalEdep_pe_plan");
  TH1D * Muon_TotalEdep_pe_grid = (TH1D*) fMuon->Get("TotalEdep_pe_grid");

  TCanvas * cPlan = new TCanvas("cPlan");
  Muon_TotalEdep_pe_plan->GetXaxis()->SetTitle("p.e"); 
  Muon_TotalEdep_pe_plan->GetXaxis()->SetTitleOffset(1.3);
  Muon_TotalEdep_pe_plan->GetYaxis()->SetTitle("Number of hits");
  Muon_TotalEdep_pe_plan->GetYaxis()->SetTitleOffset(1.3);
  Muon_TotalEdep_pe_plan->SetLineColor(kMagenta);
  Muon_TotalEdep_pecorrfiber_plan->SetLineColor(kCyan+2);
  Muon_TotalEdep_pecorr_plan->SetLineColor(kBlue);
  Muon_TotalEdep_pe_plan->Draw();
  Muon_TotalEdep_pecorrfiber_plan->Draw("same");
  Muon_TotalEdep_pecorr_plan->Draw("same");
  TLegend * lPlan = new TLegend(0.6,0.6,0.99,0.99);
  lPlan->AddEntry(Muon_TotalEdep_pe_plan,"Raw");
  lPlan->AddEntry(Muon_TotalEdep_pecorrfiber_plan,"w/ fiber att.");
  lPlan->AddEntry(Muon_TotalEdep_pecorr_plan,"w/ fiber and scint. att.");
  lPlan->Draw("same");
  cPlan->SaveAs("PlanScintillator.png");
  //Muon_TotalEdep_pe_plan->GetYaxis()->SetRangeUser
    
  TCanvas * cPlanGrid = new TCanvas("cPlanGrid");
  Muon_TotalEdep_pecorr_plan->GetXaxis()->SetTitle("p.e"); 
  Muon_TotalEdep_pecorr_plan->GetXaxis()->SetTitleOffset(1.3);
  Muon_TotalEdep_pecorr_plan->GetYaxis()->SetTitle("Number of hits");
  Muon_TotalEdep_pecorr_plan->GetYaxis()->SetTitleOffset(1.3);
  Muon_TotalEdep_pecorr_plan->SetLineColor(1.);
  Muon_TotalEdep_pecorr_grid->SetLineColor(kBlue);
  Muon_TotalEdep_pecorr_plan->Draw();
  Muon_TotalEdep_pecorr_grid->Draw("same");
  TLegend * lPlanGrid = new TLegend(0.7,0.7,0.99,0.99);
  lPlanGrid->AddEntry(Muon_TotalEdep_pecorr_plan,"Plan");
  lPlanGrid->AddEntry(Muon_TotalEdep_pecorr_grid,"Grid");
  lPlanGrid->Draw("same");
  cPlanGrid->SaveAs("PlanGridScintillator.png");

  //////////
  TH2D * MuonEventDisplay_Side = (TH2D*)fMuon->Get("EventDisplay_Side_1");
  TH2D * MuonEventDisplay_Top = (TH2D*)fMuon->Get("EventDisplay_Top_1");
  TCanvas * cEventDisplay = new TCanvas("cEventDisplay");
  cEventDisplay->Divide(1,2);
  cEventDisplay->cd(1);
  MuonEventDisplay_Side->Draw("colz");
  cEventDisplay->cd(2);
  MuonEventDisplay_Top->Draw("colz");
  cEventDisplay->SaveAs("MuonEvent.png");





  
  
  //////////////////////////////////////////////////////////////////////////////////
  //Step 2. Read/output of the proton particle gun
  TFile * fProton = new TFile("PGproton2-Targ1-MuM-ParID0-CC_Evt0_NEvt1.root","read");
  TH1D * Proton_TotalEdep_pecorr_plan = (TH1D*) fProton->Get("TotalEdep_pecorr_plan");
  TH1D * Proton_TotalEdep_pecorr_grid = (TH1D*) fProton->Get("TotalEdep_pecorr_grid");

  TH1D * Proton_TotalEdep_pecorrfiber_plan = (TH1D*) fProton->Get("TotalEdep_pecorrfiber_plan");
  TH1D * Proton_TotalEdep_pecorrfiber_grid = (TH1D*) fProton->Get("TotalEdep_pecorrfiber_grid");

  TH1D * Proton_TotalEdep_pe_plan = (TH1D*) fProton->Get("TotalEdep_pe_plan");
  TH1D * Proton_TotalEdep_pe_grid = (TH1D*) fProton->Get("TotalEdep_pe_grid");

    //////////////////////////////////////////////////////////////////////////////////
  TFile * fPion = new TFile("PGpion2-Targ1-MuM-ParID0-CC_Evt0_NEvt1.root","read");
  TH1D * Pion_TotalEdep_pecorr_plan = (TH1D*) fPion->Get("TotalEdep_pecorr_plan");
  TH1D * Pion_TotalEdep_pecorr_grid = (TH1D*) fPion->Get("TotalEdep_pecorr_grid");

  TH1D * Pion_TotalEdep_pecorrfiber_plan = (TH1D*) fPion->Get("TotalEdep_pecorrfiber_plan");
  TH1D * Pion_TotalEdep_pecorrfiber_grid = (TH1D*) fPion->Get("TotalEdep_pecorrfiber_grid");

  TH1D * Pion_TotalEdep_pe_plan = (TH1D*) fPion->Get("TotalEdep_pe_plan");
  TH1D * Pion_TotalEdep_pe_grid = (TH1D*) fPion->Get("TotalEdep_pe_grid");

  TCanvas * cMuonProton = new TCanvas("cMuonProton");
  Muon_TotalEdep_pe_plan->GetXaxis()->SetTitle("p.e"); 
  Muon_TotalEdep_pe_plan->GetXaxis()->SetTitleOffset(1.3);
  Muon_TotalEdep_pe_plan->GetYaxis()->SetTitle("Number of hits");
  Muon_TotalEdep_pe_plan->GetYaxis()->SetTitleOffset(1.3);
  Muon_TotalEdep_pe_plan->SetLineColor(kBlue);
  Pion_TotalEdep_pe_plan->SetLineColor(kGreen+2);
  Proton_TotalEdep_pe_plan->SetLineColor(kRed);
  Muon_TotalEdep_pe_plan->DrawNormalized();
  Pion_TotalEdep_pe_plan->DrawNormalized("same");
  Proton_TotalEdep_pe_plan->DrawNormalized("same");
  TLegend * lMuonProton = new TLegend(0.7,0.7,0.99,0.99);
  lMuonProton->AddEntry(Muon_TotalEdep_pe_plan,"Muon");
  lMuonProton->AddEntry(Pion_TotalEdep_pe_plan,"Pion");
  lMuonProton->AddEntry(Proton_TotalEdep_pe_plan,"Proton");
  lMuonProton->Draw("same");
  cMuonProton->SaveAs("MuonProton.png");
  
  TCanvas * cMuonProtonCorr = new TCanvas("cMuonProtonCorr");
  Muon_TotalEdep_pecorr_plan->GetXaxis()->SetTitle("p.e"); 
  Muon_TotalEdep_pecorr_plan->GetXaxis()->SetTitleOffset(1.3);
  Muon_TotalEdep_pecorr_plan->GetYaxis()->SetTitle("Number of hits");
  Muon_TotalEdep_pecorr_plan->GetYaxis()->SetTitleOffset(1.3);
  Muon_TotalEdep_pecorr_plan->SetLineColor(kBlue);
  Pion_TotalEdep_pecorr_plan->SetLineColor(kGreen+2);
  Proton_TotalEdep_pecorr_plan->SetLineColor(kRed);
  Muon_TotalEdep_pecorr_plan->DrawNormalized();
  Pion_TotalEdep_pecorr_plan->DrawNormalized("same");
  Proton_TotalEdep_pecorr_plan->DrawNormalized("same");
  lMuonProton->Draw("same");
  cMuonProtonCorr->SaveAs("MuonProtonCorr.png");

  TCanvas * cMuonProtonGRID = new TCanvas("cMuonProtonGRID");
  Muon_TotalEdep_pe_grid->GetXaxis()->SetTitle("p.e"); 
  Muon_TotalEdep_pe_grid->GetXaxis()->SetTitleOffset(1.3);
  Muon_TotalEdep_pe_grid->GetYaxis()->SetTitle("Number of hits");
  Muon_TotalEdep_pe_grid->GetYaxis()->SetTitleOffset(1.3);
  Muon_TotalEdep_pe_grid->SetLineColor(kBlue);
  Pion_TotalEdep_pe_grid->SetLineColor(kGreen+2);
  Proton_TotalEdep_pe_grid->SetLineColor(kRed);
  Muon_TotalEdep_pe_grid->DrawNormalized();
  Pion_TotalEdep_pe_grid->DrawNormalized("same");
  Proton_TotalEdep_pe_grid->DrawNormalized("same");
  lMuonProton->Draw("same");
  cMuonProtonGRID->SaveAs("MuonProtonGRID.png");
  
  TCanvas * cMuonProtonGRIDCorr = new TCanvas("cMuonProtonGRIDCorr");
  Muon_TotalEdep_pecorr_grid->GetXaxis()->SetTitle("p.e"); 
  Muon_TotalEdep_pecorr_grid->GetXaxis()->SetTitleOffset(1.3);
  Muon_TotalEdep_pecorr_grid->GetYaxis()->SetTitle("Number of hits");
  Muon_TotalEdep_pecorr_grid->GetYaxis()->SetTitleOffset(1.3);
  Muon_TotalEdep_pecorr_grid->SetLineColor(kBlue);
  Pion_TotalEdep_pecorr_grid->SetLineColor(kGreen+2);
  Proton_TotalEdep_pecorr_grid->SetLineColor(kRed);
  Muon_TotalEdep_pecorr_grid->DrawNormalized();
  Pion_TotalEdep_pecorr_grid->DrawNormalized("same");
  Proton_TotalEdep_pecorr_grid->DrawNormalized("same");
  lMuonProton->Draw("same");
  cMuonProtonGRIDCorr->SaveAs("MuonProtonGRIDCorr.png");
   
  //////////
  TH2D * ProtonEventDisplay_Side = (TH2D*)fProton->Get("EventDisplay_Side_1");
  TH2D * ProtonEventDisplay_Top = (TH2D*)fProton->Get("EventDisplay_Top_1");
  TCanvas * cProtonEventDisplay = new TCanvas("cProtonEventDisplay");
  cProtonEventDisplay->Divide(1,2);
  cProtonEventDisplay->cd(1);
  ProtonEventDisplay_Side->Draw("colz");
  cProtonEventDisplay->cd(2);
  ProtonEventDisplay_Top->Draw("colz");
  cProtonEventDisplay->SaveAs("ProtonEvent.png");





  
  
  //////////////////////////////////////////////////////////////////////////
  //Step 3. Read/output of the neutrino event
  //TFile * fGENIE = new TFile("PGGENIE4-Targ1-MuM-ParID0-CC_Evt0_NEvt1.root","read");
  //TFile * fGENIE = new TFile("Output_GENIE-Targ1-MuM-ParID0-CC_Evt0_NEvt1.root","read");
  TFile * fGENIE = new TFile("Output_PGMuon2-Targ1-MuM-ParID0-CC_Evt0_NEvt1.root","read");

  TH1D * GENIE_TotalEdep_pecorr_plan = (TH1D*) fGENIE->Get("TotalEdep_pecorr_plan");
  TH1D * GENIE_TotalEdep_pecorr_grid = (TH1D*) fGENIE->Get("TotalEdep_pecorr_grid");

  TH1D * GENIE_TotalEdep_pecorrfiber_plan = (TH1D*) fGENIE->Get("TotalEdep_pecorrfiber_plan");
  TH1D * GENIE_TotalEdep_pecorrfiber_grid = (TH1D*) fGENIE->Get("TotalEdep_pecorrfiber_grid");

  TH1D * GENIE_TotalEdep_pe_plan = (TH1D*) fGENIE->Get("TotalEdep_pe_plan");
  TH1D * GENIE_TotalEdep_pe_grid = (TH1D*) fGENIE->Get("TotalEdep_pe_grid");

  TCanvas * cGENIEPlan = new TCanvas("cGENIEPlan");
  GENIE_TotalEdep_pe_plan->GetXaxis()->SetTitle("p.e"); 
  GENIE_TotalEdep_pe_plan->GetXaxis()->SetTitleOffset(1.3);
  GENIE_TotalEdep_pe_plan->GetYaxis()->SetTitle("Number of hits");
  GENIE_TotalEdep_pe_plan->GetYaxis()->SetTitleOffset(1.3);
  GENIE_TotalEdep_pe_plan->SetLineColor(kMagenta);
  GENIE_TotalEdep_pecorrfiber_plan->SetLineColor(kCyan+2);
  GENIE_TotalEdep_pecorr_plan->SetLineColor(kBlue);
  GENIE_TotalEdep_pe_plan->Draw();
  GENIE_TotalEdep_pecorrfiber_plan->Draw("same");
  GENIE_TotalEdep_pecorr_plan->Draw("same");
  TLegend * lGENIEPlan = new TLegend(0.6,0.6,0.99,0.99);
  lGENIEPlan->AddEntry(GENIE_TotalEdep_pe_plan,"Raw");
  lGENIEPlan->AddEntry(GENIE_TotalEdep_pecorrfiber_plan,"w/ fiber att.");
  lGENIEPlan->AddEntry(GENIE_TotalEdep_pecorr_plan,"w/ fiber and scint. att.");
  lGENIEPlan->Draw("same");
  cGENIEPlan->SaveAs("GENIEPlanScintillator.png");
  
  TCanvas * cGENIEGrid = new TCanvas("cGENIEGrid");
  GENIE_TotalEdep_pe_grid->GetXaxis()->SetTitle("p.e"); 
  GENIE_TotalEdep_pe_grid->GetXaxis()->SetTitleOffset(1.3);
  GENIE_TotalEdep_pe_grid->GetYaxis()->SetTitle("Number of hits");
  GENIE_TotalEdep_pe_grid->GetYaxis()->SetTitleOffset(1.3);
  GENIE_TotalEdep_pe_grid->SetLineColor(kMagenta);
  GENIE_TotalEdep_pecorrfiber_grid->SetLineColor(kCyan+2);
  GENIE_TotalEdep_pecorr_grid->SetLineColor(kBlue);
  GENIE_TotalEdep_pe_grid->Draw();
  GENIE_TotalEdep_pecorrfiber_grid->Draw("same");
  GENIE_TotalEdep_pecorr_grid->Draw("same");
  TLegend * lGENIEGrid = new TLegend(0.6,0.6,0.99,0.99);
  lGENIEGrid->AddEntry(GENIE_TotalEdep_pe_grid,"Raw");
  lGENIEGrid->AddEntry(GENIE_TotalEdep_pecorrfiber_grid,"w/ fiber att.");
  lGENIEGrid->AddEntry(GENIE_TotalEdep_pecorr_grid,"w/ fiber and scint. att.");
  lGENIEGrid->Draw("same");
  //Temp
  GENIE_TotalEdep_pe_grid->GetYaxis()->SetRangeUser(0,1.2e4);
  cGENIEGrid->SaveAs("GENIEGridScintillator.png");
  
  TCanvas * cGENIEPlanGrid = new TCanvas("cGENIEPlanGrid");
  GENIE_TotalEdep_pecorr_plan->GetXaxis()->SetTitle("p.e"); 
  GENIE_TotalEdep_pecorr_plan->GetXaxis()->SetTitleOffset(1.3);
  GENIE_TotalEdep_pecorr_plan->GetYaxis()->SetTitle("Number of hits");
  GENIE_TotalEdep_pecorr_plan->GetYaxis()->SetTitleOffset(1.3);
  GENIE_TotalEdep_pecorr_plan->SetLineColor(1.);
  GENIE_TotalEdep_pecorr_grid->SetLineColor(kBlue);
  GENIE_TotalEdep_pecorr_plan->Draw();
  GENIE_TotalEdep_pecorr_grid->Draw("same");
  TLegend * lGENIEPlanGrid = new TLegend(0.7,0.7,0.99,0.99);
  lGENIEPlanGrid->AddEntry(GENIE_TotalEdep_pecorr_plan,"Plan");
  lGENIEPlanGrid->AddEntry(GENIE_TotalEdep_pecorr_grid,"Grid");
  lGENIEPlanGrid->Draw("same");
 cGENIEPlanGrid->SaveAs("GENIEPlanGridScintillator.png");
   //////////

 //If wish event display
 /*
  TH2D * GENIEEventDisplay_Side = (TH2D*)fGENIE->Get("EventDisplay_Side_0");
  TH2D * GENIEEventDisplay_Top = (TH2D*)fGENIE->Get("EventDisplay_Top_0");
  TCanvas * cGENIEEventDisplay = new TCanvas("cGENIEEventDisplay");
  cGENIEEventDisplay->Divide(1,2);
  cGENIEEventDisplay->cd(1);
  GENIEEventDisplay_Side->Draw("colz");
  cGENIEEventDisplay->cd(2);
  GENIEEventDisplay_Top->Draw("colz");
  cGENIEEventDisplay->SaveAs("cGENIEEventDisplay.png");
 */
  //////////
  /*
  TH3D * GENIEEdep_positioninscinti_planhoriz = (TH3D*) fGENIE->Get("Edep_positioninscinti_planhoriz");
  TH2D * GENIEEdep_positioninscinti_planhoriz_fiberatt = (TH2D*) GENIEEdep_positioninscinti_planhoriz->Project3D("zx");

  TCanvas * cEdep_planhoriz_fiberatt = new TCanvas("cEdep_planhoriz_fiberatt");
  GENIEEdep_positioninscinti_planhoriz_fiberatt->Draw("colz");
  cEdep_planhoriz_fiberatt->SaveAs("GENIE_EnergyDeposition_FiberAttenuation.png");
  */
  TH3D * GENIEEdep_positioninscinti_planvert = (TH3D*) fGENIE->Get("Edep_positioninscinti_planvert");
  GENIEEdep_positioninscinti_planvert->RebinX(10);
  GENIEEdep_positioninscinti_planvert->GetYaxis()->SetRangeUser(0,15);
  TH2D * GENIEEdep_positioninscinti_planvert_fiberatt = (TH2D*) GENIEEdep_positioninscinti_planvert->Project3D("zx");
  TH2D * GENIEEdep_positioninscinti_planvert_scintiatt = (TH2D*) GENIEEdep_positioninscinti_planvert->Project3D("zy");
  TProfile2D * GENIEEdep_positioninscinti_planvert_corr = (TProfile2D*) GENIEEdep_positioninscinti_planvert->Project3DProfile("xy");

  
  TCanvas * cEdep_planvert_fiberatt = new TCanvas("cEdep_planvert_fiberatt");
  GENIEEdep_positioninscinti_planvert_fiberatt->Draw("colz");
  GENIEEdep_positioninscinti_planvert_fiberatt->GetYaxis()->SetRangeUser(0,50);
  cEdep_planvert_fiberatt->SaveAs("GENIE_EnergyDeposition_FiberAttenuation_Plan.png");

  TCanvas * cEdep_planvert_scintiatt = new TCanvas("cEdep_planvert_scintiatt");
  GENIEEdep_positioninscinti_planvert_scintiatt->Draw("colz");
  GENIEEdep_positioninscinti_planvert_scintiatt->GetYaxis()->SetRangeUser(0,50);
  cEdep_planvert_scintiatt->SaveAs("GENIE_EnergyDeposition_ScintiAttenuation_Plan.png");
  
  TCanvas * cEdep_planvert_corr = new TCanvas("cEdep_planvert_corr");
  GENIEEdep_positioninscinti_planvert_corr->Draw("colz");
  cEdep_planvert_corr->SaveAs("GENIE_EnergyDeposition_Correction_Plan.png");

  TCanvas * cEdep_planvert_fiberatt_1D = new TCanvas("cEdep_planvert_fiberatt_1D");
  //GENIEEdep_positioninscinti_planvert_fiberatt->Draw("colz");
  TH1D * GENIEEdep_positioninscinti_planvert_fiberatt_1D = (TH1D*) (GENIEEdep_positioninscinti_planvert_fiberatt->ProfileX("GENIEEdep_positioninscinti_planvert_fiberatt_1D",1,GENIEEdep_positioninscinti_planvert_fiberatt->GetYaxis()->GetNbins()));
  GENIEEdep_positioninscinti_planvert_fiberatt_1D->Draw();
  TF1 * fGENIEEdep_positioninscinti_planvert_fiberatt_1D = new TF1("fGENIEEdep_positioninscinti_planvert_fiberatt_1D","[0]*TMath::Exp(-x/[1])",0,600);
  fGENIEEdep_positioninscinti_planvert_fiberatt_1D->SetParameter(0,1);
  fGENIEEdep_positioninscinti_planvert_fiberatt_1D->SetParameter(1,1);
  GENIEEdep_positioninscinti_planvert_fiberatt_1D->Fit(fGENIEEdep_positioninscinti_planvert_fiberatt_1D);
  cEdep_planvert_fiberatt_1D->SaveAs("cEdep_planvert_fiberatt_1D.png");
  
  
  TCanvas * cEdep_planvert_scintiatt_1D = new TCanvas("cEdep_planvert_scintiatt_1D");
  //GENIEEdep_positioninscinti_planvert_scintiatt->Draw("colz");
  TH1D * GENIEEdep_positioninscinti_planvert_scintiatt_1D = (TH1D*) (GENIEEdep_positioninscinti_planvert_scintiatt->ProfileX("GENIEEdep_positioninscinti_planvert_scintiatt_1D",1,GENIEEdep_positioninscinti_planvert_scintiatt->GetYaxis()->GetNbins()));
  GENIEEdep_positioninscinti_planvert_scintiatt_1D->Draw();
  TF1 * fGENIEEdep_positioninscinti_planvert_scintiatt_1D = new TF1("fGENIEEdep_positioninscinti_planvert_scintiatt_1D","[0]*TMath::Exp(-x/[1])",0,600);
  fGENIEEdep_positioninscinti_planvert_scintiatt_1D->SetParameter(0,1);
  fGENIEEdep_positioninscinti_planvert_scintiatt_1D->SetParameter(1,1);
  GENIEEdep_positioninscinti_planvert_scintiatt_1D->Fit(fGENIEEdep_positioninscinti_planvert_scintiatt_1D);
  cEdep_planvert_scintiatt_1D->SaveAs("cEdep_planvert_scintiatt_1D.png");
  
  //////////
  TH3D * GENIEEdep_positioninscinti_gridvert = (TH3D*) fGENIE->Get("Edep_positioninscinti_gridvert");
  TH2D * GENIEEdep_positioninscinti_gridvert_fiberatt = (TH2D*) GENIEEdep_positioninscinti_gridvert->Project3D("zx");
  TH2D * GENIEEdep_positioninscinti_gridvert_scintiatt = (TH2D*) GENIEEdep_positioninscinti_gridvert->Project3D("zy");
  TProfile2D * GENIEEdep_positioninscinti_gridvert_corr = (TProfile2D*) GENIEEdep_positioninscinti_gridvert->Project3DProfile("xy");
  
  /*  TCanvas * cEdep_gridvert_fiberatt = new TCanvas("cEdep_gridvert_fiberatt");
  //GENIEEdep_positioninscinti_gridvert_fiberatt->Draw("colz");
  (GENIEEdep_positioninscinti_gridvert_fiberatt->ProfileX("GENIEEdep_positioninscinti_gridvert_fiberatt_1D",1,GENIEEdep_positioninscinti_gridvert_fiberatt->GetYaxis()->GetNbins()))->Draw();
  
  TCanvas * cEdep_gridvert_scintiatt = new TCanvas("cEdep_gridvert_scintiatt");
  //GENIEEdep_positioninscinti_gridvert_scintiatt->Draw("colz");
  (GENIEEdep_positioninscinti_gridvert_scintiatt->ProfileX("  (GENIEEdep_positioninscinti_gridvert_scintiatt_1D",1,GENIEEdep_positioninscinti_gridvert_scintiatt->GetYaxis()->GetNbins()))->Draw();
  */
  TCanvas * cEdep_gridvert_corr = new TCanvas("cEdep_gridvert_corr");
  GENIEEdep_positioninscinti_gridvert_corr->Draw("colz");
  cEdep_gridvert_corr->SaveAs("GENIE_EnergyDeposition_Correction_Grid.png");
  GENIEEdep_positioninscinti_gridvert_corr->GetYaxis()->SetRangeUser(0.,10.);
  cEdep_gridvert_corr->SaveAs("GENIE_EnergyDeposition_Correction_Grid_Zoomed.png");
  
  //////////////////////////////////////////////////////////////////////////////////
 //////////
  TH3D * GENIEEdep_positioninscintiabsolute_gridvert = (TH3D*) fGENIE->Get("Edep_positioninscintiabsolute_gridvert");
  TH2D * GENIEEdep_positioninscintiabsolute_gridvert_fiberatt = (TH2D*) GENIEEdep_positioninscintiabsolute_gridvert->Project3D("zx");
  TH2D * GENIEEdep_positioninscintiabsolute_gridvert_scintiabsoluteatt = (TH2D*) GENIEEdep_positioninscintiabsolute_gridvert->Project3D("zy");
  TProfile2D * GENIEEdep_positioninscintiabsolute_gridvert_corr = (TProfile2D*) GENIEEdep_positioninscintiabsolute_gridvert->Project3DProfile("xy");

  TCanvas * cEdep_gridabsolutevert_corr = new TCanvas("cEdep_gridabsolutevert_corr");
  GENIEEdep_positioninscintiabsolute_gridvert_corr->Draw("colz");
  cEdep_gridabsolutevert_corr->SaveAs("GENIE_EnergyDeposition_Correction_Grid.png");
  
  ///////////////////
  //GENIEEdep_positioninscintiabsolute_planvert_corr->GetYaxis()->SetRangeUser(0.,10.);
  //cEdep_planvert_corr->SaveAs("GENIE_EnergyDeposition_Correction_Plan_Zoomed.png");
  TH3D * GENIEEdep_positioninscintiabsolute_planvert = (TH3D*) fGENIE->Get("Edep_positioninscintiabsolute_planvert");
  //  GENIEEdep_positioninscintiabsolute_planvert->Rebin3D(2,4,1);
    
  TH2D * GENIEEdep_positioninscintiabsolute_planvert_fiberatt = (TH2D*) GENIEEdep_positioninscintiabsolute_planvert->Project3D("zx");
  TH2D * GENIEEdep_positioninscintiabsolute_planvert_scintiabsoluteatt = (TH2D*) GENIEEdep_positioninscintiabsolute_planvert->Project3D("zy");
  TProfile2D * GENIEEdep_positioninscintiabsolute_planvert_corr = (TProfile2D*) GENIEEdep_positioninscintiabsolute_planvert->Project3DProfile("xy");

  TCanvas * cEdep_planabsolutevert_corr = new TCanvas("cEdep_planabsolutevert_corr");
  GENIEEdep_positioninscintiabsolute_planvert_corr->Draw("colz");
  cEdep_planabsolutevert_corr->SaveAs("GENIE_EnergyDeposition_Correction_Plan.png");
  //GENIEEdep_positioninscintiabsolute_planvert_corr->GetYaxis()->SetRangeUser(0.,10.);
  //cEdep_planvert_corr->SaveAs("GENIE_EnergyDeposition_Correction_Plan_Zoomed.png");
}
