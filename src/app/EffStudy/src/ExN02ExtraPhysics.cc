#include <globals.hh>
#include <G4ParticleTypes.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleWithCuts.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>
#include <G4ParticleTypes.hh>
#include <G4ParticleTable.hh>
#include <G4Material.hh>
#include <G4ios.hh>
#include <G4StepLimiter.hh>

#include "ExN02ExtraPhysics.hh"

ExN02ExtraPhysics::ExN02ExtraPhysics() 
    : G4VPhysicsConstructor("Extra") { }

ExN02ExtraPhysics::~ExN02ExtraPhysics() { }

void ExN02ExtraPhysics::ConstructParticle() { }

void ExN02ExtraPhysics::ConstructProcess() {
  G4cout << "ExN02ExtraPhysics:: Add Extra Physics Processes" << G4endl;

G4ParticleTable::G4PTblDicIterator* ParticleIterator;

#ifndef NEW_G4
  ParticleIterator = aParticleIterator;
#else 
  ParticleIterator = GetParticleIterator();
#endif

  ParticleIterator->reset();
  while ((*ParticleIterator)()) {
    
    G4ParticleDefinition* particle = ParticleIterator->value();

    G4ProcessManager* pman = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    G4String particleType = particle->GetParticleType();
    G4double charge = particle->GetPDGCharge();
    
    if (!pman) {
      G4ExceptionDescription msg;
      msg << "Particle without a Process Manager" << G4endl;
      G4Exception("ExN02ExtraPhysics::ConstructProcess()",
		  "MyCode0002",FatalException, msg);
    }
    
    // All charged particles should have a step limiter to make sure that
    // the steps do not get too long.
    if (std::abs(charge) > 0.1) {
      pman->AddDiscreteProcess(new G4StepLimiter("Step Limit"));
    }
  }
}
