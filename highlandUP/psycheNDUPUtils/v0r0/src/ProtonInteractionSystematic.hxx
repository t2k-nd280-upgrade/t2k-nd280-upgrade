#ifndef ProtonInteractionSystematic_h
#define ProtonInteractionSystematic_h

#include "SecondaryInteractionSystematic.hxx"
#include "Parameters.hxx"
 
namespace proton_si{
  
  class ProtonSIPropagator:  public si_syst::ParticleSIPropagator{
    
  public:
    ProtonSIPropagator(){
      // For the moment read the uncertainty from the param file
      _uncertainty = (Float_t)ND::params().GetParameterD("psycheUtils.ProtonSI.XSec.Uncertainty"); 
    }
    
    
    virtual Double_t GetCrossSection(const si_syst::InteractionType&, const Float_t&, const Int_t&, TGeoNode*) const;
   
    virtual Double_t GetCrossSectionError(const si_syst::InteractionType& type, const Float_t&, const Int_t&, TGeoNode*) const{
      if (type != si_syst::kInelastic) return 0.; 
      return _uncertainty;
    }
   
    /// Whether a track is in VOI given a position and detector
    virtual Bool_t InVOI(const TVector3&, const SubDetId::SubDetEnum&) const;
    
  protected:
     void SetInteractionTypes(){
       
       _intTypes.push_back(si_syst::kInelastic);
     }
    
     void SetParameters(){
       
      // Read from the param files
      _lengthStepSize = (Float_t)ND::params().GetParameterD("psycheUtils.ProtonSI.StepSize");
      _momStepSize    = (Float_t)ND::params().GetParameterD("psycheUtils.ProtonSI.StepMom");
     }
    
  private:
    
    Double_t GetProtonInelasticCrossSection(const Double_t& momentum, const Double_t& Z, const Double_t& A) const;
    
    Float_t _uncertainty;
   
    static const Float_t _thEnergy; 
  };
   
  class ProtonSIManager: public si_syst::ParticleSIManager{
  
  public:
    
    ///ctor
    ProtonSIManager(){
      _propagator = new proton_si::ProtonSIPropagator;
      _propagator->Initialize();
    }
    
    ///dtor
    virtual ~ProtonSIManager(){}
    
    /// Fills the relevant tracks with the corresponding interaction types
    using ParticleSIManager::GetRelevantParticles;
    si_syst::SISystInput* GetRelevantParticles(AnaTrueParticleB** allTrajs, int nTrajs, const SubDetId::SubDetEnum& det) const{
      return si_syst::ParticleSIManager::GetRelevantParticles(allTrajs, nTrajs, det, 2212); // Protons 
    }
  

  };
  
}

#endif
