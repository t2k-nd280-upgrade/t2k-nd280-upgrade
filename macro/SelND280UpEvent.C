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

#include "/atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.hh"
#include "/atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.hh"
#include "/atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.hh"
#include "/atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.hh"

//#include "/Users/davidesgalaberna/Desktop/GENEVA_postdoc/CODE/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.hh"
//#include "/Users/davidesgalaberna/Desktop/GENEVA_postdoc/CODE/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.hh"
//#include "/Users/davidesgalaberna/Desktop/GENEVA_postdoc/CODE/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.hh"
//#include "/Users/davidesgalaberna/Desktop/GENEVA_postdoc/CODE/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.hh"

int GetReacAll(int neut_reaction_mode);
string StringReacAll(int neut_reaction_mode);
//bool IsTargetIn(int targetID,double x,double y,double z);

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
		);

//bool IsFGDIn(int fgdID,double x,double y,double z,
//		// Target 1                                                                                                                                                              
//              double vtx_min_x_1,
//              double vtx_max_x_1,
//              double vtx_min_y_1,
//              double vtx_max_y_1,
//              double vtx_min_z_1,
//              double vtx_max_z_1,
//              // Target 2                                                                                                                                                   
// double vtx_min_x_2,
//              double vtx_max_x_2,
//                double vtx_min_y_2,
//              double vtx_max_y_2,
//              double vtx_min_z_2,
//              double vtx_max_z_2
//              );

void SelND280UpEvent
(
 const int evtfirst = 0,
 const int nevents = 1,
 
 string tag = "outputs/prova",
 
 string infilename = "../bin/ciao.root",

 // Definition of Fiducial Volume for target thickness=60cm (mm)  
 // Target 1
 const double vtx_min_x_1 = -1150,
 const double vtx_max_x_1 = +1150,  
 const double vtx_min_y_1 = -300, 
 const double vtx_max_y_1 = +300, 
 const double vtx_min_z_1 = -2487,  
 const double vtx_max_z_1 = -487, 
 // Target 2
 //const double vtx_min_x_2 = -1150,  
 //const double vtx_max_x_2 = +1150,  
 //const double vtx_min_y_2 = -300, 
 //const double vtx_max_y_2 = +300, 
 //const double vtx_min_z_2 = +487,  
 //const double vtx_max_z_2 = +2487,  
 // FGD 1        
 //const double vtx_min_x_fgd1 = -1150,
 //const double vtx_max_x_fgd1 = +1150,
 //const double vtx_min_y_fgd1 = -300,
 //const double vtx_max_y_fgd1 = +300,
 //const double vtx_min_z_fgd1 = -2487,
 //const double vtx_max_z_fgd1 = -487,
 // FGD 2
 //const double vtx_min_x_fgd2 = -1150,
 //const double vtx_max_x_fgd2 = +1150,
 //const double vtx_min_y_fgd2 = -300,
 //const double vtx_max_y_fgd2 = +300,
 //const double vtx_min_z_fgd2 = +487,
 //const double vtx_max_z_fgd2 = +2487,


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
  //gROOT->ProcessLine(".L /atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+");
  //gROOT->ProcessLine(".L /atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+");
  //gROOT->ProcessLine(".L /atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+");
  //gROOT->ProcessLine(".L /atlas/users/dsgalabe/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+");
  

  // // Cut 0: Select only CC mode                                                                                                                          
  //const bool doCutCC = true;
  // Cut 1: Select reaction mode                                                                                                                                                             
  //const bool doCutReac = false;                                                                                                                                                        
  //const int cut_reac = 0; // 0=CCQE, 1=2p2h, 2=CC1pi, 3=CCcoh, 4=CCDIS, 5=NC                                                                                                        
  // Cut 2: Vertex cut (inside the Targets and FGDs)                                                                                                                             
  //const bool doCutMater = false;                                                                                                                                                            
  //const bool doCutVtx = true;                                                                                                                                                                     
   
  //const bool doCutVtxX = false; // width                                                                                                                           
  //const double cut_xmin = 0;
  //const double cut_xmax = 0;
  //const bool doCutVtxZ = false; // length                                                                                                                          
  //const double cut_zmin = 0;
  //const double cut_zmax = 0;

  // Cut 5: Charge cut (TPCs and Target)                                                                                                                                                                  
  //const bool doCutCharge = true;
  //const double cut_charge = 1; // absolute value                                                                                                                                                             

 
  // Efficiency
  
  // My
  const int NBins_Mom = 33;
  double BinEdges_Mom[NBins_Mom+1] = {0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1200,1400,
  				      1600,1800,2000,2500,3000,4000,5000,6000,7000,8000,10000};
  const int NBins_CosTh = 20;
  double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

  // // Final - Muons (Numu)
  // const int NBins_Mom = 33;
  // double BinEdges_Mom[NBins_Mom+1] = {0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1200,1400,
  //   				      1600,1800,2000,2500,3000,4000,5000,6000,7000,8000,30000};
  // const int NBins_CosTh = 10;
  // double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0};

  // // Final - Pions (Numu)
  // const int NBins_Mom = 21;
  // double BinEdges_Mom[NBins_Mom+1] = {0,100,200,300,400,500,600,700,800,900,1000,1400,
  // 				      1800,2000,2500,3000,4000,5000,6000,7000,8000,30000};
  // const int NBins_CosTh = 10;
  // double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0};

  // // Final - Muons (NumuBar)
  // const int NBins_Mom = 21;
  // double BinEdges_Mom[NBins_Mom+1] = {0,100,200,300,400,500,600,700,800,900,1000,1400,
  //  				      1800,2000,2500,3000,4000,5000,6000,7000,8000,30000};
  // const int NBins_CosTh = 8;
  // double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.6,-0.2,0.0,0.2,0.4,0.6,0.8,1.0};

  // // Final - Electrons (Nue)
  // const int NBins_Mom = 2;
  // double BinEdges_Mom[NBins_Mom+1] = {0,1000,30000};
  // const int NBins_CosTh = 3;
  // double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.4,0.4,1.0};

  const int NBins_Phi = 20;
  double BinEdges_Phi[NBins_Phi+1];
  double phi_width = 2*TMath::Pi() / NBins_Phi;
  for(int iphi = 0;iphi<NBins_Phi+1;iphi++){
    BinEdges_Phi[iphi] = -TMath::Pi() + iphi*phi_width;
    //cout << BinEdges_Phi[iphi] << ", ";
  }
  
  TH1D *hPhi = new TH1D("hPhi","hPhi",40,-4,4);
  TH1D *hTheta = new TH1D("hTheta","hTheta",40,-4,4);
  
  TH2D *hCosThetaVsMom_FV = new TH2D("hCosThetaVsMom_FV","hCosThetaVsMom_FV",NBins_CosTh,BinEdges_CosTh,NBins_Mom,BinEdges_Mom);
  TH2D *hCosThetaVsMom_Target       
    = (TH2D*) hCosThetaVsMom_FV->Clone("hCosThetaVsMom_Target");
  TH2D *hCosThetaVsMom_TPC
    = (TH2D*) hCosThetaVsMom_FV->Clone("hCosThetaVsMom_TPC");
  TH2D *hCosThetaVsMom_TargetOrTPC  
    = (TH2D*) hCosThetaVsMom_FV->Clone("hCosThetaVsMom_TargetOrTPC");
  TH2D *hCosThetaVsMom_TargetAndTPC 
    = (TH2D*) hCosThetaVsMom_FV->Clone("hCosThetaVsMom_TargetAndTPC");  
  
  TH2D *hPhiVsMom_FV = new TH2D("hPhiVsMom_FV","hPhiVsMom_FV",NBins_Phi,BinEdges_Phi,NBins_Mom,BinEdges_Mom);
  TH2D *hPhiVsMom_Target       
    = (TH2D*) hPhiVsMom_FV->Clone("hPhiVsMom_Target");
  TH2D *hPhiVsMom_TPC          
    = (TH2D*) hPhiVsMom_FV->Clone("hPhiVsMom_TPC");
  TH2D *hPhiVsMom_TargetOrTPC  
    = (TH2D*) hPhiVsMom_FV->Clone("hPhiVsMom_TargetOrTPC");
  TH2D *hPhiVsMom_TargetAndTPC 
    = (TH2D*) hPhiVsMom_FV->Clone("hPhiVsMom_TargetAndTPC");  

  TH2D *hThetaVsPhi_FV = new TH2D("hThetaVsPhi_FV","hCosThetaVsMom_FV",NBins_Phi,BinEdges_Phi,NBins_Phi,BinEdges_Phi);
  TH2D *hThetaVsPhi_Target       
    = (TH2D*) hThetaVsPhi_FV->Clone("hThetaVsPhi_Target");
  TH2D *hThetaVsPhi_TPC          
    = (TH2D*) hThetaVsPhi_FV->Clone("hThetaVsPhi_TPC");
  TH2D *hThetaVsPhi_TargetOrTPC  
    = (TH2D*) hThetaVsPhi_FV->Clone("hThetaVsPhi_TargetOrTPC");
  TH2D *hThetaVsPhi_TargetAndTPC 
    = (TH2D*) hThetaVsPhi_FV->Clone("hThetaVsPhi_TargetAndTPC");  






  // FV histograms
  
  TH2D *hTPCTrkPtXY = new TH2D("hTPCTrkPtXY","hTPCTrkPtXY",400,-2000,+2000,400,-4000,+4000);
  TH2D *hTPCTrkPtXZ = new TH2D("hTPCTrkPtXZ","hTPCTrkPtXZ",400,-2000,+2000,400,-4000,+4000);
  TH2D *hTPCTrkPtYZ = new TH2D("hTPCTrkPtYZ","hTPCTrkPtYZ",400,-2000,+2000,400,-4000,+4000);

  TH2D *hDsECalTrkPtXY = new TH2D("hDsECalTrkPtXY","hDsECalTrkPtXY",400,-2000,+2000,400,-4000,+4000);
  TH2D *hDsECalTrkPtXZ = new TH2D("hDsECalTrkPtXZ","hDsECalTrkPtXZ",400,-2000,+2000,400,-4000,+4000);
  TH2D *hDsECalTrkPtYZ = new TH2D("hDsECalTrkPtYZ","hDsECalTrkPtYZ",400,-2000,+2000,400,-4000,+4000);

  TH2D *hBrlP0DECalTrkPtXY = new TH2D("hBrlP0DECalTrkPtXY","hBrlP0DECalTrkPtXY",400,-2000,+2000,400,-4000,+4000);
  TH2D *hBrlP0DECalTrkPtXZ = new TH2D("hBrlP0DECalTrkPtXZ","hBrlP0DECalTrkPtXZ",400,-2000,+2000,400,-4000,+4000);
  TH2D *hBrlP0DECalTrkPtYZ = new TH2D("hBrlP0DECalTrkPtYZ","hBrlP0DECalTrkPtYZ",400,-2000,+2000,400,-4000,+4000);

  TH2D *hECalP0DTrkPtXY = new TH2D("hECalP0DTrkPtXY","hECalP0DTrkPtXY",400,-2000,+2000,400,-4000,+4000);
  TH2D *hECalP0DTrkPtXZ = new TH2D("hECalP0DTrkPtXZ","hECalP0DTrkPtXZ",400,-2000,+2000,400,-4000,+4000);
  TH2D *hECalP0DTrkPtYZ = new TH2D("hECalP0DTrkPtYZ","hECalP0DTrkPtYZ",400,-2000,+2000,400,-4000,+4000);

  
  TH1D *hL_SD = new TH1D("hL_SD","hL_SD",200,0,2000);
  TH2D *hLVsMom_SD = new TH2D("hLVsMom_SD","hLVsMom_SD",200,0,2000,100,0,5000);  

  TH1D *hL_Targ1_FV = new TH1D("hL_Targ1_FV","hL_Targ1_FV",200,0,2000);
  TH1D *hL_Targ2_FV = new TH1D("hL_Targ2_FV","hL_Targ2_FV",200,0,2000);
  TH1D *hL_FGD1_FV = new TH1D("hL_FGD1_FV","hL_FGD1_FV",200,0,2000);
  TH1D *hL_FGD2_FV = new TH1D("hL_FGD2_FV","hL_FGD2_FV",200,0,2000);
  TH1D *hL_TPCUp1_FV = new TH1D("hL_TPCUp1_FV","hL_TPCUp1_FV",200,0,2000);
  TH1D *hL_TPCUp2_FV = new TH1D("hL_TPCUp2_FV","hL_TPCUp2_FV",200,0,2000);
  TH1D *hL_TPCDown1_FV = new TH1D("hL_TPCDown1_FV","hL_TPCDown1_FV",200,0,2000);
  TH1D *hL_TPCDown2_FV = new TH1D("hL_TPCDown2_FV","hL_TPCDown2_FV",200,0,2000);
  TH1D *hL_ForwTPC1_FV = new TH1D("hL_ForwTPC1_FV","hL_ForwTPC1_FV",200,0,2000);
  TH1D *hL_ForwTPC2_FV = new TH1D("hL_ForwTPC2_FV","hL_ForwTPC2_FV",200,0,2000);
  TH1D *hL_ForwTPC3_FV = new TH1D("hL_ForwTPC3_FV","hL_ForwTPC3_FV",200,0,2000);
  
  TH1D *hL_Targ1_Cut = new TH1D("hL_Targ1_Cut","hL_Targ1_Cut",200,0,2000);
  TH1D *hL_Targ2_Cut = new TH1D("hL_Targ2_Cut","hL_Targ2_Cut",200,0,2000);
  TH1D *hL_FGD1_Cut = new TH1D("hL_FGD1_Cut","hL_FGD1_Cut",200,0,2000);
  TH1D *hL_FGD2_Cut = new TH1D("hL_FGD2_Cut","hL_FGD2_Cut",200,0,2000);
  TH1D *hL_TPCUp1_Cut = new TH1D("hL_TPCUp1_Cut","hL_TPCUp1_Cut",200,0,2000);
  TH1D *hL_TPCUp2_Cut = new TH1D("hL_TPCUp2_Cut","hL_TPCUp2_Cut",200,0,2000);
  TH1D *hL_TPCDown1_Cut = new TH1D("hL_TPCDown1_Cut","hL_TPCDown1_Cut",200,0,2000);
  TH1D *hL_TPCDown2_Cut = new TH1D("hL_TPCDown2_Cut","hL_TPCDown2_Cut",200,0,2000);
  TH1D *hL_ForwTPC1_Cut = new TH1D("hL_ForwTPC1_Cut","hL_ForwTPC1_Cut",200,0,2000);
  TH1D *hL_ForwTPC2_Cut = new TH1D("hL_ForwTPC2_Cut","hL_ForwTPC2_Cut",200,0,2000);
  TH1D *hL_ForwTPC3_Cut = new TH1D("hL_ForwTPC3_Cut","hL_ForwTPC3_Cut",200,0,2000);
  
  TH2D *hLVsMom_Targ1_FV = new TH2D("hLVsMom_Targ1_FV","hLVsMom_Targ1_FV",200,0,2000,100,0,5000);  
  TH2D *hLVsMom_Targ2_FV = new TH2D("hLVsMom_Targ2_FV","hLVsMom_Targ2_FV",200,0,2000,100,0,5000);  
  TH2D *hLVsMom_FGD1_FV = new TH2D("hLVsMom_FGD1_FV","hLVsMom_FGD1_FV",200,0,2000,100,0,5000);
  TH2D *hLVsMom_FGD2_FV = new TH2D("hLVsMom_FGD2_FV","hLVsMom_FGD2_FV",200,0,2000,100,0,5000);
  TH2D *hLVsMom_TPCUp1_FV = new TH2D("hLVsMom_TPCUp1_FV","hLVsMom_TPCUp1_FV",200,0,2000,100,0,5000);
  TH2D *hLVsMom_TPCUp2_FV = new TH2D("hLVsMom_TPCUp2_FV","hLVsMom_TPCUp2_FV",200,0,2000,100,0,5000);
  TH2D *hLVsMom_TPCDown1_FV = new TH2D("hLVsMom_TPCDown1_FV","hLVsMom_TPCDown1_FV",200,0,2000,100,0,5000);
  TH2D *hLVsMom_TPCDown2_FV = new TH2D("hLVsMom_TPCDown2_FV","hLVsMom_TPCDown2_FV",200,0,2000,100,0,5000);
  TH2D *hLVsMom_ForwTPC1_FV = new TH2D("hLVsMom_ForwTPC1_FV","hLVsMom_ForwTPC1_FV",200,0,2000,100,0,5000);
  TH2D *hLVsMom_ForwTPC2_FV = new TH2D("hLVsMom_ForwTPC2_FV","hLVsMom_ForwTPC2_FV",200,0,2000,100,0,5000);
  TH2D *hLVsMom_ForwTPC3_FV = new TH2D("hLVsMom_ForwTPC3_FV","hLVsMom_ForwTPC3_FV",200,0,2000,100,0,5000);

  TH2D *hLVsMom_Targ1_Cut = new TH2D("hLVsMom_Targ1_Cut","hLVsMom_Targ1_Cut",200,0,2000,100,0,5000);  
  TH2D *hLVsMom_Targ2_Cut = new TH2D("hLVsMom_Targ2_Cut","hLVsMom_Targ2_Cut",200,0,2000,100,0,5000);  
  TH2D *hLVsMom_FGD1_Cut = new TH2D("hLVsMom_FGD1_Cut","hLVsMom_FGD1_Cut",200,0,2000,100,0,5000);
  TH2D *hLVsMom_FGD2_Cut = new TH2D("hLVsMom_FGD2_Cut","hLVsMom_FGD2_Cut",200,0,2000,100,0,5000);
  TH2D *hLVsMom_TPCUp1_Cut = new TH2D("hLVsMom_TPCUp1_Cut","hLVsMom_TPCUp1_Cut",200,0,2000,100,0,5000);
  TH2D *hLVsMom_TPCUp2_Cut = new TH2D("hLVsMom_TPCUp2_Cut","hLVsMom_TPCUp2_Cut",200,0,2000,100,0,5000);
  TH2D *hLVsMom_TPCDown1_Cut = new TH2D("hLVsMom_TPCDown1_Cut","hLVsMom_TPCDown1_Cut",200,0,2000,100,0,5000);
  TH2D *hLVsMom_TPCDown2_Cut = new TH2D("hLVsMom_TPCDown2_Cut","hLVsMom_TPCDown2_Cut",200,0,2000,100,0,5000);
  TH2D *hLVsMom_ForwTPC1_Cut = new TH2D("hLVsMom_ForwTPC1_Cut","hLVsMom_ForwTPC1_Cut",200,0,2000,100,0,5000);
  TH2D *hLVsMom_ForwTPC2_Cut = new TH2D("hLVsMom_ForwTPC2_Cut","hLVsMom_ForwTPC2_Cut",200,0,2000,100,0,5000);
  TH2D *hLVsMom_ForwTPC3_Cut = new TH2D("hLVsMom_ForwTPC3_Cut","hLVsMom_ForwTPC3_Cut",200,0,2000,100,0,5000);

  TH1D *hLyz_TPCUp1 = new TH1D("hLyz_TPCUp1","hLyz_TPCUp1",200,0,4000);
  TH1D *hLyz_TPCUp2 = new TH1D("hLyz_TPCUp2","hLyz_TPCUp2",200,0,4000);
  TH1D *hLyz_TPCDown1 = new TH1D("hLyz_TPCDown1","hLyz_TPCDown1",200,0,4000);
  TH1D *hLyz_TPCDown2 = new TH1D("hLyz_TPCDown2","hLyz_TPCDown2",200,0,4000);
  TH1D *hLyz_ForwTPC1 = new TH1D("hLyz_ForwTPC1","hLyz_ForwTPC1",200,0,4000);
  TH1D *hLyz_ForwTPC2 = new TH1D("hLyz_ForwTPC2","hLyz_ForwTPC2",200,0,4000);
  TH1D *hLyz_ForwTPC3 = new TH1D("hLyz_ForwTPC3","hLyz_ForwTPC3",200,0,4000);
  TH2D *hLyzVsMom_TPCUp1 = new TH2D("hLyzVsMom_TPCUp1","hLyzVsMom_TPCUp1",200,0,4000,100,0,5000);
  TH2D *hLyzVsMom_TPCUp2 = new TH2D("hLyzVsMom_TPCUp2","hLyzVsMom_TPCUp2",200,0,4000,100,0,5000);
  TH2D *hLyzVsMom_TPCDown1 = new TH2D("hLyzVsMom_TPCDown1","hLyzVsMom_TPCDown1",200,0,4000,100,0,5000);
  TH2D *hLyzVsMom_TPCDown2 = new TH2D("hLyzVsMom_TPCDown2","hLyzVsMom_TPCDown2",200,0,4000,100,0,5000);
  TH2D *hLyzVsMom_ForwTPC1 = new TH2D("hLyzVsMom_ForwTPC1","hLyzVsMom_ForwTPC1",200,0,4000,100,0,5000);
  TH2D *hLyzVsMom_ForwTPC2 = new TH2D("hLyzVsMom_ForwTPC2","hLyzVsMom_ForwTPC2",200,0,4000,100,0,5000);
  TH2D *hLyzVsMom_ForwTPC3 = new TH2D("hLyzVsMom_ForwTPC3","hLyzVsMom_ForwTPC3",200,0,4000,100,0,5000);
  TH2D *hLyzVsPhi_TPCUp1 = new TH2D("hLyzVsPhi_TPCUp1","hLyzVsPhi_TPCUp1",200,0,4000,40,-4,4);
  TH2D *hLyzVsPhi_TPCUp2 = new TH2D("hLyzVsPhi_TPCUp2","hLyzVsPhi_TPCUp2",200,0,4000,40,-4,4);
  TH2D *hLyzVsPhi_TPCDown1 = new TH2D("hLyzVsPhi_TPCDown1","hLyzVsPhi_TPCDown1",200,0,4000,40,-4,4);
  TH2D *hLyzVsPhi_TPCDown2 = new TH2D("hLyzVsPhi_TPCDown2","hLyzVsPhi_TPCDown2",200,0,4000,40,-4,4);
  TH2D *hLyzVsPhi_ForwTPC1 = new TH2D("hLyzVsPhi_ForwTPC1","hLyzVsPhi_ForwTPC1",200,0,4000,40,-4,4);
  TH2D *hLyzVsPhi_ForwTPC2 = new TH2D("hLyzVsPhi_ForwTPC2","hLyzVsPhi_ForwTPC2",200,0,4000,40,-4,4);
  TH2D *hLyzVsPhi_ForwTPC3 = new TH2D("hLyzVsPhi_ForwTPC3","hLyzVsPhi_ForwTPC3",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_TPCUp1 = new TH2D("hLyzVsTheta_TPCUp1","hLyzVsTheta_TPCUp1",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_TPCUp2 = new TH2D("hLyzVsTheta_TPCUp2","hLyzVsTheta_TPCUp2",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_TPCDown1 = new TH2D("hLyzVsTheta_TPCDown1","hLyzVsTheta_TPCDown1",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_TPCDown2 = new TH2D("hLyzVsTheta_TPCDown2","hLyzVsTheta_TPCDown2",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_ForwTPC1 = new TH2D("hLyzVsTheta_ForwTPC1","hLyzVsTheta_ForwTPC1",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_ForwTPC2 = new TH2D("hLyzVsTheta_ForwTPC2","hLyzVsTheta_ForwTPC2",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_ForwTPC3 = new TH2D("hLyzVsTheta_ForwTPC3","hLyzVsTheta_ForwTPC3",200,0,4000,40,-4,4);

  TH2D *hLyzVsPhi_Rej_TPCUp1 = new TH2D("hLyzVsPhi_Rej_TPCUp1","hLyzVsPhi_Rej_TPCUp1",200,0,4000,40,-4,4);
  TH2D *hLyzVsPhi_Rej_TPCUp2 = new TH2D("hLyzVsPhi_Rej_TPCUp2","hLyzVsPhi_Rej_TPCUp2",200,0,4000,40,-4,4);
  TH2D *hLyzVsPhi_Rej_TPCDown1 = new TH2D("hLyzVsPhi_Rej_TPCDown1","hLyzVsPhi_Rej_TPCDown1",200,0,4000,40,-4,4);
  TH2D *hLyzVsPhi_Rej_TPCDown2 = new TH2D("hLyzVsPhi_Rej_TPCDown2","hLyzVsPhi_Rej_TPCDown2",200,0,4000,40,-4,4);
  TH2D *hLyzVsPhi_Rej_ForwTPC1 = new TH2D("hLyzVsPhi_Rej_ForwTPC1","hLyzVsPhi_Rej_ForwTPC1",200,0,4000,40,-4,4);
  TH2D *hLyzVsPhi_Rej_ForwTPC2 = new TH2D("hLyzVsPhi_Rej_ForwTPC2","hLyzVsPhi_Rej_ForwTPC2",200,0,4000,40,-4,4);
  TH2D *hLyzVsPhi_Rej_ForwTPC3 = new TH2D("hLyzVsPhi_Rej_ForwTPC3","hLyzVsPhi_Rej_ForwTPC3",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_Rej_TPCUp1 = new TH2D("hLyzVsTheta_Rej_TPCUp1","hLyzVsTheta_Rej_TPCUp1",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_Rej_TPCUp2 = new TH2D("hLyzVsTheta_Rej_TPCUp2","hLyzVsTheta_Rej_TPCUp2",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_Rej_TPCDown1 = new TH2D("hLyzVsTheta_Rej_TPCDown1","hLyzVsTheta_Rej_TPCDown1",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_Rej_TPCDown2 = new TH2D("hLyzVsTheta_Rej_TPCDown2","hLyzVsTheta_Rej_TPCDown2",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_Rej_ForwTPC1 = new TH2D("hLyzVsTheta_Rej_ForwTPC1","hLyzVsTheta_Rej_ForwTPC1",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_Rej_ForwTPC2 = new TH2D("hLyzVsTheta_Rej_ForwTPC2","hLyzVsTheta_Rej_ForwTPC2",200,0,4000,40,-4,4);
  TH2D *hLyzVsTheta_Rej_ForwTPC3 = new TH2D("hLyzVsTheta_Rej_ForwTPC3","hLyzVsTheta_Rej_ForwTPC3",200,0,4000,40,-4,4);
  
  // ECal

  TH1D *hEdep_DsECal_FV = new TH1D("hEdep_DsECal_FV","hEdep_DsECal_FV",200,0,2000);
  TH1D *hEdep_BrlECal_FV = new TH1D("hEdep_BrlECal_FV","hEdep_BrlECal_FV",200,0,2000);
  TH1D *hEdep_P0DECal_FV = new TH1D("hEdep_P0DECal_FV","hEdep_P0DECal_FV",200,0,2000);
  TH1D *hEdep_USECalP0D_FV = new TH1D("hEdep_USECalP0D_FV","hEdep_USECalP0D_FV",200,0,2000);
  TH1D *hEdep_DsECal_NoTPC = new TH1D("hEdep_DsECal_NoTPC","hEdep_DsECal_NoTPC",200,0,2000);
  TH1D *hEdep_BrlECal_NoTPC = new TH1D("hEdep_BrlECal_NoTPC","hEdep_BrlECal_NoTPC",200,0,2000);
  TH1D *hEdep_P0DECal_NoTPC = new TH1D("hEdep_P0DECal_NoTPC","hEdep_P0DECal_NoTPC",200,0,2000);
  TH1D *hEdep_USECalP0D_NoTPC = new TH1D("hEdep_USECalP0D_NoTPC","hEdep_USECalP0D_NoTPC",200,0,2000);
  
  TH1D *hL_DsECal_FV = new TH1D("hL_DsECal_FV","hL_DsECal_FV",200,0,2000);
  TH1D *hL_BrlECal_FV = new TH1D("hL_BrlECal_FV","hL_BrlECal_FV",200,0,2000);
  TH1D *hL_P0DECal_FV = new TH1D("hL_P0DECal_FV","hL_P0DECal_FV",200,0,2000);
  TH1D *hL_USECalP0D_FV = new TH1D("hL_USECalP0D_FV","hL_USECalP0D_FV",200,0,2000);
  TH1D *hL_DsECal_NoTPC = new TH1D("hL_DsECal_NoTPC","hL_DsECal_NoTPC",200,0,2000);
  TH1D *hL_BrlECal_NoTPC = new TH1D("hL_BrlECal_NoTPC","hL_BrlECal_NoTPC",200,0,2000);
  TH1D *hL_P0DECal_NoTPC = new TH1D("hL_P0DECal_NoTPC","hL_P0DECal_NoTPC",200,0,2000);
  TH1D *hL_USECalP0D_NoTPC = new TH1D("hL_USECalP0D_NoTPC","hL_USECalP0D_NoTPC",200,0,2000);

  TH2D *hLVsMom_DsECal_FV = new TH2D("hLVsMom_DsECal_FV","hLVsMom_DsECal_FV",200,0,2000,100,0,5000);
  TH2D *hLVsMom_BrlECal_FV = new TH2D("hLVsMom_BrlECal_FV","hLVsMom_BrlECal_FV",200,0,2000,100,0,5000);
  TH2D *hLVsMom_P0DECal_FV = new TH2D("hLVsMom_P0DECal_FV","hLVsMom_P0DECal_FV",200,0,2000,100,0,5000);
  TH2D *hLVsMom_USECalP0D_FV = new TH2D("hLVsMom_USECalP0D_FV","hLVsMom_USECalP0D_FV",200,0,2000,100,0,5000);
  TH2D *hLVsMom_DsECal_NoTPC = new TH2D("hLVsMom_DsECal_NoTPC","hLVsMom_DsECal_NoTPC",200,0,2000,100,0,5000);
  TH2D *hLVsMom_BrlECal_NoTPC = new TH2D("hLVsMom_BrlECal_NoTPC","hLVsMom_BrlECal_NoTPC",200,0,2000,100,0,5000);
  TH2D *hLVsMom_P0DECal_NoTPC = new TH2D("hLVsMom_P0DECal_NoTPC","hLVsMom_P0DECal_NoTPC",200,0,2000,100,0,5000);
  TH2D *hLVsMom_USECalP0D_NoTPC = new TH2D("hLVsMom_USECalP0D_NoTPC","hLVsMom_USECalP0D_NoTPC",200,0,2000,100,0,5000);

  // TH2D *hLVsMomLoc_DsECal_FV = new TH2D("hLVsMomLoc_DsECal_FV","hLVsMomLoc_DsECal_FV",200,0,2000,100,0,5000);
  // TH2D *hLVsMomLoc_BrlECal_FV = new TH2D("hLVsMomLoc_BrlECal_FV","hLVsMomLoc_BrlECal_FV",200,0,2000,100,0,5000);
  // TH2D *hLVsMomLoc_P0DECal_FV = new TH2D("hLVsMomLoc_P0DECal_FV","hLVsMomLoc_P0DECal_FV",200,0,2000,100,0,5000);
  // TH2D *hLVsMomLoc_DsECal_NoTPC = new TH2D("hLVsMomLoc_DsECal_NoTPC","hLVsMomLoc_DsECal_NoTPC",200,0,2000,100,0,5000);
  // TH2D *hLVsMomLoc_BrlECal_NoTPC = new TH2D("hLVsMomLoc_BrlECal_NoTPC","hLVsMomLoc_BrlECal_NoTPC",200,0,2000,100,0,5000);
  // TH2D *hLVsMomLoc_P0DECal_NoTPC = new TH2D("hLVsMomLoc_P0DECal_NoTPC","hLVsMomLoc_P0DECal_NoTPC",200,0,2000,100,0,5000);
  
  //
  
  TH1D *hMom = new TH1D("hMom","hMom",200,0,10000); 
  TH1D *hPDG = new TH1D("hPDG","hPDG",3000,-500,2500);

  TH1D *hCut = new TH1D("hCut","hCut",10,0,10);
  hCut->GetXaxis()->SetBinLabel(1,"Evt gener");
  hCut->GetXaxis()->SetBinLabel(2,"Evt FV");
  hCut->GetXaxis()->SetBinLabel(3,"Evt CC");
  hCut->GetXaxis()->SetBinLabel(4,"Evt Mode");
  hCut->GetXaxis()->SetBinLabel(5,"Trk PDG");
  hCut->GetXaxis()->SetBinLabel(6,"Trk ParID");
  hCut->GetXaxis()->SetBinLabel(7,"Trk Charge");
  hCut->GetXaxis()->SetBinLabel(8,"Trk L(Targ)");
  hCut->GetXaxis()->SetBinLabel(9,"Trk Lyz(TPC)");

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

  // Track Point
  TH2D *hLastPt_Target_PhiVsZ = new TH2D("hLastPt_Target_PhiVsZ","hLastPt_Target_PhiVsZ",40,-4,+4,800,-4000,+4000);
  TH2D *hLastPt_Target_ThetaVsZ = new TH2D("hLastPt_Target_ThetaVsZ","hLastPt_Target_ThetaVsZ",40,-4,+4,800,-4000,+4000);
  TH1D *hLastPt_Target_Z = new TH1D("hLastPt_Target_Z","hLastPt_Target_Z",800,-4000,+4000);
  TH2D *hLastPt_Target_XY = new TH2D("hLastPt_Target_XY","hLastPt_Target_XY",800,-4000,+4000,800,-4000,+4000);
  TH2D *hLastPt_Target_YZ = new TH2D("hLastPt_Target_YZ","hLastPt_Target_YZ",800,-4000,+4000,800,-4000,+4000);
  TH2D *hLastPt_Target_XZ = new TH2D("hLastPt_Target_XZ","hLastPt_Target_XZ",800,-4000,+4000,800,-4000,+4000);

  TH2D *hLastPt_Oth_PhiVsZ = new TH2D("hLastPt_Oth_PhiVsZ","hLastPt_Oth_PhiVsZ",40,-4,+4,800,-8000,+8000);
  TH2D *hLastPt_Oth_ThetaVsZ = new TH2D("hLastPt_Oth_ThetaVsZ","hLastPt_Oth_ThetaVsZ",40,-4,+4,800,-8000,+8000);
  TH1D *hLastPt_Oth_Z = new TH1D("hLastPt_Oth_Z","hLastPt_Oth_Z",800,-8000,+8000);
  TH2D *hLastPt_Oth_XY = new TH2D("hLastPt_Oth_XY","hLastPt_Oth_XY",800,-8000,+8000,800,-8000,+8000);
  TH2D *hLastPt_Oth_YZ = new TH2D("hLastPt_Oth_YZ","hLastPt_Oth_YZ",800,-8000,+8000,800,-8000,+8000);
  TH2D *hLastPt_Oth_XZ = new TH2D("hLastPt_Oth_XZ","hLastPt_Oth_XZ",800,-8000,+8000,800,-8000,+8000);
  
  // TH1D *hEdgePt_ForwTPC1_Z = new TH1D("hEdgePt_ForwTPC1_Z","hEdgePt_ForwTPC1_Z",800,-4000,+4000);
  // TH1D *hEdgePt_ForwTPC2_Z = new TH1D("hEdgePt_ForwTPC2_Z","hEdgePt_ForwTPC2_Z",800,-4000,+4000);
  // TH1D *hEdgePt_ForwTPC3_Z = new TH1D("hEdgePt_ForwTPC3_Z","hEdgePt_ForwTPC3_Z",800,-4000,+4000);
  // TH1D *hFirstPt_ForwTPC1_Z = new TH1D("hFirstPt_ForwTPC1_Z","hFirstPt_ForwTPC1_Z",800,-4000,+4000);
  // TH1D *hFirstPt_ForwTPC2_Z = new TH1D("hFirstPt_ForwTPC2_Z","hFirstPt_ForwTPC2_Z",800,-4000,+4000);
  // TH1D *hFirstPt_ForwTPC3_Z = new TH1D("hFirstPt_ForwTPC3_Z","hFirstPt_ForwTPC3_Z",800,-4000,+4000);  
  // TH1D *hLastPt_ForwTPC1_Z = new TH1D("hLastPt_ForwTPC1_Z","hLastPt_ForwTPC1_Z",800,-4000,+4000);
  // TH1D *hLastPt_ForwTPC2_Z = new TH1D("hLastPt_ForwTPC2_Z","hLastPt_ForwTPC2_Z",800,-4000,+4000);
  // TH1D *hLastPt_ForwTPC3_Z = new TH1D("hLastPt_ForwTPC3_Z","hLastPt_ForwTPC3_Z",800,-4000,+4000);
  // TH2D *hEdgePt_ForwTPC1_XY = new TH2D("hEdgePt_ForwTPC1_XY","hEdgePt_ForwTPC1_XY",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hEdgePt_ForwTPC2_XY = new TH2D("hEdgePt_ForwTPC2_XY","hEdgePt_ForwTPC2_XY",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hEdgePt_ForwTPC3_XY = new TH2D("hEdgePt_ForwTPC3_XY","hEdgePt_ForwTPC3_XY",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hFirstPt_ForwTPC1_XY = new TH2D("hFirstPt_ForwTPC1_XY","hFirstPt_ForwTPC1_XY",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hFirstPt_ForwTPC2_XY = new TH2D("hFirstPt_ForwTPC2_XY","hFirstPt_ForwTPC2_XY",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hFirstPt_ForwTPC3_XY = new TH2D("hFirstPt_ForwTPC3_XY","hFirstPt_ForwTPC3_XY",800,-4000,+4000,800,-4000,+4000);  
  // TH2D *hLastPt_ForwTPC1_XY = new TH2D("hLastPt_ForwTPC1_XY","hLastPt_ForwTPC1_XY",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hLastPt_ForwTPC2_XY = new TH2D("hLastPt_ForwTPC2_XY","hLastPt_ForwTPC2_XY",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hLastPt_ForwTPC3_XY = new TH2D("hLastPt_ForwTPC3_XY","hLastPt_ForwTPC3_XY",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hEdgePt_ForwTPC1_YZ = new TH2D("hEdgePt_ForwTPC1_YZ","hEdgePt_ForwTPC1_YZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hEdgePt_ForwTPC2_YZ = new TH2D("hEdgePt_ForwTPC2_YZ","hEdgePt_ForwTPC2_YZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hEdgePt_ForwTPC3_YZ = new TH2D("hEdgePt_ForwTPC3_YZ","hEdgePt_ForwTPC3_YZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hFirstPt_ForwTPC1_YZ = new TH2D("hFirstPt_ForwTPC1_YZ","hFirstPt_ForwTPC1_YZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hFirstPt_ForwTPC2_YZ = new TH2D("hFirstPt_ForwTPC2_YZ","hFirstPt_ForwTPC2_YZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hFirstPt_ForwTPC3_YZ = new TH2D("hFirstPt_ForwTPC3_YZ","hFirstPt_ForwTPC3_YZ",800,-4000,+4000,800,-4000,+4000);  
  // TH2D *hLastPt_ForwTPC1_YZ = new TH2D("hLastPt_ForwTPC1_YZ","hLastPt_ForwTPC1_YZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hLastPt_ForwTPC2_YZ = new TH2D("hLastPt_ForwTPC2_YZ","hLastPt_ForwTPC2_YZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hLastPt_ForwTPC3_YZ = new TH2D("hLastPt_ForwTPC3_YZ","hLastPt_ForwTPC3_YZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hEdgePt_ForwTPC1_XZ = new TH2D("hEdgePt_ForwTPC1_XZ","hEdgePt_ForwTPC1_XZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hEdgePt_ForwTPC2_XZ = new TH2D("hEdgePt_ForwTPC2_XZ","hEdgePt_ForwTPC2_XZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hEdgePt_ForwTPC3_XZ = new TH2D("hEdgePt_ForwTPC3_XZ","hEdgePt_ForwTPC3_XZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hFirstPt_ForwTPC1_XZ = new TH2D("hFirstPt_ForwTPC1_XZ","hFirstPt_ForwTPC1_XZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hFirstPt_ForwTPC2_XZ = new TH2D("hFirstPt_ForwTPC2_XZ","hFirstPt_ForwTPC2_XZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hFirstPt_ForwTPC3_XZ = new TH2D("hFirstPt_ForwTPC3_XZ","hFirstPt_ForwTPC3_XZ",800,-4000,+4000,800,-4000,+4000);  
  // TH2D *hLastPt_ForwTPC1_XZ = new TH2D("hLastPt_ForwTPC1_XZ","hLastPt_ForwTPC1_XZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hLastPt_ForwTPC2_XZ = new TH2D("hLastPt_ForwTPC2_XZ","hLastPt_ForwTPC2_XZ",800,-4000,+4000,800,-4000,+4000);
  // TH2D *hLastPt_ForwTPC3_XZ = new TH2D("hLastPt_ForwTPC3_XZ","hLastPt_ForwTPC3_XZ",800,-4000,+4000,800,-4000,+4000);

  // # of events after each cut  
  int NTotCC = 0; // Cut 1
  int NTotVtxReac = 0; // Cut 1
  int NTotEvtFV = 0; // Cut 2
  int NTotTrkPassPDG = 0; // Cut 3
  int NTotTrkPassParentID = 0; // Cut 4
  int NTotTrkPassCharge = 0; // Cut 5
  int NTotTrkPassLtarget = 0; // Cut 6
  int NTotTrkPassLtpc = 0; // Cut 7
  
  // Check the inputs
  if(!doCutTarget1 && !doCutTarget2 && !doCutFGD1 && !doCutFGD2){
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





    //if(ievt!=574) continue;
    




    //nd280UpEvent->PrintEvent();

    bool PassCutVtx = false; // if at least 1 vertex does it
    bool PassCutReac = false; // cut on reaction mode
    bool PassCutCC = false;  // reject NC

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

    TND280UpVertex *nd280UpVertex;
    for(int ivtx=0;ivtx<NVertices;ivtx++){
      nd280UpVertex = nd280UpEvent->GetVertex(ivtx);
      //TND280UpVertex *nd280UpVertex = new TND280UpVertex();
      //nd280UpEvent->GetVertex(ivtx)->Copy(*nd280UpVertex);
      
      TND280UpTrack *nd280UpVtxTrkIn1 = nd280UpVertex->GetInTrack(0);
      TND280UpTrack *nd280UpVtxTrkIn2 = nd280UpVertex->GetInTrack(1);
      int pdg_vtxtrk1 = nd280UpVtxTrkIn1->GetPDG();
      int pdg_vtxtrk2 = nd280UpVtxTrkIn2->GetPDG();      

      int reacmode = abs(nd280UpVertex->GetReacMode());
      int reacmode_all = GetReacAll(reacmode);
      int cut_reac_all = GetReacAll(cut_reac); 

      //
      // Cut 0: don't consider NC events
      //

      if(doCutCC && reacmode_all==5) PassCutCC = false; 
      else PassCutCC = true;

      //
      // Cut 1: true reaction mode
      //

      if(doCutReac && reacmode_all==cut_reac) PassCutReac = true;
      else PassCutReac = true;
      	
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
	
	double VtxX = nd280UpVertex->GetPosition().X();
	double VtxY = nd280UpVertex->GetPosition().Y();
	double VtxZ = nd280UpVertex->GetPosition().Z();

	// Require only 1 target as FV
	int NumOfFVTargets = 0;
	if(doCutTarget1) NumOfFVTargets++;
	if(doCutTarget2) NumOfFVTargets++;
	if(doCutFGD1)    NumOfFVTargets++;
	if(doCutFGD2)    NumOfFVTargets++;
	if(NumOfFVTargets!=1){
	  cerr << endl;
	  cerr << "More than one target are FV!!!" << endl;
	  cerr << endl;
	  exit(1);
	}

	// Target 1 FV
	if(doCutTarget1 || doCutTarget2 || doCutFGD1 || doCutFGD2 ){
	  PassCutVtx = IsTargetIn(VtxX,VtxY,VtxZ,
				  // Target 1
				  vtx_min_x_1,
				  vtx_max_x_1,  
				  vtx_min_y_1, 
				  vtx_max_y_1, 
				  vtx_min_z_1,  
				  vtx_max_z_1 
				  );
	  FillVtxInFV = PassCutVtx;
	}
       	// don't apply this cut
	else if(!doCutTarget1){ // && !doCutTarget2 && !doCutFGD1 && !doCutFGD2){
	  PassCutVtx = true;
	  FillVtxInFV = true;
	}
	else{
          cerr << endl;
          cerr << "You can select only 1 target/fgd at a time!!!" << endl;
          cerr << endl;
          exit(1);
        }
	
	// Cut on X vertex position
	if(doCutVtxX){
	  if( VtxX < cut_xmin || 
	      VtxX > cut_xmax ){
	    PassCutVtx = false;
	    FillVtxInFV = false;
	  }
	}	  
	// Cut on Z vertex position
	if(doCutVtxZ){
	  if( VtxZ < cut_zmin || 
	      VtxZ > cut_zmax ){
	    PassCutVtx = false;
	    FillVtxInFV = false;
	  }
	}
		
	//
	// Fill this way because only 1 vertex per event!!!
	//
	if(!FillVtxInFV){
	  hVtxOut_XY->Fill(VtxX,VtxY);
	  hVtxOut_XZ->Fill(VtxX,VtxZ);
	  hVtxOut_YZ->Fill(VtxY,VtxZ);
	  hVtxOut_X->Fill(VtxX);
	  hVtxOut_Y->Fill(VtxY);
	  hVtxOut_Z->Fill(VtxZ);
	}
	else{
          hVtx_XY->Fill(VtxX,VtxY);
          hVtx_XZ->Fill(VtxX,VtxZ);
	  hVtx_YZ->Fill(VtxY,VtxZ);
          hVtx_X->Fill(VtxX);
	  hVtx_Y->Fill(VtxY);
	  hVtx_Z->Fill(VtxZ);
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
      //nd280UpVtxTrkIn1 = 0;
      //delete nd280UpVtxTrkIn2;
      //nd280UpVtxTrkIn2 = 0;
      //delete nd280UpVertex;
      //nd280UpVertex = 0;

    } // end loop over vertices

    hCut->Fill(0.);

    if(doCutVtx && !PassCutVtx) continue;
    NTotEvtFV++; // Pass Cut 2: at least 1 vertex in FV + reaction mode
    hCut->Fill(1.); 
    
    if(doCutCC && !PassCutCC) continue;
    NTotCC++;
    hCut->Fill(2.);
    
    if(doCutReac && !PassCutReac) continue;
    NTotVtxReac++;
    hCut->Fill(3.);  
    
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
    
      double dirX = nd280UpTrack->GetInitMom().X() / mom;
      double dirY = nd280UpTrack->GetInitMom().Y() / mom;
      double dirZ = nd280UpTrack->GetInitMom().Z() / mom;
      double phi = atan2(dirY,dirX);
      double cosphi = cos(phi);
      //double phi = atan(dirY/dirX);
      //double cosphi = dirY;
      //double phi = acos(cosphi); 
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
      double length_fgd1     = nd280UpTrack->GetLengthFGD1();
      double length_fgd2     = nd280UpTrack->GetLengthFGD2();
      double length_tpcup1   = nd280UpTrack->GetLengthTPCUp1();
      double length_tpcup2   = nd280UpTrack->GetLengthTPCUp2();
      double length_tpcdown1 = nd280UpTrack->GetLengthTPCDown1();
      double length_tpcdown2 = nd280UpTrack->GetLengthTPCDown2();
      double length_forwtpc1 = nd280UpTrack->GetLengthForwTPC1();
      double length_forwtpc2 = nd280UpTrack->GetLengthForwTPC2();
      double length_forwtpc3 = nd280UpTrack->GetLengthForwTPC3();      
      
      double length_dsecal    = nd280UpTrack->GetLengthDsECal();
      double length_brlecal   = nd280UpTrack->GetLengthBrlECal();
      double length_p0decal   = nd280UpTrack->GetLengthP0DECal();
      double length_usecalp0d = nd280UpTrack->GetLengthUSECalP0D();
      double edep_dsecal      = nd280UpTrack->GetEdepDsECal();
      double edep_brlecal     = nd280UpTrack->GetEdepBrlECal();
      double edep_p0decal     = nd280UpTrack->GetEdepP0DECal();
      double edep_usecalp0d   = nd280UpTrack->GetEdepUSECalP0D();
      
      // if(length_dsecal>0.) cout << "length_dsecal = " << length_dsecal << endl;
      //if(length_brlecal>0.) cout << "length_brlecal = " << length_brlecal << endl;
      // if(length_p0decal>0.) cout << "length_p0decal = " << length_p0decal << endl;
      //if(length_usecalp0d>0.) cout << "length_usecalp0d = " << length_usecalp0d << endl;
      //if(edep_dsecal>0.) cout << "edep_dsecal = " << edep_dsecal << endl;
      //if(edep_brlecal>0.) cout << "edep_brlecal = " << edep_brlecal << endl;
      //if(edep_p0decal>0.) cout << "edep_p0decal = " << edep_p0decal << endl;
      //if(edep_usecalp0d>0.) cout << "edep_usecalp0d = " << edep_usecalp0d << endl;
      
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
      hCut->Fill(4.);
      
      //
      // Cut 4: parent ID = 0
      //
      if(doCutParentID){
	if(parentID!=cut_parentID) continue;
      }
      NTotTrkPassParentID++;
      hCut->Fill(5.);
      
      //
      // Cut 5: particle charge
      //
      if(doCutCharge){
	if(abs(charge)!=cut_charge) continue;
      }
      NTotTrkPassCharge++;
      hCut->Fill(6.);
      
      int NPoints = nd280UpTrack->GetNPoints();    
      
      // Loop over the track points      
      
      //cout << "Evt " << ievt << " : " << "NPoints = " << NPoints << endl;    
      
      for(int ipt=0;ipt<NPoints;ipt++){
	TND280UpTrackPoint *nd280UpTrackPoint = nd280UpTrack->GetPoint(ipt);	
	double length = nd280UpTrackPoint->GetStepLength();
       	double PtX = nd280UpTrackPoint->GetPostPosition().X();
       	double PtY = nd280UpTrackPoint->GetPostPosition().Y();
       	double PtZ = nd280UpTrackPoint->GetPostPosition().Z();	
	//string volname = nd280UpTrackPoint->GetPhysVolName();
	string volname = nd280UpTrackPoint->GetLogVolName();
	
	//cout << PtX << ", " << PtY << ", " << PtZ << " --> " << volname << endl;
	
	// All the parts of TPCs in SDRegion ("Half" and "MM" only for ForwTPCs) 
	if( volname.find("/t2k/OA/Magnet/Basket/ForwTPC1/MM")   != string::npos ||
	    //volname.find("/t2k/OA/Magnet/Basket/ForwTPC1/Half") != string::npos ||
	    volname.find("/t2k/OA/Magnet/Basket/ForwTPC2/MM")   != string::npos ||
	    //volname.find("/t2k/OA/Magnet/Basket/ForwTPC2/Half") != string::npos ||
	    volname.find("/t2k/OA/Magnet/Basket/ForwTPC3/MM")   != string::npos 
	    //volname.find("/t2k/OA/Magnet/Basket/ForwTPC3/Half") != string::npos ||
	    //volname.find("/t2k/OA/Magnet/Basket/TPCUp")         != string::npos ||
	    //volname.find("/t2k/OA/Magnet/Basket/TPCDown")       != string::npos 
	    ){
	  hTPCTrkPtXY->Fill(PtX,PtY);
	  hTPCTrkPtXZ->Fill(PtX,PtZ);
	  hTPCTrkPtYZ->Fill(PtY,PtZ);	  
	}

	// All the parts of Brl+P0D ECal in SDRegion (name must contain "Bar") 

	// else if( (volname.find("BrlECal")!=string::npos && volname.find("Bar")!=string::npos) ||
	// 	 (volname.find("P0DECal")!=string::npos && volname.find("Bar")!=string::npos) 
	// 	 ){
	//   hBrlP0DECalTrkPtXY->Fill(PtX,PtY);
	//   hBrlP0DECalTrkPtXZ->Fill(PtX,PtZ);
	//   hBrlP0DECalTrkPtYZ->Fill(PtY,PtZ);	  
	// }

	// else if( (volname.find("DsECal") != string::npos && volname.find("Bar")!=string::npos) ){
	//   hDsECalTrkPtXY->Fill(PtX,PtY);
	//   hDsECalTrkPtXZ->Fill(PtX,PtZ);
	//   hDsECalTrkPtYZ->Fill(PtY,PtZ);	  
	// }
	//else if( (volname.find("DsECal/Module/Active/ScintHoriz/Bar") != string::npos) || 
	//	 (volname.find("DsECal/Module/Active/ScintVert/Bar") != string::npos) 
	//	 ){
	
	//else 
	if( (volname.find("DsECal") != string::npos) ){
	  //cout << volname << endl;
	  hDsECalTrkPtXY->Fill(PtX,PtY);
	  hDsECalTrkPtXZ->Fill(PtX,PtZ);
	  hDsECalTrkPtYZ->Fill(PtY,PtZ);	  
	}
	
	// else if( (volname.find("ECalP0D") != string::npos && volname.find("Bar")!=string::npos) ){
	//   hECalP0DTrkPtXY->Fill(PtX,PtY);
	//   hECalP0DTrkPtXZ->Fill(PtX,PtZ);
	//   hECalP0DTrkPtYZ->Fill(PtY,PtZ);	  
	// }
	
	
		  
	// // ECal entrance point
	// if (Tpoint->GetPhysVolName().find("BrlECal") != std::string::npos) {
	//   if (Tpoint->GetMomentum().Mag() < BrlECalZ) {
	//     lastBrlECal = Tpoint;
	//     BrlECalZ = Tpoint->GetMomentum().Mag();
	//   }
	//   if (Tpoint->GetMomentum().Mag() > BrlECalfZ) {
	//     firstBrlECal = Tpoint;
	//     BrlECalfZ = Tpoint->GetMomentum().Mag();
	//   }
	// }



	//delete nd280UpTrackPoint;
	//nd280UpTrackPoint = 0;

      } // loop over points
      

      // Fill selected particles in FV

      if(lyz_tpcup1>0.){
	hLyz_TPCUp1->Fill(lyz_tpcup1);
	hLyzVsMom_TPCUp1->Fill(lyz_tpcup1,mom);
	hLyzVsPhi_TPCUp1->Fill(lyz_tpcup1,phi);
      	hLyzVsTheta_TPCUp1->Fill(lyz_tpcup1,theta);
      }
      if(lyz_tpcup2>0.){
	hLyz_TPCUp2->Fill(lyz_tpcup2);
	hLyzVsMom_TPCUp2->Fill(lyz_tpcup2,mom);
	hLyzVsPhi_TPCUp2->Fill(lyz_tpcup2,phi);
      	hLyzVsTheta_TPCUp2->Fill(lyz_tpcup2,theta);
      }
      if(lyz_tpcdown1>0.){
	hLyz_TPCDown1->Fill(lyz_tpcdown1);
	hLyzVsMom_TPCDown1->Fill(lyz_tpcdown1,mom);
	hLyzVsPhi_TPCDown1->Fill(lyz_tpcdown1,phi);
	hLyzVsTheta_TPCDown1->Fill(lyz_tpcdown1,theta);
      }
      if(lyz_tpcdown2>0.){
	hLyz_TPCDown2->Fill(lyz_tpcdown2);
	hLyzVsMom_TPCDown2->Fill(lyz_tpcdown2,mom);
	hLyzVsPhi_TPCDown2->Fill(lyz_tpcdown2,phi);
	hLyzVsTheta_TPCDown2->Fill(lyz_tpcdown2,theta);
      }      
      if(lyz_forwtpc1>0.){
	hLyz_ForwTPC1->Fill(lyz_forwtpc1);
	hLyzVsMom_ForwTPC1->Fill(lyz_forwtpc1,mom);
	hLyzVsPhi_ForwTPC1->Fill(lyz_forwtpc1,phi);
	hLyzVsTheta_ForwTPC1->Fill(lyz_forwtpc1,theta);
      }
      if(lyz_forwtpc2>0.){
	hLyz_ForwTPC2->Fill(lyz_forwtpc2);
	hLyzVsMom_ForwTPC2->Fill(lyz_forwtpc2,mom);
	hLyzVsPhi_ForwTPC2->Fill(lyz_forwtpc2,phi);
	hLyzVsTheta_ForwTPC2->Fill(lyz_forwtpc2,theta);
      }
      if(lyz_forwtpc3>0.){
	hLyz_ForwTPC3->Fill(lyz_forwtpc3);
	hLyzVsMom_ForwTPC3->Fill(lyz_forwtpc3,mom);
	hLyzVsPhi_ForwTPC3->Fill(lyz_forwtpc3,phi);
	hLyzVsTheta_ForwTPC3->Fill(lyz_forwtpc3,theta);
      }
      
      if(length_target1>0.){ 
	hL_Targ1_FV->Fill(length_target1);
	hLVsMom_Targ1_FV->Fill(length_target1,mom);
      }
      if(length_target2>0.){
	hL_Targ2_FV->Fill(length_target2);
	hLVsMom_Targ2_FV->Fill(length_target2,mom);
      }
      if(length_fgd1>0.){
        hL_FGD1_FV->Fill(length_fgd1);
        hLVsMom_FGD1_FV->Fill(length_fgd1,mom);
      }
      if(length_fgd2>0.){
        hL_FGD2_FV->Fill(length_fgd2);
        hLVsMom_FGD2_FV->Fill(length_fgd2,mom);
      }
      if(length_tpcup1>0.){ 
	hL_TPCUp1_FV->Fill(length_tpcup1);
	hLVsMom_TPCUp1_FV->Fill(length_tpcup1,mom);
      }
      if(length_tpcup2>0.){ 
	hL_TPCUp2_FV->Fill(length_tpcup2);
	hLVsMom_TPCUp2_FV->Fill(length_tpcup2,mom);
      }
      if(length_tpcdown1>0.){ 
	hL_TPCDown1_FV->Fill(length_tpcdown1);
	hLVsMom_TPCDown1_FV->Fill(length_tpcdown1,mom);
      }
      if(length_tpcdown2>0.){ 
	hL_TPCDown2_FV->Fill(length_tpcdown2);
	hLVsMom_TPCDown2_FV->Fill(length_tpcdown2,mom);
      }      
      if(length_forwtpc1>0.){ 
	hL_ForwTPC1_FV->Fill(length_forwtpc1);
	hLVsMom_ForwTPC1_FV->Fill(length_forwtpc1,mom);
      }
      if(length_forwtpc2>0.){ 
	hL_ForwTPC2_FV->Fill(length_forwtpc2);
	hLVsMom_ForwTPC2_FV->Fill(length_forwtpc2,mom);
      }
      if(length_forwtpc3>0.){ 
	hL_ForwTPC3_FV->Fill(length_forwtpc3);
	hLVsMom_ForwTPC3_FV->Fill(length_forwtpc3,mom);
      }

      if(edep_dsecal>0.){ 
	hEdep_DsECal_FV->Fill(edep_dsecal);
	hL_DsECal_FV->Fill(length_dsecal);
	hLVsMom_DsECal_FV->Fill(length_dsecal,mom);
      }
      if(edep_brlecal>0.){ 
	hEdep_BrlECal_FV->Fill(edep_brlecal);
	hL_BrlECal_FV->Fill(length_brlecal);
	hLVsMom_BrlECal_FV->Fill(length_brlecal,mom);
      }
      if(edep_p0decal>0.){ 
	hEdep_P0DECal_FV->Fill(edep_p0decal);
	hL_P0DECal_FV->Fill(length_p0decal);
	hLVsMom_P0DECal_FV->Fill(length_p0decal,mom);
      }
      if(edep_usecalp0d>0.){ 
	hEdep_USECalP0D_FV->Fill(edep_usecalp0d);
	hL_USECalP0D_FV->Fill(length_usecalp0d);
	hLVsMom_USECalP0D_FV->Fill(length_usecalp0d,mom);
      }
      
      hPhi->Fill(phi);
      hTheta->Fill(theta);
      hCosThetaVsMom_FV->Fill(costheta,mom);
      hThetaVsPhi_FV->Fill(theta,phi);
      hPhiVsMom_FV->Fill(phi,mom);
      
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
        if(doCutFGD1 &&
           (length_fgd1 > cut_length_target_min))
          {
            PassCutTarget = true;
          }
        if(doCutFGD2 &&
           (length_fgd2 > cut_length_target_min))
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

      bool PassCutTPC = false; 
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
	
	if(length_target1>0.){
	  hL_Targ1_Cut->Fill(length_target1);
	  hLVsMom_Targ1_Cut->Fill(length_target1,mom);
	}
	if(length_target2>0.){
	  hL_Targ2_Cut->Fill(length_target2);
	  hLVsMom_Targ2_Cut->Fill(length_target2,mom);
	}
        if(length_fgd1>0.){
          hL_FGD1_Cut->Fill(length_fgd1);
          hLVsMom_FGD1_Cut->Fill(length_fgd1,mom);
        }
        if(length_fgd2>0.){
          hL_FGD2_Cut->Fill(length_fgd2);
          hLVsMom_FGD2_Cut->Fill(length_fgd2,mom);
        }
	if(length_tpcup1>0.){ 
	  hL_TPCUp1_Cut->Fill(length_tpcup1);
	  hLVsMom_TPCUp1_Cut->Fill(length_tpcup1,mom);
	}
	if(length_tpcup2>0.){ 
	  hL_TPCUp2_Cut->Fill(length_tpcup2);
	  hLVsMom_TPCUp2_Cut->Fill(length_tpcup2,mom);
	}
	if(length_tpcdown1>0.){ 
	  hL_TPCDown1_Cut->Fill(length_tpcdown1);
	  hLVsMom_TPCDown1_Cut->Fill(length_tpcdown1,mom);
	}
	if(length_tpcdown2>0.){ 
	  hL_TPCDown2_Cut->Fill(length_tpcdown2);
	  hLVsMom_TPCDown2_Cut->Fill(length_tpcdown2,mom);
	}
	if(length_forwtpc1>0.){ 
	  hL_ForwTPC1_Cut->Fill(length_forwtpc1);
	  hLVsMom_ForwTPC1_Cut->Fill(length_forwtpc1,mom);
	}
	if(length_forwtpc2>0.){ 
	  hL_ForwTPC2_Cut->Fill(length_forwtpc2);
	  hLVsMom_ForwTPC2_Cut->Fill(length_forwtpc2,mom);
	}
	if(length_forwtpc3>0.){ 
	  hL_ForwTPC3_Cut->Fill(length_forwtpc3);
	  hLVsMom_ForwTPC3_Cut->Fill(length_forwtpc3,mom);
	}
	
	hCosThetaVsMom_Target->Fill(costheta,mom);
	hThetaVsPhi_Target->Fill(theta,phi);
	hPhiVsMom_Target->Fill(phi,mom);
	IsDetected = true;	
	hCut->Fill(7.);	    
      }
      
      if(PassCutTPC){
      	NTotTrkPassLtpc++;
      	hCosThetaVsMom_TPC->Fill(costheta,mom);
      	hThetaVsPhi_TPC->Fill(theta,phi);
      	hPhiVsMom_TPC->Fill(phi,mom);
      	IsDetected = true;
      	hCut->Fill(8.);
      }
      if(PassCutTarget && PassCutTPC){ 
      	hCosThetaVsMom_TargetAndTPC->Fill(costheta,mom);      
      	hThetaVsPhi_TargetAndTPC->Fill(theta,phi);      
      	hPhiVsMom_TargetAndTPC->Fill(phi,mom);      
      	IsDetected = true;
      }
      if(PassCutTarget || PassCutTPC){
      	hCosThetaVsMom_TargetOrTPC->Fill(costheta,mom);
      	hThetaVsPhi_TargetOrTPC->Fill(theta,phi);
      	hPhiVsMom_TargetOrTPC->Fill(phi,mom);
      	IsDetected = true;
      }
      

      // Tracks not detected by any TPC --> try w/ ECal!!!

      if(!PassCutTPC){
	
	if(edep_dsecal>0.){
	  hEdep_DsECal_NoTPC->Fill(edep_dsecal);
	  hL_DsECal_NoTPC->Fill(length_dsecal);
	  hLVsMom_DsECal_NoTPC->Fill(length_dsecal,mom);
	}
	if(edep_brlecal>0.){
	  hEdep_BrlECal_NoTPC->Fill(edep_brlecal);
	  hL_BrlECal_NoTPC->Fill(length_brlecal);
	  hLVsMom_BrlECal_NoTPC->Fill(length_brlecal,mom);
	}
	if(edep_p0decal>0.){
	  hEdep_P0DECal_NoTPC->Fill(edep_p0decal);
	  hL_P0DECal_NoTPC->Fill(length_p0decal);
	  hLVsMom_P0DECal_NoTPC->Fill(length_p0decal,mom);
	}
	if(edep_usecalp0d>0.){
	  hEdep_USECalP0D_NoTPC->Fill(edep_usecalp0d);
	  hL_USECalP0D_NoTPC->Fill(length_usecalp0d);
	  hLVsMom_USECalP0D_NoTPC->Fill(length_usecalp0d,mom);
	}



	TND280UpTrackPoint *nd280UpLastPoint = nd280UpTrack->GetPoint(NPoints-1);
	double PtX = nd280UpLastPoint->GetPostPosition().X();
	double PtY = nd280UpLastPoint->GetPostPosition().Y();
	double PtZ = nd280UpLastPoint->GetPostPosition().Z();
      
	//double dirX = nd280UpTrack->GetInitMom().X() / mom;
	//double dirY = nd280UpTrack->GetInitMom().Y() / mom;
	//double dirZ = nd280UpTrack->GetInitMom().Z() / mom;
	//double phi = atan2(dirY,dirX);
	//double cosphi = cos(phi);
	//double costheta = dirZ;
	//double theta = acos(costheta);


	bool FlagIsTargetIn1 = IsTargetIn(PtX,PtY,PtZ,
					  // Target 1
					  vtx_min_x_1,
					  vtx_max_x_1,  
					  vtx_min_y_1, 
					  vtx_max_y_1, 
					  vtx_min_z_1,  
					  vtx_max_z_1 
					  );
	
	if( FlagIsTargetIn1 ){ 
	  hLastPt_Target_Z->Fill(PtZ);
	  hLastPt_Target_XY->Fill(PtX,PtY);
	  hLastPt_Target_YZ->Fill(PtY,PtZ);
	  hLastPt_Target_XZ->Fill(PtX,PtZ);
	  hLastPt_Target_PhiVsZ->Fill(phi,PtZ);
	  hLastPt_Target_ThetaVsZ->Fill(theta,PtZ);
       	}
	else{
	  hLastPt_Oth_Z->Fill(PtZ);
	  hLastPt_Oth_XY->Fill(PtX,PtY);
	  hLastPt_Oth_YZ->Fill(PtY,PtZ);
	  hLastPt_Oth_XZ->Fill(PtX,PtZ);
	  hLastPt_Oth_PhiVsZ->Fill(phi,PtZ);
	  hLastPt_Oth_ThetaVsZ->Fill(theta,PtZ);

	  hLyzVsPhi_Rej_TPCUp1->Fill(lyz_tpcup1,phi);	
	  hLyzVsPhi_Rej_TPCUp2->Fill(lyz_tpcup2,phi);
	  hLyzVsPhi_Rej_TPCDown1->Fill(lyz_tpcdown1,phi);
	  hLyzVsPhi_Rej_TPCDown2->Fill(lyz_tpcdown2,phi);
	  hLyzVsPhi_Rej_ForwTPC1->Fill(lyz_forwtpc1,phi);
	  hLyzVsPhi_Rej_ForwTPC2->Fill(lyz_forwtpc2,phi);
	  hLyzVsPhi_Rej_ForwTPC3->Fill(lyz_forwtpc3,phi);

	  hLyzVsTheta_Rej_TPCUp1->Fill(lyz_tpcup1,theta);
	  hLyzVsTheta_Rej_TPCUp2->Fill(lyz_tpcup2,theta);	  
	  hLyzVsTheta_Rej_TPCDown1->Fill(lyz_tpcdown1,theta);
	  hLyzVsTheta_Rej_TPCDown2->Fill(lyz_tpcdown2,theta);
	  hLyzVsTheta_Rej_ForwTPC1->Fill(lyz_forwtpc1,theta);
	  hLyzVsTheta_Rej_ForwTPC2->Fill(lyz_forwtpc2,theta);
	  hLyzVsTheta_Rej_ForwTPC3->Fill(lyz_forwtpc3,theta);
	}

	      
	//delete nd280UpLastPoint;
	//nd280UpLastPoint = 0;
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

      //delete nd280UpTrack;
      //nd280UpTrack = 0;
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
    
    delete nd280UpEvent;    
    nd280UpEvent = 0;
  } // end loop over events
  
   
  // Write output file
  TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);
  TFile *out = new TFile(outfilename.Data(),"RECREATE");
  
  hTPCTrkPtXY->Write();
  hTPCTrkPtXZ->Write();
  hTPCTrkPtYZ->Write();

  hDsECalTrkPtXY->Write();
  hDsECalTrkPtXZ->Write();
  hDsECalTrkPtYZ->Write();

  hBrlP0DECalTrkPtXY->Write();
  hBrlP0DECalTrkPtXZ->Write();
  hBrlP0DECalTrkPtYZ->Write();

  hECalP0DTrkPtXY->Write();
  hECalP0DTrkPtXZ->Write();
  hECalP0DTrkPtYZ->Write();

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
  hPhiVsMom_FV->Write();
  hPhiVsMom_Target->Write();
  hPhiVsMom_TPC->Write();
  hPhiVsMom_TargetAndTPC->Write();
  hPhiVsMom_TargetOrTPC->Write();
  hThetaVsPhi_FV->Write();
  hThetaVsPhi_Target->Write();
  hThetaVsPhi_TPC->Write();
  hThetaVsPhi_TargetAndTPC->Write();
  hThetaVsPhi_TargetOrTPC->Write();
  // Neutrino
  hNuReacFV->Write();
  hNuMom->Write();
  hNuPDG->Write();
  hEvis->Write();
  hDiffEtrueEvis->Write();
  hEtrueVsEvis->Write();
  // Track
  hMom->Write();
  hPDG->Write();
  hCut->Write();
  hPhi->Write();
  hTheta->Write();
  // Last Pt Track
  hLastPt_Target_PhiVsZ->Write();
  hLastPt_Target_ThetaVsZ->Write();
  hLastPt_Target_Z->Write();
  hLastPt_Target_XY->Write();
  hLastPt_Target_YZ->Write();
  hLastPt_Target_XZ->Write();
  hLastPt_Oth_PhiVsZ->Write();
  hLastPt_Oth_ThetaVsZ->Write();
  hLastPt_Oth_Z->Write();
  hLastPt_Oth_XY->Write();
  hLastPt_Oth_YZ->Write();
  hLastPt_Oth_XZ->Write();
  //
  // hEdgePt_ForwTPC1_Z->Write();
  // hEdgePt_ForwTPC2_Z->Write();
  // hEdgePt_ForwTPC3_Z->Write();
  // hFirstPt_ForwTPC1_Z->Write();
  // hFirstPt_ForwTPC2_Z->Write();
  // hFirstPt_ForwTPC3_Z->Write();
  // hLastPt_ForwTPC1_Z->Write();
  // hLastPt_ForwTPC2_Z->Write();
  // hLastPt_ForwTPC3_Z->Write();
  // hEdgePt_ForwTPC1_XY->Write();
  // hEdgePt_ForwTPC2_XY->Write();
  // hEdgePt_ForwTPC3_XY->Write();
  // hFirstPt_ForwTPC1_XY->Write();
  // hFirstPt_ForwTPC2_XY->Write();
  // hFirstPt_ForwTPC3_XY->Write();
  // hLastPt_ForwTPC1_XY->Write();
  // hLastPt_ForwTPC2_XY->Write();
  // hLastPt_ForwTPC3_XY->Write();
  // hEdgePt_ForwTPC1_XZ->Write();
  // hEdgePt_ForwTPC2_XZ->Write();
  // hEdgePt_ForwTPC3_XZ->Write();
  // hFirstPt_ForwTPC1_XZ->Write();
  // hFirstPt_ForwTPC2_XZ->Write();
  // hFirstPt_ForwTPC3_XZ->Write();
  // hLastPt_ForwTPC1_XZ->Write();
  // hLastPt_ForwTPC2_XZ->Write();
  // hLastPt_ForwTPC3_XZ->Write();
  // hEdgePt_ForwTPC1_YZ->Write();
  // hEdgePt_ForwTPC2_YZ->Write();
  // hEdgePt_ForwTPC3_YZ->Write();
  // hFirstPt_ForwTPC1_YZ->Write();
  // hFirstPt_ForwTPC2_YZ->Write();
  // hFirstPt_ForwTPC3_YZ->Write();
  // hLastPt_ForwTPC1_YZ->Write();
  // hLastPt_ForwTPC2_YZ->Write();
  // hLastPt_ForwTPC3_YZ->Write();
  //
  hL_SD->Write();
  hLVsMom_SD->Write();
  hL_Targ1_FV->Write();
  hL_Targ2_FV->Write();
  hL_FGD1_FV->Write();
  hL_FGD2_FV->Write();
  hL_TPCUp1_FV->Write();
  hL_TPCUp2_FV->Write();
  hL_TPCDown1_FV->Write();
  hL_TPCDown2_FV->Write();
  hL_ForwTPC1_FV->Write();
  hL_ForwTPC2_FV->Write();
  hL_ForwTPC3_FV->Write();
  //
  hEdep_DsECal_FV->Write();
  hEdep_BrlECal_FV->Write();
  hEdep_P0DECal_FV->Write();
  hEdep_USECalP0D_FV->Write();
  hEdep_DsECal_NoTPC->Write();
  hEdep_BrlECal_NoTPC->Write();
  hEdep_P0DECal_NoTPC->Write();
  hEdep_USECalP0D_NoTPC->Write();
  hL_DsECal_FV->Write();
  hL_BrlECal_FV->Write();
  hL_P0DECal_FV->Write();
  hL_USECalP0D_FV->Write();
  hL_DsECal_NoTPC->Write();
  hL_BrlECal_NoTPC->Write();
  hL_P0DECal_NoTPC->Write();
  hL_USECalP0D_NoTPC->Write();
  hLVsMom_DsECal_FV->Write();
  hLVsMom_BrlECal_FV->Write();
  hLVsMom_P0DECal_FV->Write();
  hLVsMom_USECalP0D_FV->Write();
  hLVsMom_DsECal_NoTPC->Write();
  hLVsMom_BrlECal_NoTPC->Write();
  hLVsMom_P0DECal_NoTPC->Write();
  hLVsMom_USECalP0D_NoTPC->Write();
  // hLVsMomLoc_DsECal_FV->Write();
  // hLVsMomLoc_BrlECal_FV->Write();
  // hLVsMomLoc_P0DECal_FV->Write();
  // hLVsMomLoc_DsECal_NoTPC->Write();
  // hLVsMomLoc_BrlECal_NoTPC->Write();
  // hLVsMomLoc_P0DECal_NoTPC->Write();
  //
  hL_Targ1_Cut->Write();
  hL_Targ2_Cut->Write();
  hL_FGD1_Cut->Write();
  hL_FGD2_Cut->Write();
  hL_TPCUp1_Cut->Write();
  hL_TPCUp2_Cut->Write();
  hL_TPCDown1_Cut->Write();
  hL_TPCDown2_Cut->Write();
  hL_ForwTPC1_Cut->Write();
  hL_ForwTPC2_Cut->Write();
  hL_ForwTPC3_Cut->Write();
  //
  hLVsMom_Targ1_FV->Write();
  hLVsMom_Targ2_FV->Write();
  hLVsMom_FGD1_FV->Write();
  hLVsMom_FGD2_FV->Write();
  hLVsMom_TPCUp1_FV->Write();
  hLVsMom_TPCUp2_FV->Write();
  hLVsMom_TPCDown1_FV->Write();
  hLVsMom_TPCDown2_FV->Write();
  hLVsMom_ForwTPC1_FV->Write();
  hLVsMom_ForwTPC2_FV->Write();
  hLVsMom_ForwTPC3_FV->Write();
  //
  hLVsMom_Targ1_Cut->Write();
  hLVsMom_Targ2_Cut->Write();
  hLVsMom_FGD1_Cut->Write();
  hLVsMom_FGD2_Cut->Write();
  hLVsMom_TPCUp1_Cut->Write();
  hLVsMom_TPCUp2_Cut->Write();
  hLVsMom_TPCDown1_Cut->Write();
  hLVsMom_TPCDown2_Cut->Write();
  hLVsMom_ForwTPC1_Cut->Write();
  hLVsMom_ForwTPC2_Cut->Write();
  hLVsMom_ForwTPC3_Cut->Write();
  //
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
  hLyzVsPhi_TPCUp1->Write();  
  hLyzVsPhi_TPCUp2->Write();  
  hLyzVsPhi_TPCDown1->Write();  
  hLyzVsPhi_TPCDown2->Write();  
  hLyzVsPhi_ForwTPC1->Write();  
  hLyzVsPhi_ForwTPC2->Write();  
  hLyzVsPhi_ForwTPC3->Write();  
  hLyzVsTheta_TPCUp1->Write();  
  hLyzVsTheta_TPCUp2->Write();  
  hLyzVsTheta_TPCDown1->Write();  
  hLyzVsTheta_TPCDown2->Write();  
  hLyzVsTheta_ForwTPC1->Write();  
  hLyzVsTheta_ForwTPC2->Write();  
  hLyzVsTheta_ForwTPC3->Write();  
  //
  hLyzVsPhi_Rej_TPCUp1->Write();  
  hLyzVsPhi_Rej_TPCUp2->Write();  
  hLyzVsPhi_Rej_TPCDown1->Write();  
  hLyzVsPhi_Rej_TPCDown2->Write();  
  hLyzVsPhi_Rej_ForwTPC1->Write();  
  hLyzVsPhi_Rej_ForwTPC2->Write();  
  hLyzVsPhi_Rej_ForwTPC3->Write();  
  hLyzVsTheta_Rej_TPCUp1->Write();  
  hLyzVsTheta_Rej_TPCUp2->Write();  
  hLyzVsTheta_Rej_TPCDown1->Write();  
  hLyzVsTheta_Rej_TPCDown2->Write();  
  hLyzVsTheta_Rej_ForwTPC1->Write();  
  hLyzVsTheta_Rej_ForwTPC2->Write();  
  hLyzVsTheta_Rej_ForwTPC3->Write();  
  //
  out->Close();
  
  // Print output
  cout << endl;
  cout << "Total # of Tree events: " << NTreeEntries << endl;
  cout << "Total # of simulated events: " << Nentries << endl;
  cout << "Select: " << endl;
  if(doCutCC) cout << " - Select CC events" << endl;
  if(doCutReac) cout << " - Reac = " << StringReacAll(cut_reac) << endl;
  if(doCutPDG) cout << " - PDG = " << cut_pdg << endl;
  if(doCutCharge) cout << " - Charge = " << cut_charge << endl;
  if(doCutDLTarget)
    cout << " - length Target > " << cut_length_target_min << " mm" << endl; 
  if(doCutDLyzTPC)
    cout << " - dLyz TPC > " << cut_dlyz_tpc_min << " mm" << endl;
  cout << endl;
  cout << "Total # of vtx after CC cut = " << NTotCC << endl;
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
