#ifndef nueCCMultiPiSelection_h
#define nueCCMultiPiSelection_h

#include "nueCCSelection.hxx"
#include "nueCCUtils.hxx"

class nueCCMultiPiSelection: public SelectionBase {
public:
  nueCCMultiPiSelection(bool forceBreak=true);
  ~nueCCMultiPiSelection() {}

  void DefineSteps();
  void DefineDetectorFV(){_nueCCSelection->DefineDetectorFV();}
  inline ToyBoxB* MakeToyBox();
  void InitializeEvent(AnaEventC& event){_nueCCSelection->InitializeEvent(event);}

  SelectionBase* _nueCCSelection;
};

namespace nueCCUtils {
  bool FindTPCPions(AnaEventC& event, ToyBoxB& boxB, SubDetId::SubDetEnum det);
  bool FindIsoTargetPions(AnaEventC& event, ToyBoxB& boxB, SubDetId::SubDetEnum det);
};

class ToyBoxCC4piMultiPi: public ToyBoxCC4pi{
public:
  virtual void Reset(){
    ToyBoxCC4pi::Reset();
    nPosPions = nNegPions = nOtherPions = 0;
    NegativePionTPCtracks.clear();
    PositivePionTPCtracks.clear();
    IsoTargetPiontracks.clear();
    PosPi0TPCtracks.clear();
    ElPi0TPCtracks.clear();
    MichelElectrons.clear();
    nNegativePionTPCtracks = nPositivePionTPCtracks = nPosPi0TPCtracks = nElPi0TPCtracks = nMichelElectrons = 0;
  }
  std::vector<AnaTrueParticleB*> TruePiPlus, TruePiMinus, TruePiZero;

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
  int nNegPions;
  int nOtherPions;
};

inline ToyBoxB* nueCCMultiPiSelection::MakeToyBox() {return new ToyBoxCC4piMultiPi();}

class FindPions: public StepBase{
public:
  FindPions() {
    Int_t selection_charge = (Int_t)ND::params().GetParameterI("nueCCAnalysis.SelectCharge");
    if (selection_charge < 0)
      _AntiNu = false;
    else _AntiNu = true;
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindPions();}
private:
  bool _AntiNu;
};

class nueCC1piCut: public StepBase{
public:
  nueCC1piCut() {
    Int_t selection_charge = (Int_t)ND::params().GetParameterI("nueCCAnalysis.SelectCharge");
    if (selection_charge < 0)
      _AntiNu = false;
    else _AntiNu = true;
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new nueCC1piCut();}
private:
  bool _AntiNu;
};

class nueCCOtherCut: public StepBase{
public:
  nueCCOtherCut() {
    Int_t selection_charge = (Int_t)ND::params().GetParameterI("nueCCAnalysis.SelectCharge");
    if (selection_charge < 0)
      _AntiNu = false;
    else _AntiNu = true;
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new nueCCOtherCut();}
private:
  bool _AntiNu;
};

class nueCC0piCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new nueCC0piCut();}
};

#endif