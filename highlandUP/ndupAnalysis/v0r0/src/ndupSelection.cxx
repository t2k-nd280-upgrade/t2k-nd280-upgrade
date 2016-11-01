#include "ndupSelection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBox.hxx"
#include "EventBoxUtils.hxx"

#include "SystematicUtils.hxx"
#include "SelectionUtils.hxx"
#include "DataClasses.hxx"

//********************************************************************
ndupSelection::ndupSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxNDUP) {
//********************************************************************

  // Initialize systematic tuning parameters
  //  systTuning::Initialize();
}

//********************************************************************
void ndupSelection::DefineSteps() {
//********************************************************************

  // Cuts must be added in the right order
  // last "true" means the step sequence is broken if cut is not passed (default is "false")
   AddStep(StepBase::kAction, "find true vertex",        new FindTrueVertexAction());

  AddStep( StepBase::kCut, " true vertex in target  ",        new TrueVertexInTarget1Cut(), true);
  AddStep(StepBase::kCut,    "> 0 tracks ",        new TotalMultiplicityCut(),      true);
  AddStep(StepBase::kAction, "find vertex",        new FindVertexAction());
  AddStep(StepBase::kAction, "find leading tracks", new FindLeadingTracksAction());
  AddStep( StepBase::kCut, " vertex in target  ",        new VertexInTarget1Cut(), true);
  AddStep(StepBase::kCut,    "muon PID",           new MuonPIDCut(),true);
 
 SetBranchAlias(0, "muon track in target ");


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
void ndupSelection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kTarget1);
}

//**************************************************
bool FindVertexAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    (void)event;

    // Cast the ToyBox to the appropriate type
    ToyBoxNDUP& box = *static_cast<ToyBoxNDUP*>(&boxB); 

    // reset the vertex 
    if (box.Vertex) delete box.Vertex;
    box.Vertex = NULL;

    // also the true vertex
    box.TrueVertex = NULL;

    if (!box.MainTrack) return false;

    box.Vertex = new AnaVertexB();
    anaUtils::CreateArray(box.Vertex->Particles, 1);

    box.Vertex->nParticles = 0;
    box.Vertex->Particles[box.Vertex->nParticles++] = box.MainTrack;

    for(int i = 0; i < 4; ++i){
        box.Vertex->Position[i] = box.MainTrack->PositionStart[i];
    }
    if(  box.MainTrack->GetTrueParticle() ){ 

      box.TrueVertex = box.Vertex->TrueVertex = box.MainTrack->GetTrueParticle()->TrueVertex;
    }
    return true;
}

//**************************************************
bool TotalMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& box) const{
//**************************************************

  (void)box;
  // Check we have at least one reconstructed track in the Subdet2
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];  
  return (EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPC]>0);
}

//**************************************************
bool FiducialCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;

  // This cut check the existence of track with position inside the Fiducial Volume and 
  // with a minimum number of hits

  // Cast the ToyBox to the appropriate type
  ToyBoxNDUP& box = *dynamic_cast<ToyBoxNDUP*>(&boxB); 

  if (!box.MainTrack) return false;

  return cutUtils::FiducialCut(*box.MainTrack, _detector);
}

//**************************************************
bool VertexInTarget2Cut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  ToyBoxNDUP& box = *dynamic_cast<ToyBoxNDUP*>(&boxB); 
    if(!box.MainTrack)return false;
  return cutUtils::FiducialCut(box.MainTrack->PositionStart,SubDetId::kTarget2);
}

//**************************************************
bool VertexInTarget1Cut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  ToyBoxNDUP& box = *dynamic_cast<ToyBoxNDUP*>(&boxB); 
  if(!box.MainTrack)return false;
  return cutUtils::FiducialCut(box.MainTrack->PositionStart,SubDetId::kTarget1);
}
//**************************************************
bool TrueVertexInTarget1Cut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxNDUP& box = *dynamic_cast<ToyBoxNDUP*>(&boxB); 

      return cutUtils::FiducialCut(box.Vertex->Position,SubDetId::kTarget1);

}

//**************************************************
bool TrackQualityFiducialCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;

  // This cut check the existence of track with position inside the Fiducial Volume and 
  // with a minimum number of hits

  // Cast the ToyBox to the appropriate type
  ToyBoxNDUP& box = *dynamic_cast<ToyBoxNDUP*>(&boxB); 

  return (box.Vertex);
}
//**************************************************
bool FindTrueVertexAction::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void)event;
  ToyBoxNDUP& box = *dynamic_cast<ToyBoxNDUP*>(&boxB); 
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
bool FindLeadingTracksAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************y

    // Cast the ToyBox to the appropriate type
    ToyBoxNDUP& box = *dynamic_cast<ToyBoxNDUP*>(&boxB); 
        // Find the longest tracks among all tracks passing the quality criteria
    SelUtils::FindLeadingTracks(event,box);

    // For this selection the main track is the Longest track
  box.MainTrack = box.HMNtrack;
  return true;
}

//**************************************************
bool ndupSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  (void)systId;
  (void)recObj;

  return true;
}

//**************************************************
bool ndupSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  (void)systId;
  (void)trueObj;
  return true;
}

//**************************************************
bool ndupSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  (void)boxB;
  (void)systId;
  (void)track;



  return true;
}

//**************************************************
bool ndupSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  (void)boxB;
  (void)systId;
  (void)trueObj;

  return true;
}

//**************************************************
bool ndupSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  (void)box;
  (void)systId;

  return true;
}

//**************************************************
void ndupSelection::InitializeEvent(AnaEventC& eventBB){
//**************************************************

  AnaEventB& event = *dynamic_cast<AnaEventB*>(&eventBB); 

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxNDUP])
    event.EventBoxes[EventBoxId::kEventBoxNDUP] = new EventBox();


  boxUtils::FillTracksWithTPC(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithTarget(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTrajsChargedInTPC(event);
  boxUtils::FillTrajsChargedInTargetAndNoTPC(event, static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
 }

//********************************************************************
bool ndupSelection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
//********************************************************************

  (void)eventC;
  (void)PreviousToyBoxB;
  (void)redoFromStep;

  return true;
  return false;
}

//**************************************************
bool MuonPIDCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    (void)event;

    // Cast the ToyBox to the appropriate type
    ToyBoxNDUP& box = *static_cast<ToyBoxNDUP*>(&boxB); 
//
    if (!box.MainTrack) return false;
   if (box.MainTrack->Momentum < 0.) return false;
   return cutUtils::MuonPIDCut(*(box.MainTrack), _prod5Cut);
    return true;
}
