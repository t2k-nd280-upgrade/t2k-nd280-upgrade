////////////////////////////////////////////////////////////
// $Id: ND280UserTrackingAction.hh,v 1.3 2004/03/18 20:49:27 t2k Exp $
//
#ifndef ND280UserTrackingAction_h
#define ND280UserTrackingAction_h 1

#include "G4UserTrackingAction.hh"
class G4Track;

class ND280UserTrackingAction : public G4UserTrackingAction
{
  public:
    ND280UserTrackingAction();
    virtual ~ND280UserTrackingAction();

    virtual void PreUserTrackingAction(const G4Track*);
};
#endif
