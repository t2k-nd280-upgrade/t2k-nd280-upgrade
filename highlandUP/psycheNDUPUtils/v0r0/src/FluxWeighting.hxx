#ifndef FluxWeighting_h
#define FluxWeighting_h

#include "BaseDataClasses.hxx"
#include <TH1D.h>
#include <TFile.h>
#include <string>

/// This class provides functions to weight events according to a flux tuning.
/// A flux tuning file must be downloaded from t2k.org, and be accessible to
/// this code. The path to this file must be specified in the constructor of
/// this class. Analysers may wish to use the Parameters class, so that the
/// path to the weighting file may be specified in a parameters file, rather
/// than hard-coded.
///
/// Flux tuning files can be downloaded from:
///   http://www.t2k.org/beam/NuFlux/FluxRelease
///
/// baseAnalysis provides tools to simplify the application of the FluxWeighting.
/// In your analysis class, you should specify the following three lines in your
/// class constructor:
///  _applyFluxWeight = <whether to enable the flux weighting>;
///  _fluxFile = <path to the file containing flux histograms>;
///  _fluxTuning = <name of the tuning to apply (say, tuned11b)>;
///
/// baseAnalysis will then save the appropriate weight based on the true neutrino
/// flavour and energy in the ND::box().Vertex->TrueVertex member.
/// So, you should set the the ND::box().Vertex and ND::box().Vertex->TrueVertex
/// members in your analysis, based on the track you have selected. See
/// std_numucc_actions::FindVertex() for an example of how to set them. Note that
/// std_numucc_actions::FindVertex() is called in StandardCutsNumuCC::ApplyAllCuts().
class FluxWeighting {

  public:

    /// Instantiate the flux tuning, reading in the relevant histograms for
    /// applying the flux tuning.
    ///
    /// fluxfile is the ROOT file containing the flux weighting histograms.
    ///
    /// tuning if the name of the tuning to use, as specified in the histogram
    /// titles. For example, the 'tuned11b' tuning will use the histograms with
    /// titles like enu_nd5_tuned11b_numu_ratio.
    FluxWeighting(std::string fluxfile, std::string version, std::string tuning);

    virtual ~FluxWeighting();

    /// Update the weight associated with this bunch, based on the true vertex.
    /// The bunch weight is saved automatically to the output micro-tree, and
    /// is used when plotting histograms with the DrawingTools.
    void UpdateBunchWeight(AnaBunchB& bunch, AnaTrueVertexB* vertex, int RunPeriod);

    /// Update the weight associated with this event, based on the true vertex.
    /// The bunch weight is saved automatically to the output micro-tree, and
    /// is used when plotting histograms with the DrawingTools.
    void UpdateEventWeight(AnaEventB& event, AnaTrueVertexB* vertex);


    /// Get the flux weighting to apply to this vertex, based on the true
    /// neutrino type and energy.
    ///
    /// Analysers should generally use the UpdateBunchWeight() function, rather
    /// than the GetWeight() function, as the former will automatically save
    /// the weight to the output micro-tree, for use with the DrawingTools.
    Float_t GetWeight(AnaTrueVertexB* vertex, int RunPeriod);

    /// const-safe version of the GetWeight function. See other interface for
    /// details.
    Float_t GetWeight(const AnaTrueVertexB& vertex, int RunPeriod);

  private:

    /// Histogram of weights for muon neutrinos.
    TH1D* run1_weight_numu;
    TH1D* run2_weight_numu;
    TH1D* run3b_weight_numu;
    TH1D* run3c_weight_numu;
    TH1D* run4_weight_numu;
    TH1D* run5a_weight_numu;
    TH1D* run5b_weight_numu;
    TH1D* run5c_weight_numu;
    TH1D* run6b_weight_numu;
    TH1D* run6c_weight_numu;
    TH1D* run6d_weight_numu;
    TH1D* run6e_weight_numu;

    /// Histogram of weights for muon anti-neutrinos.
    TH1D* run1_weight_numubar;
    TH1D* run2_weight_numubar;
    TH1D* run3b_weight_numubar;
    TH1D* run3c_weight_numubar;
    TH1D* run4_weight_numubar;
    TH1D* run5a_weight_numubar;
    TH1D* run5b_weight_numubar;
    TH1D* run5c_weight_numubar;
    TH1D* run6b_weight_numubar;
    TH1D* run6c_weight_numubar;
    TH1D* run6d_weight_numubar;
    TH1D* run6e_weight_numubar;

    /// Histogram of weights for electron neutrinos.
    TH1D* run1_weight_nue;
    TH1D* run2_weight_nue;
    TH1D* run3b_weight_nue;
    TH1D* run3c_weight_nue;
    TH1D* run4_weight_nue;
    TH1D* run5a_weight_nue;
    TH1D* run5b_weight_nue;
    TH1D* run5c_weight_nue;
    TH1D* run6b_weight_nue;
    TH1D* run6c_weight_nue;
    TH1D* run6d_weight_nue;
    TH1D* run6e_weight_nue;


    /// Histogram of weights for electron anti-neutrinos.
    TH1D* run1_weight_nuebar;
    TH1D* run2_weight_nuebar;
    TH1D* run3b_weight_nuebar;
    TH1D* run3c_weight_nuebar;
    TH1D* run4_weight_nuebar;
    TH1D* run5a_weight_nuebar;
    TH1D* run5b_weight_nuebar;
    TH1D* run5c_weight_nuebar;
    TH1D* run6b_weight_nuebar;
    TH1D* run6c_weight_nuebar;
    TH1D* run6d_weight_nuebar;
    TH1D* run6e_weight_nuebar;

    /// The input file containing the weights histograms.
    TFile* run1_file;
    TFile* run2_file;
    TFile* run3b_file;
    TFile* run3c_file;
    TFile* run4_file;
    TFile* run5a_file;
    TFile* run5b_file;
    TFile* run5c_file;
    TFile* run6b_file;
    TFile* run6c_file;
    TFile* run6d_file;
    TFile* run6e_file;

};

#endif
