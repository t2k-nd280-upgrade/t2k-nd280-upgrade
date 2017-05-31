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
  void AddCategories();
  void FillCategories();

private:
  bool _useTarget1;
  bool _useTarget2;
  bool _useFGD1;
  bool _useFGD2;

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
    selmu_mom,
    //selmu_MomRangeMuon,
    selmu_costheta,
    //selmu_ToF_PoD,
    //selmu_ToF_ECAL,
    //selmu_ToF_FGD2,
    selmu_likemu,
    selmu_likeel,
    selmu_likepi,
    selmu_likepr,
    selmu_likemip,
    //selmu_end_ecal,

    selmu_PDG,
    selmu_ParentPDG,
    selmu_GParentPDG,
    selmu_ID,
    selmu_truemom,
    selmu_truecostheta,
    selmu_truedir,
    selmu_truepos,
    selmu_trueendpos,
    selmu_truefgdlayer,

    selmu_detectors,
    selmu_charge,
    selmu_dir,
    selmu_enddir,
    selmu_pos,
    selmu_endpos,

    selmu_ecal_mipem,
    selmu_ecal_EneOnL,
    selmu_ecal_stopping,
    selmu_longestTPC,

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

    selmu_ToF_mass,
    selmu_ToF_true_mass,
    selmu_ToF_lkl_muon,
    selmu_ToF_lkl_pion,
    selmu_ToF_lkl_electron,
    selmu_ToF_lkl_proton,
    selmu_ToF_pull_muon,
    selmu_ToF_pull_pion,
    selmu_ToF_pull_electron,
    selmu_ToF_pull_proton,
    selmu_dedx_tpc,

    true_pdg,
    true_det,
    true_SDlength,
    true_Edep,
    true_mom,
    true_costheta,
    true_phi,
    true_charge,
    true_position,
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
