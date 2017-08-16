#ifndef nueCCAnalysis_h
#define nueCCAnalysis_h

#include "nueCCSelection.hxx"

#include "baseAnalysis.hxx"

class nueCCAnalysis: public baseAnalysis {
public:
  nueCCAnalysis(AnalysisAlgorithm* ana = NULL);
  virtual ~nueCCAnalysis() {}

  //---- These are mandatory functions
  void DefineSelections();
  void DefineCorrections();
  void DefineSystematics();
  void DefineConfigurations();
  void DefineMicroTrees(bool addBase = true);
  void DefineTruthTree();
  void FillMicroTrees(bool addBase = true);
  void FillToyVarsInMicroTrees(bool addBase = true);
  bool CheckFillTruthTree(const AnaTrueVertex & vtx);

  virtual const ToyBoxCC4pi& cc4pibox(Int_t isel=-1) const {return *dynamic_cast<const ToyBoxCC4pi*>(&boxB(isel));}

  // Return the vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaVertexB * GetVertex() const { return cc4pibox().Vertex;} //box().Vertex;};

  // Return the true vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaTrueVertexB * GetTrueVertex() const { return cc4pibox().TrueVertex;} //box().TrueVertex;};

  using baseAnalysis::FillTruthTree;
  using baseAnalysis::FillTruthTreeBase;
  void FillTruthTree(const AnaTrueVertex& vtx);

  bool Initialize();
  void FillCategories();

private:
  bool _useTarget1;
  bool _useTarget2;
  bool _useFGD1;
  bool _useFGD2;

  bool _AntiNu;

public:

  enum enumStandardMicroTrees_numuCC4piAnalysis {
    truelepton_mom = enumStandardMicroTreesLast_baseAnalysis + 1,
    truelepton_PDG,
    truelepton_dir,
    truelepton_costheta,
    truelepton_pos,
    truelepton_fgdlayer,
    truelepton_det,
    // toy variables
    NTPC,
    NECal,
    Toy_CC4pi,
    selelec_mom,
    //selelec_MomRangeMuon,
    selelec_costheta,
    //selelec_ToF_PoD,
    //selelec_ToF_ECAL,
    //selelec_ToF_FGD2,
    selelec_pull_mu,
    selelec_pull_ele,
    selelec_pull_pi,
    selelec_pull_pr,
    selelec_likemu,
    selelec_likeel,
    selelec_likepi,
    selelec_likepr,
    selelec_likemip,
    //selelec_end_ecal,

    selelec_PDG,
    selelec_ParentPDG,
    selelec_GParentPDG,
    selelec_ID,
    selelec_truemom,
    selelec_truecostheta,
    selelec_stopped,
    selelec_flipped,
    selelec_truedir,
    selelec_truepos,
    selelec_trueendpos,
    selelec_truefgdlayer,
    selelec_daughterPDG,

    selelec_detectors,
    selelec_charge,
    selelec_dir,
    selelec_enddir,
    selelec_pos,
    selelec_endpos,

    selelec_ecal_mipem,
    selelec_ecal_EneOnL,
    selelec_ecal_stopping,
    selelec_longestTPC,

    TPCVeto_Deltaz,
    ECalVeto_Deltaz,
    NNearTracks,
    NFarTracks,

    TruePairTrack_true_mom,
    TruePairTrack_pdg,
    PairTrack_mom,
    PairTrack_costheta,
    PairTrack_pdg,
    PairTrack_InvMass,
    PairTrack_charge,
    PairTrack_DistToMainTrack,
    PairTrack_startdir,
    PairTrack_startpos,
    PairTrack_true_startdir,
    PairTrack_true_startpos,
    PairTrack_likemuon,
    PairTrack_likeelec,
    PairTrack_likepion,
    PairTrack_likeprot,
    PairTrack_gamma_dir,
    PairTrack_gamma_true_dir,

    selelec_ToF,
    selelec_ToF_true,
    Beta_ToF,
    Beta_true_ToF,
    selelec_ToF_mass,
    selelec_ToF_true_mass,
    selelec_ToF_lkl_muon,
    selelec_ToF_lkl_pion,
    selelec_ToF_lkl_electron,
    selelec_ToF_lkl_proton,
    selelec_ToF_pull_muon,
    selelec_ToF_pull_pion,
    selelec_ToF_pull_electron,
    selelec_ToF_pull_proton,
    selelec_dedx_tpc,
    selelec_first_ToF,
    selelec_second_ToF,

    true_pdg,
    true_det,
    true_SDlength,
    true_target_start_mom,
    true_target_end_mom,
    true_target_length,
    true_target_dE,
    true_mom,
    true_costheta,
    true_phi,
    true_charge,
    true_position,
    true_end,
    true_ekin,
    true_Q2,
    true_TPC,
    true_part,
    true_parentID,
    true_Nu_pdg,
    true_Target_pdg,
    true_Nu_mom,
    true_reaction_code,
    true_vertex_position,
    true_vertex_intarget,
    enumStandardMicroTreesLast_numuCC4piAnalysis
  };


};

#endif
