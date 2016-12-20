#include "PIDCorrection.hxx"
#include "CutUtils.hxx"
#include "PIDUtils.hxx"
#include "Parameters.hxx"
#include <TRandom.h>
#include "Units.hxx"
//#define UseRecPack
#ifdef  UseRecPack
#include "AnaRecPackManager.hxx"
#endif


//********************************************************************
PIDCorrection::PIDCorrection(SubDetId::SubDetEnum det) :  CorrectionBase(){
  //********************************************************************
  _det = det;
}

//********************************************************************
void PIDCorrection::Apply(AnaSpillC& spillBB) {
  //********************************************************* ***********
  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

  for (UInt_t i = 0; i < spill.Bunches.size(); i++) {
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);
    for (UInt_t j = 0; j < bunch->Particles.size(); j++) {

      AnaTrackB* track = static_cast<AnaTrackB*>(bunch->Particles[j]);
      if (!track) continue;
      
      // Check whether a track is relevant to apply the correction
      //if (!IsRelevantTrack(*track)) continue;
    AnaTrueParticle *trueP = dynamic_cast<AnaTrueParticle*>(track->GetTrueParticle());

      for (int subdet = 0; subdet < 7; subdet++) {
        AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( *track, static_cast<SubDetId::SubDetEnum >(subdet)));
        if (TPCSegment) {

          anaUtils::ComputeTPCPull(*TPCSegment,*track);
 
        }
      }

    }
  }
}

//********************************************************************
bool PIDCorrection::IsRelevantTrack(const AnaTrackB& track) const{
  //********************************************************************
  if (!SubDetId::GetDetectorUsed(track.Detector, _det)) return false;

  // Should fail TPC track quality cut
 // if (cutUtils::TrackQualityCut(track)) return false;

  // Should start or stop in the volume of interest
//  if (!anaUtils::InDetVolume(_det, track.PositionStart) && 
  //    !anaUtils::InDetVolume(_det, track.PositionEnd)) return false;  

  return true;
}
