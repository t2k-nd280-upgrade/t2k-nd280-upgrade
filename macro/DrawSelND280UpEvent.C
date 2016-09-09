
void DrawSelND280UpEvent
( 
 bool doPrint = false,

 string infilename = "../../../files/Efficiencies/Studies_8_9_16/SelND280Up_NuMu_6E20-Targ1-MuM-ParID0_All_60cm.root"
 //string infilename = "../../../files/Efficiencies/Studies_8_9_16/SelND280Up_NuMu_6E20_30cm-Targ1-MuM-ParID0_All.root"

 //string infilename = "../../../files/SelND280Up_NuMu_6E20-Targ1-MuM-ParID0_All_7_9_16.root"
 //string infilename = "../../../files/SelND280Up_NuMu_6E20-Targ2-MuM-ParID0_All_7_9_16.root"
 
 //string infilename = "../../../files/"
 //string infilename = "../../../files/"
 
 //string infilename = "../../../files/"
 //string infilename = "../../../files/"
 
 //string infilename = "../../../files/"
 //string infilename = "../../../files/"
 
 //string infilename = "../../../files/"
 )
{
  gROOT->SetStyle("T2K");                  
  gROOT->ForceStyle();                 
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadRightMargin(0.15);
  //gStyle->SetStatX(.4); gStyle->SetStatY(.6);

  TFile *infile = new TFile(infilename.c_str(),"READ");
  
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
  // For efficiency
  TH2D *hCosThetaVsMom_FV = (TH2D*)infile->Get("hCosThetaVsMom_FV"); 
  TH2D *hCosThetaVsMom_Target = (TH2D*)infile->Get("hCosThetaVsMom_Target");       
  TH2D *hCosThetaVsMom_TPC = (TH2D*)infile->Get("hCosThetaVsMom_TPC");          
  TH2D *hCosThetaVsMom_TargetOrTPC = (TH2D*)infile->Get("hCosThetaVsMom_TargetOrTPC"); 
  TH2D *hCosThetaVsMom_TargetAndTPC = (TH2D*)infile->Get("hCosThetaVsMom_TargetAndTPC"); 
  TH2D *hPhiVsMom_FV = (TH2D*)infile->Get("hPhiVsMom_FV"); 
  TH2D *hPhiVsMom_Target = (TH2D*)infile->Get("hPhiVsMom_Target");       
  TH2D *hPhiVsMom_TPC = (TH2D*)infile->Get("hPhiVsMom_TPC");          
  TH2D *hPhiVsMom_TargetOrTPC = (TH2D*)infile->Get("hPhiVsMom_TargetOrTPC"); 
  TH2D *hPhiVsMom_TargetAndTPC = (TH2D*)infile->Get("hPhiVsMom_TargetAndTPC"); 
  TH2D *hThetaVsPhi_FV = (TH2D*)infile->Get("hThetaVsPhi_FV"); 
  TH2D *hThetaVsPhi_Target = (TH2D*)infile->Get("hThetaVsPhi_Target");       
  TH2D *hThetaVsPhi_TPC = (TH2D*)infile->Get("hThetaVsPhi_TPC");          
  TH2D *hThetaVsPhi_TargetOrTPC = (TH2D*)infile->Get("hThetaVsPhi_TargetOrTPC"); 
  TH2D *hThetaVsPhi_TargetAndTPC = (TH2D*)infile->Get("hThetaVsPhi_TargetAndTPC"); 
  // Neutrino 
  TH1D *hNuReacFV = (TH1D*)infile->Get("hNuReacFV");
  TH1D *hNuMom = (TH1D*)infile->Get("hNuMom");
  TH1D *hNuPDG = (TH1D*)infile->Get("hNuPDG");
  TH1D *hEvis = (TH1D*)infile->Get("hEvis");
  TH1D *hDiffEtrueEvis = (TH1D*)infile->Get("hDiffEtrueEvis");
  TH2D *hEtrueVsEvis = (TH2D*)infile->Get("hEtrueVsEvis");
  //TH1D *hEreco = (TH1D*)infile->Get("hEreco");
  //TH1D *hDiffEtrueEreco = (TH1D*)infile->Get("hDiffEtrueEreco");
  //TH2D *hEtrueVsEreco = (TH2D*)infile->Get("hEtrueVsEreco");
  // Track
  TH1D *hMom = (TH1D*)infile->Get("hMom");
  TH1D *hPDG = (TH1D*)infile->Get("hPDG");
  TH1D *hCut = (TH1D*)infile->Get("hCut");
  TH1D *hPhi = (TH1D*)infile->Get("hPhi_FV"); 
  TH1D *hTheta = (TH1D*)infile->Get("hTheta_FV"); 
  // Last Pt Track
  TH2D *hLastPt_Target_PhiVsZ = (TH2D*)infile->Get("hLastPt_Target_PhiVsZ");
  TH2D *hLastPt_Target_ThetaVsZ = (TH2D*)infile->Get("hLastPt_Target_ThetaVsZ");
  TH2D *hLastPt_Target_Z = (TH2D*)infile->Get("hLastPt_Target_Z");
  TH2D *hLastPt_Target_XY = (TH2D*)infile->Get("hLastPt_Target_XY");
  TH2D *hLastPt_Target_YZ = (TH2D*)infile->Get("hLastPt_Target_YZ");
  TH2D *hLastPt_Target_XZ = (TH2D*)infile->Get("hLastPt_Target_XZ");
  TH2D *hLastPt_Oth_PhiVsZ = (TH2D*)infile->Get("hLastPt_Oth_PhiVsZ");
  TH2D *hLastPt_Oth_ThetaVsZ = (TH2D*)infile->Get("hLastPt_Oth_ThetaVsZ");
  TH2D *hLastPt_Oth_Z = (TH2D*)infile->Get("hLastPt_Oth_Z");
  TH2D *hLastPt_Oth_XY = (TH2D*)infile->Get("hLastPt_Oth_XY");
  TH2D *hLastPt_Oth_YZ = (TH2D*)infile->Get("hLastPt_Oth_YZ");
  TH2D *hLastPt_Oth_XZ = (TH2D*)infile->Get("hLastPt_Oth_XZ");
  // Track Length
  TH1D *hL_SD = (TH1D*)infile->Get("hL_SD");
  TH2D *hLVsMom_SD = (TH2D*)infile->Get("hLVsMom_SD");
  //
  TH1D *hL_Targ1_FV = (TH1D*)infile->Get("hL_Targ1_FV");
  TH1D *hL_Targ2_FV = (TH1D*)infile->Get("hL_Targ2_FV");
  TH1D *hL_TPCUp1_FV = (TH1D*)infile->Get("hL_TPCUp1_FV");
  TH1D *hL_TPCUp2_FV = (TH1D*)infile->Get("hL_TPCUp2_FV");
  TH1D *hL_TPCDown1_FV = (TH1D*)infile->Get("hL_TPCDown1_FV");
  TH1D *hL_TPCDown2_FV = (TH1D*)infile->Get("hL_TPCDown2_FV");
  TH1D *hL_ForwTPC1_FV = (TH1D*)infile->Get("hL_ForwTPC1_FV");
  TH1D *hL_ForwTPC2_FV = (TH1D*)infile->Get("hL_ForwTPC2_FV");
  TH1D *hL_ForwTPC3_FV = (TH1D*)infile->Get("hL_ForwTPC3_FV");
  //
  TH1D *hL_Targ1_Cut = (TH1D*)infile->Get("hL_Targ1_Cut");
  TH1D *hL_Targ2_Cut = (TH1D*)infile->Get("hL_Targ2_Cut");
  TH1D *hL_TPCUp1_Cut = (TH1D*)infile->Get("hL_TPCUp1_Cut");
  TH1D *hL_TPCUp2_Cut = (TH1D*)infile->Get("hL_TPCUp2_Cut");
  TH1D *hL_TPCDown1_Cut = (TH1D*)infile->Get("hL_TPCDown1_Cut");
  TH1D *hL_TPCDown2_Cut = (TH1D*)infile->Get("hL_TPCDown2_Cut");
  TH1D *hL_ForwTPC1_Cut = (TH1D*)infile->Get("hL_ForwTPC1_Cut");
  TH1D *hL_ForwTPC2_Cut = (TH1D*)infile->Get("hL_ForwTPC2_Cut");
  TH1D *hL_ForwTPC3_Cut = (TH1D*)infile->Get("hL_ForwTPC3_Cut");
  //
  TH2D *hLVsMom_Targ1_FV = (TH2D*)infile->Get("hLVsMom_Targ1_FV");
  TH2D *hLVsMom_Targ2_FV = (TH2D*)infile->Get("hLVsMom_Targ2_FV");
  TH2D *hLVsMom_TPCUp1_FV = (TH2D*)infile->Get("hLVsMom_Targ2_FV");
  TH2D *hLVsMom_TPCUp2_FV = (TH2D*)infile->Get("hLVsMom_Targ2_FV");
  TH2D *hLVsMom_TPCDown1_FV = (TH2D*)infile->Get("hLVsMom_TPCDown1_FV");
  TH2D *hLVsMom_TPCDown2_FV = (TH2D*)infile->Get("hLVsMom_TPCDown2_FV");
  TH2D *hLVsMom_ForwTPC1_FV = (TH2D*)infile->Get("hLVsMom_ForwTPC1_FV");
  TH2D *hLVsMom_ForwTPC2_FV = (TH2D*)infile->Get("hLVsMom_ForwTPC2_FV");
  TH2D *hLVsMom_ForwTPC3_FV = (TH2D*)infile->Get("hLVsMom_ForwTPC3_FV");
  //
  TH2D *hLVsMom_Targ1_Cut = (TH2D*)infile->Get("hLVsMom_Targ1_Cut");
  TH2D *hLVsMom_Targ2_Cut = (TH2D*)infile->Get("hLVsMom_Targ2_Cut");
  TH2D *hLVsMom_TPCUp1_Cut = (TH2D*)infile->Get("hLVsMom_Targ2_Cut");
  TH2D *hLVsMom_TPCUp2_Cut = (TH2D*)infile->Get("hLVsMom_Targ2_Cut");
  TH2D *hLVsMom_TPCDown1_Cut = (TH2D*)infile->Get("hLVsMom_TPCDown1_Cut");
  TH2D *hLVsMom_TPCDown2_Cut = (TH2D*)infile->Get("hLVsMom_TPCDown2_Cut");
  TH2D *hLVsMom_ForwTPC1_Cut = (TH2D*)infile->Get("hLVsMom_ForwTPC1_Cut");
  TH2D *hLVsMom_ForwTPC2_Cut = (TH2D*)infile->Get("hLVsMom_ForwTPC2_Cut");
  TH2D *hLVsMom_ForwTPC3_Cut = (TH2D*)infile->Get("hLVsMom_ForwTPC3_Cut");
  //
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
  TH2D *hLastPt_Target_PhiVsZ = (TH2D*)infile->Get("hLastPt_Target_PhiVsZ");
  TH2D *hLastPt_Target_ThetaVsZ = (TH2D*)infile->Get("hLastPt_Target_ThetaVsZ");
  TH2D *hLastPt_Target_Z = (TH2D*)infile->Get("hLastPt_Target_Z");
  TH2D *hLastPt_Target_XY = (TH2D*)infile->Get("hLastPt_Target_XY");
  TH2D *hLastPt_Target_YZ = (TH2D*)infile->Get("hLastPt_Target_YZ");
  TH2D *hLastPt_Target_XZ = (TH2D*)infile->Get("hLastPt_Target_XZ");

  TH2D *hLastPt_Oth_PhiVsZ = (TH2D*)infile->Get("hLastPt_Oth_PhiVsZ");
  TH2D *hLastPt_Oth_ThetaVsZ = (TH2D*)infile->Get("hLastPt_Oth_ThetaVsZ");
  TH2D *hLastPt_Oth_Z = (TH2D*)infile->Get("hLastPt_Oth_Z");
  TH2D *hLastPt_Oth_XY = (TH2D*)infile->Get("hLastPt_Oth_XY");
  TH2D *hLastPt_Oth_YZ = (TH2D*)infile->Get("hLastPt_Oth_YZ");
  TH2D *hLastPt_Oth_XZ = (TH2D*)infile->Get("hLastPt_Oth_XZ");
  
  // Get efficiencies
  TH1D *hMom_FV = hCosThetaVsMom_FV->ProjectionY()->Clone("hMom_FV");
  TH1D *hCosTheta_FV = hCosThetaVsMom_FV->ProjectionX()->Clone("hCosTheta_FV");
  TH1D *hPhi_FV = hPhiVsMom_FV->ProjectionX()->Clone("hPhi_FV");
  
  TH2D *hCosThetaVsMom_Target_Eff 
    = (TH2D*)hCosThetaVsMom_Target->Clone("hCosThetaVsMom_Target_Eff");
  hCosThetaVsMom_Target_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_Target_Eff->SetTitle("hCosThetaVsMom_Target_Eff");
  TH2D *hPhiVsMom_Target_Eff 
    = (TH2D*)hPhiVsMom_Target->Clone("hPhiVsMom_Target_Eff");
  hPhiVsMom_Target_Eff->Divide(hPhiVsMom_FV);
  hPhiVsMom_Target_Eff->SetTitle("hPhiVsMom_Target_Eff");
 TH2D *hThetaVsPhi_Target_Eff 
    = (TH2D*)hThetaVsPhi_Target->Clone("hThetaVsPhi_Target_Eff");
  hThetaVsPhi_Target_Eff->Divide(hThetaVsPhi_FV);
  hThetaVsPhi_Target_Eff->SetTitle("hThetaVsPhi_Target_Eff");  
  TH1D *hMom_Target_Eff = hCosThetaVsMom_Target->ProjectionY()->Clone("hMom_Target_Eff");
  hMom_Target_Eff->Divide(hMom_FV);
  hMom_Target_Eff->SetTitle("hMom_Target_Eff");
  TH1D *hCosTheta_Target_Eff = hCosThetaVsMom_Target->ProjectionX()->Clone("hCosTheta_Target_Eff");
  hCosTheta_Target_Eff->Divide(hCosTheta_FV);
  hCosTheta_Target_Eff->SetTitle("hCosTheta_Target_Eff");
  TH1D *hPhi_Target_Eff = hPhiVsMom_Target->ProjectionX()->Clone("hPhi_Target_Eff");
  hPhi_Target_Eff->Divide(hPhi_FV);
  hPhi_Target_Eff->SetTitle("hPhi_Target_Eff");

  TH2D *hCosThetaVsMom_TPC_Eff 
    = (TH2D*)hCosThetaVsMom_TPC->Clone("hCosThetaVsMom_TPC_Eff");
  hCosThetaVsMom_TPC_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_TPC_Eff->SetTitle("hCosThetaVsMom_TPC_Eff");
  TH2D *hPhiVsMom_TPC_Eff 
    = (TH2D*)hPhiVsMom_TPC->Clone("hPhiVsMom_TPC_Eff");
  hPhiVsMom_TPC_Eff->Divide(hPhiVsMom_FV);
  hPhiVsMom_TPC_Eff->SetTitle("hPhiVsMom_TPC_Eff");
  TH2D *hThetaVsPhi_TPC_Eff 
    = (TH2D*)hThetaVsPhi_TPC->Clone("hThetaVsPhi_TPC_Eff");
  hThetaVsPhi_TPC_Eff->Divide(hThetaVsPhi_FV);
  hThetaVsPhi_TPC_Eff->SetTitle("hThetaVsPhi_TPC_Eff");
  TH1D *hMom_TPC_Eff = hCosThetaVsMom_TPC->ProjectionY()->Clone("hMom_TPC_Eff");
  hMom_TPC_Eff->Divide(hMom_FV);
  hMom_TPC_Eff->SetTitle("hMom_TPC_Eff");
  TH1D *hCosTheta_TPC_Eff = hCosThetaVsMom_TPC->ProjectionX()->Clone("hCosTheta_TPC_Eff");
  hCosTheta_TPC_Eff->Divide(hCosTheta_FV);
  hCosTheta_TPC_Eff->SetTitle("hCosTheta_TPC_Eff");
  TH1D *hPhi_TPC_Eff = hPhiVsMom_TPC->ProjectionX()->Clone("hPhi_TPC_Eff");
  hPhi_TPC_Eff->Divide(hPhi_FV);
  hPhi_TPC_Eff->SetTitle("hPhi_TPC_Eff");

  TH2D *hCosThetaVsMom_TargetOrTPC_Eff 
    = (TH2D*)hCosThetaVsMom_TargetOrTPC->Clone("hCosThetaVsMom_TargetOrTPC_Eff");
  hCosThetaVsMom_TargetOrTPC_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_TargetOrTPC_Eff->SetTitle("hCosThetaVsMom_TargetOrTPC_Eff");
  TH2D *hPhiVsMom_TargetOrTPC_Eff 
    = (TH2D*)hPhiVsMom_TargetOrTPC->Clone("hPhiVsMom_TargetOrTPC_Eff");
  hPhiVsMom_TargetOrTPC_Eff->Divide(hPhiVsMom_FV);
  hPhiVsMom_TargetOrTPC_Eff->SetTitle("hPhiVsMom_TargetOrTPC_Eff");
 TH2D *hThetaVsPhi_TargetOrTPC_Eff 
    = (TH2D*)hThetaVsPhi_TargetOrTPC->Clone("hThetaVsPhi_TargetOrTPC_Eff");
  hThetaVsPhi_TargetOrTPC_Eff->Divide(hThetaVsPhi_FV);
  hThetaVsPhi_TargetOrTPC_Eff->SetTitle("hThetaVsPhi_TargetOrTPC_Eff");
  TH1D *hMom_TargetOrTPC_Eff = hCosThetaVsMom_TargetOrTPC->ProjectionY()->Clone("hMom_TargetOrTPC_Eff");
  hMom_TargetOrTPC_Eff->Divide(hMom_FV);
  hMom_TargetOrTPC_Eff->SetTitle("hMom_TargetOrTPC_Eff");
  TH1D *hCosTheta_TargetOrTPC_Eff = hCosThetaVsMom_TargetOrTPC->ProjectionX()->Clone("hCosTheta_TargetOrTPC_Eff");
  hCosTheta_TargetOrTPC_Eff->Divide(hCosTheta_FV);
  hCosTheta_TargetOrTPC_Eff->SetTitle("hCosTheta_TargetOrTPC_Eff");
  TH1D *hPhi_TargetOrTPC_Eff = hPhiVsMom_TargetOrTPC->ProjectionX()->Clone("hPhi_TargetOrTPC_Eff");
  hPhi_TargetOrTPC_Eff->Divide(hPhi_FV);
  hPhi_TargetOrTPC_Eff->SetTitle("hPhi_TargetOrTPC_Eff");
  
  TH2D *hCosThetaVsMom_TargetAndTPC_Eff 
    = (TH2D*)hCosThetaVsMom_TargetAndTPC->Clone("hCosThetaVsMom_TargetAndTPC_Eff");
  hCosThetaVsMom_TargetAndTPC_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_TargetAndTPC_Eff->SetTitle("hCosThetaVsMom_TargetAndTPC_Eff");
  TH2D *hPhiVsMom_TargetAndTPC_Eff 
    = (TH2D*)hPhiVsMom_TargetAndTPC->Clone("hPhiVsMom_TargetAndTPC_Eff");
  hPhiVsMom_TargetAndTPC_Eff->Divide(hPhiVsMom_FV);
  hPhiVsMom_TargetAndTPC_Eff->SetTitle("hPhiVsMom_TargetAndTPC_Eff");
 TH2D *hThetaVsPhi_TargetAndTPC_Eff 
    = (TH2D*)hThetaVsPhi_TargetAndTPC->Clone("hThetaVsPhi_TargetAndTPC_Eff");
  hThetaVsPhi_TargetAndTPC_Eff->Divide(hThetaVsPhi_FV);
  hThetaVsPhi_TargetAndTPC_Eff->SetTitle("hThetaVsPhi_TargetAndTPC_Eff");
  TH1D *hMom_TargetAndTPC_Eff = hCosThetaVsMom_TargetAndTPC->ProjectionY()->Clone("hMom_TargetAndTPC_Eff");
  hMom_TargetAndTPC_Eff->Divide(hMom_FV);
  hMom_TargetAndTPC_Eff->SetTitle("hMom_TargetAndTPC_Eff");
  TH1D *hCosTheta_TargetAndTPC_Eff = hCosThetaVsMom_TargetAndTPC->ProjectionX()->Clone("hCosTheta_TargetAndTPC_Eff");
  hCosTheta_TargetAndTPC_Eff->Divide(hCosTheta_FV);
  hCosTheta_TargetAndTPC_Eff->SetTitle("hCosTheta_TargetAndTPC_Eff");
  TH1D *hPhi_TargetAndTPC_Eff = hPhiVsMom_TargetAndTPC->ProjectionX()->Clone("hPhi_TargetAndTPC_Eff");
  hPhi_TargetAndTPC_Eff->Divide(hPhi_FV);
  hPhi_TargetAndTPC_Eff->SetTitle("hPhi_TargetAndTPC_Eff");
  
  //
  // Draw 
  //
  
  TCanvas *cVtx_XY = new TCanvas("cVtx_XY","cVtx_XY");
  hVtx_XY->GetXaxis()->SetTitle("X position (mm)");
  hVtx_XY->GetYaxis()->SetTitle("Y position (mm)");
  hVtx_XY->SetStats(0);
  hVtx_XY->DrawClone("colz");
  if(doPrint) cVtx_XY->Print("cVtx_XY.pdf");
  TCanvas *cVtx_XZ = new TCanvas("cVtx_XZ","cVtx_XZ");
  hVtx_XZ->GetXaxis()->SetTitle("X position (mm)");
  hVtx_XZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtx_XZ->SetStats(0);
  hVtx_XZ->DrawClone("colz");  
  if(doPrint) cVtx_XZ->Print("cVtx_XZ.pdf");
  TCanvas *cVtx_YZ = new TCanvas("cVtx_YZ","cVtx_YZ");
  hVtx_YZ->GetXaxis()->SetTitle("Y position (mm)");
  hVtx_YZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtx_YZ->SetStats(0);
  hVtx_YZ->DrawClone("colz");
  if(doPrint) cVtx_YZ->Print("cVtx_YZ.pdf");
  TCanvas *cVtxOut_XY = new TCanvas("cVtxOut_XY","cVtxOut_XY");
  hVtxOut_XY->GetXaxis()->SetTitle("X position (mm)");
  hVtxOut_XY->GetYaxis()->SetTitle("Y position (mm)");
  hVtxOut_XY->SetStats(0);
  hVtxOut_XY->DrawClone("colz");
  if(doPrint) cVtxOut_XY->Print("cVtxOut_XY.pdf");
  TCanvas *cVtxOut_XZ = new TCanvas("cVtxOut_XZ","cVtxOut_XZ");
  hVtxOut_XZ->GetXaxis()->SetTitle("X position (mm)");
  hVtxOut_XZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtxOut_XZ->SetStats(0);
  hVtxOut_XZ->DrawClone("colz");
  if(doPrint) cVtxOut_XZ->Print("cVtxOut_XZ.pdf");
  TCanvas *cVtxOut_YZ = new TCanvas("cVtxOut_YZ","cVtxOut_YZ");
  hVtxOut_YZ->GetXaxis()->SetTitle("Y position (mm)");
  hVtxOut_YZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtxOut_YZ->SetStats(0);
  hVtxOut_YZ->DrawClone("colz");
  if(doPrint) cVtxOut_YZ->Print("cVtxOut_YZ.pdf");
  TCanvas *cVtx_X = new TCanvas("cVtx_X","cVtx_X");
  hVtx_X->GetXaxis()->SetTitle("X position (mm)");
  hVtx_X->SetStats(0);
  hVtx_X->DrawClone("");
  if(doPrint) cVtx_X->Print("cVtx_X.pdf");
  TCanvas *cVtx_Y = new TCanvas("cVtx_Y","cVtx_Y");
  hVtx_Y->GetXaxis()->SetTitle("Y position (mm)");
  hVtx_Y->SetStats(0);
  hVtx_Y->DrawClone("");
  if(doPrint) cVtx_Y->Print("cVtx_Y.pdf");
  TCanvas *cVtx_Z = new TCanvas("cVtx_Z","cVtx_Z");
  hVtx_Z->GetXaxis()->SetTitle("Z position (mm)");
  hVtx_Z->SetStats(0);
  hVtx_Z->DrawClone("");
  if(doPrint) cVtx_Z->Print("cVtx_Z.pdf");
  TCanvas *cVtxOut_X = new TCanvas("cVtxOut_X","cVtxOut_X");
  hVtxOut_X->GetXaxis()->SetTitle("X position (mm)");
  hVtxOut_X->SetStats(0);
  hVtxOut_X->DrawClone("");
  if(doPrint) cVtxOut_X->Print("cVtxOut_X.pdf");
  TCanvas *cVtxOut_Y = new TCanvas("cVtxOut_Y","cVtxOut_Y");
  hVtxOut_Y->GetXaxis()->SetTitle("Y position (mm)");
  hVtxOut_Y->SetStats(0);
  hVtxOut_Y->DrawClone("");
  if(doPrint) cVtxOut_Y->Print("cVtxOut_Y.pdf");
  TCanvas *cVtxOut_Z = new TCanvas("cVtxOut_Z","cVtxOut_Z");
  hVtxOut_Z->GetXaxis()->SetTitle("Z position (mm)");
  hVtxOut_Z->SetStats(0);
  hVtxOut_Z->DrawClone("");
  if(doPrint) cVtxOut_Z->Print("cVtxOut_Z.pdf");
  
  TCanvas *cCut = new TCanvas("cCut","cCut");  
  hCut->SetStats(0);
  hCut->DrawClone();
  if(doPrint) cCut->Print("cCut.pdf");

  TCanvas *cNuReacFV = new TCanvas("cNuReac","cNuReac");
  hNuReacFV->SetStats(0);  
  hNuReacFV->DrawClone();
  if(doPrint) cNuReacFV->Print("cNuReacFV.pdf");

  TCanvas *cNuMom = new TCanvas("cNuMom","cNuMom");
  hNuMom->GetXaxis()->SetTitle("True #nu Momentum (MeV/c)");
  hNuMom->SetStats(0);
  hNuMom->DrawClone();
  if(doPrint) cNuMom->Print("cNuMom.pdf");

  TCanvas *cDiffEtrueEvis = new TCanvas("cDiffEtrueEvis","cDiffEtrueEvis");
  hDiffEtrueEvis->GetXaxis()->SetTitle("E^{#nu}_{True} - E_{vis} (MeV)");
  hDiffEtrueEvis->DrawClone("");
  if(doPrint) cDiffEtrueEvis->Print("cDiffEtrueEvis.pdf");

  TCanvas *cLTarg1 = new TCanvas("cLTarg1","cLTarg1");
  hL_Targ1_FV->GetXaxis()->SetTitle("Target 1 track Length (mm)");
  hL_Targ1_Cut->SetLineColor(kRed);
  hL_Targ1_FV->SetStats(0); hL_Targ1_Cut->SetStats(0);
  hL_Targ1_FV->DrawClone();
  hL_Targ1_Cut->DrawClone("same");
  if(doPrint) cLTarg1->Print("cLTarg1.pdf");
  TCanvas *cLTarg2 = new TCanvas("cLTarg2","cLTarg2");
  hL_Targ2_FV->GetXaxis()->SetTitle("Target 2 track Length (mm)");
  hL_Targ2_Cut->SetLineColor(kRed);
  hL_Targ2_FV->SetStats(0); hL_Targ2_Cut->SetStats(0);
  hL_Targ2_FV->DrawClone();
  hL_Targ2_Cut->DrawClone("same");
  if(doPrint) cLTarg2->Print("cLTarg2.pdf");
  TCanvas *cL_TPCUp1 = new TCanvas("cL_TPCUp1","cL_TPCUp1");
  hL_TPCUp1_FV->GetXaxis()->SetTitle("TPCUp1 track Length (mm)");
  hL_TPCUp1_FV->SetStats(0);
  hL_TPCUp1_FV->DrawClone();
  if(doPrint) cL_TPCUp1->Print("cL_TPCUp1.pdf");
  TCanvas *cL_TPCUp2 = new TCanvas("cL_TPCUp2","cL_TPCUp2");
  hL_TPCUp2_FV->GetXaxis()->SetTitle("TPCUp2 track Length (mm)");
  hL_TPCUp2_FV->SetStats(0);
  hL_TPCUp2_FV->DrawClone();
  if(doPrint) cL_TPCUp2->Print("cL_TPCUp2.pdf");
  TCanvas *cL_TPCDown1 = new TCanvas("cL_TPCDown1","cL_TPCDown1");
  hL_TPCDown1_FV->GetXaxis()->SetTitle("TPCDown1 track Length (mm)");
  hL_TPCDown1_FV->SetStats(0);
  hL_TPCDown1_FV->DrawClone();
  if(doPrint) cL_TPCDown1->Print("cL_TPCDown1.pdf");
  TCanvas *cL_TPCDown2 = new TCanvas("cL_TPCDown2","cL_TPCDown2");
  hL_TPCDown2_FV->GetXaxis()->SetTitle("TPCDown2 track Length (mm)");
  hL_TPCDown2_FV->SetStats(0);
  hL_TPCDown2_FV->DrawClone();
  if(doPrint) cL_TPCDown2->Print("cL_TPCDown2.pdf");
  TCanvas *cL_ForwTPC1 = new TCanvas("cL_ForwTPC1","cL_ForwTPC1");
  hL_ForwTPC1_FV->GetXaxis()->SetTitle("ForwTPC1 track Length (mm)");
  hL_ForwTPC1_FV->SetStats(0);
  hL_ForwTPC1_FV->DrawClone();
  if(doPrint) cL_ForwTPC1->Print("cL_ForwTPC1.pdf");
  TCanvas *cL_ForwTPC2 = new TCanvas("cL_ForwTPC2","cL_ForwTPC2");
  hL_ForwTPC2_FV->GetXaxis()->SetTitle("ForwTPC2 track Length (mm)");
  hL_ForwTPC2_FV->SetStats(0);
  hL_ForwTPC2_FV->DrawClone();
  if(doPrint) cL_ForwTPC2->Print("cL_ForwTPC2.pdf");
  TCanvas *cL_ForwTPC3 = new TCanvas("cL_ForwTPC3","cL_ForwTPC3");
  hL_ForwTPC3_FV->GetXaxis()->SetTitle("ForwTPC3 track Length (mm)");
  hL_ForwTPC3_FV->SetStats(0);
  hL_ForwTPC3_FV->DrawClone();
  if(doPrint) cL_ForwTPC3->Print("cL_ForwTPC3.pdf");
    
  TCanvas *cLyz_TPCUp1 = new TCanvas("cLyz_TPCUp1","cLyz_TPCUp1");
  hLyz_TPCUp1->GetXaxis()->SetTitle("TPCUp1 track Length YZ (mm)");
  hLyz_TPCUp1->SetStats(0);
  hLyz_TPCUp1->DrawClone();
  if(doPrint) cLyz_TPCUp1->Print("cLyz_TPCUp1.pdf");
  TCanvas *cLyz_TPCUp2 = new TCanvas("cLyz_TPCUp2","cLyz_TPCUp2");
  hLyz_TPCUp2->GetXaxis()->SetTitle("TPCUp2 track Length YZ (mm)");
  hLyz_TPCUp2->SetStats(0);
  hLyz_TPCUp2->DrawClone();
  if(doPrint) cLyz_TPCUp2->Print("cLyz_TPCUp2.pdf");
  TCanvas *cLyz_TPCDown1 = new TCanvas("cLyz_TPCDown1","cLyz_TPCDown1");
  hLyz_TPCDown1->GetXaxis()->SetTitle("TPCDown1 track Length YZ (mm)");
  hLyz_TPCDown1->SetStats(0);
  hLyz_TPCDown1->DrawClone();
  if(doPrint) cLyz_TPCDown1->Print("cLyz_TPCDown1.pdf");
  TCanvas *cLyz_TPCDown2 = new TCanvas("cLyz_TPCDown2","cLyz_TPCDown2");
  hLyz_TPCDown2->GetXaxis()->SetTitle("TPCDown2 track Length YZ (mm)");
  hLyz_TPCDown2->SetStats(0);
  hLyz_TPCDown2->DrawClone();
  if(doPrint) cLyz_TPCDown2->Print("cLyz_TPCDown2.pdf");
  TCanvas *cLyz_ForwTPC1 = new TCanvas("cLyz_ForwTPC1","cLyz_ForwTPC1");
  hLyz_ForwTPC1->GetXaxis()->SetRangeUser(0,2000);
  hLyz_ForwTPC1->GetXaxis()->SetTitle("ForwTPC1 track Length YZ (mm)");
  hLyz_ForwTPC1->SetStats(0);
  hLyz_ForwTPC1->DrawClone();
  if(doPrint) cLyz_ForwTPC1->Print("cLyz_ForwTPC1.pdf");
  TCanvas *cLyz_ForwTPC2 = new TCanvas("cLyz_ForwTPC2","cLyz_ForwTPC2");
  hLyz_ForwTPC2->GetXaxis()->SetRangeUser(0,2000);
  hLyz_ForwTPC2->GetXaxis()->SetTitle("ForwTPC2 track Length YZ (mm)");
  hLyz_ForwTPC2->SetStats(0);
  hLyz_ForwTPC2->DrawClone();
  if(doPrint) cLyz_ForwTPC2->Print("cLyz_ForwTPC2.pdf");
  TCanvas *cLyz_ForwTPC3 = new TCanvas("cLyz_ForwTPC3","cLyz_ForwTPC3");
  hLyz_ForwTPC3->GetXaxis()->SetRangeUser(0,2000);
  hLyz_ForwTPC3->GetXaxis()->SetTitle("ForwTPC3 track Length YZ (mm)");
  hLyz_ForwTPC3->SetStats(0);
  hLyz_ForwTPC3->DrawClone();
  if(doPrint) cLyz_ForwTPC3->Print("cLyz_ForwTPC3.pdf");

  // Last Track Point

  TCanvas *cLastPt_Target_PhiVsZ = new TCanvas("cLastPt_Target_PhiVsZ","cLastPt_Target_PhiVsZ");
  hLastPt_Target_PhiVsZ->GetXaxis()->SetTitle("#phi (radians)");
  hLastPt_Target_PhiVsZ->GetYaxis()->SetTitle("Track Last Z (mm)");
  hLastPt_Target_PhiVsZ->SetStats(0);
  hLastPt_Target_PhiVsZ->DrawClone("colz");
  if(doPrint) hLastPt_Target_PhiVsZ->Print("cLastPt_Target_PhiVsZ.pdf");
  TCanvas *cLastPt_Target_ThetaVsZ = new TCanvas("cLastPt_Target_ThetaVsZ","cLastPt_Target_ThetaVsZ");
  hLastPt_Target_ThetaVsZ->GetXaxis()->SetTitle("#theta (radians)");
  hLastPt_Target_ThetaVsZ->GetYaxis()->SetTitle("Track Last Z (mm)");
  hLastPt_Target_ThetaVsZ->SetStats(0);
  hLastPt_Target_ThetaVsZ->DrawClone("colz");
  if(doPrint) cLastPt_Target_ThetaVsZ->Print("cLastPt_Target_ThetaVsZ.pdf");
  TCanvas *cLastPt_Target_Z = new TCanvas("cLastPt_Target_Z","cLastPt_Target_Z");
  hLastPt_Target_Z->GetXaxis()->SetTitle("Track Last Z (mm)");
  hLastPt_Target_Z->SetStats(0);
  hLastPt_Target_Z->DrawClone();
  if(doPrint) cLastPt_Target_Z->Print("cLastPt_Target_Z.pdf");
  TCanvas *cLastPt_Target_XY = new TCanvas("cLastPt_Target_XY","cLastPt_Target_XY");
  hLastPt_Target_XY->GetXaxis()->SetTitle("Track Last X (mm)");
  hLastPt_Target_XY->GetYaxis()->SetTitle("Track Last Y (mm)");
  hLastPt_Target_XY->SetStats(0);
  hLastPt_Target_XY->DrawClone("colz");
  if(doPrint) cLastPt_Target_XY->Print("cLastPt_Target_XY.pdf");
  TCanvas *cLastPt_Target_YZ = new TCanvas("cLastPt_Target_YZ","cLastPt_Target_YZ");
  hLastPt_Target_YZ->GetXaxis()->SetTitle("Track Last Y (mm)");
  hLastPt_Target_YZ->GetYaxis()->SetTitle("Track Last Z (mm)");
  hLastPt_Target_YZ->SetStats(0);
  hLastPt_Target_YZ->DrawClone("colz");
  if(doPrint) cLastPt_Target_YZ->Print("cLastPt_Target_YZ.pdf");
  TCanvas *cLastPt_Target_XZ = new TCanvas("cLastPt_Target_XZ","cLastPt_Target_XZ");
  hLastPt_Target_XZ->GetXaxis()->SetTitle("Track Last X (mm)");
  hLastPt_Target_XZ->GetYaxis()->SetTitle("Track Last Z (mm)");
  hLastPt_Target_XZ->SetStats(0);
  hLastPt_Target_XZ->DrawClone("colz");
  if(doPrint) cLastPt_Target_XZ->Print("cLastPt_Target_XZ.pdf");

  TCanvas *cLastPt_Oth_PhiVsZ = new TCanvas("cLastPt_Oth_PhiVsZ","cLastPt_Oth_PhiVsZ");
  hLastPt_Oth_PhiVsZ->GetXaxis()->SetTitle("#phi (radians)");
  hLastPt_Oth_PhiVsZ->GetYaxis()->SetTitle("Track Last Z (mm)");
  hLastPt_Oth_PhiVsZ->SetStats(0);
  hLastPt_Oth_PhiVsZ->DrawClone("colz");
  if(doPrint) hLastPt_Oth_PhiVsZ->Print("cLastPt_Oth_PhiVsZ.pdf");
  TCanvas *cLastPt_Oth_ThetaVsZ = new TCanvas("cLastPt_Oth_ThetaVsZ","cLastPt_Oth_ThetaVsZ");
  hLastPt_Oth_ThetaVsZ->GetXaxis()->SetTitle("#theta (radians)");
  hLastPt_Oth_ThetaVsZ->GetYaxis()->SetTitle("Track Last Z (mm)");
  hLastPt_Oth_ThetaVsZ->SetStats(0);
  hLastPt_Oth_ThetaVsZ->DrawClone("colz");
  if(doPrint) cLastPt_Oth_ThetaVsZ->Print("cLastPt_Oth_ThetaVsZ.pdf");
  TCanvas *cLastPt_Oth_Z = new TCanvas("cLastPt_Oth_Z","cLastPt_Oth_Z");
  hLastPt_Oth_Z->GetXaxis()->SetTitle("Track Last Z (mm)");
  hLastPt_Oth_Z->SetStats(0);
  hLastPt_Oth_Z->DrawClone();
  if(doPrint) cLastPt_Oth_Z->Print("cLastPt_Oth_Z.pdf");
  TCanvas *cLastPt_Oth_XY = new TCanvas("cLastPt_Oth_XY","cLastPt_Oth_XY");
  hLastPt_Oth_XY->GetXaxis()->SetTitle("Track Last X (mm)");
  hLastPt_Oth_XY->GetYaxis()->SetTitle("Track Last Y (mm)");
  hLastPt_Oth_XY->SetStats(0);
  hLastPt_Oth_XY->DrawClone("colz");
  if(doPrint) cLastPt_Oth_XY->Print("cLastPt_Oth_XY.pdf");
  TCanvas *cLastPt_Oth_YZ = new TCanvas("cLastPt_Oth_YZ","cLastPt_Oth_YZ");
  hLastPt_Oth_YZ->GetXaxis()->SetTitle("Track Last Y (mm)");
  hLastPt_Oth_YZ->GetYaxis()->SetTitle("Track Last Z (mm)");
  hLastPt_Oth_YZ->SetStats(0);
  hLastPt_Oth_YZ->DrawClone("colz");
  if(doPrint) cLastPt_Oth_YZ->Print("cLastPt_Oth_YZ.pdf");
  TCanvas *cLastPt_Oth_XZ = new TCanvas("cLastPt_Oth_XZ","cLastPt_Oth_XZ");
  hLastPt_Oth_XZ->GetXaxis()->SetTitle("Track Last X (mm)");
  hLastPt_Oth_XZ->GetYaxis()->SetTitle("Track Last Z (mm)");
  hLastPt_Oth_XZ->SetStats(0);
  hLastPt_Oth_XZ->DrawClone("colz");
  if(doPrint) cLastPt_Oth_XZ->Print("cLastPt_Oth_XZ.pdf");

  return;



  // Efficiencies
  
  TCanvas *cCosThetaVsMom_Target_Eff = new TCanvas("cCosThetaVsMom_Target_Eff","cCosThetaVsMom_Target_Eff");
  hCosThetaVsMom_Target_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosThetaVsMom_Target_Eff->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hCosThetaVsMom_Target_Eff->SetStats(0);
  hCosThetaVsMom_Target_Eff->DrawClone("colz");
  if(doPrint) cCosThetaVsMom_Target_Eff->Print("cCosThetaVsMom_Target_Eff.pdf");
  hCosThetaVsMom_Target_Eff->GetYaxis()->SetRangeUser(0,1000);
  hCosThetaVsMom_Target_Eff->DrawClone("colz");
  if(doPrint) cCosThetaVsMom_Target_Eff->Print("cCosThetaVsMom_Target_Eff_ZOOM.pdf");
  TCanvas *cPhiVsMom_Target_Eff = new TCanvas("cPhiVsMom_Target_Eff","cPhiVsMom_Target_Eff");
  hPhiVsMom_Target_Eff->GetXaxis()->SetTitle("True #phi");
  hPhiVsMom_Target_Eff->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hPhiVsMom_Target_Eff->SetStats(0);
  hPhiVsMom_Target_Eff->DrawClone("colz");
  if(doPrint) cPhiVsMom_Target_Eff->Print("cPhiVsMom_Target_Eff.pdf");
  hPhiVsMom_Target_Eff->GetYaxis()->SetRangeUser(0,1000);
  hPhiVsMom_Target_Eff->DrawClone("colz");
  if(doPrint) cPhiVsMom_Target_Eff->Print("cPhiVsMom_Target_Eff_ZOOM.pdf");
  TCanvas *cThetaVsPhi_Target_Eff = new TCanvas("cThetaVsPhi_Target_Eff","cThetaVsPhi_Target_Eff");
  hThetaVsPhi_Target_Eff->GetXaxis()->SetTitle("True #theta");
  hThetaVsPhi_Target_Eff->GetYaxis()->SetTitle("True #phi");
  hThetaVsPhi_Target_Eff->SetStats(0);
  hThetaVsPhi_Target_Eff->DrawClone("colz");
  if(doPrint) cThetaVsPhi_Target_Eff->Print("cThetaVsPhi_Target_Eff.pdf");
  TCanvas *cMom_Target_Eff = new TCanvas("cMom_Target_Eff","cMom_Target_Eff");
  hMom_Target_Eff->GetXaxis()->SetRangeUser(0,2000.);
  hMom_Target_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hMom_Target_Eff->GetXaxis()->SetTitle("True Momentum (MeV/c)");
  hMom_Target_Eff->SetStats(0);
  hMom_Target_Eff->DrawClone("");
  if(doPrint) cMom_Target_Eff->Print("cMom_Target_Eff.pdf");
  TCanvas *cCosTheta_Target_Eff = new TCanvas("cCosTheta_Target_Eff","cCosTheta_Target_Eff");
  hCosTheta_Target_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hCosTheta_Target_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosTheta_Target_Eff->SetStats(0);
  hCosTheta_Target_Eff->DrawClone("");
  if(doPrint) cCosTheta_Target_Eff->Print("cCosTheta_Target_Eff.pdf");
  TCanvas *cPhi_Target_Eff = new TCanvas("cPhi_Target_Eff","cPhi_Target_Eff");
  hPhi_Target_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hPhi_Target_Eff->GetXaxis()->SetTitle("True #phi");
  hPhi_Target_Eff->SetStats(0);
  hPhi_Target_Eff->DrawClone("");
  if(doPrint) cPhi_Target_Eff->Print("cPhi_Target_Eff.pdf");

  TCanvas *cCosThetaVsMom_TPC_Eff = new TCanvas("cCosThetaVsMom_TPC_Eff","cCosThetaVsMom_TPC_Eff");
  hCosThetaVsMom_TPC_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosThetaVsMom_TPC_Eff->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hCosThetaVsMom_TPC_Eff->SetStats(0);
  hCosThetaVsMom_TPC_Eff->DrawClone("colz");
  if(doPrint) cCosThetaVsMom_TPC_Eff->Print("cCosThetaVsMom_TPC_Eff.pdf");
  hCosThetaVsMom_TPC_Eff->GetYaxis()->SetRangeUser(0,1000);
  hCosThetaVsMom_TPC_Eff->DrawClone("colz");
  if(doPrint) cCosThetaVsMom_TPC_Eff->Print("cCosThetaVsMom_TPC_Eff_ZOOM.pdf");
  TCanvas *cPhiVsMom_TPC_Eff = new TCanvas("cPhiVsMom_TPC_Eff","cPhiVsMom_TPC_Eff");
  hPhiVsMom_TPC_Eff->GetXaxis()->SetTitle("True #phi");
  hPhiVsMom_TPC_Eff->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hPhiVsMom_TPC_Eff->SetStats(0);
  hPhiVsMom_TPC_Eff->DrawClone("colz");
  if(doPrint) cPhiVsMom_TPC_Eff->Print("cPhiVsMom_TPC_Eff.pdf");
  hPhiVsMom_TPC_Eff->GetYaxis()->SetRangeUser(0,1000);
  hPhiVsMom_TPC_Eff->DrawClone("colz");
  if(doPrint) cPhiVsMom_TPC_Eff->Print("cPhiVsMom_TPC_Eff_ZOOM.pdf");
  TCanvas *cThetaVsPhi_TPC_Eff = new TCanvas("cThetaVsPhi_TPC_Eff","cThetaVsPhi_TPC_Eff");
  hThetaVsPhi_TPC_Eff->GetXaxis()->SetTitle("True #theta");
  hThetaVsPhi_TPC_Eff->GetYaxis()->SetTitle("True #phi");
  hThetaVsPhi_TPC_Eff->SetStats(0);
  hThetaVsPhi_TPC_Eff->DrawClone("colz");
  if(doPrint) cThetaVsPhi_TPC_Eff->Print("cThetaVsPhi_TPC_Eff.pdf");
  TCanvas *cMom_TPC_Eff = new TCanvas("cMom_TPC_Eff","cMom_TPC_Eff");
  hMom_TPC_Eff->GetXaxis()->SetRangeUser(0,2000.);
  hMom_TPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hMom_TPC_Eff->GetXaxis()->SetTitle("True Momentum (MeV/c)");
  hMom_TPC_Eff->SetStats(0);
  hMom_TPC_Eff->DrawClone("");
  if(doPrint) cMom_TPC_Eff->Print("cMom_TPC_Eff.pdf");
  TCanvas *cCosTheta_TPC_Eff = new TCanvas("cCosTheta_TPC_Eff","cCosTheta_TPC_Eff");
  hCosTheta_TPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hCosTheta_TPC_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosTheta_TPC_Eff->SetStats(0);
  hCosTheta_TPC_Eff->DrawClone("");
  if(doPrint) cCosTheta_TPC_Eff->Print("cCosTheta_TPC_Eff.pdf");
  TCanvas *cPhi_TPC_Eff = new TCanvas("cPhi_TPC_Eff","cPhi_TPC_Eff");
  hPhi_TPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hPhi_TPC_Eff->GetXaxis()->SetTitle("True #phi");
  hPhi_TPC_Eff->SetStats(0);
  hPhi_TPC_Eff->DrawClone("");
  if(doPrint) cPhi_TPC_Eff->Print("cPhi_TPC_Eff.pdf");

  TCanvas *cCosThetaVsMom_TargetOrTPC_Eff = new TCanvas("cCosThetaVsMom_TargetOrTPC_Eff","cCosThetaVsMom_TargetOrTPC_Eff");
  hCosThetaVsMom_TargetOrTPC_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosThetaVsMom_TargetOrTPC_Eff->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hCosThetaVsMom_TargetOrTPC_Eff->SetStats(0);
  hCosThetaVsMom_TargetOrTPC_Eff->DrawClone("colz");
  if(doPrint) cCosThetaVsMom_TargetOrTPC_Eff->Print("cCosThetaVsMom_TargetOrTPC_Eff.pdf");
  hCosThetaVsMom_TargetOrTPC_Eff->GetYaxis()->SetRangeUser(0,1000);
  hCosThetaVsMom_TargetOrTPC_Eff->DrawClone("colz");
  if(doPrint) cCosThetaVsMom_TargetOrTPC_Eff->Print("cCosThetaVsMom_TargetOrTPC_Eff_ZOOM.pdf");
  TCanvas *cPhiVsMom_TargetOrTPC_Eff = new TCanvas("cPhiVsMom_TargetOrTPC_Eff","cPhiVsMom_TargetOrTPC_Eff");
  hPhiVsMom_TargetOrTPC_Eff->GetXaxis()->SetTitle("True #phi");
  hPhiVsMom_TargetOrTPC_Eff->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hPhiVsMom_TargetOrTPC_Eff->SetStats(0);
  hPhiVsMom_TargetOrTPC_Eff->DrawClone("colz");
  if(doPrint) cPhiVsMom_TargetOrTPC_Eff->Print("cPhiVsMom_TargetOrTPC_Eff.pdf");
  hPhiVsMom_TargetOrTPC_Eff->GetYaxis()->SetRangeUser(0,1000);
  hPhiVsMom_TargetOrTPC_Eff->DrawClone("colz");
  if(doPrint) cPhiVsMom_TargetOrTPC_Eff->Print("cPhiVsMom_TargetOrTPC_Eff_ZOOM.pdf");
  TCanvas *cThetaVsPhi_TargetOrTPC_Eff = new TCanvas("cThetaVsPhi_TargetOrTPC_Eff","cThetaVsPhi_TargetOrTPC_Eff");
  hThetaVsPhi_TargetOrTPC_Eff->GetXaxis()->SetTitle("True #theta");
  hThetaVsPhi_TargetOrTPC_Eff->GetYaxis()->SetTitle("True #phi");
  hThetaVsPhi_TargetOrTPC_Eff->SetStats(0);
  hThetaVsPhi_TargetOrTPC_Eff->DrawClone("colz");
  if(doPrint) cThetaVsPhi_TargetOrTPC_Eff->Print("cThetaVsPhi_TargetOrTPC_Eff.pdf");
  TCanvas *cMom_TargetOrTPC_Eff = new TCanvas("cMom_TargetOrTPC_Eff","cMom_TargetOrTPC_Eff");
  hMom_TargetOrTPC_Eff->GetXaxis()->SetRangeUser(0,2000.);
  hMom_TargetOrTPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hMom_TargetOrTPC_Eff->GetXaxis()->SetTitle("True Momentum (MeV/c)");
  hMom_TargetOrTPC_Eff->SetStats(0);
  hMom_TargetOrTPC_Eff->DrawClone("");
  if(doPrint) cMom_TargetOrTPC_Eff->Print("cMom_TargetOrTPC_Eff.pdf");
  TCanvas *cCosTheta_TargetOrTPC_Eff = new TCanvas("cCosTheta_TargetOrTPC_Eff","cCosTheta_TargetOrTPC_Eff");
  hCosTheta_TargetOrTPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hCosTheta_TargetOrTPC_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosTheta_TargetOrTPC_Eff->SetStats(0);
  hCosTheta_TargetOrTPC_Eff->DrawClone("");
  if(doPrint) cCosTheta_TargetOrTPC_Eff->Print("cCosTheta_TargetOrTPC_Eff.pdf");
  TCanvas *cPhi_TargetOrTPC_Eff = new TCanvas("cPhi_TargetOrTPC_Eff","cPhi_TargetOrTPC_Eff");
  hPhi_TargetOrTPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hPhi_TargetOrTPC_Eff->GetXaxis()->SetTitle("True #phi");
  hPhi_TargetOrTPC_Eff->SetStats(0);
  hPhi_TargetOrTPC_Eff->DrawClone("");
  if(doPrint) cPhi_TargetOrTPC_Eff->Print("cPhi_TargetOrTPC_Eff.pdf");

  TCanvas *cCosThetaVsMom_TargetAndTPC_Eff = new TCanvas("cCosThetaVsMom_TargetAndTPC_Eff","cCosThetaVsMom_TargetAndTPC_Eff");
  hCosThetaVsMom_TargetAndTPC_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosThetaVsMom_TargetAndTPC_Eff->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hCosThetaVsMom_TargetAndTPC_Eff->SetStats(0);
  hCosThetaVsMom_TargetAndTPC_Eff->DrawClone("colz");
  if(doPrint) cCosThetaVsMom_TargetAndTPC_Eff->Print("cCosThetaVsMom_TargetAndTPC_Eff.pdf");
  hCosThetaVsMom_TargetAndTPC_Eff->GetYaxis()->SetRangeUser(0,1000);
  hCosThetaVsMom_TargetAndTPC_Eff->DrawClone("colz");
  if(doPrint) cCosThetaVsMom_TargetAndTPC_Eff->Print("cCosThetaVsMom_TargetAndTPC_Eff_ZOOM.pdf");
  TCanvas *cPhiVsMom_TargetAndTPC_Eff = new TCanvas("cPhiVsMom_TargetAndTPC_Eff","cPhiVsMom_TargetAndTPC_Eff");
  hPhiVsMom_TargetAndTPC_Eff->GetXaxis()->SetTitle("True #phi");
  hPhiVsMom_TargetAndTPC_Eff->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hPhiVsMom_TargetAndTPC_Eff->SetStats(0);
  hPhiVsMom_TargetAndTPC_Eff->DrawClone("colz");
  if(doPrint) cPhiVsMom_TargetAndTPC_Eff->Print("cPhiVsMom_TargetAndTPC_Eff.pdf");
  hPhiVsMom_TargetAndTPC_Eff->GetYaxis()->SetRangeUser(0,1000);
  hPhiVsMom_TargetAndTPC_Eff->DrawClone("colz");
  if(doPrint) cPhiVsMom_TargetAndTPC_Eff->Print("cPhiVsMom_TargetAndTPC_Eff_ZOOM.pdf");
  TCanvas *cThetaVsPhi_TargetAndTPC_Eff = new TCanvas("cThetaVsPhi_TargetAndTPC_Eff","cThetaVsPhi_TargetAndTPC_Eff");
  hThetaVsPhi_TargetAndTPC_Eff->GetXaxis()->SetTitle("True #theta");
  hThetaVsPhi_TargetAndTPC_Eff->GetYaxis()->SetTitle("True #phi");
  hThetaVsPhi_TargetAndTPC_Eff->SetStats(0);
  hThetaVsPhi_TargetAndTPC_Eff->DrawClone("colz");
  if(doPrint) cThetaVsPhi_TargetAndTPC_Eff->Print("cThetaVsPhi_TargetAndTPC_Eff.pdf");
  TCanvas *cMom_TargetAndTPC_Eff = new TCanvas("cMom_TargetAndTPC_Eff","cMom_TargetAndTPC_Eff");
  hMom_TargetAndTPC_Eff->GetXaxis()->SetRangeUser(0,2000.);
  hMom_TargetAndTPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hMom_TargetAndTPC_Eff->GetXaxis()->SetTitle("True Momentum (MeV/c)");
  hMom_TargetAndTPC_Eff->SetStats(0);
  hMom_TargetAndTPC_Eff->DrawClone("");
  if(doPrint) cMom_TargetAndTPC_Eff->Print("cMom_TargetAndTPC_Eff.pdf");
  TCanvas *cCosTheta_TargetAndTPC_Eff = new TCanvas("cCosTheta_TargetAndTPC_Eff","cCosTheta_TargetAndTPC_Eff");
  hCosTheta_TargetAndTPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hCosTheta_TargetAndTPC_Eff->GetXaxis()->SetTitle("True cos #theta");
  hCosTheta_TargetAndTPC_Eff->SetStats(0);
  hCosTheta_TargetAndTPC_Eff->DrawClone("");
  if(doPrint) cCosTheta_TargetAndTPC_Eff->Print("cCosTheta_TargetAndTPC_Eff.pdf");
  TCanvas *cPhi_TargetAndTPC_Eff = new TCanvas("cPhi_TargetAndTPC_Eff","cPhi_TargetAndTPC_Eff");
  hPhi_TargetAndTPC_Eff->GetYaxis()->SetRangeUser(0,1.1);
  hPhi_TargetAndTPC_Eff->GetXaxis()->SetTitle("True #phi");
  hPhi_TargetAndTPC_Eff->SetStats(0);
  hPhi_TargetAndTPC_Eff->DrawClone("");
  if(doPrint) cPhi_TargetAndTPC_Eff->Print("cPhi_TargetAndTPC_Eff.pdf");

  TCanvas *cCosThetaVsMom_FV = new TCanvas("cCosThetaVsMom_FV","cCosThetaVsMom_FV");
  hCosThetaVsMom_FV->GetXaxis()->SetTitle("True cos #theta");
  hCosThetaVsMom_FV->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hCosThetaVsMom_FV->SetStats(0);
  hCosThetaVsMom_FV->DrawClone("colz");
  if(doPrint) cCosThetaVsMom_FV->Print("cCosThetaVsMom_FV.pdf");
  hCosThetaVsMom_FV->GetYaxis()->SetRangeUser(0,1000);
  hCosThetaVsMom_FV->DrawClone("colz");
  if(doPrint) cCosThetaVsMom_FV->Print("cCosThetaVsMom_FV_ZOOM.pdf");
  TCanvas *cPhiVsMom_FV = new TCanvas("cPhiVsMom_FV","cPhiVsMom_FV");
  hPhiVsMom_FV->GetXaxis()->SetTitle("True #phi");
  hPhiVsMom_FV->GetYaxis()->SetTitle("True Momentum (MeV/c)");
  hPhiVsMom_FV->SetStats(0);
  hPhiVsMom_FV->DrawClone("colz");
  if(doPrint) cPhiVsMom_FV->Print("cPhiVsMom_FV.pdf");
  hPhiVsMom_FV->GetYaxis()->SetRangeUser(0,1000);
  hPhiVsMom_FV->DrawClone("colz");
  if(doPrint) cPhiVsMom_FV->Print("cPhiVsMom_FV_ZOOM.pdf");
  TCanvas *cThetaVsPhi_FV = new TCanvas("cThetaVsPhi_FV","cThetaVsPhi_FV");
  hThetaVsPhi_FV->GetXaxis()->SetTitle("True #theta");
  hThetaVsPhi_FV->GetYaxis()->SetTitle("True #phi");
  hThetaVsPhi_FV->SetStats(0);
  hThetaVsPhi_FV->DrawClone("colz");
  if(doPrint) cThetaVsPhi_FV->Print("cThetaVsPhi_FV.pdf");

  TCanvas *cMom_FV = new TCanvas("cMom_FV","cMom_FV");
  hMom_FV->GetXaxis()->SetTitle("True Track Momentum (MeV/c)");
  hMom_FV->GetXaxis()->SetRangeUser(0,1000);
  hMom_FV->SetStats(0);
  hMom_FV->DrawClone();
  if(doPrint) cMom_FV->Print("cMom_FV.pdf");
  TCanvas *cCosTheta_FV = new TCanvas("cCosTheta_FV","cCosTheta_FV");
  hCosTheta_FV->GetXaxis()->SetTitle("True Track cos #theta");
  hCosTheta_FV->SetStats(0);
  hCosTheta_FV->DrawClone();
  if(doPrint) cCosTheta_FV->Print("cCosTheta_FV.pdf");
  TCanvas *cPhi_FV = new TCanvas("cPhi_FV","cPhi_FV");
  hPhi_FV->GetXaxis()->SetTitle("True Track #phi");
  hPhi_FV->GetXaxis()->SetRangeUser(0,hPhi_FV->GetMaximum()*1.2);
  hPhi_FV->SetStats(0);
  hPhi_FV->DrawClone();
  if(doPrint) cPhi_FV->Print("cPhi_FV.pdf");

  TCanvas *cPDG = new TCanvas("cPDG","cPDG");
  hPDG->GetXaxis()->SetTitle("Track PDG");
  hPDG->DrawClone();
  if(doPrint) cPDG->Print("cPDG.pdf");

  return;
}
