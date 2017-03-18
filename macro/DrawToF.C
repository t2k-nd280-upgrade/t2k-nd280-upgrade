
void DrawToF
( 
  )
{
  gROOT->SetStyle("T2K");                  
  gROOT->ForceStyle();                 
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadRightMargin(0.15);
  //gStyle->SetStatX(.4); gStyle->SetStatY(.6);

  const int Nfiles = 4;
  string infilename[Nfiles];
  string label[Nfiles];

  infilename[0] = "/neutrino/data7/davide/out_cluster/PROVA/pid_ToF/TimeRes_100ps/ToFND280Up_Ref_numu_1E21-Targ1-MuM-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.1ns_All.root";
  label[0] = "#mu^{-} - #DeltaT=100ps";

  infilename[1] = "/neutrino/data7/davide/out_cluster/PROVA/pid_ToF/TimeRes_100ps/ToFND280Up_Ref_numu_1E21-Targ1-PiP-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.1ns_All.root";
  label[1] = "#pi^{+} - #DeltaT=100ps";
  
  infilename[2] = "/neutrino/data7/davide/out_cluster/PROVA/pid_ToF/TimeRes_100ps/ToFND280Up_Ref_numu_1E21-Targ1-Prot-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.1ns_All.root";
  label[2] = "p - #DeltaT=100ps";
  
  infilename[3] = "/neutrino/data7/davide/out_cluster/PROVA/pid_ToF/TimeRes_100ps/ToFND280Up_Ref_nue_1E21-Targ1-EleM-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.1ns_Evt0_NEvt10000.root";
  label[3] = "e^{-} - #DeltaT=100ps";


  TFile *infile[Nfiles];

  TH1D *hCut[Nfiles]; 
  //
  TH1D *hVtx_X[Nfiles];
  TH1D *hVtx_Y[Nfiles];	
  TH1D *hVtx_Z[Nfiles];
  TH2D *hVtx_XY[Nfiles];
  TH2D *hVtx_XZ[Nfiles];
  TH2D *hVtx_YZ[Nfiles];
  TH1D *hVtxOut_X[Nfiles];
  TH1D *hVtxOut_Y[Nfiles];	
  TH1D *hVtxOut_Z[Nfiles];
  TH2D *hVtxOut_XY[Nfiles];
  TH2D *hVtxOut_XZ[Nfiles];
  TH2D *hVtxOut_YZ[Nfiles];
  //
  TH1D *hTimeTarg1_DsECal[Nfiles];  
  TH1D *hTimeTarg1_BrlECal[Nfiles];
  TH1D *hTimeTarg1_P0DECal[Nfiles];
  TH1D *hTimeTarg1_USECalP0D[Nfiles];
  TH1D *hTimeTarg1_ForwTPC1[Nfiles];
  TH1D *hTimeTarg1_ForwTPC2[Nfiles];
  TH1D *hTimeTarg1_ForwTPC3[Nfiles];
  TH1D *hTimeTarg1_Target1[Nfiles];
  TH1D *hTimeTarg1_Target2[Nfiles];
  TH1D *hTimeTarg1_TPCUp1[Nfiles];
  TH1D *hTimeTarg1_TPCUp2[Nfiles];
  TH1D *hTimeTarg1_TPCDown1[Nfiles];
  TH1D *hTimeTarg1_TPCDown2[Nfiles];
  TH1D *hTimeTarg1_ToFTopUp[Nfiles];
  TH1D *hTimeTarg1_ToFBotUp[Nfiles];
  TH1D *hTimeTarg1_ToFRightUp[Nfiles];
  TH1D *hTimeTarg1_ToFLeftUp[Nfiles];
  TH1D *hTimeTarg1_ToFBackUp[Nfiles];
  TH1D *hTimeTarg1_ToFFrontUp[Nfiles];
  TH1D *hTimeTarg1_ToFTopDown[Nfiles];
  TH1D *hTimeTarg1_ToFBotDown[Nfiles];
  TH1D *hTimeTarg1_ToFRightDown[Nfiles];
  TH1D *hTimeTarg1_ToFLeftDown[Nfiles];
  TH1D *hTimeTarg1_ToFBackDown[Nfiles];
  TH1D *hTimeTarg1_ToFFrontDown[Nfiles];
  TH1D *hTimeTarg1_ToFECalP0D[Nfiles];
  TH1D *hTimeTarg1_ToFAll[Nfiles];
  //
  TH1D *hMassTarg1_DsECal[Nfiles];  
  TH1D *hMassTarg1_BrlECal[Nfiles];
  TH1D *hMassTarg1_P0DECal[Nfiles];
  TH1D *hMassTarg1_USECalP0D[Nfiles];
  TH1D *hMassTarg1_ForwTPC1[Nfiles];
  TH1D *hMassTarg1_ForwTPC2[Nfiles];
  TH1D *hMassTarg1_ForwTPC3[Nfiles];
  TH1D *hMassTarg1_Target1[Nfiles];
  TH1D *hMassTarg1_Target2[Nfiles];
  TH1D *hMassTarg1_TPCUp1[Nfiles];
  TH1D *hMassTarg1_TPCUp2[Nfiles];
  TH1D *hMassTarg1_TPCDown1[Nfiles];
  TH1D *hMassTarg1_TPCDown2[Nfiles];
  TH1D *hMassTarg1_ToFTopUp[Nfiles];
  TH1D *hMassTarg1_ToFBotUp[Nfiles];
  TH1D *hMassTarg1_ToFRightUp[Nfiles];
  TH1D *hMassTarg1_ToFLeftUp[Nfiles];
  TH1D *hMassTarg1_ToFBackUp[Nfiles];
  TH1D *hMassTarg1_ToFFrontUp[Nfiles];
  TH1D *hMassTarg1_ToFTopDown[Nfiles];
  TH1D *hMassTarg1_ToFBotDown[Nfiles];
  TH1D *hMassTarg1_ToFRightDown[Nfiles];
  TH1D *hMassTarg1_ToFLeftDown[Nfiles];
  TH1D *hMassTarg1_ToFBackDown[Nfiles];
  TH1D *hMassTarg1_ToFFrontDown[Nfiles];
  TH1D *hMassTarg1_ToFECalP0D[Nfiles];
  TH1D *hMassTarg1_ToFAll[Nfiles];

  // Loop over the files
  
  for(int ifile=0;ifile<Nfiles;ifile++){
    
    cout << "Open file: " << infilename[ifile] << endl;
    
    infile[ifile] = new TFile(infilename[ifile].c_str(),"READ");
    
    hVtx_X[ifile] = (TH1D*)infile[ifile]->Get("hVtx_X");
    hVtx_Y[ifile] = (TH1D*)infile[ifile]->Get("hVtx_Y");
    hVtx_Z[ifile] = (TH1D*)infile[ifile]->Get("hVtx_Z");
    hVtx_XY[ifile] = (TH2D*)infile[ifile]->Get("hVtx_XY");
    hVtx_XZ[ifile] = (TH2D*)infile[ifile]->Get("hVtx_XZ");
    hVtx_YZ[ifile] = (TH2D*)infile[ifile]->Get("hVtx_YZ");    
    hVtxOut_X[ifile] = (TH1D*)infile[ifile]->Get("hVtxOut_X");
    hVtxOut_Y[ifile] = (TH1D*)infile[ifile]->Get("hVtxOut_Y");
    hVtxOut_Z[ifile] = (TH1D*)infile[ifile]->Get("hVtxOut_Z");
    hVtxOut_XY[ifile] = (TH2D*)infile[ifile]->Get("hVtxOut_XY");
    hVtxOut_XZ[ifile] = (TH2D*)infile[ifile]->Get("hVtxOut_XZ");
    hVtxOut_YZ[ifile] = (TH2D*)infile[ifile]->Get("hVtxOut_YZ");
    //
    hTimeTarg1_DsECal[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_DsECal");  
    hTimeTarg1_BrlECal[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_BrlECal"); 
    hTimeTarg1_P0DECal[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_P0DECal"); 
    hTimeTarg1_USECalP0D[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_USECalP0D"); 
    hTimeTarg1_ForwTPC1[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ForwTPC1"); 
    hTimeTarg1_ForwTPC2[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ForwTPC2"); 
    hTimeTarg1_ForwTPC3[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ForwTPC3"); 
    hTimeTarg1_Target1[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_Target1"); 
    hTimeTarg1_Target2[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_Target2"); 
    hTimeTarg1_TPCUp1[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_TPCUp1"); 
    hTimeTarg1_TPCUp2[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_TPCUp2"); 
    hTimeTarg1_TPCDown1[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_TPCDown1"); 
    hTimeTarg1_TPCDown2[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_TPCDown2"); 
    hTimeTarg1_ToFTopUp[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFTopUp"); 
    hTimeTarg1_ToFBotUp[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFBotUp"); 
    hTimeTarg1_ToFRightUp[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFRightUp"); 
    hTimeTarg1_ToFLeftUp[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFLeftUp"); 
    hTimeTarg1_ToFBackUp[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFBackUp"); 
    hTimeTarg1_ToFFrontUp[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFFrontUp"); 
    hTimeTarg1_ToFTopDown[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFTopDown"); 
    hTimeTarg1_ToFBotDown[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFBotDown"); 
    hTimeTarg1_ToFRightDown[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFRightDown"); 
    hTimeTarg1_ToFLeftDown[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFLeftDown"); 
    hTimeTarg1_ToFBackDown[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFBackDown"); 
    hTimeTarg1_ToFFrontDown[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFFrontDown"); 
    hTimeTarg1_ToFECalP0D[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFECalP0D"); 
    //
    hMassTarg1_DsECal[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_DsECal");  hMassTarg1_ToFAll[ifile] = (TH1D*)hMassTarg1_DsECal[ifile]->Clone("hMassTarg1_ToFAll");
    hMassTarg1_BrlECal[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_BrlECal"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_BrlECal[ifile]);
    hMassTarg1_P0DECal[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_P0DECal"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_P0DECal[ifile]);
    hMassTarg1_USECalP0D[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_USECalP0D"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_USECalP0D[ifile]);
    hMassTarg1_ForwTPC1[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ForwTPC1"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ForwTPC1[ifile]);
    hMassTarg1_ForwTPC2[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ForwTPC2"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ForwTPC2[ifile]);
    hMassTarg1_ForwTPC3[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ForwTPC3"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ForwTPC3[ifile]);
    hMassTarg1_Target1[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_Target1"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_Target1[ifile]);
    hMassTarg1_Target2[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_Target2"); hMassTarg1_ToFAll[ifile]->Add( hMassTarg1_Target2[ifile]);
    hMassTarg1_TPCUp1[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_TPCUp1"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_TPCUp1[ifile]);
    hMassTarg1_TPCUp2[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_TPCUp2"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_TPCUp2[ifile]);
    hMassTarg1_TPCDown1[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_TPCDown1"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_TPCDown1[ifile]);
    hMassTarg1_TPCDown2[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_TPCDown2"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_TPCDown2[ifile]);
    hMassTarg1_ToFTopUp[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFTopUp"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFTopUp[ifile]);
    hMassTarg1_ToFBotUp[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFBotUp"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFBotUp[ifile]);
    hMassTarg1_ToFRightUp[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFRightUp"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFRightUp[ifile]);
    hMassTarg1_ToFLeftUp[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFLeftUp"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFLeftUp[ifile]);
    hMassTarg1_ToFBackUp[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFBackUp"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFBackUp[ifile]);
    hMassTarg1_ToFFrontUp[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFFrontUp"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFFrontUp[ifile]);
    hMassTarg1_ToFTopDown[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFTopDown"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFTopDown[ifile]);
    hMassTarg1_ToFBotDown[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFBotDown"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFBotDown[ifile]);
    hMassTarg1_ToFRightDown[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFRightDown"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFRightDown[ifile]);
    hMassTarg1_ToFLeftDown[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFLeftDown"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFLeftDown[ifile]);
    hMassTarg1_ToFBackDown[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFBackDown"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFBackDown[ifile]);
    hMassTarg1_ToFFrontDown[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFFrontDown"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFFrontDown[ifile]);
    hMassTarg1_ToFECalP0D[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFECalP0D"); hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFECalP0D[ifile]);    
  }
    
  
  TCanvas *cTimeTarg1_DsECal = new TCanvas("cTimeTarg1_DsECal","cTimeTarg1_DsECal");  
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile=0) hTimeTarg1_DsECal[ifile]->Draw();
    else        hTimeTarg1_DsECal[ifile]->Draw("same");
  }

  return;

  TCanvas *cTimeTarg1_BrlECal = new TCanvas("cTimeTarg1_BrlECal","cTimeTarg1_BrlECal"); 
  TCanvas *cTimeTarg1_P0DECal = new TCanvas("cTimeTarg1_P0DECal","cTimeTarg1_P0DECal");
  TCanvas *cTimeTarg1_USECalP0D = new TCanvas("cTimeTarg1_USECalP0D","cTimeTarg1_USECalP0D");
  TCanvas *cTimeTarg1_ForwTPC1 = new TCanvas("cTimeTarg1_ForwTPC1","cTimeTarg1_ForwTPC1");
  TCanvas *cTimeTarg1_ForwTPC2 = new TCanvas("cTimeTarg1_ForwTPC2","cTimeTarg1_ForwTPC2");
  TCanvas *cTimeTarg1_ForwTPC3 = new TCanvas("cTimeTarg1_ForwTPC3","cTimeTarg1_ForwTPC3");
  TCanvas *cTimeTarg1_Target1 = new TCanvas("cTimeTarg1_Target1","cTimeTarg1_Target1");
  TCanvas *cTimeTarg1_Target2 = new TCanvas("cTimeTarg1_Target2","cTimeTarg1_Target2");
  TCanvas *cTimeTarg1_TPCUp1 = new TCanvas("cTimeTarg1_TPCUp1","cTimeTarg1_TPCUp1");
  TCanvas *cTimeTarg1_TPCUp2 = new TCanvas("cTimeTarg1_TPCUp2","cTimeTarg1_TPCUp2");
  TCanvas *cTimeTarg1_TPCDown1 = new TCanvas("cTimeTarg1_TPCDown1","cTimeTarg1_TPCDown1");
  TCanvas *cTimeTarg1_TPCDown2 = new TCanvas("cTimeTarg1_TPCDown2","cTimeTarg1_TPCDown2");
  TCanvas *cTimeTarg1_ToFTopUp = new TCanvas("cTimeTarg1_ToFTopUp","cTimeTarg1_ToFTopUp");
  TCanvas *cTimeTarg1_ToFBotUp = new TCanvas("cTimeTarg1_ToFBotUp","cTimeTarg1_ToFBotUp");
  TCanvas *cTimeTarg1_ToFRightUp = new TCanvas("cTimeTarg1_ToFRightUp","cTimeTarg1_ToFRightUp");
  TCanvas *cTimeTarg1_ToFLeftUp = new TCanvas("cTimeTarg1_ToFLeftUp","cTimeTarg1_ToFLeftUp");
  TCanvas *cTimeTarg1_ToFBackUp = new TCanvas("cTimeTarg1_ToFBackUp","cTimeTarg1_ToFBackUp");
  TCanvas *cTimeTarg1_ToFFrontUp = new TCanvas("cTimeTarg1_ToFFrontUp","cTimeTarg1_ToFFrontUp");
  TCanvas *cTimeTarg1_ToFTopDown = new TCanvas("cTimeTarg1_ToFTopDown","cTimeTarg1_ToFTopDown");
  TCanvas *cTimeTarg1_ToFBotDown = new TCanvas("cTimeTarg1_ToFBotDown","cTimeTarg1_ToFBotDown");
  TCanvas *cTimeTarg1_ToFRightDown = new TCanvas("cTimeTarg1_ToFRightDown","cTimeTarg1_ToFRightDown");
  TCanvas *cTimeTarg1_ToFLeftDown = new TCanvas("cTimeTarg1_ToFLeftDown","cTimeTarg1_ToFLeftDown");
  TCanvas *cTimeTarg1_ToFBackDown = new TCanvas("cTimeTarg1_ToFBackDown","cTimeTarg1_ToFBackDown");
  TCanvas *cTimeTarg1_ToFFrontDown = new TCanvas("cTimeTarg1_ToFFrontDown","cTimeTarg1_ToFFrontDown");
  TCanvas *cTimeTarg1_ToFECalP0D = new TCanvas("cTimeTarg1_ToFECalP0D","cTimeTarg1_ToFECalP0D");
  TCanvas *cTimeTarg1_ToFAll = new TCanvas("cTimeTarg1_ToFAll","cTimeTarg1_ToFAll");
  //
  TCanvas *cMassTarg1_DsECal = new TCanvas("cMassTarg1_DsECal","cMassTarg1_DsECal");  
  TCanvas *cMassTarg1_BrlECal = new TCanvas("cMassTarg1_BrlECal","cMassTarg1_BrlECal");
  TCanvas *cMassTarg1_P0DECal = new TCanvas("cMassTarg1_P0DECal","cMassTarg1_P0DECal");
  TCanvas *cMassTarg1_USECalP0D = new TCanvas("cMassTarg1_USECalP0D","cMassTarg1_USECalP0D");
  TCanvas *cMassTarg1_ForwTPC1 = new TCanvas("cMassTarg1_ForwTPC1","cMassTarg1_ForwTPC1");
  TCanvas *cMassTarg1_ForwTPC2 = new TCanvas("cMassTarg1_ForwTPC2","cMassTarg1_ForwTPC2");
  TCanvas *cMassTarg1_ForwTPC3 = new TCanvas("cMassTarg1_ForwTPC3","cMassTarg1_ForwTPC3");
  TCanvas *cMassTarg1_Target1 = new TCanvas("cMassTarg1_Target1","cMassTarg1_Target1");
  TCanvas *cMassTarg1_Target2 = new TCanvas("cMassTarg1_Target2","cMassTarg1_Target2");
  TCanvas *cMassTarg1_TPCUp1 = new TCanvas("cMassTarg1_TPCUp1","cMassTarg1_TPCUp1");
  TCanvas *cMassTarg1_TPCUp2 = new TCanvas("cMassTarg1_TPCUp2","cMassTarg1_TPCUp2");
  TCanvas *cMassTarg1_TPCDown1 = new TCanvas("cMassTarg1_TPCDown1","cMassTarg1_TPCDown1");
  TCanvas *cMassTarg1_TPCDown2 = new TCanvas("cMassTarg1_TPCDown2","cMassTarg1_TPCDown2");
  TCanvas *cMassTarg1_ToFTopUp = new TCanvas("cMassTarg1_ToFTopUp","cMassTarg1_ToFTopUp");
  TCanvas *cMassTarg1_ToFBotUp = new TCanvas("cMassTarg1_ToFBotUp","cMassTarg1_ToFBotUp");
  TCanvas *cMassTarg1_ToFRightUp = new TCanvas("cMassTarg1_ToFRightUp","cMassTarg1_ToFRightUp");
  TCanvas *cMassTarg1_ToFLeftUp = new TCanvas("cMassTarg1_ToFLeftUp","cMassTarg1_ToFLeftUp");
  TCanvas *cMassTarg1_ToFBackUp = new TCanvas("cMassTarg1_ToFBackUp","cMassTarg1_ToFBackUp");
  TCanvas *cMassTarg1_ToFFrontUp = new TCanvas("cMassTarg1_ToFFrontUp","cMassTarg1_ToFFrontUp");
  TCanvas *cMassTarg1_ToFTopDown = new TCanvas("cMassTarg1_ToFTopDown","cMassTarg1_ToFTopDown");
  TCanvas *cMassTarg1_ToFBotDown = new TCanvas("cMassTarg1_ToFBotDown","cMassTarg1_ToFBotDown");
  TCanvas *cMassTarg1_ToFRightDown = new TCanvas("cMassTarg1_ToFRightDown","cMassTarg1_ToFRightDown");
  TCanvas *cMassTarg1_ToFLeftDown = new TCanvas("cMassTarg1_ToFLeftDown","cMassTarg1_ToFLeftDown");
  TCanvas *cMassTarg1_ToFBackDown = new TCanvas("cMassTarg1_ToFBackDown","cMassTarg1_ToFBackDown");
  TCanvas *cMassTarg1_ToFFrontDown = new TCanvas("cMassTarg1_ToFFrontDown","cMassTarg1_ToFFrontDown");
  TCanvas *cMassTarg1_ToFECalP0D = new TCanvas("cMassTarg1_ToFECalP0D","cMassTarg1_ToFECalP0D");
  TCanvas *cMassTarg1_ToFAll = new TCanvas("cMassTarg1_ToFAll","cMassTarg1_ToFAll");
 



    

  return;



  /*
  TFile *infile_muon = new TFile(infilename_muon.c_str(),"READ");
  TFile *infile_elec = new TFile(infilename_muon.c_str(),"READ");
  TFile *infile_ = new TFile(infilename_muon.c_str(),"READ");
  TFile *infile_muon = new TFile(infilename_muon.c_str(),"READ");
  */
  
  // // Vertex
  // TH2D *hForwTPC1TrkPtXY = (TH2D*)infile->Get("hForwTPC1TrkPtXY");
  // TH2D *hForwTPC1TrkPtXZ = (TH2D*)infile->Get("hForwTPC1TrkPtXZ");
  // TH2D *hForwTPC1TrkPtYZ = (TH2D*)infile->Get("hForwTPC1TrkPtYZ");
  // TH2D *hForwTPC2TrkPtXY = (TH2D*)infile->Get("hForwTPC2TrkPtXY");
  // TH2D *hForwTPC2TrkPtXZ = (TH2D*)infile->Get("hForwTPC2TrkPtXZ");
  // TH2D *hForwTPC2TrkPtYZ = (TH2D*)infile->Get("hForwTPC2TrkPtYZ");
  // TH2D *hForwTPC3TrkPtXY = (TH2D*)infile->Get("hForwTPC3TrkPtXY");
  // TH2D *hForwTPC3TrkPtXZ = (TH2D*)infile->Get("hForwTPC3TrkPtXZ");
  // TH2D *hForwTPC3TrkPtYZ = (TH2D*)infile->Get("hForwTPC3TrkPtYZ");
  // TH2D *hTPCUp1TrkPtXY = (TH2D*)infile->Get("hTPCUp1TrkPtXY");
  // TH2D *hTPCUp1TrkPtXZ = (TH2D*)infile->Get("hTPCUp1TrkPtXZ");
  // TH2D *hTPCUp1TrkPtYZ = (TH2D*)infile->Get("hTPCUp1TrkPtYZ");
  // TH2D *hTPCUp2TrkPtXY = (TH2D*)infile->Get("hTPCUp2TrkPtXY");
  // TH2D *hTPCUp2TrkPtXZ = (TH2D*)infile->Get("hTPCUp2TrkPtXZ");
  // TH2D *hTPCUp2TrkPtYZ = (TH2D*)infile->Get("hTPCUp2TrkPtYZ");
  // TH2D *hTPCDown1TrkPtXY = (TH2D*)infile->Get("hTPCDown1TrkPtXY");
  // TH2D *hTPCDown1TrkPtXZ = (TH2D*)infile->Get("hTPCDown1TrkPtXZ");
  // TH2D *hTPCDown1TrkPtYZ = (TH2D*)infile->Get("hTPCDown1TrkPtYZ");
  // TH2D *hTPCDown2TrkPtXY = (TH2D*)infile->Get("hTPCDown2TrkPtXY");
  // TH2D *hTPCDown2TrkPtXZ = (TH2D*)infile->Get("hTPCDown2TrkPtXZ");
  // TH2D *hTPCDown2TrkPtYZ = (TH2D*)infile->Get("hTPCDown2TrkPtYZ");
  // TH2D *hDsECalTrkPtXY = (TH2D*)infile->Get("hDsECalTrkPtXY");
  // TH2D *hDsECalTrkPtXZ = (TH2D*)infile->Get("hDsECalTrkPtXZ");
  // TH2D *hDsECalTrkPtYZ = (TH2D*)infile->Get("hDsECalTrkPtYZ");
  // TH2D *hBrlECalTrkPtXY = (TH2D*)infile->Get("hBrlECalTrkPtXY");
  // TH2D *hBrlECalTrkPtXZ = (TH2D*)infile->Get("hBrlECalTrkPtXZ");
  // TH2D *hBrlECalTrkPtYZ = (TH2D*)infile->Get("hBrlECalTrkPtYZ");
  // TH2D *hP0DECalTrkPtXY = (TH2D*)infile->Get("hP0DECalTrkPtXY");
  // TH2D *hP0DECalTrkPtXZ = (TH2D*)infile->Get("hP0DECalTrkPtXZ");
  // TH2D *hP0DECalTrkPtYZ = (TH2D*)infile->Get("hP0DECalTrkPtYZ");  
  // TH2D *hUSECalP0DTrkPtXY = (TH2D*)infile->Get("hUSECalP0DTrkPtXY");
  // TH2D *hUSECalP0DTrkPtXZ = (TH2D*)infile->Get("hUSECalP0DTrkPtXZ");
  // TH2D *hUSECalP0DTrkPtYZ = (TH2D*)infile->Get("hUSECalP0DTrkPtYZ");
  // //
  // TH2D *hVtxOut_XY = (TH2D*)infile->Get("hVtxOut_XY");
  // TH2D *hVtxOut_XZ = (TH2D*)infile->Get("hVtxOut_XZ");
  // TH2D *hVtxOut_YZ = (TH2D*)infile->Get("hVtxOut_YZ");
  // TH1D *hVtxOut_X = (TH1D*)infile->Get("hVtxOut_X");
  // TH1D *hVtxOut_Y = (TH1D*)infile->Get("hVtxOut_Y");
  // TH1D *hVtxOut_Z = (TH1D*)infile->Get("hVtxOut_Z");
  // TH2D *hVtx_XY = (TH2D*)infile->Get("hVtx_XY");
  // TH2D *hVtx_XZ = (TH2D*)infile->Get("hVtx_XZ");
  // TH2D *hVtx_YZ = (TH2D*)infile->Get("hVtx_YZ");
  // TH1D *hVtx_X = (TH1D*)infile->Get("hVtx_X");
  // TH1D *hVtx_Y = (TH1D*)infile->Get("hVtx_Y");
  // TH1D *hVtx_Z = (TH1D*)infile->Get("hVtx_Z");
  //
  //TH1D *hCut = (TH1D*)infile->Get("hCut");

  
  /*

  // Print # of events

  cout << endl;
  cout << "Total # of events: " << hCut->GetBinContent(1) << endl;
  cout << hNuMom->GetEntries() << endl;
  cout << "Total # of vertices in FV: " << hVtx_X->Integral() << endl;
  cout << "Cuts:" << endl;
  for(int icut=0;icut<hCut->GetNbinsX();icut++){
    if(hCut->GetBinContent(icut+1) > 0.){
      cout << hCut->GetXaxis()->GetBinLabel(icut+1) 
	   << " --> " << hCut->GetBinContent(icut+1) << " events" 
	   << endl;
    }
  }
  cout << endl;

  
  //
  // Draw 
  //
  
  TCanvas *cVtx_XY = new TCanvas("cVtx_XY","cVtx_XY");
  hVtx_XY->GetXaxis()->SetTitle("X position (mm)");
  hVtx_XY->GetYaxis()->SetTitle("Y position (mm)");
  hVtx_XY->SetStats(0);
  hVtx_XY->DrawClone("colz");
  if(doPrint) cVtx_XY->Print(TString::Format("%s_Vtx_XY.pdf",tag.c_str()).Data());
  TCanvas *cVtx_XZ = new TCanvas("cVtx_XZ","cVtx_XZ");
  hVtx_XZ->GetXaxis()->SetTitle("X position (mm)");
  hVtx_XZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtx_XZ->SetStats(0);
  hVtx_XZ->DrawClone("colz");
  if(doPrint) cVtx_XZ->Print(TString::Format("%s_Vtx_XZ.pdf",tag.c_str()).Data());
  TCanvas *cVtx_YZ = new TCanvas("cVtx_YZ","cVtx_YZ");
  hVtx_YZ->GetXaxis()->SetTitle("Y position (mm)");
  hVtx_YZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtx_YZ->SetStats(0);
  hVtx_YZ->DrawClone("colz");
  if(doPrint) cVtx_YZ->Print(TString::Format("%s_Vtx_YZ.pdf",tag.c_str()).Data());
  TCanvas *cVtxOut_XY = new TCanvas("cVtxOut_XY","cVtxOut_XY");
  hVtxOut_XY->GetXaxis()->SetTitle("X position (mm)");
  hVtxOut_XY->GetYaxis()->SetTitle("Y position (mm)");
  hVtxOut_XY->SetStats(0);
  hVtxOut_XY->DrawClone("colz");
  if(doPrint) cVtxOut_XY->Print(TString::Format("%s_VtxOut_XY.pdf",tag.c_str()).Data());
  TCanvas *cVtxOut_XZ = new TCanvas("cVtxOut_XZ","cVtxOut_XZ");
  hVtxOut_XZ->GetXaxis()->SetTitle("X position (mm)");
  hVtxOut_XZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtxOut_XZ->SetStats(0);
  hVtxOut_XZ->DrawClone("colz");
  if(doPrint) cVtxOut_XZ->Print(TString::Format("%s_VtxOut_XZ.pdf",tag.c_str()).Data());
  TCanvas *cVtxOut_YZ = new TCanvas("cVtxOut_YZ","cVtxOut_YZ");
  hVtxOut_YZ->GetXaxis()->SetTitle("Y position (mm)");
  hVtxOut_YZ->GetYaxis()->SetTitle("Z position (mm)");
  hVtxOut_YZ->SetStats(0);
  hVtxOut_YZ->DrawClone("colz");
  if(doPrint) cVtxOut_YZ->Print(TString::Format("%s_VtxOut_YZ.pdf",tag.c_str()).Data());
  TCanvas *cVtx_X = new TCanvas("cVtx_X","cVtx_X");
  hVtx_X->GetXaxis()->SetTitle("X position (mm)");
  hVtx_X->SetStats(0);
  hVtx_X->DrawClone("");
  if(doPrint) cVtx_X->Print(TString::Format("%s_Vtx_X.pdf",tag.c_str()).Data());
  TCanvas *cVtx_Y = new TCanvas("cVtx_Y","cVtx_Y");
  hVtx_Y->GetXaxis()->SetTitle("Y position (mm)");
  hVtx_Y->SetStats(0);
  hVtx_Y->DrawClone("");
  if(doPrint) cVtx_Y->Print(TString::Format("%s_Vtx_Y.pdf",tag.c_str()).Data());
  TCanvas *cVtx_Z = new TCanvas("cVtx_Z","cVtx_Z");
  hVtx_Z->GetXaxis()->SetTitle("Z position (mm)");
  hVtx_Z->SetStats(0);
  hVtx_Z->DrawClone("");
  if(doPrint) cVtx_Z->Print(TString::Format("%s_Vtx_Z.pdf",tag.c_str()).Data());
  TCanvas *cVtxOut_X = new TCanvas("cVtxOut_X","cVtxOut_X");
  hVtxOut_X->GetXaxis()->SetTitle("X position (mm)");
  hVtxOut_X->SetStats(0);
  hVtxOut_X->DrawClone("");
  if(doPrint) cVtxOut_X->Print(TString::Format("%s_VtxOut_X.pdf",tag.c_str()).Data());
  TCanvas *cVtxOut_Y = new TCanvas("cVtxOut_Y","cVtxOut_Y");
  hVtxOut_Y->GetXaxis()->SetTitle("Y position (mm)");
  hVtxOut_Y->SetStats(0);
  hVtxOut_Y->DrawClone("");
  if(doPrint) cVtxOut_Y->Print(TString::Format("%s_VtxOut_Y.pdf",tag.c_str()).Data());
  TCanvas *cVtxOut_Z = new TCanvas("cVtxOut_Z","cVtxOut_Z");
  hVtxOut_Z->GetXaxis()->SetTitle("Z position (mm)");
  hVtxOut_Z->SetStats(0);
  hVtxOut_Z->DrawClone("");
  if(doPrint) cVtxOut_Z->Print(TString::Format("%s_VtxOut_Z.pdf",tag.c_str()).Data());

  TCanvas *cCut = new TCanvas("cCut","cCut");  
  hCut->SetStats(0);
  hCut->DrawClone();
  if(doPrint) cCut->Print(TString::Format("%s_Cut.pdf",tag.c_str()).Data());

  */

  return;
}
