#ifndef PIDCorrection_h
#define PIDCorrection_h
#include "ND280UPAnalysisUtils.hxx"
#include "CorrectionBase.hxx"
#include "DataClasses.hxx"
#include "SubDetId.hxx"


class PIDCorrection: public CorrectionBase {
	
	public:

		PIDCorrection(SubDetId::SubDetEnum det = SubDetId::kTPC);
		virtual ~PIDCorrection(){}

		void Apply(AnaSpillC& spill);
    
  protected:
    bool IsRelevantTrack(const AnaTrackB& track) const; 
    SubDetId::SubDetEnum _det;

};

#endif
