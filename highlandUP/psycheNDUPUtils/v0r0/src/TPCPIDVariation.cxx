#include "TPCPIDVariation.hxx"

//#define DEBUG

//**************************************************
void TPCPIDVariation::ApplyVariation(AnaTrackB* track, const ToyExperiment& exp){
  //**************************************************


#ifdef DEBUG
  std::cout << " TPCPIDVariation::ApplyVariation(): start " << std::endl;  
#endif


  // Check track validity
  if (!track) return;

  // Do not do anything if a track does use a TPC
  if (track->nTPCSegments == 0) return;

  // Loop over TPC segments
  for (int k = 0; k < track->nTPCSegments; k++) {

    if (!track->TPCSegments[k]) continue;

    // The segment to be modified
    AnaTPCParticleB* tpcTrack = track->TPCSegments[k];

#ifdef DEBUG
    std::cout << " TPCPIDVariation::ApplyVariation(): tpcSegment " << std::endl; 
    tpcTrack->Print();
#endif

    // The un-corrected TPC track t 
    const AnaTPCParticleB* original = static_cast<const AnaTPCParticleB*>(tpcTrack->Original);

    if (!track->TrueObject)           continue; //?
    if (!original)                    continue; //?
    if (original->dEdxMeas == -99999) continue; //?

    Float_t mean_var, sigma_var;

    if (!GetVariation(*tpcTrack, mean_var, sigma_var, *track, exp)) continue;

#ifdef DEBUG
    std::cout << " TPCPIDVariation::ApplyVariation(): variation params -- mean var " << mean_var 
      << " sigma_var " << sigma_var << std::endl; 
#endif


    // The expected dEdX
    Float_t dEdxExp;
    Float_t dEdxSigma;

    if (!GetPIDParams(*tpcTrack, dEdxExp, dEdxSigma, *track)) continue;

#ifdef DEBUG
    std::cout << " TPCPIDVariation::ApplyVariation(): dEdX params -- dEdxExp " << dEdxExp 
      << " dEdxSigma " << dEdxSigma << " dEdxMeas_0 " << tpcTrack->dEdxMeas <<std::endl; 
#endif      
    // Apply the systematic variation for the sigma
    // smear/queeze to match the new resolution
    Float_t dEdXMeasNew = dEdxExp + (tpcTrack->dEdxMeas - dEdxExp) * sigma_var; 
    if (dEdXMeasNew < 0) continue;
    tpcTrack->dEdxMeas  = dEdXMeasNew; 

#ifdef DEBUG
    std::cout << " TPCPIDVariation::ApplyVariation(): dEdxMeas_1 (sigma) " << tpcTrack->dEdxMeas <<std::endl; 
#endif    

    // Apply variation related to sigma
    dEdXMeasNew  = tpcTrack->dEdxMeas + (mean_var) * dEdxSigma; 
    if (dEdXMeasNew  < 0) continue;

    tpcTrack->dEdxMeas = dEdXMeasNew; 

#ifdef DEBUG
    std::cout << " TPCPIDVariation::ApplyVariation(): dEdxMeas_2 (mean) " << tpcTrack->dEdxMeas <<std::endl; 
#endif        
  }

#ifdef DEBUG
  std::cout << " TPCPIDVariation::ApplyVariation(): end \n " << std::endl;  
#endif

}

//**************************************************
bool TPCPIDVariation::GetPIDParams(const AnaTPCParticleB& tpcTrack, 
      Float_t& dEdxExp, Float_t& dEdxSigma, 
      const AnaTrackB& track){
  //**************************************************
  
  // Need true particle
  if (!track.GetTrueParticle()) return false; 

  // Get the expected dEdx and error on the dEdx depending on the true particle of the 
  // (global) track
  
  Int_t PDG = abs(track.GetTrueParticle()->PDG);
  
  switch (PDG){
    case 13: // Muon
      dEdxExp = (Float_t)tpcTrack.dEdxexpMuon;
      dEdxSigma = (Float_t)tpcTrack.dEdxSigmaMuon;
      break;    
    case 211: // Pion
      dEdxExp = (Float_t)tpcTrack.dEdxexpPion;
      dEdxSigma = (Float_t)tpcTrack.dEdxSigmaPion;
      break;
    case 11: // Electron
      dEdxExp = (Float_t)tpcTrack.dEdxexpEle;
      dEdxSigma = (Float_t)tpcTrack.dEdxSigmaEle;
      break;
    case 2212: // Proton
      dEdxExp = (Float_t)tpcTrack.dEdxexpProton;
      dEdxSigma = (Float_t)tpcTrack.dEdxSigmaProton;
      break;
    default:
      return false;
      break;
  }
  
  // Is it needed?
  if (dEdxSigma == -99999    || dEdxExp ==-99999)    return false;
  if (dEdxSigma == -0xABCDEF || dEdxExp ==-0xABCDEF) return false;

  return true; 

}



