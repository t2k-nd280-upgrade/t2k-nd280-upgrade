#ifndef CorrectionBase_h
#define CorrectionBase_h

#include <TObject.h>
#include "CoreDataClasses.hxx"

/// This is the base class that all corrections should inherit.
/// They change information contained in the current AnaSpill.
///
/// A correction fixes some aspect of the AnaSpill that is deemed incorrect.
/// The correction is only applied once per spill. The correction should
/// work on the input().GetCorrectedSpill() version of the spill.

/// All corrections should be registered with the CorrectionManager.
class CorrectionBase: public TObject{
  
public:
  
  CorrectionBase();

  /// Copy constructor
  CorrectionBase(const CorrectionBase& corr);
  
  /// Everyone should have a destructor.
  virtual ~CorrectionBase() {}
  
  //----------------------------------------------------------------
  /// This is the function that applies the correction to the input data.
  /// This MUST be overridden in the derived class.
  virtual void Apply(AnaSpillC& spill){(void)spill;}
  //----------------------------------------------------------------

  /// Return the name of this correction. This overrides the TObject::GetName() interface.
  virtual const char* GetName() const {return _name.c_str();}

  /// Return the name of this correction.
  const std::string& Name() const {return _name;}
  
  /// Set the name of this correction.
  void SetName(const std::string& name) {_name = name;}
  
  /// Enable the correction
  void Enable(){_enabled=true;}

  /// Disable the correction
  void Disable(){_enabled=false;}

  /// Is the correction enabled
  bool IsEnabled() const {return _enabled;}

  /// Set the correction as applied in the input file
  void SetAppliedInInput(bool ap) {_appliedInInput=ap;}

  /// Is the correction already applied in the input file ?
  bool IsAppliedInInput() const {return _appliedInInput;}
  
  /// Return the index of this correction
  Int_t GetIndex() const {return _index;}
  
  /// Set the index of this correction
  void SetIndex(Int_t index) {_index = index;}

  
  ClassDef(CorrectionBase, 2);
  
protected:
  
  /// The name of this correction.
  std::string _name;

  /// Is the correction enabled?
  bool _enabled;

  /// Is the correction already applied in the input file ?
  bool _appliedInInput;
  
  /// The index of the correction
  Int_t _index; 
};

#endif
