#ifndef ToyExperiment_h
#define ToyExperiment_h

#include "ToyVariations.hxx"

/// The maximum number of toyVariations that is supported.
/// This is defined here because it is the lowest level Systematic class
const UInt_t NMAXSYSTEMATICS=100;

/// A ToyExperiemnt is defined a set of ToyVariations, one for each of the toyVariations
/// It contains a vector of variations one for each systematic parameter
/// 

class ToyExperiment{
  
public:
  
  /// Create the Toy experiment 
  ToyExperiment();

  /// Copy constructor
  ToyExperiment(const ToyExperiment& toy);

  /// Everyone should have a destructor.
  virtual ~ToyExperiment();
  
  /// Add a systematic, specifying the systematic index and number of systematic parameters
  void AddToyVariation(Int_t index, UInt_t npar);

  /// returns the variations for a given systematic (index)
  ToyVariations* GetToyVariations(UInt_t index) const{return _toyVariations[index];}

  /// Get the number of toyVariations in this toy
  UInt_t GetNToyVariations() const{return _nToyVariations;}

  /// Get the number of parameters in this systematic
  UInt_t GetNParameters(UInt_t index) const {return GetToyVariations(index)->GetNParameters();} 

  /// Set the variation for a given systematic (index) and a given parameter (ipar) in that systematic
  void SetToyVariation(UInt_t index, UInt_t ipar, Float_t var, Float_t weight=1.) {_toyVariations[index]->Variations[ipar] = var;_toyVariations[index]->Weights[ipar] = weight; } 

  /// Set the variation for a given systematic (index) with a single parameter
  void SetToyVariation(UInt_t index, Float_t var, Float_t weight=1.) {_toyVariations[index]->Variations[0] = var;_toyVariations[index]->Weights[0] = weight;} 

public:

  /// vector of variations (one for each Systematic)
  ToyVariations* _toyVariations[NMAXSYSTEMATICS];

  /// Total number of toyVariations
  UInt_t _nToyVariations;
};

#endif
