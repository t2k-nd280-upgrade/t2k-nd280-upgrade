
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

void SkimNtuple()
{
    const char *tag = "prova";
    //const char *filename = "../bin/EffStudy_highstat.root";
    const char *filename = "../bin/EffStudy.root";
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
    
    const int doPrintTrack = true;
    
    ////////////////////////
    
    
    // True tracks
    TH1D *hNTracks = new TH1D("hNTracks","hNTracks",500,0,100);
    TH1D *hTrackE = new TH1D("hTrackE","hTrackE",500,0,2000);
    TH1D *hTrackPDG = new TH1D("hTrackPDG","hTrackPDG",500,-500,500);
    TH1D *hTrackMom  = new TH1D("hTrackMom","hTrackMom",500,0,1000);
    
    // From Sensitive detector
    TH1D *hEabsTot = new TH1D("hEabsTot","hEabsTot",500,0,150);
    TH1D *hEabsTarget1 = new TH1D("hEabsTarget1","hEabsTarget1",500,0,150);
    TH1D *hEabsTarget2 = new TH1D("hEabsTarget2","hEabsTarget2",500,0,150);
    TH1D *hEabsTPCup1 = new TH1D("hEabsTPCup1","hEabsTPCup1",500,0,150);
    TH1D *hEabsTPCup2 = new TH1D("hEabsTPCup2","hEabsTPCup2",500,0,150);
    TH1D *hEabsTPCdown1 = new TH1D("hEabsTPCdown1","hEabsTPCdown1",500,0,150);
    TH1D *hEabsTPCdown2 = new TH1D("hEabsTPCdown2","hEabsTPCdown2",500,0,150);
    
    TH1D *hReco_Mom = new TH1D("hReco_Mom","hReco_Mom",500,0,10000);
    TH1D *hReco_CosTheta = new TH1D("hReco_CosTheta","hReco_CosTheta",50,-1,+1);
    TH1D *hReco_PDG = new TH1D("hReco_PDG","hReco_PDG",800,-400,400);
    TH2D *hReco_CosThetaVsMom_Target = new TH2D("hReco_CosThetaVsMom_Target","hReco_CosThetaVsMom_Target",10,-1,1,20,0,10000);
    TH2D *hReco_CosThetaVsMom_Fin = new TH2D("hReco_CosThetaVsMom_Fin","hReco_CosThetaVsMom_Fin",10,-1,1,20,0,10000);
    TH2D *hReco_CosThetaVsMom_Eff; // = new TH2D("hReco_CosThetaVsMom_Eff","hReco_CosThetaVsMom_Eff",10,-1,1,20,0,10000);
    
    TH1D *hTPCUp1_MomX = new TH1D("hTPCUp1_MomX","hTPCUp1_MomX",500,0,10000);
    TH1D *hTPCUp1_MomY = new TH1D("hTPCUp1_MomY","hTPCUp1_MomY",500,0,10000);
    TH1D *hTPCUp1_MomZ = new TH1D("hTPCUp1_MomZ","hTPCUp1_MomZ",500,0,10000);
    TH1D *hTPCUp1_Mom = new TH1D("hTPCUp1_Mom","hTPCUp1_Mom",500,0,10000);
    TH1D *hTPCUp1_Length = new TH1D("hTPCUp1_Length","hTPCUp1_Length",500,0,3000); // mm
    TH1D *hTPCUp1_DeltaLyz = new TH1D("hTPCUp1_DeltaLyz","hTPCUp1_DeltaLyz",500,0,3000); // mm
    TH1D *hTPCUp1_Edep = new TH1D("hTPCUp1_Edep","hTPCUp1_Edep",500,0,300);
    TH1D *hTPCUp1_Charge = new TH1D("hTPCUp1_Charge","hTPCUp1_Charge",6,-3,3);
    TH1D *hTPCUp1_PDG = new TH1D("hTPCUp1_PDG","hTPCUp1_PDG",800,-400,400);
    TH1D *hTPCUp1_PosIniX = new TH1D("hTPCUp1_PosIniX","hTPCUp1_PosIniX",600,-3000,3000);
    TH1D *hTPCUp1_PosIniY = new TH1D("hTPCUp1_PosIniY","hTPCUp1_PosIniY",600,-3000,3000);
    TH1D *hTPCUp1_PosIniZ = new TH1D("hTPCUp1_PosIniZ","hTPCUp1_PosIniZ",600,-3000,3000);
    TH1D *hTPCUp1_PosFinX = new TH1D("hTPCUp1_PosFinX","hTPCUp1_PosFinX",600,-3000,3000);
    TH1D *hTPCUp1_PosFinY = new TH1D("hTPCUp1_PosFinY","hTPCUp1_PosFinY",600,-3000,3000);
    TH1D *hTPCUp1_PosFinZ = new TH1D("hTPCUp1_PosFinZ","hTPCUp1_PosFinZ",600,-3000,3000);
    TH1D *hTPCUp1_CosTheta = new TH1D("hTPCUp1_CosTheta","hTPCUp1_CosTheta",50,-1,1);

    TH1D *hTPCUp2_MomX = new TH1D("hTPCUp2_MomX","hTPCUp2_MomX",500,0,10000);
    TH1D *hTPCUp2_MomY = new TH1D("hTPCUp2_MomY","hTPCUp2_MomY",500,0,10000);
    TH1D *hTPCUp2_MomZ = new TH1D("hTPCUp2_MomZ","hTPCUp2_MomZ",500,0,10000);
    TH1D *hTPCUp2_Mom = new TH1D("hTPCUp2_Mom","hTPCUp2_Mom",500,0,10000);
    TH1D *hTPCUp2_Length = new TH1D("hTPCUp2_Length","hTPCUp2_Length",500,0,3000); // mm
    TH1D *hTPCUp2_DeltaLyz = new TH1D("hTPCUp2_DeltaLyz","hTPCUp2_DeltaLyz",500,0,3000); // mm
    TH1D *hTPCUp2_Edep = new TH1D("hTPCUp2_Edep","hTPCUp2_Edep",500,0,300);
    TH1D *hTPCUp2_Charge = new TH1D("hTPCUp2_Charge","hTPCUp2_Charge",6,-3,3);
    TH1D *hTPCUp2_PDG = new TH1D("hTPCUp2_PDG","hTPCUp2_PDG",800,-400,400);
    TH1D *hTPCUp2_PosIniX = new TH1D("hTPCUp2_PosIniX","hTPCUp2_PosIniX",600,-3000,3000);
    TH1D *hTPCUp2_PosIniY = new TH1D("hTPCUp2_PosIniY","hTPCUp2_PosIniY",600,-3000,3000);
    TH1D *hTPCUp2_PosIniZ = new TH1D("hTPCUp2_PosIniZ","hTPCUp2_PosIniZ",600,-3000,3000);
    TH1D *hTPCUp2_PosFinX = new TH1D("hTPCUp2_PosFinX","hTPCUp2_PosFinX",600,-3000,3000);
    TH1D *hTPCUp2_PosFinY = new TH1D("hTPCUp2_PosFinY","hTPCUp2_PosFinY",600,-3000,3000);
    TH1D *hTPCUp2_PosFinZ = new TH1D("hTPCUp2_PosFinZ","hTPCUp2_PosFinZ",600,-3000,3000);
    TH1D *hTPCUp2_CosTheta = new TH1D("hTPCUp2_CosTheta","hTPCUp2_CosTheta",50,-1,1);
    
    TH1D *hTPCDown1_MomX = new TH1D("hTPCDown1_MomX","hTPCDown1_MomX",500,0,10000);
    TH1D *hTPCDown1_MomY = new TH1D("hTPCDown1_MomY","hTPCDown1_MomY",500,0,10000);
    TH1D *hTPCDown1_MomZ = new TH1D("hTPCDown1_MomZ","hTPCDown1_MomZ",500,0,10000);
    TH1D *hTPCDown1_Mom = new TH1D("hTPCDown1_Mom","hTPCDown1_Mom",500,0,10000);
    TH1D *hTPCDown1_Length = new TH1D("hTPCDown1_Length","hTPCDown1_Length",500,0,3000); // mm
    TH1D *hTPCDown1_DeltaLyz = new TH1D("hTPCDown1_DeltaLyz","hTPCDown1_DeltaLyz",500,0,3000); // mm
    TH1D *hTPCDown1_Edep = new TH1D("hTPCDown1_Edep","hTPCDown1_Edep",500,0,300);
    TH1D *hTPCDown1_Charge = new TH1D("hTPCDown1_Charge","hTPCDown1_Charge",6,-3,3);
    TH1D *hTPCDown1_PDG = new TH1D("hTPCDown1_PDG","hTPCDown1_PDG",800,-400,400);
    TH1D *hTPCDown1_PosIniX = new TH1D("hTPCDown1_PosIniX","hTPCDown1_PosIniX",600,-3000,3000);
    TH1D *hTPCDown1_PosIniY = new TH1D("hTPCDown1_PosIniY","hTPCDown1_PosIniY",600,-3000,3000);
    TH1D *hTPCDown1_PosIniZ = new TH1D("hTPCDown1_PosIniZ","hTPCDown1_PosIniZ",600,-3000,3000);
    TH1D *hTPCDown1_PosFinX = new TH1D("hTPCDown1_PosFinX","hTPCDown1_PosFinX",600,-3000,3000);
    TH1D *hTPCDown1_PosFinY = new TH1D("hTPCDown1_PosFinY","hTPCDown1_PosFinY",600,-3000,3000);
    TH1D *hTPCDown1_PosFinZ = new TH1D("hTPCDown1_PosFinZ","hTPCDown1_PosFinZ",600,-3000,3000);
    TH1D *hTPCDown1_CosTheta = new TH1D("hTPCDown1_CosTheta","hTPCDown1_CosTheta",50,-1,1);
 
    TH1D *hTPCDown2_MomX = new TH1D("hTPCDown2_MomX","hTPCDown2_MomX",500,0,10000);
    TH1D *hTPCDown2_MomY = new TH1D("hTPCDown2_MomY","hTPCDown2_MomY",500,0,10000);
    TH1D *hTPCDown2_MomZ = new TH1D("hTPCDown2_MomZ","hTPCDown2_MomZ",500,0,10000);
    TH1D *hTPCDown2_Mom = new TH1D("hTPCDown2_Mom","hTPCDown2_Mom",500,0,10000);
    TH1D *hTPCDown2_Length = new TH1D("hTPCDown2_Length","hTPCDown2_Length",500,0,3000); // mm
    TH1D *hTPCDown2_DeltaLyz = new TH1D("hTPCDown2_DeltaLyz","hTPCDown2_DeltaLyz",500,0,3000); // mm
    TH1D *hTPCDown2_Edep = new TH1D("hTPCDown2_Edep","hTPCDown2_Edep",500,0,300);
    TH1D *hTPCDown2_Charge = new TH1D("hTPCDown2_Charge","hTPCDown2_Charge",6,-3,3);
    TH1D *hTPCDown2_PDG = new TH1D("hTPCDown2_PDG","hTPCDown2_PDG",800,-400,400);
    TH1D *hTPCDown2_PosIniX = new TH1D("hTPCDown2_PosIniX","hTPCDown2_PosIniX",600,-3000,3000);
    TH1D *hTPCDown2_PosIniY = new TH1D("hTPCDown2_PosIniY","hTPCDown2_PosIniY",600,-3000,3000);
    TH1D *hTPCDown2_PosIniZ = new TH1D("hTPCDown2_PosIniZ","hTPCDown2_PosIniZ",600,-3000,3000);
    TH1D *hTPCDown2_PosFinX = new TH1D("hTPCDown2_PosFinX","hTPCDown2_PosFinX",600,-3000,3000);
    TH1D *hTPCDown2_PosFinY = new TH1D("hTPCDown2_PosFinY","hTPCDown2_PosFinY",600,-3000,3000);
    TH1D *hTPCDown2_PosFinZ = new TH1D("hTPCDown2_PosFinZ","hTPCDown2_PosFinZ",600,-3000,3000);
    TH1D *hTPCDown2_CosTheta = new TH1D("hTPCDown2_CosTheta","hTPCDown2_CosTheta",50,-1,1);
    
    TH1D *hTarget1_MomX = new TH1D("hTarget1_MomX","hTarget1_MomX",500,0,10000);
    TH1D *hTarget1_MomY = new TH1D("hTarget1_MomY","hTarget1_MomY",500,0,10000);
    TH1D *hTarget1_MomZ = new TH1D("hTarget1_MomZ","hTarget1_MomZ",500,0,10000);
    TH1D *hTarget1_Mom = new TH1D("hTarget1_Mom","hTarget1_Mom",500,0,10000);
    TH1D *hTarget1_Length = new TH1D("hTarget1_Length","hTarget1_Length",500,0,3000); // mm
    TH1D *hTarget1_DeltaLyz = new TH1D("hTarget1_DeltaLyz","hTarget1_DeltaLyz",500,0,3000); // mm
    TH1D *hTarget1_Edep = new TH1D("hTarget1_Edep","hTarget1_Edep",500,0,300);
    TH1D *hTarget1_Charge = new TH1D("hTarget1_Charge","hTarget1_Charge",6,-3,3);
    TH1D *hTarget1_PDG = new TH1D("hTarget1_PDG","hTarget1_PDG",800,-400,400);
    TH1D *hTarget1_PosIniX = new TH1D("hTarget1_PosIniX","hTarget1_PosIniX",600,-3000,3000);
    TH1D *hTarget1_PosIniY = new TH1D("hTarget1_PosIniY","hTarget1_PosIniY",600,-300,300);
    TH1D *hTarget1_PosIniZ = new TH1D("hTarget1_PosIniZ","hTarget1_PosIniZ",600,-3000,3000);
    TH1D *hTarget1_PosFinX = new TH1D("hTarget1_PosFinX","hTarget1_PosFinX",600,-3000,3000);
    TH1D *hTarget1_PosFinY = new TH1D("hTarget1_PosFinY","hTarget1_PosFinY",600,-300,300);
    TH1D *hTarget1_PosFinZ = new TH1D("hTarget1_PosFinZ","hTarget1_PosFinZ",600,-3000,3000);
    TH1D *hTarget1_CosTheta = new TH1D("hTarget1_CosTheta","hTarget1_CosTheta",50,-1,1);

    TH1D *hTarget2_MomX = new TH1D("hTarget2_MomX","hTarget2_MomX",500,0,10000);
    TH1D *hTarget2_MomY = new TH1D("hTarget2_MomY","hTarget2_MomY",500,0,10000);
    TH1D *hTarget2_MomZ = new TH1D("hTarget2_MomZ","hTarget2_MomZ",500,0,10000);
    TH1D *hTarget2_Mom = new TH1D("hTarget2_Mom","hTarget2_Mom",500,0,10000);
    TH1D *hTarget2_Length = new TH1D("hTarget2_Length","hTarget2_Length",500,0,3000); // mm
    TH1D *hTarget2_DeltaLyz = new TH1D("hTarget2_DeltaLyz","hTarget2_DeltaLyz",500,0,3000); // mm
    TH1D *hTarget2_Edep = new TH1D("hTarget2_Edep","hTarget2_Edep",500,0,300);
    TH1D *hTarget2_Charge = new TH1D("hTarget2_Charge","hTarget2_Charge",6,-3,3);
    TH1D *hTarget2_PDG = new TH1D("hTarget2_PDG","hTarget2_PDG",800,-400,400);
    TH1D *hTarget2_PosIniX = new TH1D("hTarget2_PosIniX","hTarget2_PosIniX",600,-3000,3000);
    TH1D *hTarget2_PosIniY = new TH1D("hTarget2_PosIniY","hTarget2_PosIniY",600,-300,300);
    TH1D *hTarget2_PosIniZ = new TH1D("hTarget2_PosIniZ","hTarget2_PosIniZ",600,-3000,3000);
    TH1D *hTarget2_PosFinX = new TH1D("hTarget2_PosFinX","hTarget2_PosFinX",600,-3000,3000);
    TH1D *hTarget2_PosFinY = new TH1D("hTarget2_PosFinY","hTarget2_PosFinY",600,-300,300);
    TH1D *hTarget2_PosFinZ = new TH1D("hTarget2_PosFinZ","hTarget2_PosFinZ",600,-3000,3000);
    TH1D *hTarget2_CosTheta = new TH1D("hTarget2_CosTheta","hTarget2_CosTheta",50,-1,1);
    
    // Vertex
    TH2D *hVtx_XY = new TH2D("hVtx_XY","hVtx_XY",6000,-3000,3000,600,-300,300); // mm
    TH2D *hVtx_XZ = new TH2D("hVtx_XZ","hVtx_XZ",6000,-6000,6000,600,-300,300); // mm
    TH2D *hVtx_YZ = new TH2D("hVtx_YZ","hVtx_YZ",6000,-6000,6000,600,-300,300); // mm
    TH1D *hVtx_X = new TH1D("hVtx_X","hVtx_X",6000,-3000,3000); // mm
    TH1D *hVtx_Y = new TH1D("hVtx_Y","hVtx_Y",6000,-300,300); // mm
    TH1D *hVtx_Z = new TH1D("hVtx_Z","hVtx_Z",6000,-3000,3000); // mm
    
    // First Step position Vs Vertex position
    TH1D *hVtxMinusPosIni_Target1_X = new TH1D("hVtxMinusPosIni_Target1_X","hVtxMinusPosIni_Target1_X",6000,-3000,3000);
    TH1D *hVtxMinusPosIni_Target1_Y = new TH1D("hVtxMinusPosIni_Target1_Y","hVtxMinusPosIni_Target1_Y",6000,-3000,3000);
    TH1D *hVtxMinusPosIni_Target1_Z = new TH1D("hVtxMinusPosIni_Target1_Z","hVtxMinusPosIni_Target1_Z",6000,-3000,3000);
    
    TH1D *hVtxMinusPosIni_Target2_X = new TH1D("hVtxMinusPosIni_Target2_X","hVtxMinusPosIni_Target2_X",6000,-3000,3000);
    TH1D *hVtxMinusPosIni_Target2_Y = new TH1D("hVtxMinusPosIni_Target2_Y","hVtxMinusPosIni_Target2_Y",6000,-3000,3000);
    TH1D *hVtxMinusPosIni_Target2_Z = new TH1D("hVtxMinusPosIni_Target2_Z","hVtxMinusPosIni_Target2_Z",6000,-3000,3000);
    
    std::vector<double> LastStepZdir;
    std::vector<double> LastStepYdir;
    
    cout << endl;
    cout << "Reading file:" << endl;
    cout << filename << endl;
    cout << endl;
    
    
    TFile *filein = new TFile(filename,"READ");
    TTree *treein = (TTree*)filein->Get("EffStudy");
    
    
    // Set ntuple branch addresses
    
    double EabsTarget1=0.; // Energy deposited
    double EabsTarget2=0.; // Energy deposited
    double EabsForwTPC1=0.;
    double EabsTPCup1=0.;
    double EabsTPCup2=0.;
    double EabsTPCdown1=0.;
    double EabsTPCdown2=0.;
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

    // Forward TPC 1 informations
    int     ForwTPC1_NTracks;          // # of tracks in Forward TPC 1
    std::vector<int>   * VecForwTPC1_TrackID = 0; // Vector of trackID in Forw TPC 1
    std::vector<int>   * VecForwTPC1_ParentID = 0; // Vector of parentID in Forw TPC 1
    std::vector<double>* VecForwTPC1_TrackMomX = 0; // Vector of initial mom in Forw TPC 1 (MeV)
    std::vector<double>* VecForwTPC1_TrackMomY = 0; // Vector of initial mom in Forw TPC 1 (MeV)
    std::vector<double>* VecForwTPC1_TrackMomZ = 0; // Vector of initial mom in Forw TPC 1 (MeV)
    std::vector<double>* VecForwTPC1_TrackLength = 0; // Vector of corrected (mult. scatt.) length in Forw TPC 1 (mm)
    std::vector<double>* VecForwTPC1_TrackDeltaLyz = 0; // Vector of geometrical length yz in Forw TPC 1 (mm)
    std::vector<double>* VecForwTPC1_TrackEdep = 0; // Vector of edep in Forw TPC 1 (MeV)
    std::vector<double>* VecForwTPC1_TrackCharge = 0; // Vector of charge in Fow TPC 1 (e-)
    std::vector<int>   * VecForwTPC1_TrackPDG = 0; // Vector of pdg in Forw TPC 1
    std::vector<double> *VecForwTPC1_TrackStepFirstX = 0; // Vector of first step position in Forw TPC 1
    std::vector<double> *VecForwTPC1_TrackStepFirstY = 0; // Vector of first step position in Forw TPC 1
    std::vector<double> *VecForwTPC1_TrackStepFirstZ = 0; // Vector of first step position in Forw TPC 1
    std::vector<double> *VecForwTPC1_TrackStepLastX = 0; // Vector of last step position in Forw TPC 1
    std::vector<double> *VecForwTPC1_TrackStepLastY = 0; // Vector of last step position in Forw TPC 1
    std::vector<double> *VecForwTPC1_TrackStepLastZ = 0; // Vector of last step position in Forw TPC 1
    std::vector<double> *VecForwTPC1_TrackCosTheta = 0; // Vector of cos(theta) wrt nu direction (z axis) in Forw TPC 1
    
    // TPC up 1 informations
    int     TPCUp1_NTracks;          // # of tracks in TPC Up
    std::vector<int>   * VecTPCUp1_TrackID = 0; // Vector of trackID in TPCup
    std::vector<int>   * VecTPCUp1_ParentID = 0; // Vector of parentID in TPCup
    std::vector<double>* VecTPCUp1_TrackMomX = 0; // Vector of initial mom in TPC Up (MeV)
    std::vector<double>* VecTPCUp1_TrackMomY = 0; // Vector of initial mom in TPC Up (MeV)
    std::vector<double>* VecTPCUp1_TrackMomZ = 0; // Vector of initial mom in TPC Up (MeV)
    std::vector<double>* VecTPCUp1_TrackLength = 0; // Vector of corrected (mult. scatt.) length in TPC Up (mm)
    std::vector<double>* VecTPCUp1_TrackDeltaLyz = 0; // Vector of geometrical length yz in TPC Up (mm)
    std::vector<double>* VecTPCUp1_TrackEdep = 0; // Vector of edep in TPC Up (MeV)
    std::vector<double>* VecTPCUp1_TrackCharge = 0; // Vector of charge in TPC Up (e-)
    std::vector<int>   * VecTPCUp1_TrackPDG = 0; // Vector of pdg in TPC Up
    std::vector<double> *VecTPCUp1_TrackStepFirstX = 0; // Vector of first step position in TPC Up
    std::vector<double> *VecTPCUp1_TrackStepFirstY = 0; // Vector of first step position in TPC Up
    std::vector<double> *VecTPCUp1_TrackStepFirstZ = 0; // Vector of first step position in TPC Up
    std::vector<double> *VecTPCUp1_TrackStepLastX = 0; // Vector of last step position in TPC Up
    std::vector<double> *VecTPCUp1_TrackStepLastY = 0; // Vector of last step position in TPC Up
    std::vector<double> *VecTPCUp1_TrackStepLastZ = 0; // Vector of last step position in TPC Up
    std::vector<double> *VecTPCUp1_TrackCosTheta = 0; // Vector of cos(theta) wrt nu direction (z axis) in TPC Up

    // TPC up 2 informations
    int     TPCUp2_NTracks;          // # of tracks in TPC Up
    std::vector<int>   * VecTPCUp2_TrackID = 0; // Vector of trackID in TPCup
    std::vector<int>   * VecTPCUp2_ParentID = 0; // Vector of parentID in TPCup
    std::vector<double>* VecTPCUp2_TrackMomX = 0; // Vector of initial mom in TPC Up (MeV)
    std::vector<double>* VecTPCUp2_TrackMomY = 0; // Vector of initial mom in TPC Up (MeV)
    std::vector<double>* VecTPCUp2_TrackMomZ = 0; // Vector of initial mom in TPC Up (MeV)
    std::vector<double>* VecTPCUp2_TrackLength = 0; // Vector of corrected (mult. scatt.) length in TPC Up (mm)
    std::vector<double>* VecTPCUp2_TrackDeltaLyz = 0; // Vector of geometrical length yz in TPC Up (mm)
    std::vector<double>* VecTPCUp2_TrackEdep = 0; // Vector of edep in TPC Up (MeV)
    std::vector<double>* VecTPCUp2_TrackCharge = 0; // Vector of charge in TPC Up (e-)
    std::vector<int>   * VecTPCUp2_TrackPDG = 0; // Vector of pdg in TPC Up
    std::vector<double> *VecTPCUp2_TrackStepFirstX = 0; // Vector of first step position in TPC Up
    std::vector<double> *VecTPCUp2_TrackStepFirstY = 0; // Vector of first step position in TPC Up
    std::vector<double> *VecTPCUp2_TrackStepFirstZ = 0; // Vector of first step position in TPC Up
    std::vector<double> *VecTPCUp2_TrackStepLastX = 0; // Vector of last step position in TPC Up
    std::vector<double> *VecTPCUp2_TrackStepLastY = 0; // Vector of last step position in TPC Up
    std::vector<double> *VecTPCUp2_TrackStepLastZ = 0; // Vector of last step position in TPC Up
    std::vector<double> *VecTPCUp2_TrackCosTheta = 0; // Vector of cos(theta) wrt nu direction (z axis) in TPC Up
    
    // TPC down 1 informations
    int     TPCDown1_NTracks = 0;          // # of tracks in TPC Down
    std::vector<int>   * VecTPCDown1_TrackID = 0; // Vector of trackID in TPCDown
    std::vector<int>   * VecTPCDown1_ParentID = 0; // Vector of parentID in TPCDown
    std::vector<double>* VecTPCDown1_TrackMomX = 0; // Vector of initial mom in TPC Down (MeV)
    std::vector<double>* VecTPCDown1_TrackMomY = 0; // Vector of initial mom in TPC Down (MeV)
    std::vector<double>* VecTPCDown1_TrackMomZ = 0; // Vector of initial mom in TPC Down (MeV)
    std::vector<double>* VecTPCDown1_TrackLength = 0; // Vector of corrected (mult.scatt.) length in TPC Down (mm)
    std::vector<double>* VecTPCDown1_TrackDeltaLyz = 0; // Vector of geometrical length yz in TPC Down (mm)
    std::vector<double>* VecTPCDown1_TrackEdep = 0; // Vector of edep in TPC Down (MeV)
    std::vector<double>* VecTPCDown1_TrackCharge = 0; // Vector of charge in TPC Down (e-)
    std::vector<int>   * VecTPCDown1_TrackPDG = 0; // Vector of pdg in TPC Down
    std::vector<double> *VecTPCDown1_TrackStepFirstX = 0; // Vector of first step position in TPC Down
    std::vector<double> *VecTPCDown1_TrackStepFirstY = 0; // Vector of first step position in TPC Down
    std::vector<double> *VecTPCDown1_TrackStepFirstZ = 0; // Vector of first step position in TPC Down
    std::vector<double> *VecTPCDown1_TrackStepLastX = 0; // Vector of last step position in TPC Down
    std::vector<double> *VecTPCDown1_TrackStepLastY = 0; // Vector of last step position in TPC Down
    std::vector<double> *VecTPCDown1_TrackStepLastZ = 0; // Vector of last step position in TPC Down
    std::vector<double> *VecTPCDown1_TrackCosTheta = 0; // Vector of cos(theta) wrt nu direction (z axis) in TPC Down

    // TPC down 2 informations
    int     TPCDown2_NTracks = 0;          // # of tracks in TPC Down
    std::vector<int>   * VecTPCDown2_TrackID = 0; // Vector of trackID in TPCDown
    std::vector<int>   * VecTPCDown2_ParentID = 0; // Vector of parentID in TPCDown
    std::vector<double>* VecTPCDown2_TrackMomX = 0; // Vector of initial mom in TPC Down (MeV)
    std::vector<double>* VecTPCDown2_TrackMomY = 0; // Vector of initial mom in TPC Down (MeV)
    std::vector<double>* VecTPCDown2_TrackMomZ = 0; // Vector of initial mom in TPC Down (MeV)
    std::vector<double>* VecTPCDown2_TrackLength = 0; // Vector of corrected (mult.scatt.) length in TPC Down (mm)
    std::vector<double>* VecTPCDown2_TrackDeltaLyz = 0; // Vector of geometrical length yz in TPC Down (mm)
    std::vector<double>* VecTPCDown2_TrackEdep = 0; // Vector of edep in TPC Down (MeV)
    std::vector<double>* VecTPCDown2_TrackCharge = 0; // Vector of charge in TPC Down (e-)
    std::vector<int>   * VecTPCDown2_TrackPDG = 0; // Vector of pdg in TPC Down
    std::vector<double> *VecTPCDown2_TrackStepFirstX = 0; // Vector of first step position in TPC Down
    std::vector<double> *VecTPCDown2_TrackStepFirstY = 0; // Vector of first step position in TPC Down
    std::vector<double> *VecTPCDown2_TrackStepFirstZ = 0; // Vector of first step position in TPC Down
    std::vector<double> *VecTPCDown2_TrackStepLastX = 0; // Vector of last step position in TPC Down
    std::vector<double> *VecTPCDown2_TrackStepLastY = 0; // Vector of last step position in TPC Down
    std::vector<double> *VecTPCDown2_TrackStepLastZ = 0; // Vector of last step position in TPC Down
    std::vector<double> *VecTPCDown2_TrackCosTheta = 0; // Vector of cos(theta) wrt nu direction (z axis) in TPC Down
    
    // Target 1 informations
    int     Target1_NTracks;          // # of tracks in Target
    std::vector<int>   * VecTarget1_TrackID = 0; // Vector of trackID in Target
    std::vector<int>   * VecTarget1_ParentID = 0; // Vector of parentID in Target
    std::vector<double>* VecTarget1_TrackMomX = 0; // Vector of initial mom in Target (MeV)
    std::vector<double>* VecTarget1_TrackMomY = 0; // Vector of initial mom in Target (MeV)
    std::vector<double>* VecTarget1_TrackMomZ = 0; // Vector of initial mom in Target (MeV)
    std::vector<double>* VecTarget1_TrackLength = 0; // Vector of corrected (mult. scatt.) length in Target (mm)
    std::vector<double>* VecTarget1_TrackDeltaLyz = 0; // Vector of geometrical length yz in Target (mm)
    std::vector<double>* VecTarget1_TrackEdep = 0; // Vector of edep in Target (MeV)
    std::vector<double>* VecTarget1_TrackCharge = 0; // Vector of charge in Target (e-)
    std::vector<int>   * VecTarget1_TrackPDG = 0; // Vector of pdg in Target
    std::vector<double> *VecTarget1_TrackStepFirstX = 0; // Vector of first step position in Target
    std::vector<double> *VecTarget1_TrackStepFirstY = 0; // Vector of first step position in Target
    std::vector<double> *VecTarget1_TrackStepFirstZ = 0; // Vector of first step position in Target
    std::vector<double> *VecTarget1_TrackStepLastX = 0; // Vector of last step position in Target
    std::vector<double> *VecTarget1_TrackStepLastY = 0; // Vector of last step position in Target
    std::vector<double> *VecTarget1_TrackStepLastZ = 0; // Vector of last step position in Target
    std::vector<double> *VecTarget1_TrackCosTheta = 0; // Vector of cos(theta) wrt nu direction (z axis) in Target

    // Target 2 informations
    int     Target2_NTracks;          // # of tracks in Target
    std::vector<int>   * VecTarget2_TrackID = 0; // Vector of trackID in Target
    std::vector<int>   * VecTarget2_ParentID = 0; // Vector of parentID in Target
    std::vector<double>* VecTarget2_TrackMomX = 0; // Vector of initial mom in Target (MeV)
    std::vector<double>* VecTarget2_TrackMomY = 0; // Vector of initial mom in Target (MeV)
    std::vector<double>* VecTarget2_TrackMomZ = 0; // Vector of initial mom in Target (MeV)
    std::vector<double>* VecTarget2_TrackLength = 0; // Vector of corrected (mult. scatt.) length in Target (mm)
    std::vector<double>* VecTarget2_TrackDeltaLyz = 0; // Vector of geometrical length yz in Target (mm)
    std::vector<double>* VecTarget2_TrackEdep = 0; // Vector of edep in Target (MeV)
    std::vector<double>* VecTarget2_TrackCharge = 0; // Vector of charge in Target (e-)
    std::vector<int>   * VecTarget2_TrackPDG = 0; // Vector of pdg in Target
    std::vector<double> *VecTarget2_TrackStepFirstX = 0; // Vector of first step position in Target
    std::vector<double> *VecTarget2_TrackStepFirstY = 0; // Vector of first step position in Target
    std::vector<double> *VecTarget2_TrackStepFirstZ = 0; // Vector of first step position in Target
    std::vector<double> *VecTarget2_TrackStepLastX = 0; // Vector of last step position in Target
    std::vector<double> *VecTarget2_TrackStepLastY = 0; // Vector of last step position in Target
    std::vector<double> *VecTarget2_TrackStepLastZ = 0; // Vector of last step position in Target
    std::vector<double> *VecTarget2_TrackCosTheta = 0; // Vector of cos(theta) wrt nu direction (z axis) in Target
    
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
    
    treein->SetBranchAddress("EabsTarget1", &EabsTarget1);
    treein->SetBranchAddress("EabsForwTPC1",&EabsForwTPC1);
    treein->SetBranchAddress("EabsTPCup1",  &EabsTPCup1);
    treein->SetBranchAddress("EabsTPCdown1",&EabsTPCdown1);
    treein->SetBranchAddress("EabsTarget2", &EabsTarget2);
    treein->SetBranchAddress("EabsTPCup2",  &EabsTPCup2);
    treein->SetBranchAddress("EabsTPCdown2",&EabsTPCdown2);

    treein->SetBranchAddress("ForwTPC1_NTracks",      &ForwTPC1_NTracks);
    treein->SetBranchAddress("VecForwTPC1_TrackID",   &VecForwTPC1_TrackID);
    treein->SetBranchAddress("VecForwTPC1_ParentID",   &VecForwTPC1_ParentID);
    treein->SetBranchAddress("VecForwTPC1_TrackMomX", &VecForwTPC1_TrackMomX);
    treein->SetBranchAddress("VecForwTPC1_TrackMomY", &VecForwTPC1_TrackMomY);
    treein->SetBranchAddress("VecForwTPC1_TrackMomZ", &VecForwTPC1_TrackMomZ);
    treein->SetBranchAddress("VecForwTPC1_TrackLength", &VecForwTPC1_TrackLength);
    treein->SetBranchAddress("VecForwTPC1_TrackDeltaLyz", &VecForwTPC1_TrackDeltaLyz);
    treein->SetBranchAddress("VecForwTPC1_TrackEdep", &VecForwTPC1_TrackEdep);
    treein->SetBranchAddress("VecForwTPC1_TrackCharge", &VecForwTPC1_TrackCharge);
    treein->SetBranchAddress("VecForwTPC1_TrackPDG", &VecForwTPC1_TrackPDG);
    treein->SetBranchAddress("VecForwTPC1_TrackStepFirstX",&VecForwTPC1_TrackStepFirstX);
    treein->SetBranchAddress("VecForwTPC1_TrackStepFirstY",&VecForwTPC1_TrackStepFirstY);
    treein->SetBranchAddress("VecForwTPC1_TrackStepFirstZ",&VecForwTPC1_TrackStepFirstZ);
    treein->SetBranchAddress("VecForwTPC1_TrackStepLastX",&VecForwTPC1_TrackStepLastX);
    treein->SetBranchAddress("VecForwTPC1_TrackStepLastY",&VecForwTPC1_TrackStepLastY);
    treein->SetBranchAddress("VecForwTPC1_TrackStepLastZ",&VecForwTPC1_TrackStepLastZ);
    treein->SetBranchAddress("VecForwTPC1_TrackCosTheta",&VecForwTPC1_TrackCosTheta);
    
    treein->SetBranchAddress("TPCUp1_NTracks",      &TPCUp1_NTracks);
    treein->SetBranchAddress("VecTPCUp1_TrackID",   &VecTPCUp1_TrackID);
    treein->SetBranchAddress("VecTPCUp1_ParentID",   &VecTPCUp1_ParentID);
    treein->SetBranchAddress("VecTPCUp1_TrackMomX", &VecTPCUp1_TrackMomX);
    treein->SetBranchAddress("VecTPCUp1_TrackMomY", &VecTPCUp1_TrackMomY);
    treein->SetBranchAddress("VecTPCUp1_TrackMomZ", &VecTPCUp1_TrackMomZ);
    treein->SetBranchAddress("VecTPCUp1_TrackLength", &VecTPCUp1_TrackLength);
    treein->SetBranchAddress("VecTPCUp1_TrackDeltaLyz", &VecTPCUp1_TrackDeltaLyz);
    treein->SetBranchAddress("VecTPCUp1_TrackEdep", &VecTPCUp1_TrackEdep);
    treein->SetBranchAddress("VecTPCUp1_TrackCharge", &VecTPCUp1_TrackCharge);
    treein->SetBranchAddress("VecTPCUp1_TrackPDG", &VecTPCUp1_TrackPDG);
    treein->SetBranchAddress("VecTPCUp1_TrackStepFirstX",&VecTPCUp1_TrackStepFirstX);
    treein->SetBranchAddress("VecTPCUp1_TrackStepFirstY",&VecTPCUp1_TrackStepFirstY);
    treein->SetBranchAddress("VecTPCUp1_TrackStepFirstZ",&VecTPCUp1_TrackStepFirstZ);
    treein->SetBranchAddress("VecTPCUp1_TrackStepLastX",&VecTPCUp1_TrackStepLastX);
    treein->SetBranchAddress("VecTPCUp1_TrackStepLastY",&VecTPCUp1_TrackStepLastY);
    treein->SetBranchAddress("VecTPCUp1_TrackStepLastZ",&VecTPCUp1_TrackStepLastZ);
    treein->SetBranchAddress("VecTPCUp1_TrackCosTheta",&VecTPCUp1_TrackCosTheta);
 
    treein->SetBranchAddress("TPCUp2_NTracks",      &TPCUp2_NTracks);
    treein->SetBranchAddress("VecTPCUp2_TrackID",   &VecTPCUp2_TrackID);
    treein->SetBranchAddress("VecTPCUp2_ParentID",   &VecTPCUp2_ParentID);
    treein->SetBranchAddress("VecTPCUp2_TrackMomX", &VecTPCUp2_TrackMomX);
    treein->SetBranchAddress("VecTPCUp2_TrackMomY", &VecTPCUp2_TrackMomY);
    treein->SetBranchAddress("VecTPCUp2_TrackMomZ", &VecTPCUp2_TrackMomZ);
    treein->SetBranchAddress("VecTPCUp2_TrackLength", &VecTPCUp2_TrackLength);
    treein->SetBranchAddress("VecTPCUp2_TrackDeltaLyz", &VecTPCUp2_TrackDeltaLyz);
    treein->SetBranchAddress("VecTPCUp2_TrackEdep", &VecTPCUp2_TrackEdep);
    treein->SetBranchAddress("VecTPCUp2_TrackCharge", &VecTPCUp2_TrackCharge);
    treein->SetBranchAddress("VecTPCUp2_TrackPDG", &VecTPCUp2_TrackPDG);
    treein->SetBranchAddress("VecTPCUp2_TrackStepFirstX",&VecTPCUp2_TrackStepFirstX);
    treein->SetBranchAddress("VecTPCUp2_TrackStepFirstY",&VecTPCUp2_TrackStepFirstY);
    treein->SetBranchAddress("VecTPCUp2_TrackStepFirstZ",&VecTPCUp2_TrackStepFirstZ);
    treein->SetBranchAddress("VecTPCUp2_TrackStepLastX",&VecTPCUp2_TrackStepLastX);
    treein->SetBranchAddress("VecTPCUp2_TrackStepLastY",&VecTPCUp2_TrackStepLastY);
    treein->SetBranchAddress("VecTPCUp2_TrackStepLastZ",&VecTPCUp2_TrackStepLastZ);
    treein->SetBranchAddress("VecTPCUp2_TrackCosTheta",&VecTPCUp2_TrackCosTheta);
    
    treein->SetBranchAddress("TPCDown1_NTracks",      &TPCDown1_NTracks);
    treein->SetBranchAddress("VecTPCDown1_TrackID",   &VecTPCDown1_TrackID);
    treein->SetBranchAddress("VecTPCDown1_ParentID",   &VecTPCDown1_ParentID);
    treein->SetBranchAddress("VecTPCDown1_TrackMomX", &VecTPCDown1_TrackMomX);
    treein->SetBranchAddress("VecTPCDown1_TrackMomY", &VecTPCDown1_TrackMomY);
    treein->SetBranchAddress("VecTPCDown1_TrackMomZ", &VecTPCDown1_TrackMomZ);
    treein->SetBranchAddress("VecTPCDown1_TrackLength", &VecTPCDown1_TrackLength);
    treein->SetBranchAddress("VecTPCDown1_TrackDeltaLyz", &VecTPCDown1_TrackDeltaLyz);
    treein->SetBranchAddress("VecTPCDown1_TrackEdep", &VecTPCDown1_TrackEdep);
    treein->SetBranchAddress("VecTPCDown1_TrackCharge", &VecTPCDown1_TrackCharge);
    treein->SetBranchAddress("VecTPCDown1_TrackPDG", &VecTPCDown1_TrackPDG);
    treein->SetBranchAddress("VecTPCDown1_TrackStepFirstX",&VecTPCDown1_TrackStepFirstX);
    treein->SetBranchAddress("VecTPCDown1_TrackStepFirstY",&VecTPCDown1_TrackStepFirstY);
    treein->SetBranchAddress("VecTPCDown1_TrackStepFirstZ",&VecTPCDown1_TrackStepFirstZ);
    treein->SetBranchAddress("VecTPCDown1_TrackStepLastX",&VecTPCDown1_TrackStepLastX);
    treein->SetBranchAddress("VecTPCDown1_TrackStepLastY",&VecTPCDown1_TrackStepLastY);
    treein->SetBranchAddress("VecTPCDown1_TrackStepLastZ",&VecTPCDown1_TrackStepLastZ);
    treein->SetBranchAddress("VecTPCDown1_TrackCosTheta",&VecTPCDown1_TrackCosTheta);

    treein->SetBranchAddress("TPCDown2_NTracks",      &TPCDown2_NTracks);
    treein->SetBranchAddress("VecTPCDown2_TrackID",   &VecTPCDown2_TrackID);
    treein->SetBranchAddress("VecTPCDown2_ParentID",   &VecTPCDown2_ParentID);
    treein->SetBranchAddress("VecTPCDown2_TrackMomX", &VecTPCDown2_TrackMomX);
    treein->SetBranchAddress("VecTPCDown2_TrackMomY", &VecTPCDown2_TrackMomY);
    treein->SetBranchAddress("VecTPCDown2_TrackMomZ", &VecTPCDown2_TrackMomZ);
    treein->SetBranchAddress("VecTPCDown2_TrackLength", &VecTPCDown2_TrackLength);
    treein->SetBranchAddress("VecTPCDown2_TrackDeltaLyz", &VecTPCDown2_TrackDeltaLyz);
    treein->SetBranchAddress("VecTPCDown2_TrackEdep", &VecTPCDown2_TrackEdep);
    treein->SetBranchAddress("VecTPCDown2_TrackCharge", &VecTPCDown2_TrackCharge);
    treein->SetBranchAddress("VecTPCDown2_TrackPDG", &VecTPCDown2_TrackPDG);
    treein->SetBranchAddress("VecTPCDown2_TrackStepFirstX",&VecTPCDown2_TrackStepFirstX);
    treein->SetBranchAddress("VecTPCDown2_TrackStepFirstY",&VecTPCDown2_TrackStepFirstY);
    treein->SetBranchAddress("VecTPCDown2_TrackStepFirstZ",&VecTPCDown2_TrackStepFirstZ);
    treein->SetBranchAddress("VecTPCDown2_TrackStepLastX",&VecTPCDown2_TrackStepLastX);
    treein->SetBranchAddress("VecTPCDown2_TrackStepLastY",&VecTPCDown2_TrackStepLastY);
    treein->SetBranchAddress("VecTPCDown2_TrackStepLastZ",&VecTPCDown2_TrackStepLastZ);
    treein->SetBranchAddress("VecTPCDown2_TrackCosTheta",&VecTPCDown2_TrackCosTheta);
    
    treein->SetBranchAddress("Target1_NTracks",      &Target1_NTracks);
    treein->SetBranchAddress("VecTarget1_TrackID",   &VecTarget1_TrackID);
    treein->SetBranchAddress("VecTarget1_ParentID",   &VecTarget1_ParentID);
    treein->SetBranchAddress("VecTarget1_TrackMomX", &VecTarget1_TrackMomX);
    treein->SetBranchAddress("VecTarget1_TrackMomY", &VecTarget1_TrackMomY);
    treein->SetBranchAddress("VecTarget1_TrackMomZ", &VecTarget1_TrackMomZ);
    treein->SetBranchAddress("VecTarget1_TrackLength", &VecTarget1_TrackLength);
    treein->SetBranchAddress("VecTarget1_TrackDeltaLyz", &VecTarget1_TrackDeltaLyz);
    treein->SetBranchAddress("VecTarget1_TrackEdep", &VecTarget1_TrackEdep);
    treein->SetBranchAddress("VecTarget1_TrackCharge", &VecTarget1_TrackCharge);
    treein->SetBranchAddress("VecTarget1_TrackPDG", &VecTarget1_TrackPDG);
    treein->SetBranchAddress("VecTarget1_TrackStepFirstX",&VecTarget1_TrackStepFirstX);
    treein->SetBranchAddress("VecTarget1_TrackStepFirstY",&VecTarget1_TrackStepFirstY);
    treein->SetBranchAddress("VecTarget1_TrackStepFirstZ",&VecTarget1_TrackStepFirstZ);
    treein->SetBranchAddress("VecTarget1_TrackStepLastX",&VecTarget1_TrackStepLastX);
    treein->SetBranchAddress("VecTarget1_TrackStepLastY",&VecTarget1_TrackStepLastY);
    treein->SetBranchAddress("VecTarget1_TrackStepLastZ",&VecTarget1_TrackStepLastZ);
    treein->SetBranchAddress("VecTarget1_TrackCosTheta",&VecTarget1_TrackCosTheta);
 
    treein->SetBranchAddress("Target2_NTracks",      &Target2_NTracks);
    treein->SetBranchAddress("VecTarget2_TrackID",   &VecTarget2_TrackID);
    treein->SetBranchAddress("VecTarget2_ParentID",   &VecTarget2_ParentID);
    treein->SetBranchAddress("VecTarget2_TrackMomX", &VecTarget2_TrackMomX);
    treein->SetBranchAddress("VecTarget2_TrackMomY", &VecTarget2_TrackMomY);
    treein->SetBranchAddress("VecTarget2_TrackMomZ", &VecTarget2_TrackMomZ);
    treein->SetBranchAddress("VecTarget2_TrackLength", &VecTarget2_TrackLength);
    treein->SetBranchAddress("VecTarget2_TrackDeltaLyz", &VecTarget2_TrackDeltaLyz);
    treein->SetBranchAddress("VecTarget2_TrackEdep", &VecTarget2_TrackEdep);
    treein->SetBranchAddress("VecTarget2_TrackCharge", &VecTarget2_TrackCharge);
    treein->SetBranchAddress("VecTarget2_TrackPDG", &VecTarget2_TrackPDG);
    treein->SetBranchAddress("VecTarget2_TrackStepFirstX",&VecTarget2_TrackStepFirstX);
    treein->SetBranchAddress("VecTarget2_TrackStepFirstY",&VecTarget2_TrackStepFirstY);
    treein->SetBranchAddress("VecTarget2_TrackStepFirstZ",&VecTarget2_TrackStepFirstZ);
    treein->SetBranchAddress("VecTarget2_TrackStepLastX",&VecTarget2_TrackStepLastX);
    treein->SetBranchAddress("VecTarget2_TrackStepLastY",&VecTarget2_TrackStepLastY);
    treein->SetBranchAddress("VecTarget2_TrackStepLastZ",&VecTarget2_TrackStepLastZ);
    treein->SetBranchAddress("VecTarget2_TrackCosTheta",&VecTarget2_TrackCosTheta);
    
    treein->SetBranchAddress("NVtx", &NVtx);
    treein->SetBranchAddress("VtxX", &VecVtx_X);
    treein->SetBranchAddress("VtxY", &VecVtx_Y);
    treein->SetBranchAddress("VtxZ", &VecVtx_Z);
    treein->SetBranchAddress("VecVtx_NuPDG", &VecVtx_NuPDG);
    treein->SetBranchAddress("VecVtx_ReacMode", &VecVtx_ReacMode);
    treein->SetBranchAddress("VecVtx_EvtProb", &VecVtx_EvtProb);
    treein->SetBranchAddress("VecVtx_EvtWeight", &VecVtx_EvtWeight);
    
    // Create tree to store outputs
    
    double ForwTPC1_ID;
    double ForwTPC1_mom;
    double ForwTPC1_costheta;
    double ForwTPC1_len;
    double ForwTPC1_dlyz;
    double ForwTPC1_charge;
    double ForwTPC1_edep;
    double ForwTPC1_pdg;
    double ForwTPC1_parent_pdg;
    double ForwTPC1_PosIniX;
    double ForwTPC1_PosIniY;
    double ForwTPC1_PosIniZ;
    double ForwTPC1_PosFinX;
    double ForwTPC1_PosFinY;
    double ForwTPC1_PosFinZ;

    double TPCUp1_ID;
    double TPCUp1_mom;
    double TPCUp1_costheta;
    double TPCUp1_len;
    double TPCUp1_dlyz;
    double TPCUp1_charge;
    double TPCUp1_edep;
    double TPCUp1_pdg;
    double TPCUp1_parent_pdg;
    double TPCUp1_PosIniX;
    double TPCUp1_PosIniY;
    double TPCUp1_PosIniZ;
    double TPCUp1_PosFinX;
    double TPCUp1_PosFinY;
    double TPCUp1_PosFinZ;

    double TPCUp2_ID;
    double TPCUp2_mom;
    double TPCUp2_costheta;
    double TPCUp2_len;
    double TPCUp2_dlyz;
    double TPCUp2_charge;
    double TPCUp2_edep;
    double TPCUp2_pdg;
    double TPCUp2_parent_pdg;
    double TPCUp2_PosIniX;
    double TPCUp2_PosIniY;
    double TPCUp2_PosIniZ;
    double TPCUp2_PosFinX;
    double TPCUp2_PosFinY;
    double TPCUp2_PosFinZ;

    double TPCDown1_ID;
    double TPCDown1_mom;
    double TPCDown1_costheta;
    double TPCDown1_len;
    double TPCDown1_dlyz;
    double TPCDown1_charge;
    double TPCDown1_edep;
    double TPCDown1_pdg;
    double TPCDown1_parent_pdg;
    double TPCDown1_PosIniX;
    double TPCDown1_PosIniY;
    double TPCDown1_PosIniZ;
    double TPCDown1_PosFinX;
    double TPCDown1_PosFinY;
    double TPCDown1_PosFinZ;

    double TPCDown2_ID;
    double TPCDown2_mom;
    double TPCDown2_costheta;
    double TPCDown2_len;
    double TPCDown2_dlyz;
    double TPCDown2_charge;
    double TPCDown2_edep;
    double TPCDown2_pdg;
    double TPCDown2_parent_pdg;
    double TPCDown2_PosIniX;
    double TPCDown2_PosIniY;
    double TPCDown2_PosIniZ;
    double TPCDown2_PosFinX;
    double TPCDown2_PosFinY;
    double TPCDown2_PosFinZ;

    double Target1_ID;
    double Target1_mom;
    double Target1_costheta;
    double Target1_len;
    double Target1_dlyz;
    double Target1_charge;
    double Target1_edep;
    double Target1_pdg;
    double Target1_parent_pdg;
    double Target1_PosIniX;
    double Target1_PosIniY;
    double Target1_PosIniZ;
    double Target1_PosFinX;
    double Target1_PosFinY;
    double Target1_PosFinZ;

    double Target2_ID;
    double Target2_mom;
    double Target2_costheta;
    double Target2_len;
    double Target2_dlyz;
    double Target2_charge;
    double Target2_edep;
    double Target2_pdg;
    double Target2_parent_pdg;
    double Target2_PosIniX;
    double Target2_PosIniY;
    double Target2_PosIniZ;
    double Target2_PosFinX;
    double Target2_PosFinY;
    double Target2_PosFinZ;
    
    // TFile *fileoa = new TFile("../bin/fileoa.root","RECREATE");
    // TTree * treeoa = new TTree("treeoa","Skimmed MC");
    // treeoa->Branch("TPCUp1_ID",      &TPCUp1_ID);
    // treeoa->Branch("TPCUp1_mom",      &TPCUp1_mom);
    // treeoa->Branch("TPCUp1_costheta", &TPCUp1_costheta);
    // treeoa->Branch("TPCUp1_len",      &TPCUp1_len);
    // treeoa->Branch("TPCUp1_dlyz",      &TPCUp1_dlyz);
    // treeoa->Branch("TPCUp1_charge",   &TPCUp1_charge);
    // treeoa->Branch("TPCUp1_edep",     &TPCUp1_edep);
    // treeoa->Branch("TPCUp1_pdg",      &TPCUp1_pdg);
    // treeoa->Branch("TPCUp1_parent_pdg",&TPCUp1_parent_pdg);
    // treeoa->Branch("TPCUp1_PosIniX",&TPCUp1_PosIniX);
    // treeoa->Branch("TPCUp1_PosIniY",&TPCUp1_PosIniY);
    // treeoa->Branch("TPCUp1_PosIniZ",&TPCUp1_PosIniZ);
    // treeoa->Branch("TPCUp1_PosFinX",&TPCUp1_PosFinX);
    // treeoa->Branch("TPCUp1_PosFinY",&TPCUp1_PosFinY);
    // treeoa->Branch("TPCUp1_PosFinZ",&TPCUp1_PosFinZ);
    // treeoa->Branch("TPCUp2_ID",      &TPCUp2_ID);
    // treeoa->Branch("TPCUp2_mom",      &TPCUp2_mom);
    // treeoa->Branch("TPCUp2_costheta", &TPCUp2_costheta);
    // treeoa->Branch("TPCUp2_len",      &TPCUp2_len);
    // treeoa->Branch("TPCUp2_dlyz",      &TPCUp2_dlyz);
    // treeoa->Branch("TPCUp2_charge",   &TPCUp2_charge);
    // treeoa->Branch("TPCUp2_edep",     &TPCUp2_edep);
    // treeoa->Branch("TPCUp2_pdg",      &TPCUp2_pdg);
    // treeoa->Branch("TPCUp2_parent_pdg",&TPCUp2_parent_pdg);
    // treeoa->Branch("TPCUp2_PosIniX",&TPCUp2_PosIniX);
    // treeoa->Branch("TPCUp2_PosIniY",&TPCUp2_PosIniY);
    // treeoa->Branch("TPCUp2_PosIniZ",&TPCUp2_PosIniZ);
    // treeoa->Branch("TPCUp2_PosFinX",&TPCUp2_PosFinX);
    // treeoa->Branch("TPCUp2_PosFinY",&TPCUp2_PosFinY);
    // treeoa->Branch("TPCUp2_PosFinZ",&TPCUp2_PosFinZ);
    // treeoa->Branch("TPCDown1_ID",    &TPCDown1_ID);
    // treeoa->Branch("TPCDown1_mom",    &TPCDown1_mom);
    // treeoa->Branch("TPCDown1_costheta",&TPCDown1_costheta);
    // treeoa->Branch("TPCDown1_len",    &TPCDown1_len);
    // treeoa->Branch("TPCDown1_dlyz",    &TPCDown1_dlyz);
    // treeoa->Branch("TPCDown1_charge", &TPCDown1_charge);
    // treeoa->Branch("TPCDown1_edep",   &TPCDown1_edep);
    // treeoa->Branch("TPCDown1_pdg",    &TPCDown1_pdg);
    // treeoa->Branch("TPCDown1_parent_pdg",&TPCDown1_parent_pdg);
    // treeoa->Branch("TPCDown1_PosIniX",&TPCDown1_PosIniX);
    // treeoa->Branch("TPCDown1_PosIniY",&TPCDown1_PosIniY);
    // treeoa->Branch("TPCDown1_PosIniZ",&TPCDown1_PosIniZ);
    // treeoa->Branch("TPCDown1_PosFinX",&TPCDown1_PosFinX);
    // treeoa->Branch("TPCDown1_PosFinY",&TPCDown1_PosFinY);
    // treeoa->Branch("TPCDown1_PosFinZ",&TPCDown1_PosFinZ);
    // treeoa->Branch("TPCDown2_ID",    &TPCDown2_ID);
    // treeoa->Branch("TPCDown2_mom",    &TPCDown2_mom);
    // treeoa->Branch("TPCDown2_costheta",&TPCDown2_costheta);
    // treeoa->Branch("TPCDown2_len",    &TPCDown2_len);
    // treeoa->Branch("TPCDown2_dlyz",    &TPCDown2_dlyz);
    // treeoa->Branch("TPCDown2_charge", &TPCDown2_charge);
    // treeoa->Branch("TPCDown2_edep",   &TPCDown2_edep);
    // treeoa->Branch("TPCDown2_pdg",    &TPCDown2_pdg);
    // treeoa->Branch("TPCDown2_parent_pdg",&TPCDown2_parent_pdg);
    // treeoa->Branch("TPCDown2_PosIniX",&TPCDown2_PosIniX);
    // treeoa->Branch("TPCDown2_PosIniY",&TPCDown2_PosIniY);
    // treeoa->Branch("TPCDown2_PosIniZ",&TPCDown2_PosIniZ);
    // treeoa->Branch("TPCDown2_PosFinX",&TPCDown2_PosFinX);
    // treeoa->Branch("TPCDown2_PosFinY",&TPCDown2_PosFinY);
    // treeoa->Branch("TPCDown2_PosFinZ",&TPCDown2_PosFinZ);
    // treeoa->Branch("Target1_ID",     &Target1_ID);
    // treeoa->Branch("Target1_mom",     &Target1_mom);
    // treeoa->Branch("Target1_costheta",&Target1_costheta);
    // treeoa->Branch("Target1_len",     &Target1_len);
    // treeoa->Branch("Target1_dlyz",     &Target1_dlyz);
    // treeoa->Branch("Target1_charge",  &Target1_charge);
    // treeoa->Branch("Target1_edep",    &Target1_edep);
    // treeoa->Branch("Target1_pdg",     &Target1_pdg);
    // treeoa->Branch("Target1_parent_pdg",&Target1_parent_pdg);
    // treeoa->Branch("Target1_PosIniX",&Target1_PosIniX);
    // treeoa->Branch("Target1_PosIniY",&Target1_PosIniY);
    // treeoa->Branch("Target1_PosIniZ",&Target1_PosIniZ);
    // treeoa->Branch("Target1_PosFinX",&Target1_PosFinX);
    // treeoa->Branch("Target1_PosFinY",&Target1_PosFinY);
    // treeoa->Branch("Target1_PosFinZ",&Target1_PosFinZ);
    // treeoa->Branch("Target2_ID",     &Target2_ID);
    // treeoa->Branch("Target2_mom",     &Target2_mom);
    // treeoa->Branch("Target2_costheta",&Target2_costheta);
    // treeoa->Branch("Target2_len",     &Target2_len);
    // treeoa->Branch("Target2_dlyz",     &Target2_dlyz);
    // treeoa->Branch("Target2_charge",  &Target2_charge);
    // treeoa->Branch("Target2_edep",    &Target2_edep);
    // treeoa->Branch("Target2_pdg",     &Target2_pdg);
    // treeoa->Branch("Target2_parent_pdg",&Target2_parent_pdg);
    // treeoa->Branch("Target2_PosIniX",&Target2_PosIniX);
    // treeoa->Branch("Target2_PosIniY",&Target2_PosIniY);
    // treeoa->Branch("Target2_PosIniZ",&Target2_PosIniZ);
    // treeoa->Branch("Target2_PosFinX",&Target2_PosFinX);
    // treeoa->Branch("Target2_PosFinY",&Target2_PosFinY);
    // treeoa->Branch("Target2_PosFinZ",&Target2_PosFinZ);
    
    
    // Loop over the ntuple events
    
    int Nentries = treein->GetEntries();
    
    for(int ientry=0;ientry<Nentries;ientry++){
        //for(int ientry=0;ientry<10;ientry++){
        
        if(doSingleEvent){
            ientry = SingleEventID;
            Nentries = ientry+1;
        }
        
        // Reset variables
        ForwTPC1_ID = -9999;
        ForwTPC1_mom = -9999;
        ForwTPC1_costheta = -9999;
        ForwTPC1_len = -9999;
        ForwTPC1_dlyz = -9999;
        ForwTPC1_charge = -9999;
        ForwTPC1_edep = -9999;
        ForwTPC1_pdg = -9999;
        ForwTPC1_parent_pdg = -9999;
        ForwTPC1_PosIniX = -9999;
        ForwTPC1_PosIniY = -9999;
        ForwTPC1_PosIniZ = -9999;
        ForwTPC1_PosFinX = -9999;
        ForwTPC1_PosFinY = -9999;
        ForwTPC1_PosFinZ = -9999;
        TPCUp1_ID = -9999;
        TPCUp1_mom = -9999;
        TPCUp1_costheta = -9999;
        TPCUp1_len = -9999;
        TPCUp1_dlyz = -9999;
        TPCUp1_charge = -9999;
        TPCUp1_edep = -9999;
        TPCUp1_pdg = -9999;
        TPCUp1_parent_pdg = -9999;
        TPCUp1_PosIniX = -9999;
        TPCUp1_PosIniY = -9999;
        TPCUp1_PosIniZ = -9999;
        TPCUp1_PosFinX = -9999;
        TPCUp1_PosFinY = -9999;
        TPCUp1_PosFinZ = -9999;
        TPCUp2_ID = -9999;
        TPCUp2_mom = -9999;
        TPCUp2_costheta = -9999;
        TPCUp2_len = -9999;
        TPCUp2_dlyz = -9999;
        TPCUp2_charge = -9999;
        TPCUp2_edep = -9999;
        TPCUp2_pdg = -9999;
        TPCUp2_parent_pdg = -9999;
        TPCUp2_PosIniX = -9999;
        TPCUp2_PosIniY = -9999;
        TPCUp2_PosIniZ = -9999;
        TPCUp2_PosFinX = -9999;
        TPCUp2_PosFinY = -9999;
        TPCUp2_PosFinZ = -9999;
        TPCDown1_ID = -9999;
        TPCDown1_mom = -9999;
        TPCDown1_costheta = -9999;
        TPCDown1_len = -9999;
        TPCDown1_dlyz = -9999;
        TPCDown1_charge = -9999;
        TPCDown1_edep = -9999;
        TPCDown1_pdg = -9999;
        TPCDown1_parent_pdg = -9999;
        TPCDown1_PosIniX = -9999;
        TPCDown1_PosIniY = -9999;
        TPCDown1_PosIniZ = -9999;
        TPCDown1_PosFinX = -9999;
        TPCDown1_PosFinY = -9999;
        TPCDown1_PosFinZ = -9999;
        TPCDown2_ID = -9999;
        TPCDown2_mom = -9999;
        TPCDown2_costheta = -9999;
        TPCDown2_len = -9999;
        TPCDown2_dlyz = -9999;
        TPCDown2_charge = -9999;
        TPCDown2_edep = -9999;
        TPCDown2_pdg = -9999;
        TPCDown2_parent_pdg = -9999;
        TPCDown2_PosIniX = -9999;
        TPCDown2_PosIniY = -9999;
        TPCDown2_PosIniZ = -9999;
        TPCDown2_PosFinX = -9999;
        TPCDown2_PosFinY = -9999;
        TPCDown2_PosFinZ = -9999;
        Target1_ID = -9999;
        Target1_mom = -9999;
        Target1_costheta = -9999;
        Target1_len = -9999;
        Target1_dlyz = -9999;
        Target1_charge = -9999;
        Target1_edep = -9999;
        Target1_pdg = -9999;
        Target1_parent_pdg = -9999;
        Target1_PosIniX = -9999;
        Target1_PosIniY = -9999;
        Target1_PosIniZ = -9999;
        Target1_PosFinX = -9999;
        Target1_PosFinY = -9999;
        Target1_PosFinZ = -9999;
	Target2_ID = -9999;
        Target2_mom = -9999;
        Target2_costheta = -9999;
        Target2_len = -9999;
        Target2_dlyz = -9999;
        Target2_charge = -9999;
        Target2_edep = -9999;
        Target2_pdg = -9999;
        Target2_parent_pdg = -9999;
        Target2_PosIniX = -9999;
        Target2_PosIniY = -9999;
        Target2_PosIniZ = -9999;
        Target2_PosFinX = -9999;
        Target2_PosFinY = -9999;
        Target2_PosFinZ = -9999;
        
        treein->GetEntry(ientry);
        
        if(!(ientry%100)) cout << "Event " << ientry << endl;
        if(doPrintTrack){
	  cout << "Event " << ientry << endl;
	  cout <<" - Vtx reac mode: " << VecVtx_ReacMode->at(0) << endl;
	}

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
        
        int NStepFirst_ForwTPC1 = VecForwTPC1_TrackStepFirstX->size();
        int NStepLast_ForwTPC1 = VecForwTPC1_TrackStepLastX->size();
        int NStepFirst_TPCUp1 = VecTPCUp1_TrackStepFirstX->size();
        int NStepLast_TPCUp1 = VecTPCUp1_TrackStepLastX->size();
        int NStepFirst_TPCDown1 = VecTPCDown1_TrackStepFirstX->size();
        int NStepLast_TPCDown1 = VecTPCDown1_TrackStepLastX->size();
        int NStepFirst_Target1 = VecTarget1_TrackStepFirstX->size();
        int NStepLast_Target1 = VecTarget1_TrackStepLastX->size();

        int NStepFirst_TPCUp2 = VecTPCUp2_TrackStepFirstX->size();
        int NStepLast_TPCUp2 = VecTPCUp2_TrackStepLastX->size();
        int NStepFirst_TPCDown2 = VecTPCDown2_TrackStepFirstX->size();
        int NStepLast_TPCDown2 = VecTPCDown2_TrackStepLastX->size();
        int NStepFirst_Target2 = VecTarget2_TrackStepFirstX->size();
        int NStepLast_Target2 = VecTarget2_TrackStepLastX->size();
        
        if(NStepFirst_ForwTPC1 != NStepLast_ForwTPC1){
	  cout << "Size ForwTPC1 first step = " << VecForwTPC1_TrackStepFirstX->size() << endl;
	  cout << "Size ForwTPC1 last step = " << VecForwTPC1_TrackStepLastX->size() << endl;
	  exit(1);
        }
        if(NStepFirst_TPCUp1 != NStepLast_TPCUp1){
	  cout << "Size TPCUp1 first step = " << VecTPCUp1_TrackStepFirstX->size() << endl;
	  cout << "Size TPCUp1 last step = " << VecTPCUp1_TrackStepLastX->size() << endl;
	  exit(1);
        }
        if(NStepFirst_TPCDown1 != NStepLast_TPCDown1){
	  cout << "Size TPCDown1 first step = " << VecTPCDown1_TrackStepFirstX->size() << endl;
	  cout << "Size TPCDown1 last step = " << VecTPCDown1_TrackStepLastX->size() << endl;
	  exit(1);
        }
        if(NStepFirst_Target1 != NStepLast_Target1){
	  cout << "Size Target1 first step = " << VecTarget1_TrackStepFirstX->size() << endl;
	  cout << "Size Target1 last step = " << VecTarget1_TrackStepLastX->size() << endl;
	  exit(1);
        }        
        
        //
        // Loop over all tracks
        //
        
         /////////////////////////////////////////////////
        //                                             //
        // Check tracks in each detector independently //
        //                                             //
        /////////////////////////////////////////////////

        //
        // Forward TPC 1 tracks
        //
        
	cout << "# of Forward TPC 1 tracks: " << VecForwTPC1_TrackID->size() << endl;

        for(int itrk=0;itrk<(int)VecForwTPC1_TrackID->size();itrk++){
            
            int id = VecForwTPC1_TrackID->at(itrk);
            double costheta = VecForwTPC1_TrackCosTheta->at(itrk);
            double momX = VecForwTPC1_TrackMomX->at(itrk);
            double momY = VecForwTPC1_TrackMomY->at(itrk);
            double momZ = VecForwTPC1_TrackMomZ->at(itrk);
            double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
            double length = VecForwTPC1_TrackLength->at(itrk);
            double deltaLyz = VecForwTPC1_TrackDeltaLyz->at(itrk);
            double edep = VecForwTPC1_TrackEdep->at(itrk);
            double charge = VecForwTPC1_TrackCharge->at(itrk);
            double pdg = VecForwTPC1_TrackPDG->at(itrk);
            double posX = VecForwTPC1_TrackStepFirstX->at(itrk);
            double posY = VecForwTPC1_TrackStepFirstY->at(itrk);
            double posZ = VecForwTPC1_TrackStepFirstZ->at(itrk);
            double posLastX = VecForwTPC1_TrackStepLastX->at(itrk);
            double posLastY = VecForwTPC1_TrackStepLastY->at(itrk);
            double posLastZ = VecForwTPC1_TrackStepLastZ->at(itrk);
	    
	    //cout << "id = " << id << endl;
            
            if(doSelPDG && pdg!=SetPDG) continue;
            
            // Cut 2: Select tracks with dLyz > 200mm
            if(doCut_dL && deltaLyz < 200) continue;
            
            // Cut 3: Select charged particles
            if(doCut_poscharge && fabs(charge)!=1) continue;
            
            if(deltaLyz < 950 || deltaLyz > 1050) continue;
            //if(posZ < 1199) continue;
            
            // Cut 4: Select highest momentum track
            //if(mom > ForwTPC1_mom){
	    ForwTPC1_ID = id;
	    ForwTPC1_mom = mom;
	    ForwTPC1_costheta = costheta;
	    ForwTPC1_len = length;
	    ForwTPC1_dlyz = deltaLyz;
	    ForwTPC1_charge = charge;
	    ForwTPC1_edep = edep;
	    ForwTPC1_pdg = pdg;
	    ForwTPC1_PosIniX = posX;
	    ForwTPC1_PosIniY = posY;
	    ForwTPC1_PosIniZ = posZ;
	    ForwTPC1_PosFinX = posLastX;
	    ForwTPC1_PosFinY = posLastY;
	    ForwTPC1_PosFinZ = posLastZ;
            //}
	    
	    if(doPrintTrack){
	      cout << "TPC Up 1 track: " << " ";
	      cout << "id = " << id << ", ";
	      cout << "event = " << ientry << ", ";
	      cout << "mom = " << mom << ", ";
	      cout << "deltaLyz = " << deltaLyz << ", ";
	      cout << "charge = " << charge << ", ";
	      cout << "edep = " << edep << ", ";
	      cout << "pdg = " << pdg << endl;
	      cout << "Last x = " << posLastX << endl;
	      cout << "Last y = " << posLastY << endl;
	      cout << "Last z = " << posLastZ << endl;
            }
        } // end TPC Up 1 tracks



        
        //
        // TPC Up 1 tracks
        //
        
        for(int itrk=0;itrk<(int)VecTPCUp1_TrackID->size();itrk++){
            
            int id = VecTPCUp1_TrackID->at(itrk);
            double costheta = VecTPCUp1_TrackCosTheta->at(itrk);
            double momX = VecTPCUp1_TrackMomX->at(itrk);
            double momY = VecTPCUp1_TrackMomY->at(itrk);
            double momZ = VecTPCUp1_TrackMomZ->at(itrk);
            double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
            double length = VecTPCUp1_TrackLength->at(itrk);
            double deltaLyz = VecTPCUp1_TrackDeltaLyz->at(itrk);
            double edep = VecTPCUp1_TrackEdep->at(itrk);
            double charge = VecTPCUp1_TrackCharge->at(itrk);
            double pdg = VecTPCUp1_TrackPDG->at(itrk);
            double posX = VecTPCUp1_TrackStepFirstX->at(itrk);
            double posY = VecTPCUp1_TrackStepFirstY->at(itrk);
            double posZ = VecTPCUp1_TrackStepFirstZ->at(itrk);
            double posLastX = VecTPCUp1_TrackStepLastX->at(itrk);
            double posLastY = VecTPCUp1_TrackStepLastY->at(itrk);
            double posLastZ = VecTPCUp1_TrackStepLastZ->at(itrk);

            if(doSelPDG && pdg!=SetPDG) continue;
            
            // Cut 2: Select tracks with dLyz > 200mm
            if(doCut_dL && deltaLyz < 200) continue;
            
            // Cut 3: Select charged particles
            if(doCut_poscharge && fabs(charge)!=1) continue;
            
            if(deltaLyz < 950 || deltaLyz > 1050) continue;
            //if(posZ < 1199) continue;
            
            // Cut 4: Select highest momentum track
            //if(mom > TPCUp_mom){
	    TPCUp1_ID = id;
	    TPCUp1_mom = mom;
	    TPCUp1_costheta = costheta;
	    TPCUp1_len = length;
	    TPCUp1_dlyz = deltaLyz;
	    TPCUp1_charge = charge;
	    TPCUp1_edep = edep;
	    TPCUp1_pdg = pdg;
	    TPCUp1_PosIniX = posX;
	    TPCUp1_PosIniY = posY;
	    TPCUp1_PosIniZ = posZ;
	    TPCUp1_PosFinX = posLastX;
	    TPCUp1_PosFinY = posLastY;
	    TPCUp1_PosFinZ = posLastZ;
            //}
	    
	    if(doPrintTrack){
	      cout << "TPC Up 1 track: " << " ";
	      cout << "id = " << id << ", ";
	      cout << "event = " << ientry << ", ";
	      cout << "mom = " << mom << ", ";
	      cout << "deltaLyz = " << deltaLyz << ", ";
	      cout << "charge = " << charge << ", ";
	      cout << "edep = " << edep << ", ";
	      cout << "pdg = " << pdg << endl;
	      cout << "Last x = " << posLastX << endl;
	      cout << "Last y = " << posLastY << endl;
	      cout << "Last z = " << posLastZ << endl;
            }
        } // end TPC Up 1 tracks




        //
        // TPC Up 2 tracks
        //
        
        for(int itrk=0;itrk<(int)VecTPCUp2_TrackID->size();itrk++){
            
            int id = VecTPCUp2_TrackID->at(itrk);
            double costheta = VecTPCUp2_TrackCosTheta->at(itrk);
            double momX = VecTPCUp2_TrackMomX->at(itrk);
            double momY = VecTPCUp2_TrackMomY->at(itrk);
            double momZ = VecTPCUp2_TrackMomZ->at(itrk);
            double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
            double length = VecTPCUp2_TrackLength->at(itrk);
            double deltaLyz = VecTPCUp2_TrackDeltaLyz->at(itrk);
            double edep = VecTPCUp2_TrackEdep->at(itrk);
            double charge = VecTPCUp2_TrackCharge->at(itrk);
            double pdg = VecTPCUp2_TrackPDG->at(itrk);
            double posX = VecTPCUp2_TrackStepFirstX->at(itrk);
            double posY = VecTPCUp2_TrackStepFirstY->at(itrk);
            double posZ = VecTPCUp2_TrackStepFirstZ->at(itrk);
            double posLastX = VecTPCUp2_TrackStepLastX->at(itrk);
            double posLastY = VecTPCUp2_TrackStepLastY->at(itrk);
            double posLastZ = VecTPCUp2_TrackStepLastZ->at(itrk);

            if(doSelPDG && pdg!=SetPDG) continue;
            
            // Cut 2: Select tracks with dLyz > 200mm
            if(doCut_dL && deltaLyz < 200) continue;
            
            // Cut 3: Select charged particles
            if(doCut_poscharge && fabs(charge)!=1) continue;
            
            if(deltaLyz < 950 || deltaLyz > 1050) continue;
            //if(posZ < 1199) continue;
            
            // Cut 4: Select highest momentum track
            //if(mom > TPCUp_mom){
	    TPCUp2_ID = id;
	    TPCUp2_mom = mom;
	    TPCUp2_costheta = costheta;
	    TPCUp2_len = length;
	    TPCUp2_dlyz = deltaLyz;
	    TPCUp2_charge = charge;
	    TPCUp2_edep = edep;
	    TPCUp2_pdg = pdg;
	    TPCUp2_PosIniX = posX;
	    TPCUp2_PosIniY = posY;
	    TPCUp2_PosIniZ = posZ;
	    TPCUp2_PosFinX = posLastX;
	    TPCUp2_PosFinY = posLastY;
	    TPCUp2_PosFinZ = posLastZ;
            //}
	    
	    if(doPrintTrack){
	      cout << "TPC Up 2 track: " << " ";
	      cout << "id = " << id << ", ";
	      cout << "event = " << ientry << ", ";
	      cout << "mom = " << mom << ", ";
	      cout << "deltaLyz = " << deltaLyz << ", ";
	      cout << "charge = " << charge << ", ";
	      cout << "edep = " << edep << ", ";
	      cout << "pdg = " << pdg << endl;
	      cout << "Last x = " << posLastX << endl;
	      cout << "Last y = " << posLastY << endl;
	      cout << "Last z = " << posLastZ << endl;
            }
        } // end TPC Up 2 tracks



        //
        // TPC Down 1 tracks
        //
        
        for(int itrk=0;itrk<(int)VecTPCDown1_TrackID->size();itrk++){
	  
	  int id = VecTPCDown1_TrackID->at(itrk);
            double costheta = VecTPCDown1_TrackCosTheta->at(itrk);
            double momX = VecTPCDown1_TrackMomX->at(itrk);
            double momY = VecTPCDown1_TrackMomY->at(itrk);
            double momZ = VecTPCDown1_TrackMomZ->at(itrk);
            double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
            double length = VecTPCDown1_TrackLength->at(itrk);
            double deltaLyz = VecTPCDown1_TrackDeltaLyz->at(itrk);
            double edep = VecTPCDown1_TrackEdep->at(itrk);
            double charge = VecTPCDown1_TrackCharge->at(itrk);
            double pdg = VecTPCDown1_TrackPDG->at(itrk);
            double posX = VecTPCDown1_TrackStepFirstX->at(itrk);
            double posY = VecTPCDown1_TrackStepFirstY->at(itrk);
            double posZ = VecTPCDown1_TrackStepFirstZ->at(itrk);
            double posLastX = VecTPCDown1_TrackStepLastX->at(itrk);
            double posLastY = VecTPCDown1_TrackStepLastY->at(itrk);
            double posLastZ = VecTPCDown1_TrackStepLastZ->at(itrk);
            
            if(doSelPDG && pdg!=SetPDG) continue;
            
            // Cut 2: Select tracks with dLyz > 200mm
            if(doCut_dL && deltaLyz < 200) continue;
            
            // Cut 3: Select charged particles
            if(doCut_poscharge && fabs(charge)!=1) continue;
            
            //if(deltaLyz < 950 || deltaLyz > 1050) continue;
            
            // Cut 4: Select highest momentum track
            //if(mom > TPCDown_mom){
            TPCDown1_ID = id;
            TPCDown1_mom = mom;
            TPCDown1_costheta = costheta;
            TPCDown1_len = length;
            TPCDown1_dlyz = deltaLyz;
            TPCDown1_charge = charge;
            TPCDown1_edep = edep;
            TPCDown1_pdg = pdg;
            TPCDown1_PosIniX = posX;
            TPCDown1_PosIniY = posY;
            TPCDown1_PosIniZ = posZ;
            TPCDown1_PosFinX = posLastX;
            TPCDown1_PosFinY = posLastY;
            TPCDown1_PosFinZ = posLastZ;
            //}
            
            if(doPrintTrack){
                cout << "TPC Down 1 track: " << " ";
                cout << "id = " << id << ", ";
                cout << "event = " << ientry << ", ";
                cout << "mom = " << mom << ", ";
                cout << "deltaLyz = " << deltaLyz << ", ";
                cout << "charge = " << charge << ", ";
                cout << "edep = " << edep << ", ";
                cout << "pdg = " << pdg << endl;
                cout << "Last x = " << posLastX << endl;
                cout << "Last y = " << posLastY << endl;
                cout << "Last z = " << posLastZ << endl;    
            }
            
            hTPCDown1_CosTheta->Fill(TPCDown1_costheta);
            hTPCDown1_Mom   ->Fill(TPCDown1_mom);
            hTPCDown1_Length->Fill(TPCDown1_len);
            hTPCDown1_DeltaLyz->Fill(TPCDown1_dlyz);
            hTPCDown1_Charge->Fill(TPCDown1_charge);
            hTPCDown1_PDG   ->Fill(TPCDown1_pdg);
            hTPCDown1_Edep  ->Fill(TPCDown1_edep);
            hTPCDown1_PosIniX->Fill(TPCDown1_PosIniX);
            hTPCDown1_PosIniY->Fill(TPCDown1_PosIniY);
            hTPCDown1_PosIniZ->Fill(TPCDown1_PosIniZ);
            hTPCDown1_PosFinX->Fill(TPCDown1_PosFinX);
            hTPCDown1_PosFinY->Fill(TPCDown1_PosFinY);
            hTPCDown1_PosFinZ->Fill(TPCDown1_PosFinZ);
            LastStepYdir.push_back(TPCDown1_PosFinY);
            LastStepZdir.push_back(TPCDown1_PosFinZ);
            
        } // end TPC Down 1 tracks







        //
        // TPC Down 2 tracks
        //
        
        for(int itrk=0;itrk<(int)VecTPCDown2_TrackID->size();itrk++){
	  
	  int id = VecTPCDown2_TrackID->at(itrk);
            double costheta = VecTPCDown2_TrackCosTheta->at(itrk);
            double momX = VecTPCDown2_TrackMomX->at(itrk);
            double momY = VecTPCDown2_TrackMomY->at(itrk);
            double momZ = VecTPCDown2_TrackMomZ->at(itrk);
            double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
            double length = VecTPCDown2_TrackLength->at(itrk);
            double deltaLyz = VecTPCDown2_TrackDeltaLyz->at(itrk);
            double edep = VecTPCDown2_TrackEdep->at(itrk);
            double charge = VecTPCDown2_TrackCharge->at(itrk);
            double pdg = VecTPCDown2_TrackPDG->at(itrk);
            double posX = VecTPCDown2_TrackStepFirstX->at(itrk);
            double posY = VecTPCDown2_TrackStepFirstY->at(itrk);
            double posZ = VecTPCDown2_TrackStepFirstZ->at(itrk);
            double posLastX = VecTPCDown2_TrackStepLastX->at(itrk);
            double posLastY = VecTPCDown2_TrackStepLastY->at(itrk);
            double posLastZ = VecTPCDown2_TrackStepLastZ->at(itrk);
            
            if(doSelPDG && pdg!=SetPDG) continue;
            
            // Cut 2: Select tracks with dLyz > 200mm
            if(doCut_dL && deltaLyz < 200) continue;
            
            // Cut 3: Select charged particles
            if(doCut_poscharge && fabs(charge)!=1) continue;
            
            //if(deltaLyz < 950 || deltaLyz > 1050) continue;
            
            // Cut 4: Select highest momentum track
            //if(mom > TPCDown_mom){
            TPCDown2_ID = id;
            TPCDown2_mom = mom;
            TPCDown2_costheta = costheta;
            TPCDown2_len = length;
            TPCDown2_dlyz = deltaLyz;
            TPCDown2_charge = charge;
            TPCDown2_edep = edep;
            TPCDown2_pdg = pdg;
            TPCDown2_PosIniX = posX;
            TPCDown2_PosIniY = posY;
            TPCDown2_PosIniZ = posZ;
            TPCDown2_PosFinX = posLastX;
            TPCDown2_PosFinY = posLastY;
            TPCDown2_PosFinZ = posLastZ;
            //}
            
            if(doPrintTrack){
                cout << "TPC Down 2 track: " << " ";
                cout << "id = " << id << ", ";
                cout << "event = " << ientry << ", ";
                cout << "mom = " << mom << ", ";
                cout << "deltaLyz = " << deltaLyz << ", ";
                cout << "charge = " << charge << ", ";
                cout << "edep = " << edep << ", ";
                cout << "pdg = " << pdg << endl;
                cout << "Last x = " << posLastX << endl;
                cout << "Last y = " << posLastY << endl;
                cout << "Last z = " << posLastZ << endl;    
            }
            
            hTPCDown2_CosTheta->Fill(TPCDown2_costheta);
            hTPCDown2_Mom   ->Fill(TPCDown2_mom);
            hTPCDown2_Length->Fill(TPCDown2_len);
            hTPCDown2_DeltaLyz->Fill(TPCDown2_dlyz);
            hTPCDown2_Charge->Fill(TPCDown2_charge);
            hTPCDown2_PDG   ->Fill(TPCDown2_pdg);
            hTPCDown2_Edep  ->Fill(TPCDown2_edep);
            hTPCDown2_PosIniX->Fill(TPCDown2_PosIniX);
            hTPCDown2_PosIniY->Fill(TPCDown2_PosIniY);
            hTPCDown2_PosIniZ->Fill(TPCDown2_PosIniZ);
            hTPCDown2_PosFinX->Fill(TPCDown2_PosFinX);
            hTPCDown2_PosFinY->Fill(TPCDown2_PosFinY);
            hTPCDown2_PosFinZ->Fill(TPCDown2_PosFinZ);
            LastStepYdir.push_back(TPCDown2_PosFinY);
            LastStepZdir.push_back(TPCDown2_PosFinZ);
            
        } // end TPC Down 2 tracks




        //
        // Target 1 tracks
        //
        
        for(int itrk=0;itrk<(int)VecTarget1_TrackID->size();itrk++){
            
            int id = VecTarget1_TrackID->at(itrk);
            double costheta = VecTarget1_TrackCosTheta->at(itrk);
            double momX = VecTarget1_TrackMomX->at(itrk);
            double momY = VecTarget1_TrackMomY->at(itrk);
            double momZ = VecTarget1_TrackMomZ->at(itrk);
            double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
            double length = VecTarget1_TrackLength->at(itrk);
            double deltaLyz = VecTarget1_TrackDeltaLyz->at(itrk);
            double edep = VecTarget1_TrackEdep->at(itrk);
            double charge = VecTarget1_TrackCharge->at(itrk);
            double pdg = VecTarget1_TrackPDG->at(itrk);
            double posX = VecTarget1_TrackStepFirstX->at(itrk);
            double posY = VecTarget1_TrackStepFirstY->at(itrk);
            double posZ = VecTarget1_TrackStepFirstZ->at(itrk);
            double posLastX = VecTarget1_TrackStepLastX->at(itrk);
            double posLastY = VecTarget1_TrackStepLastY->at(itrk);
            double posLastZ = VecTarget1_TrackStepLastZ->at(itrk);
            
            if(doSelPDG && pdg!=SetPDG) continue;
            
            // Cut 2: Select tracks with dLyz > 50mm
            if(doCut_dL && deltaLyz < 50) continue;
            
            // Cut 3: Select charged particles
            if(doCut_poscharge && fabs(charge)!=1) continue;
            
            // Cut 4: Select highest momentum track
            //if(mom > Target_mom){
            Target1_ID = id;
            Target1_costheta = costheta;
            Target1_mom = mom;
            Target1_len = length;
            Target1_dlyz = deltaLyz;
            Target1_charge = charge;
            Target1_edep = edep;
            Target1_pdg = pdg;
            Target1_PosIniX = posX;
            Target1_PosIniY = posY;
            Target1_PosIniZ = posZ;
            Target1_PosFinX = posLastX;
            Target1_PosFinY = posLastY;
            Target1_PosFinZ = posLastZ;
            //}
            
            if(doPrintTrack){
             cout << "Target 1 track: " << " ";
             cout << "id = " << id << ", ";
             cout << "event = " << ientry << ", ";
             cout << "mom = " << mom << ", ";
             cout << "deltaLyz = " << deltaLyz << ", ";
             cout << "charge = " << charge << ", ";
             cout << "edep = " << edep << ", ";
             cout << "pdg = " << pdg << endl;
             }
            
            hTarget1_CosTheta->Fill(Target1_costheta);
            hTarget1_Mom   ->Fill(Target1_mom);
            hTarget1_Length->Fill(Target1_len);
            hTarget1_DeltaLyz->Fill(Target1_dlyz);
            hTarget1_Charge->Fill(Target1_charge);
            hTarget1_PDG   ->Fill(Target1_pdg);
            hTarget1_Edep  ->Fill(Target1_edep);
            hTarget1_PosIniX->Fill(Target1_PosIniX);
            hTarget1_PosIniY->Fill(Target1_PosIniY);
            hTarget1_PosIniZ->Fill(Target1_PosIniZ);
            hTarget1_PosFinX->Fill(Target1_PosFinX);
            hTarget1_PosFinY->Fill(Target1_PosFinY);
            hTarget1_PosFinZ->Fill(Target1_PosFinZ);
            
            hVtxMinusPosIni_Target1_X->Fill(vtx_x-Target1_PosIniX);
            hVtxMinusPosIni_Target1_Y->Fill(vtx_y-Target1_PosIniY);
            hVtxMinusPosIni_Target1_Z->Fill(vtx_z-Target1_PosIniZ);
            
        } // end Target 1 tracks


        //
        // Target 2 tracks
        //
        
        for(int itrk=0;itrk<(int)VecTarget2_TrackID->size();itrk++){
	  
            int id = VecTarget2_TrackID->at(itrk);
            double costheta = VecTarget2_TrackCosTheta->at(itrk);
            double momX = VecTarget2_TrackMomX->at(itrk);
            double momY = VecTarget2_TrackMomY->at(itrk);
            double momZ = VecTarget2_TrackMomZ->at(itrk);
            double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
            double length = VecTarget2_TrackLength->at(itrk);
            double deltaLyz = VecTarget2_TrackDeltaLyz->at(itrk);
            double edep = VecTarget2_TrackEdep->at(itrk);
            double charge = VecTarget2_TrackCharge->at(itrk);
            double pdg = VecTarget2_TrackPDG->at(itrk);
            double posX = VecTarget2_TrackStepFirstX->at(itrk);
            double posY = VecTarget2_TrackStepFirstY->at(itrk);
            double posZ = VecTarget2_TrackStepFirstZ->at(itrk);
            double posLastX = VecTarget2_TrackStepLastX->at(itrk);
            double posLastY = VecTarget2_TrackStepLastY->at(itrk);
            double posLastZ = VecTarget2_TrackStepLastZ->at(itrk);
            
            if(doSelPDG && pdg!=SetPDG) continue;
            
            // Cut 2: Select tracks with dLyz > 50mm
            if(doCut_dL && deltaLyz < 50) continue;
            
            // Cut 3: Select charged particles
            if(doCut_poscharge && fabs(charge)!=1) continue;
            
            // Cut 4: Select highest momentum track
            //if(mom > Target_mom){
            Target2_ID = id;
            Target2_costheta = costheta;
            Target2_mom = mom;
            Target2_len = length;
            Target2_dlyz = deltaLyz;
            Target2_charge = charge;
            Target2_edep = edep;
            Target2_pdg = pdg;
            Target2_PosIniX = posX;
            Target2_PosIniY = posY;
            Target2_PosIniZ = posZ;
            Target2_PosFinX = posLastX;
            Target2_PosFinY = posLastY;
            Target2_PosFinZ = posLastZ;
            //}
            
            if(doPrintTrack){
             cout << "Target 2 track: " << " ";
             cout << "id = " << id << ", ";
             cout << "event = " << ientry << ", ";
             cout << "mom = " << mom << ", ";
             cout << "deltaLyz = " << deltaLyz << ", ";
             cout << "charge = " << charge << ", ";
             cout << "edep = " << edep << ", ";
             cout << "pdg = " << pdg << endl;
             }
            
            hTarget2_CosTheta->Fill(Target2_costheta);
            hTarget2_Mom   ->Fill(Target2_mom);
            hTarget2_Length->Fill(Target2_len);
            hTarget2_DeltaLyz->Fill(Target2_dlyz);
            hTarget2_Charge->Fill(Target2_charge);
            hTarget2_PDG   ->Fill(Target2_pdg);
            hTarget2_Edep  ->Fill(Target2_edep);
            hTarget2_PosIniX->Fill(Target2_PosIniX);
            hTarget2_PosIniY->Fill(Target2_PosIniY);
            hTarget2_PosIniZ->Fill(Target2_PosIniZ);
            hTarget2_PosFinX->Fill(Target2_PosFinX);
            hTarget2_PosFinY->Fill(Target2_PosFinY);
            hTarget2_PosFinZ->Fill(Target2_PosFinZ);
            
            hVtxMinusPosIni_Target2_X->Fill(vtx_x-Target2_PosIniX);
            hVtxMinusPosIni_Target2_Y->Fill(vtx_y-Target2_PosIniY);
            hVtxMinusPosIni_Target2_Z->Fill(vtx_z-Target2_PosIniZ);
            
        } // end Target 2 tracks
        


        
        
        // Print out the highest momentum track
        
        if(TPCUp1_mom>0.){
            /*
             if(doPrintTrack){
             cout << "TPC Up high mom track: " << " ";
             cout << "event = " << ientry << ", ";
             cout << "id = " << TPCUp_ID << ", ";
             cout << "mom = " << TPCUp_mom << ", ";
             cout << "length = " << TPCUp_len << ", ";
             cout << "charge = " << TPCUp_charge << ", ";
             cout << "edep = " << TPCUp_edep << ", ";
             cout << "pdg = " << TPCUp_pdg << endl;
             }
             
             hTPCUp_CosTheta->Fill(TPCUp_costheta);
             hTPCUp_Mom   ->Fill(TPCUp_mom);
             hTPCUp_Length->Fill(TPCUp_len);
             hTPCUp_DeltaLyz->Fill(TPCUp_dlyz);
             hTPCUp_Charge->Fill(TPCUp_charge);
             hTPCUp_PDG   ->Fill(TPCUp_pdg);
             hTPCUp_Edep  ->Fill(TPCUp_edep);
             hTPCUp_PosIniX->Fill(TPCUp_PosIniX);
             hTPCUp_PosIniY->Fill(TPCUp_PosIniY);
             hTPCUp_PosIniZ->Fill(TPCUp_PosIniZ);
             hTPCUp_PosFinX->Fill(TPCUp_PosFinX);
             hTPCUp_PosFinY->Fill(TPCUp_PosFinY);
             hTPCUp_PosFinZ->Fill(TPCUp_PosFinZ);
	    */
        }
	if(TPCUp2_mom>0.){
        }

        if(TPCDown1_mom>0.){
            
            /*if(doPrintTrack){
             cout << "TPC Down high mom track: " << " ";
             cout << "event = " << ientry << ", ";
             cout << "id = " << TPCDown_ID << ", ";
             cout << "mom = " << TPCDown_mom << ", ";
             cout << "length = " << TPCDown_len << ", ";
             cout << "charge = " << TPCDown_charge << ", ";
             cout << "edep = " << TPCDown_edep << ", ";
             cout << "pdg = " << TPCDown_pdg << endl;
             }
             
             hTPCDown_CosTheta->Fill(TPCDown_costheta);
             hTPCDown_Mom   ->Fill(TPCDown_mom);
             hTPCDown_Length->Fill(TPCDown_len);
             hTPCDown_DeltaLyz->Fill(TPCDown_dlyz);
             hTPCDown_Charge->Fill(TPCDown_charge);
             hTPCDown_PDG   ->Fill(TPCDown_pdg);
             hTPCDown_Edep  ->Fill(TPCDown_edep);
             hTPCDown_PosIniX->Fill(TPCDown_PosIniX);
             hTPCDown_PosIniY->Fill(TPCDown_PosIniY);
             hTPCDown_PosIniZ->Fill(TPCDown_PosIniZ);
             hTPCDown_PosFinX->Fill(TPCDown_PosFinX);
             hTPCDown_PosFinY->Fill(TPCDown_PosFinY);
             hTPCDown_PosFinZ->Fill(TPCDown_PosFinZ);*/
        }
	if(TPCDown2_mom>0.){
        }
	
        if(Target1_mom>0.){
            
            /*if(doPrintTrack){
             cout << "Target high mom track: " << " ";
             cout << "event = " << ientry << ", ";
             cout << "id = " << Target_ID << ", ";
             cout << "mom = " << Target_mom << ", ";
             cout << "length = " << Target_len << ", ";
             cout << "charge = " << Target_charge << ", ";
             cout << "edep = " << Target_edep << ", ";
             cout << "pdg = " << Target_pdg << endl;
             }
             
             hTarget_CosTheta->Fill(Target_costheta);
             hTarget_Mom   ->Fill(Target_mom);
             hTarget_Length->Fill(Target_len);
             hTarget_DeltaLyz->Fill(Target_dlyz);
             hTarget_Charge->Fill(Target_charge);
             hTarget_PDG   ->Fill(Target_pdg);
             hTarget_Edep  ->Fill(Target_edep);
             hTarget_PosIniX->Fill(Target_PosIniX);
             hTarget_PosIniY->Fill(Target_PosIniY);
             hTarget_PosIniZ->Fill(Target_PosIniZ);
             hTarget_PosFinX->Fill(Target_PosFinX);
             hTarget_PosFinY->Fill(Target_PosFinY);
             hTarget_PosFinZ->Fill(Target_PosFinZ);
             
             hVtxMinusPosIni_Target_X->Fill(vtx_x-Target_PosIniX);
             hVtxMinusPosIni_Target_Y->Fill(vtx_y-Target_PosIniY);
             hVtxMinusPosIni_Target_Z->Fill(vtx_z-Target_PosIniZ);*/
            
            // if( fabs(vtx_x-Target_PosIniX) > 0.){
            // 	cout << "Vtx - Target first step pos:"
            // 	     << " x = " << vtx_x-Target_PosIniX
            // 	     << " y = " << vtx_y-Target_PosIniY
            // 	     << " z = " << vtx_z-Target_PosIniZ
            // 	     << " - Vtx pos:"
            // 	     << " x = " << vtx_x
            // 	     << " y = " << vtx_y
            // 	     << " z = " << vtx_z
            // 	     << " - Vtx reac mode: " << VecVtx_ReacMode->at(0)
            // 	     << " - High track: "
            // 	     << " mom = " << Target_mom
            // 	     << " costheta = " << Target_costheta
            // 	     << " pdg = " << Target_pdg
            // 	     << endl;
            // }
        }
	if(Target2_mom>0.){
	}
    } // end loop over events
    
    
    // Store the Tree
    //treeoa->Write();
    //fileoa->Close();


    
    exit(1);
    
    
    /*
    
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
    
    */
    
    /*double lsY[LastStepYdir.size()];
    std::copy(LastStepYdir.begin(), LastStepYdir.end(), lsY);
    double lsZ[LastStepZdir.size()];
    std::copy(LastStepZdir.begin(), LastStepZdir.end(), lsZ);
    TGraph* LastStepYZ = new TGraph(LastStepYdir.size(),lsY,lsZ);
    //TGraph* LastStepYZ = new TGraph(LastStepYdir.size(),&LastStepYdir[0],&LastStepZdir[0]);
    
    TCanvas *LastStepYZ_canvas = new TCanvas("LastStepYZ","LastStepYZ");
    LastStepYZ->Draw("AP");*/
    
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
    
    /*
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


    */
    
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
