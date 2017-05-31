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

  char filename[256];
  sprintf(filename, "%s/data/ECalPID.root", getenv("NUECCANALYSISROOT"));
  _file_ECAL_PDF = TFile::Open(filename);

  // Get charge confusion splines
  sprintf(filename, "%s/data/FGD_1_ChargeConfusion_TrueMom.root", getenv("NUECCANALYSISROOT"));
  _file_charge_confusion_1 = TFile::Open(filename);
  
  sprintf(filename, "%s/data/FGD_2_ChargeConfusion_TrueMom.root", getenv("NUECCANALYSISROOT"));
  _file_charge_confusion_2 = TFile::Open(filename);

  // Ecal
  _ECal_reco_eff = new BinnedParams(std::string(getenv("NUECCANALYSISROOT")) + "/data",
				    "ECal_recoEff", BinnedParams::k1D_SYMMETRIC); 
  
  _ECal_FGDmatch_eff = new BinnedParams(std::string(getenv("NUECCANALYSISROOT")) + "/data",
					"ECal_FGDmatchEff", BinnedParams::k1D_SYMMETRIC); 

  _randomGen = new TRandom3();
  
}

//********************************************************************
void nueCCSelection::DefineSteps(){
  //********************************************************************


  //last true means that if that cut is not fulfill the sequence is stop
  AddStep(StepBase::kAction, "find true vertex",      new FindTrueVertexAction());

  AddStep(StepBase::kCut,    "> 0 tracks ",           new TotalMultiplicityCut(), true);
  
  // Apply Charge confusion correction
  AddStep(StepBase::kAction, "Charge Confusion",      new ApplyChargeConfusion(_file_charge_confusion_1, _file_charge_confusion_2));

  // Find the main track
  AddStep(StepBase::kAction, "Sort TPC tracks",       new SortTracksAction());

  // FV and quality
  AddStep(StepBase::kCut,    "quality+fiducial",      new TrackGQandFVCut(),      true);

  AddStep(StepBase::kAction, "find vertex",           new FindVertexAction());
  AddStep(StepBase::kAction, "fill summary",          new FillSummaryAction_nueCC());
  //AddStep(StepBase::kAction, "find pions",            new FindPionsAction());

  // This is added temporary to make sure that the vertex is indeed generated in the target
  // It seems that there is some confusion (bug?) where the true vertex position is
  AddStep(StepBase::kCut,    "Vertex quality",        new TrueVertexInTargetCut(), true);

  // Track length cut
  AddStep(StepBase::kCut,    "TPC Quality Cut",       new TPC_Quality());

  // ToF info
  AddStep(StepBase::kAction, "ToF",                   new ToF_senseDetermination());

  // TPC+ECal PID
  AddStep(StepBase::kCut,    "TPC Electron PID Cut",  new TPCElectronPID());
  AddStep(StepBase::kCut,    "ECal PID Cut",          new ECal_PID(_file_ECAL_PDF));
  AddStep(StepBase::kCut,    "TPC Muon PID Cut",      new TPCMuonPID()); // For events not going in the Ecal

  // Pair cut
  AddStep(StepBase::kAction, "find best e+e- pair",   new FindPairsAction());
  AddStep(StepBase::kCut,    "Pair Veto",	      new PairCut());

  // TPC Veto
  AddStep(StepBase::kCut,    "TPC Veto Cut",          new TPCVeto());

  SetBranchAlias(0,  "CCNuE Inclusive");
  SetPreSelectionAccumLevel(2);

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

  boxUtils::FillTracksWithTPC(event,    static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithTarget(event, static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithFGD(event,    static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
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
  //ToyBoxNDUP& box = *dynamic_cast<ToyBoxNDUP*>(&boxB);
  ToyBoxCC4pi* box = static_cast<ToyBoxCC4pi*>(&boxB);
  AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(box->MainTrack->GetTrueParticle()->TrueVertex);

  if(!vtx) return false;

  bool TrueVtxFV = false;
  int Detector_tmp;
  convUtils::ConvertBitFieldToTrueParticleDetEnum(vtx->Detector, Detector_tmp);
  if(useTarget1 && Detector_tmp == SubDetId::kTarget1) TrueVtxFV = true;
  if(useTarget2 && Detector_tmp == SubDetId::kTarget2) TrueVtxFV = true;
  if(useFGD1    && Detector_tmp == SubDetId::kFGD1)    TrueVtxFV = true;
  if(useFGD2    && Detector_tmp == SubDetId::kFGD2)    TrueVtxFV = true;

  // If the selected track is not a primary then accept the event. This is to make sure that events coming from OOFV photons are properly included
  if(!TrueVtxFV){
    AnaTrueParticleB* trueParticle = box->MainTrack->GetTrueParticle();
    if(trueParticle){
      if(trueParticle->ParentPDG != 0)
	TrueVtxFV = true;
    }
  }

  return TrueVtxFV;
  
  //return ( (useTarget1 && cutUtils::FiducialCut(box.Vertex->Position, SubDetId::kTarget1)) ||
  //	   (useTarget2 && cutUtils::FiducialCut(box.Vertex->Position, SubDetId::kTarget2)) ||
  //	   (useFGD1    && cutUtils::FiducialCut(box.Vertex->Position, SubDetId::kFGD1   )) ||
  //	   (useFGD2    && cutUtils::FiducialCut(box.Vertex->Position, SubDetId::kFGD2   )));
  
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
  
  //========================
  //Find TPCGoodQuality tracks in Fwd and Bwd
  //========================
  if (useTarget1) {
    
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV][i]);

      if(selectpositrons){
	if(track->Charge <= 0.5) continue;
      }
      else{
	if ( track->Charge>=-0.5 ) continue;
      }
      
      cc4pibox->TPCTracks.push_back(track);
    }
    
  }
  if (useTarget2) {
    
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV][i]);

      if(selectpositrons){
	if(track->Charge <= 0.5) continue;
      }
      else{
	if ( track->Charge>=-0.5 ) continue;
      }
      
      cc4pibox->TPCTracks.push_back(track);
    }
    
  }
  if (useFGD1) {
    
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD1FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD1FV][i]);

      if(selectpositrons){
	if(track->Charge <= 0.5) continue;
      }
      else{
	if ( track->Charge>=-0.5 ) continue;
      }
      
      cc4pibox->TPCTracks.push_back(track);
    }
    
  }
  if (useFGD2) {
    
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD2FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD2FV][i]);

      if(selectpositrons){
	if(track->Charge <= 0.5) continue;
      }
      else{
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
  AnaTrackB* ecalonlytracks[500];
  Int_t nEcalOnly = anaUtils::GetAllTracksUsingOnlyDet(static_cast<const AnaEventB&> (event), SubDetId::kBrlECal, ecalonlytracks);
  for(Int_t i=0; i<nEcalOnly; i++){
    AnaTrackB* ecaltrack = ecalonlytracks[i];
    if(ecaltrack->PositionStart[2] == -999) continue;
    if(!cc4pibox->ECalVetoTrack)
      cc4pibox->ECalVetoTrack = ecaltrack;
    else{
      if(ecaltrack->PositionStart[2] < cc4pibox->ECalVetoTrack->PositionStart[2])
	cc4pibox->ECalVetoTrack = ecaltrack;
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
  if(!cutUtils::DeltaLYZTPCCut(*cc4pibox->MainTrack)) return false;
  
  return true;

}

//**************************************************
bool TPCElectronPID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  return ( nueCCUtils::TPCElectronPIDCut(*cc4pibox->MainTrack, lik_elec) );

}

//**************************************************
bool TPCMuonPID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

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
  if(!IsEcalReco){
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
  else if(firstECal_cross->Detector_name.Contains("POD/USECal"))
    findecal = -1;
  else
    findecal = 0;

   // No P0DEcal
  if(findecal < 0){
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

  if(!cc4pibox->TPCVetoTrack) return true;

  Float_t DZ = cc4pibox->TPCVetoTrack->PositionStart[2] - cc4pibox->MainTrack->PositionStart[2];

  return (DZ > DeltaZ);

}

//**************************************************
bool ToF_senseDetermination::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //**************************************************
  
  (void)event;

  ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);
  if (!box->MainTrack) return false;
  
  // do not consider for truly forward-going track
  //if (box->MainTrack->TrueObject && box->MainTrack->GetTrueParticle()->Direction[2] > 0)
  //return true;
  
  AnaParticleB *p1=0, *p2=0;
  float sigma=0;
  float ToF = nueCCUtils::GetToF(box->MainTrack, p1, p2, sigma, _randomGen);

  if (!p1 || !p2)    return true;
  if (ToF == -999.0) return true;
  
  float true_ToF = p2->PositionStart[3]-p1->PositionStart[3];
  
  // ==========
  // ToF mass only calculated for the main track
  // ==========
  
  // smear the momentum
  float true_mom = box->MainTrack->GetTrueParticle()->Momentum;
  float mom = box->MainTrack->SmearedMomentum;
  if (fabs(true_mom-mom)<1e-3)
    mom = _randomGen->Gaus(true_mom, 0.10*true_mom);
  
  // compute the length using RecPack + smear it
  double length1 = -999.0, length2 = -999.0;
  RP::State state1, state2;
  TVector3 pos  = anaUtils::ArrayToTVector3(box->MainTrack->PositionStart);
  TVector3 pos1 = anaUtils::ArrayToTVector3(p1->PositionStart);
  TVector3 pos2 = anaUtils::ArrayToTVector3(p2->PositionStart);
  TVector3 dir1 = anaUtils::ArrayToTVector3(p1->DirectionStart);
  TVector3 dir2 = anaUtils::ArrayToTVector3(p2->DirectionStart);	 
 
  if (!ND::tman().AnaTrueParticleB_to_RPState(*(box->MainTrack->GetTrueParticle()), state1) || !ND::tman().PropagateToSurface(pos1, dir1, state1, length1, false) ){
    if ((pos-pos1).Mag()<2) length1=0;
  }
  if (!ND::tman().AnaTrueParticleB_to_RPState(*(box->MainTrack->GetTrueParticle()), state2) || !ND::tman().PropagateToSurface(pos2, dir2, state2, length2, false) ){
    if ((pos-pos2).Mag()<2) length2=0;
  }

  if (length1>-1 && length2>-1) {
    float sigma_length = 10;
    if (cutUtils::DeltaLYZTPCCut(*(box->MainTrack))) sigma_length = 1;

    float true_length = fabs(length2-length1);
    float length      = _randomGen->Gaus(true_length, sigma_length); 
    float m_reco      = nueCCUtils::ComputeToFMass(mom, ToF, length);
    float m_true      = nueCCUtils::ComputeToFMass(true_mom, true_ToF, true_length);

    box->ToF_mass = m_reco;
    box->ToF_true_mass = m_true;
    
    float invp = 1/mom;
    float dinvp = box->MainTrack->MomentumError;
    if (dinvp==0) dinvp=0.10*invp;

    // expected mass
    float m = 0;
    // factor which is defined here to clarify the code
    float a = 1+pow(mom/m_reco, 2);
    // resolution on reconstructed mass
    float sigma_m = m_reco*sqrt(pow(dinvp/invp,2)+pow(a*sigma/ToF,2)+pow(a*sigma_length/length,2));

    // compute the pull for each particle hypothesis
    m = units::pdgBase->GetParticle(13)->Mass()*1000; // muon
    box->ToF_pull_muon = (m_reco-m)/sigma_m;
    m = units::pdgBase->GetParticle(211)->Mass()*1000; // pion
    box->ToF_pull_pion = (m_reco-m)/sigma_m;
    m = units::pdgBase->GetParticle(11)->Mass()*1000; // electron
    box->ToF_pull_electron = (m_reco-m)/sigma_m;
    m = units::pdgBase->GetParticle(2212)->Mass()*1000; // proton
    box->ToF_pull_proton = (m_reco-m)/sigma_m;

    // compute the likelihood for each particle hypothesis
    float P_mu = exp(-pow(box->ToF_pull_muon,2)/2);
    float P_pi = exp(-pow(box->ToF_pull_pion,2)/2);
    float P_e  = exp(-pow(box->ToF_pull_electron,2)/2);
    float P_p  = exp(-pow(box->ToF_pull_proton,2)/2);
    float sum_prob = P_mu+P_pi+P_e+P_p;
    box->ToF_lkl_muon = P_mu/sum_prob;
    box->ToF_lkl_pion = P_pi/sum_prob;
    box->ToF_lkl_electron = P_e/sum_prob;
    box->ToF_lkl_proton = P_p/sum_prob;
  }
    
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
bool ApplyChargeConfusion::Apply(AnaEventC& event, ToyBoxB& boxB) const {
  //**************************************************

  if(!_applychargeconfusion) return true;

  (void)boxB;
  
  // Retrieve the EventBoxNDUP
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

  TSpline3 *elecspline_1 = (TSpline3*)_file_charge_confusion_1->Get("ElecSpline");
  TSpline3 *muonspline_1 = (TSpline3*)_file_charge_confusion_1->Get("MuonSpline");
  TSpline3 *pionspline_1 = (TSpline3*)_file_charge_confusion_1->Get("PionSpline");
  TSpline3 *protspline_1 = (TSpline3*)_file_charge_confusion_1->Get("ProtSpline");

  TSpline3 *elecspline_2 = (TSpline3*)_file_charge_confusion_2->Get("ElecSpline");
  TSpline3 *muonspline_2 = (TSpline3*)_file_charge_confusion_2->Get("MuonSpline");
  TSpline3 *pionspline_2 = (TSpline3*)_file_charge_confusion_2->Get("PionSpline");
  TSpline3 *protspline_2 = (TSpline3*)_file_charge_confusion_2->Get("ProtSpline");

  TRandom3 random_gen;

  if (useTarget1) {  
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV][i]);
      if(!track->GetTrueParticle()) continue;
      Double_t mom = (Double_t)track->Momentum;
      // Charge confusion only estimated up to 5 GeV
      if(mom > 5000.0)
	mom = 5000.0;

      Double_t prob = 0.0;

      if(abs(track->GetTrueParticle()->PDG) == 11)
	prob = elecspline_1->Eval(mom);
      else if(abs(track->GetTrueParticle()->PDG) == 13)
	prob = muonspline_1->Eval(mom);
      else if(abs(track->GetTrueParticle()->PDG) == 211)
	prob = pionspline_1->Eval(mom);
      else if(abs(track->GetTrueParticle()->PDG) == 2212)
	prob = protspline_1->Eval(mom);
    
      if(prob <= 0.0) continue;

      random_gen.SetSeed(mom*mom + track->Momentum*track->Momentum);      
      Double_t rand = random_gen.Uniform(10000)/10000.0;

      if(rand < prob)
	track->Charge = -track->Charge;
    }
  }

  if (useTarget2) {
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV][i]);
      if(!track->GetTrueParticle()) continue;
      Double_t mom = (Double_t)track->SmearedMomentum;
      Double_t prob = 0.0;

      if(abs(track->GetTrueParticle()->PDG) == 11)
	prob = elecspline_1->Eval(mom);
      else if(abs(track->GetTrueParticle()->PDG) == 13)
	prob = muonspline_1->Eval(mom);
      else if(abs(track->GetTrueParticle()->PDG) == 211)
	prob = pionspline_1->Eval(mom);
      else if(abs(track->GetTrueParticle()->PDG) == 2212)
	prob = protspline_1->Eval(mom);
    
      if(prob <= 0.0) continue;

      random_gen.SetSeed(mom*mom + track->Momentum*track->Momentum);      
      Double_t rand = random_gen.Uniform(10000)/10000.0;

      if(rand < prob)
	track->Charge = -track->Charge;
    }
  }

  if (useFGD1) {
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD1FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD1FV][i]);
      if(!track->GetTrueParticle()) continue;
      Double_t mom = (Double_t)track->SmearedMomentum;
      Double_t prob = 0.0;

      if(abs(track->GetTrueParticle()->PDG) == 11)
	prob = elecspline_1->Eval(mom);
      else if(abs(track->GetTrueParticle()->PDG) == 13)
	prob = muonspline_1->Eval(mom);
      else if(abs(track->GetTrueParticle()->PDG) == 211)
	prob = pionspline_1->Eval(mom);
      else if(abs(track->GetTrueParticle()->PDG) == 2212)
	prob = protspline_1->Eval(mom);
    
      if(prob <= 0.0) continue;

      random_gen.SetSeed(mom*mom + track->Momentum*track->Momentum);      
      Double_t rand = random_gen.Uniform(10000)/10000.0;

      if(rand < prob)
	track->Charge = -track->Charge;
    }
  }

  if (useFGD2) {   
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD2FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD2FV][i]);
      if(!track->GetTrueParticle()) continue;
      Double_t mom = (Double_t)track->SmearedMomentum;
      Double_t prob = 0.0;

      if(abs(track->GetTrueParticle()->PDG) == 11)
	prob = elecspline_1->Eval(mom);
      else if(abs(track->GetTrueParticle()->PDG) == 13)
	prob = muonspline_1->Eval(mom);
      else if(abs(track->GetTrueParticle()->PDG) == 211)
	prob = pionspline_1->Eval(mom);
      else if(abs(track->GetTrueParticle()->PDG) == 2212)
	prob = protspline_1->Eval(mom);
    
      if(prob <= 0.0) continue;

      random_gen.SetSeed(mom*mom + track->Momentum*track->Momentum);      
      Double_t rand = random_gen.Uniform(10000)/10000.0;

      if(rand < prob)
	track->Charge = -track->Charge;
    }
  }
  
  delete elecspline_1;
  delete muonspline_1;
  delete pionspline_1;
  delete protspline_1;
  
  delete elecspline_2;
  delete muonspline_2;
  delete pionspline_2;
  delete protspline_2;
  
  return true;
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
