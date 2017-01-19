#ifndef numuCCMultiPiSelection_h
#define numuCCMultiPiSelection_h

#include "numuCCSelection.hxx"
#include "Parameters.hxx"
#include "BasicUtils.hxx"
class numuCCMultiPiSelection: public SelectionBase{

public:
  numuCCMultiPiSelection(bool forceBreak=true);
  virtual ~numuCCMultiPiSelection(){}

  void DefineSteps();
  void DefineDetectorFV();
  inline ToyBoxB* MakeToyBox();
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId::SampleEnum GetSampleEnum(){return SampleId::kTarget1NuMuCC0Pi;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox,  Int_t& redoFromStep);

protected:
  numuCCSelection _numuCCSelection;

  Int_t _MuonPIDCutIndex;
  Int_t _FindPionsStepIndex;   
};

class ToyBoxCCMultiPi: public ToyBoxNDUP{

public:
  ToyBoxCCMultiPi(){
    nPositivePionTPCtracks = 0;
    nNegativePionTPCtracks = 0;
   // nMichelElectrons       = 0;
    nIsoTargetPiontracks      = 0;
    nPosPi0TPCtracks       = 0;
    nElPi0TPCtracks        = 0;
    nIsoTargetElPi0tracks     = 0;
    nPosPions              = 0;
    nOtherPions            = 0;
    nIsoEcals              = 0;

    // For ECal Pi0 veto
    HObject        = NULL;
    Pi0Ecaltrack   = NULL;
    nPi0Ecaltracks = 0;

    anaUtils::ClearArray(PositivePionTPCtracks, 5000);
    anaUtils::ClearArray(NegativePionTPCtracks, 5000);
    //anaUtils::ClearArray(MichelElectrons,       100);
    anaUtils::ClearArray(IsoTargetPiontracks,      5000);
    anaUtils::ClearArray(PosPi0TPCtracks,       5000);
    anaUtils::ClearArray(ElPi0TPCtracks,        5000);
    anaUtils::ClearArray(IsoTargetElPi0tracks,     5000);
    anaUtils::ClearArray(IsoEcals,              5000);

  }

  virtual ~ToyBoxCCMultiPi(){}

  void Reset(){
    anaUtils::ClearArray(PositivePionTPCtracks, nPositivePionTPCtracks);
    anaUtils::ClearArray(NegativePionTPCtracks, nNegativePionTPCtracks);
   // anaUtils::ClearArray(MichelElectrons,       nMichelElectrons);
    anaUtils::ClearArray(IsoTargetPiontracks,      nIsoTargetPiontracks);
    anaUtils::ClearArray(PosPi0TPCtracks,       nPosPi0TPCtracks);
    anaUtils::ClearArray(ElPi0TPCtracks,        nElPi0TPCtracks);
    anaUtils::ClearArray(IsoTargetElPi0tracks,     nIsoTargetElPi0tracks);
    anaUtils::ClearArray(IsoEcals,              nIsoEcals);

    nPositivePionTPCtracks = 0;
    nNegativePionTPCtracks = 0;
  //  nMichelElectrons       = 0;
    nIsoTargetPiontracks      = 0;
    nPosPi0TPCtracks       = 0;
    nElPi0TPCtracks        = 0;
    nIsoTargetElPi0tracks     = 0;
    nPosPions              = 0;
    nOtherPions            = 0;
    nIsoEcals              = 0;

    HObject = NULL;
    Pi0Ecaltrack  = NULL;
    nPi0Ecaltracks = 0;
  }

  // positive  TPC pion candidates 
  AnaTrackB* PositivePionTPCtracks[5000];
  int nPositivePionTPCtracks;

  // negative TPC pion candidates 
  AnaTrackB* NegativePionTPCtracks[5000];
  int nNegativePionTPCtracks;

  //  Michel Electron candidates 
  //AnaTargetTimeBinB* MichelElectrons[100];
  //int nMichelElectrons;

  // IsoTarget Pion candidates 
  AnaTrackB* IsoTargetPiontracks[5000];
  int nIsoTargetPiontracks;

  // Electrons and positrons from Pio in the TPC
  AnaTrackB* PosPi0TPCtracks[5000];
  AnaTrackB* ElPi0TPCtracks[5000];
  int nPosPi0TPCtracks;
  int nElPi0TPCtracks;

  // IsoTarget electron and positron candidates 
  AnaTrackB* IsoTargetElPi0tracks[5000];
  int nIsoTargetElPi0tracks;

  int nPosPions;
  int nOtherPions;

  // isoECal track
  int nIsoEcals;
  AnaTrackB* IsoEcals[5000];

  // Most energetic object in ECal
  AnaTrackB* HObject;
  // Candidate Pi0 from Ecal veto
  AnaTrackB* Pi0Ecaltrack;

  int nPi0Ecaltracks;
};

inline ToyBoxB* numuCCMultiPiSelection::MakeToyBox(){return new ToyBoxCCMultiPi();}

namespace numuCCMultiPiUtils{
  //From NumuCCBANFF Standard Actions.
  void FindTPCPions(AnaEventC& event, ToyBoxB& box, SubDetId::SubDetEnum det, bool useOldSecondaryPID);
 // void FindIsoTargetPions(AnaEventC& event, ToyBoxB& box, SubDetId::SubDetEnum det);
 // void FindIsoTargetElPi0(AnaEventC& event, ToyBoxB& box, SubDetId::SubDetEnum det);
 // void FindMEPions(AnaEventC& event, SubDetId::SubDetEnum det, bool prod5Cut = 0);

  //From NumuCCBANFF Analysis Utils.
  bool TPCpionSelection(AnaTrackB *track);
  bool TPCElPi0Selection(AnaTrackB *track);
 // bool TargetpionSelection(AnaTrackB *track, SubDetId::SubDetEnum det);
 // bool TargetElPi0Selection(AnaEventC& event, ToyBoxB& box, AnaTrackB *track, SubDetId::SubDetEnum det);
}


class NoPionCut: public StepBase{  
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new NoPionCut();}
};

class OnePionCut: public StepBase{  
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new OnePionCut();}
};

class OthersCut: public StepBase{  
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new OthersCut();}
};

class FillSummaryAction_numuCCMultiPi: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_numuCCMultiPi();}
};

class FindPionsAction: public StepBase{
public:
  using StepBase::Apply;
  FindPionsAction(){
    useTPCPions = (bool)ND::params().GetParameterI("psycheSelections.numuCCMultiPi.UseTPCPions");
    useME = (bool)ND::params().GetParameterI("psycheSelections.numuCCMultiPi.UseME");
    useTargetPions = (bool)ND::params().GetParameterI("psycheSelections.numuCCMultiPi.UseTargetPions");
    useOldSecondaryPID = (bool) ND::params().GetParameterI("psycheSelections.numuCCMultiPi.OldSecondaryPID");
    prod5Cut = (bool)ND::params().GetParameterI("psycheSelections.numuCCMultiPi.Prod5Cuts");
  }
  bool useTPCPions;
  bool useME;
  bool useTargetPions;
  bool useOldSecondaryPID;
  bool prod5Cut;

  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FindPionsAction();}
};
/*
class  FindEcalPhotonsAction: public StepBase{
public:
  using StepBase::Apply;
  FindEcalPhotonsAction(){
    _MostUpstreamLayerHitCut = (int)ND::params().GetParameterI("psycheSelections.numuCCMultiPi.MostUpstreamLayerHitCut");
  }
 
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new  FindEcalPhotonsAction();}

protected:
 int  _MostUpstreamLayerHitCut; 

};

class  EcalPi0VetoCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new  EcalPi0VetoCut();}
};
*/
#endif
