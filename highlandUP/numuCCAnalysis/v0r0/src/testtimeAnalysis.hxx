#ifndef testtimeAnalysis_h
#define testtimeAnalysis_h

#include "baseAnalysis.hxx"
//#include "ND280AnalysisUtils.hxx"
#include "ToyBoxNDUP.hxx"

class testtimeAnalysis: public baseAnalysis {
 public:
  testtimeAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~testtimeAnalysis(){}

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
    virtual const ToyBoxNDUP& box(Int_t isel=-1) const {return *dynamic_cast<const ToyBoxNDUP*>(&boxB(isel));}

  // Return the vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaVertexB* GetVertex() const { return box().Vertex;}//box().Vertex;};

  // Return the true vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaTrueVertexB* GetTrueVertex() const { return box().TrueVertex;} //box().TrueVertex;};


private:

  SubDetId::SubDetEnum _detector;

public:

  enum enumStandardMicroTrees_testtimeAnalysis{
    selmu_truemom = enumStandardMicroTreesLast_baseAnalysis + 1,
    selmu_truebeta,
 
    //truth tree variable
    true_parentID,

    true_Nu_pdg,
    true_Nu_mom,
    true_reaction_code,
    true_vertex_position,
    true_pdg,
    true_mom,
    true_costheta,
    true_part,
    true_in,
    true_length_BU, 
    true_length_FU, 
    true_length_BcD, 
    true_length_LU, 
    true_length_UE, 
    true_mass_BU, 
    true_mass_FU, 
    true_mass_BcD, 
    true_mass_LU, 
    true_mass_UE, 
    true_time_diff_BU, 
    true_time_diff_FU, 
    true_time_diff_BcD, 
    true_time_diff_LU, 
    true_time_diff_UE, 




   // true_tpc_sigma,
    Nu_pdg,
    Nu_mom,
    reaction_code,
    current_code,
    enumStandardMicroTreesLast_testtimeAnalysis
  };

  enum enumConf_testtimeAnalysis{
    detmass_syst=baseAnalysis::enumConfLast_baseAnalysis+1,    
    dedx_syst,
    enumConfLast_testtimeAnalysis
  };



};

#endif
