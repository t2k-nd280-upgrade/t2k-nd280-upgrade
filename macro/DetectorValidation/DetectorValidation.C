//##################################################################################
//##################################################################################
//##################### Created by B. Quilain on 2017/05/20 ########################
//################ Output based on SelND280UpHit.C by D. Sgalaberna ################
//################### Last modification by B. Quilain on 2017/05/31 ################
//##################################################################################
//################################################################################## 

//
// Available sensitive detectors (1/2/17):
//
//
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/X/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/Y/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/X/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/Y/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/X/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/Y/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/X/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/Y/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/X/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/Y/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/X/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/Y/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/X/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/Y/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/DsECal/Module/Active/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/DsECal/Module/Active/ScintVert/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/LeftClam/P0DECal/TopLeftBotRight/Active/ScintPara/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/LeftClam/P0DECal/LeftSide/Active/ScintPara/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/LeftClam/P0DECal/BotLeftTopRight/Active/ScintPara/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/LeftClam/BrlECal/TopLeftBotRight/Active/ScintPara/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/LeftClam/BrlECal/TopLeftBotRight/Active/ScintPerp/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/LeftClam/BrlECal/LeftSide/Active/ScintPara/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/LeftClam/BrlECal/LeftSide/Active/ScintPerp/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/LeftClam/BrlECal/BotLeftTopRight/Active/ScintPara/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/LeftClam/BrlECal/BotLeftTopRight/Active/ScintPerp/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/RightClam/P0DECal/BotLeftTopRight/Active/ScintPara/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/RightClam/P0DECal/RightSide/Active/ScintPara/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/RightClam/P0DECal/TopLeftBotRight/Active/ScintPara/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/RightClam/BrlECal/BotLeftTopRight/Active/ScintPara/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/RightClam/BrlECal/BotLeftTopRight/Active/ScintPerp/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/RightClam/BrlECal/RightSide/Active/ScintPara/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/RightClam/BrlECal/RightSide/Active/ScintPerp/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/RightClam/BrlECal/TopLeftBotRight/Active/ScintPara/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/RightClam/BrlECal/TopLeftBotRight/Active/ScintPerp/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ForwTPC1/Half --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ForwTPC1/Half --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ForwTPC1/MM --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ForwTPC2/Half --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ForwTPC2/Half --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ForwTPC2/MM --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ForwTPC3/Half --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ForwTPC3/Half --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ForwTPC3/MM --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/TPCUp1 --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/TPCUp2 --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/TPCDown1 --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/TPCDown2 --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/Target1 --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/Target2 --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/TopDown/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/TopDown/ScintVert/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/BotDown/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/BotDown/ScintVert/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/RightDown/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/RightDown/ScintVert/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/LeftDown/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/LeftDown/ScintVert/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/BackDown/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/BackDown/ScintVert/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/FrontDown/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/FrontDown/ScintVert/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/TopUp/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/TopUp/ScintVert/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/BotUp/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/BotUp/ScintVert/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/RightUp/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/RightUp/ScintVert/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/LeftUp/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/LeftUp/ScintVert/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/BackUp/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/BackUp/ScintVert/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/FrontUp/ScintHoriz/Bar --> Included and same name as corresponding physical volume
// /t2k/OA/Magnet/Basket/ToF/FrontUp/ScintVert/Bar --> Included and same name as corresponding physical volume



#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
//#define DEBUG

#include "/home/bquilain/ND280upgrade/V2/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.hh"
#include "/home/bquilain/ND280upgrade/V2/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.hh"
#include "/home/bquilain/ND280upgrade/V2/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.hh"
#include "/home/bquilain/ND280upgrade/V2/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.hh"
#include "/home/bquilain/ND280upgrade/V2/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.hh"
int GetReacAll(int neut_reaction_mode);
string StringReacAll(int neut_reaction_mode);
bool IsTargetIn(double x,double y,double z,
		// Target 1
		double vtx_min_x_1,
		double vtx_max_x_1,  
		double vtx_min_y_1, 
		double vtx_max_y_1, 
		double vtx_min_z_1,  
		double vtx_max_z_1 
		);

int SelND280UpHit
(
 const int evtfirst = 0,
 const int nevents = 1,
 
 string tag = "outputs/prova",
 
 string infilename = "../bin/ciao.root",

 // Definition of Fiducial Volume for target thickness=60cm (mm)  
 const double vtx_min_x_1 = -1150,
 const double vtx_max_x_1 = +1150,  
 const double vtx_min_y_1 = -300, 
 const double vtx_max_y_1 = +300, 
 const double vtx_min_z_1 = -2487,  
 const double vtx_max_z_1 = -487, 

 // Cut 0: Select only CC mode
 const bool doCutCC = true,

 // Cut 1: Select reaction mode
 const bool doCutReac = false,
 const int cut_reac = 0, // 0=CCQE, 1=2p2h, 2=CC1pi, 3=CCcoh, 4=CCDIS, 5=NC
 
 // Cut 2: Vertex cut (inside the Targets and FGDs)
 const bool doCutMater = false,
 const bool doCutVtx = true, 
 
 const bool doCutTarget1 = true, // Select vertex in Target1
 const bool doCutTarget2 = false, // Select vertex in Target2
 const bool doCutFGD1 = true, // Select vertex in FGD1
 const bool doCutFGD2 = false, // Select vertex in FGD2 

 const bool doCutVtxX = false, // width
 const double cut_xmin = 0,
 const double cut_xmax = 0,
 
 const bool doCutVtxZ = false, // length
 const double cut_zmin = 0,
 const double cut_zmax = 0,
 
 // Cut 3: Select PDG
 const bool doCutPDG = true,
 const int cut_pdg = 13, //+211, //+2212; //+13; // +211; //+11
 
 // Cut 4: Select parent ID (0 --> it's primary particle)
 const bool doCutParentID = true,
 const int cut_parentID = 0,

 // Cut 5: Charge cut (TPCs and Target)  
 const bool doCutCharge = true,
 const double cut_charge = 1, // absolute value 
 
 // Cut 6: Length cut (Targets) 
 const bool doCutDLTarget = true, // full length in target and dlyz in tpcs    
 const double cut_length_target_min = 50, //mm  
 
 // Cut 7: DeltaLyz cut (TPCs)
 const bool doCutDLyzTPC = true,
 const double cut_dlyz_tpc_min = 200 //mm  
 
 )
{
  // Define histograms
#ifdef DEBUG
  cout << "Hello Benjamin " << endl;
#endif
  //const double MeV2PE_WAGASCI = 31.5;
  const double MeV2PE_WAGASCI = 31.5;
  
  const int NEvtDisplTot = 50; 

  TH2F *hMPPCHits_XY[NEvtDisplTot]; TH2F *hMPPCHits_XZ[NEvtDisplTot]; TH2F *hMPPCHits_YZ[NEvtDisplTot];
  
  //

  TFile *finput = new TFile(infilename.c_str(),"READ");

  // Take 2D projection histograms with MPPC hits 
  /*Temp  by BNJ
  TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
  h2d_xy = (TH2F*)finput->Get("OutMPPCProj2D_XY");
  h2d_xz = (TH2F*)finput->Get("OutMPPCProj2D_XZ");
  h2d_yz = (TH2F*)finput->Get("OutMPPCProj2D_YZ");
  
  for(int ievtdispl=0;ievtdispl<NEvtDisplTot;ievtdispl++){
    TString name;
    name = TString::Format("hMPPCHits_XY_%d",ievtdispl);
    hMPPCHits_XY[ievtdispl] = (TH2F*)h2d_xy->Clone(name);
    name = TString::Format("hMPPCHits_XZ_%d",ievtdispl);
    hMPPCHits_XZ[ievtdispl] = (TH2F*)h2d_xz->Clone(name);
    name = TString::Format("hMPPCHits_YZ_%d",ievtdispl);
    hMPPCHits_YZ[ievtdispl] = (TH2F*)h2d_yz->Clone(name);
  }
  */
  TH1D * TotalEdep = new TH1D("TotalEdep","",200,0,0.2);
  TH1D * TotalEdep_pe = new TH1D("TotalEdep_pe","",200,0,200);
  TH1D * TotalEdep_pe_plan = new TH1D("TotalEdep_pe_plan","",200,0,200);
  TH1D * TotalEdep_pe_grid = new TH1D("TotalEdep_pe_grid","",200,0,200);
  TH1D * TotalEdep_pecorrfiber_plan = new TH1D("TotalEdep_pecorrfiber_plan","",200,0,200);
  TH1D * TotalEdep_pecorrfiber_grid = new TH1D("TotalEdep_pecorrfiber_grid","",200,0,200);
  TH1D * TotalEdep_pecorr_plan = new TH1D("TotalEdep_pecorr_plan","",200,0,200);
  TH1D * TotalEdep_pecorr_grid = new TH1D("TotalEdep_pecorr_grid","",200,0,200);

  //Careful: detector dependent
  TH3D * Edep_positioninscinti_planhoriz = new TH3D("Edep_positioninscinti_planhoriz","",60,0,2300,10,0,25,100,0,200);
  Edep_positioninscinti_planhoriz->GetXaxis()->SetTitle("Position along fiber");
  Edep_positioninscinti_planhoriz->GetYaxis()->SetTitle("Position orthogonal to fiber");
  Edep_positioninscinti_planhoriz->GetZaxis()->SetTitle("p.e");
  TH3D * Edep_positioninscinti_planvert = new TH3D("Edep_positioninscinti_planvert","",300,0,600,25,0,25,100,0,200);
  Edep_positioninscinti_planvert->GetXaxis()->SetTitle("Position along fiber");
  Edep_positioninscinti_planvert->GetYaxis()->SetTitle("Position orthogonal to fi ber");
  Edep_positioninscinti_planvert->GetZaxis()->SetTitle("p.e");
  TH3D * Edep_positioninscinti_gridhoriz = new TH3D("Edep_positioninscinti_gridhoriz","",60,0,2300,10,0,25,100,0,200);
  Edep_positioninscinti_gridhoriz->GetXaxis()->SetTitle("Position along fiber");
  Edep_positioninscinti_gridhoriz->GetYaxis()->SetTitle("Position orthogonal to fiber");
  Edep_positioninscinti_gridhoriz->GetZaxis()->SetTitle("p.e");
  TH3D * Edep_positioninscinti_gridvert = new TH3D("Edep_positioninscinti_gridvert","",300,0,600,25,0,25,100,0,200);
  Edep_positioninscinti_gridvert->GetXaxis()->SetTitle("Position along fiber");
  Edep_positioninscinti_gridvert->GetYaxis()->SetTitle("Position orthogonal to fiber");
  Edep_positioninscinti_gridvert->GetZaxis()->SetTitle("p.e");

  TH3D * Edep_positioninscintiabsolute_planvert = new TH3D("Edep_positioninscintiabsolute_planvert","",300,-300,300,40,-20.,20.,100,0,200);
  Edep_positioninscintiabsolute_planvert->GetXaxis()->SetTitle("Position along fiber");
  Edep_positioninscintiabsolute_planvert->GetYaxis()->SetTitle("Position orthogonal to fiber");
  Edep_positioninscintiabsolute_planvert->GetZaxis()->SetTitle("p.e");

  TH3D * Edep_positioninscintiabsolute_gridvert = new TH3D("Edep_positioninscintiabsolute_gridvert","",300,-300,300,40,-20.,20.,100,0,200);
  Edep_positioninscintiabsolute_gridvert->GetXaxis()->SetTitle("Position along fiber");
  Edep_positioninscintiabsolute_gridvert->GetYaxis()->SetTitle("Position orthogonal to fiber");
  Edep_positioninscintiabsolute_gridvert->GetZaxis()->SetTitle("p.e");

  double Size[3] = {2300,1200,1300};
  double NBins[3];
  for(int i=0;i<3;i++) NBins[i] = ((int) Size[i]/25);
  double sci[4]={0,0,0,0};  					 
  //

#ifdef DEBUG
  cout << "Opening " << infilename << " and trying to open the tree inside called ND280UpEvents "<< endl;
#endif  
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

#ifdef DEBUG
  cout << "Number of entries of this tree = " << tinput->GetEntries() << endl;
#endif    
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();  
  tinput->SetBranchAddress("Event",&nd280UpEvent);
  
  int NTreeEntries = tinput->GetEntries();  
  int evtlasttree = NTreeEntries-1;
  int Nentries = -999;

  cout << endl;
  cout << "# of entries in the Tree: " << NTreeEntries << endl;
  cout << "last tree event ID: " << evtlasttree << endl;
  cout << "first event ID: " << evtfirst << endl;
  cout << endl;
  
  if(evtfirst > evtlasttree){
    cout << endl;
    cout << "first selection evt ID > first evt ID of tree:" << endl;
    cout << " - # of tree events = " << NTreeEntries << endl;
    cout << " - last tree event = " << evtlasttree << endl;
    cout << " - first event = " << evtfirst << endl;
    cout << "Exit!!!" << endl;
    cout << endl;
    exit(1);
  }
  else if(NTreeEntries < (evtfirst+nevents)){
    
    Nentries = NTreeEntries - evtfirst;
    
    cout << endl;
    cout << "WARNING:" << endl;
    cout << "first evt ID + # of events > tot # of tree events:" << endl;
    cout << " - # of tree events = " << NTreeEntries << endl;
    cout << " - # of events to run = " << nevents << endl;
    cout << " - first event = " << evtfirst << endl;
    cout << "Set # of events to run to " << Nentries;
    cout << endl;
  }
  else{
    //B.Q
    Nentries = NTreeEntries - evtfirst;    
  }
  
  //int EntryLast = 0;
  int EntryLast = evtfirst+Nentries-1;
  
  cout << endl;
  cout << "First event = " << evtfirst << endl;
  cout << "Last event = " << EntryLast << endl;
  cout << "# of events = " << Nentries << endl;
  cout << endl;


  int NEvtWithHit = 0;

  //
  TH2D * EventDisplay_Side[EntryLast-evtfirst+1];
  TH2D * EventDisplay_Top[EntryLast-evtfirst+1];
  for(int ievt=evtfirst;ievt<=EntryLast;ievt++){ // get last entry
    
    if(ievt%1000==0) cout<<"Number of event="<<ievt<<" / "<<EntryLast-evtfirst+1<<endl;
    //BQif(NEvtWithHit>=NEvtDisplTot) break;
    
    tinput->GetEntry(ievt);
    
    int NHits = nd280UpEvent->GetNHits();

#ifdef DEBUG
    cout << "Event " << ievt << ", EventWithHit " << NEvtWithHit << endl;
    cout << "# of hits = " << nd280UpEvent->GetNHits() << endl;
#endif    

    if(NHits>0) NEvtWithHit++;

    //B.Q !!!! DETECTOR DEPENDENT
    EventDisplay_Top[ievt-evtfirst] = new TH2D(Form("EventDisplay_Top_%d",ievt-evtfirst),"",NBins[2],-Size[2]/2.,Size[2]/2.,NBins[0],-Size[0]/2.,Size[0]/2.);
    EventDisplay_Side[ievt-evtfirst] = new TH2D(Form("EventDisplay_Side_%d",ievt-evtfirst),"",NBins[2],-Size[2]/2.,Size[2]/2.,NBins[1],-Size[1]/2.,Size[1]/2.);
    //EventDisplay_Top[ievt-evtfirst] = new TH2D(Form("EventDisplay_Top_%d",ievt-evtfirst),"",NBins[2],-Size[2]/2.,0,NBins[0],-Size[0]/2.,Size[0]/2.);
    //EventDisplay_Side[ievt-evtfirst] = new TH2D(Form("EventDisplay_Side_%d",ievt-evtfirst),"",NBins[2],-Size[2]/2.,0.,NBins[1],0,Size[1]);

    vector <int> HitID;
    HitID.clear();
    vector <double> hitpex;
    vector <double> hitpey;
    vector <double> hitpez;
    hitpex.clear();
    hitpey.clear();
    hitpez.clear();
    vector <string> HitName;
    HitName.clear();
    //
    vector < vector <double> > HitPos;
    for(int i=0;i<HitPos.size();i++) HitPos[i].clear();
    HitPos.clear();
    //
    vector < vector <double> > MPPCLocPos;
    for(int i=0;i<MPPCLocPos.size();i++) MPPCLocPos[i].clear();
    MPPCLocPos.clear();
    //
    vector < vector <double> > MPPCPos;
    for(int i=0;i<MPPCPos.size();i++) MPPCPos[i].clear();
    MPPCPos.clear();
    
    for(int ihit=0;ihit<NHits;ihit++){ // get last entry
      
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);
      //nd280UpHit->PrintHit();
      
      double mppcx = nd280UpHit->GetLocPosX();
      double mppcy = nd280UpHit->GetLocPosY();
      double mppcz = nd280UpHit->GetLocPosZ();

      double pex = nd280UpHit->GetPEX(); // along X
      double pey = nd280UpHit->GetPEY(); // along Y
      double pez = nd280UpHit->GetPEZ(); // along Z

      //cout << pex << " " << pey << " " << pez << endl;
      //cout << mppcx << " " << mppcy << " " << mppcz << endl;

      int ievtWithHit = NEvtWithHit-1;
      /*Temp by BNJ
      hMPPCHits_XY[ievtWithHit]->Fill(mppcx,mppcy,pez); // pe along X
      hMPPCHits_XZ[ievtWithHit]->Fill(mppcx,mppcz,pey); // pe along Y
      hMPPCHits_YZ[ievtWithHit]->Fill(mppcy,mppcz,pex); // pe along Z 
      */
      
      //B.Q
      int detID = nd280UpHit->GetDetID();
      bool used=false;
      for(int i=0;i<HitID.size();i++){
	//cout << detID << ", " << HitID[i] <<endl;
	if(HitID[i] == detID){
	  used=true;
	  hitpex[i] += pex;
	  hitpey[i] += pey;
	  hitpez[i] += pez;
	  break;
	}
      }
      
      if(!used){
	HitID.push_back(detID);
	hitpex.push_back(pex);
	hitpey.push_back(pey);
	hitpez.push_back(pez);
	HitName.push_back(nd280UpHit->GetDetName());
	vector <double> pos;
	pos.clear();
	pos.push_back(nd280UpHit->GetLocPosX());
	pos.push_back(nd280UpHit->GetLocPosY());
	pos.push_back(nd280UpHit->GetLocPosZ());
	HitPos.push_back(pos);

	vector <double> mppclocpos;
	mppclocpos.clear();
	mppclocpos.push_back(nd280UpHit->GetMPPCLocPosX());
	mppclocpos.push_back(nd280UpHit->GetMPPCLocPosY());
	mppclocpos.push_back(nd280UpHit->GetMPPCLocPosZ());
	MPPCLocPos.push_back(mppclocpos);

	vector <double> mppcpos;
	mppcpos.clear();
	mppcpos.push_back(nd280UpHit->GetMPPCPosX());
	mppcpos.push_back(nd280UpHit->GetMPPCPosY());
	mppcpos.push_back(nd280UpHit->GetMPPCPosZ());
	MPPCPos.push_back(mppcpos);
      }

      ////////////////////////////////
    }
    //nd280UpEvent->PrintEvent();
    for(int i=0;i<HitID.size();i++){
      double pe = MeV2PE_WAGASCI*hitpez[i];

			      
#ifdef DEBUG
      cout << "ID = "<< HitID[i] << ", Name = " << HitName[i] << ", edep = " << hitpez[i] << " MeV, pe = " << MeV2PE_WAGASCI*hitpez[i] ;
      cout << ", Position = (" << (HitPos[i])[0] << "," << (HitPos[i])[1] << "," << (HitPos[i])[2] << ")" ;
      cout << ", MPPCLocPosition = (" << (MPPCLocPos[i])[0] << "," << (MPPCLocPos[i])[1] << "," << (MPPCLocPos[i])[2] << ")" << endl;
#endif
      double pecorrfiber; double distfiber;double attleng=2420;//mm
      double distscinti;double sciattleng_plan=20.0;double sciattleng_grid=20.0;//mm
      double pecorr;
      //Grid size dependent: should be modified !
      if( HitName[i].compare("ScintiVerticalPlanLV") == 0 ){
	sci[0]++;
	(MPPCLocPos[i])[0] += 6.125;
      }
      else if( HitName[i].compare("ScintiHorizontalPlanLV") == 0 ){
	sci[1]++;
	(MPPCLocPos[i])[1] += 6.125;
      }
      else if( HitName[i].compare("ScintiVerticalGridLV") == 0 ){
	sci[2]++;
	(MPPCLocPos[i])[2] += 6.125;	
      }
      else if( HitName[i].compare("ScintiHorizontalGridLV") == 0 ){
	sci[3]++;
	(MPPCLocPos[i])[2] += 6.125;
      }
      //attenuation in the fiber
      if( (HitName[i].compare("ScintiVerticalPlanLV") == 0) || (HitName[i].compare("ScintiVerticalGridLV") == 0)) distfiber = TMath::Abs((MPPCLocPos[i])[1]-(HitPos[i])[1]);
      else if( (HitName[i].compare("ScintiHorizontalPlanLV") == 0) || (HitName[i].compare("ScintiHorizontalGridLV") == 0)) distfiber = TMath::Abs((MPPCLocPos[i])[0]-(HitPos[i])[0]);
      else cout<<"problem!"<<endl;
      pecorrfiber = pe*exp(-1.*distfiber/attleng);

      //attenuation in the scintillator
      if( (HitName[i].compare("ScintiVerticalPlanLV") == 0) ) distscinti = TMath::Abs((HitPos[i])[0]-(MPPCLocPos[i])[0]);
      else if( (HitName[i].compare("ScintiHorizontalPlanLV") == 0) ) distscinti = TMath::Abs((HitPos[i])[1]-(MPPCLocPos[i])[1]);
      else if( (HitName[i].compare("ScintiVerticalGridLV") == 0) || (HitName[i].compare("ScintiHorizontalGridLV") == 0) ) distscinti = TMath::Abs((HitPos[i])[2]-(MPPCLocPos[i])[2]);

      if( (HitName[i].compare("ScintiVerticalPlanLV") == 0) || (HitName[i].compare("ScintiHorizontalPlanLV") == 0) ) pecorr = pecorrfiber*exp(-1.*distscinti/sciattleng_plan);
      else if ( (HitName[i].compare("ScintiVerticalGridLV") == 0) || (HitName[i].compare("ScintiHorizontalGridLV") == 0) ) pecorr = pecorrfiber* exp(-1.*distscinti/sciattleng_grid);

#ifdef DEBUG
      cout<<"ID = "<< HitID[i] << ", Name = " << HitName[i] << ", edep = " << hitpez[i] << " MeV, pe = " << pe << ", after fiber att = " << pecorrfiber << ", after scinti att = " << pecorr << ", dist fiber = "<< distfiber << ", dist scinti = " << distscinti << endl;
      cout<<sci[0]<<","<<sci[1]<<","<<sci[2]<<","<<sci[3]<<endl;
#endif

      //Cut at 2.5 p.e
      if(pecorr<2.5) continue;



      
      if((HitName[i].compare("ScintiHorizontalPlanLV") == 0) || (HitName[i].compare("ScintiHorizontalGridLV") == 0)) EventDisplay_Side[ievt-evtfirst]->Fill((MPPCPos[i])[2],(MPPCPos[i])[1],pecorr);
      else if((HitName[i].compare("ScintiVerticalPlanLV") == 0) || (HitName[i].compare("ScintiVerticalGridLV") == 0)) EventDisplay_Top[ievt-evtfirst]->Fill((MPPCPos[i])[2],(MPPCPos[i])[0],pecorr);
      
      TotalEdep->Fill(hitpez[i]);
      TotalEdep_pe->Fill(pe);
      if( (HitName[i].compare("ScintiVerticalPlanLV") == 0) || (HitName[i].compare("ScintiHorizontalPlanLV") == 0) ){
	TotalEdep_pe_plan->Fill(pe);
	TotalEdep_pecorrfiber_plan->Fill(pecorrfiber);
	TotalEdep_pecorr_plan->Fill(pecorr);
	if(HitName[i].compare("ScintiHorizontalPlanLV") == 0) Edep_positioninscinti_planhoriz->Fill(distfiber,distscinti,pecorr);
	else if(HitName[i].compare("ScintiVerticalPlanLV") == 0){
	  Edep_positioninscinti_planvert->Fill(distfiber,distscinti,pecorr);
	  Edep_positioninscintiabsolute_planvert->Fill((HitPos[i])[1],(HitPos[i])[0],pecorr);
	}
	else cout << "Problem" <<endl;
      }
      else{
	TotalEdep_pe_grid->Fill(pe);
	TotalEdep_pecorrfiber_grid->Fill(pecorrfiber);
	TotalEdep_pecorr_grid->Fill(pecorr);
	if(HitName[i].compare("ScintiHorizontalGridLV") == 0) Edep_positioninscinti_gridhoriz->Fill(distfiber,distscinti,pecorr);
	else if(HitName[i].compare("ScintiVerticalGridLV") == 0){
	  Edep_positioninscinti_gridvert->Fill(distfiber,distscinti,pecorr);
	  Edep_positioninscintiabsolute_gridvert->Fill((HitPos[i])[1],(HitPos[i])[2],pecorr);
	}
	else cout << "Problem" <<endl;

      }
    }
    // Fill event histograms

    delete nd280UpEvent;    
    nd280UpEvent = 0;
  } // end loop over events

  // Write output file
  TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);
  TFile *out = new TFile(outfilename.Data(),"RECREATE");
  //
    TotalEdep->Write();
    TotalEdep_pe->Write();
    TotalEdep_pe_plan->Write();
    TotalEdep_pe_grid->Write();
    TotalEdep_pecorrfiber_plan->Write();
    TotalEdep_pecorrfiber_grid->Write();
    TotalEdep_pecorr_plan->Write();
    TotalEdep_pecorr_grid->Write();
    Edep_positioninscinti_planhoriz->Write();
    Edep_positioninscinti_planvert->Write();
    Edep_positioninscinti_gridhoriz->Write();
    Edep_positioninscinti_gridvert->Write();
    Edep_positioninscintiabsolute_planvert->Write();
    Edep_positioninscintiabsolute_gridvert->Write();
#ifdef DEBUG
    for(int ievt=evtfirst;ievt<=EntryLast;ievt++){ // get last entry
      EventDisplay_Side[ievt-evtfirst]->Write();
      EventDisplay_Top[ievt-evtfirst]->Write();
    }
#endif
    /*

  }
*/
  //
  out->Close();
  return 0; 
}





//___________________________________________________________________________ 
bool IsCCQE(int neut_reaction_mode)
{
  return neut_reaction_mode ==  1;
}
//___________________________________________________________________________ 
bool IsCC1pi(int neut_reaction_mode)
{
  bool is_cc1pi = (neut_reaction_mode == 11 ||
                   neut_reaction_mode == 12 ||
                   neut_reaction_mode == 13);
  return is_cc1pi;
}
//___________________________________________________________________________ 
bool IsCCcoh(int neut_reaction_mode)
{
  return neut_reaction_mode ==  16;
}
//___________________________________________________________________________ 
bool Is2p2h(int neut_reaction_mode)
{
  return neut_reaction_mode ==  2;
}
//___________________________________________________________________________
bool IsCCoth(int neut_reaction_mode)                               
{
  bool is_ccoth = (neut_reaction_mode == 17 ||
                     neut_reaction_mode == 21 ||
                     neut_reaction_mode == 22 ||
                     neut_reaction_mode == 23 ||
                     neut_reaction_mode == 26);
  return is_ccoth;
}
//___________________________________________________________________________ 
bool IsNC(int neut_reaction_mode)
{
  bool is_not_nc =  IsCCoth(neut_reaction_mode);
  is_not_nc = Is2p2h(neut_reaction_mode);
  is_not_nc = IsCCcoh(neut_reaction_mode);
  is_not_nc = IsCC1pi(neut_reaction_mode);
  is_not_nc = IsCCQE(neut_reaction_mode);
  
  bool is_nc = !is_not_nc;
  return is_nc;
}
//___________________________________________________________________________ 
int GetReacAll(int neut_reaction_mode){
  if(IsCCQE(neut_reaction_mode)){ 
    return 0;
  }
  else if(Is2p2h(neut_reaction_mode)){ 
    return 1;
  }
  else if(IsCC1pi(neut_reaction_mode)){ 
    return 2;
  }
  else if(IsCCcoh(neut_reaction_mode)){ 
    return 3;
  }
  else if(IsCCoth(neut_reaction_mode)){ 
    return 4;
  }
  else {
    return 5;
  }
}
//___________________________________________________________________________ 
string StringReacAll(int reaction_mode_all){
  if(reaction_mode_all==0){ return "CCQE";}
  else if(reaction_mode_all==1){ return "2p2h";}
  else if(reaction_mode_all==2){ return "CC1pi";}
  else if(reaction_mode_all==3){ return "CCcoh";}
  else if(reaction_mode_all==4){ return "CCOth";}
  else if(reaction_mode_all==5){ return "NC";}
  else {
    cerr << "No Reac mode" << endl;
    cerr << "reaction_mode_all = " << reaction_mode_all << endl;
    exit(1);
  }
}
//___________________________________________________________________________ 
//bool IsTargetIn(int targetID,double x,double y,double z,
bool IsTargetIn(double x,double y,double z,
		// Target 1
		double vtx_min_x_1,
		double vtx_max_x_1,  
		double vtx_min_y_1, 
		double vtx_max_y_1, 
		double vtx_min_z_1,  
		double vtx_max_z_1 
		// Target 2
		//double vtx_min_x_2,  
		//double vtx_max_x_2,  
		//double vtx_min_y_2, 
		//double vtx_max_y_2,  
		//double vtx_min_z_2,
		//double vtx_max_z_2  
){
  
  // Target 1 FV
  //if(targetID==1){
  if((x > vtx_min_x_1 && x < vtx_max_x_1) && 
     (y > vtx_min_y_1 && y < vtx_max_y_1) && 
     (z > vtx_min_z_1 && z < vtx_max_z_1)
     ){
    return true;
  }
  else{
    return false;
  }
  //}	
  // Target 2 FV
  //if(targetID==2){
  //if((x > vtx_min_x_2 && x < vtx_max_x_2) &&
  //(y > vtx_min_y_2 && y < vtx_max_y_2) &&
  //(z > vtx_min_z_2 && z < vtx_max_z_2)
  //){	  
  //return true;
  //}
  //else{
  //return false;
  //}
  //}
  //else{
  //cerr << endl;
  //cerr << "Target ID = " << targetID << " is wrong" << endl;
  //cerr << "You can choose between 1 and 2" << endl;
  //cerr << endl;
  //exit(1);
  //}
}
//___________________________________________________________________________                                                                                                                           
//bool IsTargetIn(int targetID,double x,double y,double z,
//              // Target 1                                                                                                                                                                     
//double vtx_min_x_1,
//double vtx_max_x_1,
//              double vtx_min_y_1,
//              double vtx_max_y_1,
//              double vtx_min_z_1,
//              double vtx_max_z_1,
//              // Target 2                                                                                                                                                                             
//              double vtx_min_x_2,
//              double vtx_max_x_2,
//                double vtx_min_y_2,
//              double vtx_max_y_2,
//                double vtx_min_z_2,
//              double vtx_max_z_2
//		){                                                                                
// Target 1 FV                                                                                                                                                                         
//if(targetID==1){
//    if((x > vtx_min_x_1 && x < vtx_max_x_1) &&
//     (y > vtx_min_y_1 && y < vtx_max_y_1) &&
//       (z > vtx_min_z_1 && z < vtx_max_z_1)
//     ){
//return true;
//  }
//  else{
//    return false;
//  }
//  }
// Target 2 FV                                                                                                                                                                                            
//if(targetID==2){
//  if((x > vtx_min_x_2 && x < vtx_max_x_2) &&
//     (y > vtx_min_y_2 && y < vtx_max_y_2) &&
//     (z > vtx_min_z_2 && z < vtx_max_z_2)
//     ){
//    return true;
//  }
//  else{
//    return false;
//  }
//}
//else{
// cerr << endl;
// cerr << "Target ID = " << targetID << " is wrong" << endl;
//  cerr << "You can choose between 1 and 2" << endl;
//  cerr << endl;
//  exit(1);
//}
//}
//___________________________________________________________________________ 
