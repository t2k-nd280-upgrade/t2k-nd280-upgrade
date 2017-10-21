
void DrawRecoEff
( 
 bool doPrint = true,
 bool doStore = true,
 TString tag = "prova",
 bool doDrawLegend = true
  )
{
 
  const int NFiles = 2;
  string infilename[NFiles] = {
 

    "RECONSTRUCTION/5_10_17/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT.root",
    
    //
    // Real MPPC efficiency
    //

    "RECONSTRUCTION/26_9_17/RealMPPCeff/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_100k.root"

    //"RECONSTRUCTION/26_9_17/RealMPPCeff/FGDlike-UseXY-UseXZ-UseYZ-TruthFV-Separate10_CutLayers40mm_NoCutAngle_100k.root",
    //"RECONSTRUCTION/26_9_17/RealMPPCeff/FGDlike-UseXY-UseXZ-UseYZ-TruthFV-Separate10_CutLayers30mm_100k.root" // OK
    //"RECONSTRUCTION/26_9_17/RealMPPCeff/FGDlike-UseXY-UseXZ-UseYZ-TruthFV-Separate10_100k.root"
    

    //
    // Straight MPPC efficiency
    //
    //"RECONSTRUCTION/26_9_17/StraightMPPCeff/FGDlike-UseXY-UseXZ-UseYZ-TruthFV-Separate10_100k.root",
    //"RECONSTRUCTION/26_9_17/StraightMPPCeff/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_100k.root"



    //
    // OLD
    //
    
    //"RECONSTRUCTION/20_8_17/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_30k.root",
    //"RECONSTRUCTION/20_8_17/FGDlike-UseXY-UseXZ-UseYZ-TruthFV-Separate10_30k.root"
    
    //"RECONSTRUCTION/20_8_17/SuperFGD_newTrkLen_30k.root",
    //"RECONSTRUCTION/20_8_17/FGDlike_newTrkLen_30k.root",
    
    //"RECONSTRUCTION/20_8_17/SuperFGD_30k.root",
    //"RECONSTRUCTION/20_8_17/FGDlike_30k.root",
    
    //"RECONSTRUCTION/18_7_17/SuperFGD_30k_separated1cm_Iso.root",
    //"RECONSTRUCTION/FGDlike_30k_separated1cm_Iso.root",
    
    //"RECONSTRUCTION/18_7_17/SuperFGD_30k_separated1cm_Iso_NewOutFV.root",
    //"RECONSTRUCTION/18_7_17/FGDlike_30k_separated1cm_Iso_NewOutFV.root"
    
    //"RECONSTRUCTION/18_7_17/SuperFGD_30k_separated2cm_Iso.root",
    //"RECONSTRUCTION/18_7_17/FGDlike_30k_separated2cm_Iso.root"
    
    //"RECONSTRUCTION/18_7_17/SuperFGD_INFV_30k_Separate_1cm.root"
    //"RECONSTRUCTION/18_7_17/SuperFGD_INFV_30k_NoSeparated.root"
    //"SuperFGD_INFV_30k_NoSeparate.root"
    
  };

  int color[NFiles] = {
    kBlue,
    kRed
    //kGreen
    //kOrange
    //kCyan
  };

  TString label[NFiles] = {

     //"CutLayers30mm",
    //"CutLayers40mm"

    "OK",
    "Uniform"
    
    //"SuperFGD",
    //"FGDXZ"
    
    //"SuperFGD New",
    //"SuperFGD Old"
    //"SuperFGD - separated 2cm",
    //"SuperFGD - no separated"
    
    //"SuperFGD XY - separated 1cm",
    //"SuperFGD XZ - separated 1cm",
    
    //"FGD XZ - separated 2cm"
    //"FGD XZ - Truth OutFV"
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

  TH1D *hMuon_AllIso_TrMom[NFiles];
  TH1D *hPion_AllIso_TrMom[NFiles];
  TH1D *hProt_AllIso_TrMom[NFiles];
  
  TH1D *hMuon_EffIso_TrMom[NFiles];
  TH1D *hPion_EffIso_TrMom[NFiles];
  TH1D *hProt_EffIso_TrMom[NFiles];
  
  TH1D *hMuon_AllIso_TrCosTh[NFiles];
  TH1D *hPion_AllIso_TrCosTh[NFiles];
  TH1D *hProt_AllIso_TrCosTh[NFiles];

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

    hMuon_AllIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hMuon_AllIso_TrMom");
    hPion_AllIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hPion_AllIso_TrMom");
    hProt_AllIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hProt_AllIso_TrMom");
    
    hMuon_EffIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hMuon_EffIso_TrMom");
    hPion_EffIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hPion_EffIso_TrMom");
    hProt_EffIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hProt_EffIso_TrMom");
    
    hMuon_AllIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hMuon_AllIso_TrCosTh");
    hPion_AllIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hPion_AllIso_TrCosTh");
    hProt_AllIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hProt_AllIso_TrCosTh");

    hMuon_EffIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hMuon_EffIso_TrCosTh");
    hPion_EffIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hPion_EffIso_TrCosTh");
    hProt_EffIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hProt_EffIso_TrCosTh");
 
    cout << "# of events Muon: " << hMuon_AllIso_TrMomVsTrCosTh[ifile]->Integral() << endl;  
    cout << "# of events Pion: " << hPion_AllIso_TrMomVsTrCosTh[ifile]->Integral() << endl;
    cout << "# of events Prot: " << hProt_AllIso_TrMomVsTrCosTh[ifile]->Integral() << endl;
    //cout << "# of events Elec: " << hElec_AllIso_TrMomVsTrCosTh[ifile]->Integral() << endl;

  
    //
    // Compute efficiencies
    //
    
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->Divide(hMuon_AllIso_TrMomVsTrCosTh[ifile]);
    hPion_EffIso_TrMomVsTrCosTh[ifile]->Divide(hPion_AllIso_TrMomVsTrCosTh[ifile]);
    hProt_EffIso_TrMomVsTrCosTh[ifile]->Divide(hProt_AllIso_TrMomVsTrCosTh[ifile]);
    
    hMuon_EffIso_TrMom[ifile]->Divide(hMuon_AllIso_TrMom[ifile]);
    hPion_EffIso_TrMom[ifile]->Divide(hPion_AllIso_TrMom[ifile]);
    hProt_EffIso_TrMom[ifile]->Divide(hProt_AllIso_TrMom[ifile]);
    
    hMuon_EffIso_TrCosTh[ifile]->Divide(hMuon_AllIso_TrCosTh[ifile]);
    hPion_EffIso_TrCosTh[ifile]->Divide(hPion_AllIso_TrCosTh[ifile]);
    hProt_EffIso_TrCosTh[ifile]->Divide(hProt_AllIso_TrCosTh[ifile]);
  
  }
  

  //
  // Draw 
  //

  TLegend *legend = new TLegend(0.4,0.15,0.8,0.4);
  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetTextFont(132);
  legend->SetTextSize(0.05);
  for(int ifile=0;ifile<NFiles;ifile++){
    legend->AddEntry(hMuon_EffIso_TrMom[ifile],label[ifile].Data(),"l");
  }

  // Muons

  TCanvas *cMuon_AllIso_TrMomVsTrCosTh[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cMuon_AllIso_TrMomVsTrCosTh_%i",ifile);
    cMuon_AllIso_TrMomVsTrCosTh[ifile] = new TCanvas(name,label[ifile].Data());
    hMuon_AllIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitle("True cos #theta");
    hMuon_AllIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
    hMuon_AllIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
    hMuon_AllIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);
    hMuon_AllIso_TrMomVsTrCosTh[ifile]->SetStats(0);
    hMuon_AllIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hMuon_AllIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
    hMuon_AllIso_TrMomVsTrCosTh[ifile]->DrawClone("colz");
  }

  TCanvas *cMuon_EffIso_TrMomVsTrCosTh[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cMuon_EffIso_TrMomVsTrCosTh_%i",ifile);
    cMuon_EffIso_TrMomVsTrCosTh[ifile] = new TCanvas(name,label[ifile].Data());
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitle("True cos #theta");
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->SetStats(0);
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->DrawClone("colz");
  }

  TCanvas *cMuon_EffIso_TrMom = new TCanvas("cMuon_EffIso_TrMom","cMuon_EffIso_TrMom");
  for(int ifile=0;ifile<NFiles;ifile++){
    hMuon_EffIso_TrMom[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
    hMuon_EffIso_TrMom[ifile]->GetYaxis()->SetTitle("Efficiency");
    hMuon_EffIso_TrMom[ifile]->GetYaxis()->SetTitleSize(0.05);
    hMuon_EffIso_TrMom[ifile]->GetXaxis()->SetTitleSize(0.05);
    hMuon_EffIso_TrMom[ifile]->SetStats(0);
    hMuon_EffIso_TrMom[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hMuon_EffIso_TrMom[ifile]->GetYaxis()->SetRangeUser(0,1.1);
    hMuon_EffIso_TrMom[ifile]->SetLineColor(color[ifile]);
    hMuon_EffIso_TrMom[ifile]->SetLineWidth(2.);
    if(ifile==0) hMuon_EffIso_TrMom[ifile]->DrawClone("");
    else         hMuon_EffIso_TrMom[ifile]->DrawClone("same");
  }
  if(doDrawLegend) legend->Draw();

  TCanvas *cMuon_EffIso_TrCosTh = new TCanvas("cMuon_EffIso_TrCosTh","cMuon_EffIso_TrCosTh");
  for(int ifile=0;ifile<NFiles;ifile++){
    hMuon_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
    hMuon_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitle("Efficiency");
    hMuon_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
    hMuon_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);
    hMuon_EffIso_TrCosTh[ifile]->SetStats(0);
    hMuon_EffIso_TrCosTh[ifile]->GetXaxis()->SetRangeUser(-1,+1);
    hMuon_EffIso_TrCosTh[ifile]->GetYaxis()->SetRangeUser(0,1);
    hMuon_EffIso_TrCosTh[ifile]->SetLineColor(color[ifile]);
    hMuon_EffIso_TrCosTh[ifile]->SetLineWidth(2.);
    if(ifile==0) hMuon_EffIso_TrCosTh[ifile]->DrawClone("");
    else         hMuon_EffIso_TrCosTh[ifile]->DrawClone("same");
  }
  if(doDrawLegend) legend->Draw();
  
  



  // Pions

  TCanvas* cPion_AllIso_TrMomVsTrCosTh[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cPion_AllIso_TrMomVsTrCosTh_%i",ifile);
    cPion_AllIso_TrMomVsTrCosTh[ifile] = new TCanvas(name,label[ifile].Data());
    hPion_AllIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitle("True cos #theta");
    hPion_AllIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
    hPion_AllIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
    hPion_AllIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);  
    hPion_AllIso_TrMomVsTrCosTh[ifile]->SetStats(0);
    hPion_AllIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hPion_AllIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
    hPion_AllIso_TrMomVsTrCosTh[ifile]->DrawClone("colz");
  }    

  TCanvas* cPion_EffIso_TrMomVsTrCosTh[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cPion_EffIso_TrMomVsTrCosTh_%i",ifile);
    cPion_EffIso_TrMomVsTrCosTh[ifile] = new TCanvas(name,label[ifile].Data());
    hPion_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitle("True cos #theta");
    hPion_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");  
    hPion_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
    hPion_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);  
    hPion_EffIso_TrMomVsTrCosTh[ifile]->SetStats(0);
    hPion_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hPion_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
    hPion_EffIso_TrMomVsTrCosTh[ifile]->DrawClone("colz");
  }    

  TCanvas *cPion_EffIso_TrMom = new TCanvas("cPion_EffIso_TrMom","cPion_EffIso_TrMom");
  for(int ifile=0;ifile<NFiles;ifile++){
    hPion_EffIso_TrMom[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
    hPion_EffIso_TrMom[ifile]->GetYaxis()->SetTitle("Efficiency");
    hPion_EffIso_TrMom[ifile]->GetYaxis()->SetTitleSize(0.05);
    hPion_EffIso_TrMom[ifile]->GetXaxis()->SetTitleSize(0.05);  
    hPion_EffIso_TrMom[ifile]->SetStats(0);
    hPion_EffIso_TrMom[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hPion_EffIso_TrMom[ifile]->GetYaxis()->SetRangeUser(0,1.1);
    hPion_EffIso_TrMom[ifile]->SetLineColor(color[ifile]);
    hPion_EffIso_TrMom[ifile]->SetLineWidth(2.);
    if(ifile==0) hPion_EffIso_TrMom[ifile]->DrawClone("");
    else         hPion_EffIso_TrMom[ifile]->DrawClone("same");
  }  
  if(doDrawLegend) legend->Draw();

  TCanvas *cPion_EffIso_TrCosTh = new TCanvas("cPion_EffIso_TrCosTh","cPion_EffIso_TrCosTh");
  for(int ifile=0;ifile<NFiles;ifile++){
    hPion_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
    hPion_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitle("Efficiency");
    hPion_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
    hPion_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);  
    hPion_EffIso_TrCosTh[ifile]->SetStats(0);
    hPion_EffIso_TrCosTh[ifile]->GetXaxis()->SetRangeUser(-1,+1);
    hPion_EffIso_TrCosTh[ifile]->GetYaxis()->SetRangeUser(0,1.1);
    hPion_EffIso_TrCosTh[ifile]->SetLineColor(color[ifile]);
    hPion_EffIso_TrCosTh[ifile]->SetLineWidth(2.);
    if(ifile==0) hPion_EffIso_TrCosTh[ifile]->DrawClone("");
    else         hPion_EffIso_TrCosTh[ifile]->DrawClone("same");
  }
  if(doDrawLegend) legend->Draw();



  // Protons

  TCanvas *cProt_AllIso_TrMomVsTrCosTh[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cProt_AllIso_TrMomVsTrCosTh_%i",ifile);
    cProt_AllIso_TrMomVsTrCosTh[ifile] = new TCanvas(name,label[ifile].Data());
    hProt_AllIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitle("True cos #theta");
    hProt_AllIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
    hProt_AllIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
    hProt_AllIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);  
    hProt_AllIso_TrMomVsTrCosTh[ifile]->SetStats(0);
    hProt_AllIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hProt_AllIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
    hProt_AllIso_TrMomVsTrCosTh[ifile]->DrawClone("colz");
  }

  TCanvas *cProt_EffIso_TrMomVsTrCosTh[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cProt_EffIso_TrMomVsTrCosTh_%i",ifile);
    cProt_EffIso_TrMomVsTrCosTh[ifile] = new TCanvas(name,label[ifile].Data());
    hProt_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitle("True cos #theta");
    hProt_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");    
    hProt_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
    hProt_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);  
    hProt_EffIso_TrMomVsTrCosTh[ifile]->SetStats(0);
    hProt_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hProt_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
    hProt_EffIso_TrMomVsTrCosTh[ifile]->DrawClone("colz");
  }

  TCanvas *cProt_EffIso_TrMom = new TCanvas("cProt_EffIso_TrMom","cProt_EffIso_TrMom");
  for(int ifile=0;ifile<NFiles;ifile++){
    hProt_EffIso_TrMom[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
    hProt_EffIso_TrMom[ifile]->GetYaxis()->SetTitle("Efficiency");
    hProt_EffIso_TrMom[ifile]->GetYaxis()->SetTitleSize(0.05);
    hProt_EffIso_TrMom[ifile]->GetXaxis()->SetTitleSize(0.05);  
    hProt_EffIso_TrMom[ifile]->SetStats(0);
    hProt_EffIso_TrMom[ifile]->GetXaxis()->SetRangeUser(0,2000);
    hProt_EffIso_TrMom[ifile]->GetYaxis()->SetRangeUser(0,1.1);
    hProt_EffIso_TrMom[ifile]->SetLineColor(color[ifile]);
    hProt_EffIso_TrMom[ifile]->SetLineWidth(2.);
    if(ifile==0) hProt_EffIso_TrMom[ifile]->DrawClone("");
    else         hProt_EffIso_TrMom[ifile]->DrawClone("same");
  }
  if(doDrawLegend) legend->Draw();

  TCanvas *cProt_EffIso_TrCosTh = new TCanvas("cProt_EffIso_TrCosTh","cProt_EffIso_TrCosTh");
  for(int ifile=0;ifile<NFiles;ifile++){
    hProt_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
    hProt_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitle("Efficiency");
    hProt_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
    hProt_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);  
    hProt_EffIso_TrCosTh[ifile]->SetStats(0);
    hProt_EffIso_TrCosTh[ifile]->GetXaxis()->SetRangeUser(-1,+1);
    hProt_EffIso_TrCosTh[ifile]->GetYaxis()->SetRangeUser(0,1);
    hProt_EffIso_TrCosTh[ifile]->SetLineColor(color[ifile]);
    hProt_EffIso_TrCosTh[ifile]->SetLineWidth(2.);
    if(ifile==0) hProt_EffIso_TrCosTh[ifile]->DrawClone("");
    else         hProt_EffIso_TrCosTh[ifile]->DrawClone("same");
  }
  if(doDrawLegend) legend->Draw();
  

  // Store efficiencies in output ROOT file
  if(doStore){

    TString name = TString::Format("TargetEfficiency_%s.root",tag.Data());
    TFile *outfile = new TFile(name,"RECREATE");

    for(int ifile=0;ifile<NFiles;ifile++){

      name = TString::Format("%s_TrueMomVsTrueCosTh_IsoTarget_Muon",label[ifile].Data());
      hMuon_EffIso_TrMomVsTrCosTh[ifile]->SetName(name);
      hMuon_EffIso_TrMomVsTrCosTh[ifile]->Write();

      name = TString::Format("%s_TrueMomVsTrueCosTh_IsoTarget_Pion",label[ifile].Data());
      hPion_EffIso_TrMomVsTrCosTh[ifile]->SetName(name);    
      hPion_EffIso_TrMomVsTrCosTh[ifile]->Write();    

      name = TString::Format("%s_TrueMomVsTrueCosTh_IsoTarget_Prot",label[ifile].Data());
      hProt_EffIso_TrMomVsTrCosTh[ifile]->SetName(name);
      hProt_EffIso_TrMomVsTrCosTh[ifile]->Write();
    }

  }

  return;
}
