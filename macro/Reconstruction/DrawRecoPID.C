
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

double GetPvalue_Histo_rightside(TH1F *histo,double data);
double GetPvalue_Histo_leftside(TH1F *histo,double data);

void DrawRecoPID
( 
 bool doPrint = true,
 bool doStore = false,
 TString tag = "prova",
 bool doDrawLegend = true
  )
{
 
  const int NFiles = 2;
  string infilename[NFiles] = {

    // Real MPPC efficiency (sampling)

    "RECONSTRUCTION/26_9_17/RealMPPCeff/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_100k.root",
    "RECONSTRUCTION/26_9_17/RealMPPCeff/FGDlike-UseXY-UseXZ-UseYZ-TruthFV-Separate10_CutLayers30mm_100k.root"

    // Straight MPPC efficiency

    //"RECONSTRUCTION/26_9_17/StraightMPPCeff/FGDlike-UseXY-UseXZ-UseYZ-TruthFV-Separate10_100k.root"
    //"RECONSTRUCTION/26_9_17/StraightMPPCeff/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_100k.root"
    
  };
  
  double cut_MuonVsProt[NFiles] = {
    32.  // SuperFGD
    //8. // FGD XZ
    ,8. // FGD XZ
  };

  double cut_MuonVsPion[NFiles] = {
    0. // SuperFGD
    ,0. // FGD XZ
  };

  double cut_PionVsProt[NFiles] = {
    36. // SuperFGD
    //9. // FGD XZ
    ,9. // FGD XZ
  };
 
  TString label[NFiles] = {
    "SuperFGD"    
    ,"FGDXZ"
  };

  
  // gROOT->SetStyle("T2K");                  
  // gROOT->ForceStyle();                 
  // gStyle->SetPadLeftMargin(0.14);
  // gStyle->SetPadRightMargin(0.15);
  // //gStyle->SetStatX(.4); gStyle->SetStatY(.6);

  TFile *infile[NFiles];

  TH1F *hLikeRatio_MuProt_TrueMu[NFiles];
  TH1F *hLikeRatio_MuProt_TrueProt[NFiles];   
  TH1F *hLikeRatio_MuPion_TrueMu[NFiles];
  TH1F *hLikeRatio_MuPion_TruePion[NFiles];   
  TH1F *hLikeRatio_PionProt_TruePion[NFiles];
  TH1F *hLikeRatio_PionProt_TrueProt[NFiles];
   
  TH1F *hMuon_Stopped_EdepOverLen[NFiles];
  TH1F *hPion_Stopped_EdepOverLen[NFiles];
  TH1F *hProt_Stopped_EdepOverLen[NFiles];

  TH1F *hMuon_Stopped_EdepOverLen_ManyBins[NFiles];
  TH1F *hPion_Stopped_EdepOverLen_ManyBins[NFiles];
  TH1F *hProt_Stopped_EdepOverLen_ManyBins[NFiles];
  
  TGraph *gMuon_Stopped_EdepVsLen[NFiles];
  TGraph *gPion_Stopped_EdepVsLen[NFiles];
  TGraph *gProt_Stopped_EdepVsLen[NFiles];

  TGraph *gMuon_Stopped_EdepVsLen_Truth[NFiles];
  TGraph *gPion_Stopped_EdepVsLen_Truth[NFiles];
  TGraph *gProt_Stopped_EdepVsLen_Truth[NFiles];

  TH1F *hBinsMom[NFiles];
  const int NBinsMom = 5; // to check with # of bins in hBinsMom

  TH1F *hMuonMisidAsProt[NFiles]; 
  TH1F *hProtMisidAsMuon[NFiles]; 
  TH1F *hPionMisidAsProt[NFiles]; 
  TH1F *hProtMisidAsPion[NFiles]; 
  TH1F *hMuonMisidAsProt_VsTrMom[NFiles]; // same binning as hBinsMom
  TH1F *hProtMisidAsMuon_VsTrMom[NFiles]; // same binning as hBinsMom
  TH1F *hPionMisidAsProt_VsTrMom[NFiles]; // same binning as hBinsMom
  TH1F *hProtMisidAsPion_VsTrMom[NFiles]; // same binning as hBinsMom
 
  TGraph *gMuon_Stopped_EdepVsLen_Mom[NFiles][NBinsMom];
  TGraph *gPion_Stopped_EdepVsLen_Mom[NFiles][NBinsMom];
  TGraph *gProt_Stopped_EdepVsLen_Mom[NFiles][NBinsMom];

  TH1F *hMuon_Stopped_EdepOverLen_Mom[NFiles][NBinsMom];
  TH1F *hPion_Stopped_EdepOverLen_Mom[NFiles][NBinsMom];
  TH1F *hProt_Stopped_EdepOverLen_Mom[NFiles][NBinsMom];

  TH1F *hMuon_Stopped_EdepOverLen_Mom_ManyBins[NFiles][NBinsMom];
  TH1F *hPion_Stopped_EdepOverLen_Mom_ManyBins[NFiles][NBinsMom];
  TH1F *hProt_Stopped_EdepOverLen_Mom_ManyBins[NFiles][NBinsMom];

  TLine *lCut_MuonVsProt[NFiles];
  TLine *lCut_MuonVsPion[NFiles];
  TLine *lCut_PionVsProt[NFiles];
  
  for(int ifile=0;ifile<NFiles;ifile++){

    // Read from files

    TString name;
    
    cout << "Read file: " << infilename[ifile].c_str() << endl;
    
    infile[ifile] = new TFile(infilename[ifile].c_str(),"READ");

    //infile[ifile]->cd();
    
    hLikeRatio_MuProt_TrueMu[ifile]     = (TH1F*)infile[ifile]->Get("hLikeRatio_MuProt_TrueMu");    
    hLikeRatio_MuProt_TrueProt[ifile]   = (TH1F*)infile[ifile]->Get("hLikeRatio_MuProt_TrueProt");    
    hLikeRatio_MuPion_TrueMu[ifile]     = (TH1F*)infile[ifile]->Get("hLikeRatio_MuPion_TrueMu");    
    hLikeRatio_MuPion_TruePion[ifile]   = (TH1F*)infile[ifile]->Get("hLikeRatio_MuPion_TruePion");    
    hLikeRatio_PionProt_TruePion[ifile] = (TH1F*)infile[ifile]->Get("hLikeRatio_PionProt_TruePion");    
    hLikeRatio_PionProt_TrueProt[ifile] = (TH1F*)infile[ifile]->Get("hLikeRatio_PionProt_TrueProt");    
    
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

    // PID histograms

    name = TString::Format("hMuonMisidAsProt_%s",label[ifile].Data());
    hMuonMisidAsProt[ifile] = new TH1F(name,name,1,0,1);

    name = TString::Format("hProtMisidAsMuon_%s",label[ifile].Data());
    hProtMisidAsMuon[ifile] = new TH1F(name,name,1,0,1);

    name = TString::Format("hPionMisidAsProt_%s",label[ifile].Data());
    hPionMisidAsProt[ifile] = new TH1F(name,name,1,0,1);

    name = TString::Format("hProtMisidAsPion_%s",label[ifile].Data());
    hProtMisidAsPion[ifile] = new TH1F(name,name,1,0,1);
    
    name = TString::Format("hMuonMisidAsProt_VsTrMom_%s",label[ifile].Data());
    hMuonMisidAsProt_VsTrMom[ifile] = (TH1F*)hBinsMom[ifile]->Clone(name.Data());
    hMuonMisidAsProt_VsTrMom[ifile]->Reset();

    name = TString::Format("hProtMisidAsMuon_VsTrMom_%s",label[ifile].Data());
    hProtMisidAsMuon_VsTrMom[ifile] = (TH1F*)hBinsMom[ifile]->Clone(name.Data());
    hProtMisidAsMuon_VsTrMom[ifile]->Reset();

    name = TString::Format("hPionMisidAsProt_VsTrMom_%s",label[ifile].Data());
    hPionMisidAsProt_VsTrMom[ifile] = (TH1F*)hBinsMom[ifile]->Clone(name.Data()); 
    hPionMisidAsProt_VsTrMom[ifile]->Reset();

    name = TString::Format("hProtMisidAsPion_VsTrMom_%s",label[ifile].Data());
    hProtMisidAsPion_VsTrMom[ifile] = (TH1F*)hBinsMom[ifile]->Clone(name.Data()); 
    hProtMisidAsPion_VsTrMom[ifile]->Reset();    
  
    //

 

    for(int imom=0;imom<NBinsMom;imom++){
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);

      name = TString::Format("gMuon_Stopped_EdepVsLen_Mom_%1.0f_%1.0f",lowmom,upmom);
      gMuon_Stopped_EdepVsLen_Mom[ifile][imom] = (TGraph*)infile[ifile]->Get(name);	

      name = TString::Format("gPion_Stopped_EdepVsLen_Mom_%1.0f_%1.0f",lowmom,upmom);
      gPion_Stopped_EdepVsLen_Mom[ifile][imom] = (TGraph*)infile[ifile]->Get(name);	

      name = TString::Format("gProt_Stopped_EdepVsLen_Mom_%1.0f_%1.0f",lowmom,upmom);
      gProt_Stopped_EdepVsLen_Mom[ifile][imom] = (TGraph*)infile[ifile]->Get(name);	

      name = TString::Format("hMuon_Stopped_EdepOverLen_Mom_%1.0f_%1.0f",lowmom,upmom);
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom] = (TH1F*)infile[ifile]->Get(name);

      name = TString::Format("hPion_Stopped_EdepOverLen_Mom_%1.0f_%1.0f",lowmom,upmom);
      hPion_Stopped_EdepOverLen_Mom[ifile][imom] = (TH1F*)infile[ifile]->Get(name);

      name = TString::Format("hProt_Stopped_EdepOverLen_Mom_%1.0f_%1.0f",lowmom,upmom);
      hProt_Stopped_EdepOverLen_Mom[ifile][imom] = (TH1F*)infile[ifile]->Get(name);

      name = TString::Format("hMuon_Stopped_EdepOverLen_ManyBins_Mom_%1.0f_%1.0f",lowmom,upmom);
      hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom] = (TH1F*)infile[ifile]->Get(name);

      name = TString::Format("hPion_Stopped_EdepOverLen_ManyBins_Mom_%1.0f_%1.0f",lowmom,upmom);
      hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom] = (TH1F*)infile[ifile]->Get(name);

      name = TString::Format("hProt_Stopped_EdepOverLen_ManyBins_Mom_%1.0f_%1.0f",lowmom,upmom);
      hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom] = (TH1F*)infile[ifile]->Get(name);
    }
  
  
    // should be already normalized to 1, since it's used as PDF

    hMuon_Stopped_EdepOverLen[ifile]->Scale(1./hMuon_Stopped_EdepOverLen[ifile]->Integral());
    hPion_Stopped_EdepOverLen[ifile]->Scale(1./hPion_Stopped_EdepOverLen[ifile]->Integral());
    hProt_Stopped_EdepOverLen[ifile]->Scale(1./hProt_Stopped_EdepOverLen[ifile]->Integral());


    for(int imom=0;imom<NBinsMom;imom++){
      if(hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->Integral() > 0){
       	hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->Integral());
      }
      if(hPion_Stopped_EdepOverLen_Mom[ifile][imom]->Integral() > 0){
       	hPion_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hPion_Stopped_EdepOverLen_Mom[ifile][imom]->Integral());
      }
      if(hProt_Stopped_EdepOverLen_Mom[ifile][imom]->Integral() > 0){
       	hProt_Stopped_EdepOverLen_Mom[ifile][imom]->Scale(1./hProt_Stopped_EdepOverLen_Mom[ifile][imom]->Integral());
      }      
      if(hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() > 0){
	hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral());
      }
      if(hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() > 0){
	hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hPion_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral());
      }
      if(hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral() > 0){
	hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Scale(1./hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom]->Integral());
      }
    }

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
    
    // Set the cut lines

    lCut_MuonVsProt[ifile] = new TLine(cut_MuonVsProt[ifile],0.,cut_MuonVsProt[ifile],hMuon_Stopped_EdepOverLen[ifile]->GetMaximum());
    lCut_MuonVsProt[ifile]->SetLineWidth(2.);
    lCut_MuonVsPion[ifile] = new TLine(cut_MuonVsPion[ifile],0.,cut_MuonVsPion[ifile],hMuon_Stopped_EdepOverLen[ifile]->GetMaximum()); 
    lCut_MuonVsPion[ifile]->SetLineWidth(2.);
    lCut_PionVsProt[ifile] = new TLine(cut_PionVsProt[ifile],0.,cut_PionVsProt[ifile],hPion_Stopped_EdepOverLen[ifile]->GetMaximum());
    lCut_PionVsProt[ifile]->SetLineWidth(2.);

    


  }



 



  //
  // Print results
  //
  
  for(int ifile=0;ifile<NFiles;ifile++){

    double Mean = 0.;
    double RMS = 0.;

    cout << endl;
    cout << label[ifile] << ": " << endl;

    // Muon
    Mean = hMuon_Stopped_EdepOverLen[ifile]->GetMean();
    RMS  = hMuon_Stopped_EdepOverLen[ifile]->GetRMS();    
    cout << "  Muons: " << endl;
    cout << "   - Mean = " << Mean << endl;
    cout << "   - RMS  = " << RMS  << endl;
    cout << "   - RMS/Mean = " << RMS / Mean << endl;    
    cout << endl;
 
    // Pion
    Mean = hPion_Stopped_EdepOverLen[ifile]->GetMean();
    RMS  = hPion_Stopped_EdepOverLen[ifile]->GetRMS();    
    cout << "  Pions: " << endl;
    cout << "   - Mean = " << Mean << endl;
    cout << "   - RMS  = " << RMS  << endl;
    cout << "   - RMS/Mean = " << RMS / Mean << endl;    
    cout << endl;
 
    // Proton
    Mean = hProt_Stopped_EdepOverLen[ifile]->GetMean();
    RMS  = hProt_Stopped_EdepOverLen[ifile]->GetRMS();    
    cout << "  Protons: " << endl;
    cout << "   - Mean = " << Mean << endl;
    cout << "   - RMS  = " << RMS  << endl;
    cout << "   - RMS/Mean = " << RMS / Mean << endl;    
    cout << endl;
  }
  
  //
  // Print contamination --> PID probability
  // 

  // Muon Vs Proton
  cout << endl;
  cout << "/// PID Muon Vs Proton ///" << endl;

  for(int ifile=0;ifile<NFiles;ifile++){
    double pvalue_muon = GetPvalue_Histo_rightside( hMuon_Stopped_EdepOverLen_ManyBins[ifile],cut_MuonVsProt[ifile]);
    double pvalue_prot = GetPvalue_Histo_leftside(hProt_Stopped_EdepOverLen_ManyBins[ifile],cut_MuonVsProt[ifile]);
    cout << endl;
    cout << label[ifile] << ": " << endl;
    cout << "Prob. Misid (Muon as Proton) = " << pvalue_muon << endl;
    cout << "Prob. Misid (Proton as Muon) = " << pvalue_prot << endl;

    hMuonMisidAsProt[ifile]->SetBinContent(1,pvalue_muon);
    hProtMisidAsMuon[ifile]->SetBinContent(1,pvalue_prot);
  
    // momenta bins
    for(int imom=0;imom<NBinsMom;imom++){
      pvalue_muon = GetPvalue_Histo_rightside( hMuon_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom],cut_MuonVsProt[ifile]);
      pvalue_prot = GetPvalue_Histo_leftside(hProt_Stopped_EdepOverLen_Mom_ManyBins[ifile][imom],cut_MuonVsProt[ifile]);
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format(" - Momentum: %1.0f - %1.0f",lowmom,upmom);
      cout << name << endl;
      cout << "Prob. Misid (Muon as Proton) = " << pvalue_muon << endl;
      cout << "Prob. Misid (Proton as Muon) = " << pvalue_prot << endl;

      hMuonMisidAsProt_VsTrMom[ifile]->SetBinContent(histobin,pvalue_muon);
      hProtMisidAsMuon_VsTrMom[ifile]->SetBinContent(histobin,pvalue_prot);
    }    
  }



  // Pion Vs Proton
  cout << endl;
  cout << "/// PID Pion Vs Proton ///" << endl;
  for(int ifile=0;ifile<NFiles;ifile++){
    double pvalue_pion = GetPvalue_Histo_rightside( hPion_Stopped_EdepOverLen_ManyBins[ifile],cut_PionVsProt[ifile]);
    double pvalue_prot = GetPvalue_Histo_leftside(hProt_Stopped_EdepOverLen_ManyBins[ifile],cut_PionVsProt[ifile]);
    cout << endl;
    cout << label[ifile] << ": " << endl;
    cout << "Prob. Misid (Pion as Proton) = " << pvalue_pion << endl;
    cout << "Prob. Misid (Proton as Pion) = " << pvalue_prot << endl;

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
  }


  //
  // Draw 
  //
  
  TLegend *leg = new TLegend(0.4,0.15,0.6,0.4);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->SetTextFont(132);
  leg->SetTextSize(0.05);
  for(int ifile=0;ifile<NFiles;ifile++){
    leg->AddEntry(hMuon_Stopped_EdepOverLen[ifile],label[ifile],"l");
  }
  
  TLegend *leg_particle = new TLegend(0.4,0.15,0.6,0.4);
  leg_particle->SetFillColor(0);
  leg_particle->SetLineColor(0);
  leg_particle->SetTextFont(132);
  leg_particle->SetTextSize(0.05);
  leg_particle->AddEntry(hMuon_Stopped_EdepOverLen[0],"True Muon","l");
  leg_particle->AddEntry(hProt_Stopped_EdepOverLen[0],"True Proton","l");
  leg_particle->AddEntry(hPion_Stopped_EdepOverLen[0],"True Pion","l");

  TLegend *leg_MuonVsProt = new TLegend(0.4,0.15,0.6,0.4);
  leg_MuonVsProt->SetFillColor(0);
  leg_MuonVsProt->SetLineColor(0);
  leg_MuonVsProt->SetTextFont(132);
  leg_MuonVsProt->SetTextSize(0.05);
  leg_MuonVsProt->AddEntry(hMuon_Stopped_EdepOverLen[0],"True Muon","l");
  leg_MuonVsProt->AddEntry(hProt_Stopped_EdepOverLen[0],"True Proton","l");

  TLegend *leg_MuonVsPion = new TLegend(0.4,0.15,0.6,0.4);
  leg_MuonVsPion->SetFillColor(0);
  leg_MuonVsPion->SetLineColor(0);
  leg_MuonVsPion->SetTextFont(132);
  leg_MuonVsPion->SetTextSize(0.05);
  leg_MuonVsPion->AddEntry(hMuon_Stopped_EdepOverLen[0],"True Muon","l");
  leg_MuonVsPion->AddEntry(hPion_Stopped_EdepOverLen[0],"True Pion","l");
  
  TLegend *leg_PionVsProt = new TLegend(0.4,0.15,0.6,0.4);
  leg_PionVsProt->SetFillColor(0);
  leg_PionVsProt->SetLineColor(0);
  leg_PionVsProt->SetTextFont(132);
  leg_PionVsProt->SetTextSize(0.05);
  leg_PionVsProt->AddEntry(hPion_Stopped_EdepOverLen[0],"True Pion","l");
  leg_PionVsProt->AddEntry(hProt_Stopped_EdepOverLen[0],"True Proton","l");

  
  //
  // Edep - Length ratio
  //

  // Muon Vs Proton

  TCanvas *cMuonVsProt_Stopped_EdepOverLen[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cMuonVsProt_Stopped_EdepOverLen_%s",label[ifile].Data());
    cMuonVsProt_Stopped_EdepOverLen[ifile] = new TCanvas(name,name);
    hMuon_Stopped_EdepOverLen[ifile]->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
    hMuon_Stopped_EdepOverLen[ifile]->GetYaxis()->SetTitle("# of events");
    hMuon_Stopped_EdepOverLen[ifile]->GetYaxis()->SetTitleSize(0.05);
    hMuon_Stopped_EdepOverLen[ifile]->GetXaxis()->SetTitleSize(0.05);
    hMuon_Stopped_EdepOverLen[ifile]->SetStats(0);
    hMuon_Stopped_EdepOverLen[ifile]->SetLineColor(kBlue);
    hMuon_Stopped_EdepOverLen[ifile]->SetLineWidth(2.);
    hProt_Stopped_EdepOverLen[ifile]->SetStats(0);
    hProt_Stopped_EdepOverLen[ifile]->SetLineColor(kRed);
    hProt_Stopped_EdepOverLen[ifile]->SetLineWidth(2.);
    if(ifile==0){
      hMuon_Stopped_EdepOverLen[ifile]->DrawClone("");
      hProt_Stopped_EdepOverLen[ifile]->DrawClone("same");
    }
    else{
      hMuon_Stopped_EdepOverLen[ifile]->DrawClone("same");
      hProt_Stopped_EdepOverLen[ifile]->DrawClone("same");
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
      
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->GetYaxis()->SetTitle("# of events");
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->GetYaxis()->SetTitleSize(0.05);
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->GetXaxis()->SetTitleSize(0.05);
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->SetStats(0);
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->SetLineColor(kBlue);
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->SetLineWidth(2.);
      hProt_Stopped_EdepOverLen_Mom[ifile][imom]->SetStats(0);
      hProt_Stopped_EdepOverLen_Mom[ifile][imom]->SetLineColor(kRed);
      hProt_Stopped_EdepOverLen_Mom[ifile][imom]->SetLineWidth(2.);
      if(ifile==0){
	hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->DrawClone("");
	hProt_Stopped_EdepOverLen_Mom[ifile][imom]->DrawClone("same");
      }
      else{
	hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->DrawClone("same");
	hProt_Stopped_EdepOverLen_Mom[ifile][imom]->DrawClone("same");
      }
      lCut_MuonVsProt[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }


  // Muon Vs Pion

  TCanvas *cMuonVsPion_Stopped_EdepOverLen[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cMuonVsPion_Stopped_EdepOverLen_%s",label[ifile].Data());
    cMuonVsPion_Stopped_EdepOverLen[ifile] = new TCanvas(name,name);
    hMuon_Stopped_EdepOverLen[ifile]->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
    hMuon_Stopped_EdepOverLen[ifile]->GetYaxis()->SetTitle("# of events");
    hMuon_Stopped_EdepOverLen[ifile]->GetYaxis()->SetTitleSize(0.05);
    hMuon_Stopped_EdepOverLen[ifile]->GetXaxis()->SetTitleSize(0.05);
    hMuon_Stopped_EdepOverLen[ifile]->SetStats(0);
    hMuon_Stopped_EdepOverLen[ifile]->SetLineColor(kBlue);
    hMuon_Stopped_EdepOverLen[ifile]->SetLineWidth(2.);
    hPion_Stopped_EdepOverLen[ifile]->SetStats(0);
    hPion_Stopped_EdepOverLen[ifile]->SetLineColor(kGreen);
    hPion_Stopped_EdepOverLen[ifile]->SetLineWidth(2.);
    if(ifile==0){
      hMuon_Stopped_EdepOverLen[ifile]->DrawClone("");
      hPion_Stopped_EdepOverLen[ifile]->DrawClone("same");
    }
    else{
      hMuon_Stopped_EdepOverLen[ifile]->DrawClone("same");
      hPion_Stopped_EdepOverLen[ifile]->DrawClone("same");
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
      
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->GetYaxis()->SetTitle("# of events");
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->GetYaxis()->SetTitleSize(0.05);
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->GetXaxis()->SetTitleSize(0.05);
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->SetStats(0);
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->SetLineColor(kBlue);
      hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->SetLineWidth(2.);
      hPion_Stopped_EdepOverLen_Mom[ifile][imom]->SetStats(0);
      hPion_Stopped_EdepOverLen_Mom[ifile][imom]->SetLineColor(kGreen);
      hPion_Stopped_EdepOverLen_Mom[ifile][imom]->SetLineWidth(2.);
      if(ifile==0){
	hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->DrawClone("");
	hPion_Stopped_EdepOverLen_Mom[ifile][imom]->DrawClone("same");
      }
      else{
	hMuon_Stopped_EdepOverLen_Mom[ifile][imom]->DrawClone("same");
	hPion_Stopped_EdepOverLen_Mom[ifile][imom]->DrawClone("same");
      }
      lCut_MuonVsPion[ifile]->Draw();
    }
    if(doDrawLegend) leg_particle->Draw();    
  }

  // Pion Vs Proton

  TCanvas *cPionVsProt_Stopped_EdepOverLen[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cPionVsProt_Stopped_EdepOverLen_%s",label[ifile].Data());
    cPionVsProt_Stopped_EdepOverLen[ifile] = new TCanvas(name,name);
    hPion_Stopped_EdepOverLen[ifile]->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
    hPion_Stopped_EdepOverLen[ifile]->GetYaxis()->SetTitle("# of events");
    hPion_Stopped_EdepOverLen[ifile]->GetYaxis()->SetTitleSize(0.05);
    hPion_Stopped_EdepOverLen[ifile]->GetXaxis()->SetTitleSize(0.05);
    hPion_Stopped_EdepOverLen[ifile]->SetStats(0);
    hPion_Stopped_EdepOverLen[ifile]->SetLineColor(kGreen);
    hPion_Stopped_EdepOverLen[ifile]->SetLineWidth(2.);
    hProt_Stopped_EdepOverLen[ifile]->SetStats(0);
    hProt_Stopped_EdepOverLen[ifile]->SetLineColor(kRed);
    hProt_Stopped_EdepOverLen[ifile]->SetLineWidth(2.);
    if(ifile==0){
      hPion_Stopped_EdepOverLen[ifile]->DrawClone("");
      hProt_Stopped_EdepOverLen[ifile]->DrawClone("same");
    }
    else{
      hPion_Stopped_EdepOverLen[ifile]->DrawClone("same");
      hProt_Stopped_EdepOverLen[ifile]->DrawClone("same");
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
      
      hPion_Stopped_EdepOverLen_Mom[ifile][imom]->GetXaxis()->SetTitle("dEdx (p.e. / mm)");
      hPion_Stopped_EdepOverLen_Mom[ifile][imom]->GetYaxis()->SetTitle("# of events");
      hPion_Stopped_EdepOverLen_Mom[ifile][imom]->GetYaxis()->SetTitleSize(0.05);
      hPion_Stopped_EdepOverLen_Mom[ifile][imom]->GetXaxis()->SetTitleSize(0.05);
      hPion_Stopped_EdepOverLen_Mom[ifile][imom]->SetStats(0);
      hPion_Stopped_EdepOverLen_Mom[ifile][imom]->SetLineColor(kGreen);
      hPion_Stopped_EdepOverLen_Mom[ifile][imom]->SetLineWidth(2.);
      hProt_Stopped_EdepOverLen_Mom[ifile][imom]->SetStats(0);
      hProt_Stopped_EdepOverLen_Mom[ifile][imom]->SetLineColor(kRed);
      hProt_Stopped_EdepOverLen_Mom[ifile][imom]->SetLineWidth(2.);
      if(ifile==0){
	hProt_Stopped_EdepOverLen_Mom[ifile][imom]->DrawClone("");
	hPion_Stopped_EdepOverLen_Mom[ifile][imom]->DrawClone("same");
      }
      else{
	hProt_Stopped_EdepOverLen_Mom[ifile][imom]->DrawClone("same");
	hPion_Stopped_EdepOverLen_Mom[ifile][imom]->DrawClone("same");
      }
      lCut_PionVsProt[ifile]->Draw();
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
    gMuon_Stopped_EdepVsLen[ifile]->SetMarkerSize(1.);
    gPion_Stopped_EdepVsLen[ifile]->SetMarkerSize(1.);
    gProt_Stopped_EdepVsLen[ifile]->SetMarkerSize(1.);
    gMuon_Stopped_EdepVsLen[ifile]->SetMarkerStyle(2.);
    gPion_Stopped_EdepVsLen[ifile]->SetMarkerStyle(2.);
    gProt_Stopped_EdepVsLen[ifile]->SetMarkerStyle(2.);

    gProt_Stopped_EdepVsLen[ifile]->Draw("p same");
    gPion_Stopped_EdepVsLen[ifile]->Draw("p same");
    gMuon_Stopped_EdepVsLen[ifile]->Draw("p same");
    if(doDrawLegend) leg_particle->Draw();
  }



  // Truth

  TH2F *hCanvas_Truth = new TH2F("hCanvas_Truth","hCanvas_Truths",1000,0,2000,1000,0,500);

  TCanvas *cParticle_Stopped_EdepVsLen_Truth[NFiles];
  for(int ifile=0;ifile<NFiles;ifile++){
    TString name = TString::Format("cParticle_Stopped_EdepVsLen_Truth_%s",label[ifile].Data());
    cParticle_Stopped_EdepVsLen_Truth[ifile] = new TCanvas(name,name);

    name = TString::Format("All particles (Truth)");
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
    gMuon_Stopped_EdepVsLen_Truth[ifile]->SetMarkerSize(1.);
    gPion_Stopped_EdepVsLen_Truth[ifile]->SetMarkerSize(1.);
    gProt_Stopped_EdepVsLen_Truth[ifile]->SetMarkerSize(1.);
    gMuon_Stopped_EdepVsLen_Truth[ifile]->SetMarkerStyle(2.);
    gPion_Stopped_EdepVsLen_Truth[ifile]->SetMarkerStyle(2.);
    gProt_Stopped_EdepVsLen_Truth[ifile]->SetMarkerStyle(2.);

    gProt_Stopped_EdepVsLen_Truth[ifile]->Draw("p same");
    gPion_Stopped_EdepVsLen_Truth[ifile]->Draw("p same");
    gMuon_Stopped_EdepVsLen_Truth[ifile]->Draw("p same");
    if(doDrawLegend) leg_particle->Draw();
  }

 
  //
  // Edep Vs Length per Momentum bin
  //

  // Reco 

  TCanvas *cParticle_Stopped_EdepVsLen_Mom[NFiles];

  for(int ifile=0;ifile<NFiles;ifile++){
    
    TString name = TString::Format("cParticle_Stopped_EdepVsLen_Mom_%s",label[ifile].Data());
    cParticle_Stopped_EdepVsLen_Mom[ifile] = new TCanvas(name,name);
    cParticle_Stopped_EdepVsLen_Mom[ifile]->Divide(3,2);
      
    for(int imom=0;imom<NBinsMom;imom++){
      cParticle_Stopped_EdepVsLen_Mom[ifile]->cd(imom+1);
      
      int histobin = imom+1;
      double lowmom = hBinsMom[ifile]->GetXaxis()->GetBinLowEdge(histobin);
      double upmom  = hBinsMom[ifile]->GetXaxis()->GetBinUpEdge(histobin);
      TString name = TString::Format("Momentum: %1.0f - %1.0f",lowmom,upmom);
      hCanvas->SetTitle(name);
      hCanvas->GetXaxis()->SetTitle("Length (mm)");
      hCanvas->GetYaxis()->SetTitle("Edep (p.e.)"); 
      hCanvas->GetYaxis()->SetTitleSize(0.05);
      hCanvas->GetXaxis()->SetTitleSize(0.05);
      hCanvas->DrawClone();
     
      gMuon_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerColor(kBlue);
      gPion_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerColor(kGreen);
      gProt_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerColor(kRed);

      gMuon_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerSize(1.);
      gPion_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerSize(1.);
      gProt_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerSize(1.);
      
      gMuon_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerStyle(2.);
      gPion_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerStyle(2.);
      gProt_Stopped_EdepVsLen_Mom[ifile][imom]->SetMarkerStyle(2.);
      
      if(gProt_Stopped_EdepVsLen_Mom[ifile][imom]->GetN() > 0.) gProt_Stopped_EdepVsLen_Mom[ifile][imom]->Draw("p same");      
      if(gPion_Stopped_EdepVsLen_Mom[ifile][imom]->GetN() > 0.) gPion_Stopped_EdepVsLen_Mom[ifile][imom]->Draw("p same");
      if(gMuon_Stopped_EdepVsLen_Mom[ifile][imom]->GetN() > 0.) gMuon_Stopped_EdepVsLen_Mom[ifile][imom]->Draw("p same");
    }
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

    hMuonMisidAsProt_VsTrMom[ifile]->Write();
    hProtMisidAsMuon_VsTrMom[ifile]->Write();
    hPionMisidAsProt_VsTrMom[ifile]->Write();
    hProtMisidAsPion_VsTrMom[ifile]->Write();
    
    hBinsMom[ifile]->Write();
  }
    

  return;
}



double GetPvalue_Histo_rightside(TH1F *histo,double data){

  if(histo->GetEntries() == 0) return -999;

  int bin_data = histo->FindBin(data);
  double pvalue = histo->Integral(bin_data,histo->GetNbinsX());
  pvalue /= histo->Integral();
  
  return pvalue;
}

double GetPvalue_Histo_leftside(TH1F *histo,double data){

  if(histo->GetEntries() == 0) return -999;
  
  int bin_data = histo->FindBin(data);
  double pvalue = histo->Integral(1,bin_data);
  pvalue /= histo->Integral();
  
  return pvalue;
}
