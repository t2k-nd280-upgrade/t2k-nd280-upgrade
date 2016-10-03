#ifndef TPCPIDVariation_h
#define TPCPIDVariation_h

#include "BinnedParams.hxx"
#include "BaseDataClasses.hxx"

class ToyExperiment;


/// This variation that smears that the measured dEdXMeas (CT) for the TPC segments of the track
/// The smearing is done assuming the variation based on pull parameters
class TPCPIDVariation{
public:
  
  /// Enum for particle hypothesis
  enum HypEnum{
    kMuon = 0, 
    kElectron, 
    kProton, 
    kPion, 
    kKaon, 
    kNPULLS
  };
  
  
  
  /// Instantiate the PID variation,  
  TPCPIDVariation(){
    for (unsigned int i = 0; i < kNPULLS; i++){
      _mean[i]   = NULL;
      _sigma[i]  = NULL;
    } 
  };
  
  virtual ~TPCPIDVariation() {
    for (unsigned int i = 0; i < kNPULLS; i++){
      if (_mean[i])  delete _mean[i];   _mean[i]   = NULL;
      if (_sigma[i]) delete _sigma[i];  _sigma[i]  = NULL;
    }
  }
  
  /// Apply variation for a track, the most general case given a certain ToyExperiment
  virtual void ApplyVariation(AnaTrackB* track, const ToyExperiment& exp);
  
  /// Get the variation, two params: one for mean and one for sigma
  /// Sigma_var: is the param to control how one wants to scale the pull sigma: (pull_sigma_new/pull_sigma_old)
  /// Mean_var: the shift one wants to apply to pull mean:  pull_mean_new - pull_mean_old
  /// In principle should be enough to have it for the TPC segment but in the "regular"
  /// TPC PID syst code the corresponding truth track and momentum are retrived from the global track
  /// so keep it as a parameter as well
  /// Toy experiment is used to retrieve the variation
  virtual bool GetVariation(const AnaTPCParticleB& tpcTrack, 
      Float_t& mean_var, Float_t& sigma_var, 
      const AnaTrackB& track, const ToyExperiment& exp) = 0;
  
  /// Get the expected dEdX and error, keep it virtual so that one can easily add new hypothesis, e.g. kaons
  /// Global track is also provided so that one can use it for example to retrieve the PID info
  virtual bool GetPIDParams(const AnaTPCParticleB& tpcTrack, 
      Float_t& dEdxExp, Float_t& dEdxSigma, 
      const AnaTrackB& track); 

protected:
  
  /// Expect that the derived classes should use some data to store the params for the variations
  /// so keep the data-storage here
  BinnedParams* _mean[kNPULLS]; // data for pull mean: a correction (shift) or its error
  BinnedParams* _sigma[kNPULLS]; // data for pull simga: a correction (scaling ) or its error
};

#endif
