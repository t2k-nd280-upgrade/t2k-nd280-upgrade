////////////////////////////////////////////////////////////
// $Id: ND280UserStackingAction.cc,v 1.4 2011/09/06 18:58:35 mcgrew Exp $
//

#include <globals.hh>
#include <G4ParticleDefinition.hh>
// #include <G4VProcess.hh>

#include "ND280UserStackingAction.hh"

//#include "G4SystemOfUnits.hh" // NEW GLOBAL
#include <CLHEP/Units/SystemOfUnits.h>

ND280UserStackingAction::ND280UserStackingAction() { }

ND280UserStackingAction::~ND280UserStackingAction() { }

G4ClassificationOfNewTrack
ND280UserStackingAction::ClassifyNewTrack(const G4Track* aTrack) {
    

  // // From GEANT4 examples
  // G4ClassificationOfNewTrack     classification = fUrgent;
  // // kill all secondaries
  // if(aTrack->GetParentID() != 0) classification = fKill;
  // return classification;
  
  
  if (aTrack->GetCurrentStepNumber() == 0) {
    // Get the particle type of the new track.
    const G4ParticleDefinition* particle = aTrack->GetDefinition();
    
    // This is where we can throw away particles that we don't want to
    // track.
    if (particle->GetParticleName() == "gamma") {
      if (aTrack->GetKineticEnergy() < 10.*CLHEP::keV) return fKill;
    }

    // // NEW ND280UPGRADE
    // if (particle->GetParticleName() == "neutron") {
    //   return fKill;
    // }
    // //
    
  }

    return fUrgent;
}
