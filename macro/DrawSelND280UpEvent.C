
void DrawSelND280UpEvent
(
 string infilename = "/neutrino/data7/davide/files/SelND280Up_NuMu_MuM_ParID0_All.root"
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
  TH1D *hLength_SD = (TH1D*)infile->Get("hLength_SD");
  TH1D *hLength_Targ = (TH1D*)infile->Get("hLength_Targ");
  TH1D *hLyz_TPC = (TH1D*)infile->Get("hLyz_TPC");
  TH2D *hLengthVsMom_SD = (TH2D*)infile->Get("hLengthVsMom_SD");
  TH2D *hLengthVsMom_Targ = (TH2D*)infile->Get("hLengthVsMom_Targ");
  TH2D *hLyzVsMom_TPC = (TH2D*)infile->Get("hLyzVsMom_TPC");
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
  TH2D *hCosThetaVsMom_Target_Eff 
    = (TH2D*)hCosThetaVsMom_Target->Clone("hCosThetaVsMom_Target_Eff");
  hCosThetaVsMom_Target_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_Target_Eff->SetTitle("hCosThetaVsMom_Target_Eff");

  TH2D *hCosThetaVsMom_TPC_Eff 
    = (TH2D*)hCosThetaVsMom_TPC->Clone("hCosThetaVsMom_TPC_Eff");
  hCosThetaVsMom_TPC_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_TPC_Eff->SetTitle("hCosThetaVsMom_TPC_Eff");

  TH2D *hCosThetaVsMom_TargetOrTPC_Eff 
    = (TH2D*)hCosThetaVsMom_TargetOrTPC->Clone("hCosThetaVsMom_TargetOrTPC_Eff");
  hCosThetaVsMom_TargetOrTPC_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_TargetOrTPC_Eff->SetTitle("hCosThetaVsMom_TargetOrTPC_Eff");
  
  TH2D *hCosThetaVsMom_TargetAndTPC_Eff 
    = (TH2D*)hCosThetaVsMom_TargetAndTPC->Clone("hCosThetaVsMom_TargetAndTPC_Eff");
  hCosThetaVsMom_TargetAndTPC_Eff->Divide(hCosThetaVsMom_FV);
  hCosThetaVsMom_TargetAndTPC_Eff->SetTitle("hCosThetaVsMom_TargetAndTPC_Eff");

  // Draw 
  TCanvas *cCosThetaVsMom_Target_Eff = new TCanvas("hCosThetaVsMom_Target_Eff","hCosThetaVsMom_Target_Eff");
  hCosThetaVsMom_Target_Eff->Draw("colz");
  TCanvas *cCosThetaVsMom_TPC_Eff = new TCanvas("hCosThetaVsMom_TPC_Eff","hCosThetaVsMom_TPC_Eff");
  hCosThetaVsMom_TPC_Eff->Draw("colz");
  TCanvas *cCosThetaVsMom_TargetOrTPC_Eff = new TCanvas("hCosThetaVsMom_TargetOrTPC_Eff","hCosThetaVsMom_TargetOrTPC_Eff");
  hCosThetaVsMom_TargetOrTPC_Eff->Draw("colz");
  TCanvas *cCosThetaVsMom_TargetAndTPC_Eff = new TCanvas("hCosThetaVsMom_TargetAndTPC_Eff","hCosThetaVsMom_TargetAndTPC_Eff");
  hCosThetaVsMom_TargetAndTPC_Eff->Draw("colz");


  return;
}
