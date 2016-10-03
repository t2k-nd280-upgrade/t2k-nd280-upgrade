#ifndef EventVariationBase_h
#define EventVariationBase_h

#include "SystematicBase.hxx"
#include "ToyExperiment.hxx"

/// This is the base class that all systematics variations should inherit from. 
/// - Variation: modify some aspect of the input data. The selection needs to be redone 
///   in general. This is what we call Toy Experiments

class EventVariationBase: public SystematicBase{
  
public:
  
  /// Create the systematic, with one parameter
  EventVariationBase(){_type=kVariation;}

  /// Create the systematic, specifying the number of systematic parameters
  EventVariationBase(UInt_t npar):SystematicBase(npar){_type=kVariation;}
  
  /// Everyone should have a destructor.
  virtual ~EventVariationBase() {}
  
  /// Initialize the SystBox for this event
  virtual void InitializeEvent(const AnaEventC& event, const SelectionBase& sel);
  virtual void InitializeEvent(const AnaEventC& event);

  //----------------------- MANDATORY METHODS -----------------------------------------

  /// Apply the systematic
  /// This MUST be overridden in the derived class.
  virtual void Apply(const ToyExperiment& toy, AnaEventC& event) = 0;

protected:

  // Get the IDs of the RecObject groups that are relevant for this systematic in a given selection
  virtual Int_t GetRelevantRecObjectGroups(const SelectionBase& sel, Int_t* IDs) const = 0;

  //----------------------- OPTIONAL METHODS -----------------------------------------

public:

  /// Undo  the systematic variations done by ApplyVariation. This is faster tha reseting the full Spill
  /// This undos the variations. If it return true the Spill will be reset  
  /// By default the Spill is reset
  virtual bool UndoSystematic(AnaEventC&){return true;}

protected:

  /// Check whether a AnaRecObject is relevant for this systematic or not
  virtual bool IsRelevantRecObject(const AnaEventC&, const AnaRecObjectC&) const {return true;}

  //----------------------- INTERNAL METHODS -----------------------------------------

  /// Fills the SystBox
  void FillSystBox(const AnaEventC& event, const SelectionBase& sel, SystBoxB& box);

};

#endif
