
void DrawRecoVar
( 
 int myevt = -999, // select event display to show. If <0 the Truth Vtx distribution is not shown

 bool doNormalize = true,

 //bool doPrint = true,
 //bool doStore = false,
 string tag = "prova",
 bool drawMuon = true,
 bool drawPion = false,
 bool drawProt = false,
 bool drawElec = false
  )
{
 
  const int NFiles = 3;
  string infilename[NFiles] = {

    //"SuperFGD-NuMu-UseXY-UseYZ-TruthFV-Separate10_Evt0_NEvt10.root",
    
    "RECONSTRUCTION/16_11_17/USETRACKLENGTH/SuperFGD_numu.root",
    "RECONSTRUCTION/16_11_17/USETRACKLENGTH/SuperFGD_XZ_numu.root",
    
    //"RECONSTRUCTION/16_11_17/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E22POT_NuE.root",
    //"RECONSTRUCTION/16_11_17/FGDXZ-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E22POT_NuE.root"

    //"RECONSTRUCTION/16_11_17/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT_NuMu.root"
    "RECONSTRUCTION/16_11_17/FGDXZ-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT_NuMu.root"
    
    ///"RECONSTRUCTION/5_10_17/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT_NuMu.root"
    //"RECONSTRUCTION/5_10_17/FGDlike-UseXY-UseXZ-UseYZ-TruthFV-Separate10_1E21POT_NuMu.root"
  };

  int color[NFiles] = {
    kBlue
    ,kRed
    ,kGreen
    //kOrange
    //kCyan
  };

  TString label[NFiles] = {
    "SuperFGD"
    ,"SuperFGD-XZ"
    ,"FGDlike"

    //"OK"
    //xs,"Uniform"

    //"Normal",
    //"True OOFV"
  };

  
  gROOT->SetStyle("T2K");                  
  gROOT->ForceStyle();                 
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadRightMargin(0.15);
  //gStyle->SetStatX(.4); gStyle->SetStatY(.6);


  TString name;
  TFile *infile[NFiles];

  TCanvas *cVtxOut_XY[NFiles];
  TCanvas *cVtxOut_XZ[NFiles];
  TCanvas *cVtxOut_YZ[NFiles];
  TCanvas *cVtx_XY[NFiles];
  TCanvas *cVtx_XZ[NFiles];
  TCanvas *cVtx_YZ[NFiles];
  
  TH2F *hVtxOut_XY[NFiles];
  TH2F *hVtxOut_XZ[NFiles];
  TH2F *hVtxOut_YZ[NFiles];
  TH2F *hVtx_XY[NFiles];
  TH2F *hVtx_XZ[NFiles];
  TH2F *hVtx_YZ[NFiles];

  TH2F *hMPPCHits_XY[NFiles]; // done
  TH2F *hMPPCHits_XZ[NFiles]; // done
  TH2F *hMPPCHits_YZ[NFiles]; // done
  
  TGraph *gMCHits_XY[NFiles]; // done 
  TGraph *gMCHits_XZ[NFiles]; // done
  TGraph *gMCHits_YZ[NFiles]; // done

  TH2F *hMuon_CosTh_TrueVsReco[NFiles]; // done
  TH1F *hMuon_CosTh_RecMinTr[NFiles]; // done
  TH2F *hMuon_Len_TrueVsReco[NFiles]; // done
  TH1F *hMuon_Len_RecMinTr[NFiles]; // done
  TH2F *hMuon_Len_RecMinTr_Vs_TrLen[NFiles];
  TH2F *hPion_CosTh_TrueVsReco[NFiles];
  TH1F *hPion_CosTh_RecMinTr[NFiles];
  TH2F *hPion_Len_TrueVsReco[NFiles];
  TH1F *hPion_Len_RecMinTr[NFiles];
  TH2F *hPion_Len_RecMinTr_Vs_TrLen[NFiles];
  TH2F *hProt_CosTh_TrueVsReco[NFiles];
  TH1F *hProt_CosTh_RecMinTr[NFiles];
  TH2F *hProt_Len_TrueVsReco[NFiles];
  TH1F *hProt_Len_RecMinTr[NFiles];
  TH2F *hProt_Len_RecMinTr_Vs_TrLen[NFiles];
  TH2F *hElec_CosTh_TrueVsReco[NFiles];
  TH1F *hElec_CosTh_RecMinTr[NFiles];
  TH2F *hElec_Len_TrueVsReco[NFiles];
  TH1F *hElec_Len_RecMinTr[NFiles];
  TH2F *hElec_Len_RecMinTr_Vs_TrLen[NFiles];

    
  for(int ifile=0;ifile<NFiles;ifile++){
    
    cout << "Read file: " << infilename[ifile].c_str() << endl;
    
    infile[ifile] = new TFile(infilename[ifile].c_str(),"READ");

    hVtxOut_XY[ifile] = (TH2F*)infile[ifile]->Get("hVtxOut_XY");
    hVtxOut_XZ[ifile] = (TH2F*)infile[ifile]->Get("hVtxOut_XZ");
    hVtxOut_YZ[ifile] = (TH2F*)infile[ifile]->Get("hVtxOut_YZ");
    hVtx_XY[ifile] = (TH2F*)infile[ifile]->Get("hVtx_XY");
    hVtx_XZ[ifile] = (TH2F*)infile[ifile]->Get("hVtx_XZ");
    hVtx_YZ[ifile] = (TH2F*)infile[ifile]->Get("hVtx_YZ");

    cout << "Tot # of vertices inside  the FV = " << hVtx_XY[ifile]->Integral() << endl;
    cout << "Tot # of vertices outside the FV = " << hVtxOut_XY[ifile]->Integral() << endl;

    if(myevt >= 0){
      name = TString::Format("hMPPCHits_XY_%d",myevt);
      hMPPCHits_XY[ifile] = (TH2F*)infile[ifile]->Get(name);
      name = TString::Format("hMPPCHits_XZ_%d",myevt);
      hMPPCHits_XZ[ifile] = (TH2F*)infile[ifile]->Get(name);
      name = TString::Format("hMPPCHits_YZ_%d",myevt);
      hMPPCHits_YZ[ifile] = (TH2F*)infile[ifile]->Get(name);
      
      name = TString::Format("gMCHits_XY_%d",myevt);
      gMCHits_XY[ifile] = (TGraph*)infile[ifile]->Get(name);
      gMCHits_XY[ifile]->SetMarkerSize(1);
      gMCHits_XY[ifile]->SetMarkerStyle(20);
      name = TString::Format("gMCHits_XZ_%d",myevt);
      gMCHits_XZ[ifile] = (TGraph*)infile[ifile]->Get(name);
      gMCHits_XZ[ifile]->SetMarkerSize(1);
      gMCHits_XZ[ifile]->SetMarkerStyle(20);
      name = TString::Format("gMCHits_YZ_%d",myevt);
      gMCHits_YZ[ifile] = (TGraph*)infile[ifile]->Get(name);
      gMCHits_YZ[ifile]->SetMarkerSize(1);
      gMCHits_YZ[ifile]->SetMarkerStyle(20);
    }

    hMuon_CosTh_TrueVsReco[ifile] = (TH2F*)infile[ifile]->Get("hMuon_CosTh_TrueVsReco");    
    hMuon_CosTh_RecMinTr[ifile] = (TH1F*)infile[ifile]->Get("hMuon_CosTh_RecMinTr");    
    hMuon_Len_TrueVsReco[ifile] = (TH2F*)infile[ifile]->Get("hMuon_Len_TrueVsReco");    
    hMuon_Len_RecMinTr[ifile] = (TH1F*)infile[ifile]->Get("hMuon_Len_RecMinTr");    
    hMuon_Len_RecMinTr_Vs_TrLen[ifile] = (TH2F*)infile[ifile]->Get("hMuon_Len_RecMinTr_Vs_TrLen");    
    hPion_CosTh_TrueVsReco[ifile] = (TH2F*)infile[ifile]->Get("hPion_CosTh_TrueVsReco");    
    hPion_CosTh_RecMinTr[ifile] = (TH1F*)infile[ifile]->Get("hPion_CosTh_RecMinTr");    
    hPion_Len_TrueVsReco[ifile] = (TH2F*)infile[ifile]->Get("hPion_Len_TrueVsReco");    
    hPion_Len_RecMinTr[ifile] = (TH1F*)infile[ifile]->Get("hPion_Len_RecMinTr");    
    hPion_Len_RecMinTr_Vs_TrLen[ifile] = (TH2F*)infile[ifile]->Get("hPion_Len_RecMinTr_Vs_TrLen");    
    hProt_CosTh_TrueVsReco[ifile] = (TH2F*)infile[ifile]->Get("hProt_CosTh_TrueVsReco");    
    hProt_CosTh_RecMinTr[ifile] = (TH1F*)infile[ifile]->Get("hProt_CosTh_RecMinTr");    
    hProt_Len_TrueVsReco[ifile] = (TH2F*)infile[ifile]->Get("hProt_Len_TrueVsReco");    
    hProt_Len_RecMinTr[ifile] = (TH1F*)infile[ifile]->Get("hProt_Len_RecMinTr");    
    hProt_Len_RecMinTr_Vs_TrLen[ifile] = (TH2F*)infile[ifile]->Get("hProt_Len_RecMinTr_Vs_TrLen");    
    hElec_CosTh_TrueVsReco[ifile] = (TH2F*)infile[ifile]->Get("hElec_CosTh_TrueVsReco");    
    hElec_CosTh_RecMinTr[ifile] = (TH1F*)infile[ifile]->Get("hElec_CosTh_RecMinTr");    
    hElec_Len_TrueVsReco[ifile] = (TH2F*)infile[ifile]->Get("hElec_Len_TrueVsReco");    
    hElec_Len_RecMinTr[ifile] = (TH1F*)infile[ifile]->Get("hElec_Len_RecMinTr");    
    hElec_Len_RecMinTr_Vs_TrLen[ifile] = (TH2F*)infile[ifile]->Get("hElec_Len_RecMinTr_Vs_TrLen");    
  }  

  //
  // Draw 
  //
  
  // legend for 1d plots
  TLegend *legend = new TLegend(0.4,0.15,0.8,0.4);
  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetTextFont(132);
  legend->SetTextSize(0.05);
  for(int ifile=0;ifile<NFiles;ifile++){
    legend->AddEntry(hMuon_Len_RecMinTr[ifile],label[ifile].Data(),"l");
  }

  // Event display

  if(myevt >=0 ){
    
    TCanvas *cEventDisplay_XY[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      if(hMPPCHits_XY[ifile]){
	name = TString::Format("cEventDisplay_XY_%i",ifile);
	cEventDisplay_XY[ifile] = new TCanvas(name,label[ifile]);
	hMPPCHits_XY[ifile]->GetXaxis()->SetTitle("X (mm)");
	hMPPCHits_XY[ifile]->GetYaxis()->SetTitle("Y (mm)");
	hMPPCHits_XY[ifile]->SetStats(0);
	hMPPCHits_XY[ifile]->DrawClone("colz");
	gMCHits_XY[ifile]->Draw("p");
      }
    }
    
    TCanvas *cEventDisplay_XZ[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      if(hMPPCHits_XZ[ifile]){
	name = TString::Format("cEventDisplay_XZ_%i",ifile);
	cEventDisplay_XZ[ifile] = new TCanvas(name,label[ifile]);
	hMPPCHits_XZ[ifile]->GetXaxis()->SetTitle("X (mm)");
	hMPPCHits_XZ[ifile]->GetYaxis()->SetTitle("Z (mm)");
	hMPPCHits_XZ[ifile]->SetStats(0);
	hMPPCHits_XZ[ifile]->DrawClone("colz");
	gMCHits_XZ[ifile]->Draw("p");
      }
    }
    
    TCanvas *cEventDisplay_YZ[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      if(hMPPCHits_YZ[ifile]){
	name = TString::Format("cEventDisplay_YZ_%i",ifile);
	cEventDisplay_YZ[ifile] = new TCanvas(name,label[ifile]);
	hMPPCHits_YZ[ifile]->GetXaxis()->SetTitle("Y (mm)");
	hMPPCHits_YZ[ifile]->GetYaxis()->SetTitle("Z (mm)");
	hMPPCHits_YZ[ifile]->SetStats(0);
	hMPPCHits_YZ[ifile]->DrawClone("colz");
	gMCHits_YZ[ifile]->Draw("p");
      }
    }
    
    // Truth Vertex distribtuion
    
    for(int ifile=0;ifile<NFiles;ifile++){
      
      double NbinsX = hMPPCHits_XY[ifile]->GetXaxis()->GetNbins();
      double minX = hMPPCHits_XY[ifile]->GetXaxis()->GetBinLowEdge(1);
      double maxX = hMPPCHits_XY[ifile]->GetXaxis()->GetBinLowEdge(NbinsX);
      double NbinsY = hMPPCHits_XY[ifile]->GetYaxis()->GetNbins();
      double minY = hMPPCHits_XY[ifile]->GetYaxis()->GetBinLowEdge(1);
      double maxY = hMPPCHits_XY[ifile]->GetYaxis()->GetBinLowEdge(NbinsY);
      
      TLine *lEdgesXY_1 = new TLine(minX,minY,maxX,minY);
      TLine *lEdgesXY_2 = new TLine(minX,maxY,maxX,maxY);
      TLine *lEdgesXY_3 = new TLine(minX,minY,minX,maxY);
      TLine *lEdgesXY_4 = new TLine(maxX,minY,maxX,maxY);
      
      name = TString::Format("cVtxOut_%s_XY",label[ifile].Data());
      cVtxOut_XY[ifile] = new TCanvas(name,name);
      hVtxOut_XY[ifile]->GetXaxis()->SetTitle("X (mm)");
      hVtxOut_XY[ifile]->GetYaxis()->SetTitle("Y (mm)");
      hVtxOut_XY[ifile]->SetStats(0);
      hVtxOut_XY[ifile]->DrawClone("colz");    
      
      lEdgesXY_1->Draw();
      lEdgesXY_2->Draw();
      lEdgesXY_3->Draw();
      lEdgesXY_4->Draw();
      
      name = TString::Format("cVtx_%s_XY",label[ifile].Data());
      cVtx_XY[ifile] = new TCanvas(name,name);
      hVtx_XY[ifile]->GetXaxis()->SetTitle("X (mm)");
      hVtx_XY[ifile]->GetYaxis()->SetTitle("Y (mm)");
      hVtx_XY[ifile]->SetStats(0);
      hVtx_XY[ifile]->DrawClone("colz");    
      
      lEdgesXY_1->Draw();
      lEdgesXY_2->Draw();
      lEdgesXY_3->Draw();
      lEdgesXY_4->Draw();
    }  

    for(int ifile=0;ifile<NFiles;ifile++){
      
      double NbinsX = hMPPCHits_XZ[ifile]->GetXaxis()->GetNbins();
      double minX = hMPPCHits_XZ[ifile]->GetXaxis()->GetBinLowEdge(1);
      double maxX = hMPPCHits_XZ[ifile]->GetXaxis()->GetBinLowEdge(NbinsX);
      double NbinsZ = hMPPCHits_XZ[ifile]->GetYaxis()->GetNbins();
      double minZ = hMPPCHits_XZ[ifile]->GetYaxis()->GetBinLowEdge(1);
      double maxZ = hMPPCHits_XZ[ifile]->GetYaxis()->GetBinLowEdge(NbinsZ);
      
      TLine *lEdgesXZ_1 = new TLine(minX,minZ,maxX,minZ);
      TLine *lEdgesXZ_2 = new TLine(minX,maxZ,maxX,maxZ);
      TLine *lEdgesXZ_3 = new TLine(minX,minZ,minX,maxZ);
      TLine *lEdgesXZ_4 = new TLine(maxX,minZ,maxX,maxZ);
      
      name = TString::Format("cVtxOut_%s_XZ",label[ifile].Data());
      cVtxOut_XZ[ifile] = new TCanvas(name,name);
      hVtxOut_XZ[ifile]->GetXaxis()->SetTitle("X (mm)");
      hVtxOut_XZ[ifile]->GetYaxis()->SetTitle("Z (mm)");
      hVtxOut_XZ[ifile]->SetStats(0);
      hVtxOut_XZ[ifile]->DrawClone("colz");    
      
      lEdgesXZ_1->Draw();
      lEdgesXZ_2->Draw();
      lEdgesXZ_3->Draw();
      lEdgesXZ_4->Draw();

      name = TString::Format("cVtx_%s_XZ",label[ifile].Data());
      cVtx_XZ[ifile] = new TCanvas(name,name);
      hVtx_XZ[ifile]->GetXaxis()->SetTitle("X (mm)");
      hVtx_XZ[ifile]->GetYaxis()->SetTitle("Z (mm)");
      hVtx_XZ[ifile]->SetStats(0);
      hVtx_XZ[ifile]->DrawClone("colz");    
      
      lEdgesXZ_1->Draw();
      lEdgesXZ_2->Draw();
      lEdgesXZ_3->Draw();
      lEdgesXZ_4->Draw();
    } 

    
    for(int ifile=0;ifile<NFiles;ifile++){
      
      double NbinsY = hMPPCHits_YZ[ifile]->GetXaxis()->GetNbins();
      double minY = hMPPCHits_YZ[ifile]->GetXaxis()->GetBinLowEdge(1);
      double maxY = hMPPCHits_YZ[ifile]->GetXaxis()->GetBinLowEdge(NbinsY);
      double NbinsZ = hMPPCHits_YZ[ifile]->GetYaxis()->GetNbins();
      double minZ = hMPPCHits_YZ[ifile]->GetYaxis()->GetBinLowEdge(1);
      double maxZ = hMPPCHits_YZ[ifile]->GetYaxis()->GetBinLowEdge(NbinsZ);
      
      TLine *lEdgesYZ_1 = new TLine(minY,minZ,maxY,minZ);
      TLine *lEdgesYZ_2 = new TLine(minY,maxZ,maxY,maxZ);
      TLine *lEdgesYZ_3 = new TLine(minY,minZ,minY,maxZ);
      TLine *lEdgesYZ_4 = new TLine(maxY,minZ,maxY,maxZ);
      
      name = TString::Format("cVtxOut_%s_YZ",label[ifile].Data());
      cVtxOut_YZ[ifile] = new TCanvas(name,name);
      hVtxOut_YZ[ifile]->GetXaxis()->SetTitle("Y (mm)");
      hVtxOut_YZ[ifile]->GetYaxis()->SetTitle("Z (mm)");
      hVtxOut_YZ[ifile]->SetStats(0);
      hVtxOut_YZ[ifile]->DrawClone("colz");    
      
      lEdgesYZ_1->Draw();
      lEdgesYZ_2->Draw();
      lEdgesYZ_3->Draw();
      lEdgesYZ_4->Draw();

      name = TString::Format("cVtx_%s_YZ",label[ifile].Data());
      cVtx_YZ[ifile] = new TCanvas(name,name);
      hVtx_YZ[ifile]->GetXaxis()->SetTitle("Y (mm)");
      hVtx_YZ[ifile]->GetYaxis()->SetTitle("Z (mm)");
      hVtx_YZ[ifile]->SetStats(0);
      hVtx_YZ[ifile]->DrawClone("colz");    
      
      lEdgesYZ_1->Draw();
      lEdgesYZ_2->Draw();
      lEdgesYZ_3->Draw();
      lEdgesYZ_4->Draw();
    } 
  
  } // if myevt >= 0 


  // Muons

  if(drawMuon){
    
    TCanvas *cMuon_CosTh_TrueVsReco[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      name = TString::Format("cMuon_CosTh_TrueVsReco_%i",ifile);
      cMuon_CosTh_TrueVsReco[ifile] = new TCanvas(name,label[ifile].Data());
      hMuon_CosTh_TrueVsReco[ifile]->GetXaxis()->SetTitle("True cos #theta");
      hMuon_CosTh_TrueVsReco[ifile]->GetYaxis()->SetTitle("Reco cos #theta");
      hMuon_CosTh_TrueVsReco[ifile]->SetStats(0);
      hMuon_CosTh_TrueVsReco[ifile]->DrawClone("colz");
    }
    
    TCanvas *cMuon_CosTh_RecMinTr = new TCanvas("cMuon_CosTh_RecMinTr","cMuon_CosTh_RecMinTr");
    for(int ifile=0;ifile<NFiles;ifile++){

      if(doNormalize) hMuon_CosTh_RecMinTr[ifile]->Scale(1./hMuon_CosTh_RecMinTr[ifile]->Integral());

      hMuon_CosTh_RecMinTr[ifile]->SetLineColor(ifile+1); 
      hMuon_CosTh_RecMinTr[ifile]->SetLineWidth(2); 
      hMuon_CosTh_RecMinTr[ifile]->GetXaxis()->SetTitle("Reco-True cos #theta");
      hMuon_CosTh_RecMinTr[ifile]->GetYaxis()->SetTitle("Entries");
      //hMuon_CosTh_RecMinTr[ifile]->SetStats(0);
      if(ifile==0) hMuon_CosTh_RecMinTr[ifile]->DrawClone("");
      else         hMuon_CosTh_RecMinTr[ifile]->DrawClone("same");
    }
    legend->Draw();
    
    TCanvas *cMuon_Len_TrueVsReco[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      name = TString::Format("cMuon_Len_TrueVsReco_%i",ifile);
      cMuon_Len_TrueVsReco[ifile] = new TCanvas(name,label[ifile].Data());
      hMuon_Len_TrueVsReco[ifile]->GetXaxis()->SetTitle("True Length [mm]");
      hMuon_Len_TrueVsReco[ifile]->GetYaxis()->SetTitle("Reco Length [mm]");
      hMuon_Len_TrueVsReco[ifile]->SetStats(0);
      hMuon_Len_TrueVsReco[ifile]->DrawClone("colz");
    }
    
    TCanvas *cMuon_Len_RecMinTr = new TCanvas("cMuon_Len_RecMinTr","cMuon_Len_RecMinTr");
    for(int ifile=0;ifile<NFiles;ifile++){
 
      if(doNormalize) hMuon_Len_RecMinTr[ifile]->Scale(1./hMuon_Len_RecMinTr[ifile]->Integral());

      hMuon_Len_RecMinTr[ifile]->SetLineColor(ifile+1); 
      hMuon_Len_RecMinTr[ifile]->SetLineWidth(2); 
      hMuon_Len_RecMinTr[ifile]->GetXaxis()->SetTitle("Reco-True Length [mm]");
      hMuon_Len_RecMinTr[ifile]->GetYaxis()->SetTitle("Entries");
      //hMuon_Len_RecMinTr[ifile]->SetStats(0);
      if(ifile==0) hMuon_Len_RecMinTr[ifile]->DrawClone("");
      else         hMuon_Len_RecMinTr[ifile]->Draw("same");
    }
    legend->Draw();
    
  }




  // Pions

  if(drawPion){
    
    TCanvas *cPion_CosTh_TrueVsReco[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      name = TString::Format("cPion_CosTh_TrueVsReco_%i",ifile);
      cPion_CosTh_TrueVsReco[ifile] = new TCanvas(name,label[ifile].Data());
      hPion_CosTh_TrueVsReco[ifile]->GetXaxis()->SetTitle("True cos #theta");
      hPion_CosTh_TrueVsReco[ifile]->GetYaxis()->SetTitle("Reco cos #theta");
      hPion_CosTh_TrueVsReco[ifile]->SetStats(0);
      hPion_CosTh_TrueVsReco[ifile]->DrawClone("colz");
    }
  
    TCanvas *cPion_CosTh_RecMinTr = new TCanvas("cPion_CosTh_RecMinTr","cPion_CosTh_RecMinTr");
    for(int ifile=0;ifile<NFiles;ifile++){

      if(doNormalize) hPion_CosTh_RecMinTr[ifile]->Scale(1./hPion_CosTh_RecMinTr[ifile]->Integral());

      hPion_CosTh_RecMinTr[ifile]->SetLineColor(ifile+1); 
      hPion_CosTh_RecMinTr[ifile]->SetLineWidth(2); 
      hPion_CosTh_RecMinTr[ifile]->GetXaxis()->SetTitle("Reco-True cos #theta");
      hPion_CosTh_RecMinTr[ifile]->GetYaxis()->SetTitle("Entries");
      //hPion_CosTh_RecMinTr[ifile]->SetStats(0);
      if(ifile==0) hPion_CosTh_RecMinTr[ifile]->DrawClone("");
      else         hPion_CosTh_RecMinTr[ifile]->DrawClone("same");
    }
    legend->Draw();
    
    TCanvas *cPion_Len_TrueVsReco[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      name = TString::Format("cPion_Len_TrueVsReco_%i",ifile);
      cPion_Len_TrueVsReco[ifile] = new TCanvas(name,label[ifile].Data());
      hPion_Len_TrueVsReco[ifile]->GetXaxis()->SetTitle("True Length [mm]");
      hPion_Len_TrueVsReco[ifile]->GetYaxis()->SetTitle("Reco Length [mm]");
      hPion_Len_TrueVsReco[ifile]->SetStats(0);
      hPion_Len_TrueVsReco[ifile]->DrawClone("colz");
    }
    
    TCanvas *cPion_Len_RecMinTr = new TCanvas("cPion_Len_RecMinTr","cPion_Len_RecMinTr");
    for(int ifile=0;ifile<NFiles;ifile++){

      if(doNormalize) hPion_Len_RecMinTr[ifile]->Scale(1./hPion_Len_RecMinTr[ifile]->Integral());
    
      hPion_Len_RecMinTr[ifile]->SetLineColor(ifile+1); 
      hPion_Len_RecMinTr[ifile]->SetLineWidth(2); 
      hPion_Len_RecMinTr[ifile]->GetXaxis()->SetTitle("Reco-True Length [mm]");
      hPion_Len_RecMinTr[ifile]->GetYaxis()->SetTitle("Entries");
      //hPion_Len_RecMinTr[ifile]->SetStats(0);
      if(ifile==0) hPion_Len_RecMinTr[ifile]->DrawClone("");
      else         hPion_Len_RecMinTr[ifile]->Draw("same");
    }
    legend->Draw();
  }




  // Electrons

  if(drawElec){
    
    TCanvas *cElec_CosTh_TrueVsReco[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      name = TString::Format("cElec_CosTh_TrueVsReco_%i",ifile);
      cElec_CosTh_TrueVsReco[ifile] = new TCanvas(name,label[ifile].Data());
      hElec_CosTh_TrueVsReco[ifile]->GetXaxis()->SetTitle("True cos #theta");
      hElec_CosTh_TrueVsReco[ifile]->GetYaxis()->SetTitle("Reco cos #theta");
      hElec_CosTh_TrueVsReco[ifile]->SetStats(0);
      hElec_CosTh_TrueVsReco[ifile]->DrawClone("colz");
    }
    
    TCanvas *cElec_CosTh_RecMinTr = new TCanvas("cElec_CosTh_RecMinTr","cElec_CosTh_RecMinTr");
    for(int ifile=0;ifile<NFiles;ifile++){

      if(doNormalize) hElec_CosTh_RecMinTr[ifile]->Scale(1./hElec_CosTh_RecMinTr[ifile]->Integral());

      hElec_CosTh_RecMinTr[ifile]->SetLineColor(ifile+1); 
      hElec_CosTh_RecMinTr[ifile]->SetLineWidth(2); 
      hElec_CosTh_RecMinTr[ifile]->GetXaxis()->SetTitle("Reco-True cos #theta");
      hElec_CosTh_RecMinTr[ifile]->GetYaxis()->SetTitle("Entries");
      //hElec_CosTh_RecMinTr[ifile]->SetStats(0);
      if(ifile==0) hElec_CosTh_RecMinTr[ifile]->DrawClone("");
      else         hElec_CosTh_RecMinTr[ifile]->DrawClone("same");
    }
    legend->Draw();
    
    TCanvas *cElec_Len_TrueVsReco[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      name = TString::Format("cElec_Len_TrueVsReco_%i",ifile);
      cElec_Len_TrueVsReco[ifile] = new TCanvas(name,label[ifile].Data());
      hElec_Len_TrueVsReco[ifile]->GetXaxis()->SetTitle("True Length [mm]");
      hElec_Len_TrueVsReco[ifile]->GetYaxis()->SetTitle("Reco Length [mm]");
      hElec_Len_TrueVsReco[ifile]->SetStats(0);
      hElec_Len_TrueVsReco[ifile]->DrawClone("colz");
    }
    
    TCanvas *cElec_Len_RecMinTr = new TCanvas("cElec_Len_RecMinTr","cElec_Len_RecMinTr");

    if(doNormalize) hElec_Len_RecMinTr[ifile]->Scale(1./hElec_Len_RecMinTr[ifile]->Integral());

    for(int ifile=0;ifile<NFiles;ifile++){
      hElec_Len_RecMinTr[ifile]->SetLineColor(ifile+1); 
      hElec_Len_RecMinTr[ifile]->SetLineWidth(2); 
      hElec_Len_RecMinTr[ifile]->GetXaxis()->SetTitle("Reco-True Length [mm]");
      hElec_Len_RecMinTr[ifile]->GetYaxis()->SetTitle("Entries");
      //hElec_Len_RecMinTr[ifile]->SetStats(0);
      if(ifile==0) hElec_Len_RecMinTr[ifile]->DrawClone("");
      else         hElec_Len_RecMinTr[ifile]->Draw("same");
    }
    legend->Draw();
    
  }




  // Print results

  cout << endl;
  cout << "////// Muons //////" << endl;
  cout << " - Track Reco-True Length (mm):" << endl;
  for(int ifile=0;ifile<NFiles;ifile++){
    cout << "   // " << label[ifile] << ": ";
    cout << "  Mean = " << hMuon_Len_RecMinTr[ifile]->GetMean() 
	 << ", RMS = " <<  hMuon_Len_RecMinTr[ifile]->GetRMS()
	 << endl;
  }
  cout << " - Track Reco-True CosTheta:" << endl;
  for(int ifile=0;ifile<NFiles;ifile++){
    cout << "   // " << label[ifile] << ": ";
    cout << "  Mean = " << hMuon_CosTh_RecMinTr[ifile]->GetMean() 
	 << ", RMS = " <<  hMuon_CosTh_RecMinTr[ifile]->GetRMS()
	 << endl;
  }
  cout << endl;

  cout << endl;
  cout << "////// Pions //////" << endl;
  cout << " - Track Reco-True Length (mm):" << endl;
  for(int ifile=0;ifile<NFiles;ifile++){
    cout << "   // " << label[ifile] << ": ";
    cout << "  Mean = " << hPion_Len_RecMinTr[ifile]->GetMean() 
	 << ", RMS = " <<  hPion_Len_RecMinTr[ifile]->GetRMS()
	 << endl;
  }
  cout << " - Track Reco-True CosTheta:" << endl;
  for(int ifile=0;ifile<NFiles;ifile++){
    cout << "   // " << label[ifile] << ": ";
    cout << "  Mean = " << hPion_CosTh_RecMinTr[ifile]->GetMean() 
	 << ", RMS = " <<  hPion_CosTh_RecMinTr[ifile]->GetRMS()
	 << endl;
  }
  cout << endl;

  cout << endl;
  cout << "////// Electrons //////" << endl;
  cout << " - Track Reco-True Length (mm):" << endl;
  for(int ifile=0;ifile<NFiles;ifile++){
    cout << "   // " << label[ifile] << ": ";
    cout << "  Mean = " << hElec_Len_RecMinTr[ifile]->GetMean() 
	 << ", RMS = " <<  hElec_Len_RecMinTr[ifile]->GetRMS()
	 << endl;
  }
  cout << " - Track Reco-True CosTheta:" << endl;
  for(int ifile=0;ifile<NFiles;ifile++){
    cout << "   // " << label[ifile] << ": ";
    cout << "  Mean = " << hElec_CosTh_RecMinTr[ifile]->GetMean() 
	 << ", RMS = " <<  hElec_CosTh_RecMinTr[ifile]->GetRMS()
	 << endl;
  }
  cout << endl;

  return;
}
