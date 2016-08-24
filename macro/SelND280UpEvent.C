
//
// Available sensitive detectors:
// detname=="ForwTPC1/Half" 
// detname=="ForwTPC2/Half" 
// detname=="ForwTPC3/Half" 
// detname=="Target1" 
// detname=="Target2" 
// detname=="TPCUp1" 
// detname=="TPCDown1"
// detname=="TPCUp2" 
// detname=="TPCDown2"
//

#include <TH1D.h>
#include <TH2D.h>
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>

#include "/atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.hh"
#include "/atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.hh"
#include "/atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.hh"
#include "/atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.hh"

int GetReacAll(int neut_reaction_mode);
string StringReacAll(int neut_reaction_mode);

void SelND280UpEvent
(
 const int evtfirst = 0,
 const int nevents = 1,
 
 string tag = "outputs/prova",
 
 //string infilename = "/neutrino/data7/davide/files/g4ND280Up_6E20_All_NoPoint.root",
 string infilename = "../bin/ciao.root",

 // Cut 1: Select reaction mode
 const bool doCutReac = false,
 const int cut_reac = 0, // 0=CCQE, 1=2p2h, 2=CC1pi, 3=CCcoh, 4=CCDIS, 5=NC
 
 // Cut 2: Vertex cut (inside the Target)
 const bool doCutMater = false,
 const bool doCutVtx = true, 
 
 const bool doCutTarget1 = true, // Select vertex in Target1
 const bool doCutTarget2 = false, // Select vertex in Target2
 
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
  // Definition of Fiducial Volume (Target 1 or 2)  
  // Target 1
  const double vtx_min_x_1 = -1150;  //mm
  const double vtx_max_x_1 = +1150;  //mm
  const double vtx_min_y_1 = -300; //mm
  const double vtx_max_y_1 = +300; //mm
  const double vtx_min_z_1 = -2487;  //mm
  const double vtx_max_z_1 = -487;  //mm
  // Target 2
  const double vtx_min_x_2 = -1150;  //mm
  const double vtx_max_x_2 = +1150;  //mm
  const double vtx_min_y_2 = -300; //mm
  const double vtx_max_y_2 = +300; //mm
  const double vtx_min_z_2 = +487;  //mm
  const double vtx_max_z_2 = +2487;  //mm 


  //gROOT->ProcessLine(".L /atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+");
  //gROOT->ProcessLine(".L /atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+");
  //gROOT->ProcessLine(".L /atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+");
  //gROOT->ProcessLine(".L /atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+");
   
  // Efficiency

  const int NBins_Mom = 33;
  double BinEdges_Mom[NBins_Mom+1] = {0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1200,1400,
				      1600,1800,2000,2500,3000,4000,5000,6000,7000,8000,10000};
  const int NBins_CosTh = 20;
  double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};
  
  TH2D *hCosThetaVsMom_FV = new TH2D("hCosThetaVsMom_FV","hCosThetaVsMom_FV",NBins_CosTh,BinEdges_CosTh,NBins_Mom,BinEdges_Mom);
  TH2D *hCosThetaVsMom_Target       
    = (TH2D*) hCosThetaVsMom_FV->Clone("hCosThetaVsMom_Target");
  TH2D *hCosThetaVsMom_TPC          
    = (TH2D*) hCosThetaVsMom_FV->Clone("hCosThetaVsMom_TPC");
  TH2D *hCosThetaVsMom_TargetOrTPC  
    = (TH2D*) hCosThetaVsMom_FV->Clone("hCosThetaVsMom_TargetOrTPC");
  TH2D *hCosThetaVsMom_TargetAndTPC 
    = (TH2D*) hCosThetaVsMom_FV->Clone("hCosThetaVsMom_TargetAndTPC");  
    
  // FV histograms

  TH1D *hL_SD = new TH1D("hL_SD","hL_SD",200,0,2000);
  TH1D *hL_TargAll_FV = new TH1D("hL_TargAll_FV","hL_TargAll_FV",200,0,2000);
  TH1D *hL_Targ1_FV = new TH1D("hL_Targ1_FV","hL_Targ1_FV",200,0,2000);
  TH1D *hL_Targ2_FV = new TH1D("hL_Targ2_FV","hL_Targ2_FV",200,0,2000);
  TH1D *hL_TargAll_Cut = new TH1D("hL_TargAll_Cut","hL_TargAll_Cut",200,0,2000);
  TH1D *hL_Targ1_Cut = new TH1D("hL_Targ1_Cut","hL_Targ1_Cut",200,0,2000);
  TH1D *hL_Targ2_Cut = new TH1D("hL_Targ2_Cut","hL_Targ2_Cut",200,0,2000);
  TH2D *hLVsMom_SD = new TH2D("hLVsMom_SD","hLVsMom_SD",200,0,2000,100,0,5000);  
  TH2D *hLVsMom_TargAll_FV = new TH2D("hLVsMom_TargAll_FV","hLVsMom_TargAll_FV",200,0,2000,100,0,5000);  
  TH2D *hLVsMom_Targ1_FV = new TH2D("hLVsMom_Targ1_FV","hLVsMom_Targ1_FV",200,0,2000,100,0,5000);  
  TH2D *hLVsMom_Targ2_FV = new TH2D("hLVsMom_Targ2_FV","hLVsMom_Targ2_FV",200,0,2000,100,0,5000);  
  TH2D *hLVsMom_TargAll_Cut = new TH2D("hLVsMom_TargAll_Cut","hLVsMom_TargAll_Cut",200,0,2000,100,0,5000);  
  TH2D *hLVsMom_Targ1_Cut = new TH2D("hLVsMom_Targ1_Cut","hLVsMom_Targ1_Cut",200,0,2000,100,0,5000);  
  TH2D *hLVsMom_Targ2_Cut = new TH2D("hLVsMom_Targ2_Cut","hLVsMomTarg2_Cut",200,0,2000,100,0,5000);  

  TH1D *hLyz_TPCAll = new TH1D("hLyz_TPCAll","hLyz_TPCAll",200,0,2000);
  TH1D *hLyz_TPCUp1 = new TH1D("hLyz_TPCUp1","hLyz_TPCUp1",200,0,2000);
  TH1D *hLyz_TPCUp2 = new TH1D("hLyz_TPCUp2","hLyz_TPCUp2",200,0,2000);
  TH1D *hLyz_TPCDown1 = new TH1D("hLyz_TPCDown1","hLyz_TPCDown1",200,0,2000);
  TH1D *hLyz_TPCDown2 = new TH1D("hLyz_TPCDown2","hLyz_TPCDown2",200,0,2000);
  TH1D *hLyz_ForwTPC1 = new TH1D("hLyz_ForwTPC1","hLyz_ForwTPC1",200,0,2000);
  TH1D *hLyz_ForwTPC2 = new TH1D("hLyz_ForwTPC2","hLyz_ForwTPC2",200,0,2000);
  TH1D *hLyz_ForwTPC3 = new TH1D("hLyz_ForwTPC3","hLyz_ForwTPC3",200,0,2000);
  TH2D *hLyzVsMom_TPCAll = new TH2D("hLyzVsMom_TPCAll","hLyzVsMom_TPCAll",200,0,2000,100,0,5000);
  TH2D *hLyzVsMom_TPCUp1 = new TH2D("hLyzVsMom_TPCUp1","hLyzVsMom_TPCUp1",200,0,2000,100,0,5000);
  TH2D *hLyzVsMom_TPCUp2 = new TH2D("hLyzVsMom_TPCUp2","hLyzVsMom_TPCUp2",200,0,2000,100,0,5000);
  TH2D *hLyzVsMom_TPCDown1 = new TH2D("hLyzVsMom_TPCDown1","hLyzVsMom_TPCDown1",200,0,2000,100,0,5000);
  TH2D *hLyzVsMom_TPCDown2 = new TH2D("hLyzVsMom_TPCDown2","hLyzVsMom_TPCDown2",200,0,2000,100,0,5000);
  TH2D *hLyzVsMom_ForwTPC1 = new TH2D("hLyzVsMom_ForwTPC1","hLyzVsMom_ForwTPC1",200,0,2000,100,0,5000);
  TH2D *hLyzVsMom_ForwTPC2 = new TH2D("hLyzVsMom_ForwTPC2","hLyzVsMom_ForwTPC2",200,0,2000,100,0,5000);
  TH2D *hLyzVsMom_ForwTPC3 = new TH2D("hLyzVsMom_ForwTPC3","hLyzVsMom_ForwTPC3",200,0,2000,100,0,5000);
  
  TH1D *hMom = new TH1D("hMom","hMom",200,0,10000); 
  TH1D *hPDG = new TH1D("hPDG","hPDG",3000,-500,2500);

  TH1D *hCut = new TH1D("hCut","hCut",10,0,10);
  hCut->GetXaxis()->SetBinLabel(1,"Evt Mode");
  hCut->GetXaxis()->SetBinLabel(2,"Evt FV");
  hCut->GetXaxis()->SetBinLabel(3,"Trk PDG");
  hCut->GetXaxis()->SetBinLabel(4,"Trk ParID");
  hCut->GetXaxis()->SetBinLabel(5,"Trk Charge");
  hCut->GetXaxis()->SetBinLabel(6,"Trk L(Targ)");
  hCut->GetXaxis()->SetBinLabel(7,"Trk Lyz(TPC)");

  // Neutrino 
  TH1D *hNuMom = new TH1D("hNuMom","hNuMom",200,0,10000); 
  TH1D *hNuPDG = new TH1D("hNuPDG","hNuPDG",40,-20,20);
  TH1D *hEvis = new TH1D("hEvis","hEvis",200,0,10000);
  TH1D *hEreco = new TH1D("hEreco","hEreco",200,0,10000);
  TH1D *hDiffEtrueEvis = new TH1D("hDiffEtrueEvis","hDiffEtrueEvis",200,-2000,2000);
  TH1D *hDiffEtrueEreco = new TH1D("hDiffEtrueEreco","hDiffEtrueEreco",200,-2000,2000);
  TH2D *hEtrueVsEvis = new TH2D("hEtrueVsEvis","hEtrueVsEvis",NBins_Mom,BinEdges_Mom,NBins_Mom,BinEdges_Mom);
  TH2D *hEtrueVsEreco = new TH2D("hEtrueVsEreco","hEtrueVsEreco",NBins_Mom,BinEdges_Mom,NBins_Mom,BinEdges_Mom);  
  TH1D *hNuReacFV = new TH1D("hNuReacFV","hNuReacFV",6,0,6);
  hNuReacFV->GetXaxis()->SetBinLabel(1,"CCQE");
  hNuReacFV->GetXaxis()->SetBinLabel(2,"2p2h");
  hNuReacFV->GetXaxis()->SetBinLabel(3,"CC1pi");
  hNuReacFV->GetXaxis()->SetBinLabel(4,"CCcoh");
  hNuReacFV->GetXaxis()->SetBinLabel(5,"CCDIS");
  hNuReacFV->GetXaxis()->SetBinLabel(6,"NC");
  
  // Vertex
  TH2D *hVtxOut_XY = new TH2D("hVtxOut_XY","hVtxOut_XY",400,-2000,2000,400,-2000,2000); // mm 
  TH2D *hVtxOut_XZ = new TH2D("hVtxOut_XZ","hVtxOut_XZ",400,-2000,2000,800,-4000,4000); // mm  
  TH2D *hVtxOut_YZ = new TH2D("hVtxOut_YZ","hVtxOut_YZ",400,-2000,2000,800,-4000,4000); // mm   
  TH1D *hVtxOut_X = new TH1D("hVtxOut_X","hVtxOut_X",400,-2000,2000); // mm  
  TH1D *hVtxOut_Y = new TH1D("hVtxOut_Y","hVtxOut_Y",400,-2000,2000); // mm    
  TH1D *hVtxOut_Z = new TH1D("hVtxOut_Z","hVtxOut_Z",800,-4000,4000); // mm 
  TH2D *hVtx_XY = new TH2D("hVtx_XY","hVtx_XY",400,-2000,2000,400,-2000,2000); // mm 
  TH2D *hVtx_XZ = new TH2D("hVtx_XZ","hVtx_XZ",400,-2000,2000,800,-4000,4000); // mm  
  TH2D *hVtx_YZ = new TH2D("hVtx_YZ","hVtx_YZ",400,-2000,2000,800,-4000,4000); // mm  
  TH1D *hVtx_X = new TH1D("hVtx_X","hVtx_X",400,-2000,2000); // mm  
  TH1D *hVtx_Y = new TH1D("hVtx_Y","hVtx_Y",400,-2000,2000); // mm    
  TH1D *hVtx_Z = new TH1D("hVtx_Z","hVtx_Z",800,-4000,4000); // mm 
  
  // # of events after each cut  
  int NTotVtxReac = 0; // Cut 1
  int NTotEvtFV = 0; // Cut 2
  int NTotTrkPassPDG = 0; // Cut 3
  int NTotTrkPassParentID = 0; // Cut 4
  int NTotTrkPassCharge = 0; // Cut 5
  int NTotTrkPassLtarget = 0; // Cut 6
  int NTotTrkPassLtpc = 0; // Cut 7
  
  // Check the inputs
  if(!doCutTarget1 && !doCutTarget2){
    cout << endl;
    cout << "You are not selecting interactions in any target !!!" << endl;
    cout << endl;
    exit(1);
  }

  TFile *finput = new TFile(infilename.c_str(),"READ");
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");
  
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
    Nentries = nevents;    
  }
  
  int EntryLast = evtfirst+Nentries-1;
  
  cout << endl;
  cout << "First event = " << evtfirst << endl;
  cout << "Last event = " << EntryLast << endl;
  cout << "# of events = " << Nentries << endl;
  cout << endl;


  for(int ievt=evtfirst;ievt<=EntryLast;ievt++){ // get last entry
    
    tinput->GetEntry(ievt);
    
    if(!(ievt%1000)){
      cout << "Event " << ievt << endl;
    }
    //nd280UpEvent->PrintEvent();

    bool PassCutVtx = false; // if at least 1 vertex does it
    bool PassCutReac = false; // if at least 1 vertex does it

    double NuEtrue = 0.;
    double NuEvis = 0.;
    double NuEreco = 0.;
    
    int NVertices = nd280UpEvent->GetNVertices();
    if(NVertices>1){
      cerr << endl;
      cerr << "More than 1 vertex is not yet supported!!!" << endl;
      cerr << endl;
      exit(1);
    }
    
    for(int ivtx=0;ivtx<NVertices;ivtx++){
      TND280UpVertex *nd280UpVertex = nd280UpEvent->GetVertex(ivtx);
      //TND280UpVertex *nd280UpVertex = new TND280UpVertex();
      //nd280UpEvent->GetVertex(ivtx)->Copy(*nd280UpVertex);
      
      TND280UpTrack *nd280UpVtxTrkIn1 = nd280UpVertex->GetInTrack(0);
      TND280UpTrack *nd280UpVtxTrkIn2 = nd280UpVertex->GetInTrack(1);
      int pdg_vtxtrk1 = nd280UpVtxTrkIn1->GetPDG();
      int pdg_vtxtrk2 = nd280UpVtxTrkIn2->GetPDG();

      //
      // Cut 1: true reaction mode
      //

      int reacmode = abs(nd280UpVertex->GetReacMode());
      int reacmode_all = GetReacAll(reacmode);
      int cut_reac_all = GetReacAll(cut_reac); 

      if(doCutReac){	
	if(reacmode_all==cut_reac){
	  PassCutReac = true;
	}
      }
      else{ // if not applied
	PassCutReac = true;
      }
      
      //
      // Cut 2: vtx in FV
      //
            
      if(fabs(pdg_vtxtrk1)==12 || 
	 fabs(pdg_vtxtrk1)==14 ||
	 fabs(pdg_vtxtrk1)==16 
	 ){
	double mom_vtxtrk1 = nd280UpVtxTrkIn1->GetInitMom().Mag();
	hNuMom->Fill(mom_vtxtrk1);
	hNuPDG->Fill(pdg_vtxtrk1);
	NuEtrue = mom_vtxtrk1;
      }
      else if(fabs(pdg_vtxtrk2)==12 || 
	      fabs(pdg_vtxtrk2)==14 ||
	      fabs(pdg_vtxtrk2)==16 
	      ){
	double mom_vtxtrk2 = nd280UpVtxTrkIn2->GetInitMom().Mag();
	hNuMom->Fill(mom_vtxtrk2);
	hNuPDG->Fill(pdg_vtxtrk2);	
	NuEtrue = mom_vtxtrk2;
      }       
      
      //
      // Cut on the target material:
      // - O16 --> 1000080160
      // - C12 --> 1000060120
      // - proton --> 2212
      // - neutron --> 2112
      //
      //if(doCutMater){
      //if(pdg_vtxtrk1 == 1000080160 || // O16
      //pdg_vtxtrk2 == 1000080160 || // O16
      //pdg_vtxtrk1 == 1000060120 || // C12
      //pdg_vtxtrk2 == 1000060120 || // C12
      //pdg_vtxtrk1 == 2212 || // proton
      //pdg_vtxtrk2 == 2212 || // proton
      //pdg_vtxtrk1 == 2112 || // neutron
      //pdg_vtxtrk2 == 2112  // neutron
      //){ 
      //
      // Interaction in target materials 
      //
      //}
      //else{
      //continue;
      //}	
      //}
      
      if(doCutVtx){
	
	bool FillVtxInFV = false;
	
	double x = nd280UpVertex->GetPosition().X();
	double y = nd280UpVertex->GetPosition().Y();
	double z = nd280UpVertex->GetPosition().Z();
	
	// Target 1 FV
	if(doCutTarget1){
	  if((x > vtx_min_x_1 && x < vtx_max_x_1) && 
	     (y > vtx_min_y_1 && y < vtx_max_y_1) && 
	     (z > vtx_min_z_1 && z < vtx_max_z_1)
	     ){
	    PassCutVtx = true;
	    FillVtxInFV = true;
	  }
	}	
	// Target 2 FV
	if(doCutTarget2){
	  if((x > vtx_min_x_2 && x < vtx_max_x_2) &&
	     (y > vtx_min_y_2 && y < vtx_max_y_2) &&
	     (z > vtx_min_z_2 && z < vtx_max_z_2)
	     ){	  
	    PassCutVtx = true;
	    FillVtxInFV = true;
	  }
	}
	//else{
	//PassCutVtx = false;
	//FillVtxInFV = false;
	//}
	
	//
	// Fill this way because only 1 vertex per event!!!
	//
	if(!FillVtxInFV){
	  hVtxOut_XY->Fill(x,y);
	  hVtxOut_XZ->Fill(x,z);
	  hVtxOut_YZ->Fill(y,z);
	  hVtxOut_X->Fill(x);
	  hVtxOut_Y->Fill(y);
	  hVtxOut_Z->Fill(z);
	}
	else{
          hVtx_XY->Fill(x,y);
          hVtx_XZ->Fill(x,z);
	  hVtx_YZ->Fill(y,z);
          hVtx_X->Fill(x);
	  hVtx_Y->Fill(y);
	  hVtx_Z->Fill(z);
	}
      }
      
      if(doCutVtx){
	if(PassCutVtx) 
	  hNuReacFV->Fill(reacmode_all);
      }
      else 
	hNuReacFV->Fill(reacmode_all);
      
   
		      
      //nd280UpVertex->PrintVertex();

      //delete nd280UpVtxTrkIn1;
      //delete nd280UpVtxTrkIn2;
      //delete nd280UpVertex;

    } // end loop over vertices
    
    if(doCutReac && !PassCutReac) continue;
    NTotVtxReac++;
    hCut->Fill(0.);
    
    if(doCutVtx && !PassCutVtx) continue;
    NTotEvtFV++; // Pass Cut 2: at least 1 vertex in FV + reaction mode
    hCut->Fill(1.);   
    
    int NTracks = nd280UpEvent->GetNTracks();
    //cout << "# of tracks = " << NTracks << " --> PDG: ";
    
    for(int itrk=0;itrk<NTracks;itrk++){
      TND280UpTrack *nd280UpTrack = nd280UpEvent->GetTrack(itrk);
           
      //nd280UpTrack->PrintTrack();
      
      int trkID = nd280UpTrack->GetTrackID();
      int parentID = nd280UpTrack->GetParentID();
      int pdg = nd280UpTrack->GetPDG();
      double charge = nd280UpTrack->GetCharge();
      double ekin = nd280UpTrack->GetInitKinEnergy();
      double mom = nd280UpTrack->GetInitMom().Mag(); 
      double costheta = nd280UpTrack->GetInitCosTheta();
      double SDlength = nd280UpTrack->GetSDLength();      
      if(SDlength<0.){ 
	cout << "SDlength = " << SDlength << " !!!" << endl;
	exit(1);
      }
      
      double length_target1 = nd280UpTrack->GetLengthTarget1();
      double length_target2 = nd280UpTrack->GetLengthTarget2();
      
      //cout << length_target1 << " " << length_target2 << endl;

      double lyz_tpcup1     = nd280UpTrack->GetLyzTPCUp1();
      double lyz_tpcup2     = nd280UpTrack->GetLyzTPCUp2();
      double lyz_tpcdown1   = nd280UpTrack->GetLyzTPCDown1();
      double lyz_tpcdown2   = nd280UpTrack->GetLyzTPCDown2();
      double lyz_forwtpc1   = nd280UpTrack->GetLyzForwTPC1();
      double lyz_forwtpc2   = nd280UpTrack->GetLyzForwTPC2();
      double lyz_forwtpc3   = nd280UpTrack->GetLyzForwTPC3();      
      
      //
      // Cut 3: particle PDG
      //
      if(doCutPDG){
	if(pdg!=cut_pdg) continue;
      }
      NTotTrkPassPDG++;
      hCut->Fill(2.);
      
      //
      // Cut 4: parent ID = 0
      //
      if(doCutParentID){
	if(parentID!=cut_parentID) continue;
      }
      NTotTrkPassParentID++;
      hCut->Fill(3.);
      
      //
      // Cut 5: particle charge
      //
      if(doCutCharge){
	if(abs(charge)!=cut_charge) continue;
      }
      NTotTrkPassCharge++;
      hCut->Fill(4.);
      

      // cout << length_target1 << ", " 
      // 	   << length_target2 << ", "
      // 	   << lyz_tpcup1 << ", "
      // 	   << lyz_tpcdown1 << ", "
      // 	   << lyz_tpcup2 << ", "
      // 	   << lyz_tpcdown2 << ", "
      // 	   << lyz_forwtpc1 << ", "
      // 	   << lyz_forwtpc2 << ", "
      // 	   << lyz_forwtpc3 
      // 	   << endl;


      // Fill selected particles in FV

      double lyz_max = 0.;
      if(lyz_tpcup1>lyz_max)   lyz_max = lyz_tpcup1;
      if(lyz_tpcup2>lyz_max)   lyz_max = lyz_tpcup2;
      if(lyz_tpcdown1>lyz_max) lyz_max = lyz_tpcdown1;
      if(lyz_tpcdown2>lyz_max) lyz_max = lyz_tpcdown2;
      if(lyz_forwtpc1>lyz_max) lyz_max = lyz_forwtpc1;
      if(lyz_forwtpc2>lyz_max) lyz_max = lyz_forwtpc2;
      if(lyz_forwtpc3>lyz_max) lyz_max = lyz_forwtpc3;

      double length_target_max = 0.;
      if(length_target1>length_target2) length_target_max = length_target1;
      else                              length_target_max = length_target2;
      
      if(lyz_max>0.){
	hLyzVsMom_TPCAll->Fill(lyz_max,mom);
	hLyz_TPCAll->Fill(lyz_max);      
      }

      if(lyz_tpcup1>0.){
	hLyz_TPCUp1->Fill(lyz_tpcup1);
	hLyzVsMom_TPCUp1->Fill(lyz_tpcup1,mom);
      }
      if(lyz_tpcup2>0.){
	hLyz_TPCUp2->Fill(lyz_tpcup2);
	hLyzVsMom_TPCUp2->Fill(lyz_tpcup2,mom);
      }
      if(lyz_tpcdown1>0.){
	hLyz_TPCDown1->Fill(lyz_tpcdown1);
	hLyzVsMom_TPCDown1->Fill(lyz_tpcdown1,mom);
      }
      if(lyz_tpcdown2>0.){
	hLyz_TPCDown2->Fill(lyz_tpcdown2);
	hLyzVsMom_TPCDown2->Fill(lyz_tpcdown2,mom);
      }      
      if(lyz_forwtpc1>0.){
	hLyz_ForwTPC1->Fill(lyz_forwtpc1);
	hLyzVsMom_ForwTPC1->Fill(lyz_forwtpc1,mom);
      }
      if(lyz_forwtpc2>0.){
	hLyz_ForwTPC2->Fill(lyz_forwtpc2);
	hLyzVsMom_ForwTPC2->Fill(lyz_forwtpc2,mom);
      }
      if(lyz_forwtpc3>0.){
	hLyz_ForwTPC3->Fill(lyz_forwtpc3);
	hLyzVsMom_ForwTPC3->Fill(lyz_forwtpc3,mom);
      }
      
      if(length_target1>0.){ 
	hL_Targ1_FV->Fill(length_target1);
	hLVsMom_Targ1_FV->Fill(length_target1,mom);
      }
      if(length_target2>0.){
	hL_Targ2_FV->Fill(length_target2);
	hLVsMom_Targ2_FV->Fill(length_target2,mom);
      }
      //if(length_target_max>0.){
      //hL_TargAll_FV->Fill(length_target_max);
      //hLVsMom_TargAll_FV->Fill(length_target_max,mom);
      //}

      hCosThetaVsMom_FV->Fill(costheta,mom);

      //
      // Cut 6 : track length in a target
      //
      bool PassCutTarget = false;
      if(doCutDLTarget){
      	if(doCutTarget1 && 
	   (length_target1 > cut_length_target_min))
	  {
	    PassCutTarget = true;	  
	  }
     	if(doCutTarget2 && 
	   (length_target2 > cut_length_target_min))
	  {
	    PassCutTarget = true;	  
	  }
      }
      else // if don't apply the cut
      	PassCutTarget = true;
	
      //
      // Cut 7: track dLyz in a TPC
      //

      //
      // Add matching with target adjacent to right Target!!!
      //

      bool PassCutTPC = true; 
      if(doCutDLyzTPC){
      	if(lyz_tpcup1 > cut_dlyz_tpc_min ||
      	   lyz_tpcup2 > cut_dlyz_tpc_min ||
      	   lyz_tpcdown1 > cut_dlyz_tpc_min ||
      	   lyz_tpcdown2 > cut_dlyz_tpc_min ||
      	   lyz_forwtpc1 > cut_dlyz_tpc_min ||
      	   lyz_forwtpc2 > cut_dlyz_tpc_min ||
      	   lyz_forwtpc3 > cut_dlyz_tpc_min
      	   ){
      	  PassCutTPC = true; 
      	}
      	else{ 
      	  PassCutTPC = false;
      	}
      }
      else{ // if don't apply the cut
      	PassCutTPC = true;
      }

      //
      // Define when particle is detected
      //
      
      bool IsDetected = false;
      
      if(PassCutTarget){
	NTotTrkPassLtarget++;
	
	if(length_target_max>0.){
	  hL_TargAll_Cut->Fill(length_target_max);
	  hLVsMom_TargAll_Cut->Fill(length_target_max,mom);
	}
	if(length_target1>0.){
	  hL_Targ1_Cut->Fill(length_target1);
	  hLVsMom_Targ1_Cut->Fill(length_target1,mom);
	}
	if(length_target2>0.){
	  hL_Targ2_Cut->Fill(length_target2);
	  hLVsMom_Targ2_Cut->Fill(length_target2,mom);
	}
	
	hCosThetaVsMom_Target->Fill(costheta,mom);
	IsDetected = true;	
	hCut->Fill(5.);	    
      }

      if(PassCutTPC){
      	NTotTrkPassLtpc++;
      	hCosThetaVsMom_TPC->Fill(costheta,mom);
      	IsDetected = true;
      	hCut->Fill(6.);
      }
      if(PassCutTarget && PassCutTPC){ 
      	hCosThetaVsMom_TargetAndTPC->Fill(costheta,mom);      
      	IsDetected = true;
      }
      if(PassCutTarget || PassCutTPC){
      	hCosThetaVsMom_TargetOrTPC->Fill(costheta,mom);
      	IsDetected = true;
      }
      
      //
      // Fill detected tracks
      //
      if(IsDetected){
      	hLVsMom_SD->Fill(SDlength,mom);
      	hL_SD->Fill(SDlength);
      	hMom->Fill(mom);
      	hPDG->Fill(pdg);
	
	double mass = 0.;
	if     (abs(pdg)==211)  mass = 139.570; // pi+-
	else if(abs(pdg)==13)   mass = 105.658; // muon
	//else if(abs(pdg)==2212) mass = 938.272; // proton
	else if(abs(pdg)==11)   mass = 0.511;   // electron
      	if( abs(pdg)==211 ||
	    abs(pdg)==13  ||
	    abs(pdg)==11  ){
	  NuEvis += ekin+mass;
	}
      }

      // delete nd280UpTrack;
    } // end loop over tracks

    //cout << endl;
    
    // Fill event histograms
    hEvis->Fill(NuEvis);
    hDiffEtrueEvis->Fill(NuEtrue-NuEvis);
    hEtrueVsEvis->Fill(NuEtrue,NuEvis);
    //hEreco->Fill();
    //hDiffEtrueEreco->Fill();
    //hEtrueVsEreco->Fill();
    //hDiffEtrueEreco->Fill();

    //delete nd280UpEvent;    
  } // end loop over events
  
   
  // Write output file
  TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);
  TFile *out = new TFile(outfilename.Data(),"RECREATE");
  hVtx_X->Write();
  hVtx_Y->Write();
  hVtx_Z->Write();
  hVtx_XY->Write();
  hVtx_XZ->Write();
  hVtx_YZ->Write();
  hVtxOut_X->Write();
  hVtxOut_Y->Write();
  hVtxOut_Z->Write();
  hVtxOut_XY->Write();
  hVtxOut_XZ->Write();
  hVtxOut_YZ->Write();  
  //
  hCosThetaVsMom_FV->Write();
  hCosThetaVsMom_Target->Write();
  hCosThetaVsMom_TPC->Write();
  hCosThetaVsMom_TargetAndTPC->Write();
  hCosThetaVsMom_TargetOrTPC->Write();
  //
  hNuReacFV->Write();
  hNuMom->Write();
  hNuPDG->Write();
  hMom->Write();
  hPDG->Write();
  //
  hL_SD->Write();
  hL_TargAll_FV->Write();
  hL_Targ1_FV->Write();
  hL_Targ2_FV->Write();
  hL_TargAll_Cut->Write();
  hL_Targ1_Cut->Write();
  hL_Targ2_Cut->Write();
  hLVsMom_SD->Write();
  hLVsMom_TargAll_FV->Write();
  hLVsMom_Targ1_FV->Write();
  hLVsMom_Targ2_FV->Write();
  hLVsMom_TargAll_Cut->Write();
  hLVsMom_Targ1_Cut->Write();
  hLVsMom_Targ2_Cut->Write();
  hLyz_TPCUp1->Write();
  hLyz_TPCUp2->Write();
  hLyz_TPCDown1->Write();
  hLyz_TPCDown2->Write();
  hLyz_ForwTPC1->Write();
  hLyz_ForwTPC2->Write();
  hLyz_ForwTPC3->Write();
  hLyzVsMom_TPCUp1->Write();  
  hLyzVsMom_TPCUp2->Write();  
  hLyzVsMom_TPCDown1->Write();  
  hLyzVsMom_TPCDown2->Write();  
  hLyzVsMom_ForwTPC1->Write();  
  hLyzVsMom_ForwTPC2->Write();  
  hLyzVsMom_ForwTPC3->Write();  
  //
  hEvis->Write();
  hDiffEtrueEvis->Write();
  hEtrueVsEvis->Write();
  //
  hCut->Write();
  out->Close();
  
  // Print output
  cout << endl;
  cout << "Total # of Tree events: " << NTreeEntries << endl;
  cout << "Total # of simulated events: " << Nentries << endl;
  cout << "Select: " << endl;
  if(doCutPDG) cout << " - PDG = " << cut_pdg << endl;
  if(doCutReac) cout << " - Reac = " << StringReacAll(cut_reac) << endl;
  if(doCutCharge) cout << " - Charge = " << cut_charge << endl;
  if(doCutDLTarget)
    cout << " - length Target > " << cut_length_target_min << " mm" << endl; 
  if(doCutDLyzTPC)
    cout << " - dLyz TPC > " << cut_dlyz_tpc_min << " mm" << endl;
  cout << endl;
  cout << "Total # of vtx after reaction mode cut = " << NTotVtxReac << endl;
  cout << "Total # of events in FV = " << NTotEvtFV << endl;
  cout << "Total # of tracks after PDG cut = " << NTotTrkPassPDG << endl;
  cout << "Total # of tracks after charge cut = " << NTotTrkPassCharge << endl;
  cout << "Total # of tracks after std cut = " 
       << hCosThetaVsMom_FV->GetEntries() << endl;
  cout << "Total # of tracks after Target cut = " 
       << hCosThetaVsMom_Target->GetEntries() << endl;
  cout << "Total # of tracks after tpc Lyz cut = " 
       << hCosThetaVsMom_TPC->GetEntries() << endl;
  cout << "Total # of tracks after Target and TPC = "
       << hCosThetaVsMom_TargetAndTPC->GetEntries() << endl;
  cout << "Total # of tracks after Target or TPC = " 
       << hCosThetaVsMom_TargetOrTPC->GetEntries() << endl;
  cout << endl;
  cout << "Calculate selection efficiencies:" << endl;
  cout << " - Target only eff = " 
       <<  hCosThetaVsMom_Target->GetEntries() / hCosThetaVsMom_FV->GetEntries() 
       << endl;
  cout << " - TPC only eff = " 
       <<  hCosThetaVsMom_TPC->GetEntries() / hCosThetaVsMom_FV->GetEntries() 
       << endl;
  cout << " - Target+TPC eff = " 
       <<  hCosThetaVsMom_TargetOrTPC->GetEntries() / hCosThetaVsMom_FV->GetEntries() 
       << endl;
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
