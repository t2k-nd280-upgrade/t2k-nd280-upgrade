#ifndef HistoStack_h
#define HistoStack_h

#include <map>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <TH1F.h>
#include <TH2F.h>
#include <THStack.h>
#include <TLegend.h>
#include "DrawingUtils.hxx"

/// This class is used by the DrawingTools and DrawingToolsBase classes to aid
/// the creation of stacked histograms.
///
/// The stacked histograms are created based on track categories (for example,
/// "particle") and track types within those categories (for example, "muon"
/// and "electron"). The track categories are from the TrackCategoryDefinition
/// class, and the types from the TrackTypeDefinition class.
///
/// The underlying implementation does not use a THStack, but overlaid
/// histograms. This means we the same logic can be used to stack both TH1
/// and TH2 histograms.
class HistoStack {

  public:

    /// Instantiate the object, setting the title of the plot, the X axis, and
    /// the Y axis.
    HistoStack(const std::string& title, const std::string& titleX, const std::string& titleY);

    /// Destructor, which cleans up the cumulative histograms used internally
    /// by the class. The user should delete the histograms that were added to
    /// the stack.
    virtual ~HistoStack();

    /// Tells whether this is a 1D histo 
    bool Is1D(){return _is1D;}

    /// Tells whether this is a 2D histo 
    bool Is2D(){return _is2D;}

    /// Add a new 1D histogram to the stack, with fill colour "fc", line colour "lc".
    void Add(TH1F* h1, int lc, int lw, int fc, int fs,const std::string& leg);

    /// Add a new 2D histogram to the stack, with fill colour "fc", line colour "lc".
    void Add(TH2F* h2, int fc, int lc, const std::string& leg="");

    /// Draw the stack of histograms that have been added.
    /// If "root_opt" contains "same", then the current active pad is re-used.
    /// If "opt" contains "ETOT", then error bars are drawn on the total stack.
    void Draw(int lc, int lw, int fc, int fs, const std::string& root_opt="", const std::string& opt="",const std::string& leg_opt="",int mode=0);

    /// Same as above but with a predefined style
    void Draw(const std::string& root_opt = "", const std::string& opt = "");

    /// The total histogram is normally the sum of all the histograms that were
    /// added to the stack. This can be overriden (before calling Draw()).
    void SetTotal(TH1F* h1) { _hall1D = h1; }

    /// The total histogram is normally the sum of all the histograms that were
    /// added to the stack. This can be overriden (before calling Draw()).
    void SetTotal(TH2F* h2) { _hall2D = h2; }

    /// Return the total 1D histo 
    TH1F* GetTotal1D() { return _hall1D; }

    /// Return the total 1D histo with only stat errors
    TH1F* GetTotalStat1D() { return _hall1D_stat; }

    /// Return the total 1D histo with only systematic errors
    TH1F* GetTotalSyst1D() { return _hall1D_syst; }

    /// Return the total 2D histo 
    TH2F* GetTotal2D() { return _hall2D; }

    /// Sets the total 1D histo if it does not exists or adds to the previous one when it exists
    void AddTotal(TH1F* h1, TH1F* hsyst=NULL);

    /// Add histos for updating systematics when using several files
    void AddSystHistos(TH2F* h1, TH2F* h2,  TH2F* h1w);

    /// Get Histos for updating systematics when using several files
    void GetSystHistos(const std::string& group, TH2F*& h1, TH2F*& h2, TH2F*& h1w);

    /// Add a new group of systemtic histos
    void AddSystHistosGroup(const std::string& group){_systHistosGroups.push_back(group);}

    /// Set the current Systematics group
    void SetCurrentSystGroup(const std::string& group){_currentSystGroup = group;}


    const std::string& GetCurrentSystGroup(){return _currentSystGroup;}

    /// get the vector of groups of systemtic histos
    std::vector<std::string> GetSystHistosGroups() const {return _systHistosGroups;}

    /// Sets the total 2D histo if it does not exists or adds to the previous one when it exists
    void AddTotal(TH2F* h2);

    /// Fill the legend with info in the HistoStack
    void FillLegend(TLegend* leg);

    /// Returns one of the histos in the 1D stack. The title is used for comparison
    TH1F* GetHisto1D(const std::string& title);

    /// Returns the 1D histo with a given index
    TH1F* GetHisto1D(Int_t index);

    /// Returns one of the histos in the 2D stack. The title is used for comparison
    TH2F* GetHisto2D(const std::string& title);

    /// Returns the 2D histo with a given index
    TH2F* GetHisto2D(Int_t index);

    /// Dump the stack contents
    void Print() const;

    /// normalize all histos in the stack by area
    void NormalizeByArea(const std::string& uopt, double area=1);

    /// delete all temporary histos used for drawing the stak (with variable binning normalization) 
    void ResetDrawHistos();

    /// Get the maximum for the HistoStack
    double GetMaximum(const std::string& opt="");

    /// Get the maximum for the HistoStack taking into account the upper error
    double GetMaximumWithError(const std::string& opt="");

    /// Set the maximum for the HistoStack
    void SetMaximum(double max);

    /// Set the minimum for the HistoStack
    void SetMinimum(double min);

  protected:

    /// The TH1Fs that were added to the stack.
    std::vector<TH1F*> _histos1D;

    /// The TH1Fs that were added to the stack. Temporary histos used for drawing the stack (with variable binning normalization) 
    std::vector<TH1F*> _histos1D_draw;

    /// The TH2Fs that were added to the stack
    std::vector<TH2F*> _histos2D;

    ///  The root stack
    THStack* _stack;

    /// The "total" 1D histogram. Nominally the sum of all the stacked
    /// histograms, but can be set by the user.
    TH1F* _hall1D;

    /// The "total" 1D histogram with only stat errors
    TH1F* _hall1D_stat;

    /// The "total" 1D histogram with only systematic errors
    TH1F* _hall1D_syst;

    /// temporary histo used for drawing the total (with variable binning normalization) 
    TH1F* _hall1D_draw;

    /// The "total" 2D histogram. Nominally the sum of all the stacked
    /// histograms, but can be set by the user.
    TH2F* _hall2D;

    /// The title of the plot.
    std::string _title;

    /// The title of the X axis.
    std::string _titleX;

    /// The title of the Y axis.
    std::string _titleY;

    ///  is it 1D ?
    bool _is1D;

    ///  is it 2D ?
    bool _is2D;

    /// Histos for updating systematics when using several files
    std::vector<std::string> _systHistosGroups;

    std::string _currentSystGroup;

    std::map<std::string,TH2F*> _h1;
    std::map<std::string,TH2F*> _h1w;
    std::map<std::string,TH2F*> _h2;

};

#endif


