#ifndef ndupSelection_h
#define ndupSelection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxNDUP.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

class ndupSelection: public SelectionBase{
 public:
  ndupSelection(bool forceBreak=true);
  virtual ~ndupSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxNDUP();}
  SampleId::SampleEnum GetSampleEnum(){return SampleId::kFGD1NuMuCC;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);

protected:

  Int_t _MuonPIDCutIndex;
  Int_t _MuonPIDStepIndex;
  Int_t _FindLeadingTracksStepIndex;
  Int_t _TotalMultiplicityCutIndex;


};

class FillSummaryAction_duneExample: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_duneExample();}
};

class TrackQualityFiducialCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrackQualityFiducialCut();}
};


class DeltaLYZTPCCut: public StepBase{
 public:
          DeltaLYZTPCCut(){
            tpcLYZCut = ND::params().GetParameterD("ndupAnalysis.tpcLYZCut");

        }

        double tpcLYZCut;


  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new DeltaLYZTPCCut();}
};


class DeltaLYZTarget1Cut: public StepBase{
 public:
          DeltaLYZTarget1Cut(){
            targetLYZCut = ND::params().GetParameterD("ndupAnalysis.targetLYZCut");
        }

        double targetLYZCut;

  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new DeltaLYZTarget1Cut();}
};
class DeltaLYZTarget2Cut: public StepBase{
 public:
          DeltaLYZTarget2Cut(){
            targetLYZCut = ND::params().GetParameterD("ndupAnalysis.targetLYZCut");
        }

        double targetLYZCut;

  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new DeltaLYZTarget2Cut();}
};
class DeltaLYZTargetORTPCCut: public StepBase {
public:
  DeltaLYZTargetORTPCCut() {

    targetLYZCut = ND::params().GetParameterD("ndupAnalysis.targetLYZCut");
    tpcLYZCut = ND::params().GetParameterD("ndupAnalysis.tpcLYZCut");

  }
          double targetLYZCut;
          double tpcLYZCut;

  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone() {return new DeltaLYZTargetORTPCCut();}
};


class FiducialCut: public StepBase{
 public:
  FiducialCut(SubDetId::SubDetEnum det){_detector=det;}
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FiducialCut(_detector);}
protected:
  SubDetId::SubDetEnum _detector;

};

class TotalMultiplicityCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TotalMultiplicityCut();}
};
/// Leading tracks with good quality in Subdet1_1
class FindLeadingTracksAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindLeadingTracksAction();}
};
/*
  /// Find the Vertex. For the moment it's just the Star position of the longest track
class FindVertexAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindVertexAction();}
};
*/
  /// Find the Vertex. For the moment it's just the Star position of the longest track
class VertexInTarget1Cut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new VertexInTarget1Cut();}
};
  /// Find the Vertex. For the moment it's just the Star position of the longest track
class VertexInTarget2Cut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new VertexInTarget2Cut();}
};

  /// Find the True Vertex. For the moment it's just start position of the true particle associated to the longest track
class FindTrueVertexAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindTrueVertexAction();}
};

  /// Find the True Vertex. For the moment it's just start position of the true particle associated to the longest track
class PionCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new PionCut();}
};
class ProtonCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ProtonCut();}
};
class MuonCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new MuonCut();}
};
class ElectronCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ElectronCut();}
};

  /// Find the True Vertex. For the moment it's just start position of the true particle associated to the longest track
//class ChargCut: public StepBase{
// public:
 // using StepBase::Apply;
 // bool Apply(AnaEventC& event, ToyBoxB& box) const;
 // StepBase* MakeClone(){return new ChargCut();}
//};

#endif
