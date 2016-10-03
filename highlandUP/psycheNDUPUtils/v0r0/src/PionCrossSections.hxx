#ifndef PionCrossSections_h
#define PionCrossSections_h
#include <TGraph.h>
#include <TGraphErrors.h>
#include <map>
#include <math.h>
#include <TMath.h>
#include <utility> //for pair
#include <TGeoManager.h> //For geometry loading.
#include "ND280UPAnalysisUtils.hxx"
#define JMDEBUGFIXMATERIAL 1
namespace PiXSec{

  //===========================================
  //Carbon
  //==========================================

  //**********************
  //pi+
  //**********************

  //Total
  extern Int_t ndp_c_tot_piP;
  extern Float_t mom_c_tot_piP[24];
  extern Float_t xsec_c_tot_piP[24];
  extern Float_t err_c_tot_piP[24];

  //Reactive (i.e. all inelastic processes).
  extern Int_t ndp_c_rxn_piP;
  extern Float_t mom_c_rxn_piP[16];
  extern Float_t xsec_c_rxn_piP[16];
  extern Float_t err_c_rxn_piP[16];

  //Absorption
  extern Int_t ndp_c_abs_piP;
  extern Float_t mom_c_abs_piP[22];
  extern Float_t xsec_c_abs_piP[22];
  extern Float_t err_c_abs_piP[22];

  //Charge Exchange
  extern Int_t ndp_c_cex_piP;
  extern Float_t mom_c_cex_piP[10];
  extern Float_t xsec_c_cex_piP[10];
  extern Float_t err_c_cex_piP[10];


  //Quasi-Elastic
  extern Int_t ndp_c_qe_piP;
  extern Float_t mom_c_qe_piP[9];
  extern Float_t xsec_c_qe_piP[9];
  extern Float_t err_c_qe_piP[9];
  
  //Elastic
  extern Int_t ndp_c_el_piP;
  extern Float_t mom_c_el_piP[10];
  extern Float_t xsec_c_el_piP[10];
  extern Float_t err_c_el_piP[10];

  //**********************
  //pi-
  //**********************
  extern Int_t ndp_c_tot_piM;
  extern Float_t mom_c_tot_piM[44];
  extern Float_t xsec_c_tot_piM[44];
  extern Float_t err_c_tot_piM[44];

  //Reactive (i.e. all inelastic processes).
  extern Int_t ndp_c_rxn_piM;
  extern Float_t mom_c_rxn_piM[37];
  extern Float_t xsec_c_rxn_piM[37];
  extern Float_t err_c_rxn_piM[37];

  //Absorption
  extern Int_t ndp_c_abs_piM;
  extern Float_t mom_c_abs_piM[3];
  extern Float_t xsec_c_abs_piM[3];
  extern Float_t err_c_abs_piM[3];

  //Charge Exchange
  extern Int_t ndp_c_cex_piM;
  extern Float_t mom_c_cex_piM[2];
  extern Float_t xsec_c_cex_piM[2];
  extern Float_t err_c_cex_piM[2];


  //Quasi-Elastic
  extern Int_t ndp_c_qe_piM;
  extern Float_t mom_c_qe_piM[4];
  extern Float_t xsec_c_qe_piM[4];
  extern Float_t err_c_qe_piM[4];
  
  //Elastic
  extern Int_t ndp_c_el_piM;
  extern Float_t mom_c_el_piM[24];
  extern Float_t xsec_c_el_piM[24];
  extern Float_t err_c_el_piM[24];


  //=========================================
  //Oxygen
  //=========================================

  //**********************
  //pi+
  //**********************

  //Total
  extern Int_t ndp_o_tot_piP;
  extern Float_t mom_o_tot_piP[15];
  extern Float_t xsec_o_tot_piP[15];
  extern Float_t err_o_tot_piP[15];

  //Reactive (i.e. all inelastic processes).
  extern Int_t ndp_o_rxn_piP;
  extern Float_t mom_o_rxn_piP[3];
  extern Float_t xsec_o_rxn_piP[3];
  extern Float_t err_o_rxn_piP[3];

  //Absorption
  extern Int_t ndp_o_abs_piP;
  extern Float_t mom_o_abs_piP[3];
  extern Float_t xsec_o_abs_piP[3];
  extern Float_t err_o_abs_piP[3];

  //Charge Exchange
  extern Int_t ndp_o_cex_piP;
  extern Float_t mom_o_cex_piP[4];
  extern Float_t xsec_o_cex_piP[4];
  extern Float_t err_o_cex_piP[4];


  //Quasi-Elastic
  extern Int_t ndp_o_qe_piP;
  extern Float_t mom_o_qe_piP[3];
  extern Float_t xsec_o_qe_piP[3];
  extern Float_t err_o_qe_piP[3];
  
  //Elastic
  extern Int_t ndp_o_el_piP;
  extern Float_t mom_o_el_piP[3];
  extern Float_t xsec_o_el_piP[3];
  extern Float_t err_o_el_piP[3];

  //**********************
  //pi-
  //**********************
  extern Int_t ndp_o_tot_piM;
  extern Float_t mom_o_tot_piM[12];
  extern Float_t xsec_o_tot_piM[12];
  extern Float_t err_o_tot_piM[12];

  //Reactive (i.e. all inelastic processes).
  //No such data exists for pi- on oxygen,
  //apparently.
  extern Int_t ndp_o_rxn_piM;
  extern Float_t* mom_o_rxn_piM;
  extern Float_t* xsec_o_rxn_piM;
  extern Float_t* err_o_rxn_piM;

  //Absorption
  //Neither does this
  //So point to the pi+ data.
  extern Int_t ndp_o_abs_piM;
  extern Float_t* mom_o_abs_piM;
  extern Float_t* xsec_o_abs_piM;
  extern Float_t* err_o_abs_piM;

  //Charge Exchange
  extern Int_t ndp_o_cex_piP;
  extern Float_t mom_o_cex_piM[1];
  extern Float_t xsec_o_cex_piM[1];
  extern Float_t err_o_cex_piM[1];


  //Quasi-Elastic
  //No data
  //So use the pi+ data.
  extern Int_t ndp_o_qe_piM;
  extern Float_t* mom_o_qe_piM;
  extern Float_t* xsec_o_qe_piM;
  extern Float_t* err_o_qe_piM;
  
  //Elastic
  //No data
  extern Int_t ndp_o_el_piM;
  extern Float_t* mom_o_el_piM;
  extern Float_t* xsec_o_el_piM;
  extern Float_t* err_o_el_piM;


  //=========================================
  //Aluminum
  //=========================================

  //**********************
  //pi+
  //**********************

  //Total
  extern Int_t ndp_al_tot_piP;
  extern Float_t mom_al_tot_piP[7];
  extern Float_t xsec_al_tot_piP[7];
  extern Float_t err_al_tot_piP[7];

  //Reactive (i.e. all inelastic processes).
  extern Int_t ndp_al_rxn_piP;
  extern Float_t mom_al_rxn_piP[11];
  extern Float_t xsec_al_rxn_piP[11];
  extern Float_t err_al_rxn_piP[11];

  //Absorption
  extern Int_t ndp_al_abs_piP;
  extern Float_t mom_al_abs_piP[23];
  extern Float_t xsec_al_abs_piP[23];
  extern Float_t err_al_abs_piP[23];

  //Charge Exchange
  extern Int_t ndp_al_cex_piP;
  extern Float_t mom_al_cex_piP[6];
  extern Float_t xsec_al_cex_piP[6];
  extern Float_t err_al_cex_piP[6];


  //Quasi-Elastic
  extern Int_t ndp_al_qe_piP;
  extern Float_t mom_al_qe_piP[6];
  extern Float_t xsec_al_qe_piP[6];
  extern Float_t err_al_qe_piP[6];
  
  //Elastic
  extern Int_t ndp_al_el_piP;
  extern Float_t mom_al_el_piP[7];
  extern Float_t xsec_al_el_piP[7];
  extern Float_t err_al_el_piP[7];

  //**********************
  //pi-
  //**********************
  extern Int_t ndp_al_tot_piM;
  extern Float_t mom_al_tot_piM[12];
  extern Float_t xsec_al_tot_piM[12];
  extern Float_t err_al_tot_piM[12];

  //Reactive (i.e. all inelastic processes).
  extern Int_t ndp_al_rxn_piM;
  extern Float_t mom_al_rxn_piM[16];
  extern Float_t xsec_al_rxn_piM[16];
  extern Float_t err_al_rxn_piM[16];

  //Absorption
  extern Int_t ndp_al_abs_piM;
  extern Float_t mom_al_abs_piM[10];
  extern Float_t xsec_al_abs_piM[10];
  extern Float_t err_al_abs_piM[10];

  //Charge Exchange
  extern Int_t ndp_al_cex_piM;
  extern Float_t mom_al_cex_piM[2];
  extern Float_t xsec_al_cex_piM[2];
  extern Float_t err_al_cex_piM[2];


  //Quasi-Elastic
  extern Int_t ndp_al_qe_piM;
  extern Float_t mom_al_qe_piM[2];
  extern Float_t xsec_al_qe_piM[2];
  extern Float_t err_al_qe_piM[2];
  
  //Elastic
  extern Int_t ndp_al_el_piM;
  extern Float_t mom_al_el_piM[9];
  extern Float_t xsec_al_el_piM[9];
  extern Float_t err_al_el_piM[9];

  
  //=========================================
  //Iron
  //=========================================

  //**********************
  //pi+
  //**********************

  //Total
  extern Int_t ndp_fe_tot_piP;
  extern Float_t mom_fe_tot_piP[6];
  extern Float_t xsec_fe_tot_piP[6];
  extern Float_t err_fe_tot_piP[6];

  //Reactive (i.e. all inelastic processes).
  extern Int_t ndp_fe_rxn_piP;
  extern Float_t mom_fe_rxn_piP[12];
  extern Float_t xsec_fe_rxn_piP[12];
  extern Float_t err_fe_rxn_piP[12];

  //Absorption
  extern Int_t ndp_fe_abs_piP;
  extern Float_t mom_fe_abs_piP[24];
  extern Float_t xsec_fe_abs_piP[24];
  extern Float_t err_fe_abs_piP[24];

  //Charge Exchange
  extern Int_t ndp_fe_cex_piP;
  extern Float_t mom_fe_cex_piP[9];
  extern Float_t xsec_fe_cex_piP[9];
  extern Float_t err_fe_cex_piP[9];


  //Quasi-Elastic
  extern Int_t ndp_fe_qe_piP;
  extern Float_t mom_fe_qe_piP[7];
  extern Float_t xsec_fe_qe_piP[7];
  extern Float_t err_fe_qe_piP[7];
  
  //Elastic
  extern Int_t ndp_fe_el_piP;
  extern Float_t mom_fe_el_piP[6];
  extern Float_t xsec_fe_el_piP[6];
  extern Float_t err_fe_el_piP[6];

  //**********************
  //pi-
  //**********************
  extern Int_t ndp_fe_tot_piM;
  extern Float_t mom_fe_tot_piM[2];
  extern Float_t xsec_fe_tot_piM[2];
  extern Float_t err_fe_tot_piM[2];

  //Reactive (i.e. all inelastic processes).
  extern Int_t ndp_fe_rxn_piM;
  extern Float_t mom_fe_rxn_piM[10];
  extern Float_t xsec_fe_rxn_piM[10];
  extern Float_t err_fe_rxn_piM[10];

  //Absorption
  extern Int_t ndp_fe_abs_piM;
  extern Float_t mom_fe_abs_piM[10];
  extern Float_t xsec_fe_abs_piM[10];
  extern Float_t err_fe_abs_piM[10];

  //Charge Exchange
  extern Int_t ndp_fe_cex_piM;
  extern Float_t mom_fe_cex_piM[2];
  extern Float_t xsec_fe_cex_piM[2];
  extern Float_t err_fe_cex_piM[2];


  //Quasi-Elastic
  //No data.
  //So will use pi+ data.
  extern Int_t ndp_fe_qe_piM;
  extern Float_t* mom_fe_qe_piM;
  extern Float_t* xsec_fe_qe_piM;
  extern Float_t* err_fe_qe_piM;
  
  //Elastic
  extern Int_t ndp_fe_el_piM;
  extern Float_t mom_fe_el_piM[2];
  extern Float_t xsec_fe_el_piM[2];
  extern Float_t err_fe_el_piM[2];


  //================================================
  //GEANT4 Cross Section extraction results.
  //================================================
  extern Int_t g4ndp_al_abs_piM;
  extern Float_t g4mom_al_abs_piM[171];
  extern Float_t g4xsec_al_abs_piM[171];

  extern Int_t g4ndp_al_abs_piP;
  extern Float_t g4mom_al_abs_piP[171];
  extern Float_t g4xsec_al_abs_piP[171];

  extern Int_t g4ndp_al_cex_piM;
  extern Float_t g4mom_al_cex_piM[171];
  extern Float_t g4xsec_al_cex_piM[171];

  extern Int_t g4ndp_al_cex_piP;
  extern Float_t g4mom_al_cex_piP[171];
  extern Float_t g4xsec_al_cex_piP[171];

  extern Int_t g4ndp_al_mpi_piM;
  extern Float_t g4mom_al_mpi_piM[171];
  extern Float_t g4xsec_al_mpi_piM[171];

  extern Int_t g4ndp_al_mpi_piP;
  extern Float_t g4mom_al_mpi_piP[171];
  extern Float_t g4xsec_al_mpi_piP[171];

  extern Int_t g4ndp_al_qe_piM;
  extern Float_t g4mom_al_qe_piM[171];
  extern Float_t g4xsec_al_qe_piM[171];

  extern Int_t g4ndp_al_qe_piP;
  extern Float_t g4mom_al_qe_piP[171];
  extern Float_t g4xsec_al_qe_piP[171];

  extern Int_t g4ndp_c_abs_piM;
  extern Float_t g4mom_c_abs_piM[171];
  extern Float_t g4xsec_c_abs_piM[171];

  extern Int_t g4ndp_c_abs_piP;
  extern Float_t g4mom_c_abs_piP[171];
  extern Float_t g4xsec_c_abs_piP[171];

  extern Int_t g4ndp_c_cex_piM;
  extern Float_t g4mom_c_cex_piM[171];
  extern Float_t g4xsec_c_cex_piM[171];

  extern Int_t g4ndp_c_cex_piP;
  extern Float_t g4mom_c_cex_piP[171];
  extern Float_t g4xsec_c_cex_piP[171];

  extern Int_t g4ndp_c_mpi_piM;
  extern Float_t g4mom_c_mpi_piM[171];
  extern Float_t g4xsec_c_mpi_piM[171];

  extern Int_t g4ndp_c_mpi_piP;
  extern Float_t g4mom_c_mpi_piP[171];
  extern Float_t g4xsec_c_mpi_piP[171];

  extern Int_t g4ndp_c_qe_piM;
  extern Float_t g4mom_c_qe_piM[171];
  extern Float_t g4xsec_c_qe_piM[171];

  extern Int_t g4ndp_c_qe_piP;
  extern Float_t g4mom_c_qe_piP[171];
  extern Float_t g4xsec_c_qe_piP[171];

  extern Int_t g4ndp_fe_abs_piM;
  extern Float_t g4mom_fe_abs_piM[171];
  extern Float_t g4xsec_fe_abs_piM[171];

  extern Int_t g4ndp_fe_abs_piP;
  extern Float_t g4mom_fe_abs_piP[171];
  extern Float_t g4xsec_fe_abs_piP[171];

  extern Int_t g4ndp_fe_cex_piM;
  extern Float_t g4mom_fe_cex_piM[171];
  extern Float_t g4xsec_fe_cex_piM[171];

  extern Int_t g4ndp_fe_cex_piP;
  extern Float_t g4mom_fe_cex_piP[171];
  extern Float_t g4xsec_fe_cex_piP[171];

  extern Int_t g4ndp_fe_mpi_piM;
  extern Float_t g4mom_fe_mpi_piM[171];
  extern Float_t g4xsec_fe_mpi_piM[171];

  extern Int_t g4ndp_fe_mpi_piP;
  extern Float_t g4mom_fe_mpi_piP[171];
  extern Float_t g4xsec_fe_mpi_piP[171];

  extern Int_t g4ndp_fe_qe_piM;
  extern Float_t g4mom_fe_qe_piM[171];
  extern Float_t g4xsec_fe_qe_piM[171];

  extern Int_t g4ndp_fe_qe_piP;
  extern Float_t g4mom_fe_qe_piP[171];
  extern Float_t g4xsec_fe_qe_piP[171];

  extern Int_t g4ndp_o_abs_piM;
  extern Float_t g4mom_o_abs_piM[171];
  extern Float_t g4xsec_o_abs_piM[171];

  extern Int_t g4ndp_o_abs_piP;
  extern Float_t g4mom_o_abs_piP[171];
  extern Float_t g4xsec_o_abs_piP[171];

  extern Int_t g4ndp_o_cex_piM;
  extern Float_t g4mom_o_cex_piM[171];
  extern Float_t g4xsec_o_cex_piM[171];

  extern Int_t g4ndp_o_cex_piP;
  extern Float_t g4mom_o_cex_piP[171];
  extern Float_t g4xsec_o_cex_piP[171];

  extern Int_t g4ndp_o_mpi_piM;
  extern Float_t g4mom_o_mpi_piM[171];
  extern Float_t g4xsec_o_mpi_piM[171];

  extern Int_t g4ndp_o_mpi_piP;
  extern Float_t g4mom_o_mpi_piP[171];
  extern Float_t g4xsec_o_mpi_piP[171];

  extern Int_t g4ndp_o_qe_piM;
  extern Float_t g4mom_o_qe_piM[171];
  extern Float_t g4xsec_o_qe_piM[171];

  extern Int_t g4ndp_o_qe_piP;
  extern Float_t g4mom_o_qe_piP[171];
  extern Float_t g4xsec_o_qe_piP[171];

//===============================================
//Functions for putting the data in a usable format.
//===============================================

  //Given a TGraphErrors, generate a TGraph which is varied by some fraction of sigma, by the
  //sign of the sigma.
  //Need to have this so can generate the proper composite material cross sections in
  //AssembleXSecMap
  //NB: This function linearly interpolates between the points in the
  //TGraphErrors, both for the central value and the uncertainty.  (i.e. the
  //uncertainty will be linearly interpolated from the original data unless the TGraphErrors has had a
  //more advanced interpolation applied to it before being supplied to this
  //function.
  TGraph* GenerateVariedXSec(TGraphErrors* originalXSec, Float_t sigVar);


  //A function that takes in a vector of cross section paired with molar fraction, and
  //computes an average cross section for the material.
  TGraphErrors* GenerateCompositeXSec(std::vector<std::pair<TGraphErrors*, Float_t> > componentXSecs);

  //Takes in a vector of MC paired with molar fraction and outputs an average cross section
  //for the material.  (Simpler to do for MC, since no uncertainties to worry about.)
  TGraph* GenerateCompositeMCXSec(std::vector<std::pair<TGraph*, Float_t> > componentXSecs);

  //Assembles the data into TGraphErrors and the MC into TGraphs and maps them according to
  //pi+/- and material.  Has an option for whether to include corrected MC points in the
  //data for extrapolation beyond existing energies.
  //Currently only 0, 1, and 4 are coded in.
  //0: Absorption
  //1: Charge Exchange
  //4: Quasi-Elastic
  //-----------------
  //0-7 pi+ on Carbon
  //10-17 pi+ on Oxygen
  //20-27 pi+ on Aluminum
  //30-37 pi+ on Iron
  //40-47 pi+ on AlG10
  //50-57 pi+ on WaterSystem
  //60-67 pi+ on G10FGD1, G10
  //70-77 pi+ on FGDGlue
  //80-87 pi+ on G10Roha
  //100-107 pi- on Carbon
  //110-117 pi- on Oxygen
  //120-127 pi- on Aluminum
  //130-137 pi- on Iron
  //140-147 pi- on AlG10
  //150-157 pi- on WaterSystem
  //160-167 pi- on G10FGD1, G10
  //170-177 pi- on FGDGlue
  //180-187 pi- on G10Roha
  //The interpolation parameter determines how the uncertainties are
  //interpolated for use in determining the uncertainties on cross sections on
  //composite materials.  This does not affect the central values or the pure
  //material cross sections.  (If want to alter the interpolation of them, need
  //to assemble a separate map using one of the functions below.)
  //Breakdown is:
  //0: Linearly interpolate uncertainties between data points. (old default)
  //1: Linearly increase to a maximum mid-way between the two datapoints.
  //2: Quadratically increase between datapoints, capped at a maximum value.
  //
  //NB: If "extrapolate" is set to false, linear extrapolation is used to
  //assemble the composite cross sections, which produces absurd results.
  //Also, linear interpolation is forced in that case, since the other options
  //require information from the extrapolation.
  std::map<Int_t, std::pair<TGraphErrors*,TGraph*> > AssembleXSecMap(Bool_t extrapolate, Int_t interpolationType = 2);


  //Takes in the map and a sigma variation and generates a map of TGraphErrors where the TGraphErrors
  //are the original graph varied by that fraction of sigma.
  //std::map<Int_t, TGraphErrors*> GenerateVariedXSecMap(std::map<Int_t, std::pair<TGraphErrors*,TGraph*> > originalXSecMap,
    //					      Float_t sigVar);

  //A function that when given a TGraphErrors cross section, returns a TGraph
  //errors with points inserted midway between data points with an
  //appropriately inflated error.  This way, when the error is linearly
  //interpolated in between data points to either produce composite xsecs or to
  //extract the information for the BANFF fit, the uncertainty will linearly
  //increase between data points.
  //This function makes use of the fact that the order of points in the TGraph
  //is {DataPoints, extrapolated points starting at p = 1.0 MeV/c} in order to
  //figure out when the data points end.
  TGraphErrors* InterpolateUncertainty(TGraphErrors* xsecGraph);

  //Uses a quadratic uncertainty interpolation instead.
  TGraphErrors* QuadraticInterpolateUncertainty(TGraphErrors* xsecGraph);

  //A function to assemble cross sections where the error increases the farther
  //away you go from interpolated points as well as extrapolated points.
  //This is already done for the composite XSecs.  All this function does is
  //update the pure material xsecs in the map returned by AssembleXSecMap.
  std::map<Int_t, std::pair<TGraphErrors*,TGraph*> > AssembleIntUncXSecMap(Bool_t extrapolate);

  std::map<Int_t, std::pair<TGraphErrors*,TGraph*> > AssembleQuadIntUncXSecMap(Bool_t extrapolate);


  template<int nIntBins, int nMomBins>
  bool LoadInfo(Float_t (&xsec_array)[nIntBins][nMomBins], Float_t (&err_array)[nIntBins][nMomBins], bool MC){
    char inputFileName[256];

#if JMDEBUGFIXMATERIAL == 0
    if(MC) sprintf(inputFileName,"%s/data/SIPionXsecMCto8GeV.dat",getenv("PSYCHEUTILSROOT"));
    else sprintf(inputFileName,"%s/data/SIPionXsecto8GeV.dat",getenv("PSYCHEUTILSROOT"));
#else
    if(MC) sprintf(inputFileName,"%s/data/SIPionXsecMCto8GeVFixG10.dat",getenv("PSYCHEUTILSROOT"));
    else sprintf(inputFileName,"%s/data/SIPionXsecto8GeVFixG10.dat",getenv("PSYCHEUTILSROOT"));

#endif
    anaUtils::LoadPionCrossSections(inputFileName, xsec_array, err_array);
    return 0;
  }


  //Filled assembled XSecs.
  extern std::map<Int_t, std::pair<TGraphErrors*,TGraph*> > AllXSecs;

  extern Float_t xsec_data[200][801];
  extern Float_t err_data[200][801];
  extern Float_t xsec_MC[200][801];
  extern Float_t err_MC[200][801];


}


#endif
