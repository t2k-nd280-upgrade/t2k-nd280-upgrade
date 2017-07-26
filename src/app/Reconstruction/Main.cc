
// Davide Sgalaberna 10/7/17

//////////////////////////////////////////////////////////////////
//                                                              //
// TODO GENERAL:                                                //
// - use floats instead of doubles                              //
// - other TODOs along the code                                 // 
// - apply cut on track length                                  // 
// - apply cut on FV                                            //
// - use TSpline3 instead of TF1 --> DONE                       //
// - don't use Birk for neutrons?                               // 
// - Add Poisson fluct to response                              //
// - Ereco and momentum                                         //
// - Fill the TND280UpRecoTrack class w/ MPPC hits (not needed) //
// - Store ND280UpRecoEvent object Tree in output file          //
// - Take the ND280UpRecoEvent objects Tree from input file     //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TTree.h>
#include <TVector3.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TMath.h>

#include <iostream>
#include <fstream>

#include "ND280UpConst.hh"
#include "ND280UpApplyResponse.hh"
#include "ND280UpRecoTrack.hh"
#include "ND280UpPID.hh"

#include "TND280UpEvent.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"

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

inline void FindTruth(TND280UpEvent *event,int mytrkid,double &mymom,double &mycosth){
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

    TND280UpTrackPoint *nd280UpTrackPoint = nd280UpTrack->GetPoint(0);
    
    //cout << "trkid: " << trkID
    //<< ",pdg: " << pdg 
    //<< ", ekin: " << ekin
    //<< ", costheta_track = " << costheta 
    //<< ", mom_track = " << mom
    //<< ", detname = " <<  nd280UpTrackPoint->GetLogVolName()
    //<< endl;

    if(mytrkid==trkID){
      mymom = mom;
      mycosth = costheta;
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




/*

// Use TF1

TSpline3 *MySpline_Muon;
double Likelihood_Muon(double *pe,double *par){  
  if(!MySpline_Muon){
    cerr << "MuSpline_Muon not initialized!" << endl;
    exit(1);
  }  
  double min = MySpline_Muon->GetXmin();
  double max = MySpline_Muon->GetXmax();
  if(pe[0] < min) pe[0] = min;
  if(pe[1] > max) pe[0] = max;
  return MySpline_Muon->Eval(pe[0]);  
}

TSpline3 *MySpline_Pion;
double Likelihood_Pion(double *pe,double *par){  
  if(!MySpline_Pion){
    cerr << "MuSpline_Pion not initialized!" << endl;
    exit(1);
  }  
  double min = MySpline_Pion->GetXmin();
  double max = MySpline_Pion->GetXmax();
  if(pe[0] < min) pe[0] = min;
  if(pe[1] > max) pe[0] = max;
  return MySpline_Pion->Eval(pe[0]);  
}

TSpline3 *MySpline_Prot;
double Likelihood_Prot(double *pe,double *par){  
  if(!MySpline_Prot){
    cerr << "MuSpline_Prot not initialized!" << endl;
    exit(1);
  }  
  double min = MySpline_Prot->GetXmin();
  double max = MySpline_Prot->GetXmax();
  if(pe[0] < min) pe[0] = min;
  if(pe[1] > max) pe[0] = max;
  return MySpline_Prot->Eval(pe[0]);  
}

TSpline3 *MySpline_Elec;
double Likelihood_Elec(double *pe,double *par){  
  if(!MySpline_Elec){
    cerr << "MuSpline_Elec not initialized!" << endl;
    exit(1);
  }  
  double min = MySpline_Elec->GetXmin();
  double max = MySpline_Elec->GetXmax();
  if(pe[0] < min) pe[0] = min;
  if(pe[1] > max) pe[0] = max;
  return MySpline_Elec->Eval(pe[0]);  
}

*/




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{   

  if (argc!=7){   // batch mode                                        
    cout << "You need to provide the following arguments:" << endl;
    cout << " 1) input ROOT file name (from GEANT4 simulation) " << endl;
    cout << " 2) first event number to run" << endl;
    cout << " 3) total number of events to run" << endl;
    cout << " 4) the tag for the output file name" << endl;
    cout << " 5) detector ID: 0-->SuperFGD, 1-->FGD-like, 2-->WAGASCI" << endl;
    cout << " 6) set debug option" << endl;
    exit(1);
  }
  
  string rootfilename = argv[1];
  const int evtfirst = atoi(argv[2]);
  const int nevents = atoi(argv[3]);
  string tag = argv[4];
  const int detectorID = atoi(argv[5]);
  const int DEBUG = atoi(argv[6]);
  
  const int NEvtDisplTot = 10; //atoi(argv[5]); // max # of evt. displays 

  int NTOTALTRACKSALL_1stLOOP = 0;
  int NTOTALTRACKSALL_2ndLOOP = 0;
  int NTOTALOOFV_2ndLOOP = 0;
  int NTOTALRECO_2ndLOOP = 0;

  // Declare variable of reconstructed tracks
  vector<TH2F*> fRecoTrack_MPPCHit_XY;
  vector<TH2F*> fRecoTrack_MPPCHit_XZ;
  vector<TH2F*> fRecoTrack_MPPCHit_YZ;
  vector<int>   fRecoTrack_ID;
  vector<int>   fRecoTrack_PDG;
  vector<int>   fRecoTrack_ParentID;
  vector<double> fRecoTrack_len;
  vector<double> fRecoTrack_edep;
  vector<bool> fRecoTrack_outfv;
  vector<bool> fRecoTrack_isreco;
  vector<double> fRecoTrack_reco_costh;
  vector<double> fRecoTrack_reco_mom;
  
  vector< vector< vector<double> > > fRecoTrack_hitXY; // track< hit< (pos,edep) > >  
  //vector< vector<double*> > fRecoTrack_hitXZ;
  //vector< vector<double*> > fRecoTrack_hitYZ;

  // Declare canvases
  TCanvas *cMPPCHits_XY[NEvtDisplTot]; 
  TCanvas *cMPPCHits_XZ[NEvtDisplTot]; 
  TCanvas *cMPPCHits_YZ[NEvtDisplTot];

  // Declare histograms

  TH2F *hMPPCHits_XY[NEvtDisplTot]; 
  TH2F *hMPPCHits_XZ[NEvtDisplTot]; 
  TH2F *hMPPCHits_YZ[NEvtDisplTot];
  TH1F *hPEVsTime_x[NEvtDisplTot]; 
  TH1F *hPEVsTime_y[NEvtDisplTot]; 
  TH1F *hPEVsTime_z[NEvtDisplTot]; 
  
  // Declare PDF histograms

  TH2F * hMuon_AllIso_TrMomVsTrCosTh = new TH2F("hMuon_AllIso_TrMomVsTrCosTh","All IsoTarget Muons True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hPion_AllIso_TrMomVsTrCosTh = new TH2F("hPion_AllIso_TrMomVsTrCosTh","All IsoTarget Pions True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hProt_AllIso_TrMomVsTrCosTh = new TH2F("hProt_AllIso_TrMomVsTrCosTh","All IsoTarget Protons True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hElec_AllIso_TrMomVsTrCosTh = new TH2F("hElec_AllIso_TrMomVsTrCosTh","All IsoTarget Electrons True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hMuon_EffIso_TrMomVsTrCosTh = new TH2F("hMuon_EffIso_TrMomVsTrCosTh","Efficiency IsoTarget Muons True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hPion_EffIso_TrMomVsTrCosTh = new TH2F("hPion_EffIso_TrMomVsTrCosTh","Efficiency IsoTarget Pions True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hProt_EffIso_TrMomVsTrCosTh = new TH2F("hProt_EffIso_TrMomVsTrCosTh","Efficiency IsoTarget Protons True Mom Vs CosTheta",100,0,10000,10,-1,+1);
  TH2F * hElec_EffIso_TrMomVsTrCosTh = new TH2F("hElec_EffIso_TrMomVsTrCosTh","Efficiency IsoTarget Electrons True Mom Vs CosTheta",100,0,10000,10,-1,+1);

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

  //TH2F * hMuon_CosTh_TrueVsReco = new TH2F("hMuon_CosTh_TrueVsReco","Muon CosTheta True Vs Reco",50,-1,+1);
  //TH2F * hPion_CosTh_TrueVsReco = new TH2F("hPion_CosTh_TrueVsReco","Pion CosTheta True Vs Reco",50,-1,+1);
  //TH2F * hProt_CosTh_TrueVsReco = new TH2F("hProt_CosTh_TrueVsReco","Prot CosTheta True Vs Reco",50,-1,+1);
  //TH2F * hElec_CosTh_TrueVsReco = new TH2F("hElec_CosTh_TrueVsReco","Elec CosTheta True Vs Reco",50,-1,+1);

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
  TH1F * hElec_EdepOverLen = new TH1F("hElec_EdepOverLen","Elec energy deposit over track length",200,0,200);
  TH1F * hMuon_Stopped_EdepOverLen = new TH1F("hMuon_Stopped_EdepOverLen","Stopping Muon energy deposit over track length",200,0,200);
  TH1F * hPion_Stopped_EdepOverLen = new TH1F("hPion_Stopped_EdepOverLen","Stopping Pion energy deposit over track length",200,0,200);
  TH1F * hProt_Stopped_EdepOverLen = new TH1F("hProt_Stopped_EdepOverLen","Stopping Prot energy deposit over track length",200,0,200);
  TH1F * hElec_Stopped_EdepOverLen = new TH1F("hElec_Stopped_EdepOverLen","Stopping Elec energy deposit over track length",200,0,200);

  TH2F * hMuon_EdepVsLen = new TH2F("hMuon_EdepVsLen","Muon energy deposit Vs track length",200,0,20000,300,0,3000); // ~ pe Vs mm (cube)
  TH2F * hPion_EdepVsLen = new TH2F("hPion_EdepVsLen","Pion energy deposit Vs track length",200,0,20000,300,0,3000); // ~ pe Vs mm (cube)
  TH2F * hProt_EdepVsLen = new TH2F("hProt_EdepVsLen","Prot energy deposit Vs track length",200,0,20000,300,0,3000); // ~ pe Vs mm (cube)
  TH2F * hElec_EdepVsLen = new TH2F("hElec_EdepVsLen","Elec energy deposit Vs track length",200,0,20000,300,0,3000); // ~ pe Vs mm (cube)
  TH2F * hMuon_Stopped_EdepVsLen = new TH2F("hMuon_Stopped_EdepVsLen","Muon Stopped energy deposit Vs track length",200,0,20000,300,0,3000); // ~ pe Vs mm (cube)
  TH2F * hPion_Stopped_EdepVsLen = new TH2F("hPion_Stopped_EdepVsLen","Pion Stopped energy deposit Vs track length",200,0,20000,300,0,3000); // ~ pe Vs mm (cube)
  TH2F * hProt_Stopped_EdepVsLen = new TH2F("hProt_Stopped_EdepVsLen","Prot Stopped energy deposit Vs track length",200,0,20000,300,0,3000); // ~ pe Vs mm (cube)
  TH2F * hElec_Stopped_EdepVsLen = new TH2F("hElec_Stopped_EdepVsLen","Elec Stopped energy deposit Vs track length",200,0,20000,300,0,3000); // ~ pe Vs mm (cube)

  TH1F * hLikeRatio_MuProt_TrueMu = new TH1F("hLikeRatio_MuProt_TrueMu","hLikeRatio_MuProt_TrueMu",100,-10,10);
  TH1F * hLikeRatio_MuProt_TrueProt = new TH1F("hLikeRatio_MuProt_TrueProt","hLikeRatio_MuProt_TrueProt",100,-10,10);
  
  TH1F * hLikeRatio_PionProt_TruePion = new TH1F("hLikeRatio_PionProt_TruePion","hLikeRatio_PionProt_TruePion",100,-10,10);
  TH1F * hLikeRatio_PionProt_TrueProt = new TH1F("hLikeRatio_PionProt_TrueProt","hLikeRatio_PionProt_TrueProt",100,-10,10);

  // Set the inputs

  nd280upconv::TargetType_t DetType;
  if     (detectorID == 0) DetType = nd280upconv::kSuperFGD;
  else if(detectorID == 1) DetType = nd280upconv::kFGDlike;
  

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
  
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  //
  // Take the MPPC 2D histogram
  //

  ND280UpApplyResponse ApplyResponse;    
  ApplyResponse.SetMPPCProj2D_XY(h2d_xy);
  ApplyResponse.SetMPPCProj2D_XZ(h2d_xz);
  ApplyResponse.SetMPPCProj2D_YZ(h2d_yz);
  // FGDlike
  ApplyResponse.SetDetNameAlongX("/BarScintHoriz");
  ApplyResponse.SetDetNameAlongY("");
  ApplyResponse.SetDetNameAlongZ("/BarScintVert");
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

  for(int ievt=evtfirst;ievt<=EntryLast;ievt++){ // get last entry
    tinput->GetEntry(ievt);

    if(!(ievt%500)) cout << "Event " << ievt << endl;

    int EvtID = nd280UpEvent->GetEventID();
    
    // Initialize/Reset variables of reconstructed tracks
    fRecoTrack_MPPCHit_XY.clear();
    fRecoTrack_MPPCHit_XZ.clear();
    fRecoTrack_MPPCHit_YZ.clear();
    fRecoTrack_ID.clear();
    fRecoTrack_ParentID.clear();
    fRecoTrack_PDG.clear();
    fRecoTrack_len.clear();
    fRecoTrack_edep.clear();  
    fRecoTrack_outfv.clear();  
    fRecoTrack_isreco.clear();  
    fRecoTrack_reco_costh.clear();  
    fRecoTrack_reco_mom.clear();  
    fRecoTrack_hitXY.clear();
    for(int i=0;i<fRecoTrack_hitXY.size();i++){
      fRecoTrack_hitXY[i].clear();
      for(int j=0;j<fRecoTrack_hitXY[i].size();j++){
	fRecoTrack_hitXY[i][j].clear();
      }
    }
    
    // Initialize histograms
    
    TString name;

    if(ievt<NEvtDisplTot){
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
    
    // Loop over the hits
    
    int NHits = nd280UpEvent->GetNHits();

    if(DEBUG){
      cout << endl;
      cout << "//////////////////////" << endl;
      cout << "Event: " << ievt << endl;
      cout << "# of hits = " << nd280UpEvent->GetNHits() << endl;
    }

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
      double costheta_true = lenZ / sqrt(lenX*lenX + lenY*lenY + lenZ*lenZ);
      
      string detname = nd280UpHit->GetDetName();
      double steplength = nd280UpHit->GetTrackLength();

      if(pdg==22 || pdg==2112) continue; // don't analyze gammas and neutrons

      //cout << "trkid: " << trkid 
      //<< ", pdg = " << pdg 
      //<< ", costheta_true = " << costheta_true 
      //<< endl;



      //
      // Compute the detector response for each hit
      //
     	
      ApplyResponse.SetTargetID(DetType);
      ApplyResponse.CalcResponse(lightPos,trkid,parentid,charge,time,steplength,edep,detname);
      
      double pex = ApplyResponse.GetHitPE().x();
      double pey = ApplyResponse.GetHitPE().y();
      double pez = ApplyResponse.GetHitPE().z();
      
      //pez = 0;
      //pex = 0;
      //pey = 0;

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
      
      //cout << nd280UpHit->GetPEX() << ", " << pex << endl;
      //cout << nd280UpHit->GetPEY() << ", " << pey << endl;
      //cout << nd280UpHit->GetPEZ() << ", " << pez << endl;
      //cout << endl;      


      

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

	fRecoTrack_MPPCHit_XY.push_back((TH2F*)h2d_xy->Clone("h2d_xy"));
	fRecoTrack_MPPCHit_XZ.push_back((TH2F*)h2d_xz->Clone("h2d_xz"));
	fRecoTrack_MPPCHit_YZ.push_back((TH2F*)h2d_yz->Clone("h2d_yz"));
  
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
	//cout << fRecoTrack_hitXY[idx][0][2] << endl;
      }
      
      // Fill the event display
      fRecoTrack_MPPCHit_XY[idx]->Fill(poshitX,poshitY,pez); // pe along Z
      fRecoTrack_MPPCHit_XZ[idx]->Fill(poshitX,poshitZ,pey); // pe along Y
      fRecoTrack_MPPCHit_YZ[idx]->Fill(poshitY,poshitZ,pex); // pe along X            

      
    } // end loop over the hits


    

    
    
    //
    // Loop over the Reco Tracks and reconstruct them
    //
    // The informations for each "reco" track to look at are stored in:
    // - fRecoTrack_MPPCHit_XY: # pe Vs MPPC hit position (XY)
    // - fRecoTrack_MPPCHit_XZ: # pe Vs MPPC hit position (XZ)
    // - fRecoTrack_MPPCHit_YZ: # pe Vs MPPC hit position (YZ)
    // - fRecoTrack_ID: track ID
    // - fRecoTrack_PDG: PDG
    // - fRecoTrack_len: track length 
    // - fRecoTrack_edep: track deposited energy 
    // - fRecoTrack_outfv: flag to define track outfv (exiting or entering) 
    // - fRecoTrack_isreco: flag to define reconstructed track 
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
      
      if(DEBUG){
	cout << endl;
	cout << "Track id: " << trkid << ", ";
	cout << "PDG: " << pdg << ", ";
	cout << "ParID: " << parid << ", ";
      }
      
      // Initialize the object for track reconstruction
      ND280UpRecoTrack nd280UpRecoTrack; // class with reconstruction methods
      nd280UpRecoTrack.SetMPPCXY(fRecoTrack_MPPCHit_XY[itrk]);
      nd280UpRecoTrack.SetMPPCXZ(fRecoTrack_MPPCHit_XZ[itrk]);
      nd280UpRecoTrack.SetMPPCYZ(fRecoTrack_MPPCHit_YZ[itrk]);      
      nd280UpRecoTrack.DoTracking(DetType); // run the tracking process
      
      //
      // Get the Reco
      //
      
      // if the reco track is reconstructed 
      bool isreco = nd280UpRecoTrack.IsReco();
      // if the reco track is OutFV (entering or exiting)
      bool isoutfv = nd280UpRecoTrack.IsOutFV();
      // reco track deposited energy
      double trkedep = nd280UpRecoTrack.GetEdep();
      // reco track length
      double trklen = nd280UpRecoTrack.GetLength();
      // reco track costheta
      double costh_reco = nd280UpRecoTrack.GetRecoCosTheta();
      
      // 
      // Get the Truth
      //
      
      double mom_true = nd280upconv::kBadNum;
      double costh_true = nd280upconv::kBadNum;
      FindTruth(nd280UpEvent,trkid,mom_true,costh_true);

      if(DEBUG){
	cout << endl;
	cout << "Track ID: " << trkid << endl;
	cout << "Reco: " << endl;
	cout << " - # of hits:" << endl;
	cout << "     XY: " << nd280UpRecoTrack.GetNHitsXY() << endl;
	cout << "     XZ: " << nd280UpRecoTrack.GetNHitsXZ() << endl;
	cout << "     YZ: " << nd280UpRecoTrack.GetNHitsYZ() << endl;
	cout << " - len: " << trkedep 
	     << ", edep: " << trklen 
	     << ", costh = " << costh_reco 
	     << endl;
	cout << "Truth: " << endl;
	cout << " - pdg = " << pdg 
	     << ", mom = " << mom_true
	     << ", costh = " << costh_true
	     << endl;
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
	if(isoutfv){
	  
	  if(pdg==13){
	    hMuon_EdepOverLen->Fill(trkedep/trklen);
	    hMuon_EdepVsLen->Fill(trkedep,trklen);
	    hMuon_Edep->Fill(trkedep);
	    hMuon_Len->Fill(trklen);
	  }
	  else if(pdg==211){
	    hPion_EdepOverLen->Fill(trkedep/trklen); 
	    hPion_EdepVsLen->Fill(trkedep,trklen); 
	    hPion_Edep->Fill(trkedep); 
	    hPion_Len->Fill(trklen);
	  }
	  else if(pdg==2212){
	    hProt_EdepOverLen->Fill(trkedep/trklen); 
	    hProt_EdepVsLen->Fill(trkedep,trklen); 
	    hProt_Edep->Fill(trkedep); 
	    hProt_Len->Fill(trklen);
	  }
	  else if(pdg==11){
	    hElec_EdepOverLen->Fill(trkedep/trklen); 
	    hElec_EdepVsLen->Fill(trkedep,trklen); 
	    hElec_Edep->Fill(trkedep); 
	    hElec_Len->Fill(trklen);
	  }
	}

	else{
	  if(pdg==13){
	    hMuon_Stopped_EdepOverLen->Fill(trkedep/trklen);
	    hMuon_Stopped_EdepVsLen->Fill(trkedep,trklen);
	    hMuon_Stopped_Edep->Fill(trkedep);
	    hMuon_Stopped_Len->Fill(trklen);
	  }
	  else if(pdg==211){
	    hPion_Stopped_EdepOverLen->Fill(trkedep/trklen); 
	    hPion_Stopped_EdepVsLen->Fill(trkedep,trklen); 
	    hPion_Stopped_Edep->Fill(trkedep); 
	    hPion_Stopped_Len->Fill(trklen);
	  }
	  else if(pdg==2212){
	    hProt_Stopped_EdepOverLen->Fill(trkedep/trklen); 
	    hProt_Stopped_EdepVsLen->Fill(trkedep,trklen); 
	    hProt_Stopped_Edep->Fill(trkedep); 
	    hProt_Stopped_Len->Fill(trklen);
	  }
	  else if(pdg==11){
	    hElec_Stopped_EdepOverLen->Fill(trkedep/trklen); 
	    hElec_Stopped_EdepVsLen->Fill(trkedep,trklen); 
	    hElec_Stopped_Edep->Fill(trkedep); 
	    hElec_Stopped_Len->Fill(trklen);
	  }
	} // if outfv
      } // if reco
      
      
      // Fill the total event display
      if(ievt<NEvtDisplTot){
	//if(fRecoTrack_PDG[itrk] == 13){
	hMPPCHits_XY[ievt]->Add(fRecoTrack_MPPCHit_XY[itrk]); // pe along Z 
	hMPPCHits_XZ[ievt]->Add(fRecoTrack_MPPCHit_XZ[itrk]); // pe along Y
	hMPPCHits_YZ[ievt]->Add(fRecoTrack_MPPCHit_YZ[itrk]); // pe along X
	//}
      }
      

      // Draw the Canvas
      if(ievt<NEvtDisplTot){
	cMPPCHits_XY[ievt]->cd();
	hMPPCHits_XY[ievt]->Draw("colz");
	cMPPCHits_YZ[ievt]->cd();
	hMPPCHits_YZ[ievt]->Draw("colz");
	cMPPCHits_XZ[ievt]->cd();
	hMPPCHits_XZ[ievt]->Draw("colz");
      }
      
      // Store the track info
      TND280UpRecoTrack *fND280UpRecoTrack = new TND280UpRecoTrack();
      fND280UpRecoTrack->SetTrackID(trkid);
      fND280UpRecoTrack->SetPDG(pdg);
      fND280UpRecoTrack->SetParentID(parid); 
      fND280UpRecoTrack->SetRange(trklen);       
      fND280UpRecoTrack->SetEdep(trkedep);       
      fND280UpRecoTrack->SetOutFV(isoutfv);
      fND280UpRecoTrack->SetIsReco(isreco);
      fND280UpRecoTrack->SetTruthMom(mom_true); 
      fND280UpRecoTrack->SetTruthCosTheta(costh_true);
      //fND280UpRecoTrack->SetRecoCosTheta(costh_true); // TODO!!!
      
      //fND280UpRecoTrack->SetMPPCHit_XY(fRecoTrack_MPPCHit_XY[itrk]); // NOT NEEDED!!! TOO HEAVY!!!
      //fND280UpRecoTrack->SetMPPCHit_XZ(fRecoTrack_MPPCHit_XZ[itrk]); // NOT NEEDED!!! TOO HEAVY!!!
      //fND280UpRecoTrack->SetMPPCHit_YZ(fRecoTrack_MPPCHit_YZ[itrk]); // NOT NEEDED!!! TOO HEAVY!!!	
      
      fND280UpRecoEvent->AddTrack(fND280UpRecoTrack);
    }
    
    if(DEBUG){
      cout << endl;
    }

    fND280UpRecoEvent->SetEventID(EvtID);  

    // Fill the output tree with reco events
    fOutEventTree->Fill(); // This slows down a lot!!!
    
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

  ND280UpPID nd280UpPID; 
  nd280UpPID.SetPDF("Muon",hMuon_Stopped_EdepOverLen); // ok until TSpline3 (no TF1!)
  nd280UpPID.SetPDF("Pion",hPion_Stopped_EdepOverLen);
  nd280UpPID.SetPDF("Prot",hProt_Stopped_EdepOverLen);
  //nd280UpPID.SetPDF("Elec",hElec_EdepOverLen);
  
    
  // // Use TF1

  // MySpline_Muon = new TSpline3("MySpline_Muon",nd280UpPID.GetGraph_Muon());
  // double Start=MySpline_Muon->GetXmin(); double End=MySpline_Muon->GetXmax();
  // TF1 *MyFunc_Muon = new TF1("MyFunc_Muon",Likelihood_Muon,Start,End,2);
  // MyFunc_Muon->SetParameter(0,Start);
  // MyFunc_Muon->SetParameter(1,End);
  // double Integ_Muon = MyFunc_Muon->Integral(Start,End);
  // cout << "MyFunc_Muon->Integral: " <<  MyFunc_Muon->Integral(Start,End) << endl;

  // MySpline_Pion = new TSpline3("MySpline_Pion",nd280UpPID.GetGraph_Pion());
  // Start=MySpline_Pion->GetXmin(); End=MySpline_Pion->GetXmax();
  // TF1 *MyFunc_Pion = new TF1("MyFunc_Pion",Likelihood_Pion,Start,End,2);
  // MyFunc_Pion->SetParameter(0,Start);
  // MyFunc_Pion->SetParameter(1,End);
  // double Integ_Pion = MyFunc_Pion->Integral(Start,End);
  // cout << "MyFunc_Pion->Integral: " <<  MyFunc_Pion->Integral(Start,End) << endl;

  // MySpline_Prot = new TSpline3("MySpline_Prot",nd280UpPID.GetGraph_Prot());
  // Start=MySpline_Prot->GetXmin(); End=MySpline_Prot->GetXmax();
  // TF1 *MyFunc_Prot = new TF1("MyFunc_Prot",Likelihood_Prot,Start,End,2);
  // MyFunc_Prot->SetParameter(0,Start);
  // MyFunc_Prot->SetParameter(1,End);
  // double Integ_Prot = MyFunc_Prot->Integral(Start,End);
  // cout << "MyFunc_Prot->Integral: " <<  MyFunc_Prot->Integral(Start,End) << endl;

  // MySpline_Elec = new TSpline3("MySpline_Elec",nd280UpPID.GetGraph_Elec());
  // Start=MySpline_Elec->GetXmin(); End=MySpline_Elec->GetXmax();
  // TF1 *MyFunc_Elec = new TF1("MyFunc_Elec",Likelihood_Elec,Start,End,2);
  // MyFunc_Elec->SetParameter(0,Start);
  // MyFunc_Elec->SetParameter(1,End);
  // double Integ_Elec = MyFunc_Elec->Integral(Start,End);
  // cout << "MyFunc_Elec->Integral: " <<  MyFunc_Elec->Integral(Start,End) << endl;
  
  
  TCanvas *cfunc_muon = new TCanvas();
  nd280UpPID.GetGraph("Muon")->Draw("");
  cfunc_muon->Print("cfunc_muon.pdf");

  TCanvas *cfunc_pion = new TCanvas();
  nd280UpPID.GetGraph("Pion")->Draw("");
  cfunc_pion->Print("cfunc_pion.pdf");

  TCanvas *cfunc_prot = new TCanvas();
  nd280UpPID.GetGraph("Prot")->Draw("");
  cfunc_prot->Print("cfunc_prot.pdf");

  //TCanvas *cfunc_elec = new TCanvas();
  //nd280UpPID.GetGraph("Elec")->Draw("");
  //cfunc_elec->Print("cfunc_elec.pdf");

  //

    
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
      
      double edep = fND280UpRecoTrack->GetEdep();
      double range = fND280UpRecoTrack->GetRange();
      int trkID = fND280UpRecoTrack->GetTrackID();
      int parentID = fND280UpRecoTrack->GetParentID();
      int pdg = fND280UpRecoTrack->GetPDG();
      double mom_true = fND280UpRecoTrack->GetTruthMom();
      double costheta_true = fND280UpRecoTrack->GetTruthCosTheta();
      double edepoverrange = edep / range;

      NTOTALTRACKSALL_2ndLOOP++;
      
      if(DEBUG){
	cout << "Track id: " << trkID << ", "
	     << "PDG: " << pdg << ", "
	     << "ParID: " << parentID << ", "
	     << " costh_true: " << costheta_true << ", mom_true: " << mom_true << ", "
	     << " len: " << range << ", edep: " << edep << ", edep/len: " << edep/range << endl;	
	if(isoutfv) cout << ", Track OutFV!!!" << endl;   
	if(!isreco) cout << ", Track NOT Reconstructed!!!" << endl;   
      }

      
      //
      // Reject not reconstructed or outFV tracks
      //

      if(!isoutfv) continue;

      NTOTALOOFV_2ndLOOP++;
      
	if     (pdg==13){
	hMuon_AllIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hMuon_AllIso_TrMom->Fill(mom_true);
	hMuon_AllIso_TrCosTh->Fill(costheta_true);
      }
      else if(pdg==211){
	hPion_AllIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hPion_AllIso_TrMom->Fill(mom_true);
	hPion_AllIso_TrCosTh->Fill(costheta_true);
      }
      else if(pdg==2212){
	hProt_AllIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hProt_AllIso_TrMom->Fill(mom_true);
	hProt_AllIso_TrCosTh->Fill(costheta_true);
      }
      
      if(!isreco)  continue;

      NTOTALRECO_2ndLOOP++;

      if     (pdg==13){
	hMuon_EffIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hMuon_EffIso_TrMom->Fill(mom_true);
	hMuon_EffIso_TrCosTh->Fill(costheta_true);
      }
      else if(pdg==211){
	hPion_EffIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hPion_EffIso_TrMom->Fill(mom_true);
	hPion_EffIso_TrCosTh->Fill(costheta_true);
      }
      else if(pdg==2212){
	hProt_EffIso_TrMomVsTrCosTh->Fill(mom_true,costheta_true);
	hProt_EffIso_TrMom->Fill(mom_true);
	hProt_EffIso_TrCosTh->Fill(costheta_true);
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
      



      if(pdg==13){
	//double logratio = nd280UpPID.CalcLogLikeRatio(prob_muon,prob_prot); // use TF1
	//cout << "logratio(tf1) = " << logratio << ", ";
	double logratio = nd280UpPID.CalcLogLikeRatio("Muon","Prot",edepoverrange); 	
	hLikeRatio_MuProt_TrueMu->Fill(logratio);
     	//cout << "logratio(my) = " << logratio << endl;
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
      }

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
  // Compute track reconstruction efficiencies
  //
  
  hMuon_EffIso_TrMomVsTrCosTh->Divide(hMuon_AllIso_TrMomVsTrCosTh);
  hPion_EffIso_TrMomVsTrCosTh->Divide(hPion_AllIso_TrMomVsTrCosTh);
  hProt_EffIso_TrMomVsTrCosTh->Divide(hProt_AllIso_TrMomVsTrCosTh);
 
  hMuon_EffIso_TrMom->Divide(hMuon_AllIso_TrMom);
  hPion_EffIso_TrMom->Divide(hPion_AllIso_TrMom);
  hProt_EffIso_TrMom->Divide(hProt_AllIso_TrMom);

  hMuon_EffIso_TrCosTh->Divide(hMuon_AllIso_TrCosTh);
  hPion_EffIso_TrCosTh->Divide(hPion_AllIso_TrCosTh);
  hProt_EffIso_TrCosTh->Divide(hProt_AllIso_TrCosTh);
 



  //

  cout << endl;
  cout << "NTOTALTRACKSALL_1stLOOP = " << NTOTALTRACKSALL_1stLOOP << endl;
  cout << "NTOTALTRACKSALL_2ndLOOP = " << NTOTALTRACKSALL_2ndLOOP << endl;
  cout << "NTOTALOOFV_2ndLOOP = " << NTOTALOOFV_2ndLOOP << endl;
  cout << "NTOTALRECO_2ndLOOP = " << NTOTALRECO_2ndLOOP << endl;
  cout << endl;

  
 

   
  
  //
  // Write output file
  //
  
  fileout->cd();

  int last = evtfirst+NEvtDisplTot-1;
  for(int ievtdispl=evtfirst;ievtdispl<=last ;ievtdispl++){ 

    // Response

    if(hMPPCHits_XY[ievtdispl]->Integral()>0 ||
       hMPPCHits_XZ[ievtdispl]->Integral()>0 ||
       hMPPCHits_YZ[ievtdispl]->Integral()>0 ){
      hMPPCHits_XY[ievtdispl]->Write();  
      hMPPCHits_XZ[ievtdispl]->Write();  
      hMPPCHits_YZ[ievtdispl]->Write();
      cMPPCHits_XY[ievtdispl]->Write();  
      cMPPCHits_XZ[ievtdispl]->Write();  
      cMPPCHits_YZ[ievtdispl]->Write();
      // //hPEVsTime_x[ievtdispl]->Write(); 
      // //hPEVsTime_y[ievtdispl]->Write(); 
      // //hPEVsTime_z[ievtdispl]->Write(); 
    }
  }

  // PID

  hMuon_Edep->Write(); hMuon_Stopped_Edep->Write(); 
  hPion_Edep->Write(); hPion_Stopped_Edep->Write();
  hProt_Edep->Write(); hProt_Stopped_Edep->Write();
  hElec_Edep->Write(); hElec_Stopped_Edep->Write();

  hMuon_Len->Write(); hMuon_Stopped_Len->Write();
  hProt_Len->Write(); hProt_Stopped_Len->Write();
  hPion_Len->Write(); hPion_Stopped_Len->Write();
  hElec_Len->Write(); hElec_Stopped_Len->Write();
  
  hMuon_EdepOverLen->Write(); hMuon_Stopped_EdepOverLen->Write(); 
  hPion_EdepOverLen->Write(); hPion_Stopped_EdepOverLen->Write(); 
  hProt_EdepOverLen->Write(); hProt_Stopped_EdepOverLen->Write(); 
  hElec_EdepOverLen->Write(); hElec_Stopped_EdepOverLen->Write(); 

  hMuon_EdepVsLen->Write(); hMuon_Stopped_EdepVsLen->Write(); 
  hPion_EdepVsLen->Write(); hPion_Stopped_EdepVsLen->Write(); 
  hProt_EdepVsLen->Write(); hProt_Stopped_EdepVsLen->Write(); 
  hElec_EdepVsLen->Write(); hElec_Stopped_EdepVsLen->Write(); 

  nd280UpPID.GetGraph("Muon")->Write();
  nd280UpPID.GetGraph("Pion")->Write();
  nd280UpPID.GetGraph("Prot")->Write();
  //nd280UpPID.GetGraph("Elec")->Write();
  
  hLikeRatio_MuProt_TrueMu->Write();
  hLikeRatio_MuProt_TrueProt->Write();
  hLikeRatio_PionProt_TruePion->Write();
  hLikeRatio_PionProt_TrueProt->Write();

  // Reconstruction

  hMuon_AllIso_TrMomVsTrCosTh->Write();
  hPion_AllIso_TrMomVsTrCosTh->Write();
  hProt_AllIso_TrMomVsTrCosTh->Write();
  hMuon_EffIso_TrMomVsTrCosTh->Write();
  hPion_EffIso_TrMomVsTrCosTh->Write();
  hProt_EffIso_TrMomVsTrCosTh->Write();

  hMuon_AllIso_TrMom->Write();
  hPion_AllIso_TrMom->Write();
  hProt_AllIso_TrMom->Write();
  hMuon_EffIso_TrMom->Write();
  hPion_EffIso_TrMom->Write();
  hProt_EffIso_TrMom->Write();

  hMuon_AllIso_TrCosTh->Write();
  hPion_AllIso_TrCosTh->Write();
  hProt_AllIso_TrCosTh->Write();
  hMuon_EffIso_TrCosTh->Write();
  hPion_EffIso_TrCosTh->Write();
  hProt_EffIso_TrCosTh->Write();
 
  //

  fOutEventTree->Write();
  //
  fileout->Close();
  
  return 0;
}


