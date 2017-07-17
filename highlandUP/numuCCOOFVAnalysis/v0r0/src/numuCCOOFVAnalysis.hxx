#ifndef numuCCOOFVAnalysis_h
#define numuCCOOFVAnalysis_h

#include "numuCCOOFVSelection.hxx"

#include "baseAnalysis.hxx"

class numuCCOOFVAnalysis: public baseAnalysis {
public:
  numuCCOOFVAnalysis(AnalysisAlgorithm* ana = NULL);
  virtual ~numuCCOOFVAnalysis() {}

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

  virtual const ToyBoxCCOOFV& ccoofvbox(Int_t isel=-1) const {return *dynamic_cast<const ToyBoxCCOOFV*>(&boxB(isel));}

  // Return the vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaVertexB * GetVertex() const { return ccoofvbox().Vertex;} //box().Vertex;};

  // Return the true vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaTrueVertexB * GetTrueVertex() const { return ccoofvbox().TrueVertex;} //box().TrueVertex;};

  using baseAnalysis::FillTruthTree;
  using baseAnalysis::FillTruthTreeBase;
  void FillTruthTree(const AnaTrueVertex& vtx);

  bool Initialize();
  void AddCategories();
  void FillCategories();

public:

  enum enumStandardMicroTrees_numuCCOOFVAnalysis {
    truelepton_mom = enumStandardMicroTreesLast_baseAnalysis + 1,
    truelepton_PDG,
    truelepton_dir,
    truelepton_costheta,
    truelepton_pos,
    truelepton_fgdlayer,

    // toy variables
    NTracks,
    Toy_CCOOFV,
    selmu_mom,
    selmu_costheta,
    selmu_phi,
    selmu_likemu,
    selmu_likemip,

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
    selmu_OOFV,
	
	selmu_true_ToF,
	selmu_reco_ToF,

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

    enumStandardMicroTreesLast_numuCCOOFVAnalysis
  };


};

#endif
