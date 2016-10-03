#ifndef InputManager_h
#define InputManager_h

#include "InputConverter.hxx"
#include "Deprecated.hxx"

/// class to handle correctly any input file and convert it into the
/// analysis structure. It contains a collection of InputConverters, and
/// automatically selects the correct converter based on the file type.
///
/// The manager
/// keeps track of the current active spill, and provides three copies:
///  - RawSpill: the spill as read in from the input file.
///  - CorrectedSpill: for applying corrections to.
///  - Spill: for applying corrections and systematics to.

class InputManager{
 public :

  /// Constructor
  InputManager();

  /// Destructor, which cleans up the spills.
  virtual ~InputManager(){}
  
  /// Reset the converters and the UniqueID
  virtual void Reset();

  /// Initializes input tree(s) branches, selecting the appropriate converter
  /// and initializing it. The "conv" parameter is deprecated.
  /// Returns whether initialization was successful.
  bool Initialize(const std::string& infile_name, const std::string& conv, const bool isCosmic = false);

  /// clean up the remaining pointers to the spills.
  virtual void DeleteSpill();

  /// Read the input file, adding it to the TChains of the selected converter.
  /// If isROOTFile is false, treats the file as a text file containing a list
  /// of ROOT files to add.
  /// Returns whether the file was successfully read.
  bool ReadFile(const std::string& infile_name, bool isROOTFile);

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
  virtual  bool LoadSpill(Long64_t& entry);

  /// Fill the raw AnaEventB structure and create a AnaSuperEventB from 
  /// the raw event. The argument entry (the entry number in the input tree) 
  /// will be modified inside the method by the specific converter (hence the non-cont reference). 
  /// The way entry is modified will depend on whether the input file is EVENT based 
  /// (every call to this method will increment by one the argument entry) or 
  /// SPILL based (several events -- one per bunch -- correspond to the same spill, so 
  /// once the first event in a given spill is read entry number will not be varied 
  /// until all events in that spill are processed). At the moment oaAnalysis and MiniTree are SPILL based 
  /// while FlatTree is EVENT based.
  ///
  /// Examples on how to use this method are available in psyche/psycheSteering/vXrY/app/RunExample.cxx 
  /// and psyche/psycheSteering/vXrY/src/AnalysisManager.cxx. Just search for LoadEvent in those files. 
  ///
  /// This method Returns whether the event was successfully filled.
  bool LoadEvent(Long64_t& entry, bool delPrevious=true);

  /// Call InputConverter::LoadTree() for the selected converter.
  Long64_t LoadTree(Long64_t entry);

  /// Return the number of entries in the input file tree(s).
  Long64_t GetEntries();

  /// Return the number of events for a given number of entries in the input file tree(s).
  Int_t GetNEvents(Int_t entries=-1){return GetConverter().GetNEvents(entries);}

  /// Get a TChain with a specific name from the selected converter.
  TChain* GetChain(const std::string& name){return GetConverter().GetChain(name);}

  /// Find the first file in a list of files (if isROOTFile is true), or simply
  /// the name of the file if isROOTFile is false.
  std::string FindFirstFile(const std::string& infile_name, bool isROOTFile);

  /// Is this a ROOT file ?
  bool IsROOTFile(const std::string& infile_name);

  /// Get the current selected converter.
  InputConverter& GetConverter(){return *_selected;}

  /// Set this converter as the selected one.
  void SelectConverter(InputConverter* sel){_selected = sel;}

  /// Set the converter with name as the selected one.
  void SelectConverter(const std::string& conv);

  /// Add this converter to the vector of recognised converters.
  void AddConverter(const std::string& name, InputConverter* conv);

  /// Replace an existing converter
  void ReplaceConverter(const std::string& name, InputConverter* conv);

  /// Whether the vector of converters contains one registered with this name.
  bool HasConverter(const std::string& conv, bool err_message=true);
  
  /// Record the POT for the current spill, based on information in the AnaBeam
  /// member of the current AnaSpill.
  void IncrementPOTBySpill();

  /// Whether a FlatTree converter has been selected.
  bool InputIsFlatTree() { return (_selected->Name() == "FlatTree"); }

  /// Whether a FlatTree converter has been selected.
  bool InputIsMiniTree() { return (_selected->Name() == "MiniTree"); }

  /// Delete all the converters that have been added.
  void RemoveConverters();


  //-------------------------------

  /// Creates a clone of the corrected Spill. This must be done after applying corrections
  void MakeFinalSpill();
  
  /// Get the current spill (to have corrections applied to it).
  AnaSpillC& GetCorrectedSpill(){return *_CorrectedSpill;}

  /// Get the current spill (to have corrections and systematics applied to it).
  AnaSpillC& GetSpill(){return *_Spill;}

  /// Set the current spill (to have corrections applied to it).
  void SetCorrectedSpill(AnaSpillC*  Spill){_CorrectedSpill = Spill;}

  /// Create the event
  //  virtual AnaEventC* MakeEvent();

  /// Create the event from the Final Spill
  //  AnaEventC* MakeEventFromFinalSpill();

  /// Create the SuperEvent
  //  AnaSuperEventB*  MakeSuperEvent();

  /// Get the current spill (to have corrections and systematics applied to it).
  AnaSuperEventB& GetSuperEvent(){return *_sevent;}

  /// Reset the main spill to the corrected spill. This removes all the variations
  /// applied by the systematics.
  void ResetSpillToCorrected();

  /// Method to add a data file to the InputConverter without resetting all the trees in that converter
  /// This allows analysers to load each data event as it comes, rather than preloading them
  bool AddDataFileToConverter(const std::string& infile_name, const std::string& conv, const bool isCosmic = false, bool reset=false);

  /// Return whether thi is Monte Carlo we are dealing with
  /// This assumes data/MC status is defined once and for all for the full input (does not change on event basis)
  bool GetIsMC() {return header().GetIsMC();}

  /// Returns the software version from the header
  const std::string& GetSoftwareVersion(){return header().GetSoftwareVersion();}

  // Returns the Header class
  Header& header() {return GetConverter().header();}

 protected:

  /// Vector of converters.
  std::vector<InputConverter*> _converters;

  /// Selected converter.
  InputConverter* _selected;

  /// The current corrected spill.
  AnaSpillC* _CorrectedSpill;

  /// The current spill with corrections and systematics.
  AnaSpillC* _Spill;

  /// The super event
  AnaSuperEventB* _sevent;

  /// The previous Super Event for deleting if necessary
  AnaSuperEventB* _prevSEvent;
  
  /// Data/MC status
  bool _IsMC;

  /// The current Event UniqueID
  Int_t _uniqueID;

};

#endif


