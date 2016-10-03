#ifndef HighlandInputManager_h
#define HighlandInputManager_h

#include "InputManager.hxx"
//#include "BaseDataClasses.hxx"

/// class to handle correctly any input file and convert it into the
/// analysis structure. It contains a collection of InputConverters, and
/// automatically selects the correct converter based on the file type.
///
/// The manager
/// keeps track of the current active spill, and provides three copies:
///  - RawSpill: the spill as read in from the input file.
///  - CorrectedSpill: for applying corrections to.
///  - Spill: for applying corrections and systematics to.

class HighlandInputManager: public InputManager{
 public :

  /// Constructor
  HighlandInputManager();

  /// Destructor, which cleans up the spills.
  virtual ~HighlandInputManager(){}
  
  /// Reset the converters and the UniqueID
  void Reset();

  /// clean up the remaining pointers to the spills.
  void DeleteSpill();

  /// Read one or several entries in the input tree(s) to fill a raw AnaSpill, and
  /// sets the CorrectedSpill and Spill as copies of it (to be manipulated
  /// later).
  /// The argument entry (the entry number in the input tree) 
  /// will be modified inside the method by the specific converter (hence the non-cont reference). 
  /// The way entry is modified will depend on whether the input file is SPILL based 
  /// (every call to this method will increment by one the argument entry) or 
  /// EVENT based (entry number will be incremented in several units to account for 
  /// several events -- bunches -- in the same spill). At the moment oaAnalysis and MiniTree are SPILL based 
  /// while FlatTree is EVENT based.
  ///
  /// Examples on how to use this method are available in highland2/highlandTools/vXrY/src/AnalysisLoop.cxx, 
  /// and highland2/highlandTools/vXrY/src/SimpleLoopBase.cxx. Just search for LoadSpill in those files. 
  ///
  /// This method Returns whether the event was successfully filled.
  bool LoadSpill(Long64_t& entry);

  /// Whether an oaAnalysisTree converter has been selected.
  DEPRECATED(bool InputIsoaAnalysisTree() { return (_selected->Name() == "oaAnalysisTree"); });

  /// Whether an OriginalTree converter has been selected.
  bool InputIsOriginalTree() { return (_selected->Name() == "OriginalTree" || _selected->Name() == "oaAnalysisTree"); }

  //-------------------------------
  
  /// Get the current spill (constant, as read in from the input file).
  const AnaSpillC& GetRawSpill(){return *_RawSpill;}

  /// Set the current spill (constant, as read in from the input file).
  void SetRawSpill(AnaSpillC*  RawSpill){_RawSpill = RawSpill;}

  /// Get the current bunch (constant, as read in from the input file).
  const AnaBunchC& GetRawBunch(){return *_RawSpill->Bunches[_currentBunch];}

  /// Get the current bunch (to have corrections applied to it).
  AnaBunchC& GetCorrectedBunch(){return *_CorrectedSpill->Bunches[_currentBunch];}

  /// Get the current bunch (to have corrections and systematics applied to it).
  AnaBunchC& GetBunch(){return *_Spill->Bunches[_currentBunch];}

  /// Set the current bunch index.
  void SetCurrentBunch(int ibunch){_currentBunch = ibunch;}

  /// Create the event
  //  AnaEventC* MakeEvent();

  /// Reset the main spill to the raw spill. This removes all the variations
  /// applied by the systematics.
  void ResetSpillToRaw();

 private:

  /// The current raw spill.
  AnaSpillC* _RawSpill;

  /// The current active bunch in the current spill.
  int _currentBunch;

};

#endif


