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

  _ECal_reco_eff = new BinnedParams(std::string(getenv("NUMUCC4PIANALYSISROOT")) + "/data",
				    "ECal_recoEff", BinnedParams::k1D_SYMMETRIC); 
  
  _ECal_FGDmatch_eff = new BinnedParams(std::string(getenv("NUMUCC4PIANALYSISROOT")) + "/data",
					"ECal_FGDmatchEff", BinnedParams::k1D_SYMMETRIC); 

  
}

//********************************************************************
void numuCC4piSelection::DefineSteps(){
  //********************************************************************


  //last true means that if that cut is not fulfill the sequence is stop
  AddStep(StepBase::kAction, "find true vertex",      new numuCC4piUtils::FindTrueVertexAction());
  AddStep(StepBase::kCut,    "true vertex in target", new numuCC4piUtils::TrueVertexInTargetCut(), true);

  AddStep(StepBase::kCut,    "> 0 tracks ",         new numuCC4piUtils::TotalMultiplicityCut(), true);
  AddStep(StepBase::kAction, "Sort TPC tracks",     new numuCC4piUtils::SortTracksAction());
  AddStep(StepBase::kCut,    "quality+fiducial",    new numuCC4piUtils::TrackGQandFVCut(),      true);

  AddStep(StepBase::kAction, "find vertex",         new numuCC4piUtils::FindVertexAction());
  AddStep(StepBase::kAction, "fill summary",        new numuCC4piUtils::FillSummaryAction_numuCC4pi());
  AddStep(StepBase::kAction, "find pions",         new numuCC4piUtils::FindPionsAction());

  AddSplit(4);

  AddStep(0, StepBase::kCut, "Fwd TPC Quality Cut",  new numuCC4piUtils::FwdTPC_Quality());
  AddStep(0, StepBase::kCut, "Fwd TPC PID Cut",      new numuCC4piUtils::FwdTPC_PID());
  AddSplit(4, 0);
  AddStep(0, 0, StepBase::kCut, "CC0pi topology",    new numuCC4piUtils::CC0pi());
  AddStep(0, 1, StepBase::kCut, "CC1pi topology",    new numuCC4piUtils::CC1pi());
  AddStep(0, 2, StepBase::kCut, "CCoth topology",    new numuCC4piUtils::CCoth());
  
  AddStep(1, StepBase::kCut, "Bwd TPC Quality Cut",    new numuCC4piUtils::BwdTPC_Quality());
  AddStep(1, StepBase::kCut, "Bwd TPC PID Cut",        new numuCC4piUtils::BwdTPC_PID());
  AddSplit(4, 1);
  AddStep(1, 0, StepBase::kCut, "CC0pi topology",    new numuCC4piUtils::CC0pi());
  AddStep(1, 1, StepBase::kCut, "CC1pi topology",    new numuCC4piUtils::CC1pi());
  AddStep(1, 2, StepBase::kCut, "CCoth topology",    new numuCC4piUtils::CCoth());

  AddStep(2, StepBase::kCut, "HA TPC Quality Cut",    new numuCC4piUtils::HATPC_Quality());
  AddStep(2, StepBase::kCut, "HA TPC PID Cut",        new numuCC4piUtils::HATPC_PID());
  AddSplit(4, 2);
  AddStep(2, 0, StepBase::kCut, "CC0pi topology",    new numuCC4piUtils::CC0pi());
  AddStep(2, 1, StepBase::kCut, "CC1pi topology",    new numuCC4piUtils::CC1pi());
  AddStep(2, 2, StepBase::kCut, "CCoth topology",    new numuCC4piUtils::CCoth());

  AddStep(3, StepBase::kCut, "ECal Quality Cut",  new numuCC4piUtils::ECal_Quality(_ECal_reco_eff, _ECal_FGDmatch_eff));
  AddStep(3, StepBase::kCut, "ECal PID Cut",      new numuCC4piUtils::ECal_PID(_file_ECAL_PDF));
  AddSplit(4, 3);
  AddStep(3, 0, StepBase::kCut, "CC0pi topology",    new numuCC4piUtils::CC0pi());
  AddStep(3, 1, StepBase::kCut, "CC1pi topology",    new numuCC4piUtils::CC1pi());
  AddStep(3, 2, StepBase::kCut, "CCoth topology",    new numuCC4piUtils::CCoth());

  SetBranchAlias(0,  "CC0pi Fwd TPC",  0,0);
  SetBranchAlias(1,  "CC0pi Bwd TPC",  1,0);
  SetBranchAlias(2,  "CC0pi HA TPC",   2,0);
  SetBranchAlias(3,  "CC0pi ECal",     3,0);

  SetBranchAlias(4,  "CC1pi Fwd TPC",  0,1);
  SetBranchAlias(5,  "CC1pi Bwd TPC",  1,1);
  SetBranchAlias(6,  "CC1pi HA TPC",   2,1);
  SetBranchAlias(7,  "CC1pi ECal",     3,1);

  SetBranchAlias(8,  "CCoth Fwd TPC",  0,2);
  SetBranchAlias(9,  "CCoth Bwd TPC",  1,2);
  SetBranchAlias(10, "CCoth HA TPC",   2,2);
  SetBranchAlias(11, "CCoth ECal",     3,2);

  SetBranchAlias(12, "CCinc Fwd TPC",  0,3);
  SetBranchAlias(13, "CCinc Bwd TPC",  1,3);
  SetBranchAlias(14, "CCinc HA TPC",   2,3);
  SetBranchAlias(15, "CCinc ECal",     3,3);

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

    //========================
    //Find TPCGoodQuality tracks in Fwd and Bwd
    //========================
  
    if (useTarget1) {

      int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV];
      for (Int_t i=0;i<nTPC; ++i){
	AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV][i]);
	if ( nu_mode==1  && track->Charge>=-0.5 ) continue;
	if ( nu_mode==-1 && track->Charge<=0.5  ) continue;

	cc4pibox->TPCTracks.push_back(track);
      }

    }
    if (useTarget2) {

      int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV];
      for (Int_t i=0;i<nTPC; ++i){
	AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV][i]);
	if ( nu_mode==1  && track->Charge>=-0.5 ) continue;
	if ( nu_mode==-1 && track->Charge<=0.5  ) continue;

	cc4pibox->TPCTracks.push_back(track);
      }

    }
   if (useFGD1) {

      int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD1FV];
      for (Int_t i=0;i<nTPC; ++i){
	AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD1FV][i]);
	if ( nu_mode==1  && track->Charge>=-0.5 ) continue;
	if ( nu_mode==-1 && track->Charge<=0.5  ) continue;

	cc4pibox->TPCTracks.push_back(track);
      }

    }
    if (useFGD2) {

      int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD2FV];
      for (Int_t i=0;i<nTPC; ++i){
	AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD2FV][i]);
	if ( nu_mode==1  && track->Charge>=-0.5 ) continue;
	if ( nu_mode==-1 && track->Charge<=0.5  ) continue;

	cc4pibox->TPCTracks.push_back(track);
      }

    }


  
    //Sort TPCGoodQuality using Momentum
    std::sort(cc4pibox->TPCTracks.begin(), cc4pibox->TPCTracks.end(), numuCC4pi_utils::HGlobalMomFirst);

    //========================
    //Find ECal only tracks
    //========================  
    int nECALTracks=EventBox->nRecObjectsInGroup[EventBox::kTracksWithECal];
    for(int i=0;i<nECALTracks;i++){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithECal][i]);
      if ( (useTarget1 && anaUtils::InFiducialVolume(SubDetId::kTarget1, track->PositionStart)) ||
	   (useTarget2 && anaUtils::InFiducialVolume(SubDetId::kTarget2, track->PositionStart)) ||
	   (useFGD1    && anaUtils::InFiducialVolume(SubDetId::kFGD1,    track->PositionStart)) ||
	   (useFGD2    && anaUtils::InFiducialVolume(SubDetId::kFGD2,    track->PositionStart))) {
	if ( SubDetId::GetDetectorUsed(track->Detector,SubDetId::kDsECal)) continue;
	if ( cutUtils::DeltaLYZTPCCut(*track) )                            continue;
	if ( fabs(track->Charge) < 1e-3 ) continue;
	cc4pibox->ECalTracks.push_back(track);
      }
    }
    //Sort ECalTracks using RangeMomentum
    std::sort(cc4pibox->ECalTracks.begin(), cc4pibox->ECalTracks.end(), numuCC4pi_utils::HGlobalMomFirst);

    if      (cc4pibox->TPCTracks.size()>0)
      cc4pibox->MainTrack = cc4pibox->TPCTracks[0];
    else if (cc4pibox->ECalTracks.size()>0)
      cc4pibox->MainTrack = cc4pibox->ECalTracks[0];
    else return false;  

    cc4pibox->TPC_det = anaUtils::GetClosestTPC(*cc4pibox->MainTrack);


   for (int i=0; i<eventB.nTrueParticles; i++){
      AnaTrueParticleB* part = eventB.TrueParticles[i];
      if (part->PDG==211)  cc4pibox->TruePiPlus.push_back(part);
      if (part->PDG==-211) cc4pibox->TruePiMinus.push_back(part);
      if (part->PDG==111)  cc4pibox->TruePiZero.push_back(part);
    }


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
    if ( box.MainTrack->GetTrueParticle() ) box.TrueVertex = box.Vertex->TrueVertex = cc4pibox->MainTrack->GetTrueParticle()->TrueVertex;

    return true;

  }

  //*********************************************************************
  bool FindPionsAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
    //*********************************************************************

    SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(boxB.DetectorFV);

    ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);
    EventBox* EventBox_ = static_cast<EventBox*>(event.EventBoxes[EventBoxId::kEventBoxNDUP]);

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
    if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget1NuMuCC] = box.MainTrack->GetTrueParticle()->TrueVertex;

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

    for (unsigned int i=0; i<cc4pibox->MainTrack->nECalSegments; i++)
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

  /*
  //**************************************************
  bool ToF_senseDetermination::Apply(AnaEventC& event, ToyBoxB& boxB) const{
    //**************************************************

    (void)event;
    ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);
    if (!box->MainTrack) return false;
    bool ok = false;

    // do not consider for truly forward-going track
    if (box->MainTrack->TrueObject && box->MainTrack->GetTrueParticle()->Direction[2] > 0)
      ok = true;

    AnaParticleB *p1=0, *p2=0;
    float sigma=0;
    float ToF = anaUtils::GetToF(box->MainTrack, p1, p2, sigma);
    if (!p1 or !p2)
      return ok;

    float true_ToF = p2->PositionStart[3]-p1->PositionStart[3];
    ok = ok || (true_ToF*ToF > 0);

    // ==========
    // ToF mass
    // ==========

    // smear the momentum
    Float_t true_mom = box->MainTrack->GetTrueParticle()->Momentum;
    Float_t mom = box->MainTrack->SmearedMomentum; // TPC-smeared momentum
    // if the track is not smeared in TPC, apply a default 10% smearing
    if (fabs(true_mom-mom)<1e-3)
      mom = gRandom->Gaus(true_mom, 0.10*true_mom);

    Float_t sigma_length;
    Float_t true_length = anaUtils::GetLength(box->MainTrack, p1, p2, sigma_length, false);
    Float_t      length = anaUtils::GetLength(box->MainTrack, p1, p2, sigma_length, true); 
   
    box->ToF_mass      = anaUtils::ComputeToFMass(     mom,      ToF,      length);
    box->ToF_true_mass = anaUtils::ComputeToFMass(true_mom, true_ToF, true_length);



    // ==========
    // ToF mass for all particles
    // ==========

    AnaEventB& eventB = static_cast<AnaEventB&>(event);

    for (int i=0; i<eventB.nParticles; i++) {

      // take the track
      AnaTrackB* track_i = static_cast<AnaTrackB*>(eventB.Particles[i]);
      if (!track_i) continue;
      if (!track_i->TrueObject) continue;

      // find the two segments giving the ToF and compute it
      p1=0; p2=0;
      float ToF = anaUtils::GetToF(track_i, p1, p2, sigma);
      if (!p1 or !p2)
	continue;
      float true_ToF = p2->PositionStart[3]-p1->PositionStart[3];


      // smear the momentum
      float true_mom = track_i->GetTrueParticle()->Momentum;
      float      mom = track_i->SmearedMomentum;

      // if TPC segment available, take the momentum from TPC
      if (track_i->nTPCSegments>0){
	mom      = track_i->TPCSegments[0]->SmearedMomentum;
	true_mom = track_i->TPCSegments[0]->Momentum;
      }
      // otherwise, we smear the momentum by 10%, saying it has been measured by range
      if (fabs(true_mom-mom)<1e-3)
	mom = gRandom->Gaus(true_mom, 0.10*true_mom);

      float true_length = anaUtils::GetLength(track_i, p1, p2, sigma_length, false);
      float      length = anaUtils::GetLength(track_i, p1, p2, sigma_length, true); 

      float m_reco = numuCC4pi_utils::ComputeToFMass(     mom,      ToF,      length);
      float m_true = numuCC4pi_utils::ComputeToFMass(true_mom, true_ToF, true_length);

      float ToF_pull[4], ToF_lkl[4];
      anaUtils::ComputeToFpulls(track_i, ToF_pull, true);
      anaUtils::CalculateToFLikelihood(ToF_pull, ToF_lkl);


      // store all information related to the track
      box->All_ToF_mass.push_back(m_reco);
      box->All_ToF_true_mass.push_back(m_true);
      box->All_ToF_time_reco.push_back(ToF);

      box->All_mom.push_back(track_i->SmearedMomentum);
      box->All_cos.push_back(track_i->DirectionStart[2]);
      box->All_L.push_back(length);
      box->All_true_mom.push_back(track_i->GetTrueParticle()->Momentum);
      box->All_PDG.push_back(track_i->GetTrueParticle()->PDG);
	  
      if (SubDetId::GetSubdetectorEnum(p1->Detectors) == SubDetId::kToF)
	box->All_ToF_det_used1.push_back(-SubDetId::GetToF(((AnaToFParticleB*)p1)->Detector_name));
      else 
	box->All_ToF_det_used1.push_back(SubDetId::GetSubdetectorEnum(p1->Detectors));
      if (SubDetId::GetSubdetectorEnum(p2->Detectors) == SubDetId::kToF)
	box->All_ToF_det_used2.push_back(-SubDetId::GetToF(((AnaToFParticleB*)p2)->Detector_name));
      else 
	box->All_ToF_det_used2.push_back(SubDetId::GetSubdetectorEnum(p2->Detectors));
	  
 
      box->All_ToF_pull_muon.push_back(ToF_pull[0]);
      box->All_ToF_pull_pion.push_back(ToF_pull[3]);
      box->All_ToF_pull_electron.push_back(ToF_pull[1]);
      box->All_ToF_pull_proton.push_back(ToF_pull[2]);

      box->All_ToF_lkl_muon.push_back(ToF_lkl[0]);
      box->All_ToF_lkl_pion.push_back(ToF_lkl[3]);
      box->All_ToF_lkl_electron.push_back(ToF_lkl[1]);
      box->All_ToF_lkl_proton.push_back(ToF_lkl[2]);

      // store the TPC pulls and likelihood if we have a TPC segment
      if (track_i->nTPCSegments>0){
	Float_t pulls[4], lkl[4];
	anaUtils::ComputeTPCPull(*track_i->TPCSegments[0], pulls);
	
	box->All_TPC_pull_muon.push_back(pulls[0]);
	box->All_TPC_pull_pion.push_back(pulls[3]);
	box->All_TPC_pull_electron.push_back(pulls[1]);
	box->All_TPC_pull_proton.push_back(pulls[2]);
	box->All_TPC_lkl_muon.push_back(lkl[0]);
	box->All_TPC_lkl_pion.push_back(lkl[3]);
	box->All_TPC_lkl_electron.push_back(lkl[1]);
	box->All_TPC_lkl_proton.push_back(lkl[2]);
	box->All_TPC_dedx.push_back(track_i->TPCSegments[0]->dEdxMeas);
      }
      else{
	box->All_TPC_pull_muon.push_back(-999.);
	box->All_TPC_pull_pion.push_back(-999.);
	box->All_TPC_pull_electron.push_back(-999.);
	box->All_TPC_pull_proton.push_back(-999.);
	box->All_TPC_lkl_muon.push_back(-999.);
	box->All_TPC_lkl_pion.push_back(-999.);
	box->All_TPC_lkl_electron.push_back(-999.);
	box->All_TPC_lkl_proton.push_back(-999.);
	box->All_TPC_dedx.push_back(-999.);
      }

    }

    // ==========
    // ==========

    return ok;

  }
  */

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
