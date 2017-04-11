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

      track->SmearedMomentum = track->Momentum;
      track->MomentumError = 0;

      if (!IsRelevantTrack(*track))
	continue;
 
      AnaTrueParticle *trueP = dynamic_cast<AnaTrueParticle*>(track->GetTrueParticle());
      double length = 0.;

      for (int i=0; i<track->nTPCSegments; i++) {

	AnaTPCParticleB* TPCSegment = track->TPCSegments[i];
	if (!TPCSegment)	                   
	  continue;
	if (TPCSegment->DeltaLYZ < length)
	  continue;
	length = TPCSegment->DeltaLYZ;

	TPCSegment->SmearedMomentum = SmearMomentum(TPCSegment, ParticleId::GetParticle(trueP->PDG));
	double Momentum_dif = track->Momentum-TPCSegment->Momentum;
	track->SmearedMomentum = TPCSegment->SmearedMomentum + Momentum_dif;
	track->MomentumError = TPCSegment->MomentumError;

      }

    }
  }

}

//********************************************************************
double MomRangeCorrection::SmearMomentum( AnaTPCParticleB *cross, ParticleId::ParticleEnum partID){
//********************************************************************

  double pt_iv = anaUtils::ComputeInversePT(*cross)*units::MeV;
  double sigma = Sigma(cross,partID);

  Float_t u_t = sqrt(1-pow(cross->DirectionStart[0],2));
  double pt_iv_smeared = gRandom->Gaus(pt_iv,sigma);
  double p_smeared     = (1/pt_iv_smeared)*(1/u_t);
  
  cross->MomentumError=sigma;
  return p_smeared;
}

//********************************************************************
bool MomRangeCorrection::IsRelevantTrack(AnaTrackB& track) const{
  //********************************************************************

  return cutUtils::DeltaLYZTPCCut(track);

}

double MomRangeCorrection::Sigma(AnaTPCParticleB *cross, ParticleId::ParticleEnum partID){

  double LYZ = cross->DeltaLYZ/units::m;
  double sin_lambda = sqrt(1-pow(cross->DirectionStart[2],2));
  double cos_lambda = fabs(cross->DirectionStart[2]);
  
  int N=0;
  //if (cross->DirectionStart[1] < cross->DirectionStart[2])
    N = abs(cross->DeltaLYZ/(9.7*units::mm)) > 72 ? 72:abs(cross->DeltaLYZ/(9.7*units::mm));
  //else
    //N = abs(cross->DeltaLYZ*cross->DirectionStart[1]/(6.9*units::mm));

  if (LYZ < 1e-6)
    return 0;

  //  if     (pdg==211)  mass = 139.570; // pi+-
  //  else if(pdg==13)   mass = 105.658; // muon
  //  else if(pdg==2212) mass = 938.272; // proton
  //  else if(pdg==11)   mass = 0.511;   // electron
  double mass = anaUtils::GetParticleMass(partID);

  double bg      = cross->Momentum/mass;
  double beta   = bg / sqrt(1. + bg * bg);
  double sigma_x = ND::params().GetParameterD("highlandCorrections.mom_corrections.sigma_x");
  double B       = ND::params().GetParameterD("highlandCorrections.mom_corrections.B");
  double X0      = ND::params().GetParameterD("highlandCorrections.mom_corrections.x0");

  double p_iv = anaUtils::ComputeInversePT(*cross)*units::GeV;

  double sigma_inv_p_point = 1e-3/(300*B*LYZ*LYZ)*sigma_x*sqrt(720/(N+4));
  double sigma_inv_p_MS    = 1e-3*0.016*p_iv/(300*B*LYZ*cos_lambda)*sqrt(LYZ/X0);
  double sigma_inv_p       = sqrt(TMath::Power(sigma_inv_p_point,2)+TMath::Power(sigma_inv_p_MS,2));
  //std::cout << "sigma comp.: " << LYZ << " " << cos_lambda << " " << N << " " << 1/p_iv << " " << sigma_inv_p_point << " " << sigma_inv_p_MS << " " << sigma_inv_p << std::endl;

  return sigma_inv_p;

}
