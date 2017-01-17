#include "CutUtils.hxx"
#include "FiducialVolumeDefinition.hxx"
//#include "EventBoxId.hxx"
//#include "EventBoxTracker.hxx"
#include <TVector3.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "Parameters.hxx"
#include "ConstituentsUtils.hxx"

//**************************************************
bool cutUtils::TrackQualityCut(AnaTrackB& track){
//**************************************************

    if(track.TPCQualityCut > -1){
        return track.TPCQualityCut;
    }
    // Gets all segments in the closest TPC
    AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTPC(track));

    if (TPCSegment){
        bool passed = TPCTrackQualityCut(*TPCSegment);

        track.TPCQualityCut = passed;
        return passed;
    }
    else{
        track.TPCQualityCut = 0;
        return false;
    }
}


//**************************************************
bool cutUtils::TrackQualityCut(const AnaTrackB& track){
//**************************************************

    // Gets all segments in the closest TPC
    AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTPC(track));

    if (TPCSegment){
        bool passed = TPCTrackQualityCut(*TPCSegment);

        return passed;
    }
    else{
        return false;
    }
}
//**************************************************
bool cutUtils::TPCTrackQualityCut(const AnaTPCParticleB& tpcSeg){
//**************************************************

    Float_t cut = 200;  
    if (tpcSeg.DeltaLYZ > cut) 
        return true;
    else
        return false;
}

//**************************************************
bool cutUtils::MuonPIDCut(const AnaTrackB& track, bool prod5Cut){
//**************************************************
   Float_t cut1 = ND::params().GetParameterD("psycheNDUPUtils.cututils.pidmuoncut");
   Float_t cut2 = ND::params().GetParameterD("psycheNDUPUtils.cututils.pidmipcut");

    Float_t pcut = 500;

    Float_t PIDLikelihood[4];
    anaUtils::GetPIDLikelihood(track, PIDLikelihood, prod5Cut);

    if (((PIDLikelihood[0]+PIDLikelihood[3])/(1-PIDLikelihood[2]) > cut1 || track.Momentum > pcut )){//; && (PIDLikelihood[0]>cut2)){
        return true; 
    }

    return false;
}
//**************************************************
bool cutUtils::ProtonPIDCut(const AnaParticleB& part){
//**************************************************

  (void)part;
  return true;
/*
    if (anaUtils::GetPIDLikelihood(track,2)>0.9)
        return true;
    else    
        return false;
*/
}

//**************************************************
bool cutUtils::FiducialCut(const AnaParticleB& track, const SubDetId::SubDetEnum det){
//**************************************************
    if (!SubDetId::GetDetectorUsed(track.Detectors, det)) return false;
    return FiducialCut(track.PositionStart, det);
}

//**************************************************
bool cutUtils::FiducialCut(const Float_t* pos, const SubDetId::SubDetEnum det){
//**************************************************

    return anaUtils::InFiducialVolume(det,pos);
}

//**************************************************
bool cutUtils::CommonVertexCut(const AnaParticleB& track1, const AnaParticleB& track2, int cutx, int cuty, int cutz){
//**************************************************

    if (fabs(track1.PositionStart[0]-track2.PositionStart[0]) > cutx ) return false; 
    if (fabs(track1.PositionStart[1]-track2.PositionStart[1]) > cuty ) return false; 
    if (fabs(track1.PositionStart[2]-track2.PositionStart[2]) > cutz ) return false;   

    return true;
}



//**************************************************
bool cutUtils::DeltaLYZTargetCut( AnaTrackB& track,const SubDetId::SubDetEnum det) {
//**************************************************

  float targetLYZCut = ND::params().GetParameterD("psycheNDUPUtils.cututils.targetLYZCut");
  
    AnaTargetParticleB* TargetSegment = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( track, det));
    if (TargetSegment)
    { if (TargetSegment->DeltaLYZ > targetLYZCut) {
        return true;
      }
    
  }

  return false;
}


//**************************************************
bool cutUtils::DeltaLYZTPCCut(AnaTrackB& track)  {
//**************************************************


  // This cut check the existence of track with position inside the Fiducial Volume and
  // with a minimum number of hits
   float tpcLYZCut = ND::params().GetParameterD("psycheNDUPUtils.cututils.tpcLYZCut");

  // Cast the ToyBox to the appropriate type

  for (int subdet = 0; subdet < 7; subdet++) {
    AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( track, static_cast<SubDetId::SubDetEnum >(subdet)));
    if (TPCSegment) {

      if (TPCSegment->DeltaLYZ > tpcLYZCut) {
        return true;
      }
    }
  }
  return false;
}


//**************************************************
bool cutUtils::DeltaLYZTargetORTPCCut(AnaTrackB& track,const SubDetId::SubDetEnum det)  {
//**************************************************


  // This cut check the existence of track with position inside the Fiducial Volume and
  // with a minimum number of hits
   float tpcLYZCut = ND::params().GetParameterD("psycheNDUPUtils.cututils.tpcLYZCut");

  // Cast the ToyBox to the appropriate type

  for (int subdet = 0; subdet < 7; subdet++) {
    AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( track, static_cast<SubDetId::SubDetEnum >(subdet)));
    if (TPCSegment) {

      if (TPCSegment->DeltaLYZ > tpcLYZCut) {
        return true;
      }
    }
  }
  float targetLYZCut = ND::params().GetParameterD("psycheNDUPUtils.cututils.targetLYZCut");
  
    AnaTargetParticleB* TargetSegment = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( track, det));
    if (TargetSegment)
    { if (TargetSegment->DeltaLYZ > targetLYZCut) {
        return true;
      }
    }

  return false;
}


//___________________________________________________________________________ 
bool cutUtils::IsCCQE(int neut_reaction_mode)
{
  return neut_reaction_mode ==  1;
}
//___________________________________________________________________________ 
bool cutUtils::IsCC1pi(int neut_reaction_mode)
{
  bool is_cc1pi = (neut_reaction_mode == 11 ||
                   neut_reaction_mode == 12 ||
                   neut_reaction_mode == 13);
  return is_cc1pi;
}
//___________________________________________________________________________ 
bool cutUtils::IsCCcoh(int neut_reaction_mode)
{
  return neut_reaction_mode ==  16;
}
//___________________________________________________________________________ 
bool cutUtils::Is2p2h(int neut_reaction_mode)
{
  return neut_reaction_mode ==  2;
}
//___________________________________________________________________________
bool cutUtils::IsCCoth(int neut_reaction_mode)                               
{
  bool is_ccoth = (neut_reaction_mode == 17 ||
                     neut_reaction_mode == 21 ||
                     neut_reaction_mode == 22 ||
                     neut_reaction_mode == 23 ||
                     neut_reaction_mode == 26);
  return is_ccoth;
}
//___________________________________________________________________________ 
bool cutUtils::IsNC(int neut_reaction_mode)
{
  bool is_not_nc =  IsCCoth(neut_reaction_mode);
  is_not_nc = Is2p2h(neut_reaction_mode);
  is_not_nc = IsCCcoh(neut_reaction_mode);
  is_not_nc = IsCC1pi(neut_reaction_mode);
  is_not_nc = IsCCQE(neut_reaction_mode);
  
  bool is_nc = !is_not_nc;
  return is_nc;
}
//___________________________________________________________________________ 
int cutUtils::GetReacAll(int neut_reaction_mode){
  if(IsCCQE(neut_reaction_mode)){ 
    return 0;
  }
  else if(Is2p2h(neut_reaction_mode)){ 
    return 1;
  }
  else if(IsCC1pi(neut_reaction_mode)){ 
    return 2;
  }
  else if(IsCCcoh(neut_reaction_mode)){ 
    return 3;
  }
  else if(IsCCoth(neut_reaction_mode)){ 
    return 4;
  }
  else {
    return 5;
  }
}
