#ifndef truenumuCCSelection_h
#define truenumuCCSelection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "trueToyBoxNDUP.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

class truenumuCCSelection: public SelectionBase{
 public:
  truenumuCCSelection(bool forceBreak=true);
  virtual ~truenumuCCSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new trueToyBoxNDUP();}
  SampleId::SampleEnum GetSampleEnum(){return SampleId::kTarget1NuMuCC;}
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
namespace trueNDUP{

class FillSummaryAction_duneExample: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_duneExample();}
};


class TPCLengthCut: public StepBase{
 public:
          TPCLengthCut(){
            tpcLYZCut = ND::params().GetParameterD("numuCCAnalysis.Selection.tpcLYZCut");

        }

        double tpcLYZCut;


  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TPCLengthCut();}
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
  /// Find the Vertex. For the moment it's just the Star position of the longest track
class VertexInTargetCut: public StepBase{
 public:
    VertexInTargetCut(SubDetId::SubDetEnum det){_detector=det;}

  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new VertexInTargetCut(_detector);}
    SubDetId::SubDetEnum _detector;

};
//nt it's just the Star position of the longest track
class TPCOrTargetLengthCut: public StepBase{
 public:
      TPCOrTargetLengthCut(SubDetId::SubDetEnum det){_detector=det;}

  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TPCOrTargetLengthCut(_detector);}
    SubDetId::SubDetEnum _detector;

};
class TargetLengthCut: public StepBase{
 public:
      TargetLengthCut(SubDetId::SubDetEnum det){_detector=det;}

  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TargetLengthCut(_detector);}
    SubDetId::SubDetEnum _detector;

};

  /// Find the Vertex. For the moment it's just the Star position of the longest track
class CCVertexCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new CCVertexCut();}
};

  /// Find the Vertex. For the moment it's just the Star position of the longest track
class modeVertexCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new modeVertexCut();}
};

  /// Find the True Vertex. For the moment it's just start position of the true particle associated to the longest track
class FindTrueVertexAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindTrueVertexAction();}
};
}
#endif
