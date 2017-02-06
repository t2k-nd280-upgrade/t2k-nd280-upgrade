#ifndef numuCC4piECalCanSelection_h
#define numuCC4piECalCanSelection_h

#include "SelectionBase.hxx"
#include "SubDetId.hxx"
#include "ToyBoxNDUP.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"
#include "numuCC4piFwdCanSelection.hxx"
#include "TFile.h"

//#include "ToFSenseCorrector.hxx"

class numuCC4piECalCanSelection: public SelectionBase{
public:
  numuCC4piECalCanSelection(bool forceBreak=true);
  virtual ~numuCC4piECalCanSelection(){
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
inline ToyBoxB* numuCC4piECalCanSelection::MakeToyBox() {return new ToyBoxCC4pi();}

namespace numuCC4piCanUtils{
class TrackECalGQandFVCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrackECalGQandFVCut();}
};


class ECal_Quality: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ECal_Quality();}
};
class ECal_PID: public StepBase{
public:
  using StepBase::Apply;
    ECal_PID(TFile *f) {
    _file_ECAL_PDF = f;
  }

  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ECal_PID(_file_ECAL_PDF);}
    TFile* _file_ECAL_PDF;

};

}
#endif
