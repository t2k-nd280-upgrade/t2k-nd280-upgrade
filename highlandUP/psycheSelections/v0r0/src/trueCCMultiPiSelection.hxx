#ifndef trueCCMultiPiSelection_h
#define trueCCMultiPiSelection_h
#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "BaseDataClasses.hxx"
#include "TRandom2.h"
#include "AnalysisUtils.hxx"
#include "TFile.h"
#include "TRandom3.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TRandom.h"
#include "TFile.h"
#include "AnaTrueBox.hxx"
class trueCCMultiPiSelection: public SelectionBase{
 public:
  trueCCMultiPiSelection(bool forceBreak=true);
  virtual ~trueCCMultiPiSelection(){            
    _file0->Close();
     delete _file0;
  }

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new AnaTrueBox();}
  SampleId::SampleEnum GetSampleEnum(){return SampleId::kTarget1NuMuCC0Pi;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
    bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
        TFile *_file0;

protected:

  Int_t _MuonPIDCutIndex;
  Int_t _MuonPIDStepIndex;
  Int_t _FindLeadingTracksStepIndex;
  Int_t _TotalMultiplicityCutIndex;


};




namespace trueUtils{
    
    /// Look for the lepton track for each event
    class LeptonTrackAction: public StepBase {
    public:
        using StepBase::Apply;

        LeptonTrackAction() {
        }

        /// Apply the event quality cut. See EventQualityCut class documentation
        /// for details.
        bool Apply(AnaEventC & event, ToyBoxB & box) const;
        StepBase * MakeClone() {return new LeptonTrackAction();}
    };

    /// Look for the lepton track for each event
    class CC0piCut: public StepBase {
    public:
        using StepBase::Apply;
        
        CC0piCut(){
        }
        
        /// Apply the event quality cut. See EventQualityCut class documentation
        /// for details.
        bool Apply(AnaEventC & event, ToyBoxB & box) const;
        StepBase* MakeClone(){return new CC0piCut();}
    };
    /// Look for the lepton track for each event
    class CCOtherCut: public StepBase {
    public:
        using StepBase::Apply;
        
        CCOtherCut(){
        }
        
        /// Apply the event quality cut. See EventQualityCut class documentation
        /// for details.
        bool Apply(AnaEventC & event, ToyBoxB & box) const;
        StepBase* MakeClone(){return new CCOtherCut();}
    };
    /// Look for the lepton track for each event
    class CC1piCut: public StepBase {
    public:
        using StepBase::Apply;
        
        CC1piCut(){
        }
        
        /// Apply the event quality cut. See EventQualityCut class documentation
        /// for details.
        bool Apply(AnaEventC & event, ToyBoxB & box) const;
        StepBase* MakeClone(){return new CC1piCut();}
    };
    /// Look for the lepton track for each event
    class EfficiencyCut: public StepBase {
    public:
        using StepBase::Apply;
        
        EfficiencyCut(TFile *_file0) {
            _file0_=_file0;
            TH2 *muEff_Target1 = (TH2*) _file0->Get("muEff_Target1");
            TH2 *muEff_Target2 = (TH2*) _file0->Get("muEff_Target2");
            TH2 *HM_Other_Target1 = (TH2*) _file0->Get("HM_Other_Target1");
            TH2 *HM_Other_Target2 = (TH2*) _file0->Get("HM_Other_Target2");

        }

        /// Apply the event quality cut. See EventQualityCut class documentation
        /// for details.
        bool Apply(AnaEventC & event, ToyBoxB & box) const;
        StepBase * MakeClone() {return new EfficiencyCut(_file0_);}
        TH2 *muEff_Target1;
        TH2 *muEff_Target2;
        TH2 *HM_Other_Target1;
        TH2 *HM_Other_Target2;
        TFile *_file0_;


    };
    /// Efficiency of the pion reconstruction
    class PionEfficiencyAction: public StepBase {
    public:
        using StepBase::Apply;

        PionEfficiencyAction(TFile *_file1) {
            _file1_=_file1;
            pi0Eff = (TH2*) _file1->Get("pi0Eff");
            pipEff = (TH2*) _file1->Get("pipEff");
            pimEff = (TH2*) _file1->Get("pimEff");
            proEff = (TH2*) _file1->Get("proEff");
            mupiEff = (TH2*) _file1->Get("mupiEff");
            other_pipEff = (TH2*) _file1->Get("other_pipEff");
            other_pimEff = (TH2*) _file1->Get("other_pimEff");

        }

        /// Apply the event quality cut. See EventQualityCut class documentation
        /// for details.
        bool Apply(AnaEventC & event, ToyBoxB & box) const;
        StepBase * MakeClone() {return new PionEfficiencyAction(_file1_);}

        TH2 *pi0Eff;
        TH2 *pipEff;
        TH2 *pimEff;
        TH2 *proEff;
        TH2 *mupiEff;
        TH2 *other_pipEff;
        TH2 *other_pimEff;
        TFile *_file1_;


    };
    /// Smearing of the momentum
    class SmearingAction: public StepBase {
    public:
        using StepBase::Apply;
        
        SmearingAction(){
        }
        
        /// Apply the event quality cut. See EventQualityCut class documentation
        /// for details.
        bool Apply(AnaEventC & event, ToyBoxB & box) const;
        StepBase* MakeClone(){return new SmearingAction();}
    };
    /// Cut for the true vertex to be in Target1
    class inTarget1Cut: public StepBase {
    public:
        using StepBase::Apply;
        
        inTarget1Cut(){
        }
        
        /// Apply the event quality cut. See EventQualityCut class documentation
        /// for details.
  bool Apply(AnaEventC & event, ToyBoxB & box) const;
          StepBase* MakeClone(){return new inTarget1Cut();}
    };
    
    /// Cut for the true vertex to be in Target2
    class inTarget2Cut: public StepBase {
    public:
        using StepBase::Apply;
        
        inTarget2Cut(){
        }
        
        /// Apply the event quality cut. See EventQualityCut class documentation
        /// for details.
  bool Apply(AnaEventC & event, ToyBoxB & box) const;
          StepBase* MakeClone(){return new inTarget2Cut();}
    };
    
}


class FillSummaryAction_trueCCMultiPi: public StepBase{
public:
    using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
    StepBase* MakeClone(){return new FillSummaryAction_trueCCMultiPi();}
};

#endif
