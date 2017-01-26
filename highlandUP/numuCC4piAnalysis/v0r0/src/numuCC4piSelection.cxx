#include "numuCC4piSelection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "EventBox.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "SelectionUtils.hxx"
#include "numuCC4piUtils.hxx"
#include "Parameters.hxx"

//********************************************************************
numuCC4piSelection::numuCC4piSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxNDUP) {
  //********************************************************************

  char filename[256];

  sprintf(filename, "%s/data/ECAL_PDF.root", getenv("NUMUCC4PIANALYSISROOT"));
  _file_ECAL_PDF = TFile::Open(filename);

}

//********************************************************************
void numuCC4piSelection::DefineSteps(){
  //********************************************************************
  int branch=ND::params().GetParameterI("numuCC4piAnalysis.Branch");
  //last true means that if that cut is not fulfill the sequence is stop
  AddStep(StepBase::kAction,"find true vertex",   new FindTrueVertexAction());
  AddStep(StepBase::kCut," true vertex in target  ",   new TrueVertexInTargetCut(), true);

  AddStep(StepBase::kCut,    "> 0 tracks ",         new TotalMultiplicityCut(), true); //if passed accum_level=2 
  AddStep(StepBase::kCut, "Sort TPC tracks",     new SortTracksAction());
  AddStep(StepBase::kCut,    "quality+fiducial",    new TrackGQandFVCut(),      true); //if passed accum_level=3
  //	AddStep(StepBase::kAction, "veto Action",         new VetoAction());
  AddStep(StepBase::kAction, "muon PID Action",     new PIDAction(_file_ECAL_PDF));
  AddStep(StepBase::kAction, "find vertex",         new FindVertexAction());
  AddStep(StepBase::kAction, "fill summary",        new FillSummaryAction_numuCC4pi());

  //AddSplit(4);
  if(branch==0){
    AddStep( StepBase::kCut, "Fwd Quality Cut",     new Fwd_Quality());         //if passed accum_level=4
    //AddStep(0, StepBase::kCut, "Fwd Veto Cut",        new Fwd_Veto());            //if passed accum_level=5

    //AddSplit(3,0);

    AddStep(StepBase::kCut, "Fwd PID Cut",      new Fwd_PID());             //if passed accum_level=6
    AddStep( StepBase::kCut, "Fwd 4pi Cut",      new Fwd_4pi());
    SetBranchAlias(0, "Fwd");

  }             //if passed accum_level=7
  //	AddStep(0, 1, StepBase::kCut, "CSFGD2 PID Cut",   new CSFGD2_PID());
  //	AddStep(0, 1, StepBase::kCut, "CSFGD2 4pi Cut",   new CSFGD2_4pi());
  //	AddStep(0, 2, StepBase::kCut, "CSECAL PID Cut",   new CSECAL_PID());
  //	AddStep(0, 2, StepBase::kCut, "CSECAL 4pi Cut",   new CSECAL_4pi());
  else if(branch==1){

    AddStep( StepBase::kCut, "Bwd Quality Cut",    new Bwd_Quality());
    //	AddStep(1, StepBase::kCut, "Bwd Veto Cut",       new Bwd_Veto());
    AddStep( StepBase::kCut, "Bwd PID Cut",        new Bwd_PID());
    AddStep( StepBase::kCut, "Bwd 4pi Cut",        new Bwd_4pi());
    SetBranchAlias(0, "Bwd");

  }
  else if(branch==2){
    AddStep( StepBase::kCut, "HAFwd Quality Cut",  new HAFwd_Quality());
    //	AddStep(2, StepBase::kCut, "HAFwd Veto Cut",     new HAFwd_Veto());
    AddStep( StepBase::kCut, "HAFwd PID Cut",      new HAFwd_PID());
    AddStep( StepBase::kCut, "HAFwd 4pi Cut",      new HAFwd_4pi());
    SetBranchAlias(0, "HaFwd");

  }else if(branch==3){
    AddStep( StepBase::kCut, "HABwd Quality Cut",  new HABwd_Quality());
    //	AddStep(3, StepBase::kCut, "HABwd Veto Cut",     new HABwd_Veto());
    AddStep( StepBase::kCut, "HABwd PID Cut",      new HABwd_PID());
    AddStep( StepBase::kCut, "HABwd 4pi Cut",      new HABwd_4pi());
    SetBranchAlias(0, "HaBwd");

  }

  //SetBranchAlias(1, "Bwd",    1);
  //SetBranchAlias(2, "HAFwd",  2);
  //	SetBranchAlias(3, "HABwd",  3);
  //	SetBranchAlias(4, "CSFGD2", 0, 1);
  //	SetBranchAlias(5, "CSECAL", 0, 2);

  //if first two cuts are not fulfill dont throw toys
  SetPreSelectionAccumLevel(2);

}

//********************************************************************
void numuCC4piSelection::DefineDetectorFV(){
  //********************************************************************

  // Change FV definition to take all thickness
  // Note! this will affect all downstream stuff
  //FVDef::FVdefminTarget1[2] = 0;
  //FVDef::FVdefmaxTarget1[2] = 0;

  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kTarget1);

}

//**************************************************
void numuCC4piSelection::InitializeEvent(AnaEventC& eventBB){
  //**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB);

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxNDUP]) event.EventBoxes[EventBoxId::kEventBoxNDUP] = new EventBox();

  boxUtils::FillTracksWithTPC(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithTarget(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));

  boxUtils::FillTracksWithECal(event);
  //boxUtils::FillTrajsChargedInTPC(event);
  //	boxUtils::FillTrajsChargedInFGDAndNoTPC(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  //boxUtils::FillTrajsChargedHATracker(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));

}

//********************************************************************
bool numuCC4piSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
  //********************************************************************

  if(allCutsPassed[0]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[1]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[2]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[3]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[4]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[5]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;

  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kTarget1NuMuCC);
}
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
  return cutUtils::FiducialCut(box.Vertex->Position, SubDetId::kTarget1);

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
  int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV];
  for (Int_t i=0;i<nTPC; ++i){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV][i]);
    if ( track->Charge!=-1 ) continue;
    if ( SubDetId::GetDetectorUsed(track->Detector,SubDetId::kTPCUp1) || SubDetId::GetDetectorUsed(track->Detector,SubDetId::kTPCDown1)) continue; 
		
    cc4pibox->LowAngle.push_back(track);
  }
  //Sort TPCGoodQuality using Momentum
  std::sort(cc4pibox->LowAngle.begin(), cc4pibox->LowAngle.end(), numuCC4pi_utils::HGlobalMomFirst);

  int nECALTracks=EventBox->nRecObjectsInGroup[EventBox::kTracksWithECal];
  for(int i=0;i<nECALTracks;i++){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithECal][i]);
    if ( anaUtils::InFiducialVolume(SubDetId::kTarget1, track->PositionStart) ) {
      if ( !cutUtils::TrackQualityCut(*track) ) continue;
      if ( SubDetId::GetDetectorUsed(track->Detector,SubDetId::kDsECal)) continue; 
      cc4pibox->HighAngle.push_back(track);
    }
  }
  //Sort HighAngle using RangeMomentum
  std::sort(cc4pibox->HighAngle.begin(), cc4pibox->HighAngle.end(), numuCC4pi_utils::HGlobalMomFirst);
  return true;

}

//**************************************************
bool TrackGQandFVCut::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  for(UInt_t i=0;i<cc4pibox->LowAngle.size();i++ ){
    if ( anaUtils::InFiducialVolume(SubDetId::kTarget1, cc4pibox->LowAngle[i]->PositionStart, LAFVmin, LAFVmax) ){
      if ( numuCC4pi_utils::IsForward(*cc4pibox->LowAngle[i]) ) {cc4pibox->FwdTracks.push_back(cc4pibox->LowAngle[i]);
	//	std::cout<<"cos "<<(*cc4pibox->LowAngle[i]).DirectionStart[2]<<"cc4pibox->LowAngle[i] "<<(*cc4pibox->LowAngle[i]).PositionStart[2]<<" "<<(*cc4pibox->LowAngle[i]).PositionEnd[2]<<" "<<(*cc4pibox->LowAngle[i]).PositionStart[0]<<" "<<(*cc4pibox->LowAngle[i]).PositionEnd[0]<<" "<<(*cc4pibox->LowAngle[i]).PositionStart[1]<<" "<<(*cc4pibox->LowAngle[i]).PositionEnd[1]<<std::endl;

      }
      else cc4pibox->BwdTracks.push_back(cc4pibox->LowAngle[i]);
      break;
    }
  }

  for(UInt_t i=0;i<cc4pibox->HighAngle.size();i++){
    if ( anaUtils::InFiducialVolume(SubDetId::kTarget1, cc4pibox->HighAngle[i]->PositionStart, HAFVmin, HAFVmax) ) {
      if ( numuCC4pi_utils::IsForward(*cc4pibox->HighAngle[i]) ) cc4pibox->HAFwdTracks.push_back(cc4pibox->HighAngle[i]);
      else cc4pibox->HABwdTracks.push_back(cc4pibox->HighAngle[i]);
    }
  }

  if      ( cc4pibox->FwdTracks.size()>0   ) { cc4pibox->MainTrack = cc4pibox->FwdTracks[0];   return true; }
  else if ( cc4pibox->BwdTracks.size()>0   ) { cc4pibox->MainTrack = cc4pibox->BwdTracks[0];   return true; }
  else if ( cc4pibox->HAFwdTracks.size()>0 ) { cc4pibox->MainTrack = cc4pibox->HAFwdTracks[0]; return true; }
  else if ( cc4pibox->HABwdTracks.size()>0 ) { cc4pibox->MainTrack = cc4pibox->HABwdTracks[0]; return true; }
	
  return false;

}

//**************************************************
bool VetoAction::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  AnaEventB& eventB = *static_cast<AnaEventB*>(&event); 

  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  for (UInt_t i=0;i<cc4pibox->FwdTracks.size();i++){
    //	if ( numuCC4pi_utils::VetoCut(0,eventB,*(cc4pibox->FwdTracks[i])) ) {
    /*if ( numuCC4pi_utils::ExternalCut(0,event,*(cc4pibox->FwdTracks[i])) ) */cc4pibox->FwdTracks.push_back(cc4pibox->FwdTracks[i]);
    //	}
  }

  for (UInt_t i=0;i<cc4pibox->BwdTracks.size();i++){
    cc4pibox->BwdTracks.push_back(cc4pibox->BwdTracks[i]);
  }

  for (UInt_t i=0;i<cc4pibox->HAFwdTracks.size();i++){
    /*if ( numuCC4pi_utils::VetoCut(2,eventB,*(cc4pibox->HAFwdTracks[i])) ) */cc4pibox->HAFwdTracks.push_back(cc4pibox->HAFwdTracks[i]);
  }

  for (UInt_t i=0;i<cc4pibox->HABwdTracks.size();i++){
    /*if ( numuCC4pi_utils::VetoCut(3,eventB,*(cc4pibox->HABwdTracks[i])) )*/ cc4pibox->HABwdTracks.push_back(cc4pibox->HABwdTracks[i]);
  }

  if      ( cc4pibox->FwdTracks.size()>0   ) cc4pibox->MainTrack = cc4pibox->FwdTracks[0];
  else if ( cc4pibox->BwdTracks.size()>0   ) cc4pibox->MainTrack = cc4pibox->BwdTracks[0];
  else if ( cc4pibox->HAFwdTracks.size()>0 ) cc4pibox->MainTrack = cc4pibox->HAFwdTracks[0];
  else if ( cc4pibox->HABwdTracks.size()>0 ) cc4pibox->MainTrack = cc4pibox->HABwdTracks[0];

  return true;

}

//**************************************************
bool PIDAction::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  (void)event;

  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  for (UInt_t i=0;i<cc4pibox->FwdTracks.size();i++){
    if ( numuCC4pi_utils::PIDCut(0,*(cc4pibox->FwdTracks[i]))==1 ) cc4pibox->FwdTracks_PID.push_back(cc4pibox->FwdTracks[i]);
  }

  for (UInt_t i=0;i<cc4pibox->BwdTracks.size();i++){
    if ( numuCC4pi_utils::PIDCut(1,*(cc4pibox->BwdTracks[i]))==1 ) cc4pibox->BwdTracks_PID.push_back(cc4pibox->BwdTracks[i]);
  }

  for (UInt_t i=0;i<cc4pibox->HAFwdTracks.size();i++){
    if ( numuCC4pi_utils::PIDCut(2,*(cc4pibox->HAFwdTracks[i]))==1 ){ cc4pibox->HAFwdTracks_PID.push_back(cc4pibox->HAFwdTracks[i]);}
  }

  for (UInt_t i=0;i<cc4pibox->HABwdTracks.size();i++){
    if ( numuCC4pi_utils::PIDCut(3,*(cc4pibox->HABwdTracks[i]))==1 ) cc4pibox->HABwdTracks_PID.push_back(cc4pibox->HABwdTracks[i]);
  }

  //for (UInt_t i=0;i<cc4pibox->FwdTracks.size();i++){
  //	if ( numuCC4pi_utils::PIDCut(0,*(cc4pibox->FwdTracks[i]))==2 ) cc4pibox->CSFD2Tracks_PID.push_back(cc4pibox->FwdTracks[i]);
  ///	}
  //
  //for (UInt_t i=0;i<cc4pibox->FwdTracks.size();i++){
  //		if ( numuCC4pi_utils::PIDCut(0,*(cc4pibox->FwdTracks[i]))==3 ) cc4pibox->CSECALTracks_PID.push_back(cc4pibox->FwdTracks[i]);
  //	}

  if      ( cc4pibox->FwdTracks_PID.size()>0    ) cc4pibox->MainTrack = cc4pibox->FwdTracks_PID[0];
  else if ( cc4pibox->BwdTracks_PID.size()>0    ) cc4pibox->MainTrack = cc4pibox->BwdTracks_PID[0];
  else if ( cc4pibox->HAFwdTracks_PID.size()>0  ) cc4pibox->MainTrack = cc4pibox->HAFwdTracks_PID[0];
  else if ( cc4pibox->HABwdTracks_PID.size()>0  ) cc4pibox->MainTrack = cc4pibox->HABwdTracks_PID[0];
  //else if ( cc4pibox->CSFD2Tracks_PID.size()>0  ) cc4pibox->MainTrack = cc4pibox->CSFD2Tracks_PID[0];
  //else if ( cc4pibox->CSECALTracks_PID.size()>0 ) cc4pibox->MainTrack = cc4pibox->CSECALTracks_PID[0];

  return true;

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
  if (cc4pibox->FwdTracks.size()>0) return true;
  return false;
}
/*//**************************************************
  bool Fwd_Veto::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->FwdTracks.size()>0) return true;
  return false;
  }*/
//**************************************************
bool Fwd_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->FwdTracks_PID.size()>0) return true;
  return false;
}
//**************************************************
bool Fwd_4pi::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->FwdTracks_PID.size()>0) return true;
  return false;
}

//**************************************************
bool Bwd_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->BwdTracks.size()>0) return true;
  return false;
}/*
 //**************************************************
 bool Bwd_Veto::Apply(AnaEventC& event, ToyBoxB& box) const{
 //**************************************************
 (void)event;
 ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
 if (cc4pibox->BwdTracks.size()>0) return true;
 return false;
 }*/
//**************************************************
bool Bwd_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->BwdTracks_PID.size()>0) return true;
  return false;
}
//**************************************************
bool Bwd_4pi::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->BwdTracks_PID.size()>0 && cc4pibox->FwdTracks_PID.size()==0) return true;
  return false;
}

//**************************************************
bool HAFwd_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HAFwdTracks.size()>0) return true;
  return false;
}/*
 //**************************************************
 bool HAFwd_Veto::Apply(AnaEventC& event, ToyBoxB& box) const{
 //**************************************************
 (void)event;
 ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
 if (cc4pibox->HAFwdTracks.size()>0) return true;
 return false;
 }*/
//**************************************************
bool HAFwd_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HAFwdTracks_PID.size()>0) return true;
  return false;
}
//**************************************************
bool HAFwd_4pi::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HAFwdTracks_PID.size()>0 /*&& cc4pibox->FwdTracks_PID.size()==0*/ && cc4pibox->BwdTracks_PID.size()==0) return true;
  return false;
}

//**************************************************
bool HABwd_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HABwdTracks.size()>0) return true;
  return false;
}/*
 //**************************************************
 bool HABwd_Veto::Apply(AnaEventC& event, ToyBoxB& box) const{
 //**************************************************
 (void)event;
 ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
 if (cc4pibox->HABwdTracks.size()>0) return true;
 return false;
 }*/
//**************************************************
bool HABwd_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HABwdTracks_PID.size()>0) return true;
  return false;
}
//**************************************************
bool HABwd_4pi::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HABwdTracks_PID.size()>0 && cc4pibox->FwdTracks_PID.size()==0 && cc4pibox->BwdTracks_PID.size()==0 && cc4pibox->HAFwdTracks_PID.size()==0) return true;
  return false;
}
/*
//**************************************************
bool CSFGD2_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
//**************************************************
(void)event;
ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box); 
if (cc4pibox->CSFD2Tracks_PID.size()>0) return true;
return false;
}
//**************************************************
bool CSFGD2_4pi::Apply(AnaEventC& event, ToyBoxB& box) const{
//**************************************************
(void)event;
ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box); 
if (cc4pibox->CSFD2Tracks_PID.size()>0 && cc4pibox->FwdTracks_PID.size()==0 && cc4pibox->BwdTracks_PID.size()==0 && cc4pibox->HAFwdTracks_PID.size()==0 && cc4pibox->HABwdTracks_PID.size()==0) return true;
return false;
}

//**************************************************
bool CSECAL_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
//**************************************************
(void)event;
ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box); 
if (cc4pibox->CSECALTracks_PID.size()>0) return true;
return false;
}
//**************************************************
bool CSECAL_4pi::Apply(AnaEventC& event, ToyBoxB& box) const{
//**************************************************
(void)event;
ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box); 
if (cc4pibox->CSECALTracks_PID.size()>0 && cc4pibox->FwdTracks_PID.size()==0 && cc4pibox->BwdTracks_PID.size()==0 && cc4pibox->HAFwdTracks_PID.size()==0 && cc4pibox->HABwdTracks_PID.size()==0 && cc4pibox->CSFD2Tracks_PID.size()==0) return true;
return false;
}
*/
//**************************************************
bool numuCC4piSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
  //**************************************************
	
  (void)event;
  (void)box;

  if(systId == SystId::kPileUp){
    if (branch==1) return false;
  }

  return true;

}

//**************************************************
bool numuCC4piSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

  if (!track) return false;
  if (!track->TrueObject) return false;

  return true;

}

//**************************************************
bool numuCC4piSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************
	
  (void)event;
  (void)branch;

  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);

  if (!trueTrack) return false;

  return true;

}

//**************************************************
bool numuCC4piSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
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
bool numuCC4piSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
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
bool numuCC4piSelection::CheckRedoSelection(const AnaEventC& eventBB, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
  //********************************************************************

  (void)eventBB;
  (void)PreviousToyBox;
  (void)redoFromStep;

  return true;

}
