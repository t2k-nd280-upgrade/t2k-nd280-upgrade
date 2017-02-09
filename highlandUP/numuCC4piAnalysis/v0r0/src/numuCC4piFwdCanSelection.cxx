#include "numuCC4piFwdCanSelection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "EventBox.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"

#include "SelectionUtils.hxx"
#include "numuCC4piUtils.hxx"
#include "Parameters.hxx"
#include "CategoriesUtils.hxx"
//********************************************************************
numuCC4piFwdCanSelection::numuCC4piFwdCanSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxNDUP) {
  //********************************************************************

  char filename[256];
  sprintf(filename, "%s/data/pdfs_dsecal.root", getenv("NUMUCC4PIANALYSISROOT"));
  _file_ECAL_PDF = TFile::Open(filename);

}

//********************************************************************
void numuCC4piFwdCanSelection::DefineSteps(){
  //********************************************************************
  int branch=ND::params().GetParameterI("numuCC4piAnalysis.Branch");
  //last true means that if that cut is not fulfill the sequence is stop
  // AddSplit(2);

  AddStep(StepBase::kAction,"find true vertex",   new numuCC4piCanUtils::FindTrueVertexAction());
  AddStep(StepBase::kCut," true vertex in target  ",   new numuCC4piCanUtils::TrueVertexInTargetCut(), true);

  AddStep(StepBase::kCut,    "> 0 tracks ",         new numuCC4piCanUtils::TotalMultiplicityCut(), true); //if passed accum_level=2 
  AddStep(StepBase::kAction, "Sort TPC tracks",     new numuCC4piCanUtils::SortTracksAction());
  AddStep(StepBase::kCut,    "quality+fiducial",    new numuCC4piCanUtils::TrackGQandFVCut(),      true); //if passed accum_level=3

  AddStep(StepBase::kAction, "find vertex",         new numuCC4piCanUtils::FindVertexAction());
  AddStep(StepBase::kAction, "fill summary",        new numuCC4piCanUtils::FillSummaryAction_numuCC4pi());

 // AddSplit(2);


  AddStep( StepBase::kCut, "Fwd Quality Cut",     new numuCC4piCanUtils::Fwd_Quality(),true);     

  AddStep( StepBase::kCut, "Fwd PID Cut",      new numuCC4piCanUtils::Fwd_PID(_file_ECAL_PDF),true);             //if passed accum_level=6
 
  

//  AddStep(1, StepBase::kCut, "Bwd Quality Cut",    new numuCC4piCanUtils::Bwd_Quality(),true);
 // AddStep(1, StepBase::kCut, "Bwd PID Cut",      new numuCC4piCanUtils::Bwd_PID(_file_ECAL_PDF),true);             //if passed accum_level=6

  SetBranchAlias(0, "Fwd");
 // SetBranchAlias(1, "Bwd",    1);
  //if first two cuts are not fulfill dont throw toys
  SetPreSelectionAccumLevel(0);

}

//********************************************************************
void numuCC4piFwdCanSelection::DefineDetectorFV(){
  //********************************************************************

  // Change FV definition to take all thickness
  // Note! this will affect all downstream stuff
  //FVDef::FVdefminTarget1[2] = 0;
  //FVDef::FVdefmaxTarget1[2] = 0;

  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kTarget1);

}

//**************************************************
void numuCC4piFwdCanSelection::InitializeEvent(AnaEventC& eventBB){
  //**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB);

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxNDUP]) event.EventBoxes[EventBoxId::kEventBoxNDUP] = new EventBox();

  boxUtils::FillTracksWithTPC(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithTarget(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithECal(event);
  
  //boxUtils::FillTrajsChargedInTPC(event);
  //boxUtils::FillTrajsChargedInFGDAndNoTPC(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  //boxUtils::FillTrajsChargedHATracker(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));

}

//********************************************************************
bool numuCC4piFwdCanSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
  //********************************************************************

  if(allCutsPassed[0]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[1]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[2]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[3]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[4]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[5]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;

  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kTarget1NuMuCC);
}
/*
namespace numuCC4piCanUtils{

//**************************************************
bool FindTrueVertexAction::Apply(AnaEventC & event, ToyBoxB & boxB) const {
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
bool TrueVertexInTargetCut::Apply(AnaEventC & event, ToyBoxB & boxB) const{
  //**************************************************

  // Cast the ToyBox to the appropriate type
  ToyBoxNDUP& box = *dynamic_cast<ToyBoxNDUP*>(&boxB);
  AnaEventB& eventB = *static_cast<AnaEventB*>(&event);
    AnaTrueVertex* vertex = static_cast<AnaTrueVertex*>(box.Vertex->TrueVertex);

  return anaUtils::GetReaction(*vertex, SubDetId::kTarget1,0);

}

//**************************************************
bool TotalMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  (void)box;

  // Check we have at least one reconstructed track in the FGD
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
  return (EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1]>0);

}

//**************************************************
bool SortTracksAction::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  // Retrieve the EventBoxNDUP
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
  //Find TPCGoodQuality tracks in Fwd and Bwd
  int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCAndTarget1];
  for (Int_t i=0;i<nTPC; ++i){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCAndTarget1][i]);
    if ( track->Charge!=-1 ) continue;
 		
    cc4pibox->LowAngle.push_back(track);
  }
  //Sort TPCGoodQuality using Momentum
  std::sort(cc4pibox->LowAngle.begin(), cc4pibox->LowAngle.end(), numuCC4pi_utils::HGlobalMomFirst);

  int nECALTracks=EventBox->nRecObjectsInGroup[EventBox::kTracksWithECal];
  for(int i=0;i<nECALTracks;i++){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithECal][i]);
      cc4pibox->HighAngle.push_back(track);
   // }
  }
  //Sort HighAngle using RangeMomentum
  std::sort(cc4pibox->HighAngle.begin(), cc4pibox->HighAngle.end(), numuCC4pi_utils::HGlobalMomFirst);
  if(cc4pibox->LowAngle.size()>0){
  cc4pibox->MainTrack=cc4pibox->LowAngle[0];
  return true;}
  return false;

}

//**************************************************
bool TrackGQandFVCut::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  for(UInt_t i=0;i<cc4pibox->LowAngle.size();i++ ){
    if ( anaUtils::InFiducialVolume(SubDetId::kTarget1, cc4pibox->LowAngle[i]->PositionStart, LAFVmin, LAFVmax) ){
      if (cutUtils::DeltaLYZTPCCut(*cc4pibox->LowAngle[i])) {

         cc4pibox->LowAngleGQ.push_back(cc4pibox->LowAngle[i]);
      }
    }
  }

  for(UInt_t i=0;i<cc4pibox->HighAngle.size();i++){
    if ( anaUtils::InFiducialVolume(SubDetId::kTarget1, cc4pibox->HighAngle[i]->PositionStart, HAFVmin, HAFVmax) ) {
          if (cutUtils::DeltaLYZTPCCut(*cc4pibox->HighAngle[i])) {
              cc4pibox->HighAngleGQ.push_back(cc4pibox->HighAngle[i]);
          }
    }
  }
  if (!cc4pibox->MainTrack) return false;

  if ( anaUtils::InFiducialVolume(SubDetId::kTarget1, cc4pibox->MainTrack->PositionStart, LAFVmin, LAFVmax) ) {
    if (cutUtils::DeltaLYZTPCCut(*cc4pibox->MainTrack)) {
      return true;
    }
  }
	
  return false;

}


//Define vertex
//**************************************************
bool FindVertexAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //**************************************************

  (void)event;

  ToyBoxNDUP& box = *static_cast<ToyBoxNDUP*>(&boxB);
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&boxB);

  // reset the vertex
  box.Vertex = NULL;
  box.TrueVertex = NULL;

  if ( !cc4pibox->MainTrack ) return false;
  box.MainTrack = cc4pibox->MainTrack;

  box.Vertex = new AnaVertexB();
  anaUtils::CreateArray(box.Vertex->Particles, 1);

  box.Vertex->nParticles = 0;
  box.Vertex->Particles[box.Vertex->nParticles++] = box.MainTrack;

  for(int i = 0; i < 4; ++i) box.Vertex->Position[i] = box.MainTrack->PositionStart[i];

  if ( box.MainTrack->GetTrueParticle() ) box.TrueVertex = box.Vertex->TrueVertex = cc4pibox->MainTrack->GetTrueParticle()->TrueVertex;

  return true;

}

//********************************************************************
bool FillSummaryAction_numuCC4pi::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //********************************************************************

  // Cast the ToyBox to the appropriate type
  ToyBoxNDUP& box = *static_cast<ToyBoxNDUP*>(&boxB);

  if(!box.MainTrack) return 1;

  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kTarget1NuMuCC] = box.MainTrack;

  for(int i = 0; i < 4; ++i) static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kTarget1NuMuCC][i] = box.MainTrack->PositionStart[i];

  if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget1NuMuCC] = box.MainTrack->GetTrueParticle()->TrueVertex;

  return true;

}

//**************************************************
bool Fwd_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  for (UInt_t i = 0; i < cc4pibox->LowAngleGQ.size(); i++ ) {

    if ( numuCC4pi_utils::IsForward(*cc4pibox->LowAngleGQ[i]) ) {
      cc4pibox->FwdTracks.push_back(cc4pibox->LowAngleGQ[i]);
    }else{
      cc4pibox->BwdTracks.push_back(cc4pibox->LowAngleGQ[i]);

    }
  }
  for(UInt_t i=0;i<cc4pibox->HighAngleGQ.size();i++){
    if ( numuCC4pi_utils::IsForward(*cc4pibox->HighAngleGQ[i]) ) {
      cc4pibox->HAFwdTracks.push_back(cc4pibox->HighAngleGQ[i]);
    }else{
      cc4pibox->HABwdTracks.push_back(cc4pibox->HighAngleGQ[i]);

    }
  }
      if (!cc4pibox->MainTrack) return false;

    if ( numuCC4pi_utils::IsForward(*cc4pibox->MainTrack) ) {
      return true;
  }  
  return false;
}
//**************************************************
bool Fwd_PID::Apply(AnaEventC& event, ToyBoxB& box) const {
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  for (UInt_t i=0;i<cc4pibox->FwdTracks.size();i++){
    if ( numuCC4pi_utils::PIDCut(0, *(cc4pibox->FwdTracks[i]),   _file_ECAL_PDF)==1 ) cc4pibox->FwdTracks_PID.push_back(cc4pibox->FwdTracks[i]);
  }

  for (UInt_t i=0;i<cc4pibox->BwdTracks.size();i++){
    if ( numuCC4pi_utils::PIDCut(1, *(cc4pibox->BwdTracks[i]),   _file_ECAL_PDF)==1 ) cc4pibox->BwdTracks_PID.push_back(cc4pibox->BwdTracks[i]);
  }

  for (UInt_t i=0;i<cc4pibox->HAFwdTracks.size();i++){
    if ( numuCC4pi_utils::PIDCut(2, *(cc4pibox->HAFwdTracks[i]), _file_ECAL_PDF)==1 ) cc4pibox->HAFwdTracks_PID.push_back(cc4pibox->HAFwdTracks[i]);
  }

  for (UInt_t i=0;i<cc4pibox->HABwdTracks.size();i++){
    if ( numuCC4pi_utils::PIDCut(3, *(cc4pibox->HABwdTracks[i]), _file_ECAL_PDF)==1 ) cc4pibox->HABwdTracks_PID.push_back(cc4pibox->HABwdTracks[i]);
  }
      if (!cc4pibox->MainTrack) return false;

  if (numuCC4pi_utils::PIDCut(0, *cc4pibox->MainTrack,   _file_ECAL_PDF) == 1 ) {
    return true;
  }
  return false;
}

//**************************************************
bool Bwd_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  for (UInt_t i = 0; i < cc4pibox->LowAngleGQ.size(); i++ ) {

    if ( numuCC4pi_utils::IsForward(*cc4pibox->LowAngleGQ[i]) ) {
      cc4pibox->FwdTracks.push_back(cc4pibox->LowAngleGQ[i]);
    }else{
      cc4pibox->BwdTracks.push_back(cc4pibox->LowAngleGQ[i]);

    }
  }
  for(UInt_t i=0;i<cc4pibox->HighAngleGQ.size();i++){
    if ( numuCC4pi_utils::IsForward(*cc4pibox->HighAngleGQ[i]) ) {
      cc4pibox->HAFwdTracks.push_back(cc4pibox->HighAngleGQ[i]);
    }else{
      cc4pibox->HABwdTracks.push_back(cc4pibox->HighAngleGQ[i]);

    }
  }
      if (!cc4pibox->MainTrack) return false;

    if ( !numuCC4pi_utils::IsForward(*cc4pibox->MainTrack) ) {
      return true;
  }  
  return false;
}
//**************************************************
bool Bwd_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  for (UInt_t i=0;i<cc4pibox->FwdTracks.size();i++){
    if ( numuCC4pi_utils::PIDCut(0, *(cc4pibox->FwdTracks[i]),   _file_ECAL_PDF)==1 ) cc4pibox->FwdTracks_PID.push_back(cc4pibox->FwdTracks[i]);
  }

  for (UInt_t i=0;i<cc4pibox->BwdTracks.size();i++){
    if ( numuCC4pi_utils::PIDCut(1, *(cc4pibox->BwdTracks[i]),   _file_ECAL_PDF)==1 ) cc4pibox->BwdTracks_PID.push_back(cc4pibox->BwdTracks[i]);
  }

  for (UInt_t i=0;i<cc4pibox->HAFwdTracks.size();i++){
    if ( numuCC4pi_utils::PIDCut(2, *(cc4pibox->HAFwdTracks[i]), _file_ECAL_PDF)==1 ) cc4pibox->HAFwdTracks_PID.push_back(cc4pibox->HAFwdTracks[i]);
  }

  for (UInt_t i=0;i<cc4pibox->HABwdTracks.size();i++){
    if ( numuCC4pi_utils::PIDCut(3, *(cc4pibox->HABwdTracks[i]), _file_ECAL_PDF)==1 ) cc4pibox->HABwdTracks_PID.push_back(cc4pibox->HABwdTracks[i]);
  }
      if (!cc4pibox->MainTrack) return false;

  if (numuCC4pi_utils::PIDCut(1, *cc4pibox->MainTrack,   _file_ECAL_PDF) == 1 ) {
    return true;
  }
  return false;
}

}*/
//**************************************************
bool numuCC4piFwdCanSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
  //**************************************************
	
  (void)event;
  (void)box;

  if(systId == SystId::kPileUp){
    if (branch==1) return false;
  }

  return true;

}

//**************************************************
bool numuCC4piFwdCanSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

  if (!track) return false;
  if (!track->TrueObject) return false;

  return true;

}

//**************************************************
bool numuCC4piFwdCanSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************
	
  (void)event;
  (void)branch;

  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);

  if (!trueTrack) return false;

  return true;

}

//**************************************************
bool numuCC4piFwdCanSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;
  /*
    const ToyBoxCC4pi& cc4pibox = *static_cast<const ToyBoxCC4pi*>(&boxB); 

    AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

    if(systId == SystId::kOOFV){
    if (branch==1 || branch==3) return false;
    }
    if(systId == SystId::kTpcClusterEff){
    if (track->nTPCSegments > 0) {
    AnaTPCParticleB* tpcTrack = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track));
    if (tpcTrack) {
    if (tpcTrack->NNodes > 16 && tpcTrack->NNodes < 19) return true;
    }
    }
    return false;
    }
    if(systId == SystId::kChargeIDEff){
    if (track == cc4pibox.MainTrack) return true;
    return false;
    }
    if(systId == SystId::kTpcFgdMatchEff){
    if (track == cc4pibox.MainTrack) return true;
    return false;
    }
    if(systId == SystId::kECalPID){
    if (track == cc4pibox.MainTrack) {
    if (branch==2 || branch==3) return true;
    else if (branch==0 || branch==4 || branch==5) {
    if ( anaUtils::TrackUsesDet(*track,SubDetId::kTECAL) ) return true;
    if ( anaUtils::InFiducialVolume(SubDetId::kDSECAL, track->PositionEnd, _FVdefminDsECal, _FVdefmaxDsECal) ) return true;
    }
    }
    return false;
    }

  */	return true;

}

//**************************************************
bool numuCC4piFwdCanSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;
  /*
    const ToyBoxCC4pi& cc4pibox = *static_cast<const ToyBoxCC4pi*>(&boxB);

    AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);

    if ( !cc4pibox.MainTrack->GetTrueParticle() ) return false;

    if(systId == SystId::kTpcTrackEff){
    if (trueTrack->PDG == 13 && cc4pibox.MainTrack->GetTrueParticle()->PDG!=13) return true;
    if (branch!=2 && branch!=3) { if (trueTrack->ID  == cc4pibox.MainTrack->GetTrueParticle()->ID) return true; }
    return false;
    }
    if(systId == SystId::kECalTrackEff){
    if (trueTrack->PDG == 13 && cc4pibox.MainTrack->GetTrueParticle()->PDG!=13) return true;
    if (branch==2 || branch==3) { 
    if (cc4pibox.MainTrack->nTPCSegments == 0) {
    if (trueTrack->ID  == cc4pibox.MainTrack->GetTrueParticle()->ID) return true; 
    }
    }
    return false;
    }
    if(systId == SystId::kTpcECalMatchEff){
    if (branch>-1 && branch<6) { if (trueTrack->ID  == cc4pibox.MainTrack->GetTrueParticle()->ID) return true; }
    return false;
    }
    if(systId == SystId::kTpcP0dMatchEff){
    if (branch==1) { if (trueTrack->ID  == cc4pibox.MainTrack->GetTrueParticle()->ID) return true; }
    return false;
    }
    if(systId == SystId::kFgdECalMatchEff){
    if (branch==2 || branch==3) { if (trueTrack->ID  == cc4pibox.MainTrack->GetTrueParticle()->ID) return true; }
    return false;
    }
    if(systId == SystId::kFgdECalSmrdMatchEff){
    if (branch==2 || branch==3) { if (trueTrack->ID  == cc4pibox.MainTrack->GetTrueParticle()->ID) return true; }
    return false;
    }
    if(systId == SystId::kSIPion){
    // If this trueTrack is associated to the MainTrack
    if (trueTrack->ID == cc4pibox.MainTrack->GetTrueParticle()->ID){
    if (abs(cc4pibox.MainTrack->GetTrueParticle()->PDG)        == 211) return true;
    if (abs(cc4pibox.MainTrack->GetTrueParticle()->ParentPDG)  == 211) return true;
    if (abs(cc4pibox.MainTrack->GetTrueParticle()->GParentPDG) == 211) return true;
    }
    // If this trueTrack is NOT associated to the MainTrack, consider the posibility of this trueTrack to become the MainTrack and be identified as muon
    // For the moment assume a negative pion may become the MainTrack if its momentum its above 90% of the current MainTrack momentum. 
    // Ideally we should check that this true pion is not associated to any reconstructed track, but this is not possible now without looping.
    // Multiply by the charge of the MainTrack such that it can be use for antiNumu selection
    if (trueTrack->PDG == 211*((Int_t)cc4pibox.MainTrack->Charge) && trueTrack->Momentum > 0.9*cc4pibox.MainTrack->Momentum) return true;
    return false;
    }
    if(systId == SystId::kSIProton){
    // If this trueTrack is associated to the MainTrack
    if (trueTrack->ID == cc4pibox.MainTrack->GetTrueParticle()->ID){
    if (cc4pibox.MainTrack->GetTrueParticle()->PDG        == 2212) return true;
    if (cc4pibox.MainTrack->GetTrueParticle()->ParentPDG  == 2212) return true;
    if (cc4pibox.MainTrack->GetTrueParticle()->GParentPDG == 2212) return true;
    }
    return false;
    }
  */
  return true;

}

//********************************************************************
bool numuCC4piFwdCanSelection::CheckRedoSelection(const AnaEventC& eventBB, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
  //********************************************************************

  (void)eventBB;
  (void)PreviousToyBox;
  (void)redoFromStep;

  return true;

}
