#include "ProtonInteractionSystematic.hxx"

//#define DEBUG
//#define DDEBUG


const Float_t proton_si::ProtonSIPropagator::_thEnergy = 20 * units::GeV;

//##########################################################
//Proton cross-section parametrization,  GEANT4
//##########################################################
// Cross-sections for proton nuclear scattering up to 20 GeV, getting the low
// energy threshold behaviour right.
// H.P. Wellisch (TRIUMF), D. Axen (British Columbia U.). 1996. 
// Published in Phys.Rev.C54:1329-1332,1996
// Implements corrected parameterization from  
// http://laws.lanl.gov/XCI/PEOPLE/rep/pdf/RPS96.pdf

//********************************************************************
Double_t proton_si::ProtonSIPropagator::GetProtonInelasticCrossSection(const Double_t& momentum, const Double_t& Z, const Double_t& A) const{
  //********************************************************************

  Double_t kineticEnergy = anaUtils::ComputeKineticEnergy(momentum, units::mass_proton); 

  if (kineticEnergy <= 0.) { return 0.; }

  if (A < Z) { return 0.; }


  // constant cross section above ~20GeV
  if (kineticEnergy > _thEnergy) { kineticEnergy = _thEnergy; }

  Double_t A13 = std::pow(A,-0.3333333333);
  Double_t nOfNeutrons = A - Z;
  kineticEnergy /=units::GeV;
  Double_t alog10E = std::log10(kineticEnergy);

  const Double_t nuleonRadius = 1.36E-15;
  const Double_t fac = M_PI*nuleonRadius*nuleonRadius;

  Double_t b0   = 2.247-0.915*(1 - A13);
  Double_t fac1 = b0*(1 - A13);
  Double_t fac2 = 1.;
  if(nOfNeutrons > 1) { fac2=std::log(((Double_t)(nOfNeutrons))); }
  Double_t crossSection = 1.0E31*fac*fac2*(1. + 1./A13 - fac1);

  // high energy correction
  crossSection *= (1 - 0.15*std::exp(-kineticEnergy))/(1.0 - 0.0007*A);

  // first try on low energies: rise
  Double_t ff1= 0.70-0.002*A;  // slope of the drop at medium energies.
  Double_t ff2= 1.00+1/A;  // start of the slope.
  Double_t ff3= 0.8+18/A-0.002*A; // stephight

  Double_t ff4= 1.0 - (1.0/(1+std::exp(-8*ff1*(alog10E + 1.37*ff2))));

  crossSection *= (1 + ff3*ff4);

  // low energy return to zero

  ff1=1. - 1./A - 0.001*A;       // slope of the rise
  ff2=1.17 - 2.7/A - 0.0014*A;   // start of the rise

  ff4=-8.*ff1*(alog10E + 2.0*ff2);

  crossSection *= units::millibarn/(1. + std::exp(ff4));
  return crossSection;
}


//********************************************************************
Double_t proton_si::ProtonSIPropagator::GetCrossSection(const si_syst::InteractionType& type, const Float_t& mom, const Int_t& pdg, TGeoNode* node) const{
//********************************************************************
 
 // Only work with inelastic at the moment
 if (type != si_syst::kInelastic) return 0.; 
 
 (void)pdg;
  
 TGeoVolume *volume        = node->GetVolume();
 TGeoMaterial *material    = volume->GetMaterial();
 TGeoMixture *mixture      = (TGeoMixture*)material;

 return GetProtonInelasticCrossSection(mom, mixture->GetZ(), mixture->GetA());
 
}

//********************************************************************
Bool_t proton_si::ProtonSIPropagator::InVOI(const TVector3& pos, const SubDetId::SubDetEnum& det) const{
  //********************************************************************
  
  if (det == SubDetId::kTarget1){
    // This is the edges of the FGD1 volume, plus all the way out to TPCGasMixture in downstream z.
    // (Rounded to the next integer value that includes the value from the geometry in its range).
    Bool_t xOK = -1150.0 < pos.X() && pos.X() < 1150.0;
    Bool_t yOK = -1170.0 < pos.Y() && pos.Y() < 1230.0;
    Bool_t zOK = 98.0 < pos.Z() && pos.Z() < 576.0;

    return xOK && yOK && zOK;
  }
  else if (det == SubDetId::kTarget2){
    // This is from the TPCGasMixture in upstream z through FGD2 all the way out to TPCGasMixture in downstream z.
    // (Rounded to the next integer value that includes the value from the geometry in its range).
    Bool_t xOK = -1150.0 < pos.X() && pos.X() < 1150.0;
    Bool_t yOK = -1170.0 < pos.Y() && pos.Y() < 1230.0;
    Bool_t zOK = 1347.0 < pos.Z() && pos.Z() < 1934.0;
    return xOK && yOK && zOK;
  }
  return false;

}
