#include "MomRangeCorrection.hxx"
#include "CutUtils.hxx"
#include "BaseDataClasses.hxx"
#include "Parameters.hxx"
#include <TRandom.h>
#include "Units.hxx"
//#define UseRecPack
#ifdef  UseRecPack
#include "AnaRecPackManager.hxx"
#endif


//********************************************************************
MomRangeCorrection::MomRangeCorrection(SubDetId::SubDetEnum det) :  CorrectionBase(){
  //********************************************************************
  _det = det;
}

//********************************************************************
void MomRangeCorrection::Apply(AnaSpillC& spillBB) {
  //********************************************************* ***********
  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);
//std::cout << "Now running func " << __func__ << " at line " << __LINE__ << " of file " << __FILE__ << std::endl;

  for (UInt_t i = 0; i < spill.Bunches.size(); i++) {
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);
    for (UInt_t j = 0; j < bunch->Particles.size(); j++) {

      AnaTrackB* track = static_cast<AnaTrackB*>(bunch->Particles[j]);
      if (!track) continue;

      // Check whether a track is relevant to apply the correction
      //if (!IsRelevantTrack(*track)) continue;
      AnaTrueParticle *trueP = dynamic_cast<AnaTrueParticle*>(track->GetTrueParticle());
      double length = 0.;
      int ntpc = 0;
      for (int subdet = 0; subdet < 7; subdet++) {

        AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( *track, static_cast<SubDetId::SubDetEnum >(subdet)));
        if (TPCSegment) {
          double smearedMomentum = SmearMomentum(TPCSegment, ParticleId::GetParticle(trueP->PDG));
          TPCSegment->SmearedMomentum = smearedMomentum;
          // std::cout<<"smearing"<<std::endl;
          if (TPCSegment->DeltaLYZ > length) {
                 ntpc++;
     
            double Momentum_dif = track->Momentum - TPCSegment->Momentum;
            track->SmearedMomentum = smearedMomentum + Momentum_dif;
            track->MomentumError = TPCSegment->MomentumError;
            //    std::cout<<"smearing"<<std::endl;

          }
        }
        
      }
      if (ntpc == 0) {
          track->SmearedMomentum = track->Momentum;
          track->MomentumError = 0;

        }
    }
  }
}
//********************************************************************
double MomRangeCorrection::SmearMomentum( AnaTPCParticleB *cross, ParticleId::ParticleEnum partID){
//********************************************************************
  double sigma=Sigma( cross,partID)*cross->Momentum;

  double prob=gRandom->Gaus(cross->Momentum,sigma);
  cross->MomentumError=sigma;
  return prob;
}
//********************************************************************
bool MomRangeCorrection::IsRelevantTrack(const AnaTrackB& track) const{
  //********************************************************************
  if (!SubDetId::GetDetectorUsed(track.Detector, _det)) return false;

  // Should fail TPC track quality cut
 // if (cutUtils::TrackQualityCut(track)) return false;

  // Should start or stop in the volume of interest
//  if (!anaUtils::InDetVolume(_det, track.PositionStart) && 
  //    !anaUtils::InDetVolume(_det, track.PositionEnd)) return false;  

  return true;
}

double MomRangeCorrection::Sigma( AnaTPCParticleB *cross,ParticleId::ParticleEnum partID){
double LYZ=cross->DeltaLYZ*units::mm;
double N=cross->DeltaLYZ/ units::mm;
//double mass=0.;

//  if     (pdg==211)  mass = 139.570; // pi+-
//  else if(pdg==13)   mass = 105.658; // muon
//  else if(pdg==2212) mass = 938.272; // proton
//  else if(pdg==11)   mass = 0.511;   // electron
    double mass =anaUtils::GetParticleMass(partID);

double bg=cross->Momentum/mass;
double betta=bg / sqrt(1. + bg * bg);
double sigma_x= ND::params().GetParameterD("highlandCorrections.mom_corrections.sigma_x");
double B= ND::params().GetParameterD("highlandCorrections.mom_corrections.B");
double X0= ND::params().GetParameterD("highlandCorrections.mom_corrections.x0");
double p_iv=anaUtils::ComputeInversePT(*cross)*units::GeV;
double sigma_inv_p=sqrt(TMath::Power(1./(0.3*B*LYZ*LYZ)*sigma_x/p_iv*sqrt(720/(N+4)),2)+TMath::Power(0.2/(B*sqrt(LYZ*X0)*betta),2));
return sigma_inv_p;
}
