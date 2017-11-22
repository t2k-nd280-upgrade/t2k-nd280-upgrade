#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "EventBox.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "SelectionUtils.hxx"
#include "AnaRecPackManager.hxx"
#include "TreeConverterUtils.hxx"

#include "nueCCSelection.hxx"
#include "nueCCUtils.hxx"

#include <TH2F.h>
#include <TAxis.h>

#include <utility>
#include <map>

//********************************************************************
nueCCSelection::nueCCSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxNDUP) {
  //********************************************************************

  // TODO move to highlandIO
  char filename[256];
  sprintf(filename, "%s/data/ECalPID.root", getenv("NUECCANALYSISROOT"));
  _file_ECAL_PDF = TFile::Open(filename);
  
}

//********************************************************************
void nueCCSelection::DefineSteps(){
  //********************************************************************


  //last true means that if that cut is not fulfill the sequence is stop
  AddStep(StepBase::kAction, "find true vertex",      new FindTrueVertexAction());

  // 0
  AddStep(StepBase::kCut,    "> 0 tracks ",           new TotalMultiplicityCut(), true);
  
  // Find the main track
  AddStep(StepBase::kAction, "Sort TPC tracks",       new SortTracksAction());

  

  AddStep(StepBase::kAction, "find vertex",           new FindVertexAction());
  AddStep(StepBase::kAction, "fill summary",          new FillSummaryAction_nueCC());

  AddSplit(2);

  // 1 FV and quality
  AddStep(0,  StepBase::kCut,"quality+fiducial",      new TrackGQandFVCut());
  

  // 2 Track length cut
  AddStep(0,  StepBase::kCut,    "TPC Quality Cut",       new TPC_Quality());

  // ToF info
  AddStep(0,  StepBase::kAction, "ToF",                   new ToF_senseDetermination());

  // 3 TPC PID
  AddStep(0,  StepBase::kCut,    "TPC Electron PID Cut",  new TPCElectronPID());
  // 4 ECal PID
  AddStep(0,  StepBase::kCut,    "ECal PID Cut",          new ECal_PID(_file_ECAL_PDF));
  // 5 TPC PID 2
  AddStep(0,  StepBase::kCut,    "TPC Muon PID Cut",      new TPCMuonPID()); // For events not going in the Ecal

  // 6 Pair cut
  AddStep(0,  StepBase::kAction, "find best e+e- pair",   new FindPairsAction());
  AddStep(0,  StepBase::kCut,    "Pair Veto",	            new PairCut());

  // 7 TPC Veto
  AddStep(0,  StepBase::kCut,    "TPC Veto Cut",          new TPCVeto());

  // 8 ECal Veto
  AddStep(0,  StepBase::kCut,    "ECal Veto Cut",         new ECalVeto());

  // find all iso target tracks, replace Main Track
  AddStep(1,  StepBase::kAction,  "Find Iso-tar tracks",  new FindIsoTargetElectrons());

  // sucsessfully reconstructed iso target track
  AddStep(1, StepBase::kCut,       "Length and PID",       new LengthPIDcut());

  SetBranchAlias(0,  "CCNuE Inclusive TPC", 0);
  SetBranchAlias(1,  "CCNuE Inclusive target", 1);
  //SetPreSelectionAccumLevel(0);

}

//********************************************************************
void nueCCSelection::DefineDetectorFV(){
  //********************************************************************

  // Change FV definition to take all thickness
  // The detector in which the selection is applied
  bool useTarget1 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD2");

  // WARNING in case of the combine analysis for target and FGDs the FV will be set to FGD
  if ( useTarget1 && !useTarget2) SetDetectorFV(SubDetId::kTarget1);
  if (!useTarget1 &&  useTarget2) SetDetectorFV(SubDetId::kTarget2);
  if ( useTarget1 &&  useTarget2) SetDetectorFV(SubDetId::kTarget);

  if ( useFGD1 && !useFGD2) SetDetectorFV(SubDetId::kFGD1);
  if (!useFGD1 &&  useFGD2) SetDetectorFV(SubDetId::kFGD2);
  if ( useFGD1 &&  useFGD2) SetDetectorFV(SubDetId::kFGD);
}

//**************************************************
void nueCCSelection::InitializeEvent(AnaEventC& eventBB){
  //**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB);

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxNDUP]) event.EventBoxes[EventBoxId::kEventBoxNDUP] = new EventBox();

  // fill the tracks in a proper way
  bool useTarget1 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD2");

  // for the referenc configuration it is excessive if you specify the certain target
  if (useTarget1 || useTarget2) {
    boxUtils::FillTracksWithTPC(event,    static_cast<SubDetId::SubDetEnum>(SubDetId::kTarget));
    boxUtils::FillTracksWithTarget(event, static_cast<SubDetId::SubDetEnum>(SubDetId::kTarget));
  }

  if (useFGD1 || useFGD2) {
    if (GetDetectorFV() == SubDetId::kFGD) {
      boxUtils::FillTracksWithTPC(event,    static_cast<SubDetId::SubDetEnum>(SubDetId::kFGD));
      boxUtils::FillTracksWithFGD(event,    static_cast<SubDetId::SubDetEnum>(SubDetId::kFGD));
    } else 
      boxUtils::FillTracksWithFGD(event,    static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  }
 
  boxUtils::FillTracksWithECal(event);  
}

//********************************************************************
bool nueCCSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
  //********************************************************************

  if(allCutsPassed[0]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuECC;

  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kTarget1NuECC);

}

//**************************************************
bool FindTrueVertexAction::Apply(AnaEventC & event, ToyBoxB & boxB) const {
  //**************************************************
  
  (void)event;

  ToyBoxCC4pi& box  = static_cast<ToyBoxCC4pi&>(boxB);
  AnaEventB& eventB = static_cast<AnaEventB&>(event);
  
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
  
  // Retrieve the EventBoxNDUP
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

  AnaEventB& eventB = static_cast<AnaEventB&>(event);
  
  //========================
  //Find TPCGoodQuality tracks in Fwd and Bwd
  //========================
  if (useTarget1) {
    
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV][i]);

      // is selection charge is 1 then select positrons, if -1 then electrons. If 0 then don't care about the charge, select the highest momentum
      if(selection_charge > -0.5){
	if(track->Charge <= 0.5) continue;
      }
      else if (selection_charge < 0.5) {
	if ( track->Charge>=-0.5 ) continue;
      }
      
      cc4pibox->TPCTracks.push_back(track);
    }
    
  }
  if (useTarget2) {
    
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV][i]);

      if(selection_charge > -0.5){
	if(track->Charge <= 0.5) continue;
      }
      else if (selection_charge < 0.5) {
	if ( track->Charge>=-0.5 ) continue;
      }
      
      cc4pibox->TPCTracks.push_back(track);
    }
    
  }
  if (useFGD1) {
    
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD1FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD1FV][i]);

      if(selection_charge > -0.5){
	if(track->Charge <= 0.5) continue;
      }
      else if (selection_charge < 0.5) {
	if ( track->Charge>=-0.5 ) continue;
      }
      
      cc4pibox->TPCTracks.push_back(track);
    }
    
  }
  if (useFGD2) {
    
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD2FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD2FV][i]);

      if(selection_charge > -0.5){
	if(track->Charge <= 0.5) continue;
      }
      else if (selection_charge < 0.5) {
	if ( track->Charge>=-0.5 ) continue;
      }
      
      cc4pibox->TPCTracks.push_back(track);
    }
    
  }
 
  // Sort TPCGoodQuality using Momentum
  std::sort(cc4pibox->TPCTracks.begin(), cc4pibox->TPCTracks.end(), nueCCUtils::HGlobalMomFirst);
  
  //========================
  // Find ECal only tracks
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

  // Sort ECalTracks using RangeMomentum
  std::sort(cc4pibox->ECalTracks.begin(), cc4pibox->ECalTracks.end(), nueCCUtils::HGlobalMomFirst);
  
  if      (cc4pibox->TPCTracks.size()>0)
    cc4pibox->MainTrack = cc4pibox->TPCTracks[0];
  //else if (cc4pibox->ECalTracks.size()>0)
  //cc4pibox->MainTrack = cc4pibox->ECalTracks[0];
  else return false;  

  
  //========================
  // Look at the iso target primary track
  //========================
  for (Int_t i = 0; i < eventB.nTrueParticles; ++i) {
    AnaTrueParticleB* TrueTrack = eventB.TrueParticles[i];
    if (!TrueTrack)
      continue;

    if (TrueTrack->DetCrossingsVect.size() != 1)
      continue;
    if (TrueTrack->DetCrossingsVect[0]->Detector != pow(2, (int)SubDetId::kTarget1))
      continue;

    if (TrueTrack->ParentID != 0 || (abs(TrueTrack->TrueVertex->NuPDG) != 12 && abs(TrueTrack->TrueVertex->NuPDG) != 14))
      continue;

    Double_t L = TrueTrack->DetCrossingsVect[0]->SegLength;

    if (abs(TrueTrack->PDG) == 11) {
      cc4pibox->EleTargetLength   = L;
    } else if (abs(TrueTrack->PDG) == 211) {
      cc4pibox->PiTargetLength   = L;
    } else if (abs(TrueTrack->PDG) == 13) {
      cc4pibox->MuTargetLength   = L;
    } else if (abs(TrueTrack->PDG) == 2212) {
      cc4pibox->PrTargetLength   = L;
    }
  }
  

  // Protection
  if(cc4pibox->MainTrack->SmearedMomentum <= 0.0) return false;
  
  cc4pibox->TPC_det = anaUtils::GetClosestTPC(*cc4pibox->MainTrack);

  //========================
  // Find TPC veto track
  //========================
  int nTPCs=EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPC];
  for (Int_t i=0;i<nTPCs; ++i){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPC][i]);
    if(track == cc4pibox->MainTrack) continue;
    if(!cc4pibox->TPCVetoTrack)
      cc4pibox->TPCVetoTrack = track;
    else{
      if(track->SmearedMomentum > cc4pibox->TPCVetoTrack->SmearedMomentum)
        cc4pibox->TPCVetoTrack = track;
    }
  }

  //========================
  // Find ECal veto track
  //========================
  AnaTrackB* ecaltracks[500];
  bool useP0DECal = (bool)ND::params().GetParameterD("nueCCAnalysis.ECal.UseP0DECal");
  bool ECalOnlyVeto = (bool)ND::params().GetParameterD("nueCCAnalysis.ECal.ECalOnlyVeto");

  // TODO understand if we need ECal only or ECal using tracks. Need OOFV samples for this
  Int_t nEcal = 0;
  if (ECalOnlyVeto)
    nEcal  = anaUtils::GetAllTracksUsingOnlyDet(static_cast<const AnaEventB&> (event), SubDetId::kECAL, ecaltracks);
  else
    nEcal = anaUtils::GetAllTracksUsingDet(static_cast<const AnaEventB&> (event), SubDetId::kECAL, ecaltracks);

  for(Int_t i=0; i<nEcal; i++){
    AnaTrackB* ecaltrack = ecaltracks[i];
    if (SubDetId::GetDetectorUsed(ecaltrack->Detector,SubDetId::kDsECal) && !SubDetId::GetDetectorUsed(ecaltrack->Detector,SubDetId::kBrlECal))
      continue;
    if (!useP0DECal && SubDetId::GetDetectorUsed(ecaltrack->Detector,SubDetId::kP0DECal) && !SubDetId::GetDetectorUsed(ecaltrack->Detector,SubDetId::kBrlECal))
      continue;
    if(ecaltrack->PositionStart[2] == -999) continue;

    for (Int_t j = 0; j < ecaltrack->nECalSegments; ++j) {
      if (!ecaltrack->ECalSegments[j]->IsReconstructed)
        continue;

      if(!cc4pibox->ECalVetoTrack)
        cc4pibox->ECalVetoTrack = ecaltrack->ECalSegments[j];
      else{
        if(ecaltrack->ECalSegments[j]->PositionStart[2] < cc4pibox->ECalVetoTrack->PositionStart[2])
          cc4pibox->ECalVetoTrack = ecaltrack->ECalSegments[j];
      }
    }
  }

  //========================
  // Find Gamma Iso track
  //========================
  const Float_t vertex_activity = 50.0;
  if (useTarget1) {
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV][i]);
      if(track == cc4pibox->MainTrack) continue;

      Float_t diff = (anaUtils::ArrayToTVector3(track->PositionStart) -
		      anaUtils::ArrayToTVector3(cc4pibox->MainTrack->PositionStart)).Mag();
      
      if(diff > vertex_activity)
	cc4pibox->NFarTracks++;
      else
	cc4pibox->NNearTracks++;
    }
  }

  if (useTarget2) {
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV][i]);
      if(track == cc4pibox->MainTrack) continue;

      Float_t diff = (anaUtils::ArrayToTVector3(track->PositionStart) -
		      anaUtils::ArrayToTVector3(cc4pibox->MainTrack->PositionStart)).Mag();
      
      if(diff > vertex_activity)
	cc4pibox->NFarTracks++;
      else
	cc4pibox->NNearTracks++;
    }
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
bool FillSummaryAction_nueCC::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //********************************************************************
  
  // Cast the ToyBox to the appropriate type
  ToyBoxNDUP& box = *static_cast<ToyBoxNDUP*>(&boxB);
  
  if(!box.MainTrack) return 1;
  
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kTarget1NuECC] = box.MainTrack;
  
  for(int i = 0; i < 4; ++i) static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kTarget1NuECC][i] = box.MainTrack->PositionStart[i];
  if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget1NuECC] = box.MainTrack->GetTrueParticle()->TrueVertex;
  
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kTarget2NuECC] = box.MainTrack;
  
  for(int i = 0; i < 4; ++i) static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kTarget2NuECC][i] = box.MainTrack->PositionStart[i];
  if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget2NuECC] = box.MainTrack->GetTrueParticle()->TrueVertex;
  
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuECC] = box.MainTrack;
  
  for(int i = 0; i < 4; ++i) static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuECC][i] = box.MainTrack->PositionStart[i];
  if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuECC] = box.MainTrack->GetTrueParticle()->TrueVertex;
  
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuECC] = box.MainTrack;
  
  for(int i = 0; i < 4; ++i) static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuECC][i] = box.MainTrack->PositionStart[i];
  if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuECC] = box.MainTrack->GetTrueParticle()->TrueVertex;
  
  return true;
  
}

//**************************************************
bool TPC_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (!cc4pibox->MainTrack)
    return false;
  if(!cutUtils::DeltaLYZTPCCut(*cc4pibox->MainTrack)) return false;
  
  return true;

}

//**************************************************
bool TPCElectronPID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  // TODO implement pull cuts
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (!cc4pibox->MainTrack)
    return false;

  return ( nueCCUtils::TPCElectronPIDCut(*cc4pibox->MainTrack, lik_elec) );

}

//**************************************************
bool TPCMuonPID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (!cc4pibox->MainTrack)
    return false;

  if(cc4pibox->MainTrack->SmearedMomentum > lik_mom && cc4pibox->track_ECal_MipEM  != -999.0) return true;

  return ( nueCCUtils::TPCMuonPIDCut(*cc4pibox->MainTrack, lik_muon) );

}

//**************************************************
bool ECal_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  (void)event;

  if (!_file_ECAL_PDF) {
    std::cerr << "ERROR::ECALPIDCut:: No file found to define the PDFs for ECal variables. Will die." << std::endl;
    exit(0);
  }
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  if (!cc4pibox->MainTrack)
    return false;
  
  AnaTrueParticleB* trueParticle = cc4pibox->MainTrack->GetTrueParticle();
  if (!trueParticle) return false;
  
  bool IsEcalReco = false;
  for(int i=0; i<cc4pibox->MainTrack->nECalSegments; i++){
    if(cc4pibox->MainTrack->ECalSegments[i]->IsReconstructed){
      IsEcalReco = true;
      break;
    }
  }
  
  // Check if track is actually reconstructed in ecal
  if(!IsEcalReco or !_useECal){
    cc4pibox->track_ECal_MipEM  = -999.0;
    cc4pibox->track_ECal_EneOnL = -999.0;
    
    return true;
  }
  
  // find the first segment in an ECal
  AnaDetCrossingB* firstECal_cross = NULL;
  for (unsigned int i=0; i<trueParticle->DetCrossingsVect.size(); i++) {
    AnaDetCrossingB* cross = trueParticle->DetCrossingsVect[i];
    if (!cross->Detector_name.Contains("ECal") || !cross->Detector_name.Contains("Bar")) continue;
    firstECal_cross = cross;
  }
  
  // fill the information at ECal entry
  if (!firstECal_cross){
    cc4pibox->track_ECal_MipEM  = -999.0;
    cc4pibox->track_ECal_EneOnL = -999.0;
    
    return true;
  }


  Int_t findecal = -1;
  if (firstECal_cross->Detector_name.Contains("RightClam") &&
      firstECal_cross->Detector_name.Contains("BotLeftTopRight"))
    findecal = 1;
  else if (firstECal_cross->Detector_name.Contains("RightClam") &&
	   firstECal_cross->Detector_name.Contains("TopLeftBotRight"))
    findecal = 1;
  else if (firstECal_cross->Detector_name.Contains("LeftClam") &&
	   firstECal_cross->Detector_name.Contains("BotLeftTopRight"))
     findecal = 1;
  else if (firstECal_cross->Detector_name.Contains("LeftClam") &&
	   firstECal_cross->Detector_name.Contains("TopLeftBotRight"))
    findecal = 1;
  else if (firstECal_cross->Detector_name.Contains("LeftSide"))
    findecal = 1;
  else if (firstECal_cross->Detector_name.Contains("RightSide"))
    findecal = 1;
  else if(firstECal_cross->Detector_name.Contains("P0D/USECal"))
    findecal = -1;
  else
    findecal = 0;

  // No P0DUSEcal & switch P0D ECal
  bool useP0DECal = (bool)ND::params().GetParameterD("nueCCAnalysis.ECal.UseP0DECal");
  if(findecal < 0
    || (!useP0DECal && firstECal_cross->Detector_name.Contains("P0D"))){
    cc4pibox->track_ECal_MipEM  = -999.0;
    cc4pibox->track_ECal_EneOnL = -999.0;
    return true;
  }


  TAxis *h_binning = (TAxis*)_file_ECAL_PDF->Get("MomentumBins");
  Int_t nbins = h_binning->GetNbins();
  Double_t mombins[nbins];
  for (int i=1; i <= nbins; i++) {
    mombins[i-1] = h_binning->GetBinLowEdge(i);
  }
  
  TVector3 P   = anaUtils::ArrayToTVector3(firstECal_cross->EntranceMomentum);
  float mom    = P.Mag();
  if(mom > mombins[nbins-1])
    mom = mombins[nbins-1];

  Int_t findmombin = -1;
  for(Int_t k = 0; k < nbins-1; k++){
    if( mom > mombins[k] && mom <= mombins[k+1] ){
      findmombin = k;
      break;
    }
  }

  if(findmombin < 0){
    cc4pibox->track_ECal_MipEM  = -999.0;
    cc4pibox->track_ECal_EneOnL = -999.0;
    
    return true;
  }

  TString histname;
  if( abs(trueParticle->PDG) == 11 ){
    histname.Form("ElecHisto_%i_%i-%i",findecal,(int)mombins[findmombin],(int)mombins[findmombin+1]);
  }
  else if( abs(trueParticle->PDG) == 13 )
    histname.Form("MuonHisto_%i_%i-%i",findecal,(int)mombins[findmombin],(int)mombins[findmombin+1]);
  else if( abs(trueParticle->PDG) == 211 )
    histname.Form("PionHisto_%i_%i-%i",findecal,(int)mombins[findmombin],(int)mombins[findmombin+1]);
  else
    histname.Form("ProtHisto_%i_%i-%i",findecal,(int)mombins[findmombin],(int)mombins[findmombin+1]);

  TH1D* histo = (TH1D*)_file_ECAL_PDF->Get(histname.Data());
  cc4pibox->track_ECal_MipEM  = histo->GetRandom();
  cc4pibox->track_ECal_EneOnL = -999.0;

  delete h_binning;
  delete histo;

  if(cc4pibox->track_ECal_MipEM < 0.0) return false;

  return true;

}

//**************************************************
bool TPCVeto::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  if (!cc4pibox->MainTrack)
    return false;

  if(!cc4pibox->TPCVetoTrack) return true;

  Float_t DZ = cc4pibox->TPCVetoTrack->PositionStart[2] - cc4pibox->MainTrack->PositionStart[2];

  return (DZ > DeltaZ);

}

//**************************************************
bool ECalVeto::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  if (!cc4pibox->MainTrack)
    return false;

  if(!cc4pibox->ECalVetoTrack) return true;

  Float_t DZ = cc4pibox->ECalVetoTrack->PositionStart[2] - cc4pibox->MainTrack->PositionStart[2];

  return (DZ > DeltaZ);

}

//**************************************************
bool ToF_senseDetermination::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //**************************************************
  
  (void)event;

  ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);
  if (!box->MainTrack) return false;

  AnaTrackB* track = static_cast<AnaTrackB*>(box->MainTrack);
  Float_t true_mom = box->MainTrack->GetTrueParticle()->Momentum;

  // store beta value
  double c = 299.792458; // mm/ns
  box->beta_ToF       = track->ToF_reco_length / (c * track->ToF_reco_time);
  box->beta_true_ToF  = track->ToF_true_length / (c * track->ToF_true_time);

  // store ToF mass
  box->ToF_mass      = anaUtils::ComputeToFMass(track->SmearedMomentum,      track->ToF_reco_time,      track->ToF_reco_length);
  box->ToF_true_mass = anaUtils::ComputeToFMass(true_mom,                    track->ToF_true_time,      track->ToF_true_length);

  // get Llh
  Float_t ToF_llh[4];
  anaUtils::GetToFLikelihood(track->SmearedMomentum, track->MomentumError, track->ToF_reco_time, 
                             track->ToF_sigma_time, track->ToF_reco_length, track->ToF_sigma_length, ToF_llh, true);

  // store Llh
  box->ToF_lkl_muon     = ToF_llh[0];
  box->ToF_lkl_pion     = ToF_llh[3];
  box->ToF_lkl_electron = ToF_llh[1];
  box->ToF_lkl_proton   = ToF_llh[2];

  return true;
}

//**************************************************
bool FindPairsAction::Apply(AnaEventC& eventC, ToyBoxB& boxB) const {
  //**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 
  
  // Cast the ToyBox to the appropriate type
  ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);

  AnaTrackB* track = box->MainTrack;
  if(!track) return false;
  
  // Find the best pair track candidate
  Float_t distance = 100.0;
  box->PairTrack = nueCCUtils::FindPairTrack(event, *track, distance);
 
  return true;
}

//**************************************************
bool PairCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
  //**************************************************
  
  (void) event;
  
  // Cast the ToyBox to the appropriate type
  ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);

  AnaTrackB* track = box->MainTrack;
  if(!track) return false;

  AnaTrackB* pair = box->PairTrack;
  if(!pair) return true;

  Float_t mass = 0.511;

  // The minimum invariant mass required to pass the cut.
  TLorentzVector secondary_PE = nueCCUtils::GetMomLorentzVector(*pair, mass);
  TLorentzVector primary_PE   = nueCCUtils::GetMomLorentzVector(*track, mass);
  Float_t minv = 2. * mass * mass + 2. * (secondary_PE.Dot(primary_PE));

  if(minv < 0) return true; // backward track?

  minv = sqrt(minv);

  box->InvariantMass = minv;

  if(minv < _inv_mass_min) return false;

  return true;

}


//**************************************************
bool FindIsoTargetElectrons::Apply(AnaEventC& eventC, ToyBoxB& boxB) const {
  //**************************************************
  (void) eventC;

  ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 

  box->nIsoTargetTracks = 0;

  anaUtils::CreateArray(box->IsoTargetTracks, NMAXPARTICLES);

  // TODO maybe use segmens instead of the tracks
  for (Int_t i = 0; i < event.nParticles; ++i ) {
    if (!event.Particles[i]) continue;

    AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[i]);

    if (!anaUtils::TrackUsesOnlyDet(*track, SubDetId::kTarget1))
      continue;

    box->IsoTargetTracks[box->nIsoTargetTracks++] = track;
  }

  anaUtils::ResizeArray(box->IsoTargetTracks, box->nIsoTargetTracks);

  if (box->nIsoTargetTracks > 0) {
    std::sort(&(box->IsoTargetTracks[0]), &(box->IsoTargetTracks[box->nIsoTargetTracks-1]),  nueCCUtils::compare_length);
  }

  return true;  

}

//**************************************************
bool LengthPIDcut::Apply(AnaEventC& eventC, ToyBoxB& boxB) const {
  //**************************************************
  (void) eventC;

  ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);


  Double_t IsoTargetLength = ND::params().GetParameterD("nueCCAnalysis.IsoTargetEle.MinLength");

  // TODO maybe start using Segments instead of the tracks
  for (Int_t i = 0; i < box->nIsoTargetTracks; ++i) {
    AnaTrueParticleB* true_part = static_cast<AnaTrueParticleB*>(box->IsoTargetTracks[i]->GetTrueParticle());
    AnaTargetParticleB* target_part = static_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet(*box->IsoTargetTracks[i], SubDetId::kTarget1));

    if (!target_part || !true_part)
      continue;

    if (target_part->SegLength < IsoTargetLength)
      continue;

    // efficiency for muon/pion/proton reconstruction are pre-calculated
    if ((abs(true_part->PDG) == 211 || abs(true_part->PDG) == 13 || abs(true_part->PDG) == 2212) && !target_part->IsReconstructed)
      continue;

    if ((abs(true_part->PDG) == 211 || abs(true_part->PDG) == 13) && !target_part->IdAsProton)
      continue;

    // electron proton separation is 100% effective at the moment
    if (abs(true_part->PDG) == 11) {
      box->MainTrack = box->IsoTargetTracks[i];
      return true;
    }
    else return false;
  }

  return false;
}




//**************************************************
bool nueCCSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
  //**************************************************
	
  (void)event;
  (void)box;

  if(systId == SystId::kPileUp){
    if (branch==1) return false;
  }

  return true;

}

//**************************************************
bool nueCCSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

  if (!track) return false;
  if (!track->TrueObject) return false;

  return true;

}

//**************************************************
bool nueCCSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************
	
  (void)event;
  (void)branch;

  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);

  if (!trueTrack) return false;

  return true;

}

//**************************************************
bool nueCCSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;
  
  return true;

}

//**************************************************
bool nueCCSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;

  return true;

}

//********************************************************************
bool nueCCSelection::CheckRedoSelection(const AnaEventC& eventBB, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
  //********************************************************************

  (void)eventBB;
  (void)PreviousToyBox;
  (void)redoFromStep;

  return true;

}
