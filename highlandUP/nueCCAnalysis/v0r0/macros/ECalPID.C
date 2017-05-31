void ECalPID(std::string MCSelectionFile="mc_fhc_nue_AL3.root", std::string MCSelectionFile2="mc_fhc_anue_AL3.root", TString outFile="ECalPID_1.root"){

  TFile *FileMC = new TFile(MCSelectionFile.c_str(), "READ");
  // Selection tree
  TTree *TreeSignal  = (TTree*)FileMC->Get("default");

  Float_t selelec_mom, selelec_costheta_z, ecalmipem, ElemomECalEntrance;
  Int_t particle, detector, accum_level;
  Int_t isDsECal, isBrECal;

  TreeSignal->SetBranchAddress("particle",                            &particle);
  TreeSignal->SetBranchAddress("selelec_mom",                         &selelec_mom);
  TreeSignal->SetBranchAddress("detector",                            &detector);
  TreeSignal->SetBranchAddress("accum_level",                         &accum_level);
  TreeSignal->SetBranchAddress("selelec_costheta_z",                  &selelec_costheta_z);
  TreeSignal->SetBranchAddress("ecalmipem",                           &ecalmipem);
  TreeSignal->SetBranchAddress("isDsECal",                            &isDsECal);
  TreeSignal->SetBranchAddress("isBrECal",                            &isBrECal);
  TreeSignal->SetBranchAddress("ElemomECalEntrance",                  &ElemomECalEntrance);

  TFile *FileMC2 = new TFile(MCSelectionFile2.c_str(), "READ");
  // Selection tree
  TTree *TreeSignal2  = (TTree*)FileMC2->Get("default");

  TreeSignal2->SetBranchAddress("particle",                            &particle);
  TreeSignal2->SetBranchAddress("selelec_mom",                         &selelec_mom);
  TreeSignal2->SetBranchAddress("detector",                            &detector);
  TreeSignal2->SetBranchAddress("accum_level",                         &accum_level);
  TreeSignal2->SetBranchAddress("selelec_costheta_z",                  &selelec_costheta_z);
  TreeSignal2->SetBranchAddress("ecalmipem",                           &ecalmipem);
  TreeSignal2->SetBranchAddress("isDsECal",                            &isDsECal);
  TreeSignal2->SetBranchAddress("isBrECal",                            &isBrECal);  
  TreeSignal2->SetBranchAddress("ElemomECalEntrance",                  &ElemomECalEntrance);

  const int nmombins = 6;
  Double_t mombins[nmombins] = {0,300,600,1000,3000,300000};

  TAxis* axis = new TAxis(nmombins, mombins);

  const int n = 2;
  TH1D* ElecHisto[n][nmombins]; TH1D* MuonHisto[n][nmombins]; TH1D* PionHisto[n][nmombins]; TH1D* ProtHisto[n][nmombins]; TH1D* OtheHisto[n][nmombins];

  TString histname;
  for(Int_t j = 0; j < n; j++){
    for(Int_t k = 0; k < nmombins-1; k++){
      histname.Form("ElecHisto_%i_%i-%i",j,(int)mombins[k],(int)mombins[k+1]);
      ElecHisto[j][k] = new TH1D(histname.Data(),histname.Data(),50,-100,100);
      histname.Form("MuonHisto_%i_%i-%i",j,(int)mombins[k],(int)mombins[k+1]);
      MuonHisto[j][k] = new TH1D(histname.Data(),histname.Data(),50,-100,100);
      histname.Form("PionHisto_%i_%i-%i",j,(int)mombins[k],(int)mombins[k+1]);
      PionHisto[j][k] = new TH1D(histname.Data(),histname.Data(),50,-100,100);
      histname.Form("ProtHisto_%i_%i-%i",j,(int)mombins[k],(int)mombins[k+1]);
      ProtHisto[j][k] = new TH1D(histname.Data(),histname.Data(),50,-100,100);
      histname.Form("OtheHisto_%i_%i-%i",j,(int)mombins[k],(int)mombins[k+1]);
      OtheHisto[j][k] = new TH1D(histname.Data(),histname.Data(),50,-100,100);
    }
  }

  for(Int_t i = 0; i < TreeSignal->GetEntries(); i++){
    TreeSignal->GetEntry(i);
    
    if( accum_level < 4 ) continue;
    if( ecalmipem < -100.0 ) continue;
    if( detector < 0 || detector > 1) continue;
    if( particle == 999 ) continue;

    Int_t findmombin = -1;
    for(Int_t k = 0; k < nmombins-1; k++){
      if( ElemomECalEntrance > mombins[k] && ElemomECalEntrance <= mombins[k+1] ){
	findmombin = k;
	break;
      }
    }

    if( findmombin < 0) continue;

    Int_t ecalnum = -1;
    if( isDsECal )
      ecalnum = 0;
    else if( isBrECal )
      ecalnum = 1;
    
    if( ecalnum < 0 ) continue;

    if( abs(particle) == 11 )
      ElecHisto[ecalnum][findmombin]->Fill(ecalmipem);
    else if( abs(particle) == 13 )
      MuonHisto[ecalnum][findmombin]->Fill(ecalmipem);
    else if( abs(particle) == 211 )
      PionHisto[ecalnum][findmombin]->Fill(ecalmipem);
    else if( abs(particle) == 2212 )
      ProtHisto[ecalnum][findmombin]->Fill(ecalmipem);
    else
       OtheHisto[ecalnum][findmombin]->Fill(ecalmipem);
  }


  TFile *f = new TFile(outFile.Data(), "RECREATE");
  axis->Write("MomentumBins");
  for(Int_t j = 0; j < n; j++){
    for(Int_t k = 0; k < nmombins-1; k++){
      ElecHisto[j][k]->GetXaxis()->SetTitle("ECal MIP-EM PID");
      MuonHisto[j][k]->GetXaxis()->SetTitle("ECal MIP-EM PID");
      PionHisto[j][k]->GetXaxis()->SetTitle("ECal MIP-EM PID");
      ProtHisto[j][k]->GetXaxis()->SetTitle("ECal MIP-EM PID");
      OtheHisto[j][k]->GetXaxis()->SetTitle("ECal MIP-EM PID");

      ElecHisto[j][k]->Write();
      MuonHisto[j][k]->Write();
      PionHisto[j][k]->Write();
      ProtHisto[j][k]->Write();
      OtheHisto[j][k]->Write();
    }
  }
  
}
