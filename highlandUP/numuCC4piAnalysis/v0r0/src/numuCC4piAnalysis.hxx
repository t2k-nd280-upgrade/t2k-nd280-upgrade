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

    selmu_ecal_mipem,
    selmu_ecal_EneOnL,
    selmu_ecal_stopping,
    selmu_longestTPC,

    sel_nTrueProtons,
    sel_nTrueProtonsAll,
    sel_nTrueProtonsAll500,
    sel_nTrueProtons500,
    sel_TrueProton_reco,
    sel_TrueProton_mom,
    sel_TrueProton_cos,
    sel_TrueProton_posX,
    sel_TrueProton_posY,
    sel_TrueProton_posZ,
    sel_TrueProton_length,

    sel_nTruePiPlus,
    sel_TruePiPlus_reco,
    sel_TruePiPlus_mom,
    sel_TruePiPlus_cos,
    sel_nTruePiMinus,
    sel_TruePiMinus_reco,
    sel_TruePiMinus_mom,
    sel_TruePiMinus_cos,
    sel_nTruePiZero,
    sel_TruePiZero_reco,
    sel_TruePiZero_mom,
    sel_TruePiZero_cos,

    sel_nNegativePionTPCtracks,
    sel_nPositivePionTPCtracks,
    sel_nIsoTargetPiontracks,
    sel_nPosPi0TPCtracks,
    sel_nElPi0TPCtracks,
    sel_nMichelElectrons,
    sel_nOtherPions,
    sel_nPosPions,
    
    sel_NegativePionTPCtracks_PDG,
    sel_PositivePionTPCtracks_PDG,
    sel_IsoTargetPiontracks_PDG,
    sel_PosPi0TPCtracks_PDG,
    sel_ElPi0TPCtracks_PDG,
    sel_MichelElectrons_PDG,
    sel_PosPions_PDG,
    sel_OtherPions_PDG,

    sel_NegativePionTPCtracks_ParPDG,
    sel_PositivePionTPCtracks_ParPDG,
    sel_IsoTargetPiontracks_ParPDG,
    sel_PosPi0TPCtracks_ParPDG,
    sel_ElPi0TPCtracks_ParPDG,
    sel_MichelElectrons_ParPDG,
    sel_PosPions_ParPDG,
    sel_OtherPions_ParPDG,

    sel_NegativePionTPCtracks_mom,
    sel_PositivePionTPCtracks_mom,
    sel_IsoTargetPiontracks_mom,
    sel_PosPi0TPCtracks_mom,
    sel_ElPi0TPCtracks_mom,
    sel_MichelElectrons_mom,
    sel_PosPions_mom,
    sel_OtherPions_mom,

    sel_NegativePionTPCtracks_pos,
    sel_PositivePionTPCtracks_pos,
    sel_IsoTargetPiontracks_pos,
    sel_PosPi0TPCtracks_pos,
    sel_ElPi0TPCtracks_pos,
    sel_MichelElectrons_pos,
    sel_PosPions_pos,
    sel_OtherPions_pos,

    selmu_ToF_mass,
    selmu_ToF_true_mass,
    selmu_dedx_tpc,

    selAll_ToF_mass,
    selAll_ToF_true_mass,
    selAll_mom,
    selAll_cos,
    selAll_L,
    selAll_true_mom,
    selAll_PDG,
    selAll_ToF_det_used1,
    selAll_ToF_det_used2,
    selAll_ToF_time_reco,
    selAll_ToF_pull_muon,
    selAll_ToF_pull_pion,
    selAll_ToF_pull_electron,
    selAll_ToF_pull_proton,
    selAll_ToF_lkl_muon,
    selAll_ToF_lkl_pion,
    selAll_ToF_lkl_electron,
    selAll_ToF_lkl_proton,
    selAll_TPC_dedx,
    selAll_TPC_pull_muon,
    selAll_TPC_pull_pion,
    selAll_TPC_pull_electron,
    selAll_TPC_pull_proton,
    selAll_TPC_lkl_muon,
    selAll_TPC_lkl_pion,
    selAll_TPC_lkl_electron,
    selAll_TPC_lkl_proton,
    selAll_nTracks,

    /*
      selmu_1hit_pos,
      selmu_2hit_pos,
      selmu_1hit_charge,
      selmu_2hit_charge,
      selmu_1hit_fitpos,
      selmu_2hit_fitpos,
      selmu_1hit_deltapos,
      selmu_2hit_deltapos,
      selmu_1hit_deltapos2,
      selmu_2hit_deltapos2,
      selmu_pod_nnodes,

      selmu_ntpcs,
      selmu_tpc_det,
      selmu_tpc_nnodes,
      selmu_tpc_charge,
      selmu_ntargets,
      selmu_target_det,
      selmu_target_nnodes,
      selmu_target_x,
      selmu_necals,
      selmu_ecal_det,
      selmu_ecal_nnodes,
      selmu_ecal_length,
      selmu_ecal_EMenergy,
   	
      NtargetSec,
      FGDSec_ToF_PoD,
      FGDSec_ToF_ECAL,
      FGDSec_ToF_FGD2,

      NTPCSec,
      TPCSec_MomGlobal,
      TPCSec_pos,
      TPCSec_endpos,
      TPCSec_ToF_PoD,
      TPCSec_ToF_ECAL,
      TPCSec_ToF_FGD2,
    */

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

    trueHMN_pdg,

    enumStandardMicroTreesLast_numuCC4piAnalysis
  };


};

#endif
