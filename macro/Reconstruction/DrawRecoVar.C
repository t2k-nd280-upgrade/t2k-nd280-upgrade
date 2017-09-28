
void DrawRecoVar
( 
 int myevt = 6,
 //bool doPrint = true,
 //bool doStore = false,
 string tag = "prova",
 bool drawMuon = true,
 bool drawPion = false,
 bool drawProt = false,
 bool drawElec = false
  )
{
 
  const int NFiles = 2;
  string infilename[NFiles] = {

    "RECONSTRUCTION/26_9_17/RealMPPCeff/SuperFGD-UseXY-UseXZ-UseYZ-TruthFV-Separate10_100k.root",

    "RECONSTRUCTION/26_9_17/RealMPPCeff/FGDlike-UseXY-UseXZ-UseYZ-TruthFV-Separate10_CutLayers30mm_100k.root"

  };

  int color[NFiles] = {
    kBlue,
    kRed
    //kGreen
    //kOrange
    //kCyan
  };

  TString label[NFiles] = {
    "SuperFGD",
    "FGDlike"

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
    legend->AddEntry(hMuon_Len_RecMinTr[ifile],label[ifile],"l");
  }

  // Event display

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


  // Muons

  if(drawMuon){
    
    TCanvas *cMuon_CosTh_TrueVsReco[NFiles];
    for(int ifile=0;ifile<NFiles;ifile++){
      name = TString::Format("cMuon_CosTh_TrueVsReco_%i",ifile);
      cMuon_CosTh_TrueVsReco[ifile] = new TCanvas(name,label[ifile]);
      hMuon_CosTh_TrueVsReco[ifile]->GetXaxis()->SetTitle("True cos #theta");
      hMuon_CosTh_TrueVsReco[ifile]->GetYaxis()->SetTitle("Reco cos #theta");
      hMuon_CosTh_TrueVsReco[ifile]->SetStats(0);
      hMuon_CosTh_TrueVsReco[ifile]->DrawClone("colz");
    }
    
    TCanvas *cMuon_CosTh_RecMinTr = new TCanvas("cMuon_CosTh_RecMinTr","cMuon_CosTh_RecMinTr");
    for(int ifile=0;ifile<NFiles;ifile++){
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
      cMuon_Len_TrueVsReco[ifile] = new TCanvas(name,label[ifile]);
      hMuon_Len_TrueVsReco[ifile]->GetXaxis()->SetTitle("True Length [mm]");
      hMuon_Len_TrueVsReco[ifile]->GetYaxis()->SetTitle("Reco Length [mm]");
      hMuon_Len_TrueVsReco[ifile]->SetStats(0);
      hMuon_Len_TrueVsReco[ifile]->DrawClone("colz");
    }
    
    TCanvas *cMuon_Len_RecMinTr = new TCanvas("cMuon_Len_RecMinTr","cMuon_Len_RecMinTr");
    for(int ifile=0;ifile<NFiles;ifile++){
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
      cPion_CosTh_TrueVsReco[ifile] = new TCanvas(name,label[ifile]);
      hPion_CosTh_TrueVsReco[ifile]->GetXaxis()->SetTitle("True cos #theta");
      hPion_CosTh_TrueVsReco[ifile]->GetYaxis()->SetTitle("Reco cos #theta");
      hPion_CosTh_TrueVsReco[ifile]->SetStats(0);
      hPion_CosTh_TrueVsReco[ifile]->DrawClone("colz");
    }
  
    TCanvas *cPion_CosTh_RecMinTr = new TCanvas("cPion_CosTh_RecMinTr","cPion_CosTh_RecMinTr");
    for(int ifile=0;ifile<NFiles;ifile++){
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
      cPion_Len_TrueVsReco[ifile] = new TCanvas(name,label[ifile]);
      hPion_Len_TrueVsReco[ifile]->GetXaxis()->SetTitle("True Length [mm]");
      hPion_Len_TrueVsReco[ifile]->GetYaxis()->SetTitle("Reco Length [mm]");
      hPion_Len_TrueVsReco[ifile]->SetStats(0);
      hPion_Len_TrueVsReco[ifile]->DrawClone("colz");
    }
    
    TCanvas *cPion_Len_RecMinTr = new TCanvas("cPion_Len_RecMinTr","cPion_Len_RecMinTr");
    for(int ifile=0;ifile<NFiles;ifile++){
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

  return;
}
