
// Davide Sgalaberna 10/7/17

//////////////////////////////////////////////////////////////////
//                                                              //
// TODO GENERAL:                                                //
// - use floats instead of doubles                              //
// - other TODOs along the code --> DONE                        // 
// - apply cut on track length --> DONE                         // 
// - apply cut on FV --> DONE                                   //
// - use TSpline3 instead of TF1 --> DONE                       //
// - don't use Birk for neutrons? --> DONE                      // 
// - Add Poisson fluct to response --> DONE                     //
// - Ereco and momentum                                         //
// - Fill the TND280UpRecoTrack class w/ MPPC hits (NOT NEEDED) //
// - Store ND280UpRecoEvent object Tree in output file          //
// - Take the ND280UpRecoEvent objects Tree from input file     //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TTree.h>
#include <TVector3.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TMath.h>
#include <TStopwatch.h>
#include <TPolyMarker3D.h>
#include <TMarker.h>

#include <iostream>
#include <fstream>

#include "ND280UpConst.hh"
#include "ND280UpApplyResponse.hh"
#include "ND280UpRecoTrack.hh"
#include "ND280UpPID.hh"

#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"

#include "TrackReconstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline int FindIndex(vector<int> vec,int val){  
  vector<int>::iterator it;
  it=find(vec.begin(),vec.end(),val);
  int pos = distance(vec.begin(), it);  
  if(it!=vec.end()){
    //cout<<"FOUND  "<< *it<<"  at position: "<<pos<<endl;
  }
  else{
    cout<<"NOT FOUND"<<endl;
  }  
  return pos;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void AddGraph(TGraph *gAll,TGraph *gSubSet){
  
  //cout << "gSubSet->GetN() = " << gSubSet->GetN() << endl;
  
  for(int ipt=0;ipt<gSubSet->GetN();ipt++){
    double px,py;
    gSubSet->GetPoint(ipt,px,py);
    gAll->SetPoint(gAll->GetN(),px,py);
  }

  //cout << "gAll->GetN() = " << gAll->GetN() << endl;
  
  return;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PrintDaughters(TND280UpEvent *event,int mytrkid){
  int NTracks = event->GetNTracks();

  bool foundtrack = false;
  
  cout << endl;
  cout << "Print the Truth Daughters:" << endl;

  int EvtID = event->GetEventID();

  cout << "Event ID: " << EvtID << endl;

  for(int itrk=0;itrk<NTracks;itrk++){
    //cout << "itrk " << itrk << endl;
    TND280UpTrack *nd280UpTrack = event->GetTrack(itrk);
    int trkID = nd280UpTrack->GetTrackID();
    int parentID = nd280UpTrack->GetParentID();
    int pdg = nd280UpTrack->GetPDG();
    double charge = nd280UpTrack->GetCharge();
    double ekin = nd280UpTrack->GetInitKinEnergy();
    double mom = nd280UpTrack->GetInitMom().Mag();
    double dirX = nd280UpTrack->GetInitMom().X() / mom;
    double dirY = nd280UpTrack->GetInitMom().Y() / mom;
    double dirZ = nd280UpTrack->GetInitMom().Z() / mom;
    double costheta = dirZ;
    
    int NPoints = nd280UpTrack->GetNPoints();
    string process = nd280UpTrack->GetProcessName();
    
    if(mytrkid==parentID){

      double length = 0.;
      for(int ipt=0;ipt<NPoints;ipt++){
	TND280UpTrackPoint *nd280UpTrackPoint = nd280UpTrack->GetPoint(ipt);
	string detname = nd280UpTrackPoint->GetLogVolName();
	//cout << detname << " --> " << nd280UpTrackPoint->GetStepLength() << endl;
	length += nd280UpTrackPoint->GetStepLength();
      }
      
      foundtrack = true;
    
      cout << "trkid: " << trkID
	   << ", pdg: " << pdg 
	   << ", parent: " << parentID
	   << ", ekin: " << ekin
	   << ", costheta = " << costheta 
	   << ", mom = " << mom
	   << ", SD length = " << length
	//<< ", detname = " <<  nd280UpTrackPoint->GetLogVolName()
	   << ", process = " << process
	   << endl;
      
    }
  }

  if(!foundtrack){
    cerr << "Error in PrintDaughters()" << endl;
    cerr << "Look for Track ID: " << mytrkid << endl;
    cerr << "The Truth Track is not found!!!" << endl;
    exit(1);
  }
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PrintTruth(TND280UpEvent *event,int mytrkid){
  int NTracks = event->GetNTracks();

  bool foundtrack = false;
  
  cout << endl;
  cout << "%%%%%%%%%%%%%%%%%%%%" << endl;
  cout << "Print the truth:" << endl;

  int EvtID = event->GetEventID();

  cout << "Event ID: " << EvtID << endl;
  
  cout << " # of primary tracks: " << NTracks << endl;

  for(int itrk=0;itrk<NTracks;itrk++){
    //cout << "itrk " << itrk << endl;
    TND280UpTrack *nd280UpTrack = event->GetTrack(itrk);
    int trkID = nd280UpTrack->GetTrackID();
    int parentID = nd280UpTrack->GetParentID();
    int pdg = nd280UpTrack->GetPDG();
    double charge = nd280UpTrack->GetCharge();
    double ekin = nd280UpTrack->GetInitKinEnergy();
    double mom = nd280UpTrack->GetInitMom().Mag();
    double dirX = nd280UpTrack->GetInitMom().X() / mom;
    double dirY = nd280UpTrack->GetInitMom().Y() / mom;
    double dirZ = nd280UpTrack->GetInitMom().Z() / mom;
    double costheta = dirZ;
    
    double sdtotenergy = nd280UpTrack->GetSDTotalEnergyDeposit();

    int NPoints = nd280UpTrack->GetNPoints();
    string process = nd280UpTrack->GetProcessName();

    if(parentID>0) continue;
    
    if(mytrkid==trkID ||
       mytrkid==-999){

      cout << endl;
      cout << "trkid: " << trkID
	   << ", pdg: " << pdg 
	   << ", parent: " << parentID
	   << ", ekin: " << ekin
	   << ", costheta = " << costheta 
	   << ", mom = " << mom
	   << ", sdtotenergy = " << sdtotenergy
	   << ", process = " << process
	   << endl;

      double length = 0.;
      for(int ipt=0;ipt<NPoints;ipt++){
	TND280UpTrackPoint *nd280UpTrackPoint = nd280UpTrack->GetPoint(ipt);
	string detname = nd280UpTrackPoint->GetLogVolName();
	//cout << detname << " --> " << nd280UpTrackPoint->GetStepLength() << endl;
	length += nd280UpTrackPoint->GetStepLength();
	cout << detname << endl;
      }

      cout << ", SD length = " << length << endl;
	   //<< ", detname = " <<  nd280UpTrackPoint->GetLogVolName()
      
      foundtrack = true;
      
    }
  }

  if(!foundtrack){
    cerr << "Error in PrintTruth()" << endl;
    cerr << "Look for Track ID: " << mytrkid << endl;
    cerr << "The Truth Track is not found!!!" << endl;
    exit(1);
  }
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



inline void FindVtxTruth(TND280UpEvent *event,double &myVtxX,double &myVtxY,double &myVtxZ){  
  
  TND280UpVertex *nd280UpVtx = event->GetVertex(0);
  
  myVtxX = nd280UpVtx->GetPosition().X();
  myVtxY = nd280UpVtx->GetPosition().Y();
  myVtxZ = nd280UpVtx->GetPosition().Z();
    
  return;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void FindTruth(TND280UpEvent *event,int mytrkid,double &mymom,double &mycosth,double &myphi,double &mylength){
  int NTracks = event->GetNTracks();

  bool foundtrack = false;
  
  for(int itrk=0;itrk<NTracks;itrk++){
    //cout << "itrk " << itrk << endl;
    TND280UpTrack *nd280UpTrack = event->GetTrack(itrk);
    int trkID = nd280UpTrack->GetTrackID();
    int parentID = nd280UpTrack->GetParentID();
    int pdg = nd280UpTrack->GetPDG();
    double charge = nd280UpTrack->GetCharge();
    double ekin = nd280UpTrack->GetInitKinEnergy();
    double mom = nd280UpTrack->GetInitMom().Mag();
    double dirX = nd280UpTrack->GetInitMom().X() / mom;
    double dirY = nd280UpTrack->GetInitMom().Y() / mom;
    double dirZ = nd280UpTrack->GetInitMom().Z() / mom;
    double costheta = dirZ;
    
    double phi = atan2(dirY,dirX);
    double cosphi = cos(phi);
    if(cosphi<-1 || cosphi>+1){
      cerr << endl;
      cerr << "cosphi = " << cosphi << " !!!" << endl;
      cerr << endl;
      exit(1);
    }
    
    int NPoints = nd280UpTrack->GetNPoints();
    
    //cout << "trkid: " << trkID
    //<< ",pdg: " << pdg 
    //<< ", ekin: " << ekin
    //<< ", costheta_track = " << costheta 
    //<< ", mom_track = " << mom
    //<< ", detname = " <<  nd280UpTrackPoint->GetLogVolName()
    //<< endl;

    if(mytrkid==trkID){

      double length = 0.;
      //for(int ipt=0;ipt<NPoints;ipt++){
      //TND280UpTrackPoint *nd280UpTrackPoint = nd280UpTrack->GetPoint(ipt);
      //string detname = nd280UpTrackPoint->GetLogVolName();
      //cout << detname << " --> " << nd280UpTrackPoint->GetStepLength() << endl;
      //length += nd280UpTrackPoint->GetStepLength();
      //}
      
      mymom = mom;
      mycosth = costheta;
      myphi = phi;
      mylength = length;
      foundtrack = true;
    }
  }

  if(!foundtrack){
    cerr << "Error in FindTruth()" << endl;
    cerr << "Look for Track ID: " << mytrkid << endl;
    cerr << "The Truth Track is not found!!!" << endl;
    exit(1);
  }
  
  return;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline bool IsLastPtMomZero(TND280UpEvent *event,int mytrkid){

  int NTracks = event->GetNTracks();

  bool foundtrack = false;

  for(int itrk=0;itrk<NTracks;itrk++){
    //cout << "itrk " << itrk << endl;
    TND280UpTrack *nd280UpTrack = event->GetTrack(itrk);
    int trkID = nd280UpTrack->GetTrackID();
    int pdg = nd280UpTrack->GetPDG();
    double mom = nd280UpTrack->GetInitMom().Mag();
    int NPoints = nd280UpTrack->GetNPoints();
    
    if(mytrkid==trkID){

      foundtrack = true;
      
      TND280UpTrackPoint *nd280UpTrackPoint_first = nd280UpTrack->GetPoint(0);      
      TND280UpTrackPoint *nd280UpTrackPoint_last  = nd280UpTrack->GetPoint(NPoints-1);      
      double laststep_mom = nd280UpTrackPoint_last->GetMomentum().Mag();
      if(laststep_mom<0.0001) return true;
      else{
	if(pdg==13 || pdg==211 || pdg==2212 || pdg==11){
	  cout << endl;
	  cout << "PDG = " << pdg
	       << ", detname first = " << nd280UpTrackPoint_first->GetLogVolName()
	       << ", detname last = " <<  nd280UpTrackPoint_last->GetLogVolName()
	       << ", laststep_mom = " << laststep_mom 
	       << endl;
	}
      }
    }
  }

  if(!foundtrack){
    cerr << "Error in FindTruth()" << endl;
    cerr << "Look for Track ID: " << mytrkid << endl;
    cerr << "The Truth Track is not found!!!" << endl;
    exit(1);
  }
  
  return false;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int TrackReconstruction(int argc,char** argv)
{   
  
  if (argc!=14){   // batch mode                                        
    cout << "You need to provide the following arguments:" << endl;
    cout << " 1) input ROOT file name (from GEANT4 simulation) " << endl;
    cout << " 2) first event number to run" << endl;
    cout << " 3) total number of events to run" << endl;
    cout << " 4) the tag for the output file name" << endl;
    cout << " 5) detector ID: 0-->SuperFGD, 1-->FGD-like, 2-->WAGASCI" << endl;
    cout << " 6) set debug option" << endl;
    cout << " 7) set debug plot option" << endl;
    cout << " 8) use view XY (0 or 1)" << endl;
    cout << " 9) use view XZ (0 or 1)" << endl;
    cout << " 10) use view YZ (0 or 1)" << endl;
    cout << " 11) Minimum track distance (if <=0 use default)" << endl;
    cout << " 12) Look only at tracks inside the Truth FV (use MC hits)" << endl;
    cout << " 13) Output file name. In iteractive run it can be first event, but for parallel need separate param" << endl;
    exit(1);
  }

  std::string prefix = "_tr";
  
  string rootfilename = argv[1];
  const int evtfirst = atoi(argv[2]);
  const int nevents = atoi(argv[3]);
  string tag = argv[4];
  tag += prefix;
  const int detectorID = atoi(argv[5]);
  const int DEBUG = atoi(argv[6]);
  const int DEBUGPLOT = atoi(argv[7]);
  const int UseViewXY = atoi(argv[8]);
  const int UseViewXZ = atoi(argv[9]);
  const int UseViewYZ = atoi(argv[10]);
  const double MinSeparation = atof(argv[11]); // if <0 --> use the default one
  const int UseTruthFV = atoi(argv[12]);


  // Set the inputs

  nd280upconv::TargetType_t DetType;
  if     (detectorID == 0) DetType = nd280upconv::kSuperFGD;
  else if(detectorID == 1) DetType = nd280upconv::kFGDlike;
   
  //
  
  const int NEvtDisplTot = 50; //atoi(argv[5]); // max # of evt. displays 

  int NTOTALTRACKSALL_1stLOOP = 0;
  int NTOTALTRACKSALL_2ndLOOP = 0;
  int NTOTALINFV_2ndLOOP_mu = 0;
  int NTOTALINFV_2ndLOOP_pi = 0;
  int NTOTALINFV_2ndLOOP_prot = 0;
  int NTOTALINFV_2ndLOOP_elec = 0;
  int NTOTALRECO_2ndLOOP_mu = 0;
  int NTOTALRECO_2ndLOOP_pi = 0;
  int NTOTALRECO_2ndLOOP_prot = 0;
  int NTOTALRECO_2ndLOOP_elec = 0;

  // Declare variable of reconstructed tracks
  vector<TH2F*> fRecoTrack_MPPCHit_XY;
  vector<TH2F*> fRecoTrack_MPPCHit_XZ;
  vector<TH2F*> fRecoTrack_MPPCHit_YZ;
  vector<TGraph*> fTrueTrack_MCHit_XY;
  vector<TGraph*> fTrueTrack_MCHit_XZ;
  vector<TGraph*> fTrueTrack_MCHit_YZ; 
  vector<int>   fRecoTrack_ID;
  vector<int>   fRecoTrack_PDG;
  vector<int>   fRecoTrack_ParentID;
  vector<bool> fRecoTrack_outfv;
  vector<bool> fRecoTrack_isreco;
  vector<double> fRecoTrack_reco_costh;
  vector<double> fRecoTrack_reco_mom;
  vector<double> fRecoTrack_reco_length;
  vector<double> fRecoTrack_true_length;
  vector<double> fRecoTrack_true_edep;

  vector< vector< vector<double> > > fRecoTrack_hitXY; // track< hit< (pos,edep) > >  
  //vector< vector<double*> > fRecoTrack_hitXZ;
  //vector< vector<double*> > fRecoTrack_hitYZ;

  // Declare canvases
  TCanvas *cMPPCHits_XY[NEvtDisplTot]; 
  TCanvas *cMPPCHits_XZ[NEvtDisplTot]; 
  TCanvas *cMPPCHits_YZ[NEvtDisplTot];

  //
  // Declare histograms
  //

  // Event displays

  TH2F *hMPPCHits_XY[NEvtDisplTot]; 
  TH2F *hMPPCHits_XZ[NEvtDisplTot]; 
  TH2F *hMPPCHits_YZ[NEvtDisplTot];
  
  TGraph *gMCHits_XY[NEvtDisplTot]; 
  TGraph *gMCHits_XZ[NEvtDisplTot]; 
  TGraph *gMCHits_YZ[NEvtDisplTot];

  TH1F *hPEVsTime_x[NEvtDisplTot];
  TH1F *hPEVsTime_y[NEvtDisplTot];
  TH1F *hPEVsTime_z[NEvtDisplTot];

  // Truth Vertex distributions

  // Vertex
  TH2D *hVtxOut_XY = new TH2D("hVtxOut_XY","hVtxOut_XY",400,-2000,2000,400,-2000,2000); // mm 
  TH2D *hVtxOut_XZ = new TH2D("hVtxOut_XZ","hVtxOut_XZ",400,-2000,2000,800,-4000,4000); // mm  
  TH2D *hVtxOut_YZ = new TH2D("hVtxOut_YZ","hVtxOut_YZ",400,-2000,2000,800,-4000,4000); // mm   
  TH2D *hVtx_XY = new TH2D("hVtx_XY","hVtx_XY",400,-2000,2000,400,-2000,2000); // mm 
  TH2D *hVtx_XZ = new TH2D("hVtx_XZ","hVtx_XZ",400,-2000,2000,800,-4000,4000); // mm  
  TH2D *hVtx_YZ = new TH2D("hVtx_YZ","hVtx_YZ",400,-2000,2000,800,-4000,4000); // mm  

  
  // Track measurement resolutions

  TH2F * hMuon_CosTh_TrueVsReco = new TH2F("hMuon_CosTh_TrueVsReco","Muon CosTheta True Vs Reco",50,-1,+1,50,-1,+1);
  TH2F * hPion_CosTh_TrueVsReco = new TH2F("hPion_CosTh_TrueVsReco","Pion CosTheta True Vs Reco",50,-1,+1,50,-1,+1);
  TH2F * hProt_CosTh_TrueVsReco = new TH2F("hProt_CosTh_TrueVsReco","Prot CosTheta True Vs Reco",50,-1,+1,50,-1,+1);
  TH2F * hElec_CosTh_TrueVsReco = new TH2F("hElec_CosTh_TrueVsReco","Elec CosTheta True Vs Reco",50,-1,+1,50,-1,+1);
  
  TH1F * hMuon_CosTh_RecMinTr = new TH1F("hMuon_CosTh_RecMinTr","Muon CosTheta Reco-True",100,-1,+1);
  TH1F * hPion_CosTh_RecMinTr = new TH1F("hPion_CosTh_RecMinTr","Pion CosTheta Reco-True",100,-1,+1);
  TH1F * hProt_CosTh_RecMinTr = new TH1F("hProt_CosTh_RecMinTr","Prot CosTheta Reco-True",100,-1,+1);
  TH1F * hElec_CosTh_RecMinTr = new TH1F("hElec_CosTh_RecMinTr","Elec CosTheta Reco-True",100,-1,+1);

  TH2F * hMuon_Len_TrueVsReco = new TH2F("hMuon_Len_TrueVsReco","Muon Length True Vs Reco",50,0,+3000,50,0,+3000);
  TH2F * hPion_Len_TrueVsReco = new TH2F("hPion_Len_TrueVsReco","Pion Length True Vs Reco",50,0,+3000,50,0,+3000);
  TH2F * hProt_Len_TrueVsReco = new TH2F("hProt_Len_TrueVsReco","Prot Length True Vs Reco",50,0,+3000,50,0,+3000);
  TH2F * hElec_Len_TrueVsReco = new TH2F("hElec_Len_TrueVsReco","Elec Length True Vs Reco",50,0,+3000,50,0,+3000);

  TH1F * hMuon_Len_RecMinTr = new TH1F("hMuon_Len_RecMinTr","Muon Length Reco-True",100,-50,+50);
  TH1F * hPion_Len_RecMinTr = new TH1F("hPion_Len_RecMinTr","Pion Length Reco-True",100,-50,+50);
  TH1F * hProt_Len_RecMinTr = new TH1F("hProt_Len_RecMinTr","Prot Length Reco-True",100,-50,+50);
  TH1F * hElec_Len_RecMinTr = new TH1F("hElec_Len_RecMinTr","Elec Length Reco-True",100,-50,+50);

  TH2F * hMuon_Len_RecMinTr_Vs_TrLen = new TH2F("hMuon_Len_RecMinTr_Vs_TrLen","Muon Length (Reco-True) Vs True",100,-50,+50,50,0,+3000);
  TH2F * hPion_Len_RecMinTr_Vs_TrLen = new TH2F("hPion_Len_RecMinTr_Vs_TrLen","Pion Length (Reco-True) Vs True",100,-50,+50,50,0,+3000);
  TH2F * hProt_Len_RecMinTr_Vs_TrLen = new TH2F("hProt_Len_RecMinTr_Vs_TrLen","Prot Length (Reco-True) Vs True",100,-50,+50,50,0,+3000);
  TH2F * hElec_Len_RecMinTr_Vs_TrLen = new TH2F("hElec_Len_RecMinTr_Vs_TrLen","Elec Length (Reco-True) Vs True",100,-50,+50,50,0,+3000);

  // True phase space and efficiencies

  TH2F * hMuon_TrMomVsTrCosTh = new TH2F("hMuon_TrMomVsTrCosTh","All Muons True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hPion_TrMomVsTrCosTh = new TH2F("hPion_TrMomVsTrCosTh","All Pions True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hProt_TrMomVsTrCosTh = new TH2F("hProt_TrMomVsTrCosTh","All Protons True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hElec_TrMomVsTrCosTh = new TH2F("hElec_TrMomVsTrCosTh","All Electrons True Mom Vs CosTheta",100,0,10000,10,-1,+1);

  TH2F * hMuon_TrPhiVsTrCosTh = new TH2F("hMuon_TrPhiVsTrCosTh","All Muons True Phi Vs CosTheta",10,-4,+4,10,-1,+1);
  TH2F * hPion_TrPhiVsTrCosTh = new TH2F("hPion_TrPhiVsTrCosTh","All Pions True Phi Vs CosTheta",10,-4,+4,10,-1,+1);
  TH2F * hProt_TrPhiVsTrCosTh = new TH2F("hProt_TrPhiVsTrCosTh","All Protons True Phi Vs CosTheta",10,-4,+4,10,-1,+1);
  TH2F * hElec_TrPhiVsTrCosTh = new TH2F("hElec_TrPhiVsTrCosTh","All Electrons True Phi Vs CosTheta",10,-4,+4,10,-1,+1);

  TH2F * hMuon_TrMomVsTrPhi = new TH2F("hMuon_TrMomVsTrPhi","All Muons True Mom Vs Phi",100,0,10000,10,-4,+4);
  TH2F * hPion_TrMomVsTrPhi = new TH2F("hPion_TrMomVsTrPhi","All Pions True Mom Vs Phi",100,0,10000,10,-4,+4);
  TH2F * hProt_TrMomVsTrPhi = new TH2F("hProt_TrMomVsTrPhi","All Protons True Mom Vs Phi",100,0,10000,10,-4,+4);
  TH2F * hElec_TrMomVsTrPhi = new TH2F("hElec_TrMomVsTrPhi","All Electrons True Mom Vs Phi",100,0,10000,10,-4,+4);

  TH2F * hMuon_AllIso_TrMomVsTrCosTh = new TH2F("hMuon_AllIso_TrMomVsTrCosTh","All IsoTarget Muons True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hPion_AllIso_TrMomVsTrCosTh = new TH2F("hPion_AllIso_TrMomVsTrCosTh","All IsoTarget Pions True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hProt_AllIso_TrMomVsTrCosTh = new TH2F("hProt_AllIso_TrMomVsTrCosTh","All IsoTarget Protons True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hElec_AllIso_TrMomVsTrCosTh = new TH2F("hElec_AllIso_TrMomVsTrCosTh","All IsoTarget Electrons True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hMuon_EffIso_TrMomVsTrCosTh = new TH2F("hMuon_EffIso_TrMomVsTrCosTh","Efficiency IsoTarget Muons True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hPion_EffIso_TrMomVsTrCosTh = new TH2F("hPion_EffIso_TrMomVsTrCosTh","Efficiency IsoTarget Pions True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hProt_EffIso_TrMomVsTrCosTh = new TH2F("hProt_EffIso_TrMomVsTrCosTh","Efficiency IsoTarget Protons True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hElec_EffIso_TrMomVsTrCosTh = new TH2F("hElec_EffIso_TrMomVsTrCosTh","Efficiency IsoTarget Electrons True Mom Vs CosTheta",100,0,10000,10,-1,+1);

  TH2F * hMuon_AllIso_TrMomVsTrPhi = new TH2F("hMuon_AllIso_TrMomVsTrPhi","All IsoTarget Muons True Mom Vs Phi",100,0,10000,10,-4,+4);
  TH2F * hPion_AllIso_TrMomVsTrPhi = new TH2F("hPion_AllIso_TrMomVsTrPhi","All IsoTarget Pions True Mom Vs Phi",100,0,10000,10,-4,+4);
  TH2F * hProt_AllIso_TrMomVsTrPhi = new TH2F("hProt_AllIso_TrMomVsTrPhi","All IsoTarget Protons True Mom Vs Phi",100,0,10000,10,-4,+4);
  TH2F * hElec_AllIso_TrMomVsTrPhi = new TH2F("hElec_AllIso_TrMomVsTrPhi","All IsoTarget Electrons True Mom Vs Phi",100,0,10000,10,-4,+4);
  TH2F * hMuon_EffIso_TrMomVsTrPhi = new TH2F("hMuon_EffIso_TrMomVsTrPhi","Efficiency IsoTarget Muons True Mom Vs Phi",100,0,10000,10,-4,+4);
  TH2F * hPion_EffIso_TrMomVsTrPhi = new TH2F("hPion_EffIso_TrMomVsTrPhi","Efficiency IsoTarget Pions True Mom Vs Phi",100,0,10000,10,-4,+4);
  TH2F * hProt_EffIso_TrMomVsTrPhi = new TH2F("hProt_EffIso_TrMomVsTrPhi","Efficiency IsoTarget Protons True Mom Vs Phi",100,0,10000,10,-4,+4);
  TH2F * hElec_EffIso_TrMomVsTrPhi = new TH2F("hElec_EffIso_TrMomVsTrPhi","Efficiency IsoTarget Electrons True Mom Vs Phi",100,0,10000,10,-4,+4);

  TH2F * hMuon_AllIso_TrPhiVsTrCosTh = new TH2F("hMuon_AllIso_TrPhiVsTrCosTh","All IsoTarget Muons True Phi Vs CosTheta",10,-4,+4,10,-1,+1);
  TH2F * hPion_AllIso_TrPhiVsTrCosTh = new TH2F("hPion_AllIso_TrPhiVsTrCosTh","All IsoTarget Pions True Phi Vs CosTheta",10,-4,+4,10,-1,+1);
  TH2F * hProt_AllIso_TrPhiVsTrCosTh = new TH2F("hProt_AllIso_TrPhiVsTrCosTh","All IsoTarget Protons True Phi Vs CosTheta",10,-4,+4,10,-1,+1);
  TH2F * hElec_AllIso_TrPhiVsTrCosTh = new TH2F("hElec_AllIso_TrPhiVsTrCosTh","All IsoTarget Electrons True Phi Vs CosTheta",10,-4,+4,10,-1,+1);
  TH2F * hMuon_EffIso_TrPhiVsTrCosTh = new TH2F("hMuon_EffIso_TrPhiVsTrCosTh","Efficiency IsoTarget Muons True Phi Vs CosTheta",10,-4,+4,10,-1,+1);
  TH2F * hPion_EffIso_TrPhiVsTrCosTh = new TH2F("hPion_EffIso_TrPhiVsTrCosTh","Efficiency IsoTarget Pions True Phi Vs CosTheta",10,-4,+4,10,-1,+1);
  TH2F * hProt_EffIso_TrPhiVsTrCosTh = new TH2F("hProt_EffIso_TrPhiVsTrCosTh","Efficiency IsoTarget Protons True Phi Vs CosTheta",10,-4,+4,10,-1,+1);
  TH2F * hElec_EffIso_TrPhiVsTrCosTh = new TH2F("hElec_EffIso_TrPhiVsTrCosTh","Efficiency IsoTarget Electrons True Phi Vs CosTheta",10,-4,+4,10,-1,+1);

  TH1F * hMuon_AllIso_TrMom = new TH1F("hMuon_AllIso_TrMom","All IsoTarget Muons True Mom",100,0,10000);
  TH1F * hPion_AllIso_TrMom = new TH1F("hPion_AllIso_TrMom","All IsoTarget Pions True Mom",100,0,10000);
  TH1F * hProt_AllIso_TrMom = new TH1F("hProt_AllIso_TrMom","All IsoTarget Protons True Mom",100,0,10000);
  TH1F * hElec_AllIso_TrMom = new TH1F("hElec_AllIso_TrMom","All IsoTarget Electrons True Mom",100,0,10000);
  TH1F * hMuon_EffIso_TrMom = new TH1F("hMuon_EffIso_TrMom","Efficiency IsoTarget Muons True Mom",100,0,10000);
  TH1F * hPion_EffIso_TrMom = new TH1F("hPion_EffIso_TrMom","Efficiency IsoTarget Pions True Mom",100,0,10000);
  TH1F * hProt_EffIso_TrMom = new TH1F("hProt_EffIso_TrMom","Efficiency IsoTarget Protons True Mom",100,0,10000);
  TH1F * hElec_EffIso_TrMom = new TH1F("hElec_EffIso_TrMom","Efficiency IsoTarget Electrons True Mom",100,0,10000);

  TH1F * hMuon_AllIso_TrCosTh = new TH1F("hMuon_AllIso_TrCosTh","All IsoTarget Muons True CosTheta",10,-1,+1);
  TH1F * hPion_AllIso_TrCosTh = new TH1F("hPion_AllIso_TrCosTh","All IsoTarget Pions True CosTheta",10,-1,+1);
  TH1F * hProt_AllIso_TrCosTh = new TH1F("hProt_AllIso_TrCosTh","All IsoTarget Protons True CosTheta",10,-1,+1);
  TH1F * hElec_AllIso_TrCosTh = new TH1F("hElec_AllIso_TrCosTh","All IsoTarget Electrons True CosTheta",10,-1,+1);
  TH1F * hMuon_EffIso_TrCosTh = new TH1F("hMuon_EffIso_TrCosTh","Efficiency IsoTarget Muons True CosTheta",10,-1,+1);
  TH1F * hPion_EffIso_TrCosTh = new TH1F("hPion_EffIso_TrCosTh","Efficiency IsoTarget Pions True CosTheta",10,-1,+1);
  TH1F * hProt_EffIso_TrCosTh = new TH1F("hProt_EffIso_TrCosTh","Efficiency IsoTarget Protons True CosTheta",10,-1,+1);
  TH1F * hElec_EffIso_TrCosTh = new TH1F("hElec_EffIso_TrCosTh","Efficiency IsoTarget Electrons True CosTheta",10,-1,+1);

  TH1F * hMuon_AllIso_TrPhi = new TH1F("hMuon_AllIso_TrPhi","All IsoTarget Muons True Phi",10,-4,+4);
  TH1F * hPion_AllIso_TrPhi = new TH1F("hPion_AllIso_TrPhi","All IsoTarget Pions True Phi",10,-4,+4);
  TH1F * hProt_AllIso_TrPhi = new TH1F("hProt_AllIso_TrPhi","All IsoTarget Protons True Phi",10,-4,+4);
  TH1F * hElec_AllIso_TrPhi = new TH1F("hElec_AllIso_TrPhi","All IsoTarget Electrons True Phi",10,-4,+4);
  TH1F * hMuon_EffIso_TrPhi = new TH1F("hMuon_EffIso_TrPhi","Efficiency IsoTarget Muons True Phi",10,-4,+4);
  TH1F * hPion_EffIso_TrPhi = new TH1F("hPion_EffIso_TrPhi","Efficiency IsoTarget Pions True Phi",10,-4,+4);
  TH1F * hProt_EffIso_TrPhi = new TH1F("hProt_EffIso_TrPhi","Efficiency IsoTarget Protons True Phi",10,-4,+4);
  TH1F * hElec_EffIso_TrPhi = new TH1F("hElec_EffIso_TrPhi","Efficiency IsoTarget Electrons True Phi",10,-4,+4);

  // True PDFs and Like ratio for PID

  TH1F * hMuon_Len = new TH1F("hMuon_Len","Muon track length",100,0,2000); // mm 
  TH1F * hPion_Len = new TH1F("hPion_Len","Pion track length",100,0,2000); 
  TH1F * hProt_Len = new TH1F("hProt_Len","Prot track length",100,0,2000);
  TH1F * hElec_Len = new TH1F("hElec_Len","Elec track length",100,0,2000);
  TH1F * hMuon_Stopped_Len = new TH1F("hMuon_Stopped_Len","Muon Stopped track length",100,0,2000); // mm 
  TH1F * hPion_Stopped_Len = new TH1F("hPion_Stopped_Len","Pion Stopped track length",100,0,2000); 
  TH1F * hProt_Stopped_Len = new TH1F("hProt_Stopped_Len","Prot Stopped track length",100,0,2000);
  TH1F * hElec_Stopped_Len = new TH1F("hElec_Stopped_Len","Elec Stopped track length",100,0,2000);

  TH1F * hMuon_Edep = new TH1F("hMuon_Edep","Muon energy deposit",200,0,10000); // pe
  TH1F * hPion_Edep = new TH1F("hPion_Edep","Pion energy deposit",200,0,10000);
  TH1F * hProt_Edep = new TH1F("hProt_Edep","Proton energy deposit",200,0,10000);
  TH1F * hElec_Edep = new TH1F("hElec_Edep","Electron energy deposit",200,0,10000);
  TH1F * hMuon_Stopped_Edep = new TH1F("hMuon_Stopped_Edep","Muon Stopped energy deposit",200,0,10000); // pe
  TH1F * hPion_Stopped_Edep = new TH1F("hPion_Stopped_Edep","Pion Stopped energy deposit",200,0,10000);
  TH1F * hProt_Stopped_Edep = new TH1F("hProt_Stopped_Edep","Proton Stopped energy deposit",200,0,10000);
  TH1F * hElec_Stopped_Edep = new TH1F("hElec_Stopped_Edep","Electron Stopped energy deposit",200,0,10000);

  TH1F * hMuon_EdepOverLen = new TH1F("hMuon_EdepOverLen","Muon energy deposit over track length",200,0,200); // ~ pe / mm (cube)
  TH1F * hPion_EdepOverLen = new TH1F("hPion_EdepOverLen","Pion energy deposit over track length",200,0,200); 
  TH1F * hProt_EdepOverLen = new TH1F("hProt_EdepOverLen","Prot energy deposit over track length",200,0,200);
  TH1F * hElec_EdepOverLen = new TH1F("hElec_EdepOverLen","Elec energy deposit over track length",400,0,200);

  //////////// PDFs for PID

  int nbins = 50;
  int nbins_nue = 1000;

  double max = 0.;
  if     (DetType == nd280upconv::kSuperFGD) max = 200;
  else if(DetType == nd280upconv::kFGDlike)  max = 50;
  
  TH1F * hMuon_Stopped_EdepOverLen = new TH1F("hMuon_Stopped_EdepOverLen","Stopping Muon energy deposit over track length",nbins,0,max);
  TH1F * hPion_Stopped_EdepOverLen = new TH1F("hPion_Stopped_EdepOverLen","Stopping Pion energy deposit over track length",nbins,0,max);
  TH1F * hProt_Stopped_EdepOverLen = new TH1F("hProt_Stopped_EdepOverLen","Stopping Prot energy deposit over track length",nbins,0,max);
  TH1F * hElec_Stopped_EdepOverLen = new TH1F("hElec_Stopped_EdepOverLen","Stopping Elec energy deposit over track length",nbins_nue,0,max);

  TH1F * hMuon_Stopped_EdepOverLen_ManyBins = new TH1F("hMuon_Stopped_EdepOverLen_ManyBins","Stopping Muon energy deposit over track length (for p-value)",200000,0,max);
  TH1F * hPion_Stopped_EdepOverLen_ManyBins = new TH1F("hPion_Stopped_EdepOverLen_ManyBins","Stopping Pion energy deposit over track length (for p-value)",200000,0,max);
  TH1F * hProt_Stopped_EdepOverLen_ManyBins = new TH1F("hProt_Stopped_EdepOverLen_ManyBins","Stopping Prot energy deposit over track length (for p-value)",200000,0,max);
  TH1F * hElec_Stopped_EdepOverLen_ManyBins = new TH1F("hElec_Stopped_EdepOverLen_ManyBins","Stopping Elec energy deposit over track length (for p-value)",200000,0,max);

  TGraph * gMuon_Stopped_EdepVsLen = new TGraph(); 
  gMuon_Stopped_EdepVsLen->SetName("gMuon_Stopped_EdepVsLen"); 
  gMuon_Stopped_EdepVsLen->SetTitle("Muon Stopped energy deposit Vs track length"); // ~ pe Vs mm (cube)  
  TGraph * gPion_Stopped_EdepVsLen = new TGraph(); // ~ pe Vs mm (cube)
  gPion_Stopped_EdepVsLen->SetName("gPion_Stopped_EdepVsLen"); 
  gPion_Stopped_EdepVsLen->SetTitle("Pion Stopped energy deposit Vs track length"); // ~ pe Vs mm (cube)
  TGraph * gProt_Stopped_EdepVsLen = new TGraph(); // ~ pe Vs mm (cube)
  gProt_Stopped_EdepVsLen->SetName("gProt_Stopped_EdepVsLen"); 
  gProt_Stopped_EdepVsLen->SetTitle("Prot Stopped energy deposit Vs track length"); // ~ pe Vs mm (cube)
  TGraph * gElec_Stopped_EdepVsLen = new TGraph(); // ~ pe Vs mm (cube)
  gElec_Stopped_EdepVsLen->SetName("gElec_Stopped_EdepVsLen"); 
  gElec_Stopped_EdepVsLen->SetTitle("Elec Stopped energy deposit Vs track length"); // ~ pe Vs mm (cube)

  TGraph * gMuon_Stopped_EdepVsLen_Truth = new TGraph(); 
  gMuon_Stopped_EdepVsLen_Truth->SetName("gMuon_Stopped_EdepVsLen_Truth"); 
  gMuon_Stopped_EdepVsLen_Truth->SetTitle("Muon Stopped energy deposit Vs track length (Truth)"); // ~ MeV Vs mm (cube)  
  TGraph * gPion_Stopped_EdepVsLen_Truth = new TGraph(); // ~ pe Vs mm (cube)
  gPion_Stopped_EdepVsLen_Truth->SetName("gPion_Stopped_EdepVsLen_Truth"); 
  gPion_Stopped_EdepVsLen_Truth->SetTitle("Pion Stopped energy deposit Vs track length (Truth)"); // ~ MeV Vs mm (cube)
  TGraph * gProt_Stopped_EdepVsLen_Truth = new TGraph(); // ~ pe Vs mm (cube)
  gProt_Stopped_EdepVsLen_Truth->SetName("gProt_Stopped_EdepVsLen_Truth"); 
  gProt_Stopped_EdepVsLen_Truth->SetTitle("Prot Stopped energy deposit Vs track length (Truth)"); // ~ MeV Vs mm (cube)
  TGraph * gElec_Stopped_EdepVsLen_Truth = new TGraph(); // ~ pe Vs mm (cube)
  gElec_Stopped_EdepVsLen_Truth->SetName("gElec_Stopped_EdepVsLen_Truth"); 
  gElec_Stopped_EdepVsLen_Truth->SetTitle("Elec Stopped energy deposit Vs track length (Truth)"); // ~ MeV Vs mm (cube)

  // dEdx Vs Momentum
  
  TGraph * gMuon_Stopped_MomVsdEdx = new TGraph(); 
  gMuon_Stopped_MomVsdEdx->SetName("gMuon_Stopped_MomVsdEdx"); 
  gMuon_Stopped_MomVsdEdx->SetTitle("Muon Stopped Momentum Vs dE/dx"); 
  TGraph * gPion_Stopped_MomVsdEdx = new TGraph(); 
  gPion_Stopped_MomVsdEdx->SetName("gPion_Stopped_MomVsdEdx"); 
  gPion_Stopped_MomVsdEdx->SetTitle("Pion Stopped Momentum Vs dE/dx"); 
  TGraph * gProt_Stopped_MomVsdEdx = new TGraph(); 
  gProt_Stopped_MomVsdEdx->SetName("gProt_Stopped_MomVsdEdx"); 
  gProt_Stopped_MomVsdEdx->SetTitle("Prot Stopped Momentum Vs dE/dx"); 
  TGraph * gElec_Stopped_MomVsdEdx = new TGraph(); 
  gElec_Stopped_MomVsdEdx->SetName("gElec_Stopped_MomVsdEdx"); 
  gElec_Stopped_MomVsdEdx->SetTitle("Elec Stopped Momentum Vs dE/dx"); 
 

  TGraph * gMuon_Stopped_MomVsdEdx_Truth = new TGraph(); 
  gMuon_Stopped_MomVsdEdx_Truth->SetName("gMuon_Stopped_MomVsdEdx_Truth"); 
  gMuon_Stopped_MomVsdEdx_Truth->SetTitle("Muon Stopped Momentum Vs dE/dx (Truth)"); // ~ MeV Vs mm (cube)  
  TGraph * gPion_Stopped_MomVsdEdx_Truth = new TGraph(); // ~ pe Vs mm (cube)
  gPion_Stopped_MomVsdEdx_Truth->SetName("gPion_Stopped_MomVsdEdx_Truth"); 
  gPion_Stopped_MomVsdEdx_Truth->SetTitle("Pion Stopped Momentum Vs dE/dx (Truth)"); // ~ MeV Vs mm (cube)
  TGraph * gProt_Stopped_MomVsdEdx_Truth = new TGraph(); // ~ pe Vs mm (cube)
  gProt_Stopped_MomVsdEdx_Truth->SetName("gProt_Stopped_MomVsdEdx_Truth"); 
  gProt_Stopped_MomVsdEdx_Truth->SetTitle("Prot Stopped Momentum Vs dE/dx (Truth)"); // ~ MeV Vs mm (cube)
  TGraph * gElec_Stopped_MomVsdEdx_Truth = new TGraph(); // ~ pe Vs mm (cube)
  gElec_Stopped_MomVsdEdx_Truth->SetName("gElec_Stopped_MomVsdEdx_Truth"); 
  gElec_Stopped_MomVsdEdx_Truth->SetTitle("Elec Stopped Momentum Vs dE/dx (Truth)"); // ~ MeV Vs mm (cube)

  TGraph * gMuon_All_MomVsdEdx_Truth = new TGraph(); 
  gMuon_All_MomVsdEdx_Truth->SetName("gMuon_All_MomVsdEdx_Truth"); 
  gMuon_All_MomVsdEdx_Truth->SetTitle("Muon All Momentum Vs dE/dx (Truth)"); // ~ MeV Vs mm (cube)  
  TGraph * gPion_All_MomVsdEdx_Truth = new TGraph(); // ~ pe Vs mm (cube)
  gPion_All_MomVsdEdx_Truth->SetName("gPion_All_MomVsdEdx_Truth"); 
  gPion_All_MomVsdEdx_Truth->SetTitle("Pion All Momentum Vs dE/dx (Truth)"); // ~ MeV Vs mm (cube)
  TGraph * gProt_All_MomVsdEdx_Truth = new TGraph(); // ~ pe Vs mm (cube)
  gProt_All_MomVsdEdx_Truth->SetName("gProt_All_MomVsdEdx_Truth"); 
  gProt_All_MomVsdEdx_Truth->SetTitle("Prot All Momentum Vs dE/dx (Truth)"); // ~ MeV Vs mm (cube)
  TGraph * gElec_All_MomVsdEdx_Truth = new TGraph(); // ~ pe Vs mm (cube)
  gElec_All_MomVsdEdx_Truth->SetName("gElec_All_MomVsdEdx_Truth"); 
  gElec_All_MomVsdEdx_Truth->SetTitle("Elec All Momentum Vs dE/dx (Truth)"); // ~ MeV Vs mm (cube)

  // dEdx Vs Track Length
  
  TGraph * gMuon_Stopped_LenVsdEdx = new TGraph(); 
  gMuon_Stopped_LenVsdEdx->SetName("gMuon_Stopped_LenVsdEdx"); 
  gMuon_Stopped_LenVsdEdx->SetTitle("Muon Stopped track length Vs dE/dx"); // ~ MeV Vs mm (cube)  
  TGraph * gPion_Stopped_LenVsdEdx = new TGraph(); // ~ pe Vs mm (cube)
  gPion_Stopped_LenVsdEdx->SetName("gPion_Stopped_LenVsdEdx"); 
  gPion_Stopped_LenVsdEdx->SetTitle("Pion Stopped track length Vs dE/dx"); // ~ MeV Vs mm (cube)
  TGraph * gProt_Stopped_LenVsdEdx = new TGraph(); // ~ pe Vs mm (cube)
  gProt_Stopped_LenVsdEdx->SetName("gProt_Stopped_LenVsdEdx"); 
  gProt_Stopped_LenVsdEdx->SetTitle("Prot Stopped track length Vs dE/dx"); // ~ MeV Vs mm (cube)
  TGraph * gElec_Stopped_LenVsdEdx = new TGraph(); // ~ pe Vs mm (cube)
  gElec_Stopped_LenVsdEdx->SetName("gElec_Stopped_LenVsdEdx"); 
  gElec_Stopped_LenVsdEdx->SetTitle("Elec Stopped track length Vs dE/dx"); // ~ MeV Vs mm (cube)
  
  TGraph * gMuon_Stopped_LenVsdEdx_Truth = new TGraph(); 
  gMuon_Stopped_LenVsdEdx_Truth->SetName("gMuon_Stopped_LenVsdEdx_Truth"); 
  gMuon_Stopped_LenVsdEdx_Truth->SetTitle("Muon Stopped track length Vs dE/dx (Truth)"); // ~ MeV Vs mm (cube)  
  TGraph * gPion_Stopped_LenVsdEdx_Truth = new TGraph(); // ~ pe Vs mm (cube)
  gPion_Stopped_LenVsdEdx_Truth->SetName("gPion_Stopped_LenVsdEdx_Truth"); 
  gPion_Stopped_LenVsdEdx_Truth->SetTitle("Pion Stopped track length Vs dE/dx (Truth)"); // ~ MeV Vs mm (cube)
  TGraph * gProt_Stopped_LenVsdEdx_Truth = new TGraph(); // ~ pe Vs mm (cube)
  gProt_Stopped_LenVsdEdx_Truth->SetName("gProt_Stopped_LenVsdEdx_Truth"); 
  gProt_Stopped_LenVsdEdx_Truth->SetTitle("Prot Stopped track length Vs dE/dx (Truth)"); // ~ MeV Vs mm (cube)
  TGraph * gElec_Stopped_LenVsdEdx_Truth = new TGraph(); // ~ pe Vs mm (cube)
  gElec_Stopped_LenVsdEdx_Truth->SetName("gElec_Stopped_LenVsdEdx_Truth"); 
  gElec_Stopped_LenVsdEdx_Truth->SetTitle("Elec Stopped track length Vs dE/dx (Truth)"); // ~ MeV Vs mm (cube)

  // Histograms for different momenta ranges

  const int NBinsMom = 6;
  const double BinsMom[NBinsMom+1] = {0., 300., 600., 800., 1000., 2000, 30000.};
  TH1F *hBinsMom = new TH1F("hBinsMom","hBinsMom",NBinsMom,BinsMom);
  
  TH1F * hMuon_Stopped_EdepOverLen_Mom[NBinsMom];
  TH1F * hPion_Stopped_EdepOverLen_Mom[NBinsMom];
  TH1F * hProt_Stopped_EdepOverLen_Mom[NBinsMom];
  TH1F * hElec_Stopped_EdepOverLen_Mom[NBinsMom];
  
  TH1F * hMuon_Stopped_EdepOverLen_Mom_ManyBins[NBinsMom];
  TH1F * hPion_Stopped_EdepOverLen_Mom_ManyBins[NBinsMom];
  TH1F * hProt_Stopped_EdepOverLen_Mom_ManyBins[NBinsMom];
  TH1F * hElec_Stopped_EdepOverLen_Mom_ManyBins[NBinsMom];  
  
  for(int imom=0;imom<NBinsMom;imom++){

    int histobin = imom+1;
    double lowmom = hBinsMom->GetXaxis()->GetBinLowEdge(histobin);
    double upmom  = hBinsMom->GetXaxis()->GetBinUpEdge(histobin);

    TString name;
    name = TString::Format("hMuon_Stopped_EdepOverLen_Mom_%1.0f_%1.0f",lowmom,upmom);
    hMuon_Stopped_EdepOverLen_Mom[imom] = new TH1F(name,name,nbins,0,max);
    name = TString::Format("hMuon_Stopped_EdepOverLen_Mom_ManyBins_%1.0f_%1.0f",lowmom,upmom);
    hMuon_Stopped_EdepOverLen_Mom_ManyBins[imom] = new TH1F(name,name,200000,0,max);

    name = TString::Format("hPion_Stopped_EdepOverLen_Mom_%1.0f_%1.0f",lowmom,upmom);
    hPion_Stopped_EdepOverLen_Mom[imom] = new TH1F(name,name,nbins,0,max);
    name = TString::Format("hPion_Stopped_EdepOverLen_Mom_ManyBins_%1.0f_%1.0f",lowmom,upmom);
    hPion_Stopped_EdepOverLen_Mom_ManyBins[imom] = new TH1F(name,name,200000,0,max);

    name = TString::Format("hProt_Stopped_EdepOverLen_Mom_%1.0f_%1.0f",lowmom,upmom);
    hProt_Stopped_EdepOverLen_Mom[imom] = new TH1F(name,name,nbins,0,max);
    name = TString::Format("hProt_Stopped_EdepOverLen_Mom_ManyBins_%1.0f_%1.0f",lowmom,upmom);
    hProt_Stopped_EdepOverLen_Mom_ManyBins[imom] = new TH1F(name,name,200000,0,max);

    name = TString::Format("hElec_Stopped_EdepOverLen_Mom_%1.0f_%1.0f",lowmom,upmom);
    hElec_Stopped_EdepOverLen_Mom[imom] = new TH1F(name,name,nbins_nue,0,max);
    name = TString::Format("hElec_Stopped_EdepOverLen_Mom_ManyBins_%1.0f_%1.0f",lowmom,upmom);
    hElec_Stopped_EdepOverLen_Mom_ManyBins[imom] = new TH1F(name,name,200000,0,max);

  }


  // Histograms for different reco track lengths

  const int NBinsLen = 5;
  const double BinsLen[NBinsLen+1] = {0., 200., 400., 600., 1000., 2500.}; // mm
  TH1F *hBinsLen = new TH1F("hBinsLen","hBinsLen",NBinsLen,BinsLen);
  
  TH1F * hMuon_Stopped_EdepOverLen_Len[NBinsLen];
  TH1F * hPion_Stopped_EdepOverLen_Len[NBinsLen];
  TH1F * hProt_Stopped_EdepOverLen_Len[NBinsLen];
  TH1F * hElec_Stopped_EdepOverLen_Len[NBinsLen];
  
  TH1F * hMuon_Stopped_EdepOverLen_Len_ManyBins[NBinsLen];
  TH1F * hPion_Stopped_EdepOverLen_Len_ManyBins[NBinsLen];
  TH1F * hProt_Stopped_EdepOverLen_Len_ManyBins[NBinsLen];
  TH1F * hElec_Stopped_EdepOverLen_Len_ManyBins[NBinsLen];  
  
  for(int ilen=0;ilen<NBinsLen;ilen++){

    int histobin = ilen+1;
    double lowlen = hBinsLen->GetXaxis()->GetBinLowEdge(histobin);
    double uplen  = hBinsLen->GetXaxis()->GetBinUpEdge(histobin);

    TString name;
    name = TString::Format("hMuon_Stopped_EdepOverLen_Len_%1.0f_%1.0f",lowlen,uplen);
    hMuon_Stopped_EdepOverLen_Len[ilen] = new TH1F(name,name,nbins,0,max);
    name = TString::Format("hMuon_Stopped_EdepOverLen_Len_ManyBins_%1.0f_%1.0f",lowlen,uplen);
    hMuon_Stopped_EdepOverLen_Len_ManyBins[ilen] = new TH1F(name,name,200000,0,max);

    name = TString::Format("hPion_Stopped_EdepOverLen_Len_%1.0f_%1.0f",lowlen,uplen);
    hPion_Stopped_EdepOverLen_Len[ilen] = new TH1F(name,name,nbins,0,max);
    name = TString::Format("hPion_Stopped_EdepOverLen_Len_ManyBins_%1.0f_%1.0f",lowlen,uplen);
    hPion_Stopped_EdepOverLen_Len_ManyBins[ilen] = new TH1F(name,name,200000,0,max);

    name = TString::Format("hProt_Stopped_EdepOverLen_Len_%1.0f_%1.0f",lowlen,uplen);
    hProt_Stopped_EdepOverLen_Len[ilen] = new TH1F(name,name,nbins,0,max);
    name = TString::Format("hProt_Stopped_EdepOverLen_Len_ManyBins_%1.0f_%1.0f",lowlen,uplen);
    hProt_Stopped_EdepOverLen_Len_ManyBins[ilen] = new TH1F(name,name,200000,0,max);

    name = TString::Format("hElec_Stopped_EdepOverLen_Len_%1.0f_%1.0f",lowlen,uplen);
    hElec_Stopped_EdepOverLen_Len[ilen] = new TH1F(name,name,nbins_nue,0,max);
    name = TString::Format("hElec_Stopped_EdepOverLen_Len_ManyBins_%1.0f_%1.0f",lowlen,uplen);
    hElec_Stopped_EdepOverLen_Len_ManyBins[ilen] = new TH1F(name,name,200000,0,max);

  }

  //

  TH1F * hLikeRatio_MuProt_TrueMu = new TH1F("hLikeRatio_MuProt_TrueMu","hLikeRatio_MuProt_TrueMu",100,-10,10);
  TH1F * hLikeRatio_MuProt_TrueProt = new TH1F("hLikeRatio_MuProt_TrueProt","hLikeRatio_MuProt_TrueProt",100,-10,10);

  TH1F * hLikeRatio_MuPion_TrueMu = new TH1F("hLikeRatio_MuPion_TrueMu","hLikeRatio_MuPion_TrueMu",100,-10,10);
  TH1F * hLikeRatio_MuPion_TruePion = new TH1F("hLikeRatio_MuPion_TruePion","hLikeRatio_MuPion_TruePion",100,-10,10);
  
  TH1F * hLikeRatio_PionProt_TruePion = new TH1F("hLikeRatio_PionProt_TruePion","hLikeRatio_PionProt_TruePion",100,-10,10);
  TH1F * hLikeRatio_PionProt_TrueProt = new TH1F("hLikeRatio_PionProt_TrueProt","hLikeRatio_PionProt_TrueProt",100,-10,10);

  TH1F * hLikeRatio_ElecProt_TrueElec = new TH1F("hLikeRatio_ElecProt_TrueElec","hLikeRatio_ElecProt_TrueElec",100,-10,10);
  TH1F * hLikeRatio_ElecPion_TrueElec = new TH1F("hLikeRatio_ElecPion_TrueElec","hLikeRatio_ElecPion_TrueElec",100,-10,10);
  TH1F * hLikeRatio_ElecMu_TrueElec = new TH1F("hLikeRatio_ElecMu_TrueElec","hLikeRatio_ElecMu_TrueElec",100,-10,10);


  /*

  ///////////////////////////////

  //B.Q
  TH1F * hMuon_Edep_Raw = new TH1F("hMuon_Edep_Raw","Muon energy deposit",100,0,10); TH1F * hMuon_Edep_Plan_Raw = new TH1F("hMuon_Edep_Plan_Raw","Muon energy deposit",100,0,10);  TH1F * hMuon_Edep_Grid_Raw = new TH1F("hMuon_Edep_Grid_Raw","Muon energy deposit",100,0,10); // pe
  TH1F * hPion_Edep_Raw = new TH1F("hPion_Edep_Raw","Pion energy deposit",100,0,10); TH1F * hPion_Edep_Plan_Raw = new TH1F("hPion_Edep_Plan_Raw","Pion energy deposit",100,0,10);  TH1F * hPion_Edep_Grid_Raw = new TH1F("hPion_Edep_Grid_Raw","Pion energy deposit",100,0,10); // pe
  TH1F * hProt_Edep_Raw = new TH1F("hProt_Edep_Raw","Prot energy deposit",100,0,10); TH1F * hProt_Edep_Plan_Raw = new TH1F("hProt_Edep_Plan_Raw","Prot energy deposit",100,0,10);  TH1F * hProt_Edep_Grid_Raw = new TH1F("hProt_Edep_Grid_Raw","Prot energy deposit",100,0,10); // pe
 TH1F * hElec_Edep_Raw = new TH1F("hElec_Edep_Raw","Elec energy deposit",100,0,10); TH1F * hElec_Edep_Plan_Raw = new TH1F("hElec_Edep_Plan_Raw","Elec energy deposit",100,0,10);  TH1F * hElec_Edep_Grid_Raw = new TH1F("hElec_Edep_Grid_Raw","Elec energy deposit",100,0,10); // pe

   TH1F * hMuon_Edep_Calibrated = new TH1F("hMuon_Edep_Calibrated","Muon energy deposit",200,0,200); TH1F * hMuon_Edep_Plan_Calibrated = new TH1F("hMuon_Edep_Plan_Calibrated","Muon energy deposit",200,0,200);  TH1F * hMuon_Edep_Grid_Calibrated = new TH1F("hMuon_Edep_Grid_Calibrated","Muon energy deposit",200,0,200); // pe
  TH1F * hPion_Edep_Calibrated = new TH1F("hPion_Edep_Calibrated","Pion energy deposit",200,0,200); TH1F * hPion_Edep_Plan_Calibrated = new TH1F("hPion_Edep_Plan_Calibrated","Pion energy deposit",200,0,200);  TH1F * hPion_Edep_Grid_Calibrated = new TH1F("hPion_Edep_Grid_Calibrated","Pion energy deposit",200,0,200); // pe
  TH1F * hProt_Edep_Calibrated = new TH1F("hProt_Edep_Calibrated","Prot energy deposit",200,0,200); TH1F * hProt_Edep_Plan_Calibrated = new TH1F("hProt_Edep_Plan_Calibrated","Prot energy deposit",200,0,200);  TH1F * hProt_Edep_Grid_Calibrated = new TH1F("hProt_Edep_Grid_Calibrated","Prot energy deposit",200,0,200); // pe
 TH1F * hElec_Edep_Calibrated = new TH1F("hElec_Edep_Calibrated","Elec energy deposit",200,0,200); TH1F * hElec_Edep_Plan_Calibrated = new TH1F("hElec_Edep_Plan_Calibrated","Elec energy deposit",200,0,200);  TH1F * hElec_Edep_Grid_Calibrated = new TH1F("hElec_Edep_Grid_Calibrated","Elec energy deposit",200,0,200); // pe

  TH1F * hMuon_EdepOverLen_PerHit = new TH1F("hMuon_EdepOverLen_PerHit","Muon energy deposit over track length",200,0,200); // ~ pe / mm (cube)
  TH1F * hPion_EdepOverLen_PerHit = new TH1F("hPion_EdepOverLen_PerHit","Pion energy deposit over track length",200,0,200); 
  TH1F * hProt_EdepOverLen_PerHit = new TH1F("hProt_EdepOverLen_PerHit","Prot energy deposit over track length",200,0,200);
  TH1F * hElec_EdepOverLen_PerHit = new TH1F("hElec_EdepOverLen_PerHit","Elec energy deposit over track length",200,0,200);

  TH1F * hLikeRatioPerHit_MuProt_TrueMu = new TH1F("hLikeRatioPerHit_MuProt_TrueMu","hLikeRatioPerHit_MuProt_TrueMu",500,-500,500);
  TH1F * hLikeRatioPerHit_MuProt_TrueProt = new TH1F("hLikeRatioPerHit_MuProt_TrueProt","hLikeRatioPerHit_MuProt_TrueProt",500,-500,500);
  
  TH1F * hLikeRatioPerHit_PionProt_TruePion = new TH1F("hLikeRatioPerHit_PionProt_TruePion","hLikeRatioPerHit_PionProt_TruePion",500,-500,500);
  TH1F * hLikeRatioPerHit_PionProt_TrueProt = new TH1F("hLikeRatioPerHit_PionProt_TrueProt","hLikeRatioPerHit_PionProt_TrueProt",500,-500,500);
  
  //For PID: First, Muon+pion vs proton
  TH2F * hLikeRatioPerHit_MuonAndPionVsProt_TrueMuon = new TH2F("hLikeRatioPerHit_MuonAndPionVsProt_TrueMuon","hLikeRatioPerHit_MuonAndPionVsProt_TrueMuon",500,-500,500,100,-20,20);
  TH2F * hLikeRatioPerHit_MuonAndPionVsProt_TruePion = new TH2F("hLikeRatioPerHit_MuonAndPionVsProt_TruePion","hLikeRatioPerHit_MuonAndPionVsProt_TruePion",500,-500,500,100,-20,20);
  TH2F * hLikeRatioPerHit_MuonAndPionVsProt_TrueProt = new TH2F("hLikeRatioPerHit_MuonAndPionVsProt_TrueProt","hLikeRatioPerHit_MuonAndPionVsProt_TrueProt",500,-500,500,100,-20,20);

  TH2F * hLikeRatioPerHit_MuonAndPionVsProt_TrMom_TrueMuon = new TH2F("hLikeRatioPerHit_MuonAndPionVsProt_TrMom_TrueMuon","hLikeRatioPerHit_MuonAndPionVsPro_TrMom_TrueMuon",100,0,2000,1000,-100,100);
  TH2F * hLikeRatioPerHit_MuonAndPionVsProt_TrMom_TruePion = new TH2F("hLikeRatioPerHit_MuonAndPionVsProt_TrMom_TruePion","hLikeRatioPerHit_MuonAndPionVsPro_TrMom_TruePion",100,0,2000,1000,-100,100);
  TH2F * hLikeRatioPerHit_MuonAndPionVsProt_TrMom_TrueProt = new TH2F("hLikeRatioPerHit_MuonAndPionVsProt_TrMom_TrueProt","hLikeRatioPerHit_MuonAndPionVsPro_TrMom_TrueProt",100,0,2000,1000,-100,100);

  TH1F * hIntType = new TH1F("hIntType","",4,0,4);
  TH2F * hIntTypeVsMuonLike = new TH2F("hIntTypeVsMuonLike","",4,0,4,4,0,4);

  ///////////////////
  
  */
 

  // Set outputs
  
  TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);
  TFile *fileout = new TFile(outfilename.Data(),"RECREATE"); 
  //TND280UpRecoEvent *fND280UpRecoEvent = new TND280UpRecoEvent(); // NOT SURE WHY!!!
  //TND280UpRecoEvent *fND280UpRecoEvent; 
  TND280UpRecoEvent *fND280UpRecoEvent(NULL);
  TTree *fOutEventTree = new TTree("ND280upRecoEvents","ND280 upgrade Reco Event Tree");
  fOutEventTree->Branch("RecoEvent","TND280UpRecoEvent",&fND280UpRecoEvent,128000,0);
  //fOutEventTree->Branch("RecoEvent","TND280UpRecoEvent",&fND280UpRecoEvent,256000,0);

  

  /////////////////////////////
  //                         //
  // Apply detector response //
  //                         //
  /////////////////////////////


  // Take inputs

  TFile *finput = new TFile(rootfilename.c_str(),"READ");
  cout << "Reading the input ROOT file: " << rootfilename << endl;

  TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
  h2d_xy = (TH2F*)finput->Get("OutMPPCProj2D_XY");
  h2d_xz = (TH2F*)finput->Get("OutMPPCProj2D_XZ");
  h2d_yz = (TH2F*)finput->Get("OutMPPCProj2D_YZ");


  // Take the World origin position in the target (local) reference system

  TPolyMarker3D *WorldOrigInLocal = (TPolyMarker3D*)finput->Get("WorldOrigInLocal");
  double OrigInLocX=0.;
  double OrigInLocY=0.;
  double OrigInLocZ=0.;
  WorldOrigInLocal->GetPoint(0,OrigInLocX,OrigInLocY,OrigInLocZ);
  
  cout << "The World origin in local coordinates is:" << endl; 
  cout << OrigInLocX << ", " << OrigInLocY << ", " << OrigInLocZ << endl;
  cout << endl;

  //

  TGraph *mygraph = new TGraph();
  mygraph->SetName("mygraph");
  mygraph->SetTitle("mygraph");
  
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  //
  // Take the MPPC 2D histogram
  //

  ND280UpApplyResponse ApplyResponse;    
  ApplyResponse.SetMPPCProj2D_XY(h2d_xy);
  ApplyResponse.SetMPPCProj2D_XZ(h2d_xz);
  ApplyResponse.SetMPPCProj2D_YZ(h2d_yz);
  // FGDlike
  if(DetType == nd280upconv::kFGDlike){
    ApplyResponse.SetDetNameAlongX("/BarScintHoriz");
    ApplyResponse.SetDetNameAlongY("");
    ApplyResponse.SetDetNameAlongZ("/BarScintVert");
  }
  // SciFi
  //ApplyResponse.SetDetNameAlongX("/FiberScintHoriz");
  //ApplyResponse.SetDetNameAlongY("");
  //ApplyResponse.SetDetNameAlongZ("/FiberScintVert");

  
  // Declare the input true event object
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

  //
  // Loop over the events:
  // - apply response event by event
  // - apply reconstruction event by event
  //

  TStopwatch sw_event;

  for(int ievt=evtfirst;ievt<=EntryLast;ievt++){ // get last entry
    tinput->GetEntry(ievt);


    if(!(ievt%500)){
      cout << "Event " << ievt;
      sw_event.Stop();
      //sw_event.Print();
      double CPUtime = sw_event.CpuTime();
      sw_event.Reset();
      sw_event.Start(0);
      cout << " --> CPU time = " << CPUtime << endl;
    }
    
    int EvtID = nd280UpEvent->GetEventID();

    //cout << "# of tracks: " << nd280UpEvent->GetNTracks() << endl;
    
    //PrintTruth(nd280UpEvent,-999);
    //PrintDaughters(nd280UpEvent,2);
    //continue;
    


    
    // Initialize/Reset variables of reconstructed tracks
    fRecoTrack_MPPCHit_XY.clear();
    fRecoTrack_MPPCHit_XZ.clear();
    fRecoTrack_MPPCHit_YZ.clear();
    fTrueTrack_MCHit_XY.clear();
    fTrueTrack_MCHit_XZ.clear();
    fTrueTrack_MCHit_YZ.clear();
    fRecoTrack_ID.clear();
    fRecoTrack_ParentID.clear();
    fRecoTrack_PDG.clear();
    fRecoTrack_outfv.clear();  
    fRecoTrack_isreco.clear();  
    fRecoTrack_reco_costh.clear();  
    fRecoTrack_reco_mom.clear();  
    fRecoTrack_reco_length.clear();
    fRecoTrack_true_length.clear();
    fRecoTrack_true_edep.clear();
    fRecoTrack_hitXY.clear();
    for(unsigned int i=0;i<fRecoTrack_hitXY.size();i++){
      fRecoTrack_hitXY[i].clear();
      for(unsigned int j=0;j<fRecoTrack_hitXY[i].size();j++){
	fRecoTrack_hitXY[i][j].clear();
      }
    }
    
    // Initialize histograms
    
    TString name;

    if(ievt<NEvtDisplTot){

      name = TString::Format("gMCHits_XY_%d",ievt);
      gMCHits_XY[ievt] = new TGraph(); 
      gMCHits_XY[ievt]->SetName(name);
      gMCHits_XY[ievt]->SetTitle(name);
      gMCHits_XY[ievt]->SetMarkerSize(1);
      gMCHits_XY[ievt]->SetMarkerStyle(20);
      name = TString::Format("gMCHits_XZ_%d",ievt);
      gMCHits_XZ[ievt] = new TGraph(); 
      gMCHits_XZ[ievt]->SetName(name);
      gMCHits_XZ[ievt]->SetTitle(name);
      gMCHits_XZ[ievt]->SetMarkerSize(1);
      gMCHits_XZ[ievt]->SetMarkerStyle(20);
      name = TString::Format("gMCHits_YZ_%d",ievt);
      gMCHits_YZ[ievt] = new TGraph();
      gMCHits_YZ[ievt]->SetName(name);
      gMCHits_YZ[ievt]->SetTitle(name);
      gMCHits_YZ[ievt]->SetMarkerSize(1);
      gMCHits_YZ[ievt]->SetMarkerStyle(20);

      name = TString::Format("hMPPCHits_XY_%d",ievt);
      hMPPCHits_XY[ievt] = (TH2F*)h2d_xy->Clone(name);
      name = TString::Format("hMPPCHits_XZ_%d",ievt);
      hMPPCHits_XZ[ievt] = (TH2F*)h2d_xz->Clone(name);
      name = TString::Format("hMPPCHits_YZ_%d",ievt);
      hMPPCHits_YZ[ievt] = (TH2F*)h2d_yz->Clone(name);
      
      name = TString::Format("hPEVsTime_x_%d",ievt);
      hPEVsTime_x[ievt] = new TH1F(name,name,100,0,10000);
      name = TString::Format("hPEVsTime_y_%d",ievt);
      hPEVsTime_y[ievt] = new TH1F(name,name,100,0,10000);
      name = TString::Format("hPEVsTime_z_%d",ievt);
      hPEVsTime_z[ievt] = new TH1F(name,name,100,0,10000);    
      
      // Initialize canvases
      name = TString::Format("cMPPCHits_XY_%d",ievt);
      cMPPCHits_XY[ievt] = new TCanvas(name,name);
      name = TString::Format("cMPPCHits_XZ_%d",ievt);
      cMPPCHits_XZ[ievt] = new TCanvas(name,name);
      name = TString::Format("cMPPCHits_YZ_%d",ievt);
      cMPPCHits_YZ[ievt] = new TCanvas(name,name);
    }
    
  
    
    // Get the Truth Vertex position in the Target reference system
    
    double WorldVtx_X = 0.;
    double WorldVtx_Y = 0.;
    double WorldVtx_Z = 0.;
    
    FindVtxTruth(nd280UpEvent,WorldVtx_X,WorldVtx_Y,WorldVtx_Z);
    
    double TargVtx_X = WorldVtx_X + OrigInLocX;
    double TargVtx_Y = WorldVtx_Y + OrigInLocY;
    double TargVtx_Z = WorldVtx_Z + OrigInLocZ;
    
    TMarker *TargVtx_XY = new TMarker(TargVtx_X,TargVtx_Y,0);
    TargVtx_XY->SetMarkerStyle(22);
    TargVtx_XY->SetMarkerColor(kRed);
    TMarker *TargVtx_XZ = new TMarker(TargVtx_X,TargVtx_Z,0);
    TargVtx_XZ->SetMarkerStyle(22);
    TargVtx_XZ->SetMarkerColor(kRed);
    TMarker *TargVtx_YZ = new TMarker(TargVtx_Y,TargVtx_Z,0);
    TargVtx_YZ->SetMarkerStyle(22);
    TargVtx_YZ->SetMarkerColor(kRed);      
    
    ///

  


    // Loop over the hits
    
    int NHits = nd280UpEvent->GetNHits();

    if(DEBUG){
      cout << endl;
      cout << "//////////////////////" << endl;
      cout << "Event: " << ievt << endl;
      cout << "# of hits = " << nd280UpEvent->GetNHits() << endl;
      
      //PrintTruth(nd280UpEvent,-999);
    }
    
    double posX_prev = 0.;
    double posY_prev = 0.;
    double posZ_prev = 0.;
    int trkid_prev = 0.;

    Int_t binsX = h2d_xy->GetXaxis()->GetNbins();
    Int_t binsY = h2d_xy->GetYaxis()->GetNbins();
    Int_t binsZ = h2d_xz->GetYaxis()->GetNbins();

    for(int ihit=0;ihit<NHits;ihit++){ // get last entry
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

      int parentid = nd280UpHit->GetPrimaryId(); 
      //cout << parentid << endl;
      //int detid = (int)nd280upconv::kBadNum; 
      int pdg = nd280UpHit->GetPDG(); // only if MC hits of primary tracks are stored in TrackerSD
      int trkid = nd280UpHit->fContributors.front(); 
      double edep = nd280UpHit->GetEnergyDeposit(); 
      double charge = 1.; // Apply alway Birks! 
      double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
      double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y 
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
      TVector3 lightPos(posX,posY,posZ); // already in local position

      // True costheta defined by the direction wrt first and last MC hit position
      double lenX = nd280UpHit->GetStopX() - nd280UpHit->GetStartX(); 
      double lenY = nd280UpHit->GetStopY() - nd280UpHit->GetStartY(); 
      double lenZ = nd280UpHit->GetStopZ() - nd280UpHit->GetStartZ(); 
     
      //double length_true_mchit = sqrt(lenX*lenX + lenY*lenY + lenZ*lenZ);      
      double length_true_mchit = 0.;

      //
      // compute the mc hit length
      //
      
      // If the track is different the first segment has length null
      if(trkid_prev != trkid){
	posX_prev = posX;
	posY_prev = posY;
	posZ_prev = posZ;	
      }

      double deltaX = posX - posX_prev;
      double deltaY = posY - posY_prev; 
      double deltaZ = posZ - posZ_prev;
      
      length_true_mchit = sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
      
      // if(trkid==4){
      // 	cout << "Position: " << endl;
      // 	cout << " - Pre: " << posX_prev << ", " << posY_prev << ", " << posZ_prev << endl;
      // 	cout << " - Curr: " << posX << ", " << posY << ", " << posZ << endl;
      // 	cout << " - len: " << length_true_mchit << endl;
      // }
      
      
      double steplength = nd280UpHit->GetTrackLength(); // check how it's calculated in geant4

      string detname = nd280UpHit->GetDetName();
      
      //if(pdg==22 || pdg==2112) continue; // don't analyze gammas and neutrons


      //
      // Compute the detector response for each hit
      //
     	
      ApplyResponse.SetTargetID(DetType);
      ApplyResponse.CalcResponse(lightPos,trkid,parentid,charge,time,steplength,edep,detname);
      
      double pex = ApplyResponse.GetHitPE().x();
      double pey = ApplyResponse.GetHitPE().y();
      double pez = ApplyResponse.GetHitPE().z();
      
      if(!UseViewXY) pez = 0;
      if(!UseViewYZ) pex = 0;
      if(!UseViewXZ) pey = 0;

      double timepex = ApplyResponse.GetHitTime().x();
      double timepey = ApplyResponse.GetHitTime().y();
      double timepez = ApplyResponse.GetHitTime().z();
      
      double poshitX = ApplyResponse.GetHitPos().x();
      double poshitY = ApplyResponse.GetHitPos().y();
      double poshitZ = ApplyResponse.GetHitPos().z();
      
      trkid = ApplyResponse.GetHitTrkID();            
      
      if(ievt<NEvtDisplTot){
	//hMPPCHits_XY[ievt]->Fill(poshitX,poshitY,pez); // pe along Z
	//hMPPCHits_XZ[ievt]->Fill(poshitX,poshitZ,pey); // pe along Y
	//hMPPCHits_YZ[ievt]->Fill(poshitY,poshitZ,pex); // pe along X	
	hPEVsTime_x[ievt]->Fill(timepex,pex);
	hPEVsTime_y[ievt]->Fill(timepey,pey);
	hPEVsTime_z[ievt]->Fill(timepez,pez);
      }

      
      // store light yield
      Int_t MPPCx = h2d_xz->GetXaxis()->FindBin(poshitX);
      Int_t MPPCy = h2d_yz->GetXaxis()->FindBin(poshitY);
      Int_t MPPCz = h2d_yz->GetYaxis()->FindBin(poshitZ);
      //cout << edep << ", " << endl;
      //cout << nd280UpHit->GetPEX() << ", " << pex << endl;
      //cout << nd280UpHit->GetPEY() << ", " << pey << endl;
      //cout << nd280UpHit->GetPEZ() << ", " << pez << endl;
      //cout << endl;      







      /*

      /////////////

      //B.Q

      if(pdg==13){
	hMuon_Edep_Raw->Fill(edep);
	if(pex>0)      hMuon_Edep_Calibrated->Fill(pex);
	else if(pey>0) hMuon_Edep_Calibrated->Fill(pey);
	else if(pez>0) hMuon_Edep_Calibrated->Fill(pez);
      }
      else if(pdg==211){
	hPion_Edep_Raw->Fill(edep);
	if(pex>0)      hPion_Edep_Calibrated->Fill(pex);
	else if(pey>0) hPion_Edep_Calibrated->Fill(pey);
	else if(pez>0) hPion_Edep_Calibrated->Fill(pez);
      }
      else if(pdg==2212){
	hProt_Edep_Raw->Fill(edep);
	if(pex>0)      hProt_Edep_Calibrated->Fill(pex);
	else if(pey>0) hProt_Edep_Calibrated->Fill(pey);
	else if(pez>0) hProt_Edep_Calibrated->Fill(pez);
      }
      else if(pdg==11){
	hElec_Edep_Raw->Fill(edep);
	if(pex>0)      hElec_Edep_Calibrated->Fill(pex);
	else if(pey>0) hElec_Edep_Calibrated->Fill(pey);
	else if(pez>0) hElec_Edep_Calibrated->Fill(pez);
      }

      //////////////

      */




    







      //////////////////////////////////////////////////
      //                                              //
      // True "pattern recognition" (100% efficiency) //
      //                                              //
      //////////////////////////////////////////////////

      // Fill vector of TH2F, each element for a different true track
      // Fill vector of PDG
      // Fill vector of TrackID

      bool used=false;
      for(unsigned int itrk=0;itrk<fRecoTrack_ID.size();itrk++){
	if(trkid==fRecoTrack_ID[itrk]) used=true; 
      }     
      if(!used){
	fRecoTrack_ID.push_back(trkid);
	fRecoTrack_PDG.push_back(pdg);	
	fRecoTrack_ParentID.push_back(parentid);	

	fRecoTrack_true_length.push_back(0.); // initialized. Sum of hit lenght later
	fRecoTrack_true_edep.push_back(0.); // initialized. Sum of hit lenght later

	fRecoTrack_MPPCHit_XY.push_back((TH2F*)h2d_xy->Clone("h2d_xy"));
	fRecoTrack_MPPCHit_XZ.push_back((TH2F*)h2d_xz->Clone("h2d_xz"));
	fRecoTrack_MPPCHit_YZ.push_back((TH2F*)h2d_yz->Clone("h2d_yz"));

	fTrueTrack_MCHit_XY.push_back((TGraph*)mygraph->Clone("MCHit"));
	fTrueTrack_MCHit_XZ.push_back((TGraph*)mygraph->Clone("MCHit"));
	fTrueTrack_MCHit_YZ.push_back((TGraph*)mygraph->Clone("MCHit"));

	vector<double> singleHitXY; // get hit (x,y,pe)
	singleHitXY.push_back(poshitX);
	singleHitXY.push_back(poshitY);
	singleHitXY.push_back(pez);
	vector< vector<double> > hitsXY; // add hit to hit list    
	hitsXY.push_back(singleHitXY);
	fRecoTrack_hitXY.push_back(hitsXY); // add hit list to trk (1 for each trk)	
      }
      
      // Find vector index of the track
      int idx = FindIndex(fRecoTrack_ID,trkid);
      
      // Add hits to the track vector
      if(used){ // the first hit is already added
	vector<double> infoXY; // get info of hit (x,y,pe)
	infoXY.push_back(poshitX);
	infoXY.push_back(poshitY);
	infoXY.push_back(pez);
	fRecoTrack_hitXY[idx].push_back(infoXY);
      }
      
      //cout << "length_true_mchit = " << length_true_mchit << endl;

      fRecoTrack_true_length[idx] += length_true_mchit; // initialized to 0 at first mc hit ( if(!used) )
      fRecoTrack_true_edep[idx]   += edep; // initialized to 0 at first mc hit ( if(!used) )


      // Fill the event display
      fRecoTrack_MPPCHit_XY[idx]->Fill(poshitX,poshitY,pez); // pe along Z
      fRecoTrack_MPPCHit_XZ[idx]->Fill(poshitX,poshitZ,pey); // pe along Y
      fRecoTrack_MPPCHit_YZ[idx]->Fill(poshitY,poshitZ,pex); // pe along X            
      
      fTrueTrack_MCHit_XY[idx]->SetPoint(fTrueTrack_MCHit_XY[idx]->GetN(),posX,posY);
      fTrueTrack_MCHit_XZ[idx]->SetPoint(fTrueTrack_MCHit_XZ[idx]->GetN(),posX,posZ);
      fTrueTrack_MCHit_YZ[idx]->SetPoint(fTrueTrack_MCHit_YZ[idx]->GetN(),posY,posZ);
      
      // Update the previous position
      
      posX_prev  = posX;
      posY_prev  = posY;
      posZ_prev  = posZ;
      trkid_prev = trkid;
      
    } // end loop over the hits

    // FILL HITS MAP
    TH2F* hits_map_XY = (TH2F*)h2d_xy->Clone("hits_map_XY");
    TH2F* hits_map_XZ = (TH2F*)h2d_xz->Clone("hits_map_XZ");
    TH2F* hits_map_YZ = (TH2F*)h2d_yz->Clone("hits_map_YZ");

  for(unsigned int itrk=0;itrk<fRecoTrack_ID.size();itrk++){
      hits_map_XY->Add(fRecoTrack_MPPCHit_XY[itrk]); // pe along Z
      hits_map_XZ->Add(fRecoTrack_MPPCHit_XZ[itrk]); // pe along Y
      hits_map_YZ->Add(fRecoTrack_MPPCHit_YZ[itrk]); // pe along X
    }


    // END OF FILLING HITS MAP

    if(ievt<NEvtDisplTot){
  //if(fRecoTrack_PDG[itrk] == 2112){
  //if(fRecoTrack_ID[itrk] == 4){

  //cout << "Track PDG = " << fRecoTrack_PDG[itrk] << endl;
  //    for(unsigned int itrk=0;itrk<fRecoTrack_ID.size();itrk++){
  hMPPCHits_XY[ievt]->Add(hits_map_XY); // pe along Z
  hMPPCHits_XZ[ievt]->Add(hits_map_XZ); // pe along Y
  hMPPCHits_YZ[ievt]->Add(hits_map_YZ); // pe along X
    for(unsigned int itrk=0;itrk<fRecoTrack_ID.size();itrk++){
  AddGraph(gMCHits_XY[ievt],fTrueTrack_MCHit_XY[itrk]); // pe along Z
  AddGraph(gMCHits_XZ[ievt],fTrueTrack_MCHit_XZ[itrk]); // pe along Y
  AddGraph(gMCHits_YZ[ievt],fTrueTrack_MCHit_YZ[itrk]); // pe along X
}
  //}
      }

      // Draw the Canvas
      if(ievt<NEvtDisplTot){
  cMPPCHits_XY[ievt]->cd();
  hMPPCHits_XY[ievt]->Draw("colz");
  gMCHits_XY[ievt]->Draw("p same");
  TargVtx_XY->Draw();

  cMPPCHits_YZ[ievt]->cd();
  hMPPCHits_YZ[ievt]->Draw("colz");
  gMCHits_YZ[ievt]->Draw("p same");
  TargVtx_YZ->Draw();

  cMPPCHits_XZ[ievt]->cd();
  hMPPCHits_XZ[ievt]->Draw("colz");
  gMCHits_XZ[ievt]->Draw("p same");
  TargVtx_XZ->Draw();
      }


    //
    // Loop over the Reco Tracks and reconstruct them
    //
    // The informations for each "reco" track to look at are stored in:
    // - fRecoTrack_MPPCHit_XY: # pe Vs MPPC hit position (XY)
    // - fRecoTrack_MPPCHit_XZ: # pe Vs MPPC hit position (XZ)
    // - fRecoTrack_MPPCHit_YZ: # pe Vs MPPC hit position (YZ)
    // - fRecoTrack_ID: track ID
    // - fRecoTrack_PDG: PDG
    // - fRecoTrack_outfv: flag to define track outfv (exiting or entering) 
    // - fRecoTrack_isreco: flag to define reconstructed track 
    // - fRecoTrack_true_length : true track length (sum of mc hit track lengths)
    // - fRecoTrack_true_edep : true track edep (sum of mc hit track edep)
    //
    
    fND280UpRecoEvent = new TND280UpRecoEvent();
    fND280UpRecoEvent->SetEventID(EvtID);

    if(DEBUG){
      cout << "# of tracks: " << fRecoTrack_ID.size() << endl;
    }

    for(unsigned int itrk=0;itrk<fRecoTrack_ID.size();itrk++){

      NTOTALTRACKSALL_1stLOOP++;

      int pdg = fRecoTrack_PDG[itrk];
      int trkid = fRecoTrack_ID[itrk];
      int parid = fRecoTrack_ParentID[itrk];

      /*

      /// 
      // B.Q.
      vector <double> fRecoTrack_hitEdepCorrected;
      fRecoTrack_hitEdepCorrected.clear();
      /////

      */


      if(DEBUG){
	cout << endl;
	cout << "Track id: " << trkid << ", ";
	cout << "PDG: " << pdg << ", ";
	cout << "ParID: " << parid << ", ";
	//cout << "First MC hit: " << truth_first_mchit[0] << ", " << truth_first_mchit[1] << ", " << truth_first_mchit[2] << endl;
      }

          
      // Initialize the object for track reconstruction
      ND280UpRecoTrack nd280UpRecoTrack; 



      //
      // CHANGED!!!
      //

      // 
      // Get the Truth
      //
      
      double mom_true = nd280upconv::kBadNum;
      double costh_true = nd280upconv::kBadNum;
      double lengthSD_true = nd280upconv::kBadNum;
      double phi_true = nd280upconv::kBadNum;

      FindTruth(nd280UpEvent,trkid,mom_true,costh_true,phi_true,lengthSD_true);

      //bool IsLastPtMomZero_truth = IsLastPtMomZero(nd280UpEvent,trkid); 
            
      double trklen_true = fRecoTrack_true_length[itrk];
      double trkedep_true = fRecoTrack_true_edep[itrk];

      //cout << "trkedep_true = " << trkedep_true << endl;
      //cout << "trklen_true = " << trklen_true << endl;

      if(pdg==13)        gMuon_All_MomVsdEdx_Truth->SetPoint(gMuon_All_MomVsdEdx_Truth->GetN(),mom_true,trkedep_true/trklen_true); // CHANGED!!!
      else if(pdg==211)  gPion_All_MomVsdEdx_Truth->SetPoint(gPion_All_MomVsdEdx_Truth->GetN(),mom_true,trkedep_true/trklen_true); // CHANGED!!!  
      else if(pdg==2212) gProt_All_MomVsdEdx_Truth->SetPoint(gProt_All_MomVsdEdx_Truth->GetN(),mom_true,trkedep_true/trklen_true); // CHANGED!!!
      else if(pdg==11)   gElec_All_MomVsdEdx_Truth->SetPoint(gElec_All_MomVsdEdx_Truth->GetN(),mom_true,trkedep_true/trklen_true); // CHANGED!!!

      /////////



   
      // Cut on the Truth OutFV
      if( UseTruthFV ){   // do it only for the first track to avoid multiple filling

	bool IsTruthOutFV = false;
	
	// Inputs are: for each projections give the th2f with mppc hit and the tgraph with mc hit
	// Analogous to define the OutFV in all the detectors same as for SuperFGD real OutFV cut

	// Define a track In FV truth also by looking the Truth Vtx position
	if(!IsTruthOutFV) IsTruthOutFV = nd280UpRecoTrack.CalcOutFVTrue(fRecoTrack_MPPCHit_XY[itrk],TargVtx_X,TargVtx_Y);
	if(!IsTruthOutFV) IsTruthOutFV = nd280UpRecoTrack.CalcOutFVTrue(fRecoTrack_MPPCHit_XZ[itrk],TargVtx_X,TargVtx_Z);
	if(!IsTruthOutFV) IsTruthOutFV = nd280UpRecoTrack.CalcOutFVTrue(fRecoTrack_MPPCHit_YZ[itrk],TargVtx_Y,TargVtx_Z);
	
	// Define a track In FV truth by looking the MPPC hits: starting and stopping in FV
	if(!IsTruthOutFV) IsTruthOutFV = nd280UpRecoTrack.CalcOutFVTrue(fRecoTrack_MPPCHit_XY[itrk],fTrueTrack_MCHit_XY[itrk]); 
	if(!IsTruthOutFV) IsTruthOutFV = nd280UpRecoTrack.CalcOutFVTrue(fRecoTrack_MPPCHit_XZ[itrk],fTrueTrack_MCHit_XZ[itrk]); 
	if(!IsTruthOutFV) IsTruthOutFV = nd280UpRecoTrack.CalcOutFVTrue(fRecoTrack_MPPCHit_YZ[itrk],fTrueTrack_MCHit_YZ[itrk]); 		

       	//IsTruthOutFV = false; // CHANGED!!!

	if( itrk==0 ){ // Fill only once per event
	  if(IsTruthOutFV){
	    hVtxOut_XY->Fill(TargVtx_X,TargVtx_Y);
	    hVtxOut_XZ->Fill(TargVtx_X,TargVtx_Z);
	    hVtxOut_YZ->Fill(TargVtx_Y,TargVtx_Z);
	  }
	  else{
	    hVtx_XY->Fill(TargVtx_X,TargVtx_Y);
	    hVtx_XZ->Fill(TargVtx_X,TargVtx_Z);
	    hVtx_YZ->Fill(TargVtx_Y,TargVtx_Z);
	  }
	}

	if(IsTruthOutFV){
	  if(DEBUG) cout << "Out the Truth FV --> skip it!!!" << endl;	  
	  continue; 
	}
      }
          
      //cout << "CIAO!!!" << endl;

      //
      
      nd280UpRecoTrack.SetMPPCXY(fRecoTrack_MPPCHit_XY[itrk]);
      nd280UpRecoTrack.SetMPPCXZ(fRecoTrack_MPPCHit_XZ[itrk]);
      nd280UpRecoTrack.SetMPPCYZ(fRecoTrack_MPPCHit_YZ[itrk]);
      nd280UpRecoTrack.SetMinPE(2.);
      
      if(DetType == nd280upconv::kFGDlike){
	nd280UpRecoTrack.SetTrackSeparationMin(10); // (mm) min track distance is 1 bar
      }
      else if(DetType == nd280upconv::kSuperFGD){
	nd280UpRecoTrack.SetTrackSeparationMin(10); // (mm) min track distance is 1 cube
      }      
      //else if(DetType == nd280upconv::kWAGASCI){
      //nd280UpRecoTrack.SetTrackSeparationMin(25); // (mm) min track distance is 1 cube
      //}
      
      // use the default separation if the input parameter is  < 0
      if(MinSeparation>=0.){ 
	nd280UpRecoTrack.SetTrackSeparationMin(MinSeparation); 
      }
      
      nd280UpRecoTrack.DoTracking(DetType); // run the tracking process
      
      

    
      // Check the track separation
      // if the reco track is separated from all the other tracks
      
      bool isseparated = true;      
      
      for(unsigned int itrkoth=0;itrkoth<fRecoTrack_ID.size();itrkoth++){
	
	if(itrkoth == itrk) continue; // don't test the separation on the same track
	
	// Check separation between current "itrk" and other tracks "itrkoth"
	
	TH2F *hEventOth_XY = (TH2F*)fRecoTrack_MPPCHit_XY[itrkoth]->Clone("hEvent_XY");
	TH2F *hEventOth_XZ = (TH2F*)fRecoTrack_MPPCHit_XZ[itrkoth]->Clone("hEvent_XZ");
	TH2F *hEventOth_YZ = (TH2F*)fRecoTrack_MPPCHit_YZ[itrkoth]->Clone("hEvent_YZ");
	
	nd280UpRecoTrack.SetMPPCXY_Other(hEventOth_XY);
	nd280UpRecoTrack.SetMPPCXZ_Other(hEventOth_XZ);
	nd280UpRecoTrack.SetMPPCYZ_Other(hEventOth_YZ);

	nd280UpRecoTrack.DoTrackSeparation();
		
	//TCanvas *c = new TCanvas();
	//hEventOth_XY->Draw("colz");
	//c->Print("hEventOth_XY.pdf");
	
	//hEventOth_XY->Add(fRecoTrack_MPPCHit_XY[itrk]);
	
	//TCanvas *c2 = new TCanvas();
	//hEventOth_XY->Draw("colz");
	//c2->Print("hEventBoth_XY.pdf");
	  
	delete hEventOth_XY; hEventOth_XY = 0;
	delete hEventOth_XZ; hEventOth_XZ = 0;
	delete hEventOth_YZ; hEventOth_YZ = 0;
	
	isseparated = nd280UpRecoTrack.IsSeparated(); 
	
	if(!isseparated) break; // if separated with 
      }

      
      //
      // Get the Reco
      //
      
      // if the reco track is reconstructed 
      bool isreco = nd280UpRecoTrack.IsReco();
      
      // if the reco track is OutFV (entering or exiting)
      bool isoutfv = nd280UpRecoTrack.IsOutFV();
      //isoutfv = false; // CHANGED!!!
      
      // reco track deposited energy
      double trkedep = nd280UpRecoTrack.GetEdep();
      // reco track length
      double trklen_reco = nd280UpRecoTrack.GetLength();
      double trklen_recoX = nd280UpRecoTrack.GetLengthX();
      double trklen_recoY = nd280UpRecoTrack.GetLengthY();
      double trklen_recoZ = nd280UpRecoTrack.GetLengthZ();

      //trklen /=10;//B.Q we wish in cm for now

      // reco track costheta
      double costh_reco = nd280UpRecoTrack.GetRecoCosTheta();
      

      //
      // CHANGED!!!
      //

      // // 
      // // Get the Truth
      // //
      
      // double mom_true = nd280upconv::kBadNum;
      // double costh_true = nd280upconv::kBadNum;
      // double lengthSD_true = nd280upconv::kBadNum;
      
      // FindTruth(nd280UpEvent,trkid,mom_true,costh_true,phi_true,lengthSD_true);
      
      bool IsLastPtMomZero_truth = IsLastPtMomZero(nd280UpEvent,trkid); 

      // double trklen_true = fRecoTrack_true_length[itrk];
      // double trkedep_true = fRecoTrack_true_edep[itrk];

      // if(pdg==13)        gMuon_All_MomVsdEdx_Truth->SetPoint(gMuon_All_MomVsdEdx_Truth->GetN(),mom_true,trkedep_true/trklen_true); // CHANGED!!!
      // else if(pdg==211)  gPion_All_MomVsdEdx_Truth->SetPoint(gPion_All_MomVsdEdx_Truth->GetN(),mom_true,trkedep_true/trklen_true); // CHANGED!!!  
      // else if(pdg==2212) gProt_All_MomVsdEdx_Truth->SetPoint(gProt_All_MomVsdEdx_Truth->GetN(),mom_true,trkedep_true/trklen_true); // CHANGED!!!
      // else if(pdg==11)   gElec_All_MomVsdEdx_Truth->SetPoint(gElec_All_MomVsdEdx_Truth->GetN(),mom_true,trkedep_true/trklen_true); // CHANGED!!!
      
      ////////




      if(DEBUG){
	cout << endl;
	cout << "Track ID: " << trkid << endl;
	cout << "Reco: " << endl;
	cout << " - # of hits:" << endl;
	cout << "     XY: " << nd280UpRecoTrack.GetNHitsXY() << endl;
	cout << "     XZ: " << nd280UpRecoTrack.GetNHitsXZ() << endl;
	cout << "     YZ: " << nd280UpRecoTrack.GetNHitsYZ() << endl;
	cout << " - len: " << trklen_reco 
	     << " (" << trklen_recoX << "," << trklen_recoY << "," << trklen_recoZ << ")"
	     << ", edep: " << trkedep 
	     << ", costh = " << costh_reco 
	     << endl;
	cout << "Truth: " << endl;
	cout << " - pdg = " << pdg 
	     << ", mom = " << mom_true
	     << ", costh = " << costh_true
	     << ", phi = " << phi_true
	     << ", trk edep = " << trkedep_true	 
	     << ", trk length = " << trklen_true	 
	     << ", trk length SD = " << lengthSD_true
	     << endl;
	
	if(!isreco) cout << "Is not reconstructed!!!" << endl;
	if(isoutfv) cout << "Is outfv!!!" << endl;
	if(!isseparated) cout << "Is not separated!!!" << endl;
	
	if(!isoutfv && !IsLastPtMomZero_truth) cout << "Truth Mom last point > 0!!!" << endl; // check fake stopping tracks 	
      }


      // Check fake stopping tracks: no hits in the last layers but last pt momentum non-zero

      if(DEBUGPLOT){
	
	if(pdg==13 || pdg==211 || pdg==2212 || pdg==11){
	  
	  if(!isoutfv && isreco && !IsLastPtMomZero_truth){
	    
	    cout << "Event: " << ievt << endl;
	    cout << "Is not stopping!!! --> Truth Mom last point > 0!!!" << endl;
	    
	    name = TString::Format("InFV_LastPtMomNONZERO_MPPCHit_XY_Evt%d_Trk%d_pdg%d.pdf",ievt,itrk,pdg);
	    TCanvas *cXY = new TCanvas("name","name");
	    fRecoTrack_MPPCHit_XY[itrk]->Draw("colz");
	    cXY->Print(name);
	    
	    name = TString::Format("InFV_LastPtMomNONZERO_MPPCHit_XZ_Evt%d_Trk%d_pdg%d.pdf",ievt,itrk,pdg);
	    TCanvas *cXZ = new TCanvas("name","name");
	    fRecoTrack_MPPCHit_XZ[itrk]->Draw("colz");
	    cXZ->Print(name);
	    
	    name = TString::Format("InFV_LastPtMomNONZERO_MPPCHit_YZ_Evt%d_Trk%d_pdg%d.pdf",ievt,itrk,pdg);
	    TCanvas *cYZ = new TCanvas("name","name");
	    fRecoTrack_MPPCHit_YZ[itrk]->Draw("colz");
	    cYZ->Print(name);
	  }
	}
      }

      ///


    
      if(DEBUG){
	
	if(mom_true>3000 
	   && (pdg==211)
	   && !isoutfv 
	   && (!isreco || !isseparated)){
	  
	  // InFV but not Reco
	  
	  cout << "Evt ID: " << EvtID << endl;
	  cout << "pdg = " << pdg << endl;
	  cout << "trkid = " << trkid << endl;
	  cout << "This track is rejected though:"<< endl;
	  cout << " mom_true = " << mom_true << endl;
	  cout << " phi_true = " << phi_true << endl;
	  cout << " costh_true = " << costh_true << endl;
	  cout << " costh_reco = " << costh_reco << endl;
	  cout << " trk edep = " << trkedep_true << endl;	 
	  cout << " trk length = " << trklen_true << endl;	 
	  cout << " trklen_reco = " << trklen_reco << endl;
	  cout << " - # of hits:" << endl;
	  cout << "     XY: " << nd280UpRecoTrack.GetNHitsXY() << endl;
	  cout << "     XZ: " << nd280UpRecoTrack.GetNHitsXZ() << endl;
	  cout << "     YZ: " << nd280UpRecoTrack.GetNHitsYZ() << endl;
	  if(!isreco) cout << "Is not reconstructed!!!" << endl;
	  if(isoutfv) cout << "Is outfv!!!" << endl;
	  if(!isseparated) cout << "Is not separated!!!" << endl;	
	  
	  //PrintTruth(nd280UpEvent,trkid);
	  
	  if(DEBUGPLOT){
	    
	    name = TString::Format("fRecoTrack_MPPCHit_XY_Evt%d_Trk%d.pdf",ievt,itrk);
	    TCanvas *cXY = new TCanvas(name,name);
	    fRecoTrack_MPPCHit_XY[itrk]->Draw("colz");
	    cXY->Print(name);
	    name = TString::Format("fRecoTrack_MPPCHit_XZ_Evt%d_Trk%d.pdf",ievt,itrk);
	    TCanvas *cXZ = new TCanvas(name,name);
	    fRecoTrack_MPPCHit_XZ[itrk]->Draw("colz");
	    cXZ->Print(name);
	    name = TString::Format("fRecoTrack_MPPCHit_YZ_Evt%d_Trk%d.pdf",ievt,itrk);
	    TCanvas *cYZ = new TCanvas(name,name);
	    fRecoTrack_MPPCHit_YZ[itrk]->Draw("colz");
	    cYZ->Print(name);	
	  }
	}
      }
    
      
      // TODO: Calculate the reconstructed energy / momentum      
      // TODO: Fill the TND280UpRecoTrack class w/ MPPC hits (see above)
  



    
      //////////////////////////////////////////////////
      //                                              //
      // Fill the Edep/Length PDFs to use for the PID //	
      //                                              //
      //////////////////////////////////////////////////
      
      //
      // Fill the Edep PDFs to use for the PID 	
      //
    
      if(isreco){
	if(isseparated){

	  if(isoutfv){
	    // It's done by the TPCs
	  }
	  
	  else{ // in FV

	    if(DEBUG){
	      if(trklen_reco > trklen_true) cout << "Length: Reco > True !!!" << endl; 
	      if(trklen_reco < 0.5*trklen_true){ 
		cout << "Length: Reco << True !!!" << endl; 
		//exit(1);
	      } 
	    }
	    
	    if(pdg==13){
	      hMuon_Stopped_EdepOverLen->Fill(trkedep/trklen_reco); 
	      hMuon_Stopped_EdepOverLen_ManyBins->Fill(trkedep/trklen_reco); 
	      hMuon_Stopped_Edep->Fill(trkedep); 
	      hMuon_Stopped_Len->Fill(trklen_reco);

	      gMuon_Stopped_EdepVsLen->SetPoint(gMuon_Stopped_EdepVsLen->GetN(),trklen_reco,trkedep);
	      gMuon_Stopped_EdepVsLen_Truth->SetPoint(gMuon_Stopped_EdepVsLen_Truth->GetN(),trklen_true,trkedep_true); 

	      gMuon_Stopped_MomVsdEdx->SetPoint(gMuon_Stopped_MomVsdEdx->GetN(),mom_true,trkedep/trklen_reco); // LENGTH
	      gMuon_Stopped_MomVsdEdx_Truth->SetPoint(gMuon_Stopped_MomVsdEdx_Truth->GetN(),mom_true,trkedep_true/trklen_true); // LENGTH  
	      gMuon_Stopped_LenVsdEdx->SetPoint(gMuon_Stopped_LenVsdEdx->GetN(),trklen_reco,trkedep/trklen_reco); // LENGTH
	      gMuon_Stopped_LenVsdEdx_Truth->SetPoint(gMuon_Stopped_LenVsdEdx_Truth->GetN(),trklen_true,trkedep_true/trklen_true); // LENGTH 

	      int index = hBinsMom->GetXaxis()->FindBin(mom_true) - 1;
	      hMuon_Stopped_EdepOverLen_Mom[index]->Fill(trkedep/trklen_reco);
	      hMuon_Stopped_EdepOverLen_Mom_ManyBins[index]->Fill(trkedep/trklen_reco);

	      index = hBinsLen->GetXaxis()->FindBin(trklen_reco) - 1;
	      hMuon_Stopped_EdepOverLen_Len[index]->Fill(trkedep/trklen_reco);
	      hMuon_Stopped_EdepOverLen_Len_ManyBins[index]->Fill(trkedep/trklen_reco);
	      	      
	      hMuon_CosTh_TrueVsReco->Fill(costh_true,costh_reco);
	      hMuon_CosTh_RecMinTr->Fill(costh_reco-costh_true);
	      hMuon_Len_TrueVsReco->Fill(trklen_true,trklen_reco);
	      hMuon_Len_RecMinTr->Fill(trklen_reco-trklen_true);
	      hMuon_Len_RecMinTr_Vs_TrLen->Fill(trklen_reco-trklen_true,trklen_true);
	    }
	    else if(pdg==211){
	      hPion_Stopped_EdepOverLen->Fill(trkedep/trklen_reco); 
	      hPion_Stopped_EdepOverLen_ManyBins->Fill(trkedep/trklen_reco); 
	      hPion_Stopped_Edep->Fill(trkedep); 
	      hPion_Stopped_Len->Fill(trklen_reco);

	      gPion_Stopped_EdepVsLen->SetPoint(gPion_Stopped_EdepVsLen->GetN(),trklen_reco,trkedep);
	      gPion_Stopped_EdepVsLen_Truth->SetPoint(gPion_Stopped_EdepVsLen_Truth->GetN(),trklen_true,trkedep_true); /// QUI

	      gPion_Stopped_MomVsdEdx->SetPoint(gPion_Stopped_MomVsdEdx->GetN(),mom_true,trkedep/trklen_reco); // LENGTH
	      gPion_Stopped_MomVsdEdx_Truth->SetPoint(gPion_Stopped_MomVsdEdx_Truth->GetN(),mom_true,trkedep_true/trklen_true); // LENGTH 
	      gPion_Stopped_LenVsdEdx->SetPoint(gPion_Stopped_LenVsdEdx->GetN(),trklen_reco,trkedep/trklen_reco); // LENGTH
	      gPion_Stopped_LenVsdEdx_Truth->SetPoint(gPion_Stopped_LenVsdEdx_Truth->GetN(),trklen_true,trkedep_true/trklen_true); // LENGTH 

	      int index = hBinsMom->GetXaxis()->FindBin(mom_true) - 1;
	      hPion_Stopped_EdepOverLen_Mom[index]->Fill(trkedep/trklen_reco);
	      hPion_Stopped_EdepOverLen_Mom_ManyBins[index]->Fill(trkedep/trklen_reco);

	      index = hBinsLen->GetXaxis()->FindBin(trklen_reco) - 1;
	      hPion_Stopped_EdepOverLen_Len[index]->Fill(trkedep/trklen_reco);
	      hPion_Stopped_EdepOverLen_Len_ManyBins[index]->Fill(trkedep/trklen_reco);
	      	      
	      hPion_CosTh_TrueVsReco->Fill(costh_true,costh_reco);
	      hPion_CosTh_RecMinTr->Fill(costh_reco-costh_true);
	      hPion_Len_TrueVsReco->Fill(trklen_true,trklen_reco);
	      hPion_Len_RecMinTr->Fill(trklen_reco-trklen_true);
	      hPion_Len_RecMinTr_Vs_TrLen->Fill(trklen_reco-trklen_true,trklen_true);
	    }
	    else if(pdg==2212){
	      hProt_Stopped_EdepOverLen->Fill(trkedep/trklen_reco); 
	      hProt_Stopped_EdepOverLen_ManyBins->Fill(trkedep/trklen_reco); 
	      hProt_Stopped_Edep->Fill(trkedep); 
	      hProt_Stopped_Len->Fill(trklen_reco);

	      gProt_Stopped_EdepVsLen->SetPoint(gProt_Stopped_EdepVsLen->GetN(),trklen_reco,trkedep);
	      gProt_Stopped_EdepVsLen_Truth->SetPoint(gProt_Stopped_EdepVsLen_Truth->GetN(),trklen_true,trkedep_true); /// QUI

	      gProt_Stopped_MomVsdEdx->SetPoint(gProt_Stopped_MomVsdEdx->GetN(),mom_true,trkedep/trklen_reco); // LENGTH
	      gProt_Stopped_MomVsdEdx_Truth->SetPoint(gProt_Stopped_MomVsdEdx_Truth->GetN(),mom_true,trkedep_true/trklen_true); // LENGTH 
	      gProt_Stopped_LenVsdEdx->SetPoint(gProt_Stopped_LenVsdEdx->GetN(),trklen_reco,trkedep/trklen_reco); // LENGTH
	      gProt_Stopped_LenVsdEdx_Truth->SetPoint(gProt_Stopped_LenVsdEdx_Truth->GetN(),trklen_true,trkedep_true/trklen_true); // LENGTH 

	      int index = hBinsMom->GetXaxis()->FindBin(mom_true) - 1;
	      hProt_Stopped_EdepOverLen_Mom[index]->Fill(trkedep/trklen_reco);
	      hProt_Stopped_EdepOverLen_Mom_ManyBins[index]->Fill(trkedep/trklen_reco);
	      
	      index = hBinsLen->GetXaxis()->FindBin(trklen_reco) - 1;
	      hProt_Stopped_EdepOverLen_Len[index]->Fill(trkedep/trklen_reco);
	      hProt_Stopped_EdepOverLen_Len_ManyBins[index]->Fill(trkedep/trklen_reco);

	      hProt_CosTh_TrueVsReco->Fill(costh_true,costh_reco);
	      hProt_CosTh_RecMinTr->Fill(costh_reco-costh_true);
	      hProt_Len_TrueVsReco->Fill(trklen_true,trklen_reco);
	      hProt_Len_RecMinTr->Fill(trklen_reco-trklen_true);
	      hProt_Len_RecMinTr_Vs_TrLen->Fill(trklen_reco-trklen_true,trklen_true);
	    }
	    else if(pdg==11){
	      hElec_Stopped_EdepOverLen->Fill(trkedep/trklen_reco); 
	      hElec_Stopped_EdepOverLen_ManyBins->Fill(trkedep/trklen_reco); 
	      hElec_Stopped_Edep->Fill(trkedep); 
	      hElec_Stopped_Len->Fill(trklen_reco);

	      gElec_Stopped_EdepVsLen->SetPoint(gElec_Stopped_EdepVsLen->GetN(),trklen_reco,trkedep);
	      gElec_Stopped_EdepVsLen_Truth->SetPoint(gElec_Stopped_EdepVsLen_Truth->GetN(),trklen_true,trkedep_true); /// QUI

	      gElec_Stopped_MomVsdEdx->SetPoint(gElec_Stopped_MomVsdEdx->GetN(),mom_true,trkedep/trklen_reco); // LENGTH
	      gElec_Stopped_MomVsdEdx_Truth->SetPoint(gElec_Stopped_MomVsdEdx_Truth->GetN(),mom_true,trkedep_true/trklen_true); // LENGTH 
	      gElec_Stopped_LenVsdEdx->SetPoint(gElec_Stopped_LenVsdEdx->GetN(),trklen_reco,trkedep/trklen_reco); // LENGTH
	      gElec_Stopped_LenVsdEdx_Truth->SetPoint(gElec_Stopped_LenVsdEdx_Truth->GetN(),trklen_true,trkedep_true/trklen_true); // LENGTH 
	      
	      int index = hBinsMom->GetXaxis()->FindBin(mom_true) - 1;
	      hElec_Stopped_EdepOverLen_Mom[index]->Fill(trkedep/trklen_reco);
	      hElec_Stopped_EdepOverLen_Mom_ManyBins[index]->Fill(trkedep/trklen_reco);
	      
	      index = hBinsLen->GetXaxis()->FindBin(trklen_reco) - 1;
	      hElec_Stopped_EdepOverLen_Len[index]->Fill(trkedep/trklen_reco);
	      hElec_Stopped_EdepOverLen_Len_ManyBins[index]->Fill(trkedep/trklen_reco);

	      hElec_CosTh_TrueVsReco->Fill(costh_true,costh_reco);
	      hElec_CosTh_RecMinTr->Fill(costh_reco-costh_true);
	      hElec_Len_TrueVsReco->Fill(trklen_true,trklen_reco);
	      hElec_Len_RecMinTr->Fill(trklen_reco-trklen_true);
	      hElec_Len_RecMinTr_Vs_TrLen->Fill(trklen_reco-trklen_true,trklen_true);
	    }




	   
	    /*

	    /////
	    
	    // B.Q PID.
	    
	    for(int ihit=0;ihit<fRecoTrack_hitEdep_Plan[itrk].size();ihit++){
	      double pecorrected=fRecoTrack_hitEdep_Plan[itrk][ihit]*TMath::Abs(costh_true);
	      //cout << "PE content = "<<pecorrected<<endl;
	      fRecoTrack_hitEdepCorrected.push_back(pecorrected);	  
	      if(pdg==13){
		hMuon_EdepOverLen_PerHit->Fill(pecorrected);//to change for GRID scintillator
	      }
	      else if(pdg==211){
		hPion_EdepOverLen_PerHit->Fill(pecorrected);//to change for GRID scintillator
	      }
	      else  if(pdg==2212){
		hProt_EdepOverLen_PerHit->Fill(pecorrected);//to change for GRID scintillator
	      }
	      else if(pdg==11){
		hElec_EdepOverLen_PerHit->Fill(pecorrected);//to change for GRID scintillator
	      }
	    }
	    for(int ihit=0;ihit<fRecoTrack_hitEdep_Grid[itrk].size();ihit++){
	      double sinth_true=TMath::Sqrt(1-costh_true*costh_true);//for grid scinti, correction is in sinth_true
	      double pecorrected=fRecoTrack_hitEdep_Grid[itrk][ihit]*sinth_true;
	      fRecoTrack_hitEdepCorrected.push_back(pecorrected);	  
	      
	      //cout << "PE content = "<<pecorrected<<endl;
	      if(pdg==13){
		hMuon_EdepOverLen_PerHit->Fill(pecorrected);//to change for GRID scintillator
	      }
	      else if(pdg==211){
		hPion_EdepOverLen_PerHit->Fill(pecorrected);//to change for GRID scintillator
	      }
	      else  if(pdg==2212){
		hProt_EdepOverLen_PerHit->Fill(pecorrected);//to change for GRID scintillator
	      }
	      else if(pdg==11){
		hElec_EdepOverLen_PerHit->Fill(pecorrected);//to change for GRID scintillator
	      }
	    }
	    
	    ////////////
	    
	    */





	  } // if outfv
	} // if separated
      } // if reco




      // Fill the total event display (ONLY FULLY CONTAINED PARTICLES!!!)

      if(ievt<NEvtDisplTot){
	//if(fRecoTrack_PDG[itrk] == 2112){
	//if(fRecoTrack_ID[itrk] == 4){
	
	//cout << "Track PDG = " << fRecoTrack_PDG[itrk] << endl;

	hMPPCHits_XY[ievt]->Add(fRecoTrack_MPPCHit_XY[itrk]); // pe along Z 
	hMPPCHits_XZ[ievt]->Add(fRecoTrack_MPPCHit_XZ[itrk]); // pe along Y
	hMPPCHits_YZ[ievt]->Add(fRecoTrack_MPPCHit_YZ[itrk]); // pe along X
	
	AddGraph(gMCHits_XY[ievt],fTrueTrack_MCHit_XY[itrk]); // pe along Z 
	AddGraph(gMCHits_XZ[ievt],fTrueTrack_MCHit_XZ[itrk]); // pe along Y 
	AddGraph(gMCHits_YZ[ievt],fTrueTrack_MCHit_YZ[itrk]); // pe along X 
	//}
      }
    
      //cout << "gMCHits_XY[ievt]->GetN() = " << gMCHits_XY[ievt]->GetN() << endl;

      // Draw the Canvas
      if(ievt<NEvtDisplTot){
	cMPPCHits_XY[ievt]->cd();
	hMPPCHits_XY[ievt]->Draw("colz");
	gMCHits_XY[ievt]->Draw("p same");
	TargVtx_XY->Draw();

	cMPPCHits_YZ[ievt]->cd();
	hMPPCHits_YZ[ievt]->Draw("colz");
	gMCHits_YZ[ievt]->Draw("p same");
	TargVtx_YZ->Draw();

	cMPPCHits_XZ[ievt]->cd();
	hMPPCHits_XZ[ievt]->Draw("colz");
	gMCHits_XZ[ievt]->Draw("p same");
	TargVtx_XZ->Draw();
      }



      // Store the track info
      TND280UpRecoTrack *fND280UpRecoTrack = new TND280UpRecoTrack();
      fND280UpRecoTrack->SetTrackID(trkid);
      fND280UpRecoTrack->SetPDG(pdg);
      fND280UpRecoTrack->SetParentID(parid); 
      fND280UpRecoTrack->SetRange(trklen_reco);       
      fND280UpRecoTrack->SetEdep(trkedep);       
      fND280UpRecoTrack->SetOutFV(isoutfv);
      fND280UpRecoTrack->SetIsReco(isreco);
      fND280UpRecoTrack->SetIsSeparated(isseparated);
      fND280UpRecoTrack->SetTruthMom(mom_true); 
      fND280UpRecoTrack->SetTruthCosTheta(costh_true);
      fND280UpRecoTrack->SetTruthPhi(phi_true);
      fND280UpRecoTrack->SetTruthLength(trklen_true);

      //cout << "mom_true = " << mom_true << endl;
      //cout << "costh_true = " << costh_true << endl;
      //cout << "phi_true = " << phi_true 
      //<< ", " << fND280UpRecoTrack->GetTruthPhi()
      //<< endl;

      //fND280UpRecoTrack->SetRecoCosTheta(costh_true); // TODO!!!

      //fND280UpRecoTrack->SetMPPCHit_XY(fRecoTrack_MPPCHit_XY[itrk]); // NOT NEEDED!!! TOO HEAVY!!!
      //fND280UpRecoTrack->SetMPPCHit_XZ(fRecoTrack_MPPCHit_XZ[itrk]); // NOT NEEDED!!! TOO HEAVY!!!
      //fND280UpRecoTrack->SetMPPCHit_YZ(fRecoTrack_MPPCHit_YZ[itrk]); // NOT NEEDED!!! TOO HEAVY!!!	
      
      fND280UpRecoEvent->AddTrack(fND280UpRecoTrack);	    

    } // end loop over tracks (fRecoTrack_ID)
    
    if(DEBUG){
      cout << endl;
    }

    fND280UpRecoEvent->SetEventID(EvtID);  

    // Fill the output tree with reco events
    fOutEventTree->Fill(); // This slows down a lot!!!
    
    // Free the space of all the track histograms
    for(unsigned int itrk=0;itrk<fRecoTrack_MPPCHit_YZ.size();itrk++){      
      delete (fRecoTrack_MPPCHit_YZ[itrk]); 
      delete (fRecoTrack_MPPCHit_XY[itrk]); 
      delete (fRecoTrack_MPPCHit_XZ[itrk]); 
      delete (fTrueTrack_MCHit_YZ[itrk]); 
      delete (fTrueTrack_MCHit_XY[itrk]); 
      delete (fTrueTrack_MCHit_XZ[itrk]); 
    }
    
  } // end loop over events
 

  ///////////////////////////////////////////////////////////
  //                                                       //
  // The Track Reconstruction is finished here             // 
  // TODO:                                                 //
  // - store ND280UpRecoEvent object Tree in output file   //
  //                                                       //
  // The next part of the code is totally independent:     // 
  // - reads the ND280UpRecoEvent tree                     //
  // - loop over the events and reco tracks                //
  // - for each track it calculates the PID                //
  // - store the track PID info in an output file that     //
  //   will be the input to the event selection (highland) //
  //                                                       //  
  ///////////////////////////////////////////////////////////
  

  
  


  
  //                              
  // Calculate PID on Reco Events 
  //                              
  // The PDFs are:
  // - hMuon_EdepOverLen
  // - hPion_EdepOverLen
  // - hProt_EdepOverLen 
  // - hElec_EdepOverLen
  //

  //
  // TODO:
  // - take the ND280UpRecoEvent objects Tree from input file, 
  //   output of track reconstruction code above (not stored yet)
  //   
    
  cout << endl;
  cout << "Calculate PID on Reco Events" << endl;
  cout << endl;
  
  cout << "# of entries: " << endl;
  cout << " - Muons: " << hMuon_Stopped_EdepOverLen->Integral() << endl;
  cout << " - Pions: " << hPion_Stopped_EdepOverLen->Integral() << endl;
  cout << " - Protons: " << hProt_Stopped_EdepOverLen->Integral() << endl;
  cout << " - Electrons: " << hElec_Stopped_EdepOverLen->Integral() << endl;
  cout << endl;

  /*

  // Normalize the PDFs (attention!!! before it was multiplying not subtractring!!! histograms before 16/11/17 have wrong normalization!!!)

  hMuon_Stopped_EdepOverLen->Scale(1./hMuon_Stopped_EdepOverLen->Integral());
  hProt_Stopped_EdepOverLen->Scale(1./hProt_Stopped_EdepOverLen->Integral());
  hPion_Stopped_EdepOverLen->Scale(1./hPion_Stopped_EdepOverLen->Integral());
  hElec_Stopped_EdepOverLen->Scale(1./hElec_Stopped_EdepOverLen->Integral());

  hMuon_Stopped_EdepOverLen_ManyBins->Scale(1./hMuon_Stopped_EdepOverLen_ManyBins->Integral());
  hProt_Stopped_EdepOverLen_ManyBins->Scale(1./hProt_Stopped_EdepOverLen_ManyBins->Integral());
  hPion_Stopped_EdepOverLen_ManyBins->Scale(1./hPion_Stopped_EdepOverLen_ManyBins->Integral());
  hElec_Stopped_EdepOverLen_ManyBins->Scale(1./hElec_Stopped_EdepOverLen_ManyBins->Integral());

  */
  


  /*
  ND280UpPID nd280UpPID; 
  nd280UpPID.SetPDF("Muon",hMuon_Stopped_EdepOverLen); // ok until TSpline3 (no TF1!)
  nd280UpPID.SetPDF("Pion",hPion_Stopped_EdepOverLen);
  nd280UpPID.SetPDF("Prot",hProt_Stopped_EdepOverLen);
  nd280UpPID.SetPDF("Elec",hElec_Stopped_EdepOverLen);
  */
  
  fOutEventTree->SetBranchAddress("RecoEvent",&fND280UpRecoEvent);  
  int nrecoevents = fOutEventTree->GetEntries();
  
  cout << "# of reco events: " << nrecoevents << endl;
  
  for(int ievt=0;ievt<nrecoevents;ievt++){ // get last entry
    
    fOutEventTree->GetEntry(ievt);
    int EvtID = fND280UpRecoEvent->GetEventID();

    if(!(ievt%500)) cout << "Event " << ievt << endl;

    if(DEBUG){
      cout << endl;
      cout << "ievt: " << ievt << endl;
      cout << "Evt: " << EvtID << endl;
    }

    int ntracks = fND280UpRecoEvent->GetNTracks();
    
    if(DEBUG){
      cout << "# of tracks: " << ntracks << endl;
    }

    for(int itrk=0;itrk<ntracks;itrk++){
      
      TND280UpRecoTrack *fND280UpRecoTrack = fND280UpRecoEvent->GetTrack(itrk);
      
      bool isoutfv = fND280UpRecoTrack->IsOutFV();
      bool isreco = fND280UpRecoTrack->IsReco();
      bool isseparated = fND280UpRecoTrack->IsSeparated();
      
      double edep = fND280UpRecoTrack->GetEdep();
      double range = fND280UpRecoTrack->GetRange();
      int trkID = fND280UpRecoTrack->GetTrackID();
      int parentID = fND280UpRecoTrack->GetParentID();
      int pdg = fND280UpRecoTrack->GetPDG();
      double mom_true = fND280UpRecoTrack->GetTruthMom();
      
      double costheta_true = fND280UpRecoTrack->GetTruthCosTheta();
      double phi_true = fND280UpRecoTrack->GetTruthPhi();
      
      //cout << "mom_true = " << mom_true << endl;
      //cout << "costheta_true = " << costheta_true << endl;
      //cout << "phi_true = " << phi_true << endl;
      
      double edepoverrange = edep / range;
      
      NTOTALTRACKSALL_2ndLOOP++;
      
      if(DEBUG){
	cout << "Track id: " << trkID << ", "
	     << "PDG: " << pdg << ", "
	     << "ParID: " << parentID << ", "
	     << " costh_true: " << costheta_true 
	     << ", phi_true: " << phi_true
	     << ", mom_true: " << mom_true << ", "
	     << " len: " << range << ", edep: " << edep << ", edep/len: " << edep/range << endl;	
	if(isoutfv) cout << ", Track OutFV!!!" << endl;   
	if(!isreco) cout << ", Track NOT Reconstructed!!!" << endl;   	
	if(!isseparated) cout << ", Track NOT Separated!!!" << endl;   	
      }
      
      
      //
      // Reject not reconstructed or outFV tracks
      //
	
      if     (pdg==13){   
	hMuon_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hMuon_TrPhiVsTrCosTh->Fill(phi_true,costheta_true); 
	hMuon_TrMomVsTrPhi->Fill(mom_true,phi_true);
     }
      else if(pdg==211){
	hPion_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hPion_TrPhiVsTrCosTh->Fill(phi_true,costheta_true);
	hPion_TrMomVsTrPhi->Fill(mom_true,phi_true);
      }
      else if(pdg==2212){
	hProt_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hProt_TrPhiVsTrCosTh->Fill(phi_true,costheta_true);
	hProt_TrMomVsTrPhi->Fill(mom_true,phi_true);
      }
      else if(pdg==11){
	hElec_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hElec_TrPhiVsTrCosTh->Fill(phi_true,costheta_true);
	hElec_TrMomVsTrPhi->Fill(mom_true,phi_true);
      }
      
      if(isoutfv) continue;     
      
      if     (pdg==13){
	hMuon_AllIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hMuon_AllIso_TrMomVsTrPhi->Fill(mom_true,phi_true);
	hMuon_AllIso_TrPhiVsTrCosTh->Fill(phi_true,costheta_true);
	hMuon_AllIso_TrMom->Fill(mom_true);
	hMuon_AllIso_TrCosTh->Fill(costheta_true);
	hMuon_AllIso_TrPhi->Fill(phi_true);

	NTOTALINFV_2ndLOOP_mu++;
      }
      else if(pdg==211){
	hPion_AllIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hPion_AllIso_TrMomVsTrPhi->Fill(mom_true,phi_true);
	hPion_AllIso_TrPhiVsTrCosTh->Fill(phi_true,costheta_true);
	hPion_AllIso_TrMom->Fill(mom_true);
	hPion_AllIso_TrCosTh->Fill(costheta_true);
	hPion_AllIso_TrPhi->Fill(phi_true);
	
	NTOTALINFV_2ndLOOP_pi++;
      }
      else if(pdg==2212){
	hProt_AllIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hProt_AllIso_TrMomVsTrPhi->Fill(mom_true,phi_true);
	hProt_AllIso_TrPhiVsTrCosTh->Fill(phi_true,costheta_true);
	hProt_AllIso_TrMom->Fill(mom_true);
	hProt_AllIso_TrCosTh->Fill(costheta_true);
	hProt_AllIso_TrPhi->Fill(phi_true);
	
	NTOTALINFV_2ndLOOP_prot++;
      }
      else if(pdg==11){
	hElec_AllIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hElec_AllIso_TrMomVsTrPhi->Fill(mom_true,phi_true);
	hElec_AllIso_TrPhiVsTrCosTh->Fill(phi_true,costheta_true);
	hElec_AllIso_TrMom->Fill(mom_true);
	hElec_AllIso_TrCosTh->Fill(costheta_true);
	hElec_AllIso_TrPhi->Fill(phi_true);
	
	NTOTALINFV_2ndLOOP_elec++;
      }
      
      if(!isreco)  continue;
      if(!isseparated)  continue;
      
      if     (pdg==13){
	hMuon_EffIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hMuon_EffIso_TrMomVsTrPhi->Fill(mom_true,phi_true);
	hMuon_EffIso_TrPhiVsTrCosTh->Fill(phi_true,costheta_true);
	hMuon_EffIso_TrMom->Fill(mom_true);
	hMuon_EffIso_TrCosTh->Fill(costheta_true);
	hMuon_EffIso_TrPhi->Fill(phi_true);

	NTOTALRECO_2ndLOOP_mu++;
      }
      else if(pdg==211){
	hPion_EffIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hPion_EffIso_TrMomVsTrPhi->Fill(mom_true,phi_true);
	hPion_EffIso_TrPhiVsTrCosTh->Fill(phi_true,costheta_true);
	hPion_EffIso_TrMom->Fill(mom_true);
	hPion_EffIso_TrCosTh->Fill(costheta_true);
	hPion_EffIso_TrPhi->Fill(phi_true);
	
	NTOTALRECO_2ndLOOP_pi++;
      }
      else if(pdg==2212){
	hProt_EffIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hProt_EffIso_TrMomVsTrPhi->Fill(mom_true,phi_true);
	hProt_EffIso_TrPhiVsTrCosTh->Fill(phi_true,costheta_true);
	hProt_EffIso_TrMom->Fill(mom_true);
	hProt_EffIso_TrCosTh->Fill(costheta_true);
	hProt_EffIso_TrPhi->Fill(phi_true);

	NTOTALRECO_2ndLOOP_prot++;
      }
      else if(pdg==11){
	hElec_EffIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hElec_EffIso_TrMomVsTrPhi->Fill(mom_true,phi_true);
	hElec_EffIso_TrPhiVsTrCosTh->Fill(phi_true,costheta_true);
	hElec_EffIso_TrMom->Fill(mom_true);
	hElec_EffIso_TrCosTh->Fill(costheta_true);
	hElec_EffIso_TrPhi->Fill(phi_true);

	NTOTALRECO_2ndLOOP_elec++;
      }

      
      // // Code using TF1      
      // double min_muon = MyFunc_Muon->GetParameter(0);
      // double max_muon = MyFunc_Muon->GetParameter(1);
      // double prob_muon = MyFunc_Muon->Eval(edepoverrange) / Integ_Muon;      
      // double min_prot = MyFunc_Prot->GetParameter(0);
      // double max_prot = MyFunc_Prot->GetParameter(1);
      // double prob_prot = MyFunc_Prot->Eval(edepoverrange) / Integ_Prot;      
      // double min_pion = MyFunc_Pion->GetParameter(0);
      // double max_pion = MyFunc_Pion->GetParameter(1);
      // double prob_pion = MyFunc_Pion->Eval(edepoverrange) / Integ_Pion;
      


      /*

      if(pdg==13){
	//double logratio = nd280UpPID.CalcLogLikeRatio(prob_muon,prob_prot); // use TF1
	//cout << "logratio(tf1) = " << logratio << ", ";
	double logratio = nd280UpPID.CalcLogLikeRatio("Muon","Prot",edepoverrange);
	hLikeRatio_MuProt_TrueMu->Fill(logratio);
     	//cout << "logratio(my) = " << logratio << endl;

	logratio = nd280UpPID.CalcLogLikeRatio("Muon","Pion",edepoverrange);
	hLikeRatio_MuPion_TrueMu->Fill(logratio);
      }
      else if(pdg==2212){
	//double logratio = nd280UpPID.CalcLogLikeRatio(prob_muon,prob_prot);  // use TF1
	double logratio = nd280UpPID.CalcLogLikeRatio("Muon","Prot",edepoverrange); 
	hLikeRatio_MuProt_TrueProt->Fill(logratio);
		
	//logratio = nd280UpPID.CalcLogLikeRatio(prob_pion,prob_prot);  // use TF1
	logratio = nd280UpPID.CalcLogLikeRatio("Pion","Prot",edepoverrange); 
	hLikeRatio_PionProt_TrueProt->Fill(logratio);
      }
      else if(pdg==211){
	//double logratio = nd280UpPID.CalcLogLikeRatio(prob_pion,prob_prot);  // use TF1
	double logratio = nd280UpPID.CalcLogLikeRatio("Pion","Prot",edepoverrange); 
	hLikeRatio_PionProt_TruePion->Fill(logratio);

	logratio = nd280UpPID.CalcLogLikeRatio("Muon","Pion",edepoverrange); 
	hLikeRatio_MuPion_TruePion->Fill(logratio);
      }
      else if(pdg==11){
	//double logratio = nd280UpPID.CalcLogLikeRatio(prob_elec,prob_prot); // use TF1
	//cout << "logratio(tf1) = " << logratio << ", ";
	double logratio = nd280UpPID.CalcLogLikeRatio("Elec","Prot",edepoverrange);
	hLikeRatio_ElecProt_TrueElec->Fill(logratio);
     	//cout << "logratio(my) = " << logratio << endl;

	logratio = nd280UpPID.CalcLogLikeRatio("Elec","Pion",edepoverrange);
	hLikeRatio_ElecPion_TrueElec->Fill(logratio);

	logratio = nd280UpPID.CalcLogLikeRatio("Elec","Muon",edepoverrange);
	hLikeRatio_ElecMu_TrueElec->Fill(logratio);
      }

      */

      // Apply tracking conditions (simplified pattern recognition)
      // - separation among tracks
      // - stopping particles
      // - get track length
      
      // Fill TND280UpRecoTrack after "simplified pattern recognition
      // - start / last track point
      // - track length
      // - true trkid
      // - true pdg
      // - all mppc hits (position,edep) 
      
      //ND280UpPID  nd280UpPID; // class with PID methods
      //nd280UpPID.SetMPPCHit_XY(hMPPCHits_XY[ievt]);
      //nd280UpPID.SetMPPCHit_XZ(hMPPCHits_XZ[ievt]);
      //nd280UpPID.SetMPPCHit_YZ(hMPPCHits_YZ[ievt]);
      //nd280UpPID.SetTrackID();
            
    } // end loop reco tracks
  } // end loop events



  //

  cout << endl;
  cout << "NTOTALTRACKSALL_1stLOOP = " << NTOTALTRACKSALL_1stLOOP << endl;
  cout << "NTOTALTRACKSALL_2ndLOOP = " << NTOTALTRACKSALL_2ndLOOP << endl;
  cout << "NTOTALINFV_2ndLOOP_mu = "   << NTOTALINFV_2ndLOOP_mu << endl;
  cout << "NTOTALINFV_2ndLOOP_pi = "   << NTOTALINFV_2ndLOOP_pi << endl;
  cout << "NTOTALINFV_2ndLOOP_prot = " << NTOTALINFV_2ndLOOP_prot << endl;
  cout << "NTOTALINFV_2ndLOOP_elec = " << NTOTALINFV_2ndLOOP_elec << endl;
  cout << "NTOTALRECO_2ndLOOP_mu = "   << NTOTALRECO_2ndLOOP_mu << endl;
  cout << "NTOTALRECO_2ndLOOP_pi = "   << NTOTALRECO_2ndLOOP_pi << endl;
  cout << "NTOTALRECO_2ndLOOP_prot = " << NTOTALRECO_2ndLOOP_prot << endl;
  cout << "NTOTALRECO_2ndLOOP_elec = " << NTOTALRECO_2ndLOOP_elec << endl;
  cout << endl;

  
 

   
  
  //
  // Write output file
  //
  
  fileout->cd();

  //int last = evtfirst+NEvtDisplTot-1;
  int last = evtfirst+Nentries-1;
  
  //cout << "evtfirst = " << evtfirst << endl;
  //cout << "evtlast = " << last << endl;
  //cout << "NEvtDisplTot = " << NEvtDisplTot << endl;
  //cout << "Nentries = " << Nentries << endl;

  for(int ievtdispl=evtfirst;ievtdispl<=last ;ievtdispl++){ 
    
    // Response

    if(ievtdispl<NEvtDisplTot){
      
      //cout << "ievtdispl = " << ievtdispl << endl;
      
      if(hMPPCHits_XY[ievtdispl]->Integral()>0 ||
	 hMPPCHits_XZ[ievtdispl]->Integral()>0 ||
	 hMPPCHits_YZ[ievtdispl]->Integral()>0 ){
	
	hMPPCHits_XY[ievtdispl]->Write();  
	hMPPCHits_XZ[ievtdispl]->Write();  
	hMPPCHits_YZ[ievtdispl]->Write();

	gMCHits_XY[ievtdispl]->Write();
	gMCHits_XZ[ievtdispl]->Write();
	gMCHits_YZ[ievtdispl]->Write();

	cMPPCHits_XY[ievtdispl]->Write();  
	cMPPCHits_XZ[ievtdispl]->Write();  
	cMPPCHits_YZ[ievtdispl]->Write();
	// //hPEVsTime_x[ievtdispl]->Write(); 
	// //hPEVsTime_y[ievtdispl]->Write(); 
	// //hPEVsTime_z[ievtdispl]->Write(); 
      }
    
    }
  }

  // Vertex distribution
  hVtx_XY->Write();
  hVtx_XZ->Write();
  hVtx_YZ->Write();
  hVtxOut_XY->Write();
  hVtxOut_XZ->Write();
  hVtxOut_YZ->Write();

  // Resolution

  hMuon_CosTh_TrueVsReco->Write();
  hMuon_CosTh_RecMinTr->Write();
  hMuon_Len_TrueVsReco->Write();
  hMuon_Len_RecMinTr->Write();
  hMuon_Len_RecMinTr_Vs_TrLen->Write();

  hPion_CosTh_TrueVsReco->Write();
  hPion_CosTh_RecMinTr->Write();
  hPion_Len_TrueVsReco->Write();
  hPion_Len_RecMinTr->Write();
  hPion_Len_RecMinTr_Vs_TrLen->Write();

  hProt_CosTh_TrueVsReco->Write();
  hProt_CosTh_RecMinTr->Write();
  hProt_Len_TrueVsReco->Write();
  hProt_Len_RecMinTr->Write();
  hProt_Len_RecMinTr_Vs_TrLen->Write();

  hElec_CosTh_TrueVsReco->Write();
  hElec_CosTh_RecMinTr->Write();
  hElec_Len_TrueVsReco->Write();
  hElec_Len_RecMinTr->Write();
  hElec_Len_RecMinTr_Vs_TrLen->Write();
  
  // PID

  hMuon_Edep->Write(); hMuon_Stopped_Edep->Write(); 
  hPion_Edep->Write(); hPion_Stopped_Edep->Write();
  hProt_Edep->Write(); hProt_Stopped_Edep->Write();
  hElec_Edep->Write(); hElec_Stopped_Edep->Write();

  hMuon_Len->Write(); hMuon_Stopped_Len->Write();
  hProt_Len->Write(); hProt_Stopped_Len->Write();
  hPion_Len->Write(); hPion_Stopped_Len->Write();
  hElec_Len->Write(); hElec_Stopped_Len->Write();
  
  hMuon_EdepOverLen->Write(); hMuon_Stopped_EdepOverLen->Write(); hMuon_Stopped_EdepOverLen_ManyBins->Write(); 
  hPion_EdepOverLen->Write(); hPion_Stopped_EdepOverLen->Write(); hPion_Stopped_EdepOverLen_ManyBins->Write(); 
  hProt_EdepOverLen->Write(); hProt_Stopped_EdepOverLen->Write(); hProt_Stopped_EdepOverLen_ManyBins->Write(); 
  hElec_EdepOverLen->Write(); hElec_Stopped_EdepOverLen->Write(); hElec_Stopped_EdepOverLen_ManyBins->Write(); 

  gMuon_Stopped_EdepVsLen->Write();
  gPion_Stopped_EdepVsLen->Write();
  gProt_Stopped_EdepVsLen->Write();
  gElec_Stopped_EdepVsLen->Write();

  gMuon_Stopped_EdepVsLen_Truth->Write();
  gPion_Stopped_EdepVsLen_Truth->Write();
  gProt_Stopped_EdepVsLen_Truth->Write();
  gElec_Stopped_EdepVsLen_Truth->Write();  

  gMuon_Stopped_LenVsdEdx->Write();
  gPion_Stopped_LenVsdEdx->Write();
  gProt_Stopped_LenVsdEdx->Write();
  gElec_Stopped_LenVsdEdx->Write();

  gMuon_Stopped_LenVsdEdx_Truth->Write();
  gPion_Stopped_LenVsdEdx_Truth->Write();
  gProt_Stopped_LenVsdEdx_Truth->Write();
  gElec_Stopped_LenVsdEdx_Truth->Write();  

  gMuon_Stopped_MomVsdEdx->Write();
  gPion_Stopped_MomVsdEdx->Write();
  gProt_Stopped_MomVsdEdx->Write();
  gElec_Stopped_MomVsdEdx->Write();

  gMuon_Stopped_MomVsdEdx_Truth->Write();
  gPion_Stopped_MomVsdEdx_Truth->Write();
  gProt_Stopped_MomVsdEdx_Truth->Write();
  gElec_Stopped_MomVsdEdx_Truth->Write();  

  gMuon_All_MomVsdEdx_Truth->Write();
  gPion_All_MomVsdEdx_Truth->Write();
  gProt_All_MomVsdEdx_Truth->Write();
  gElec_All_MomVsdEdx_Truth->Write();  

  hBinsMom->Write();
  for(int imom=0;imom<NBinsMom;imom++){    
    hMuon_Stopped_EdepOverLen_Mom[imom]->Write();
    hPion_Stopped_EdepOverLen_Mom[imom]->Write();
    hProt_Stopped_EdepOverLen_Mom[imom]->Write();
    hElec_Stopped_EdepOverLen_Mom[imom]->Write();

    hMuon_Stopped_EdepOverLen_Mom_ManyBins[imom]->Write();
    hPion_Stopped_EdepOverLen_Mom_ManyBins[imom]->Write();
    hProt_Stopped_EdepOverLen_Mom_ManyBins[imom]->Write();
    hElec_Stopped_EdepOverLen_Mom_ManyBins[imom]->Write();
  }    

  hBinsLen->Write();
  for(int ilen=0;ilen<NBinsLen;ilen++){    
    
    hMuon_Stopped_EdepOverLen_Len[ilen]->Write();
    hPion_Stopped_EdepOverLen_Len[ilen]->Write();
    hProt_Stopped_EdepOverLen_Len[ilen]->Write();
    hElec_Stopped_EdepOverLen_Len[ilen]->Write();

    hMuon_Stopped_EdepOverLen_Len_ManyBins[ilen]->Write();
    hPion_Stopped_EdepOverLen_Len_ManyBins[ilen]->Write();
    hProt_Stopped_EdepOverLen_Len_ManyBins[ilen]->Write();
    hElec_Stopped_EdepOverLen_Len_ManyBins[ilen]->Write();
  }    

  /*
    nd280UpPID.GetGraph("Muon")->Write();
    nd280UpPID.GetGraph("Pion")->Write();
    nd280UpPID.GetGraph("Prot")->Write();
    nd280UpPID.GetGraph("Elec")->Write();
  */
  
  hLikeRatio_MuProt_TrueMu->Write();
  hLikeRatio_MuProt_TrueProt->Write();
  hLikeRatio_MuPion_TrueMu->Write();
  hLikeRatio_MuPion_TruePion->Write();
  hLikeRatio_PionProt_TruePion->Write();
  hLikeRatio_PionProt_TrueProt->Write();
  hLikeRatio_ElecProt_TrueElec->Write();
  hLikeRatio_ElecMu_TrueElec  ->Write();
  hLikeRatio_ElecPion_TrueElec->Write();

  // Reconstruction

  hMuon_TrMomVsTrCosTh->Write();
  hPion_TrMomVsTrCosTh->Write();
  hProt_TrMomVsTrCosTh->Write();
  hElec_TrMomVsTrCosTh->Write();

  hMuon_TrPhiVsTrCosTh->Write();
  hPion_TrPhiVsTrCosTh->Write();
  hProt_TrPhiVsTrCosTh->Write();
  hElec_TrPhiVsTrCosTh->Write();

  hMuon_TrMomVsTrPhi->Write();
  hPion_TrMomVsTrPhi->Write();
  hProt_TrMomVsTrPhi->Write();
  hElec_TrMomVsTrPhi->Write();

  hMuon_AllIso_TrMomVsTrCosTh->Write();
  hPion_AllIso_TrMomVsTrCosTh->Write();
  hProt_AllIso_TrMomVsTrCosTh->Write();
  hElec_AllIso_TrMomVsTrCosTh->Write();
  hMuon_EffIso_TrMomVsTrCosTh->Write();
  hPion_EffIso_TrMomVsTrCosTh->Write();
  hProt_EffIso_TrMomVsTrCosTh->Write();
  hElec_EffIso_TrMomVsTrCosTh->Write();

  hMuon_AllIso_TrPhiVsTrCosTh->Write();
  hPion_AllIso_TrPhiVsTrCosTh->Write();
  hProt_AllIso_TrPhiVsTrCosTh->Write();
  hElec_AllIso_TrPhiVsTrCosTh->Write();
  hMuon_EffIso_TrPhiVsTrCosTh->Write();
  hPion_EffIso_TrPhiVsTrCosTh->Write();
  hProt_EffIso_TrPhiVsTrCosTh->Write();
  hElec_EffIso_TrPhiVsTrCosTh->Write();

  hMuon_AllIso_TrMomVsTrPhi->Write();
  hPion_AllIso_TrMomVsTrPhi->Write();
  hProt_AllIso_TrMomVsTrPhi->Write();
  hElec_AllIso_TrMomVsTrPhi->Write();
  hMuon_EffIso_TrMomVsTrPhi->Write();
  hPion_EffIso_TrMomVsTrPhi->Write();
  hProt_EffIso_TrMomVsTrPhi->Write();
  hElec_EffIso_TrMomVsTrPhi->Write();

  hMuon_AllIso_TrMom->Write();
  hPion_AllIso_TrMom->Write();
  hProt_AllIso_TrMom->Write();
  hElec_AllIso_TrMom->Write();
  hMuon_EffIso_TrMom->Write();
  hPion_EffIso_TrMom->Write();
  hProt_EffIso_TrMom->Write();
  hElec_EffIso_TrMom->Write();

  hMuon_AllIso_TrCosTh->Write();
  hPion_AllIso_TrCosTh->Write();
  hProt_AllIso_TrCosTh->Write();
  hElec_AllIso_TrCosTh->Write();
  hMuon_EffIso_TrCosTh->Write();
  hPion_EffIso_TrCosTh->Write();
  hProt_EffIso_TrCosTh->Write();
  hElec_EffIso_TrCosTh->Write();

  hMuon_AllIso_TrPhi->Write();
  hPion_AllIso_TrPhi->Write();
  hProt_AllIso_TrPhi->Write();
  hElec_AllIso_TrPhi->Write();
  hMuon_EffIso_TrPhi->Write();
  hPion_EffIso_TrPhi->Write();
  hProt_EffIso_TrPhi->Write();
  hElec_EffIso_TrPhi->Write();
 
  //

  fOutEventTree->Write();
  //
  fileout->Close();
  
  return 0;
}



