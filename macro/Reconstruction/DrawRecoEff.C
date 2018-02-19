
void DrawRecoEff
( 
 bool doMuons = true,
 bool doPions = false,
 bool doProtons = false,
 bool doElectrons = false,
 bool doPrint = true,
 bool doStore = true,
 TString tag = "prova",
 bool doDrawLegend = true
  )
{
 
  const int NFiles = 1;
  string infilename[NFiles] = {

    "SuperFGD-NuMu-UseXY-UseXZ-UseYZ-TruthFV-Separate10_Evt0_NEvt10000.root"

    // Not Full Stat
    //"RECONSTRUCTION/16_11_17/USETRACKLENGTH_IN_PID/SuperFGD_numu.root",
    //"RECONSTRUCTION/16_11_17/USETRACKLENGTH_IN_PID/SuperFGD_XZ_numu.root"
    //"RECONSTRUCTION/16_11_17/USETRACKLENGTH_IN_PID/SuperFGD_nue.root"
    
    //"SuperFGD-NuMu-Plastic-UseXY-UseXZ-UseYZ-TruthFV-Separate10_Evt0_NEvt2000.root"
    //"SuperFGD-NuMu-Plastic-UseXY-UseXZ-UseYZ-TruthFV-Separate10_Evt0_NEvt1000.root"
    
    //"RECONSTRUCTION/16_11_17/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E22POT_NuE.root",
    //"RECONSTRUCTION/16_11_17/FGDXZ-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E22POT_NuE.root"

    // Full Stat
    //"RECONSTRUCTION/16_11_17/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT_NuMu.root",
    //"RECONSTRUCTION/16_11_17/FGDXZ-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT_NuMu.root"
    
    //"RECONSTRUCTION/5_10_17/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT_NuMu.root"
    //"RECONSTRUCTION/5_10_17/FGDlike-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT_NuMu.root"
    
  };

  int color[NFiles] = {
    //kBlue,
    kRed
    //kGreen
    //kOrange
    //kCyan
  };

  int style[NFiles] = {
    //1,
    2
  };

  TString label[NFiles] = {

    //"CutLayers30mm",
    //"CutLayers40mm"

    //"OK",
    //"Uniform"
    
    //"NuE",
    //"NuMu"

    "Scint. cubes"
    //,"Scint. cubes XZ"
    //,"Scint. bars XZ"
    
    //"SuperFGD",
    //"SuperFGD-XZ",
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
  TH2D *hElec_AllIso_TrMomVsTrCosTh[NFiles];

  TH2D *hMuon_EffIso_TrMomVsTrCosTh[NFiles];
  TH2D *hPion_EffIso_TrMomVsTrCosTh[NFiles];
  TH2D *hProt_EffIso_TrMomVsTrCosTh[NFiles];
  TH2D *hElec_EffIso_TrMomVsTrCosTh[NFiles];

  TH2D *hMuon_AllIso_TrMomVsTrPhi[NFiles];
  TH2D *hPion_AllIso_TrMomVsTrPhi[NFiles];
  TH2D *hProt_AllIso_TrMomVsTrPhi[NFiles];
  TH2D *hElec_AllIso_TrMomVsTrPhi[NFiles];

  TH2D *hMuon_EffIso_TrMomVsTrPhi[NFiles];
  TH2D *hPion_EffIso_TrMomVsTrPhi[NFiles];
  TH2D *hProt_EffIso_TrMomVsTrPhi[NFiles];
  TH2D *hElec_EffIso_TrMomVsTrPhi[NFiles];

  TH2D *hMuon_AllIso_TrPhiVsTrCosTh[NFiles];
  TH2D *hPion_AllIso_TrPhiVsTrCosTh[NFiles];
  TH2D *hProt_AllIso_TrPhiVsTrCosTh[NFiles];
  TH2D *hElec_AllIso_TrPhiVsTrCosTh[NFiles];

  TH2D *hMuon_EffIso_TrPhiVsTrCosTh[NFiles];
  TH2D *hPion_EffIso_TrPhiVsTrCosTh[NFiles];
  TH2D *hProt_EffIso_TrPhiVsTrCosTh[NFiles];
  TH2D *hElec_EffIso_TrPhiVsTrCosTh[NFiles];

  TH1D *hMuon_AllIso_TrMom[NFiles];
  TH1D *hPion_AllIso_TrMom[NFiles];
  TH1D *hProt_AllIso_TrMom[NFiles];
  TH1D *hElec_AllIso_TrMom[NFiles];
  
  TH1D *hMuon_EffIso_TrMom[NFiles];
  TH1D *hPion_EffIso_TrMom[NFiles];
  TH1D *hProt_EffIso_TrMom[NFiles];
  TH1D *hElec_EffIso_TrMom[NFiles];
  
  TH1D *hMuon_AllIso_TrCosTh[NFiles];
  TH1D *hPion_AllIso_TrCosTh[NFiles];
  TH1D *hProt_AllIso_TrCosTh[NFiles];
  TH1D *hElec_AllIso_TrCosTh[NFiles];

  TH1D *hMuon_EffIso_TrCosTh[NFiles];
  TH1D *hPion_EffIso_TrCosTh[NFiles];
  TH1D *hProt_EffIso_TrCosTh[NFiles];
  TH1D *hElec_EffIso_TrCosTh[NFiles];

  TH1D *hMuon_AllIso_TrPhi[NFiles];
  TH1D *hPion_AllIso_TrPhi[NFiles];
  TH1D *hProt_AllIso_TrPhi[NFiles];
  TH1D *hElec_AllIso_TrPhi[NFiles];
  
  TH1D *hMuon_EffIso_TrPhi[NFiles];
  TH1D *hPion_EffIso_TrPhi[NFiles];
  TH1D *hProt_EffIso_TrPhi[NFiles];
  TH1D *hElec_EffIso_TrPhi[NFiles];
  
  
  for(int ifile=0;ifile<NFiles;ifile++){
    
    cout << "Read file: " << infilename[ifile].c_str() << endl;
    
    infile[ifile] = new TFile(infilename[ifile].c_str(),"READ");
    
    hMuon_AllIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hMuon_AllIso_TrMomVsTrCosTh");    
    hPion_AllIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hPion_AllIso_TrMomVsTrCosTh");
    hProt_AllIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hProt_AllIso_TrMomVsTrCosTh");
    hElec_AllIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hElec_AllIso_TrMomVsTrCosTh");    
    
    hMuon_EffIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hMuon_EffIso_TrMomVsTrCosTh");
    hPion_EffIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hPion_EffIso_TrMomVsTrCosTh");
    hProt_EffIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hProt_EffIso_TrMomVsTrCosTh");
    hElec_EffIso_TrMomVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hElec_EffIso_TrMomVsTrCosTh");

    hMuon_AllIso_TrMomVsTrPhi[ifile] = (TH2D*)infile[ifile]->Get("hMuon_AllIso_TrMomVsTrPhi");    
    hPion_AllIso_TrMomVsTrPhi[ifile] = (TH2D*)infile[ifile]->Get("hPion_AllIso_TrMomVsTrPhi");
    hProt_AllIso_TrMomVsTrPhi[ifile] = (TH2D*)infile[ifile]->Get("hProt_AllIso_TrMomVsTrPhi");
    hElec_AllIso_TrMomVsTrPhi[ifile] = (TH2D*)infile[ifile]->Get("hElec_AllIso_TrMomVsTrPhi");    
    
    hMuon_EffIso_TrMomVsTrPhi[ifile] = (TH2D*)infile[ifile]->Get("hMuon_EffIso_TrMomVsTrPhi");
    hPion_EffIso_TrMomVsTrPhi[ifile] = (TH2D*)infile[ifile]->Get("hPion_EffIso_TrMomVsTrPhi");
    hProt_EffIso_TrMomVsTrPhi[ifile] = (TH2D*)infile[ifile]->Get("hProt_EffIso_TrMomVsTrPhi");
    hElec_EffIso_TrMomVsTrPhi[ifile] = (TH2D*)infile[ifile]->Get("hElec_EffIso_TrMomVsTrPhi");

    hMuon_AllIso_TrPhiVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hMuon_AllIso_TrPhiVsTrCosTh");    
    hPion_AllIso_TrPhiVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hPion_AllIso_TrPhiVsTrCosTh");
    hProt_AllIso_TrPhiVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hProt_AllIso_TrPhiVsTrCosTh");
    hElec_AllIso_TrPhiVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hElec_AllIso_TrPhiVsTrCosTh");    
    
    hMuon_EffIso_TrPhiVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hMuon_EffIso_TrPhiVsTrCosTh");
    hPion_EffIso_TrPhiVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hPion_EffIso_TrPhiVsTrCosTh");
    hProt_EffIso_TrPhiVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hProt_EffIso_TrPhiVsTrCosTh");
    hElec_EffIso_TrPhiVsTrCosTh[ifile] = (TH2D*)infile[ifile]->Get("hElec_EffIso_TrPhiVsTrCosTh");

    hMuon_AllIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hMuon_AllIso_TrMom");
    hPion_AllIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hPion_AllIso_TrMom");
    hProt_AllIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hProt_AllIso_TrMom");
    hElec_AllIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hElec_AllIso_TrMom");
    
    hMuon_EffIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hMuon_EffIso_TrMom");
    hPion_EffIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hPion_EffIso_TrMom");
    hProt_EffIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hProt_EffIso_TrMom");
    hElec_EffIso_TrMom[ifile] = (TH1D*)infile[ifile]->Get("hElec_EffIso_TrMom");
    
    hMuon_AllIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hMuon_AllIso_TrCosTh");
    hPion_AllIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hPion_AllIso_TrCosTh");
    hProt_AllIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hProt_AllIso_TrCosTh");
    hElec_AllIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hElec_AllIso_TrCosTh");

    hMuon_EffIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hMuon_EffIso_TrCosTh");
    hPion_EffIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hPion_EffIso_TrCosTh");
    hProt_EffIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hProt_EffIso_TrCosTh");
    hElec_EffIso_TrCosTh[ifile] = (TH1D*)infile[ifile]->Get("hElec_EffIso_TrCosTh");

    hMuon_AllIso_TrPhi[ifile] = (TH1D*)infile[ifile]->Get("hMuon_AllIso_TrPhi");
    hPion_AllIso_TrPhi[ifile] = (TH1D*)infile[ifile]->Get("hPion_AllIso_TrPhi");
    hProt_AllIso_TrPhi[ifile] = (TH1D*)infile[ifile]->Get("hProt_AllIso_TrPhi");
    hElec_AllIso_TrPhi[ifile] = (TH1D*)infile[ifile]->Get("hElec_AllIso_TrPhi");
    
    hMuon_EffIso_TrPhi[ifile] = (TH1D*)infile[ifile]->Get("hMuon_EffIso_TrPhi");
    hPion_EffIso_TrPhi[ifile] = (TH1D*)infile[ifile]->Get("hPion_EffIso_TrPhi");
    hProt_EffIso_TrPhi[ifile] = (TH1D*)infile[ifile]->Get("hProt_EffIso_TrPhi");
    hElec_EffIso_TrPhi[ifile] = (TH1D*)infile[ifile]->Get("hElec_EffIso_TrPhi");
 
    cout << "# of events Muon: " << hMuon_AllIso_TrMomVsTrCosTh[ifile]->Integral() << endl;  
    cout << "# of events Pion: " << hPion_AllIso_TrMomVsTrCosTh[ifile]->Integral() << endl;
    cout << "# of events Prot: " << hProt_AllIso_TrMomVsTrCosTh[ifile]->Integral() << endl;
    cout << "# of events Elec: " << hElec_AllIso_TrMomVsTrCosTh[ifile]->Integral() << endl;

  
    //
    // Compute efficiencies
    //
    
    hMuon_EffIso_TrMomVsTrCosTh[ifile]->Divide(hMuon_AllIso_TrMomVsTrCosTh[ifile]);
    hPion_EffIso_TrMomVsTrCosTh[ifile]->Divide(hPion_AllIso_TrMomVsTrCosTh[ifile]);
    hProt_EffIso_TrMomVsTrCosTh[ifile]->Divide(hProt_AllIso_TrMomVsTrCosTh[ifile]);
    hElec_EffIso_TrMomVsTrCosTh[ifile]->Divide(hElec_AllIso_TrMomVsTrCosTh[ifile]);

    hMuon_EffIso_TrMomVsTrPhi[ifile]->Divide(hMuon_AllIso_TrMomVsTrPhi[ifile]);
    hPion_EffIso_TrMomVsTrPhi[ifile]->Divide(hPion_AllIso_TrMomVsTrPhi[ifile]);
    hProt_EffIso_TrMomVsTrPhi[ifile]->Divide(hProt_AllIso_TrMomVsTrPhi[ifile]);
    hElec_EffIso_TrMomVsTrPhi[ifile]->Divide(hElec_AllIso_TrMomVsTrPhi[ifile]);

    hMuon_EffIso_TrPhiVsTrCosTh[ifile]->Divide(hMuon_AllIso_TrPhiVsTrCosTh[ifile]);
    hPion_EffIso_TrPhiVsTrCosTh[ifile]->Divide(hPion_AllIso_TrPhiVsTrCosTh[ifile]);
    hProt_EffIso_TrPhiVsTrCosTh[ifile]->Divide(hProt_AllIso_TrPhiVsTrCosTh[ifile]);
    hElec_EffIso_TrPhiVsTrCosTh[ifile]->Divide(hElec_AllIso_TrPhiVsTrCosTh[ifile]);
    
    hMuon_EffIso_TrMom[ifile]->Divide(hMuon_AllIso_TrMom[ifile]);
    hPion_EffIso_TrMom[ifile]->Divide(hPion_AllIso_TrMom[ifile]);
    hProt_EffIso_TrMom[ifile]->Divide(hProt_AllIso_TrMom[ifile]);
    hElec_EffIso_TrMom[ifile]->Divide(hElec_AllIso_TrMom[ifile]);

    hMuon_EffIso_TrPhi[ifile]->Divide(hMuon_AllIso_TrPhi[ifile]);
    hPion_EffIso_TrPhi[ifile]->Divide(hPion_AllIso_TrPhi[ifile]);
    hProt_EffIso_TrPhi[ifile]->Divide(hProt_AllIso_TrPhi[ifile]);
    hElec_EffIso_TrPhi[ifile]->Divide(hElec_AllIso_TrPhi[ifile]);
    
    hMuon_EffIso_TrCosTh[ifile]->Divide(hMuon_AllIso_TrCosTh[ifile]);
    hPion_EffIso_TrCosTh[ifile]->Divide(hPion_AllIso_TrCosTh[ifile]);
    hProt_EffIso_TrCosTh[ifile]->Divide(hProt_AllIso_TrCosTh[ifile]);
    hElec_EffIso_TrCosTh[ifile]->Divide(hElec_AllIso_TrCosTh[ifile]);  
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
    if(doMuons)          legend->AddEntry(hMuon_EffIso_TrMom[ifile],label[ifile].Data(),"l");
    else if(doPions)     legend->AddEntry(hPion_EffIso_TrMom[ifile],label[ifile].Data(),"l");
    else if(doProtons)   legend->AddEntry(hProt_EffIso_TrMom[ifile],label[ifile].Data(),"l");
    else if(doElectrons) legend->AddEntry(hElec_EffIso_TrMom[ifile],label[ifile].Data(),"l");
  }

  // Muons

  if(doMuons){
    
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

    TCanvas *cMuon_AllIso_TrPhiVsTrCosTh[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      TString name = TString::Format("cMuon_AllIso_TrPhiVsTrCosTh_%i",ifile);
      cMuon_AllIso_TrPhiVsTrCosTh[ifile] = new TCanvas(name,label[ifile].Data());
      hMuon_AllIso_TrPhiVsTrCosTh[ifile]->GetYaxis()->SetTitle("True cos #theta");
      hMuon_AllIso_TrPhiVsTrCosTh[ifile]->GetXaxis()->SetTitle("True #phi (rad)");
      hMuon_AllIso_TrPhiVsTrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
      hMuon_AllIso_TrPhiVsTrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);
      hMuon_AllIso_TrPhiVsTrCosTh[ifile]->SetStats(0);
      hMuon_AllIso_TrPhiVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
      hMuon_AllIso_TrPhiVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
      hMuon_AllIso_TrPhiVsTrCosTh[ifile]->DrawClone("colz");
    }
    
    TCanvas *cMuon_EffIso_TrPhiVsTrCosTh[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      TString name = TString::Format("cMuon_EffIso_TrPhiVsTrCosTh_%i",ifile);
      cMuon_EffIso_TrPhiVsTrCosTh[ifile] = new TCanvas(name,label[ifile].Data());
      hMuon_EffIso_TrPhiVsTrCosTh[ifile]->GetYaxis()->SetTitle("True cos #theta");
      hMuon_EffIso_TrPhiVsTrCosTh[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
      hMuon_EffIso_TrPhiVsTrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
      hMuon_EffIso_TrPhiVsTrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);
      hMuon_EffIso_TrPhiVsTrCosTh[ifile]->SetStats(0);
      hMuon_EffIso_TrPhiVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
      hMuon_EffIso_TrPhiVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
      hMuon_EffIso_TrPhiVsTrCosTh[ifile]->DrawClone("colz");
    }
    
    TCanvas *cMuon_EffIso_TrMom = new TCanvas("cMuon_EffIso_TrMom","cMuon_EffIso_TrMom");
    cMuon_EffIso_TrMom->SetTicky();
    for(int ifile=0;ifile<NFiles;ifile++){
      hMuon_EffIso_TrMom[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
      hMuon_EffIso_TrMom[ifile]->GetYaxis()->SetTitle("Efficiency");
      hMuon_EffIso_TrMom[ifile]->GetYaxis()->SetTitleSize(0.05);
      hMuon_EffIso_TrMom[ifile]->GetXaxis()->SetTitleSize(0.05);
      hMuon_EffIso_TrMom[ifile]->SetStats(0);
      hMuon_EffIso_TrMom[ifile]->GetXaxis()->SetRangeUser(0,2000);
      hMuon_EffIso_TrMom[ifile]->GetYaxis()->SetRangeUser(0,1.1);
      hMuon_EffIso_TrMom[ifile]->SetLineColor(color[ifile]);
      hMuon_EffIso_TrMom[ifile]->SetLineStyle(style[ifile]);
      hMuon_EffIso_TrMom[ifile]->SetLineWidth(2.);
      if(ifile==0) hMuon_EffIso_TrMom[ifile]->DrawClone("");
      else         hMuon_EffIso_TrMom[ifile]->DrawClone("same");
    }
    if(doDrawLegend) legend->Draw(); 
    
    TCanvas *cMuon_EffIso_TrCosTh = new TCanvas("cMuon_EffIso_TrCosTh","cMuon_EffIso_TrCosTh");
    cMuon_EffIso_TrCosTh->SetTicky();
    for(int ifile=0;ifile<NFiles;ifile++){
      hMuon_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
      hMuon_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitle("Efficiency");
      hMuon_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
      hMuon_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);
      hMuon_EffIso_TrCosTh[ifile]->SetStats(0);
      hMuon_EffIso_TrCosTh[ifile]->GetXaxis()->SetRangeUser(-1,+1);
      hMuon_EffIso_TrCosTh[ifile]->GetYaxis()->SetRangeUser(0,1);
      hMuon_EffIso_TrCosTh[ifile]->SetLineColor(color[ifile]);
      hMuon_EffIso_TrCosTh[ifile]->SetLineStyle(style[ifile]);
      hMuon_EffIso_TrCosTh[ifile]->SetLineWidth(2.);
      if(ifile==0) hMuon_EffIso_TrCosTh[ifile]->DrawClone("");
      else         hMuon_EffIso_TrCosTh[ifile]->DrawClone("same");
    }
    if(doDrawLegend) legend->Draw();


    TCanvas *cMuon_EffIso_TrPhi = new TCanvas("cMuon_EffIso_TrPhi","cMuon_EffIso_TrPhi");
    cMuon_EffIso_TrPhi->SetTicky();
    for(int ifile=0;ifile<NFiles;ifile++){
      hMuon_EffIso_TrPhi[ifile]->GetXaxis()->SetTitle("True #phi (rad)");
      hMuon_EffIso_TrPhi[ifile]->GetYaxis()->SetTitle("Efficiency");
      hMuon_EffIso_TrPhi[ifile]->GetYaxis()->SetTitleSize(0.05);
      hMuon_EffIso_TrPhi[ifile]->GetXaxis()->SetTitleSize(0.05);
      hMuon_EffIso_TrPhi[ifile]->SetStats(0);
      hMuon_EffIso_TrPhi[ifile]->GetXaxis()->SetRangeUser(0,2000);
      hMuon_EffIso_TrPhi[ifile]->GetYaxis()->SetRangeUser(0,1.1);
      hMuon_EffIso_TrPhi[ifile]->SetLineColor(color[ifile]);
      hMuon_EffIso_TrPhi[ifile]->SetLineStyle(style[ifile]);
      hMuon_EffIso_TrPhi[ifile]->SetLineWidth(2.);
      if(ifile==0) hMuon_EffIso_TrPhi[ifile]->DrawClone("");
      else         hMuon_EffIso_TrPhi[ifile]->DrawClone("same");
    }
    if(doDrawLegend) legend->Draw(); 
  }
    


  // SONO ARRIVATO QUI



  // Pions

  if(doPions){
    
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
    cPion_EffIso_TrMom->SetTicky();
    for(int ifile=0;ifile<NFiles;ifile++){
      hPion_EffIso_TrMom[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
      hPion_EffIso_TrMom[ifile]->GetYaxis()->SetTitle("Efficiency");
      hPion_EffIso_TrMom[ifile]->GetYaxis()->SetTitleSize(0.05);
      hPion_EffIso_TrMom[ifile]->GetXaxis()->SetTitleSize(0.05);  
      hPion_EffIso_TrMom[ifile]->SetStats(0);
      hPion_EffIso_TrMom[ifile]->GetXaxis()->SetRangeUser(0,2000);
      hPion_EffIso_TrMom[ifile]->GetYaxis()->SetRangeUser(0,1.1);
      hPion_EffIso_TrMom[ifile]->SetLineColor(color[ifile]);
      hPion_EffIso_TrMom[ifile]->SetLineStyle(style[ifile]);
      hPion_EffIso_TrMom[ifile]->SetLineWidth(2.);
      if(ifile==0) hPion_EffIso_TrMom[ifile]->DrawClone("");
      else         hPion_EffIso_TrMom[ifile]->DrawClone("same");
    }  
    if(doDrawLegend) legend->Draw();
    
    TCanvas *cPion_EffIso_TrCosTh = new TCanvas("cPion_EffIso_TrCosTh","cPion_EffIso_TrCosTh");
    cPion_EffIso_TrCosTh->SetTicky();
    for(int ifile=0;ifile<NFiles;ifile++){
      hPion_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
      hPion_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitle("Efficiency");
      hPion_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
      hPion_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);  
      hPion_EffIso_TrCosTh[ifile]->SetStats(0);
      hPion_EffIso_TrCosTh[ifile]->GetXaxis()->SetRangeUser(-1,+1);
      hPion_EffIso_TrCosTh[ifile]->GetYaxis()->SetRangeUser(0,1.1);
      hPion_EffIso_TrCosTh[ifile]->SetLineColor(color[ifile]);
      hPion_EffIso_TrCosTh[ifile]->SetLineStyle(style[ifile]);
      hPion_EffIso_TrCosTh[ifile]->SetLineWidth(2.);
      if(ifile==0) hPion_EffIso_TrCosTh[ifile]->DrawClone("");
      else         hPion_EffIso_TrCosTh[ifile]->DrawClone("same");
    }
    if(doDrawLegend) legend->Draw();
  }


  // Protons

  if(doProtons){
    
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
    cProt_EffIso_TrMom->SetTicky();
    for(int ifile=0;ifile<NFiles;ifile++){
      hProt_EffIso_TrMom[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
      hProt_EffIso_TrMom[ifile]->GetYaxis()->SetTitle("Efficiency");
      hProt_EffIso_TrMom[ifile]->GetYaxis()->SetTitleSize(0.05);
      hProt_EffIso_TrMom[ifile]->GetXaxis()->SetTitleSize(0.05);  
      hProt_EffIso_TrMom[ifile]->SetStats(0);
      hProt_EffIso_TrMom[ifile]->GetXaxis()->SetRangeUser(0,2000);
      hProt_EffIso_TrMom[ifile]->GetYaxis()->SetRangeUser(0,1.1);
      hProt_EffIso_TrMom[ifile]->SetLineColor(color[ifile]);
      hProt_EffIso_TrMom[ifile]->SetLineStyle(style[ifile]);
      hProt_EffIso_TrMom[ifile]->SetLineWidth(2.);
      if(ifile==0) hProt_EffIso_TrMom[ifile]->DrawClone("");
      else         hProt_EffIso_TrMom[ifile]->DrawClone("same");
    }
    if(doDrawLegend) legend->Draw();
    
    TCanvas *cProt_EffIso_TrCosTh = new TCanvas("cProt_EffIso_TrCosTh","cProt_EffIso_TrCosTh");
    cProt_EffIso_TrCosTh->SetTicky();
    for(int ifile=0;ifile<NFiles;ifile++){
      hProt_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
      hProt_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitle("Efficiency");
      hProt_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
      hProt_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);  
      hProt_EffIso_TrCosTh[ifile]->SetStats(0);
      hProt_EffIso_TrCosTh[ifile]->GetXaxis()->SetRangeUser(-1,+1);
      hProt_EffIso_TrCosTh[ifile]->GetYaxis()->SetRangeUser(0,1);
      hProt_EffIso_TrCosTh[ifile]->SetLineColor(color[ifile]);
      hProt_EffIso_TrCosTh[ifile]->SetLineStyle(style[ifile]);
      hProt_EffIso_TrCosTh[ifile]->SetLineWidth(2.);
      if(ifile==0) hProt_EffIso_TrCosTh[ifile]->DrawClone("");
      else         hProt_EffIso_TrCosTh[ifile]->DrawClone("same");
    }
    if(doDrawLegend) legend->Draw();
  }
    

  
  // Electron
  
  if(doElectrons){
    
    TCanvas *cElec_AllIso_TrMomVsTrCosTh[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      TString name = TString::Format("cElec_AllIso_TrMomVsTrCosTh_%i",ifile);
      cElec_AllIso_TrMomVsTrCosTh[ifile] = new TCanvas(name,label[ifile].Data());
      hElec_AllIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitle("True cos #theta");
      hElec_AllIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
      hElec_AllIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
      hElec_AllIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);
      hElec_AllIso_TrMomVsTrCosTh[ifile]->SetStats(0);
      hElec_AllIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
      hElec_AllIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
      hElec_AllIso_TrMomVsTrCosTh[ifile]->DrawClone("colz");
    }
    
    TCanvas *cElec_EffIso_TrMomVsTrCosTh[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      TString name = TString::Format("cElec_EffIso_TrMomVsTrCosTh_%i",ifile);
      cElec_EffIso_TrMomVsTrCosTh[ifile] = new TCanvas(name,label[ifile].Data());
      hElec_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitle("True cos #theta");
      hElec_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
      hElec_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
      hElec_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);
      hElec_EffIso_TrMomVsTrCosTh[ifile]->SetStats(0);
      hElec_EffIso_TrMomVsTrCosTh[ifile]->GetXaxis()->SetRangeUser(0,2000);
      hElec_EffIso_TrMomVsTrCosTh[ifile]->GetYaxis()->SetRangeUser(-1,+1);
      hElec_EffIso_TrMomVsTrCosTh[ifile]->DrawClone("colz");
    }
    
    TCanvas *cElec_EffIso_TrMom = new TCanvas("cElec_EffIso_TrMom","cElec_EffIso_TrMom");
    cElec_EffIso_TrMom->SetTicky();
    for(int ifile=0;ifile<NFiles;ifile++){
      hElec_EffIso_TrMom[ifile]->GetXaxis()->SetTitle("True Momentum (MeV/c)");
      hElec_EffIso_TrMom[ifile]->GetYaxis()->SetTitle("Efficiency");
      hElec_EffIso_TrMom[ifile]->GetYaxis()->SetTitleSize(0.05);
      hElec_EffIso_TrMom[ifile]->GetXaxis()->SetTitleSize(0.05);
      hElec_EffIso_TrMom[ifile]->SetStats(0);
      hElec_EffIso_TrMom[ifile]->GetXaxis()->SetRangeUser(0,2000);
      hElec_EffIso_TrMom[ifile]->GetYaxis()->SetRangeUser(0,1.1);
      hElec_EffIso_TrMom[ifile]->SetLineColor(color[ifile]);
      hElec_EffIso_TrMom[ifile]->SetLineStyle(style[ifile]);
      hElec_EffIso_TrMom[ifile]->SetLineWidth(2.);
      if(ifile==0) hElec_EffIso_TrMom[ifile]->DrawClone("");
      else         hElec_EffIso_TrMom[ifile]->DrawClone("same");
    }
    if(doDrawLegend) legend->Draw();
    
    TCanvas *cElec_EffIso_TrCosTh = new TCanvas("cElec_EffIso_TrCosTh","cElec_EffIso_TrCosTh");
    cElec_EffIso_TrCosTh->SetTicky();
    for(int ifile=0;ifile<NFiles;ifile++){
      hElec_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitle("True cos #theta");
      hElec_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitle("Efficiency");
      hElec_EffIso_TrCosTh[ifile]->GetYaxis()->SetTitleSize(0.05);
      hElec_EffIso_TrCosTh[ifile]->GetXaxis()->SetTitleSize(0.05);
      hElec_EffIso_TrCosTh[ifile]->SetStats(0);
      hElec_EffIso_TrCosTh[ifile]->GetXaxis()->SetRangeUser(-1,+1);
      hElec_EffIso_TrCosTh[ifile]->GetYaxis()->SetRangeUser(0,1);
      hElec_EffIso_TrCosTh[ifile]->SetLineColor(color[ifile]);
      hElec_EffIso_TrCosTh[ifile]->SetLineStyle(style[ifile]);
      hElec_EffIso_TrCosTh[ifile]->SetLineWidth(2.);
      if(ifile==0) hElec_EffIso_TrCosTh[ifile]->DrawClone("");
      else         hElec_EffIso_TrCosTh[ifile]->DrawClone("same");
    }
    if(doDrawLegend) legend->Draw();
  }


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

      name = TString::Format("%s_TrueMomVsTrueCosTh_IsoTarget_Elec",label[ifile].Data());
      hElec_EffIso_TrMomVsTrCosTh[ifile]->SetName(name);
      hElec_EffIso_TrMomVsTrCosTh[ifile]->Write();
    }

  }

  return;
}
