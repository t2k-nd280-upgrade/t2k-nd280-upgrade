#ifndef EventBox_hxx
#define EventBox_hxx

#include "BaseDataClasses.hxx"
//#include "EventBoxUtils.hxx"
#include "EventBoxId.hxx"

class EventBox:public EventBoxB{
 public :

  enum RecObjectGroupEnum{
    kTracksUnassigned=0,
    kLongTracks,      // Tracks with more than 5 hits
    kLongTracksInFV,  // Tracks with more than 5 hits in FV
    kTracksWithTPC,
    kTracksWithTPCInTarget1FV,
    kTracksWithTPCInTarget2FV,
    kTracksWithGoodQualityTPCInTarget1FV,
    kTracksWithGoodQualityTPCInTarget2FV,
    kTracksWithTPCAndTarget1,
    kTracksWithTPCAndTarget2,
    kTracksWithTarget1AndNoTPC,
    kTracksWithTarget2AndNoTPC,
    kTracksWithTarget1,
    kTracksWithTarget2,
    kTracksWithTPCorTarget1,
    kTracksWithTPCorTarget2, 
    kTracksWithECal


  };
  
  enum TrueObjectGroupEnum{
    kTrueParticlesUnassigned=0,
    kTrueParticlesChargedInTPCInBunch,
  };

  EventBox();
  virtual ~EventBox();
};


namespace boxUtils{

  /// Fill in the EventBox several arrays of tracks with Subdet2
  void FillLongTracks(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kTPCUp1);

}

#endif
