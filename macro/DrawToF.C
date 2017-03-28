                                                                                    
void DrawToF
( 
 TString tag = "dt0.2ns",
 bool plotStore = true,
 bool plotLogY = true,
 bool doNormTo1 = false
  )
{
  gROOT->SetStyle("T2K");                  
  gROOT->ForceStyle();                 
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadRightMargin(0.15);
  //gStyle->SetStatX(.4); gStyle->SetStatY(.6);

  // legend
  double legXmin = 0.6;
  double legXmax = 0.8;
  double legYmin = 0.6;
  double legYmax = 0.85;
  // title of histograms
  string titleTimeX = "ns";
  string titleTimeY = "p.d.f.";
  string titleMassX = "Mass (MeV/c^{2})";
  string titleMassY = "p.d.f.";

  const int Nfiles = 4;
  string infilename[Nfiles];
  string label[Nfiles];
  int color[Nfiles];
  int rebin[Nfiles];

  //infilename[0] = "../../../files/pid_ToF/ToFND280Up_Ref_numu_1E21-Targ1-MuM-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.05ns_All.root";
  //label[0] = "#mu^{-} - #DeltaT=50ps";
  //infilename[0] = "../../../files/pid_ToF/ToFND280Up_Ref_numu_1E21-Targ1-MuM-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.1ns_All.root";
  //label[0] = "#mu^{-} - #DeltaT=100ps";
  infilename[0] = "../../../files/pid_ToF/ToFND280Up_Ref_numu_1E21-Targ1-MuM-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.2ns_All.root";
  label[0] = "#mu^{-} - #DeltaT=200ps";  
  color[0] = kViolet;
  rebin[0] = 1;
   
  //infilename[1] = "../../../files/pid_ToF/ToFND280Up_Ref_numu_1E21-Targ1-PiP-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.05ns_All.root";
  //label[1] = "#pi^{+} - #DeltaT=50ps";
  //infilename[1] = "../../../files/pid_ToF/ToFND280Up_Ref_numu_1E21-Targ1-PiP-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.1ns_All.root";
  //label[1] = "#pi^{+} - #DeltaT=100ps";
  infilename[1] = "../../../files/pid_ToF/ToFND280Up_Ref_numu_1E21-Targ1-PiP-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.2ns_All.root";
  label[1] = "#pi^{+} - #DeltaT=200ps";
  color[1] = kGreen;
  rebin[1] = 1;
   
  //infilename[2] = "../../../files/pid_ToF/ToFND280Up_Ref_numu_1E21-Targ1-Prot-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.05ns_All.root";
  //label[2] = "p - #DeltaT=50ps";
  //infilename[2] = "../../../files/pid_ToF/ToFND280Up_Ref_numu_1E21-Targ1-Prot-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.1ns_All.root";
  //label[2] = "p - #DeltaT=100ps";
  infilename[2] = "../../../files/pid_ToF/ToFND280Up_Ref_numu_1E21-Targ1-Prot-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.2ns_All.root";
  label[2] = "p - #DeltaT=200ps";
  color[2] = kBlue;
  rebin[2] = 2;
 
  //infilename[3] = "../../../files/pid_ToF/ToFND280Up_Ref_nue_1E21-Targ1-EleM-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.05ns_Evt0_NEvt10000.root";
  //label[3] = "e^{-} - #DeltaT=50ps";
  //infilename[3] = "../../../files/pid_ToF/ToFND280Up_Ref_nue_1E21-Targ1-EleM-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.1ns_Evt0_NEvt10000.root";
  //label[3] = "e^{-} - #DeltaT=100ps";
  infilename[3] = "../../../files/pid_ToF/ToFND280Up_Ref_nue_1E21-Targ1-EleM-ParID0-CC-SmearToF-dp0.1%-dl10mm-dt0.2ns_Evt0_NEvt10000.root";
  label[3] = "e^{-} - #DeltaT=200ps";
  color[3] = kRed+1;
  rebin[3] = 2;
  

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
    hTimeTarg1_DsECal[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_DsECal[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());

    hTimeTarg1_BrlECal[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_BrlECal");
    hTimeTarg1_BrlECal[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_BrlECal[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());

    hTimeTarg1_P0DECal[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_P0DECal"); 
    hTimeTarg1_P0DECal[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_P0DECal[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_USECalP0D[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_USECalP0D"); 
    hTimeTarg1_USECalP0D[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_USECalP0D[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ForwTPC1[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ForwTPC1"); 
    hTimeTarg1_ForwTPC1[ifile] ->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ForwTPC1[ifile] ->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ForwTPC2[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ForwTPC2");
    hTimeTarg1_ForwTPC2[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ForwTPC2[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ForwTPC3[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ForwTPC3"); 
    hTimeTarg1_ForwTPC3[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ForwTPC3[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_Target1[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_Target1"); 
    hTimeTarg1_Target1[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_Target1[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_Target2[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_Target2");    
    hTimeTarg1_Target2[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_Target2[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_TPCUp1[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_TPCUp1"); 
    hTimeTarg1_TPCUp1[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_TPCUp1[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_TPCUp2[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_TPCUp2"); 
    hTimeTarg1_TPCUp2[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_TPCUp2[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_TPCDown1[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_TPCDown1"); 
    hTimeTarg1_TPCDown1[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_TPCDown1[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_TPCDown2[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_TPCDown2"); 
    hTimeTarg1_TPCDown2[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_TPCDown2[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFTopUp[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFTopUp"); 
    hTimeTarg1_ToFTopUp[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFTopUp[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFBotUp[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFBotUp"); 
    hTimeTarg1_ToFBotUp[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFBotUp[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFRightUp[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFRightUp"); 
    hTimeTarg1_ToFRightUp[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFRightUp[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFLeftUp[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFLeftUp"); 
    hTimeTarg1_ToFLeftUp[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFLeftUp[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFBackUp[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFBackUp"); 
    hTimeTarg1_ToFBackUp[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFBackUp[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFFrontUp[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFFrontUp"); 
    hTimeTarg1_ToFFrontUp[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFFrontUp[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFTopDown[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFTopDown"); 
    hTimeTarg1_ToFTopDown[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFTopDown[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFBotDown[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFBotDown"); 
    hTimeTarg1_ToFBotDown[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFBotDown[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFRightDown[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFRightDown"); 
    hTimeTarg1_ToFRightDown[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFRightDown[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFLeftDown[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFLeftDown");
    hTimeTarg1_ToFLeftDown[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFLeftDown[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFBackDown[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFBackDown"); 
    hTimeTarg1_ToFBackDown[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFBackDown[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFFrontDown[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFFrontDown"); 
    hTimeTarg1_ToFFrontDown[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFFrontDown[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    hTimeTarg1_ToFECalP0D[ifile] = (TH1D*)infile[ifile]->Get("hTimeTarg1_ToFECalP0D"); 
    hTimeTarg1_ToFECalP0D[ifile]->GetXaxis()->SetTitle(titleTimeX.c_str()); hTimeTarg1_ToFECalP0D[ifile]->GetYaxis()->SetTitle(titleTimeY.c_str());
    
    //
    hMassTarg1_DsECal[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_DsECal"); hMassTarg1_DsECal[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_DsECal[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_DsECal[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_BrlECal[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_BrlECal"); hMassTarg1_BrlECal[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_BrlECal[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_BrlECal[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_P0DECal[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_P0DECal"); hMassTarg1_P0DECal[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_P0DECal[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_P0DECal[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_USECalP0D[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_USECalP0D"); hMassTarg1_USECalP0D[ifile]->Rebin(rebin[ifile]); 
    hMassTarg1_USECalP0D[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_USECalP0D[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ForwTPC1[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ForwTPC1"); hMassTarg1_ForwTPC1[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_ForwTPC1[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ForwTPC1[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ForwTPC2[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ForwTPC2"); hMassTarg1_ForwTPC2[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_ForwTPC2[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ForwTPC2[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ForwTPC3[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ForwTPC3"); hMassTarg1_ForwTPC3[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_ForwTPC3[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ForwTPC3[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_Target1[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_Target1"); hMassTarg1_Target1[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_Target1[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_Target1[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_Target2[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_Target2"); hMassTarg1_Target2[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_Target2[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_Target2[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_TPCUp1[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_TPCUp1"); hMassTarg1_TPCUp1[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_TPCUp1[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_TPCUp1[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_TPCUp2[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_TPCUp2"); hMassTarg1_TPCUp2[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_TPCUp2[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_TPCUp2[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_TPCDown1[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_TPCDown1"); hMassTarg1_TPCDown1[ifile]->Rebin(rebin[ifile]); 
    hMassTarg1_TPCDown1[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_TPCDown1[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_TPCDown2[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_TPCDown2"); hMassTarg1_TPCDown2[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_TPCDown2[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_TPCDown2[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFTopUp[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFTopUp"); hMassTarg1_ToFTopUp[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_ToFTopUp[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFTopUp[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFBotUp[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFBotUp"); hMassTarg1_ToFBotUp[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_ToFBotUp[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFBotUp[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFRightUp[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFRightUp"); hMassTarg1_ToFRightUp[ifile]->Rebin(rebin[ifile]); 
    hMassTarg1_ToFRightUp[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFRightUp[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFLeftUp[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFLeftUp"); hMassTarg1_ToFLeftUp[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_ToFLeftUp[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFLeftUp[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFBackUp[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFBackUp"); hMassTarg1_ToFBackUp[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_ToFBackUp[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFBackUp[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFFrontUp[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFFrontUp"); hMassTarg1_ToFFrontUp[ifile]->Rebin(rebin[ifile]); 
    hMassTarg1_ToFFrontUp[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFFrontUp[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFTopDown[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFTopDown"); hMassTarg1_ToFTopDown[ifile]->Rebin(rebin[ifile]); 
    hMassTarg1_ToFTopDown[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFTopDown[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFBotDown[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFBotDown"); hMassTarg1_ToFBotDown[ifile]->Rebin(rebin[ifile]); 
    hMassTarg1_ToFBotDown[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFBotDown[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFRightDown[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFRightDown"); hMassTarg1_ToFRightDown[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_ToFRightDown[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFRightDown[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFLeftDown[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFLeftDown"); hMassTarg1_ToFLeftDown[ifile]->Rebin(rebin[ifile]); 
    hMassTarg1_ToFLeftDown[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFLeftDown[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFBackDown[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFBackDown"); hMassTarg1_ToFBackDown[ifile]->Rebin(rebin[ifile]); 
    hMassTarg1_ToFBackDown[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFBackDown[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFFrontDown[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFFrontDown"); hMassTarg1_ToFFrontDown[ifile]->Rebin(rebin[ifile]); 
    hMassTarg1_ToFFrontDown[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFFrontDown[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    hMassTarg1_ToFECalP0D[ifile] = (TH1D*)infile[ifile]->Get("hMassTarg1_ToFECalP0D"); hMassTarg1_ToFECalP0D[ifile]->Rebin(rebin[ifile]);
    hMassTarg1_ToFECalP0D[ifile]->GetXaxis()->SetTitle(titleMassX.c_str()); hMassTarg1_ToFECalP0D[ifile]->GetYaxis()->SetTitle(titleMassY.c_str());
    
    //
    hMassTarg1_ToFAll[ifile] = (TH1D*)hMassTarg1_DsECal[ifile]->Clone("hMassTarg1_ToFAll"); 
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_P0DECal[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_BrlECal[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_USECalP0D[ifile]); 
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ForwTPC1[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ForwTPC2[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ForwTPC3[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_Target1[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_TPCUp1[ifile]);
    hMassTarg1_ToFAll[ifile]->Add( hMassTarg1_Target2[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_TPCUp2[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFECalP0D[ifile]);   
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFFrontDown[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFBackDown[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFLeftDown[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFRightDown[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFBotDown[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFTopDown[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFFrontUp[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFBackUp[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFLeftUp[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFRightUp[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFBotUp[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_ToFTopUp[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_TPCDown2[ifile]);
    hMassTarg1_ToFAll[ifile]->Add(hMassTarg1_TPCDown1[ifile]);
    hMassTarg1_ToFAll[ifile]->SetTitle("hMassTarg1_ToFAll");


    if(doNormTo1){
      hMassTarg1_DsECal[ifile]->Scale(1./hMassTarg1_DsECal[ifile]->Integral());  
      hMassTarg1_BrlECal[ifile]->Scale(1./hMassTarg1_BrlECal[ifile]->Integral());
      hMassTarg1_P0DECal[ifile]->Scale(1./hMassTarg1_P0DECal[ifile]->Integral());
      hMassTarg1_USECalP0D[ifile]->Scale(1./hMassTarg1_USECalP0D[ifile]->Integral());
      hMassTarg1_ForwTPC1[ifile]->Scale(1./hMassTarg1_ForwTPC1[ifile]->Integral());
      hMassTarg1_ForwTPC2[ifile]->Scale(1./hMassTarg1_ForwTPC2[ifile]->Integral());
      hMassTarg1_ForwTPC3[ifile]->Scale(1./hMassTarg1_ForwTPC3[ifile]->Integral());
      hMassTarg1_Target1[ifile]->Scale(1./hMassTarg1_Target1[ifile]->Integral());
      hMassTarg1_Target2[ifile]->Scale(1./hMassTarg1_Target2[ifile]->Integral());
      hMassTarg1_TPCUp1[ifile]->Scale(1./hMassTarg1_TPCUp1[ifile]->Integral());
      hMassTarg1_TPCUp2[ifile]->Scale(1./hMassTarg1_TPCUp2[ifile]->Integral());
      hMassTarg1_TPCDown1[ifile]->Scale(1./hMassTarg1_TPCDown1[ifile]->Integral());      
      hMassTarg1_TPCDown2[ifile]->Scale(1./hMassTarg1_TPCDown2[ifile]->Integral());
      hMassTarg1_ToFTopUp[ifile]->Scale(1./hMassTarg1_ToFTopUp[ifile]->Integral());
      hMassTarg1_ToFBotUp[ifile]->Scale(1./hMassTarg1_ToFBotUp[ifile]->Integral());
      hMassTarg1_ToFRightUp[ifile]->Scale(1./hMassTarg1_ToFRightUp[ifile]->Integral());
      hMassTarg1_ToFLeftUp[ifile]->Scale(1./hMassTarg1_ToFLeftUp[ifile]->Integral());
      hMassTarg1_ToFBackUp[ifile]->Scale(1./hMassTarg1_ToFBackUp[ifile]->Integral());
      hMassTarg1_ToFFrontUp[ifile]->Scale(1./hMassTarg1_ToFFrontUp[ifile]->Integral());
      hMassTarg1_ToFTopDown[ifile]->Scale(1./hMassTarg1_ToFTopDown[ifile]->Integral());
      hMassTarg1_ToFBotDown[ifile]->Scale(1./hMassTarg1_ToFBotDown[ifile]->Integral());
      hMassTarg1_ToFRightDown[ifile]->Scale(1./hMassTarg1_ToFRightDown[ifile]->Integral());
      hMassTarg1_ToFLeftDown[ifile]->Scale(1./hMassTarg1_ToFLeftDown[ifile]->Integral());
      hMassTarg1_ToFBackDown[ifile]->Scale(1./hMassTarg1_ToFBackDown[ifile]->Integral());
      hMassTarg1_ToFFrontDown[ifile]->Scale(1./hMassTarg1_ToFFrontDown[ifile]->Integral());
      hMassTarg1_ToFECalP0D[ifile]->Scale(1./hMassTarg1_ToFECalP0D[ifile]->Integral());
      hMassTarg1_ToFAll[ifile]->Scale(1./hMassTarg1_ToFAll[ifile]->Integral());  
    }
    
  }
    
  
  
  // Time of Flight

  TCanvas *cTimeTarg1_DsECal = new TCanvas("cTimeTarg1_DsECal","cTimeTarg1_DsECal");  
  if(plotLogY) cTimeTarg1_DsECal->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_DsECal[ifile]->Draw();
    else        hTimeTarg1_DsECal[ifile]->Draw("same");
    hTimeTarg1_DsECal[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_DsECal[ifile]->SetLineWidth(2);
    hTimeTarg1_DsECal[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_DsECal = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_DsECal->SetFillColor(0);
  lTimeTarg1_DsECal->SetLineColor(0);
  lTimeTarg1_DsECal->SetTextFont(132);
  lTimeTarg1_DsECal->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_DsECal->AddEntry(hTimeTarg1_DsECal[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_DsECal->Draw();
  string name = TString::Format("cTimeTarg1_DsECal_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_DsECal->Print(name.c_str());
  
  TCanvas *cTimeTarg1_BrlECal = new TCanvas("cTimeTarg1_BrlECal","cTimeTarg1_BrlECal");  
  if(plotLogY) cTimeTarg1_BrlECal->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_BrlECal[ifile]->Draw();
    else        hTimeTarg1_BrlECal[ifile]->Draw("same");
    hTimeTarg1_BrlECal[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_BrlECal[ifile]->SetLineWidth(2);
    hTimeTarg1_BrlECal[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_BrlECal = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_BrlECal->SetFillColor(0);
  lTimeTarg1_BrlECal->SetLineColor(0);
  lTimeTarg1_BrlECal->SetTextFont(132);
  lTimeTarg1_BrlECal->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_BrlECal->AddEntry(hTimeTarg1_BrlECal[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_BrlECal->Draw();
  string name = TString::Format("cTimeTarg1_BrlECal_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_BrlECal->Print(name.c_str());

  TCanvas *cTimeTarg1_P0DECal = new TCanvas("cTimeTarg1_P0DECal","cTimeTarg1_P0DECal");  
  if(plotLogY) cTimeTarg1_P0DECal->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_P0DECal[ifile]->Draw();
    else        hTimeTarg1_P0DECal[ifile]->Draw("same");
    hTimeTarg1_P0DECal[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_P0DECal[ifile]->SetLineWidth(2);
    hTimeTarg1_P0DECal[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_P0DECal = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_P0DECal->SetFillColor(0);
  lTimeTarg1_P0DECal->SetLineColor(0);
  lTimeTarg1_P0DECal->SetTextFont(132);
  lTimeTarg1_P0DECal->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_P0DECal->AddEntry(hTimeTarg1_P0DECal[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_P0DECal->Draw();
  string name = TString::Format("cTimeTarg1_P0DECal_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_P0DECal->Print(name.c_str());

  TCanvas *cTimeTarg1_USECalP0D = new TCanvas("cTimeTarg1_USECalP0D","cTimeTarg1_USECalP0D");  
  if(plotLogY) cTimeTarg1_USECalP0D->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_USECalP0D[ifile]->Draw();
    else        hTimeTarg1_USECalP0D[ifile]->Draw("same");
    hTimeTarg1_USECalP0D[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_USECalP0D[ifile]->SetLineWidth(2);
    hTimeTarg1_USECalP0D[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_USECalP0D = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_USECalP0D->SetFillColor(0);
  lTimeTarg1_USECalP0D->SetLineColor(0);
  lTimeTarg1_USECalP0D->SetTextFont(132);
  lTimeTarg1_USECalP0D->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_USECalP0D->AddEntry(hTimeTarg1_USECalP0D[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_USECalP0D->Draw();
  string name = TString::Format("cTimeTarg1_USECalP0D_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_USECalP0D->Print(name.c_str());

  TCanvas *cTimeTarg1_ForwTPC1 = new TCanvas("cTimeTarg1_ForwTPC1","cTimeTarg1_ForwTPC1");  
  if(plotLogY) cTimeTarg1_ForwTPC1->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ForwTPC1[ifile]->Draw();
    else        hTimeTarg1_ForwTPC1[ifile]->Draw("same");
    hTimeTarg1_ForwTPC1[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ForwTPC1[ifile]->SetLineWidth(2);
    hTimeTarg1_ForwTPC1[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ForwTPC1 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ForwTPC1->SetFillColor(0);
  lTimeTarg1_ForwTPC1->SetLineColor(0);
  lTimeTarg1_ForwTPC1->SetTextFont(132);
  lTimeTarg1_ForwTPC1->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ForwTPC1->AddEntry(hTimeTarg1_ForwTPC1[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ForwTPC1->Draw();
  string name = TString::Format("cTimeTarg1_ForwTPC1_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ForwTPC1->Print(name.c_str());

  TCanvas *cTimeTarg1_ForwTPC2 = new TCanvas("cTimeTarg1_ForwTPC2","cTimeTarg1_ForwTPC2");  
  if(plotLogY) cTimeTarg1_ForwTPC2->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ForwTPC2[ifile]->Draw();
    else        hTimeTarg1_ForwTPC2[ifile]->Draw("same");
    hTimeTarg1_ForwTPC2[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ForwTPC2[ifile]->SetLineWidth(2);
    hTimeTarg1_ForwTPC2[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ForwTPC2 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ForwTPC2->SetFillColor(0);
  lTimeTarg1_ForwTPC2->SetLineColor(0);
  lTimeTarg1_ForwTPC2->SetTextFont(132);
  lTimeTarg1_ForwTPC2->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ForwTPC2->AddEntry(hTimeTarg1_ForwTPC2[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ForwTPC2->Draw();
  string name = TString::Format("cTimeTarg1_ForwTPC2_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ForwTPC2->Print(name.c_str());


  TCanvas *cTimeTarg1_ForwTPC3 = new TCanvas("cTimeTarg1_ForwTPC3","cTimeTarg1_ForwTPC3");  
  if(plotLogY) cTimeTarg1_ForwTPC3->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ForwTPC3[ifile]->Draw();
    else        hTimeTarg1_ForwTPC3[ifile]->Draw("same");
    hTimeTarg1_ForwTPC3[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ForwTPC3[ifile]->SetLineWidth(2);
    hTimeTarg1_ForwTPC3[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ForwTPC3 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ForwTPC3->SetFillColor(0);
  lTimeTarg1_ForwTPC3->SetLineColor(0);
  lTimeTarg1_ForwTPC3->SetTextFont(132);
  lTimeTarg1_ForwTPC3->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ForwTPC3->AddEntry(hTimeTarg1_ForwTPC3[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ForwTPC3->Draw();
  string name = TString::Format("cTimeTarg1_ForwTPC3_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ForwTPC3->Print(name.c_str());

  TCanvas *cTimeTarg1_Target1 = new TCanvas("cTimeTarg1_Target1","cTimeTarg1_Target1");  
  if(plotLogY) cTimeTarg1_Target1->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_Target1[ifile]->Draw();
    else        hTimeTarg1_Target1[ifile]->Draw("same");
    hTimeTarg1_Target1[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_Target1[ifile]->SetLineWidth(2);
    hTimeTarg1_Target1[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_Target1 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_Target1->SetFillColor(0);
  lTimeTarg1_Target1->SetLineColor(0);
  lTimeTarg1_Target1->SetTextFont(132);
  lTimeTarg1_Target1->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_Target1->AddEntry(hTimeTarg1_Target1[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_Target1->Draw();
  string name = TString::Format("cTimeTarg1_Target1_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_Target1->Print(name.c_str());

  TCanvas *cTimeTarg1_Target2 = new TCanvas("cTimeTarg1_Target2","cTimeTarg1_Target2");  
  if(plotLogY) cTimeTarg1_Target2->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_Target2[ifile]->Draw();
    else        hTimeTarg1_Target2[ifile]->Draw("same");
    hTimeTarg1_Target2[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_Target2[ifile]->SetLineWidth(2);
    hTimeTarg1_Target2[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_Target2 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_Target2->SetFillColor(0);
  lTimeTarg1_Target2->SetLineColor(0);
  lTimeTarg1_Target2->SetTextFont(132);
  lTimeTarg1_Target2->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_Target2->AddEntry(hTimeTarg1_Target2[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_Target2->Draw();
  string name = TString::Format("cTimeTarg1_Target2_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_Target2->Print(name.c_str());

  TCanvas *cTimeTarg1_TPCUp1 = new TCanvas("cTimeTarg1_TPCUp1","cTimeTarg1_TPCUp1");  
  if(plotLogY) cTimeTarg1_TPCUp1->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_TPCUp1[ifile]->Draw();
    else        hTimeTarg1_TPCUp1[ifile]->Draw("same");
    hTimeTarg1_TPCUp1[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_TPCUp1[ifile]->SetLineWidth(2);
    hTimeTarg1_TPCUp1[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_TPCUp1 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_TPCUp1->SetFillColor(0);
  lTimeTarg1_TPCUp1->SetLineColor(0);
  lTimeTarg1_TPCUp1->SetTextFont(132);
  lTimeTarg1_TPCUp1->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_TPCUp1->AddEntry(hTimeTarg1_TPCUp1[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_TPCUp1->Draw();
  string name = TString::Format("cTimeTarg1_TPCUp1_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_TPCUp1->Print(name.c_str());

  TCanvas *cTimeTarg1_TPCUp2 = new TCanvas("cTimeTarg1_TPCUp2","cTimeTarg1_TPCUp2");  
  if(plotLogY) cTimeTarg1_TPCUp2->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_TPCUp2[ifile]->Draw();
    else        hTimeTarg1_TPCUp2[ifile]->Draw("same");
    hTimeTarg1_TPCUp2[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_TPCUp2[ifile]->SetLineWidth(2);
    hTimeTarg1_TPCUp2[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_TPCUp2 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_TPCUp2->SetFillColor(0);
  lTimeTarg1_TPCUp2->SetLineColor(0);
  lTimeTarg1_TPCUp2->SetTextFont(132);
  lTimeTarg1_TPCUp2->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_TPCUp2->AddEntry(hTimeTarg1_TPCUp2[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_TPCUp2->Draw();
  string name = TString::Format("cTimeTarg1_TPCUp2_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_TPCUp2->Print(name.c_str());

  TCanvas *cTimeTarg1_TPCDown1 = new TCanvas("cTimeTarg1_TPCDown1","cTimeTarg1_TPCDown1");  
  if(plotLogY) cTimeTarg1_TPCDown1->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_TPCDown1[ifile]->Draw();
    else        hTimeTarg1_TPCDown1[ifile]->Draw("same");
    hTimeTarg1_TPCDown1[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_TPCDown1[ifile]->SetLineWidth(2);
    hTimeTarg1_TPCDown1[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_TPCDown1 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_TPCDown1->SetFillColor(0);
  lTimeTarg1_TPCDown1->SetLineColor(0);
  lTimeTarg1_TPCDown1->SetTextFont(132);
  lTimeTarg1_TPCDown1->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_TPCDown1->AddEntry(hTimeTarg1_TPCDown1[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_TPCDown1->Draw();
  string name = TString::Format("cTimeTarg1_TPCDown1_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_TPCDown1->Print(name.c_str());

  TCanvas *cTimeTarg1_TPCDown2 = new TCanvas("cTimeTarg1_TPCDown2","cTimeTarg1_TPCDown2");  
  if(plotLogY) cTimeTarg1_TPCDown2->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_TPCDown2[ifile]->Draw();
    else        hTimeTarg1_TPCDown2[ifile]->Draw("same");
    hTimeTarg1_TPCDown2[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_TPCDown2[ifile]->SetLineWidth(2);
    hTimeTarg1_TPCDown2[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_TPCDown2 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_TPCDown2->SetFillColor(0);
  lTimeTarg1_TPCDown2->SetLineColor(0);
  lTimeTarg1_TPCDown2->SetTextFont(132);
  lTimeTarg1_TPCDown2->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_TPCDown2->AddEntry(hTimeTarg1_TPCDown2[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_TPCDown2->Draw();
  string name = TString::Format("cTimeTarg1_TPCDown2_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_TPCDown2->Print(name.c_str());

  TCanvas *cTimeTarg1_ToFTopUp = new TCanvas("cTimeTarg1_ToFTopUp","cTimeTarg1_ToFTopUp");  
  if(plotLogY) cTimeTarg1_ToFTopUp->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFTopUp[ifile]->Draw();
    else        hTimeTarg1_ToFTopUp[ifile]->Draw("same");
    hTimeTarg1_ToFTopUp[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFTopUp[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFTopUp[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ToFTopUp = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFTopUp->SetFillColor(0);
  lTimeTarg1_ToFTopUp->SetLineColor(0);
  lTimeTarg1_ToFTopUp->SetTextFont(132);
  lTimeTarg1_ToFTopUp->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFTopUp->AddEntry(hTimeTarg1_ToFTopUp[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFTopUp->Draw();
  string name = TString::Format("cTimeTarg1_ToFTopUp_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFTopUp->Print(name.c_str());

  TCanvas *cTimeTarg1_ToFBotUp = new TCanvas("cTimeTarg1_ToFBotUp","cTimeTarg1_ToFBotUp");  
  if(plotLogY) cTimeTarg1_ToFBotUp->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFBotUp[ifile]->Draw();
    else        hTimeTarg1_ToFBotUp[ifile]->Draw("same");
    hTimeTarg1_ToFBotUp[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFBotUp[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFBotUp[ifile]->SetStats(0);
  }  
  TLegend *lTimeTarg1_ToFBotUp = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFBotUp->SetFillColor(0);
  lTimeTarg1_ToFBotUp->SetLineColor(0);
  lTimeTarg1_ToFBotUp->SetTextFont(132);
  lTimeTarg1_ToFBotUp->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFBotUp->AddEntry(hTimeTarg1_ToFBotUp[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFBotUp->Draw();
  string name = TString::Format("cTimeTarg1_ToFBotUp_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFBotUp->Print(name.c_str());

  TCanvas *cTimeTarg1_ToFRightUp = new TCanvas("cTimeTarg1_ToFRightUp","cTimeTarg1_ToFRightUp"); 
  if(plotLogY) cTimeTarg1_ToFRightUp->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFRightUp[ifile]->Draw();
    else        hTimeTarg1_ToFRightUp[ifile]->Draw("same");
    hTimeTarg1_ToFRightUp[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFRightUp[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFRightUp[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ToFRightUp = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFRightUp->SetFillColor(0);
  lTimeTarg1_ToFRightUp->SetLineColor(0);
  lTimeTarg1_ToFRightUp->SetTextFont(132);
  lTimeTarg1_ToFRightUp->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFRightUp->AddEntry(hTimeTarg1_ToFRightUp[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFRightUp->Draw();
  string name = TString::Format("cTimeTarg1_ToFRightUp_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFRightUp->Print(name.c_str());

  TCanvas *cTimeTarg1_ToFLeftUp = new TCanvas("cTimeTarg1_ToFLeftUp","cTimeTarg1_ToFLeftUp");  
  if(plotLogY) cTimeTarg1_ToFLeftUp->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFLeftUp[ifile]->Draw();
    else        hTimeTarg1_ToFLeftUp[ifile]->Draw("same");
    hTimeTarg1_ToFLeftUp[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFLeftUp[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFLeftUp[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ToFLeftUp = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFLeftUp->SetFillColor(0);
  lTimeTarg1_ToFLeftUp->SetLineColor(0);
  lTimeTarg1_ToFLeftUp->SetTextFont(132);
  lTimeTarg1_ToFLeftUp->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFLeftUp->AddEntry(hTimeTarg1_ToFLeftUp[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFLeftUp->Draw();
  string name = TString::Format("cTimeTarg1_ToFLeftUp_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFLeftUp->Print(name.c_str());

  TCanvas *cTimeTarg1_ToFBackUp = new TCanvas("cTimeTarg1_ToFBackUp","cTimeTarg1_ToFBackUp");  
  if(plotLogY) cTimeTarg1_ToFBackUp->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFBackUp[ifile]->Draw();
    else        hTimeTarg1_ToFBackUp[ifile]->Draw("same");
    hTimeTarg1_ToFBackUp[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFBackUp[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFBackUp[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ToFBackUp = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFBackUp->SetFillColor(0);
  lTimeTarg1_ToFBackUp->SetLineColor(0);
  lTimeTarg1_ToFBackUp->SetTextFont(132);
  lTimeTarg1_ToFBackUp->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFBackUp->AddEntry(hTimeTarg1_ToFBackUp[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFBackUp->Draw();
  string name = TString::Format("cTimeTarg1_ToFBackUp_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFBackUp->Print(name.c_str());

  TCanvas *cTimeTarg1_ToFTopDown = new TCanvas("cTimeTarg1_ToFTopDown","cTimeTarg1_ToFTopDown");  
  if(plotLogY) cTimeTarg1_ToFTopDown->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFTopDown[ifile]->Draw();
    else        hTimeTarg1_ToFTopDown[ifile]->Draw("same");
    hTimeTarg1_ToFTopDown[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFTopDown[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFTopDown[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ToFTopDown = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFTopDown->SetFillColor(0);
  lTimeTarg1_ToFTopDown->SetLineColor(0);
  lTimeTarg1_ToFTopDown->SetTextFont(132);
  lTimeTarg1_ToFTopDown->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFTopDown->AddEntry(hTimeTarg1_ToFTopDown[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFTopDown->Draw();
  string name = TString::Format("cTimeTarg1_ToFTopDown_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFTopDown->Print(name.c_str());

  TCanvas *cTimeTarg1_ToFBotDown = new TCanvas("cTimeTarg1_ToFBotDown","cTimeTarg1_ToFBotDown"); 
  if(plotLogY) cTimeTarg1_ToFBotDown->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFBotDown[ifile]->Draw();
    else        hTimeTarg1_ToFBotDown[ifile]->Draw("same");
    hTimeTarg1_ToFBotDown[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFBotDown[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFBotDown[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ToFBotDown = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFBotDown->SetFillColor(0);
  lTimeTarg1_ToFBotDown->SetLineColor(0);
  lTimeTarg1_ToFBotDown->SetTextFont(132);
  lTimeTarg1_ToFBotDown->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFBotDown->AddEntry(hTimeTarg1_ToFBotDown[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFBotDown->Draw();
  string name = TString::Format("cTimeTarg1_ToFBotDown_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFBotDown->Print(name.c_str());


  TCanvas *cTimeTarg1_ToFRightDown = new TCanvas("cTimeTarg1_ToFRightDown","cTimeTarg1_ToFRightDown");  
  if(plotLogY) cTimeTarg1_ToFRightDown->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFRightDown[ifile]->Draw();
    else        hTimeTarg1_ToFRightDown[ifile]->Draw("same");
    hTimeTarg1_ToFRightDown[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFRightDown[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFRightDown[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ToFRightDown = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFRightDown->SetFillColor(0);
  lTimeTarg1_ToFRightDown->SetLineColor(0);
  lTimeTarg1_ToFRightDown->SetTextFont(132);
  lTimeTarg1_ToFRightDown->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFRightDown->AddEntry(hTimeTarg1_ToFRightDown[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFRightDown->Draw();
  string name = TString::Format("cTimeTarg1_ToFRightDown_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFRightDown->Print(name.c_str());

  TCanvas *cTimeTarg1_ToFLeftDown = new TCanvas("cTimeTarg1_ToFLeftDown","cTimeTarg1_ToFLeftDown");  
  if(plotLogY) cTimeTarg1_ToFLeftDown->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFLeftDown[ifile]->Draw();
    else        hTimeTarg1_ToFLeftDown[ifile]->Draw("same");
    hTimeTarg1_ToFLeftDown[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFLeftDown[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFLeftDown[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ToFLeftDown = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFLeftDown->SetFillColor(0);
  lTimeTarg1_ToFLeftDown->SetLineColor(0);
  lTimeTarg1_ToFLeftDown->SetTextFont(132);
  lTimeTarg1_ToFLeftDown->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFLeftDown->AddEntry(hTimeTarg1_ToFLeftDown[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFLeftDown->Draw();
  string name = TString::Format("cTimeTarg1_ToFLeftDown_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFLeftDown->Print(name.c_str());


  TCanvas *cTimeTarg1_ToFBackDown = new TCanvas("cTimeTarg1_ToFBackDown","cTimeTarg1_ToFBackDown");  
  if(plotLogY) cTimeTarg1_ToFBackDown->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFBackDown[ifile]->Draw();
    else        hTimeTarg1_ToFBackDown[ifile]->Draw("same");
    hTimeTarg1_ToFBackDown[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFBackDown[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFBackDown[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ToFBackDown = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFBackDown->SetFillColor(0);
  lTimeTarg1_ToFBackDown->SetLineColor(0);
  lTimeTarg1_ToFBackDown->SetTextFont(132);
  lTimeTarg1_ToFBackDown->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFBackDown->AddEntry(hTimeTarg1_ToFBackDown[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFBackDown->Draw();
  string name = TString::Format("cTimeTarg1_ToFBackDown_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFBackDown->Print(name.c_str());

  TCanvas *cTimeTarg1_ToFFrontDown = new TCanvas("cTimeTarg1_ToFFrontDown","cTimeTarg1_ToFFrontDown");  
  if(plotLogY) cTimeTarg1_ToFFrontDown->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFFrontDown[ifile]->Draw();
    else        hTimeTarg1_ToFFrontDown[ifile]->Draw("same");
    hTimeTarg1_ToFFrontDown[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFFrontDown[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFFrontDown[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ToFFrontDown = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFFrontDown->SetFillColor(0);
  lTimeTarg1_ToFFrontDown->SetLineColor(0);
  lTimeTarg1_ToFFrontDown->SetTextFont(132);
  lTimeTarg1_ToFFrontDown->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFFrontDown->AddEntry(hTimeTarg1_ToFFrontDown[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFFrontDown->Draw();
  string name = TString::Format("cTimeTarg1_ToFFrontDown_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFFrontDown->Print(name.c_str());

  TCanvas *cTimeTarg1_ToFFrontUp = new TCanvas("cTimeTarg1_ToFFrontUp","cTimeTarg1_ToFFrontUp");  
  if(plotLogY) cTimeTarg1_ToFFrontUp->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFFrontUp[ifile]->Draw();
    else        hTimeTarg1_ToFFrontUp[ifile]->Draw("same");
    hTimeTarg1_ToFFrontUp[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFFrontUp[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFFrontUp[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ToFFrontUp = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFFrontUp->SetFillColor(0);
  lTimeTarg1_ToFFrontUp->SetLineColor(0);
  lTimeTarg1_ToFFrontUp->SetTextFont(132);
  lTimeTarg1_ToFFrontUp->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFFrontUp->AddEntry(hTimeTarg1_ToFFrontUp[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFFrontUp->Draw();
  string name = TString::Format("cTimeTarg1_ToFFrontUp_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFFrontUp->Print(name.c_str());

  TCanvas *cTimeTarg1_ToFECalP0D = new TCanvas("cTimeTarg1_ToFECalP0D","cTimeTarg1_ToFECalP0D");  
  if(plotLogY) cTimeTarg1_ToFECalP0D->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hTimeTarg1_ToFECalP0D[ifile]->Draw();
    else        hTimeTarg1_ToFECalP0D[ifile]->Draw("same");
    hTimeTarg1_ToFECalP0D[ifile]->SetLineColor(color[ifile]);
    hTimeTarg1_ToFECalP0D[ifile]->SetLineWidth(2);
    hTimeTarg1_ToFECalP0D[ifile]->SetStats(0);
  }
  TLegend *lTimeTarg1_ToFECalP0D = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lTimeTarg1_ToFECalP0D->SetFillColor(0);
  lTimeTarg1_ToFECalP0D->SetLineColor(0);
  lTimeTarg1_ToFECalP0D->SetTextFont(132);
  lTimeTarg1_ToFECalP0D->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lTimeTarg1_ToFECalP0D->AddEntry(hTimeTarg1_ToFECalP0D[ifile],label[ifile].c_str(),"l");
  }
  lTimeTarg1_ToFECalP0D->Draw();
  string name = TString::Format("cTimeTarg1_ToFECalP0D_%s.pdf",tag.Data()).Data();
  if(plotStore) cTimeTarg1_ToFECalP0D->Print(name.c_str());











  // ToF Mass

  TCanvas *cMassTarg1_DsECal = new TCanvas("cMassTarg1_DsECal","cMassTarg1_DsECal");  
  if(plotLogY) cMassTarg1_DsECal->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_DsECal[ifile]->Draw();
    else        hMassTarg1_DsECal[ifile]->Draw("same");
    hMassTarg1_DsECal[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_DsECal[ifile]->SetLineWidth(2);
    hMassTarg1_DsECal[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_DsECal = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_DsECal->SetFillColor(0);
  lMassTarg1_DsECal->SetLineColor(0);
  lMassTarg1_DsECal->SetTextFont(132);
  lMassTarg1_DsECal->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_DsECal->AddEntry(hMassTarg1_DsECal[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_DsECal->Draw();
  string name = TString::Format("cMassTarg1_DsECal_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_DsECal->Print(name.c_str());

  TCanvas *cMassTarg1_BrlECal = new TCanvas("cMassTarg1_BrlECal","cMassTarg1_BrlECal");  
  if(plotLogY) cMassTarg1_BrlECal->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_BrlECal[ifile]->Draw();
    else        hMassTarg1_BrlECal[ifile]->Draw("same");
    hMassTarg1_BrlECal[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_BrlECal[ifile]->SetLineWidth(2);
    hMassTarg1_BrlECal[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_BrlECal = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_BrlECal->SetFillColor(0);
  lMassTarg1_BrlECal->SetLineColor(0);
  lMassTarg1_BrlECal->SetTextFont(132);
  lMassTarg1_BrlECal->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_BrlECal->AddEntry(hMassTarg1_BrlECal[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_BrlECal->Draw();
  string name = TString::Format("cMassTarg1_BrlECal_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_BrlECal->Print(name.c_str());

  TCanvas *cMassTarg1_P0DECal = new TCanvas("cMassTarg1_P0DECal","cMassTarg1_P0DECal");  
  if(plotLogY) cMassTarg1_P0DECal->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_P0DECal[ifile]->Draw();
    else        hMassTarg1_P0DECal[ifile]->Draw("same");
    hMassTarg1_P0DECal[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_P0DECal[ifile]->SetLineWidth(2);
    hMassTarg1_P0DECal[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_P0DECal = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_P0DECal->SetFillColor(0);
  lMassTarg1_P0DECal->SetLineColor(0);
  lMassTarg1_P0DECal->SetTextFont(132);
  lMassTarg1_P0DECal->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_P0DECal->AddEntry(hMassTarg1_P0DECal[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_P0DECal->Draw();
  string name = TString::Format("cMassTarg1_P0DECal_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_P0DECal->Print(name.c_str());

  TCanvas *cMassTarg1_USECalP0D = new TCanvas("cMassTarg1_USECalP0D","cMassTarg1_USECalP0D");  
  if(plotLogY) cMassTarg1_USECalP0D->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_USECalP0D[ifile]->Draw();
    else        hMassTarg1_USECalP0D[ifile]->Draw("same");
    hMassTarg1_USECalP0D[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_USECalP0D[ifile]->SetLineWidth(2);
    hMassTarg1_USECalP0D[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_USECalP0D = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_USECalP0D->SetFillColor(0);
  lMassTarg1_USECalP0D->SetLineColor(0);
  lMassTarg1_USECalP0D->SetTextFont(132);
  lMassTarg1_USECalP0D->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_USECalP0D->AddEntry(hMassTarg1_USECalP0D[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_USECalP0D->Draw();
  string name = TString::Format("cMassTarg1_USECalP0D_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_USECalP0D->Print(name.c_str());

  TCanvas *cMassTarg1_ForwTPC1 = new TCanvas("cMassTarg1_ForwTPC1","cMassTarg1_ForwTPC1");  
  if(plotLogY) cMassTarg1_ForwTPC1->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ForwTPC1[ifile]->Draw();
    else        hMassTarg1_ForwTPC1[ifile]->Draw("same");
    hMassTarg1_ForwTPC1[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ForwTPC1[ifile]->SetLineWidth(2);
    hMassTarg1_ForwTPC1[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ForwTPC1 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ForwTPC1->SetFillColor(0);
  lMassTarg1_ForwTPC1->SetLineColor(0);
  lMassTarg1_ForwTPC1->SetTextFont(132);
  lMassTarg1_ForwTPC1->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ForwTPC1->AddEntry(hMassTarg1_ForwTPC1[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ForwTPC1->Draw();
  string name = TString::Format("cMassTarg1_ForwTPC1_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ForwTPC1->Print(name.c_str());

  TCanvas *cMassTarg1_ForwTPC2 = new TCanvas("cMassTarg1_ForwTPC2","cMassTarg1_ForwTPC2");  
  if(plotLogY) cMassTarg1_ForwTPC2->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ForwTPC2[ifile]->Draw();
    else        hMassTarg1_ForwTPC2[ifile]->Draw("same");
    hMassTarg1_ForwTPC2[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ForwTPC2[ifile]->SetLineWidth(2);
    hMassTarg1_ForwTPC2[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ForwTPC2 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ForwTPC2->SetFillColor(0);
  lMassTarg1_ForwTPC2->SetLineColor(0);
  lMassTarg1_ForwTPC2->SetTextFont(132);
  lMassTarg1_ForwTPC2->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ForwTPC2->AddEntry(hMassTarg1_ForwTPC2[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ForwTPC2->Draw();
  string name = TString::Format("cMassTarg1_ForwTPC2_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ForwTPC2->Print(name.c_str());

  TCanvas *cMassTarg1_ForwTPC3 = new TCanvas("cMassTarg1_ForwTPC3","cMassTarg1_ForwTPC3");  
  if(plotLogY) cMassTarg1_ForwTPC3->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ForwTPC3[ifile]->Draw();
    else        hMassTarg1_ForwTPC3[ifile]->Draw("same");
    hMassTarg1_ForwTPC3[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ForwTPC3[ifile]->SetLineWidth(2);
    hMassTarg1_ForwTPC3[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ForwTPC3 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ForwTPC3->SetFillColor(0);
  lMassTarg1_ForwTPC3->SetLineColor(0);
  lMassTarg1_ForwTPC3->SetTextFont(132);
  lMassTarg1_ForwTPC3->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ForwTPC3->AddEntry(hMassTarg1_ForwTPC3[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ForwTPC3->Draw();
  string name = TString::Format("cMassTarg1_ForwTPC3_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ForwTPC3->Print(name.c_str());

  // TCanvas *cMassTarg1_Target1 = new TCanvas("cMassTarg1_Target1","cMassTarg1_Target1");  
  // if(plotLogY) cMassTarg1_Target1->SetLogy();
  // for(int ifile=0;ifile<Nfiles;ifile++){
  //   if(ifile==0) hMassTarg1_Target1[ifile]->Draw();
  //   else        hMassTarg1_Target1[ifile]->Draw("same");
  //   hMassTarg1_Target1[ifile]->SetLineColor(color[ifile]);
  //   hMassTarg1_Target1[ifile]->SetLineWidth(2);
  //   hMassTarg1_Target1[ifile]->SetStats(0);
  // }
  // TLegend *lMassTarg1_Target1 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  // lMassTarg1_Target1->SetFillColor(0);
  // lMassTarg1_Target1->SetLineColor(0);
  // lMassTarg1_Target1->SetTextFont(132);
  // lMassTarg1_Target1->SetTextSize(0.06);
  // for(int ifile=0;ifile<Nfiles;ifile++){
  //   lMassTarg1_Target1->AddEntry(hMassTarg1_Target1[ifile],label[ifile].c_str(),"l");
  // }
  // lMassTarg1_Target1->Draw();
  // string name = TString::Format("cMassTarg1_Target1_%s.pdf",tag.Data()).Data();
  // if(plotStore) cMassTarg1_Target1->Print(name.c_str());

  TCanvas *cMassTarg1_Target2 = new TCanvas("cMassTarg1_Target2","cMassTarg1_Target2");  
  if(plotLogY) cMassTarg1_Target2->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_Target2[ifile]->Draw();
    else        hMassTarg1_Target2[ifile]->Draw("same");
    hMassTarg1_Target2[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_Target2[ifile]->SetLineWidth(2);
    hMassTarg1_Target2[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_Target2 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_Target2->SetFillColor(0);
  lMassTarg1_Target2->SetLineColor(0);
  lMassTarg1_Target2->SetTextFont(132);
  lMassTarg1_Target2->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_Target2->AddEntry(hMassTarg1_Target2[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_Target2->Draw();
  string name = TString::Format("cMassTarg1_Target2_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_Target2->Print(name.c_str());

  TCanvas *cMassTarg1_TPCUp1 = new TCanvas("cMassTarg1_TPCUp1","cMassTarg1_TPCUp1");  
  if(plotLogY) cMassTarg1_TPCUp1->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_TPCUp1[ifile]->Draw();
    else        hMassTarg1_TPCUp1[ifile]->Draw("same");
    hMassTarg1_TPCUp1[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_TPCUp1[ifile]->SetLineWidth(2);
    hMassTarg1_TPCUp1[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_TPCUp1 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_TPCUp1->SetFillColor(0);
  lMassTarg1_TPCUp1->SetLineColor(0);
  lMassTarg1_TPCUp1->SetTextFont(132);
  lMassTarg1_TPCUp1->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_TPCUp1->AddEntry(hMassTarg1_TPCUp1[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_TPCUp1->Draw();
  string name = TString::Format("cMassTarg1_TPCUp1_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_TPCUp1->Print(name.c_str());

  TCanvas *cMassTarg1_TPCUp2 = new TCanvas("cMassTarg1_TPCUp2","cMassTarg1_TPCUp2");  
  if(plotLogY) cMassTarg1_TPCUp2->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_TPCUp2[ifile]->Draw();
    else        hMassTarg1_TPCUp2[ifile]->Draw("same");
    hMassTarg1_TPCUp2[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_TPCUp2[ifile]->SetLineWidth(2);
    hMassTarg1_TPCUp2[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_TPCUp2 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_TPCUp2->SetFillColor(0);
  lMassTarg1_TPCUp2->SetLineColor(0);
  lMassTarg1_TPCUp2->SetTextFont(132);
  lMassTarg1_TPCUp2->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_TPCUp2->AddEntry(hMassTarg1_TPCUp2[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_TPCUp2->Draw();
  string name = TString::Format("cMassTarg1_TPCUp2_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_TPCUp2->Print(name.c_str());

  TCanvas *cMassTarg1_TPCDown1 = new TCanvas("cMassTarg1_TPCDown1","cMassTarg1_TPCDown1");  
  if(plotLogY) cMassTarg1_TPCDown1->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_TPCDown1[ifile]->Draw();
    else        hMassTarg1_TPCDown1[ifile]->Draw("same");
    hMassTarg1_TPCDown1[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_TPCDown1[ifile]->SetLineWidth(2);
    hMassTarg1_TPCDown1[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_TPCDown1 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_TPCDown1->SetFillColor(0);
  lMassTarg1_TPCDown1->SetLineColor(0);
  lMassTarg1_TPCDown1->SetTextFont(132);
  lMassTarg1_TPCDown1->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_TPCDown1->AddEntry(hMassTarg1_TPCDown1[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_TPCDown1->Draw();
  string name = TString::Format("cMassTarg1_TPCDown1_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_TPCDown1->Print(name.c_str());

  // TCanvas *cMassTarg1_TPCDown2 = new TCanvas("cMassTarg1_TPCDown2","cMassTarg1_TPCDown2");  
  // if(plotLogY) cMassTarg1_TPCDown2->SetLogy();
  // for(int ifile=0;ifile<Nfiles;ifile++){
  //   if(ifile==0) hMassTarg1_TPCDown2[ifile]->Draw();
  //   else        hMassTarg1_TPCDown2[ifile]->Draw("same");
  //   hMassTarg1_TPCDown2[ifile]->SetLineColor(color[ifile]);
  //   hMassTarg1_TPCDown2[ifile]->SetLineWidth(2);
  //   hMassTarg1_TPCDown2[ifile]->SetStats(0);
  // }
  // TLegend *lMassTarg1_TPCDown2 = new TLegend(legXmin,legYmin,legXmax,legYmax);
  // lMassTarg1_TPCDown2->SetFillColor(0);
  // lMassTarg1_TPCDown2->SetLineColor(0);
  // lMassTarg1_TPCDown2->SetTextFont(132);
  // lMassTarg1_TPCDown2->SetTextSize(0.06);
  // for(int ifile=0;ifile<Nfiles;ifile++){
  //   lMassTarg1_TPCDown2->AddEntry(hMassTarg1_TPCDown2[ifile],label[ifile].c_str(),"l");
  // }
  // lMassTarg1_TPCDown2->Draw();
  // string name = TString::Format("cMassTarg1_TPCDown2_%s.pdf",tag.Data()).Data();
  // if(plotStore) cMassTarg1_TPCDown2->Print(name.c_str());
  
  // return;


  TCanvas *cMassTarg1_ToFTopUp = new TCanvas("cMassTarg1_ToFTopUp","cMassTarg1_ToFTopUp");  
  if(plotLogY) cMassTarg1_ToFTopUp->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFTopUp[ifile]->Draw();
    else        hMassTarg1_ToFTopUp[ifile]->Draw("same");
    hMassTarg1_ToFTopUp[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFTopUp[ifile]->SetLineWidth(2);
    hMassTarg1_ToFTopUp[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFTopUp = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFTopUp->SetFillColor(0);
  lMassTarg1_ToFTopUp->SetLineColor(0);
  lMassTarg1_ToFTopUp->SetTextFont(132);
  lMassTarg1_ToFTopUp->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFTopUp->AddEntry(hMassTarg1_ToFTopUp[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFTopUp->Draw();
  string name = TString::Format("cMassTarg1_ToFTopUp_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFTopUp->Print(name.c_str());

  TCanvas *cMassTarg1_ToFBotUp = new TCanvas("cMassTarg1_ToFBotUp","cMassTarg1_ToFBotUp");  
  if(plotLogY) cMassTarg1_ToFBotUp->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFBotUp[ifile]->Draw();
    else        hMassTarg1_ToFBotUp[ifile]->Draw("same");
    hMassTarg1_ToFBotUp[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFBotUp[ifile]->SetLineWidth(2);
    hMassTarg1_ToFBotUp[ifile]->SetStats(0);
  }  
  TLegend *lMassTarg1_ToFBotUp = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFBotUp->SetFillColor(0);
  lMassTarg1_ToFBotUp->SetLineColor(0);
  lMassTarg1_ToFBotUp->SetTextFont(132);
  lMassTarg1_ToFBotUp->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFBotUp->AddEntry(hMassTarg1_ToFBotUp[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFBotUp->Draw();
  string name = TString::Format("cMassTarg1_ToFBotUp_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFBotUp->Print(name.c_str());

  TCanvas *cMassTarg1_ToFRightUp = new TCanvas("cMassTarg1_ToFRightUp","cMassTarg1_ToFRightUp");  
  if(plotLogY) cMassTarg1_ToFRightUp->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFRightUp[ifile]->Draw();
    else        hMassTarg1_ToFRightUp[ifile]->Draw("same");
    hMassTarg1_ToFRightUp[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFRightUp[ifile]->SetLineWidth(2);
    hMassTarg1_ToFRightUp[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFRightUp = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFRightUp->SetFillColor(0);
  lMassTarg1_ToFRightUp->SetLineColor(0);
  lMassTarg1_ToFRightUp->SetTextFont(132);
  lMassTarg1_ToFRightUp->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFRightUp->AddEntry(hMassTarg1_ToFRightUp[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFRightUp->Draw();
  string name = TString::Format("cMassTarg1_ToFRightUp_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFRightUp->Print(name.c_str());

  TCanvas *cMassTarg1_ToFLeftUp = new TCanvas("cMassTarg1_ToFLeftUp","cMassTarg1_ToFLeftUp");  
  if(plotLogY) cMassTarg1_ToFLeftUp->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFLeftUp[ifile]->Draw();
    else        hMassTarg1_ToFLeftUp[ifile]->Draw("same");
    hMassTarg1_ToFLeftUp[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFLeftUp[ifile]->SetLineWidth(2);
    hMassTarg1_ToFLeftUp[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFLeftUp = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFLeftUp->SetFillColor(0);
  lMassTarg1_ToFLeftUp->SetLineColor(0);
  lMassTarg1_ToFLeftUp->SetTextFont(132);
  lMassTarg1_ToFLeftUp->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFLeftUp->AddEntry(hMassTarg1_ToFLeftUp[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFLeftUp->Draw();
  string name = TString::Format("cMassTarg1_ToFLeftUp_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFLeftUp->Print(name.c_str());

  TCanvas *cMassTarg1_ToFBackUp = new TCanvas("cMassTarg1_ToFBackUp","cMassTarg1_ToFBackUp");  
  if(plotLogY) cMassTarg1_ToFBackUp->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFBackUp[ifile]->Draw();
    else        hMassTarg1_ToFBackUp[ifile]->Draw("same");
    hMassTarg1_ToFBackUp[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFBackUp[ifile]->SetLineWidth(2);
    hMassTarg1_ToFBackUp[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFBackUp = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFBackUp->SetFillColor(0);
  lMassTarg1_ToFBackUp->SetLineColor(0);
  lMassTarg1_ToFBackUp->SetTextFont(132);
  lMassTarg1_ToFBackUp->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFBackUp->AddEntry(hMassTarg1_ToFBackUp[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFBackUp->Draw();
  string name = TString::Format("cMassTarg1_ToFBackUp_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFBackUp->Print(name.c_str());

  TCanvas *cMassTarg1_ToFTopDown = new TCanvas("cMassTarg1_ToFTopDown","cMassTarg1_ToFTopDown");  
  if(plotLogY) cMassTarg1_ToFTopDown->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFTopDown[ifile]->Draw();
    else        hMassTarg1_ToFTopDown[ifile]->Draw("same");
    hMassTarg1_ToFTopDown[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFTopDown[ifile]->SetLineWidth(2);
    hMassTarg1_ToFTopDown[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFTopDown = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFTopDown->SetFillColor(0);
  lMassTarg1_ToFTopDown->SetLineColor(0);
  lMassTarg1_ToFTopDown->SetTextFont(132);
  lMassTarg1_ToFTopDown->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFTopDown->AddEntry(hMassTarg1_ToFTopDown[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFTopDown->Draw();
  string name = TString::Format("cMassTarg1_ToFTopDown_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFTopDown->Print(name.c_str());


  TCanvas *cMassTarg1_ToFBotDown = new TCanvas("cMassTarg1_ToFBotDown","cMassTarg1_ToFBotDown");  
  if(plotLogY) cMassTarg1_ToFBotDown->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFBotDown[ifile]->Draw();
    else        hMassTarg1_ToFBotDown[ifile]->Draw("same");
    hMassTarg1_ToFBotDown[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFBotDown[ifile]->SetLineWidth(2);
    hMassTarg1_ToFBotDown[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFBotDown = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFBotDown->SetFillColor(0);
  lMassTarg1_ToFBotDown->SetLineColor(0);
  lMassTarg1_ToFBotDown->SetTextFont(132);
  lMassTarg1_ToFBotDown->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFBotDown->AddEntry(hMassTarg1_ToFBotDown[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFBotDown->Draw();
  string name = TString::Format("cMassTarg1_ToFBotDown_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFBotDown->Print(name.c_str());

  TCanvas *cMassTarg1_ToFRightDown = new TCanvas("cMassTarg1_ToFRightDown","cMassTarg1_ToFRightDown");  
  if(plotLogY) cMassTarg1_ToFRightDown->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFRightDown[ifile]->Draw();
    else        hMassTarg1_ToFRightDown[ifile]->Draw("same");
    hMassTarg1_ToFRightDown[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFRightDown[ifile]->SetLineWidth(2);
    hMassTarg1_ToFRightDown[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFRightDown = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFRightDown->SetFillColor(0);
  lMassTarg1_ToFRightDown->SetLineColor(0);
  lMassTarg1_ToFRightDown->SetTextFont(132);
  lMassTarg1_ToFRightDown->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFRightDown->AddEntry(hMassTarg1_ToFRightDown[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFRightDown->Draw();
  string name = TString::Format("cMassTarg1_ToFRightDown_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFRightDown->Print(name.c_str());


  TCanvas *cMassTarg1_ToFLeftDown = new TCanvas("cMassTarg1_ToFLeftDown","cMassTarg1_ToFLeftDown");  
  if(plotLogY) cMassTarg1_ToFLeftDown->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFLeftDown[ifile]->Draw();
    else        hMassTarg1_ToFLeftDown[ifile]->Draw("same");
    hMassTarg1_ToFLeftDown[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFLeftDown[ifile]->SetLineWidth(2);
    hMassTarg1_ToFLeftDown[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFLeftDown = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFLeftDown->SetFillColor(0);
  lMassTarg1_ToFLeftDown->SetLineColor(0);
  lMassTarg1_ToFLeftDown->SetTextFont(132);
  lMassTarg1_ToFLeftDown->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFLeftDown->AddEntry(hMassTarg1_ToFLeftDown[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFLeftDown->Draw();
  string name = TString::Format("cMassTarg1_ToFLeftDown_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFLeftDown->Print(name.c_str());


  TCanvas *cMassTarg1_ToFBackDown = new TCanvas("cMassTarg1_ToFBackDown","cMassTarg1_ToFBackDown");  
  if(plotLogY) cMassTarg1_ToFBackDown->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFBackDown[ifile]->Draw();
    else        hMassTarg1_ToFBackDown[ifile]->Draw("same");
    hMassTarg1_ToFBackDown[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFBackDown[ifile]->SetLineWidth(2);
    hMassTarg1_ToFBackDown[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFBackDown = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFBackDown->SetFillColor(0);
  lMassTarg1_ToFBackDown->SetLineColor(0);
  lMassTarg1_ToFBackDown->SetTextFont(132);
  lMassTarg1_ToFBackDown->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFBackDown->AddEntry(hMassTarg1_ToFBackDown[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFBackDown->Draw();
  string name = TString::Format("cMassTarg1_ToFBackDown_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFBackDown->Print(name.c_str());

  TCanvas *cMassTarg1_ToFFrontDown = new TCanvas("cMassTarg1_ToFFrontDown","cMassTarg1_ToFFrontDown");  
  if(plotLogY) cMassTarg1_ToFFrontDown->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFFrontDown[ifile]->Draw();
    else        hMassTarg1_ToFFrontDown[ifile]->Draw("same");
    hMassTarg1_ToFFrontDown[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFFrontDown[ifile]->SetLineWidth(2);
    hMassTarg1_ToFFrontDown[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFFrontDown = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFFrontDown->SetFillColor(0);
  lMassTarg1_ToFFrontDown->SetLineColor(0);
  lMassTarg1_ToFFrontDown->SetTextFont(132);
  lMassTarg1_ToFFrontDown->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFFrontDown->AddEntry(hMassTarg1_ToFFrontDown[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFFrontDown->Draw();
  string name = TString::Format("cMassTarg1_ToFFrontDown_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFFrontDown->Print(name.c_str());

  TCanvas *cMassTarg1_ToFFrontUp = new TCanvas("cMassTarg1_ToFFrontUp","cMassTarg1_ToFFrontUp");  
  if(plotLogY) cMassTarg1_ToFFrontUp->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFFrontUp[ifile]->Draw();
    else        hMassTarg1_ToFFrontUp[ifile]->Draw("same");
    hMassTarg1_ToFFrontUp[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFFrontUp[ifile]->SetLineWidth(2);
    hMassTarg1_ToFFrontUp[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFFrontUp = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFFrontUp->SetFillColor(0);
  lMassTarg1_ToFFrontUp->SetLineColor(0);
  lMassTarg1_ToFFrontUp->SetTextFont(132);
  lMassTarg1_ToFFrontUp->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFFrontUp->AddEntry(hMassTarg1_ToFFrontUp[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFFrontUp->Draw();
  string name = TString::Format("cMassTarg1_ToFFrontUp_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFFrontUp->Print(name.c_str());

  TCanvas *cMassTarg1_ToFECalP0D = new TCanvas("cMassTarg1_ToFECalP0D","cMassTarg1_ToFECalP0D");  
  if(plotLogY) cMassTarg1_ToFECalP0D->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFECalP0D[ifile]->Draw();
    else        hMassTarg1_ToFECalP0D[ifile]->Draw("same");
    hMassTarg1_ToFECalP0D[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFECalP0D[ifile]->SetLineWidth(2);
    hMassTarg1_ToFECalP0D[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFECalP0D = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFECalP0D->SetFillColor(0);
  lMassTarg1_ToFECalP0D->SetLineColor(0);
  lMassTarg1_ToFECalP0D->SetTextFont(132);
  lMassTarg1_ToFECalP0D->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFECalP0D->AddEntry(hMassTarg1_ToFECalP0D[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFECalP0D->Draw();
  string name = TString::Format("cMassTarg1_ToFECalP0D_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFECalP0D->Print(name.c_str());

  TCanvas *cMassTarg1_ToFAll = new TCanvas("cMassTarg1_ToFAll","cMassTarg1_ToFAll");  
  if(plotLogY) cMassTarg1_ToFAll->SetLogy();
  for(int ifile=0;ifile<Nfiles;ifile++){
    if(ifile==0) hMassTarg1_ToFAll[ifile]->Draw();
    else        hMassTarg1_ToFAll[ifile]->Draw("same");
    hMassTarg1_ToFAll[ifile]->SetLineColor(color[ifile]);
    hMassTarg1_ToFAll[ifile]->SetLineWidth(2);
    hMassTarg1_ToFAll[ifile]->SetStats(0);
  }
  TLegend *lMassTarg1_ToFAll = new TLegend(legXmin,legYmin,legXmax,legYmax);
  lMassTarg1_ToFAll->SetFillColor(0);
  lMassTarg1_ToFAll->SetLineColor(0);
  lMassTarg1_ToFAll->SetTextFont(132);
  lMassTarg1_ToFAll->SetTextSize(0.06);
  for(int ifile=0;ifile<Nfiles;ifile++){
    lMassTarg1_ToFAll->AddEntry(hMassTarg1_ToFAll[ifile],label[ifile].c_str(),"l");
  }
  lMassTarg1_ToFAll->Draw();
  string name = TString::Format("cMassTarg1_ToFAll_%s.pdf",tag.Data()).Data();
  if(plotStore) cMassTarg1_ToFAll->Print(name.c_str());


  return;
}
