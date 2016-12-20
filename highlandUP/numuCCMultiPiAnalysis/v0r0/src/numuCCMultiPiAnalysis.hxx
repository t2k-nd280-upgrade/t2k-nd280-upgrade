#ifndef numuCCMultiPiAnalysis_h
#define numuCCMultiPiAnalysis_h

#include "ndupAnalysis.hxx"
#include "numuCCMultiPiSelection.hxx"
#include "baseAnalysis.hxx"
//#include "PionInteractionSystematic.hxx"

class numuCCMultiPiAnalysis: public baseAnalysis {
 public:

  numuCCMultiPiAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~numuCCMultiPiAnalysis(){}

  //---- These are mandatory functions

  void DefineSelections();
  void DefineCorrections(){_ndupAnalysis->DefineCorrections();}
  void DefineMicroTrees(bool addBase=false);
  void DefineTruthTree();

  void FillCategories(){_ndupAnalysis->FillCategories();}

  void FillMicroTrees(bool addBase=false);
  void FillToyVarsInMicroTrees(bool addBase=false);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  using baseAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);

  //--------------------

  bool Initialize();

//  const ToyBoxCCMultiPi& mybox(){return *static_cast<const ToyBoxCCMultiPi*>(&box());}
  virtual const ToyBoxCCMultiPi& mybox(Int_t isel=-1) const {return *static_cast<const ToyBoxCCMultiPi*>(&boxB(isel));}

  Int_t FindTPCTracks(const ToyBoxB& box, SubDetId::SubDetEnum det, AnaTrackB** TPCtracks);
  // Return the vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaVertexB* GetVertex() const { return new AnaVertexB();}//box().Vertex;};

  // Return the true vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaTrueVertexB* GetTrueVertex() const { return new AnaTrueVertexB();} //box().TrueVertex;};

private:
  Int_t _whichTarget; // 1 for Target1 analysis, 2 for Target2, 3 for both

  SubDetId::SubDetEnum _TargetID;

//  bool _addPionSIinfo;
  
  /// Pion SI manager
 // PionSIManager _pionSIManager;

protected:

  ndupAnalysis* _ndupAnalysis;

public:

  enum enumStandardMicroTrees_numuMultiPiAnalysis{
    HM_pos = ndupAnalysis::enumStandardMicroTreesLast_ndupAnalysis + 1,
    MuonVertexId,
    MuonIndex,
    NNegPion,
    NegPionMom,
    NegPionPidLik,
    NegPionElPidLik,
    NegPionTId,
    NegPionIndex,
    NegPionParentTId,
    NegPionGParentTId,
    NegPionVId,
    NegPionDir,
    NegPionPosStart,
    NegPionPosEnd,
    NPosPion,
    PosPionMom,
    PosPionTheta,
    PosPionPidLik,
    PosPionElPidLik,
    PosPionTId,
    PosPionIndex,
    PosPionParentTId,
    PosPionGParentTId,
    PosPionVId,
    PosPionDir,
    PosPionPosStart,
    PosPionPosEnd,
    NPi0El,
    Pi0ElMom,
    Pi0ElPull,
    Pi0ElTId,
    Pi0ElIndex,
    Pi0ElParentTId,
    Pi0ElGParentTId,
    Pi0ElVId,
    Pi0ElDir,
    Pi0ElPosStart,
    Pi0ElPosEnd,
    NPi0Pos,
    Pi0PosMom,
    Pi0PosPull,
    Pi0PosTId,
    Pi0PosIndex,
    Pi0PosParentTId,
    Pi0PosGParentTId,
    Pi0PosVId,
    Pi0PosDir,
    Pi0PosPosStart,
    Pi0PosPosEnd,
    NTPCSec,
    TPCSecMom,
    TPCSecPiPidLik,
    TPCSecMuPidLik,
    TPCSecPrPidLik,
    TPCSecElPidLik,
    TPCSecDetectors,
    TPCSecQ,
    TPCSecTId,
    TPCSecIndex,
    TPCSecParentTId,
    TPCSecGParentTId,
    TPCSecDir,
    TPCSecPosStart,
    TPCSecPosEnd,
    NTargetPi,
    TargetPiLength,
    TargetPiPiPull,
    TargetPiAvgTime,
    TargetPiDistance,
    TargetPiMuonangle,
    TargetPiCosTheta,
    TargetPiTId,
    TargetPiIndex,
    TargetPiParentTId,
    TargetPiGParentTId,
    TargetPiVId,
    TargetPiDir,
    TargetPiPosStart,
    TargetPiPosEnd,
    TargetPiTimeIni,
    TargetPiTimeEnd,
    TargetPiDeltaTimeIniSelMu,
    TargetPiDeltaTimeEndSelMu,
    NTargetEl,
    TargetElLength,
    TargetElPiPull,
    TargetElAvgTime,
    TargetElDistance,
    TargetElMuonangle,
    TargetElCosTheta,
    TargetElTId,
    TargetElIndex,
    TargetElParentTId,
    TargetElGParentTId,
    TargetElVId,
    TargetElDir,
    TargetElPosStart,
    TargetElPosEnd,
    TargetElTimeIni,
    TargetElTimeEnd,
    TargetElDeltaTimeIniSelMu,
    TargetElDeltaTimeEndSelMu,
    NTargetSec,
    TargetSecLength,
    TargetSecPiPull,
    TargetSecContained,
    TargetSecAvgTime,
    TargetSecCosTheta,
    TargetSecTId,
    TargetSecIndex,
    TargetSecParentTId,
    TargetSecGParentTId,
    TargetSecDir,
    TargetSecPosStart,
    TargetSecPosEnd,
    TargetSecTimeIni,
    TargetSecTimeEnd,
    TargetSecDeltaTimeIniSelMu,
    TargetSecDeltaTimeEndSelMu,
    NME,
    MENHits,
    MERawCharge,
    MEMinTime,
    MEMaxTime,
    NTPC1Track,
    TPC1TrackMom,
    TPC1TrackCosTheta,
    TPC1TrackPhi,
    TPC1TrackVId,
    TPC1TrackTId,
    TPC1TrackParentTId,
    TPC1TrackGParentTId,
    TPC1TrackPosStart,
    TPC1TrackPosEnd,
    IntType,
    NInts,
    PionType,
    PionMom,
    NPions,

    truepi_mom,
    truepi_dir,
    truepi_costheta,
    
    enumStandardMicroTreesLast_numuCCMultiPiAnalysis
  };
};

#endif
