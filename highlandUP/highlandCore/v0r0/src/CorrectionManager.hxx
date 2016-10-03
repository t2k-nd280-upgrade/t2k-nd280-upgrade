#ifndef CorrectionManager_h
#define CorrectionManager_h

#include "CorrectionBase.hxx"
#include "HLClonesArray.hxx"

/// The maximum number of corrections that is supported.
const UInt_t NMAXCORRECTIONS = 20;


/// Manager for Corrections. An Correction modifies a particular aspect
/// of the Input data. This is used for calculating systematics and applying
/// corrections.
///
/// This manager handles the registration, enabling and disabling of the
/// corrections.
class CorrectionManager: public HLClonesArray {

public:

  CorrectionManager();
  virtual ~CorrectionManager();

  /// Apply all corrections 
  void ApplyCorrections(AnaSpillC& spill);

  /// Get the input correction registered with the given index
  CorrectionBase* GetCorrection(Int_t index);

  /// Enable the correction registered with the given index for the
  /// specified configuration. (if conf=="" for all configs)
  void EnableCorrection(Int_t index);

  /// Disable the correction registered with the given index for the
  /// specified configuration.
  void DisableCorrection(Int_t index);
     
  /// Check if a particular correction is enabled in a given configuration.
  bool IsEnabled(Int_t index);
  

  /// Get the input correction registered with the given name.
	CorrectionBase* GetCorrection(const std::string& name);
	
	/// Enable the correction registered with the given name for the
	/// specified configuration. (if conf=="" for all configs)
	void EnableCorrection(const std::string& corr);
	
	/// Disable the correction registered with the given name for the
	/// specified configuration.
	void DisableCorrection(const std::string& corr);
	   
  /// Check if a particular correction is enabled in a given configuration.
  bool IsEnabled(const std::string& corr);
  

  /// Disable all corrections in a given configuration (if conf=="" for all confs)
  void DisableAllCorrections();

  /// Register an Correction as a correction. These will only be applied
  /// once per spill.
  void AddCorrection(Int_t index, const std::string& name, CorrectionBase* corr);
  
  /// This is for bwd compatibility and will be removed when approved
  void AddCorrection(const std::string& name, CorrectionBase* corr){
    AddCorrection(-1, name, corr);
  }
  
   /// If set to true corrections applied in the input file are applied again
  void SetForceApplyCorrections(bool force) {_forceApplyCorrections=force;}

  /// Readthe corrections from a file
  void ReadCorrections(const std::string& file, bool input=false);

  /// Dump all corrections
  void DumpCorrections();

  protected:

  
  /// Reset the corrections data
  void Reset();
  
  /// The registered corrections, and the names they were registered with.
  std::vector<CorrectionBase*> _corrections;

  //  std::vector<CorrectionBase*> _corrections_write;
  
  // By default corrections applied in the input file are not applied again
  bool _forceApplyCorrections;
};

#endif
