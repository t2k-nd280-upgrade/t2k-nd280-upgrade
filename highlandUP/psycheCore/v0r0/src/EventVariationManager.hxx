#ifndef EventVariationManager_h
#define EventVariationManager_h

#include "EventVariationBase.hxx"
#include "SelectionManager.hxx"
#include "TString.h"
#include "ToyExperiment.hxx"

/// The maximum number of EventVariations that is supported.
const UInt_t NMAXEVENTVARIATIONS=100;

/// Manager for EventVariations
/// This manager handles the registration, enabling and disabling of the
/// EventVariations.
class EventVariationManager {

public:

  EventVariationManager();
  virtual ~EventVariationManager();

  /// Apply all EventVariations
  void ApplyEventVariations(const ToyExperiment& toy, AnaEventC& event);

  /// Undo the event variation (Undo the variation, that is, go back to the previous values of modified observables)
  bool UndoEventVariations(AnaEventC& event);

  /// Get the EventVariation registered with the given name.
  EventVariationBase*     GetEventVariation(const std::string& name) const;

  /// Get the EventVariation registered with the given index.
  EventVariationBase*     GetEventVariation(Int_t index) const;

  /// Get the index of a EventVariation registered with the given name.
  Int_t GetEventVariationIndex(const std::string& name);

  /// Get the vector of EventVariations
  std::vector<EventVariationBase*>& GetEventVariations(){return _eventVariations;}

  /// Get the vector containing EventVariations with specific indices
  std::vector<EventVariationBase*> GetEventVariations(const std::vector<Int_t>& indices) const;

  /// Enable the EventVariation registered with the given index
  void EnableEventVariation(Int_t index);

  /// Disable the EventVariation registered with the given index
  void DisableEventVariation(Int_t index);

  /// Enable the EventVariations registered with the given indices
  void EnableEventVariations(const std::vector<Int_t>& systs);

  /// Disable the EventVariations registered with the given indices
  void DisableEventVariations(const std::vector<Int_t>& systs);

  /// Disable all eventVariations 
  void DisableAllEventVariations();

  /// Enable all EventVariation eventVariations 
  void EnableAllEventVariations();

  /// Add a new Event Variation provided its index in the manager and a pointer to it
  void AddEventVariation(Int_t index, EventVariationBase* sys);

  /// Add a new Event Variation provided its index in the manager, its name and a pointer to it
  void AddEventVariation(Int_t index, const std::string& name, EventVariationBase* sys);

  /// Replace one of the existing EventVariationBase
  void ReplaceEventVariation(Int_t index, EventVariationBase* sys);

  /// Fill the SystBox for the enabled EventVariations
  void InitializeEvent(SelectionManager& sel, AnaEventC& event);

  /// Delete the SystBox for all EventVariations
  void FinalizeEvent(AnaEventC& event);

  // Create the arrays of SystBox's
  void Initialize(Int_t nevents);

  /// Is there any EventVariation added ?
  bool HasEventVariations() const {return (_eventVariations.size()!=0);}

  /// Returns the number of EventVariations 
  UInt_t  GetNEventVariations() const {return _eventVariations.size();}

  /// Returns the vector of indices for all enabled EventVariations
  const Int_t* GetEnabledEventVariations(int& nSys){
      nSys = _nEventVariationsEnabled;    
      return _eventVariationsEnabled;
  }

  /// Returns the number of enabled EventVariations
  UInt_t GetNEnabledEventVariations(){ return _nEventVariationsEnabled;}

  /// Dump all eventVariations
  void DumpEventVariations();

  /// Check if an EventVariation with a given name already exists in the manager
  bool HasEventVariation(const std::string& name);

protected:

  /// The systematics that were registered as eventVariations.
  std::vector<EventVariationBase*> _eventVariations; 

  /// The systematics that were registered as eventVariations.
  std::vector<EventVariationBase*> _eventVariationsWithNull; 

  /// The variation systematics that are enabled
  Int_t _eventVariationsEnabled[NMAXEVENTVARIATIONS];
  int _nEventVariationsEnabled;
};

#endif
