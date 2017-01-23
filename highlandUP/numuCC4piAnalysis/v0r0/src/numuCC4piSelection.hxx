#ifndef numuCC4piSelection_h
#define numuCC4piSelection_h

#include "SelectionBase.hxx"
#include "SubDetId.hxx"
#include "ToyBoxNDUP.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

//#include "ToFSenseCorrector.hxx"

class numuCC4piSelection: public SelectionBase{
	public:
		numuCC4piSelection(bool forceBreak=true);
		virtual ~numuCC4piSelection(){}

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

};

class ToyBoxCC4pi: public ToyBoxNDUP{

	public:

		ToyBoxCC4pi() { 
			MainTrack = NULL;
			LowAngle.clear(); HighAngle.clear();
			FwdTracks.clear();      BwdTracks.clear();      HAFwdTracks.clear();      HABwdTracks.clear();
			FwdTracks_PID.clear();  BwdTracks_PID.clear();  HAFwdTracks_PID.clear();  HABwdTracks_PID.clear(); CSFD2Tracks_PID.clear(); CSECALTracks_PID.clear();
		}

		virtual void Reset(){
			ResetBase();
			MainTrack = NULL;
			LowAngle.clear(); HighAngle.clear();
			FwdTracks.clear();      BwdTracks.clear();      HAFwdTracks.clear();      HABwdTracks.clear();
			FwdTracks_PID.clear();  BwdTracks_PID.clear();  HAFwdTracks_PID.clear();  HABwdTracks_PID.clear(); CSFD2Tracks_PID.clear(); CSECALTracks_PID.clear();
		}

		virtual ~ToyBoxCC4pi(){}

		/// For storing tracks information in the bunch.
		AnaTrackB* MainTrack;
		std::vector<AnaTrackB*> LowAngle, HighAngle;
		std::vector<AnaTrackB*> FwdTracks,      BwdTracks,      HAFwdTracks,      HABwdTracks;
		std::vector<AnaTrackB*> FwdTracks_PID,  BwdTracks_PID,  HAFwdTracks_PID,  HABwdTracks_PID, CSFD2Tracks_PID, CSECALTracks_PID;

};

inline ToyBoxB* numuCC4piSelection::MakeToyBox() {return new ToyBoxCC4pi();}

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

class PIDAction: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new PIDAction();}
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
/*
class Fwd_Veto: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new Fwd_Veto();}
};*/
class Fwd_PID: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new Fwd_PID();}
};
class Fwd_4pi: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new Fwd_4pi();}
};

class Bwd_Quality: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new Bwd_Quality();}
};
/*
class Bwd_Veto: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new Bwd_Veto();}
};*/
class Bwd_PID: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new Bwd_PID();}
};
class Bwd_4pi: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new Bwd_4pi();}
};

class HAFwd_Quality: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new HAFwd_Quality();}
};
/*class HAFwd_Veto: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new HAFwd_Veto();}
};*/
class HAFwd_PID: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new HAFwd_PID();}
};
class HAFwd_4pi: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new HAFwd_4pi();}
};

class HABwd_Quality: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new HABwd_Quality();}
};
/*class HABwd_Veto: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new HABwd_Veto();}
};*/
class HABwd_PID: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new HABwd_PID();}
};
class HABwd_4pi: public StepBase{
	public:
		using StepBase::Apply;
		bool Apply(AnaEventC& event, ToyBoxB& box) const;
		StepBase* MakeClone(){return new HABwd_4pi();}
};
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
