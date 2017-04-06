#ifndef ND280UserStackingAction_hh_seen
#define ND280UserStackingAction_hh_seen

#include "G4Track.hh"
#include "G4UserStackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"

/// Control which particles are actually tracked by G4.  
class ND280UserStackingAction : public G4UserStackingAction {
public:
    ND280UserStackingAction();
    virtual ~ND280UserStackingAction();

    /// Check if a new track should be tracked.
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
};
#endif
