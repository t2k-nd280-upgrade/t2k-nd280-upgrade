void ChargeConfusion(std::string MCSelectionFile="mc_fhc_nue_AL3.root", std::string MCSelectionFile2="mc_fhc_anue_AL3.root", TString outFile="ChargeConfusion.root", Int_t whichFGD = 1){

  TFile *FileMC = new TFile(MCSelectionFile.c_str(), "READ");
  // Selection tree
  TTree *TreeSignal  = (TTree*)FileMC->Get("default");

  Float_t selelec_mom, selelec_costheta_z;
  Int_t particle, detector, accum_level;

  TreeSignal->SetBranchAddress("particle",                            &particle);
  TreeSignal->SetBranchAddress("selelec_mom",                         &selelec_mom);
  TreeSignal->SetBranchAddress("detector",                            &detector);
  TreeSignal->SetBranchAddress("accum_level",                         &accum_level);
  TreeSignal->SetBranchAddress("selelec_costheta_z",                  &selelec_costheta_z);

  TFile *FileMC2 = new TFile(MCSelectionFile2.c_str(), "READ");
  // Selection tree
  TTree *TreeSignal2  = (TTree*)FileMC2->Get("default");

  TreeSignal2->SetBranchAddress("particle",                            &particle);
  TreeSignal2->SetBranchAddress("selelec_mom",                         &selelec_mom);
  TreeSignal2->SetBranchAddress("detector",                            &detector);
  TreeSignal2->SetBranchAddress("accum_level",                         &accum_level);
  TreeSignal2->SetBranchAddress("selelec_costheta_z",                  &selelec_costheta_z);

  const int n = 2;
  TH1D* ElecHisto[n]; TH1D* MuonHisto[n]; TH1D* PionHisto[n]; TH1D* ProtHisto[n]; 

  TString histname;
  Int_t bin  = 50; Int_t xmin  = 0; Int_t xmax  = 5000;

  const int nn = 15;
  Double_t parnum[nn] = {0,100,200,400,600,800,1000,1500,2000,2500,3000,3500,4000,4500,5000};

  for(Int_t j = 0; j < n; j++){
    histname.Form("ElecHisto_%i",j);
    ElecHisto[j] = new TH1D(histname.Data(),histname.Data(),nn-1,parnum);
    histname.Form("MuonHisto_%i",j);
    MuonHisto[j] = new TH1D(histname.Data(),histname.Data(),nn-1,parnum);
    histname.Form("PionHisto_%i",j);
    PionHisto[j] = new TH1D(histname.Data(),histname.Data(),nn-1,parnum);
    histname.Form("ProtHisto_%i",j);
    ProtHisto[j] = new TH1D(histname.Data(),histname.Data(),nn-1,parnum);
  }

  for(Int_t i = 0; i < TreeSignal->GetEntries(); i++){
    TreeSignal->GetEntry(i);

    if( detector != whichFGD ) continue;
    if( accum_level < 4 ) continue;

    //if( selelec_costheta_z > 0.9 ) continue;

    if( abs(particle) == 11 )
      ElecHisto[0]->Fill(selelec_mom);
    if( particle == -11 )
      ElecHisto[1]->Fill(selelec_mom);

    if( abs(particle) == 13 )
      MuonHisto[0]->Fill(selelec_mom);
    if( particle == -13 )
      MuonHisto[1]->Fill(selelec_mom);

    if( abs(particle) == 211 )
      PionHisto[0]->Fill(selelec_mom);
    if( particle == 211 )
      PionHisto[1]->Fill(selelec_mom);

    if( particle == 2212 ){
      ProtHisto[1]->Fill(selelec_mom);
      ProtHisto[0]->Fill(selelec_mom);
    }
  }

  for(Int_t i = 0; i < TreeSignal2->GetEntries(); i++){
    TreeSignal2->GetEntry(i);

    if( detector != whichFGD ) continue;
    if( accum_level < 4 ) continue;

    //if( selelec_costheta_z > 0.9 ) continue;

    if( particle == 2212 )
      ProtHisto[0]->Fill(selelec_mom);
  }

  TGraphAsymmErrors *ElecPurity = new TGraphAsymmErrors(ElecHisto[1],ElecHisto[0]);
  TGraphAsymmErrors *MuonPurity = new TGraphAsymmErrors(MuonHisto[1],MuonHisto[0]);
  TGraphAsymmErrors *PionPurity = new TGraphAsymmErrors(PionHisto[1],PionHisto[0]);
  TGraphAsymmErrors *ProtPurity = new TGraphAsymmErrors(ProtHisto[1],ProtHisto[0]);

  TSpline3* ElecSpline = new TSpline3("ElecSpline", ElecPurity);
  TSpline3* MuonSpline = new TSpline3("MuonSpline", MuonPurity);
  TSpline3* PionSpline = new TSpline3("PionSpline", PionPurity);
  TSpline3* ProtSpline = new TSpline3("ProtSpline", ProtPurity);

  TCanvas *c = new TCanvas("ChargeConfusion", "ChargeConfusion");
  c->Divide(2,2);
  c->cd(1);
  ElecPurity->Draw("*a");
  ElecPurity->SetMarkerColor(2);
  ElecPurity->SetMarkerStyle(25);
  ElecPurity->SetLineColor(1);
  ElecPurity->GetYaxis()->SetTitleOffset(1.2);
  ElecPurity->GetYaxis()->SetTitle("Charge Mis-identification [%]");
  ElecPurity->GetXaxis()->SetTitle("Momentum [MeV/c]");
  ElecPurity->SetTitle("Electrons");
  //ElecSpline->Draw("same");

  c->cd(2);
  MuonPurity->Draw("*a");
  MuonPurity->SetMarkerColor(3);
  MuonPurity->SetMarkerStyle(25);
  MuonPurity->SetLineColor(1);
  MuonPurity->GetYaxis()->SetTitleOffset(1.2);
  MuonPurity->GetYaxis()->SetTitle("Charge Mis-identification [%]");
  MuonPurity->GetXaxis()->SetTitle("Momentum [MeV/c]");
  MuonPurity->SetTitle("Muons");
  //MuonSpline->Draw("same");

  c->cd(3);
  PionPurity->Draw("*a");
  PionPurity->SetMarkerColor(4);
  PionPurity->SetMarkerStyle(25);
  PionPurity->SetLineColor(1);
  PionPurity->GetYaxis()->SetTitleOffset(1.2);
  PionPurity->GetYaxis()->SetTitle("Charge Mis-identification [%]");
  PionPurity->GetXaxis()->SetTitle("Momentum [MeV/c]");
  PionPurity->SetTitle("Pions");
  //PionSpline->Draw("same");

  c->cd(4);
  ProtPurity->Draw("*a");
  ProtPurity->SetMarkerColor(6);
  ProtPurity->SetMarkerStyle(25);
  ProtPurity->SetLineColor(1);
  ProtPurity->GetYaxis()->SetTitleOffset(1.2);
  ProtPurity->GetYaxis()->SetTitle("Charge Mis-identification [%]");
  ProtPurity->GetXaxis()->SetTitle("Momentum [MeV/c]");
  ProtPurity->SetTitle("Protons");
  //ProtSpline->Draw("same");

  histname.Form("FGD_%i_",whichFGD+1);
  outFile = histname + outFile;
  TFile *f = new TFile(outFile.Data(), "RECREATE");
  ElecSpline->Write("ElecSpline");
  MuonSpline->Write("MuonSpline");
  PionSpline->Write("PionSpline");
  ProtSpline->Write("ProtSpline");
  ElecPurity->Write();
  MuonPurity->Write();
  PionPurity->Write();
  ProtPurity->Write();
  c->Write();
  
}
