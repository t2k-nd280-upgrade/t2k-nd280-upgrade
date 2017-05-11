#include "ExN02TargReadOut.hh"
#include <CLHEP/Random/Randomize.h>
#include <algorithm>
#include <TRandom.h>
#include "G4SystemOfUnits.hh"
#include <globals.hh>

using namespace std;
using namespace conv;

// General constants
const G4double CBIRKS = 0.0208 * cm/MeV; // used in SciBooNE MC

// WAGASCI Constants
const G4double scilen = 120.;  //cm
const G4double scilen_WAGASCI = 100.;  //cm
const G4double longlen = 130.;  //cm
const G4double longlen_pm = 120.;  //cm
const G4double shortlen = 112.;  //cm
const G4double attleng = 241.7; //cm
const G4double sciattleng = 10.46; //cm added
const G4double sciattleng_WAGASCI = 4.0; //cm added
const G4double SciBarFactor = 1.77;  //P.E. factor for SciBar scintillator
const G4double WAGASCIFactor = 3.;  //P.E. factor for WAGASCI scintillator
const G4double INGRIDFactor = 1.08;  //P.E. factor for INGRID scintillator 2014/12/16
const G4double TransTimeInFiber = 1./28.;  //  1cm/2.8e10[cm/s] * 1e9 [ns]
const G4double Pedestal = 0;//145;  // pedeltal of ADC counts
const G4double Gain = 10;  // Gain ADC counts of high gain channel
const G4double LowGain  = 1;  // Gain ADC counts of low gain channel
const G4double ElecNoise = 1.7;  // sigma of high gain electronics noise
const G4double LowElecNoise = 1.2;  // sigma of low gain electronics noise
const G4double PixelGainVari = 0.031;  // gain variation among pixels
const G4double PixelGainVari_WAGASCI = 0.133;  // gain variation among pixels
const G4double MeV2PE = 45.9;  // v3r4
const G4double MeV2PE_WAGASCI = 31.5;  // v3r4
const G4double MPPCPixel = 667.; //v3
const G4double Eff_PDE = -0.275;//*1.134;  // @deltaV = 1.1 V
const G4double Eff_PDE_WAGASCI = -1.;//
const G4double ETC_CONST = 5.0;  // v3
const G4double rPDE = 1.7;  // @deltaV = 1.1 V
const G4double cross_after_rate = 0.09;//*1.051;  // @deltaV = 1.1 V
const G4double cross_after_rate_WAGASCI = 0.05;//*1.051;  // @deltaV = 1.1 V
const G4int topview = 1;
const G4int sideview = 0;

// FGD Constants
const G4double EdepToPhotConv_FGD = 70.8 / MeV; // contains both collection in fiber and edep->gamma conversion 
const G4double DistMPPCscint_FGD = 41*mm;
const G4double LongCompFrac_FGD = 0.816;
const G4double LongAtt_FGD = 11926.*mm;
const G4double ShortAtt_FGD = 312.*mm;
const G4double DecayLength_FGD = 0.0858 / mm;
const G4double Lbar_FGD = 1864.3 * mm;

// SuperFGD constants
const G4double MPPCEff_SuperFGD = 0.38;

// Approximate collection factors from PDG2016 (Detectors and accelerators section)
const G4double CollFactor_SingleClad = 0.06;
const G4double CollFactor_DoubleClad = 0.10;

ExN02TargReadOut::ExN02TargReadOut()
{
}

ExN02TargReadOut::~ExN02TargReadOut()
{
}

G4String ExN02TargReadOut::AsStringTargType(TargetType_t targetid){  
  if(targetid == conv::kUndefined)     return "Undefined"; 
  else if(targetid == conv::kSuperFGD) return "SuperFGD";
  else if(targetid == conv::kFGD)      return "FGD";
  else if(targetid == conv::kWAGASCI)  return "WAGASCI";
  else if(targetid == conv::kSciFi)    return "SciFi";
  else{
    G4Exception("ExN02TargReadOut::AsStringTargType",
		"MyCode0002",FatalException,
		"The selected Target type does not exist!!!");
  }
  return "Undefined";
}


//////// 
G4double ExN02TargReadOut::ApplyScintiResponse(G4double &edep, G4double steplength,G4Track *aTrack)
{                                             
  BirksSaturation(edep,steplength,aTrack);
  double nphot = EdepToPhot(edep);
  
  return nphot;
}
void ExN02TargReadOut::BirksSaturation(G4double &edep, G4double steplength, G4Track *aTrack)
{
  G4ParticleDefinition* particle = aTrack->GetDefinition(); 
  if(particle->GetPDGCharge()==0) return;
  G4double dedx = edep/steplength;
  edep = edep/(1. + CBIRKS*dedx);

  return;
}
G4double ExN02TargReadOut::EdepToPhot(G4double edep)
{
  if(GetTargType() == conv::kSuperFGD){    
    
    // Account for the 3 fibers in the same scintillator cube
    double collfact = CollFactor_DoubleClad;
    double fact_fib1 = collfact;
    double fact_fib2 = (1-fact_fib1)*collfact;
    double fact_fib3 = (1-fact_fib2)*collfact;
    double CollFactAve = (fact_fib1+fact_fib2+fact_fib3)/3.;
    double NormShadowLight = CollFactAve / collfact; // fraction 
    //G4cout << "NormShadowLight = " << NormShadowLight << G4endl;
   
    return edep * EdepToPhotConv_FGD * NormShadowLight;
  }
  else{
    G4Exception("ExN02TargReadOut::ApplyFiberResponse",
		"MyCode0002",FatalException,
		"The selected Target type does not exist!!!");
  }
  
  return conv::kUndefined;
}
////////





void ExN02TargReadOut::ApplyFiberResponse(G4double &nphot, G4double &time, G4double x)
{
  // Apply the light attenuation to the collected photons
  ApplyFiberAttenuation(nphot,x);

  // Get the time when the photons reach the MPPC
  ApplyFiberTime(time,x);
  
  return;
}


void ExN02TargReadOut::ApplyFiberAttenuation(G4double &nphot, G4double x)
{
  if(GetTargType() == conv::kSuperFGD){
    nphot = GetPhotAtt_FGD(nphot,x);
  }
  else{
    G4Exception("ExN02TargReadOut::ApplyFiberAttenuation",
		"MyCode0002",FatalException,
		"The selected Target type does not exist!!!");
  }
  return;
}

G4double ExN02TargReadOut::GetPhotAtt_FGD(G4double Nphot0,G4double x){
  
  // Used for official FGD reconstruction: Eq. (4) in TN-103
  
  double a=0.;        // long attenuation component fraction
  double d=0.;        // distance MPPC-scint outside the bar
  double LongAtt=0.;  // long attenuation length
  double ShortAtt=0.; // short attenuation length
  double Ldecay=0.;   // decay length
  double Lbar=0.;     // bar length

  a = LongCompFrac_FGD;
  d = DistMPPCscint_FGD; 
  LongAtt = LongAtt_FGD;
  ShortAtt = ShortAtt_FGD;  
  Ldecay= DecayLength_FGD;
  Lbar = Lbar_FGD;
  
  double Nphot = Nphot0;
  Nphot *= ( a*exp((-x-d)/LongAtt) + (1-a)*exp((-x-d)/ShortAtt) );
  
  // Add fall-off in light intensity in end of bars 
  // where the scint light doesn't make it into the fibers
  // Nphot *= (1 - 0.5*(exp(-Ldecay*x) + exp(-Ldecay*(Lbar-x)))); 
  
  return Nphot;
}

void ExN02TargReadOut::ApplyFiberTime(G4double &time, G4double x)
{
  time += TransTimeInFiber*x;
}
//


void ExN02TargReadOut::ApplyMPPCResponse(G4double &npe)
{
  if(GetTargType() == conv::kSuperFGD){
    npe *= MPPCEff_SuperFGD;
  }
  else{
    G4Exception("ExN02TargReadOut::ApplyMPPCResponse",
		"MyCode0002",FatalException,
		"The selected Target type does not exist!!!");
  }
  
  /*
  // MPPC linearity
  npe = MPPCPixel * (1. - exp( Eff_PDE_WAGASCI * npe / MPPCPixel ));  
  // Poisson statistics & 1 pe resolution
  npe = (int) CLHEP::RandPoisson::shoot(npe);  
  //modified cross-talk and after pulse
  npe += (int) CLHEP::RandPoisson::shoot(npe*cross_after_rate);
  npe = gRandom->Gaus(npe,npe*PixelGainVari);  
  *pe = npe;
  */

  return;
}



/*
void ExN02TargReadOut::ApplyADCResponse(G4double *pe, G4double *lope, G4int* adc, G4int* loadc)
{
  G4double adc_tmp, loadc_tmp,Q,loQ;
  
  //PE to ADC
  adc_tmp = Pedestal + (*pe)*Gain;
  loadc_tmp = Pedestal + (*pe)*LowGain*14.29/13.55;
  
  //Electronics noise
  adc_tmp = gRandom->Gaus(adc_tmp,ElecNoise);
  loadc_tmp = gRandom->Gaus(loadc_tmp,LowElecNoise);

  //ADC to Charge
  //Q=(adc_tmp+53)/217;
  //loQ=(loadc_tmp+82)/26;
  Q=(adc_tmp)/135.5;
  loQ=(loadc_tmp)/14.29;

  //Non linearlity of high gain ADC
  if(Q<0.65)*adc=135.5*Q;
  else if(Q<3.2)*adc=217*Q-53;
  else if(Q<4.2)*adc=158.6*Q+133.9;
  else if(Q<14)*adc=5.1*Q+778.6;
  else *adc=850;

  //Non linearlity of low gain ADC
  if(loQ<7)*loadc=14.29*loQ;
  else if(loQ<27)*loadc=26*loQ-82;
  else if(loQ<35.5)*loadc=21.12*loQ+48.24;
  else if(loQ<178.4)*loadc=0.7*loQ+775.1;
  else *loadc=900;

  //ADC to PE
  *pe = (float)((*adc) - Pedestal)/Gain;
  *lope = (float)((*loadc) - Pedestal)/LowGain;
}
*/

 /*
void ExN02TargReadOut::ApplyTDCResponse(G4double time, G4int* tdc)
{
  *tdc = 0;
}
*/
