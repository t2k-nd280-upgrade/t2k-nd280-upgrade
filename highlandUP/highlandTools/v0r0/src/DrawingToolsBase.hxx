#ifndef DrawingToolsBase_h
#define DrawingToolsBase_h

#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TProfile.h>
#include "TLine.h"
#include "TVectorD.h"
#include "TMatrixD.h"
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TDirectory.h>
#include <TList.h>
#include <TPaveStats.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <THStack.h>
#include <TLegendEntry.h>
#include "HistoStack.hxx"
#include "Header.hxx"
#include <sstream>
#include <sys/stat.h>

#include "CategoryManager.hxx"
#include "SelectionManager.hxx"
#include "CorrectionManager.hxx"
#include "ConfigurationManager.hxx"
#include "SystematicManager.hxx"
#include "DocStringManager.hxx"
#include "VersionManager.hxx"

#include "SystematicTools.hxx"
#include "ConfigTreeTools.hxx"

//#include "ND280AnalysisUtils.hxx" TODO DUNE


const int NAUTOCOLORS=10;
const int NMAXAUTOCOLORS=30;
const int NMAXTYPES = 30;
const int NMAXBINS = 5001; // it allows NMAXBINS-1 bins

/// This is the base class for the DrawingTools, and uses a single TTree to
/// as input, rather than a DataSample object. It handles color code
/// drawing and variable binning.
///
/// Users generally shouldn't instantiate this class directly, but use the
/// DrawingTools class instead. See the DrawingTools class documentation for
/// more details.
///
/// TODO: This class must be properly documented.

class DrawingToolsBase{
public :

  DrawingToolsBase(const std::string& file="", Int_t T2KstyleIndex=1);
  virtual ~DrawingToolsBase(){}

  //---------- Utility functions ---------------

  TH1F* AddErrorsInQuadrature(TH1F* h1,TH1F* h2,const std::string& opt="");

  /// extract the same or sames option from the full root option
  std::string GetSameRootOption(const std::string& root_opt);

  /// remove same and sames options from the root option
  std::string GetNoSameRootOption(const std::string& root_opt);

  /// reset all saved histograms
  void Reset();

  /// Get number (sum of weights) of events (i.e. bunches) passing "cut"
  double GetEntries(TTree* tree, const std::string& cut);

  /// Get number (sum of weights) for variable var pasing "cut" (useful for vector variables, which number can be different than number of events/bunches)
  double GetEntries(TTree* tree, const std::string& cut, const std::string& var, const std::string& opt = "", int toy_ref = -1);

  //---------------------------------------------------

  /// Scale an histogram and its errors(when requested)
  void ScaleHisto(TH1* h, double scale, bool scale_errors=true);

  /// Get a vector with variable binning
  double* GetVariableBins(int nx, double xmin, double xmax, double*);

  /// Get the superposition level
  std::string GetSameLevel(const std::string& root_opt);

  /// Get a unique name for a histogram, to avoid name clashes when drawing on
  /// multiple canvases.
  std::string GetUniqueName(const std::string& name);

  /// create the legend
  void CreateLegend(const std::string& uopt="");

  /// Print the event number of a specific selection
  void PrintEventNumbers(TTree* tree, const std::string& cut, const std::string& file="", int toy_ref=-1);

  /// Give the appropriate format to any cut
  std::string FormatCut(const std::string& cut);

  /// Add cut on range shown on plot
  std::string AddRangeCut(const std::string& var,  int nx, double* xbins, int ny, double* ybins, const std::string& cut, const std::string& uopt);

  //------------------------------------------
  //---------- Drawing functions -------------
  //------------------------------------------

  //---------- Single distributions -------------

  // Draw a single variable "var" in given tree with the specified binning. A cut can be applied using variables in the tree. The stacked histogram colors will refer 
  // to the specified category. If no category is specified only the total will be plotted. A normalization factor can be applied to the histogram contents. 
  // By default errors are scaled when the normalization factor is not 1. 
  
  /// 1D histos
  TH1* Draw(TTree* tree, const std::string& var, int nbins, double* xbins, const std::string& categ="all",
	    const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=1,bool scale_errors=true);
  TH1* Draw(TTree* tree, const std::string& var, int nbins, double xmin, double xmax, const std::string& categ="all",
	    const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=1,bool scale_errors=true);

  /// 2D histos
  TH1* Draw(TTree* tree, const std::string& var, int nx, double* xbins, int ny, double* ybins,
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=1,bool scale_errors=true);
  TH1* Draw(TTree* tree, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax,
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=1,bool scale_errors=true);


  void Draw(TTree* tree, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax, const std::string& idVar, std::vector<int> idSel, std::vector<std::string> idName, std::vector<int> idColor, const std::string& cut="",  const std::string& root_opt = "", const std::string& opt = "", double norm = 1.,  bool scale_errors = true);


  void Draw(TTree* tree_mc, TTree* tree_data, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax, const std::string& idVar, std::vector<int> idSel, std::vector<std::string> idName, std::vector<int> idColor, const std::string& cut="",  const std::string& root_opt = "", const std::string& opt = "", double norm = 1.,  bool scale_errors = true);

  void Draw(TTree* tree, const std::string& var, int nx, double* xbins, int ny, double* ybins,
            const std::string& idvar, std::vector<int> idSel, std::vector<std::string> idName, std::vector<int> idColor,
	    const std::string& cut="",  const std::string& root_opt="", const std::string& opt="", double norm=1,  bool scale_errors=true);


  void Draw(TTree* tree_mc, TTree* tree_data, const std::string& var, int nx, double* xbins, int ny, double* ybins,
	    const std::string& idvar, std::vector<int> idSel, std::vector<std::string> idName, std::vector<int> idColor,
	    const std::string& cut="",  const std::string& root_opt="", const std::string& opt="", double norm=1,  bool scale_errors=true);

  //---------- Ratios, efficiencies, purities and significances -------------

  /// 1D Ratio between two independent cuts (the denominator cut is cut2, the numerator cut is cut1)
  void DrawRatio(TTree* tree, const std::string& var, int nbins, double* xbins,
		 const std::string& cut1, const std::string& cut2, const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  void DrawRatio(TTree* tree, const std::string& var, int nx, double xmin, double xmax,
		 const std::string& cut1, const std::string& cut2,  const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  /// Get the efficiency (BayesDivide ratio) of two histograms in which the numerator (cut1+cut2) is a subset of the denominator (cut2).
  /// Integrate over "var" between xmin and xmax. This is useful for doing quick optimisation studies,
  /// where one can loop over several cut options to see the effect they have.
  double GetEff(TTree* tree, const std::string& var, double xmin, double xmax,
               const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt = "");

  /// See other GetEff() interface for documentation. In this interface,
  /// errlow and errhigh will be filled with the upper and lower errors on the efficiency.
  double GetEff(TTree* tree, const std::string& var, double xmin, double xmax, double& errlow, double& errhigh,
               const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt = "");


  // 1D Efficiency (ratio between dependent cuts: the denominator cut is cut2, the numerator cut is cut1+cut2)
  // This method can be used to plot the efficiency of a given cut (default tree), 
  // the selection efficiency (truth tree) or the selection purity (default tree).
  // This method uses TGraphAsymmErrors
  void DrawEff(TTree* tree, const std::string& var, int nbins, double* xbins, const std::string& cut1, const std::string& cut2,
	       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  void DrawEff(TTree* tree, const std::string& var, int nx, double xmin, double xmax, const std::string& cut1, const std::string& cut2,
	       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawGraph(TGraphAsymmErrors* eff, int nbins, double* xbins,
		 const std::string& uroot_opt, const std::string& uopt, const std::string& leg, double ymax=1.05);

  void DumpGraph(TGraphAsymmErrors* graph, int nbins, const std::string& uopt);

  /// ratio between two Efficiencies
  void DrawDoubleEff(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins,
		     const std::string& cut1, const std::string& cut2, const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawDoubleEff(TTree* tree1, TTree* tree2, const std::string& var, int nx, double xmin, double xmax, 
		     const std::string& cut1, const std::string& cut2, const std::string& root_opt="", const std::string& opt="", const std::string& leg="");


  /// 1D significance
  void DrawSignificance(TTree* tree, const std::string& var, int nbins, double* xbins, const std::string& cut1, const std::string& cut2,
			double norm=1, double rel_syst=0,const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  void DrawSignificance(TTree* tree, const std::string& var, int nbins, double xmin, double xmax, const std::string& cut1, const std::string& cut2,
			double norm=1, double rel_syst=0,const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  //---------- Events, Ratios, efficiencies and purities VS cuts -------------

  TH1F* GetEventsVSCut(TTree* tree, const std::string& var, const std::string& cut_norm, int isel, int branch, int& first_cut, int& last_cut,
		       const std::string& root_opt="", const std::string& opt="");

  /// Draw the number of events passing the selection as a function of the cut
  void DrawEventsVSCut(TTree* tree, const std::string& cut_norm="", int first_cut=-1, int last_cut=-1,
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  void DrawEventsVSCut(TTree* tree, int branch, const std::string& cut_norm="", int first_cut=-1, int last_cut=-1,
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  void DrawEventsVSCut(TTree* tree, int isel, int branch, const std::string& cut_norm="", int first_cut=-1, int last_cut=-1,
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  /// Draw the ratio between two trees as a function of the cut
  void DrawRatioVSCut(TTree* tree1, TTree* tree2, const std::string& precut="", int first_cut=-1, int last_cut=-1,
                      const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=1.);
  void DrawRatioVSCut(TTree* tree1, TTree* tree2, int branch, const std::string& precut="", int first_cut=-1, int last_cut=-1,
                      const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=1.);
  void DrawRatioVSCut(TTree* tree1, TTree* tree2, int isel, int branch, const std::string& precut="", int first_cut=-1, int last_cut=-1,
                      const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=1.);

  /// Draw the selection efficiency as a function of the cut. Must use the truth tree as input
  void DrawEffVSCut(TTree* tree, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  void DrawEffVSCut(TTree* tree, int branch, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  void DrawEffVSCut(TTree* tree, int isel, int branch, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  /// Draw the selection purity as a function of the cut. Must use the default tree as input
  void DrawPurVSCut(TTree* tree, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  void DrawPurVSCut(TTree* tree, int branch, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  void DrawPurVSCut(TTree* tree, int isel, int branch, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");


  /// Draw the ratio between two samples as a function of the cut
  void DrawRatioVSCut(TH1F* h1, TH1F* h2, int isel, int branch, int first_cut,
		      const std::string& root_opt, const std::string& opt, const std::string& leg);


  //---------- Toy experiments methods -------------

  /// This method draws the distribution of the number of selected events for all toys (one entry per toy in the histogram), provided a cut
  /// This is useful to check that the systematic error bars correspond to the RMS of this distribution
  void DrawToys(TTree* tree, const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  /// This method draws the distribution of the ratio of number of selected events between two samples for all toys (one entry per toy in the histogram), 
  /// provided a cut (the same for numerator and denominator)
  void DrawToysRatio(TTree* tree1, TTree* tree2, const std::string& cut="", double norm=1, 
			const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  /// This method draws the distribution of the ratio of number of selected events between two samples for all toys (one entry per toy in the histogram), 
  /// provided two cuts, one for the numerator and another for the denominator
  void DrawToysRatio(TTree* tree1, TTree* tree2, const std::string& cut1, const std::string& cut2, double norm=1, 
			const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  
  /// Basic method used by the ones above
  void DrawToysBase(TH1F& result, TH1F& entries, const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  /// Draw the variations for systematic parameter ipar
  void DrawVariations(const std::string& name, Int_t ipar=-1, int nx=100, double xmin=-5, double xmax=5, 
                      const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  ///------------------------------------------------------------------------------
  /// ---------- comparisons between two data samples (two trees) -----------------
  ///------------------------------------------------------------------------------

  /// 1D comparison
  void Draw(TTree* tree1, TTree* tree2, const std::string& var, int nx, double xmin, double xmax,
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=1, bool scale_errors=false);
  void Draw(TTree* tree1, TTree* tree2, const std::string& var, int nbins, double* xbins,
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=1,  bool scale_errors=false);

  /// 2D comparison
  void Draw(TTree* tree1, TTree* tree2, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax,
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=1, bool scale_errors=false);
  void Draw(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, int ny, double* ybins,
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=1,  bool scale_errors=false);


  /// Ratio between the two data samples  with the same cut
  void DrawRatio(TTree* tree1, TTree* tree2, const std::string& var, int nx, double xmin, double xmax,
		 const std::string& cut="",  double norm=1, const std::string& root_opt="", const std::string& opt="",  const std::string& leg="");
  void DrawRatio(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins,
		 const std::string& cut="",  double norm=1, const std::string& root_opt="", const std::string& opt="",  const std::string& leg="");

  void DrawRatio(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, int ny, double* ybins,
		 const std::string& cut1="", const std::string& cut2="", double norm=1, const std::string& root_opt="", const std::string& opt="",  const std::string& leg="", bool scale_errors=false);


  /// Ratio between the two data samples  with different cuts
  void DrawRatio(TTree* tree1, TTree* tree2, const std::string& var, int nx, double xmin, double xmax,
		 const std::string& cut1, const std::string& cut2,  double norm=1, const std::string& root_opt="", const std::string& opt="",  const std::string& leg="");
  virtual void DrawRatio(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins,
			 const std::string& cut1,const std::string& cut2,  double norm=1, const std::string& root_opt="", const std::string& opt="",  const std::string& leg="");

  /// just Draw the ratio histogram
  void DrawRatio(TH1F* ratio, const std::string& root_opt, const std::string& uopt, const std::string& leg);

  void Project(HistoStack* hs1, HistoStack* hs2, TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, int ny, double* ybins, 
	       const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm2=1,bool scale_errors=true);


  void ProjectNew(HistoStack* hs1, HistoStack* hs2, TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, int ny, double* ybins, 
	       const std::string& categ="all", const std::string& cut1="", const std::string& cut2="", const std::string& root_opt="", const std::string& opt="", double norm2=1,bool scale_errors=true);

  //------- Basic drawing functions -------------

  /// dump histo contents
  void DumpHisto(TH1* histo, const std::string& uopt);

  /// Dump integral, underflow and overflow
  void DumpHistoInfo(TH1* histo, const std::string& uopt);

  void DrawHisto(TH1* h, int lc, int lw, int fc, int fs, const std::string& root_opt="", const std::string& opt="",const std::string& leg_opt="",int mode=0);
  void DrawHisto(TH1* h,         int lw, int fc, int fs, const std::string& root_opt="", const std::string& opt="",const std::string& leg_opt="",int mode=0);
  void DrawHisto(TH2* h, int lc, int lw, int fc, int fs, const std::string& root_opt="", const std::string& opt="",const std::string& leg_opt="",int mode=0);
  void DrawHisto(TH3F* h, int lc, int lw, int fc, int fs, const std::string& opt="");

  void DrawHistoStack(HistoStack* hs, const std::string& categ, const std::string& root_opt, const std::string& opt,const std::string& leg_opt="",int mode=2);

  void DrawHistoStack(HistoStack* hs, const std::string& categ, const std::string& root_opt, const std::string& opt, int lc, int lw, int fc, int fs, const std::string& leg_opt="",int mode=2);

  void DrawHistoStacks(HistoStack* hs1, HistoStack* hs2,
                       const std::string& categ, const std::string& root_opt, const std::string& opt, double norm);

  void DrawRatioHistoStacks(HistoStack* hs1, HistoStack* hs2,
                            const std::string& root_opt, const std::string& opt, double norm, const std::string& leg="");


  void Project(HistoStack* hs, const std::string& sample_name, TTree* tree, const std::string& var, int nx, double* xbins, int ny, double* ybins,
	       const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=1,bool scale_errors=true);

  void Project(HistoStack* hs, TTree* tree, const std::string& var, int nx, double* xbins, int ny, double* ybins,
               const std::string& idVar, std::vector<int> idSel, std::vector<std::string> idName, std::vector<int> idColor,
               const std::string& cut="",  const std::string& root_opt="", const std::string& opt="", double norm=1,  bool scale_errors=true);

  //------- Project the trees into histograms -------------

  TH1F* GetHisto(TTree* tree, const std::string& name, const std::string& var, int nx, double* xbins,
		 const std::string& cut, const std::string& root_opt, const std::string& opt, double scale, bool scale_errors=true, int toy_ref=-1);

  virtual TH2F* GetHisto(TTree* tree, const std::string& name, const std::string& var, int nx, double* xbins, int ny, double* ybins,
			 const std::string& cut, const std::string& root_opt, const std::string& opt, double scale, bool scale_errors=true, int toy_ref=-1);

  virtual TH1F* GetHisto(HistoStack* hs, TTree* tree, const std::string& name, const std::string& var, int nx, double* xbins,
			 const std::string& cut, const std::string& root_opt, const std::string& opt, TH1F*& hsyst, double scale, bool scale_errors=true, int toy_ref=-1);

  TH1F* GetHisto(HistoStack* hs, TTree* tree, const std::string& name, const std::string& var, int nx, double* xbins,
		 const std::string& cut, const std::string& root_opt, const std::string& opt, double scale, bool scale_errors=true, int toy_ref=-1);


  TH1F* GetRatioHisto(HistoStack* hs1, HistoStack* hs2, const std::string& opt);

  virtual TH1F* GetRatioHisto(TTree* tree1, TTree* tree2, const std::string& name, const std::string& var, int nx, double* xbins,
			      const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, double norm, double scale, bool scale_errors=true, int toy_ref=-1);

  virtual void FillHistoErrors(HistoStack* hs1, HistoStack* hs2, TTree* tree1,TTree* tree2, const std::string& name, const std::string& var, int nx, double* xbins,
			       const std::string& cut1, const std::string& cut2, const std::string& opt, double norm, TH1F* hstat, TH1F*& hsyst);
  
  virtual void FillHistoErrors(HistoStack* hs1, HistoStack* hs2, TH1F* histo, const std::string uopt);

  /// fill/update systematics information (fill appopriate histograms) for the stacks
  virtual void UpdateSystInfo(HistoStack* hs1, HistoStack* hs2, TTree* tree1,TTree* tree2, const std::string& var, int nx, double* xbins,
                              const std::string& cut1, const std::string& cut2, const std::string& opt, double norm);
  
  virtual void FillGraphErrors(HistoStack* hs1, HistoStack* hs2, TGraphAsymmErrors* graph,  const std::string uopt);

  //------ Drawing style ---------------


  /// Dump position and size of the legend/small legend
  void DumpLegendPos() {
    std::cout << "x = " << _legendPos[0]
              << ", y = " << _legendPos[1]
              << ", width = " << _legendSize[0]
              << ", height = " << _legendSize[1]
              << std::endl;
  }
  void DumpSmallLegendPos() {
    std::cout << "x = " << _legendPos[0]
              << ", y = " << _legendPos[1]
              << ", width = " << _legendSmallSize[0]
              << ", height = " << _legendSmallSize[1]
              << std::endl;
  }


  /// Set the height (in normalized coordinates: NDC) of each legend entry
  void SetLegendEntryHeight(double h){drawUtils::legendEntryHeight=h;}

  /// Set position and size of the legend
  void SetLegendParam(double a, double b, double c, double d);

  /// Set legend/small legend position by double (-999 keep current value)
  /// or by string ("l"=left, "r"=right , "c"=center, "t"=top, "b"=bottom)
  void SetLegendPos(double x=-999, double y=-999);
  void SetLegendPos(std::string pos);

  /// Set legend/small legend size, width and height (if -999 keep same value)
  void SetLegendSize(double w = -999, double h = -999);
  void SetSmallLegendSize(double w = -999, double h = -999){
    if (w != -999) _legendSmallSize[0] = w;
    if (h != -999) _legendSmallSize[1] = h;
  }

  /// Get position/size of the legend/small legend
  double GetLegendX() { return _legendPos[0]; }
  double GetLegendY() { return _legendPos[1]; }
  double GetLegendW() { return _legendSize[0]; }
  double GetLegendH() { return _legendSize[1]; }
  double GetSmallLegendX() { return _legendPos[0]+_legendSize[0]-_legendSmallSize[0]; }
  double GetSmallLegendY() { return _legendPos[1]+_legendSize[1]-_legendSmallSize[1]; }
  double GetSmallLegendW() { return _legendSmallSize[0]; }
  double GetSmallLegendH() { return _legendSmallSize[1]; }

  /// Set the stat option (by int or string)
  void SetOptStat(int opt){ _stat_option=opt; }
  void SetOptStat(Option_t *stat);

  /// Set/Get the stat box size
  double GetStatW() { return gStyle->GetStatH(); }
  double GetStatH() { return gStyle->GetStatW(); }
  void SetStatSize(double w = -999, double h = -999) {
    if (w != -999) gStyle->SetStatW(w);
    if (h != -999) gStyle->SetStatH(h);
  }

  /// Set/Get the stat box position (if -999 keep same value)
  double GetStatX() { return gStyle->GetStatX(); }
  double GetStatY() { return gStyle->GetStatY(); }
  void SetStatPos(double x = -999, double y = -999);

  /// Change the fill style
  void SetStackFillStyle(int FillStyle) { _stack_fill_style= FillStyle;};

  /// Make each histogram in the stack have a different fill style.
  void SetDifferentStackFillStyles(bool diff = true) { _different_fill_styles = diff; };

  /// change the marker style
  void SetMarkerStyle(int style) { _marker_style = style; }

  /// change the marker size
  void SetMarkerSize(double size) { _marker_size = size; }

  /// Set the fill style of the current histogram..
  void SetFillStyle(int style) { _fill_style = style; }

  /// Set the line width of the current histogram..
  void SetLineWidth(int width) { _line_width = width; }

  /// Set the line color of the current histogram..
  void SetLineColor(int color) { _line_color = color; }
  void SetLineColor(EColor kColor) { _line_color = kColor; }

  /// Set the line color of the current histogram..
  void SetFillColor(int color) { _fill_color = color; }
  void SetFillColor(EColor kColor) { _fill_color = kColor; }

  /// Set the color for the MC error bars
  void SetMCErrorColor(EColor kColor) { _mcerror_color = kColor; }
  void SetMCErrorColor(int color) { _mcerror_color = color; }

  void SetMCStatErrorColor(EColor kColor) { _mcstaterror_color = kColor; }
  void SetMCStatErrorColor(int color) { _mcstaterror_color = color; }

  /// Set the auto colors when superimposing histograms
  void SetAutoColors(int colors[],int ncolors){
    for (Int_t i=0;i<NMAXAUTOCOLORS;i++){
      if (i<ncolors) _auto_colors[i]= colors[i];
      else           _auto_colors[i]= _line_color;
    }
  }

  /// Set the auto Markers when superimposing histograms
  void SetAutoMarkers(int markers[], int nmarkers){
    for (Int_t i=0;i<NMAXAUTOCOLORS;i++){
      if (i<nmarkers) _auto_markers[i]= markers[i];
      else            _auto_markers[i]= _marker_style;
    }
  }

  /// Setter and Getter for params controlling the efficiency calculation
  const std::string& GetEffDivideParams() { return _eff_params;}
  void SetEffDivideParams(const std::string& params);
 
  /// Setter of default params
  void SetDefaultEffDivideParams();
  
  /// Set the title in X
  void SetTitleX(const std::string& titleX){_titleX=titleX;}

  /// Set the title in Y
  void SetTitleY(const std::string& titleY){_titleY=titleY;}

  /// Set the title that appears at the top of the plot.
  void SetTitle(const std::string& title){_title=title;}

  /// Set the label for the data when drawing comparison plots. Defaults to "Data".
  /// Setting it to "" (empty string) will disable adding it to the legend.
  void SetDataLabel(const std::string& label, Color_t color = kBlack) {
    _data_label = label;
    _data_color = color;
  }

  /// Set the label for the mc when drawing comparison plots. Defaults to "MC all".
  /// Setting it to "" (empty string) will disable adding it to the legend.
  void SetAllMCLabel(const std::string& label, Color_t color = kBlack) {
    _allmc_label = label;
    _allmc_color = color;
  }

  /// Set the label for the mc with only statistical errors when drawing comparison plots. Defaults to "MC all (stat)".
  /// Setting it to "" (empty string) will disable adding it to the legend.
  void SetAllMCStatLabel(const std::string& label){_allmcstat_label=label;}

  /// Set the label for the mc with only systematic errors when drawing comparison plots. Defaults to "MC all (syst)".
  /// Setting it to "" (empty string) will disable adding it to the legend.
  void SetAllMCSystLabel(const std::string& label){_allmcstat_label=label;}

  /// Set the minimum of the Y-axis. By default the minimum is set to 0.
  /// Call SetMinY() to revert to the default behaviour.
  void SetMinY(double minY=0);

  /// Set the maximum of the Y-axis. By default the maximum is set automatically by the DrawingTools.
  /// Call SetMaxY() to revert to the default behaviour.
  void SetMaxY(double maxY=0){_maxY = maxY;}

  /// Set the minimum and maximum of the Y-axis. See the documentation of SetMinY and SetMaxY for the
  /// default behaviour. Call SetRangeY() to revert to the default behaviour.
  void SetRangeY(double minY=0, double maxY=0) {SetMinY(minY); SetMaxY(maxY);}

  /// Reset the minimum of the Y-axis to be 0.
  /// OBSOLETE: just call SetMinY();
  void ResetMinY(){_minY = 0;}

  /// Reset the maximum of the Y-axis to be set automatically by the DrawingTools.
  /// OBSOLETE: just call SetMax();
  void ResetMaxY(){_maxY = 0;}

  /// Reset the minimum and maximum of the Y-axis. See ResetMinY() and ResetMaxY();
  /// OBSOLETE: just call SetRangeY();
  void ResetRangeY(){ResetMinY(); ResetMaxY();}

  /// Set the current histogram to be drawn with a logarithmic Y axis.
  void SetLogY(bool logY = true);

  /// Set the current 2D histogram to be drawn with a logarithmic Z axis (works only for category "all").
  void SetLogZ(bool logZ = true);

  /// Set the maximum value in Y relative to the bin with maximum content
  void SetRelativeMaxY(double maxY){_relativeMaxY=maxY;}

  /// Gets the root error style from the user option
  std::string GetErrorStyle(const std::string& opt);

  /// switch on/off 2p2h type for reaction histos
  void SetDraw2p2h(bool draw = true){_draw_2p2h=draw;}

  //------ Draw cut lines --------------

  /// Draw a vertical line at the specified X value, spanning the currently-drawn histogram.
  /// See note in DrawCutLine() about legends being re-drawn when calling this function.
  /// If addarrow is set to true, then an arrow indicating the direction of the cut is drawn.
  /// arrowdir respects the following options:
  ///  l : Draw the arrow pointing to the left
  ///  r : Draw the arrow pointing to the right (default)
  /// arrowpos specifies the fractional length along the cut line to draw the arrow at, measuring
  /// from the bottom of the line. Defaults to half way along.
  void DrawCutLineVertical(double xval, bool addarrow = false, std::string arrowdir = "l", double arrowpos = 0.5);

  /// Draw a horizontal line at the specified Y value, spanning the currently-drawn histogram.
  /// See note in DrawCutLine() about legends being re-drawn when calling this function.
  /// If addarrow is set to true, then an arrow indicating the direction of the cut is drawn.
  /// arrowdir respects the following options:
  ///  u : Draw the arrow pointing upwards (default)
  ///  d : Draw the arrow pointing downwards
  /// arrowpos specifies the fractional length along the cut line to draw the arrow at, measuring
  /// from the left end of the line. Defaults to half way along.
  void DrawCutLineHorizontal(double yval, bool addarrow = false, std::string arrowdir = "u", double arrowpos = 0.5);

  /// Draw an abritary line between two points. If a legend had already been created by the
  /// DrawingTools, and drawn on the current pad, it is re-drawn. This means that the line
  /// does not cover it up.
  void DrawCutLine(double xmin, double ymin, double xmax, double ymax);

  /// Draw a box defined by the minimum and maximum points. The opt parameter specifies,
  /// among other things, which sides of the box to draw:
  /// t : draw the (t)op of the box
  /// b : draw the (b)ottom of the box
  /// l : draw the (l)eft of the box
  /// r : draw the (r)ight of the box
  /// nochop : Don't constrain the sides of the box to stay within the current axes.
  ///
  /// See note in DrawCutLine() about legends being re-drawn when calling this function.
  void DrawCutRegion(double xmin, double ymin, double xmax, double ymax, std::string opt = "tblr");

  /// Draw an arrow between two points. If a legend had already been created by the
  /// DrawingTools, and drawn on the current pad, it is re-drawn. This means that the line
  /// does not cover it up.
  void DrawCutArrow(double xmin, double ymin, double xmax, double ymax);

  /// Set the color of lines drawn with the DrawCut* functions.
  void SetCutLineColor(int col) { _cut_line_col = col; }
  void SetCutLineColor(EColor kCol) { _cut_line_col = kCol; }

  /// Set the width of lines drawn with the DrawCut* functions.
  void SetCutLineWidth(int width) {
    _cut_line_width = width;
  }

  //------ Toy Experiments ---------------

  /// Provide easy access to the managers. 
  CorrectionManager&    corr(){return _corrections;}
  SystematicManager&    syst(){return _systematics;}
  SelectionManager&     sel() {return _selections;}
  ConfigurationManager& conf(){return _configurations;}
  CategoryManager&      cat() {return _categories;}
  DocStringManager&     doc() {return _docStrings;}

  /*
  void ReadManager(const std::string& file, const std::string& name, void* address){
    TChain* _chain = new TChain("config");
    _chain->AddFile(file.c_str());    
    // Set the branch addresses to read the tree
    _chain->SetBranchAddress(name.c_str(), address);
    Long64_t centry = _chain->LoadTree(0);
    Int_t nb = _chain->GetEntry(0);
  }
  */
  Header& header(){return _header;}

  /// an access to tools instance
  SystematicsTools& syst_tools()   {return _syst_tools;}
  ConfigTreeTools&  config_tools() {return _configTree_tools;}

  /// Print out the Standard Systematics for a given configuration 
  void DumpVariationSystematics(){
    syst().EnableAllVariationSystematics();
    syst().DumpVariationSystematics();
  }

  /// Print out the Weight Systematics for a given configuration 
  void DumpWeightSystematics(){
    syst().EnableAllWeightSystematics();
    syst().DumpWeightSystematics();
  }

  /// Print out all Systematics for a given configuration 
  void DumpSystematics(){
    syst().DumpSystematics();
  }

  /// Print out the Corrections
  void DumpCorrections(const std::string& file=""){
    if (file!="") ReadCorrections(file);
    corr().DumpCorrections();
    if (_config_file!="") ReadCorrections(_config_file);
  }

  /// Read the list of Corrections
  void ReadCorrections(const std::string& file){
    //    ReadManager(file,"CorrMan", &_corrections);
    corr().ReadCorrections(file);
  }

  void ReadSystematics(const std::string& file){
    //    ReadManager(file,"SystMan", &_systematics);
    syst().ReadSystematics(file);
  }

  /// Print out the Configurations
  void DumpConfigurations(const std::string& file=""){
    if (file!="") ReadConfigurations(file);
    conf().DumpConfigurations();
    if (_config_file!="") ReadConfigurations(_config_file);
  }

  /// Print out the Configuration
  void DumpConfiguration(const std::string& name);


  /// Read the list of Configurations
  void ReadConfigurations(const std::string& file){
    //    ReadManager(file,"ConfMan", &_configurations);
    conf().ReadConfigurations(file);
  }

  ///------ Package versions -------------
  
  /// Dumps on the screen the highland/psyche software versions used to produce this file
  void DumpSoftwareVersions(const std::string& file="");

  /// Dumps on the screen the software version of the original file (i.e. output of recon) 
  void DumpOriginalSoftwareVersion(const std::string& file="", bool all=true);

  ///------ Package versions -------------


  void DumpFileInfo(const std::string& file="");

  //------ Track Categories ---------------

  /// Print out the list of categories which can be used when plotting stacked histograms
  void DumpCategories(){
    cat().DumpCategories();
  }

  /// Read the list of categories stored in the given file which can be used when plotting
  /// stacked histograms
  void ReadCategories(const std::string& file){
    //    ReadManager(file,"CatMan", &_categories);
    cat().ReadCategories(file);
  }

  /// Print out the list of categories stored in the given file which can be used when plotting
  /// stacked histograms
  void DumpCategories(const std::string& file){
    ReadCategories(file);
    cat().DumpCategories();
    if (_config_file!="") ReadCategories(_config_file);
  }

  /// Print the options saved for the given category, including the name of each category, the value
  /// stored in the output file for that category, and the colour used to plot the category.
  void DumpCategory(std::string category) {
    cat().DumpCategory(category);
  }

  /// Update or add a category.
  /// * categ_name is the name this category will have. This must be a variable that is stored in the
  ///   output file, such as "particle" (you can of course save your own variables).
  /// * ntypes is the number of options your category will have.
  /// * names is an array of length ntypes of the name of each option.
  /// * codes is an array of length ntypes of the value stored in the output file for this option.
  /// * colors is an array of length ntypes of the colour to plot each option with.
  void ChangeCategory(const std::string& categ_name, int ntypes, std::string* names, int* codes, int* colors);

  bool HasCategory(const std::string& categ_name);

  //------ Cuts ---------------

  /// Read the list of Selections
  void ReadSelections(const std::string& file){
    //    _selections = NULL;
    //    ReadManager(file,"SelMan", &_selections);
    sel().ReadSelections(file);

  }

  /// Dump the list of Selections
  void DumpSelections(const std::string& file=""){
    if (file!="") sel().ReadSelections(file);
    sel().DumpSelections();
    if (_config_file!="") ReadSelections(_config_file);
  }

  /// Dump the list of branches in the selection provided selection index
  void DumpBranches(Int_t sel_index=0){
    if (sel().GetSelection(sel_index)) sel().GetSelection(sel_index)->DumpBranches();
  }

  /// Dump the list of branches in the selection provided selection name
  void DumpBranches(const std::string& sel_name){
    if (sel().GetSelection(sel_name)) sel().GetSelection(sel_name)->DumpBranches();
  }


  /// Print to screen the details of cuts for a given branch or selection. Specifying a branch of
  /// -1 (the default) will print details for all branches.
  /// When there are multiple selections the input index is for the selection
  void DumpCuts(int branch=-1){
    if (sel().GetNEnabledSelections()>1){
      if (sel().GetSelection(branch)) sel().GetSelection(branch)->DumpCuts();
    }
    else
      sel().GetSelection(0)->DumpCuts(branch);
  }

  /// Print to screen the details of steps for a given branch or selection. Specifying a branch of
  /// -1 (the default) will print details for all branches.
  /// When there are multiple selections the input index is for the selection
  void DumpSteps(int branch=-1){
    if (sel().GetNEnabledSelections()>1){
      if (sel().GetSelection(branch,true)) sel().GetSelection(branch)->DumpSteps();
    }
    else
      sel().GetSelection(0)->DumpSteps(branch);
  }

  /// Print to screen the details of cuts for a given branch and selection (name). Specifying a branch of
  /// -1 (the default) will print details for all branches.
  void DumpCuts(const std::string& sel_name, int branch=-1){
    if (sel().GetSelection(sel_name)) sel().GetSelection(sel_name)->DumpCuts(branch);
  }

  /// Print to screen the details of steps for a given branch and selection (name). Specifying a branch of
  /// -1 (the default) will print details for all branches.
  void DumpSteps(const std::string& sel_name,int branch=-1){
    if (sel().GetSelection(sel_name)) sel().GetSelection(sel_name)->DumpSteps(branch);
  }

  /// Print to screen the details of cuts for a given branch and selection (index). Specifying a branch of
  /// -1 (the default) will print details for all branches.
  void DumpCuts(Int_t sel_index, int branch){
    if (sel().GetSelection(sel_index)) sel().GetSelection(sel_index)->DumpCuts(branch);
  }

  /// Print to screen the details of steps for a given branch and selection (index). Specifying a branch of
  /// -1 (the default) will print details for all branches.
  void DumpSteps(Int_t sel_index,int branch){
    if (sel().GetSelection(sel_index)) sel().GetSelection(sel_index)->DumpSteps(branch);
  }

  /// Get a vector of cut names for a given branch (in order) in a selection with a given name
  std::vector<std::string> GetCutNames(const std::string& sel_name, int branch=0) {
    if (sel().GetSelection(sel_name)) return sel().GetSelection(sel_name)->GetCutNames(branch);
    return std::vector<std::string>();
  }

  /// Get a vector of steps names for a given branch (in order) in a selection with a given name
  std::vector<std::string> GetStepNames(const std::string& sel_name, int branch=0) {
    if (sel().GetSelection(sel_name)) return sel().GetSelection(sel_name)->GetStepNames(branch);
    return std::vector<std::string>();
  }
  
  /// Get a vector of cut names for a given branch (in order) in a selection with a given index
  std::vector<std::string> GetCutNames(Int_t sel_index, int branch) {
    if (sel().GetSelection(sel_index)) return sel().GetSelection(sel_index)->GetCutNames(branch);
    return std::vector<std::string>();
  }

  /// Get a vector of steps names for a given branch (in order) in a selection with a given index
  std::vector<std::string> GetStepNames(Int_t sel_index, int branch) {
    if (sel().GetSelection(sel_index)) return sel().GetSelection(sel_index)->GetStepNames(branch);
    return std::vector<std::string>();
  }

  //------ Documentation -----

  /// Explain the user drawing options
  void ExplainOption(std::string name) {drawUtils::ExplainOption(name);}

  /// List all drawing options
  void ListOptions(){drawUtils::ListOptions();}


  /// Explain the meaning of a variable stored in a tree.
  void ExplainVar(std::string name, std::string tree_name = "default") {
    doc().ExplainVar(name, tree_name);
  }

  /// List all the variables stored in a tree.
  void ListVars(std::string tree_name = "default") {
    doc().ListVars(tree_name);
  }

  /// Read the details of docstrings from a given file. The details will have been read automatically
  /// if you passed the path to a valid highland file in the DrawingTools or DrawingToolsBase constructor.
  void ReadDocStrings(const std::string& file){
    //    ReadManager(file,"DocMan", &_docStrings);
    doc().ReadDocStrings(file);
  }

  //------ POT ---------------

  /// Dump the POT information for this sample, provided the header tree
  void DumpPOT(TTree* tree){
    header().ReadHeader(tree);
    header().DumpPOT();
  }

  /// Dump the POT information for this sample, provided an input file name
  void DumpPOT(const std::string& file){
    header().ReadHeader(file);
    header().DumpPOT();
  }


  /// Read config tree
  void ReadConfig(const std::string& file);

  /// Read any other variables in the config tree
  void ReadOther(const std::string& file);
  void ReadOther(TTree* chain);

  /// Print on the screen the purities for the different categories defined by "categ" and with cuts "cut".
  /// The expected number of events are also printed, after scaling by events_ratio.
  void PrintPurities(TTree* tree, const std::string& categ,  const std::string& cut, double events_ratio = 1);

  /// Get all the graphs that have been drawn. Useful if you want to save them to file.
  std::vector< TGraph* > GetGraphs() { return _saved_graphs; }

  /// Get the last graph that was drawn. Useful if you want to modify its appearance.
  TGraph* GetLastGraph() { return _saved_graphs.back(); }

  /// Get all the 1D histos that have been drawn. Useful if you want to save them to file.
  /// NOTE! The "same" plot may appear multiple times due to the way the comparison
  /// histograms are drawn.
  std::vector< TH1* > GetHistos() { return _saved_histos; }

  /// Get the last legend that was drawn. Useful if you want to modify its appearance.
  TLegend* GetLastLegend() { return _legends.back(); }

  /// Change the label or drawing style of a entry in the legend provided the entry number 
  void ChangeLegendEntry(Int_t index, const std::string& new_label="", const std::string& new_opt="");

  /// Delete a given Legent entry provided its index
  void DeleteLegendEntry(Int_t index);

  /// Get all the 1D histos that have been drawn. Useful if you want to save them to file.
  /// NOTE! The "same" plot may appear multiple times due to the way the comparison
  /// histograms are drawn.
  std::vector< TH1* > Get2DHistos() { return _saved_histos; }

  /// Get all the last Histo that was drawn. Useful if you want to modify its appearance.
  TH1* GetLastHisto() {
    std::cout << "If it crashes you should maybe use GetLastStackTotal instead.\n";
    if (_saved_histos.back()) return _saved_histos.back();
    else std::cout << "No histos found. You can try GetLastStackTotal \n"; return NULL; 
  }

  /// Get the total from the most recently-drawn stacked histogram. Useful if you want to
  /// save it to file.
  TH1* GetLastStackTotal(int nexttolast=0) {
    std::cout << "If it crashes you should maybe use GetLastHisto instead.\n";
    if ( ! _saved_histoStacks.back()) {
      std::cout << "No histoStacks found. You can try GetLastHisto\n"; return NULL; }
    if (nexttolast>0){
      if ((int)_saved_histoStacks.size() <= nexttolast) {
        return NULL;
      } else {
        HistoStack* s = _saved_histoStacks[(int)_saved_histoStacks.size() - nexttolast - 1];
        return s->GetTotal1D();
      }
    }
    else{
      HistoStack* s = _saved_histoStacks.back();
      return s->GetTotal1D();
    }
  }

  /// Get the most recently-drawn stacked histogram. Useful to access its histograms
  HistoStack* GetLastStack(int nexttolast=0) {
    if ( ! _saved_histoStacks.back()) {
      std::cout << "No histoStacks found. You can try GetLastHisto\n"; return NULL; }
    if (nexttolast>0){
      if ((int)_saved_histoStacks.size() <= nexttolast) {
        return NULL;
      } else {
        return _saved_histoStacks[(int)_saved_histoStacks.size() - nexttolast - 1];
      }
    }
    else{
      return _saved_histoStacks.back();
    }
  }



  /// Start the creation of a PDF document and/or webpage which will contain the plots you make.
  /// This makes it particularly easy to share many plots.
  /// pdffile is the path to the PDF FILE that will be created
  /// webpath is the path to the DIRECTORY in which the webpage and images will be placed.
  ///
  ///
  /// There are two possible use cases for these functions:
  /// draw.StartDocument(...);   // Will create a canvas for you
  /// draw.Draw(...);            // Will draw on the right canvas
  /// draw.AddDocumentPage(...); // Will save the current page
  /// ...
  /// draw.FinishDocument();     // Finish and close
  ///
  /// OR
  ///
  /// TCanvas canvas(...);       // Create your own canvas
  /// draw.StartDocument(...);   // Won't create a canvas for you
  /// draw.Draw(...);            // Will draw on your canvas
  /// draw.AddDocumentPage(...); // Will save the current page
  /// ...
  /// draw.FinishDocument();     // Finish and close
  ///
  /// In the first case, a canvas was automatically created. This canvas is
  /// split into two sections - a header and a body. The header will be populated
  /// with the "name" you provide in AddDocumentPage. This makes it easier to
  /// understand what each plot is.
  /// In the second case, it is assumed you know what you want, so we don't create
  /// this header section.
  void StartDocument(const std::string& title, bool pdf = true, bool web = false, const std::string& pdffile = "plots.pdf", const std::string& webpath = "plots");

  /// Add a page to the PDF document and/or webpage that was opened with StartDocument().
  /// "name" will appear on web pages to give context to what each plot is. If StartDocument()
  /// createdg a canvas for you, "name" will also appear in the header section of the PDF page.
  void AddDocumentPage(const std::string& name);

  /// Close the PDF document / webpage that was opened with StartDocument. Note that you MUST call this
  /// function or your PDF document will be corrupt / the webpage will be empty.
  void FinishDocument();

  /// Make a directory on the filesystem.
  int MakeDirectory(const std::string& path, mode_t mode);


  //------ UTILS ---------------

  // replace accum_level[] with accum_level and selmu_ with truemu_
  std::string ConvertCutForTruthTree(const std::string cut, bool replace_selmu = true);

  /// Compute the Chi2 without normalizing by area (as Chi2Test of root does)
  /// h1/h2 should be already normalized by POT, with errors properly propagated: just get them from your DATA-mc plot with GetLastStackTotal(0) and GetLastStackTotal(1)
  double GetChi2(const TH1* h1, const TH1* h2);
  int GetNDOF(const TH1* h1, const TH1* h2);
  double GetPvalue(double chi2, int ndof) {return TMath::Prob(chi2,ndof);}
  double GetPvalue(const TH1* h1, const TH1* h2) {
    double chi2 = GetChi2(h1, h2);
    int ndof = GetNDOF(h1, h2);
    return TMath::Prob(chi2,ndof);
  }


 protected:

  /// Get the fill style to apply to the i'th histogram in a stack. Depends on whether
  /// SetDifferentStackFillStyles() has been called.
  int GetFillStyle(int i);

  /// Counter for ensuring all histograms get a unique name
  int _unique;

  bool _drawleg;
  std::vector<TLegend*> _legends;

  // appearence parameters
  double _legendParam[4];
  double _legendSize[2];
  double _legendPos[2];
  double _legendSmallSize[2];
  double _statPos[2];
  std::string _legendPosS;
  int _stat_option;
  int _stack_fill_style;
  bool _different_fill_styles;
  int _marker_style;
  int _fill_style;
  int _fill_color;
  int _line_width;
  int _line_color;
  int _mcerror_color;
  int _mcstaterror_color;
  double _marker_size;
  int _cut_line_col;
  int _cut_line_width;
  std::string _titleX;
  std::string _titleY;
  std::string _title;
  Color_t _data_color;
  Color_t _allmc_color;
  std::string _data_label;
  std::string _allmc_label;
  std::string _allmcstat_label;
  std::string _allmcsyst_label;
  double _minY;
  double _maxY;
  double _relativeMaxY;
  bool _logY;
  bool _logZ;
 
  /// parameter to control options for TGraphAsymmErrors::Divide() method, the one used to get the efficiencies
  std::string _eff_params; 
   
  // switch on/off 2p2h type for reaction histos
  bool _draw_2p2h;

  // input file
  TFile *_file;

  int _same_level;
  int _same_level_tot;
  int _auto_colors[NMAXAUTOCOLORS];
  int _auto_markers[NMAXAUTOCOLORS];

  // keep pointers to avoid memory leaks
  std::vector< TH1* > _saved_histos;
  std::vector< TH2* > _saved_histos2D;
  std::vector< HistoStack* > _saved_histoStacks;
  std::vector< TGraph* > _saved_graphs;
  std::vector< TLine* > _cut_lines;

  TCanvas* _pdfcanvas;
  TPad* _bodypad;
  TPad* _headpad;
  std::string _pdfpath;
  std::string _webpath;
  std::string _webheader;
  std::string _webbody;

  std::string _config_file;

  DocStringManager     _docStrings;
  CategoryManager      _categories;
  ConfigurationManager _configurations;
  CorrectionManager    _corrections;
  SystematicManager    _systematics;
  SelectionManager     _selections;
  SystematicsTools     _syst_tools;
  ConfigTreeTools      _configTree_tools;
  Header _header;

  Int_t _minAccumLevelToSave;

};


#endif

