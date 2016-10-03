#ifndef SystematicManager_h
#define SystematicManager_h

#include "EventVariationBase.hxx"
#include "EventWeightBase.hxx"
#include "SystematicCovariance.hxx"
#include "SelectionManager.hxx"
#include "TRandom3.h"
#include "TString.h"
#include "HLClonesArray.hxx"
#include "ToyExperiment.hxx"

class SystematicCovariance;

/// The maximum number of systematics that is supported.
//const int NMAXSYSTEMATICS=30;

/// Manager for Systematics. A systematic can be of type "variation" or "weight". The former 
/// modifies a particular aspect of the Input data. The last computes a event weight. 
/// Two different base classes are used EventVariationBase and EventWeightBase

/// This manager handles the registration, enabling and disabling of the
/// systematics.
class SystematicManager: public HLClonesArray {

public:

  SystematicManager();
  virtual ~SystematicManager();

  /// Apply all variationSystematics
  void ApplyVariationSystematics(const ToyExperiment& toy, AnaEventC& event);
  
  /// Aplpy all weightSystematics. Returns the total event normalization weight
  Weight_h ApplyWeightSystematics(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox );
 
  /// Apply all weightSystematics. Returns the total event normalization weight and vector of weights as argument (one for each systematic parameter)
  Weight_h ApplyWeightSystematics(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox, Weight_h* weights);

  /// Apply all weightSystematics for a given selection. Returns the total event normalization weight and vector of weights as argument (one for each systematic parameter)
  Weight_h ApplyWeightSystematics(const SelectionBase& sel, const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox, Weight_h* weights);

  /// Apply all fluxSystematics. Returns the total event Weight
  Weight_h ApplyFluxSystematics(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox);

  /// Apply weight Systematics with a given index. Returns the event normalization weight
  Weight_h ApplyWeightSystematic(Int_t index, const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox);

  /// Undo the variation systematic (Undo the variation, that is, go back to the previous values of modified observables
  bool UndoVariationSystematics(AnaEventC& event);

  /// Get the systematic registered with the given name.
  EventVariationBase*  GetSystematicVariation(const std::string& name) const;
  EventWeightBase*     GetSystematicWeight(const std::string& name) const;
  EventWeightBase*     GetSystematicFlux(const std::string& name) const;
  SystematicBase*           GetSystematic(const std::string& name) const;

  /// Get the systematic registered with the given index.
  EventVariationBase*  GetSystematicVariation(Int_t index) const;
  EventWeightBase*     GetSystematicWeight(Int_t index) const;
  EventWeightBase*     GetSystematicFlux(Int_t index) const;
  SystematicBase*           GetSystematic(Int_t index) const;

  /// Get the index of a systematic registered with the given name.
  Int_t GetSystematicVariationIndex(const std::string& name);
  Int_t GetSystematicWeightIndex(const std::string& name);
  Int_t GetSystematicFluxIndex(const std::string& name);

  /// Get the vector of variationSystematics
  EventVariationBase** GetVariationSystematics(int& nSys) {
    nSys = _nVariationSystematics;  
    return _variationSystematics;
  }

  /// Get the vector of weightsSystematics
  EventWeightBase** GetWeightSystematics(int& nSys) {
    nSys = _nWeightSystematics;  
    return _weightSystematics;
  }

  /// Get the vector of fluxSystematics
  EventWeightBase** GetFluxSystematics(int& nSys) {
    nSys = _nFluxSystematics;  
    return _fluxSystematics;
  }

  /// Get the vector containing all systematics (contains NULL pointers)
  std::vector<SystematicBase*>& GetSystematics() {
    return _systematics;
  }

  /// Get the vector containing systematics with specific indices
  std::vector<SystematicBase*> GetSystematics(const std::vector<Int_t>& indices) const;

  /// Enable the systematic registered with the given index
  void EnableSystematic(Int_t index);

  /// Disable the systematic registered with the given index
  void DisableSystematic(Int_t index);

  /// Enable the systematics registered with the given indices
  void EnableSystematics(const std::vector<Int_t>& systs);

  /// Disable the systematics registered with the given indices
  void DisableSystematics(const std::vector<Int_t>& systs);

  /// Disable all variationSystematics 
  void DisableAllVariationSystematics();

  /// Enable all variationSystematics 
  void EnableAllVariationSystematics();

  /// Disable all weightSystematics 
  void DisableAllWeightSystematics();

  /// Enable all systematic weightSystematics 
  void EnableAllWeightSystematics();

  /// Disable all fluxSystematics 
  void DisableAllFluxSystematics();

  /// Enable all systematic fluxSystematics 
  void EnableAllFluxSystematics();

  /// Disable all Systematics 
  void DisableAllSystematics();

  /// Enable all Systematics 
  void EnableAllSystematics();

  /// Register an EventVariationBase as a variation systematic
  void AddVariationSystematic(Int_t index, EventVariationBase* sys);

  /// Register an EventVariationBase as a variation systematic provided the index and name
  void AddVariationSystematic(Int_t index, const std::string& name, EventVariationBase* sys);

  /// Replace one of the existing EventVariationBase
  void ReplaceVariationSystematic(Int_t index, EventVariationBase* sys);

  /// Register an SystematicBase as a weight systematic
  void AddWeightSystematic(Int_t index, EventWeightBase* sys);

  /// Register an SystematicBase as a weight systematic provided the index and name
  void AddWeightSystematic(Int_t index, const std::string& name, EventWeightBase* sys);

  /// Replace one of the existing EventWeightBase
  void ReplaceWeightSystematic(Int_t index, EventWeightBase* sys);

  /// Register an SystematicBase as a flux systematic
  void AddFluxSystematic(Int_t index, EventWeightBase* sys);

  /// Register an SystematicBase as a flux systematic
  void AddFluxSystematic(Int_t index,  const std::string& name, EventWeightBase* sys);

  /// Replace one of the existing EventWeightBase
  void ReplaceFluxSystematic(Int_t index, EventWeightBase* sys);

  // Delete the SystBox for all systematics
  void FinalizeEventSystematics(AnaEventC& event);

  // Fill the SystBox for the enabled systematics
  void InitializeEventSystematics(SelectionManager& sel, AnaEventC& event);

  // Create the arrays of SysteBox's
  void Initialize(SelectionManager& sel,Int_t nevents);

  /// Is there any systematic added ?
  bool HasSystematics() const {return (_nVariationSystematics+_nWeightSystematics+_nFluxSystematics!=0);}
  
  /// Returns the number of VariationSystematics 
  UInt_t  GetNVariationSystematics() const {return _nVariationSystematics;}

  /// Returns the number of WeightSystematics 
  UInt_t  GetNWeightSystematics() const {return _nWeightSystematics;}

  /// Returns the number of Flux Systematics
  UInt_t  GetNFluxSystematics() const {return _nFluxSystematics;}

  /// Returns the vector of indices for all enabled weight systematics
  const Int_t* GetEnabledWeightSystematics(int& nSys){
      nSys = _nWeightSystematicsEnabled;    
      return _weightSystematicsEnabled;
  }

  /// Returns the number of enabled weight systematics
  UInt_t GetNEnabledWeightSystematics(){ return _nWeightSystematicsEnabled;}

  /// Returns the vector of indices for all enabled variation systematics
  const Int_t* GetEnabledVariationSystematics(int& nSys){
      nSys = _nVariationSystematicsEnabled;    
      return _variationSystematicsEnabled;
  }

  /// Returns the number of enabled variation systematics
  UInt_t GetNEnabledVariationSystematics(){ return _nVariationSystematicsEnabled;}

  /// Read the systematics from a file
  void ReadSystematics(const std::string& file);

  /// Dump all systematics
  void DumpSystematics();

  /// Dump all variationSystematics
  void DumpVariationSystematics();

  /// Dump all weightSystematics
  void DumpWeightSystematics();

  /// Dump all fluxSystematics
  void DumpFluxSystematics();

  /// Gets the PDF bin for a systematic variation with a given with index 
  Int_t GetPDFBin(int syst);

  /// Sets the PDF bin for a systematic variation with a given with index 
  void SetPDFBin(int syst, int bin);

  /// Check if a systematic already exists
  bool HasSystematic(const std::string& name);

  /// Make the covariance
  void ConstructCovarianceMatrix();

  /// Get the covariance matrix
  TMatrixT<double>* GetCovarianceMatrix() const;


protected:
  
  /// The systematics that were registered as variationSystematics.
  EventVariationBase* _variationSystematics[NMAXSYSTEMATICS]; //!
  int _nVariationSystematics;

  /// The systematics that were registered as weightSystematics.
  EventWeightBase* _weightSystematics[NMAXSYSTEMATICS];  //!
  int _nWeightSystematics;

  /// The systematics that were registered as fluxSystematics.
  EventWeightBase* _fluxSystematics[NMAXSYSTEMATICS];  //!
  int _nFluxSystematics;

  /// The systematics that were registered
  std::vector<SystematicBase*> _systematics;

  /// The variation systematics that are enabled
  Int_t _variationSystematicsEnabled[NMAXSYSTEMATICS];
  int _nVariationSystematicsEnabled;

  /// The weight systematics that are enabled
  Int_t _weightSystematicsEnabled[NMAXSYSTEMATICS];
  int _nWeightSystematicsEnabled;

  /// The weight systematics that are enabled
  Int_t _fluxSystematicsEnabled[NMAXSYSTEMATICS];
  int _nFluxSystematicsEnabled;

  /// A random generator that can be used to generate throws.
  TRandom3 _RandomGenerator;  //!
    
  ///The associated systematic covariance
  SystematicCovariance* _covariance;  //!

};

#endif
