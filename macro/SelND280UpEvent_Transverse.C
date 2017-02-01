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
#include <TMath.h>
#include <TLorentzVector.h>

#include <iostream>
#include <string>

#include "/sps/t2k/mlamoure/dev/Upgrade/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.hh"
#include "/sps/t2k/mlamoure/dev/Upgrade/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.hh"
#include "/sps/t2k/mlamoure/dev/Upgrade/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.hh"
#include "/sps/t2k/mlamoure/dev/Upgrade/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.hh"

using namespace std;

class Event: public TObject {

public:
  int cut_passed;

  double nuEtrue;
  int nuPDG, reacMode;
  double vtxX, vtxY, vtxZ;
  
  int num_tracks, num_protons;

  double muon_momentum, muon_cos_theta;
  double proton_momentum, proton_cos_theta;
  double true_Q2;
  
  double muon_length_SD, muon_length_TPC, muon_lyz_TPC;
  double muon_length_target1,  muon_length_target2;
  double muon_length_FGD1,     muon_length_FGD2;
  double muon_length_TPCUp1,   muon_length_TPCUp2;
  double muon_length_TPCDown1, muon_length_TPCDown2;
  double muon_length_ForwTPC1, muon_length_ForwTPC2, muon_length_ForwTPC3;
  
  double proton_length_SD, proton_length_TPC, proton_lyz_TPC;
  double proton_length_target1,  proton_length_target2;
  double proton_length_FGD1,     proton_length_FGD2;
  double proton_length_TPCUp1,   proton_length_TPCUp2;
  double proton_length_TPCDown1, proton_length_TPCDown2;
  double proton_length_ForwTPC1, proton_length_ForwTPC2, proton_length_ForwTPC3;

  double true_dpT, true_dphiT, true_dalphaT;

  int muon_ECal_detPos;
  string muon_ECal_detName;
  double muon_ECal_entrance_X, muon_ECal_entrance_Y, muon_ECal_entrance_Z;

  ClassDef(Event,1)

  void Initialize() {
    
    cut_passed = -1;

    nuEtrue = -1;
    nuPDG = reacMode = -1;
    vtxX = vtxY = vtxZ = -9999.;
  
    num_tracks = num_protons = true_Q2 = 0;
    muon_momentum = proton_momentum = -9999.;
    muon_cos_theta = proton_cos_theta = -9999.;
  
    muon_length_SD = muon_length_TPC = muon_lyz_TPC = 0;
    muon_length_target1 = muon_length_target2 = 0;
    muon_length_FGD1 = muon_length_FGD2 = 0;
    muon_length_TPCUp1 = muon_length_TPCUp2 = 0;
    muon_length_TPCDown1 = muon_length_TPCDown2 = 0;
    muon_length_ForwTPC1 = muon_length_ForwTPC2 = muon_length_ForwTPC3 = 0;

    proton_length_SD = proton_length_TPC = proton_lyz_TPC = 0;
    proton_length_target1 = proton_length_target2 = 0;
    proton_length_FGD1 = proton_length_FGD2 = 0;
    proton_length_TPCUp1 = proton_length_TPCUp2 = 0;
    proton_length_TPCDown1 = proton_length_TPCDown2 = 0;
    proton_length_ForwTPC1 = proton_length_ForwTPC2 = proton_length_ForwTPC3 = 0;

    true_dpT = true_dphiT = true_dalphaT = -9999;

    muon_ECal_detPos=-1;
    muon_ECal_entrance_X = muon_ECal_entrance_Y = muon_ECal_entrance_Z = -9999;

  }

};
ClassImp(Event)


int GetReacAll(int neut_reaction_mode);
string StringReacAll(int neut_reaction_mode);
int DetectionCut(TND280UpTrack *nd280UpTrack, 
		  bool doCutTarget1, bool doCutTarget2, bool doCutDLyzTPC,
		  double cut_length_target_min, double cut_dlyz_tpc_min);

bool IsTargetIn(double x,double y,double z,
		double vtx_min_x, double vtx_max_x, 
		double vtx_min_y, double vtx_max_y, 
		double vtx_min_z, double vtx_max_z 
		);

//bool IsFGDIn(int fgdID,double x,double y,double z,
//		// Target 1                                                                                                                                                              
//              double vtx_min_x_1, double vtx_max_x_1,
//              double vtx_min_y_1, double vtx_max_y_1,
//              double vtx_min_z_1, double vtx_max_z_1,
//              // Target 2                                                                                                                                                   
//              double vtx_min_x_2, double vtx_max_x_2, 
//              double vtx_min_y_2, double vtx_max_y_2, 
//              double vtx_min_z_2, double vtx_max_z_2   );

void SelND280UpEvent_Transverse
(
 const int evtfirst = 0,
 const int nevents = 1,
 
 string tag = "outputs/prova",
 
 string infilename = "../files/g4ND280Up_6E20_classic_AfterDetUpdate_19_1_17_USECalP0D.root",

 // Definition of Fiducial Volume for target thickness=60cm (mm)  
 // Target 1
 const double vtx_min_x_1 = -1150,
 const double vtx_max_x_1 = +1150,  
 const double vtx_min_y_1 = -300, 
 const double vtx_max_y_1 = +300, 
 const double vtx_min_z_1 = -2487,  
 const double vtx_max_z_1 = -487, 
 // Target 2
 const double vtx_min_x_2 = -1150,  
 const double vtx_max_x_2 = +1150,  
 const double vtx_min_y_2 = -300, 
 const double vtx_max_y_2 = +300, 
 const double vtx_min_z_2 = +487,  
 const double vtx_max_z_2 = +2487,  
 
 // Cut 0: Select only CC mode
 const bool doCutCC = true,

 // Cut 1: Select reaction mode
 const bool doCutReac = false,
 const int cut_reac = 0, // 0=CCQE, 1=2p2h, 2=CC1pi, 3=CCcoh, 4=CCDIS, 5=NC
 
 // Cut 2: Vertex cut (inside the Targets and FGDs)
 const bool doCutVtx = true, 
 
 const bool doCutTarget1 = true,  // Select vertex in Target/FGD 1
 const bool doCutTarget2 = false, // Select vertex in Target/FGD 2

 const bool doCutVtxX = false, // width
 const double cut_xmin = 0,
 const double cut_xmax = 0,
 
 const bool doCutVtxZ = false, // length
 const double cut_zmin = 0,
 const double cut_zmax = 0,
 
 // Length cut (Targets) 
 const bool doCutDLTarget = true, // full length in target and dlyz in tpcs    
 const double cut_length_target_min = 50, //mm  
 
 // DeltaLyz cut (TPCs)
 const bool doCutDLyzTPC = true,
 const double cut_dlyz_tpc_min = 200 //mm  
 
 )
{

  // # of events after each cut  
  int NTotCC = 0; // Cut 1
  int NTotVtxReac = 0; // Cut 2
  int NTotEvtFV = 0; // Cut 3
  int NTotMuonProton = 0; // Cut 4
  
  // Check the inputs
  if(!doCutTarget1 && !doCutTarget2){
    cout << endl;
    cout << "You are not selecting interactions in any target !!!" << endl;
    cout << endl;
    exit(1);
  }
  

  TFile *f_input = new TFile(infilename.c_str(),"READ");
  TTree *t_input = (TTree*) f_input->Get("ND280upEvents");
  
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();  
  t_input->SetBranchAddress("Event",&nd280UpEvent);
  
  int NTreeEntries = t_input->GetEntries();  
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
  else
    Nentries = nevents;    

  
  int EntryLast = evtfirst+Nentries-1;
  
  cout << endl;
  cout << "First event = " << evtfirst << endl;
  cout << "Last event = " << EntryLast << endl;
  cout << "# of events = " << Nentries << endl;
  cout << endl;


  // Prepare output

  TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);
  TFile *f_output = new TFile(outfilename.Data(),"RECREATE");
  TTree *t_output = new TTree("output", "Selection muon-proton events");
  Event *ev = 0; 
  t_output->Branch("event", "Event", &ev, 64000, 2);

  TH1D *hCut = new TH1D("hCut","hCut",10,0,10);
  hCut->GetXaxis()->SetBinLabel(1,"Evt gener");
  hCut->GetXaxis()->SetBinLabel(2,"Evt CC");
  hCut->GetXaxis()->SetBinLabel(3,"Evt Mode");
  hCut->GetXaxis()->SetBinLabel(4,"Evt FV");
  hCut->GetXaxis()->SetBinLabel(5,"Muon+Proton");
  

  for (int ievt=evtfirst;ievt<=EntryLast;ievt++){ // get last entry
   
    t_input->GetEntry(ievt);
    if (!(ievt%1000))
      cout << "Event " << ievt << endl;

    ev->Initialize();
    
    //nd280UpEvent->PrintEvent();

    bool PassCutVtx  = false; // if at least 1 vertex does it
    bool PassCutReac = false; // cut on reaction mode
    bool PassCutCC   = false; // reject NC

    double NuEtrue = 0.;
    double NuEvis  = 0.;
    double NuEreco = 0.;
    
    int NVertices = nd280UpEvent->GetNVertices();
    if (NVertices>1){
      cerr << endl;
      cerr << "More than 1 vertex is not yet supported!!!" << endl;
      cerr << endl;
      exit(1);
    }

    TVector3 nu_direction(-9999, -9999, -9999);

    for (int ivtx=0;ivtx<NVertices;ivtx++){
      TND280UpVertex *nd280UpVertex   = nd280UpEvent->GetVertex(ivtx);
      TND280UpTrack *nd280UpVtxTrkIn1 = nd280UpVertex->GetInTrack(0);
      TND280UpTrack *nd280UpVtxTrkIn2 = nd280UpVertex->GetInTrack(1);
      int pdg_vtxtrk1 = nd280UpVtxTrkIn1->GetPDG();
      int pdg_vtxtrk2 = nd280UpVtxTrkIn2->GetPDG();      

      int reacmode = abs(nd280UpVertex->GetReacMode());
      int reacmode_all = GetReacAll(reacmode);
      int cut_reac_all = GetReacAll(cut_reac); 

      /*
      cout << "Reaction = " << reacmode << endl;
      cout << "ReacAll  = " << reacmode_all << endl;
      cout << "PDG1     = " << pdg_vtxtrk1 << endl;
      cout << "PDG2     = " << pdg_vtxtrk2 << endl;
      */

      // ================================================
      // Cut 0: don't consider NC events
      // ================================================
      
      if(doCutCC && reacmode_all==5) PassCutCC = false; 
      else                           PassCutCC = true;

      
      // ================================================
      // Cut 1: true reaction mode
      // ================================================
      
      if(doCutReac && reacmode_all==cut_reac) PassCutReac = true;
      else                                    PassCutReac = false;

      
      // ================================================
      // Cut 2: vtx in FV
      // ================================================
      
      if (fabs(pdg_vtxtrk1)==12 || fabs(pdg_vtxtrk1)==14 || fabs(pdg_vtxtrk1)==16){
	double mom_vtxtrk1 = nd280UpVtxTrkIn1->GetInitMom().Mag();
	ev->nuEtrue = mom_vtxtrk1;
	ev->nuPDG   = pdg_vtxtrk1;
	nu_direction = nd280UpVtxTrkIn1->GetInitMom();
      }
      else if (fabs(pdg_vtxtrk2)==12 || fabs(pdg_vtxtrk2)==14 || fabs(pdg_vtxtrk2)==16){
	double mom_vtxtrk2 = nd280UpVtxTrkIn2->GetInitMom().Mag();
	ev->nuEtrue = mom_vtxtrk2;
	ev->nuPDG   = pdg_vtxtrk2;
	nu_direction = nd280UpVtxTrkIn2->GetInitMom();
      }       
         
      if(doCutVtx){
	
	bool FillVtxInFV = false;
	
        ev->vtxX = nd280UpVertex->GetPosition().X();
        ev->vtxY = nd280UpVertex->GetPosition().Y();
	ev->vtxZ = nd280UpVertex->GetPosition().Z();

	// Require only 1 target as FV
	int NumOfFVTargets = 0;
	if(doCutTarget1) NumOfFVTargets++;
	if(doCutTarget2) NumOfFVTargets++;
	//if(doCutFGD1)    NumOfFVTargets++;
	//if(doCutFGD2)    NumOfFVTargets++;
	if(NumOfFVTargets!=1){
	  cerr << endl;
	  cerr << "More than one target are FV!!!" << endl;
	  cerr << endl;
	  exit(1);
	}

	// Target 1 FV
	if(doCutTarget1){
	  PassCutVtx = IsTargetIn(ev->vtxX, ev->vtxY, ev->vtxZ,
				  vtx_min_x_1,
				  vtx_max_x_1,  
				  vtx_min_y_1, 
				  vtx_max_y_1, 
				  vtx_min_z_1,  
				  vtx_max_z_1 
				  );
	  FillVtxInFV = PassCutVtx;
	}

	// Target 2 FV
	if(doCutTarget2){
	  PassCutVtx = IsTargetIn(ev->vtxX, ev->vtxY, ev->vtxZ,
				  vtx_min_x_2,
				  vtx_max_x_2,  
				  vtx_min_y_2, 
				  vtx_max_y_2, 
				  vtx_min_z_2,  
				  vtx_max_z_2 
				  );
	  FillVtxInFV = PassCutVtx;
	}


	// Cut on X vertex position
	if(doCutVtxX){
	  if( ev->vtxX < cut_xmin || 
	      ev->vtxX > cut_xmax ){
	    PassCutVtx = false;
	    FillVtxInFV = false;
	  }
	}

	// Cut on Z vertex position
	if(doCutVtxZ){
	  if( ev->vtxZ < cut_zmin || 
	      ev->vtxZ > cut_zmax ){
	    PassCutVtx = false;
	    FillVtxInFV = false;
	  }
	}

      }
      
      ev->reacMode = reacmode_all;

    } // end loop over vertices

    hCut->Fill(0.);
    ev->cut_passed=0;

    if(doCutCC && !PassCutCC) {t_output->Fill(); continue;}
    NTotCC++;
    hCut->Fill(1.);
    ev->cut_passed=1;
    
    if(doCutReac && !PassCutReac && ev->cut_passed==1) {t_output->Fill(); continue;};
    NTotVtxReac++;
    hCut->Fill(2.);
    ev->cut_passed=2;
    
    if(doCutVtx && !PassCutVtx && ev->cut_passed==2) {t_output->Fill(); continue;}
    NTotEvtFV++; // Pass Cut 2: at least 1 vertex in FV + reaction mode
    hCut->Fill(3.);
    ev->cut_passed=3;
    

    int NTracks = nd280UpEvent->GetNTracks();
    ev->num_tracks  = NTracks;
    ev->num_protons = 0;

    //====================================
    //====================================
    //====================================
    //====================================

    TND280UpTrack* muon   = NULL;
    TND280UpTrack* proton = NULL;
    bool detected_muon = false, detected_proton = false;
    double mom_muon=0, mom_proton=0;

    for(int itrk=0;itrk<NTracks;itrk++) {

      TND280UpTrack *nd280UpTrack = nd280UpEvent->GetTrack(itrk);
      
      double mom = nd280UpTrack->GetInitMom().Mag(); 
      int pdg = nd280UpTrack->GetPDG();

      int detection = DetectionCut(nd280UpTrack, 
				   doCutTarget1, doCutTarget2, doCutDLyzTPC,
				   cut_length_target_min, cut_dlyz_tpc_min);

      if (pdg == 2212 and mom>10) {
	if (ev->num_protons == 0) {
	  proton = nd280UpTrack;
	  mom_proton = mom;
	}
	if (detection >= 1 and mom >= mom_proton) {
	  proton = nd280UpTrack;
	  detected_proton = true;
	  mom_proton = mom;
	}
	ev->num_protons++;
      }

      if (pdg == 13) {
	if (!muon) {
	  muon = nd280UpTrack;
	  mom_muon = mom;
	}
	if (detection >= 2 and mom >= mom_muon) {
	  muon = nd280UpTrack;
	  detected_muon = true;
	  mom_muon = mom;
	}
      }

    }

    
    // ================================================
    // Cut 4: consider only events with detected muon and proton
    // ================================================

    if (detected_proton && detected_muon && ev->cut_passed==3) {
      NTotMuonProton++;
      hCut->Fill(4.);
      ev->cut_passed=4;
    }

    // Fill the variables for muon and proton

    if (muon) {

      ev->muon_momentum  = muon->GetInitMom().Mag();
      ev->muon_cos_theta = muon->GetInitMom().CosTheta();

      ev->muon_length_SD       = muon->GetSDLength();
      ev->muon_length_target1  = muon->GetLengthTarget1();
      ev->muon_length_target2  = muon->GetLengthTarget2();
      ev->muon_length_FGD1     = muon->GetLengthFGD1();
      ev->muon_length_FGD2     = muon->GetLengthFGD2();
      ev->muon_length_TPCUp1   = muon->GetLengthTPCUp1();
      ev->muon_length_TPCUp2   = muon->GetLengthTPCUp2();
      ev->muon_length_TPCDown1 = muon->GetLengthTPCDown1();
      ev->muon_length_TPCDown2 = muon->GetLengthTPCDown2();
      ev->muon_length_ForwTPC1 = muon->GetLengthForwTPC1();
      ev->muon_length_ForwTPC2 = muon->GetLengthForwTPC3();
      ev->muon_length_ForwTPC3 = muon->GetLengthForwTPC3();

      ev->muon_length_TPC = muon->GetLengthTPCUp1() + muon->GetLengthTPCUp2() +
	muon->GetLengthTPCDown1() + muon->GetLengthTPCDown2() + 
	muon->GetLengthForwTPC1() + muon->GetLengthForwTPC3() + 
	muon->GetLengthForwTPC3();
      ev->muon_lyz_TPC = muon->GetLyzTPCUp1() + muon->GetLyzTPCUp2() +
	muon->GetLyzTPCDown1() + muon->GetLyzTPCDown2() + 
	muon->GetLyzForwTPC1() + muon->GetLyzForwTPC3() + 
	muon->GetLyzForwTPC3();


      //=======================================

      int NPoints = muon->GetNPoints();    
      TND280UpTrackPoint* firstECal=NULL;
      double ECal_max_mom = -9999;
      int ECal_detPos = 0;
      string ECal_detName;
	  
      for(int ipt=0;ipt<NPoints;ipt++){
		  
       	TND280UpTrackPoint *nd280UpTrackPoint = muon->GetPoint(ipt);
	TString detName(nd280UpTrackPoint->GetPhysVolName());
	if (!detName.Contains("ECal")) 
	  continue;
	  
	if (nd280UpTrackPoint->GetMomentum().Mag() > ECal_max_mom) {
	  firstECal    = nd280UpTrackPoint;
	  ECal_detName = detName;
	  ECal_max_mom = nd280UpTrackPoint->GetMomentum().Mag();
	  if (detName.Contains("BrlECal") && detName.Contains("RightClam") && detName.Contains("BotLeftTopRight"))
	    ECal_detPos = 1; // (+Y)
	  else if (detName.Contains("BrlECal") && detName.Contains("RightClam")  && detName.Contains("TopLeftBotRight"))
	    ECal_detPos = 2; // (-Y) 
	  else if (detName.Contains("BrlECal") && detName.Contains("LeftClam")  && detName.Contains("BotLeftTopRight"))
	    ECal_detPos = 3; // (-Y)
	  else if (detName.Contains("BrlECal") && detName.Contains("LeftClam") && detName.Contains("TopLeftBotRight"))
	    ECal_detPos = 4; // (+Y)
	  else if (detName.Contains("BrlECal") && detName.Contains("LeftSide"))
	    ECal_detPos = 5; // (+X)
	  else if (detName.Contains("BrlECal") && detName.Contains("RightSide"))
	    ECal_detPos = 6; // (-X)
	  else if (detName.Contains("DsECal"))
	    ECal_detPos = 7; // (+Z)
	  else
	    ECal_detPos = 8;
	}
	  
      }

      if (firstECal) {
	ev->muon_ECal_entrance_X = firstECal->GetPrePosition().X();
	ev->muon_ECal_entrance_Y = firstECal->GetPrePosition().Y();
	ev->muon_ECal_entrance_Z = firstECal->GetPrePosition().Z();
	ev->muon_ECal_detPos  = ECal_detPos;
	ev->muon_ECal_detName = ECal_detName;
      }
      
      //=======================================

    }

    if (proton) {

      ev->proton_momentum  = proton->GetInitMom().Mag();
      ev->proton_cos_theta = proton->GetInitMom().CosTheta();

      ev->proton_length_SD       = proton->GetSDLength();
      ev->proton_length_target1  = proton->GetLengthTarget1();
      ev->proton_length_target2  = proton->GetLengthTarget2();
      ev->proton_length_FGD1     = proton->GetLengthFGD1();
      ev->proton_length_FGD2     = proton->GetLengthFGD2();
      ev->proton_length_TPCUp1   = proton->GetLengthTPCUp1();
      ev->proton_length_TPCUp2   = proton->GetLengthTPCUp2();
      ev->proton_length_TPCDown1 = proton->GetLengthTPCDown1();
      ev->proton_length_TPCDown2 = proton->GetLengthTPCDown2();
      ev->proton_length_ForwTPC1 = proton->GetLengthForwTPC1();
      ev->proton_length_ForwTPC2 = proton->GetLengthForwTPC3();
      ev->proton_length_ForwTPC3 = proton->GetLengthForwTPC3();

      ev->proton_length_TPC = proton->GetLengthTPCUp1() + proton->GetLengthTPCUp2() +
	proton->GetLengthTPCDown1() + proton->GetLengthTPCDown2() + 
	proton->GetLengthForwTPC1() + proton->GetLengthForwTPC3() + 
	proton->GetLengthForwTPC3();
      ev->proton_lyz_TPC = proton->GetLyzTPCUp1() + proton->GetLyzTPCUp2() +
	proton->GetLyzTPCDown1() + proton->GetLyzTPCDown2() + 
	proton->GetLyzForwTPC1() + proton->GetLyzForwTPC3() + 
	proton->GetLyzForwTPC3();

    }

    if (muon and proton) {    

      TVector3   muon_mom = muon->GetInitMom();
      TVector3 proton_mom = proton->GetInitMom();

      TVector3 pT_muon   = muon_mom - nu_direction.Unit().Dot(muon_mom)*nu_direction.Unit();
      TVector3 pT_proton = proton_mom - nu_direction.Unit().Dot(proton_mom)*nu_direction.Unit();
      TVector3 pT        = pT_muon + pT_proton;

      TLorentzVector P_muon(-9999, -9999, -9999, -9999);
      P_muon.SetVectM(muon_mom, 105.66);
      TLorentzVector P_nu(-9999, -9999, -9999, -9999);
      P_nu.SetVectM(nu_direction, 0);

      ev->true_Q2 = -(P_muon-P_nu).Mag2()/1e6;
      ev->true_dpT     = pT.Mag();
      ev->true_dphiT   = pT_muon.Angle(-pT_proton);
      ev->true_dalphaT = pT.Angle(-pT_proton);

    }

    t_output->Fill();
     
  } // end loop over events
  
   
  // Write output file

  t_output->Write();
  hCut->Write();
  f_output->Close();
  
  // Print output
  cout << endl;
  cout << "Total # of Tree events: " << NTreeEntries << endl;
  cout << "Total # of simulated events: " << Nentries << endl;
  cout << "Select: " << endl;
  if(doCutCC) 
    cout << " - Select CC events" << endl;
  if(doCutReac) 
    cout << " - Reac = " << StringReacAll(cut_reac) << endl;
  if(doCutDLTarget)
    cout << " - length Target > " << cut_length_target_min << " mm" << endl; 
  if(doCutDLyzTPC)
    cout << " - dLyz TPC > " << cut_dlyz_tpc_min << " mm" << endl;
  cout << endl;

  cout << "Total # of vtx after CC cut = " << NTotCC << endl;
  cout << "Total # of vtx after reaction mode cut = " << NTotVtxReac << endl;
  cout << "Total # of events in FV = " << NTotEvtFV << endl;
  cout << "Total # of events with muon and proton = " << NTotMuonProton << endl;

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
  is_not_nc = is_not_nc or Is2p2h(neut_reaction_mode);
  is_not_nc = is_not_nc or IsCCcoh(neut_reaction_mode);
  is_not_nc = is_not_nc or IsCC1pi(neut_reaction_mode);
  is_not_nc = is_not_nc or IsCCQE(neut_reaction_mode);
  
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
		double vtx_min_x, double vtx_max_x,  
		double vtx_min_y, double vtx_max_y, 
		double vtx_min_z, double vtx_max_z 
){
 
  if((x > vtx_min_x && x < vtx_max_x) && 
     (y > vtx_min_y && y < vtx_max_y) && 
     (z > vtx_min_z && z < vtx_max_z) )
    return true;
  else
    return false;
  
}


int DetectionCut(TND280UpTrack *nd280UpTrack, 
		 bool doCutTarget1, bool doCutTarget2, bool doCutDLyzTPC,
		 double cut_length_target_min, double cut_dlyz_tpc_min) {

  int trkID = nd280UpTrack->GetTrackID();
  int parentID = nd280UpTrack->GetParentID();
  int pdg = nd280UpTrack->GetPDG();
  double charge = nd280UpTrack->GetCharge();
  double ekin = nd280UpTrack->GetInitKinEnergy();
  double mom = nd280UpTrack->GetInitMom().Mag(); 
    
  if(parentID   != 0) return false;
  if(abs(charge)!= 1) return false;

  double dirX = nd280UpTrack->GetInitMom().X() / mom;
  double dirY = nd280UpTrack->GetInitMom().Y() / mom;
  double dirZ = nd280UpTrack->GetInitMom().Z() / mom;

  double phi = atan2(dirY,dirX);
  double cosphi = cos(phi);
  if(cosphi<-1 || cosphi>+1){
    cerr << endl;
    cerr << "cosphi = " << cosphi << " !!!" << endl;
    cerr << endl;
    exit(1);
  }

  double costheta = dirZ;
  double theta = acos(costheta);
  if(costheta<-1 || costheta>+1){
    cerr << endl;
    cerr << "costheta = " << costheta << " !!!" << endl;
    cerr << endl;
    exit(1);
  }

  double SDlength = nd280UpTrack->GetSDLength();      
  if(SDlength<0.){ 
    cout << "SDlength = " << SDlength << " !!!" << endl;
    exit(1);
  }
   
  double length_target1  = nd280UpTrack->GetLengthTarget1();
  double length_target2  = nd280UpTrack->GetLengthTarget2();
  double length_tpcup1   = nd280UpTrack->GetLengthTPCUp1();
  double length_tpcup2   = nd280UpTrack->GetLengthTPCUp2();
  double length_tpcdown1 = nd280UpTrack->GetLengthTPCDown1();
  double length_tpcdown2 = nd280UpTrack->GetLengthTPCDown2();
  double length_forwtpc1 = nd280UpTrack->GetLengthForwTPC1();
  double length_forwtpc2 = nd280UpTrack->GetLengthForwTPC2();
  double length_forwtpc3 = nd280UpTrack->GetLengthForwTPC3();      
      
  double lyz_tpcup1     = nd280UpTrack->GetLyzTPCUp1();
  double lyz_tpcup2     = nd280UpTrack->GetLyzTPCUp2();
  double lyz_tpcdown1   = nd280UpTrack->GetLyzTPCDown1();
  double lyz_tpcdown2   = nd280UpTrack->GetLyzTPCDown2();
  double lyz_forwtpc1   = nd280UpTrack->GetLyzForwTPC1();
  double lyz_forwtpc2   = nd280UpTrack->GetLyzForwTPC2();
  double lyz_forwtpc3   = nd280UpTrack->GetLyzForwTPC3();      
      
  //
  // track length in a target
  //
  bool PassCutTarget = false;
  if(doCutTarget1 && (length_target1 > cut_length_target_min))
    PassCutTarget = true;	  
  if(doCutTarget2 && (length_target2 > cut_length_target_min))
    PassCutTarget = true;
  else if (!doCutTarget1 && !doCutTarget2)
    PassCutTarget = true;
 
  //
  // track dLyz in a TPC
  //

  bool PassCutTPC = false; 
  if(doCutDLyzTPC){
    if(lyz_tpcup1 > cut_dlyz_tpc_min   || 
       lyz_tpcup2 > cut_dlyz_tpc_min   ||
       lyz_tpcdown1 > cut_dlyz_tpc_min || 
       lyz_tpcdown2 > cut_dlyz_tpc_min ||
       lyz_forwtpc1 > cut_dlyz_tpc_min || 
       lyz_forwtpc2 > cut_dlyz_tpc_min ||
       lyz_forwtpc3 > cut_dlyz_tpc_min ){
      PassCutTPC = true; 
    }
  }
  else // if don't apply the cut
    PassCutTPC = true;
  
    
  //
  // Define when particle is detected
  //
      
  int detection = 0;
      
  if(PassCutTarget)
    detection += 1;	   
      
  if(PassCutTPC)
    detection += 2;

  return detection;

}
