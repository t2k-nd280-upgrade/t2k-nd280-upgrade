
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
    const char *filename = "../bin/EffStudy.root";
    //const char *filename = "../bin/EffStudy.root",
    const bool doprint = false;
    
    // Vertex cut (inside the Target)
    const bool doCut_Vtx = true;
    const double vtx_min_x_1 = -1150;  //mm
    const double vtx_max_x_1 = +1150;  //mm
    const double vtx_min_y_1 = -151.5; //mm
    const double vtx_max_y_1 = +151.1; //mm
    const double vtx_min_z_1 = +487;  //mm
    const double vtx_max_z_1 = +2487;  //mm
    
    const double vtx_min_x_2 = -1150;  //mm
    const double vtx_max_x_2 = +1150;  //mm
    const double vtx_min_y_2 = -151.5; //mm
    const double vtx_max_y_2 = +151.1; //mm
    const double vtx_min_z_2 = -2487;  //mm
    const double vtx_max_z_2 = -487;  //mm
	
    // DeltaLyz cut (TPCs and Target)
    const bool doCut_dL = true; // full length in target and dlyz in tpcs
    const double len_Target1_min = 50; //mm
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
    
    
    double pbins[37]= {0.,50.,100.,150.,200.,250.,300.,350.,400.,450.,500.,550.,600.,650.,700.,750.,800.,850.,900.,950.,1000.,1050.,1100.,1150.,1250.,1350.,1500.,1750.,2000.,2500.,3000.,4000.,5000.,7500.,10000.,20000.,30000.};
    double thetabins[27]={-1,-0.7,-0.5,-0.2,-0.1,0.,0.1,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.92,0.94,0.98,0.99,1};
    
    //Efficiencies
	
	//For target 1 + 2
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
	
	//For target 1 only
    TH2D *Denom_1 = new TH2D("Denom_1","Denom_1",26,thetabins,36,pbins);
    //Target only
    TH2D *Target_all_1 = new TH2D("Target_all_1","Target_all_1",26,thetabins,36,pbins);
    //Target only
    TH2D *Target_only_1 = new TH2D("Target_only_1","Target_only_1",26,thetabins,36,pbins);
    //Target+TPC
    TH2D *Target_TPC_1 = new TH2D("Target_TPC_1","Target_TPC_1",26,thetabins,36,pbins);
    //TPC only
    TH2D *TPC_only_1 = new TH2D("TPC_only_1","TPC_only_1",26,thetabins,36,pbins);   
    //TPC only or Target only or Target + TPC
    TH2D *All_topology_1 = new TH2D("All_topology_1","All_topology_1",26,thetabins,36,pbins);    
    //TPC only or Target + TPC
    TH2D *All_topology2_1 = new TH2D("All_topology2_1","All_topology2_1",26,thetabins,36,pbins);
	
	//For target 2 only
    TH2D *Denom_2 = new TH2D("Denom_2","Denom_2",26,thetabins,36,pbins);
    //Target only
    TH2D *Target_all_2 = new TH2D("Target_all_2","Target_all_2",26,thetabins,36,pbins);
    //Target only
    TH2D *Target_only_2 = new TH2D("Target_only_2","Target_only_2",26,thetabins,36,pbins);
    //Target+TPC
    TH2D *Target_TPC_2 = new TH2D("Target_TPC_2","Target_TPC_2",26,thetabins,36,pbins);
    //TPC only
    TH2D *TPC_only_2 = new TH2D("TPC_only_2","TPC_only_2",26,thetabins,36,pbins);   
    //TPC only or Target only or Target + TPC
    TH2D *All_topology_2 = new TH2D("All_topology_2","All_topology_2",26,thetabins,36,pbins);    
    //TPC only or Target + TPC
    TH2D *All_topology2_2 = new TH2D("All_topology2_2","All_topology2_2",26,thetabins,36,pbins);
	
	
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
    double EabsForwTPC2=0.;
    double EabsForwTPC3=0.;
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
    
    // Forward TPC 2 informations
    int     ForwTPC2_NTracks;          // # of tracks in Forward TPC 2
    std::vector<int>   * VecForwTPC2_TrackID = 0; // Vector of trackID in Forw TPC 2
    std::vector<int>   * VecForwTPC2_ParentID = 0; // Vector of parentID in Forw TPC 2
    std::vector<double>* VecForwTPC2_TrackMomX = 0; // Vector of initial mom in Forw TPC 2 (MeV)
    std::vector<double>* VecForwTPC2_TrackMomY = 0; // Vector of initial mom in Forw TPC 2 (MeV)
    std::vector<double>* VecForwTPC2_TrackMomZ = 0; // Vector of initial mom in Forw TPC 2 (MeV)
    std::vector<double>* VecForwTPC2_TrackLength = 0; // Vector of corrected (mult. scatt.) length in Forw TPC 2 (mm)
    std::vector<double>* VecForwTPC2_TrackDeltaLyz = 0; // Vector of geometrical length yz in Forw TPC 2 (mm)
    std::vector<double>* VecForwTPC2_TrackEdep = 0; // Vector of edep in Forw TPC 2 (MeV)
    std::vector<double>* VecForwTPC2_TrackCharge = 0; // Vector of charge in Fow TPC 2 (e-)
    std::vector<int>   * VecForwTPC2_TrackPDG = 0; // Vector of pdg in Forw TPC 2
    std::vector<double> *VecForwTPC2_TrackStepFirstX = 0; // Vector of first step position in Forw TPC 2
    std::vector<double> *VecForwTPC2_TrackStepFirstY = 0; // Vector of first step position in Forw TPC 2
    std::vector<double> *VecForwTPC2_TrackStepFirstZ = 0; // Vector of first step position in Forw TPC 2
    std::vector<double> *VecForwTPC2_TrackStepLastX = 0; // Vector of last step position in Forw TPC 2
    std::vector<double> *VecForwTPC2_TrackStepLastY = 0; // Vector of last step position in Forw TPC 2
    std::vector<double> *VecForwTPC2_TrackStepLastZ = 0; // Vector of last step position in Forw TPC 2
    std::vector<double> *VecForwTPC2_TrackCosTheta = 0; // Vector of cos(theta) wrt nu direction (z axis) in Forw TPC 2
    
    // Forward TPC 3 informations
    int     ForwTPC3_NTracks;          // # of tracks in Forward TPC 3
    std::vector<int>   * VecForwTPC3_TrackID = 0; // Vector of trackID in Forw TPC 3
    std::vector<int>   * VecForwTPC3_ParentID = 0; // Vector of parentID in Forw TPC 3
    std::vector<double>* VecForwTPC3_TrackMomX = 0; // Vector of initial mom in Forw TPC 3 (MeV)
    std::vector<double>* VecForwTPC3_TrackMomY = 0; // Vector of initial mom in Forw TPC 3 (MeV)
    std::vector<double>* VecForwTPC3_TrackMomZ = 0; // Vector of initial mom in Forw TPC 3 (MeV)
    std::vector<double>* VecForwTPC3_TrackLength = 0; // Vector of corrected (mult. scatt.) length in Forw TPC 3 (mm)
    std::vector<double>* VecForwTPC3_TrackDeltaLyz = 0; // Vector of geometrical length yz in Forw TPC 3 (mm)
    std::vector<double>* VecForwTPC3_TrackEdep = 0; // Vector of edep in Forw TPC 3 (MeV)
    std::vector<double>* VecForwTPC3_TrackCharge = 0; // Vector of charge in Fow TPC 3 (e-)
    std::vector<int>   * VecForwTPC3_TrackPDG = 0; // Vector of pdg in Forw TPC 3
    std::vector<double> *VecForwTPC3_TrackStepFirstX = 0; // Vector of first step position in Forw TPC 3
    std::vector<double> *VecForwTPC3_TrackStepFirstY = 0; // Vector of first step position in Forw TPC 3
    std::vector<double> *VecForwTPC3_TrackStepFirstZ = 0; // Vector of first step position in Forw TPC 3
    std::vector<double> *VecForwTPC3_TrackStepLastX = 0; // Vector of last step position in Forw TPC 3
    std::vector<double> *VecForwTPC3_TrackStepLastY = 0; // Vector of last step position in Forw TPC 3
    std::vector<double> *VecForwTPC3_TrackStepLastZ = 0; // Vector of last step position in Forw TPC 3
    std::vector<double> *VecForwTPC3_TrackCosTheta = 0; // Vector of cos(theta) wrt nu direction (z axis) in Forw TPC 3
    
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
    treein->SetBranchAddress("EabsForwTPC2",&EabsForwTPC2);
    treein->SetBranchAddress("EabsForwTPC3",&EabsForwTPC3);
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
    
    treein->SetBranchAddress("ForwTPC2_NTracks",      &ForwTPC2_NTracks);
    treein->SetBranchAddress("VecForwTPC2_TrackID",   &VecForwTPC2_TrackID);
    treein->SetBranchAddress("VecForwTPC2_ParentID",   &VecForwTPC2_ParentID);
    treein->SetBranchAddress("VecForwTPC2_TrackMomX", &VecForwTPC2_TrackMomX);
    treein->SetBranchAddress("VecForwTPC2_TrackMomY", &VecForwTPC2_TrackMomY);
    treein->SetBranchAddress("VecForwTPC2_TrackMomZ", &VecForwTPC2_TrackMomZ);
    treein->SetBranchAddress("VecForwTPC2_TrackLength", &VecForwTPC2_TrackLength);
    treein->SetBranchAddress("VecForwTPC2_TrackDeltaLyz", &VecForwTPC2_TrackDeltaLyz);
    treein->SetBranchAddress("VecForwTPC2_TrackEdep", &VecForwTPC2_TrackEdep);
    treein->SetBranchAddress("VecForwTPC2_TrackCharge", &VecForwTPC2_TrackCharge);
    treein->SetBranchAddress("VecForwTPC2_TrackPDG", &VecForwTPC2_TrackPDG);
    treein->SetBranchAddress("VecForwTPC2_TrackStepFirstX",&VecForwTPC2_TrackStepFirstX);
    treein->SetBranchAddress("VecForwTPC2_TrackStepFirstY",&VecForwTPC2_TrackStepFirstY);
    treein->SetBranchAddress("VecForwTPC2_TrackStepFirstZ",&VecForwTPC2_TrackStepFirstZ);
    treein->SetBranchAddress("VecForwTPC2_TrackStepLastX",&VecForwTPC2_TrackStepLastX);
    treein->SetBranchAddress("VecForwTPC2_TrackStepLastY",&VecForwTPC2_TrackStepLastY);
    treein->SetBranchAddress("VecForwTPC2_TrackStepLastZ",&VecForwTPC2_TrackStepLastZ);
    treein->SetBranchAddress("VecForwTPC2_TrackCosTheta",&VecForwTPC2_TrackCosTheta);
    
    treein->SetBranchAddress("ForwTPC3_NTracks",      &ForwTPC3_NTracks);
    treein->SetBranchAddress("VecForwTPC3_TrackID",   &VecForwTPC3_TrackID);
    treein->SetBranchAddress("VecForwTPC3_ParentID",   &VecForwTPC3_ParentID);
    treein->SetBranchAddress("VecForwTPC3_TrackMomX", &VecForwTPC3_TrackMomX);
    treein->SetBranchAddress("VecForwTPC3_TrackMomY", &VecForwTPC3_TrackMomY);
    treein->SetBranchAddress("VecForwTPC3_TrackMomZ", &VecForwTPC3_TrackMomZ);
    treein->SetBranchAddress("VecForwTPC3_TrackLength", &VecForwTPC3_TrackLength);
    treein->SetBranchAddress("VecForwTPC3_TrackDeltaLyz", &VecForwTPC3_TrackDeltaLyz);
    treein->SetBranchAddress("VecForwTPC3_TrackEdep", &VecForwTPC3_TrackEdep);
    treein->SetBranchAddress("VecForwTPC3_TrackCharge", &VecForwTPC3_TrackCharge);
    treein->SetBranchAddress("VecForwTPC3_TrackPDG", &VecForwTPC3_TrackPDG);
    treein->SetBranchAddress("VecForwTPC3_TrackStepFirstX",&VecForwTPC3_TrackStepFirstX);
    treein->SetBranchAddress("VecForwTPC3_TrackStepFirstY",&VecForwTPC3_TrackStepFirstY);
    treein->SetBranchAddress("VecForwTPC3_TrackStepFirstZ",&VecForwTPC3_TrackStepFirstZ);
    treein->SetBranchAddress("VecForwTPC3_TrackStepLastX",&VecForwTPC3_TrackStepLastX);
    treein->SetBranchAddress("VecForwTPC3_TrackStepLastY",&VecForwTPC3_TrackStepLastY);
    treein->SetBranchAddress("VecForwTPC3_TrackStepLastZ",&VecForwTPC3_TrackStepLastZ);
    treein->SetBranchAddress("VecForwTPC3_TrackCosTheta",&VecForwTPC3_TrackCosTheta);
    
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

    double ForwTPC2_ID;
    double ForwTPC2_mom;
    double ForwTPC2_costheta;
    double ForwTPC2_len;
    double ForwTPC2_dlyz;
    double ForwTPC2_charge;
    double ForwTPC2_edep;
    double ForwTPC2_pdg;
    double ForwTPC2_parent_pdg;
    double ForwTPC2_PosIniX;
    double ForwTPC2_PosIniY;
    double ForwTPC2_PosIniZ;
    double ForwTPC2_PosFinX;
    double ForwTPC2_PosFinY;
    double ForwTPC2_PosFinZ;

    double ForwTPC3_ID;
    double ForwTPC3_mom;
    double ForwTPC3_costheta;
    double ForwTPC3_len;
    double ForwTPC3_dlyz;
    double ForwTPC3_charge;
    double ForwTPC3_edep;
    double ForwTPC3_pdg;
    double ForwTPC3_parent_pdg;
    double ForwTPC3_PosIniX;
    double ForwTPC3_PosIniY;
    double ForwTPC3_PosIniZ;
    double ForwTPC3_PosFinX;
    double ForwTPC3_PosFinY;
    double ForwTPC3_PosFinZ;

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
    
	int Which_target;
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
        ForwTPC2_ID = -9999;
        ForwTPC2_mom = -9999;
        ForwTPC2_costheta = -9999;
        ForwTPC2_len = -9999;
        ForwTPC2_dlyz = -9999;
        ForwTPC2_charge = -9999;
        ForwTPC2_edep = -9999;
        ForwTPC2_pdg = -9999;
        ForwTPC2_parent_pdg = -9999;
        ForwTPC2_PosIniX = -9999;
        ForwTPC2_PosIniY = -9999;
        ForwTPC2_PosIniZ = -9999;
        ForwTPC2_PosFinX = -9999;
        ForwTPC2_PosFinY = -9999;
        ForwTPC2_PosFinZ = -9999;
        ForwTPC3_ID = -9999;
        ForwTPC3_mom = -9999;
        ForwTPC3_costheta = -9999;
        ForwTPC3_len = -9999;
        ForwTPC3_dlyz = -9999;
        ForwTPC3_charge = -9999;
        ForwTPC3_edep = -9999;
        ForwTPC3_pdg = -9999;
        ForwTPC3_parent_pdg = -9999;
        ForwTPC3_PosIniX = -9999;
        ForwTPC3_PosIniY = -9999;
        ForwTPC3_PosIniZ = -9999;
        ForwTPC3_PosFinX = -9999;
        ForwTPC3_PosFinY = -9999;
        ForwTPC3_PosFinZ = -9999;
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
        
		Which_target = 0;
		
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
            if((vtx_x < vtx_min_x_1 || vtx_x > vtx_max_x_1) && (vtx_x < vtx_min_x_2 || vtx_x > vtx_max_x_2))  continue;
            if((vtx_y < vtx_min_y_1 || vtx_y > vtx_max_y_1) && (vtx_y < vtx_min_y_2 || vtx_y > vtx_max_y_2))  continue;
            if((vtx_z < vtx_min_z_1 || vtx_z > vtx_max_z_1) && (vtx_z < vtx_min_z_2 || vtx_z > vtx_max_z_2))  continue;
		}      
        
		if((vtx_x > vtx_min_x_1 && vtx_x < vtx_max_x_1) && (vtx_y > vtx_min_y_1 && vtx_y < vtx_max_y_1) && (vtx_z > vtx_min_z_1 && vtx_z < vtx_max_z_1)) Which_target = 1;
		else if((vtx_x > vtx_min_x_2 && vtx_x < vtx_max_x_2) && (vtx_y > vtx_min_y_2 && vtx_y < vtx_max_y_2) && (vtx_z > vtx_min_z_2 && vtx_z < vtx_max_z_2)) Which_target = 2;
		
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
        // if(NTracks!=VecTarget1_TrackID->size()){
        //   cerr << endl;
        //   cerr << "VecTarget1_TrackID->size() != NTracks" << endl;
        //   cerr << "VecTarget1_TrackID->size() = " << VecTarget1_TrackID->size() << endl;
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
        int NStepFirst_ForwTPC2 = VecForwTPC2_TrackStepFirstX->size();
        int NStepLast_ForwTPC2 = VecForwTPC2_TrackStepLastX->size();
        int NStepFirst_ForwTPC3 = VecForwTPC3_TrackStepFirstX->size();
        int NStepLast_ForwTPC3 = VecForwTPC3_TrackStepLastX->size();
        
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
        if(NStepFirst_ForwTPC2 != NStepLast_ForwTPC2){
	  cout << "Size ForwTPC2 first step = " << VecForwTPC2_TrackStepFirstX->size() << endl;
	  cout << "Size ForwTPC2 last step = " << VecForwTPC2_TrackStepLastX->size() << endl;
	  exit(1);
        }
        if(NStepFirst_ForwTPC3 != NStepLast_ForwTPC3){
	  cout << "Size ForwTPC3 first step = " << VecForwTPC3_TrackStepFirstX->size() << endl;
	  cout << "Size ForwTPC3 last step = " << VecForwTPC3_TrackStepLastX->size() << endl;
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
        if(NStepFirst_TPCUp2 != NStepLast_TPCUp2){
	  cout << "Size TPCUp2 first step = " << VecTPCUp2_TrackStepFirstX->size() << endl;
	  cout << "Size TPCUp2 last step = " << VecTPCUp1_TrackStepLastX->size() << endl;
	  exit(1);
        }
        if(NStepFirst_TPCDown2 != NStepLast_TPCDown2){
	  cout << "Size TPCDown2 first step = " << VecTPCDown2_TrackStepFirstX->size() << endl;
	  cout << "Size TPCDown2 last step = " << VecTPCDown1_TrackStepLastX->size() << endl;
	  exit(1);
        }
        if(NStepFirst_Target2 != NStepLast_Target2){
	  cout << "Size Target2 first step = " << VecTarget2_TrackStepFirstX->size() << endl;
	  cout << "Size Target2 last step = " << VecTarget2_TrackStepLastX->size() << endl;
	  exit(1);   
  	    }
        
        //
        // Loop over all tracks
        //
        
        for(int trkid=1;trkid<=NTracks;trkid++){ // loop over track ID (1 to NTracks)
            
            int vecel_1=-999,id_1=-999;
            double costheta_1=-999,momX_1=-999,momY_1=-999,momZ_1=-999,mom_1=-999,length_1=-999,deltaLyz_1=-999,edep_1=-999,charge_1=-999,pdg_1=-999,posX_1=-999,posY_1=-999,posZ_1=-999,posLastX_1=-999,posLastY_1=-999,posLastZ_1=-999;
			
            int vecel_2=-999,id_2=-999;
            double costheta_2=-999,momX_2=-999,momY_2=-999,momZ_2=-999,mom_2=-999,length_2=-999,deltaLyz_2=-999,edep_2=-999,charge_2=-999,pdg_2=-999,posX_2=-999,posY_2=-999,posZ_2=-999,posLastX_2=-999,posLastY_2=-999,posLastZ_2=-999;
			
            int vecel_fwd1=-999,id_fwd1;
            double length_fwd1=-999,deltaLyz_fwd1=-999,edep_fwd1=-999,charge_fwd1=-999,pdg_fwd1=-999;
			
            int vecel_fwd2=-999,id_fwd2;
            double length_fwd2=-999,deltaLyz_fwd2=-999,edep_fwd2=-999,charge_fwd2=-999,pdg_fwd2=-999;
			
            int vecel_fwd3=-999,id_fwd3;
            double length_fwd3=-999,deltaLyz_fwd3=-999,edep_fwd3=-999,charge_fwd3=-999,pdg_fwd3=-999;
			
            int vecel_up1=-999,id_up1;
            double length_up1=-999,deltaLyz_up1=-999,edep_up1=-999,charge_up1=-999,pdg_up1=-999;
			
            int vecel_up2=-999,id_up2;
            double length_up2=-999,deltaLyz_up2=-999,edep_up2=-999,charge_up2=-999,pdg_up2=-999;
			
            int vecel_down1=-999,id_down1;
            double length_down1=-999,deltaLyz_down1=-999,edep_down1=-999,charge_down1=-999,pdg_down1=-999;
            
            int vecel_down2=-999,id_down2;
            double length_down2=-999,deltaLyz_down2=-999,edep_down2=-999,charge_down2=-999,pdg_down2=-999;
			
            //Vertex in Target 1
			if(Which_target == 1){
	            //
	            // Check if reconstructed in Target 1
	            //
	            double check = 0;
	             vecel_1 = FindTrackElem(VecTarget1_TrackID,trkid);
	             if(vecel_1>=0){
	             	 id_1 = VecTarget1_TrackID->at(vecel_1);
	             	 costheta_1 = VecTarget1_TrackCosTheta->at(vecel_1);
	             	 momX_1 = VecTarget1_TrackMomX->at(vecel_1);
	            	 momY_1 = VecTarget1_TrackMomY->at(vecel_1);
	            	 momZ_1 = VecTarget1_TrackMomZ->at(vecel_1);
	            	 mom_1 = sqrt( momX_1*momX_1 + momY_1*momY_1 + momZ_1*momZ_1 );
	            	 deltaLyz_1 = VecTarget1_TrackDeltaLyz->at(vecel_1);
	            	 length_1 = VecTarget1_TrackLength->at(vecel_1);
	            	 edep_1 = VecTarget1_TrackEdep->at(vecel_1);
	            	 charge_1 = VecTarget1_TrackCharge->at(vecel_1);
	            	 pdg_1 = VecTarget1_TrackPDG->at(vecel_1);
             
	            	 // Fill the denominator
	            	 if ( (doSelPDG && pdg_1==SetPDG) ){
						 Denom->Fill(costheta_1, mom_1);
						 Denom_1->Fill(costheta_1, mom_1);
						 check = 1;
					 }
			 	 }
			
	             vecel_up1 = FindTrackElem(VecTPCUp1_TrackID,trkid);
	             if(vecel_up1>=0){
	                 id_up1 = VecTPCUp1_TrackID->at(vecel_up1);
	                 deltaLyz_up1 = VecTPCUp1_TrackDeltaLyz->at(vecel_up1);
	                 length_up1 = VecTPCUp1_TrackLength->at(vecel_up1);
	                 edep_up1 = VecTPCUp1_TrackEdep->at(vecel_up1);
	                 charge_up1 = VecTPCUp1_TrackCharge->at(vecel_up1);
	                 pdg_up1 = VecTPCUp1_TrackPDG->at(vecel_up1);
	            	 if (check == 0 && (doSelPDG && pdg_1==SetPDG) ){
						 Denom->Fill(costheta_1, mom_1);
						 Denom_1->Fill(costheta_1, mom_1);
						 check = 1;
					 }
	             }
             
	             vecel_down1 = FindTrackElem(VecTPCDown1_TrackID,trkid);
	             if(vecel_down1>=0){
	                 id_down1 = VecTPCDown1_TrackID->at(vecel_down1);
	                 deltaLyz_down1 = VecTPCDown1_TrackDeltaLyz->at(vecel_down1);
	                 length_down1 = VecTPCDown1_TrackLength->at(vecel_down1);
	                 edep_down1 = VecTPCDown1_TrackEdep->at(vecel_down1);
	                 charge_down1 = VecTPCDown1_TrackCharge->at(vecel_down1);
	                 pdg_down1 = VecTPCDown1_TrackPDG->at(vecel_down1);
	            	 if (check == 0 && (doSelPDG && pdg_1==SetPDG) ){
						 Denom->Fill(costheta_1, mom_1);
						 Denom_1->Fill(costheta_1, mom_1);
						 check = 1;
					 }
	             }
				 
	             vecel_fwd1 = FindTrackElem(VecForwTPC1_TrackID,trkid);
	             if(vecel_fwd1>=0){
	                 id_fwd1 = VecForwTPC1_TrackID->at(vecel_fwd1);
	                 deltaLyz_fwd1 = VecForwTPC1_TrackDeltaLyz->at(vecel_fwd1);
	                 length_fwd1 = VecForwTPC1_TrackLength->at(vecel_fwd1);
	                 edep_fwd1 = VecForwTPC1_TrackEdep->at(vecel_fwd1);
	                 charge_fwd1 = VecForwTPC1_TrackCharge->at(vecel_fwd1);
	                 pdg_fwd1 = VecForwTPC1_TrackPDG->at(vecel_fwd1);
	            	 if (check == 0 && (doSelPDG && pdg_1==SetPDG) ){
						 Denom->Fill(costheta_1, mom_1);
						 Denom_1->Fill(costheta_1, mom_1);
						 check = 1;
					 }
	             }
             
	             vecel_fwd2 = FindTrackElem(VecForwTPC2_TrackID,trkid);
	             if(vecel_fwd2>=0){
	                 id_fwd2 = VecForwTPC2_TrackID->at(vecel_fwd2);
	                 deltaLyz_fwd2 = VecForwTPC2_TrackDeltaLyz->at(vecel_fwd2);
	                 length_fwd2 = VecForwTPC2_TrackLength->at(vecel_fwd2);
	                 edep_fwd2 = VecForwTPC2_TrackEdep->at(vecel_fwd2);
	                 charge_fwd2 = VecForwTPC2_TrackCharge->at(vecel_fwd2);
	                 pdg_fwd2 = VecForwTPC2_TrackPDG->at(vecel_fwd2);
	            	 if (check == 0 && (doSelPDG && pdg_1==SetPDG) ){
						 Denom->Fill(costheta_1, mom_1);
						 Denom_1->Fill(costheta_1, mom_1);
						 check = 1;
					 }
	             }
			 
   
	             // Cut 1: Select track PDG
	             // Cut 2: Select tracks with dLyz > 50mm
	             // Cut 3: Select charged particles
	             if(vecel_1 >= 0 && (doSelPDG && pdg_1==SetPDG) && (doCut_dL && length_1 > 0.5*len_Target1_min) && (fabs(charge_1)==cut_charge)){
                              
	                 if(doPrintTrack){
	                 cout << "  Track ID = " << trkid << endl;
	                 cout << "     - Target 1 track: " << " ";
	                 cout << "event = " << ientry << ", ";
	                 cout << "mom = " << mom_1 << ", ";
	                 cout << "length = " << length_1 << ", ";
	                 cout << "charge = " << charge_1 << ", ";
	                 cout << "edep = " << edep_1 << ", ";
	                 cout << "pdg = " << pdg_1 << ", ";
	                 cout << endl;
	                 }
                 
	                 //
	                 // Check if reconstructed in TPCUp1 or TPC Down1 or TPC Fwd 1 or 2
	                 //
                 
	                 // Cut 2: Select tracks with dLyz > 200mm
	                 // Cut 3: Select charged particles
	                 if(vecel_up1 >= 0 && (doCut_dL && deltaLyz_up1 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_up1)==cut_charge)){
                 
	                     Target_TPC->Fill(costheta_1, mom_1);
	                     All_topology->Fill(costheta_1, mom_1);
	                     All_topology2->Fill(costheta_1, mom_1);
		                 Target_all->Fill(costheta_1, mom_1);
						 
	                     Target_TPC_1->Fill(costheta_1, mom_1);
	                     All_topology_1->Fill(costheta_1, mom_1);
	                     All_topology2_1->Fill(costheta_1, mom_1);
		                 Target_all_1->Fill(costheta_1, mom_1);
						 
						 continue;
	                     if(doPrintTrack){
	                         cout << "     - TPCUp1 track with Target: " << " ";
	                         cout << "event = " << ientry << ", ";
	                         cout << "mom = " << mom_1 << ", ";
	                         cout << "deltaLyz = " << deltaLyz_up1 << ", ";
	                         cout << "charge = " << charge_up1 << ", ";
	                         cout << "edep = " << edep_up1 << ", ";
	                         cout << "pdg = " << pdg_up1 << ", ";
	                         cout << endl;
	                     }
	                 }
	                 else if(vecel_down1 >= 0 && (doCut_dL && deltaLyz_down1 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_down1)==cut_charge)){

	                     Target_TPC->Fill(costheta_1, mom_1);
	                     All_topology->Fill(costheta_1, mom_1);
	                     All_topology2->Fill(costheta_1, mom_1);
		                 Target_all->Fill(costheta_1, mom_1);
						 
	                     Target_TPC_1->Fill(costheta_1, mom_1);
	                     All_topology_1->Fill(costheta_1, mom_1);
	                     All_topology2_1->Fill(costheta_1, mom_1);
		                 Target_all_1->Fill(costheta_1, mom_1);
						 
						 continue;
	                     if(doPrintTrack){
	                         cout << "     - TPCDown1 track with Target: " << " ";
	                         cout << "event = " << ientry << ", ";
	                         cout << "mom = " << mom_1 << ", ";
	                         cout << "deltaLyz = " << deltaLyz_down1 << ", ";
	                         cout << "charge = " << charge_down1 << ", ";
	                         cout << "edep = " << edep_down1 << ", ";
	                         cout << "pdg = " << pdg_down1 << ", ";
	                         cout << endl;
	                     }
                    
	                 }
	                 else if(vecel_fwd1 >= 0 && (doCut_dL && deltaLyz_fwd1 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_fwd1)==cut_charge)){

	                     Target_TPC->Fill(costheta_1, mom_1);
	                     All_topology->Fill(costheta_1, mom_1);
	                     All_topology2->Fill(costheta_1, mom_1);
		                 Target_all->Fill(costheta_1, mom_1);
						 
	                     Target_TPC_1->Fill(costheta_1, mom_1);
	                     All_topology_1->Fill(costheta_1, mom_1);
	                     All_topology2_1->Fill(costheta_1, mom_1);
		                 Target_all_1->Fill(costheta_1, mom_1);
						 
						 continue;
	                     if(doPrintTrack){
	                         cout << "     - TPC Fwd 1 track with Target: " << " ";
	                         cout << "event = " << ientry << ", ";
	                         cout << "mom = " << mom_1 << ", ";
	                         cout << "deltaLyz = " << deltaLyz_fwd1 << ", ";
	                         cout << "charge = " << charge_fwd1 << ", ";
	                         cout << "edep = " << edep_fwd1 << ", ";
	                         cout << "pdg = " << pdg_fwd1 << ", ";
	                         cout << endl;
	                     }
                    
	                 }	                 
					 else if(vecel_fwd2 >= 0 && (doCut_dL && deltaLyz_fwd2 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_fwd2)==cut_charge)){

	                     Target_TPC->Fill(costheta_1, mom_1);
	                     All_topology->Fill(costheta_1, mom_1);
	                     All_topology2->Fill(costheta_1, mom_1);
		                 Target_all->Fill(costheta_1, mom_1);
						 
	                     Target_TPC_1->Fill(costheta_1, mom_1);
	                     All_topology_1->Fill(costheta_1, mom_1);
	                     All_topology2_1->Fill(costheta_1, mom_1);
		                 Target_all_1->Fill(costheta_1, mom_1);
						 
						 continue;
	                     if(doPrintTrack){
	                         cout << "     - TPC Fwd2 track with Target: " << " ";
	                         cout << "event = " << ientry << ", ";
	                         cout << "mom = " << mom_1 << ", ";
	                         cout << "deltaLyz = " << deltaLyz_fwd2 << ", ";
	                         cout << "charge = " << charge_fwd2 << ", ";
	                         cout << "edep = " << edep_fwd2 << ", ";
	                         cout << "pdg = " << pdg_fwd2 << ", ";
	                         cout << endl;
	                     }
	                 } // reco in TPCs
                 
	                 Target_all->Fill(costheta_1, mom_1);
	                 Target_only->Fill(costheta_1, mom_1);
	                 All_topology->Fill(costheta_1, mom_1);
					 
	                 Target_all_1->Fill(costheta_1, mom_1);
	                 Target_only_1->Fill(costheta_1, mom_1);
	                 All_topology_1->Fill(costheta_1, mom_1);
					 
					 continue;
	            }
	             // Cut 2: Select tracks with dLyz > 200mm
	             // Cut 3: Select charged particles
	             else if(vecel_up1 >= 0 &&  (doSelPDG && pdg_up1==SetPDG) && (doCut_dL && deltaLyz_up1 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_up1)==cut_charge)){
                 
	                 TPC_only->Fill(costheta_1, mom_1);
	                 All_topology->Fill(costheta_1, mom_1);
	                 All_topology2->Fill(costheta_1, mom_1);
					 
	                 TPC_only_1->Fill(costheta_1, mom_1);
	                 All_topology_1->Fill(costheta_1, mom_1);
	                 All_topology2_1->Fill(costheta_1, mom_1);
					 continue;
                 
	                 if(doPrintTrack){
	                     cout << "     - TPCUp1 track: " << " ";
	                     cout << "event = " << ientry << ", ";
	                     cout << "mom = " << mom_1 << ", ";
	                     cout << "deltaLyz = " << deltaLyz_up1 << ", ";
	                     cout << "charge = " << charge_up1 << ", ";
	                     cout << "edep = " << edep_up1 << ", ";
	                     cout << "pdg = " << pdg_up1 << ", ";
	                     cout << endl;
	                 }
	             } // reco in TPCUp
	             else if(vecel_down1 >= 0 &&  (doSelPDG && pdg_down1==SetPDG) && (doCut_dL && deltaLyz_down1 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_down1)==cut_charge)){
                 
	                 TPC_only->Fill(costheta_1, mom_1);
	                 All_topology->Fill(costheta_1, mom_1);
	                 All_topology2->Fill(costheta_1, mom_1);
					 
	                 TPC_only_1->Fill(costheta_1, mom_1);
	                 All_topology_1->Fill(costheta_1, mom_1);
	                 All_topology2_1->Fill(costheta_1, mom_1);
					 continue;
                 
	                 if(doPrintTrack){
	                     cout << "     - TPCDown1 track: " << " ";
	                     cout << "event = " << ientry << ", ";
	                     cout << "mom = " << mom_1 << ", ";
	                     cout << "deltaLyz = " << deltaLyz_down1 << ", ";
	                     cout << "charge = " << charge_down1 << ", ";
	                     cout << "edep = " << edep_down1 << ", ";
	                     cout << "pdg = " << pdg_down1 << ", ";
	                     cout << endl;
	                 }
	             } // reco in TPCDown
	             else if(vecel_fwd1 >= 0 &&  (doSelPDG && pdg_fwd1==SetPDG) && (doCut_dL && deltaLyz_fwd1 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_fwd1)==cut_charge)){
                 
	                 TPC_only->Fill(costheta_1, mom_1);
	                 All_topology->Fill(costheta_1, mom_1);
	                 All_topology2->Fill(costheta_1, mom_1);
					 
	                 TPC_only_1->Fill(costheta_1, mom_1);
	                 All_topology_1->Fill(costheta_1, mom_1);
	                 All_topology2_1->Fill(costheta_1, mom_1);
					 continue;
                 
	                 if(doPrintTrack){
	                     cout << "     - TPC Fwd 1 track: " << " ";
	                     cout << "event = " << ientry << ", ";
	                     cout << "mom = " << mom_1 << ", ";
	                     cout << "deltaLyz = " << deltaLyz_fwd1 << ", ";
	                     cout << "charge = " << charge_fwd1 << ", ";
	                     cout << "edep = " << edep_fwd1 << ", ";
	                     cout << "pdg = " << pdg_fwd1 << ", ";
	                     cout << endl;
	                 }
	             } // reco in TPC Fwd 1
	             else if(vecel_fwd2 >= 0 &&  (doSelPDG && pdg_fwd2==SetPDG) && (doCut_dL && deltaLyz_fwd2 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_fwd2)==cut_charge)){
                 
	                 TPC_only->Fill(costheta_1, mom_1);
	                 All_topology->Fill(costheta_1, mom_1);
	                 All_topology2->Fill(costheta_1, mom_1);
					 
	                 TPC_only_1->Fill(costheta_1, mom_1);
	                 All_topology_1->Fill(costheta_1, mom_1);
	                 All_topology2_1->Fill(costheta_1, mom_1);
					 continue;
                 
	                 if(doPrintTrack){
	                     cout << "     - TPCDown1 track: " << " ";
	                     cout << "event = " << ientry << ", ";
	                     cout << "mom = " << mom_1 << ", ";
	                     cout << "deltaLyz = " << deltaLyz_fwd2 << ", ";
	                     cout << "charge = " << charge_fwd2 << ", ";
	                     cout << "edep = " << edep_fwd2 << ", ";
	                     cout << "pdg = " << pdg_fwd2 << ", ";
	                     cout << endl;
	                 }
	             } // reco in TPC Fwd 2
			 } // in the Target 1
			 else if(Which_target == 2){
 	            //
 	            // Check if reconstructed in Target 2
 	            //
 	            double check = 0;
 	             vecel_2 = FindTrackElem(VecTarget2_TrackID,trkid);
 	             if(vecel_2>=0){
 	             	 id_2 = VecTarget2_TrackID->at(vecel_2);
 	             	 costheta_2 = VecTarget2_TrackCosTheta->at(vecel_2);
 	             	 momX_2 = VecTarget2_TrackMomX->at(vecel_2);
 	            	 momY_2 = VecTarget2_TrackMomY->at(vecel_2);
 	            	 momZ_2 = VecTarget2_TrackMomZ->at(vecel_2);
 	            	 mom_2 = sqrt( momX_2*momX_2 + momY_2*momY_2 + momZ_2*momZ_2 );
 	            	 deltaLyz_2 = VecTarget2_TrackDeltaLyz->at(vecel_2);
 	            	 length_2 = VecTarget2_TrackLength->at(vecel_2);
 	            	 edep_2 = VecTarget2_TrackEdep->at(vecel_2);
 	            	 charge_2 = VecTarget2_TrackCharge->at(vecel_2);
 	            	 pdg_2 = VecTarget2_TrackPDG->at(vecel_2);
             
 	            	 // Fill the denominator
 	            	 if ( (doSelPDG && pdg_2==SetPDG) ){
 						 Denom->Fill(costheta_2, mom_2);
 						 Denom_2->Fill(costheta_2, mom_2);
 						 check = 1;
 					 }
 			 	 }
			
 	             vecel_up2 = FindTrackElem(VecTPCUp2_TrackID,trkid);
 	             if(vecel_up2>=0){
 	                 id_up2 = VecTPCUp2_TrackID->at(vecel_up2);
 	                 deltaLyz_up2 = VecTPCUp2_TrackDeltaLyz->at(vecel_up2);
 	                 length_up2 = VecTPCUp2_TrackLength->at(vecel_up2);
 	                 edep_up2 = VecTPCUp2_TrackEdep->at(vecel_up2);
 	                 charge_up2 = VecTPCUp2_TrackCharge->at(vecel_up2);
 	                 pdg_up2 = VecTPCUp2_TrackPDG->at(vecel_up2);
 	            	 if (check == 0 && (doSelPDG && pdg_2==SetPDG) ){
 						 Denom->Fill(costheta_2, mom_2);
 						 Denom_2->Fill(costheta_2, mom_2);
 						 check = 1;
 					 }
 	             }
             
 	             vecel_down2 = FindTrackElem(VecTPCDown2_TrackID,trkid);
 	             if(vecel_down2>=0){
 	                 id_down2 = VecTPCDown2_TrackID->at(vecel_down2);
 	                 deltaLyz_down2 = VecTPCDown2_TrackDeltaLyz->at(vecel_down2);
 	                 length_down2 = VecTPCDown2_TrackLength->at(vecel_down2);
 	                 edep_down2 = VecTPCDown2_TrackEdep->at(vecel_down2);
 	                 charge_down2 = VecTPCDown2_TrackCharge->at(vecel_down2);
 	                 pdg_down2 = VecTPCDown2_TrackPDG->at(vecel_down2);
 	            	 if (check == 0 && (doSelPDG && pdg_2==SetPDG) ){
 						 Denom->Fill(costheta_2, mom_2);
 						 Denom_2->Fill(costheta_2, mom_2);
 						 check = 1;
 					 }
 	             }
				 
 	             vecel_fwd2 = FindTrackElem(VecForwTPC2_TrackID,trkid);
 	             if(vecel_fwd2>=0){
 	                 id_fwd2 = VecForwTPC2_TrackID->at(vecel_fwd2);
 	                 deltaLyz_fwd2 = VecForwTPC2_TrackDeltaLyz->at(vecel_fwd2);
 	                 length_fwd2 = VecForwTPC2_TrackLength->at(vecel_fwd2);
 	                 edep_fwd2 = VecForwTPC2_TrackEdep->at(vecel_fwd2);
 	                 charge_fwd2 = VecForwTPC2_TrackCharge->at(vecel_fwd2);
 	                 pdg_fwd2 = VecForwTPC2_TrackPDG->at(vecel_fwd2);
 	            	 if (check == 0 && (doSelPDG && pdg_2==SetPDG) ){
 						 Denom->Fill(costheta_2, mom_2);
 						 Denom_2->Fill(costheta_2, mom_2);
 						 check = 1;
 					 }
 	             }
             
 	             vecel_fwd3 = FindTrackElem(VecForwTPC3_TrackID,trkid);
 	             if(vecel_fwd3>=0){
 	                 id_fwd3 = VecForwTPC3_TrackID->at(vecel_fwd3);
 	                 deltaLyz_fwd3 = VecForwTPC3_TrackDeltaLyz->at(vecel_fwd3);
 	                 length_fwd3 = VecForwTPC3_TrackLength->at(vecel_fwd3);
 	                 edep_fwd3 = VecForwTPC3_TrackEdep->at(vecel_fwd3);
 	                 charge_fwd3 = VecForwTPC3_TrackCharge->at(vecel_fwd3);
 	                 pdg_fwd3 = VecForwTPC3_TrackPDG->at(vecel_fwd3);
 	            	 if (check == 0 && (doSelPDG && pdg_2==SetPDG) ){
 						 Denom->Fill(costheta_2, mom_2);
 						 Denom_2->Fill(costheta_2, mom_2);
 						 check = 1;
 					 }
 	             }
			 
   
 	             // Cut 2: Select track PDG
 	             // Cut 2: Select tracks with dLyz > 50mm
 	             // Cut 3: Select charged particles
 	             if(vecel_2 >= 0 && (doSelPDG && pdg_2==SetPDG) && (doCut_dL && length_2 > 0.5*len_Target1_min) && (fabs(charge_2)==cut_charge)){
                              
 	                 if(doPrintTrack){
 	                 cout << "  Track ID = " << trkid << endl;
 	                 cout << "     - Target 2 track: " << " ";
 	                 cout << "event = " << ientry << ", ";
 	                 cout << "mom = " << mom_2 << ", ";
 	                 cout << "length = " << length_2 << ", ";
 	                 cout << "charge = " << charge_2 << ", ";
 	                 cout << "edep = " << edep_2 << ", ";
 	                 cout << "pdg = " << pdg_2 << ", ";
 	                 cout << endl;
 	                 }
                 
 	                 //
 	                 // Check if reconstructed in TPCUp2 or TPC Down2 or TPC Fwd 2 or 3
 	                 //
                 
 	                 // Cut 2: Select tracks with dLyz > 200mm
 	                 // Cut 3: Select charged particles
 	                 if(vecel_up2 >= 0 && (doCut_dL && deltaLyz_up2 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_up2)==cut_charge)){
                 
 	                     Target_TPC->Fill(costheta_2, mom_2);
 	                     All_topology->Fill(costheta_2, mom_2);
 	                     All_topology2->Fill(costheta_2, mom_2);
 		                 Target_all->Fill(costheta_2, mom_2);
						 
 	                     Target_TPC_2->Fill(costheta_2, mom_2);
 	                     All_topology_2->Fill(costheta_2, mom_2);
 	                     All_topology2_2->Fill(costheta_2, mom_2);
 		                 Target_all_2->Fill(costheta_2, mom_2);
						 
 						 continue;
 	                     if(doPrintTrack){
 	                         cout << "     - TPCUp2 track with Target: " << " ";
 	                         cout << "event = " << ientry << ", ";
 	                         cout << "mom = " << mom_2 << ", ";
 	                         cout << "deltaLyz = " << deltaLyz_up2 << ", ";
 	                         cout << "charge = " << charge_up2 << ", ";
 	                         cout << "edep = " << edep_up2 << ", ";
 	                         cout << "pdg = " << pdg_up2 << ", ";
 	                         cout << endl;
 	                     }
 	                 } // reco in TPCUp2
 	                 else if(vecel_down2 >= 0 && (doCut_dL && deltaLyz_down2 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_down2)==cut_charge)){

 	                     Target_TPC->Fill(costheta_2, mom_2);
 	                     All_topology->Fill(costheta_2, mom_2);
 	                     All_topology2->Fill(costheta_2, mom_2);
 		                 Target_all->Fill(costheta_2, mom_2);
						 
 	                     Target_TPC_2->Fill(costheta_2, mom_2);
 	                     All_topology_2->Fill(costheta_2, mom_2);
 	                     All_topology2_2->Fill(costheta_2, mom_2);
 		                 Target_all_2->Fill(costheta_2, mom_2);
						 
 						 continue;
 	                     if(doPrintTrack){
 	                         cout << "     - TPCDown2 track with Target: " << " ";
 	                         cout << "event = " << ientry << ", ";
 	                         cout << "mom = " << mom_2 << ", ";
 	                         cout << "deltaLyz = " << deltaLyz_down2 << ", ";
 	                         cout << "charge = " << charge_down2 << ", ";
 	                         cout << "edep = " << edep_down2 << ", ";
 	                         cout << "pdg = " << pdg_down2 << ", ";
 	                         cout << endl;
 	                     }
                    
 	                 }
 	                 else if(vecel_fwd2 >= 0 && (doCut_dL && deltaLyz_fwd2 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_fwd2)==cut_charge)){

 	                     Target_TPC->Fill(costheta_2, mom_2);
 	                     All_topology->Fill(costheta_2, mom_2);
 	                     All_topology2->Fill(costheta_2, mom_2);
 		                 Target_all->Fill(costheta_2, mom_2);
						 
 	                     Target_TPC_2->Fill(costheta_2, mom_2);
 	                     All_topology_2->Fill(costheta_2, mom_2);
 	                     All_topology2_2->Fill(costheta_2, mom_2);
 		                 Target_all_2->Fill(costheta_2, mom_2);
						 
 						 continue;
 	                     if(doPrintTrack){
 	                         cout << "     - TPC Fwd 2 track with Target: " << " ";
 	                         cout << "event = " << ientry << ", ";
 	                         cout << "mom = " << mom_2 << ", ";
 	                         cout << "deltaLyz = " << deltaLyz_fwd2 << ", ";
 	                         cout << "charge = " << charge_fwd2 << ", ";
 	                         cout << "edep = " << edep_fwd2 << ", ";
 	                         cout << "pdg = " << pdg_fwd2 << ", ";
 	                         cout << endl;
 	                     }
                    
 	                 }	                 
 					 else if(vecel_fwd3 >= 0 && (doCut_dL && deltaLyz_fwd3 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_fwd3)==cut_charge)){

 	                     Target_TPC->Fill(costheta_2, mom_2);
 	                     All_topology->Fill(costheta_2, mom_2);
 	                     All_topology2->Fill(costheta_2, mom_2);
 		                 Target_all->Fill(costheta_2, mom_2);
						 
 	                     Target_TPC_2->Fill(costheta_2, mom_2);
 	                     All_topology_2->Fill(costheta_2, mom_2);
 	                     All_topology2_2->Fill(costheta_2, mom_2);
 		                 Target_all_2->Fill(costheta_2, mom_2);
						 
 						 continue;
 	                     if(doPrintTrack){
 	                         cout << "     - TPC Fwd3 track with Target: " << " ";
 	                         cout << "event = " << ientry << ", ";
 	                         cout << "mom = " << mom_2 << ", ";
 	                         cout << "deltaLyz = " << deltaLyz_fwd3 << ", ";
 	                         cout << "charge = " << charge_fwd3 << ", ";
 	                         cout << "edep = " << edep_fwd3 << ", ";
 	                         cout << "pdg = " << pdg_fwd3 << ", ";
 	                         cout << endl;
 	                     }
 	                 } // reco in TPCs
                 
 	                 Target_all->Fill(costheta_2, mom_2);
 	                 Target_only->Fill(costheta_2, mom_2);
 	                 All_topology->Fill(costheta_2, mom_2);
					 
 	                 Target_all_2->Fill(costheta_2, mom_2);
 	                 Target_only_2->Fill(costheta_2, mom_2);
 	                 All_topology_2->Fill(costheta_2, mom_2);
					 
 					 continue;
 	            } // in the Target
 	             // Cut 2: Select tracks with dLyz > 200mm
 	             // Cut 3: Select charged particles
 	             else if(vecel_up2 >= 0 &&  (doSelPDG && pdg_up2==SetPDG) && (doCut_dL && deltaLyz_up2 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_up2)==cut_charge)){
                 
 	                 TPC_only->Fill(costheta_2, mom_2);
 	                 All_topology->Fill(costheta_2, mom_2);
 	                 All_topology2->Fill(costheta_2, mom_2);
					 
 	                 TPC_only_2->Fill(costheta_2, mom_2);
 	                 All_topology_2->Fill(costheta_2, mom_2);
 	                 All_topology2_2->Fill(costheta_2, mom_2);
 					 continue;
                 
 	                 if(doPrintTrack){
 	                     cout << "     - TPCUp2 track: " << " ";
 	                     cout << "event = " << ientry << ", ";
 	                     cout << "mom = " << mom_2 << ", ";
 	                     cout << "deltaLyz = " << deltaLyz_up2 << ", ";
 	                     cout << "charge = " << charge_up2 << ", ";
 	                     cout << "edep = " << edep_up2 << ", ";
 	                     cout << "pdg = " << pdg_up2 << ", ";
 	                     cout << endl;
 	                 }
 	             } // reco in TPCUp2
 	             else if(vecel_down2 >= 0 &&  (doSelPDG && pdg_down2==SetPDG) && (doCut_dL && deltaLyz_down2 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_down2)==cut_charge)){
                 
 	                 TPC_only->Fill(costheta_2, mom_2);
 	                 All_topology->Fill(costheta_2, mom_2);
 	                 All_topology2->Fill(costheta_2, mom_2);
					 
 	                 TPC_only_2->Fill(costheta_2, mom_2);
 	                 All_topology_2->Fill(costheta_2, mom_2);
 	                 All_topology2_2->Fill(costheta_2, mom_2);
 					 continue;
                 
 	                 if(doPrintTrack){
 	                     cout << "     - TPCDown2 track: " << " ";
 	                     cout << "event = " << ientry << ", ";
 	                     cout << "mom = " << mom_2 << ", ";
 	                     cout << "deltaLyz = " << deltaLyz_down2 << ", ";
 	                     cout << "charge = " << charge_down2 << ", ";
 	                     cout << "edep = " << edep_down2 << ", ";
 	                     cout << "pdg = " << pdg_down2 << ", ";
 	                     cout << endl;
 	                 }
 	             } // reco in TPCDown2
 	             else if(vecel_fwd2 >= 0 &&  (doSelPDG && pdg_fwd2==SetPDG) && (doCut_dL && deltaLyz_fwd2 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_fwd2)==cut_charge)){
                 
 	                 TPC_only->Fill(costheta_2, mom_2);
 	                 All_topology->Fill(costheta_2, mom_2);
 	                 All_topology2->Fill(costheta_2, mom_2);
					 
 	                 TPC_only_2->Fill(costheta_2, mom_2);
 	                 All_topology_2->Fill(costheta_2, mom_2);
 	                 All_topology2_2->Fill(costheta_2, mom_2);
 					 continue;
                 
 	                 if(doPrintTrack){
 	                     cout << "     - TPC Fwd 2 track: " << " ";
 	                     cout << "event = " << ientry << ", ";
 	                     cout << "mom = " << mom_2 << ", ";
 	                     cout << "deltaLyz = " << deltaLyz_fwd2 << ", ";
 	                     cout << "charge = " << charge_fwd2 << ", ";
 	                     cout << "edep = " << edep_fwd2 << ", ";
 	                     cout << "pdg = " << pdg_fwd2 << ", ";
 	                     cout << endl;
 	                 }
 	             } // reco in TPC Fwd 2
 	             else if(vecel_fwd3 >= 0 &&  (doSelPDG && pdg_fwd3==SetPDG) && (doCut_dL && deltaLyz_fwd3 > dlyz_tpc_min) && (doCut_poscharge && fabs(charge_fwd3)==cut_charge)){
                 
 	                 TPC_only->Fill(costheta_2, mom_2);
 	                 All_topology->Fill(costheta_2, mom_2);
 	                 All_topology2->Fill(costheta_2, mom_2);
					 
 	                 TPC_only_2->Fill(costheta_2, mom_2);
 	                 All_topology_2->Fill(costheta_2, mom_2);
 	                 All_topology2_2->Fill(costheta_2, mom_2);
 					 continue;
                 
 	                 if(doPrintTrack){
 	                     cout << "     - TOC Fwd 3 track: " << " ";
 	                     cout << "event = " << ientry << ", ";
 	                     cout << "mom = " << mom_2 << ", ";
 	                     cout << "deltaLyz = " << deltaLyz_fwd3 << ", ";
 	                     cout << "charge = " << charge_fwd3 << ", ";
 	                     cout << "edep = " << edep_fwd3 << ", ";
 	                     cout << "pdg = " << pdg_fwd3 << ", ";
 	                     cout << endl;
 	                 }
 	             } // reco in TPC Fwd 3
			 }// End Target2
                    
        } // end loop over all tracks
        
    } // end loop over events
    
    
    // Store the Tree
    //treeoa->Write();
    //fileoa->Close();
    
    
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
	
    TCanvas *Target_only_c_1 = new TCanvas("Target_only_c_1","Target_only_c_1");
    Target_only_1->Divide(Denom_1);
    Target_only_1->Draw("COLZ");
    TCanvas *Target_TPC_c_1 = new TCanvas("Target_TPC_c_1","Target_TPC_c_1");
    Target_TPC_1->Divide(Denom_1);
    Target_TPC_1->Draw("COLZ");
    TCanvas *TPC_only_c_1 = new TCanvas("TPC_only_c_1","TPC_only_c_1");
    TPC_only_1->Divide(Denom_1);
    TPC_only_1->Draw("COLZ");
    TCanvas *All_topology_c_1 = new TCanvas("All_topology_c_1","All_topology_c_1");
    All_topology_1->Divide(Denom_1);
    All_topology_1->Draw("COLZ");
    TCanvas *All_topology2_c_1 = new TCanvas("All_topology2_c_1","All_topology2_c_1");
    All_topology2_1->Divide(Denom_1);
    All_topology2_1->Draw("COLZ");
    TCanvas *Target_all_c_1 = new TCanvas("Target_all_c_1","Target_all_c_1");
    Target_all_1->Divide(Denom_1);
    Target_all_1->Draw("COLZ");
	
    TCanvas *Target_only_c_2 = new TCanvas("Target_only_c_2","Target_only_c_2");
    Target_only_2->Divide(Denom_2);
    Target_only_2->Draw("COLZ");
    TCanvas *Target_TPC_c_2 = new TCanvas("Target_TPC_c_2","Target_TPC_c_2");
    Target_TPC_2->Divide(Denom_2);
    Target_TPC_2->Draw("COLZ");
    TCanvas *TPC_only_c_2 = new TCanvas("TPC_only_c_2","TPC_only_c_2");
    TPC_only_2->Divide(Denom_2);
    TPC_only_2->Draw("COLZ");
    TCanvas *All_topology_c_2 = new TCanvas("All_topology_c_2","All_topology_c_2");
    All_topology_2->Divide(Denom_2);
    All_topology_2->Draw("COLZ");
    TCanvas *All_topology2_c_2 = new TCanvas("All_topology2_c_2","All_topology2_c_2");
    All_topology2_2->Divide(Denom_2);
    All_topology2_2->Draw("COLZ");
    TCanvas *Target_all_c_2 = new TCanvas("Target_all_c_2","Target_all_c_2");
    Target_all_2->Divide(Denom_2);
    Target_all_2->Draw("COLZ");
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
