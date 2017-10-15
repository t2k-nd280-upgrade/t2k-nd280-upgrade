#include "nueCCMultiPiSelection.hxx"
#include "nueCCSelection.hxx"
#include "AnalysisUtils.hxx"
#include "ConstituentsUtils.hxx"
#include "EventBox.hxx"
//#include "psycheNDUPUtils.hxx"
#include "PIDUtils.hxx"


//********************************************************************
nueCCMultiPiSelection::nueCCMultiPiSelection(bool forceBreak) : SelectionBase(forceBreak,  EventBoxId::kEventBoxNDUP) {
  //********************************************************************

  _nueCCSelection = NULL;
  _nueCCSelection = new nueCCSelection(forceBreak);
}


//********************************************************************
void nueCCMultiPiSelection::DefineSteps() {
  //********************************************************************

  if (!_nueCCSelection)
    return;
  
  CopySteps(*_nueCCSelection, 0, 0, 14);

  // fill pion info
  AddStep(StepBase::kAction,       "Fill TPC tracks info",      new FindPions());

  AddSplit(3);
  AddStep(0, StepBase::kCut,        "nueCC0pi cut",             new nueCC0piCut());
  AddStep(1, StepBase::kCut,        "nueCC1pi cut",             new nueCC1piCut());
  AddStep(2, StepBase::kCut,        "nueCCOther cut",           new nueCCOtherCut());

  SetBranchAlias(0, "nueCC0pi",     0);
  SetBranchAlias(1, "nueCC1pi",     1);
  SetBranchAlias(2, "nueCCOther",   2);

}

//*********************************************************************
bool FindPions::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //*********************************************************************
  
  ToyBoxCC4piMultiPi* box = static_cast<ToyBoxCC4piMultiPi*>(&boxB);  

  SubDetId::SubDetEnum det = SubDetId::kInvalid;
  if (anaUtils::InFiducialVolume(SubDetId::kTarget1, box->MainTrack->PositionStart))
    det = SubDetId::kTarget1;
  if (anaUtils::InFiducialVolume(SubDetId::kTarget2, box->MainTrack->PositionStart))
    det = SubDetId::kTarget2;
  if (anaUtils::InFiducialVolume(SubDetId::kFGD1, box->MainTrack->PositionStart))
    det = SubDetId::kFGD1;
  if (anaUtils::InFiducialVolume(SubDetId::kFGD2, box->MainTrack->PositionStart))
    det = SubDetId::kFGD2;

  nueCCUtils::FindTPCPions(event, boxB, det);
  nueCCUtils::FindIsoTargetPions(event, boxB, det);

  box->nPosPions = box->nPositivePionTPCtracks;
  box->nNegPions = box->nNegativePionTPCtracks;
  box->nOtherPions = box->nPosPi0TPCtracks + box->nElPi0TPCtracks;

  if (_AntiNu)
    box->nNegPions += box->nIsoTargetPiontracks;
  else
    box->nPosPions += box->nIsoTargetPiontracks;

  return true;  
}

//*********************************************************************
bool nueCC0piCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //*********************************************************************

  ToyBoxCC4piMultiPi* box = static_cast<ToyBoxCC4piMultiPi*>(&boxB);

  return (box->nPosPions + box->nNegPions + box->nOtherPions < 1);
}

//*********************************************************************
bool nueCC1piCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //*********************************************************************

  ToyBoxCC4piMultiPi* box = static_cast<ToyBoxCC4piMultiPi*>(&boxB);

  if (box->nPosPions + box->nNegPions + box->nOtherPions > 1)
    return false;

  if (_AntiNu)
    return (box->nNegPions == 1);
  else 
    return (box->nPosPions == 1);

  return false;
}

//*********************************************************************
bool nueCCOtherCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //*********************************************************************

  ToyBoxCC4piMultiPi* box = static_cast<ToyBoxCC4piMultiPi*>(&boxB);

  if (box->nPosPions + box->nNegPions + box->nOtherPions > 1)
    return true;

  if (_AntiNu)
    return (box->nPosPions > 0);
  else 
    return (box->nNegPions > 0);

  return false;
}

//*********************************************************************
bool nueCCUtils::FindTPCPions(AnaEventC& event, ToyBoxB& boxB, SubDetId::SubDetEnum det){
  //*********************************************************************

  ToyBoxCC4piMultiPi* box = static_cast<ToyBoxCC4piMultiPi*>(&boxB);

  box->nPositivePionTPCtracks = 0;
  box->nPosPi0TPCtracks = 0;
  box->nNegativePionTPCtracks = 0;
  box->nElPi0TPCtracks = 0;
  AnaEventB& eventB = *static_cast<AnaEventB*>(&event); 
  
  EventBox::RecObjectGroupEnum groupID;
  if      (det==SubDetId::kTarget1) groupID = EventBox::kTracksWithGoodQualityTPCInTarget1FV;
  else if (det==SubDetId::kTarget2) groupID = EventBox::kTracksWithGoodQualityTPCInTarget2FV;
  else if (det==SubDetId::kFGD1)    groupID = EventBox::kTracksWithGoodQualityTPCInFGD1FV;
  else if (det==SubDetId::kFGD2)    groupID = EventBox::kTracksWithGoodQualityTPCInFGD2FV;
  else return false;
  
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
  
  // Look for pions in positive tracks 
  for(int i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {
  
    AnaTrackB *ptrack = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
    if( box->MainTrack == ptrack ) continue; // Same as the leading track.
    if (ptrack->Momentum < 20) continue;
    
    if (ptrack->Charge>0){
      
      Float_t PIDLikelihood[4];
      anaUtils::GetPIDLikelihood(*ptrack, PIDLikelihood);
      
      // For Positive tracks we distinguish pions, electrons and protons.
      double ElLklh = PIDLikelihood[1];  
      double ProtonLklh = PIDLikelihood[2];  
      double PionLklh = PIDLikelihood[3];
      double norm = ElLklh+ProtonLklh+PionLklh;
      ProtonLklh /= norm; 
      ElLklh /= norm; 
      PionLklh /= norm; 
      
      if( ProtonLklh > ElLklh && ProtonLklh > PionLklh )
  continue;
      
      // Id associated to the largest of the two probabilities.
      if (PionLklh > ElLklh) {
  box->nPositivePionTPCtracks++;
  box->PositivePionTPCtracks.push_back(ptrack);
      }
      else {
  if (ptrack->Momentum > 900.) // This is mainly protons.
    continue;
  box->nPosPi0TPCtracks++;
  box->PosPi0TPCtracks.push_back(ptrack);
      }
    }
    
    else if (ptrack->Charge<0){
      
      // For Negative tracks we distinguish pions, electrons and protons.
      Float_t PIDLikelihood[4];
      anaUtils::GetPIDLikelihood(*ptrack, PIDLikelihood);
      
      // For Positive tracks we distinguish pions, electrons and protons.
      double ElLklh = PIDLikelihood[1];  
      double PionLklh = PIDLikelihood[3];  
      double norm = ElLklh+PionLklh;
      ElLklh /= norm; 
      PionLklh /= norm;
      
      if( PionLklh > 0.8 ) {
  box->nNegativePionTPCtracks++;
  box->NegativePionTPCtracks.push_back(ptrack);
      }
      else {
  box->nElPi0TPCtracks++;
  box->ElPi0TPCtracks.push_back(ptrack);
      }
      
    }
    
  } // end loop on tracks
   return true;
}

//*********************************************************************
bool nueCCUtils::FindIsoTargetPions(AnaEventC& event, ToyBoxB& boxB, SubDetId::SubDetEnum det){
  //*********************************************************************
  
  ToyBoxCC4piMultiPi* box = static_cast<ToyBoxCC4piMultiPi*>(&boxB);
  box->nIsoTargetPiontracks = 0;
  
  EventBox::RecObjectGroupEnum groupID;
  if      (det==SubDetId::kTarget1) groupID = EventBox::kTracksWithTarget1;
  else if (det==SubDetId::kTarget2) groupID = EventBox::kTracksWithTarget2;
  else if (det==SubDetId::kFGD1)    groupID = EventBox::kTracksWithFGD1;
  else if (det==SubDetId::kFGD2)    groupID = EventBox::kTracksWithFGD2;
  else return false;
    
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

  Double_t MIP_eff_tar     = ND::params().GetParameterD("nueCCAnalysis.IsoTargetPi.EfficiencyPID_Target");
  Double_t MIP_mis_eff_tar = ND::params().GetParameterD("nueCCAnalysis.IsoTargetPi.ContaminationPID_Target");

  Double_t MIP_eff_fgd     = ND::params().GetParameterD("nueCCAnalysis.IsoTargetPi.EfficiencyPID_FGD");
  Double_t MIP_mis_eff_fgd = ND::params().GetParameterD("nueCCAnalysis.IsoTargetPi.ContaminationPID_FGD");
    
  //loop over tracks
  for (int i=0;i<EventBox->nRecObjectsInGroup[groupID]; i++ ){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
    if (track == box->MainTrack) continue;
    
    if (!anaUtils::InFiducialVolume(det, track->PositionStart) ||
  !anaUtils::InFiducialVolume(det, track->PositionEnd) )
      continue;
        
    if (det==SubDetId::kTarget1 || det==SubDetId::kTarget2) {
      if (track->nTargetSegments != 1) continue;
      
      AnaTargetParticleB* TargetSegment = 
  dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet(*track, det));
      if (!TargetSegment) continue;
      if (!TargetSegment->IsReconstructed) continue;

      AnaTrueParticleB* true_part = track->GetTrueParticle();
      if (!true_part) continue;

      float r=gRandom->Rndm();
      float cut=0;
      if (abs(true_part->PDG) == 211 || abs(true_part->PDG) == 13)
  cut = MIP_eff_tar;
      else
  cut = MIP_mis_eff_tar;
      if (r<cut) {
  box->nIsoTargetPiontracks++;
  box->IsoTargetPiontracks.push_back(track);
      }

    }
    if (det==SubDetId::kFGD1 || det==SubDetId::kFGD2) {
      if (track->nFGDSegments != 1) continue;
    
      AnaFGDParticleB* FGDSegment = 
  dynamic_cast<AnaFGDParticleB*>(anaUtils::GetSegmentInDet(*track, det));
      if (!FGDSegment) continue;
      if (!FGDSegment->IsReconstructed) continue;
      
      AnaTrueParticleB* true_part = track->GetTrueParticle();
      if (!true_part) continue;

      float r=gRandom->Rndm();
      float cut=0;
      if (abs(true_part->PDG) == 211 || abs(true_part->PDG) == 13)
  cut = MIP_eff_fgd;
      else
  cut = MIP_mis_eff_fgd;
      if (r<cut) {
  box->nIsoTargetPiontracks++;
  box->IsoTargetPiontracks.push_back(track);
  /*
  double prange;
  ND::tman().GetMomentumFromRangeLinear(*track, prange, ParticleId::kPiPos);
  std::cout << track->Momentum << " " << prange << std::endl;
  */
      }

    }
    
  }
  return true;
}