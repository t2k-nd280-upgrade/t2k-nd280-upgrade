#include <TH1D.h>
#include <TH2D.h>
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include <TGraph.h>
#include <TF1.h>
#include <TPolyMarker3D.h>

#include "TND280UpHit.hh"
#include "TND280UpTrackPoint.hh"
#include "TND280UpTrack.hh"
#include "TND280UpVertex.hh"
#include "TND280UpEvent.hh"

#include "ND280UpConst.hh"
#include <ND280UpApplyResponse.hh>
#include "ND280UpRecoTrack.hh"
#include "ND280UpPID.hh"

#include "LightYield.hh"

bool IsTargetIn(double x,double y,double z,
                // Target 1
                double vtx_min_x_1,
                double vtx_max_x_1,
                double vtx_min_y_1,
                double vtx_max_y_1,
                double vtx_min_z_1,
                double vtx_max_z_1
                );
TF1 *RatioCalc(TString name, int e_step_tot, TH1F *hPE_Track, double &ratio);
void Fill_RejHiLo(TH1F *hETrack_HiLoRatio_Rej, TH1F *hRejected_View, TH1F *hETrack_RejNumHit, TH2F *hRejView_RejNumHit, TH2F *hRejView_HiLoRej, double e_step_tot_N, double &fratio_hilo_N, double viewN);

double CalcPtDistance(double *pt1, double *pt2);
bool CheckTrackSeparation(TGraph *track1, TGraph *track2, TH2F *hMPPCHits);

inline void AddGraph(TGraph *gAll,TGraph *gSubSet);
inline int FindIndex(vector<int> vec,int val);

int SuperFGDEGammaSeparation (int argc, char** argv){
 if (argc!=10) {
   
   cout << "1) input ROOT file name" << endl;
   cout << "2) first event number" << endl;
   cout << "3) total number of events" << endl;
   cout << "4) tag ([tag].root)" << endl;
   cout << "5) Neutrino interaction (1, nue) or particle gun (0)" << endl;
   cout << "6) use electron" << endl;
   cout << "7) e/gamma ly ratio criteria" << endl;
   cout << "8) Write plots (1=yes, 0=no)" << endl;
   cout << "9) # of plots to write" << endl;
   exit(1);

 }

 string infilename = argv[1];
 const int evtfirst = atoi(argv[2]);
 const int nevents = atoi(argv[3]);
 string tag = argv[4];

 // Set the inputs

 nd280upconv::TargetType_t DetType;
 DetType = nd280upconv::kSuperFGD;

 const bool isNuInt = atoi(argv[5]);
 const bool useElectron = atoi(argv[6]);
 const double RatioCriteria = atoi(argv[7]);
 const bool writePlots = atoi(argv[8]);
 const int nPlotsWrite = atoi(argv[9]);

 int e_trkid = 0;
 int pos_trkid = 0;

  if(nevents>1000){
    cout << "You cannot use more than 1000 events!!!" << endl;
    exit(1);
  }

  const int NEvtDisplTot = nevents; 

  //Defines Track ID to study (currently optimized for particle gun settings)
  if (useElectron) e_trkid = 1;
  else e_trkid = 2;
  pos_trkid = 3;

  TH1F *hPE_X_etrack[NEvtDisplTot];
  TH1F *hPE_Y_etrack[NEvtDisplTot];
  TH1F *hPE_Z_etrack[NEvtDisplTot];

  TF1 *fstep_X[NEvtDisplTot];
  TF1 *fstep_Y[NEvtDisplTot];
  TF1 *fstep_Z[NEvtDisplTot];

  double fratio_hilo_X[NEvtDisplTot];
  double fratio_hilo_Y[NEvtDisplTot];
  double fratio_hilo_Z[NEvtDisplTot];
  double fratio_hilo_Prod[NEvtDisplTot];

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

  vector< vector< vector<double> > > fRecoTrack_hitXY; // track< hit< (trkid,pdg,pos,pe) > >
  vector< vector< vector<double> > > fRecoTrack_hitXZ;
  vector< vector< vector<double> > > fRecoTrack_hitYZ;

  TH2F *hMPPCHits_XY[NEvtDisplTot];
  TH2F *hMPPCHits_XZ[NEvtDisplTot];
  TH2F *hMPPCHits_YZ[NEvtDisplTot];

  TGraph *gMCElectronHits_XY[NEvtDisplTot];
  TGraph *gMCElectronHits_XZ[NEvtDisplTot];
  TGraph *gMCElectronHits_YZ[NEvtDisplTot];

  TGraph *gMCPositronHits_XY[NEvtDisplTot];
  TGraph *gMCPositronHits_XZ[NEvtDisplTot];
  TGraph *gMCPositronHits_YZ[NEvtDisplTot];

  TH1F *hETrack_HiLoRatio_X = new TH1F("hETrack_HiLoRatio_X","hETrack_HiLoRatio_X",40,0.,10.);
  TH1F *hETrack_HiLoRatio_Y = new TH1F("hETrack_HiLoRatio_Y","hETrack_HiLoRatio_Y",40,0.,10.);
  TH1F *hETrack_HiLoRatio_Z = new TH1F("hETrack_HiLoRatio_Z","hETrack_HiLoRatio_Z",40,0.,10.);

  TH1F *hETrack_HiLoRatio_Prod = new TH1F("hETrack_HiLoRatio_Prod","hETrack_HiLoRatio_Prod",80,0,20.);

  TH2F *hView_HiLo = new TH2F("hView_HiLo","",3,0.5,3.5,40,0.,10.);
  TH2F *hView_NumHit = new TH2F("hView_NumHit","",3,0.5,3.5,100,0.,200.);

  const char *view_labels[3] = {"XY","XZ","YZ"};
  for (int l=0;l<3;l++){
    hView_HiLo->GetXaxis()->SetBinLabel(l+1,view_labels[l]);
    hView_NumHit->GetXaxis()->SetBinLabel(l+1,view_labels[l]);
  }
  hView_HiLo->GetYaxis()->SetTitle("Ratio Value");
  hView_NumHit->GetYaxis()->SetTitle("Number of MPPC Hits");

  ////

  TH2F *hElecMom_CosTheta_Passed = new TH2F("hElecMom_CosTheta_Passed","hElecMom_CosTheta_Passed",20,0.,1000.,10.,-1.,1.);
  hElecMom_CosTheta_Passed->GetXaxis()->SetTitle("Electron Momentum (MeV/c)");
  hElecMom_CosTheta_Passed->GetYaxis()->SetTitle("cos #theta");

  TH2F *hElecMom_CosTheta_All = new TH2F("hElecMom_CosTheta_All","hElecMom_CosTheta_All",20,0.,1000.,10.,-1.,1.);
  hElecMom_CosTheta_All->GetXaxis()->SetTitle("Electron Momentum (MeV/c)");
  hElecMom_CosTheta_All->GetYaxis()->SetTitle("cos #theta");

  TFile *finput = new TFile(infilename.c_str(),"READ");

  // Take 2D projection histograms with MPPC hits 

  TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
  h2d_xy = (TH2F*)finput->Get("fMPPCProj2D_XY_Mod01");
  h2d_xz = (TH2F*)finput->Get("fMPPCProj2D_XZ_Mod01");
  h2d_yz = (TH2F*)finput->Get("fMPPCProj2D_YZ_Mod01");  
  //

  // Take the World origin position in the target (local) reference system
  TPolyMarker3D *WorldOrigInLocal = (TPolyMarker3D*)finput->Get("WorldOrigInLocal");
  double OrigInLocX=0.;
  double OrigInLocY=0.;
  double OrigInLocZ=0.;
  WorldOrigInLocal->GetPoint(0,OrigInLocX,OrigInLocY,OrigInLocZ);

  cout << "The World origin in local coordinates is:" << endl;
  cout << OrigInLocX << ", " << OrigInLocY << ", " << OrigInLocZ << endl;
  cout << endl;
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
  //

  
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();  
  tinput->SetBranchAddress("Event",&nd280UpEvent);
  
  int NTreeEntries = tinput->GetEntries();  
  int evtlasttree = evtfirst+NTreeEntries-1;
  int Nentries = nevents;
  //  int Nentries = 1000;

  cout << endl;
  cout << "# of entries in the Tree: " << NTreeEntries << endl;
  cout << "last tree event ID: " << evtlasttree << endl;
  cout << "first event ID: " << evtfirst << endl;
  cout << endl;


  int EntryLast = evtfirst+Nentries-1;

  int elike_count = 0;
  int sepcut_count = 0;

  vector <int> passed_evt;
  passed_evt.clear();

  for(int ievt=0;ievt<Nentries;ievt++){ // get last entry
    
    tinput->GetEntry(ievt);
    
    cout << "evt#" << ievt << endl;
    
    TString name;

    /////

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
    fRecoTrack_hitXZ.clear();
    fRecoTrack_hitYZ.clear();

    for(unsigned int i=0;i<fRecoTrack_hitXY.size();i++){
      fRecoTrack_hitXY[i].clear();
      for(unsigned int j=0;j<fRecoTrack_hitXY[i].size();j++){
        fRecoTrack_hitXY[i][j].clear();
      }
    }

    for(unsigned int i=0;i<fRecoTrack_hitXZ.size();i++){
      fRecoTrack_hitXZ[i].clear();
      for(unsigned int j=0;j<fRecoTrack_hitXZ[i].size();j++){
        fRecoTrack_hitXZ[i][j].clear();
      }
    }


    for(unsigned int i=0;i<fRecoTrack_hitYZ.size();i++){
      fRecoTrack_hitYZ[i].clear();
      for(unsigned int j=0;j<fRecoTrack_hitYZ[i].size();j++){
        fRecoTrack_hitYZ[i][j].clear();
      }
    }


      name = TString::Format("gMCElectronHits_XY_%d",ievt);
      gMCElectronHits_XY[ievt] = new TGraph();
      gMCElectronHits_XY[ievt]->SetName(name);
      gMCElectronHits_XY[ievt]->SetTitle(name);
      gMCElectronHits_XY[ievt]->SetMarkerSize(1);
      gMCElectronHits_XY[ievt]->SetMarkerStyle(20);
      name = TString::Format("gMCElectronHits_XZ_%d",ievt);
      gMCElectronHits_XZ[ievt] = new TGraph();
      gMCElectronHits_XZ[ievt]->SetName(name);
      gMCElectronHits_XZ[ievt]->SetTitle(name);
      gMCElectronHits_XZ[ievt]->SetMarkerSize(1);
      gMCElectronHits_XZ[ievt]->SetMarkerStyle(20);
      name = TString::Format("gMCElectronHits_YZ_%d",ievt);
      gMCElectronHits_YZ[ievt] = new TGraph();
      gMCElectronHits_YZ[ievt]->SetName(name);
      gMCElectronHits_YZ[ievt]->SetTitle(name);
      gMCElectronHits_YZ[ievt]->SetMarkerSize(1);
      gMCElectronHits_YZ[ievt]->SetMarkerStyle(20);

      name = TString::Format("gMCPositronHits_XY_%d",ievt);
      gMCPositronHits_XY[ievt] = new TGraph();
      gMCPositronHits_XY[ievt]->SetName(name);
      gMCPositronHits_XY[ievt]->SetTitle(name);
      gMCPositronHits_XY[ievt]->SetMarkerSize(1);
      gMCPositronHits_XY[ievt]->SetMarkerStyle(20);
      name = TString::Format("gMCPositronHits_XZ_%d",ievt);
      gMCPositronHits_XZ[ievt] = new TGraph();
      gMCPositronHits_XZ[ievt]->SetName(name);
      gMCPositronHits_XZ[ievt]->SetTitle(name);
      gMCPositronHits_XZ[ievt]->SetMarkerSize(1);
      gMCPositronHits_XZ[ievt]->SetMarkerStyle(20);
      name = TString::Format("gMCPositronHits_YZ_%d",ievt);
      gMCPositronHits_YZ[ievt] = new TGraph();
      gMCPositronHits_YZ[ievt]->SetName(name);
      gMCPositronHits_YZ[ievt]->SetTitle(name);
      gMCPositronHits_YZ[ievt]->SetMarkerSize(1);
      gMCPositronHits_YZ[ievt]->SetMarkerStyle(20);




      name = TString::Format("hMPPCHits_XY_%d",ievt);
      hMPPCHits_XY[ievt] = (TH2F*)h2d_xy->Clone(name);
      hMPPCHits_XY[ievt]->SetTitle("");
      hMPPCHits_XY[ievt]->GetXaxis()->SetTitle("Distance from Target Center in X axis (mm)");
      hMPPCHits_XY[ievt]->GetYaxis()->SetTitle("Distance from Target Center in Y axis (mm)");

      name = TString::Format("hMPPCHits_XZ_%d",ievt);
      hMPPCHits_XZ[ievt] = (TH2F*)h2d_xz->Clone(name);
      hMPPCHits_XZ[ievt]->SetTitle("");
      hMPPCHits_XZ[ievt]->GetXaxis()->SetTitle("Distance from Target Center in X axis (mm)");
      hMPPCHits_XZ[ievt]->GetYaxis()->SetTitle("Distance from Target Center in Z axis (mm)");
      
      name = TString::Format("hMPPCHits_YZ_%d",ievt);
      hMPPCHits_YZ[ievt] = (TH2F*)h2d_yz->Clone(name);
      hMPPCHits_YZ[ievt]->SetTitle("");
      hMPPCHits_YZ[ievt]->GetXaxis()->SetTitle("Distance from Target Center in Y axis (mm)");
      hMPPCHits_YZ[ievt]->GetYaxis()->SetTitle("Distance from Target Center in Z axis (mm)");


    /////

      name = TString::Format("fstep_X_%d",ievt);

      fstep_X[ievt] = new TF1(name, "pol0(0)", 0, 200.);
      fstep_X[ievt]->SetParameter(0,0.);
      
      name = TString::Format("fstep_Y_%d",ievt);
      
      fstep_Y[ievt] = new TF1(name, "pol0(0)", 0, 200.);
      fstep_Y[ievt]->SetParameter(0,0.);
      
      name = TString::Format("fstep_Z_%d",ievt);
      
      fstep_Z[ievt] = new TF1(name, "pol0(0)", 0, 200.);
      fstep_Z[ievt]->SetParameter(0,0.);
      
    // Initialize the histograms
      
      name = TString::Format("hPE_X_etrack_%d",ievt);
      hPE_X_etrack[ievt] = new TH1F(name,"",200,0,200);
      hPE_X_etrack[ievt]->GetXaxis()->SetTitle("i-th hit of the e-like track");
      hPE_X_etrack[ievt]->GetYaxis()->SetTitle("p.e.");
      
      name = TString::Format("hPE_Y_etrack_%d",ievt);
      hPE_Y_etrack[ievt] = new TH1F(name,"",200,0,200);
      hPE_Y_etrack[ievt]->GetXaxis()->SetTitle("i-th hit of the e-like track");
      hPE_Y_etrack[ievt]->GetYaxis()->SetTitle("p.e.");
      
      name = TString::Format("hPE_Z_etrack_%d",ievt);
      hPE_Z_etrack[ievt] = new TH1F(name,"",200,0,200);
      hPE_Z_etrack[ievt]->GetXaxis()->SetTitle("i-th hit of the e-like track");
      hPE_Z_etrack[ievt]->GetYaxis()->SetTitle("p.e.");
      

    /////////////
    vector <double> track_momentum;
    track_momentum.clear();

    vector <double> track_costheta;
    track_costheta.clear();

    vector <double> gamma_momentum;
    gamma_momentum.clear();
    /////////////

    //STORES HitInfo X,Y,Z for each e-like track separately
    vector < vector <double> > fHitTrack_X;
    vector < vector <double> > fHitTrack_Y;
    vector < vector <double> > fHitTrack_Z;

    for (size_t i=0; i<fHitTrack_X.size();i++) fHitTrack_X[i].clear();
    fHitTrack_X.clear();
    for (size_t i=0; i<fHitTrack_Y.size();i++) fHitTrack_Y[i].clear();
    fHitTrack_Y.clear();
    for (size_t i=0; i<fHitTrack_Z.size();i++) fHitTrack_Z[i].clear();
    fHitTrack_Z.clear();
    ///

    // Loop over the tracks
  
    int NTracks = nd280UpEvent->GetNTracks();

    bool pair_track_a = false;
    bool pair_track_b = false;

    bool e_track = false;
    bool gamma_track = false;
    bool inthetarget = true;


  
    int NVertices = nd280UpEvent->GetNVertices();
    if(NVertices>1){
      cerr << endl;
      cerr << "More than 1 vertex is not yet supported!!!" << endl;
      cerr << endl;
      exit(1);
    }


    TND280UpVertex *nd280UpVertex;

    bool isCCQE = false;
    bool isnue = false;


    for (int ivtx=0;ivtx<NVertices;ivtx++)
      {
        nd280UpVertex = nd280UpEvent->GetVertex(ivtx);
        //      nd280UpVertex->PrintVertex();
        TND280UpTrack *nd280UpVtxTrkIn1 = nd280UpVertex->GetInTrack(0);
	int pdg_vtxtrk1 = nd280UpVtxTrkIn1->GetPDG();
        int reacmode = abs(nd280UpVertex->GetReacMode());

        if (pdg_vtxtrk1 == 12) isnue = true;
        if (reacmode == 1) isCCQE = true;

      }

    if (isNuInt) {
      if (!isnue) continue;
      if (!isCCQE) continue;
    }

  
    for(int itrk=0;itrk<NTracks;itrk++){
      
      TND280UpTrack *nd280UpTrack = nd280UpEvent->GetTrack(itrk);
      //            nd280UpTrack->PrintTrack();      
      int trkID = nd280UpTrack->GetTrackID();
      int parentID = nd280UpTrack->GetParentID();
      int PDG = nd280UpTrack->GetPDG();
      double costheta = nd280UpTrack->GetInitCosTheta();
      double charge = nd280UpTrack->GetCharge();
      double ekin = nd280UpTrack->GetInitKinEnergy();
      double mom = nd280UpTrack->GetInitMom().Mag(); 
      double range = nd280UpTrack->GetRange(); 
      string procName = nd280UpTrack->GetProcessName();
      int NPoints = nd280UpTrack->GetNPoints();

      TND280UpTrackPoint *nd280UpTrackPoint = nd280UpTrack->GetPoint(0); 
      double length = nd280UpTrackPoint->GetStepLength();
      double PtX = nd280UpTrackPoint->GetPostPosition().X();
      double PtY = nd280UpTrackPoint->GetPostPosition().Y();
      double PtZ = nd280UpTrackPoint->GetPostPosition().Z();
      double PtMom = nd280UpTrackPoint->GetMomentum().Mag();
      string volname = nd280UpTrackPoint->GetLogVolName();

      if (parentID==0){
        inthetarget = IsTargetIn(PtX,PtY,PtZ,-1020.,1020.,-260.,260.,-940.,940.);
      }



      if (!e_track && parentID ==0 && PDG ==11){
	cout << "e track..." << endl;
	cout << ievt << ", parentID: " << parentID << ", trkID: " << trkID << ", pdg: " << PDG << ", mom: " << mom << ", costheta: " <<costheta << ", range: " << range<<  ", proc: "<<procName<< endl;
	track_momentum.push_back(mom);
	track_costheta.push_back(costheta);
	e_track = true;
      }

     
      if (!gamma_track && parentID ==0 && PDG ==22) {
	gamma_momentum.push_back(mom);
	gamma_track = true; 
	cout << ievt << ", gamma"<<endl;
	}

      if (!inthetarget){
        cout << "Out of Target; rejected."<<endl;
        continue;
      }


      if (gamma_track){
	
	if (!pair_track_a && parentID == 1 && PDG==11){
	  cout << "e- #" << ievt << ", parentID: " << parentID << ", trkID: " << trkID << ", pdg: " << PDG << ", mom: " << mom << ", costheta: " <<costheta << ", range: " << range<<  ", proc: "<<procName<< endl;
	  track_momentum.push_back(mom);
	  track_costheta.push_back(costheta);
	  pair_track_a = true;
	}
	
	if (!pair_track_b && parentID == 1 && PDG == -11){
	  cout<< "e+ #" <<ievt << ", parentID: " << parentID << ", trkID: " << trkID << ", pdg: " << PDG << ", mom: " << mom << ", costheta: " <<costheta << ", range: " << range<<  ", proc: "<<procName<< endl;
	  track_momentum.push_back(mom);
	  pair_track_b = true;
	}
      }
    }

  
    if (gamma_track && !(pair_track_a &&pair_track_b)) {
      cout << ievt << ":not e- e+ pair, rejected" <<endl;
      continue;
    }



    /*
    else{
      if (track_momentum[0]<200.){
	cout << ievt << ":low momentum, rejected" <<endl;
	continue;
      }
    }
    */ 
    
    cout << endl;
    cout << " >>> Passed preliminary track cuts  <<< " << endl;
    cout << endl;
    
      passed_evt.push_back(ievt);  

    
    int NHits = nd280UpEvent->GetNHits();

    cout << "NHits:" << NHits << endl;

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
      // nd280UpHit->PrintHit();
      double charge = 1.; // Apply alway Birks!
      int trkid = nd280UpHit->fContributors.front();
      double edep = nd280UpHit->GetEnergyDeposit();
      double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
      double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
      int pdg = nd280UpHit->GetPDG(); // only if MC hits of primary tracks are stored in TrackerSD

      TString detname = nd280UpHit->GetDetName();

      TVector3 lightPos(posX,posY,posZ); // already in local position

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

      double steplength = nd280UpHit->GetTrackLength(); // check how it's calculated in geant4

      ApplyResponse.SetTargetID(DetType);
      ApplyResponse.CalcResponse(lightPos,trkid,parentid,charge,time,steplength,edep,detname);

      double pex = ApplyResponse.GetHitPE().x();
      double pey = ApplyResponse.GetHitPE().y();
      double pez = ApplyResponse.GetHitPE().z();

      double poshitX = ApplyResponse.GetHitPos().x();
      double poshitY = ApplyResponse.GetHitPos().y();
      double poshitZ = ApplyResponse.GetHitPos().z();

      trkid = ApplyResponse.GetHitTrkID();

      ///                
      int particleID = pdg;
  

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
	//	cout << trkid << ", "<< pdg<<  endl;
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

        vector<double> singleHitXY; // get hit (x,y,pez)
	singleHitXY.push_back(trkid);
	singleHitXY.push_back(pdg);
        singleHitXY.push_back(poshitX);
        singleHitXY.push_back(poshitY);
        singleHitXY.push_back(pez);
        vector< vector<double> > hitsXY; // add hit to hit list
        hitsXY.push_back(singleHitXY);
        fRecoTrack_hitXY.push_back(hitsXY); // add hit list to trk (1 for each trk)

	vector<double> singleHitXZ; // get hit (x,z,pey)
	singleHitXZ.push_back(trkid);
	singleHitXZ.push_back(pdg);
	singleHitXZ.push_back(poshitX);
        singleHitXZ.push_back(poshitZ);
        singleHitXZ.push_back(pey);
        vector< vector<double> > hitsXZ; // add hit to hit list
        hitsXZ.push_back(singleHitXZ);
        fRecoTrack_hitXZ.push_back(hitsXZ); // add hit list to trk (1 for each trk)

	vector<double> singleHitYZ; // get hit (y,z,pex)
	singleHitYZ.push_back(trkid);
	singleHitYZ.push_back(pdg);
        singleHitYZ.push_back(poshitY);
        singleHitYZ.push_back(poshitZ);
        singleHitYZ.push_back(pex);
        vector< vector<double> > hitsYZ; // add hit to hit list
        hitsYZ.push_back(singleHitYZ);
        fRecoTrack_hitYZ.push_back(hitsYZ); // add hit list to trk (1 for each trk)
      

      }
      // Find vector index of the track
      int idx = FindIndex(fRecoTrack_ID,trkid);
      //      cout << "idx = " << idx << endl;
      // Add hits to the track vector
      if(used){ // the first hit is already added
        vector<double> infoXY; // get info of hit (x,y,pe)
	infoXY.push_back(trkid);
	infoXY.push_back(pdg);
        infoXY.push_back(poshitX);
        infoXY.push_back(poshitY);
        infoXY.push_back(pez);
        fRecoTrack_hitXY[idx].push_back(infoXY);

        vector<double> infoXZ; // get info of hit (x,z,pe)
	infoXZ.push_back(trkid);
	infoXZ.push_back(pdg);
        infoXZ.push_back(poshitX);
        infoXZ.push_back(poshitZ);
        infoXZ.push_back(pey);
        fRecoTrack_hitXZ[idx].push_back(infoXZ);

        vector<double> infoYZ; // get info of hit (y,z,pe)
	infoYZ.push_back(trkid);
	infoYZ.push_back(pdg);
        infoYZ.push_back(poshitY);
        infoYZ.push_back(poshitZ);
        infoYZ.push_back(pex);
        fRecoTrack_hitYZ[idx].push_back(infoYZ);

      }

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

    cout << "EVT #: " << ievt<<endl;
    cout << "# of RecoTrack: " << fRecoTrack_ID.size()<<endl;
    if (fRecoTrack_ID.size() <= 0) {
      cout << "No RecoTrack!" << endl;
      continue;
    }      
    for(unsigned int itrk=0;itrk<fRecoTrack_ID.size();itrk++){
      // if (fRecoTrack_ID[itrk] != e_trkid) continue;
      hits_map_XY->Add(fRecoTrack_MPPCHit_XY[itrk]); // pe along Z
      hits_map_XZ->Add(fRecoTrack_MPPCHit_XZ[itrk]); // pe along Y
      hits_map_YZ->Add(fRecoTrack_MPPCHit_YZ[itrk]); // pe along X
    }

    hMPPCHits_XY[ievt]->Add(hits_map_XY); // pe along Z
    hMPPCHits_XZ[ievt]->Add(hits_map_XZ); // pe along Y
    hMPPCHits_YZ[ievt]->Add(hits_map_YZ); // pe along X
    
  for(unsigned int itrk=0;itrk<fRecoTrack_ID.size();itrk++){
    if (fRecoTrack_ID[itrk] == e_trkid) {
      AddGraph(gMCElectronHits_XY[ievt],fTrueTrack_MCHit_XY[itrk]); // pe along Z
      AddGraph(gMCElectronHits_XZ[ievt],fTrueTrack_MCHit_XZ[itrk]); // pe along Y
      AddGraph(gMCElectronHits_YZ[ievt],fTrueTrack_MCHit_YZ[itrk]); // pe along X
    }
   else if (!useElectron && fRecoTrack_ID[itrk] == pos_trkid){
      AddGraph(gMCPositronHits_XY[ievt],fTrueTrack_MCHit_XY[itrk]); // pe along Z
      AddGraph(gMCPositronHits_XZ[ievt],fTrueTrack_MCHit_XZ[itrk]); // pe along Y
      AddGraph(gMCPositronHits_YZ[ievt],fTrueTrack_MCHit_YZ[itrk]); // pe along X
    }
  }
  
  double e_track_id = fRecoTrack_hitYZ[0][0][0];
  double e_particle_id = fRecoTrack_hitYZ[0][0][1];

  cout << "track ID of the first hit: " << e_track_id << endl;
  cout << "particle ID of the first hit: " << e_particle_id<< endl;
    

  double poshitI_prev;
  double poshitJ_prev;
  int binI;
  int binJ;

  double ith_pe;
  
  int ithbin[3] = {1,1,1};
  bool used_cube;
   
  //ETRACK
  
  //YZ

  used_cube = false;
  poshitI_prev = fRecoTrack_hitYZ[0][0][2];
  poshitJ_prev = fRecoTrack_hitYZ[0][0][3];
  binI = hMPPCHits_YZ[ievt]->GetXaxis()->FindBin(poshitI_prev);
  binJ = hMPPCHits_YZ[ievt]->GetYaxis()->FindBin(poshitJ_prev);
  ith_pe =  hMPPCHits_YZ[ievt]->GetBinContent(binI,binJ);

  for (int i=0;i<fRecoTrack_hitYZ[0].size();i++){

    double poshitI = fRecoTrack_hitYZ[0][i][2];
    double poshitJ = fRecoTrack_hitYZ[0][i][3];
          
    binI = hMPPCHits_YZ[ievt]->GetXaxis()->FindBin(poshitI);
    binJ = hMPPCHits_YZ[ievt]->GetYaxis()->FindBin(poshitJ);
    ith_pe = hMPPCHits_YZ[ievt]->GetBinContent(binI,binJ);

    if ((poshitI != poshitI_prev || poshitJ != poshitJ_prev) && ith_pe > 3.0){
      ithbin[0]++;
      poshitI_prev = poshitI;
      poshitJ_prev = poshitJ;
      used_cube = false;
    }

    
    if (!used_cube) {
      used_cube = true;
      hPE_X_etrack[ievt]->SetBinContent(ithbin[0],ith_pe);
    }
  }
 
  //XZ

  used_cube = false;
  poshitI_prev = fRecoTrack_hitXZ[0][0][2];
  poshitJ_prev = fRecoTrack_hitXZ[0][0][3];
  binI = hMPPCHits_XZ[ievt]->GetXaxis()->FindBin(poshitI_prev);
  binJ = hMPPCHits_XZ[ievt]->GetYaxis()->FindBin(poshitJ_prev);
  ith_pe =  hMPPCHits_XZ[ievt]->GetBinContent(binI,binJ);

  for (int i=0;i<fRecoTrack_hitXZ[0].size();i++){

    double poshitI = fRecoTrack_hitXZ[0][i][2];
    double poshitJ = fRecoTrack_hitXZ[0][i][3];

    binI = hMPPCHits_XZ[ievt]->GetXaxis()->FindBin(poshitI);
    binJ = hMPPCHits_XZ[ievt]->GetYaxis()->FindBin(poshitJ);
    ith_pe =  hMPPCHits_XZ[ievt]->GetBinContent(binI,binJ);

    if ((poshitI != poshitI_prev || poshitJ != poshitJ_prev) && ith_pe >3.0){
      ithbin[1]++;
      poshitI_prev = poshitI;
      poshitJ_prev = poshitJ;
      used_cube = false;
    }


    if (!used_cube) {
      used_cube = true;
      hPE_Y_etrack[ievt]->SetBinContent(ithbin[1],ith_pe);
    }
  }

  //XY

  used_cube = false;
  poshitI_prev = fRecoTrack_hitXY[0][0][2];
  poshitJ_prev = fRecoTrack_hitXY[0][0][3];
  binI = hMPPCHits_XY[ievt]->GetXaxis()->FindBin(poshitI_prev);
  binJ = hMPPCHits_XY[ievt]->GetYaxis()->FindBin(poshitJ_prev);
  ith_pe =  hMPPCHits_XY[ievt]->GetBinContent(binI,binJ);

  for (int i=0;i<fRecoTrack_hitXY[0].size();i++){

    double poshitI = fRecoTrack_hitXY[0][i][2];
    double poshitJ = fRecoTrack_hitXY[0][i][3];

    binI = hMPPCHits_XY[ievt]->GetXaxis()->FindBin(poshitI);
    binJ = hMPPCHits_XY[ievt]->GetYaxis()->FindBin(poshitJ);
    ith_pe =  hMPPCHits_XY[ievt]->GetBinContent(binI,binJ);

    if ((poshitI != poshitI_prev || poshitJ != poshitJ_prev) && ith_pe >3.0){
      ithbin[2]++;
      poshitI_prev = poshitI;
      poshitJ_prev = poshitJ;
      used_cube = false;
    }


    if (!used_cube) {
      used_cube = true;
      hPE_Z_etrack[ievt]->SetBinContent(ithbin[2],ith_pe);
    }
  }

  //

  
  
    int NFailedView = 0;   
    int NHitViewMin = 8;

    bool rejX = false;
    bool rejY = false;
    bool rejZ = false;

    int MinimumNBin = TMath::MinElement(3,ithbin);

    cout << "# of cubes w/ electron hits (X):" << ithbin[0]<<endl;
    cout << "# of cubes w/ electron hits (Y):" << ithbin[1]<<endl;
    cout << "# of cubes w/ electron hits (Z):" << ithbin[2]<<endl;

    if (ithbin[0] < NHitViewMin) {
      NFailedView++;
      rejX = true;
    }
    if (ithbin[1] < NHitViewMin) {
      NFailedView++;
      rejY = true;
    }
    if (ithbin[2] < NHitViewMin) {
      NFailedView++;
      rejZ = true;
    }
    if (NFailedView>=2) {
      cout << "Less than 2 views have <3 hits!" << endl;
      continue;
    }
  
      /////////////////////////

  
    int NSepView = 0;
   
    bool TrackSepCheckXY = false;
    bool TrackSepCheckXZ = false;
    bool TrackSepCheckYZ = false;
       
    bool TwoTrackLike = false;

    cout <<"EVT:"<< ievt<<endl;

    if (!useElectron){
      cout << "Track Separation Check (XY):" << endl;
      TrackSepCheckXY = CheckTrackSeparation(gMCElectronHits_XY[ievt], gMCPositronHits_XY[ievt], hMPPCHits_XY[ievt]);
      cout << "Track Separation Check (XZ):" << endl;
      TrackSepCheckXZ = CheckTrackSeparation(gMCElectronHits_XZ[ievt], gMCPositronHits_XZ[ievt], hMPPCHits_XZ[ievt]);
      cout << "Track Separation Check (YZ):" << endl;
      TrackSepCheckYZ = CheckTrackSeparation(gMCElectronHits_YZ[ievt], gMCPositronHits_YZ[ievt], hMPPCHits_YZ[ievt]);
    }
    

    if (TrackSepCheckXY) NSepView++;
    if (TrackSepCheckXZ) NSepView++;
    if (TrackSepCheckYZ) NSepView++;
    
    cout << "DONE" <<endl;
    

    if (!useElectron && NSepView >= 2){
      cout << "Track Separation Cut!" << endl;
      sepcut_count++;
      TwoTrackLike = true;
      //     continue;
    }

    ///
    
    hElecMom_CosTheta_All->Fill(track_momentum[0],track_costheta[0]);
    cout << track_momentum[0] << ", " << track_costheta[0] << endl;

    //SCRIPT FOR RATIO CALC

    cout << "Ratio Study Result (YZ):" <<endl; 
    name = TString::Format("fstep_X_%d",ievt);
    fstep_X[ievt] = RatioCalc(name, ithbin[0], hPE_X_etrack[ievt], fratio_hilo_X[ievt]);
    
    cout << "Ratio Study Result (XZ):" <<endl; 
    name = TString::Format("fstep_Y_%d",ievt);
    fstep_Y[ievt] = RatioCalc(name, ithbin[1], hPE_Y_etrack[ievt], fratio_hilo_Y[ievt]);

    cout << "Ratio Study Result (XY):" <<endl; 
    name = TString::Format("fstep_Z_%d",ievt);
    fstep_Z[ievt] = RatioCalc(name, ithbin[2], hPE_Z_etrack[ievt], fratio_hilo_Z[ievt]);

    hETrack_HiLoRatio_X->Fill(fratio_hilo_X[ievt]);
    hETrack_HiLoRatio_Y->Fill(fratio_hilo_Y[ievt]);
    hETrack_HiLoRatio_Z->Fill(fratio_hilo_Z[ievt]);

    hView_HiLo->Fill(3.,fratio_hilo_X[ievt]);
    hView_HiLo->Fill(2.,fratio_hilo_Y[ievt]);
    hView_HiLo->Fill(1.,fratio_hilo_Z[ievt]);

    hView_NumHit->Fill(3.,ithbin[0]);
    hView_NumHit->Fill(2.,ithbin[1]);
    hView_NumHit->Fill(1.,ithbin[2]);


    if (!rejX && !rejY && !rejZ) {
      if (fratio_hilo_X[ievt]>3.5) rejX = true;
      if (fratio_hilo_Y[ievt]>3.5) rejY = true; 
      if (fratio_hilo_Z[ievt]>3.5) rejZ = true; 
    }

    if (!rejX && !rejY && !rejZ) {
      if (MinimumNBin == ithbin[0]) rejX = true;
      else if (MinimumNBin == ithbin[1]) rejY = true;
      else if (MinimumNBin == ithbin[2]) rejZ = true;
    }

    cout << "rejected views:" << rejX << ", " <<rejY  << ", " << rejZ << endl;
    cout << rejX + rejY + rejZ << endl;

    if ((rejX+rejY+rejZ) >1) {
      cout << "More than one views rejected!" <<endl;
      continue;
    }

    if (rejX) fratio_hilo_X[ievt] = 1.;
    if (rejY) fratio_hilo_Y[ievt] = 1.;
    if (rejZ) fratio_hilo_Z[ievt] = 1.;




    cout << "LY ratio (X) = " << fratio_hilo_X[ievt] << endl;
    cout << "LY ratio (Y) = " << fratio_hilo_Y[ievt] << endl;
    cout << "LY ratio (Z) = " << fratio_hilo_Z[ievt] << endl;

    fratio_hilo_Prod[ievt] = fratio_hilo_X[ievt]*fratio_hilo_Y[ievt]*fratio_hilo_Z[ievt];

    cout << "LY ratio product = " << fratio_hilo_Prod[ievt] << endl;

    //    if (TwoTrackLike) fratio_hilo_Prod[ievt] = 20.;

    hETrack_HiLoRatio_Prod->Fill(fratio_hilo_Prod[ievt]);
     

    if (fratio_hilo_Prod[ievt]<RatioCriteria){//accepted
      hElecMom_CosTheta_Passed->Fill(track_momentum[0],track_costheta[0]);
      elike_count++;
  }
    
    delete nd280UpEvent;    
    nd280UpEvent = 0;
  } // end loop over events

  // Write output file
  TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);
  TFile *out = new TFile(outfilename.Data(),"RECREATE");
  // 
  
  cout << "out of " << passed_evt.size() << " events, " << sepcut_count << " events were two-track-like." <<endl;
  cout << elike_count << " events passed e/gamma selection w/ p.e ratio product criteria set at " <<RatioCriteria <<"."<< endl; 



    if (writePlots) {
  
      for(size_t ievtdispl=0;ievtdispl<passed_evt.size();ievtdispl++){ // get last entry
		
	if (ievtdispl < nPlotsWrite) {
	  hMPPCHits_XY[passed_evt[ievtdispl]]->Write();
	  hMPPCHits_XZ[passed_evt[ievtdispl]]->Write();
	  hMPPCHits_YZ[passed_evt[ievtdispl]]->Write();
	  
	  gMCElectronHits_XY[passed_evt[ievtdispl]]->Write();
	  gMCElectronHits_XZ[passed_evt[ievtdispl]]->Write();
	  gMCElectronHits_YZ[passed_evt[ievtdispl]]->Write();
	  
	  gMCPositronHits_XY[passed_evt[ievtdispl]]->Write();
	  gMCPositronHits_XZ[passed_evt[ievtdispl]]->Write();
	  gMCPositronHits_YZ[passed_evt[ievtdispl]]->Write();
      
	  hPE_X_etrack[passed_evt[ievtdispl]]->Write();
	  fstep_X[passed_evt[ievtdispl]]->Write();
	  
	  hPE_Y_etrack[passed_evt[ievtdispl]]->Write();
	  fstep_Y[passed_evt[ievtdispl]]->Write();
	  
	  hPE_Z_etrack[passed_evt[ievtdispl]]->Write();
	  fstep_Z[passed_evt[ievtdispl]]->Write();
	}
      }
    }

  hETrack_HiLoRatio_X->Write();
  hETrack_HiLoRatio_Y->Write();
  hETrack_HiLoRatio_Z->Write();
  hETrack_HiLoRatio_Prod->Write();

  hView_HiLo->Write();
  hView_NumHit->Write();
  hElecMom_CosTheta_All->Write();
  hElecMom_CosTheta_Passed->Write();
  
  out->Close();
}

//___________________________________________________________________________ 

TF1 *RatioCalc(TString name, int e_step_tot, TH1F *hPE_etrack, double &ratio){

  TF1 *fstep_hi = new TF1("fstep_hi", "pol0(0)", 0, e_step_tot);
  TF1 *fstep_lo = new TF1("fstep_lo", "pol0(0)", 0, e_step_tot);    

    int nbinignore =1;
    int nminhits = 3;
    bool minimumhits = false;

    double frange_hi[2] = {0.,1.};
    double fval_hi = 0.;

    double frange_lo[2] = {0.,1.};
    double fval_lo = 0.;
    
    double fratio_hilo = 0.;
    
    double ffitval_hi = 0.;
    double ffitval_lo = 0.;

    for(int i=nbinignore;i<(e_step_tot-nbinignore-1);i++){
      hPE_etrack->Fit("fstep_hi","Q0","0",(nbinignore),i+1);
      hPE_etrack->Fit("fstep_lo","Q0","0",i+1,(e_step_tot-nbinignore));
      fval_hi = fstep_hi->GetParameter(0);
      fval_lo = fstep_lo->GetParameter(0);
	
	if (((e_step_tot-(2.*nbinignore)))>=(nminhits) && 
	    (i+1)>=(nbinignore+nminhits) && 
	    (e_step_tot-nbinignore-i-1)>=nminhits &&
	    fratio_hilo < (fval_hi/fval_lo)){
	 
	  fratio_hilo = (fval_hi/fval_lo);
	  ffitval_hi = fval_hi;
	  ffitval_lo = fval_lo;
	  
	  frange_hi[0] = nbinignore;
	  frange_hi[1] = i+1;
	  
	  frange_lo[0] = i+1;
	  frange_lo[1] = e_step_tot-nbinignore;
	}
      }

    cout << "Ratio Result: " << ffitval_hi<<"/"<<ffitval_lo<<" = " << fratio_hilo<<endl;
    cout << "High Range: " << frange_hi[0] << " to " << frange_hi[1] <<endl;
    cout << "Low Range: " << frange_lo[0] << " to " << frange_lo[1] <<endl;

    TF1 *fseg = new TF1(name, "(x<[0])*[1]+(x>=[0])*[2]", frange_hi[0], frange_lo[1]);
    fseg->SetParameter(0,frange_hi[1]);
    fseg->SetParameter(1,ffitval_hi);
    fseg->SetParameter(2,ffitval_lo);

    ratio = fratio_hilo;
    if (ratio <=0) {
      ratio = 1. ;
	}
    return fseg;
}

void Fill_RejHiLo(TH1F *hETrack_HiLoRatio_Rej, TH1F *hRejected_View, TH1F *hETrack_RejNumHit, TH2F *hRejView_RejNumHit, TH2F *hRejView_HiLoRej, double e_step_tot_N, double &fratio_hilo_N, double viewN){
 
  // if (fratio_hilo_N>3.2){
    // cout << "Z has the smallest number of steps: " << e_step_tot_Z[ievt]<<endl;
    //     cout << "Rejected Z Ratio Value = " << fratio_hilo_Z[ievt]<<endl;
    hETrack_HiLoRatio_Rej->Fill(fratio_hilo_N);
    hRejected_View->Fill(viewN);
    hETrack_RejNumHit->Fill(e_step_tot_N);
    hRejView_RejNumHit->Fill(viewN,e_step_tot_N);
    //      hRejView_SinglePE->Fill(1.,single_pehit);
    hRejView_HiLoRej->Fill(viewN,fratio_hilo_N);
    //      hHiloRej_SinglePE->Fill(fratio_hilo_Z[ievt],single_pehit);
    fratio_hilo_N = 1.;
    //  }

    //  return;
      
}


double CalcPtDistance(double *pt1, double *pt2){
  double dist = 0.;
  double dX = pt1[0] - pt2[0];
  double dY = pt1[1] - pt2[1];
  dist = sqrt(dX*dX + dY*dY);
  return dist;

}

bool CheckTrackSeparation(TGraph *track1, TGraph *track2, TH2F *hMPPCHits){
  
  double trk1pi[2];
  double trk1pf[2];
  double trk2pi[2];
  double trk2pf[2];
  

  double vtx1pi[2];
  double vtx1pf[2];
  double vtx2pi[2];
  double vtx2pf[2];

  double difvtx1[2];
  double difvtx2[2];

  int NShortTrack = 0;


  if (track1->GetN() < 3 && track2->GetN() < 3){
    cout <<"short tracks" << endl;
    return false;
  }

  if ((track2->GetN() < 3 && track1->GetN() > 3) || (track2->GetN() > 3 && track1->GetN() < 3) ){
    cout << "electron/positron track < 3 hits; single-track-like." <<endl;
    return false;
  }

  if (track2->GetN() > track1->GetN()){
    cout << "positron track longer than electron track; two-track-like." << endl;
    //   return true;
  }


  NShortTrack = std::min(track1->GetN(),track2->GetN());
  
  
  track1->GetPoint(0,trk1pi[0],trk1pi[1]);
  track1->GetPoint(NShortTrack-1,trk1pf[0],trk1pf[1]);

  track2->GetPoint(0,trk2pi[0],trk2pi[1]);
  track2->GetPoint(NShortTrack-1,trk2pf[0],trk2pf[1]);
  /*
  cout << "Track 1, initial: " << trk1pi[0] << ", " << trk1pi[1]<< ", N = " <<track1->GetN()-1 << endl;
  cout << "Track 1, final: " << trk1pf[0] << ", " << trk1pf[1]<< ", N = " <<track1->GetN()-1 << endl;

  cout << "Track 2, initial: " << trk2pi[0] << ", " << trk2pi[1]<< ", N = " <<track2->GetN()-1 << endl;
  cout << "Track 2, final: " << trk2pf[0] << ", " << trk2pf[1]<< ", N = " <<track2->GetN()-1 << endl;
  */

  vtx1pi[0] = hMPPCHits->GetXaxis()->GetBinCenter(hMPPCHits->GetXaxis()->FindBin(trk1pi[0]));
  vtx1pi[1] = hMPPCHits->GetYaxis()->GetBinCenter(hMPPCHits->GetYaxis()->FindBin(trk1pi[1]));

  vtx1pf[0] = hMPPCHits->GetXaxis()->GetBinCenter(hMPPCHits->GetXaxis()->FindBin(trk1pf[0]));
  vtx1pf[1] = hMPPCHits->GetYaxis()->GetBinCenter(hMPPCHits->GetYaxis()->FindBin(trk1pf[1]));

  vtx2pi[0] = hMPPCHits->GetXaxis()->GetBinCenter(hMPPCHits->GetXaxis()->FindBin(trk2pi[0]));
  vtx2pi[1] = hMPPCHits->GetYaxis()->GetBinCenter(hMPPCHits->GetYaxis()->FindBin(trk2pi[1]));

  vtx2pf[0] = hMPPCHits->GetXaxis()->GetBinCenter(hMPPCHits->GetXaxis()->FindBin(trk2pf[0]));
  vtx2pf[1] = hMPPCHits->GetYaxis()->GetBinCenter(hMPPCHits->GetYaxis()->FindBin(trk2pf[1]));
  /*
  cout << "Vertex 1, initial: " << vtx1pi[0] << ", " << vtx1pi[1]<< endl;
  cout << "Vertex 1, final: " << vtx1pf[0] << ", " << vtx1pf[1]<< endl;

  cout << "Vertex 2, initial: " << vtx2pi[0] << ", " << vtx2pi[1]<< endl;
  cout << "Vertex 2, final: " << vtx2pf[0] << ", " << vtx2pf[1]<< endl;
  */

  for (int i=0;i<2;i++){
    difvtx1[i] = vtx1pf[i] - vtx1pi[i];
    difvtx2[i] = vtx2pf[i] - vtx2pi[i];
  }

  //  cout << "Vertex 1, Diff: " << difvtx1[0] << ", " << difvtx1[1]<< endl;
  // cout << "Vertex 2, Diff: " << difvtx2[0] << ", " << difvtx2[1]<< endl;

  //  double disttrk1 = CalcPtDistance(trk1pi,trk1pf);
  double distvtx1 = CalcPtDistance(vtx1pi,vtx1pf);

  //  double disttrk2 = CalcPtDistance(trk2pi,trk2pf);
  double distvtx2 = CalcPtDistance(vtx2pi,vtx2pf);

  //  cout << "Track 1, Dist: " << disttrk1 << endl;
  //  cout << "Vertex 1, Dist: " << distvtx1 << endl;
  // cout << "Track 2, Dist: " << disttrk2 << endl;
  //  cout << "Vertex 2, Dist: " << distvtx2 << endl;

  double scalar_prod = difvtx1[0]*difvtx2[0] + difvtx1[1]*difvtx2[1];
  double costh = scalar_prod /(distvtx1*distvtx2) ;

  //  cout << "Scalar Prod: " << scalar_prod << endl;
  // cout << "Costheta " << costh << endl;


  double distance = std::min(distvtx1,distvtx2)*TMath::Sin(TMath::ACos(costh));
  //  double truedist = CalcPtDistance(vtx1pf,vtx2pf);

  cout << "Track Separation = " << distance << " mm" << endl;
  //  cout << "Track Separation (true) = " << truedist << " mm" << endl;

  if (distance > 20.) return true;
  else return false;

}



inline int FindIndex(vector<int> vec,int val){
  vector<int>::iterator it;
  it=find(vec.begin(),vec.end(),val);
  int pos = distance(vec.begin(), it);
  if(it!=vec.end()){
   cout<<"FOUND  "<< *it<<"  at position: "<<pos<<endl;
  }
  else{
    cout<<"NOT FOUND"<<endl;
  }
  return pos;
}


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

bool IsTargetIn(double x,double y,double z,
                // Target 1
		double vtx_min_x_1,
                double vtx_max_x_1,
                double vtx_min_y_1,
		double vtx_max_y_1,
                double vtx_min_z_1,
                double vtx_max_z_1
		){
  if((x > vtx_min_x_1 && x < vtx_max_x_1) &&
     (y > vtx_min_y_1 && y < vtx_max_y_1) &&
     (z > vtx_min_z_1 && z < vtx_max_z_1)
     ){
    return true;
  }
  else{
    return false;
  }
}
