///////////////////////////////////////////////////////////////
// $Id: ND280VolumeID.hh,v 1.2 2004/11/18 01:30:40 jnurep Exp $
//
#ifndef ND280VolumeID_hh_seen
#define ND280VolumeID_hh_seen

#include <vector>

#include <G4VPhysicalVolume.hh>
#include <G4TouchableHandle.hh>

/// A class to provide a unique identifier for all physical volumes.
/// One of the features of G4 is that physical volumes are identified by their
/// position in the volume tree.  This has many advantages, but it means that
/// it can be quite trick to figure out if two physical volume pointers refer
/// to the same physical volume, or different replicas of the same physical
/// volume.  (Consider the case where a you have an unreplicated sub-volume of
/// a replicated parent).  This class provides a unique comparable object that
/// allows the an equality test between volumes which takes into account the
/// full position in the hierarchy.  It is similar in function to
/// G4TouchableHandle, but provides better comparison operators.
class ND280VolumeID {
public:
    /// Construct a new volume ID.
    ND280VolumeID(const G4TouchableHandle& handle);
    /// Construct an empty volume ID.
    ND280VolumeID();
    ~ND280VolumeID();

    /// Explicitly add a new volume to the volume ID.
    void AddVolume(G4VPhysicalVolume* fHandle,int fReplica);

    bool operator !() {
        return (fVolumes.size()==0);
    };

    ND280VolumeID& operator = (const G4TouchableHandle& handle);
    ND280VolumeID& operator = (const ND280VolumeID& id);

    friend bool operator ==(const ND280VolumeID& x, 
                            const G4TouchableHandle& y);
    friend bool operator ==(const G4TouchableHandle& x,
                            const ND280VolumeID& y);
    friend bool operator ==(const ND280VolumeID& x,
                            const ND280VolumeID& y);

    friend bool operator !=(const ND280VolumeID& x, 
                            const G4TouchableHandle& y);
    friend bool operator !=(const G4TouchableHandle& x,
                            const ND280VolumeID& y);
    friend bool operator !=(const ND280VolumeID& x,
                            const ND280VolumeID& y);

    friend bool operator <(const ND280VolumeID& x,
                           const ND280VolumeID& y);

private:
    class Volume {
    public:
        G4VPhysicalVolume* fHandle;
        int fReplica;
    };
    typedef std::vector<Volume> Volumes;

    /// This is a vector of information required to uniquely ID the volume.
    /// It is arranges so that the most local information is at the front of
    /// the vector and the world volume will be the last element in the
    /// vector. 
    Volumes fVolumes;
};
#endif
