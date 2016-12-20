#include "truenumuCCSelection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "trueEventBox.hxx"
#include "SystematicUtils.hxx"
#include "trueSelectionUtils.hxx"
#include "DataClasses.hxx"

//********************************************************************
truenumuCCSelection::truenumuCCSelection(bool forceBreak): SelectionBase(forceBreak, EventBoxId::ktrueEventBoxNDUP) {
//********************************************************************

  // Initialize systematic tuning parameters
  //  systTuning::Initialize();
}

//********************************************************************
void truenumuCCSelection::DefineSteps() {
//********************************************************************
  int sel  = ND::params().GetParameterI("ndupAnalysis.Selection");
  int target  = ND::params().GetParameterI("ndupAnalysis.target");

  SubDetId::SubDetEnum _detector = SubDetId::kTarget1;
  if (target) {
    _detector = SubDetId::kTarget1;
  } else {
    _detector = SubDetId::kTarget2;
  }
  // Cuts must be added in the right order
  // last "true" means the step sequence is broken if cut is not passed (default is "false")
  AddStep(StepBase::kAction, "find true vertex",        new trueNDUP::FindTrueVertexAction());

  AddStep( StepBase::kCut, " vertex in target  ",        new trueNDUP::VertexInTargetCut( _detector), false);
  AddStep( StepBase::kCut, " CC cut",        new trueNDUP::CCVertexCut(), false);

  AddStep(StepBase::kCut, "find leading tracks", new trueNDUP::FindLeadingTracksAction(), false);
  if (sel == 1) {

    AddStep(StepBase::kCut, "tpc length", new trueNDUP::TPCLengthCut(), false);
  } else if (sel == 2) {
    AddStep(StepBase::kCut, "target length", new trueNDUP::TargetLengthCut(_detector), false);

  } else if (sel == 3) {
    AddStep(StepBase::kCut, "target length", new trueNDUP::TargetLengthCut(_detector), false);

    AddStep(StepBase::kCut, "tpc length", new trueNDUP::TPCLengthCut(), false);

  } else if (sel == 4) {
    AddStep(StepBase::kCut, "tpc or target length", new trueNDUP::TPCOrTargetLengthCut(_detector), false);

  }
  SetBranchAlias(0, "muon track in target 1");


  // By default the preselection correspond to cuts 0-2.
  // It means that if any of the three first cuts (0,1,2) is not passed
  // the loop over toys will be broken ===> A single toy will be run
  SetPreSelectionAccumLevel(0);

  // Step and Cut numbers needed by CheckRedoSelection
  // _MuonPIDCutIndex            = GetCutNumber("find vertex");
  // _MuonPIDStepIndex           = GetStepNumber("find vertex");
  // _FindLeadingTracksStepIndex = GetStepNumber("find leading tracks");
  // _TotalMultiplicityCutIndex  = GetCutNumber("> 0 tracks ");
}

//********************************************************************
void truenumuCCSelection::DefineDetectorFV() {
//********************************************************************

  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kTarget1);
}
namespace trueNDUP {
//**************************************************
bool TotalMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& box) const {
//**************************************************

  (void)box;
  // Check we have at least one reconstructed track in the Subdet2
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::ktrueEventBoxNDUP];
  return (EventBox->nRecObjectsInGroup[EventBox::kLongTracks] > 0);
}

//**************************************************
bool FiducialCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void)event;

  // This cut check the existence of track with position inside the Fiducial Volume and
  // with a minimum number of hits

  // Cast the ToyBox to the appropriate type
  trueToyBoxNDUP& box = *dynamic_cast<trueToyBoxNDUP*>(&boxB);

  if (!box.MainTrack) return false;

  return cutUtils::FiducialCut(*box.MainTrack, _detector);
}


//**************************************************
bool CCVertexCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  trueToyBoxNDUP& box = *dynamic_cast<trueToyBoxNDUP*>(&boxB);
  AnaTrueVertex* trueVer = dynamic_cast<AnaTrueVertex*>(box.Vertex->TrueVertex);

  return (cutUtils::GetReacAll(trueVer->ReacCode) != 5);
}

//**************************************************
bool modeVertexCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  trueToyBoxNDUP& box = *dynamic_cast<trueToyBoxNDUP*>(&boxB);
  AnaTrueVertex* trueVer = dynamic_cast<AnaTrueVertex*>(box.Vertex->TrueVertex);

  return (cutUtils::GetReacAll(trueVer->ReacCode) == 0);
}

//**************************************************
//bool ChargeCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

//trueToyBoxNDUP& box = *dynamic_cast<trueToyBoxNDUP*>(&boxB);
//if()
//box.MainTrack=HElectronT;

//}

//**************************************************
bool VertexInTargetCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  trueToyBoxNDUP& box = *dynamic_cast<trueToyBoxNDUP*>(&boxB);
  return cutUtils::FiducialCut(box.Vertex->Position, _detector);
}

//**************************************************
bool FindTrueVertexAction::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void)event;
  trueToyBoxNDUP& box = *dynamic_cast<trueToyBoxNDUP*>(&boxB);
  AnaEventB& eventB = *static_cast<AnaEventB*>(&event);

  // Cast the ToyBox to the appropriate type
  for (Int_t i = 0; i < eventB.nVertices; i++) {

    AnaVertexB* vertex = static_cast<AnaVertexB*>(eventB.Vertices[i]);
    box.Vertex = vertex->Clone();
  }

  if (!box.Vertex) return false;
  box.TrueVertex = box.Vertex->TrueVertex;

  return true;
}

//**************************************************
bool TPCLengthCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void)event;
  trueToyBoxNDUP& box = *dynamic_cast<trueToyBoxNDUP*>(&boxB);

  return cutUtils::DeltaLYZTPCCut(*box.MainTrack);
}

//**************************************************
bool TargetLengthCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void)event;
  trueToyBoxNDUP& box = *dynamic_cast<trueToyBoxNDUP*>(&boxB);

  return cutUtils::DeltaLYZTargetCut(*box.MainTrack, _detector);
}

//**************************************************
bool TPCOrTargetLengthCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void)event;
  trueToyBoxNDUP& box = *dynamic_cast<trueToyBoxNDUP*>(&boxB);

  return cutUtils::DeltaLYZTargetORTPCCut(*box.MainTrack, _detector);
}


//**************************************************
bool FindLeadingTracksAction::Apply(AnaEventC & event, ToyBoxB & boxB) const {
//**************************************************

  // Cast the ToyBox to the appropriate type
  trueToyBoxNDUP& box = *dynamic_cast<trueToyBoxNDUP*>(&boxB);

  // Find the longest tracks among all tracks passing the quality criteria
  trueSelUtils::FindLeadingTracks(event, box);
  int  particle = ND::params().GetParameterI("ndupAnalysis.particle");

  // For this selection the main track is the Longest track
  if (particle == 13) {
    if (box.HMT) {
      box.MainTrack = box.HMT;
      return true;
    } else {
      return false;
    }
  } else if (particle == 211) {
    if (box.HPlusPionT) {
      box.MainTrack = box.HPlusPionT;
      return true;
    } else {
      return false;
    }

  } else if (particle == -211) {
    if (box.HMinusPionT) {
      box.MainTrack = box.HMinusPionT;
      return true;
    } else {
      return false;
    }

  } else if (particle == 2212) {
    if (box.HProtonT) {
      box.MainTrack = box.HProtonT;
      return true;
    } else {
      return false;
    }

  }


}
}
//**************************************************
bool truenumuCCSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const {
//**************************************************

  (void)event;
  (void)branch;
  (void)systId;
  (void)recObj;

  return true;
}

//**************************************************
bool truenumuCCSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const {
//**************************************************

  (void)event;
  (void)branch;
  (void)systId;
  (void)trueObj;
  return true;
}

//**************************************************
bool truenumuCCSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* track, SystId_h systId, Int_t branch) const {
//**************************************************

  (void)event;
  (void)branch;
  (void)boxB;
  (void)systId;
  (void)track;



  return true;
}

//**************************************************
bool truenumuCCSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const {
//**************************************************

  (void)event;
  (void)branch;
  (void)boxB;
  (void)systId;
  (void)trueObj;

  return true;
}

//**************************************************
bool truenumuCCSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const {
//**************************************************

  (void)event;
  (void)branch;
  (void)box;
  (void)systId;

  return true;
}

//**************************************************
void truenumuCCSelection::InitializeEvent(AnaEventC& eventBB) {
//**************************************************

  AnaEventB& event = *dynamic_cast<AnaEventB*>(&eventBB);

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::ktrueEventBoxNDUP])
    event.EventBoxes[EventBoxId::ktrueEventBoxNDUP] = new trueEventBox();

  trueboxUtils::FillLongTracks(event,       static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
}

//********************************************************************
bool truenumuCCSelection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep) {
//********************************************************************

  (void)eventC;
  (void)PreviousToyBoxB;
  (void)redoFromStep;

  return true;
  return false;
}
