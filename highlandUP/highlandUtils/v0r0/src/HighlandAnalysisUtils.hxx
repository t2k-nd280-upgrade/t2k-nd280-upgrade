#ifndef HighlandAnalysisUtils_h
#define HighlandAnalysisUtils_h

#include "BaseDataClasses.hxx"
#include "ND280UPAnalysisUtils.hxx"
#include "CutUtils.hxx"
#include "SubDetId.hxx"
#include "DataClasses.hxx"
#include "InputManager.hxx"
#include "VersionManager.hxx"

  /// Enum for the different productions suported
namespace ProdId{
  enum ProdEnum{
    unknown=UNKNOWN_PRODUCTION,
    MCC5,
    MCC6
  };
}

/// This namespace contains useful functions for analyses. 
/// This is in addition to those defined in psycheUtils
namespace anaUtils{

    template<size_t SIZE, class T> inline size_t size_array(T (&)[SIZE]) {
      return SIZE;
    }


    /// Get all the tracks in a given bunch using a specific detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingDet(const AnaBunchB& bunch, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the given bunch (AnaBunchB) that use the TPC and no Target, sorted by decreasing NHits.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingTPCAndNoTarget(const AnaBunchB& bunch, AnaTrackB* selTracks[]);
    int GetAllTracksUsingTPCAndNoFGD(const AnaBunchB& bunch, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the given bunch (AnaBunchB) that use the TPC or Target, not sorted.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingTPCorTarget(const AnaBunchB& bunch, AnaTrackB* selTracks[]);
    int GetAllTracksUsingTPCorFGD(const AnaBunchB& bunch, AnaTrackB* selTracks[]);

    /// Get all the tracks in a given bunch using only a specific detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingOnlyDet(const AnaBunchB& bunch, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]);

    /// Function to recompute the pull for a Target track segment including the kaon hypothesis
    void ComputeTPCPullIncludingKaon(const AnaTPCParticleB &track, Float_t* pulls);
    
    /// The likelihood of a track being a given particle hypothesis including kaons, 
    /// based on the pull values of all Target segments in the track.
    ///
    /// hypo is one of:
    /// * 0 - muon
    /// * 1 - electron
    /// * 2 - proton
    /// * 3 - pion
    /// * 4 - kaon
//    Float_t GetPIDLikelihoodIncludingKaon(const AnaTrackB& track, Int_t hypo);
    
    /// Get all likelihoods including kaons
    void GetPIDLikelihoodIncludingKaon(const AnaTrackB&, Float_t* hypo);
        
    /// An utility function that takes a set of tracks and returns a corresponding global vertex found in the event
    /// valid GV will be returned in case both tracks are its constituents (may be others),  the first found will be returned
    /// note! it is assumed the GV for an event  are ordered by PrimaryIndex, see TN-49
    AnaVertexB* GetGlobalVertex(const AnaEventB& event, AnaTrackB** Tracks, int nTracks);
    
  }
#endif


