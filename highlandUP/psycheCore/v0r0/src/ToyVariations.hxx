#ifndef ToyVariations_h
#define ToyVariations_h

#include "BasicTypes.hxx"


/// This class represents a set of variations for a single Systematic
///
///

class ToyVariations{
  
public:
  
  /// Create the systematic, 
  ToyVariations(Int_t npar);
  
  /// Everyone should have a destructor.
  virtual ~ToyVariations();

  /// Number of parameters for this systematic
  UInt_t GetNParameters() const{return nVariations;}
  
  /// Computes the total weight for all systematic parameters
  Double_t GetWeight() const;

  /// Computes the total weight for all systematic parameters that were used
  Double_t GetWeight(const Int_t* bins, int nBins) const;

public:

  /// the vector of Variations, one for each of the systematic parameters
  Float_t* Variations;
  Int_t nVariations;

  /// the vector of Weights, one for each of the systematic parameters
  Float_t* Weights;
  Int_t nWeights;


};

#endif
