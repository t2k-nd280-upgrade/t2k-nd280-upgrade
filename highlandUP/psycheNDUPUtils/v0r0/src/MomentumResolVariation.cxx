#include "MomentumResolVariation.hxx"
#include "ND280UPAnalysisUtils.hxx"
#include "VersioningUtils.hxx"
#include "ConstituentsUtils.hxx"


//#define DEBUG true

//********************************************************************
void MomentumResolVariation::ApplyVariation(AnaTrackB* track){
//********************************************************************
  
  if (!track) return;
  
  // Initial momenum to be varied
  Float_t p = track->Momentum;

  // Get the true momentum
  if (!track->TrueObject) return;
  
  // Then the change cannot be undone
   if(!track->Original) return;
   
  Float_t variation;
  
  if (!GetVariation(track, variation)) return;
  
  Float_t p0_true = track->GetTrueParticle()->Momentum;
  
#ifdef DEBUG
  std::cout << " MomentumResolVariation::ApplyVariation() p0, p0_true = " << p << " " << p0_true << std::endl;
#endif
    
  // Get the inverse transverse momentum 
  Float_t pt0_inv = anaUtils::ComputeInversePT(*track);
  
  
  if (pt0_inv < 0) return;
  
  // Get the true inverse transverse momentum
  Float_t pt0_inv_true  = anaUtils::ComputeInversePT(*(track->GetTrueParticle())); 
    
#ifdef DEBUG
    std::cout << " MomentumResolVariation::ApplyVariation() 1/pt0, 1/pt0_true = " << pt0_inv << " " << pt0_inv_true << std::endl;    
    std::cout << " MomentumResolVariation::ApplyVariation() var = " << variation << std::endl;
#endif
   
    // Apply momentum resolution smearing
    Float_t pt_inv = (1 + variation) * (pt0_inv - pt0_inv_true) + pt0_inv_true;
      
    // Convert back to the full momentum
    p = fabs(anaUtils::ComputeMomentumFromInversePT( *(track), pt_inv));

    if (p < 0) return;
    

    // Set the new momentum
    track->Momentum = p;
 
    // Apply the smearing to the TPC constituents
    if (track->nTPCSegments == 0) return;
     
    
    Float_t p0 = track->GetOriginalTrack()->Momentum;
    
    // Translate the smearing on the error of the local momentum and its error
    Float_t smearingFact    = (p - p0_true)/(p0 - p0_true);
    Float_t smearingFactMom = p / p0;
    
    for(int iseg = 0; iseg < track->nTPCSegments; iseg++){
      AnaTPCParticleB* tpcTrackOrig = static_cast<const AnaTrackB*>(track->Original)->TPCSegments[iseg];
      AnaTPCParticleB* tpcTrack     = track->TPCSegments[iseg];
      Float_t          tpcmomerr    = tpcTrackOrig->MomentumError;
      Float_t          tpcmom       = tpcTrackOrig->Momentum;
      
      tpcTrack->MomentumError = tpcmomerr * smearingFact;
      tpcTrack->Momentum      = tpcmom    * smearingFactMom;
    }

#ifdef DEBUG
    std::cout << " MomentumResolVariation::ApplyVariation() p = " << p << " "  << std::endl;
#endif

  
}

//********************************************************************
bool MomentumResolVariation::GetXBinnedValues(
    AnaTrackB* track, 
    Float_t& value1, 
    Float_t& value2, 
    Int_t& index1, 
    Int_t& index2, 
    ModeEnum mode){
//********************************************************************
  
  if (!track) return false;
  
  if (!_params) return false;
   
  // Use a x dependence for the moment for prod6
  Float_t x_zmin = -10000;
  Float_t x_zmax = -10000;
  Float_t zmin   = 10000;
  Float_t zmax   = -10000;
  // Get array of TPC segments, but only one per TPC, the one with more nodes
  AnaTPCParticleB* TPCSegments[3];
  Int_t nTPCSegments = anaUtils::GetOneSegmentPerTPC(track->TPCSegments, track->nTPCSegments, TPCSegments);

  // Loop over TPC segments
  for (int k = 0; k < nTPCSegments; k++) {
    AnaTPCParticleB* tpcTrack = TPCSegments[k];
   
    // Find the Most upstream TPC point in the global track
    if (tpcTrack->PositionStart[2]<zmin){
      x_zmin = tpcTrack->PositionStart[0];
      zmin = tpcTrack->PositionStart[2];
    }
    // Find the Most downstream TPC point in the global track
    if (tpcTrack->PositionEnd[2]>zmax){
      x_zmax = tpcTrack->PositionEnd[0];
      zmax = tpcTrack->PositionEnd[2];
    }
  }

  // A protection against crazy z values leading to x values not found
  if (x_zmin < -1000 || x_zmax < -1000) return false;

 
  Float_t val1_tmp, val2_tmp;
  
  // Get parameters for upstream point
  bool ok1 = mode == kCorr ? _params->GetBinValues(x_zmin, value1, val1_tmp, index1) :
    _params->GetBinValues(x_zmin, val1_tmp, value1, index1);

  // Get parameters for downstream point
  bool ok2 = mode == kCorr ? _params->GetBinValues(x_zmax, value2, val2_tmp, index2) :
    _params->GetBinValues(x_zmax, val2_tmp, value2, index2);


#ifdef DEBUG
  std::cout <<" MomentumResolVariation::GetXBinnedValues()  mode " << mode
    <<  " bin: " << x_zmin << " " << x_zmax << " " << zmin << " " << zmax 
    << " " << value1 << " " << value2 << std::endl;
#endif

  if (ok1 && ok2){
    return true;
  }
  else if (ok1 && !ok2){
    value2 = value1;
    index2 = index1;
    return true;
  }
  else if  (!ok1 && ok2){
    value1 = value2;
    index1 = index2;
    return true;
  }
  else 
    return false;

}

