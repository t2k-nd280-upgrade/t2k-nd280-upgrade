#ifndef PionInteractionSystematic_h
#define PionInteractionSystematic_h

#include "Interaction.hxx"
#include "SelectionBase.hxx"
#include "PionCrossSections.hxx"

//For geometry stuff.
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoVolume.h>
#include <TGeoMaterial.h>
#include <TKey.h>

#include <math.h>

#include <string>

//For getenv() for getting the geom file.
#include <stdlib.h>

//Creating a class for easy storage of variables to
//pass them along.
class PionInteractionSystematic{

public:

  Float_t weightMCToData;

  Int_t nPions;

  Bool_t* pionType;

  Int_t totalSteps;

  Int_t* nSteps;

  Float_t* initMom;

  Float_t* stepLengths;

  Int_t nInteractions;

  Float_t* pInteraction;

  Int_t* typeInteraction;

  Float_t MCSumNSigmaStepLength;

  Float_t DataSumNSigmaStepLength;

  //Probability of no interaction in MC and data.
  Float_t MCProbNoInt;

  Float_t DataProbNoInt;

  Int_t* pionID;

  AnaTrueParticleB** TrueParticles;
  AnaTrueParticleB** InteractionTrueParticles;

  //The final position according to the stepping algorithm
  //For testing/validation only.
  //They are the momentum and position at the VOI exit point
  //(either due to interaction, or leaving the VOI).
  std::vector<Float_t> testFinalMom;
  std::vector<TLorentzVector> testFinalPos;
  std::vector<Int_t> testInteractionType;

  //Constructor
  //Just set all the class variables individually.
  PionInteractionSystematic();
  
  virtual ~PionInteractionSystematic();

};

//A class to contain the result of the stepping code.
class SteppingResult{

public:
  
  //The sum of MC N*sigma*stepLengths
  Float_t MCSumNSigmaStepLength;

  //The sum of Data N*sigma*stepLengths
  Float_t DataSumNSigmaStepLength;

  //The vector of step lengths.
  std::vector<Float_t> stepLengths;

  //The Final Momentum according to the stepping.
  Float_t finalMom;

  //The final position, according to the stepping.
  TLorentzVector finalPos;

  //Constructor.  Fill it all at once.
  SteppingResult(Float_t, Float_t, std::vector<Float_t>,Float_t,TLorentzVector);

};




Double_t DScattCentres(TGeoMixture* mixture);

Int_t GetMaterialSeries(std::string matName);

Double_t GetIElement(Int_t Z);

Double_t GetZoverAMaterial(TGeoMixture* mat);

Double_t GetIMaterial(TGeoMixture* mat, Double_t ZoverA);

Double_t computeEkinFromMom(Double_t mom);

Double_t Interpolate(double xsec1, double xsec2, double mom1, double mom);

AnaTrueParticleB* GetParent(AnaTrueParticleB* track, AnaTrueParticleB* allTrajInTPCFGD[], Int_t nTraj);

std::pair<TLorentzVector,TVector3>  TakeSmallStep(Int_t charge, 
						  TLorentzVector initPos, 
						  TVector3 initMom,
						  Double_t stepLength, //in mm
						  TGeoMaterial* material,
						  std::string materialName,
						  TGeoMixture* mixture);

/// Combine everything that may be analysis/selection dependent int a simple class, 
/// so that can extended 
class PionSIManager{

  
public:
   
  /// ctor
  PionSIManager(){}
  
  ///dtor
  virtual ~PionSIManager(){}

  // Functions related to the volumes of interest to the analysis
  virtual Bool_t InVOI(SubDetId_h det, Float_t* pos) const;
  virtual Bool_t InVOI1(Float_t* pos) const;
  virtual Bool_t InVOI2(Float_t* pos) const;
  virtual Bool_t InVOIext(Float_t* pos) const;

  
  SteppingResult StepBetweenPoints(SubDetId_h det, Int_t charge,
		   TLorentzVector initPos, 
		   TVector3 initMom, 
		   TLorentzVector finalPos,
		   Double_t stepLength,
		   TGeoManager* geom) const;


  /// Calculates the information needed to compute an event weight,
  /// as well as a weight correcting Geant4 to Data.
  /// Returns a PionInteractionSystematic object containing all the needed information
  /// The detector can be provided externally (not only through the selection since
  /// for the moment the info is filled prior to any cuts sequence applied) to give more flexibility 
  PionInteractionSystematic* ComputePionWeightInfo(const AnaEventB& event, SubDetId_h det) const;


  PionInteractionSystematic* ComputePionWeightInfo(const AnaEventB& event, const SelectionBase& sel, Int_t branch) const{
    return ComputePionWeightInfo(event, sel.GetDetectorFV(branch)); 
  
  }
 
}; 


//The detector geometry.
extern TGeoManager *pionSIGeom;

//Pulls in the full detector geometry from a file that contains
//the full geometry.
TGeoManager* GetPionSIGeometry();

#endif
