#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "EventBox.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "SelectionUtils.hxx"
#include "AnaRecPackManager.hxx"

#include "numuCCOOFVSelection.hxx"
#include "numuCCOOFVUtils.hxx"

#include "TH2F.h"

#include <utility>
#include <map>

//********************************************************************
numuCCOOFVSelection::numuCCOOFVSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxNDUP) {
  //********************************************************************

  char filename[256];
  sprintf(filename, "%s/data/pdfs_dsecal.root", getenv("NUMUCCOOFVANALYSISROOT"));
  _file_ECAL_PDF = TFile::Open(filename);

  _ECal_reco_eff = new BinnedParams(std::string(getenv("NUMUCCOOFVANALYSISROOT")) + "/data",
				    "ECal_recoEff", BinnedParams::k1D_SYMMETRIC); 
  
  _ECal_FGDmatch_eff = new BinnedParams(std::string(getenv("NUMUCCOOFVANALYSISROOT")) + "/data",
					"ECal_FGDmatchEff", BinnedParams::k1D_SYMMETRIC); 

  _randomGen = new TRandom3();
  
}

//********************************************************************
void numuCCOOFVSelection::DefineSteps(){
  //********************************************************************


  //last true means that if that cut is not fulfill the sequence is stop
  AddStep(StepBase::kAction, "find true vertex",    new numuCCOOFVUtils::FindTrueVertexAction());
  AddStep(StepBase::kCut,    "> 0 tracks ",         new numuCCOOFVUtils::TotalMultiplicityCut(), true);
  AddStep(StepBase::kAction, "Sort TPC tracks",     new numuCCOOFVUtils::SortTracksAction());
  AddStep(StepBase::kCut,    "quality+fiducial",    new numuCCOOFVUtils::TrackGQandFVCut(),      true);

  AddStep(StepBase::kAction, "find vertex",         new numuCCOOFVUtils::FindVertexAction());
  AddStep(StepBase::kAction, "fill summary",        new numuCCOOFVUtils::FillSummaryAction_numuCCOOFV());

  AddSplit(2);
  AddStep(0, StepBase::kCut, "TPC PID Cut",      new numuCCOOFVUtils::TPCPIDCut());
  AddStep(1, StepBase::kCut, "ECal PID Cut",     new numuCCOOFVUtils::ECalPIDCut(_file_ECAL_PDF));

  AddStep(0, StepBase::kCut, "ToF",          new numuCCOOFVUtils::ToFCut());
  AddStep(1, StepBase::kCut, "ToF",          new numuCCOOFVUtils::ToFCut());
  
  SetBranchAlias(0,  "CCinc TPC",  0);
  SetBranchAlias(1,  "CCinc ECal", 1);
  SetPreSelectionAccumLevel(0);

}

//********************************************************************
void numuCCOOFVSelection::DefineDetectorFV(){
  //********************************************************************

  // Change FV definition to take all thickness
  // Note! this will affect all downstream stuff
  //FVDef::FVdefminTarget1[2] = 0;
  //FVDef::FVdefmaxTarget1[2] = 0;

  // The detector in which the selection is applied
  bool useTarget1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD2");

  if ( useTarget1 && !useTarget2) SetDetectorFV(SubDetId::kTarget1);
  if (!useTarget1 &&  useTarget2) SetDetectorFV(SubDetId::kTarget2);
  if ( useTarget1 &&  useTarget2) SetDetectorFV(SubDetId::kTarget);

  if ( useFGD1 && !useFGD2) SetDetectorFV(SubDetId::kFGD1);
  if (!useFGD1 &&  useFGD2) SetDetectorFV(SubDetId::kFGD2);
  if ( useFGD1 &&  useFGD2) SetDetectorFV(SubDetId::kFGD);

}

//**************************************************
void numuCCOOFVSelection::InitializeEvent(AnaEventC& eventBB){
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
bool numuCCOOFVSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
  //********************************************************************

  if(allCutsPassed[0]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[1]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[2]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  if(allCutsPassed[3]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC;
  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kTarget1NuMuCC);

}

namespace numuCCOOFVUtils{

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

    ToyBoxCCOOFV* ccoofvbox = static_cast<ToyBoxCCOOFV*>(&box);
    (void)event;

    // Retrieve the EventBoxNDUP
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

    int nu_mode = ND::params().GetParameterI("numuCCOOFVAnalysis.NeutrinoMode");

    if (useTarget1) {

      int n=EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1];
      for (Int_t i=0;i<n; ++i){
	AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget1][i]);
	if ( nu_mode==1  && track->Charge>=-0.5 ) continue;
	if ( nu_mode==-1 && track->Charge<=0.5  ) continue;
	ccoofvbox->Tracks.push_back(track);
      }

    }
    if (useTarget2) {

      int n=EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget2];
      for (Int_t i=0;i<n; ++i){
	AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget2][i]);
	if ( nu_mode==1  && track->Charge>=-0.5 ) continue;
	if ( nu_mode==-1 && track->Charge<=0.5  ) continue;
	ccoofvbox->Tracks.push_back(track);
      }

    }
   if (useFGD1) {

      int n=EventBox->nRecObjectsInGroup[EventBox::kTracksWithFGD1];
      for (Int_t i=0;i<n; ++i){
	AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithFGD1][i]);
	if ( nu_mode==1  && track->Charge>=-0.5 ) continue;
	if ( nu_mode==-1 && track->Charge<=0.5  ) continue;
	ccoofvbox->Tracks.push_back(track);
      }

    }
    if (useFGD2) {

      int n=EventBox->nRecObjectsInGroup[EventBox::kTracksWithFGD2];
      for (Int_t i=0;i<n; ++i){
	AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithFGD2][i]);
	if ( nu_mode==1  && track->Charge>=-0.5 ) continue;
	if ( nu_mode==-1 && track->Charge<=0.5  ) continue;
	ccoofvbox->Tracks.push_back(track);
      }

    }

  
    //Sort TPCGoodQuality using Momentum
    std::sort(ccoofvbox->Tracks.begin(), ccoofvbox->Tracks.end(), numuCCOOFV_utils::HGlobalMomFirst);
    if (ccoofvbox->Tracks.size()>0)
      ccoofvbox->MainTrack = ccoofvbox->Tracks[0];
    else 
      return false;

    ccoofvbox->TPC_det = anaUtils::GetClosestTPC(*ccoofvbox->MainTrack);

    return true;

  }

  //**************************************************
  bool TrackGQandFVCut::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************

    ToyBoxCCOOFV* ccoofvbox = static_cast<ToyBoxCCOOFV*>(&box);

    if (!ccoofvbox->MainTrack)
      return false;

    SubDetId::SubDetEnum det = SubDetId::kInvalid;
    if (useTarget1) det = SubDetId::kTarget1;
    if (useTarget2) det = SubDetId::kTarget2;
    if (useFGD1)    det = SubDetId::kFGD1;
    if (useFGD2)    det = SubDetId::kFGD2;

    if (anaUtils::InFiducialVolume(det, ccoofvbox->MainTrack->PositionStart, LAFVmin, LAFVmax))
      { ccoofvbox->OOFV = 0; return true; }
    if (anaUtils::InFiducialVolume(det, ccoofvbox->MainTrack->PositionEnd, LAFVmin, LAFVmax))
      { ccoofvbox->OOFV = 1; return true; }
    else
      { ccoofvbox->OOFV = 2; return true; }


    return false;
  
  }


  //**************************************************
  bool VetoAction::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************

    return true;

  }


  //Define vertex
  //**************************************************
  bool FindVertexAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
    //**************************************************

    (void)event;

    ToyBoxNDUP& box = *static_cast<ToyBoxNDUP*>(&boxB);
    ToyBoxCCOOFV* ccoofvbox = static_cast<ToyBoxCCOOFV*>(&boxB);

    // reset the vertex
    box.Vertex = NULL;
    box.TrueVertex = NULL;

    if ( !ccoofvbox->MainTrack ) return false;
    box.MainTrack = ccoofvbox->MainTrack;

    box.Vertex = new AnaVertexB();
    anaUtils::CreateArray(box.Vertex->Particles, 1);

    box.Vertex->nParticles = 0;
    box.Vertex->Particles[box.Vertex->nParticles++] = box.MainTrack;

    for(int i = 0; i < 4; ++i) box.Vertex->Position[i] = box.MainTrack->PositionStart[i];
    if ( box.MainTrack->GetTrueParticle() ) 
      box.TrueVertex = box.Vertex->TrueVertex = ccoofvbox->MainTrack->GetTrueParticle()->TrueVertex;

    return true;

  }


  //********************************************************************
  bool FillSummaryAction_numuCCOOFV::Apply(AnaEventC& event, ToyBoxB& boxB) const{
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
  bool TPCPIDCut::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    (void)event;
    ToyBoxCCOOFV* ccoofvbox = static_cast<ToyBoxCCOOFV*>(&box);

    // TPC segment case
    if ( cutUtils::DeltaLYZTPCCut(*ccoofvbox->MainTrack) ) {
      if ( numuCCOOFV_utils::MuonPIDCut(*ccoofvbox->MainTrack, false) ) 
	return true;
    }

    return false;

  }

  //**************************************************
  bool ECalPIDCut::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    (void)event;
    ToyBoxCCOOFV* ccoofvbox = static_cast<ToyBoxCCOOFV*>(&box);

    // no TPC segment case
    if ( !cutUtils::DeltaLYZTPCCut(*ccoofvbox->MainTrack) ) {

      TH2F *h_binning = (TH2F*)_file_ECAL_PDF->Get("hBinning");
      AnaTrueParticleB* trueParticle = ccoofvbox->MainTrack->GetTrueParticle();
      if (!trueParticle) return false;

      // find the first segment in an ECal
  
      AnaDetCrossingB* firstECal_cross=NULL;
      for (unsigned int i=0; i<trueParticle->DetCrossingsVect.size(); i++) {
	AnaDetCrossingB* cross = trueParticle->DetCrossingsVect[i];
	if (!cross->Detector_name.Contains("ECal") || !cross->Detector_name.Contains("Bar")) continue;
	firstECal_cross = cross;
      }

      // need a segment, otherwise it is finished
      if (!firstECal_cross)
	return false;

      // fill the information at ECal entry

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

      ccoofvbox->track_ECal_MipEM  = MipEM;
      ccoofvbox->track_ECal_EneOnL = EneOnL;
  
      // apply cuts on ECal variables
  
      if (MipEM < 0 && EneOnL < 1.25)
	return true;

    }

    return false;

  }


  //**************************************************
  bool ToFCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
    //**************************************************

    (void)event;
    ToyBoxCCOOFV *box = static_cast<ToyBoxCCOOFV*>(&boxB);
    if (!box->MainTrack) return false;

    AnaParticleB *p1=0, *p2=0;
    float sigma=0;
    float reco_ToF = numuCCOOFV_utils::GetToF(box->MainTrack, p1, p2, sigma, _randomGen);
    if (!p1 or !p2)
      return false;

    float true_ToF = p2->PositionStart[3]-p1->PositionStart[3];
    box->reco_ToF = reco_ToF;
    box->true_ToF = true_ToF;

    // ==========
    // ==========

    // if the track comes from FGD/Target and is reconstructed with correct sense
    if (box->OOFV==0 && true_ToF*reco_ToF > 0) return true; 
    // if the track comes from OOFV and stops in FGD/Target but is reconstructed with wrong sense
    if (box->OOFV>0 && true_ToF*reco_ToF < 0) return true; 

	return false;
	
  }

}
 
//**************************************************
bool numuCCOOFVSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
  //**************************************************
	
  (void)event;
  (void)box;

  if(systId == SystId::kPileUp){
    if (branch==1) return false;
  }

  return true;

}

//**************************************************
bool numuCCOOFVSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

  if (!track) return false;
  if (!track->TrueObject) return false;

  return true;

}

//**************************************************
bool numuCCOOFVSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************
	
  (void)event;
  (void)branch;

  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);

  if (!trueTrack) return false;

  return true;

}

//**************************************************
bool numuCCOOFVSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;
  return true;

}

//**************************************************
bool numuCCOOFVSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;
  return true;

}

//********************************************************************
bool numuCCOOFVSelection::CheckRedoSelection(const AnaEventC& eventBB, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
  //********************************************************************

  (void)eventBB;
  (void)PreviousToyBox;
  (void)redoFromStep;

  return true;

}
