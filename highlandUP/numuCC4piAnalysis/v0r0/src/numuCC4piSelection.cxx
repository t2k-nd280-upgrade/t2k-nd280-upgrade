#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "EventBox.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "SelectionUtils.hxx"
#include "AnaRecPackManager.hxx"

#include "numuCC4piSelection.hxx"
#include "numuCC4piUtils.hxx"

#include "TH2F.h"

#include <utility>
#include <map>

//********************************************************************
numuCC4piSelection::numuCC4piSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxNDUP) {
  //********************************************************************
	
  char filename[256];
  sprintf(filename, "%s/data/pdfs_dsecal.root", getenv("NUMUCC4PIANALYSISROOT"));
  _file_ECAL_PDF = TFile::Open(filename);
	
	
}

//********************************************************************
void numuCC4piSelection::DefineSteps(){
  //********************************************************************
	
	
  //last true means that if that cut is not fulfill the sequence is stop
  AddStep(StepBase::kAction, "find true vertex",      new numuCC4piUtils::FindTrueVertexAction());
  AddStep(StepBase::kCut,    "true vertex in target", new numuCC4piUtils::TrueVertexInTargetCut(), true);
	
  AddStep(StepBase::kCut,    "> 0 tracks ",         new numuCC4piUtils::TotalMultiplicityCut(), true);
  AddStep(StepBase::kAction, "Sort TPC tracks",     new numuCC4piUtils::SortTracksAction());
  AddStep(StepBase::kCut,    "find the muon",       new numuCC4piUtils::FindMuonCut(_file_ECAL_PDF));
  AddStep(StepBase::kCut,    "quality+fiducial",    new numuCC4piUtils::TrackGQandFVCut(),      true);
	
  AddStep(StepBase::kAction, "find vertex",         new numuCC4piUtils::FindVertexAction());
  AddStep(StepBase::kAction, "fill summary",        new numuCC4piUtils::FillSummaryAction_numuCC4pi());
  AddStep(StepBase::kAction, "find pions",          new numuCC4piUtils::FindPionsAction());
	
  AddSplit(8);
	
  AddStep(0, StepBase::kCut, "CC0pi topology",    new numuCC4piUtils::CC0pi());
  AddStep(1, StepBase::kCut, "CC1pi topology",    new numuCC4piUtils::CC1pi());
  AddStep(2, StepBase::kCut, "CCoth topology",    new numuCC4piUtils::CCoth());
	
  AddStep(3, StepBase::kCut, "CC-inc FWD",        new numuCC4piUtils::FWD());
  AddStep(4, StepBase::kCut, "CC-inc BWD",        new numuCC4piUtils::BWD());
  AddStep(5, StepBase::kCut, "CC-inc HA",         new numuCC4piUtils::HA());
  AddStep(6, StepBase::kCut, "CC-inc Target",     new numuCC4piUtils::Target());
  AddStep(7, StepBase::kCut, "CC-inc ECal",       new numuCC4piUtils::ECal());
	
  SetBranchAlias(0, "CC0pi", 0);
  SetBranchAlias(1, "CC1pi", 1);
  SetBranchAlias(2, "CCoth", 2);
	
  SetBranchAlias(3, "CC-inc FWD", 3);
  SetBranchAlias(4, "CC-inc BWD", 4);	
  SetBranchAlias(5, "CC-inc HA", 5);	
  SetBranchAlias(6, "CC-inc Target", 6);	
  SetBranchAlias(7, "CC-inc ECal", 7);
	
  SetPreSelectionAccumLevel(0);
	
}

//********************************************************************
void numuCC4piSelection::DefineDetectorFV(){
  //********************************************************************
	
  // Change FV definition to take all thickness
  // Note! this will affect all downstream stuff
  //FVDef::FVdefminTarget1[2] = 0;
  //FVDef::FVdefmaxTarget1[2] = 0;
	
  // The detector in which the selection is applied
  bool useTarget1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD2");
	
  if ( useTarget1 && !useTarget2) SetDetectorFV(SubDetId::kTarget1);
  if (!useTarget1 &&  useTarget2) SetDetectorFV(SubDetId::kTarget2);
  if ( useTarget1 &&  useTarget2) SetDetectorFV(SubDetId::kTarget);
	
  if ( useFGD1 && !useFGD2) SetDetectorFV(SubDetId::kFGD1);
  if (!useFGD1 &&  useFGD2) SetDetectorFV(SubDetId::kFGD2);
  if ( useFGD1 &&  useFGD2) SetDetectorFV(SubDetId::kFGD);
	
}

//**************************************************
void numuCC4piSelection::InitializeEvent(AnaEventC& eventBB){
  //**************************************************
	
  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB);
	
  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxNDUP]) event.EventBoxes[EventBoxId::kEventBoxNDUP] = new EventBox();
	
  boxUtils::FillTracksWithTPC(event,    static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithTarget(event, static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithFGD(event,    static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithECal(event);
	
}

//********************************************************************
bool numuCC4piSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
  //********************************************************************
	
  if(allCutsPassed[0]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[1]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[2]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[3]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kTarget1NuMuCC);
	
}

namespace numuCC4piUtils{
	
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
		
    (void)event;
		
    // Cast the ToyBox to the appropriate type
    ToyBoxNDUP& box = *dynamic_cast<ToyBoxNDUP*>(&boxB);
		
    return ( (useTarget1 && cutUtils::FiducialCut(box.Vertex->Position, SubDetId::kTarget1)) ||
	     (useTarget2 && cutUtils::FiducialCut(box.Vertex->Position, SubDetId::kTarget2)) ||
	     (useFGD1    && cutUtils::FiducialCut(box.Vertex->Position, SubDetId::kFGD1   )) ||
	     (useFGD2    && cutUtils::FiducialCut(box.Vertex->Position, SubDetId::kFGD2   )));
		
  }
	
  //**************************************************
  bool TotalMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
		
    (void)box;
		
    // Check we have at least one reconstructed track in the FGD
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
		
    return ((useTarget1 && EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1]>0) ||
	    (useTarget2 && EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget2]>0) ||
	    (useFGD1    && EventBox->nRecObjectsInGroup[EventBox::kTracksWithFGD1]>0   ) ||
	    (useFGD2    && EventBox->nRecObjectsInGroup[EventBox::kTracksWithFGD2]>0   ));
		
  }
	
  //**************************************************
  bool SortTracksAction::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
		
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
    AnaEventB& eventB = *static_cast<AnaEventB*>(&event);
		
    // Retrieve the EventBoxNDUP
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
		
    int nu_mode = ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode");
		
    SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(box.DetectorFV);
    int smpl = 0, smpl2 = 0;
    if (useTarget1) {
      smpl  = EventBox::kTracksWithGoodQualityTPCInTarget1FV;
      smpl2 = EventBox::kTracksWithTarget1AndNoTPC;
    }
    if (useTarget2) {
      smpl  = EventBox::kTracksWithGoodQualityTPCInTarget2FV;
      smpl2 = EventBox::kTracksWithTarget2AndNoTPC;
    }
    if (useFGD1) {
      smpl  = EventBox::kTracksWithGoodQualityTPCInFGD1FV;
    }
    if (useFGD2) {
      smpl  = EventBox::kTracksWithGoodQualityTPCInFGD2FV;
    }
		
    //========================
    //Find TPCGoodQuality tracks in Fwd and Bwd
    //========================
		
    UInt_t nTPC=EventBox->nRecObjectsInGroup[smpl];
    for (UInt_t i=0; i<nTPC; i++){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[smpl][i]);
      if ( nu_mode==1  && track->Charge>=-0.5 ) continue;
      if ( nu_mode==-1 && track->Charge<= 0.5 ) continue;
      if ( !cutUtils::DeltaLYZTPCCut(*track) ) continue;
      cc4pibox->TPCTracks.push_back(track);
    }
    //Sort TPCGoodQuality using Momentum
    std::sort(cc4pibox->TPCTracks.begin(), cc4pibox->TPCTracks.end(), numuCC4pi_utils::HGlobalMomFirst);
		
		
    //========================
    //Find ECal only tracks
    //======================== 
		
    UInt_t nECALTracks=EventBox->nRecObjectsInGroup[EventBox::kTracksWithECal];
    for(UInt_t i=0; i<nECALTracks; i++){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithECal][i]);
      if ( (useTarget1 && anaUtils::InFiducialVolume(SubDetId::kTarget1, track->PositionStart)) ||
	   (useTarget2 && anaUtils::InFiducialVolume(SubDetId::kTarget2, track->PositionStart)) ||
	   (useFGD1    && anaUtils::InFiducialVolume(SubDetId::kFGD1,    track->PositionStart)) ||
	   (useFGD2    && anaUtils::InFiducialVolume(SubDetId::kFGD2,    track->PositionStart))) {
	if ( SubDetId::GetDetectorUsed(track->Detector,SubDetId::kDsECal)) continue;
	if ( cutUtils::DeltaLYZTPCCut(*track) )                            continue;
	cc4pibox->ECalTracks.push_back(track);
      }
    }
    //Sort ECalTracks using RangeMomentum
    std::sort(cc4pibox->ECalTracks.begin(), cc4pibox->ECalTracks.end(), numuCC4pi_utils::HGlobalMomFirst);
		
		
    //========================
    //Find Target only tracks
    //======================== 
		
		
    if (useTarget1 || useTarget2) {
      UInt_t nTarget=EventBox->nRecObjectsInGroup[smpl2];
      for (UInt_t i=0; i<nTarget; i++){
	AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[smpl2][i]);			
	if (!anaUtils::InFiducialVolume(det, track->PositionStart) ||
	    !anaUtils::InFiducialVolume(det, track->PositionEnd) )
	  continue;
				
	if (track->nTargetSegments < 1) continue;
	AnaTargetParticleB* TargetSegment = 
	  dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet(*track, det));
	if (!TargetSegment) continue;
	if (!TargetSegment->IsReconstructed) continue;
				
	cc4pibox->TargetTracks.push_back(track);
      }
    }
		
		
    //========================
    //Find true pions
    //======================== 
		
    for (Int_t i=0; i<eventB.nTrueParticles; i++){
      AnaTrueParticleB* part = eventB.TrueParticles[i];
      if (part->PDG==211)  cc4pibox->TruePiPlus.push_back(part);
      if (part->PDG==-211) cc4pibox->TruePiMinus.push_back(part);
      if (part->PDG==111)  cc4pibox->TruePiZero.push_back(part);
    }
		
    return true;
		
  }
	
	
  //**************************************************
  bool FindMuonCut::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
		
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
		
    //========================
    //Find the better muon candidate
    //======================== 
		
    cc4pibox->MainTrack = NULL;
		
    for (UInt_t i=0; i<cc4pibox->TPCTracks.size() && !cc4pibox->MainTrack; i++) {
			
      AnaTrackB* track = cc4pibox->TPCTracks[i];
      SubDetId::SubDetEnum TPC = anaUtils::GetClosestTPC(*track);
			
      if (!(TPC == SubDetId::kTPCUp1   ||
	    TPC == SubDetId::kTPCUp2   || 
	    TPC == SubDetId::kTPCDown1 || 
	    TPC == SubDetId::kTPCDown2) ) {
	// FWD category
	if (numuCC4pi_utils::IsForward(*track) &&
	    numuCC4pi_utils::MuonPIDCut(*track, true) ) 
	  {cc4pibox->MainTrack = track; cc4pibox->categMuon = 0;}
	// BWD category
	else if (!numuCC4pi_utils::IsForward(*track) &&
		 numuCC4pi_utils::MuonPIDCut(*track, false) ) 
	  {cc4pibox->MainTrack = track; cc4pibox->categMuon = 1;}
      }
      else {
	// HA category
	if (numuCC4pi_utils::MuonPIDCut(*track, false))
	  {cc4pibox->MainTrack = track; cc4pibox->categMuon = 2;}
      }
			
    }
		
		
    for (UInt_t i=0; i<cc4pibox->TargetTracks.size() && !cc4pibox->MainTrack; i++) {
			
      AnaTrackB* track = cc4pibox->TargetTracks[i];
      if (!track) continue;
			
      // pseudo-PID reconstruction
      bool prot_pid=false;
      for (int i=0; i<track->nTargetSegments; i++){
	if (track->TargetSegments[i]->IdAsProton)
	  prot_pid=true;
      }
      if (prot_pid) continue; // reject event if it is seen as a proton
			
      cc4pibox->MainTrack = track; cc4pibox->categMuon = 3;
			
    }
		
		
    for (UInt_t i=0; i<cc4pibox->ECalTracks.size() && !cc4pibox->MainTrack; i++) {
			
      AnaTrackB* track = cc4pibox->ECalTracks[i];
      if (!cutUtils::StoppingECALCut( *track ))
	continue;
			
      bool reco=false;
      for (int i=0; i<track->nECalSegments; i++)
	if (track->ECalSegments[i]->IsReconstructed)
	  reco=true;
      // ECal category
      if (reco && numuCC4pi_utils::MuonECalPIDCut(*track, box, _file_ECAL_PDF))
	{cc4pibox->MainTrack = track; cc4pibox->categMuon = 4;}
			
    }
		
    if (!cc4pibox->MainTrack)
      return false;
		
    cc4pibox->TPC_det = anaUtils::GetClosestTPC(*cc4pibox->MainTrack);
		
    return true;
		
  }
	
  //**************************************************
  bool TrackGQandFVCut::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
		
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
		
    if (!cc4pibox->MainTrack)
      return false;
		
    if ( (useTarget1 && anaUtils::InFiducialVolume(SubDetId::kTarget1, cc4pibox->MainTrack->PositionStart,
						   LAFVmin, LAFVmax)) ||
	 (useTarget2 && anaUtils::InFiducialVolume(SubDetId::kTarget2, cc4pibox->MainTrack->PositionStart,
						   LAFVmin, LAFVmax)) ||
	 (useFGD1 && anaUtils::InFiducialVolume(SubDetId::kFGD1, cc4pibox->MainTrack->PositionStart,
						LAFVmin, LAFVmax)) ||
	 (useFGD2 && anaUtils::InFiducialVolume(SubDetId::kFGD2, cc4pibox->MainTrack->PositionStart,
						LAFVmin, LAFVmax)))
      return true;
		
    return false;
		
  }
	
	
  //**************************************************
  bool VetoAction::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
		
    return true;
		
    /*
      AnaEventB& eventB = *static_cast<AnaEventB*>(&event); 
      ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
			
      for (UInt_t i=0;i<cc4pibox->FwdTracks.size();i++){
      if ( numuCC4pi_utils::VetoCut(0,eventB,*(cc4pibox->FwdTracks[i])) ) {
      if ( numuCC4pi_utils::ExternalCut(0,event,*(cc4pibox->FwdTracks[i])) ) 
      cc4pibox->FwdTracks.push_back(cc4pibox->FwdTracks[i]);
      //	}
      }
			
      for (UInt_t i=0;i<cc4pibox->BwdTracks.size();i++){
      cc4pibox->BwdTracks.push_back(cc4pibox->BwdTracks[i]);
      }
			
      for (UInt_t i=0;i<cc4pibox->HAFwdTracks.size();i++){
      if ( numuCC4pi_utils::VetoCut(2,eventB,*(cc4pibox->HAFwdTracks[i])) )
      cc4pibox->HAFwdTracks.push_back(cc4pibox->HAFwdTracks[i]);
      }
			
      for (UInt_t i=0;i<cc4pibox->HABwdTracks.size();i++){
      if ( numuCC4pi_utils::VetoCut(3,eventB,*(cc4pibox->HABwdTracks[i])) )
      cc4pibox->HABwdTracks.push_back(cc4pibox->HABwdTracks[i]);
      }
			
      if      ( cc4pibox->FwdTracks.size()>0   ) cc4pibox->MainTrack = cc4pibox->FwdTracks[0];
      else if ( cc4pibox->BwdTracks.size()>0   ) cc4pibox->MainTrack = cc4pibox->BwdTracks[0];
      else if ( cc4pibox->HAFwdTracks.size()>0 ) cc4pibox->MainTrack = cc4pibox->HAFwdTracks[0];
      else if ( cc4pibox->HABwdTracks.size()>0 ) cc4pibox->MainTrack = cc4pibox->HABwdTracks[0];
			
      return true;
    */
		
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
    if ( box.MainTrack->TrueObject )
      box.TrueVertex = box.Vertex->TrueVertex = cc4pibox->MainTrack->GetTrueParticle()->TrueVertex;
    return true;
		
  }
	
  //*********************************************************************
  bool FindPionsAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
    //*********************************************************************
		
    SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(boxB.DetectorFV);
		
    ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);
		
    numuCC4pi_utils::FindTPCPions(event, boxB, det);
    numuCC4pi_utils::FindIsoTargetPions(event, boxB, det);
    numuCC4pi_utils::FindMEPions(event, boxB, det);
		
    int nPospionsTPC     =  box->nPositivePionTPCtracks;
    int nNegpionsTPC     =  box->nNegativePionTPCtracks;
    int nIsoTargetpions  =  box->nIsoTargetPiontracks;
    int npi0             =  box->nPosPi0TPCtracks + box->nElPi0TPCtracks;
    int nME              =  box->nMichelElectrons;
		
    int pionTarget = nME;
    if (!nME && nIsoTargetpions > 0) pionTarget = 1;
		
    int pionTarget_nubar = 0;
    if (!nME && nIsoTargetpions > 0) pionTarget_nubar = 1;
		
    int nu_mode = ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode");
		
    if (nu_mode==1) {
      box->nPosPions   = nPospionsTPC + pionTarget;
      box->nNegPions   = nNegpionsTPC;
      box->nOtherPions = nNegpionsTPC + npi0;
    }
    else if (nu_mode==-1) {
      box->nPosPions   = nPospionsTPC + nME;
      box->nNegPions   = nNegpionsTPC + pionTarget_nubar;
      box->nOtherPions = nPospionsTPC + nME + npi0;
    }
		
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
    if (box.MainTrack->GetTrueParticle()) 
      static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget1NuMuCC] = box.MainTrack->GetTrueParticle()->TrueVertex;
		
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kTarget2NuMuCC] = box.MainTrack;
		
    for(int i = 0; i < 4; ++i) static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kTarget2NuMuCC][i] = box.MainTrack->PositionStart[i];
    if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget2NuMuCC] = box.MainTrack->GetTrueParticle()->TrueVertex;
		
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuMuCC] = box.MainTrack;
		
    for(int i = 0; i < 4; ++i) static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuMuCC][i] = box.MainTrack->PositionStart[i];
    if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuMuCC] = box.MainTrack->GetTrueParticle()->TrueVertex;
		
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuMuCC] = box.MainTrack;
		
    for(int i = 0; i < 4; ++i) static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuMuCC][i] = box.MainTrack->PositionStart[i];
    if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuMuCC] = box.MainTrack->GetTrueParticle()->TrueVertex;
		
    return true;
		
  }
	
  //**************************************************
  bool FwdTPC_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    (void)event;
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
    if ( cutUtils::DeltaLYZTPCCut(*cc4pibox->MainTrack)   && 
	 numuCC4pi_utils::IsForward(*cc4pibox->MainTrack) &&
	 !(cc4pibox->TPC_det == SubDetId::kTPCUp1 ||
	   cc4pibox->TPC_det == SubDetId::kTPCUp2 || 
	   cc4pibox->TPC_det == SubDetId::kTPCDown1 || 
	   cc4pibox->TPC_det == SubDetId::kTPCDown2) ) {
      return true;
    }
    return false;
  }
	
  //**************************************************
  bool FwdTPC_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    (void)event;
		
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
		
    if ( numuCC4pi_utils::MuonPIDCut(*cc4pibox->MainTrack, true) ) return true;
    return false;
  }
	
  //**************************************************
  bool BwdTPC_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    (void)event;
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
    if ( cutUtils::DeltaLYZTPCCut(*cc4pibox->MainTrack)    && 
	 !numuCC4pi_utils::IsForward(*cc4pibox->MainTrack) &&
	 !(cc4pibox->TPC_det == SubDetId::kTPCUp1 ||
	   cc4pibox->TPC_det == SubDetId::kTPCUp2 || 
	   cc4pibox->TPC_det == SubDetId::kTPCDown1 || 
	   cc4pibox->TPC_det == SubDetId::kTPCDown2) )
      return true;
    return false;
  }
	
  //**************************************************
  bool BwdTPC_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    (void)event;
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
    if ( numuCC4pi_utils::MuonPIDCut(*cc4pibox->MainTrack, false) ) return true;
    return false;
  }
	
  //**************************************************
  bool HATPC_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    (void)event;
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
    if ( cutUtils::DeltaLYZTPCCut(*cc4pibox->MainTrack) && 
	 (cc4pibox->TPC_det == SubDetId::kTPCUp1   ||
	  cc4pibox->TPC_det == SubDetId::kTPCUp2   || 
	  cc4pibox->TPC_det == SubDetId::kTPCDown1 ||
	  cc4pibox->TPC_det == SubDetId::kTPCDown2 ) )
      return true;
    return false;
  }
	
  //**************************************************
  bool HATPC_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    (void)event;
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
    if ( numuCC4pi_utils::MuonPIDCut(*cc4pibox->MainTrack, false) ) return true;
    return false;
  }
	
  //**************************************************
  bool ECal_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
		
    (void)event;
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
		
    // do not consider this category in RHC
    if (ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode") == -1)
      return false;
		
    // if long enough TPC track, it should be selected by other branches
    if (cutUtils::DeltaLYZTPCCut(*cc4pibox->MainTrack))
      return false;
		
    if (!cutUtils::StoppingECALCut( *cc4pibox->MainTrack ))
      return false;
		
    for (int i=0; i<cc4pibox->MainTrack->nECalSegments; i++)
      if (cc4pibox->MainTrack->ECalSegments[i]->IsReconstructed)
	return true;
		
    return false;
  }
	
  //**************************************************
  bool ECal_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    (void)event;
		
		
    // initializing
		
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
		
    if (!_file_ECAL_PDF) {
      std::cerr << "In cutUtils::MuonECALPIDCut : No file found to define the PDFs for ECal variables" << std::endl;
      return false;
    }
		
    TH2F *h_binning = (TH2F*)_file_ECAL_PDF->Get("hBinning");
    AnaTrueParticleB* trueParticle = cc4pibox->MainTrack->GetTrueParticle();
    if (!trueParticle) return false;
		
		
    // find the first segment in an ECal
		
    AnaDetCrossingB* firstECal_cross=NULL;
    for (unsigned int i=0; i<trueParticle->DetCrossingsVect.size(); i++) {
      AnaDetCrossingB* cross = trueParticle->DetCrossingsVect[i];
      if (!cross->Detector_name.Contains("ECal") || !cross->Detector_name.Contains("Bar")) continue;
      firstECal_cross = cross;
    }
		
		
    // fill the information at ECal entry
		
    if (!firstECal_cross)
      return false;
		
    TVector3 P   = anaUtils::ArrayToTVector3(firstECal_cross->EntranceMomentum);
    TVector3 pos = anaUtils::ArrayToTVector3(firstECal_cross->EntrancePosition);
    TVector3 entryNormal_vect(0,0,0);
		
    if (firstECal_cross->Detector_name.Contains("RightClam") &&
	firstECal_cross->Detector_name.Contains("BotLeftTopRight"))
      entryNormal_vect.SetY(1);  // (+Y)
    else if (firstECal_cross->Detector_name.Contains("RightClam") &&
	     firstECal_cross->Detector_name.Contains("TopLeftBotRight"))
      entryNormal_vect.SetY(-1); // (-Y)
    else if (firstECal_cross->Detector_name.Contains("LeftClam") &&
	     firstECal_cross->Detector_name.Contains("BotLeftTopRight"))
      entryNormal_vect.SetY(-1); // (-Y)
    else if (firstECal_cross->Detector_name.Contains("LeftClam") &&
	     firstECal_cross->Detector_name.Contains("TopLeftBotRight"))
      entryNormal_vect.SetY(1);  // (+Y)
    else if (firstECal_cross->Detector_name.Contains("LeftSide"))
      entryNormal_vect.SetX(1);  // (+X)
    else if (firstECal_cross->Detector_name.Contains("RightSide"))
      entryNormal_vect.SetX(-1); // (-X)
    else if(firstECal_cross->Detector_name.Contains("POD/USECal"))
      entryNormal_vect.SetZ(-1); // (-Z)
    else
      entryNormal_vect.SetZ(1);  // (+Z)
		
    float mom = P.Mag();
    float cos = P.Dot(entryNormal_vect)/mom;
    int bin   = h_binning->GetBinContent(h_binning->FindBin(cos,mom));
		
		
    // throw random the ECal variables using the pdfs
		
    TH2F *h_bin  = (TH2F*)_file_ECAL_PDF->Get(TString::Format("mipem_Vs_EneOnL_%i", bin));
    double MipEM, EneOnL;
    h_bin->GetRandom2(MipEM, EneOnL);
		
		
    // fill the ECal variables in the box
		
    cc4pibox->track_ECal_MipEM  = MipEM;
    cc4pibox->track_ECal_EneOnL = EneOnL;
		
		
    // apply cuts on ECal variables
		
    if (MipEM < 0 && EneOnL < 1.25)
      return true;
    return false;
		
  }
	
  //**************************************************
  bool CC0pi::Apply(AnaEventC& event, ToyBoxB& boxB) const{
    //**************************************************
		
    (void)event;
    ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);
    return (box->nPosPions == 0 && box->nNegPions == 0 && box->nOtherPions == 0);
		
  }
	
  //**************************************************
  bool CC1pi::Apply(AnaEventC& event, ToyBoxB& boxB) const{
    //**************************************************
		
    (void)event;
    ToyBoxCC4pi* box = static_cast<ToyBoxCC4pi*>(&boxB);
    int nu_mode = ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode");
		
    if (nu_mode==1)
      return (box->nPosPions == 1 && box->nOtherPions == 0);
    else
      return (box->nNegPions == 1 && box->nOtherPions == 0);
		
    return false;
		
  }
	
  //**************************************************
  bool CCoth::Apply(AnaEventC& event, ToyBoxB& boxB) const{
    //**************************************************
		
    (void)event;
    ToyBoxCC4pi* box = static_cast<ToyBoxCC4pi*>(&boxB);
    int nu_mode = ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode");
		
    if (nu_mode==1)
      return (box->nPosPions > 1 || box->nOtherPions > 0);
    else
      return (box->nNegPions > 1 || box->nOtherPions > 0);
		
    return false;
		
  }
	
}


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
	
  return true;
	
}

//**************************************************
bool numuCC4piSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************
	
  (void)event;
  (void)branch;
	
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
