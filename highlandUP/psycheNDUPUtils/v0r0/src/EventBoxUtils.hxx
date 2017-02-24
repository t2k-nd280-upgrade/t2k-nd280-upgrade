#ifndef BoxUtils_h
#define BoxUtils_h

#include "ND280UPAnalysisUtils.hxx"
#include "BaseDataClasses.hxx"
#include "EventBoxId.hxx"
#include "EventBox.hxx" 
namespace boxUtils{
  
  /// Fill in the EventBox several arrays of tracks with TPC
  void FillTracksWithTPC(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kTarget);

  /// Fill in the EventBox the array of tracks using Target
  void FillTracksWithTarget(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kTarget);

  /// Fill in the EventBox the array of tracks using FGD
  void FillTracksWithFGD(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kFGD);

  /// Fill in the EventBox the array of true tracks passing through TPC
  void FillTrajsChargedInTPC(AnaEventB& event);

  /// Fill in the EventBox the array of true tracks passing through Target and no TPC
  void FillTrajsChargedInTargetAndNoTPC(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kTarget);

  /// Fill in the EventBox the array of true tracks passing through FGD and no TPC
  void FillTrajsChargedInFGDAndNoTPC(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kFGD);

  /// Fill in the EventBox the array of true tracks passing through Target or TPC
  void FillTrajsChargedInTPCorTarget(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kTarget);

  /// Fill in the EventBox the array of true tracks passing through FGD or TPC
  void FillTrajsChargedInTPCorFGD(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kFGD);

 /// Fill in the EventBox the arrays of true tracks relevant for high-angle business: Target-ECal and Target-ECal-SMRD
//  void FillTrajsChargedHATracker(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kTarget);
  /// Fill in the EventBox the arrays of true tracks relevant for ECal systematics
  void FillTracksWithECal(AnaEventB& event);


}

#endif
