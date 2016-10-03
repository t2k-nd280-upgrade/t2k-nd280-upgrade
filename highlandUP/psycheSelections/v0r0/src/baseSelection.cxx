#include "baseSelection.hxx"
//#include "ND280AnalysisUtils.hxx"
#include "BasicUtils.hxx"
//#include "CutUtils.hxx"

//********************************************************************
baseSelection::baseSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxNDUP) {
//********************************************************************  
}

//********************************************************************
void baseSelection::DefineSteps(){
//********************************************************************

  AddStep(StepBase::kCut, "event quality",      new EventQualityCut(),           true);
  SetBranchAlias(0,"trunk");

}

//********************************************************************
void baseSelection::DefineDetectorFV(){
//********************************************************************

}
//**************************************************
bool EventQualityCut::Apply(AnaEventC& eventBB, ToyBoxB& box) const{
//**************************************************

  (void)box;

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

 // if(event.GetIsMC())  return true;              // This is MC, ignore DQ. 

//  if (enableDQCut) {
  //  if(!event.DataQuality->GoodDaq ) return false;  // Bad ND280 DQ
 // }

//  if (enableBeamQualityCut) {
  //  if(!event.Beam->GoodSpill ) return false;      // Bad Spill
 // }

  return true;
}

//********************************************************************
bool baseSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************
  
  (void)event;
  (void)allCutsPassed;

  return 0;
}
//**************************************************
void baseSelection::InitializeEvent(AnaEventC& eventBB){
//**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxNDUP])
    event.EventBoxes[EventBoxId::kEventBoxNDUP] = new EventBox();

  boxUtils::FillLongTracks(event,       static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  
  //  boxUtils::FillTrajsChargedInSubdet2(event);
  //  boxUtils::FillTrajsChargedInSubdet1AndNoSubdet2(event, static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
 }
