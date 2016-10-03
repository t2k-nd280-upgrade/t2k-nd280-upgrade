#ifndef Interaction_h
#define Interaction_h

#include <TLorentzVector.h>
#include "BaseDataClasses.hxx"

//A class for grouping true trajectories together by the interaction
//that produced them.  (Same parent, same 4-position.) 
class Interaction{

public:

  //Class variables.
  //The PDG encoding of the parent particle.
  Int_t parentPDG;

  //The ID of the parent particle.
  Int_t parentID;

  //The 4-position of this interaction.
  Float_t position[4];

  //A vector containing all the trajectories leaving this interaction.
  std::vector<AnaTrueParticleB*> trajectories;

  // The parent
  AnaTrueParticleB* parent;

  //Empty Constructor.
  Interaction();

  //Constructor.  Defined by the trajectory used to initialize it.
  Interaction(AnaTrueParticleB*, AnaTrueParticleB* parent);

  //If given an AnaTrueParticle, checks whether it belongs to a given interaction.
  //(i.e. whether its start 4-position is the same as the Interaction position.)
  Bool_t IncludesTrajectory(AnaTrueParticleB*);

  // Add a trajectory to the interation
  void AddTrajectory(AnaTrueParticleB* traj, AnaTrueParticleB* parent);

  //Count the number of particles of a given PDG value.
  Int_t NumberOfParticleType(Int_t);

  //Count the number of pi0 from decay products.
  Int_t NPiZeroFromDecayProducts();

  //Count the number of exotic particles.
  Int_t CountExoticParticles();

  //Get all indices of the particles of a given PDG value in
  //the trajectories vector.
  std::vector<UInt_t> IndicesOfParticleType(Int_t);

  
};

//=====Additional helper methods for dealing with interactions.========

//For use in comparing two Interaction objects so can sort a vector of them according to the time
//they occurred at (from first to last.)
Bool_t CompareInteractions(Interaction a, Interaction b);

//highland takes the 4-momentum InitMomentum and expresses it as a TVector3 direction and a double momentum.
//This converts that information back to a 4-vector.  A 4-vector is needed to compute the
//invariant mass of pi0 decay photon pairs.
TLorentzVector GetFourMomentum(TVector3 direction, double momentum, Double_t mass);


#endif
