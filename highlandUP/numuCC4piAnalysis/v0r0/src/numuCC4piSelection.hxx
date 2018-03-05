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

struct MuonCategory { 
  enum Categ { FwdTPC=0, BwdTPC, HATPC, Target, ECal };
};

class numuCC4piSelection: public SelectionBase{
public:
  numuCC4piSelection(bool forceBreak=true);
  virtual ~numuCC4piSelection(){
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

private:
  TFile *_file_ECAL_PDF;

};

class ToyBoxCC4pi: public ToyBoxNDUP{

public:

  ToyBoxCC4pi() {
    track_ECal_MipEM = -0xABCDEF;
    track_ECal_EneOnL = -0xABCDEF;
    MainTrack = NULL;
    categMuon = -1;
    TPCTracks.clear();       ECalTracks.clear();    TargetTracks.clear();
    TruePiPlus.clear(); TruePiMinus.clear(); TruePiZero.clear();
    NegativePionTPCtracks.clear();
    PositivePionTPCtracks.clear();
    IsoTargetPiontracks.clear();
    PosPi0TPCtracks.clear();
    ElPi0TPCtracks.clear();
    MichelElectrons.clear();
    Target_det = SubDetId::kInvalid;
    TPC_det = SubDetId::kInvalid;
  }

  virtual void Reset(){
    ResetBase();
    track_ECal_MipEM = -0xABCDEF;
    track_ECal_EneOnL = -0xABCDEF;
    MainTrack = NULL;
    categMuon = -1;
    TPCTracks.clear();       ECalTracks.clear();    TargetTracks.clear();
    TruePiPlus.clear(); TruePiMinus.clear(); TruePiZero.clear();
    NegativePionTPCtracks.clear();
    PositivePionTPCtracks.clear();
    IsoTargetPiontracks.clear();
    PosPi0TPCtracks.clear();
    ElPi0TPCtracks.clear();
    MichelElectrons.clear();
    Target_det = SubDetId::kInvalid;
    TPC_det = SubDetId::kInvalid;
  }

  virtual ~ToyBoxCC4pi(){}

  /// For storing tracks information in the bunch
  float track_ECal_MipEM, track_ECal_EneOnL;
  AnaTrackB* MainTrack;
  std::vector<AnaTrackB*> TPCTracks, ECalTracks, TargetTracks;
  SubDetId::SubDetEnum Target_det;
  SubDetId::SubDetEnum TPC_det;
  int categMuon;

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

  int nProtonTPCtracks;
  std::vector<AnaTrackB*> ProtonTPCtracks;
  int nIsoTargetProtontracks;
  std::vector<AnaTrackB*> IsoTargetProtontracks;

  int nPosPions;
  int nNegPions;
  int nOtherPions;
  int nProtons;
  
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

  class FindMuonCut: public StepBase{
  public:
    FindMuonCut(TFile *f){_file_ECAL_PDF=f;}
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new FindMuonCut(_file_ECAL_PDF);}
  private:
    TFile* _file_ECAL_PDF; 
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

  class FindProtonsAction: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new FindProtonsAction();}
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

  class FWD: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const{
	 ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
	 return (cc4pibox->categMuon == MuonCategory::FwdTPC);
	};
    StepBase* MakeClone(){return new FWD();}
  };
  
  class BWD: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const{
	 ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
	 return (cc4pibox->categMuon == MuonCategory::BwdTPC);
	};
    StepBase* MakeClone(){return new BWD();}
  };
  
  class HA: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const{
	 ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
	 return (cc4pibox->categMuon == MuonCategory::HATPC);
	};
    StepBase* MakeClone(){return new HA();}
  };
  
  class Target: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const{
	 ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
	 return (cc4pibox->categMuon == MuonCategory::Target);
	};
    StepBase* MakeClone(){return new Target();}
  };
  
  class ECal: public StepBase{
  public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const{
	 ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
	 return (cc4pibox->categMuon == MuonCategory::ECal);
	};
    StepBase* MakeClone(){return new ECal();}
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
