#ifndef INGRID_TRACKING_ACTION_H
#define INGRID_TRACKING_ACTION_H 1

#include "G4UserTrackingAction.hh"
#include "IngridRunAction.hh"
#include "IngridEventAction.hh"

#include "IngridSimParticleSummary.h"

class IngridTrackingAction : public G4UserTrackingAction {

public:
  IngridTrackingAction(IngridRunAction*,IngridEventAction*);
  virtual ~IngridTrackingAction();
   
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);

private:
  IngridRunAction* runaction;
  IngridEventAction* eventaction;
  IngridSimParticleSummary *simpart;
  int Tracking_Flag;

    float posi[3];
    float momi[3];

	double initE;
};

#endif
