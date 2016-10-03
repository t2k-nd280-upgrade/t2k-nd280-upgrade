#ifndef SecondaryInteractionSystematic_h
#define SecondaryInteractionSystematic_h

#include "AnalysisUtils.hxx"

#include "HEPConstants.hxx"
#include "Units.hxx"

//For geometry stuff.
#include "GeometryManager.hxx"
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoVolume.h>
#include <TGeoMaterial.h>

#include <string>

/// A general class to do various business related to secondary interactions: retrieve particles of intereset
/// and




namespace si_syst{
  /// Interaction types,  keep a wide list here, one can add what needed
  enum InteractionType{
    kNoInteraction = 0, // particle does not interact in the volume of interest
    kElastic, 
    kInelastic 
  };
  
  /// Basic structures to be able to associate a track with its "relatives"
  typedef std::vector<std::pair<AnaTrueParticleB*, std::vector<AnaTrueParticleB*> > > ParentsAndDaughters;
  typedef std::vector<std::pair<AnaTrueParticleB*, std::vector<AnaTrueParticleB*> > >::iterator ParentsAndDaughtersIt;
  
  
  /// A simple structure to represent a state object: position, direction, momentum, charge
  struct ParticleState{
    
    ParticleState(){}
    
    ParticleState(const AnaTrueParticleB&){}
    TVector3 Pos;
    TVector3 Dir;
    Int_t Charge; // needed for propagation
    Float_t Momentum; 
    Float_t Mass; // needed for propagation

  };
  
  
  /// A simple structure to repesent a pair: xsection value and its uncertainty
  struct SIXSecData{
    
    SIXSecData(Double_t xsec, Double_t err){
      XSec    = xsec;
      XSecErr = err;
    }
    Float_t XSec;
    Float_t XSecErr;
 
  };
  
 
  /// The magnetic field is 0.2 T in the x direction --> used for stepping --> keep it general
  const TVector3 field = TVector3(0.2, 0., 0.);
    
  /// Bethe-Bloch constants
  const Double_t K = 0.307075;
 
  /// Bethe-Bloch dEdX given particle state + TGeoNode (the particle is currently crossing) to specify material properties
  Double_t BetheBloch(const ParticleState&);
  
  /// Bethe-Bloch dEdX given particle state + TGeoNode (the particle is currently crossing) to specify material properties
  /// this is not to read the node if already avaiable
  Double_t BetheBloch(const ParticleState&, TGeoNode*);
 
  /// Propagate a state in a small step
  void TakeSmallStep(ParticleState&, const Double_t&);
  
  /// Propagate a state in a small step provided the node (material properties) and a step
  /// this is not to read the node if already avaiable
  void TakeSmallStep(ParticleState&, const Double_t&, TGeoNode*);
 
  
  /// ZoverA, material properties retrieved from node
  Double_t GetZoverAMaterial(TGeoNode*);
  
  /// Calculates the effective mean excitation energy for
  /// Equation taken from Leo p.29.
  /// Material properties retrieved from node
  Double_t GetIMaterial(TGeoNode* node);

  /// Takes in an atomic number and returns the Mean Ionization Potential for
  /// that element, from http://physics.nist.gov/PhysRefData/XrayMassCoef/tab1.html
  /// Only filled for elements present in ND280UserDetectorConstruction.cc
  /// Uses units of eV.  Must be converted to MeV later.
  Double_t GetIElement(Int_t Z);


  
  /// A simple class to store all the info of a true particle relevant for SI systematics evaluation
  class ParticleHistory{
    
    public:
      /// Each step is charactarized with the corresponding n*sigma*step_length and uncertainty value (!)
      /// various channels can be involved --> hence a vector
      typedef std::vector<SIXSecData> StepInfo;
      
      /// A default ctor
      ParticleHistory(){}
      
      virtual ~ParticleHistory(){}
      
      
      /// A ctor given the interaction type and true particle
      ParticleHistory(const  InteractionType& type, const AnaTrueParticleB& track){
        intType      = type;
        stateStart   = ParticleState(track);
        trueTrack    = &track;
        IntXSecError = 0.;
        Reset();
      }
      
      void Print(){}
      
      void Reset(){
        // Reset the current state
        stateCurrent = stateStart;
        
        // Clear the steps
        propSteps.clear();
        propSteps.reserve(200);
        
        // Reset the error
        IntXSecError = 0.;
      }
       
      /*!
       * Interaction type:
       */
      InteractionType intType;
     
      /// Initial state of the particle
      ParticleState stateStart;
      
      /// Current state of the particle
      ParticleState stateCurrent;
      
      /// Vector of steps considered with the corresponding info
      std::vector<StepInfo> propSteps;
      
      /// A pointer to the original true track
      const AnaTrueParticleB* trueTrack;
      
      /// Error corresponding to the interaction
      Float_t IntXSecError;
         
  };
  


  /// A simple class to provide the data for the systematic itself
  class SISystInput{

  public:

    int nParticles;

    ParticleHistory* Particles;

    SISystInput(){
      nParticles = 0;
      Particles  = NULL;
    }

    virtual ~SISystInput(){
      if (Particles)
        delete [] Particles;
      Particles   = NULL;
      nParticles  = 0;
    
    }

  };

  
  /// A class that does the actual propagation of a true track, taking into account and storing the xsec
  /// values and uncertainties
  class ParticleSIPropagator{
  public:
    
    ParticleSIPropagator(){
      _initialized = false;
    }
   
    virtual ~ParticleSIPropagator(){}
    
    
    /// Initialization
    void Initialize(){
      if (_initialized) return;
      SetInteractionTypes();
      SetParameters();
      _initialized = true;
    }

    
    /// Propagates a particle to a given pos filling all the history info
    virtual void Propagate(ParticleHistory&, const TVector3&, const SubDetId::SubDetEnum& det) const;
    
    /// Whether the propagator was initialized
    bool IsInitialized(){return _initialized;}
    
    
    /// Is a point inside a volume of interest: the region where the propagation is relevant
    virtual Bool_t InVOI(const TVector3&, const SubDetId::SubDetEnum& det) const = 0;
    
    
    /// The function that gives a x-section value given a channel, momentum value, and node 
    /// + particle type so to make it fully general
    virtual Double_t GetCrossSection(const si_syst::InteractionType&, const Float_t&, const Int_t&, TGeoNode*) const = 0;
    
    /// Same but given the state
   Double_t GetCrossSection(const si_syst::InteractionType& type, const ParticleState& state, const Int_t& PDG){
     
     TGeoNode *node = ND::hgman().GeoManager()->FindNode(state.Pos.X(), state.Pos.Y(), state.Pos.Z());
     
     return GetCrossSection(type, state.Momentum, PDG, node);

   }
   
    /// The function that gives a x-section value` uncertainty given a channel, momentum value and 
    /// + particle type so to make it fully general
    virtual Double_t GetCrossSectionError(const si_syst::InteractionType&, const Float_t&, const Int_t&, TGeoNode*) const = 0;
        
    /// Same but given the state
    Double_t GetCrossSectionError(const si_syst::InteractionType& type, const ParticleState& state, const Int_t& PDG){
    
     TGeoNode *node = ND::hgman().GeoManager()->FindNode(state.Pos.X(), state.Pos.Y(), state.Pos.Z());
     
     return GetCrossSectionError(type, state.Momentum, PDG, node);

   }
   
  protected: 
  

    virtual void SetInteractionTypes() = 0; 
    
    virtual void SetParameters() = 0; 
     
    /// Density of scattering centers: n/cm^3
    /// keep virtual if need fine tuning material based
    virtual Double_t DScattCenters(TGeoNode*) const; 
    
    /// The step size in terms of length
    Double_t _lengthStepSize;
    
    /// The step size in terms of monetum
    Double_t _momStepSize; 
    
    /// The array of relevant interaction types to be considered while propagating
    std::vector<si_syst::InteractionType> _intTypes; 
  
  private:
    bool _initialized;
  };
  
    
  /// Manager 
  class ParticleSIManager{
  
  public:
    
    /// ctor
    ParticleSIManager(){
      _propagator   = NULL;
    }
    
    /// dtor
    virtual ~ParticleSIManager(){
      if (_propagator)
        delete _propagator;
      _propagator = NULL;  
    }
    
    /// Set the propagator
    virtual void SetPropagator(ParticleSIPropagator* propagator){
      
      if (_propagator)
        delete _propagator;
      
      _propagator = propagator;
      _propagator->Initialize();
    }
    
    /// Calculates the information needed to compute an event weight
    SISystInput* CollectParticleInteractions(AnaTrueParticleB**, int, const SubDetId::SubDetEnum&) const;
   
    
    /// Get relevant particles
    /// Given array of input true tracks, its size, detector of interest
    virtual si_syst::SISystInput* GetRelevantParticles(AnaTrueParticleB**, int, const SubDetId::SubDetEnum&) const = 0;
 
  protected:
    
    
    /// A simple function to get relevant tracks and interactions of a given PDG
    /// Given array of input true tracks, its size, detector of interest, PDG of interest
    /// Will select primary tracks in VOI, and decide of their interction by considering daughters in VOI
    /// Three interaction types: no interaction in VOI, elastic, inelastic
    virtual si_syst::SISystInput* GetRelevantParticles(AnaTrueParticleB**, int, const SubDetId::SubDetEnum&, int) const;
    
    ParticleSIPropagator* _propagator; 
    
  };
  
};

std::ostream& operator<<(std::ostream& os, const TVector3& vect);
std::ostream& operator<<(std::ostream& os, const TLorentzVector& vect);


#endif
