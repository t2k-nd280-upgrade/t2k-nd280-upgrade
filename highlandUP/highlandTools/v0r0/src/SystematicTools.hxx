#ifndef SystematicsTools_h
#define SystematicsTools_h

#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include "TVectorD.h"
#include "TMatrixD.h"
#include "HistoStack.hxx"

/// This class handles computing systematic errors.
///
/// TODO: This class needs to be documented properly.
class SystematicsTools{
 public :

  bool errdebug;

  SystematicsTools();

  virtual ~SystematicsTools(){}
  
  //----------- Functions to compute systematics ------------------

  /// Update the histos necesary to compute the covariance matrix
  void UpdateSystematicCov(HistoStack* hs, TTree* tree, const std::string& var, int nx, double* xbins, const std::string& cut, int NTOYS, const std::string& uopt);

  void FillSystematicHistos(TTree* tree, const std::string& var, int nx, double* xbins, const std::string& cut, int NTOYS, const std::string& uopt, TH2F* h2, TH2F* h1w);

  const TMatrixD& GetSystematicCovBase(HistoStack* hs1, HistoStack* hs2, const std::string& uopt, const std::string& group);
  void GetSystematicHistos(const std::string& group, HistoStack* hs1, HistoStack* hs2, TH2F*& h2, TH2F*& h1w);

  const TMatrixD& GetSystematicCov(HistoStack* hs, const std::string& uopt, const std::string& group="");
  const TMatrixD& GetRatioSystematicCov(HistoStack* hs1, HistoStack* hs2, const std::string& uopt, const std::string& group="");
  const TMatrixD& GetSystematicCov(TTree* tree, const std::string& var, int nx, double* xbins, const std::string& cut, int NTOYS, const std::string& uopt);
  const TMatrixD& GetSystematicCovBase(TH2F& h2, TH2F& h1w, const std::string& uopt);


  bool CheckSystComputed(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, const std::string& cut1, const std::string& cut2, double norm, int NTOYS, const std::string& uopt);

  bool TreeHasVar(TTree* tree, const std::string& var);
  int GetVarFromTree(TTree* tree, const std::string& var);

  // convert integer to string
  std::string GetString(int code);

  void SetSoftwareVersion(const std::string& ver){_softwareVersion = ver;}

  bool IsValidWeight(Float_t weight);

 protected:

  TMatrixD _covTotal;
  TMatrixD _cov;
  TMatrixD _w_cov;
  TMatrixD _st_cov;
  bool _syst_computed;

  TTree* _tree1_syst;
  TTree* _tree2_syst;
  std::string _var_syst;
  int _nbins_syst;
  double _xbins_syst[1000];
  std::string _cut_syst1;
  std::string _cut_syst2;
  double _norm_syst;
  int _NTOYS_syst;
  std::string _uopt_syst;

  std::string _softwareVersion;

};



#endif


