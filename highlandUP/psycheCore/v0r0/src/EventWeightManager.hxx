#ifndef EventWeightManager_h
#define EventWeightManager_h

#include "EventWeightBase.hxx"
#include "SelectionManager.hxx"
#include "TString.h"
#include "ToyExperiment.hxx"

/// The maximum number of EventWeights that is supported.
const UInt_t NMAXEVENTWEIGHTS=100;

/// Manager for EventWeights
/// This manager handles the registration, enabling and disabling of the
/// EventWeights.
class EventWeightManager{

public:

  EventWeightManager();
  virtual ~EventWeightManager();

  /// Compute all eventWeights. Returns the total event normalization weight
  Weight_h ComputeEventWeights(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox );
 
  /// Compute all eventWeights. Returns the total event normalization weight and vector of weights as argument (one for each EventWeight)
  Weight_h ComputeEventWeights(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox, Weight_h* weights);

  /// Compute all eventWeights for a given selection. Returns the total event normalization weight and vector of weights as argument (one for each EventWeight)
  Weight_h ComputeEventWeights(const SelectionBase& sel, const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox, Weight_h* weights);

  /// Compute EventWeight with a given index. Returns the event normalization weight
  Weight_h ComputeEventWeight(Int_t index, const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox);

  /// Get the EventWeight registered with the given name.
  EventWeightBase*     GetEventWeight(const std::string& name) const;

  /// Get the EventWeight registered with the given index.
  EventWeightBase*     GetEventWeight(Int_t index) const;

  /// Get the index of a EventWeight registered with the given name.
  Int_t GetEventWeightIndex(const std::string& name);

  /// Get the vector of EventWeights
  std::vector<EventWeightBase*>& GetEventWeights(){return _eventWeights;}

  /// Get the vector containing EventWeights with specific indices
  std::vector<EventWeightBase*> GetEventWeights(const std::vector<Int_t>& indices) const;

  /// Enable the EventWeight registered with the given index
  void EnableEventWeight(Int_t index);

  /// Disable the EventWeight registered with the given index
  void DisableEventWeight(Int_t index);

  /// Enable the EventWeights registered with the given indices
  void EnableEventWeights(const std::vector<Int_t>& systs);

  /// Disable the EventWeights registered with the given indices
  void DisableEventWeights(const std::vector<Int_t>& systs);

  /// Disable all eventWeights 
  void DisableAllEventWeights();

  /// Enable all EventWeight eventWeights 
  void EnableAllEventWeights();

  /// Add a new Event Weight provided its index in the manager and a pointer to it
  void AddEventWeight(Int_t index, EventWeightBase* sys);

  /// Add a new Event Weight provided its index in the manager, its name and a pointer to it
  void AddEventWeight(Int_t index, const std::string& name, EventWeightBase* sys);

  /// Replace one of the existing EventWeightBase
  void ReplaceEventWeight(Int_t index, EventWeightBase* sys);

  /// Fill the SystBox for the enabled EventWeights
  void InitializeEvent(SelectionManager& sel, AnaEventC& event);

  /// Delete the SystBox for all EventWeights
  void FinalizeEvent(AnaEventC& event);

  // Create the arrays of SystBox's
  void Initialize(SelectionManager& sel,Int_t nevents);

  /// Is there any EventWeight added ?
  bool HasEventWeights() const {return (_eventWeights.size()!=0);}

  /// Returns the number of EventWeights 
  UInt_t  GetNEventWeights() const {return _eventWeights.size();}

  /// Returns the vector of indices for all enabled EventWeights
  const Int_t* GetEnabledEventWeights(int& nSys){
      nSys = _nEventWeightsEnabled;    
      return _eventWeightsEnabled;
  }

  /// Returns the number of enabled EventWeights
  UInt_t GetNEnabledEventWeights(){ return _nEventWeightsEnabled;}

  /// Dump all eventWeights
  void DumpEventWeights();

  /// Check if an EventWeight with a given name already exists in the manager
  bool HasEventWeight(const std::string& name);

protected:

  /// The systematics that were registered as eventWeights.
  std::vector<EventWeightBase*> _eventWeights; 

  /// The systematics that were registered as eventWeights.
  std::vector<EventWeightBase*> _eventWeightsWithNull; 

  /// The weight systematics that are enabled
  Int_t _eventWeightsEnabled[NMAXEVENTWEIGHTS];
  int _nEventWeightsEnabled;
};

#endif
