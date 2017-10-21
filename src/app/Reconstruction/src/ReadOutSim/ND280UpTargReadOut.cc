#include <algorithm>
#include <globals.hh>

//#include <CLHEP/Random/Randomize.h>
#include <CLHEP/Units/SystemOfUnits.h>

//#include <TRandom.h>
#include <TString.h>

#include "Randomize.hh"

#include "ND280UpTargReadOut.hh"

using namespace std;

// General constants
const double CBIRKS = 0.0208 * CLHEP::cm/CLHEP::MeV; // used in SciBooNE MC

// WAGASCI Constants
const double scilen = 120.;  //cm
const double scilen_WAGASCI = 100.;  //cm
const double longlen = 130.;  //cm
const double longlen_pm = 120.;  //cm
const double shortlen = 112.;  //cm
const double attleng = 241.7; //cm
const double sciattleng = 10.46; //cm added
const double sciattleng_WAGASCI = 4.0; //cm added
const double SciBarFactor = 1.77;  //P.E. factor for SciBar scintillator
const double WAGASCIFactor = 3.;  //P.E. factor for WAGASCI scintillator
const double INGRIDFactor = 1.08;  //P.E. factor for INGRID scintillator 2014/12/16
const double TransTimeInFiber = 1./28.;  //  1cm/2.8e10[cm/s] * 1e9 [ns]
const double Pedestal = 0;//145;  // pedeltal of ADC counts
const double Gain = 10;  // Gain ADC counts of high gain channel
const double LowGain  = 1;  // Gain ADC counts of low gain channel
const double ElecNoise = 1.7;  // sigma of high gain electronics noise
const double LowElecNoise = 1.2;  // sigma of low gain electronics noise
const double PixelGainVari = 0.031;  // gain variation among pixels
const double PixelGainVari_WAGASCI = 0.133;  // gain variation among pixels
const double MeV2PE = 45.9;  // v3r4
const double MeV2PE_WAGASCI = 31.5;  // v3r4
const double MPPCPixel = 667.; //v3
const double Eff_PDE = -0.275;//*1.134;  // @deltaV = 1.1 V
const double Eff_PDE_WAGASCI = -1.;//
const double ETC_CONST = 5.0;  // v3
const double rPDE = 1.7;  // @deltaV = 1.1 V
const double cross_after_rate = 0.09;//*1.051;  // @deltaV = 1.1 V
const double cross_after_rate_WAGASCI = 0.05;//*1.051;  // @deltaV = 1.1 V
const int topview = 1;
const int sideview = 0;

// FGD Constants
const double EdepToPhotConv_FGD = 70.8 / CLHEP::MeV; // contains both collection in fiber and edep->gamma conversion 
const double DistMPPCscint_FGD = 41*CLHEP::mm;
const double LongCompFrac_FGD = 0.816;
const double LongAtt_FGD = 11926.*CLHEP::mm;
const double ShortAtt_FGD = 312.*CLHEP::mm;
const double DecayLength_FGD = 0.0858 / CLHEP::mm;
const double Lbar_FGD = 1864.3 * CLHEP::mm;

// SuperFGD constants
const double MPPCEff_SuperFGD = 0.38;
const double EdepToPhotConv_SuperFGD = EdepToPhotConv_FGD * 1.3;

// SciFi constants
const double EdepToPhotConv_SciFi_SingleClad_2mm = 23.7 / CLHEP::MeV; 
const double MPPCEff_SciFi = 0.38;

// Approximate collection factors from PDG2016 (Detectors and accelerators section)
const double CollFactor_SingleClad = 0.06;
//const double CollFactor_DoubleClad = 0.054; // from Licciardi's thesis  
const double CollFactor_DoubleClad = 0.10;

ND280UpTargReadOut::ND280UpTargReadOut()
{
}

ND280UpTargReadOut::~ND280UpTargReadOut()
{
}

TString ND280UpTargReadOut::AsStringTargType(TargetType_t targetid){  
  if(targetid == nd280upconv::kUndefined)     return "Undefined"; 
  else if(targetid == nd280upconv::kSuperFGD) return "SuperFGD";
  else if(targetid == nd280upconv::kFGD)      return "FGD";
  else if(targetid == nd280upconv::kFGDlike)  return "FGDlike";
  else if(targetid == nd280upconv::kWAGASCI)  return "WAGASCI";
  else if(targetid == nd280upconv::kSciFi)    return "SciFi";
  else{
    cout << "ND280UpTargReadOut::AsStringTargType" << endl;
    cout << "The selected Target type does not exist!!!" << endl;
    exit(1);
  }
  return "Undefined";
}

double ND280UpTargReadOut::ApplyScintiResponse(double &edep, double steplength,double charge)
{                                             
  BirksSaturation(edep,steplength,charge);
  double nphot = EdepToPhot(edep);  
  return nphot;
}

void ND280UpTargReadOut::BirksSaturation(double &edep, double steplength, double charge)
{
  if(charge==0) return; // why?  
  double dedx = edep/steplength;
  edep = edep/(1. + CBIRKS*dedx);
  return;
}

double ND280UpTargReadOut::EdepToPhot(double edep)
{
  if(GetTargType() == nd280upconv::kSuperFGD){        
    /*
    // Account for the 3 fibers in the same scintillator cube
    double collfact = CollFactor_DoubleClad;
    double fact_fib1 = collfact;
    double fact_fib2 = (1-fact_fib1)*collfact;
    double fact_fib3 = (1-fact_fib2)*collfact;
    double CollFactAve = (fact_fib1+fact_fib2+fact_fib3)/3.;
    double NormShadowLight = CollFactAve / collfact; // fraction 
    //cout << "NormShadowLight = " << NormShadowLight << endl;   
    return edep * EdepToPhotConv_FGD * NormShadowLight;
    */
    return edep * EdepToPhotConv_SuperFGD;     
  }
  else if(GetTargType() == nd280upconv::kFGDlike){
    return edep * EdepToPhotConv_FGD;
  }
  else if(GetTargType() == nd280upconv::kSciFi){    
    return edep * EdepToPhotConv_SciFi_SingleClad_2mm;
  }
  else{
    cout << "ND280UpTargReadOut::ApplyFiberResponse" << endl;
    cout << "The selected Target type does not exist!!!" << endl;
    exit(1);
  }
  
  return nd280upconv::kUndefined;
}
////////

void ND280UpTargReadOut::ApplyFiberResponse(double &nphot, double &time, double x)
{
  // Apply the light attenuation to the collected photons
  ApplyFiberAttenuation(nphot,x);

  // Get the time when the photons reach the MPPC
  ApplyFiberTime(time,x);
  
  return;
}

void ND280UpTargReadOut::ApplyFiberAttenuation(double &nphot, double x)
{
  if(GetTargType() == nd280upconv::kSuperFGD ||
     GetTargType() == nd280upconv::kFGDlike
     ){
    nphot = GetPhotAtt_FGD(nphot,x);
  }
  else if(GetTargType() == nd280upconv::kSciFi){    
    //nphot = GetPhotAtt_SciFi(nphot,x); // not available yet
    nphot = GetPhotAtt_FGD(nphot,x);    
  }
  else{
    cout << "ND280UpTargReadOut::ApplyFiberAttenuation" << endl;
    cout << "The selected Target type does not exist!!!" << endl;
    exit(1);
  }
  return;
}

double ND280UpTargReadOut::GetPhotAtt_FGD(double Nphot0,double x){
  
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

double ND280UpTargReadOut::GetPhotAtt_SciFi(double Nphot0,double x){
  cout << "ND280UpTargReadOut::GetPhotAtt_SciFi" << endl;
  cout << "This method has not been implemented yet!!!" << endl;
  return nd280upconv::kUndefined;
}

void ND280UpTargReadOut::ApplyFiberTime(double &time, double x)
{
  time += TransTimeInFiber*x;
}




/*

// No Stat fluctuations

void ND280UpTargReadOut::ApplyMPPCResponse(double &npe)
{
  if(GetTargType() == nd280upconv::kSuperFGD ||
     GetTargType() == nd280upconv::kFGDlike
     ){
    npe *= MPPCEff_SuperFGD;
  }
  else if(GetTargType() == nd280upconv::kSciFi){
    npe *= MPPCEff_SciFi;
  }
  else{
    cout << "ND280UpTargReadOut::ApplyMPPCResponse" << endl;
    cout << "The selected Target type does not exist!!!" << endl;
    exit(1);
  }
*/


void ND280UpTargReadOut::ApplyMPPCResponse(G4double &npe)
{

  double rndunif =0.;
  double npe_passed = 0.;
  double mppc_eff = 0.;
  
  int npe_integer = (int) (npe+0.5);
  
  if(GetTargType() == nd280upconv::kSuperFGD ||
     GetTargType() == nd280upconv::kFGDlike){
    mppc_eff = MPPCEff_SuperFGD;
  }
  else if(GetTargType() == nd280upconv::kSciFi){
    mppc_eff = MPPCEff_SciFi;
  }
  else{
    G4Exception("ExN02TargReadOut::ApplyMPPCResponse",
		"MyCode0002",FatalException,
		"The selected Target type does not exist!!!");
  }
  
  for (int i=0;i<npe_integer;i++){
    rndunif = G4UniformRand();
    if (rndunif < MPPCEff_SuperFGD) npe_passed++;
  }
  
  //cout << "npe = " << npe << " - npe_passed = " << npe_passed << endl;
  
  npe = npe_passed;
  
  return;
}


