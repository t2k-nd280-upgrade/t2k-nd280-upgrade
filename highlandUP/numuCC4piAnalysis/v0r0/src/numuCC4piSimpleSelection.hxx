#ifndef numuCC4piSimpleSelection_h
#define numuCC4piSimpleSelection_h

#include "SelectionBase.hxx"
#include "SubDetId.hxx"
#include "ToyBoxNDUP.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

#include "TFile.h"

//#include "ToFSenseCorrector.hxx"

class numuCC4piSimpleSelection: public SelectionBase{
public:
  numuCC4piSimpleSelection(bool forceBreak=true);
  virtual ~numuCC4piSimpleSelection(){
    _file_ECAL_PDF->Close();
    delete _file_ECAL_PDF;
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

  TFile *_file_ECAL_PDF;

};

class ToyBoxCC4piSimple: public ToyBoxNDUP{

public:

  ToyBoxCC4piSimple() {
    track_ECal_MipEM = -0xABCDEF;
    track_ECal_EneOnL = -0xABCDEF;
    MainTrack = NULL;
    LowAngle.clear();       HighAngle.clear();
  }

  virtual void Reset(){
    ResetBase();
    track_ECal_MipEM = -0xABCDEF;
    track_ECal_EneOnL = -0xABCDEF;
    MainTrack = NULL;
    LowAngle.clear();       HighAngle.clear();
  }

  virtual ~ToyBoxCC4piSimple(){}

  /// For storing tracks information in the bunch
  float track_ECal_MipEM, track_ECal_EneOnL;
  AnaTrackB* MainTrack;
  std::vector<AnaTrackB*> LowAngle, HighAngle;

};

inline ToyBoxB* numuCC4piSimpleSelection::MakeToyBox() {return new ToyBoxCC4piSimple();}

namespace numuCC4piSimpleUtils{

class TotalMultiplicityCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TotalMultiplicityCut();}
};

class SortTracksAction: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new SortTracksAction();}
};

class TrackGQandFVCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrackGQandFVCut();}
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
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrueVertexInTargetCut();}
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



class Fwd_Quality: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new Fwd_Quality();}
};

class Fwd_PID: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new Fwd_PID();}
};


class Bwd_Quality: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new Bwd_Quality();}
};

class Bwd_PID: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new Bwd_PID();}
};


class ECal_Quality: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ECal_Quality();}
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
