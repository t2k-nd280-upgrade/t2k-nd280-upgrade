#ifndef trueEventBox_hxx
#define trueEventBox_hxx

#include "BaseDataClasses.hxx"
//#include "trueEventBoxUtils.hxx"
#include "EventBoxId.hxx"

class trueEventBox:public EventBoxB{
 public :

  enum RecObjectGroupEnum{
    kTracksUnassigned=0,
    kLongTracks,      // Tracks with more than 5 hits
    kLongTracksInFV   // Tracks with more than 5 hits in FV
  };
  
  enum TrueObjectGroupEnum{
    kTrueParticlesUnassigned=0,
    kTrueParticlesChargedInTPCInBunch,
  };

  trueEventBox();
  virtual ~trueEventBox();
};


namespace trueboxUtils{

  /// Fill in the trueEventBox several arrays of tracks with Subdet2
  void FillLongTracks(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kTarget1);

}

#endif
