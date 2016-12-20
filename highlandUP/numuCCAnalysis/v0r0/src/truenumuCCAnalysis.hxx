#ifndef truenumuCCAnalysis_h
#define truenumuCCAnalysis_h

#include "baseAnalysis.hxx"
//#include "ND280AnalysisUtils.hxx"
#include "trueToyBoxNDUP.hxx"

class truenumuCCAnalysis: public baseAnalysis {
 public:
  truenumuCCAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~truenumuCCAnalysis(){}

  //---- These are mandatory functions
  void DefineSelections();
  void DefineCorrections();
  void DefineSystematics();
  void DefineConfigurations();
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();

  void FillMicroTrees(bool addBase=true);
  void FillToyVarsInMicroTrees(bool addBase=true);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  using baseAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);
  using baseAnalysis::FillTruthTreeBase;
  void FillTruthTreeBase(const AnaTrueVertex& vtx, bool IsAntinu = false);
  //--------------------

  bool Initialize();
  void FillCategories();

  /// Returns the ToyBoxDUNE
    virtual const trueToyBoxNDUP& box(Int_t isel=-1) const {return *dynamic_cast<const trueToyBoxNDUP*>(&boxB(isel));}

  // Return the vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaVertexB* GetVertex() const { return new AnaVertexB();}//box().Vertex;};

  // Return the true vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaTrueVertexB* GetTrueVertex() const { return new AnaTrueVertexB();} //box().TrueVertex;};


private:

  SubDetId::SubDetEnum _detector;

public:

  enum enumStandardMicroTrees_truenumuCCAnalysis{
    selmu_truemom = enumStandardMicroTreesLast_baseAnalysis + 1,
    selmu_truebeta,
 
    //truth tree variable
    true_parentID,
    true_tpcup1_det,
    true_tpcup1_charge,
    true_tpcup1_Edep,
    true_tpcup1_DeltaLYZ,
    true_tpcup2_det,
    true_tpcup2_charge,
    true_tpcup2_Edep,
    true_tpcup2_DeltaLYZ,
    true_tpcdown1_det,
    true_tpcdown1_charge,
    true_tpcdown1_Edep,
    true_tpcdown1_DeltaLYZ,
    true_tpcdown2_det,
    true_tpcdown2_charge,
    true_tpcdown2_Edep,
    true_tpcdown2_DeltaLYZ,
    true_forwtpc1_det,
    true_forwtpc1_charge,
    true_forwtpc1_Edep,
    true_forwtpc1_DeltaLYZ,
    true_forwtpc2_det,
    true_forwtpc2_charge,
    true_forwtpc2_Edep,
    true_forwtpc2_DeltaLYZ,
    true_forwtpc3_det,
    true_forwtpc3_charge,
    true_forwtpc3_Edep,
    true_forwtpc3_DeltaLYZ,
    true_target1_det,
    true_target2_det,
    true_target_charge,
    true_target1_length,
    true_target1_Edep,
    true_target1_DeltaLYZ,
    true_target2_length,
    true_target2_Edep,
    true_target2_DeltaLYZ,
    true_tpc_Edep,
    true_tpc_de_dx,
    true_tpc_momentum,
    true_tpc_length,
    true_tpc_exitmomentum,
    true_ntpcs,
    true_ntarget1s,
    true_ntarget2s,
    true_Nu_pdg,
    true_Nu_mom,
    true_reaction_code,
    true_vertex_position,
    true_tpc_pull_muon,
    true_tpc_pull_proton,
    true_tpc_pull_pion,
    true_tpc_pull_electron,
    true_tpc_sigdedx_muon,
    true_tpc_sigdedx_proton,
    true_tpc_sigdedx_pion,
    true_tpc_sigdedx_ele,
    true_tpc_dedx_muon,
    true_tpc_dedx_proton,
    true_tpc_dedx_pion,
    true_tpc_dedx_ele,
    true_pdg,
    true_phi,
    true_SDlength,
    true_Edep,
    true_mom,
    true_costheta,
    true_charge,
    true_ekin,
    true_part,



   // true_tpc_sigma,
    Nu_pdg,
    Nu_mom,
    reaction_code,
    current_code,
    isLastinTarget1,
    isLastinTarget2,
    vertex_position,
    muon_pdg,
    muon_mom,
    muon_SDLength,
    muon_charge,
    muon_costheta,
    muon_theta,
    muon_phi,

    muon_target1_entrancepos,
    muon_target1_exitpos,
    muon_target2_entrancepos,
    muon_target2_exitpos,
    muon_tpc_entrancepos,
    muon_tpc_exitpos,
    muon_tpc_pull_muon,
    muon_tpc_pull_proton,
    muon_tpc_pull_pion,
    muon_tpc_pull_electron,
    muon_tpc_sigdedx_muon,
    muon_tpc_sigdedx_proton,
    muon_tpc_sigdedx_pion,
    muon_tpc_sigdedx_ele,
    muon_tpc_dedx_muon,
    muon_tpc_dedx_proton,
    muon_tpc_dedx_pion,
    muon_tpc_dedx_ele,
    muon_tpc_length,
    muon_tpc_EDep,
    muon_EDep_Target1,
    muon_EDep_Target2,
    muon_LYZTPC,
    muon_LYZTPCUp1,
    muon_LYZTPCForw2,
    muon_pidlikelihood1,
    muon_pidlikelihood2,
    muon_pidlikelihood3,
    muon_pidlikelihood4,
    mip_pidlikelihood,

    muon_tpc_de_dx,
    muon_tpc_momentum,
    muon_cosphi,
    muon_tpc_det,
    muon_LTarget1,
    muon_LTarget2,
    tpc_muon,
    muon_isLastinTarget1,
    muon_isLastinTarget2,
    muon_ekin,
    muon_tpc_smeared_momentum,
    muon_tpc_momentum_error,
    muon_istarget1,
    muon_istarget2,
    muon_istpccut,


    ppion_pdg,
    ppion_mom,
    ppion_SDLength,
    ppion_charge,
    ppion_costheta,
    ppion_theta,
    ppion_phi,
    ppion_target1_entrancepos,
    ppion_target1_exitpos,
    ppion_target2_entrancepos,
    ppion_target2_exitpos,
    ppion_tpc_entrancepos,
    ppion_tpc_exitpos,
    ppion_tpc_pull_muon,
    ppion_tpc_pull_proton,
    ppion_tpc_pull_pion,
    ppion_tpc_pull_electron,
    ppion_tpc_sigdedx_muon,
    ppion_tpc_sigdedx_proton,
    ppion_tpc_sigdedx_pion,
    ppion_tpc_sigdedx_ele,
    ppion_tpc_dedx_muon,
    ppion_tpc_dedx_proton,
    ppion_tpc_dedx_pion,
    ppion_tpc_dedx_ele,
    ppion_tpc_length,
    ppion_tpc_EDep,
    ppion_EDep_Target1,
    ppion_EDep_Target2,
    ppion_LYZTPC,
    ppion_tpc_de_dx,
    ppion_tpc_momentum,
    ppion_cosphi,
    ppion_tpc_det,
    ppion_tpc_c,
    ppion_LTarget1,
    ppion_LTarget2,
    tpc_ppion,
    ppion_isLastinTarget1,
    ppion_isLastinTarget2,
    ppion_ekin,
    ppion_tpc_smeared_momentum,
    ppion_tpc_momentum_error,
    ppion_istarget1,
    ppion_istarget2,
    ppion_istpccut,


    mpion_pdg,
    mpion_mom,
    mpion_SDLength,
    mpion_charge,
    mpion_costheta,
    mpion_theta,
    mpion_phi,
    mpion_target1_entrancepos,
    mpion_target1_exitpos,
    mpion_target2_entrancepos,
    mpion_target2_exitpos,
    mpion_tpc_entrancepos,
    mpion_tpc_exitpos,
    mpion_tpc_pull_muon,
    mpion_tpc_pull_proton,
    mpion_tpc_pull_pion,
    mpion_tpc_pull_electron,
    mpion_tpc_sigdedx_muon,
    mpion_tpc_sigdedx_proton,
    mpion_tpc_sigdedx_pion,
    mpion_tpc_sigdedx_ele,
    mpion_tpc_dedx_muon,
    mpion_tpc_dedx_proton,
    mpion_tpc_dedx_pion,
    mpion_tpc_dedx_ele,
    mpion_tpc_length,
    mpion_tpc_EDep,
    mpion_EDep_Target1,
    mpion_EDep_Target2,
    mpion_LYZTPC,
    mpion_tpc_de_dx,
    mpion_tpc_momentum,
    mpion_cosphi,
    mpion_tpc_det,
    mpion_LTarget1,
    mpion_LTarget2,
    tpc_mpion,
    mpion_isLastinTarget1,
    mpion_isLastinTarget2,
    mpion_ekin,
    mpion_tpc_smeared_momentum,
    mpion_tpc_momentum_error,
    mpion_istarget1,
    mpion_istarget2,
    mpion_istpccut,
 

    proton_pdg,
    proton_mom,
    proton_SDLength,
    proton_charge,
    proton_costheta,
    proton_theta,
    proton_phi,
    proton_target1_entrancepos,
    proton_target1_exitpos,
    proton_target2_entrancepos,
    proton_target2_exitpos,
    proton_tpc_entrancepos,
    proton_tpc_exitpos,
    proton_tpc_pull_muon,
    proton_tpc_pull_proton,
    proton_tpc_pull_pion,
    proton_tpc_pull_electron,
    proton_tpc_sigdedx_muon,
    proton_tpc_sigdedx_proton,
    proton_tpc_sigdedx_pion,
    proton_tpc_sigdedx_ele,
    proton_tpc_dedx_muon,
    proton_tpc_dedx_proton,
    proton_tpc_dedx_pion,
    proton_tpc_dedx_ele,
    proton_tpc_length,
    proton_tpc_EDep,
    proton_EDep_Target1,
    proton_EDep_Target2,
    proton_LYZTPC,
    proton_tpc_de_dx,
    proton_tpc_momentum,
    proton_cosphi,
    proton_tpc_det,
    proton_LTarget1,
    proton_LTarget2,
    tpc_proton,
    proton_isLastinTarget1,
    proton_isLastinTarget2,
    proton_ekin,
    proton_tpc_smeared_momentum,
    proton_tpc_momentum_error,
    proton_istarget1,
    proton_istarget2,
    proton_istpccut,
            tpc_pull_muon,
        tpc_pull_proton,
        tpc_pull_pion,
        tpc_pull_ele,


    charge,
    det_enum,
    ntpc_seg,

  
    tpc_smeared_momentum,
    LYZmax,
    LTargetmax,

    enumStandardMicroTreesLast_truenumuCCAnalysis
  };

  enum enumConf_truenumuCCAnalysis{
    detmass_syst=baseAnalysis::enumConfLast_baseAnalysis+1,    
    dedx_syst,
    enumConfLast_truenumuCCAnalysis
  };



};

#endif
