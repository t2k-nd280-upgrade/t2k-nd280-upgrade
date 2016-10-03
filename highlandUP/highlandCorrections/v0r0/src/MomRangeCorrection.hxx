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
		virtual double Sigma(AnaTPCParticleB *cross,int pdg);
		virtual double SmearMomentum(AnaTPCParticleB *cross,int pdg);

		void Apply(AnaSpillC& spill);
    
  protected:
    bool IsRelevantTrack(const AnaTrackB& track) const; 
    SubDetId::SubDetEnum _det;

};

#endif
