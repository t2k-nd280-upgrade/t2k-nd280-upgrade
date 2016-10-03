#include "SecondaryInteractionSystematic.hxx"
#include "Parameters.hxx"
#include <math.h>
#include <cmath>

//#define DEBUG
//#define DDEBUG

//********************************************************************
Double_t si_syst::GetIElement(Int_t Z){
//********************************************************************
  switch(Z){    
  case 1:     //Hydrogen
    return 19.2;    
  case 5:      //Boron
    return 76.0;    
  case 6:      //Carbon
    return 78.0;    
  case 7:     //Nitrogen
    return 82.0;
  case 8:     //Oxygen
    return 95.0;    
  case 9:     //Fluorine
    return 115.0;
  case 11:    //Sodium
    return 149.0;    
  case 13:    //Aluminum
    return 166.0;
  case 14:    //Silicon
    return 173.0;    
  case 17:    //Chlorine
    return 174.0;
  case 18:    //Argon
    return 188.0;
  case 22:    //Titanium
    return 233.0;    
  case 26:    //Iron
    return 286.0;    
  case 27:    //Cobalt
    return 297.0;    
  case 29:    //Copper
    return 322.0;    
  case 30:    //Zinc
    return 330.0;    
  case 50:    //Tin
    return 488.0;    
  case 82:    //Lead
    return 823.0;    
  default:    //If unrecognized, just assume FGD scintillator.
    return 68.7;
  }    
}

//********************************************************************
Double_t si_syst::GetZoverAMaterial(TGeoNode* node){ 
//******************************************************************** 
  TGeoVolume   *volume   = node->GetVolume();
  TGeoMaterial *material = volume->GetMaterial();
  TGeoMixture  *mixture  = (TGeoMixture*)material;
  
  Double_t ZoverA = 0;
  
  // Get the number of elements
  Int_t NElements = mixture->GetNelements();
  
  // Get the Z values for the material constituents.
  Double_t* Zmixt = mixture->GetZmixt();
  
  // Get the A values for the material constituents.
  Double_t* Amixt = mixture->GetAmixt();
  
  // Get the weight by mass of the elements.
  Double_t* Wmixt = mixture->GetWmixt();

  // Compute Z/A
  for(Int_t i = 0; i < NElements; i++){
    
    ZoverA += Wmixt[i]*(Zmixt[i]/Amixt[i]);
    
  }  
  return ZoverA;
}

//********************************************************************
Double_t si_syst::GetIMaterial(TGeoNode* node){
//********************************************************************
   
  TGeoVolume   *volume   = node->GetVolume();
  TGeoMaterial *material = volume->GetMaterial();
  TGeoMixture  *mixture  = (TGeoMixture*)material;
  
  Double_t ZoverA = GetZoverAMaterial(node);
    
  Double_t lnI = 0;
  
  //Get the number of elements
  Int_t NElements = mixture->GetNelements();
  
  //Get the Z values for the material constituents.
  Double_t* Zmixt = mixture->GetZmixt();
  
  //Get the A values for the material constituents.
  Double_t* Amixt = mixture->GetAmixt();
  
  //Get the weight by mass of the elements.
  Double_t* Wmixt = mixture->GetWmixt();
  
  //lnIeff = sum(w_i*(Z_i/A_i)*lnI_i)/(Z/A)
  for(Int_t i = 0; i < NElements; i++){
    
    lnI += (Wmixt[i]*(Zmixt[i]/Amixt[i])*log(GetIElement((Int_t)Zmixt[i])))/ZoverA;
  }

  return exp(lnI);  
}



//********************************************************************
Double_t si_syst::BetheBloch(const ParticleState& state, TGeoNode* node){
  //********************************************************************

  
  TGeoVolume   *volume   = node->GetVolume();
  TGeoMaterial *material = volume->GetMaterial();
  
  // Now, set up the values that we need for the dEdX
  Double_t betagamma  = state.Momentum/state.Mass;
  Double_t energy     = sqrt(state.Momentum*state.Momentum + state.Mass*state.Mass);
  Double_t beta       = state.Momentum/energy;
  Double_t gamma      = betagamma/beta;
  Double_t T_max      = (2*units::mass_electron*pow(betagamma,2))/(1 + 2*gamma*(units::mass_electron/state.Mass) + pow(units::mass_electron/state.Mass,2));
 
  // Potential
  Double_t I       = GetIMaterial(node)*(1E-6); //Make sure to convert to MeV!

  // The conversion factor converts the density stored in the geometry to g/cm^3
  Double_t rho     = (1000.0/(6.2415e21))*(material->GetDensity());
 
  Double_t ZoverA = si_syst::GetZoverAMaterial(node);
  //0.1 to convert to mm 
  return 0.1*rho*K*1.*(ZoverA)*(1/pow(beta,2))*((1.0/2.0)*log(2*units::mass_electron*pow(betagamma,2)*T_max/pow(I,2)) - pow(beta,2));

}

//********************************************************************
void si_syst::TakeSmallStep(ParticleState& state, const Double_t& stepLength, TGeoNode* node){
  //********************************************************************

  //Find the initial kinetic energy.
  Double_t initEkin = anaUtils::ComputeKineticEnergy(state.Momentum, state.Mass);

  //Find the initial direction at the beginning of the step
  TVector3 initDir = state.Dir;

  Double_t p          = state.Momentum;
  Double_t betagamma  = p / state.Mass;
  Double_t beta       =  p /(initEkin + state.Mass);
  Double_t gamma      = betagamma / beta;
  
  Double_t dEdx = BetheBloch(state, node);

  //Remember, it is dE/dx * length in material * density of material
  //Used 1.032 g/cm^3, the density of the scintillator bars (from TN-91), as opposed
  //to the full module density which took into account the G10, glue, and fiber.
  //Factor of 0.1 to convert the mm tracklength to cm.
  Double_t finalEkin = initEkin - dEdx*(0.1)*stepLength;

  if (finalEkin <= 0 ){
    state.Dir = TVector3(0., 0., 0.);
    state.Momentum = 0.;
    return;
  }

  //Now, since we're taking small steps, we're assuming that the change in momentum is small, so we'll
  //just use the initial momentum as the feed through into the helix code.
  //The velocity of the pion in m/s)
  //PDG speed of light value: 299 792 458 m/s
  Double_t c = 299792458;
  TVector3 initVelocity = beta*c*initDir;

  //dp/dt = q(v x B), where p = \beta\gamma*mc
  //1 unit of charge is 1.602176e-19 Coulombs
  TVector3 dpdt = 1.*(1.602176e-19)*initVelocity.Cross(field);

  //So the change in momentum is therefore dpdt*dt, where dt = stepLength/initVelocity.Mag()
  //0.001 converts stepLength from mm to m.
  Double_t dt = ((0.001*stepLength)/initVelocity.Mag());
  TVector3 dp = dpdt*dt;

  //So that we're working all in the same units, use gamma*m*initVelocity to get the initial momentum back
  //(i.e. p = gamma*m*v)
  //Convert m_pi from MeV/c^2 to kg with factor of 1.782662e-30
  TVector3 pFinal = gamma*state.Mass*(1.782662e-30)*initVelocity + dp;

    //Now we know the final momentum in kg*m/s.  The Lorentz force only changes the direction and not the
  //magnitude of the momentum.  So, all we care about is the direction.
  state.Dir = pFinal*(1/pFinal.Mag());
    
  //The final position of a step is going to be
  //the result of going the step length in the direction of the initial momentum.
  //The final momentum is going to be the final kinetic energy converted back to a momentum
  //and in the direction of the final direction.
  // set new position
  state.Pos += stepLength*initDir;

  state.Momentum = sqrt(pow(finalEkin + state.Mass, 2) - pow(state.Mass, 2));

  return; 

}

// Stuff related to the full propagation taken into account the xsec etc

//********************************************************************
Double_t si_syst::ParticleSIPropagator::DScattCenters(TGeoNode* node) const{
//********************************************************************

  TGeoVolume   *volume   = node->GetVolume();
  TGeoMaterial *material = volume->GetMaterial();
  TGeoMixture  *mixture  = (TGeoMixture*)material;

  //Get the atomic number, mass (i.e. molar mass) and weight by mass arrays,
  //as well as the number of elements.
  // Double_t* ZArray  = mixture->GetZmixt();
  Double_t* AArray  = mixture->GetAmixt();
  Double_t* WArray  = mixture->GetWmixt();
  Int_t NElts       = mixture->GetNelements();
  
  //Go through and compute the number of scattering centres of each element per unit
  //volume.  Add them together to get the total number of scattering centres per
  //unit volume.
  Double_t result = 0;
  for(Int_t i = 0; i< NElts; i++)
      //Extra factor required to convert density to g/cm^3
      result += (units::Avogadro/AArray[i])*WArray[i]*((1000.0/(6.2415e21))*mixture->GetDensity()); 
  
  //All constituents having now been looped over, return the value.
  return result; 
}



//********************************************************************
void si_syst::ParticleSIPropagator::Propagate(ParticleHistory& particle, const TVector3& finalPos, const SubDetId::SubDetEnum& det) const{
  //********************************************************************

  // Do nothing if a stae is not in VOI
  if (!InVOI(particle.stateCurrent.Pos, det)) return;
  
  // Do nothing if no interactions are available
  if (_intTypes.size() == 0) return;
  
  
#ifdef DDEBUG
  TGeoVolume *volume        = node->GetVolume();
  TGeoMaterial *material    = volume->GetMaterial();
  std::string materialName  = material->GetName();
  TGeoMixture *mixture      = (TGeoMixture*)material;
#endif

  //Loop over taking steps until the separation from start to end point
  //is the is the same from oaAnalysis saved and from stepping.
  
  Double_t initPointSep = (finalPos - particle.stateCurrent.Pos).Mag();
  Double_t stepPointSep = 0.0;

  // Reference point
  TVector3 initPos = particle.stateCurrent.Pos;

#ifdef DDEBUG
  std::cout << " ======= " << std::endl;
  std::cout << " si_syst::ParticleSIPropagator::Propagate: "  << std::endl;
  particle.Print();
  std::cout << " finalPos: "      << finalPos    << std::endl;
  std::cout << " initPointSep: "  << initPointSep << std::endl;
 
#endif

   
  // Also need to save the momentum of the particle at the last saved point.
  Double_t lastSavedMomentum  = particle.stateCurrent.Momentum;

  // A variable to store the step length that is to be saved.
  // (in mm, is the number of small steps in a saved step, multiplied by the step length.)
  Double_t totalStepLength = 0;

  while (stepPointSep < initPointSep){
    //Grab the material information for this step.
    //We use the material at the step start position for the step's material.
    //(Steps are small enough that this should be pretty accurate.)
    TGeoNode *node            = ND::hgman().GeoManager()->FindNode(particle.stateCurrent.Pos.X(), 
        particle.stateCurrent.Pos.Y(), particle.stateCurrent.Pos.Z());

    // Store the momentum before the step
    Double_t momStartStep = particle.stateCurrent.Momentum;

#ifdef DDEBUG
    std::cout << " Start of step: " << std::endl;
    particle.Print();
    std::cout << " momStartStep: "  << momStartStep << std::endl; 
#endif
 
    // Given the information supplied, take a small step.
    TakeSmallStep(particle.stateCurrent, _lengthStepSize, node);
    
    totalStepLength += _lengthStepSize; // increment the total length


    // Also compute the separation between the step end point and the initial point.
    stepPointSep = (particle.stateCurrent.Pos - initPos).Mag();

#ifdef DDEBUG
    std::cout << " End of step: " << std::endl;
    particle.Print();
    std::cout << " stepPointSep: "  << stepPointSep         << std::endl; 
    std::cout << " material : "     << materialName         << std::endl; 
#endif
    
    
    // Define the number density of scattering centres for the material (mixture) 
    Double_t NDMat = DScattCenters(node); // nuclei/cm^3

   
    if (particle.stateCurrent.Momentum == 0. || !InVOI(particle.stateCurrent.Pos, det)){
      
      std::vector<SIXSecData> data_tmp;
      
      // Loop over the interactions of interest
      std::vector<si_syst::InteractionType>::const_iterator iter = _intTypes.begin();
      for (; iter != _intTypes.end(); iter++){
      
        // Get the cross-section
        Double_t xsec   = GetCrossSection(*iter, momStartStep, particle.trueTrack->PDG, node);
       
        // And error
        Double_t xsec_err   = GetCrossSectionError(*iter, momStartStep, particle.trueTrack->PDG, node);
       
        Double_t factor = NDMat*xsec*totalStepLength*1.e-3; // length to cm and xsec to cm
        
        data_tmp.push_back(SIXSecData(factor, xsec_err));
      
#ifdef DDEBUG
      
        TGeoVolume *volume        = node->GetVolume();
        TGeoMaterial *material    = volume->GetMaterial();
        std::string materialName  = material->GetName();
        TGeoMixture *mixture      = (TGeoMixture*)material;
      
        std::cout << " out of VOI/zero-mom,  int: "   << *iter            << std::endl;
        std::cout << " out of VOI/zero-mom,  xsec: "  << xsec             << std::endl;
        std::cout << " material: "                    << materialName     << std::endl; 
        std::cout << " material Z: "                  << mixture->GetZ()  << std::endl; 
        std::cout << " material A: "                  << mixture->GetA()  << std::endl; 
        std::cout << " momStartStep : "               << momStartStep     << std::endl;
        std::cout << " NDMat: "                       << NDMat            << std::endl;
        std::cout << " totalStepLength: "             << totalStepLength  << std::endl;
        std::cout << " factor: "                      << factor           << std::endl;
#endif
      }
       
      particle.propSteps.push_back(data_tmp); 
      
#ifdef DDEBUG
      std::cout << " ======= " << std::endl;
#endif 
      
      return;
    }

    //Only save a point if the momentum has changed by step size
    //Also save a point if we're going to break out of the while loop.
    if (fabs(lastSavedMomentum - particle.stateCurrent.Momentum) >= _momStepSize
        || stepPointSep >= initPointSep){
      
      std::vector<SIXSecData> data_tmp;
      
      // Loop over the interactions of interest
      std::vector<si_syst::InteractionType>::const_iterator iter = _intTypes.begin();
      for (; iter != _intTypes.end(); iter++){

        // Get the cross-section
        Double_t xsec   = GetCrossSection(*iter, momStartStep, particle.trueTrack->PDG, node);
       
        // And error
        Double_t xsec_err   = GetCrossSectionError(*iter, momStartStep, particle.trueTrack->PDG, node);
       
        Double_t factor = NDMat*xsec*totalStepLength*1.e-3; // length to cm and xsec to cm
        
        data_tmp.push_back(SIXSecData(factor, xsec_err));
      
#ifdef DDEBUG
      
        TGeoVolume *volume        = node->GetVolume();
        TGeoMaterial *material    = volume->GetMaterial();
        std::string materialName  = material->GetName();
        TGeoMixture *mixture      = (TGeoMixture*)material;
      
        std::cout << " out of VOI/zero-mom,  int: "   << *iter            << std::endl;
        std::cout << " out of VOI/zero-mom,  xsec: "  << xsec             << std::endl;
        std::cout << " material: "                    << materialName     << std::endl; 
        std::cout << " material Z: "                  << mixture->GetZ()  << std::endl; 
        std::cout << " material A: "                  << mixture->GetA()  << std::endl; 
        std::cout << " momStartStep : "               << momStartStep     << std::endl;
        std::cout << " NDMat: "                       << NDMat            << std::endl;
        std::cout << " totalStepLength: "             << totalStepLength  << std::endl;
        std::cout << " factor: "                      << factor           << std::endl;
#endif
      
      
      }
      
      particle.propSteps.push_back(data_tmp); 

      lastSavedMomentum  = particle.stateCurrent.Momentum;
      totalStepLength = 0.;
    }

  }//End while loop.

#ifdef DDEBUG
  std::cout << " ======= " << std::endl;
#endif 
 

  return;

}

// SI manager

//********************************************************************
si_syst::SISystInput* si_syst::ParticleSIManager::CollectParticleInteractions(AnaTrueParticleB** allTrajs, int nTrajs, 
    const SubDetId::SubDetEnum& det) const{
//********************************************************************

#ifdef DEBUG
  std::cout << " ======= " << std::endl;
  std::cout << " si_syst::ParticleSIManager::CollectParticleInteractions " << std::endl;
#endif 
  
  // This will create a collection of particles` histories with the corresponding interactions etc
  si_syst::SISystInput* result = GetRelevantParticles(allTrajs, nTrajs, det);

  for (int i = 0; i < result->nParticles; i++){
     
    // Fill the steps information for this proton
    _propagator->Propagate(result->Particles[i], result->Particles[i].trueTrack->PositionEnd, det);

    // Set the uncertainty at the interaction point
   result->Particles[i].IntXSecError = _propagator->GetCrossSectionError(result->Particles[i].intType, 
       result->Particles[i].stateCurrent, result->Particles[i].trueTrack->PDG);
    
  }
  
#ifdef DEBUG
  std::cout << " ======= " << std::endl;
#endif 
  return result;
}

// SI manager

//********************************************************************
si_syst::SISystInput* si_syst::ParticleSIManager::GetRelevantParticles(AnaTrueParticleB** allTrajs, int nTrajs, 
    const SubDetId::SubDetEnum& det, int PDG) const{
//********************************************************************

#ifdef DEBUG
  std::cout << " ======= " << std::endl;
  std::cout << " si_syst::ParticleSIManager::GetRelevantParticles " << std::endl;
#endif 
  
  si_syst::SISystInput* result = new si_syst::SISystInput();

  //We want to count all primary protons that originate in particular volume 
  //and group them (if exist) with the daughters of their probable interactions
  si_syst::ParentsAndDaughters ParticlesAndInteractions;

  for (Int_t it = 0; it < nTrajs; it++){

    AnaTrueParticleB* track = allTrajs[it];

    if (track->ParentID != 0)
      continue;


    if (track->PDG != PDG)
      continue;

    // Should start in the VOI
    if (!_propagator->InVOI(track->Position, det)) continue;

    ParticlesAndInteractions.push_back(std::make_pair(track, std::vector<AnaTrueParticleB*>()));

    // Now loop over trajectories and find those that mention this proton as parent 
    // those daughtes should start in the volume of interest as well
    for (Int_t jt = 0; jt < nTrajs; jt++){
      AnaTrueParticleB* track2 = allTrajs[jt];

      if ((track->ID == track2->ParentID) && 
          _propagator->InVOI(track->Position, det)){
        
        // Add this trajectory to interaction list
        ParticlesAndInteractions.back().second.push_back(track2);

      }

    } //second loop over true tracks
  }//first loop over true tracks

  //now we have all primary protons grouped together with their possible daughters
  //we can so arrange them into interactions

  result->nParticles = ParticlesAndInteractions.size();

  result->Particles = new si_syst::ParticleHistory[result->nParticles]; 

  
  /// Fill the interaction types
  for(Int_t i = 0; i < result->nParticles; i++){

    result->Particles[i] = si_syst::ParticleHistory(si_syst::kInelastic, *(ParticlesAndInteractions[i].first));

    // No interactions in the volume
    if (ParticlesAndInteractions[i].second.size() == 0)
      result->Particles[i].intType = si_syst::kNoInteraction;  

    // Elastic
    else if(ParticlesAndInteractions[i].second.size() == 1 && ParticlesAndInteractions[i].second[0]->PDG == PDG)
      result->Particles[i].intType = si_syst::kElastic;
    
    // The rest
    else 
      result->Particles[i].intType = si_syst::kInelastic;
  }
  
#ifdef DEBUG
  std::cout << " ======= " << std::endl;
#endif 
  return result;
}




//********************************************************************
std::ostream& operator<<(std::ostream& os, const TLorentzVector& vect){
//********************************************************************
  os << " X " << vect.X() << " Y " << vect.Y() << " Z " << vect.Z() << " T " << vect.T() << std::endl;
  return os;
}

//********************************************************************
std::ostream& operator<<(std::ostream& os, const TVector3& vect){
//********************************************************************
  os << " X " << vect.X() << " Y " << vect.Y() << " Z " << vect.Z() << std::endl;
  return os;
}


