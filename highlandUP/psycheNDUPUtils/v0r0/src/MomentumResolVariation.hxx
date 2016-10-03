#ifndef MomentumResolVariation_h
#define MomentumResolVariation_h

#include "BinnedParams.hxx"

/// The code provides the functionality to smear the momentum of tracks with TPC information
/// Is a base one to the corresponding corrections and systematics

class MomentumResolVariation{
public:
 
  // Enum to distinguish if one wants to deal with the correction or systematics
  enum ModeEnum{
    kCorr, 
    kSyst
  };
  
  MomentumResolVariation(){
    _params = NULL;
  }
  
  virtual ~MomentumResolVariation() {
    if (_params) delete _params;
    _params = NULL;
  }
  
  /// Apply the variation to the track,  the variation is assumed to be an additional factor one adds to the true-rec difference of pt_inv
  /// pt_inv_var = (1 + var)(pt_inv_rec - pt_inv_true) + pt_inv_true
  /// the variation is calculated on the fly
  virtual void ApplyVariation(AnaTrackB* track);
  
  /// Abstract class to get the variation given a track
  virtual bool GetVariation(AnaTrackB* track, Float_t& variation) = 0;
  
protected:

  /// Get parameters for this global track assumed one uses X bins
  bool GetXBinnedValues(AnaTrackB* track, Float_t& value1, Float_t& value2, Int_t& index1, Int_t& index2, ModeEnum mode);
 
  
  /// Binned data to read the parameters
  BinnedParams* _params;

};

#endif
