
void DrawRecoEff
( 
 bool doPrint = true,
 bool doStore = false,
 string tag = "prova"
  )
{

  const int NFiles = 4;
  string infilename[NFiles] = {
    "prova_Evt0_NEvt30000_SuperFGD.root",
    //"prova_Evt0_NEvt30000_SuperFGD_PEX0.root",
    "prova_Evt0_NEvt30000_SuperFGD_PEY0.root",
    "prova_Evt0_NEvt30000_SuperFGD_PEZ0.root",
    //"prova_Evt0_NEvt30000_SuperFGD_PEY0_FGDlike.root",
    //"prova_Evt0_NEvt30000_SuperFGD_PEZ0_FGDlike.root",
    "prova_Evt0_NEvt30000_SuperFGD_PEZ0_FGDlike_CutLengthZ.root"
  };
  
  int color[NFiles] = {
    kBlue,
    kRed,
    kGreen,
    kOrange
    //kCyan
  };

  TString label[NFiles] = {
    "SuperFGD",
    //"SuperFGD - # pe X = 0",
    "SuperFGD - # pe Y = 0",
    "SuperFGD - # pe Z = 0",
    //"FGDlike  - # pe Y = 0",
    "FGDlike  - # pe Z = 0"
  };

  
  gROOT->SetStyle("T2K");                  
  gROOT->ForceStyle();                 
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadRightMargin(0.15);
  //gStyle->SetStatX(.4); gStyle->SetStatY(.6);




  TFile *infile[NFiles];
  
  TH2D *hMuon_AllIso_TrMomVsTrCosTh[NFiles];
  TH2D *hPion_AllIso_TrMomVsTrCosTh[NFiles];
  TH2D *hProt_AllIso_TrMomVsTrCosTh[NFiles];

  TH2D *hMuon_EffIso_TrMomVsTrCosTh[NFiles];
  TH2D *hPion_EffIso_TrMomVsTrCosTh[NFiles];
  TH2D *hProt_EffIso_TrMomVsTrCosTh[NFiles];

  TH1D *hMuon_EffIso_TrMom[NFiles];
  TH1D *hPion_EffIso_TrMom[NFiles];
  TH1D *hProt_EffIso_TrMom[NFiles];
  
  TH1D *hMuon_EffIso_TrCosTh[NFiles];
  TH1D *hPion_EffIso_TrCosTh[NFiles];
  TH1D *hProt_EffIso_TrCosTh[NFiles];
  
  
  for(int ifile=0;ifile<NFiles;ifile++){
    
    cout << "Read file: " << infilename[ifile].c_str() << endl;
    
    infile[ifile] = new TFile(infilename[ifile].c_str(),"READ");
    
    hMuon_AllIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hMuon_AllIso_TrMomVsTrCosTh");
    
    hPion_AllIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hPion_AllIso_TrMomVsTrCosTh");
    hProt_AllIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hProt_AllIso_TrMomVsTrCosTh");
    
    hMuon_EffIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hMuon_EffIso_TrMomVsTrCosTh");
    hPion_EffIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hPion_EffIso_TrMomVsTrCosTh");
    hProt_EffIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hProt_EffIso_TrMomVsTrCosTh");
    
    hMuon_EffIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hMuon_EffIso_TrMom");
    hPion_EffIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hPion_EffIso_TrMom");
    hProt_EffIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hProt_EffIso_TrMom");
    
    hMuon_EffIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hMuon_EffIso_TrCosTh");
    hPion_EffIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hPion_EffIso_TrCosTh");
    hProt_EffIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hProt_EffIso_TrCosTh");
  }


  //

  //cout << "# of events Muon: " << hMuon_AllIso_TrMomVsTrCosTh->Integral() << endl;  
  //cout << "# of events Pion: " << hPion_AllIso_TrMomVsTrCosTh->Integral() << endl;
  //cout << "# of events Prot: " << hProt_AllIso_TrMomVsTrCosTh->Integral() << endl;
 
  
  //
  // Draw 
  //

  TLegend *legend = new TLegend(0.4,0.15,0.8,0.4);
  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetTextFont(132);
  legend->SetTextSize(0.05);
  for(int ifile=0;ifile<NFiles;ifile++){
    legend->AddEntry(hMuon_EffIso_TrMom[ifile],label[ifile],"l");
  }

  // Muons

  TCanvas *cMuon_EffIso_TrMomVsTrCosTh[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cMuon_EffIso_TrMomVsTrCosTh_%i",ifile);
    cMuon_EffIso_TrMomVsTrCosTh[ifile] = new TCanvas(name,label[ifile]);
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitle("True Momentum (MeV/c)");
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->SetStats(0);
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->DrawClone("colz");
  }

  TCanvas *cMuon_EffIso_TrMom = new TCanvas("cMuon_EffIso_TrMom","cMuon_EffIso_TrMom");
  for(int ifile=0;ifile<NFiles;ifile++){
    hMuon_EffIso_TrMom[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
    hMuon_EffIso_TrMom[ifile]->SetStats(0);
    hMuon_EffIso_TrMom[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hMuon_EffIso_TrMom[ifile]->GetYaxis()->SetRangeUser(0,1.1);
    hMuon_EffIso_TrMom[ifile]->SetLineColor(color[ifile]);
    hMuon_EffIso_TrMom[ifile]->SetLineWidth(2.);
    if(ifile==0) hMuon_EffIso_TrMom[ifile]->DrawClone("");
    else         hMuon_EffIso_TrMom[ifile]->DrawClone("same");
  }
  legend->Draw();

  TCanvas *cMuon_EffIso_TrCosTh = new TCanvas("cMuon_EffIso_TrCosTh","cMuon_EffIso_TrCosTh");
  for(int ifile=0;ifile<NFiles;ifile++){
    hMuon_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
    hMuon_EffIso_TrCosTh[ifile]->SetStats(0);
    hMuon_EffIso_TrCosTh[ifile]->GetXaxis()->SetRangeUser(-1,+1);
    hMuon_EffIso_TrCosTh[ifile]->GetYaxis()->SetRangeUser(0,1);
    hMuon_EffIso_TrCosTh[ifile]->SetLineColor(color[ifile]);
    hMuon_EffIso_TrCosTh[ifile]->SetLineWidth(2.);
    if(ifile==0) hMuon_EffIso_TrCosTh[ifile]->DrawClone("");
    else         hMuon_EffIso_TrCosTh[ifile]->DrawClone("same");
  }
  legend->Draw();
  
  



  // Pions

  TCanvas* cPion_EffIso_TrMomVsTrCosTh[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cPion_EffIso_TrMomVsTrCosTh_%i",ifile);
    cPion_EffIso_TrMomVsTrCosTh[ifile] = new TCanvas(name,label[ifile]);
    hPion_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
    hPion_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitle("True Momentum (MeV/c)");
    hPion_EffIso_TrMomVsTrCosTh[ifile]->SetStats(0);
    hPion_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hPion_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
    hPion_EffIso_TrMomVsTrCosTh[ifile]->DrawClone("colz");
  }    

  TCanvas *cPion_EffIso_TrMom = new TCanvas("cPion_EffIso_TrMom","cPion_EffIso_TrMom");
  for(int ifile=0;ifile<NFiles;ifile++){
    hPion_EffIso_TrMom[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
    hPion_EffIso_TrMom[ifile]->SetStats(0);
    hPion_EffIso_TrMom[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hPion_EffIso_TrMom[ifile]->GetYaxis()->SetRangeUser(0,1.1);
    hPion_EffIso_TrMom[ifile]->SetLineColor(color[ifile]);
    hPion_EffIso_TrMom[ifile]->SetLineWidth(2.);
    if(ifile==0) hPion_EffIso_TrMom[ifile]->DrawClone("");
    else         hPion_EffIso_TrMom[ifile]->DrawClone("same");
  }  
  legend->Draw();

  TCanvas *cPion_EffIso_TrCosTh = new TCanvas("cPion_EffIso_TrCosTh","cPion_EffIso_TrCosTh");
  for(int ifile=0;ifile<NFiles;ifile++){
    hPion_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
    hPion_EffIso_TrCosTh[ifile]->SetStats(0);
    hPion_EffIso_TrCosTh[ifile]->GetXaxis()->SetRangeUser(-1,+1);
    hPion_EffIso_TrCosTh[ifile]->GetYaxis()->SetRangeUser(0,1.1);
    hPion_EffIso_TrCosTh[ifile]->SetLineColor(color[ifile]);
    hPion_EffIso_TrCosTh[ifile]->SetLineWidth(2.);
    if(ifile==0) hPion_EffIso_TrCosTh[ifile]->DrawClone("");
    else         hPion_EffIso_TrCosTh[ifile]->DrawClone("same");
  }
  legend->Draw();



  // Protons

  TCanvas *cProt_EffIso_TrMomVsTrCosTh[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cProt_EffIso_TrMomVsTrCosTh_%i",ifile);
    cProt_EffIso_TrMomVsTrCosTh[ifile] = new TCanvas(name,label[ifile]);
    hProt_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
    hProt_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitle("True Momentum (MeV/c)");
    hProt_EffIso_TrMomVsTrCosTh[ifile]->SetStats(0);
    hProt_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hProt_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
    hProt_EffIso_TrMomVsTrCosTh[ifile]->DrawClone("colz");
  }

  TCanvas *cProt_EffIso_TrMom = new TCanvas("cProt_EffIso_TrMom","cProt_EffIso_TrMom");
  for(int ifile=0;ifile<NFiles;ifile++){
    hProt_EffIso_TrMom[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
    hProt_EffIso_TrMom[ifile]->SetStats(0);
    hProt_EffIso_TrMom[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hProt_EffIso_TrMom[ifile]->GetYaxis()->SetRangeUser(0,1.1);
    hProt_EffIso_TrMom[ifile]->SetLineColor(color[ifile]);
    hProt_EffIso_TrMom[ifile]->SetLineWidth(2.);
    if(ifile==0) hProt_EffIso_TrMom[ifile]->DrawClone("");
    else         hProt_EffIso_TrMom[ifile]->DrawClone("same");
  }
  legend->Draw();

  TCanvas *cProt_EffIso_TrCosTh = new TCanvas("cProt_EffIso_TrCosTh","cProt_EffIso_TrCosTh");
  for(int ifile=0;ifile<NFiles;ifile++){
    hProt_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
    hProt_EffIso_TrCosTh[ifile]->SetStats(0);
    hProt_EffIso_TrCosTh[ifile]->GetXaxis()->SetRangeUser(-1,+1);
    hProt_EffIso_TrCosTh[ifile]->GetYaxis()->SetRangeUser(0,1);
    hProt_EffIso_TrCosTh[ifile]->SetLineColor(color[ifile]);
    hProt_EffIso_TrCosTh[ifile]->SetLineWidth(2.);
    if(ifile==0) hProt_EffIso_TrCosTh[ifile]->DrawClone("");
    else         hProt_EffIso_TrCosTh[ifile]->DrawClone("same");
  }
  legend->Draw();
  


  return;
}
