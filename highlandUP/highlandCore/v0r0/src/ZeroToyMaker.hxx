#ifndef ZeroToyMaker_h
#define ZeroToyMaker_h

#include "ToyMaker.hxx"

class ZeroToyMaker: public ToyMaker{
  
public:
  
  /// Create the Toy experiment 
  ZeroToyMaker();
  
  /// Everyone should have a destructor.
  virtual ~ZeroToyMaker() {}

  /// Fills the Toy Experiment with a given index
  void FillToyExperiment(ToyExperiment& toy);

};

#endif
