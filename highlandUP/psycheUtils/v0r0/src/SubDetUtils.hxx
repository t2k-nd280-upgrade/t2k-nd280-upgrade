#ifndef SubDetUtils_h
#define SubDetUtils_h

#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include "BaseDataClasses.hxx"
#include "SubDetId.hxx"

#include <bitset>

/// This namespace contains useful functions for analyses. Functions include
/// those related to fiducial volumes, accessing tracks with specific
/// characteristics, and more.
namespace anaUtils{

    /// Get the number of AnaParticle segments that use the specified detector system.
    Int_t GetNSegmentsInDet(const AnaTrackB& track, SubDetId::SubDetEnum det);

    bool HasTrackUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det);


    /// Get all the true traj. in the bunch, 
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTrajInBunch(const AnaEventB& event, AnaTrueParticleB* traj[]);
    /// Get all the true traj. in the bunch that are charged 
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllChargedTrajInBunch(const AnaEventB& event, AnaTrueParticleB* traj[]);

    /// Check whether this TrueParticle enters a given subdetector
    bool TrueParticleEntersDet(const AnaTrueParticleB*   track, SubDetId::SubDetEnum det);
    
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

    /// Whether a track uses all the specified detector systems.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    bool TrackUsesDets(const AnaTrackB& track, SubDetId::SubDetEnum dets[], int nDets);

    /// Whether a track uses the specified detector system.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    bool TrackUsesDet(const AnaTrackB& track, SubDetId::SubDetEnum det);

    /// Whether a subtrack uses the specified detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    bool TrackUsesDet(const AnaParticleB& track, SubDetId::SubDetEnum det);

    /// Get all the tracks using only a specific detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingOnlyDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]);

    /// Whether a track uses only the specified detector system.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    bool TrackUsesOnlyDet(const AnaTrackB& track, SubDetId::SubDetEnum det);

    /// Whether a subtrack uses only the specified detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    bool TrackUsesOnlyDet(const AnaParticleB& track, SubDetId::SubDetEnum det);
}
#endif
