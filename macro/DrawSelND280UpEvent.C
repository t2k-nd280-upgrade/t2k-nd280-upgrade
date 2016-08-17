
void DrawSelND280UpEvent
(
 string tag = "NuMu_6E20_Targ1_MuM_ParID",

 //string infilename = "outputs/prova_Evt0_NEvt1000.root"
 string infilename = "/neutrino/data7/davide/files/SelND280Up_NuMu_6E20-Targ1-MuM-ParID0_All.root"
 //string infilename = "/neutrino/data7/davide/files/SelND280Up_NuMu_6E20-Targ2-MuM-ParID0_All.root"
 //string infilename = "/neutrino/data7/davide/files/SelND280Up_NuMu_6E20-Targ1-PiM-ParID0_All.root"
 //string infilename = "/neutrino/data7/davide/files/SelND280Up_NuMu_6E20-Targ2-PiM-ParID0_All.root"
 //string infilename = "/neutrino/data7/davide/files/SelND280Up_NuMu_6E20-Targ1-PiP-ParID0_All.root"
 //string infilename = "/neutrino/data7/davide/files/SelND280Up_NuMu_6E20-Targ2-PiP-ParID0_All.root"
 //string infilename = "/neutrino/data7/davide/files/SelND280Up_NuMu_6E20-Targ1-Prot-ParID0_All.root"
 //string infilename = "/neutrino/data7/davide/files/SelND280Up_NuMu_6E20-Targ2-Prot-ParID0_All.root"
 )
{
  TFile *infile = new TFile(infilename.c_str(),"READ");

  // For efficiency
  TH2D *hCosThetaVsMom_FV = (TH2D*)infile->Get("hCosThetaVsMom_FV"); 
  TH2D *hCosThetaVsMom_Target = (TH2D*)infile->Get("hCosThetaVsMom_Target");       
  TH2D *hCosThetaVsMom_TPC = (TH2D*)infile->Get("hCosThetaVsMom_TPC");          
  TH2D *hCosThetaVsMom_TargetOrTPC = (TH2D*)infile->Get("hCosThetaVsMom_TargetOrTPC"); 
  TH2D *hCosThetaVsMom_TargetAndTPC = (TH2D*)infile->Get("hCosThetaVsMom_TargetAndTPC"); 
  // Track
  TH1D *hL_SD = (TH1D*)infile->Get("hL_SD");
  TH1D *hL_TargAll_FV = (TH1D*)infile->Get("hL_TargAll_FV");
  TH1D *hL_Targ1_FV = (TH1D*)infile->Get("hL_Targ1_FV");
  TH1D *hL_Targ2_FV = (TH1D*)infile->Get("hL_Targ2_FV");
  //
  TH1D *hL_TargAll_Cut = (TH1D*)infile->Get("hL_TargAll_Cut");
  TH1D *hL_Targ1_Cut = (TH1D*)infile->Get("hL_Targ1_Cut");
  TH1D *hL_Targ2_Cut = (TH1D*)infile->Get("hL_Targ2_Cut");
  //
  TH2D *hLVsMom_SD = (TH2D*)infile->Get("hLVsMom_SD");
  TH2D *hLVsMom_TargAll_FV = (TH2D*)infile->Get("hLVsMom_TargAll_FV");
  TH2D *hLVsMom_Targ1_FV = (TH2D*)infile->Get("hLVsMom_Targ1_FV");
  TH2D *hLVsMom_Targ2_FV = (TH2D*)infile->Get("hLVsMom_Targ2_FV");
  TH2D *hLVsMom_TargAll_Cut = (TH2D*)infile->Get("hLVsMom_TargAll_Cut");
  TH2D *hLVsMom_Targ1_Cut = (TH2D*)infile->Get("hLVsMom_Targ1_Cut");
  TH2D *hLVsMom_Targ2_Cut = (TH2D*)infile->Get("hLVsMom_Targ2_Cut");
  //
  TH1D *hLyz_TPCAll = (TH1D*)infile->Get("hLyz_TPCAll");
  TH1D *hLyz_TPCUp1 = (TH1D*)infile->Get("hLyz_TPCUp1");
  TH1D *hLyz_TPCUp2 = (TH1D*)infile->Get("hLyz_TPCUp2");
  TH1D *hLyz_TPCDown1 = (TH1D*)infile->Get("hLyz_TPCDown1");
  TH1D *hLyz_TPCDown2 = (TH1D*)infile->Get("hLyz_TPCDown2");
  TH1D *hLyz_ForwTPC1 = (TH1D*)infile->Get("hLyz_ForwTPC1");
  TH1D *hLyz_ForwTPC2 = (TH1D*)infile->Get("hLyz_ForwTPC2");
  TH1D *hLyz_ForwTPC3 = (TH1D*)infile->Get("hLyz_ForwTPC3");
  TH2D *hLyzVsMom_TPCAll = (TH2D*)infile->Get("hLyzVsMom_TPCAll");
  TH2D *hLyzVsMom_TPCUp1 = (TH2D*)infile->Get("hLyzVsMom_TPCUp1");
  TH2D *hLyzVsMom_TPCUp2 = (TH2D*)infile->Get("hLyzVsMom_TPCUp2");
  TH2D *hLyzVsMom_TPCDown1 = (TH2D*)infile->Get("hLyzVsMom_TPCDown1");
  TH2D *hLyzVsMom_TPCDown2 = (TH2D*)infile->Get("hLyzVsMom_TPCDown2");
  TH2D *hLyzVsMom_ForwTPC1 = (TH2D*)infile->Get("hLyzVsMom_ForwTPC1");
  TH2D *hLyzVsMom_ForwTPC2 = (TH2D*)infile->Get("hLyzVsMom_ForwTPC2");
  TH2D *hLyzVsMom_ForwTPC3 = (TH2D*)infile->Get("hLyzVsMom_ForwTPC3");
  //  
  TH1D *hMom = (TH1D*)infile->Get("hMom");
  TH1D *hPDG = (TH1D*)infile->Get("hPDG");
  TH1D *hCut = (TH1D*)infile->Get("hCut");
  // Neutrino 
  TH1D *hNuMom = (TH1D*)infile->Get("hNuMom");
  TH1D *hNuPDG = (TH1D*)infile->Get("hNuPDG");
  TH1D *hEvis = (TH1D*)infile->Get("hEvis");
  TH1D *hEreco = (TH1D*)infile->Get("hEreco");
  TH1D *hDiffEtrueEvis = (TH1D*)infile->Get("hDiffEtrueEvis");
  TH1D *hDiffEtrueEreco = (TH1D*)infile->Get("hDiffEtrueEreco");
  TH2D *hEtrueVsEvis = (TH2D*)infile->Get("hEtrueVsEvis");
  TH2D *hEtrueVsEreco = (TH2D*)infile->Get("hEtrueVsEreco");
  TH1D *hNuReacFV = (TH1D*)infile->Get("hNuReacFV");
  // Vertex
  TH2D *hVtxOut_XY = (TH2D*)infile->Get("hVtxOut_XY");
  TH2D *hVtxOut_XZ = (TH2D*)infile->Get("hVtxOut_XZ");
  TH2D *hVtxOut_YZ = (TH2D*)infile->Get("hVtxOut_YZ");
  TH1D *hVtxOut_X = (TH1D*)infile->Get("hVtxOut_X");
  TH1D *hVtxOut_Y = (TH1D*)infile->Get("hVtxOut_Y");
  TH1D *hVtxOut_Z = (TH1D*)infile->Get("hVtxOut_Z");
  TH2D *hVtx_XY = (TH2D*)infile->Get("hVtx_XY");
  TH2D *hVtx_XZ = (TH2D*)infile->Get("hVtx_XZ");
  TH2D *hVtx_YZ = (TH2D*)infile->Get("hVtx_YZ");
  TH1D *hVtx_X = (TH1D*)infile->Get("hVtx_X");
  TH1D *hVtx_Y = (TH1D*)infile->Get("hVtx_Y");
  TH1D *hVtx_Z = (TH1D*)infile->Get("hVtx_Z");
  
  // Get efficiencies
  TH1D *hMom_FV = hCosThetaVsMom_FV->ProjectionY()->Clone("hMom_Target_Eff");
  TH1D *hCosTheta_FV = hCosThetaVsMom_FV->ProjectionX()->Clone("hCosTheta_Target_Eff");
  
  TH2D *hCosThetaVsMom_Target_Eff 
    = (TH2D*)hCosThetaVsMom_Target->Clone("hCosThetaVsMom_Target_Eff");
  hCosThetaVsMom_Target_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_Target_Eff->SetTitle("hCosThetaVsMom_Target_Eff");
  TH1D *hMom_Target_Eff = hCosThetaVsMom_Target->ProjectionY()->Clone("hMom_Target_Eff");
  hMom_Target_Eff->Divide(hMom_FV);
  TH1D *hCosTheta_Target_Eff = hCosThetaVsMom_Target->ProjectionX()->Clone("hCosTheta_Target_Eff");
  hCosTheta_Target_Eff->Divide(hCosTheta_FV);
    
  TH2D *hCosThetaVsMom_TPC_Eff 
    = (TH2D*)hCosThetaVsMom_TPC->Clone("hCosThetaVsMom_TPC_Eff");
  hCosThetaVsMom_TPC_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_TPC_Eff->SetTitle("hCosThetaVsMom_TPC_Eff");
  TH1D *hMom_TPC_Eff = hCosThetaVsMom_TPC->ProjectionY()->Clone("hMom_TPC_Eff");
  hMom_TPC_Eff->Divide(hMom_FV);
  TH1D *hCosTheta_TPC_Eff = hCosThetaVsMom_TPC->ProjectionX()->Clone("hCosTheta_TPC_Eff");
  hCosTheta_TPC_Eff->Divide(hCosTheta_FV);

  TH2D *hCosThetaVsMom_TargetOrTPC_Eff 
    = (TH2D*)hCosThetaVsMom_TargetOrTPC->Clone("hCosThetaVsMom_TargetOrTPC_Eff");
  hCosThetaVsMom_TargetOrTPC_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_TargetOrTPC_Eff->SetTitle("hCosThetaVsMom_TargetOrTPC_Eff");
  TH1D *hMom_TargetOrTPC_Eff = hCosThetaVsMom_TargetOrTPC->ProjectionY()->Clone("hMom_TargetOrTPC_Eff");
  hMom_TargetOrTPC_Eff->Divide(hMom_FV);
  TH1D *hCosTheta_TargetOrTPC_Eff = hCosThetaVsMom_TargetOrTPC->ProjectionX()->Clone("hCosTheta_TargetOrTPC_Eff");
  hCosTheta_TargetOrTPC_Eff->Divide(hCosTheta_FV);
  
  TH2D *hCosThetaVsMom_TargetAndTPC_Eff 
    = (TH2D*)hCosThetaVsMom_TargetAndTPC->Clone("hCosThetaVsMom_TargetAndTPC_Eff");
  hCosThetaVsMom_TargetAndTPC_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_TargetAndTPC_Eff->SetTitle("hCosThetaVsMom_TargetAndTPC_Eff");
  TH1D *hMom_TargetAndTPC_Eff = hCosThetaVsMom_TargetAndTPC->ProjectionY()->Clone("hMom_TargetAndTPC_Eff");
  hMom_TargetAndTPC_Eff->Divide(hMom_FV);
  TH1D *hCosTheta_TargetAndTPC_Eff = hCosThetaVsMom_TargetAndTPC->ProjectionX()->Clone("hCosTheta_TargetAndTPC_Eff");
  hCosTheta_TargetAndTPC_Eff->Divide(hCosTheta_FV);

  //
  // Draw 
  //
  
  TCanvas *cVtx_XY = new TCanvas("cVtx_XY","cVtx_XY");
  hVtx_XY->GetXaxis()->SetTitle("X position (mm)");
  hVtx_XY->GetYaxis()->SetTitle("Y position (mm)");
  hVtx_XY->Draw("colz");
  TCanvas *cVtx_XZ = new TCanvas("cVtx_XZ","cVtx_XZ");
  hVtx_XZ->GetXaxis()->SetTitle("X position (mm)");
  hVtx_XZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtx_XZ->Draw("colz");
  TCanvas *cVtx_YZ = new TCanvas("cVtx_YZ","cVtx_YZ");
  hVtx_YZ->GetXaxis()->SetTitle("Y position (mm)");
  hVtx_YZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtx_YZ->Draw("colz");

  TCanvas *cVtxOut_XY = new TCanvas("cVtxOut_XY","cVtxOut_XY");
  hVtxOut_XY->GetXaxis()->SetTitle("X position (mm)");
  hVtxOut_XY->GetYaxis()->SetTitle("Y position (mm)");
  hVtxOut_XY->Draw("colz");
  TCanvas *cVtxOut_XZ = new TCanvas("cVtxOut_XZ","cVtxOut_XZ");
  hVtxOut_XZ->GetXaxis()->SetTitle("X position (mm)");
  hVtxOut_XZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtxOut_XZ->Draw("colz");
  TCanvas *cVtxOut_YZ = new TCanvas("cVtxOut_YZ","cVtxOut_YZ");
  hVtxOut_YZ->GetXaxis()->SetTitle("Y position (mm)");
  hVtxOut_YZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtxOut_YZ->Draw("colz");

  TCanvas *cVtx_X = new TCanvas("cVtx_X","cVtx_X");
  hVtx_X->GetXaxis()->SetTitle("X position (mm)");
  hVtx_X->Draw("");
  TCanvas *cVtx_Y = new TCanvas("cVtx_Y","cVtx_Y");
  hVtx_Y->GetXaxis()->SetTitle("Y position (mm)");
  hVtx_Y->Draw("");
  TCanvas *cVtx_Z = new TCanvas("cVtx_Z","cVtx_Z");
  hVtx_Z->GetXaxis()->SetTitle("Z position (mm)");
  hVtx_Z->Draw("");
  TCanvas *cVtxOut_X = new TCanvas("cVtxOut_X","cVtxOut_X");
  hVtxOut_X->GetXaxis()->SetTitle("X position (mm)");
  hVtxOut_X->Draw("");
  TCanvas *cVtxOut_Y = new TCanvas("cVtxOut_Y","cVtxOut_Y");
  hVtxOut_Y->GetXaxis()->SetTitle("Y position (mm)");
  hVtxOut_Y->Draw("");
  TCanvas *cVtxOut_Z = new TCanvas("cVtxOut_Z","cVtxOut_Z");
  hVtxOut_Z->GetXaxis()->SetTitle("Z position (mm)");
  hVtxOut_Z->Draw("");

  TCanvas *cCut = new TCanvas("cCut","cCut");
  hCut->Draw();
  TCanvas *cNuReacFV = new TCanvas("cNuReac","cNuReac");
  hNuReacFV->Draw();
  TCanvas *cNuMom = new TCanvas("cNuMom","cNuMom");
  hNuMom->GetXaxis()->SetTitle("Lepton momentum (MeV/c)");
  hNuMom->Draw();
  TCanvas *cDiffEtrueEvis = new TCanvas("cDiffEtrueEvis","cDiffEtrueEvis");
  hDiffEtrueEvis->GetXaxis()->SetTitle("E^{#nu}_{True} - E_{vis} (MeV)");
  hDiffEtrueEvis->Draw("");

  TCanvas *cLTarg1 = new TCanvas("cLTarg1","cLTarg1");
  hL_Targ1_FV->GetXaxis()->SetTitle("Target 1 track Length (mm)");
  hL_Targ1_Cut->SetLineColor(kRed);
  hL_Targ1_FV->Draw();
  hL_Targ1_Cut->Draw("same");
  TCanvas *cLTarg2 = new TCanvas("cLTarg2","cLTarg2");
  hL_Targ2_FV->GetXaxis()->SetTitle("Target 2 track Length (mm)");
  hL_Targ2_Cut->SetLineColor(kRed);
  hL_Targ2_FV->Draw();
  hL_Targ2_Cut->Draw("same");
  
  TCanvas *cLyz_TPCUp1 = new TCanvas("cLyz_TPCUp1","cLyz_TPCUp1");
  hLyz_TPCUp1->GetXaxis()->SetTitle("TPCUp1 track Length (mm)");
  hLyz_TPCUp1->Draw();
  TCanvas *cLyz_TPCUp2 = new TCanvas("cLyz_TPCUp2","cLyz_TPCUp2");
  hLyz_TPCUp2->GetXaxis()->SetTitle("TPCUp2 track Length (mm)");
  hLyz_TPCUp2->Draw();
  TCanvas *cLyz_TPCDown1 = new TCanvas("cLyz_TPCDown1","cLyz_TPCDown1");
  hLyz_TPCDown1->GetXaxis()->SetTitle("TPCDown1 track Length (mm)");
  hLyz_TPCDown1->Draw();
  TCanvas *cLyz_TPCDown2 = new TCanvas("cLyz_TPCDown2","cLyz_TPCDown2");
  hLyz_TPCDown2->GetXaxis()->SetTitle("TPCDown2 track Length (mm)");
  hLyz_TPCDown2->Draw();
  TCanvas *cLyz_ForwTPC1 = new TCanvas("cLyz_ForwTPC1","cLyz_ForwTPC1");
  hLyz_ForwTPC1->GetXaxis()->SetTitle("ForwTPC1 track Length (mm)");
  hLyz_ForwTPC1->Draw();
  TCanvas *cLyz_ForwTPC2 = new TCanvas("cLyz_ForwTPC2","cLyz_ForwTPC2");
  hLyz_ForwTPC2->GetXaxis()->SetTitle("ForwTPC2 track Length (mm)");
  hLyz_ForwTPC2->Draw();
  TCanvas *cLyz_ForwTPC3 = new TCanvas("cLyz_ForwTPC3","cLyz_ForwTPC3");
  hLyz_ForwTPC3->GetXaxis()->SetTitle("ForwTPC3 track Length (mm)");
  hLyz_ForwTPC3->Draw();

  TCanvas *cCosThetaVsMom_Target_Eff = new TCanvas("hCosThetaVsMom_Target_Eff","hCosThetaVsMom_Target_Eff");
  hCosThetaVsMom_Target_Eff->Draw("colz");
  TCanvas *cMom_Target_Eff = new TCanvas("hMom_Target_Eff","hMom_Target_Eff");
  hMom_Target_Eff->Draw("");
  TCanvas *cCosTheta_Target_Eff = new TCanvas("hCosTheta_Target_Eff","hCosTheta_Target_Eff");
  hCosTheta_Target_Eff->Draw("");

  TCanvas *cCosThetaVsMom_TPC_Eff = new TCanvas("hCosThetaVsMom_TPC_Eff","hCosThetaVsMom_TPC_Eff");
  hCosThetaVsMom_TPC_Eff->Draw("colz");
  TCanvas *cMom_TPC_Eff = new TCanvas("hMom_TPC_Eff","hMom_TPC_Eff");
  hMom_TPC_Eff->Draw("");
  TCanvas *cCosTheta_TPC_Eff = new TCanvas("hCosTheta_TPC_Eff","hCosTheta_TPC_Eff");
  hCosTheta_TPC_Eff->Draw("");

  TCanvas *cCosThetaVsMom_TargetOrTPC_Eff = new TCanvas("hCosThetaVsMom_TargetOrTPC_Eff","hCosThetaVsMom_TargetOrTPC_Eff");
  hCosThetaVsMom_TargetOrTPC_Eff->Draw("colz");
  TCanvas *cMom_TargetOrTPC_Eff = new TCanvas("hMom_TargetOrTPC_Eff","hMom_TargetOrTPC_Eff");
  hMom_TargetOrTPC_Eff->Draw("");
  TCanvas *cCosTheta_TargetOrTPC_Eff = new TCanvas("hCosTheta_TargetOrTPC_Eff","hCosTheta_TargetOrTPC_Eff");
  hCosTheta_TargetOrTPC_Eff->Draw("");

  TCanvas *cCosThetaVsMom_TargetAndTPC_Eff = new TCanvas("hCosThetaVsMom_TargetAndTPC_Eff","hCosThetaVsMom_TargetAndTPC_Eff");
  hCosThetaVsMom_TargetAndTPC_Eff->Draw("colz");
  TCanvas *cMom_TargetAndTPC_Eff = new TCanvas("hMom_TargetAndTPC_Eff","hMom_TargetAndTPC_Eff");
  hMom_TargetAndTPC_Eff->Draw("");
  TCanvas *cCosTheta_TargetAndTPC_Eff = new TCanvas("hCosTheta_TargetAndTPC_Eff","hCosTheta_TargetAndTPC_Eff");
  hCosTheta_TargetAndTPC_Eff->Draw("");


  // Print files
  cVtx_XY->Print("outputs/NuMu_6E20_Targ1_MuM_ParID/cVtx_XY.pdf");

  return;
}
