#ifndef numuCC4piAnalysis_h
#define numuCC4piAnalysis_h

#include "numuCC4piSelection.hxx"

#include "baseAnalysis.hxx"

class numuCC4piAnalysis: public baseAnalysis {
public:
  numuCC4piAnalysis(AnalysisAlgorithm* ana = NULL);
  virtual ~numuCC4piAnalysis() {}

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
  int _nuMode;

public:

  enum enumStandardMicroTrees_numuCC4piAnalysis {
    truelepton_mom = enumStandardMicroTreesLast_baseAnalysis + 1,
    truelepton_PDG,
    truelepton_dir,
    truelepton_costheta,
    truelepton_pos,
    truelepton_fgdlayer,

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
    selmu_likee,
    selmu_likemip,
    //selmu_end_ecal,

    selmu_PDG,
    selmu_ID,
    selmu_truemom,
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
    selmu_categ,
    selmu_target_length,

    selmu_ecal_mipem,
    selmu_ecal_EneOnL,
    selmu_ecal_stopping,
    selmu_longestTPC,

    sel_nTruePiPlus,
    sel_TruePiPlus_reco,
    sel_TruePiPlus_mom,
    sel_TruePiPlus_cos,
    sel_TruePiPlus_merged,
    sel_nTruePiMinus,
    sel_TruePiMinus_reco,
    sel_TruePiMinus_mom,
    sel_TruePiMinus_cos,
    sel_TruePiMinus_merged,
    sel_nTruePiZero,
    sel_TruePiZero_reco,
    sel_TruePiZero_mom,
    sel_TruePiZero_cos,
    sel_TruePiZero_merged,

    sel_nTPCProtons,
    sel_nTargetProtons,

    selmu_ToF_time,
    selmu_ToF_true_time,
    selmu_ToF_length,
    selmu_ToF_true_length,
    selmu_ToF_mass,
    selmu_ToF_true_mass,
    selmu_ToF_mulkl,
    selmu_ToF_elkl,
    selmu_ToF_firstDet,
    selmu_ToF_secondDet,
    selmu_dedx_tpc,

    true_pdg,
    true_SDlength,
    true_Edep,
    true_mom,
    true_costheta,
    true_phi,
    true_charge,
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
	true_contained,

    trueHMN_pdg,

    enumStandardMicroTreesLast_numuCC4piAnalysis
  };


};

#endif
