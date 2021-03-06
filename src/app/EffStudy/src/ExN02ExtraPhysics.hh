#ifndef ExN02ExtraPhysics_hh_seen
#define ExN02ExtraPhysics_hh_seen
////////////////////////////////////////////////////////////
// $Id: ND280ExtraPhysics.hh,v 1.1 2007/01/19 20:35:22 mcgrew Exp $
////////////////////////////////////////////////////////////

#include "globals.hh"

#include "G4VPhysicsConstructor.hh"

/// A G4VPhysicsConstructor to provide extra physics processes required by the
/// ND280mc such as step length limiters.  The extra physics processes must be
/// explicitly added to every physics list (as an EM list) that is created in
/// ND280PhysicsList::AddPhysicsList(); however, this only effects code
/// actually in that method.  This class should not be used outside of
/// ND280PhysicsList.

class ExN02ExtraPhysics: public G4VPhysicsConstructor {
public:

    ExN02ExtraPhysics();
    virtual ~ExN02ExtraPhysics();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

};
#endif
