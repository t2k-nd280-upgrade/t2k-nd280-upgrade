#ifndef CutUtils_h
#define CutUtils_h

#include "ND280UPAnalysisUtils.hxx"
#include "BaseDataClasses.hxx"

namespace cutUtils{

  /// Whether the closest TPC segment has > 18 nodes.
  bool TrackQualityCut(const AnaTrackB& track);
  bool TrackQualityCut(AnaTrackB& track);

  /// Whether the TPC segment has > 18 nodes.
  bool TPCTrackQualityCut(const AnaTPCParticleB& tpcSeg);

  /// Whether the track starts in the specified fiducial volume. FGD, TPCUp1_1, TPCUp1_2
  /// and P0D fiducial volumes are supported, as well as TrECal and DsECal active 
  /// volumes. The detector volumes are defined in DetDef::fgd1min and similar 
  /// variables. The amount by which the detector volume is shrunk to form the 
  /// fiducial volume is specified by FVdefminTPCUp1_1 and similar variables.
  bool FiducialCut(const AnaParticleB& track, const SubDetId::SubDetEnum det = SubDetId::kTPCUp1);
  bool FiducialCut(const Float_t* pos, const SubDetId::SubDetEnum det = SubDetId::kTPCUp1);

  /// Whether the track is muon-like, as defined for the NuMu analysis.
  bool MuonPIDCut(const AnaTrackB& track, bool prod5Cut = 0);

  /// Whether the track is proton-like
  bool ProtonPIDCut(const AnaParticleB& part);

  bool DeltaLYZTPCCut(AnaTrackB&);

  bool DeltaLYZTargetCut(AnaTrackB& ,const SubDetId::SubDetEnum det);


  /// Retuns true if the two tracks start within the input tolerance 
  bool CommonVertexCut(const AnaParticleB& track1, const AnaParticleB& track2, int cutx, int cuty, int cutz);
}

#endif
