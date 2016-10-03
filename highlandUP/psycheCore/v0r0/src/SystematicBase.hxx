#ifndef SystematicBase_h
#define SystematicBase_h

#include "CoreDataClasses.hxx"
#include "ToyVariations.hxx"
#include "SelectionBase.hxx"
//#include "SystId.hxx"
#include "SystBoxB.hxx"
#include "CoreUtils.hxx"


/// This is the base class that all systematics should inherit from. 
/// - Variation: modify some aspect of the input data. The selection needs to be redone 
///   in general. This is what we call Toy Experiments
/// - Weight: A weight for the event is calculated, which depends on the event properties, but no variation on the event is 
///   performed. Selection does not have to be redone
///

class SystematicBase: public TObject{
  
public:

  /// Enumerator describing the values that _type member can take.
  enum TypeEnum {
    kVariation = 0,
    kWeight,
    kFlux,
    kUnknown
  }; //!

  /// Enumerator describing the values that _PDF member can take.

  enum PDFEnum {
    kGaussian = 0,
    kUniform,
    kBinomial,
    kMultinomial,
    kUnknownPDF
  }; //!

  /// Create the systematic, with one parameter
  SystematicBase();
    
  /// Create the systematic, specifying the number of systematic parameters
  SystematicBase(UInt_t npar);

  /// Everyone should have a destructor.
  virtual ~SystematicBase() {}
  
  /// Returns the number of systematic parameters associated to this systematic
  UInt_t GetNParameters() const {return _nParameters;}

  /// Set the number of systematic parameters associated to this systematic
  void SetNParameters(int N){_nParameters=N;}

  /// Return the name of this systematic. This overrides the TObject::GetName() interface.
  virtual const char* GetName() const {return _name.c_str();}

  /// Return the name of this systematic.
  const std::string& Name() const {return _name;}
  
  /// Set the name of this systematic.
  void SetName(const std::string& name) {_name = name;}

  /// Return the index of this systematic.
  Int_t GetIndex() const {return _index;}
  
  /// Set the index of this systematic.
  void SetIndex(Int_t index) {_index = index;}

  /// Return the type of this systematic.
  TypeEnum Type() const {return _type;}
  
  /// Return the type of this systematic.
  std::string ConvertType();
  
  /// Set the type of this systematic.
  void SetType(TypeEnum type) {_type = type;}

  /// Return the PDF of this systematic.
  PDFEnum PDF() const {return _PDF;}
  
  /// Return the PDF of this systematic.
  std::string ConvertPDF();
  
  /// Set the PDF of this systematic.
  void SetPDF(PDFEnum PDF) {_PDF = PDF;}

  /// Set the enable bit
  void SetEnabled(bool ena) {_enabled = ena;}

  /// Returns the enable bit
  bool IsEnabled() const {return _enabled;}

  /// Delete the SystBox for this event
  virtual void FinalizeEvent(const AnaEventC& event);

  /// Get the SystBox corresponding to a selection, branch and event
  SystBoxB* GetSystBox(const AnaEventC& event, Int_t isel=0, Int_t ibranch=0) const;

  /// Create the array of SystBox
  virtual void Initialize(Int_t nsel, Int_t isel, Int_t nbranch, Int_t nevents);

  ClassDef(SystematicBase, 2);
  
protected:

  /// The name of this systematic.
  std::string _name;

  /// The index of this systematic (needed by SystematicsManager);
  Int_t _index; 

  /// The type of this systematic (variation, weight or flux)
  TypeEnum _type;

  /// Is this systematic enabled ?
  bool _enabled;

  /// The PDF use for the systematic parameter scan
  PDFEnum _PDF;
  
  /// the number of systematic parameters
  UInt_t _nParameters;

protected:

  ///---------- Relevant objects for this systematic ------------------

  /// The triple array of SystBox (selection:branch:event)
  SystBoxB**** _systBoxes;  //!

  // Keep the size of the previous array. TODO: Need a better way of doing this
  Int_t _systBoxesNSel;  //!
  Int_t _systBoxesNBranches[NMAXSELECTIONS]; //!
  Int_t _systBoxesNEvents[NMAXSELECTIONS][NMAXBRANCHES]; //!

};

#endif
