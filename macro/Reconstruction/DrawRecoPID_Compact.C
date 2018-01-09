
#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH2F.h>
#include <TLine.h>

#include <iostream>

using namespace std;

const int NFiles = 1;
const int NBinsMom = 6; // to check with # of bins in hBinsMom
const int NBinsLen = 5; // to check with # of bins in hBinsLen

double GetPvalue_Histo_rightside(TH1F *histo,double data);
double GetPvalue_Histo_leftside(TH1F *histo,double data);

void GetPvalue_bins_mom(TH1F *hHypLeft[NFiles][NBinsMom],
			double cutLeft[NFiles],
			TH1F *hHypRight[NFiles][NBinsMom],
			double cutRigth[NFiles],
			TH1F *hBins[NFiles],
			TH1F *hMisidLeft[NFiles],
			TH1F *hMisidRight[NFiles]);

void DrawRecoPID
( 
 const bool UseManyBins = false,
 const bool doPrint = true,
 const bool doStore = false,
 const TString tag = "prova",
 const bool doDrawLegend = true,
 const int RebinNum = 1 //200
  )
{
 
  //const int NFiles = 1;
  string infilename[NFiles] = {
    
    "RECONSTRUCTION/16_11_17/USETRACKLENGTH/ciao.root"
    
    //"SuperFGD-NuMu-GasAr-UseXY-UseXZ-UseYZ-TruthFV-Separate10_Evt0_NEvt1000.root"
    //"SuperFGD-NuMu-Plastic-UseXY-UseXZ-UseYZ-TruthFV-Separate10_Evt0_NEvt1000.root"
        
    //"RECONSTRUCTION/16_11_17/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT_NuMu.root",
    //"RECONSTRUCTION/16_11_17/FGDXZ-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT_NuMu.root"
    
    //"RECONSTRUCTION/5_10_17/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT_NuMu.root",
    //"RECONSTRUCTION/5_10_17/FGDlike-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT_NuMu.root"
  };


  string infilename_Elec[NFiles] = {

    "RECONSTRUCTION/16_11_17/USETRACKLENGTH/ciao_nue.root"

    //"SuperFGD-NuE-GasAr-UseXY-UseXZ-UseYZ-TruthFV-Separate10_Evt0_NEvt1000.root"
    //"SuperFGD-NuE-Plastic-UseXY-UseXZ-UseYZ-TruthFV-Separate10_Evt0_NEvt1000.root"
    
    //"RECONSTRUCTION/16_11_17/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E22POT_NuE.root",
    //"RECONSTRUCTION/16_11_17/FGDXZ-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E22POT_NuE.root"
  };
  
  double cut_MuonVsProt[NFiles] = {
    32.  // SuperFGD
    //,
    //8.2 // FGD XZ
  };

  double cut_MuonVsPion[NFiles] = {
    14. // SuperFGD
    //,
    //3.9 // FGD XZ
  };

  double cut_PionVsProt[NFiles] = {
    33.5 // SuperFGD
    //,
    //8.9 // FGD XZ
  };

  double cut_ElecVsMuon[NFiles] = {
    13.6 // SuperFGD
    //,
    //3.9 // FGD XZ
  }; 

 double cut_ElecVsPion[NFiles] = {
   13.25 // SuperFGD
   //,
   //3.8 // FGD XZ
  }; 

 double cut_ElecVsProt[NFiles] = {
   14.6 // SuperFGD
   //,
   //4.7 // FGD XZ
 }; 

  TString label[NFiles] = {
    "SuperFGD"    
    //,
    //"FGDXZ"
  };

  // gROOT->SetStyle("T2K");                  
  // gROOT->ForceStyle();                 
  // gStyle->SetPadLeftMargin(0.14);
  // gStyle->SetPadRightMargin(0.15);
  // //gStyle->SetStatX(.4); gStyle->SetStatY(.6);

  TFile *infile[NFiles];
  TFile *infile_Elec[NFiles];

  /*
  TH1F *hLikeRatio_MuProt_TrueMu[NFiles];
  TH1F *hLikeRatio_MuProt_TrueProt[NFiles];   
  TH1F *hLikeRatio_MuPion_TrueMu[NFiles];
  TH1F *hLikeRatio_MuPion_TruePion[NFiles];   
  TH1F *hLikeRatio_PionProt_TruePion[NFiles];
  TH1F *hLikeRatio_PionProt_TrueProt[NFiles];
  */

  
  // Graphs
  
  TGraph *gMuon_Stopped_EdepVsLen[NFiles];
  TGraph *gPion_Stopped_EdepVsLen[NFiles];
  TGraph *gProt_Stopped_EdepVsLen[NFiles];
  TGraph *gElec_Stopped_EdepVsLen[NFiles];

  TGraph *gMuon_Stopped_EdepVsLen_Truth[NFiles];
  TGraph *gPion_Stopped_EdepVsLen_Truth[NFiles];
  TGraph *gProt_Stopped_EdepVsLen_Truth[NFiles];
  TGraph *gElec_Stopped_EdepVsLen_Truth[NFiles];

  TGraph *gMuon_Stopped_LenVsdEdx[NFiles];
  TGraph *gPion_Stopped_LenVsdEdx[NFiles];
  TGraph *gProt_Stopped_LenVsdEdx[NFiles];
  TGraph *gElec_Stopped_LenVsdEdx[NFiles];

  TGraph *gMuon_Stopped_LenVsdEdx_Truth[NFiles];
  TGraph *gPion_Stopped_LenVsdEdx_Truth[NFiles];
  TGraph *gProt_Stopped_LenVsdEdx_Truth[NFiles];
  TGraph *gElec_Stopped_LenVsdEdx_Truth[NFiles];

  TGraph *gMuon_Stopped_MomVsdEdx[NFiles];
  TGraph *gPion_Stopped_MomVsdEdx[NFiles];
  TGraph *gProt_Stopped_MomVsdEdx[NFiles];
  TGraph *gElec_Stopped_MomVsdEdx[NFiles];

  TGraph *gMuon_Stopped_MomVsdEdx_Truth[NFiles];
  TGraph *gPion_Stopped_MomVsdEdx_Truth[NFiles];
  TGraph *gProt_Stopped_MomVsdEdx_Truth[NFiles];
  TGraph *gElec_Stopped_MomVsdEdx_Truth[NFiles];

  TGraph *gMuon_All_MomVsdEdx_Truth[NFiles];
  TGraph *gPion_All_MomVsdEdx_Truth[NFiles];
  TGraph *gProt_All_MomVsdEdx_Truth[NFiles];
  TGraph *gElec_All_MomVsdEdx_Truth[NFiles];

  // Histograms 

  TH1F *hMuon_Stopped_EdepOverLen[NFiles];
  TH1F *hPion_Stopped_EdepOverLen[NFiles];
  TH1F *hProt_Stopped_EdepOverLen[NFiles];
  TH1F *hElec_Stopped_EdepOverLen[NFiles];

  TH1F *hMuon_Stopped_EdepOverLen_ManyBins[NFiles];
  TH1F *hPion_Stopped_EdepOverLen_ManyBins[NFiles];
  TH1F *hProt_Stopped_EdepOverLen_ManyBins[NFiles];
  TH1F *hElec_Stopped_EdepOverLen_ManyBins[NFiles];



  TH1F *hBinsMom[NFiles];
  //const int NBinsMom = 6; // to check with # of bins in hBinsMom

  TH1F *hBinsLen[NFiles];
  //const int NBinsLen = 5; // to check with # of bins in hBinsLen

  TH1F *hMuonMisidAsProt[NFiles]; // muon
  TH1F *hProtMisidAsMuon[NFiles]; 
  TH1F *hPionMisidAsProt[NFiles]; // pion
  TH1F *hProtMisidAsPion[NFiles]; 
  TH1F *hElecMisidAsProt[NFiles]; // electron
  TH1F *hProtMisidAsElec[NFiles]; 
  TH1F *hElecMisidAsMuon[NFiles]; 
  TH1F *hMuonMisidAsElec[NFiles]; 
  TH1F *hElecMisidAsPion[NFiles]; 
  TH1F *hPionMisidAsElec[NFiles]; 
  
  // same binning as hBinsMom
  TH1F *hMuonMisidAsProt_VsTrMom[NFiles]; // muon 
  TH1F *hProtMisidAsMuon_VsTrMom[NFiles];
  TH1F *hPionMisidAsProt_VsTrMom[NFiles]; // pion
  TH1F *hProtMisidAsPion_VsTrMom[NFiles];
  TH1F *hElecMisidAsProt_VsTrMom[NFiles]; // electron
  TH1F *hProtMisidAsElec_VsTrMom[NFiles]; 
  TH1F *hElecMisidAsMuon_VsTrMom[NFiles]; 
  TH1F *hMuonMisidAsElec_VsTrMom[NFiles]; 
  TH1F *hElecMisidAsPion_VsTrMom[NFiles]; 
  TH1F *hPionMisidAsElec_VsTrMom[NFiles]; 

  // same binning as hBinsLen
  TH1F *hMuonMisidAsProt_VsTrLen[NFiles]; // muon 
  TH1F *hProtMisidAsMuon_VsTrLen[NFiles];
  TH1F *hPionMisidAsProt_VsTrLen[NFiles]; // pion
  TH1F *hProtMisidAsPion_VsTrLen[NFiles];
  TH1F *hElecMisidAsProt_VsTrLen[NFiles]; // electron
  TH1F *hProtMisidAsElec_VsTrLen[NFiles]; 
  TH1F *hElecMisidAsMuon_VsTrLen[NFiles]; 
  TH1F *hMuonMisidAsElec_VsTrLen[NFiles]; 
  TH1F *hElecMisidAsPion_VsTrLen[NFiles]; 
  TH1F *hPionMisidAsElec_VsTrLen[NFiles]; 

  // Momentum bins

  TH1F *hMuon_Stopped_EdepOverLen_Mom[NFiles][NBinsMom];
  TH1F *hPion_Stopped_EdepOverLen_Mom[NFiles][NBinsMom];
  TH1F *hProt_Stopped_EdepOverLen_Mom[NFiles][NBinsMom];
  TH1F *hElec_Stopped_EdepOverLen_Mom[NFiles][NBinsMom];

  TH1F *hMuon_Stopped_EdepOverLen_Mom_ManyBins[NFiles][NBinsMom];
  TH1F *hPion_Stopped_EdepOverLen_Mom_ManyBins[NFiles][NBinsMom];
  TH1F *hProt_Stopped_EdepOverLen_Mom_ManyBins[NFiles][NBinsMom];
  TH1F *hElec_Stopped_EdepOverLen_Mom_ManyBins[NFiles][NBinsMom];

  // Track Length bins

  TH1F * hMuon_Stopped_EdepOverLen_Len[NFiles][NBinsLen];
  TH1F * hPion_Stopped_EdepOverLen_Len[NFiles][NBinsLen];
  TH1F * hProt_Stopped_EdepOverLen_Len[NFiles][NBinsLen];
  TH1F * hElec_Stopped_EdepOverLen_Len[NFiles][NBinsLen];
  
  TH1F * hMuon_Stopped_EdepOverLen_Len_ManyBins[NFiles][NBinsLen];
  TH1F * hPion_Stopped_EdepOverLen_Len_ManyBins[NFiles][NBinsLen];
  TH1F * hProt_Stopped_EdepOverLen_Len_ManyBins[NFiles][NBinsLen];
  TH1F * hElec_Stopped_EdepOverLen_Len_ManyBins[NFiles][NBinsLen];  
  
  // muon
  TLine *lCut_MuonVsProt[NFiles]; 
  TLine *lCut_MuonVsPion[NFiles]; 
  // pion
  TLine *lCut_PionVsProt[NFiles]; 
  // electron
  TLine *lCut_ElecVsMuon[NFiles]; 
  TLine *lCut_ElecVsPion[NFiles];
  TLine *lCut_ElecVsProt[NFiles];

  //

  for(int ifile=0;ifile<NFiles;ifile++){

    TString name;

    //
    // Read from files --> NuMu files (Muons, Pions, Protons)
    //
    
    cout << "Read files NuMu (muons, pions, protons): " << infilename[ifile].c_str() << endl;
       
    infile[ifile] = new TFile(infilename[ifile].c_str(),"READ");

    /*
    hLikeRatio_MuProt_TrueMu[ifile]     = (TH1F*)infile[ifile]->Get("hLikeRatio_MuProt_TrueMu");    
    hLikeRatio_MuProt_TrueProt[ifile]   = (TH1F*)infile[ifile]->Get("hLikeRatio_MuProt_TrueProt");    
    hLikeRatio_MuPion_TrueMu[ifile]     = (TH1F*)infile[ifile]->Get("hLikeRatio_MuPion_TrueMu");    
    hLikeRatio_MuPion_TruePion[ifile]   = (TH1F*)infile[ifile]->Get("hLikeRatio_MuPion_TruePion");    
    hLikeRatio_PionProt_TruePion[ifile] = (TH1F*)infile[ifile]->Get("hLikeRatio_PionProt_TruePion");    
    hLikeRatio_PionProt_TrueProt[ifile] = (TH1F*)infile[ifile]->Get("hLikeRatio_PionProt_TrueProt");    
    */
    
    hMuon_Stopped_EdepOverLen[ifile] = (TH1F*)infile[ifile]->Get("hMuon_Stopped_EdepOverLen");
    hPion_Stopped_EdepOverLen[ifile] = (TH1F*)infile[ifile]->Get("hPion_Stopped_EdepOverLen");
    hProt_Stopped_EdepOverLen[ifile] = (TH1F*)infile[ifile]->Get("hProt_Stopped_EdepOverLen");
    
    name = TString::Format("Stopped Muon Edep Over track length (%s)",label[ifile].Data());
    hMuon_Stopped_EdepOverLen[ifile]->SetTitle(name);
    name = TString::Format("Stopped Proton Edep Over track length (%s)",label[ifile].Data());
    hPion_Stopped_EdepOverLen[ifile]->SetTitle(name);
    name = TString::Format("Stopped Pion Edep Over track length (%s)",label[ifile].Data());
    hProt_Stopped_EdepOverLen[ifile]->SetTitle(name);
    
    // for p-values computation
    hMuon_Stopped_EdepOverLen_ManyBins[ifile] = (TH1F*)infile[ifile]->Get("hMuon_Stopped_EdepOverLen_ManyBins");
    hPion_Stopped_EdepOverLen_ManyBins[ifile] = (TH1F*)infile[ifile]->Get("hPion_Stopped_EdepOverLen_ManyBins");
    hProt_Stopped_EdepOverLen_ManyBins[ifile] = (TH1F*)infile[ifile]->Get("hProt_Stopped_EdepOverLen_ManyBins");
    
    cout << "hMuon_Stopped_EdepOverLen_ManyBins: # of bins = " <<  hMuon_Stopped_EdepOverLen_ManyBins[ifile]->GetNbinsX() << endl;
    cout << "hPion_Stopped_EdepOverLen_ManyBins: # of bins = " <<  hPion_Stopped_EdepOverLen_ManyBins[ifile]->GetNbinsX() << endl;
    cout << "hProt_Stopped_EdepOverLen_ManyBins: # of bins = " <<  hProt_Stopped_EdepOverLen_ManyBins[ifile]->GetNbinsX() << endl;

    //

    gMuon_Stopped_EdepVsLen[ifile] = (TGraph*)infile[ifile]->Get("gMuon_Stopped_EdepVsLen");
    name = TString::Format("Stopped Muon Edep Vs track length (%s)",label[ifile].Data());
    gMuon_Stopped_EdepVsLen[ifile]->SetTitle(name);
    gPion_Stopped_EdepVsLen[ifile] = (TGraph*)infile[ifile]->Get("gPion_Stopped_EdepVsLen");
    name = TString::Format("Stopped Pion Edep Vs track length (%s)",label[ifile].Data());
    gPion_Stopped_EdepVsLen[ifile]->SetTitle(name);
    gProt_Stopped_EdepVsLen[ifile] = (TGraph*)infile[ifile]->Get("gProt_Stopped_EdepVsLen");	
    name = TString::Format("Stopped Proton Edep Vs track length (%s)",label[ifile].Data());
    gProt_Stopped_EdepVsLen[ifile]->SetTitle(name);
       
    gMuon_Stopped_EdepVsLen_Truth[ifile] = (TGraph*)infile[ifile]->Get("gMuon_Stopped_EdepVsLen_Truth");
    name = TString::Format("Stopped Muon Edep Vs track length Truth (%s)",label[ifile].Data());
    gMuon_Stopped_EdepVsLen_Truth[ifile]->SetTitle(name);
    gPion_Stopped_EdepVsLen_Truth[ifile] = (TGraph*)infile[ifile]->Get("gPion_Stopped_EdepVsLen_Truth");
    name = TString::Format("Stopped Pion Edep Vs track length Truth (%s)",label[ifile].Data());
    gPion_Stopped_EdepVsLen_Truth[ifile]->SetTitle(name);
    gProt_Stopped_EdepVsLen_Truth[ifile] = (TGraph*)infile[ifile]->Get("gProt_Stopped_EdepVsLen_Truth");	
    name = TString::Format("Stopped Proton Edep Vs track length Truth (%s)",label[ifile].Data());
    gProt_Stopped_EdepVsLen_Truth[ifile]->SetTitle(name);

    // dEdx Vs Momentum

    gMuon_Stopped_MomVsdEdx[ifile] = (TGraph*)infile[ifile]->Get("gMuon_Stopped_MomVsdEdx");
    name = TString::Format("Stopped Muon Momentum Vs dE/dx (%s)",label[ifile].Data());
    gMuon_Stopped_MomVsdEdx[ifile]->SetTitle(name);
    gPion_Stopped_MomVsdEdx[ifile] = (TGraph*)infile[ifile]->Get("gPion_Stopped_MomVsdEdx");
    name = TString::Format("Stopped Pion Momentum Vs dE/dx (%s)",label[ifile].Data());
    gPion_Stopped_MomVsdEdx[ifile]->SetTitle(name);
    gProt_Stopped_MomVsdEdx[ifile] = (TGraph*)infile[ifile]->Get("gProt_Stopped_MomVsdEdx");	
    name = TString::Format("Stopped Proton Momentum Vs dE/dx (%s)",label[ifile].Data());
    gProt_Stopped_MomVsdEdx[ifile]->SetTitle(name);
       
    gMuon_Stopped_MomVsdEdx_Truth[ifile] = (TGraph*)infile[ifile]->Get("gMuon_Stopped_MomVsdEdx_Truth");
    name = TString::Format("Stopped Muon Momentum Vs dE/dx Truth (%s)",label[ifile].Data());
    gMuon_Stopped_MomVsdEdx_Truth[ifile]->SetTitle(name);
    gPion_Stopped_MomVsdEdx_Truth[ifile] = (TGraph*)infile[ifile]->Get("gPion_Stopped_MomVsdEdx_Truth");
    name = TString::Format("Stopped Pion Momentum Vs dE/dx Truth (%s)",label[ifile].Data());
    gPion_Stopped_MomVsdEdx_Truth[ifile]->SetTitle(name);
    gProt_Stopped_MomVsdEdx_Truth[ifile] = (TGraph*)infile[ifile]->Get("gProt_Stopped_MomVsdEdx_Truth");	
    name = TString::Format("Stopped Proton Momentum Vs dE/dx Truth (%s)",label[ifile].Data());
    gProt_Stopped_MomVsdEdx_Truth[ifile]->SetTitle(name);

    gMuon_All_MomVsdEdx_Truth[ifile] = (TGraph*)infile[ifile]->Get("gMuon_All_MomVsdEdx_Truth");
    name = TString::Format("All Muon Momentum Vs dE/dx Truth (%s)",label[ifile].Data());
    gMuon_All_MomVsdEdx_Truth[ifile]->SetTitle(name);
    gPion_All_MomVsdEdx_Truth[ifile] = (TGraph*)infile[ifile]->Get("gPion_All_MomVsdEdx_Truth");
    name = TString::Format("All Pion Momentum Vs dE/dx Truth (%s)",label[ifile].Data());
    gPion_All_MomVsdEdx_Truth[ifile]->SetTitle(name);
    gProt_All_MomVsdEdx_Truth[ifile] = (TGraph*)infile[ifile]->Get("gProt_All_MomVsdEdx_Truth");	
    name = TString::Format("All Proton Momentum Vs dE/dx Truth (%s)",label[ifile].Data());
    gProt_All_MomVsdEdx_Truth[ifile]->SetTitle(name);
 
    // dEdx Vs Track Length 

    gMuon_Stopped_LenVsdEdx[ifile] = (TGraph*)infile[ifile]->Get("gMuon_Stopped_LenVsdEdx");
    name = TString::Format("Stopped Muon track length Vs dE/dx (%s)",label[ifile].Data());
    gMuon_Stopped_LenVsdEdx[ifile]->SetTitle(name);
    gPion_Stopped_LenVsdEdx[ifile] = (TGraph*)infile[ifile]->Get("gPion_Stopped_LenVsdEdx");
    name = TString::Format("Stopped Pion track length Vs dE/dx (%s)",label[ifile].Data());
    gPion_Stopped_LenVsdEdx[ifile]->SetTitle(name);
    gProt_Stopped_LenVsdEdx[ifile] = (TGraph*)infile[ifile]->Get("gProt_Stopped_LenVsdEdx");	
    name = TString::Format("Stopped Proton track length Vs dE/dx (%s)",label[ifile].Data());
    gProt_Stopped_LenVsdEdx[ifile]->SetTitle(name);
       
    gMuon_Stopped_LenVsdEdx_Truth[ifile] = (TGraph*)infile[ifile]->Get("gMuon_Stopped_LenVsdEdx_Truth");
    name = TString::Format("Stopped Muon track length Vs dE/dx Truth (%s)",label[ifile].Data());
    gMuon_Stopped_LenVsdEdx_Truth[ifile]->SetTitle(name);
    gPion_Stopped_LenVsdEdx_Truth[ifile] = (TGraph*)infile[ifile]->Get("gPion_Stopped_LenVsdEdx_Truth");
    name = TString::Format("Stopped Pion track length Vs dE/dx Truth (%s)",label[ifile].Data());
    gPion_Stopped_LenVsdEdx_Truth[ifile]->SetTitle(name);
    gProt_Stopped_LenVsdEdx_Truth[ifile] = (TGraph*)infile[ifile]->Get("gProt_Stopped_LenVsdEdx_Truth");	
    name = TString::Format("Stopped Proton track length Vs dE/dx Truth (%s)",label[ifile].Data());
    gProt_Stopped_LenVsdEdx_Truth[ifile]->SetTitle(name);
   
    // momentum bins

    name = TString::Format("hBinsMom_%s",label[ifile].Data());
    hBinsMom[ifile] = (TH1F*)infile[ifile]->Get("hBinsMom");
    hBinsMom[ifile]->SetName(name);
    
    if(hBinsMom[ifile]->GetNbinsX() != NBinsMom){
      cout << endl;
      cout << "The # of momentum bins for Edep Vs Len doesn't correspond to the original one!!!" << endl;
      cout << "NBinsMom = " << NBinsMom << endl;
      cout << "hBinsMom->GetNbinsX() = " << hBinsMom[ifile]->GetNbinsX() << endl;
      cout << endl;
      exit(1);
    }

    // track length bins

    name = TString::Format("hBinsLen_%s",label[ifile].Data());
    hBinsLen[ifile] = (TH1F*)infile[ifile]->Get("hBinsLen");
    hBinsLen[ifile]->SetName(name);
    
    if(hBinsLen[ifile]->GetNbinsX() != NBinsLen){
      cout << endl;
      cout << "The # of track length bins for Edep Vs Len doesn't correspond to the original one!!!" << endl;
      cout << "NBinsLen = " << NBinsLen << endl;
      cout << "hBinsLen->GetNbinsX() = " << hBinsLen[ifile]->GetNbinsX() << endl;
      cout << endl;
      exit(1);
    }

    // PID histograms
    
    // muon
    name = TString::Format("hMuonMisidAsProt_%s",label[ifile].Data());
    hMuonMisidAsProt[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hProtMisidAsMuon_%s",label[ifile].Data());
    hProtMisidAsMuon[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hPionMisidAsProt_%s",label[ifile].Data());
    // pion
    hPionMisidAsProt[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hProtMisidAsPion_%s",label[ifile].Data());
    hProtMisidAsPion[ifile] = new TH1F(name,name,1,0,1);
  
    // muon
    name = TString::Format("hMuonMisidAsProt_VsTrMom_%s",label[ifile].Data());
    hMuonMisidAsProt_VsTrMom[ifile] = (TH1F*)hBinsMom[ifile]->Clone(name.Data());
    hMuonMisidAsProt_VsTrMom[ifile]->Reset();
    name = TString::Format("hProtMisidAsMuon_VsTrMom_%s",label[ifile].Data());
    hProtMisidAsMuon_VsTrMom[ifile] = (TH1F*)hBinsMom[ifile]->Clone(name.Data());
    hProtMisidAsMuon_VsTrMom[ifile]->Reset();
    // pion
    name = TString::Format("hPionMisidAsProt_VsTrMom_%s",label[ifile].Data());
    hPionMisidAsProt_VsTrMom[ifile] = (TH1F*)hBinsMom[ifile]->Clone(name.Data()); 
    hPionMisidAsProt_VsTrMom[ifile]->Reset();
    name = TString::Format("hProtMisidAsPion_VsTrMom_%s",label[ifile].Data());
    hProtMisidAsPion_VsTrMom[ifile] = (TH1F*)hBinsMom[ifile]->Clone(name.Data()); 
    hProtMisidAsPion_VsTrMom[ifile]->Reset();    
 
    // muon
    name = TString::Format("hMuonMisidAsProt_VsTrLen_%s",label[ifile].Data());
    hMuonMisidAsProt_VsTrLen[ifile] = (TH1F*)hBinsLen[ifile]->Clone(name.Data());
    hMuonMisidAsProt_VsTrLen[ifile]->Reset();
    name = TString::Format("hProtMisidAsMuon_VsTrLen_%s",label[ifile].Data());
    hProtMisidAsMuon_VsTrLen[ifile] = (TH1F*)hBinsLen[ifile]->Clone(name.Data());
    hProtMisidAsMuon_VsTrLen[ifile]->Reset();
    // pion
    name = TString::Format("hPionMisidAsProt_VsTrLen_%s",label[ifile].Data());
    hPionMisidAsProt_VsTrLen[ifile] = (TH1F*)hBinsLen[ifile]->Clone(name.Data()); 
    hPionMisidAsProt_VsTrLen[ifile]->Reset();
    name = TString::Format("hProtMisidAsPion_VsTrLen_%s",label[ifile].Data());
    hProtMisidAsPion_VsTrLen[ifile] = (TH1F*)hBinsLen[ifile]->Clone(name.Data()); 
    hProtMisidAsPion_VsTrLen[ifile]->Reset();    
    
    //

 
    for(int imom=0;imom<NBinsMom;imom++){
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
     
      name = TString::Format("hMuon_Stopped_EdepOverLen_Mom_%1.0f_%1.0f",lowmom,upmom);
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom] = (TH1F*)infile[ifile]->Get(name);
      name = TString::Format("hPion_Stopped_EdepOverLen_Mom_%1.0f_%1.0f",lowmom,upmom);
      hPion_Stopped_EdepOverLen_Mom[ifile][imom] = (TH1F*)infile[ifile]->Get(name);
      name = TString::Format("hProt_Stopped_EdepOverLen_Mom_%1.0f_%1.0f",lowmom,upmom);
      hProt_Stopped_EdepOverLen_Mom[ifile][imom] = (TH1F*)infile[ifile]->Get(name);
     
      name = TString::Format("hMuon_Stopped_EdepOverLen_Mom_ManyBins_%1.0f_%1.0f",lowmom,upmom);
      hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom] = (TH1F*)infile[ifile]->Get(name);
      name = TString::Format("hPion_Stopped_EdepOverLen_Mom_ManyBins_%1.0f_%1.0f",lowmom,upmom);
      hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom] = (TH1F*)infile[ifile]->Get(name);
      name = TString::Format("hProt_Stopped_EdepOverLen_Mom_ManyBins_%1.0f_%1.0f",lowmom,upmom);
      hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom] = (TH1F*)infile[ifile]->Get(name);
    }
  
    for(int ilen=0;ilen<NBinsLen;ilen++){
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);

      name = TString::Format("hMuon_Stopped_EdepOverLen_Len_%1.0f_%1.0f",lowlen,uplen);
      hMuon_Stopped_EdepOverLen_Len[ifile][ilen] = (TH1F*)infile[ifile]->Get(name);
      name = TString::Format("hPion_Stopped_EdepOverLen_Len_%1.0f_%1.0f",lowlen,uplen);
      hPion_Stopped_EdepOverLen_Len[ifile][ilen] = (TH1F*)infile[ifile]->Get(name);
      name = TString::Format("hProt_Stopped_EdepOverLen_Len_%1.0f_%1.0f",lowlen,uplen);
      hProt_Stopped_EdepOverLen_Len[ifile][ilen] = (TH1F*)infile[ifile]->Get(name);
     
      name = TString::Format("hMuon_Stopped_EdepOverLen_Len_ManyBins_%1.0f_%1.0f",lowlen,uplen);
      hMuon_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen] = (TH1F*)infile[ifile]->Get(name);
      name = TString::Format("hPion_Stopped_EdepOverLen_Len_ManyBins_%1.0f_%1.0f",lowlen,uplen);
      hPion_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen] = (TH1F*)infile[ifile]->Get(name);
      name = TString::Format("hProt_Stopped_EdepOverLen_Len_ManyBins_%1.0f_%1.0f",lowlen,uplen);
      hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen] = (TH1F*)infile[ifile]->Get(name);
    }
  
    //
    // Read from NuE files --> Electrons
    //
    
    cout << "Read files NuE (electrons): " << infilename_Elec[ifile].c_str() << endl;
    
    infile_Elec[ifile] = new TFile(infilename_Elec[ifile].c_str(),"READ");
    
    hElec_Stopped_EdepOverLen[ifile] = (TH1F*)infile_Elec[ifile]->Get("hElec_Stopped_EdepOverLen");
    
    name = TString::Format("Stopped Electron Edep Over track length (%s)",label[ifile].Data());
    hElec_Stopped_EdepOverLen[ifile]->SetTitle(name);
    
    // for p-values computation
    hElec_Stopped_EdepOverLen_ManyBins[ifile] = (TH1F*)infile_Elec[ifile]->Get("hElec_Stopped_EdepOverLen_ManyBins");
    
    cout << "hElec_Stopped_EdepOverLen_ManyBins: # of bins = " <<  hElec_Stopped_EdepOverLen_ManyBins[ifile]->GetNbinsX() << endl;

    //
     
    gElec_Stopped_EdepVsLen[ifile] = (TGraph*)infile_Elec[ifile]->Get("gElec_Stopped_EdepVsLen");
    name = TString::Format("Stopped Electron Edep Vs track length (%s)",label[ifile].Data());
    gElec_Stopped_EdepVsLen[ifile]->SetTitle(name);
    
    gElec_Stopped_EdepVsLen_Truth[ifile] = (TGraph*)infile_Elec[ifile]->Get("gElec_Stopped_EdepVsLen_Truth");
    name = TString::Format("Stopped Electron Edep Vs track length Truth (%s)",label[ifile].Data());
    gElec_Stopped_EdepVsLen_Truth[ifile]->SetTitle(name);

    gElec_Stopped_MomVsdEdx[ifile] = (TGraph*)infile_Elec[ifile]->Get("gElec_Stopped_MomVsdEdx");
    name = TString::Format("Stopped Electron Momentum Vs dE/dx (%s)",label[ifile].Data());
    gElec_Stopped_MomVsdEdx[ifile]->SetTitle(name);
    
    gElec_Stopped_MomVsdEdx_Truth[ifile] = (TGraph*)infile_Elec[ifile]->Get("gElec_Stopped_MomVsdEdx_Truth");
    name = TString::Format("Stopped Electron Momentum Vs dE/dx Truth (%s)",label[ifile].Data());
    gElec_Stopped_MomVsdEdx_Truth[ifile]->SetTitle(name);

    gElec_All_MomVsdEdx_Truth[ifile] = (TGraph*)infile_Elec[ifile]->Get("gElec_All_MomVsdEdx_Truth");
    name = TString::Format("All Electron Momentum Vs dE/dx Truth (%s)",label[ifile].Data());
    gElec_All_MomVsdEdx_Truth[ifile]->SetTitle(name);

    gElec_Stopped_LenVsdEdx[ifile] = (TGraph*)infile_Elec[ifile]->Get("gElec_Stopped_LenVsdEdx");
    name = TString::Format("Stopped Electron track length Vs dE/dx (%s)",label[ifile].Data());
    gElec_Stopped_LenVsdEdx[ifile]->SetTitle(name);
    
    gElec_Stopped_LenVsdEdx_Truth[ifile] = (TGraph*)infile_Elec[ifile]->Get("gElec_Stopped_LenVsdEdx_Truth");
    name = TString::Format("Stopped Electron track length Vs dE/dx Truth (%s)",label[ifile].Data());
    gElec_Stopped_LenVsdEdx_Truth[ifile]->SetTitle(name);
    
    // PID histograms
    
    name = TString::Format("hElecMisidAsProt_%s",label[ifile].Data());
    hElecMisidAsProt[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hProtMisidAsElec_%s",label[ifile].Data());
    hProtMisidAsElec[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hElecMisidAsMuon_%s",label[ifile].Data());
    hElecMisidAsMuon[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hMuonMisidAsElec_%s",label[ifile].Data());
    hMuonMisidAsElec[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hElecMisidAsPion_%s",label[ifile].Data());
    hElecMisidAsPion[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hPionMisidAsElec_%s",label[ifile].Data());
    hPionMisidAsElec[ifile] = new TH1F(name,name,1,0,1);
    
    name = TString::Format("hElecMisidAsProt_VsTrMom_%s",label[ifile].Data());
    hElecMisidAsProt_VsTrMom[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hProtMisidAsElec_VsTrMom_%s",label[ifile].Data());
    hProtMisidAsElec_VsTrMom[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hElecMisidAsMuon_VsTrMom_%s",label[ifile].Data());
    hElecMisidAsMuon_VsTrMom[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hMuonMisidAsElec_VsTrMom_%s",label[ifile].Data());
    hMuonMisidAsElec_VsTrMom[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hElecMisidAsPion_VsTrMom_%s",label[ifile].Data());
    hElecMisidAsPion_VsTrMom[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hPionMisidAsElec_VsTrMom_%s",label[ifile].Data());
    hPionMisidAsElec_VsTrMom[ifile] = new TH1F(name,name,1,0,1);
 
    name = TString::Format("hElecMisidAsProt_VsTrLen_%s",label[ifile].Data());
    hElecMisidAsProt_VsTrLen[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hProtMisidAsElec_VsTrLen_%s",label[ifile].Data());
    hProtMisidAsElec_VsTrLen[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hElecMisidAsMuon_VsTrLen_%s",label[ifile].Data());
    hElecMisidAsMuon_VsTrLen[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hMuonMisidAsElec_VsTrLen_%s",label[ifile].Data());
    hMuonMisidAsElec_VsTrLen[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hElecMisidAsPion_VsTrLen_%s",label[ifile].Data());
    hElecMisidAsPion_VsTrLen[ifile] = new TH1F(name,name,1,0,1);
    name = TString::Format("hPionMisidAsElec_VsTrLen_%s",label[ifile].Data());
    hPionMisidAsElec_VsTrLen[ifile] = new TH1F(name,name,1,0,1);

    // Momentum bins
    
    for(int imom=0;imom<NBinsMom;imom++){
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      
      name = TString::Format("hElec_Stopped_EdepOverLen_Mom_%1.0f_%1.0f",lowmom,upmom);
      hElec_Stopped_EdepOverLen_Mom[ifile][imom] = (TH1F*)infile_Elec[ifile]->Get(name);
      
      name = TString::Format("hElec_Stopped_EdepOverLen_Mom_ManyBins_%1.0f_%1.0f",lowmom,upmom);
      hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom] = (TH1F*)infile_Elec[ifile]->Get(name);
    }

    // Track Length bins

    for(int ilen=0;ilen<NBinsLen;ilen++){
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      
      name = TString::Format("hElec_Stopped_EdepOverLen_Len_%1.0f_%1.0f",lowlen,uplen);
      hElec_Stopped_EdepOverLen_Len[ifile][ilen] = (TH1F*)infile_Elec[ifile]->Get(name);
      
      name = TString::Format("hElec_Stopped_EdepOverLen_Len_ManyBins_%1.0f_%1.0f",lowlen,uplen);
      hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen] = (TH1F*)infile_Elec[ifile]->Get(name);      
    }


    //
    // Print results
    //
    
    double Mean = 0.;
    double RMS = 0.;
    double Nevents = 0.;
    
    cout << endl;
    cout << label[ifile] << ": " << endl;
    
    // Muon
    Nevents = hMuon_Stopped_EdepOverLen_ManyBins[ifile]->Integral();
    Mean = hMuon_Stopped_EdepOverLen[ifile]->GetMean();
    RMS  = hMuon_Stopped_EdepOverLen[ifile]->GetRMS();    
    cout << "  Muons: " << endl;
    cout << "   - # of events = " << Nevents << endl;
    if(Nevents>0){
      cout << "   - Mean = " << Mean << endl;
      cout << "   - RMS  = " << RMS  << endl;
      cout << "   - RMS/Mean = " << RMS / Mean << endl;    
    }
    cout << endl;
    
    // Pion
    Nevents = hPion_Stopped_EdepOverLen_ManyBins[ifile]->Integral();
    Mean = hPion_Stopped_EdepOverLen[ifile]->GetMean();
    RMS  = hPion_Stopped_EdepOverLen[ifile]->GetRMS();    
    cout << "  Pions: " << endl;
    cout << "   - # of events = " << Nevents << endl;
    if(Nevents>0){
      cout << "   - Mean = " << Mean << endl;
      cout << "   - RMS  = " << RMS  << endl;
      cout << "   - RMS/Mean = " << RMS / Mean << endl;    
    }
    cout << endl;
    
    // Proton
    Nevents = hProt_Stopped_EdepOverLen_ManyBins[ifile]->Integral();
    Mean = hProt_Stopped_EdepOverLen[ifile]->GetMean();
    RMS  = hProt_Stopped_EdepOverLen[ifile]->GetRMS();    
    cout << "  Protons: " << endl;
    cout << "   - # of events = " << Nevents << endl;
    if(Nevents>0){
      cout << "   - Mean = " << Mean << endl;
      cout << "   - RMS  = " << RMS  << endl;
      cout << "   - RMS/Mean = " << RMS / Mean << endl;    
    }
    cout << endl;
    
    // Electron
    Nevents = hElec_Stopped_EdepOverLen_ManyBins[ifile]->Integral();
    Mean = hElec_Stopped_EdepOverLen[ifile]->GetMean();
    RMS  = hElec_Stopped_EdepOverLen[ifile]->GetRMS();    
    cout << "  Electrons: " << endl;
    cout << "   - # of events = " << Nevents << endl;
    if(Nevents>0){
      cout << "   - Mean = " << Mean << endl;
      cout << "   - RMS  = " << RMS  << endl;
      cout << "   - RMS/Mean = " << RMS / Mean << endl;    
    }
    cout << endl;



    //
    // CHANGED!!!
    //

    // should be already normalized to 1, since it's used as PDF
    
    // Momentum bins

    // hMuon_Stopped_EdepOverLen[ifile]->Scale(1./hMuon_Stopped_EdepOverLen[ifile]->Integral());
    // hPion_Stopped_EdepOverLen[ifile]->Scale(1./hPion_Stopped_EdepOverLen[ifile]->Integral());
    // hProt_Stopped_EdepOverLen[ifile]->Scale(1./hProt_Stopped_EdepOverLen[ifile]->Integral());
    // hElec_Stopped_EdepOverLen[ifile]->Scale(1./hElec_Stopped_EdepOverLen[ifile]->Integral());    

    // hMuon_Stopped_EdepOverLen_ManyBins[ifile]->Scale(1./hMuon_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
    // hPion_Stopped_EdepOverLen_ManyBins[ifile]->Scale(1./hPion_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
    // hProt_Stopped_EdepOverLen_ManyBins[ifile]->Scale(1./hProt_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
    // hElec_Stopped_EdepOverLen_ManyBins[ifile]->Scale(1./hElec_Stopped_EdepOverLen_ManyBins[ifile]->Integral());    

    // for(int imom=0;imom<NBinsMom;imom++){

      // if(hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->Integral() > 0){
      // 	hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->Integral());
      // 	//hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hMuon_Stopped_EdepOverLen[ifile]->Integral());
      // }
      // if(hPion_Stopped_EdepOverLen_Mom[ifile][imom]->Integral() > 0){
      // 	hPion_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hPion_Stopped_EdepOverLen_Mom[ifile][imom]->Integral());
      // 	//hPion_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hPion_Stopped_EdepOverLen[ifile]->Integral());
      // }
      // if(hProt_Stopped_EdepOverLen_Mom[ifile][imom]->Integral() > 0){
      // 	hProt_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hProt_Stopped_EdepOverLen_Mom[ifile][imom]->Integral());
      // 	//hProt_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hProt_Stopped_EdepOverLen[ifile]->Integral());
      // } 
      // if(hElec_Stopped_EdepOverLen_Mom[ifile][imom]->Integral() > 0){
      // 	hElec_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hElec_Stopped_EdepOverLen_Mom[ifile][imom]->Integral());
      // 	//hElec_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hElec_Stopped_EdepOverLen[ifile]->Integral());
      // }
      
      // if(hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() > 0){
      // 	hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral());
      // 	//hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hMuon_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
      // }
      // if(hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() > 0){
      // 	hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral());
      // 	//hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hPion_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
      // }
      // if(hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() > 0){
      // 	hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral());
      // 	//hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hProt_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
      // }
      // if(hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() > 0){
      // 	hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral());
      // 	//hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hElec_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
      // }

    //}
    
    // // Track Length bins
    
    // for(int ilen=0;ilen<NBinsLen;ilen++){

    //   if(hMuon_Stopped_EdepOverLen_Len[ifile][ilen]->Integral() > 0){
    // 	hMuon_Stopped_EdepOverLen_Len[ifile][ilen]->Scale(1./hMuon_Stopped_EdepOverLen_Len[ifile][ilen]->Integral());
    //   }
    //   if(hPion_Stopped_EdepOverLen_Len[ifile][ilen]->Integral() > 0){
    // 	hPion_Stopped_EdepOverLen_Len[ifile][ilen]->Scale(1./hPion_Stopped_EdepOverLen_Len[ifile][ilen]->Integral());
    //   }
    //   if(hProt_Stopped_EdepOverLen_Len[ifile][ilen]->Integral() > 0){
    // 	hProt_Stopped_EdepOverLen_Len[ifile][ilen]->Scale(1./hProt_Stopped_EdepOverLen_Len[ifile][ilen]->Integral());
    //   } 
    //   if(hElec_Stopped_EdepOverLen_Len[ifile][ilen]->Integral() > 0){
    // 	hElec_Stopped_EdepOverLen_Len[ifile][ilen]->Scale(1./hElec_Stopped_EdepOverLen_Len[ifile][ilen]->Integral());
    //   }
      
    //   if(hMuon_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral() > 0){
    // 	hMuon_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Scale(1./hMuon_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral());
    //   }
    //   if(hPion_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral() > 0){
    // 	hPion_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Scale(1./hPion_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral());
    //   }
    //   if(hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral() > 0){
    // 	hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Scale(1./hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral());
    //   }
    //   if(hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral() > 0){
    // 	hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Scale(1./hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral());
    //   }

    // }
 
    ////////////////

   




    /*
      if(hLikeRatio_MuProt_TrueMu[ifile]->Integral()>0.){
      hLikeRatio_MuProt_TrueMu[ifile]->Scale(1./hLikeRatio_MuProt_TrueMu[ifile]->Integral());
      }
      if(hLikeRatio_MuProt_TrueProt[ifile]->Integral()>0.){
      hLikeRatio_MuProt_TrueProt[ifile]->Scale(1./hLikeRatio_MuProt_TrueProt[ifile]->Integral());
      }
      if(hLikeRatio_MuPion_TrueMu[ifile]->Integral()>0.){
      hLikeRatio_MuPion_TrueMu[ifile]->Scale(1./hLikeRatio_MuPion_TrueMu[ifile]->Integral());
      }
      if(hLikeRatio_MuPion_TruePion[ifile]->Integral()>0.){
      hLikeRatio_MuPion_TruePion[ifile]->Scale(1./hLikeRatio_MuPion_TruePion[ifile]->Integral());
      }
      if(hLikeRatio_PionProt_TruePion[ifile]->Integral()>0.){
      hLikeRatio_PionProt_TruePion[ifile]->Scale(1./hLikeRatio_PionProt_TruePion[ifile]->Integral());
      }
      if(hLikeRatio_PionProt_TrueProt[ifile]->Integral()>0.){
      hLikeRatio_PionProt_TrueProt[ifile]->Scale(1./hLikeRatio_PionProt_TrueProt[ifile]->Integral());
      }
    */
    
    // // rebin
    // hLikeRatio_MuProt_TrueMu[ifile]->Rebin(2);
    // hLikeRatio_MuProt_TrueMu[ifile]->Rebin(2);
    // hLikeRatio_MuProt_TrueProt[ifile]->Rebin(2);
    // hLikeRatio_MuProt_TrueProt[ifile]->Rebin(2);
    
    // hLikeRatio_MuPion_TrueMu[ifile]->Rebin(2);
    // hLikeRatio_MuPion_TrueMu[ifile]->Rebin(2);
    // hLikeRatio_MuPion_TruePion[ifile]->Rebin(2);
    // hLikeRatio_MuPion_TruePion[ifile]->Rebin(2);
    
    // hLikeRatio_PionProt_TruePion[ifile]->Rebin(2);
    // hLikeRatio_PionProt_TruePion[ifile]->Rebin(2);
    // hLikeRatio_PionProt_TrueProt[ifile]->Rebin(2);
    // hLikeRatio_PionProt_TrueProt[ifile]->Rebin(2);
    
    //cout << "Integral hMuon_Stopped_EdepOverLen = " << hMuon_Stopped_EdepOverLen[ifile]->Integral() << endl;
    //cout << "Integral hPion_Stopped_EdepOverLen = " << hPion_Stopped_EdepOverLen[ifile]->Integral() << endl;
    //cout << "Integral hProt_Stopped_EdepOverLen = " << hProt_Stopped_EdepOverLen[ifile]->Integral() << endl;
    //cout << "Integral hElec_Stopped_EdepOverLen = " << hElec_Stopped_EdepOverLen[ifile]->Integral() << endl;



      
    // Set the cut lines
    
    // muon
    lCut_MuonVsProt[ifile] = new TLine(cut_MuonVsProt[ifile],0.,cut_MuonVsProt[ifile],hMuon_Stopped_EdepOverLen[ifile]->GetMaximum());
    lCut_MuonVsProt[ifile]->SetLineWidth(2.);
    lCut_MuonVsPion[ifile] = new TLine(cut_MuonVsPion[ifile],0.,cut_MuonVsPion[ifile],hMuon_Stopped_EdepOverLen[ifile]->GetMaximum()); 
    lCut_MuonVsPion[ifile]->SetLineWidth(2.);
    // pion
    lCut_PionVsProt[ifile] = new TLine(cut_PionVsProt[ifile],0.,cut_PionVsProt[ifile],hPion_Stopped_EdepOverLen[ifile]->GetMaximum());
    lCut_PionVsProt[ifile]->SetLineWidth(2.);
    // electron
    lCut_ElecVsProt[ifile] = new TLine(cut_ElecVsProt[ifile],0.,cut_ElecVsProt[ifile],hElec_Stopped_EdepOverLen[ifile]->GetMaximum());
    lCut_ElecVsProt[ifile]->SetLineWidth(2.);    
    lCut_ElecVsPion[ifile] = new TLine(cut_ElecVsPion[ifile],0.,cut_ElecVsPion[ifile],hElec_Stopped_EdepOverLen[ifile]->GetMaximum()); 
    lCut_ElecVsPion[ifile]->SetLineWidth(2.);
    lCut_ElecVsMuon[ifile] = new TLine(cut_ElecVsMuon[ifile],0.,cut_ElecVsMuon[ifile],hElec_Stopped_EdepOverLen[ifile]->GetMaximum()); 
    lCut_ElecVsMuon[ifile]->SetLineWidth(2.);


    /*
      if(hLikeRatio_MuProt_TrueMu[ifile]->Integral()>0.){
      hLikeRatio_MuProt_TrueMu[ifile]->Scale(1./hLikeRatio_MuProt_TrueMu[ifile]->Integral());
      }
      if(hLikeRatio_MuProt_TrueProt[ifile]->Integral()>0.){
      hLikeRatio_MuProt_TrueProt[ifile]->Scale(1./hLikeRatio_MuProt_TrueProt[ifile]->Integral());
      }
      if(hLikeRatio_MuPion_TrueMu[ifile]->Integral()>0.){
      hLikeRatio_MuPion_TrueMu[ifile]->Scale(1./hLikeRatio_MuPion_TrueMu[ifile]->Integral());
      }
      if(hLikeRatio_MuPion_TruePion[ifile]->Integral()>0.){
      hLikeRatio_MuPion_TruePion[ifile]->Scale(1./hLikeRatio_MuPion_TruePion[ifile]->Integral());
      }
      if(hLikeRatio_PionProt_TruePion[ifile]->Integral()>0.){
      hLikeRatio_PionProt_TruePion[ifile]->Scale(1./hLikeRatio_PionProt_TruePion[ifile]->Integral());
      }
      if(hLikeRatio_PionProt_TrueProt[ifile]->Integral()>0.){
      hLikeRatio_PionProt_TrueProt[ifile]->Scale(1./hLikeRatio_PionProt_TrueProt[ifile]->Integral());
      }
    */
    
    // // rebin
    // hLikeRatio_MuProt_TrueMu[ifile]->Rebin(2);
    // hLikeRatio_MuProt_TrueMu[ifile]->Rebin(2);
    // hLikeRatio_MuProt_TrueProt[ifile]->Rebin(2);
    // hLikeRatio_MuProt_TrueProt[ifile]->Rebin(2);

    // hLikeRatio_MuPion_TrueMu[ifile]->Rebin(2);
    // hLikeRatio_MuPion_TrueMu[ifile]->Rebin(2);
    // hLikeRatio_MuPion_TruePion[ifile]->Rebin(2);
    // hLikeRatio_MuPion_TruePion[ifile]->Rebin(2);

    // hLikeRatio_PionProt_TruePion[ifile]->Rebin(2);
    // hLikeRatio_PionProt_TruePion[ifile]->Rebin(2);
    // hLikeRatio_PionProt_TrueProt[ifile]->Rebin(2);
    // hLikeRatio_PionProt_TrueProt[ifile]->Rebin(2);

    //cout << "Integral hMuon_Stopped_EdepOverLen = " << hMuon_Stopped_EdepOverLen[ifile]->Integral() << endl;
    //cout << "Integral hPion_Stopped_EdepOverLen = " << hPion_Stopped_EdepOverLen[ifile]->Integral() << endl;
    //cout << "Integral hProt_Stopped_EdepOverLen = " << hProt_Stopped_EdepOverLen[ifile]->Integral() << endl;
    //cout << "Integral hElec_Stopped_EdepOverLen = " << hElec_Stopped_EdepOverLen[ifile]->Integral() << endl;

  } // end loop over ifile


  
  //
  // Print contamination --> PID probability
  // 
  //
  // N.B. Don't normalize the PDFs before computing the p-values for different track length bins
  //      Normalized distributions are important only for plotting
  //

  // Muon Vs Proton

  cout << endl;
  cout << "/// PID Muon Vs Proton ///" << endl;
  
  for(int ifile=0;ifile<NFiles;ifile++){

    double pvalue_muon = 0.;
    double pvalue_prot = 0.;

    pvalue_muon = GetPvalue_Histo_rightside( hMuon_Stopped_EdepOverLen_ManyBins[ifile],cut_MuonVsProt[ifile]);
    pvalue_prot = GetPvalue_Histo_leftside(hProt_Stopped_EdepOverLen_ManyBins[ifile],cut_MuonVsProt[ifile]);

    cout << endl;
    cout << label[ifile] << ": " << endl;
    cout << "Cut on dEdx = " << cut_MuonVsProt[ifile] << endl;
    cout << endl;
    cout << "Prob. Misid (Muon as Proton) = " << pvalue_muon << endl;
    cout << "Prob. Misid (Proton as Muon) = " << pvalue_prot << endl;
    cout << endl;

    hMuonMisidAsProt[ifile]->SetBinContent(1,pvalue_muon);
    hProtMisidAsMuon[ifile]->SetBinContent(1,pvalue_prot);
  
    // momenta bins
  
    double pvalue_muon_tot = 0.;
    double pvalue_prot_tot = 0.;
    pvalue_muon = 0.;
    pvalue_prot = 0.;

    for(int imom=0;imom<NBinsMom;imom++){
      
      pvalue_muon = GetPvalue_Histo_rightside( hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom],cut_MuonVsProt[ifile]);
      pvalue_prot = GetPvalue_Histo_leftside(  hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom],cut_MuonVsProt[ifile]);

      // Un-normalize the p-value in each single momentum bin and afterward normalize to 1 the sum of the p-values  
      pvalue_muon *= hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral(); 
      pvalue_prot *= hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral();
     
      pvalue_muon_tot += pvalue_muon;
      pvalue_prot_tot += pvalue_prot;

      //cout << "pvalue_muon_tot = " << pvalue_muon_tot << endl;
      //cout << "pvalue_prot_tot = " << pvalue_prot_tot << endl;

      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format(" - Momentum: %1.0f - %1.0f",lowmom,upmom);
      cout << name << endl;

      cout << "# of entries (Muon) = " << hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() << endl;
      cout << "# of entries (Proton) = " << hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() << endl;

      cout << "Prob. Misid (Muon as Proton) = " << pvalue_muon << endl;
      cout << "Prob. Misid (Proton as Muon) = " << pvalue_prot << endl;
      cout << endl;

      hMuonMisidAsProt_VsTrMom[ifile]->SetBinContent(histobin,pvalue_muon);
      hProtMisidAsMuon_VsTrMom[ifile]->SetBinContent(histobin,pvalue_prot);
    }    
    
    pvalue_muon_tot /= hMuon_Stopped_EdepOverLen_ManyBins[ifile]->Integral();
    pvalue_prot_tot /= hProt_Stopped_EdepOverLen_ManyBins[ifile]->Integral();

    cout << "Tot. Prob. Misid (Muon as Proton) = " << pvalue_muon_tot << endl;
    cout << "Tot. Prob. Misid (Proton as Muon) = " << pvalue_prot_tot << endl;
  
    cout << endl;
    cout << "////////////////////////" << endl;
    cout << endl;


    /*
    TH1F *hHypLeft = (TH1F*) hMuon_Stopped_EdepOverLen_Mom_ManyBins; 
    TH1F *hHypRight = (TH1F*) hProt_Stopped_EdepOverLen_Mom_ManyBins; 
    TH1F *hBins = (TH1F*) hBinsMom;
    TH1F *hMisidLeft = (TH1F*) hMuonMisidAsProt_VsTrMom;
    TH1F *hMisidRight = (TH1F*) hProtMisidAsMuon_VsTrMom;

    GetPvalue_bins_mom(hHypLeft,
		       cut_MuonVsProt,
		       hHypRight,
		       cut_MuonVsProt,
		       hBins,
		       hMisidLeft,
		       hMisidRight
		       );
    */

    GetPvalue_bins_mom(hMuon_Stopped_EdepOverLen_Mom_ManyBins,
		       cut_MuonVsProt,
		       hProt_Stopped_EdepOverLen_Mom_ManyBins,
		       cut_MuonVsProt,
		       hBinsMom,
		       hMuonMisidAsProt_VsTrMom,
		       hProtMisidAsMuon_VsTrMom
		       );

    return;
    
    //continue;










    // track length bins
    cout << endl;

    pvalue_muon = 0.;
    pvalue_prot = 0.;
    for(int ilen=0;ilen<NBinsLen;ilen++){
      pvalue_muon = GetPvalue_Histo_rightside( hMuon_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen],cut_MuonVsProt[ifile]);
      pvalue_prot = GetPvalue_Histo_leftside(hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen],cut_MuonVsProt[ifile]);
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format(" - Track Length: %1.0f - %1.0f",lowlen,uplen);
      cout << name << endl;
      cout << "Prob. Misid (Muon as Proton) = " << pvalue_muon << endl;
      cout << "Prob. Misid (Proton as Muon) = " << pvalue_prot << endl;
      
      hMuonMisidAsProt_VsTrLen[ifile]->SetBinContent(histobin,pvalue_muon);
      hProtMisidAsMuon_VsTrLen[ifile]->SetBinContent(histobin,pvalue_prot);
    }    
  }

  //return;


  

  // Pion Vs Proton
  cout << endl;
  cout << "/// PID Pion Vs Proton ///" << endl;
  for(int ifile=0;ifile<NFiles;ifile++){
    double pvalue_pion = GetPvalue_Histo_rightside( hPion_Stopped_EdepOverLen_ManyBins[ifile],cut_PionVsProt[ifile]);
    double pvalue_prot = GetPvalue_Histo_leftside(hProt_Stopped_EdepOverLen_ManyBins[ifile],cut_PionVsProt[ifile]);
    cout << endl;
    cout << label[ifile] << ": " << endl;
    cout << "Cut on dEdx = " << cut_MuonVsPion[ifile] << endl;
    cout << endl;
    cout << "Prob. Misid (Pion as Proton) = " << pvalue_pion << endl;
    cout << "Prob. Misid (Proton as Pion) = " << pvalue_prot << endl;
    cout << endl;

    hPionMisidAsProt[ifile]->SetBinContent(1,pvalue_pion);
    hProtMisidAsPion[ifile]->SetBinContent(1,pvalue_prot);

    // momenta bins
    for(int imom=0;imom<NBinsMom;imom++){
      pvalue_pion = GetPvalue_Histo_rightside( hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom],cut_PionVsProt[ifile]);
      pvalue_prot = GetPvalue_Histo_leftside(hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom],cut_PionVsProt[ifile]);
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format(" - Momentum: %1.0f - %1.0f",lowmom,upmom);
      cout << name << endl;
      cout << "Prob. Misid (Pion as Proton) = " << pvalue_pion << endl;
      cout << "Prob. Misid (Proton as Pion) = " << pvalue_prot << endl;
      
      hPionMisidAsProt_VsTrMom[ifile]->SetBinContent(histobin,pvalue_pion);
      hProtMisidAsPion_VsTrMom[ifile]->SetBinContent(histobin,pvalue_prot);
    }

    // track length bins
    for(int ilen=0;ilen<NBinsLen;ilen++){
      pvalue_pion = GetPvalue_Histo_rightside( hPion_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen],cut_PionVsProt[ifile]);
      pvalue_prot = GetPvalue_Histo_leftside(hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen],cut_PionVsProt[ifile]);
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format(" - Track Length: %1.0f - %1.0f",lowlen,uplen);
      cout << name << endl;
      cout << "Prob. Misid (Pion as Proton) = " << pvalue_pion << endl;
      cout << "Prob. Misid (Proton as Pion) = " << pvalue_prot << endl;
      
      hPionMisidAsProt_VsTrLen[ifile]->SetBinContent(histobin,pvalue_pion);
      hProtMisidAsPion_VsTrLen[ifile]->SetBinContent(histobin,pvalue_prot);
    }
  }



  // Electron Vs Muon
  cout << endl;
  cout << "/// PID Electron Vs Muon ///" << endl;

  for(int ifile=0;ifile<NFiles;ifile++){
    double pvalue_elec = GetPvalue_Histo_rightside( hElec_Stopped_EdepOverLen_ManyBins[ifile],cut_ElecVsMuon[ifile]);
    double pvalue_muon = GetPvalue_Histo_leftside(hMuon_Stopped_EdepOverLen_ManyBins[ifile],cut_ElecVsMuon[ifile]);
    cout << endl;
    cout << label[ifile] << ": " << endl;
    cout << endl;
    cout << "Cut on dEdx = " << cut_ElecVsMuon[ifile] << endl;
    cout << "Prob. Misid (Electron as Muon) = " << pvalue_elec << endl;
    cout << "Prob. Misid (Muon as Electron) = " << pvalue_muon << endl;
    cout << endl;

    hElecMisidAsMuon[ifile]->SetBinContent(1,pvalue_elec);
    hMuonMisidAsElec[ifile]->SetBinContent(1,pvalue_muon);
  
    // momenta bins
    for(int imom=0;imom<NBinsMom;imom++){
      pvalue_elec = GetPvalue_Histo_rightside( hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom],cut_ElecVsMuon[ifile]);
      pvalue_muon = GetPvalue_Histo_leftside(hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom],cut_ElecVsMuon[ifile]);
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format(" - Momentum: %1.0f - %1.0f",lowmom,upmom);
      cout << name << endl;
      cout << "Prob. Misid (Elec as Muon) = " << pvalue_elec << endl;
      cout << "Prob. Misid (Muon as Electron) = " << pvalue_muon << endl;

      hElecMisidAsMuon_VsTrMom[ifile]->SetBinContent(histobin,pvalue_elec);
      hMuonMisidAsElec_VsTrMom[ifile]->SetBinContent(histobin,pvalue_muon);
    }    

    // track length bins
    for(int ilen=0;ilen<NBinsLen;ilen++){
      pvalue_elec = GetPvalue_Histo_rightside( hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen],cut_ElecVsMuon[ifile]);
      pvalue_muon = GetPvalue_Histo_leftside(hMuon_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen],cut_ElecVsMuon[ifile]);
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format(" - Track Length: %1.0f - %1.0f",lowlen,uplen);
      cout << name << endl;
      cout << "Prob. Misid (Elec as Muon) = " << pvalue_elec << endl;
      cout << "Prob. Misid (Muon as Electron) = " << pvalue_muon << endl;

      hElecMisidAsMuon_VsTrLen[ifile]->SetBinContent(histobin,pvalue_elec);
      hMuonMisidAsElec_VsTrLen[ifile]->SetBinContent(histobin,pvalue_muon);
    }    

  }


  // Electron Vs Pion
  cout << endl;
  cout << "/// PID Electron Vs Pion ///" << endl;

  for(int ifile=0;ifile<NFiles;ifile++){
    double pvalue_elec = GetPvalue_Histo_rightside( hElec_Stopped_EdepOverLen_ManyBins[ifile],cut_ElecVsPion[ifile]);
    double pvalue_pion = GetPvalue_Histo_leftside(hPion_Stopped_EdepOverLen_ManyBins[ifile],cut_ElecVsPion[ifile]);
    cout << endl;
    cout << label[ifile] << ": " << endl;
    cout << endl;
    cout << "Cut on dEdx = " << cut_ElecVsPion[ifile] << endl;
    cout << "Prob. Misid (Electron as Pion) = " << pvalue_elec << endl;
    cout << "Prob. Misid (Pion as Electron) = " << pvalue_pion << endl;
    cout << endl;

    hElecMisidAsPion[ifile]->SetBinContent(1,pvalue_elec);
    hPionMisidAsElec[ifile]->SetBinContent(1,pvalue_pion);
  
    // momenta bins
    for(int imom=0;imom<NBinsMom;imom++){
      pvalue_elec = GetPvalue_Histo_rightside( hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom],cut_ElecVsPion[ifile]);
      pvalue_pion = GetPvalue_Histo_leftside(hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom],cut_ElecVsPion[ifile]);
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format(" - Momentum: %1.0f - %1.0f",lowmom,upmom);
      cout << name << endl;
      cout << "Prob. Misid (Elec as Pion) = " << pvalue_elec << endl;
      cout << "Prob. Misid (Pion as Electron) = " << pvalue_pion << endl;

      hElecMisidAsPion_VsTrMom[ifile]->SetBinContent(histobin,pvalue_elec);
      hPionMisidAsElec_VsTrMom[ifile]->SetBinContent(histobin,pvalue_pion);
    }    

    // track length bins
    for(int ilen=0;ilen<NBinsLen;ilen++){
      pvalue_elec = GetPvalue_Histo_rightside( hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen],cut_ElecVsPion[ifile]);
      pvalue_pion = GetPvalue_Histo_leftside(hPion_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen],cut_ElecVsPion[ifile]);
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format(" - Track Length: %1.0f - %1.0f",lowlen,uplen);
      cout << name << endl;
      cout << "Prob. Misid (Elec as Pion) = " << pvalue_elec << endl;
      cout << "Prob. Misid (Pion as Electron) = " << pvalue_pion << endl;

      hElecMisidAsPion_VsTrLen[ifile]->SetBinContent(histobin,pvalue_elec);
      hPionMisidAsElec_VsTrLen[ifile]->SetBinContent(histobin,pvalue_pion);
    }    
  }


  // Electron Vs Proton
  cout << endl;
  cout << "/// PID Electron Vs Proton ///" << endl;

  for(int ifile=0;ifile<NFiles;ifile++){
    double pvalue_elec = GetPvalue_Histo_rightside( hElec_Stopped_EdepOverLen_ManyBins[ifile],cut_ElecVsProt[ifile]);
    double pvalue_prot = GetPvalue_Histo_leftside(hProt_Stopped_EdepOverLen_ManyBins[ifile],cut_ElecVsProt[ifile]);
    cout << endl;
    cout << label[ifile] << ": " << endl;
    cout << endl;
    cout << "Cut on dEdx = " << cut_ElecVsProt[ifile] << endl;
    cout << "Prob. Misid (Electron as Proton) = " << pvalue_elec << endl;
    cout << "Prob. Misid (Proton as Electron) = " << pvalue_prot << endl;
    cout << endl;

    hElecMisidAsProt[ifile]->SetBinContent(1,pvalue_elec);
    hProtMisidAsElec[ifile]->SetBinContent(1,pvalue_prot);
  
    // momenta bins
    for(int imom=0;imom<NBinsMom;imom++){
      pvalue_elec = GetPvalue_Histo_rightside( hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom],cut_ElecVsProt[ifile]);
      pvalue_prot = GetPvalue_Histo_leftside(hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom],cut_ElecVsProt[ifile]);
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format(" - Momentum: %1.0f - %1.0f",lowmom,upmom);
      cout << name << endl;
      cout << "Prob. Misid (Elec as Proton) = " << pvalue_elec << endl;
      cout << "Prob. Misid (Proton as Electron) = " << pvalue_prot << endl;

      hElecMisidAsProt_VsTrMom[ifile]->SetBinContent(histobin,pvalue_elec);
      hProtMisidAsElec_VsTrMom[ifile]->SetBinContent(histobin,pvalue_prot);
    }    

    // track length bins
    for(int ilen=0;ilen<NBinsLen;ilen++){
      pvalue_elec = GetPvalue_Histo_rightside( hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen],cut_ElecVsProt[ifile]);
      pvalue_prot = GetPvalue_Histo_leftside(hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen],cut_ElecVsProt[ifile]);
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format(" - Track Length: %1.0f - %1.0f",lowlen,uplen);
      cout << name << endl;
      cout << "Prob. Misid (Elec as Proton) = " << pvalue_elec << endl;
      cout << "Prob. Misid (Proton as Electron) = " << pvalue_prot << endl;

      hElecMisidAsProt_VsTrLen[ifile]->SetBinContent(histobin,pvalue_elec);
      hProtMisidAsElec_VsTrLen[ifile]->SetBinContent(histobin,pvalue_prot);
    }    
  }






    // should be already normalized to 1, since it's used as PDF
    
    // Momentum bins

  for(int ifile=0;ifile<NFiles;ifile++){

    hMuon_Stopped_EdepOverLen[ifile]->Scale(1./hMuon_Stopped_EdepOverLen[ifile]->Integral());
    hPion_Stopped_EdepOverLen[ifile]->Scale(1./hPion_Stopped_EdepOverLen[ifile]->Integral());
    hProt_Stopped_EdepOverLen[ifile]->Scale(1./hProt_Stopped_EdepOverLen[ifile]->Integral());
    hElec_Stopped_EdepOverLen[ifile]->Scale(1./hElec_Stopped_EdepOverLen[ifile]->Integral());    

    hMuon_Stopped_EdepOverLen_ManyBins[ifile]->Scale(1./hMuon_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
    hPion_Stopped_EdepOverLen_ManyBins[ifile]->Scale(1./hPion_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
    hProt_Stopped_EdepOverLen_ManyBins[ifile]->Scale(1./hProt_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
    hElec_Stopped_EdepOverLen_ManyBins[ifile]->Scale(1./hElec_Stopped_EdepOverLen_ManyBins[ifile]->Integral());    


    for(int imom=0;imom<NBinsMom;imom++){

      if(hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->Integral() > 0){
      	hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->Integral());
      	//hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hMuon_Stopped_EdepOverLen[ifile]->Integral());
      }
      if(hPion_Stopped_EdepOverLen_Mom[ifile][imom]->Integral() > 0){
      	hPion_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hPion_Stopped_EdepOverLen_Mom[ifile][imom]->Integral());
      	//hPion_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hPion_Stopped_EdepOverLen[ifile]->Integral());
      }
      if(hProt_Stopped_EdepOverLen_Mom[ifile][imom]->Integral() > 0){
      	hProt_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hProt_Stopped_EdepOverLen_Mom[ifile][imom]->Integral());
      	//hProt_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hProt_Stopped_EdepOverLen[ifile]->Integral());
      } 
      if(hElec_Stopped_EdepOverLen_Mom[ifile][imom]->Integral() > 0){
      	hElec_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hElec_Stopped_EdepOverLen_Mom[ifile][imom]->Integral());
      	//hElec_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hElec_Stopped_EdepOverLen[ifile]->Integral());
      }
      
      if(hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() > 0){
      	hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral());
      	//hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hMuon_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
      }
      if(hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() > 0){
      	hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral());
      	//hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hPion_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
      }
      if(hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() > 0){
      	hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral());
      	//hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hProt_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
      }
      if(hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() > 0){
      	hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral());
      	//hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hElec_Stopped_EdepOverLen_ManyBins[ifile]->Integral());
      }

    }


    // Track Length bins
    
    for(int ilen=0;ilen<NBinsLen;ilen++){

      if(hMuon_Stopped_EdepOverLen_Len[ifile][ilen]->Integral() > 0){
    	hMuon_Stopped_EdepOverLen_Len[ifile][ilen]->Scale(1./hMuon_Stopped_EdepOverLen_Len[ifile][ilen]->Integral());
      }
      if(hPion_Stopped_EdepOverLen_Len[ifile][ilen]->Integral() > 0){
    	hPion_Stopped_EdepOverLen_Len[ifile][ilen]->Scale(1./hPion_Stopped_EdepOverLen_Len[ifile][ilen]->Integral());
      }
      if(hProt_Stopped_EdepOverLen_Len[ifile][ilen]->Integral() > 0){
    	hProt_Stopped_EdepOverLen_Len[ifile][ilen]->Scale(1./hProt_Stopped_EdepOverLen_Len[ifile][ilen]->Integral());
      } 
      if(hElec_Stopped_EdepOverLen_Len[ifile][ilen]->Integral() > 0){
    	hElec_Stopped_EdepOverLen_Len[ifile][ilen]->Scale(1./hElec_Stopped_EdepOverLen_Len[ifile][ilen]->Integral());
      }
      
      if(hMuon_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral() > 0){
    	hMuon_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Scale(1./hMuon_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral());
      }
      if(hPion_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral() > 0){
    	hPion_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Scale(1./hPion_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral());
      }
      if(hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral() > 0){
    	hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Scale(1./hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral());
      }
      if(hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral() > 0){
    	hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Scale(1./hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen]->Integral());
      }

    }
  } 





  //
  // Draw 
  //

  for(int ifile=0;ifile<NFiles;ifile++){
    hMuon_Stopped_EdepOverLen_ManyBins[ifile]->Rebin(RebinNum);
    hPion_Stopped_EdepOverLen_ManyBins[ifile]->Rebin(RebinNum);
    hProt_Stopped_EdepOverLen_ManyBins[ifile]->Rebin(RebinNum);
    hElec_Stopped_EdepOverLen_ManyBins[ifile]->Rebin(RebinNum);
  }
  
  TH1F *histo_Part1;
  TH1F *histo_Part2;

  TLegend *leg = new TLegend(0.4,0.15,0.6,0.4);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->SetTextFont(132);
  leg->SetTextSize(0.05);
  for(int ifile=0;ifile<NFiles;ifile++){
    if(UseManyBins) leg->AddEntry(hMuon_Stopped_EdepOverLen_ManyBins[ifile],label[ifile],"l");
    else            leg->AddEntry(hMuon_Stopped_EdepOverLen_ManyBins[ifile],label[ifile],"l");
  }
  
  TLegend *leg_particle = new TLegend(0.4,0.15,0.6,0.4);
  leg_particle->SetFillColor(0);
  leg_particle->SetLineColor(0);
  leg_particle->SetTextFont(132);
  leg_particle->SetTextSize(0.05);
  if(UseManyBins){
    leg_particle->AddEntry(hMuon_Stopped_EdepOverLen_ManyBins[0],"True Muon","l");
    leg_particle->AddEntry(hProt_Stopped_EdepOverLen_ManyBins[0],"True Proton","l");
    leg_particle->AddEntry(hPion_Stopped_EdepOverLen_ManyBins[0],"True Pion","l");
    leg_particle->AddEntry(hElec_Stopped_EdepOverLen_ManyBins[0],"True Electron","l");
  }
  else{
    leg_particle->AddEntry(hMuon_Stopped_EdepOverLen[0],"True Muon","l");
    leg_particle->AddEntry(hProt_Stopped_EdepOverLen[0],"True Proton","l");
    leg_particle->AddEntry(hPion_Stopped_EdepOverLen[0],"True Pion","l");
    leg_particle->AddEntry(hElec_Stopped_EdepOverLen[0],"True Electron","l");
  }

  TLegend *leg_MuonVsProt = new TLegend(0.4,0.15,0.6,0.4);
  leg_MuonVsProt->SetFillColor(0);
  leg_MuonVsProt->SetLineColor(0);
  leg_MuonVsProt->SetTextFont(132);
  leg_MuonVsProt->SetTextSize(0.05);
  if(UseManyBins){
    leg_MuonVsProt->AddEntry(hMuon_Stopped_EdepOverLen_ManyBins[0],"True Muon","l");
    leg_MuonVsProt->AddEntry(hProt_Stopped_EdepOverLen_ManyBins[0],"True Proton","l");
  }
  else{
    leg_MuonVsProt->AddEntry(hMuon_Stopped_EdepOverLen[0],"True Muon","l");
    leg_MuonVsProt->AddEntry(hProt_Stopped_EdepOverLen[0],"True Proton","l");
  }
    
  TLegend *leg_MuonVsPion = new TLegend(0.4,0.15,0.6,0.4);
  leg_MuonVsPion->SetFillColor(0);
  leg_MuonVsPion->SetLineColor(0);
  leg_MuonVsPion->SetTextFont(132);
  leg_MuonVsPion->SetTextSize(0.05);
  if(UseManyBins){
    leg_MuonVsPion->AddEntry(hMuon_Stopped_EdepOverLen_ManyBins[0],"True Muon","l");
    leg_MuonVsPion->AddEntry(hPion_Stopped_EdepOverLen_ManyBins[0],"True Pion","l");
  }
  else{
    leg_MuonVsPion->AddEntry(hMuon_Stopped_EdepOverLen[0],"True Muon","l");
    leg_MuonVsPion->AddEntry(hPion_Stopped_EdepOverLen[0],"True Pion","l");
  }

  TLegend *leg_PionVsProt = new TLegend(0.4,0.15,0.6,0.4);
  leg_PionVsProt->SetFillColor(0);
  leg_PionVsProt->SetLineColor(0);
  leg_PionVsProt->SetTextFont(132);
  leg_PionVsProt->SetTextSize(0.05);
  if(UseManyBins){
    leg_PionVsProt->AddEntry(hPion_Stopped_EdepOverLen_ManyBins[0],"True Pion","l");
    leg_PionVsProt->AddEntry(hProt_Stopped_EdepOverLen_ManyBins[0],"True Proton","l");
  }
  else{
    leg_PionVsProt->AddEntry(hPion_Stopped_EdepOverLen[0],"True Pion","l");
    leg_PionVsProt->AddEntry(hProt_Stopped_EdepOverLen[0],"True Proton","l");
  }

  TLegend *leg_ElecVsMuon = new TLegend(0.4,0.15,0.6,0.4);
  leg_ElecVsMuon->SetFillColor(0);
  leg_ElecVsMuon->SetLineColor(0);
  leg_ElecVsMuon->SetTextFont(132);
  leg_ElecVsMuon->SetTextSize(0.05);
  if(UseManyBins){
    leg_ElecVsMuon->AddEntry(hElec_Stopped_EdepOverLen_ManyBins[0],"True Electron","l");
    leg_ElecVsMuon->AddEntry(hMuon_Stopped_EdepOverLen_ManyBins[0],"True Muon","l");
  }
  else{
    leg_ElecVsMuon->AddEntry(hElec_Stopped_EdepOverLen[0],"True Electron","l");
    leg_ElecVsMuon->AddEntry(hMuon_Stopped_EdepOverLen[0],"True Muon","l");
  }

  TLegend *leg_ElecVsPion = new TLegend(0.4,0.15,0.6,0.4);
  leg_ElecVsPion->SetFillColor(0);
  leg_ElecVsPion->SetLineColor(0);
  leg_ElecVsPion->SetTextFont(132);
  leg_ElecVsPion->SetTextSize(0.05);
  if(UseManyBins){
    leg_ElecVsPion->AddEntry(hElec_Stopped_EdepOverLen_ManyBins[0],"True Electron","l");
    leg_ElecVsPion->AddEntry(hPion_Stopped_EdepOverLen_ManyBins[0],"True Pion","l");
  }
  else{
    leg_ElecVsPion->AddEntry(hElec_Stopped_EdepOverLen[0],"True Electron","l");
    leg_ElecVsPion->AddEntry(hPion_Stopped_EdepOverLen[0],"True Pion","l");
  }

  TLegend *leg_ElecVsProt = new TLegend(0.4,0.15,0.6,0.4);
  leg_ElecVsProt->SetFillColor(0);
  leg_ElecVsProt->SetLineColor(0);
  leg_ElecVsProt->SetTextFont(132);
  leg_ElecVsProt->SetTextSize(0.05);
  if(UseManyBins){
    leg_ElecVsProt->AddEntry(hElec_Stopped_EdepOverLen_ManyBins[0],"True Electron","l");
    leg_ElecVsProt->AddEntry(hProt_Stopped_EdepOverLen_ManyBins[0],"True Proton","l");
  }
  else{
    leg_ElecVsProt->AddEntry(hElec_Stopped_EdepOverLen[0],"True Electron","l");
    leg_ElecVsProt->AddEntry(hProt_Stopped_EdepOverLen[0],"True Proton","l");
  }

  
  //
  // Edep Over Length
  //

  //
  // Muon Vs Proton
  //

  TCanvas *cMuonVsProt_Stopped_EdepOverLen[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cMuonVsProt_Stopped_EdepOverLen_%s",label[ifile].Data());
    cMuonVsProt_Stopped_EdepOverLen[ifile] = new TCanvas(name,name);
    if(UseManyBins){
      histo_Part1 = (TH1F*) hMuon_Stopped_EdepOverLen_ManyBins[ifile];
      histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_ManyBins[ifile];
    }
    else{
      histo_Part1 = (TH1F*) hMuon_Stopped_EdepOverLen[ifile];
      histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen[ifile];
    }
    histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
    histo_Part1->GetYaxis()->SetTitle("# of events");
    histo_Part1->GetYaxis()->SetTitleSize(0.05);
    histo_Part1->GetXaxis()->SetTitleSize(0.05);
    histo_Part1->SetStats(0);
    histo_Part1->SetLineColor(kBlue);
    histo_Part1->SetLineWidth(2.);
    histo_Part2->SetStats(0);
    histo_Part2->SetLineColor(kRed);
    histo_Part2->SetLineWidth(2.);
    if(ifile==0){
      histo_Part1->DrawClone("");
      histo_Part2->DrawClone("same");
    }
    else{
      histo_Part1->DrawClone("same");
      histo_Part2->DrawClone("same");
    }
    if(doDrawLegend) leg_MuonVsProt->Draw();
    lCut_MuonVsProt[ifile]->Draw();
  }
  

  // Muon Vs Proton per Momentum bin
  
  TCanvas *cMuonVsProt_Stopped_EdepOverLen_Mom[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cMuonVsProt_Stopped_EdepOverLen_Mom_%s",label[ifile].Data());
    cMuonVsProt_Stopped_EdepOverLen_Mom[ifile] = new TCanvas(name,name);
    cMuonVsProt_Stopped_EdepOverLen_Mom[ifile]->Divide(3,2);
    
    for(int imom=0;imom<NBinsMom;imom++){
      cMuonVsProt_Stopped_EdepOverLen_Mom[ifile]->cd(imom+1);
      
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Momentum: %1.0f - %1.0f",lowmom,upmom);
      
      if(UseManyBins){
	histo_Part1 = (TH1F*) hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom];
	histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom];
      }
      else{
	histo_Part1 = (TH1F*) hMuon_Stopped_EdepOverLen_Mom[ifile][imom];
	histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_Mom[ifile][imom];
      }
      
      histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      histo_Part1->GetYaxis()->SetTitle("# of events");
      histo_Part1->GetYaxis()->SetTitleSize(0.05);
      histo_Part1->GetXaxis()->SetTitleSize(0.05);
      histo_Part1->SetStats(0);
      histo_Part1->SetLineColor(kBlue);
      histo_Part1->SetLineWidth(2.);
      histo_Part2->SetStats(0);
      histo_Part2->SetLineColor(kRed);
      histo_Part2->SetLineWidth(2.);
      if(ifile==0){
	histo_Part1->DrawClone("");
	histo_Part2->DrawClone("same");
      }
      else{
	histo_Part1->DrawClone("same");
	histo_Part2->DrawClone("same");
      }
      lCut_MuonVsProt[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }
  
  // Muon Vs Proton per Track Length bin
  
  TCanvas *cMuonVsProt_Stopped_EdepOverLen_Len[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cMuonVsProt_Stopped_EdepOverLen_Len_%s",label[ifile].Data());
    cMuonVsProt_Stopped_EdepOverLen_Len[ifile] = new TCanvas(name,name);
    cMuonVsProt_Stopped_EdepOverLen_Len[ifile]->Divide(3,2);
    
    for(int ilen=0;ilen<NBinsLen;ilen++){
      cMuonVsProt_Stopped_EdepOverLen_Len[ifile]->cd(ilen+1);
      
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Track Length: %1.0f - %1.0f",lowlen,uplen);
      
      if(UseManyBins){
	histo_Part1 = (TH1F*) hMuon_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen];
	histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen];
      }
      else{
	histo_Part1 = (TH1F*) hMuon_Stopped_EdepOverLen_Len[ifile][ilen];
	histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_Len[ifile][ilen];
      }
      
      histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      histo_Part1->GetYaxis()->SetTitle("# of events");
      histo_Part1->GetYaxis()->SetTitleSize(0.05);
      histo_Part1->GetXaxis()->SetTitleSize(0.05);
      histo_Part1->SetStats(0);
      histo_Part1->SetLineColor(kBlue);
      histo_Part1->SetLineWidth(2.);
      histo_Part2->SetStats(0);
      histo_Part2->SetLineColor(kRed);
      histo_Part2->SetLineWidth(2.);
      if(ifile==0){
	histo_Part1->DrawClone("");
	histo_Part2->DrawClone("same");
      }
      else{
	histo_Part1->DrawClone("same");
	histo_Part2->DrawClone("same");
      }
      lCut_MuonVsProt[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }


  //
  // Muon Vs Pion
  //

  TCanvas *cMuonVsPion_Stopped_EdepOverLen[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cMuonVsPion_Stopped_EdepOverLen_%s",label[ifile].Data());
    cMuonVsPion_Stopped_EdepOverLen[ifile] = new TCanvas(name,name);

    if(UseManyBins){
      histo_Part1 = (TH1F*) hMuon_Stopped_EdepOverLen_ManyBins[ifile];
      histo_Part2 = (TH1F*) hPion_Stopped_EdepOverLen_ManyBins[ifile];
    }
    else{
      histo_Part1 = (TH1F*) hMuon_Stopped_EdepOverLen[ifile];
      histo_Part2 = (TH1F*) hPion_Stopped_EdepOverLen[ifile];
    }
    histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
    histo_Part1->GetYaxis()->SetTitle("# of events");
    histo_Part1->GetYaxis()->SetTitleSize(0.05);
    histo_Part1->GetXaxis()->SetTitleSize(0.05);
    histo_Part1->SetStats(0);
    histo_Part1->SetLineColor(kBlue);
    histo_Part1->SetLineWidth(2.);
    histo_Part2->SetStats(0);
    histo_Part2->SetLineColor(kGreen);
    histo_Part2->SetLineWidth(2.);
    if(ifile==0){
      histo_Part1->DrawClone("");
      histo_Part2->DrawClone("same");
    }
    else{
      histo_Part1->DrawClone("same");
      histo_Part2->DrawClone("same");
    }
    if(doDrawLegend) leg_MuonVsPion->Draw();
    lCut_MuonVsPion[ifile]->Draw();
  }
  


  // Muon Vs Pion per Momentum bin
  
  TCanvas *cMuonVsPion_Stopped_EdepOverLen_Mom[NFiles];
  
  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cMuonVsPion_Stopped_EdepOverLen_Mom_%s",label[ifile].Data());
    cMuonVsPion_Stopped_EdepOverLen_Mom[ifile] = new TCanvas(name,name);
    cMuonVsPion_Stopped_EdepOverLen_Mom[ifile]->Divide(3,2);
    
    for(int imom=0;imom<NBinsMom;imom++){
      cMuonVsPion_Stopped_EdepOverLen_Mom[ifile]->cd(imom+1);
      
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Momentum: %1.0f - %1.0f",lowmom,upmom);

      if(UseManyBins){
	histo_Part1 = (TH1F*) hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom];
	histo_Part2 = (TH1F*) hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom];
      }
      else{
	histo_Part1 = (TH1F*) hMuon_Stopped_EdepOverLen_Mom[ifile][imom];
	histo_Part2 = (TH1F*) hPion_Stopped_EdepOverLen_Mom[ifile][imom];
      }
      histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      histo_Part1->GetYaxis()->SetTitle("# of events");
      histo_Part1->GetYaxis()->SetTitleSize(0.05);
      histo_Part1->GetXaxis()->SetTitleSize(0.05);
      histo_Part1->SetStats(0);
      histo_Part1->SetLineColor(kBlue);
      histo_Part1->SetLineWidth(2.);
      histo_Part2->SetStats(0);
      histo_Part2->SetLineColor(kGreen);
      histo_Part2->SetLineWidth(2.);
      if(ifile==0){
	histo_Part1->DrawClone("");
	histo_Part2->DrawClone("same");
      }
      else{
	histo_Part1->DrawClone("same");
	histo_Part2->DrawClone("same");
      }
      lCut_MuonVsPion[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }
  
  
  // Muon Vs Pion per Track Length bin
  
  TCanvas *cMuonVsPion_Stopped_EdepOverLen_Len[NFiles];
  
  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cMuonVsPion_Stopped_EdepOverLen_Len_%s",label[ifile].Data());
    cMuonVsPion_Stopped_EdepOverLen_Len[ifile] = new TCanvas(name,name);
    cMuonVsPion_Stopped_EdepOverLen_Len[ifile]->Divide(3,2);
    
    for(int ilen=0;ilen<NBinsLen;ilen++){
      cMuonVsPion_Stopped_EdepOverLen_Len[ifile]->cd(ilen+1);
      
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Track Length: %1.0f - %1.0f",lowlen,uplen);

      if(UseManyBins){
	histo_Part1 = (TH1F*) hMuon_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen];
	histo_Part2 = (TH1F*) hPion_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen];
      }
      else{
	histo_Part1 = (TH1F*) hMuon_Stopped_EdepOverLen_Len[ifile][ilen];
	histo_Part2 = (TH1F*) hPion_Stopped_EdepOverLen_Len[ifile][ilen];
      }
      histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      histo_Part1->GetYaxis()->SetTitle("# of events");
      histo_Part1->GetYaxis()->SetTitleSize(0.05);
      histo_Part1->GetXaxis()->SetTitleSize(0.05);
      histo_Part1->SetStats(0);
      histo_Part1->SetLineColor(kBlue);
      histo_Part1->SetLineWidth(2.);
      histo_Part2->SetStats(0);
      histo_Part2->SetLineColor(kGreen);
      histo_Part2->SetLineWidth(2.);
      if(ifile==0){
	histo_Part1->DrawClone("");
	histo_Part2->DrawClone("same");
      }
      else{
	histo_Part1->DrawClone("same");
	histo_Part2->DrawClone("same");
      }
      lCut_MuonVsPion[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
}


  //
  // Pion Vs Proton
  //

  TCanvas *cPionVsProt_Stopped_EdepOverLen[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cPionVsProt_Stopped_EdepOverLen_%s",label[ifile].Data());
    cPionVsProt_Stopped_EdepOverLen[ifile] = new TCanvas(name,name);

    if(UseManyBins){
      histo_Part1 = (TH1F*) hPion_Stopped_EdepOverLen_ManyBins[ifile];
      histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_ManyBins[ifile];
    }
    else{
      histo_Part1 = (TH1F*) hPion_Stopped_EdepOverLen[ifile];
      histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen[ifile];
    }
    histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
    histo_Part1->GetYaxis()->SetTitle("# of events");
    histo_Part1->GetYaxis()->SetTitleSize(0.05);
    histo_Part1->GetXaxis()->SetTitleSize(0.05);
    histo_Part1->SetStats(0);
    histo_Part1->SetLineColor(kGreen);
    histo_Part1->SetLineWidth(2.);
    histo_Part2->SetStats(0);
    histo_Part2->SetLineColor(kRed);
    histo_Part2->SetLineWidth(2.);
    if(ifile==0){
      histo_Part1->DrawClone("");
      histo_Part2->DrawClone("same");
    }
    else{
      histo_Part1->DrawClone("same");
      histo_Part2->DrawClone("same");
    }
    if(doDrawLegend) leg_PionVsProt->Draw();
    lCut_PionVsProt[ifile]->Draw();
  }


  // Pion Vs Prot per Momentum bin
  
  TCanvas *cPionVsProt_Stopped_EdepOverLen_Mom[NFiles];

  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cPionVsProt_Stopped_EdepOverLen_Mom_%s",label[ifile].Data());
    cPionVsProt_Stopped_EdepOverLen_Mom[ifile] = new TCanvas(name,name);
    cPionVsProt_Stopped_EdepOverLen_Mom[ifile]->Divide(3,2);
      
    for(int imom=0;imom<NBinsMom;imom++){
      cPionVsProt_Stopped_EdepOverLen_Mom[ifile]->cd(imom+1);
      
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Momentum: %1.0f - %1.0f",lowmom,upmom);
      
      if(UseManyBins){
	histo_Part1 = (TH1F*) hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom];
	histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom];
      }
      else{
	histo_Part1 = (TH1F*) hPion_Stopped_EdepOverLen_Mom[ifile][imom];
	histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_Mom[ifile][imom];
      }
      histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      histo_Part1->GetYaxis()->SetTitle("# of events");
      histo_Part1->GetYaxis()->SetTitleSize(0.05);
      histo_Part1->GetXaxis()->SetTitleSize(0.05);
      histo_Part1->SetStats(0);
      histo_Part1->SetLineColor(kGreen);
      histo_Part1->SetLineWidth(2.);
      histo_Part2->SetStats(0);
      histo_Part2->SetLineColor(kRed);
      histo_Part2->SetLineWidth(2.);
      if(ifile==0){
	histo_Part1->DrawClone("");
	histo_Part2->DrawClone("same");
      }
      else{
	histo_Part1->DrawClone("same");
	histo_Part2->DrawClone("same");
      }
       lCut_PionVsProt[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }

  // Pion Vs Prot per Track Length bin
  
  TCanvas *cPionVsProt_Stopped_EdepOverLen_Len[NFiles];

  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cPionVsProt_Stopped_EdepOverLen_Len_%s",label[ifile].Data());
    cPionVsProt_Stopped_EdepOverLen_Len[ifile] = new TCanvas(name,name);
    cPionVsProt_Stopped_EdepOverLen_Len[ifile]->Divide(3,2);
      
    for(int ilen=0;ilen<NBinsLen;ilen++){
      cPionVsProt_Stopped_EdepOverLen_Len[ifile]->cd(ilen+1);
      
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Track Length: %1.0f - %1.0f",lowlen,uplen);
      
      if(UseManyBins){
	histo_Part1 = (TH1F*) hPion_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen];
	histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen];
      }
      else{
	histo_Part1 = (TH1F*) hPion_Stopped_EdepOverLen_Len[ifile][ilen];
	histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_Len[ifile][ilen];
      }
      histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      histo_Part1->GetYaxis()->SetTitle("# of events");
      histo_Part1->GetYaxis()->SetTitleSize(0.05);
      histo_Part1->GetXaxis()->SetTitleSize(0.05);
      histo_Part1->SetStats(0);
      histo_Part1->SetLineColor(kGreen);
      histo_Part1->SetLineWidth(2.);
      histo_Part2->SetStats(0);
      histo_Part2->SetLineColor(kRed);
      histo_Part2->SetLineWidth(2.);
      if(ifile==0){
	histo_Part1->DrawClone("");
	histo_Part2->DrawClone("same");
      }
      else{
	histo_Part1->DrawClone("same");
	histo_Part2->DrawClone("same");
      }
       lCut_PionVsProt[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }


  //
  // Electron Vs Muon
  //

  TCanvas *cElecVsMuon_Stopped_EdepOverLen[NFiles];
  
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cElecVsMuon_Stopped_EdepOverLen_%s",label[ifile].Data());
    cElecVsMuon_Stopped_EdepOverLen[ifile] = new TCanvas(name,name);
    if(UseManyBins){
      histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_ManyBins[ifile];
      histo_Part2 = (TH1F*) hMuon_Stopped_EdepOverLen_ManyBins[ifile];
    }
    else{
      histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen[ifile];
      histo_Part2 = (TH1F*) hMuon_Stopped_EdepOverLen[ifile];
    }
    histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
    histo_Part1->GetYaxis()->SetTitle("# of events");
    histo_Part1->GetYaxis()->SetTitleSize(0.05);
    histo_Part1->GetXaxis()->SetTitleSize(0.05);
    histo_Part1->SetStats(0);
    histo_Part1->SetLineColor(kCyan);
    histo_Part1->SetLineWidth(2.);
    histo_Part2->SetStats(0);
    histo_Part2->SetLineColor(kBlue);
    histo_Part2->SetLineWidth(2.);
    if(ifile==0){
      histo_Part1->DrawClone("");
      histo_Part2->DrawClone("same");
    }
    else{
      histo_Part1->DrawClone("same");
      histo_Part2->DrawClone("same");
    }
   if(doDrawLegend) leg_ElecVsMuon->Draw();
    lCut_ElecVsMuon[ifile]->Draw();
  }
    

  // Electron Vs Muon per Momentum bin
  
  TCanvas *cElecVsMuon_Stopped_EdepOverLen_Mom[NFiles];

  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cElecVsMuon_Stopped_EdepOverLen_Mom_%s",label[ifile].Data());
    cElecVsMuon_Stopped_EdepOverLen_Mom[ifile] = new TCanvas(name,name);
    cElecVsMuon_Stopped_EdepOverLen_Mom[ifile]->Divide(3,2);
      
    for(int imom=0;imom<NBinsMom;imom++){
      cElecVsMuon_Stopped_EdepOverLen_Mom[ifile]->cd(imom+1);
      
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Momentum: %1.0f - %1.0f",lowmom,upmom);
      
      if(UseManyBins){
	histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom];
	histo_Part2 = (TH1F*) hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom];
      }
      else{
	histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_Mom[ifile][imom];
	histo_Part2 = (TH1F*) hMuon_Stopped_EdepOverLen_Mom[ifile][imom];
      }
      
      histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      histo_Part1->GetYaxis()->SetTitle("# of events");
      histo_Part1->GetYaxis()->SetTitleSize(0.05);
      histo_Part1->GetXaxis()->SetTitleSize(0.05);
      histo_Part1->SetStats(0);
      histo_Part1->SetLineColor(kCyan);
      histo_Part1->SetLineWidth(2.);
      histo_Part2->SetStats(0);
      histo_Part2->SetLineColor(kBlue);
      histo_Part2->SetLineWidth(2.);
      if(ifile==0){
	histo_Part1->DrawClone("");
	histo_Part2->DrawClone("same");
      }
      else{
	histo_Part1->DrawClone("same");
	histo_Part2->DrawClone("same");
      }

      lCut_ElecVsMuon[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }


  // Electron Vs Muon per Track Length bin
  
  TCanvas *cElecVsMuon_Stopped_EdepOverLen_Len[NFiles];

  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cElecVsMuon_Stopped_EdepOverLen_Len_%s",label[ifile].Data());
    cElecVsMuon_Stopped_EdepOverLen_Len[ifile] = new TCanvas(name,name);
    cElecVsMuon_Stopped_EdepOverLen_Len[ifile]->Divide(3,2);
      
    for(int ilen=0;ilen<NBinsLen;ilen++){
      cElecVsMuon_Stopped_EdepOverLen_Len[ifile]->cd(ilen+1);
      
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Track Length: %1.0f - %1.0f",lowlen,uplen);
      
      if(UseManyBins){
	histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen];
	histo_Part2 = (TH1F*) hMuon_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen];
      }
      else{
	histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_Len[ifile][ilen];
	histo_Part2 = (TH1F*) hMuon_Stopped_EdepOverLen_Len[ifile][ilen];
      }
      
      histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      histo_Part1->GetYaxis()->SetTitle("# of events");
      histo_Part1->GetYaxis()->SetTitleSize(0.05);
      histo_Part1->GetXaxis()->SetTitleSize(0.05);
      histo_Part1->SetStats(0);
      histo_Part1->SetLineColor(kCyan);
      histo_Part1->SetLineWidth(2.);
      histo_Part2->SetStats(0);
      histo_Part2->SetLineColor(kBlue);
      histo_Part2->SetLineWidth(2.);
      if(ifile==0){
	histo_Part1->DrawClone("");
	histo_Part2->DrawClone("same");
      }
      else{
	histo_Part1->DrawClone("same");
	histo_Part2->DrawClone("same");
      }

      lCut_ElecVsMuon[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }


  //
  // Electron Vs Pion
  //

  TCanvas *cElecVsPion_Stopped_EdepOverLen[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cElecVsPion_Stopped_EdepOverLen_%s",label[ifile].Data());
    cElecVsPion_Stopped_EdepOverLen[ifile] = new TCanvas(name,name);

    if(UseManyBins){
      histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_ManyBins[ifile];
      histo_Part2 = (TH1F*) hPion_Stopped_EdepOverLen_ManyBins[ifile];
    }
    else{
      histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen[ifile];
      histo_Part2 = (TH1F*) hPion_Stopped_EdepOverLen[ifile];
    }
    histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
    histo_Part1->GetYaxis()->SetTitle("# of events");
    histo_Part1->GetYaxis()->SetTitleSize(0.05);
    histo_Part1->GetXaxis()->SetTitleSize(0.05);
    histo_Part1->SetStats(0);
    histo_Part1->SetLineColor(kCyan);
    histo_Part1->SetLineWidth(2.);
    histo_Part2->SetStats(0);
    histo_Part2->SetLineColor(kGreen);
    histo_Part2->SetLineWidth(2.);
    if(ifile==0){
      histo_Part1->DrawClone("");
      histo_Part2->DrawClone("same");
    }
    else{
      histo_Part1->DrawClone("same");
      histo_Part2->DrawClone("same");
    }
 
    if(doDrawLegend) leg_ElecVsPion->Draw();
    lCut_ElecVsPion[ifile]->Draw();
  }

  // Electron Vs Pion per Momentum bin
  
  TCanvas *cElecVsPion_Stopped_EdepOverLen_Mom[NFiles];

  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cElecVsPion_Stopped_EdepOverLen_Mom_%s",label[ifile].Data());
    cElecVsPion_Stopped_EdepOverLen_Mom[ifile] = new TCanvas(name,name);
    cElecVsPion_Stopped_EdepOverLen_Mom[ifile]->Divide(3,2);
      
    for(int imom=0;imom<NBinsMom;imom++){
      cElecVsPion_Stopped_EdepOverLen_Mom[ifile]->cd(imom+1);
      
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Momentum: %1.0f - %1.0f",lowmom,upmom);
      
      if(UseManyBins){
	histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom];
	histo_Part2 = (TH1F*) hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom];
      }
      else{
	histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_Mom[ifile][imom];
	histo_Part2 = (TH1F*) hPion_Stopped_EdepOverLen_Mom[ifile][imom];
      }
      
      histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      histo_Part1->GetYaxis()->SetTitle("# of events");
      histo_Part1->GetYaxis()->SetTitleSize(0.05);
      histo_Part1->GetXaxis()->SetTitleSize(0.05);
      histo_Part1->SetStats(0);
      histo_Part1->SetLineColor(kCyan);
      histo_Part1->SetLineWidth(2.);
      histo_Part2->SetStats(0);
      histo_Part2->SetLineColor(kGreen);
      histo_Part2->SetLineWidth(2.);
      if(ifile==0){
	histo_Part1->DrawClone("");
	histo_Part2->DrawClone("same");
      }
      else{
	histo_Part1->DrawClone("same");
	histo_Part2->DrawClone("same");
      }

     lCut_ElecVsPion[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }



  // Electron Vs Pion per Track Length bin
  
  TCanvas *cElecVsPion_Stopped_EdepOverLen_Len[NFiles];

  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cElecVsPion_Stopped_EdepOverLen_Len_%s",label[ifile].Data());
    cElecVsPion_Stopped_EdepOverLen_Len[ifile] = new TCanvas(name,name);
    cElecVsPion_Stopped_EdepOverLen_Len[ifile]->Divide(3,2);
      
    for(int ilen=0;ilen<NBinsLen;ilen++){
      cElecVsPion_Stopped_EdepOverLen_Len[ifile]->cd(ilen+1);
      
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Track Length: %1.0f - %1.0f",lowlen,uplen);
      
      if(UseManyBins){
	histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen];
	histo_Part2 = (TH1F*) hPion_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen];
      }
      else{
	histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_Len[ifile][ilen];
	histo_Part2 = (TH1F*) hPion_Stopped_EdepOverLen_Len[ifile][ilen];
      }
      
      histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      histo_Part1->GetYaxis()->SetTitle("# of events");
      histo_Part1->GetYaxis()->SetTitleSize(0.05);
      histo_Part1->GetXaxis()->SetTitleSize(0.05);
      histo_Part1->SetStats(0);
      histo_Part1->SetLineColor(kCyan);
      histo_Part1->SetLineWidth(2.);
      histo_Part2->SetStats(0);
      histo_Part2->SetLineColor(kGreen);
      histo_Part2->SetLineWidth(2.);
      if(ifile==0){
	histo_Part1->DrawClone("");
	histo_Part2->DrawClone("same");
      }
      else{
	histo_Part1->DrawClone("same");
	histo_Part2->DrawClone("same");
      }

     lCut_ElecVsPion[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }


  //
  // Electron Vs Proton
  //
  
  TCanvas *cElecVsProt_Stopped_EdepOverLen[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cElecVsProt_Stopped_EdepOverLen_%s",label[ifile].Data());
    cElecVsProt_Stopped_EdepOverLen[ifile] = new TCanvas(name,name);

    if(UseManyBins){
      histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_ManyBins[ifile];
      histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_ManyBins[ifile];
    }
    else{
      histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen[ifile];
      histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen[ifile];
    }
    histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
    histo_Part1->GetYaxis()->SetTitle("# of events");
    histo_Part1->GetYaxis()->SetTitleSize(0.05);
    histo_Part1->GetXaxis()->SetTitleSize(0.05);
    histo_Part1->SetStats(0);
    histo_Part1->SetLineColor(kCyan);
    histo_Part1->SetLineWidth(2.);
    histo_Part2->SetStats(0);
    histo_Part2->SetLineColor(kRed);
    histo_Part2->SetLineWidth(2.);
    if(ifile==0){
      histo_Part1->DrawClone("");
      histo_Part2->DrawClone("same");
    }
    else{
      histo_Part1->DrawClone("same");
      histo_Part2->DrawClone("same");
    }

    if(doDrawLegend) leg_ElecVsProt->Draw();
    lCut_ElecVsProt[ifile]->Draw();
  }

  // Electron Vs Proton per Momentum bin
  
  TCanvas *cElecVsProt_Stopped_EdepOverLen_Mom[NFiles];

  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cElecVsProt_Stopped_EdepOverLen_Mom_%s",label[ifile].Data());
    cElecVsProt_Stopped_EdepOverLen_Mom[ifile] = new TCanvas(name,name);
    cElecVsProt_Stopped_EdepOverLen_Mom[ifile]->Divide(3,2);
      
    for(int imom=0;imom<NBinsMom;imom++){
      cElecVsProt_Stopped_EdepOverLen_Mom[ifile]->cd(imom+1);
      
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Momentum: %1.0f - %1.0f",lowmom,upmom);
      
      if(UseManyBins){
	histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom];
	histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom];
      }
      else{
	histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_Mom[ifile][imom];
	histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_Mom[ifile][imom];
      }
      
      histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      histo_Part1->GetYaxis()->SetTitle("# of events");
      histo_Part1->GetYaxis()->SetTitleSize(0.05);
      histo_Part1->GetXaxis()->SetTitleSize(0.05);
      histo_Part1->SetStats(0);
      histo_Part1->SetLineColor(kCyan);
      histo_Part1->SetLineWidth(2.);
      histo_Part2->SetStats(0);
      histo_Part2->SetLineColor(kRed);
      histo_Part2->SetLineWidth(2.);
      if(ifile==0){
	histo_Part1->DrawClone("");
	histo_Part2->DrawClone("same");
      }
      else{
	histo_Part1->DrawClone("same");
	histo_Part2->DrawClone("same");
      }

      lCut_ElecVsProt[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }


  // Electron Vs Proton per Track Length bin
  
  TCanvas *cElecVsProt_Stopped_EdepOverLen_Len[NFiles];

  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cElecVsProt_Stopped_EdepOverLen_Len_%s",label[ifile].Data());
    cElecVsProt_Stopped_EdepOverLen_Len[ifile] = new TCanvas(name,name);
    cElecVsProt_Stopped_EdepOverLen_Len[ifile]->Divide(3,2);
      
    for(int ilen=0;ilen<NBinsLen;ilen++){
      cElecVsProt_Stopped_EdepOverLen_Len[ifile]->cd(ilen+1);
      
      int histobin = ilen+1;
      double lowlen = hBinsLen[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double uplen  = hBinsLen[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Track Length: %1.0f - %1.0f",lowlen,uplen);
      
      if(UseManyBins){
	histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen];
	histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_Len_ManyBins[ifile][ilen];
      }
      else{
	histo_Part1 = (TH1F*) hElec_Stopped_EdepOverLen_Len[ifile][ilen];
	histo_Part2 = (TH1F*) hProt_Stopped_EdepOverLen_Len[ifile][ilen];
      }
      
      histo_Part1->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      histo_Part1->GetYaxis()->SetTitle("# of events");
      histo_Part1->GetYaxis()->SetTitleSize(0.05);
      histo_Part1->GetXaxis()->SetTitleSize(0.05);
      histo_Part1->SetStats(0);
      histo_Part1->SetLineColor(kCyan);
      histo_Part1->SetLineWidth(2.);
      histo_Part2->SetStats(0);
      histo_Part2->SetLineColor(kRed);
      histo_Part2->SetLineWidth(2.);
      if(ifile==0){
	histo_Part1->DrawClone("");
	histo_Part2->DrawClone("same");
      }
      else{
	histo_Part1->DrawClone("same");
	histo_Part2->DrawClone("same");
      }

      lCut_ElecVsProt[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }





  //
  // Edep Vs Length
  //

  // Reco 

  TH2F *hCanvas = new TH2F("hCanvas","hCanvas",1000,0,2000,1000,0,20000);

  TCanvas *cParticle_Stopped_EdepVsLen[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cParticle_Stopped_EdepVsLen_%s",label[ifile].Data());
    cParticle_Stopped_EdepVsLen[ifile] = new TCanvas(name,name);

    name = TString::Format("All particles");
    hCanvas->SetStats(0);
    hCanvas->SetTitle(name);
    hCanvas->SetName(name);
    hCanvas->GetXaxis()->SetTitle("Length (mm)");
    hCanvas->GetYaxis()->SetTitle("Edep (p.e.)"); 
    hCanvas->GetXaxis()->SetTitleSize(0.05);
    hCanvas->GetYaxis()->SetTitleSize(0.05);
    hCanvas->DrawClone();
    
    gMuon_Stopped_EdepVsLen[ifile]->SetMarkerColor(kBlue);
    gPion_Stopped_EdepVsLen[ifile]->SetMarkerColor(kGreen);
    gProt_Stopped_EdepVsLen[ifile]->SetMarkerColor(kRed);
    gElec_Stopped_EdepVsLen[ifile]->SetMarkerColor(kCyan);
    gMuon_Stopped_EdepVsLen[ifile]->SetMarkerSize(1.);
    gPion_Stopped_EdepVsLen[ifile]->SetMarkerSize(1.);
    gProt_Stopped_EdepVsLen[ifile]->SetMarkerSize(1.);
    gElec_Stopped_EdepVsLen[ifile]->SetMarkerSize(1.);
    gMuon_Stopped_EdepVsLen[ifile]->SetMarkerStyle(2.);
    gPion_Stopped_EdepVsLen[ifile]->SetMarkerStyle(2.);
    gProt_Stopped_EdepVsLen[ifile]->SetMarkerStyle(2.);
    gElec_Stopped_EdepVsLen[ifile]->SetMarkerStyle(2.);

    gProt_Stopped_EdepVsLen[ifile]->Draw("p same");
    gPion_Stopped_EdepVsLen[ifile]->Draw("p same");
    gMuon_Stopped_EdepVsLen[ifile]->Draw("p same");
    gElec_Stopped_EdepVsLen[ifile]->Draw("p same");
    if(doDrawLegend) leg_particle->Draw();
  }



  // Truth

  TH2F *hCanvas_Truth = new TH2F("hCanvas_Truth","hCanvas_Truths",1000,0,2000,1000,0,500);

  TCanvas *cParticle_Stopped_EdepVsLen_Truth[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cParticle_Stopped_EdepVsLen_Truth_%s",label[ifile].Data());
    cParticle_Stopped_EdepVsLen_Truth[ifile] = new TCanvas(name,name);

    name = TString::Format("All particles (Truth)");
    hCanvas_Truth->SetStats(0);
    hCanvas_Truth->SetTitle(name);
    hCanvas_Truth->SetName(name);
    hCanvas_Truth->GetXaxis()->SetTitle("Length (mm)");
    hCanvas_Truth->GetYaxis()->SetTitle("Edep (MeV)"); 
    hCanvas_Truth->GetYaxis()->SetTitleSize(0.05);
    hCanvas_Truth->GetXaxis()->SetTitleSize(0.05);
    hCanvas_Truth->DrawClone();

    gMuon_Stopped_EdepVsLen_Truth[ifile]->SetMarkerColor(kBlue);
    gPion_Stopped_EdepVsLen_Truth[ifile]->SetMarkerColor(kGreen);
    gProt_Stopped_EdepVsLen_Truth[ifile]->SetMarkerColor(kRed);
    gElec_Stopped_EdepVsLen_Truth[ifile]->SetMarkerColor(kCyan);
    gMuon_Stopped_EdepVsLen_Truth[ifile]->SetMarkerSize(1.);
    gPion_Stopped_EdepVsLen_Truth[ifile]->SetMarkerSize(1.);
    gProt_Stopped_EdepVsLen_Truth[ifile]->SetMarkerSize(1.);
    gElec_Stopped_EdepVsLen_Truth[ifile]->SetMarkerSize(1.);
    gMuon_Stopped_EdepVsLen_Truth[ifile]->SetMarkerStyle(2.);
    gPion_Stopped_EdepVsLen_Truth[ifile]->SetMarkerStyle(2.);
    gProt_Stopped_EdepVsLen_Truth[ifile]->SetMarkerStyle(2.);
    gElec_Stopped_EdepVsLen_Truth[ifile]->SetMarkerStyle(2.);

    gProt_Stopped_EdepVsLen_Truth[ifile]->Draw("p same");
    gPion_Stopped_EdepVsLen_Truth[ifile]->Draw("p same");
    gMuon_Stopped_EdepVsLen_Truth[ifile]->Draw("p same");
    gElec_Stopped_EdepVsLen_Truth[ifile]->Draw("p same");
    if(doDrawLegend) leg_particle->Draw();
  }

 
  // //
  // // Edep Vs Length per Momentum bin
  // //

  // // Reco 

  // // Momentum bins

  // TCanvas *cParticle_Stopped_EdepVsLen_Mom[NFiles];

  // for(int ifile=0;ifile<NFiles;ifile++){
    
  //   TString name = TString::Format("cParticle_Stopped_EdepVsLen_Mom_%s",label[ifile].Data());
  //   cParticle_Stopped_EdepVsLen_Mom[ifile] = new TCanvas(name,name);
  //   cParticle_Stopped_EdepVsLen_Mom[ifile]->Divide(3,2);
      
  //   for(int imom=0;imom<NBinsMom;imom++){
  //     cParticle_Stopped_EdepVsLen_Mom[ifile]->cd(imom+1);
      
  //     int histobin = imom+1;
  //     double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
  //     double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
  //     TString name = TString::Format("Momentum: %1.0f - %1.0f",lowmom,upmom);
  //     hCanvas->SetTitle(name);
  //     hCanvas->GetXaxis()->SetTitle("Length (mm)");
  //     hCanvas->GetYaxis()->SetTitle("Edep (p.e.)"); 
  //     hCanvas->GetYaxis()->SetTitleSize(0.05);
  //     hCanvas->GetXaxis()->SetTitleSize(0.05);
  //     hCanvas->DrawClone();
     
  //     gMuon_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerColor(kBlue);
  //     gPion_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerColor(kGreen);
  //     gProt_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerColor(kRed);
  //     gElec_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerColor(kCyan);

  //     gMuon_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerSize(1.);
  //     gPion_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerSize(1.);
  //     gProt_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerSize(1.);
  //     gElec_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerSize(1.);
      
  //     gMuon_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerStyle(2.);
  //     gPion_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerStyle(2.);
  //     gProt_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerStyle(2.);
  //     gElec_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerStyle(2.);
      
  //     if(gProt_Stopped_EdepVsLen_Mom[ifile][imom]->GetN() > 0.) gProt_Stopped_EdepVsLen_Mom[ifile][imom]->Draw("p same");      
  //     if(gPion_Stopped_EdepVsLen_Mom[ifile][imom]->GetN() > 0.) gPion_Stopped_EdepVsLen_Mom[ifile][imom]->Draw("p same");
  //     if(gMuon_Stopped_EdepVsLen_Mom[ifile][imom]->GetN() > 0.) gMuon_Stopped_EdepVsLen_Mom[ifile][imom]->Draw("p same");
  //     if(gElec_Stopped_EdepVsLen_Mom[ifile][imom]->GetN() > 0.) gElec_Stopped_EdepVsLen_Mom[ifile][imom]->Draw("p same");
  //   }
  //   if(doDrawLegend) leg_particle->Draw();    
  // }




  //
  // dEdx Vs Length
  //

  // Reco 

  TH2F *hCanvas_len = new TH2F("hCanvas_len","hCanvas_len",1000,0,2000,1500,0,150);

  TCanvas *cParticle_Stopped_LenVsdEdx[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cParticle_Stopped_LenVsdEdx_%s",label[ifile].Data());
    cParticle_Stopped_LenVsdEdx[ifile] = new TCanvas(name,name);
    
    name = TString::Format("Stopping particles");
    hCanvas_len->SetStats(0);
    hCanvas_len->SetTitle(name);
    hCanvas_len->SetName(name);
    hCanvas_len->GetXaxis()->SetTitle("Length (mm)");
    hCanvas_len->GetYaxis()->SetTitle("dE/dx (p.e.)"); 
    hCanvas_len->GetXaxis()->SetTitleSize(0.05);
    hCanvas_len->GetYaxis()->SetTitleSize(0.05);
    hCanvas_len->DrawClone();
    
    gMuon_Stopped_LenVsdEdx[ifile]->SetMarkerColor(kBlue);
    gPion_Stopped_LenVsdEdx[ifile]->SetMarkerColor(kGreen);
    gProt_Stopped_LenVsdEdx[ifile]->SetMarkerColor(kRed);
    gElec_Stopped_LenVsdEdx[ifile]->SetMarkerColor(kCyan);
    gMuon_Stopped_LenVsdEdx[ifile]->SetMarkerSize(1.);
    gPion_Stopped_LenVsdEdx[ifile]->SetMarkerSize(1.);
    gProt_Stopped_LenVsdEdx[ifile]->SetMarkerSize(1.);
    gElec_Stopped_LenVsdEdx[ifile]->SetMarkerSize(1.);
    gMuon_Stopped_LenVsdEdx[ifile]->SetMarkerStyle(2.);
    gPion_Stopped_LenVsdEdx[ifile]->SetMarkerStyle(2.);
    gProt_Stopped_LenVsdEdx[ifile]->SetMarkerStyle(2.);
    gElec_Stopped_LenVsdEdx[ifile]->SetMarkerStyle(2.);

    gProt_Stopped_LenVsdEdx[ifile]->Draw("p same");
    gPion_Stopped_LenVsdEdx[ifile]->Draw("p same");
    gMuon_Stopped_LenVsdEdx[ifile]->Draw("p same");
    gElec_Stopped_LenVsdEdx[ifile]->Draw("p same");
    if(doDrawLegend) leg_particle->Draw();
  }

  // Truth

  TH2F *hCanvas_len_Truth = new TH2F("hCanvas_len_Truth","hCanvas_len_Truth",1000,0,2000,1000,0,4);

  TCanvas *cParticle_Stopped_LenVsdEdx_Truth[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cParticle_Stopped_LenVsdEdx_Truth_%s",label[ifile].Data());
    cParticle_Stopped_LenVsdEdx_Truth[ifile] = new TCanvas(name,name);

    name = TString::Format("All particles (Truth)");
    hCanvas_len_Truth->SetStats(0);
    hCanvas_len_Truth->SetTitle(name);
    hCanvas_len_Truth->SetName(name);
    hCanvas_len_Truth->GetXaxis()->SetTitle("Length (mm)");
    hCanvas_len_Truth->GetYaxis()->SetTitle("dEdx (MeV)"); 
    hCanvas_len_Truth->GetYaxis()->SetTitleSize(0.05);
    hCanvas_len_Truth->GetXaxis()->SetTitleSize(0.05);
    hCanvas_len_Truth->DrawClone();

    gMuon_Stopped_LenVsdEdx_Truth[ifile]->SetMarkerColor(kBlue);
    gPion_Stopped_LenVsdEdx_Truth[ifile]->SetMarkerColor(kGreen);
    gProt_Stopped_LenVsdEdx_Truth[ifile]->SetMarkerColor(kRed);
    gElec_Stopped_LenVsdEdx_Truth[ifile]->SetMarkerColor(kCyan);
    gMuon_Stopped_LenVsdEdx_Truth[ifile]->SetMarkerSize(1.);
    gPion_Stopped_LenVsdEdx_Truth[ifile]->SetMarkerSize(1.);
    gProt_Stopped_LenVsdEdx_Truth[ifile]->SetMarkerSize(1.);
    gElec_Stopped_LenVsdEdx_Truth[ifile]->SetMarkerSize(1.);
    gMuon_Stopped_LenVsdEdx_Truth[ifile]->SetMarkerStyle(2.);
    gPion_Stopped_LenVsdEdx_Truth[ifile]->SetMarkerStyle(2.);
    gProt_Stopped_LenVsdEdx_Truth[ifile]->SetMarkerStyle(2.);
    gElec_Stopped_LenVsdEdx_Truth[ifile]->SetMarkerStyle(2.);

    gProt_Stopped_LenVsdEdx_Truth[ifile]->Draw("p same");
    gPion_Stopped_LenVsdEdx_Truth[ifile]->Draw("p same");
    gMuon_Stopped_LenVsdEdx_Truth[ifile]->Draw("p same");
    gElec_Stopped_LenVsdEdx_Truth[ifile]->Draw("p same");
    if(doDrawLegend) leg_particle->Draw();
  }




  //
  // dEdx Vs Momentum
  //

  // Reco 

  TH2F *hCanvas_mom = new TH2F("hCanvas_mom","hCanvas_mom",1000,0,2000,1500,0,150);

  TCanvas *cParticle_Stopped_dEdxVsMom[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cParticle_Stopped_MomVsdEdx_%s",label[ifile].Data());
    cParticle_Stopped_dEdxVsMom[ifile] = new TCanvas(name,name);

    name = TString::Format("Stopping particles");
    hCanvas_mom->SetStats(0);
    hCanvas_mom->SetTitle(name);
    hCanvas_mom->SetName(name);
    hCanvas_mom->GetXaxis()->SetTitle("Truth Momentum (MeV/c)");
    hCanvas_mom->GetYaxis()->SetTitle("dE/dx (p.e.)"); 
    hCanvas_mom->GetXaxis()->SetTitleSize(0.05);
    hCanvas_mom->GetYaxis()->SetTitleSize(0.05);
    hCanvas_mom->DrawClone();
    
    gMuon_Stopped_MomVsdEdx[ifile]->SetMarkerColor(kBlue);
    gPion_Stopped_MomVsdEdx[ifile]->SetMarkerColor(kGreen);
    gProt_Stopped_MomVsdEdx[ifile]->SetMarkerColor(kRed);
    gElec_Stopped_MomVsdEdx[ifile]->SetMarkerColor(kCyan);
    gMuon_Stopped_MomVsdEdx[ifile]->SetMarkerSize(1.);
    gPion_Stopped_MomVsdEdx[ifile]->SetMarkerSize(1.);
    gProt_Stopped_MomVsdEdx[ifile]->SetMarkerSize(1.);
    gElec_Stopped_MomVsdEdx[ifile]->SetMarkerSize(1.);
    gMuon_Stopped_MomVsdEdx[ifile]->SetMarkerStyle(2.);
    gPion_Stopped_MomVsdEdx[ifile]->SetMarkerStyle(2.);
    gProt_Stopped_MomVsdEdx[ifile]->SetMarkerStyle(2.);
    gElec_Stopped_MomVsdEdx[ifile]->SetMarkerStyle(2.);

    gProt_Stopped_MomVsdEdx[ifile]->Draw("p same");
    gPion_Stopped_MomVsdEdx[ifile]->Draw("p same");
    gMuon_Stopped_MomVsdEdx[ifile]->Draw("p same");
    gElec_Stopped_MomVsdEdx[ifile]->Draw("p same");
    if(doDrawLegend) leg_particle->Draw();
  }

  // Truth

  TH2F *hCanvas_mom_Truth = new TH2F("hCanvas_mom_Truth","hCanvas_mom_Truths",1000,0,2000,1000,0,4);

  TCanvas *cParticle_Stopped_MomVsdEdx_Truth[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cParticle_Stopped_MomVsdEdx_Truth_%s",label[ifile].Data());
    cParticle_Stopped_MomVsdEdx_Truth[ifile] = new TCanvas(name,name);

    name = TString::Format("Stopping particles (Truth)");
    hCanvas_mom_Truth->SetStats(0);
    hCanvas_mom_Truth->SetTitle(name);
    hCanvas_mom_Truth->SetName(name);
    hCanvas_mom_Truth->GetXaxis()->SetTitle("True Momentum (MeV/c)");
    hCanvas_mom_Truth->GetYaxis()->SetTitle("True dEdx (MeV)"); 
    hCanvas_mom_Truth->GetYaxis()->SetTitleSize(0.05);
    hCanvas_mom_Truth->GetXaxis()->SetTitleSize(0.05);
    hCanvas_mom_Truth->DrawClone();

    gMuon_Stopped_MomVsdEdx_Truth[ifile]->SetMarkerColor(kBlue);
    gPion_Stopped_MomVsdEdx_Truth[ifile]->SetMarkerColor(kGreen);
    gProt_Stopped_MomVsdEdx_Truth[ifile]->SetMarkerColor(kRed);
    gElec_Stopped_MomVsdEdx_Truth[ifile]->SetMarkerColor(kCyan);
    gMuon_Stopped_MomVsdEdx_Truth[ifile]->SetMarkerSize(1.);
    gPion_Stopped_MomVsdEdx_Truth[ifile]->SetMarkerSize(1.);
    gProt_Stopped_MomVsdEdx_Truth[ifile]->SetMarkerSize(1.);
    gElec_Stopped_MomVsdEdx_Truth[ifile]->SetMarkerSize(1.);
    gMuon_Stopped_MomVsdEdx_Truth[ifile]->SetMarkerStyle(2.);
    gPion_Stopped_MomVsdEdx_Truth[ifile]->SetMarkerStyle(2.);
    gProt_Stopped_MomVsdEdx_Truth[ifile]->SetMarkerStyle(2.);
    gElec_Stopped_MomVsdEdx_Truth[ifile]->SetMarkerStyle(2.);

    gProt_Stopped_MomVsdEdx_Truth[ifile]->Draw("p same");
    gPion_Stopped_MomVsdEdx_Truth[ifile]->Draw("p same");
    gMuon_Stopped_MomVsdEdx_Truth[ifile]->Draw("p same");
    gElec_Stopped_MomVsdEdx_Truth[ifile]->Draw("p same");
    if(doDrawLegend) leg_particle->Draw();
  }



  // Truth - All particles

  TH2F *hCanvas_mom_Truth_All = new TH2F("hCanvas_mom_Truth_All","hCanvas_mom_Truth_All",1000,0,2000,10000,0,4);

  TCanvas *cParticle_All_MomVsdEdx_Truth[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cParticle_All_MomVsdEdx_Truth_%s",label[ifile].Data());
    cParticle_All_MomVsdEdx_Truth[ifile] = new TCanvas(name,name);

    name = TString::Format("All particles (Truth)");
    hCanvas_mom_Truth_All->SetStats(0);
    hCanvas_mom_Truth_All->SetTitle(name);
    hCanvas_mom_Truth_All->SetName(name);
    hCanvas_mom_Truth_All->GetXaxis()->SetTitle("True Momentum (MeV/c)");
    hCanvas_mom_Truth_All->GetYaxis()->SetTitle("True dEdx (MeV)"); 
    hCanvas_mom_Truth_All->GetYaxis()->SetTitleSize(0.05);
    hCanvas_mom_Truth_All->GetXaxis()->SetTitleSize(0.05);
    hCanvas_mom_Truth_All->DrawClone();

    gMuon_All_MomVsdEdx_Truth[ifile]->SetMarkerColor(kBlue);
    gPion_All_MomVsdEdx_Truth[ifile]->SetMarkerColor(kGreen);
    gProt_All_MomVsdEdx_Truth[ifile]->SetMarkerColor(kRed);
    gElec_All_MomVsdEdx_Truth[ifile]->SetMarkerColor(kCyan);
    gMuon_All_MomVsdEdx_Truth[ifile]->SetMarkerSize(1.);
    gPion_All_MomVsdEdx_Truth[ifile]->SetMarkerSize(1.);
    gProt_All_MomVsdEdx_Truth[ifile]->SetMarkerSize(1.);
    gElec_All_MomVsdEdx_Truth[ifile]->SetMarkerSize(1.);
    gMuon_All_MomVsdEdx_Truth[ifile]->SetMarkerStyle(2.);
    gPion_All_MomVsdEdx_Truth[ifile]->SetMarkerStyle(2.);
    gProt_All_MomVsdEdx_Truth[ifile]->SetMarkerStyle(2.);
    gElec_All_MomVsdEdx_Truth[ifile]->SetMarkerStyle(2.);

    gProt_All_MomVsdEdx_Truth[ifile]->Draw("p same");
    gPion_All_MomVsdEdx_Truth[ifile]->Draw("p same");
    gMuon_All_MomVsdEdx_Truth[ifile]->Draw("p same");
    gElec_All_MomVsdEdx_Truth[ifile]->Draw("p same");
    if(doDrawLegend) leg_particle->Draw();
  
}














  


  /*

  //
  // Log-Likelihood ratio
  //

  // Muon Vs Proton

  TCanvas *cLikeRatio_MuProt[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cLikeRatio_MuProt_%s",label[ifile].Data());
    cLikeRatio_MuProt[ifile] = new TCanvas(name,name);
    hLikeRatio_MuProt_TrueMu[ifile]->GetXaxis()->SetTitle("LogLikelihood Ratio");
    hLikeRatio_MuProt_TrueMu[ifile]->GetYaxis()->SetTitle("# of events");
    hLikeRatio_MuProt_TrueMu[ifile]->GetYaxis()->SetTitleSize(0.05);
    hLikeRatio_MuProt_TrueMu[ifile]->GetXaxis()->SetTitleSize(0.05);
    hLikeRatio_MuProt_TrueMu[ifile]->SetStats(0);
    hLikeRatio_MuProt_TrueMu[ifile]->SetLineColor(kBlue);
    hLikeRatio_MuProt_TrueMu[ifile]->SetLineWidth(2.);
    hLikeRatio_MuProt_TrueProt[ifile]->SetStats(0);
    hLikeRatio_MuProt_TrueProt[ifile]->SetLineColor(kRed);
    hLikeRatio_MuProt_TrueProt[ifile]->SetLineWidth(2.);
    if(ifile==0){
      hLikeRatio_MuProt_TrueMu[ifile]->DrawClone("");
      hLikeRatio_MuProt_TrueProt[ifile]->DrawClone("same");
    }
    else{
      hLikeRatio_MuProt_TrueMu[ifile]->DrawClone("same");
      hLikeRatio_MuProt_TrueProt[ifile]->DrawClone("same");
    }
    if(doDrawLegend) leg_MuonVsProt->Draw();
    //lCut_MuonVsProt[ifile]->Draw();
  }

  // Muon Vs Pion

  TCanvas *cLikeRatio_MuPion[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cLikeRatio_MuPion_%s",label[ifile].Data());
    cLikeRatio_MuPion[ifile] = new TCanvas(name,name);
    hLikeRatio_MuPion_TrueMu[ifile]->GetXaxis()->SetTitle("LogLikelihood Ratio");
    hLikeRatio_MuPion_TrueMu[ifile]->GetYaxis()->SetTitle("# of events");
    hLikeRatio_MuPion_TrueMu[ifile]->GetYaxis()->SetTitleSize(0.05);
    hLikeRatio_MuPion_TrueMu[ifile]->GetXaxis()->SetTitleSize(0.05);
    hLikeRatio_MuPion_TrueMu[ifile]->SetStats(0);
    hLikeRatio_MuPion_TrueMu[ifile]->SetLineColor(kBlue);
    hLikeRatio_MuPion_TrueMu[ifile]->SetLineWidth(2.);
    hLikeRatio_MuPion_TruePion[ifile]->SetStats(0);
    hLikeRatio_MuPion_TruePion[ifile]->SetLineColor(kGreen);
    hLikeRatio_MuPion_TruePion[ifile]->SetLineWidth(2.);
    if(ifile==0){
      hLikeRatio_MuPion_TrueMu[ifile]->DrawClone("");
      hLikeRatio_MuPion_TruePion[ifile]->DrawClone("same");
    }
    else{
      hLikeRatio_MuPion_TrueMu[ifile]->DrawClone("same");
      hLikeRatio_MuPion_TruePion[ifile]->DrawClone("same");
    }
    if(doDrawLegend) leg_MuonVsPion->Draw();
    //lCut_MuonVsPion[ifile]->Draw();
  }

  // Pion Vs Prot

  TCanvas *cLikeRatio_PionProt[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cLikeRatio_PionProt_%s",label[ifile].Data());
    cLikeRatio_PionProt[ifile] = new TCanvas(name,name);
    hLikeRatio_PionProt_TruePion[ifile]->GetXaxis()->SetTitle("LogLikelihood Ratio");
    hLikeRatio_PionProt_TruePion[ifile]->GetYaxis()->SetTitle("# of events");
    hLikeRatio_PionProt_TruePion[ifile]->GetYaxis()->SetTitleSize(0.05);
    hLikeRatio_PionProt_TruePion[ifile]->GetXaxis()->SetTitleSize(0.05);
    hLikeRatio_PionProt_TruePion[ifile]->SetStats(0);
    hLikeRatio_PionProt_TruePion[ifile]->SetLineColor(kGreen);
    hLikeRatio_PionProt_TruePion[ifile]->SetLineWidth(2.);
    hLikeRatio_PionProt_TrueProt[ifile]->SetStats(0);
    hLikeRatio_PionProt_TrueProt[ifile]->SetLineColor(kRed);
    hLikeRatio_PionProt_TrueProt[ifile]->SetLineWidth(2.);
    if(ifile==0){
      hLikeRatio_PionProt_TruePion[ifile]->DrawClone("");
      hLikeRatio_PionProt_TrueProt[ifile]->DrawClone("same");
    }
    else{
      hLikeRatio_PionProt_TruePion[ifile]->DrawClone("same");
      hLikeRatio_PionProt_TrueProt[ifile]->DrawClone("same");
    }
    if(doDrawLegend) leg_PionVsProt->Draw();
    //lCut_MuonVsProt[ifile]->Draw();
  }

  */





  // Store PID probabilities
  
  TString name = TString::Format("TargetPID_%s.root",tag.Data());
  TFile *outfile = new TFile(name,"RECREATE");
  
  for(int ifile=0;ifile<NFiles;ifile++){
    hMuonMisidAsProt[ifile]->Write();
    hProtMisidAsMuon[ifile]->Write();
    hPionMisidAsProt[ifile]->Write();
    hProtMisidAsPion[ifile]->Write();
    hElecMisidAsProt[ifile]->Write();
    hProtMisidAsElec[ifile]->Write();

    hMuonMisidAsProt_VsTrMom[ifile]->Write();
    hProtMisidAsMuon_VsTrMom[ifile]->Write();
    hPionMisidAsProt_VsTrMom[ifile]->Write();
    hProtMisidAsPion_VsTrMom[ifile]->Write();
    hElecMisidAsProt_VsTrMom[ifile]->Write();
    hProtMisidAsElec_VsTrMom[ifile]->Write();

    hMuonMisidAsProt_VsTrLen[ifile]->Write();
    hProtMisidAsMuon_VsTrLen[ifile]->Write();
    hPionMisidAsProt_VsTrLen[ifile]->Write();
    hProtMisidAsPion_VsTrLen[ifile]->Write();
    hElecMisidAsProt_VsTrLen[ifile]->Write();
    hProtMisidAsElec_VsTrLen[ifile]->Write();
    
    hBinsMom[ifile]->Write();
    hBinsLen[ifile]->Write();
  }
    

  return;
}



double GetPvalue_Histo_rightside(TH1F *histo,double data){

  double pvalue = 0.;

  if(histo->GetEntries() == 0){
    pvalue = -999;
    return pvalue;
  }

  int bin_data = histo->FindBin(data);
  pvalue = histo->Integral(bin_data,histo->GetNbinsX());
  pvalue /= histo->Integral();

  return pvalue;
}

double GetPvalue_Histo_leftside(TH1F *histo,double data){

  double pvalue = 0.;

  if(histo->GetEntries() == 0){
    pvalue = -999;
    return pvalue;
  }

  int bin_data = histo->FindBin(data);
  pvalue = histo->Integral(1,bin_data);
  pvalue /= histo->Integral();
  
  return pvalue;
}

void GetPvalue_bins_mom(TH1F *hHypLeft[NFiles][NBinsMom],
			double cutLeft[NFiles],
			TH1F *hHypRight[NFiles][NBinsMom],
			double cutRigth[NFiles],
			TH1F *hBins[NFiles],
			TH1F *hMisidLeft[NFiles],
			TH1F *hMisidRight[NFiles]
			){
  
  double pvalue_muon_tot = 0.;
  double pvalue_prot_tot = 0.;
  double pvalue_muon = 0.;
  double pvalue_prot = 0.;
    
  for(int ifile=0;ifile<NFiles;ifile++){
    
    for(int imom=0;imom<NBinsMom;imom++){
      
      cout << hHypLeft[ifile][imom]->Integral() << endl;

      /*
      pvalue_muon = GetPvalue_Histo_rightside( hHypLeft[imom], cutLeft);
      pvalue_prot = GetPvalue_Histo_leftside(  hHypRight[imom],cutRight);
      
      // Un-normalize the p-value in each single momentum bin and afterward normalize to 1 the sum of the p-values  
      pvalue_muon *= hHypLeft[imom]->Integral(); 
      pvalue_prot *= hHypRight[imom]->Integral();
      
      pvalue_muon_tot += pvalue_muon;
      pvalue_prot_tot += pvalue_prot;
      
      int histobin = imom+1;
      double lowmom = hBins->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBins->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format(" - Momentum: %1.0f - %1.0f",lowmom,upmom);
      cout << name << endl;
      
      cout << "# of entries (Muon) = "   << hHypLeft[imom]->Integral() << endl;
      cout << "# of entries (Proton) = " << hHypRight[imom]->Integral() << endl;
      
      cout << "Prob. Misid (Muon as Proton) = " << pvalue_muon << endl;
      cout << "Prob. Misid (Proton as Muon) = " << pvalue_prot << endl;
      cout << endl;
      
      hMisidLeft->SetBinContent(histobin,pvalue_muon);
      hMisidRight->SetBinContent(histobin,pvalue_prot);
    }    
    
    //pvalue_muon_tot /= hHypLeft->Integral();
    //pvalue_prot_tot /= hHypRight->Integral();
    
    cout << "Tot. Prob. Misid (Muon as Proton) = " << pvalue_muon_tot << endl;
    cout << "Tot. Prob. Misid (Proton as Muon) = " << pvalue_prot_tot << endl;
    */

      return;
    }
  }
}
