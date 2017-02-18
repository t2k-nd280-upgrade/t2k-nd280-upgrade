#include "numuCC4piSelection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "EventBox.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "SelectionUtils.hxx"
#include "numuCC4piUtils.hxx"

#include "TH2F.h"


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

  _randomGen = new TRandom3();
}

//********************************************************************
void numuCC4piSelection::DefineSteps(){
  //********************************************************************
  int branch=ND::params().GetParameterI("numuCC4piAnalysis.Branch");

  //last true means that if that cut is not fulfill the sequence is stop
  AddStep(StepBase::kAction, "find true vertex",      new numuCC4piUtils::FindTrueVertexAction());
  AddStep(StepBase::kCut,    "true vertex in target", new numuCC4piUtils::TrueVertexInTargetCut(), true);

  AddStep(StepBase::kCut,    "> 0 tracks ",         new numuCC4piUtils::TotalMultiplicityCut(), true);
  AddStep(StepBase::kAction, "Sort TPC tracks",     new numuCC4piUtils::SortTracksAction());
  AddStep(StepBase::kCut,    "quality+fiducial",    new numuCC4piUtils::TrackGQandFVCut(),      true);
  //AddStep(StepBase::kAction, "veto Action",         new VetoAction());

  AddStep(StepBase::kAction, "find vertex",         new numuCC4piUtils::FindVertexAction());
  AddStep(StepBase::kAction, "fill summary",        new numuCC4piUtils::FillSummaryAction_numuCC4pi());


  if (branch == 0) {
    AddStep(StepBase::kCut, "Fwd TPC Quality Cut",  new numuCC4piUtils::FwdTPC_Quality());
    AddStep(StepBase::kCut, "Fwd TPC PID Cut",      new numuCC4piUtils::FwdTPC_PID());
  }

  /*
    AddSplit(3,0);
    AddStep(0, 1, StepBase::kCut, "CSFGD2 PID Cut",   new CSFGD2_PID());
    AddStep(0, 1, StepBase::kCut, "CSFGD2 4pi Cut",   new CSFGD2_4pi());
    AddStep(0, 2, StepBase::kCut, "CSECAL PID Cut",   new CSECAL_PID());
    AddStep(0, 2, StepBase::kCut, "CSECAL 4pi Cut",   new CSECAL_4pi());
  */

  if (branch == 1) {
    AddStep(StepBase::kCut, "Bwd TPC Quality Cut",    new numuCC4piUtils::BwdTPC_Quality());
    AddStep(StepBase::kCut, "Bwd TPC PID Cut",        new numuCC4piUtils::BwdTPC_PID());
  }

  if (branch == 2) {
    AddStep(StepBase::kCut, "HA TPC Quality Cut",    new numuCC4piUtils::HATPC_Quality());
    AddStep(StepBase::kCut, "HA TPC PID Cut",        new numuCC4piUtils::HATPC_PID());
  }

  if (branch == 3) {
    AddStep(StepBase::kCut, "ECal Quality Cut",  new numuCC4piUtils::ECal_Quality(_randomGen, _ECal_reco_eff, _ECal_FGDmatch_eff));
    AddStep(StepBase::kCut, "ECal PID Cut",      new numuCC4piUtils::ECal_PID(_file_ECAL_PDF));
  }

  
  //SetBranchAlias(0, "Fwd TPC",  0);
  //SetBranchAlias(1, "Bwd TPC",  1);
  //SetBranchAlias(2, "HA TPC",   2);
  //SetBranchAlias(3, "ECal",     3);

  SetBranchAlias(0, "test");
  
  //SetBranchAlias(4, "CSFGD2", 0, 1);
  //SetBranchAlias(5, "CSECAL", 0, 2);

  //if first two cuts are not fulfill dont throw toys
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

  if ( useTarget1 && !useTarget2) SetDetectorFV(SubDetId::kTarget1);
  if (!useTarget1 &&  useTarget2) SetDetectorFV(SubDetId::kTarget2);
  if ( useTarget1 &&  useTarget2) SetDetectorFV(SubDetId::kTarget);

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
  //boxUtils::FillTrajsChargedInFGDAndNoTPC(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  //boxUtils::FillTrajsChargedHATracker(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));

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
	     (useTarget2 && cutUtils::FiducialCut(box.Vertex->Position, SubDetId::kTarget2)) );
  
  }

  //**************************************************
  bool TotalMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************

    (void)box;

    // Check we have at least one reconstructed track in the FGD
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

    return ((useTarget1 && EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1]>0) ||
	    (useTarget2 && EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget2]>0));

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
	if ( track->Charge!=-1 ) continue;

	cc4pibox->TPCTracks.push_back(track);
      }

    }
    if (useTarget2) {

      int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV];
      for (Int_t i=0;i<nTPC; ++i){
	AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV][i]);
	if ( track->Charge!=-1 ) continue;

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
	   (useTarget2 && anaUtils::InFiducialVolume(SubDetId::kTarget2, track->PositionStart)) ) {
	if ( SubDetId::GetDetectorUsed(track->Detector,SubDetId::kDsECal)) continue;
	if ( cutUtils::DeltaLYZTPCCut(*track) )                            continue;
	if ( track->Charge!=-1 ) continue;
	cc4pibox->ECalTracks.push_back(track);
      }
    }
    //Sort ECalTracks using RangeMomentum
    std::sort(cc4pibox->ECalTracks.begin(), cc4pibox->ECalTracks.end(), numuCC4pi_utils::HGlobalMomFirst);


    if      (cc4pibox->TPCTracks.size()>0  && cc4pibox->ECalTracks.size()==0)
      cc4pibox->MainTrack = cc4pibox->TPCTracks[0];
    else if (cc4pibox->TPCTracks.size()==0 && cc4pibox->ECalTracks.size()>0)
      cc4pibox->MainTrack = cc4pibox->ECalTracks[0];
    else if (cc4pibox->TPCTracks.size()>0  && cc4pibox->ECalTracks.size()>0)
      if (cc4pibox->TPCTracks[0]->Momentum > cc4pibox->ECalTracks[0]->Momentum)
	cc4pibox->MainTrack = cc4pibox->TPCTracks[0];
      else
	cc4pibox->MainTrack = cc4pibox->ECalTracks[0];
    else return false;  

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
	  cc4pibox->TPC_det == SubDetId::kTPCDown2) )
      return true;
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
	
	// if long enough TPC track, it should be selected by other branches
    if (cutUtils::DeltaLYZTPCCut(*cc4pibox->MainTrack))
      return false;

	double mom = cc4pibox->MainTrack->Momentum;
	double cos = cc4pibox->MainTrack->DirectionStart[2];
	float reco_eff, FGDmatch_eff;
	float reco_err, FGDmatch_err;
	
	if (!_ECal_reco_eff->GetBinValues(mom, reco_eff, reco_err)) return false;
	if (!_ECal_FGDmatch_eff->GetBinValues(cos, FGDmatch_eff, FGDmatch_err)) return false;

	// throw two random numbers between 0 and 1
	double r_eff[2];
	_randomGen->RndmArray(2, r_eff);
	
	// select artificially only a fraction of the events, 
	// by applying the reconstruction/FGD-ECal match efficiencies
	if (r_eff[0] < reco_eff && r_eff[1] < FGDmatch_eff)
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
}
  
/*
  bool CSFGD2_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box); 
  if (cc4pibox->CSFD2Tracks_PID.size()>0) return true;
  return false;
  }
  bool CSFGD2_4pi::Apply(AnaEventC& event, ToyBoxB& box) const{
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box); 
  if (cc4pibox->CSFD2Tracks_PID.size()>0 && cc4pibox->FwdTracks_PID.size()==0 && cc4pibox->BwdTracks_PID.size()==0 && cc4pibox->HAFwdTracks_PID.size()==0 && cc4pibox->HABwdTracks_PID.size()==0) return true;
  return false;
  }

  bool CSECAL_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box); 
  if (cc4pibox->CSECALTracks_PID.size()>0) return true;
  return false;
  }
  bool CSECAL_4pi::Apply(AnaEventC& event, ToyBoxB& box) const{
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
  */
  
  return true;

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
