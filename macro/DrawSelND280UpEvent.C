
void DrawSelND280UpEvent
( 
 string infilename = "../../../files/SelND280Up_NuMu_6E20-Targ1-MuM-ParID0_30cm_All.root"

 //string infilename = "../../../files/SelND280Up_NuMu_6E20-Targ1-MuM-ParID0_All.root "
 //string infilename = "../../../files/SelND280Up_NuMu_6E20-Targ2-MuM-ParID0_All.root"
 
 //string infilename = "../../../files/SelND280Up_NuMu_6E20-Targ1-PiM-ParID0_All.root"
 //string infilename = "../../../files/SelND280Up_NuMu_6E20-Targ2-PiM-ParID0_All.root"
 
 //string infilename = "../../../files/SelND280Up_NuMu_6E20-Targ1-PiP-ParID0_All.root"
 //string infilename = "../../../files/SelND280Up_NuMu_6E20-Targ2-PiP-ParID0_All.root"
 
 //string infilename = "../../../files/SelND280Up_NuMu_6E20-Targ1-Prot-ParID0_All.root"
 //string infilename = "../../../files/SelND280Up_NuMu_6E20-Targ2-Prot-ParID0_All.root"
 
 //string infilename = "../../../files/SelND280Up_NuMu_6E20-Targ1-Targ2_All.root"
 )
{
  gROOT->SetStyle("T2K");                  
  gROOT->ForceStyle();                 
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadRightMargin(0.15);
  //gStyle->SetStatX(.4); gStyle->SetStatY(.6);

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
  TH1D *hMom_FV = hCosThetaVsMom_FV->ProjectionY()->Clone("hMom_FV");
  TH1D *hCosTheta_FV = hCosThetaVsMom_FV->ProjectionX()->Clone("hCosTheta_FV");
  
  TH2D *hCosThetaVsMom_Target_Eff 
    = (TH2D*)hCosThetaVsMom_Target->Clone("hCosThetaVsMom_Target_Eff");
  hCosThetaVsMom_Target_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_Target_Eff->SetTitle("hCosThetaVsMom_Target_Eff");
  TH1D *hMom_Target_Eff = hCosThetaVsMom_Target->ProjectionY()->Clone("hMom_Target_Eff");
  hMom_Target_Eff->Divide(hMom_FV);
  hMom_Target_Eff->SetTitle("hMom_Target_Eff");
  TH1D *hCosTheta_Target_Eff = hCosThetaVsMom_Target->ProjectionX()->Clone("hCosTheta_Target_Eff");
  hCosTheta_Target_Eff->Divide(hCosTheta_FV);
  hCosTheta_Target_Eff->SetTitle("hCosTheta_Target_Eff");
    
  TH2D *hCosThetaVsMom_TPC_Eff 
    = (TH2D*)hCosThetaVsMom_TPC->Clone("hCosThetaVsMom_TPC_Eff");
  hCosThetaVsMom_TPC_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_TPC_Eff->SetTitle("hCosThetaVsMom_TPC_Eff");
  TH1D *hMom_TPC_Eff = hCosThetaVsMom_TPC->ProjectionY()->Clone("hMom_TPC_Eff");
  hMom_TPC_Eff->Divide(hMom_FV);
  hMom_TPC_Eff->SetTitle("hMom_TPC_Eff");
  TH1D *hCosTheta_TPC_Eff = hCosThetaVsMom_TPC->ProjectionX()->Clone("hCosTheta_TPC_Eff");
  hCosTheta_TPC_Eff->Divide(hCosTheta_FV);
  hCosTheta_TPC_Eff->SetTitle("hCosTheta_TPC_Eff");

  TH2D *hCosThetaVsMom_TargetOrTPC_Eff 
    = (TH2D*)hCosThetaVsMom_TargetOrTPC->Clone("hCosThetaVsMom_TargetOrTPC_Eff");
  hCosThetaVsMom_TargetOrTPC_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_TargetOrTPC_Eff->SetTitle("hCosThetaVsMom_TargetOrTPC_Eff");
  TH1D *hMom_TargetOrTPC_Eff = hCosThetaVsMom_TargetOrTPC->ProjectionY()->Clone("hMom_TargetOrTPC_Eff");
  hMom_TargetOrTPC_Eff->Divide(hMom_FV);
  hMom_TargetOrTPC_Eff->SetTitle("hMom_TargetOrTPC_Eff");
  TH1D *hCosTheta_TargetOrTPC_Eff = hCosThetaVsMom_TargetOrTPC->ProjectionX()->Clone("hCosTheta_TargetOrTPC_Eff");
  hCosTheta_TargetOrTPC_Eff->Divide(hCosTheta_FV);
  hCosTheta_TargetOrTPC_Eff->SetTitle("hCosTheta_TargetOrTPC_Eff");
  
  TH2D *hCosThetaVsMom_TargetAndTPC_Eff 
    = (TH2D*)hCosThetaVsMom_TargetAndTPC->Clone("hCosThetaVsMom_TargetAndTPC_Eff");
  hCosThetaVsMom_TargetAndTPC_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_TargetAndTPC_Eff->SetTitle("hCosThetaVsMom_TargetAndTPC_Eff");
  TH1D *hMom_TargetAndTPC_Eff = hCosThetaVsMom_TargetAndTPC->ProjectionY()->Clone("hMom_TargetAndTPC_Eff");
  hMom_TargetAndTPC_Eff->Divide(hMom_FV);
  hMom_TargetAndTPC_Eff->SetTitle("hMom_TargetAndTPC_Eff");
  TH1D *hCosTheta_TargetAndTPC_Eff = hCosThetaVsMom_TargetAndTPC->ProjectionX()->Clone("hCosTheta_TargetAndTPC_Eff");
  hCosTheta_TargetAndTPC_Eff->Divide(hCosTheta_FV);
  hCosTheta_TargetAndTPC_Eff->SetTitle("hCosTheta_TargetAndTPC_Eff");

  //
  // Draw 
  //
  
  TCanvas *cVtx_XY = new TCanvas("cVtx_XY","cVtx_XY");
  hVtx_XY->GetXaxis()->SetTitle("X position (mm)");
  hVtx_XY->GetYaxis()->SetTitle("Y position (mm)");
  hVtx_XY->SetStats(0);
  hVtx_XY->DrawClone("colz");
  cVtx_XY->Print("cVtx_XY.pdf");

  TCanvas *cVtx_XZ = new TCanvas("cVtx_XZ","cVtx_XZ");
  hVtx_XZ->GetXaxis()->SetTitle("X position (mm)");
  hVtx_XZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtx_XZ->SetStats(0);
  hVtx_XZ->DrawClone("colz");  
  cVtx_XZ->Print("cVtx_XZ.pdf");

  TCanvas *cVtx_YZ = new TCanvas("cVtx_YZ","cVtx_YZ");
  hVtx_YZ->GetXaxis()->SetTitle("Y position (mm)");
  hVtx_YZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtx_YZ->SetStats(0);
  hVtx_YZ->DrawClone("colz");
  cVtx_YZ->Print("cVtx_YZ.pdf");

  TCanvas *cVtxOut_XY = new TCanvas("cVtxOut_XY","cVtxOut_XY");
  hVtxOut_XY->GetXaxis()->SetTitle("X position (mm)");
  hVtxOut_XY->GetYaxis()->SetTitle("Y position (mm)");
  hVtxOut_XY->SetStats(0);
  hVtxOut_XY->DrawClone("colz");
  cVtxOut_XY->Print("cVtxOut_XY.pdf");

  TCanvas *cVtxOut_XZ = new TCanvas("cVtxOut_XZ","cVtxOut_XZ");
  hVtxOut_XZ->GetXaxis()->SetTitle("X position (mm)");
  hVtxOut_XZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtxOut_XZ->SetStats(0);
  hVtxOut_XZ->DrawClone("colz");
  cVtxOut_XZ->Print("cVtxOut_XZ.pdf");

  TCanvas *cVtxOut_YZ = new TCanvas("cVtxOut_YZ","cVtxOut_YZ");
  hVtxOut_YZ->GetXaxis()->SetTitle("Y position (mm)");
  hVtxOut_YZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtxOut_YZ->SetStats(0);
  hVtxOut_YZ->DrawClone("colz");
  cVtxOut_YZ->Print("cVtxOut_YZ.pdf");

  TCanvas *cVtx_X = new TCanvas("cVtx_X","cVtx_X");
  hVtx_X->GetXaxis()->SetTitle("X position (mm)");
  hVtx_X->SetStats(0);
  hVtx_X->DrawClone("");
  cVtx_X->Print("cVtx_X.pdf");

  TCanvas *cVtx_Y = new TCanvas("cVtx_Y","cVtx_Y");
  hVtx_Y->GetXaxis()->SetTitle("Y position (mm)");
  hVtx_Y->SetStats(0);
  hVtx_Y->DrawClone("");
  cVtx_Y->Print("cVtx_Y.pdf");

  TCanvas *cVtx_Z = new TCanvas("cVtx_Z","cVtx_Z");
  hVtx_Z->GetXaxis()->SetTitle("Z position (mm)");
  hVtx_Z->SetStats(0);
  hVtx_Z->DrawClone("");
  cVtx_Z->Print("cVtx_Z.pdf");

  TCanvas *cVtxOut_X = new TCanvas("cVtxOut_X","cVtxOut_X");
  hVtxOut_X->GetXaxis()->SetTitle("X position (mm)");
  hVtxOut_X->SetStats(0);
  hVtxOut_X->DrawClone("");
  cVtxOut_X->Print("cVtxOut_X.pdf");

  TCanvas *cVtxOut_Y = new TCanvas("cVtxOut_Y","cVtxOut_Y");
  hVtxOut_Y->GetXaxis()->SetTitle("Y position (mm)");
  hVtxOut_Y->SetStats(0);
  hVtxOut_Y->DrawClone("");
  cVtxOut_Y->Print("cVtxOut_Y.pdf");

  TCanvas *cVtxOut_Z = new TCanvas("cVtxOut_Z","cVtxOut_Z");
  hVtxOut_Z->GetXaxis()->SetTitle("Z position (mm)");
  hVtxOut_Z->SetStats(0);
  hVtxOut_Z->DrawClone("");
  cVtxOut_Z->Print("cVtxOut_Z.pdf");

  TCanvas *cCut = new TCanvas("cCut","cCut");  
  hCut->SetStats(0);
  hCut->DrawClone();
  cCut->Print("cCut.pdf");

  TCanvas *cNuReacFV = new TCanvas("cNuReac","cNuReac");
  hNuReacFV->SetStats(0);  
  hNuReacFV->DrawClone();
  cNuReacFV->Print("cNuReacFV.pdf");

  TCanvas *cNuMom = new TCanvas("cNuMom","cNuMom");
  hNuMom->GetXaxis()->SetTitle("True #nu Momentum (MeV/c)");
  hNuMom->SetStats(0);
  hNuMom->DrawClone();
  cNuMom->Print("cNuMom.pdf");

  TCanvas *cDiffEtrueEvis = new TCanvas("cDiffEtrueEvis","cDiffEtrueEvis");
  hDiffEtrueEvis->GetXaxis()->SetTitle("E^{#nu}_{True} - E_{vis} (MeV)");
  hDiffEtrueEvis->DrawClone("");
  cDiffEtrueEvis->Print("cDiffEtrueEvis.pdf");

  TCanvas *cLTarg1 = new TCanvas("cLTarg1","cLTarg1");
  hL_Targ1_FV->GetXaxis()->SetTitle("Target 1 track Length (mm)");
  hL_Targ1_Cut->SetLineColor(kRed);
  hL_Targ1_FV->SetStats(0); hL_Targ1_Cut->SetStats(0);
  hL_Targ1_FV->DrawClone();
  hL_Targ1_Cut->DrawClone("same");
  cLTarg1->Print("cLTarg1.pdf");

  cout << "hL_Targ1_FV = " << hL_Targ1_FV->GetEntries() << endl;
  cout << "hL_Targ2_FV = " << hL_Targ2_FV->GetEntries() << endl;
  cout << "hL_Targ1_Cut = " << hL_Targ1_Cut->GetEntries() << endl;
  cout << "hL_Targ2_Cut = " << hL_Targ2_Cut->GetEntries() << endl;
  cout << "hCosThetaVsMom_FV = " << hCosThetaVsMom_FV->GetEntries() << endl;
  cout << "hCosThetaVsMom_Target = " << hCosThetaVsMom_Target->GetEntries() << endl;

  TCanvas *cLTarg2 = new TCanvas("cLTarg2","cLTarg2");
  hL_Targ2_FV->GetXaxis()->SetTitle("Target 2 track Length (mm)");
  hL_Targ2_Cut->SetLineColor(kRed);
  hL_Targ2_FV->SetStats(0); hL_Targ2_Cut->SetStats(0);
  hL_Targ2_FV->DrawClone();
  hL_Targ2_Cut->DrawClone("same");
  cLTarg2->Print("cLTarg2.pdf");
    
  TCanvas *cLyz_TPCUp1 = new TCanvas("cLyz_TPCUp1","cLyz_TPCUp1");
  hLyz_TPCUp1->GetXaxis()->SetTitle("TPCUp1 track Length (mm)");
  hLyz_TPCUp1->SetStats(0);
  hLyz_TPCUp1->DrawClone();
  cLyz_TPCUp1->Print("cLyz_TPCUp1.pdf");
  TCanvas *cLyz_TPCUp2 = new TCanvas("cLyz_TPCUp2","cLyz_TPCUp2");
  hLyz_TPCUp2->GetXaxis()->SetTitle("TPCUp2 track Length (mm)");
  hLyz_TPCUp2->SetStats(0);
  hLyz_TPCUp2->DrawClone();
  cLyz_TPCUp2->Print("cLyz_TPCUp2.pdf");
  TCanvas *cLyz_TPCDown1 = new TCanvas("cLyz_TPCDown1","cLyz_TPCDown1");
  hLyz_TPCDown1->GetXaxis()->SetTitle("TPCDown1 track Length (mm)");
  hLyz_TPCDown1->SetStats(0);
  hLyz_TPCDown1->DrawClone();
  cLyz_TPCDown1->Print("cLyz_TPCDown1.pdf");
  TCanvas *cLyz_TPCDown2 = new TCanvas("cLyz_TPCDown2","cLyz_TPCDown2");
  hLyz_TPCDown2->GetXaxis()->SetTitle("TPCDown2 track Length (mm)");
  hLyz_TPCDown2->SetStats(0);
  hLyz_TPCDown2->DrawClone();
  cLyz_TPCDown2->Print("cLyz_TPCDown2.pdf");
  TCanvas *cLyz_ForwTPC1 = new TCanvas("cLyz_ForwTPC1","cLyz_ForwTPC1");
  hLyz_ForwTPC1->GetXaxis()->SetRangeUser(0,600);
  hLyz_ForwTPC1->GetXaxis()->SetTitle("ForwTPC1 track Length (mm)");
  hLyz_ForwTPC1->SetStats(0);
  hLyz_ForwTPC1->DrawClone();
  cLyz_ForwTPC1->Print("cLyz_ForwTPC1.pdf");
  TCanvas *cLyz_ForwTPC2 = new TCanvas("cLyz_ForwTPC2","cLyz_ForwTPC2");
  hLyz_ForwTPC2->GetXaxis()->SetRangeUser(0,600);
  hLyz_ForwTPC2->GetXaxis()->SetTitle("ForwTPC2 track Length (mm)");
  hLyz_ForwTPC2->SetStats(0);
  hLyz_ForwTPC2->DrawClone();
  cLyz_ForwTPC2->Print("cLyz_ForwTPC2.pdf");
  TCanvas *cLyz_ForwTPC3 = new TCanvas("cLyz_ForwTPC3","cLyz_ForwTPC3");
  hLyz_ForwTPC3->GetXaxis()->SetRangeUser(0,600);
  hLyz_ForwTPC3->GetXaxis()->SetTitle("ForwTPC3 track Length (mm)");
  hLyz_ForwTPC3->SetStats(0);
  hLyz_ForwTPC3->DrawClone();
  cLyz_ForwTPC3->Print("cLyz_ForwTPC3.pdf");

  TCanvas *cCosThetaVsMom_Target_Eff = new TCanvas("hCosThetaVsMom_Target_Eff","hCosThetaVsMom_Target_Eff");
  hCosThetaVsMom_Target_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosThetaVsMom_Target_Eff->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hCosThetaVsMom_Target_Eff->SetStats(0);
  hCosThetaVsMom_Target_Eff->DrawClone("colz");
  cCosThetaVsMom_Target_Eff->Print("cCosThetaVsMom_Target_Eff.pdf");
  hCosThetaVsMom_Target_Eff->GetYaxis()->SetRangeUser(0,1000);
  hCosThetaVsMom_Target_Eff->DrawClone("colz");
  cCosThetaVsMom_Target_Eff->Print("cCosThetaVsMom_Target_Eff_ZOOM.pdf");
  TCanvas *cMom_Target_Eff = new TCanvas("hMom_Target_Eff","hMom_Target_Eff");
  hMom_Target_Eff->GetXaxis()->SetRangeUser(0,2000.);
  hMom_Target_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hMom_Target_Eff->GetXaxis()->SetTitle("True Momentum (MeV/c)");
  hMom_Target_Eff->SetStats(0);
  hMom_Target_Eff->DrawClone("");
  cMom_Target_Eff->Print("cMom_Target_Eff.pdf");
  TCanvas *cCosTheta_Target_Eff = new TCanvas("hCosTheta_Target_Eff","hCosTheta_Target_Eff");
  hCosTheta_Target_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hCosTheta_Target_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosTheta_Target_Eff->SetStats(0);
  hCosTheta_Target_Eff->DrawClone("");
  cCosTheta_Target_Eff->Print("cCosTheta_Target_Eff.pdf");

  TCanvas *cCosThetaVsMom_TPC_Eff = new TCanvas("hCosThetaVsMom_TPC_Eff","hCosThetaVsMom_TPC_Eff");
  hCosThetaVsMom_TPC_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosThetaVsMom_TPC_Eff->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hCosThetaVsMom_TPC_Eff->SetStats(0);
  hCosThetaVsMom_TPC_Eff->DrawClone("colz");
  cCosThetaVsMom_TPC_Eff->Print("cCosThetaVsMom_TPC_Eff.pdf");
  hCosThetaVsMom_TPC_Eff->GetYaxis()->SetRangeUser(0,1000);
  hCosThetaVsMom_TPC_Eff->DrawClone("colz");
  cCosThetaVsMom_TPC_Eff->Print("cCosThetaVsMom_TPC_Eff_ZOOM.pdf");
  TCanvas *cMom_TPC_Eff = new TCanvas("hMom_TPC_Eff","hMom_TPC_Eff");
  hMom_TPC_Eff->GetXaxis()->SetRangeUser(0,2000.);
  hMom_TPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hMom_TPC_Eff->GetXaxis()->SetTitle("True Momentum (MeV/c)");
  hMom_TPC_Eff->SetStats(0);
  hMom_TPC_Eff->DrawClone("");
  cMom_TPC_Eff->Print("cMom_TPC_Eff.pdf");
  TCanvas *cCosTheta_TPC_Eff = new TCanvas("hCosTheta_TPC_Eff","hCosTheta_TPC_Eff");
  hCosTheta_TPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hCosTheta_TPC_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosTheta_TPC_Eff->SetStats(0);
  hCosTheta_TPC_Eff->DrawClone("");
  cCosTheta_TPC_Eff->Print("cCosTheta_TPC_Eff.pdf");

  TCanvas *cCosThetaVsMom_TargetOrTPC_Eff = new TCanvas("hCosThetaVsMom_TargetOrTPC_Eff","hCosThetaVsMom_TargetOrTPC_Eff");
  hCosThetaVsMom_TargetOrTPC_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosThetaVsMom_TargetOrTPC_Eff->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hCosThetaVsMom_TargetOrTPC_Eff->SetStats(0);
  hCosThetaVsMom_TargetOrTPC_Eff->DrawClone("colz");
  cCosThetaVsMom_TargetOrTPC_Eff->Print("cCosThetaVsMom_TargetOrTPC_Eff.pdf");
  hCosThetaVsMom_TargetOrTPC_Eff->GetYaxis()->SetRangeUser(0,1000);
  hCosThetaVsMom_TargetOrTPC_Eff->DrawClone("colz");
  cCosThetaVsMom_TargetOrTPC_Eff->Print("cCosThetaVsMom_TargetOrTPC_Eff_ZOOM.pdf");
  TCanvas *cMom_TargetOrTPC_Eff = new TCanvas("hMom_TargetOrTPC_Eff","hMom_TargetOrTPC_Eff");
  hMom_TargetOrTPC_Eff->GetXaxis()->SetRangeUser(0,2000.);
  hMom_TargetOrTPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hMom_TargetOrTPC_Eff->GetXaxis()->SetTitle("True Momentum (MeV/c)");
  hMom_TargetOrTPC_Eff->SetStats(0);
  hMom_TargetOrTPC_Eff->DrawClone("");
  cMom_TargetOrTPC_Eff->Print("cMom_TargetOrTPC_Eff.pdf");
  TCanvas *cCosTheta_TargetOrTPC_Eff = new TCanvas("hCosTheta_TargetOrTPC_Eff","hCosTheta_TargetOrTPC_Eff");
  hCosTheta_TargetOrTPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hCosTheta_TargetOrTPC_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosTheta_TargetOrTPC_Eff->SetStats(0);
  hCosTheta_TargetOrTPC_Eff->DrawClone("");
  cCosTheta_TargetOrTPC_Eff->Print("cCosTheta_TargetOrTPC_Eff.pdf");

  TCanvas *cCosThetaVsMom_TargetAndTPC_Eff = new TCanvas("hCosThetaVsMom_TargetAndTPC_Eff","hCosThetaVsMom_TargetAndTPC_Eff");
  hCosThetaVsMom_TargetAndTPC_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosThetaVsMom_TargetAndTPC_Eff->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hCosThetaVsMom_TargetAndTPC_Eff->SetStats(0);
  hCosThetaVsMom_TargetAndTPC_Eff->DrawClone("colz");
  cCosThetaVsMom_TargetAndTPC_Eff->Print("cCosThetaVsMom_TargetAndTPC_Eff.pdf");
  hCosThetaVsMom_TargetAndTPC_Eff->GetYaxis()->SetRangeUser(0,1000);
  hCosThetaVsMom_TargetAndTPC_Eff->DrawClone("colz");
  cCosThetaVsMom_TargetAndTPC_Eff->Print("cCosThetaVsMom_TargetAndTPC_Eff_ZOOM.pdf");
  TCanvas *cMom_TargetAndTPC_Eff = new TCanvas("hMom_TargetAndTPC_Eff","hMom_TargetAndTPC_Eff");
  hMom_TargetAndTPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hMom_TargetAndTPC_Eff->GetXaxis()->SetTitle("True Momentum (MeV/c)");
  hMom_TargetAndTPC_Eff->SetStats(0);
  hMom_TargetAndTPC_Eff->DrawClone("");
  cMom_TargetAndTPC_Eff->Print("cMom_TargetAndTPC_Eff.pdf");
  TCanvas *cCosTheta_TargetAndTPC_Eff = new TCanvas("hCosTheta_TargetAndTPC_Eff","hCosTheta_TargetAndTPC_Eff");
  hCosTheta_TargetAndTPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hCosTheta_TargetAndTPC_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosTheta_TargetAndTPC_Eff->SetStats(0);
  hCosTheta_TargetAndTPC_Eff->DrawClone("");
  cCosTheta_TargetAndTPC_Eff->Print("cCosTheta_TargetAndTPC_Eff.pdf");

  TCanvas *cCosThetaVsMom_FV = new TCanvas("cCosThetaVsMom_FV","cCosThetaVsMom_FV");
  hCosThetaVsMom_FV->GetXaxis()->SetTitle("True cos #theta");
  hCosThetaVsMom_FV->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hCosThetaVsMom_FV->SetStats(0);
  hCosThetaVsMom_FV->DrawClone("colz");
  cCosThetaVsMom_FV->Print("cCosThetaVsMom_FV.pdf");
  hCosThetaVsMom_FV->GetYaxis()->SetRangeUser(0,1000);
  hCosThetaVsMom_FV->DrawClone("colz");
  cCosThetaVsMom_FV->Print("cCosThetaVsMom_FV_ZOOM.pdf");
  TCanvas *cMom_FV = new TCanvas("cMom_FV","cMom_FV");
  hMom_FV->GetXaxis()->SetTitle("True Track Momentum (MeV/c)");
  hMom_FV->GetXaxis()->SetRangeUser(0,1000);
  hMom_FV->SetStats(0);
  hMom_FV->DrawClone();
  cMom_FV->Print("cMom_FV.pdf");
  TCanvas *cCosTheta_FV = new TCanvas("cCosTheta_FV","cCosTheta_FV");
  hCosTheta_FV->GetXaxis()->SetTitle("True Track cos #theta");
  hCosTheta_FV->SetStats(0);
  hCosTheta_FV->DrawClone();
  cCosTheta_FV->Print("cCosTheta_FV.pdf");

  TCanvas *cPDG = new TCanvas("cPDG","cPDG");
  hPDG->GetXaxis()->SetTitle("Track PDG");
  hPDG->DrawClone();
  cPDG->Print("cPDG");

  return;
}
