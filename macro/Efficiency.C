
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
#include <TGraph.h>

using namespace std;

int FindTrackElem(std::vector<int> *vectrk,int trkid);

void Efficiency()
{
    const char *tag = "prova";
    const char *filename = "../bin/EffStudy_highstat.root";
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
    const double len_target_min = 50; //mm
    const double dlyz_tpc_min = 200; //mm
    
    // Charge cut (TPCs and Target)
    const bool doCut_poscharge = true;
    const double cut_charge = 1; // only charge particles
    
    const bool doSelPDG = true;
    const int SetPDG = +2212; //+2212; //+13;
    
    // For debugging:
    const bool doSingleEvent = false;
    const int SingleEventID = 9; // 87
    
    const int doPrintTrack = false;
    
    ////////////////////////
    
    
    // True tracks
    TH1D *hNTracks = new TH1D("hNTracks","hNTracks",500,0,100);
    TH1D *hTrackE = new TH1D("hTrackE","hTrackE",500,0,2000);
    TH1D *hTrackPDG = new TH1D("hTrackPDG","hTrackPDG",500,-500,500);
    TH1D *hTrackMom  = new TH1D("hTrackMom","hTrackMom",500,0,1000);
    
    // From Sensitive detector
    TH1D *hEabsTot = new TH1D("hEabsTot","hEabsTot",500,0,150);
    TH1D *hEabsTarget = new TH1D("hEabsTarget","hEabsTarget",500,0,150);
    TH1D *hEabsTPCup = new TH1D("hEabsTPCup","hEabsTPCup",500,0,150);
    TH1D *hEabsTPCdown = new TH1D("hEabsTPCdown","hEabsTPCdown",500,0,150);
    
    TH1D *hReco_Mom = new TH1D("hReco_Mom","hReco_Mom",500,0,10000);
    TH1D *hReco_CosTheta = new TH1D("hReco_CosTheta","hReco_CosTheta",50,-1,+1);
    TH1D *hReco_PDG = new TH1D("hReco_PDG","hReco_PDG",800,-400,400);
    TH2D *hReco_CosThetaVsMom_Target = new TH2D("hReco_CosThetaVsMom_Target","hReco_CosThetaVsMom_Target",10,-1,1,20,0,10000);
    TH2D *hReco_CosThetaVsMom_Fin = new TH2D("hReco_CosThetaVsMom_Fin","hReco_CosThetaVsMom_Fin",10,-1,1,20,0,10000);
    TH2D *hReco_CosThetaVsMom_Eff; // = new TH2D("hReco_CosThetaVsMom_Eff","hReco_CosThetaVsMom_Eff",10,-1,1,20,0,10000);
    
    TH1D *hTPCUp_MomX = new TH1D("hTPCUp_MomX","hTPCUp_MomX",500,0,10000);
    TH1D *hTPCUp_MomY = new TH1D("hTPCUp_MomY","hTPCUp_MomY",500,0,10000);
    TH1D *hTPCUp_MomZ = new TH1D("hTPCUp_MomZ","hTPCUp_MomZ",500,0,10000);
    TH1D *hTPCUp_Mom = new TH1D("hTPCUp_Mom","hTPCUp_Mom",500,0,10000);
    TH1D *hTPCUp_Length = new TH1D("hTPCUp_Length","hTPCUp_Length",500,0,3000); // mm
    TH1D *hTPCUp_DeltaLyz = new TH1D("hTPCUp_DeltaLyz","hTPCUp_DeltaLyz",500,0,3000); // mm
    TH1D *hTPCUp_Edep = new TH1D("hTPCUp_Edep","hTPCUp_Edep",500,0,300);
    TH1D *hTPCUp_Charge = new TH1D("hTPCUp_Charge","hTPCUp_Charge",6,-3,3);
    TH1D *hTPCUp_PDG = new TH1D("hTPCUp_PDG","hTPCUp_PDG",800,-400,400);
    TH1D *hTPCUp_PosIniX = new TH1D("hTPCUp_PosIniX","hTPCUp_PosIniX",600,-3000,3000);
    TH1D *hTPCUp_PosIniY = new TH1D("hTPCUp_PosIniY","hTPCUp_PosIniY",600,-3000,3000);
    TH1D *hTPCUp_PosIniZ = new TH1D("hTPCUp_PosIniZ","hTPCUp_PosIniZ",600,-3000,3000);
    TH1D *hTPCUp_PosFinX = new TH1D("hTPCUp_PosFinX","hTPCUp_PosFinX",600,-3000,3000);
    TH1D *hTPCUp_PosFinY = new TH1D("hTPCUp_PosFinY","hTPCUp_PosFinY",600,-3000,3000);
    TH1D *hTPCUp_PosFinZ = new TH1D("hTPCUp_PosFinZ","hTPCUp_PosFinZ",600,-3000,3000);
    TH1D *hTPCUp_CosTheta = new TH1D("hTPCUp_CosTheta","hTPCUp_CosTheta",50,-1,1);
    
    TH1D *hTPCDown_MomX = new TH1D("hTPCDown_MomX","hTPCDown_MomX",500,0,10000);
    TH1D *hTPCDown_MomY = new TH1D("hTPCDown_MomY","hTPCDown_MomY",500,0,10000);
    TH1D *hTPCDown_MomZ = new TH1D("hTPCDown_MomZ","hTPCDown_MomZ",500,0,10000);
    TH1D *hTPCDown_Mom = new TH1D("hTPCDown_Mom","hTPCDown_Mom",500,0,10000);
    TH1D *hTPCDown_Length = new TH1D("hTPCDown_Length","hTPCDown_Length",500,0,3000); // mm
    TH1D *hTPCDown_DeltaLyz = new TH1D("hTPCDown_DeltaLyz","hTPCDown_DeltaLyz",500,0,3000); // mm
    TH1D *hTPCDown_Edep = new TH1D("hTPCDown_Edep","hTPCDown_Edep",500,0,300);
    TH1D *hTPCDown_Charge = new TH1D("hTPCDown_Charge","hTPCDown_Charge",6,-3,3);
    TH1D *hTPCDown_PDG = new TH1D("hTPCDown_PDG","hTPCDown_PDG",800,-400,400);
    TH1D *hTPCDown_PosIniX = new TH1D("hTPCDown_PosIniX","hTPCDown_PosIniX",600,-3000,3000);
    TH1D *hTPCDown_PosIniY = new TH1D("hTPCDown_PosIniY","hTPCDown_PosIniY",600,-3000,3000);
    TH1D *hTPCDown_PosIniZ = new TH1D("hTPCDown_PosIniZ","hTPCDown_PosIniZ",600,-3000,3000);
    TH1D *hTPCDown_PosFinX = new TH1D("hTPCDown_PosFinX","hTPCDown_PosFinX",600,-3000,3000);
    TH1D *hTPCDown_PosFinY = new TH1D("hTPCDown_PosFinY","hTPCDown_PosFinY",600,-3000,3000);
    TH1D *hTPCDown_PosFinZ = new TH1D("hTPCDown_PosFinZ","hTPCDown_PosFinZ",600,-3000,3000);
    TH1D *hTPCDown_CosTheta = new TH1D("hTPCDown_CosTheta","hTPCDown_CosTheta",50,-1,1);
    
    TH1D *hTarget_MomX = new TH1D("hTarget_MomX","hTarget_MomX",500,0,10000);
    TH1D *hTarget_MomY = new TH1D("hTarget_MomY","hTarget_MomY",500,0,10000);
    TH1D *hTarget_MomZ = new TH1D("hTarget_MomZ","hTarget_MomZ",500,0,10000);
    TH1D *hTarget_Mom = new TH1D("hTarget_Mom","hTarget_Mom",500,0,10000);
    TH1D *hTarget_Length = new TH1D("hTarget_Length","hTarget_Length",500,0,3000); // mm
    TH1D *hTarget_DeltaLyz = new TH1D("hTarget_DeltaLyz","hTarget_DeltaLyz",500,0,3000); // mm
    TH1D *hTarget_Edep = new TH1D("hTarget_Edep","hTarget_Edep",500,0,300);
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
    
    
    double pbins[37]= {0.,50.,100.,150.,200.,250.,300.,350.,400.,450.,500.,550.,600.,650.,700.,750.,800.,850.,900.,950.,1000.,1050.,1100.,1150.,1250.,1350.,1500.,1750.,2000.,2500.,3000.,4000.,5000.,7500.,10000.,20000.,30000.};
    double thetabins[27]={-1,-0.7,-0.5,-0.2,-0.1,0.,0.1,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.92,0.94,0.98,0.99,1};
    
    //Efficiencies
    //Denominator
    TH2D *Denom = new TH2D("Denom","Denom",26,thetabins,36,pbins);

    //Target only
    TH2D *Target_all = new TH2D("Target_all","Target_all",26,thetabins,36,pbins);

    //Target only
    TH2D *Target_only = new TH2D("Target_only","Target_only",26,thetabins,36,pbins);

    //Target+TPC
    TH2D *Target_TPC = new TH2D("Target_TPC","Target_TPC",26,thetabins,36,pbins);

    //TPC only
    TH2D *TPC_only = new TH2D("TPC_only","TPC_only",26,thetabins,36,pbins);
    
    //TPC only or Target only or Target + TPC
    TH2D *All_topology = new TH2D("All_topology","All_topology",26,thetabins,36,pbins);
    
    //TPC only or Target + TPC
    TH2D *All_topology2 = new TH2D("All_topology2","All_topology2",26,thetabins,36,pbins);
    
    std::vector<double> LastStepZdir;
    std::vector<double> LastStepYdir;
    
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
        TPCUp_High_ID = -9999;
        TPCUp_High_mom = -9999;
        TPCUp_High_costheta = -9999;
        TPCUp_High_len = -9999;
        TPCUp_High_dlyz = -9999;
        TPCUp_High_charge = -9999;
        TPCUp_High_edep = -9999;
        TPCUp_High_pdg = -9999;
        TPCUp_High_parent_pdg = -9999;
        TPCUp_High_PosIniX = -9999;
        TPCUp_High_PosIniY = -9999;
        TPCUp_High_PosIniZ = -9999;
        TPCUp_High_PosFinX = -9999;
        TPCUp_High_PosFinY = -9999;
        TPCUp_High_PosFinZ = -9999;
        TPCDown_High_ID = -9999;
        TPCDown_High_mom = -9999;
        TPCDown_High_costheta = -9999;
        TPCDown_High_len = -9999;
        TPCDown_High_dlyz = -9999;
        TPCDown_High_charge = -9999;
        TPCDown_High_edep = -9999;
        TPCDown_High_pdg = -9999;
        TPCDown_High_parent_pdg = -9999;
        TPCDown_High_PosIniX = -9999;
        TPCDown_High_PosIniY = -9999;
        TPCDown_High_PosIniZ = -9999;
        TPCDown_High_PosFinX = -9999;
        TPCDown_High_PosFinY = -9999;
        TPCDown_High_PosFinZ = -9999;
        Target_High_ID = -9999;
        Target_High_mom = -9999;
        Target_High_costheta = -9999;
        Target_High_len = -9999;
        Target_High_dlyz = -9999;
        Target_High_charge = -9999;
        Target_High_edep = -9999;
        Target_High_pdg = -9999;
        Target_High_parent_pdg = -9999;
        Target_High_PosIniX = -9999;
        Target_High_PosIniY = -9999;
        Target_High_PosIniZ = -9999;
        Target_High_PosFinX = -9999;
        Target_High_PosFinY = -9999;
        Target_High_PosFinZ = -9999;
        
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
            
            int vecel=-999,id=-999;
            double costheta=-999,momX=-999,momY=-999,momZ=-999,mom=-999,length=-999,deltaLyz=-999,edep=-999,charge=-999,pdg=-999,posX=-999,posY=-999,posZ=-999,posLastX=-999,posLastY=-999,posLastZ=-999;
            int vecel_up=-999,id_up;
            double length_up=-999,deltaLyz_up=-999,edep_up=-999,charge_up=-999,pdg_up=-999;
            int vecel_down=-999,id_down;
            double length_down=-999,deltaLyz_down=-999,edep_down=-999,charge_down=-999,pdg_down=-999;
            
            
            //
            // Check if reconstructed in Target
            //
            double check = 0;
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
             
            	 // Fill the denominator
            	 if ( (doSelPDG && pdg==SetPDG) ){
					 Denom->Fill(costheta, mom);
					 check = 1;
				 }
		 	 }
			
             vecel_up = FindTrackElem(VecTPCUp_TrackID,trkid);
             if(vecel_up>=0){
                 id_up = VecTPCUp_TrackID->at(vecel_up);
                 deltaLyz_up = VecTPCUp_TrackDeltaLyz->at(vecel_up);
                 length_up = VecTPCUp_TrackLength->at(vecel_up);
                 edep_up = VecTPCUp_TrackEdep->at(vecel_up);
                 charge_up = VecTPCUp_TrackCharge->at(vecel_up);
                 pdg_up = VecTPCUp_TrackPDG->at(vecel_up);
            	 if (check == 0 && (doSelPDG && pdg==SetPDG) ){
					 Denom->Fill(costheta, mom);
					 check = 1;
				 }
             }
             
             vecel_down = FindTrackElem(VecTPCDown_TrackID,trkid);
             if(vecel_down>=0){
                 id_down = VecTPCDown_TrackID->at(vecel_down);
                 deltaLyz_down = VecTPCDown_TrackDeltaLyz->at(vecel_down);
                 length_down = VecTPCDown_TrackLength->at(vecel_down);
                 edep_down = VecTPCDown_TrackEdep->at(vecel_down);
                 charge_down = VecTPCDown_TrackCharge->at(vecel_down);
                 pdg_down = VecTPCDown_TrackPDG->at(vecel_down);
            	 if (check == 0 && (doSelPDG && pdg==SetPDG) ){
					 Denom->Fill(costheta, mom);
					 check = 1;
				 }
             }
   
             // Cut 1: Select track PDG
             // Cut 2: Select tracks with dLyz > 50mm
             // Cut 3: Select charged particles
             if(vecel >= 0 && (doSelPDG && pdg==SetPDG) && (doCut_dL && length > len_target_min) && (fabs(charge)==cut_charge)){
                              
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
                 // Check if reconstructed in TPCUp or TPC Down
                 //
                 
                 // Cut 2: Select tracks with dLyz > 200mm
                 // Cut 3: Select charged particles
                 if(vecel_up >= 0 && (doCut_dL && deltaLyz_up > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_up)==cut_charge)){
                 
                     Target_TPC->Fill(costheta, mom);
                     All_topology->Fill(costheta, mom);
                     All_topology2->Fill(costheta, mom);
	                 Target_all->Fill(costheta, mom);
					 continue;
                     if(doPrintTrack){
                         cout << "     - TPCUp track with Target: " << " ";
                         cout << "event = " << ientry << ", ";
                         cout << "mom = " << mom << ", ";
                         cout << "deltaLyz = " << deltaLyz_up << ", ";
                         cout << "charge = " << charge_up << ", ";
                         cout << "edep = " << edep_up << ", ";
                         cout << "pdg = " << pdg_up << ", ";
                         cout << endl;
                     }
                 } // reco in TPCUp
                 else if(vecel_down >= 0 && (doCut_dL && deltaLyz_down > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_down)==cut_charge)){

                     Target_TPC->Fill(costheta, mom);
                     All_topology->Fill(costheta, mom);
                     All_topology2->Fill(costheta, mom);
	                 Target_all->Fill(costheta, mom);
					 continue;
                     if(doPrintTrack){
                         cout << "     - TPCDown track with Target: " << " ";
                         cout << "event = " << ientry << ", ";
                         cout << "mom = " << mom << ", ";
                         cout << "deltaLyz = " << deltaLyz_down << ", ";
                         cout << "charge = " << charge_down << ", ";
                         cout << "edep = " << edep_down << ", ";
                         cout << "pdg = " << pdg_down << ", ";
                         cout << endl;
                     }
                    
                 } // reco in TPCs
                 
                 Target_all->Fill(costheta, mom);
                 Target_only->Fill(costheta, mom);
                 All_topology->Fill(costheta, mom);
				 continue;
            } // in the Target
             // Cut 2: Select tracks with dLyz > 200mm
             // Cut 3: Select charged particles
             else if(vecel_up >= 0 &&  (doSelPDG && pdg==SetPDG) && (doCut_dL && deltaLyz_up > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_up)==cut_charge)){
                 
                 TPC_only->Fill(costheta, mom);
                 All_topology->Fill(costheta, mom);
                 All_topology2->Fill(costheta, mom);
				 continue;
                 
                 if(doPrintTrack){
                     cout << "     - TPCUp track: " << " ";
                     cout << "event = " << ientry << ", ";
                     cout << "mom = " << mom << ", ";
                     cout << "deltaLyz = " << deltaLyz_up << ", ";
                     cout << "charge = " << charge_up << ", ";
                     cout << "edep = " << edep_up << ", ";
                     cout << "pdg = " << pdg_up << ", ";
                     cout << endl;
                 }
             } // reco in TPCUp
             else if(vecel_down >= 0 &&  (doSelPDG && pdg==SetPDG) && (doCut_dL && deltaLyz_down > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_down)==cut_charge)){
                 
                 TPC_only->Fill(costheta, mom);
                 All_topology->Fill(costheta, mom);
                 All_topology2->Fill(costheta, mom);
				 continue;
                 
                 if(doPrintTrack){
                     cout << "     - TPCDown track: " << " ";
                     cout << "event = " << ientry << ", ";
                     cout << "mom = " << mom << ", ";
                     cout << "deltaLyz = " << deltaLyz_down << ", ";
                     cout << "charge = " << charge_down << ", ";
                     cout << "edep = " << edep_down << ", ";
                     cout << "pdg = " << pdg_down << ", ";
                     cout << endl;
                 }
                 
             } // reco in TPCDown
                    
        } // end loop over all tracks
        
    } // end loop over events
    
    
    // Store the Tree
    treeoa->Write();
    fileoa->Close();
    
    
    // Get the efficiency
    hReco_CosThetaVsMom_Eff = (TH2D*)hReco_CosThetaVsMom_Fin->Clone("hReco_CosThetaVsMom_Eff");
    hReco_CosThetaVsMom_Eff->Divide(hReco_CosThetaVsMom_Target);
    
    
    
    // Draw histograms
    
    TCanvas *Target_only_c = new TCanvas("Target_only_c","Target_only_c");
    Target_only->Divide(Denom);
    Target_only->Draw("COLZ");
    TCanvas *Target_TPC_c = new TCanvas("Target_TPC_c","Target_TPC_c");
    Target_TPC->Divide(Denom);
    Target_TPC->Draw("COLZ");
    TCanvas *TPC_only_c = new TCanvas("TPC_only_c","TPC_only_c");
    TPC_only->Divide(Denom);
    TPC_only->Draw("COLZ");
    TCanvas *All_topology_c = new TCanvas("All_topology_c","All_topology_c");
    All_topology->Divide(Denom);
    All_topology->Draw("COLZ");
    TCanvas *All_topology2_c = new TCanvas("All_topology2_c","All_topology2_c");
    All_topology2->Divide(Denom);
    All_topology2->Draw("COLZ");
    TCanvas *Target_all_c = new TCanvas("Target_all_c","Target_all_c");
    Target_all->Divide(Denom);
    Target_all->Draw("COLZ");
    // Print 
    
    // Store plots
    
    char name[300];
    if(doprint){    

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
