
#include <list>       
#include <iterator>
#include <algorithm>
#include <vector>
#include <iostream>

#include <TCanvas.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>

using namespace std;

int FindTrackElem(std::vector<int> *vectrk,int trkid);

void SkimNtuple()
{  
  const char *tag = "prova";
  const char *filename = "../bin/EffStudy_AllEvents.root";
  //const char *filename = "../bin/EffStudy.root",
  const bool doprint = false;

  // Vertex cut (inside the Target)
  const bool doCut_Vtx = true; 
  const double vtx_min_x = -1150;  //mm
  const double vtx_max_x = +1150;  //mm
  const double vtx_min_y = -151.5; //mm
  const double vtx_max_y = +151.1; //mm
  const double vtx_min_z = -1200;  //mm
  const double vtx_max_z = +1200;  //mm

  // DeltaLyz cut (TPCs and Target)
  const bool doCut_dL = true; // full length in target and dlyz in tpcs
  const double dlyz_target_min = 50; //mm 
  const double len_tpc_min = 200; //mm
  
  // Charge cut (TPCs and Target)
  const bool doCut_poscharge = true;
  const double cut_charge = 1; // only charge particles
  
  const bool doSelPDG = false;
  const int SetPDG = +2212; //+2212; //+13;

  // For debugging:
  const bool doSingleEvent = false;
  const int SingleEventID = 9; // 87

  const int doPrintTrack = false;

  ////////////////////////


  // True tracks
  TH1D *hNTracks = new TH1D("hNTracks","hNTracks",100,0,100);
  TH1D *hTrackE = new TH1D("hTrackE","hTrackE",100,0,2000);
  TH1D *hTrackPDG = new TH1D("hTrackPDG","hTrackPDG",500,-500,500);
  TH1D *hTrackMom  = new TH1D("hTrackMom","hTrackMom",100,0,1000);
  
  // From Sensitive detector
  TH1D *hEabsTot = new TH1D("hEabsTot","hEabsTot",100,0,150);
  TH1D *hEabsTarget = new TH1D("hEabsTarget","hEabsTarget",100,0,150);
  TH1D *hEabsTPCup = new TH1D("hEabsTPCup","hEabsTPCup",100,0,150);
  TH1D *hEabsTPCdown = new TH1D("hEabsTPCdown","hEabsTPCdown",100,0,150);
  
  TH1D *hReco_Mom = new TH1D("hReco_Mom","hReco_Mom",100,0,10000);
  TH1D *hReco_CosTheta = new TH1D("hReco_CosTheta","hReco_CosTheta",50,-1,+1);
  TH1D *hReco_PDG = new TH1D("hReco_PDG","hReco_PDG",800,-400,400);
  TH2D *hReco_CosThetaVsMom_Target = new TH2D("hReco_CosThetaVsMom_Target","hReco_CosThetaVsMom_Target",10,-1,1,20,0,10000);
  TH2D *hReco_CosThetaVsMom_Fin = new TH2D("hReco_CosThetaVsMom_Fin","hReco_CosThetaVsMom_Fin",10,-1,1,20,0,10000);
  TH2D *hReco_CosThetaVsMom_Eff; // = new TH2D("hReco_CosThetaVsMom_Eff","hReco_CosThetaVsMom_Eff",10,-1,1,20,0,10000);
  
  TH1D *hTPCUp_MomX = new TH1D("hTPCUp_MomX","hTPCUp_MomX",100,0,10000);
  TH1D *hTPCUp_MomY = new TH1D("hTPCUp_MomY","hTPCUp_MomY",100,0,10000);
  TH1D *hTPCUp_MomZ = new TH1D("hTPCUp_MomZ","hTPCUp_MomZ",100,0,10000);
  TH1D *hTPCUp_Mom = new TH1D("hTPCUp_Mom","hTPCUp_Mom",100,0,10000);
  TH1D *hTPCUp_Length = new TH1D("hTPCUp_Length","hTPCUp_Length",100,0,3000); // mm
  TH1D *hTPCUp_DeltaLyz = new TH1D("hTPCUp_DeltaLyz","hTPCUp_DeltaLyz",100,0,3000); // mm
  TH1D *hTPCUp_Edep = new TH1D("hTPCUp_Edep","hTPCUp_Edep",100,0,300);
  TH1D *hTPCUp_Charge = new TH1D("hTPCUp_Charge","hTPCUp_Charge",6,-3,3);
  TH1D *hTPCUp_PDG = new TH1D("hTPCUp_PDG","hTPCUp_PDG",800,-400,400);
  TH1D *hTPCUp_PosIniX = new TH1D("hTPCUp_PosIniX","hTPCUp_PosIniX",600,-3000,3000);
  TH1D *hTPCUp_PosIniY = new TH1D("hTPCUp_PosIniY","hTPCUp_PosIniY",600,-3000,3000);
  TH1D *hTPCUp_PosIniZ = new TH1D("hTPCUp_PosIniZ","hTPCUp_PosIniZ",600,-3000,3000);
  TH1D *hTPCUp_PosFinX = new TH1D("hTPCUp_PosFinX","hTPCUp_PosFinX",600,-3000,3000);
  TH1D *hTPCUp_PosFinY = new TH1D("hTPCUp_PosFinY","hTPCUp_PosFinY",600,-3000,3000);
  TH1D *hTPCUp_PosFinZ = new TH1D("hTPCUp_PosFinZ","hTPCUp_PosFinZ",600,-3000,3000);
  TH1D *hTPCUp_CosTheta = new TH1D("hTPCUp_CosTheta","hTPCUp_CosTheta",50,-1,1);

  TH1D *hTPCDown_MomX = new TH1D("hTPCDown_MomX","hTPCDown_MomX",100,0,10000);
  TH1D *hTPCDown_MomY = new TH1D("hTPCDown_MomY","hTPCDown_MomY",100,0,10000);
  TH1D *hTPCDown_MomZ = new TH1D("hTPCDown_MomZ","hTPCDown_MomZ",100,0,10000);
  TH1D *hTPCDown_Mom = new TH1D("hTPCDown_Mom","hTPCDown_Mom",100,0,10000);
  TH1D *hTPCDown_Length = new TH1D("hTPCDown_Length","hTPCDown_Length",100,0,3000); // mm
  TH1D *hTPCDown_DeltaLyz = new TH1D("hTPCDown_DeltaLyz","hTPCDown_DeltaLyz",100,0,3000); // mm
  TH1D *hTPCDown_Edep = new TH1D("hTPCDown_Edep","hTPCDown_Edep",100,0,300);
  TH1D *hTPCDown_Charge = new TH1D("hTPCDown_Charge","hTPCDown_Charge",6,-3,3);
  TH1D *hTPCDown_PDG = new TH1D("hTPCDown_PDG","hTPCDown_PDG",800,-400,400);
  TH1D *hTPCDown_PosIniX = new TH1D("hTPCDown_PosIniX","hTPCDown_PosIniX",600,-3000,3000);
  TH1D *hTPCDown_PosIniY = new TH1D("hTPCDown_PosIniY","hTPCDown_PosIniY",600,-3000,3000);
  TH1D *hTPCDown_PosIniZ = new TH1D("hTPCDown_PosIniZ","hTPCDown_PosIniZ",600,-3000,3000);
  TH1D *hTPCDown_PosFinX = new TH1D("hTPCDown_PosFinX","hTPCDown_PosFinX",600,-3000,3000);
  TH1D *hTPCDown_PosFinY = new TH1D("hTPCDown_PosFinY","hTPCDown_PosFinY",600,-3000,3000);
  TH1D *hTPCDown_PosFinZ = new TH1D("hTPCDown_PosFinZ","hTPCDown_PosFinZ",600,-3000,3000);
  TH1D *hTPCDown_CosTheta = new TH1D("hTPCDown_CosTheta","hTPCDown_CosTheta",50,-1,1);

  TH1D *hTarget_MomX = new TH1D("hTarget_MomX","hTarget_MomX",100,0,10000);
  TH1D *hTarget_MomY = new TH1D("hTarget_MomY","hTarget_MomY",100,0,10000);
  TH1D *hTarget_MomZ = new TH1D("hTarget_MomZ","hTarget_MomZ",100,0,10000);
  TH1D *hTarget_Mom = new TH1D("hTarget_Mom","hTarget_Mom",100,0,10000);
  TH1D *hTarget_Length = new TH1D("hTarget_Length","hTarget_Length",100,0,3000); // mm
  TH1D *hTarget_DeltaLyz = new TH1D("hTarget_DeltaLyz","hTarget_DeltaLyz",100,0,3000); // mm
  TH1D *hTarget_Edep = new TH1D("hTarget_Edep","hTarget_Edep",100,0,300);
  TH1D *hTarget_Charge = new TH1D("hTarget_Charge","hTarget_Charge",6,-3,3);
  TH1D *hTarget_PDG = new TH1D("hTarget_PDG","hTarget_PDG",800,-400,400);
  TH1D *hTarget_PosIniX = new TH1D("hTarget_PosIniX","hTarget_PosIniX",600,-3000,3000);
  TH1D *hTarget_PosIniY = new TH1D("hTarget_PosIniY","hTarget_PosIniY",600,-300,300);
  TH1D *hTarget_PosIniZ = new TH1D("hTarget_PosIniZ","hTarget_PosIniZ",600,-3000,3000);
  TH1D *hTarget_PosFinX = new TH1D("hTarget_PosFinX","hTarget_PosFinX",600,-3000,3000);
  TH1D *hTarget_PosFinY = new TH1D("hTarget_PosFinY","hTarget_PosFinY",600,-300,300);
  TH1D *hTarget_PosFinZ = new TH1D("hTarget_PosFinZ","hTarget_PosFinZ",600,-3000,3000);
  TH1D *hTarget_CosTheta = new TH1D("hTarget_CosTheta","hTarget_CosTheta",50,-1,1);

  // Vertex
  TH2D *hVtx_XY = new TH2D("hVtx_XY","hVtx_XY",6000,-3000,3000,600,-300,300); // mm
  TH2D *hVtx_XZ = new TH2D("hVtx_XZ","hVtx_XZ",6000,-6000,6000,600,-300,300); // mm
  TH2D *hVtx_YZ = new TH2D("hVtx_YZ","hVtx_YZ",6000,-6000,6000,600,-300,300); // mm
  TH1D *hVtx_X = new TH1D("hVtx_X","hVtx_X",6000,-3000,3000); // mm
  TH1D *hVtx_Y = new TH1D("hVtx_Y","hVtx_Y",6000,-300,300); // mm
  TH1D *hVtx_Z = new TH1D("hVtx_Z","hVtx_Z",6000,-3000,3000); // mm

  // First Step position Vs Vertex position
  TH1D *hVtxMinusPosIni_Target_X = new TH1D("hVtxMinusPosIni_Target_X","hVtxMinusPosIni_Target_X",6000,-3000,3000);
  TH1D *hVtxMinusPosIni_Target_Y = new TH1D("hVtxMinusPosIni_Target_Y","hVtxMinusPosIni_Target_Y",6000,-3000,3000);
  TH1D *hVtxMinusPosIni_Target_Z = new TH1D("hVtxMinusPosIni_Target_Z","hVtxMinusPosIni_Target_Z",6000,-3000,3000);

 
  cout << endl;
  cout << "Reading file:" << endl;
  cout << filename << endl;
  cout << endl;

  
  TFile *filein = new TFile(filename,"READ");
  TTree *treein = (TTree*)filein->Get("EffStudy");


  // Set ntuple branch addresses

  double EabsTarget=0.; // Energy deposited
  double EabsTPCup=0.;
  double EabsTPCdown=0.;
  int NTracks=0;

  // Global informations
  vector<int>   * VecTrackID = 0; 
  vector<int>   * VecParentID = 0; 
  vector<int>   * VecTrackPDG = 0; 
  vector<double>* VecTrackE   = 0; 
  vector<double>* VecTrackMomX = 0; 
  vector<double>* VecTrackMomY = 0; 
  vector<double>* VecTrackMomZ = 0; 
  vector<double>* VecTrackMom  = 0; 

  // TPC up informations
  int     TPCUp_NTracks;          // # of tracks in TPC Up
  std::vector<int>   * VecTPCUp_TrackID = 0; // Vector of trackID in TPCup
  std::vector<int>   * VecTPCUp_ParentID = 0; // Vector of parentID in TPCup
  std::vector<double>* VecTPCUp_TrackMomX = 0; // Vector of initial mom in TPC Up (MeV)
  std::vector<double>* VecTPCUp_TrackMomY = 0; // Vector of initial mom in TPC Up (MeV)
  std::vector<double>* VecTPCUp_TrackMomZ = 0; // Vector of initial mom in TPC Up (MeV)
  std::vector<double>* VecTPCUp_TrackLength = 0; // Vector of corrected (mult. scatt.) length in TPC Up (mm)
  std::vector<double>* VecTPCUp_TrackDeltaLyz = 0; // Vector of geometrical length yz in TPC Up (mm)
  std::vector<double>* VecTPCUp_TrackEdep = 0; // Vector of edep in TPC Up (MeV)
  std::vector<double>* VecTPCUp_TrackCharge = 0; // Vector of charge in TPC Up (e-)
  std::vector<int>   * VecTPCUp_TrackPDG = 0; // Vector of pdg in TPC Up 
  std::vector<double> *VecTPCUp_TrackStepFirstX = 0; // Vector of first step position in TPC Up
  std::vector<double> *VecTPCUp_TrackStepFirstY = 0; // Vector of first step position in TPC Up
  std::vector<double> *VecTPCUp_TrackStepFirstZ = 0; // Vector of first step position in TPC Up
  std::vector<double> *VecTPCUp_TrackStepLastX = 0; // Vector of last step position in TPC Up
  std::vector<double> *VecTPCUp_TrackStepLastY = 0; // Vector of last step position in TPC Up
  std::vector<double> *VecTPCUp_TrackStepLastZ = 0; // Vector of last step position in TPC Up
  std::vector<double> *VecTPCUp_TrackCosTheta = 0; // Vector of cos(theta) wrt nu direction (z axis) in TPC Up

  // TPC down informations
  int     TPCDown_NTracks = 0;          // # of tracks in TPC Down
  std::vector<int>   * VecTPCDown_TrackID = 0; // Vector of trackID in TPCDown
  std::vector<int>   * VecTPCDown_ParentID = 0; // Vector of parentID in TPCDown
  std::vector<double>* VecTPCDown_TrackMomX = 0; // Vector of initial mom in TPC Down (MeV)
  std::vector<double>* VecTPCDown_TrackMomY = 0; // Vector of initial mom in TPC Down (MeV)
  std::vector<double>* VecTPCDown_TrackMomZ = 0; // Vector of initial mom in TPC Down (MeV)
  std::vector<double>* VecTPCDown_TrackLength = 0; // Vector of corrected (mult.scatt.) length in TPC Down (mm)
  std::vector<double>* VecTPCDown_TrackDeltaLyz = 0; // Vector of geometrical length yz in TPC Down (mm)
  std::vector<double>* VecTPCDown_TrackEdep = 0; // Vector of edep in TPC Down (MeV)
  std::vector<double>* VecTPCDown_TrackCharge = 0; // Vector of charge in TPC Down (e-)
  std::vector<int>   * VecTPCDown_TrackPDG = 0; // Vector of pdg in TPC Down 
  std::vector<double> *VecTPCDown_TrackStepFirstX = 0; // Vector of first step position in TPC Down
  std::vector<double> *VecTPCDown_TrackStepFirstY = 0; // Vector of first step position in TPC Down
  std::vector<double> *VecTPCDown_TrackStepFirstZ = 0; // Vector of first step position in TPC Down
  std::vector<double> *VecTPCDown_TrackStepLastX = 0; // Vector of last step position in TPC Down
  std::vector<double> *VecTPCDown_TrackStepLastY = 0; // Vector of last step position in TPC Down
  std::vector<double> *VecTPCDown_TrackStepLastZ = 0; // Vector of last step position in TPC Down
  std::vector<double> *VecTPCDown_TrackCosTheta = 0; // Vector of cos(theta) wrt nu direction (z axis) in TPC Down

  // Target informations
  int     Target_NTracks;          // # of tracks in Target
  std::vector<int>   * VecTarget_TrackID = 0; // Vector of trackID in Target
  std::vector<int>   * VecTarget_ParentID = 0; // Vector of parentID in Target
  std::vector<double>* VecTarget_TrackMomX = 0; // Vector of initial mom in Target (MeV)
  std::vector<double>* VecTarget_TrackMomY = 0; // Vector of initial mom in Target (MeV)
  std::vector<double>* VecTarget_TrackMomZ = 0; // Vector of initial mom in Target (MeV)
  std::vector<double>* VecTarget_TrackLength = 0; // Vector of corrected (mult. scatt.) length in Target (mm)
  std::vector<double>* VecTarget_TrackDeltaLyz = 0; // Vector of geometrical length yz in Target (mm)
  std::vector<double>* VecTarget_TrackEdep = 0; // Vector of edep in Target (MeV)
  std::vector<double>* VecTarget_TrackCharge = 0; // Vector of charge in Target (e-)
  std::vector<int>   * VecTarget_TrackPDG = 0; // Vector of pdg in Target
  std::vector<double> *VecTarget_TrackStepFirstX = 0; // Vector of first step position in Target
  std::vector<double> *VecTarget_TrackStepFirstY = 0; // Vector of first step position in Target
  std::vector<double> *VecTarget_TrackStepFirstZ = 0; // Vector of first step position in Target
  std::vector<double> *VecTarget_TrackStepLastX = 0; // Vector of last step position in Target
  std::vector<double> *VecTarget_TrackStepLastY = 0; // Vector of last step position in Target
  std::vector<double> *VecTarget_TrackStepLastZ = 0; // Vector of last step position in Target
  std::vector<double> *VecTarget_TrackCosTheta = 0; // Vector of cos(theta) wrt nu direction (z axis) in Target

  // Vertex informations                                          
  int     NVtx;
  std::vector<double> *VecVtx_X = 0; // Vector of vertex X position
  std::vector<double> *VecVtx_Y = 0; // Vector of vertex Y position
  std::vector<double> *VecVtx_Z = 0; // Vector of vertex Z position
  std::vector<int>    *VecVtx_NuPDG = 0;     // Vector of vertex Nu PDG
  std::vector<int>    *VecVtx_ReacMode = 0;  // Vector of vertex reaction mode
  std::vector<double> *VecVtx_EvtProb = 0;   // Vector of vertex event prob
  std::vector<double> *VecVtx_EvtWeight = 0; // Vector of vertex event weight

  //
  // From all the volume
  //

  treein->SetBranchAddress("NTracks",    &NTracks);  
  treein->SetBranchAddress("VecTrackID",  &VecTrackID);
  treein->SetBranchAddress("VecParentID",  &VecParentID);
  treein->SetBranchAddress("VecTrackPDG", &VecTrackPDG);
  treein->SetBranchAddress("VecTrackE",   &VecTrackE);
  treein->SetBranchAddress("VecTrackMomX",   &VecTrackMomX);
  treein->SetBranchAddress("VecTrackMomY",   &VecTrackMomY);
  treein->SetBranchAddress("VecTrackMomZ",   &VecTrackMomZ);
  treein->SetBranchAddress("VecTrackMomMag", &VecTrackMom);

  //
  // From sensitive detector
  //
  
  treein->SetBranchAddress("EabsTarget", &EabsTarget);
  treein->SetBranchAddress("EabsTPCup",  &EabsTPCup);
  treein->SetBranchAddress("EabsTPCdown",&EabsTPCdown);

  treein->SetBranchAddress("TPCUp_NTracks",      &TPCUp_NTracks);
  treein->SetBranchAddress("VecTPCUp_TrackID",   &VecTPCUp_TrackID);
  treein->SetBranchAddress("VecTPCUp_ParentID",   &VecTPCUp_ParentID);
  treein->SetBranchAddress("VecTPCUp_TrackMomX", &VecTPCUp_TrackMomX);
  treein->SetBranchAddress("VecTPCUp_TrackMomY", &VecTPCUp_TrackMomY);
  treein->SetBranchAddress("VecTPCUp_TrackMomZ", &VecTPCUp_TrackMomZ);
  treein->SetBranchAddress("VecTPCUp_TrackLength", &VecTPCUp_TrackLength);
  treein->SetBranchAddress("VecTPCUp_TrackDeltaLyz", &VecTPCUp_TrackDeltaLyz);
  treein->SetBranchAddress("VecTPCUp_TrackEdep", &VecTPCUp_TrackEdep);
  treein->SetBranchAddress("VecTPCUp_TrackCharge", &VecTPCUp_TrackCharge);
  treein->SetBranchAddress("VecTPCUp_TrackPDG", &VecTPCUp_TrackPDG);
  treein->SetBranchAddress("VecTPCUp_TrackStepFirstX",&VecTPCUp_TrackStepFirstX);
  treein->SetBranchAddress("VecTPCUp_TrackStepFirstY",&VecTPCUp_TrackStepFirstY);
  treein->SetBranchAddress("VecTPCUp_TrackStepFirstZ",&VecTPCUp_TrackStepFirstZ);
  treein->SetBranchAddress("VecTPCUp_TrackStepLastX",&VecTPCUp_TrackStepLastX);
  treein->SetBranchAddress("VecTPCUp_TrackStepLastY",&VecTPCUp_TrackStepLastY);
  treein->SetBranchAddress("VecTPCUp_TrackStepLastZ",&VecTPCUp_TrackStepLastZ);
  treein->SetBranchAddress("VecTPCUp_TrackCosTheta",&VecTPCUp_TrackCosTheta);

  treein->SetBranchAddress("TPCDown_NTracks",      &TPCDown_NTracks);
  treein->SetBranchAddress("VecTPCDown_TrackID",   &VecTPCDown_TrackID);
  treein->SetBranchAddress("VecTPCDown_ParentID",   &VecTPCDown_ParentID);
  treein->SetBranchAddress("VecTPCDown_TrackMomX", &VecTPCDown_TrackMomX);
  treein->SetBranchAddress("VecTPCDown_TrackMomY", &VecTPCDown_TrackMomY);
  treein->SetBranchAddress("VecTPCDown_TrackMomZ", &VecTPCDown_TrackMomZ);
  treein->SetBranchAddress("VecTPCDown_TrackLength", &VecTPCDown_TrackLength);
  treein->SetBranchAddress("VecTPCDown_TrackDeltaLyz", &VecTPCDown_TrackDeltaLyz);
  treein->SetBranchAddress("VecTPCDown_TrackEdep", &VecTPCDown_TrackEdep);
  treein->SetBranchAddress("VecTPCDown_TrackCharge", &VecTPCDown_TrackCharge);
  treein->SetBranchAddress("VecTPCDown_TrackPDG", &VecTPCDown_TrackPDG);
  treein->SetBranchAddress("VecTPCDown_TrackStepFirstX",&VecTPCDown_TrackStepFirstX);
  treein->SetBranchAddress("VecTPCDown_TrackStepFirstY",&VecTPCDown_TrackStepFirstY);
  treein->SetBranchAddress("VecTPCDown_TrackStepFirstZ",&VecTPCDown_TrackStepFirstZ);
  treein->SetBranchAddress("VecTPCDown_TrackStepLastX",&VecTPCDown_TrackStepLastX);
  treein->SetBranchAddress("VecTPCDown_TrackStepLastY",&VecTPCDown_TrackStepLastY);
  treein->SetBranchAddress("VecTPCDown_TrackStepLastZ",&VecTPCDown_TrackStepLastZ);
  treein->SetBranchAddress("VecTPCDown_TrackCosTheta",&VecTPCDown_TrackCosTheta);

  treein->SetBranchAddress("Target_NTracks",      &Target_NTracks);
  treein->SetBranchAddress("VecTarget_TrackID",   &VecTarget_TrackID);
  treein->SetBranchAddress("VecTarget_ParentID",   &VecTarget_ParentID);
  treein->SetBranchAddress("VecTarget_TrackMomX", &VecTarget_TrackMomX);
  treein->SetBranchAddress("VecTarget_TrackMomY", &VecTarget_TrackMomY);
  treein->SetBranchAddress("VecTarget_TrackMomZ", &VecTarget_TrackMomZ);
  treein->SetBranchAddress("VecTarget_TrackLength", &VecTarget_TrackLength);
  treein->SetBranchAddress("VecTarget_TrackDeltaLyz", &VecTarget_TrackDeltaLyz);
  treein->SetBranchAddress("VecTarget_TrackEdep", &VecTarget_TrackEdep);
  treein->SetBranchAddress("VecTarget_TrackCharge", &VecTarget_TrackCharge);
  treein->SetBranchAddress("VecTarget_TrackPDG", &VecTarget_TrackPDG);  
  treein->SetBranchAddress("VecTarget_TrackStepFirstX",&VecTarget_TrackStepFirstX);
  treein->SetBranchAddress("VecTarget_TrackStepFirstY",&VecTarget_TrackStepFirstY);
  treein->SetBranchAddress("VecTarget_TrackStepFirstZ",&VecTarget_TrackStepFirstZ);
  treein->SetBranchAddress("VecTarget_TrackStepLastX",&VecTarget_TrackStepLastX);
  treein->SetBranchAddress("VecTarget_TrackStepLastY",&VecTarget_TrackStepLastY);
  treein->SetBranchAddress("VecTarget_TrackStepLastZ",&VecTarget_TrackStepLastZ);
  treein->SetBranchAddress("VecTarget_TrackCosTheta",&VecTarget_TrackCosTheta);

  treein->SetBranchAddress("NVtx", &NVtx);
  treein->SetBranchAddress("VtxX", &VecVtx_X);
  treein->SetBranchAddress("VtxY", &VecVtx_Y);
  treein->SetBranchAddress("VtxZ", &VecVtx_Z);
  treein->SetBranchAddress("VecVtx_NuPDG", &VecVtx_NuPDG);
  treein->SetBranchAddress("VecVtx_ReacMode", &VecVtx_ReacMode);
  treein->SetBranchAddress("VecVtx_EvtProb", &VecVtx_EvtProb);
  treein->SetBranchAddress("VecVtx_EvtWeight", &VecVtx_EvtWeight);
  
  // Create tree to store outputs

  double TPCUp_High_ID;
  double TPCUp_High_mom;
  double TPCUp_High_costheta;
  double TPCUp_High_len;
  double TPCUp_High_dlyz;
  double TPCUp_High_charge;
  double TPCUp_High_edep;
  double TPCUp_High_pdg;
  double TPCUp_High_parent_pdg;
  double TPCUp_High_PosIniX;
  double TPCUp_High_PosIniY;
  double TPCUp_High_PosIniZ;
  double TPCUp_High_PosFinX;
  double TPCUp_High_PosFinY;
  double TPCUp_High_PosFinZ;
  double TPCDown_High_ID;
  double TPCDown_High_mom;
  double TPCDown_High_costheta;
  double TPCDown_High_len;
  double TPCDown_High_dlyz;
  double TPCDown_High_charge;
  double TPCDown_High_edep;
  double TPCDown_High_pdg;
  double TPCDown_High_parent_pdg;
  double TPCDown_High_PosIniX;
  double TPCDown_High_PosIniY;
  double TPCDown_High_PosIniZ;
  double TPCDown_High_PosFinX;
  double TPCDown_High_PosFinY;
  double TPCDown_High_PosFinZ;
  double Target_High_ID;
  double Target_High_mom;
  double Target_High_costheta;
  double Target_High_len;
  double Target_High_dlyz;
  double Target_High_charge;
  double Target_High_edep;
  double Target_High_pdg;
  double Target_High_parent_pdg;
  double Target_High_PosIniX;
  double Target_High_PosIniY;
  double Target_High_PosIniZ;
  double Target_High_PosFinX;
  double Target_High_PosFinY;
  double Target_High_PosFinZ;

  TFile *fileoa = new TFile("../bin/fileoa.root","RECREATE");
  TTree * treeoa = new TTree("treeoa","Skimmed MC");
  treeoa->Branch("TPCUp_High_ID",      &TPCUp_High_ID);
  treeoa->Branch("TPCUp_High_mom",      &TPCUp_High_mom);
  treeoa->Branch("TPCUp_High_costheta", &TPCUp_High_costheta);
  treeoa->Branch("TPCUp_High_len",      &TPCUp_High_len);
  treeoa->Branch("TPCUp_High_dlyz",      &TPCUp_High_dlyz);
  treeoa->Branch("TPCUp_High_charge",   &TPCUp_High_charge);
  treeoa->Branch("TPCUp_High_edep",     &TPCUp_High_edep);
  treeoa->Branch("TPCUp_High_pdg",      &TPCUp_High_pdg);
  treeoa->Branch("TPCUp_High_parent_pdg",&TPCUp_High_parent_pdg);
  treeoa->Branch("TPCUp_High_PosIniX",&TPCUp_High_PosIniX);
  treeoa->Branch("TPCUp_High_PosIniY",&TPCUp_High_PosIniY);
  treeoa->Branch("TPCUp_High_PosIniZ",&TPCUp_High_PosIniZ);
  treeoa->Branch("TPCUp_High_PosFinX",&TPCUp_High_PosFinX);
  treeoa->Branch("TPCUp_High_PosFinY",&TPCUp_High_PosFinY);
  treeoa->Branch("TPCUp_High_PosFinZ",&TPCUp_High_PosFinZ);
  treeoa->Branch("TPCDown_High_ID",    &TPCDown_High_ID);
  treeoa->Branch("TPCDown_High_mom",    &TPCDown_High_mom);
  treeoa->Branch("TPCDown_High_costheta",&TPCDown_High_costheta);
  treeoa->Branch("TPCDown_High_len",    &TPCDown_High_len);
  treeoa->Branch("TPCDown_High_dlyz",    &TPCDown_High_dlyz);
  treeoa->Branch("TPCDown_High_charge", &TPCDown_High_charge);
  treeoa->Branch("TPCDown_High_edep",   &TPCDown_High_edep);
  treeoa->Branch("TPCDown_High_pdg",    &TPCDown_High_pdg);
  treeoa->Branch("TPCDown_High_parent_pdg",&TPCDown_High_parent_pdg);
  treeoa->Branch("TPCDown_High_PosIniX",&TPCDown_High_PosIniX);
  treeoa->Branch("TPCDown_High_PosIniY",&TPCDown_High_PosIniY);
  treeoa->Branch("TPCDown_High_PosIniZ",&TPCDown_High_PosIniZ);
  treeoa->Branch("TPCDown_High_PosFinX",&TPCDown_High_PosFinX);
  treeoa->Branch("TPCDown_High_PosFinY",&TPCDown_High_PosFinY);
  treeoa->Branch("TPCDown_High_PosFinZ",&TPCDown_High_PosFinZ);
  treeoa->Branch("Target_High_ID",     &Target_High_ID);
  treeoa->Branch("Target_High_mom",     &Target_High_mom);
  treeoa->Branch("Target_High_costheta",&Target_High_costheta);
  treeoa->Branch("Target_High_len",     &Target_High_len);
  treeoa->Branch("Target_High_dlyz",     &Target_High_dlyz);
  treeoa->Branch("Target_High_charge",  &Target_High_charge);
  treeoa->Branch("Target_High_edep",    &Target_High_edep);
  treeoa->Branch("Target_High_pdg",     &Target_High_pdg);
  treeoa->Branch("Target_High_parent_pdg",&Target_High_parent_pdg);
  treeoa->Branch("Target_High_PosIniX",&Target_High_PosIniX);
  treeoa->Branch("Target_High_PosIniY",&Target_High_PosIniY);
  treeoa->Branch("Target_High_PosIniZ",&Target_High_PosIniZ);
  treeoa->Branch("Target_High_PosFinX",&Target_High_PosFinX);
  treeoa->Branch("Target_High_PosFinY",&Target_High_PosFinY);
  treeoa->Branch("Target_High_PosFinZ",&Target_High_PosFinZ);


  // Loop over the ntuple events
  
  int Nentries = treein->GetEntries();
  
  for(int ientry=0;ientry<Nentries;ientry++){
  //for(int ientry=0;ientry<10;ientry++){
  
    if(doSingleEvent){
      ientry = SingleEventID;
      Nentries = ientry+1;
    }

    // Reset variables
    TPCUp_High_ID = -999;
    TPCUp_High_mom = -999;
    TPCUp_High_costheta = -999;
    TPCUp_High_len = -999;
    TPCUp_High_dlyz = -999;
    TPCUp_High_charge = -999;
    TPCUp_High_edep = -999;
    TPCUp_High_pdg = -999;
    TPCUp_High_parent_pdg = -999;
    TPCUp_High_PosIniX = -999;
    TPCUp_High_PosIniY = -999;
    TPCUp_High_PosIniZ = -999;
    TPCUp_High_PosFinX = -999;
    TPCUp_High_PosFinY = -999;
    TPCUp_High_PosFinZ = -999;
    TPCDown_High_ID = -999;
    TPCDown_High_mom = -999;
    TPCDown_High_costheta = -999;
    TPCDown_High_len = -999;
    TPCDown_High_dlyz = -999;
    TPCDown_High_charge = -999;
    TPCDown_High_edep = -999;
    TPCDown_High_pdg = -999;
    TPCDown_High_parent_pdg = -999;
    TPCDown_High_PosIniX = -999;
    TPCDown_High_PosIniY = -999;
    TPCDown_High_PosIniZ = -999;
    TPCDown_High_PosFinX = -999;
    TPCDown_High_PosFinY = -999;
    TPCDown_High_PosFinZ = -999;
    Target_High_ID = -999;
    Target_High_mom = -999;
    Target_High_costheta = -999;
    Target_High_len = -999;
    Target_High_dlyz = -999;
    Target_High_charge = -999;
    Target_High_edep = -999;
    Target_High_pdg = -999;
    Target_High_parent_pdg = -999;
    Target_High_PosIniX = -999;
    Target_High_PosIniY = -999;
    Target_High_PosIniZ = -999;
    Target_High_PosFinX = -999;
    Target_High_PosFinY = -999;
    Target_High_PosFinZ = -999;

    treein->GetEntry(ientry);

    if(!(ientry%100)) cout << "Event " << ientry << endl;
    if(doPrintTrack) cout << "Event " << ientry << endl;

    //
    // Read all the tracks in the event (all volume)
    //

    if(NVtx!=1){
      cerr << endl;
      cerr << "The # of vertices is " << NVtx << "!!!" << endl;
      cerr << endl;
      exit(1);
    }
    
    double vtx_x = VecVtx_X->at(0);
    double vtx_y = VecVtx_Y->at(0);
    double vtx_z = VecVtx_Z->at(0);
    
    //
    // Cut 1: Apply vertex cuts (in the target FV)
    //
    if(doCut_Vtx){
      if(vtx_x < vtx_min_x || vtx_x > vtx_max_x)  continue;
      if(vtx_y < vtx_min_y || vtx_y > vtx_max_y)  continue;
      if(vtx_z < vtx_min_z || vtx_z > vtx_max_z)  continue;
    }

    hVtx_XY->Fill(vtx_x,vtx_y);
    hVtx_XZ->Fill(vtx_x,vtx_z);
    hVtx_YZ->Fill(vtx_y,vtx_z);      
    hVtx_X->Fill(vtx_x);
    hVtx_Y->Fill(vtx_y);
    hVtx_Z->Fill(vtx_z);

    if(NTracks!=(int)VecTrackID->size()){
      cerr << endl;
      cerr << "VecTrackID->size() != NTracks" << endl;
      cerr << "VecTrackID->size() = " << VecTrackID->size() << endl;
      cerr << "NTracks = " << NTracks << endl;
      cerr << endl;
      exit(1);
    }
    // if(NTracks!=VecTarget_TrackID->size()){
    //   cerr << endl;
    //   cerr << "VecTarget_TrackID->size() != NTracks" << endl;
    //   cerr << "VecTarget_TrackID->size() = " << VecTarget_TrackID->size() << endl;
    //   cerr << "NTracks = " << NTracks << endl;
    //   cerr << endl;
    //   exit(1);
    // }
    // if(NTracks!=VecTPCUp_TrackID->size()){
    //   cerr << endl;
    //   cerr << "VecTPCUp_TrackID->size() != NTracks" << endl;
    //   cerr << "VecTPCUp_TrackID->size() = " << VecTPCUp_TrackID->size() << endl;
    //   cerr << "NTracks = " << NTracks << endl;
    //   cerr << endl;
    //   exit(1);
    // }
    // if(NTracks!=VecTPCDown_TrackID->size()){
    //   cerr << endl;
    //   cerr << "VecTPCDown_TrackID->size() != NTracks" << endl;
    //   cerr << "VecTPCDown_TrackID->size() = " << VecTPCDown_TrackID->size() << endl;
    //   cerr << "NTracks = " << NTracks << endl;
    //   cerr << endl;
    //   exit(1);
    // }

    int NStepFirst_TPCUp = VecTPCUp_TrackStepFirstX->size();
    int NStepLast_TPCUp = VecTPCUp_TrackStepLastX->size();
    int NStepFirst_TPCDown = VecTPCDown_TrackStepFirstX->size();
    int NStepLast_TPCDown = VecTPCDown_TrackStepLastX->size();
    int NStepFirst_Target = VecTarget_TrackStepFirstX->size();
    int NStepLast_Target = VecTarget_TrackStepLastX->size();

    if(NStepFirst_TPCUp != NStepLast_TPCUp){
      cout << "Size TPCUp first step = " << VecTPCUp_TrackStepFirstX->size() << endl;
      cout << "Size TPCUp last step = " << VecTPCUp_TrackStepLastX->size() << endl;
      exit(1);
    }
    if(NStepFirst_TPCDown != NStepLast_TPCDown){
      cout << "Size TPCDown first step = " << VecTPCDown_TrackStepFirstX->size() << endl;
      cout << "Size TPCDown last step = " << VecTPCDown_TrackStepLastX->size() << endl;
      exit(1);
    }
    if(NStepFirst_Target != NStepLast_Target){
      cout << "Size Target first step = " << VecTarget_TrackStepFirstX->size() << endl;
      cout << "Size Target last step = " << VecTarget_TrackStepLastX->size() << endl;
      exit(1);
    }


    //
    // Loop over all tracks
    // 

    for(int trkid=1;trkid<=NTracks;trkid++){ // loop over track ID (1 to NTracks)
    //for(int trkid=1;trkid<=1;trkid++){ // loop over track ID (1 to NTracks)
          
      int vecel=-999,id=-999;
      double costheta=-999,momX=-999,momY=-999,momZ=-999,mom=-999,length=-999,deltaLyz=-999,edep=-999,charge=-999,pdg=-999,posX=-999,posY=-999,posZ=-999,posLastX=-999,posLastY=-999,posLastZ=-999;   
      
      //
      // Check if reconstructed in Target
      //

      vecel = FindTrackElem(VecTarget_TrackID,trkid);      
      if(vecel>=0){
	id = VecTarget_TrackID->at(vecel);
	costheta = VecTarget_TrackCosTheta->at(vecel);
	momX = VecTarget_TrackMomX->at(vecel);
	momY = VecTarget_TrackMomY->at(vecel);
	momZ = VecTarget_TrackMomZ->at(vecel);
	mom = sqrt( momX*momX + momY*momY + momZ*momZ );
	deltaLyz = VecTarget_TrackDeltaLyz->at(vecel);
	length = VecTarget_TrackLength->at(vecel);
	edep = VecTarget_TrackEdep->at(vecel);
	charge = VecTarget_TrackCharge->at(vecel);
	pdg = VecTarget_TrackPDG->at(vecel);
	
	// Cut 1: Select track PDG 
	if(doSelPDG && pdg!=SetPDG) continue;

	// Cut 2: Select tracks with dLyz > 50mm
	if(doCut_dL && length < dlyz_target_min) continue;

	// Cut 3: Select charged particles
	if(fabs(charge)!=cut_charge) continue;

	hReco_CosThetaVsMom_Target->Fill(costheta,mom); // Fill efficiency denominator

	if(doPrintTrack){
	  cout << "  Track ID = " << trkid << endl;
	  cout << "     - Target track: " << " ";
	  cout << "event = " << ientry << ", ";
	  cout << "mom = " << mom << ", ";
	  cout << "length = " << length << ", ";
	  cout << "charge = " << charge << ", ";
	  cout << "edep = " << edep << ", ";    
	  cout << "pdg = " << pdg << ", ";
	  cout << endl;    
	}
	
	//
	// Check if reconstructed in TPCUp
	//
	
	vecel = FindTrackElem(VecTPCUp_TrackID,trkid);  
	if(vecel>=0){
	  id = VecTPCUp_TrackID->at(vecel);
	  costheta = VecTPCUp_TrackCosTheta->at(vecel);
	  momX = VecTPCUp_TrackMomX->at(vecel);
	  momY = VecTPCUp_TrackMomY->at(vecel);
	  momZ = VecTPCUp_TrackMomZ->at(vecel);
	  mom = sqrt( momX*momX + momY*momY + momZ*momZ );
	  deltaLyz = VecTPCUp_TrackDeltaLyz->at(vecel);
	  length = VecTPCUp_TrackLength->at(vecel);
	  edep = VecTPCUp_TrackEdep->at(vecel);
	  charge = VecTPCUp_TrackCharge->at(vecel);
	  pdg = VecTPCUp_TrackPDG->at(vecel);

	  // Cut 2: Select tracks with dLyz > 200mm
	  if(doCut_dL && deltaLyz < len_tpc_min) continue;

	  // Cut 3: Select charged particles
	  if(doCut_poscharge && fabs(charge)!=cut_charge) continue;
	  
	  hReco_CosThetaVsMom_Fin->Fill(costheta,mom); // Fill efficiency denominator
	  
	  if(doPrintTrack){
	    cout << "     - TPCUp track: " << " ";
	    cout << "event = " << ientry << ", ";
	    cout << "mom = " << mom << ", ";
	    cout << "deltaLyz = " << deltaLyz << ", ";
	    cout << "charge = " << charge << ", ";
	    cout << "edep = " << edep << ", ";    
	    cout << "pdg = " << pdg << ", ";
	    cout << endl;
	  }

	} // reco in TPCUp
	
	
	else {
	  
	  //
	  // Check if reconstructed in TPCDown
	  //
	  
	  vecel = FindTrackElem(VecTPCDown_TrackID,trkid);      
	  if(vecel>=0){
	    id = VecTPCDown_TrackID->at(vecel);
	    costheta = VecTPCDown_TrackCosTheta->at(vecel);
	    momX = VecTPCDown_TrackMomX->at(vecel);
	    momY = VecTPCDown_TrackMomY->at(vecel);
	    momZ = VecTPCDown_TrackMomZ->at(vecel);
	    mom = sqrt( momX*momX + momY*momY + momZ*momZ );
	    deltaLyz = VecTPCDown_TrackDeltaLyz->at(vecel);
	    length = VecTPCDown_TrackLength->at(vecel);
	    edep = VecTPCDown_TrackEdep->at(vecel);
	    charge = VecTPCDown_TrackCharge->at(vecel);
	    pdg = VecTPCDown_TrackPDG->at(vecel);

	    // Cut 2: Select tracks with dLyz > 200mm
	    if(doCut_dL && deltaLyz < len_tpc_min) continue;
	    
	    // Cut 3: Select charged particles
	    if(doCut_poscharge && fabs(charge)!=cut_charge) continue;
	    
	    hReco_CosThetaVsMom_Fin->Fill(costheta,mom); // Fill efficiency denominator
	    hReco_CosTheta->Fill(costheta);
	    hReco_Mom->Fill(mom);
	    
	    if(doPrintTrack){
	      cout << "     - TPCDown track: " << " ";
	      cout << "event = " << ientry << ", ";
	      cout << "mom = " << mom << ", ";
	      cout << "deltaLyz = " << deltaLyz << ", ";
	      cout << "charge = " << charge << ", ";
	      cout << "edep = " << edep << ", ";    
	      cout << "pdg = " << pdg << ", ";
	      cout << endl;
	    }
	  } // reco in TPCDown
	} // not in TPCDown
		
      } // in the Target
     
    } // end loop over all tracks
  



    


    
   
    /////////////////////////////////////////////////
    //                                             //
    // Check tracks in each detector independently //
    //                                             //
    /////////////////////////////////////////////////

    //
    // TPC Up tracks
    //
        
    for(int itrk=0;itrk<(int)VecTPCUp_TrackID->size();itrk++){
      
      int id = VecTPCUp_TrackID->at(itrk);
      double costheta = VecTPCUp_TrackCosTheta->at(itrk);
      double momX = VecTPCUp_TrackMomX->at(itrk);
      double momY = VecTPCUp_TrackMomY->at(itrk);
      double momZ = VecTPCUp_TrackMomZ->at(itrk);
      double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
      double length = VecTPCUp_TrackLength->at(itrk);
      double deltaLyz = VecTPCUp_TrackDeltaLyz->at(itrk);
      double edep = VecTPCUp_TrackEdep->at(itrk);
      double charge = VecTPCUp_TrackCharge->at(itrk);
      double pdg = VecTPCUp_TrackPDG->at(itrk);
      double posX = VecTPCUp_TrackStepFirstX->at(itrk);
      double posY = VecTPCUp_TrackStepFirstY->at(itrk);
      double posZ = VecTPCUp_TrackStepFirstZ->at(itrk);
      double posLastX = VecTPCUp_TrackStepLastX->at(itrk);
      double posLastY = VecTPCUp_TrackStepLastY->at(itrk);
      double posLastZ = VecTPCUp_TrackStepLastZ->at(itrk);

      if(doPrintTrack){
    	cout << "TPC Up track: " << " ";
    	cout << "id = " << id << ", ";
    	cout << "event = " << ientry << ", ";
    	cout << "mom = " << mom << ", ";
    	cout << "deltaLyz = " << deltaLyz << ", ";
    	cout << "charge = " << charge << ", ";
    	cout << "edep = " << edep << ", ";    
    	cout << "pdg = " << pdg << endl;    
      }
      
      // Cut 2: Select tracks with dLyz > 200mm
      if(doCut_dL && deltaLyz < 200) continue;

      // Cut 3: Select charged particles
      if(doCut_poscharge && fabs(charge)!=1) continue;
      
      // Cut 4: Select highest momentum track
      if(mom > TPCUp_High_mom){
    	TPCUp_High_ID = id;
    	TPCUp_High_mom = mom;
    	TPCUp_High_costheta = costheta;
    	TPCUp_High_len = length;
    	TPCUp_High_dlyz = deltaLyz;
    	TPCUp_High_charge = charge;
    	TPCUp_High_edep = edep;
    	TPCUp_High_pdg = pdg;
    	TPCUp_High_PosIniX = posX;
    	TPCUp_High_PosIniY = posY;
    	TPCUp_High_PosIniZ = posZ;
    	TPCUp_High_PosFinX = posLastX;
    	TPCUp_High_PosFinY = posLastY;
    	TPCUp_High_PosFinZ = posLastZ;
      }
    } // end TPC Up tracks


    //
    // TPC Down tracks
    //
    
    for(int itrk=0;itrk<(int)VecTPCDown_TrackID->size();itrk++){

      int id = VecTPCDown_TrackID->at(itrk);
      double costheta = VecTPCDown_TrackCosTheta->at(itrk);
      double momX = VecTPCDown_TrackMomX->at(itrk);
      double momY = VecTPCDown_TrackMomY->at(itrk);
      double momZ = VecTPCDown_TrackMomZ->at(itrk);
      double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
      double length = VecTPCDown_TrackLength->at(itrk);
      double deltaLyz = VecTPCDown_TrackDeltaLyz->at(itrk);
      double edep = VecTPCDown_TrackEdep->at(itrk);
      double charge = VecTPCDown_TrackCharge->at(itrk);
      double pdg = VecTPCDown_TrackPDG->at(itrk);
      double posX = VecTPCDown_TrackStepFirstX->at(itrk);
      double posY = VecTPCDown_TrackStepFirstY->at(itrk);
      double posZ = VecTPCDown_TrackStepFirstZ->at(itrk);
      double posLastX = VecTPCDown_TrackStepLastX->at(itrk);
      double posLastY = VecTPCDown_TrackStepLastY->at(itrk);
      double posLastZ = VecTPCDown_TrackStepLastZ->at(itrk);

      if(doPrintTrack){
    	cout << "TPC Down track: " << " ";
    	cout << "id = " << id << ", ";
    	cout << "event = " << ientry << ", ";
    	cout << "mom = " << mom << ", ";
    	cout << "deltaLyz = " << deltaLyz << ", ";
    	cout << "charge = " << charge << ", ";
    	cout << "edep = " << edep << ", ";    
    	cout << "pdg = " << pdg << endl;    
      }

      // Cut 2: Select tracks with dLyz > 200mm
      if(doCut_dL && deltaLyz < 200) continue;

      // Cut 3: Select charged particles
      if(doCut_poscharge && fabs(charge)!=1) continue;
      
      // Cut 4: Select highest momentum track      
      if(mom > TPCDown_High_mom){
    	TPCDown_High_ID = id;
    	TPCDown_High_mom = mom;
    	TPCDown_High_costheta = costheta;
    	TPCDown_High_len = length;
    	TPCDown_High_dlyz = deltaLyz;
    	TPCDown_High_charge = charge;
    	TPCDown_High_edep = edep;
    	TPCDown_High_pdg = pdg;
    	TPCDown_High_PosIniX = posX;
    	TPCDown_High_PosIniY = posY;
    	TPCDown_High_PosIniZ = posZ;
    	TPCDown_High_PosFinX = posLastX;
    	TPCDown_High_PosFinY = posLastY;
    	TPCDown_High_PosFinZ = posLastZ;
      }     
    } // end TPC Down tracks
    
    //
    // Target tracks
    //
    
    for(int itrk=0;itrk<(int)VecTarget_TrackID->size();itrk++){

      int id = VecTarget_TrackID->at(itrk);
      double costheta = VecTarget_TrackCosTheta->at(itrk);
      double momX = VecTarget_TrackMomX->at(itrk);
      double momY = VecTarget_TrackMomY->at(itrk);
      double momZ = VecTarget_TrackMomZ->at(itrk);
      double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
      double length = VecTarget_TrackLength->at(itrk);
      double deltaLyz = VecTarget_TrackDeltaLyz->at(itrk);
      double edep = VecTarget_TrackEdep->at(itrk);
      double charge = VecTarget_TrackCharge->at(itrk);
      double pdg = VecTarget_TrackPDG->at(itrk);
      double posX = VecTarget_TrackStepFirstX->at(itrk);
      double posY = VecTarget_TrackStepFirstY->at(itrk);
      double posZ = VecTarget_TrackStepFirstZ->at(itrk);
      double posLastX = VecTarget_TrackStepLastX->at(itrk);
      double posLastY = VecTarget_TrackStepLastY->at(itrk);
      double posLastZ = VecTarget_TrackStepLastZ->at(itrk);

      if(doPrintTrack){
    	cout << "Target track: " << " ";
    	cout << "id = " << id << ", ";
    	cout << "event = " << ientry << ", ";
    	cout << "mom = " << mom << ", ";
    	cout << "deltaLyz = " << deltaLyz << ", ";
    	cout << "charge = " << charge << ", ";
    	cout << "edep = " << edep << ", ";    
    	cout << "pdg = " << pdg << endl;    
      }

      // Cut 2: Select tracks with dLyz > 50mm
      if(doCut_dL && deltaLyz < 50) continue;
       
      // Cut 3: Select charged particles
      if(doCut_poscharge && fabs(charge)!=1) continue;

      // Cut 4: Select highest momentum track
      if(mom > Target_High_mom){
    	Target_High_ID = id;
    	Target_High_costheta = costheta;
    	Target_High_mom = mom;
    	Target_High_len = length;
    	Target_High_dlyz = deltaLyz;
    	Target_High_charge = charge;
    	Target_High_edep = edep;
    	Target_High_pdg = pdg;
    	Target_High_PosIniX = posX;
    	Target_High_PosIniY = posY;
    	Target_High_PosIniZ = posZ;
    	Target_High_PosFinX = posLastX;
    	Target_High_PosFinY = posLastY;
    	Target_High_PosFinZ = posLastZ;
      }
    } // end Target tracks

    
    
    // Print out the highest momentum track
 
    if(TPCUp_High_mom>0.){
      
      if(doPrintTrack){
	cout << "TPC Up high mom track: " << " ";
	cout << "event = " << ientry << ", ";
	cout << "id = " << TPCUp_High_ID << ", ";
	cout << "mom = " << TPCUp_High_mom << ", ";
	cout << "length = " << TPCUp_High_len << ", ";
	cout << "charge = " << TPCUp_High_charge << ", ";
	cout << "edep = " << TPCUp_High_edep << ", ";
	cout << "pdg = " << TPCUp_High_pdg << endl;    
      }

      hTPCUp_CosTheta->Fill(TPCUp_High_costheta);
      hTPCUp_Mom   ->Fill(TPCUp_High_mom);
      hTPCUp_Length->Fill(TPCUp_High_len);
      hTPCUp_DeltaLyz->Fill(TPCUp_High_dlyz);
      hTPCUp_Charge->Fill(TPCUp_High_charge);
      hTPCUp_PDG   ->Fill(TPCUp_High_pdg);
      hTPCUp_Edep  ->Fill(TPCUp_High_edep);
      hTPCUp_PosIniX->Fill(TPCUp_High_PosIniX);
      hTPCUp_PosIniY->Fill(TPCUp_High_PosIniY);
      hTPCUp_PosIniZ->Fill(TPCUp_High_PosIniZ);
      hTPCUp_PosFinX->Fill(TPCUp_High_PosFinX);
      hTPCUp_PosFinY->Fill(TPCUp_High_PosFinY);
      hTPCUp_PosFinZ->Fill(TPCUp_High_PosFinZ);
    }

    if(TPCDown_High_mom>0.){

      if(doPrintTrack){
	cout << "TPC Down high mom track: " << " ";
	cout << "event = " << ientry << ", ";
	cout << "id = " << TPCDown_High_ID << ", ";
	cout << "mom = " << TPCDown_High_mom << ", ";
	cout << "length = " << TPCDown_High_len << ", ";
	cout << "charge = " << TPCDown_High_charge << ", ";
	cout << "edep = " << TPCDown_High_edep << ", ";
	cout << "pdg = " << TPCDown_High_pdg << endl;    
      }
      
      hTPCDown_CosTheta->Fill(TPCDown_High_costheta);
      hTPCDown_Mom   ->Fill(TPCDown_High_mom);
      hTPCDown_Length->Fill(TPCDown_High_len);
      hTPCDown_DeltaLyz->Fill(TPCDown_High_dlyz);
      hTPCDown_Charge->Fill(TPCDown_High_charge);
      hTPCDown_PDG   ->Fill(TPCDown_High_pdg);
      hTPCDown_Edep  ->Fill(TPCDown_High_edep);
      hTPCDown_PosIniX->Fill(TPCDown_High_PosIniX);
      hTPCDown_PosIniY->Fill(TPCDown_High_PosIniY);
      hTPCDown_PosIniZ->Fill(TPCDown_High_PosIniZ);
      hTPCDown_PosFinX->Fill(TPCDown_High_PosFinX);
      hTPCDown_PosFinY->Fill(TPCDown_High_PosFinY);
      hTPCDown_PosFinZ->Fill(TPCDown_High_PosFinZ);
    }    

    if(Target_High_mom>0.){

      if(doPrintTrack){
	cout << "Target high mom track: " << " ";
	cout << "event = " << ientry << ", ";
	cout << "id = " << Target_High_ID << ", ";
	cout << "mom = " << Target_High_mom << ", ";
	cout << "length = " << Target_High_len << ", ";
	cout << "charge = " << Target_High_charge << ", ";
	cout << "edep = " << Target_High_edep << ", ";
	cout << "pdg = " << Target_High_pdg << endl;    
      }

      hTarget_CosTheta->Fill(Target_High_costheta);
      hTarget_Mom   ->Fill(Target_High_mom);
      hTarget_Length->Fill(Target_High_len);
      hTarget_DeltaLyz->Fill(Target_High_dlyz);
      hTarget_Charge->Fill(Target_High_charge);
      hTarget_PDG   ->Fill(Target_High_pdg);
      hTarget_Edep  ->Fill(Target_High_edep);
      hTarget_PosIniX->Fill(Target_High_PosIniX);
      hTarget_PosIniY->Fill(Target_High_PosIniY);
      hTarget_PosIniZ->Fill(Target_High_PosIniZ);
      hTarget_PosFinX->Fill(Target_High_PosFinX);
      hTarget_PosFinY->Fill(Target_High_PosFinY);
      hTarget_PosFinZ->Fill(Target_High_PosFinZ);
      
      hVtxMinusPosIni_Target_X->Fill(vtx_x-Target_High_PosIniX);
      hVtxMinusPosIni_Target_Y->Fill(vtx_y-Target_High_PosIniY);
      hVtxMinusPosIni_Target_Z->Fill(vtx_z-Target_High_PosIniZ);
      
      // if( fabs(vtx_x-Target_High_PosIniX) > 0.){
      // 	cout << "Vtx - Target first step pos:" 
      // 	     << " x = " << vtx_x-Target_High_PosIniX
      // 	     << " y = " << vtx_y-Target_High_PosIniY
      // 	     << " z = " << vtx_z-Target_High_PosIniZ
      // 	     << " - Vtx pos:"
      // 	     << " x = " << vtx_x
      // 	     << " y = " << vtx_y
      // 	     << " z = " << vtx_z
      // 	     << " - Vtx reac mode: " << VecVtx_ReacMode->at(0)
      // 	     << " - High track: "
      // 	     << " mom = " << Target_High_mom
      // 	     << " costheta = " << Target_High_costheta
      // 	     << " pdg = " << Target_High_pdg
      // 	     << endl;
      // }      
    }

    
    
  } // end loop over events


  // Store the Tree 
  treeoa->Write();   
  fileoa->Close();

  
  // Get the efficiency
  hReco_CosThetaVsMom_Eff = (TH2D*)hReco_CosThetaVsMom_Fin->Clone("hReco_CosThetaVsMom_Eff");
  hReco_CosThetaVsMom_Eff->Divide(hReco_CosThetaVsMom_Target);
    
   

  // Draw histograms

  TCanvas *cTPCUp_TrackMom = new TCanvas("cTPCUp_TrackMom","cTPCUp_TrackMom");
  hTPCUp_Mom->Draw();
  TCanvas *cTPCUp_TrackCosTheta = new TCanvas("cTPCUp_TrackCosTheta","cTPCUp_TrackCosTheta");
  hTPCUp_CosTheta->Draw();
  TCanvas *cTPCUp_TrackLength = new TCanvas("cTPCUp_TrackLength","cTPCUp_TrackLength");
  hTPCUp_Length->Draw();
  TCanvas *cTPCUp_TrackDeltaLyz = new TCanvas("cTPCUp_TrackDeltaLyz","cTPCUp_TrackDeltaLyz");
  hTPCUp_DeltaLyz->Draw();
  // TCanvas *cTPCUp_TrackEdep = new TCanvas("cTPCUp_TrackEdep","cTPCUp_TrackEdep");
  // hTPCUp_Edep->Draw();
  TCanvas *cTPCUp_TrackCharge = new TCanvas("cTPCUp_TrackCharge","cTPCUp_TrackCharge");
  hTPCUp_Charge->Draw();
  // TCanvas *cTPCUp_TrackPDG = new TCanvas("cTPCUp_TrackPDG","cTPCUp_TrackPDG");
  // hTPCUp_PDG->Draw();
  TCanvas *cTPCUp_PosIniX = new TCanvas("cTPCUp_PosIniX","cTPCUp_PosIniX");
  hTPCUp_PosIniX->Draw();
  TCanvas *cTPCUp_PosIniY = new TCanvas("cTPCUp_PosIniY","cTPCUp_PosIniY");
  hTPCUp_PosIniY->Draw();
  TCanvas *cTPCUp_PosIniZ = new TCanvas("cTPCUp_PosIniZ","cTPCUp_PosIniZ");
  hTPCUp_PosIniZ->Draw();
  TCanvas *cTPCUp_PosFinX = new TCanvas("cTPCUp_PosFinX","cTPCUp_PosFinX");
  hTPCUp_PosFinX->Draw();
  TCanvas *cTPCUp_PosFinY = new TCanvas("cTPCUp_PosFinY","cTPCUp_PosFinY");
  hTPCUp_PosFinY->Draw();
  TCanvas *cTPCUp_PosFinZ = new TCanvas("cTPCUp_PosFinZ","cTPCUp_PosFinZ");
  hTPCUp_PosFinZ->Draw();
 
  TCanvas *cTPCDown_TrackMom = new TCanvas("cTPCDown_TrackMom","cTPCDown_TrackMom");
  hTPCDown_Mom->Draw();
  TCanvas *cTPCDown_TrackCosTheta = new TCanvas("cTPCDown_TrackCosTheta","cTPCDown_TrackCosTheta");
  hTPCDown_CosTheta->Draw();
  TCanvas *cTPCDown_TrackLength = new TCanvas("cTPCDown_TrackLength","cTPCDown_TrackLength");
  hTPCDown_Length->Draw();
  TCanvas *cTPCDown_TrackDeltaLyz = new TCanvas("cTPCDown_TrackDeltaLyz","cTPCDown_TrackDeltaLyz");
  hTPCDown_DeltaLyz->Draw();
  // TCanvas *cTPCDown_TrackEdep = new TCanvas("cTPCDown_TrackEdep","cTPCDown_TrackEdep");
  // hTPCDown_Edep->Draw();
  TCanvas *cTPCDown_TrackCharge = new TCanvas("cTPCDown_TrackCharge","cTPCDown_TrackCharge");
  hTPCDown_Charge->Draw();
  // TCanvas *cTPCDown_TrackPDG = new TCanvas("cTPCDown_TrackPDG","cTPCDown_TrackPDG");
  // hTPCDown_PDG->Draw();
  TCanvas *cTPCDown_PosIniX = new TCanvas("cTPCDown_PosIniX","cTPCDown_PosIniX");
  hTPCDown_PosIniX->Draw();
  TCanvas *cTPCDown_PosIniY = new TCanvas("cTPCDown_PosIniY","cTPCDown_PosIniY");
  hTPCDown_PosIniY->Draw();
  TCanvas *cTPCDown_PosIniZ = new TCanvas("cTPCDown_PosIniZ","cTPCDown_PosIniZ");
  hTPCDown_PosIniZ->Draw();
  TCanvas *cTPCDown_PosFinX = new TCanvas("cTPCDown_PosFinX","cTPCDown_PosFinX");
  hTPCDown_PosFinX->Draw();
  TCanvas *cTPCDown_PosFinY = new TCanvas("cTPCDown_PosFinY","cTPCDown_PosFinY");
  hTPCDown_PosFinY->Draw();
  TCanvas *cTPCDown_PosFinZ = new TCanvas("cTPCDown_PosFinZ","cTPCDown_PosFinZ");
  hTPCDown_PosFinZ->Draw();

  TCanvas *cTarget_TrackMom = new TCanvas("cTarget_TrackMom","cTarget_TrackMom");
  hTarget_Mom->Draw();
  TCanvas *cTarget_TrackCosTheta = new TCanvas("cTarget_TrackCosTheta","cTarget_TrackCosTheta");
  hTarget_CosTheta->Draw();
  TCanvas *cTarget_TrackLength = new TCanvas("cTarget_TrackLength","cTarget_TrackLength");
  hTarget_Length->Draw();
  TCanvas *cTarget_TrackDeltaLyz = new TCanvas("cTarget_TrackDeltaLyz","cTarget_TrackDeltaLyz");
  hTarget_DeltaLyz->Draw();
  TCanvas *cTarget_TrackEdep = new TCanvas("cTarget_TrackEdep","cTarget_TrackEdep");
  hTarget_Edep->Draw();
  // TCanvas *cTarget_TrackCharge = new TCanvas("cTarget_TrackCharge","cTarget_TrackCharge");
  // hTarget_Charge->Draw();
  // TCanvas *cTarget_TrackPDG = new TCanvas("cTarget_TrackPDG","cTarget_TrackPDG");
  // hTarget_PDG->Draw();

  TCanvas *cVtx_X = new TCanvas("cVtx_X","cVtx_X");
  hVtx_X->GetXaxis()->SetTitle("X (mm)");
  hVtx_X->Draw();
  TCanvas *cVtx_Y = new TCanvas("cVtx_Y","cVtx_Y");
  hVtx_Y->GetXaxis()->SetTitle("Y (mm)");
  hVtx_Y->Draw();
  TCanvas *cVtx_Z = new TCanvas("cVtx_Z","cVtx_Z");
  hVtx_Z->GetXaxis()->SetTitle("Z (mm)");
  hVtx_Z->Draw();
  

  TCanvas *cReco_CosThetaVsMom_Target = new TCanvas("cReco_CosThetaVsMom_Target","cReco_CosThetaVsMom_Target");
  hReco_CosThetaVsMom_Target->Draw("colz");
  TCanvas *cReco_CosThetaVsMom_Fin = new TCanvas("cReco_CosThetaVsMom_Fin","cReco_CosThetaVsMom_Fin");
  hReco_CosThetaVsMom_Fin->Draw("colz");
  TCanvas *cReco_CosThetaVsMom_Eff = new TCanvas("cReco_CosThetaVsMom_Eff","cReco_CosThetaVsMom_Eff");
  hReco_CosThetaVsMom_Eff->Draw("colz");

  TCanvas *cReco_Mom = new TCanvas("cReco_Mom","cReco_Mom");
  hReco_Mom->Draw();
  TCanvas *cTarget_CosTheta = new TCanvas("cTarget_CosTheta","cTarget_CosTheta");
  hReco_CosTheta->Draw();
  TCanvas *cReco_PDG = new TCanvas("cReco_PDG","cReco_PDG");
  hReco_PDG->Draw();
  

  // TCanvas *cTarget_PosIniX = new TCanvas("cTarget_PosIniX","cTarget_PosIniX");
  // hTarget_PosIniX->Draw();
  // TCanvas *cTarget_PosIniY = new TCanvas("cTarget_PosIniY","cTarget_PosIniY");
  // hTarget_PosIniY->Draw();
  // TCanvas *cTarget_PosIniZ = new TCanvas("cTarget_PosIniZ","cTarget_PosIniZ");
  // hTarget_PosIniZ->Draw();
  // TCanvas *cTarget_PosFinX = new TCanvas("cTarget_PosFinX","cTarget_PosFinX");
  // hTarget_PosFinX->Draw();
  // TCanvas *cTarget_PosFinY = new TCanvas("cTarget_PosFinY","cTarget_PosFinY");
  // hTarget_PosFinY->Draw();
  // TCanvas *cTarget_PosFinZ = new TCanvas("cTarget_PosFinZ","cTarget_PosFinZ");
  // hTarget_PosFinZ->Draw();

  // TCanvas *cVtxMinusPosIni_Target_X = new TCanvas("cVtxMinusPosIni_Target_X","cVtxMinusPosIni_Target_X");
  // hVtxMinusPosIni_Target_X->GetXaxis()->SetTitle("Vtx pos - First step pos (mm)");
  // hVtxMinusPosIni_Target_X->Draw();
  // TCanvas *cVtxMinusPosIni_Target_Y = new TCanvas("cVtxMinusPosIni_Target_Y","cVtxMinusPosIni_Target_Y");
  // hVtxMinusPosIni_Target_Y->GetXaxis()->SetTitle("Vtx pos - First step pos (mm)");
  // hVtxMinusPosIni_Target_Y->Draw();
  // TCanvas *cVtxMinusPosIni_Target_Z = new TCanvas("cVtxMinusPosIni_Target_Z","cVtxMinusPosIni_Target_Z");
  // hVtxMinusPosIni_Target_Z->GetXaxis()->SetTitle("Vtx pos - First step pos (mm)");
  // hVtxMinusPosIni_Target_Z->Draw();


  // Print 

  // Store plots

  char name[300];
  if(doprint){    
    sprintf(name,"outputs/cVecVtx_X_%s.pdf",tag); cVtx_X->Print(name);
    sprintf(name,"outputs/cVecVtx_Y_%s.pdf",tag); cVtx_Y->Print(name);
    sprintf(name,"outputs/cVecVtx_Z_%s.pdf",tag); cVtx_Z->Print(name);
 
    //sprintf(name,"outputs/cTarget_PosIniX_%s.pdf",tag); cTarget_PosIniX->Print(name);
    //sprintf(name,"outputs/cTarget_PosIniY_%s.pdf",tag); cTarget_PosIniY->Print(name);
    //sprintf(name,"outputs/cTarget_PosIniZ_%s.pdf",tag); cTarget_PosIniZ->Print(name);
    //sprintf(name,"outputs/cTarget_PosFinX_%s.pdf",tag); cTarget_PosFinX->Print(name);
    //sprintf(name,"outputs/cTarget_PosFinY_%s.pdf",tag); cTarget_PosFinY->Print(name);
    //sprintf(name,"outputs/cTarget_PosFinZ_%s.pdf",tag); cTarget_PosFinZ->Print(name);

    //sprintf(name,"outputs/cTPCUp_PosIniX_%s.pdf",tag); cTPCUp_PosIniX->Print(name);
    //sprintf(name,"outputs/cTPCUp_PosIniY_%s.pdf",tag); cTPCUp_PosIniY->Print(name);
    //sprintf(name,"outputs/cTPCUp_PosIniZ_%s.pdf",tag); cTPCUp_PosIniZ->Print(name);
    //sprintf(name,"outputs/cTPCUp_PosFinX_%s.pdf",tag); cTPCUp_PosFinX->Print(name);
    //sprintf(name,"outputs/cTPCUp_PosFinY_%s.pdf",tag); cTPCUp_PosFinY->Print(name);
    //sprintf(name,"outputs/cTPCUp_PosFinZ_%s.pdf",tag); cTPCUp_PosFinZ->Print(name);

    //sprintf(name,"outputs/cTPCDown_PosIniX_%s.pdf",tag); cTPCDown_PosIniX->Print(name);
    //sprintf(name,"outputs/cTPCDown_PosIniY_%s.pdf",tag); cTPCDown_PosIniY->Print(name);
    //sprintf(name,"outputs/cTPCDown_PosIniZ_%s.pdf",tag); cTPCDown_PosIniZ->Print(name);
    //sprintf(name,"outputs/cTPCDown_PosFinX_%s.pdf",tag); cTPCDown_PosFinX->Print(name);
    //sprintf(name,"outputs/cTPCDown_PosFinY_%s.pdf",tag); cTPCDown_PosFinY->Print(name);
    //sprintf(name,"outputs/cTPCDown_PosFinZ_%s.pdf",tag); cTPCDown_PosFinZ->Print(name);
  }
  
  // // Write text file
  
  // ofstream * outfilep = new ofstream(Form("outputs/%s.txt",tag),ios_base::trunc);
  // ofstream & outfile = *outfilep;

  // stringstream streamer;
  // //streamer << endl;
  // //streamer << endl;
  // cout << streamer.str() << endl;
  
  // // get size of file
  // long size;
  // streamer.seekg(0,ifstream::end);
  // size=streamer.tellg();
  // streamer.seekg(0);

  // outfile.write (streamer.str().c_str(),size);
  // outfile.close();



  }




int FindTrackElem(std::vector<int> *vectrk,int trkid){
 
  //cout << "Find index of value " << trkid << endl;
  //cout << "vectrk.size() = " << vectrk->size() << endl;
  
  int nPosition = -999;
   
  std::vector<int>::iterator it = vectrk->begin();
  
  //while (it != vectrk->end()){
  //cout << *it << endl;
  //++ it;
  //}
  
  it = find(vectrk->begin(),vectrk->end(),trkid);
  
  //cout << "The found value is " << *it << endl;
  
  if(it != vectrk->end()){
    //nPosition = std::distance( vectrk->begin(), it); // not defined in CINT
    nPosition = it - vectrk->begin();
    //cout << "Value "<< *it;
    //cout << " found in the vector at position: " << nPosition << endl;
  }
  
  return nPosition;
}
