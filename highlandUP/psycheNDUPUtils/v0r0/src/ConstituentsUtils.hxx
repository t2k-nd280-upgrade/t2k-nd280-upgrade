#ifndef ConstituentsUtils_h
#define ConstituentsUtils_h

#include "DetectorDefinition.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "AnalysisUtils.hxx"

/// This namespace contains useful functions for analyses. Functions include
/// those related to fiducial volumes, accessing tracks with specific
/// characteristics, and more.
namespace anaUtils{

  //----- Utility functions -------------

  /// Whether the specified position is in the volume of the given detector.
  /// Accepted detectors are kTPC_1, kTPC_2, kP0D, kDSECal, k(Top, Bottom, Left, Right)T(P)ECal (or SMRD)
  bool InDetVolume(SubDetId::SubDetEnum det, const Float_t* pos);

  /// Whether the specified position is in the fiducial volume of the given detector.
  /// Accepted detectors are kTPC_1, kTPC_2, kP0D, kDSECal, k(Top, Bottom, Left, Right)T(P)ECal (or SMRD)
  /// The fiducial volume is specified using the FVdefmin and FVdefmax vectors. These
  /// vectors are the amount of space to *exclude*, compared to the nominal side of
  /// the detector.
  bool InFiducialVolume(SubDetId::SubDetEnum det, const Float_t* pos, const Float_t* FVdefmin, const Float_t* FVdefmax);
  bool InFiducialVolume(SubDetId::SubDetEnum det, const Float_t* pos);


  /// Return the detector in which the position is.
  SubDetId::SubDetEnum GetDetector(const Float_t* pos);


  Int_t GetOneSegmentPerTPC(AnaTPCParticleB* in[], Int_t nseg, AnaTPCParticleB* out[]);

  /// For tracks that start in the TPC, get the closest Target in the direction of the track.
  SubDetId::SubDetEnum GetClosestTPC(const AnaTrackB& track);

  /// Get the vector of AnaParticle segment that uses the specified detector.
  /// See SubDetId::SubDetEnum for the detector numbering convention.
  /// Return the number of entries in the input array, the number of tracks found
  int GetSegmentsInDet(const AnaTrackB& track, SubDetId::SubDetEnum det, AnaParticleB* selTracks[]);

  /// Method to get the subtrack with most nodes in a given detector
  AnaParticleB* GetSegmentWithMostNodesInDet(const AnaTrackB& track, SubDetId::SubDetEnum det);

  /// Combined function to address NuMu selection needs as efficiently as possible - gets the Target segment with the most nodes in the Target closest to the start of the global track
  AnaParticleB* GetSegmentWithMostNodesInClosestTPC(const AnaTrackB& track);

  /// Smae as above but closest to a given point
  AnaParticleB* GetSegmentWithMostNodesInClosestTPC(const AnaTrackB& track, const Float_t* pos);

  /// Get the AnaParticle segment that uses the specified detector.
  /// See SubDetId::SubDetEnum for the detector numbering convention.
  AnaParticleB* GetSegmentInDet(const AnaTrackB& track, SubDetId::SubDetEnum det);

  bool HasTrackUsingTarget(const AnaEventB& event);
  bool HasTrackUsingFGD(const AnaEventB& event);

  int GetAllChargedTrajInTPCInBunch(const AnaEventB& event, AnaTrueParticleB* traj[]);
  /// Get all the true traj. in the bunch that are charged and crosses the FGD
  /// Returns the number of entries in the input array, the number of tracks found
  int GetAllChargedTrajInTargetInBunch(const AnaEventB& event, AnaTrueParticleB* traj[],SubDetId::SubDetEnum det);
  int GetAllChargedTrajInFGDInBunch(const AnaEventB& event, AnaTrueParticleB* traj[],SubDetId::SubDetEnum det);

  int GetAllBigEnoughChargedTrajInTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]);
  /// Get all the true traj. in the bunch that are charged and crosses the Target and the TPC (Target_1-TPC_1, TPC_1-Target_2, Target_2-TPC_2, TPC_2-Target_3)
  /// Returns the number of entries in the input array, the number of tracks found
  int GetAllChargedTrajInTPCTargetInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]);
  int GetAllChargedTrajInTPCFGDInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]);
  /// Get all the true traj. in the bunch that are charged and crosses the the TPC but not the Target
  /// Returns the number of entries in the input array, the number of tracks found
  int GetAllChargedTrajInTargetAndNoTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[],SubDetId::SubDetEnum det);
  int GetAllChargedTrajInFGDAndNoTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[],SubDetId::SubDetEnum det);

  /// Get all the tracks using a specific detector.
  /// See SubDetId::SubDetEnum for the detector numbering convention.
  /// Returns the number of entries in the input array, the number of tracks found
  int GetAllTracksUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]);
  int GetAllTracksUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det, std::vector<AnaTrackB*>& selTracks);
  int GetAllTracks(const AnaEventB& event, AnaTrackB* selTracks[]);

  /// Get all the tracks not using a specific detector.
  /// See SubDetId::SubDetEnum for the detector numbering convention.
  /// Returns the number of entries in the input array, the number of tracks found
  int GetAllTracksNotUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]);

  /// Access function to get all the tracks in the bunch that use the TPC, sorted by decreasing momentum.
  /// Returns the number of entries in the input array, the number of tracks found
  int GetAllTracksUsingTPC(const AnaEventB& event, AnaTrackB* selTracks[]);

  /// Access function to get all the tracks in the bunch that use the Target, sorted by decreasing momentum.
  /// Returns the number of entries in the input array, the number of tracks found
  int GetAllTracksUsingTarget(const AnaEventB& event, AnaTrackB* selTracks[]);
  int GetAllTracksUsingFGD(const AnaEventB& event, AnaTrackB* selTracks[]);
  /// Access function to get all the tracks in the bunch that use the ECAL, sorted by decreasing momentum.
  /// Returns the number of entries in the input array, the number of tracks found
  int GetAllTracksUsingECAL(const AnaEventB& event, AnaTrackB* selTracks[]);

  /// Access function to get all the tracks in the bunch that use the Target or the TPC, sorted by decreasing number of hits
  /// Returns the number of entries in the input array, the number of tracks found
  int GetAllTracksUsingTPCorTarget(const AnaEventB& event, AnaTrackB* selTracks[]);
  int GetAllTracksUsingTPCorFGD(const AnaEventB& event, AnaTrackB* selTracks[]);

  /// Access function to get all the tracks in the bunch that use the TPC and no Target, sorted by decreasing NHits.
  /// Returns the number of entries in the input array, the number of tracks found
  int GetAllTracksUsingTargetAndNoTPC(const AnaEventB& event, AnaTrackB* selTracks[],SubDetId::SubDetEnum fgddet);
  int GetAllTracksUsingFGDAndNoTPC(const AnaEventB& event, AnaTrackB* selTracks[],SubDetId::SubDetEnum fgddet);

  /// Returns the number of tracks using both the Target and the subdetector 'det'.
  int GetNTracksUsingTPCAndDet(const AnaEventB& event, SubDetId::SubDetEnum det);

  /// Return an integer corresponding to the array index of the track in the old local detector enumeration
  int GetLocalDetEnum(SubDetId::SubDetEnum det, int i);


}
#endif

//  LocalWords:  ifndef
