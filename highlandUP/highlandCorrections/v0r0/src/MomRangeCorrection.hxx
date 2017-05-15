#ifndef MomRangeCorrection_h
#define MomRangeCorrection_h
#include "ND280UPAnalysisUtils.hxx"
#include "CorrectionBase.hxx"
#include "DataClasses.hxx"
#include "SubDetId.hxx"


class MomRangeCorrection: public CorrectionBase {
	
public:
  
  MomRangeCorrection(SubDetId::SubDetEnum det = SubDetId::kTPC);
  virtual ~MomRangeCorrection(){}
  virtual double Sigma(AnaTPCParticleB *cross,ParticleId::ParticleEnum partID);
  virtual double SmearMomentum(AnaTPCParticleB *cross,ParticleId::ParticleEnum partID);
  virtual float ElectronMomentumCorrection(AnaTrueParticle *trueparticle);
  
  void Apply(AnaSpillC& spill);
  
protected:
  bool IsRelevantTrack(AnaTrackB& track) const; 
  SubDetId::SubDetEnum _det;

};

#endif
