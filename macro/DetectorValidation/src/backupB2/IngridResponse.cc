#include "IngridResponse.hh"
#include <CLHEP/Random/Randomize.h>
#include <algorithm>

using namespace std;

//
const G4double scilen = 120.;  //cm
const G4double longlen = 130.;  //cm
const G4double shortlen = 112.;  //cm
const G4double attleng = 241.7; //cm
const G4double CBIRKS = 0.0208; // used in SciBooNE MC
const G4double TransTimeInFiber = 1./28.;  //  1cm/2.8e10[cm/s] * 1e9 [ns]

//
//const G4double MeV2PE = 15./2.6;  // pe/MeV
//const G4double MeV2PE = 18.45/2.6;  // pe/MeV
//const G4double MeV2PE = 36.6;  // v1r1_3
//const G4double MeV2PE = 35.1;  // v2
//const G4double MeV2PE = 37.2;  // v2_1
//const G4double MeV2PE = 39.1;  // v2r2
//const G4double MeV2PE = 60.;  // v3
const G4double MeV2PE = 60.6;  // v3r1
const G4double MPPCPixel = 667.; //v3
//const G4double Eff_PDE = -0.292;  // @deltaV = 1.14 V
//const G4double Eff_PDE = -0.38;  // @deltaV = 1.4 V
const G4double Eff_PDE = -0.275;  // @deltaV = 1.1 V

//
const G4double ETC_CONST = 5.0;  // v3
const G4double rPDE = 1.7;  // @deltaV = 1.1 V
const G4double cross_after_rate = 0.09;  // @deltaV = 1.1 V

//
const G4int topview = 1;
const G4int sideview = 0;

//
IngridResponse::IngridResponse()
{
}

//
IngridResponse::~IngridResponse()
{
}

//
void IngridResponse::ApplyScintiResponse(G4double* edep, G4Track* aTrack)
{
    //quenching
    BirksSaturation(edep,aTrack);

    return;
}

//
void IngridResponse::BirksSaturation(G4double* edep, G4Track* aTrack)
{
  //const G4double CBIRKS = sbcard->Birks;

  G4double              kineticE = aTrack->GetKineticEnergy();
  G4ParticleDefinition* particle = aTrack->GetDefinition();
  G4Material*           material = aTrack->GetMaterial();

/*
	G4cout << "=======" << G4endl;
	G4cout << "edep : " << (*edep) << G4endl;
	G4cout << "kineticE : " << kineticE << G4endl;
	G4cout << "particle : " << particle->GetParticleName() << G4endl;
	G4cout << "material : " << material->GetName() << G4endl;
*/

  if(particle->GetPDGCharge()==0) return;

  G4double dedx = emcal.GetDEDX(kineticE, particle, material)/(MeV/cm);

/*
	G4cout << "dedx : " << dedx << G4endl;
	G4cout << G4endl;
*/

  (*edep) = (*edep)/(1. + CBIRKS*dedx);

  /*
  G4double mmu = 105.65837;
  for (int i=0;i<100000;i++) {
    G4double p = mmu * (i+1)*0.1;
    G4double kineticE = (sqrt(mmu*mmu+p*p)-mmu)*MeV;
    G4double dedx = emcal.GetDEDX(kineticE, particle, material)/(MeV/cm)/1.032;

    G4cout << (i+1)*0.1 << " " << dedx << G4endl;
  }
  */
  return;
}

//
void IngridResponse::ApplyFiberResponse(G4double* edep, G4double* time, G4int view, G4ThreeVector pos)
{
    G4double x = 0.;

    if( view==topview ) x = fabs(scilen/2. - pos[1]/cm);
    else if( view==sideview ) x = fabs(scilen/2. + pos[0]/cm);

    // attenuation
    *edep *= exp(-1.*x/attleng);

    // delay in fiber
    *time += TransTimeInFiber*x;

  return;
}

//
void IngridResponse::ApplyFiberResponseV(G4double* edep, G4double* time, G4int pln, G4ThreeVector pos)
{
    G4double x = 0.;

    if( pln==11 ) x = fabs(longlen/2. - (pos[1]/cm + 3.7) );
    if( pln==12 ) x = fabs(longlen/2. + (pos[1]/cm - 0.3) );
    if( pln==13 ) x = fabs(shortlen/2. - (pos[0]/cm + 0.9) );
    if( pln==14 ) x = fabs(longlen/2. + (pos[0]/cm - 5.9) );

    // attenuation
    *edep *= exp(-1.*x/attleng);

    // delay in fiber
    *time += TransTimeInFiber*x;

  return;
}

//
void IngridResponse::ApplyMPPCResponse(G4double edep, G4double* pe)
{
	G4double npe;

    // energy to p.e.
    //pe = (edep/MeV)*MeV2PE;
    *pe = edep*(MeV2PE);

	// PDE 
/*
	npe = edep * rPDE * ETC_CONST;
*/

	// MPPC linearity
	npe = MPPCPixel * (1. - exp( Eff_PDE * (*pe) / MPPCPixel ));
	*pe = npe;
/*
	npe = MPPCPixel * (1. - exp( -1.* npe / MPPCPixel ));
*/

	// fake signal p.e. from cross-talk & after pulse
/*
	npe = npe / (1. - cross_after_rate);
*/

    // Poisson statistics & 1 pe resolution
    npe = CLHEP::RandPoisson::shoot(npe);

	//
    *pe = npe;


  return;
}

//
void IngridResponse::ApplyADCResponse(G4double pe, G4int* adc)
{
    *adc = (G4int)(pe*150);
/*
  const G4double VA_NOISE = 1.6;
  const G4double VA_OVERFLOW = 2048.0;

  G4double adc_tmp;

  IngridMap* sbmap= sbdb->GetSciBarMap();

  for (G4int view=0;view<2;view++) {
    for (G4int layer=0;layer<64;layer++) {
      for (G4int strip=2;strip<114;strip++) {

	G4int isci = sbmap->sbisci(view,layer,strip);

	// pe -> ADC conversion
	SciBarCalibration* sbcal= sbdb->GetSciBarCalibrationInfo();
	adc_tmp = pe[isci]/sbcal->pe[isci];

	// VA noise
	adc_tmp += CLHEP::RandGaussQ::shoot(0.0,VA_NOISE);

	// overflow
	if (adc_tmp>VA_OVERFLOW) adc_tmp = VA_OVERFLOW;

	adc[isci] = (int)adc_tmp;
      }
    }
  }
*/
}

//
void IngridResponse::ApplyTDCResponse(G4double time, G4int* tdc)
{
    *tdc = 0;
}
