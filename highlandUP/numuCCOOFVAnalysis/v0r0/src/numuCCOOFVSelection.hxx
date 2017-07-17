#ifndef numuCCOOFVSelection_h
#define numuCCOOFVSelection_h

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

class numuCCOOFVSelection: public SelectionBase{
public:
  numuCCOOFVSelection(bool forceBreak=true);
  virtual ~numuCCOOFVSelection(){
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

class ToyBoxCCOOFV: public ToyBoxNDUP{

public:

  ToyBoxCCOOFV() {
    track_ECal_MipEM = -0xABCDEF;
    track_ECal_EneOnL = -0xABCDEF;
    MainTrack = NULL;
    Tracks.clear();
    OOFV = -1;
    TPC_det = SubDetId::kInvalid;
  }

  virtual void Reset(){
    ResetBase();
    track_ECal_MipEM = -0xABCDEF;
    track_ECal_EneOnL = -0xABCDEF;
    MainTrack = NULL;
    Tracks.clear();
    OOFV = -1;
    TPC_det = SubDetId::kInvalid;
  }

  virtual ~ToyBoxCCOOFV(){}

  /// For storing tracks information in the bunch
  float track_ECal_MipEM, track_ECal_EneOnL;
  AnaTrackB* MainTrack;
  std::vector<AnaTrackB*> Tracks;
  int OOFV;
  SubDetId::SubDetEnum TPC_det;
  
  float reco_ToF, true_ToF;

};

inline ToyBoxB* numuCCOOFVSelection::MakeToyBox() {return new ToyBoxCCOOFV();}

namespace numuCCOOFVUtils{

  class TotalMultiplicityCut: public StepBase{
  public:
    TotalMultiplicityCut() {
      useTarget1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget1");
      useTarget2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget2");
      useFGD1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD1");
      useFGD2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD2");
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
      useTarget1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget1");
      useTarget2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget2");
      useFGD1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD1");
      useFGD2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD2");
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
      useTarget1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget1");
      useTarget2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget2");
      useFGD1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD1");
      useFGD2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD2");
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

  class FillSummaryAction_numuCCOOFV: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;  
    StepBase* MakeClone(){return new FillSummaryAction_numuCCOOFV();}
  };

  class TPCPIDCut: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new TPCPIDCut();}
  };

  class ECalPIDCut: public StepBase{
  public:
    ECalPIDCut(TFile* file_ECAL_PDF){
      _file_ECAL_PDF = file_ECAL_PDF;
    }
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new ECalPIDCut(_file_ECAL_PDF);}
  private:
   TFile* _file_ECAL_PDF;
  };

  class ToFCut: public StepBase{
  public:
    ToFCut(){
      _randomGen = new TRandom3(0);
    }
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new ToFCut();}
  private:
    TRandom3* _randomGen;
  };

}

#endif
