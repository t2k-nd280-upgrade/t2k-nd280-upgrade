#ifndef ToyMaker_h
#define ToyMaker_h

#include "ToyExperiment.hxx"
#include "SystematicBase.hxx"

/// Creates the ToyExperiments with the proper variations for all systematic parameters
/// 

class ToyMaker{
  
public:
  
  /// Create the Toy experiment 
  ToyMaker();
  
  /// Everyone should have a destructor.
  virtual ~ToyMaker();
  
  //------------- MANDATORY METHOD -------------------------

  /// Fills the Toy Experiment with a given index
  virtual void FillToyExperiment(ToyExperiment& toy) = 0;

  //--------------------------------------------------------

  /// Create the Toy Experiment, provided the number of toys, 
  // and the vector of enabled systematics
  void CreateToyExperiments(Int_t ntoys, const std::vector<SystematicBase*>& systematicsEnabled);

  /// Add a SystematicBase
  void AddSystematic(SystematicBase* sys);

  /// Returns the Toy experiment with a given index
  ToyExperiment* GetToyExperiment(UInt_t index){return _toys[index];}

  /// Returns the Toy experiment vector
  const std::vector<ToyExperiment*>& GetToyExperiments()  const {return _toys;}

protected:

  /// The vector of toys
  std::vector<ToyExperiment*> _toys;

  /// The systematics that need to be considered
  SystematicBase* _systematics[NMAXSYSTEMATICS];
  int _nSystematics;

};

#endif
