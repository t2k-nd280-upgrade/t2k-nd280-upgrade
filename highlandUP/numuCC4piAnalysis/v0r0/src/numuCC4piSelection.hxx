#ifndef numuCC4piSelection_h
#define numuCC4piSelection_h

#include "SelectionBase.hxx"
#include "SubDetId.hxx"
#include "ToyBoxNDUP.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"
#include "Parameters.hxx"

#include "BinnedParams.hxx"

#include "TRandom3.h"
#include "TFile.h"

//#include "ToFSenseCorrector.hxx"

class numuCC4piSelection: public SelectionBase{
public:
  numuCC4piSelection(bool forceBreak=true);
  virtual ~numuCC4piSelection(){
    _file_ECAL_PDF->Close();
    delete _file_ECAL_PDF;
    delete _randomGen;
  }

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  inline ToyBoxB* MakeToyBox();
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId::SampleEnum GetSampleEnum(){return SampleId::kTarget1NuMuCC;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);

private:
  BinnedParams *_ECal_reco_eff, *_ECal_FGDmatch_eff;
  TFile *_file_ECAL_PDF;
  TRandom3 *_randomGen;
  
};

class ToyBoxCC4pi: public ToyBoxNDUP{

public:

  ToyBoxCC4pi() {
    track_ECal_MipEM = -0xABCDEF;
    track_ECal_EneOnL = -0xABCDEF;
    MainTrack = NULL;
    TPCTracks.clear();       ECalTracks.clear();
    NegativePionTPCtracks.clear();
    PositivePionTPCtracks.clear();
    IsoTargetPiontracks.clear();
    PosPi0TPCtracks.clear();
    ElPi0TPCtracks.clear();
    MichelElectrons.clear();
    PosPions.clear();
    OtherPions.clear();
    TPC_det = SubDetId::kInvalid;
  }

  virtual void Reset(){
    ResetBase();
    track_ECal_MipEM = -0xABCDEF;
    track_ECal_EneOnL = -0xABCDEF;
    MainTrack = NULL;
    TPCTracks.clear();       ECalTracks.clear();
    NegativePionTPCtracks.clear();
    PositivePionTPCtracks.clear();
    IsoTargetPiontracks.clear();
    PosPi0TPCtracks.clear();
    ElPi0TPCtracks.clear();
    MichelElectrons.clear();
    PosPions.clear();
    OtherPions.clear();
    TPC_det = SubDetId::kInvalid;
  }

  virtual ~ToyBoxCC4pi(){}

  /// For storing tracks information in the bunch
  float track_ECal_MipEM, track_ECal_EneOnL;
  AnaTrackB* MainTrack;
  std::vector<AnaTrackB*> TPCTracks, ECalTracks;
  SubDetId::SubDetEnum TPC_det;

  int nNegativePionTPCtracks;
  std::vector<AnaTrackB*> NegativePionTPCtracks;
  int nPositivePionTPCtracks;
  std::vector<AnaTrackB*> PositivePionTPCtracks;
  int nIsoTargetPiontracks;
  std::vector<AnaTrackB*> IsoTargetPiontracks;
  int nPosPi0TPCtracks;
  std::vector<AnaTrackB*> PosPi0TPCtracks;
  int nElPi0TPCtracks;
  std::vector<AnaTrackB*> ElPi0TPCtracks;
  int nMichelElectrons;
  std::vector<AnaTrackB*> MichelElectrons;
  int nPosPions;
  std::vector<AnaTrackB*> PosPions;
  int nOtherPions;
  std::vector<AnaTrackB*> OtherPions;
  
};

inline ToyBoxB* numuCC4piSelection::MakeToyBox() {return new ToyBoxCC4pi();}

namespace numuCC4piUtils{

  class TotalMultiplicityCut: public StepBase{
  public:
    TotalMultiplicityCut() {
      useTarget1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget1");
      useTarget2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget2");
      useFGD1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD1");
      useFGD2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD2");
    };
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new TotalMultiplicityCut();}
  private:
    bool useTarget1, useTarget2;
    bool useFGD1, useFGD2;
  };

  class SortTracksAction: public StepBase{
  public:
    SortTracksAction() {
      useTarget1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget1");
      useTarget2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget2");
      useFGD1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD1");
      useFGD2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD2");
    };
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new SortTracksAction();}
  private:
    bool useTarget1, useTarget2;
    bool useFGD1, useFGD2;
  };

  class TrackGQandFVCut: public StepBase{
  public:
    TrackGQandFVCut() {
      useTarget1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget1");
      useTarget2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget2");
      useFGD1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD1");
      useFGD2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD2");
    };
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new TrackGQandFVCut();}

  private:
    bool useTarget1, useTarget2;
    bool useFGD1, useFGD2;
  };

  class VetoAction: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new VetoAction();}
  };
  /// Find the Vertex. For the moment it's just the Star position of the longest track
  class TrueVertexInTargetCut: public StepBase{
  public:
    TrueVertexInTargetCut() {
      useTarget1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget1");
      useTarget2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget2");
      useFGD1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD1");
      useFGD2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD2");
    };
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new TrueVertexInTargetCut();}

  private:
    bool useTarget1, useTarget2;
    bool useFGD1, useFGD2;
  };

  /// Find the True Vertex. For the moment it's just start position of the true particle associated to the longest track
  class FindTrueVertexAction: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new FindTrueVertexAction();}
  };

  class FindVertexAction: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new FindVertexAction();}
  };

  class FillSummaryAction_numuCC4pi: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;  
    StepBase* MakeClone(){return new FillSummaryAction_numuCC4pi();}
  };

  class FindPionsAction: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new FindPionsAction();}
  };

  class FwdTPC_Quality: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new FwdTPC_Quality();}
  };

  class FwdTPC_PID: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new FwdTPC_PID();}
  };


  class BwdTPC_Quality: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new BwdTPC_Quality();}
  };

  class BwdTPC_PID: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new BwdTPC_PID();}
  };


  class HATPC_Quality: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new HATPC_Quality();}
  };

  class HATPC_PID: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new HATPC_PID();}
  };


  class ECal_Quality: public StepBase{
  public:
    ECal_Quality(TRandom3 *r, BinnedParams* b1, BinnedParams* b2){
      _randomGen = r;
      _ECal_reco_eff = b1;
      _ECal_FGDmatch_eff = b2;
    }
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new ECal_Quality(_randomGen, _ECal_reco_eff, _ECal_FGDmatch_eff);}
  private:
    TRandom3* _randomGen;
    BinnedParams *_ECal_reco_eff, *_ECal_FGDmatch_eff;
  };

  class ECal_PID: public StepBase{
  public:
    ECal_PID(TFile *f){_file_ECAL_PDF=f;}
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new ECal_PID(_file_ECAL_PDF);}
  private:
    TFile* _file_ECAL_PDF; 
  };

  class CC0pi: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new CC0pi();}
  };

  class CC1pi: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new CC1pi();}
  };

  class CCoth: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new CCoth();}
  };

  class ToF_senseDetermination: public StepBase{
  public:
    ToF_senseDetermination(){
      _randomGen = new TRandom3(0);
    }
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new ToF_senseDetermination();}
  private:
    TRandom3* _randomGen;
  };

}
  
/*
  class CSFGD2_PID: public StepBase{
  public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new CSFGD2_PID();}
  };
  class CSFGD2_4pi: public StepBase{
  public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new CSFGD2_4pi();}
  };

  class CSECAL_PID: public StepBase{
  public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new CSECAL_PID();}
  };
  class CSECAL_4pi: public StepBase{
  public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new CSECAL_4pi();}
  };
*/

#endif
