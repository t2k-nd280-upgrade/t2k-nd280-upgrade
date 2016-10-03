#ifndef SelectionBase_h
#define SelectionBase_h

#include "StepBase.hxx"
#include "HLClonesArray.hxx"

#include "CoreDataClasses.hxx"
#include "Deprecated.hxx"
#include <vector>
#include <string>
#include <sstream>

/// The maximum number of steps that is supported.
const UInt_t NMAXSTEPS=50;

/// The maximum number of branches that are supported. May be expanded in the
/// future.
const UInt_t NMAXBRANCHES=20;

/// The maximum number of selections that is supported.
const UInt_t NMAXSELECTIONS=10;

/// Base class for an event selection
/// It should implement the methods  
///  - DefineSteps
///  - MakeBox
///  - MakeEventSummary
///  - FillEventSummary

/// This class that handles the registration of steps (cuts and actions),
/// and records which cuts were passed
///
/// The main way you will interact with this class is using the AddStep() function
///
/// If your analysis only has a single "branch" to it, then adding cuts is very simple. All you have to do
/// is define cuts that inherit from the CutBase base class, and add them using
///   AddStep(Step type, "Pretty name",  new YourNewCut());
///
/// You should add them in DefineSteps() method of your selection. The steps should be added in the
/// order in which you call them. You can have up to NMAXSTEPS cuts. For example for the 
/// CC-Inclusive selection
///
///   AddStep(StepBase::kCut,    "event quality",      new EventQualityCut(),           true);
///   AddStep(StepBase::kCut,    "> 0 tracks ",        new TotalMultiplicityCut(),      true);  
///   AddStep(StepBase::kAction, "find leading tracks",new FindLeadingTracksAction());  
///   AddStep(StepBase::kAction, "find vertex",        new FindVertexAction());  
///   AddStep(StepBase::kCut,    "quality+fiducial",   new TrackQualityFiducialCut(),   true);  
///   AddStep(StepBase::kAction, "find veto track",    new FindVetoTrackAction());
///   AddStep(StepBase::kCut,    "veto",               new ExternalVetoCut());
///   AddStep(StepBase::kAction, "find oofv track",    new FindOOFVTrackAction());
///   AddStep(StepBase::kCut,    "External FGD1",      new ExternalFGD1lastlayersCut());
///   AddStep(StepBase::kCut,    "muon PID",           new MuonPIDCut());
///
/// last "true" means the step sequence is broken if cut is not passed (default is "false")

/// Cut branches
///
/// Your analysis can contain several branches that contain common steps. For example, CC0pi, CC1pi and
/// CCNpi branches. You can have up to NMAXBRANCHES branches. There are a few subtleties to be aware of
/// when using branches, which are best explained using the following example code, which would all appear
/// in your yourSelection class. For the CC multi-pion selection

///   Additional actions for the multi-pi selection.
///
///     AddStep(StepBase::kAction, "find_pions", new FindPionsAction());
///     AddStep(StepBase::kAction, "find_tpc1", new FindTPC1TracksAction());

///  Add a split to the trunk with 3 branches.
///     AddSplit(3);

///  First branch is for CC-0pi
///     AddStep(0, StepBase::kCut, "CC-0pi", new NoPionCut());

///  Second branch is for CC-1pi
///     AddStep(1, StepBase::kCut, "CC-1pi", new OnePionCut());

///  Third branch is for CC-Other
///     AddStep(2, StepBase::kCut, "CC-Other", new OthersCut());

///   Set the branch aliases to the three branches
///      SetBranchAlias(0,"CC-0pi",0);
///      SetBranchAlias(1,"CC-1pi",1);
///      SetBranchAlias(2,"CC-Other",2);


class SelectionBase: public TObject{
public:
  SelectionBase(bool forceBreak=true, Int_t eventBoxId=UNASSIGNEDID);
  virtual ~SelectionBase(){}
  
  //========= These are mandatory functions ==================

  /// Define all steps in the selection
  virtual void DefineSteps(){std::cout << "ERROR: DefineSteps must be implemented in your Selection!!!" << std::endl;exit(1);}// = 0;    

  /// Define the detector Fiducial Volume in which this selection is applied
  virtual void DefineDetectorFV(){std::cout << "ERROR: DefineDetectorFV must be implemented in your Selection!!!" << std::endl;exit(1);}// = 0;    

  /// Create the appropriate type of box
  virtual ToyBoxB* MakeToyBox(){std::cout << "ERROR: MakeToyBox must be implemented in your Selection!!!" << std::endl;exit(1);return NULL;}// = 0;

  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&){std::cout << "ERROR: InitializeEvent must be implemented in your Selection!!!" << std::endl;exit(1);}// = 0;

  //==========================================================

  //---- These are optional functions, needed by FITTERS but not by highland2 analyses --------------

  /// Fill the Event Summary, needed by fitters
  virtual bool FillEventSummary(AnaEventC&, Int_t*){return true;}// = 0;

  /// Return the sample type, needed by fitters
  virtual SampleId_h GetSampleId(){return UNASSIGNEDID;}// = 0;
  
  //---- These are optional functions, but recommended to customize systematics and increase speed --------------

  /// Is this track relevant for a given systematic (prior to selection, call when initializing the event, valid for all toys in the same event)
  virtual bool IsRelevantRecObjectForSystematic(const AnaEventC&, AnaRecObjectC*, SystId_h syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}

  /// Is this true track relevant for a given systematic (prior to selection, call when initializing the event, valid for all toys in the same event)
  virtual bool IsRelevantTrueObjectForSystematic(const AnaEventC&, AnaTrueObjectC*, SystId_h syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}

  /// Is this track relevant for a given systematic (after selection, called for each toy)
  virtual bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}

  /// Is this true track relevant for a given systematic (after selection, called for each toy)
  virtual bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}

  /// Is this systematic relevant for this selection
  virtual bool IsRelevantSystematic(const AnaEventC&, const ToyBoxB&, SystId_h syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}

  /// This method is called after the systematic is applied (Apply). It should decide wheder the 
  /// selection has to be redone or not. 
  virtual bool CheckRedoSelection(const AnaEventC&, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){(void)PreviousToyBox;redoFromStep=0;return true;}

  //==========================================================

  /// Initialize this selection: defines the steps and the detectorFV
  void Initialize();

  /// Validates this selection
  void Validate() const;

  /// Apply all steps in the selection 
  bool Apply(AnaEventC& event, bool& redo);

  /// Return the title of this selection. A nice version of the name.
  const std::string& Title() const{return _title;}

  /// Return the name of this selection. This overrides the TObject::GetName() interface.
  virtual const char* GetName() const {return _name.c_str();}

  /// Return the name of this selection.
  const std::string& Name() const{return _name;}

  /// Set the title of this selection, which is the "nice" version of the selection name,
  void SetTitle(const std::string& title){_title=title;}

  /// Set the name of this selection, which is used internally by the SelectionManager
  void SetName(const std::string& name){_name=name;}

  /// enable and disable selection
  void Enable(){_enabled=true;}
  void Disable(){_enabled=false;}

  // Is this selection enabled ?
  bool IsEnabled() const {return _enabled;}

  /// When using the framework by a fitter (i.e. BANFF) we want to break the step sequence when a cut is not passed, that is 
  /// we are interested about events that pass all cuts. In this case _forceBreak should be true. On the other hand it should be false 
  /// in highland since we might be interested about events that didn't pass a specific cut. 
  bool GetForceBreak() const {return _forceBreak;}

  // Print statistics for this selection
  void PrintStatistics() const;
  void PrintStatistics(const std::string& name) const;
  void PrintStatistics(UInt_t ID) const;

  /// Create the array of PreviousToyBox
  void CreateToyBoxArray(Int_t nevents);

  /// Delete the PreviousToyBox pointer for the last toy of the event
  void FinalizeEvent(const AnaEventC& event);

  //-----------------------------------------------------------------

  /// Add a user step to the selection. 
  /// One should specify the branching sequence, the step type, the title and the cut_break (whether to break the step sequence quen a cut is not passed)
  void AddStep(                                                                                StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);
  void AddStep(Int_t b0,                                                                       StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);
  void AddStep(Int_t b0, Int_t b1,                                                             StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);
  void AddStep(Int_t b0, Int_t b1, Int_t b2,                                                   StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);
  void AddStep(Int_t b0, Int_t b1, Int_t b2, Int_t b3,                                         StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);
  void AddStep(Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4,                               StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);
  void AddStep(Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5,                     StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);
  void AddStep(Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5, Int_t b6,           StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);
  void AddStep(Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5, Int_t b6, Int_t b7, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);
  void AddStep(const std::vector<UInt_t>& branch_seq,                                          StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);

  // This is the basic function to add the step
  void AddStep(const std::vector<UInt_t>& branch_seq, StepBase* step);

  // This is the basic function to add several step
  void AddSteps(const std::vector<UInt_t>& branch_seq, const std::vector<StepBase*>& steps);

  /// Add a split in the step sequence. The user should specify the number of branches in this split and the branch sequence
  void AddSplit(UInt_t nbranches, Int_t b0=-1, Int_t b1=-1, Int_t b2=-1, Int_t b3=-1, Int_t b4=-1, Int_t b5=-1, Int_t b6=-1, Int_t b7=-1);
  void AddSplit(UInt_t nbranches, const std::vector<UInt_t>& branch_seq);

  void AddBranch(Int_t ibranch, const std::vector<UInt_t>& branch_seq, const std::string& alias="");


  /// Replace a step (same structure of AddStep, but for only one step at once)
//  void ReplaceStep(const std::vector<UInt_t>& branch_seq, UInt_t step_nbr, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);
  
  /// Remove a step
//  void RemoveStep(const std::vector<UInt_t>& branch_seq, UInt_t step_nbr);

  /// Insert a step after the specified cut
//  void InsertStep(const std::vector<UInt_t>& branch_seq, UInt_t step_nbr,StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);

  /// Disable step
//  void DisableStep(const std::vector<UInt_t>& branch_seq, UInt_t step_nbr);

  /// Disable step provided title and branch ID
  void DisableStep(const std::string& title, Int_t ID=-1);

  /// Disable step provided title and branch ID
  void RemoveStep(const std::string& title, Int_t ID=-1);

  /// Replace step provided title and branch ID of the step to replace
  void ReplaceStep(const std::string& old_title, Int_t ID, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);

  // Insert step after step with a given title and branch ID
  void InsertStep(const std::string& previous_title,  Int_t ID, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break=false);


  /// Copy a clone of the steps in range first-last from branch sbranch1 in selection ssel1 to sbranch2 in selection ssel2
  void CopySteps(SelectionBase& sel1, UInt_t branchID1,                    UInt_t first, UInt_t last, 
                 Int_t b0=-1, Int_t b1=-1, Int_t b2=-1, Int_t b3=-1, Int_t b4=-1, Int_t b5=-1, Int_t b6=-1, Int_t b7=-1);

  void CopySteps(SelectionBase& sel1, const std::string& sbranch1, UInt_t first, UInt_t last, 
                 Int_t b0=-1, Int_t b1=-1, Int_t b2=-1, Int_t b3=-1, Int_t b4=-1, Int_t b5=-1, Int_t b6=-1, Int_t b7=-1);

  void CopySteps(SelectionBase& sel1, UInt_t branchID1, UInt_t first, UInt_t last, const std::vector<UInt_t>& branch_seq2);
  //  void CopySteps(SelectionBase& sel1, UInt_t first, UInt_t last,const std::string& sbranch2="trunk"){
  //    CopySteps(sel1,"trunk",first,last,sbranch2);
  //  }
  
  /// Copy a clone of all the steps from branch sbranch1 in selection ssel1 to first branch in selection 2
  void CopySteps(SelectionBase& sel1, const std::string& sbranch1="trunk");
    
  
  /// Copy a clone of the steps with number istep from branch sbranch1 in selection ssel1 to sbranch2 in selection ssel2
  void CopyStep(SelectionBase& sel1, const std::string& sbranch1, UInt_t istep, Int_t b0=-1, Int_t b1=-1, Int_t b2=-1, Int_t b3=-1, Int_t b4=-1, Int_t b5=-1, Int_t b6=-1, Int_t b7=-1);  
  void CopyStep(SelectionBase& sel1, UInt_t istep, Int_t b0=-1, Int_t b1=-1, Int_t b2=-1, Int_t b3=-1, Int_t b4=-1, Int_t b5=-1, Int_t b6=-1, Int_t b7=-1){
    CopyStep(sel1,"trunk",istep, b0,b1,b2,b3,b4,b5,b6,b7);
  }


  //-----------------------------------------------------

  /// Get all steps in a given branch provided the branch sequence (vector of integers)
  std::vector<StepBase*> GetStepsInBranch(const std::vector<UInt_t>& branch_seq, UInt_t first=0, UInt_t last=NMAXSTEPS-1) const;

  /// Get all steps in a given branch provided the branch sequence (vector of integers)
  std::vector<StepBase*> GetStepsInBranchWithDummy(const std::vector<UInt_t>& branch_seq, UInt_t first=0, UInt_t last=NMAXSTEPS-1) const;

  /// Get all steps in a given branch provided the branch alias
  std::vector<StepBase*> GetStepsInBranch(const std::string& branch, UInt_t first=0, UInt_t last=NMAXSTEPS-1) const;

  /// Get all steps in a given branch provided the branch uniqueID
  std::vector<StepBase*> GetStepsInBranch(UInt_t ID, UInt_t first=0, UInt_t last=NMAXSTEPS-1) const;

  /// Get all cuts in a given branch provided the branch alias
  std::vector<StepBase*> GetCutsInBranch(const std::string& branch) const;

  /// Get all cuts in a given branch provided the branch uniqueID
  std::vector<StepBase*> GetCutsInBranch(UInt_t ID) const;

  /// Get the last step in a given branch provided the branch sequence (individual integers)
  StepBase* GetLastStepInBranch(Int_t b0=-1, Int_t b1=-1, Int_t b2=-1, Int_t b3=-1, Int_t b4=-1, Int_t b5=-1, Int_t b6=-1, Int_t b7=-1) const;

  /// Get the last step in a given branch provided the branch sequence (vector of integers)
  StepBase* GetLastStepInBranch(const std::vector<UInt_t>& branch) const;

  /// Get the last step in a given branch provided the branch alias
  StepBase* GetLastStepInBranch(const std::string& branch) const;

  /// Get step in a given branch provided the branch sequence (vector of integers) and the step index
  StepBase* GetStepInBranch(const std::vector<UInt_t>& branch_seq, UInt_t index) const;

  /// Get step in a given branch provided the branch alias and the step index
  StepBase* GetStepInBranch(const std::string& branch, UInt_t index) const;

  /// Get step in a given branch provided the branch uniqueID and the step index
  StepBase* GetStepInBranch(UInt_t ID, UInt_t index) const;

  //-----------------------------------------------------

  /// Convert the branch sequence into a string
  std::string ConvertBranchToString(const std::vector<UInt_t>& branch, Int_t upToSplit=-1) const;

  /// Convert the branch sequence of individual integers into a vector of integers
  std::vector<UInt_t> ConvertBranch(Int_t b0=-1, Int_t b1=-1, Int_t b2=-1, Int_t b3=-1, Int_t b4=-1, Int_t b5=-1, Int_t b6=-1, Int_t b7=-1) const;
  
  /// Set the branch alias and unique ID provided the branch sequence
  void SetBranchAlias(Int_t ID, const std::string& name, Int_t b0=-1, Int_t b1=-1, Int_t b2=-1, Int_t b3=-1, Int_t b4=-1, Int_t b5=-1, Int_t b6=-1, Int_t b7=-1);
  
  /// Get the branch sequence for a specific branch alias
  std::vector<UInt_t> GetBranchSequence(const std::string& name) const;

  /// Get the branch sequence for a specific branch unique ID
  std::vector<UInt_t> GetBranchSequence(Int_t ID) const;

  /// Gets the branch unique ID with a given alias
  Int_t GetBranchUniqueID(const std::string& name) const;

  /// Gets the branch alias for a given branch unique ID
  std::string GetBranchAlias(Int_t ID) const;

  //-----------------------------------------------------

  /// Apply a range of steps in a branch
  bool ApplySteps(AnaEventC& event, ToyBoxB& box, const std::string& branch, Int_t ifirst=0, Int_t ilast=NMAXSTEPS);

  /// Apply all registered steps in a threadsafe way
  bool ApplySteps(AnaEventC& event, ToyBoxB& box, Int_t firstStepToApply);

  /// Return whether the specified cut was passed or not.
  bool GetCutPassed(Int_t icut, Int_t branch=0) const{return _cut_passed[branch][icut];}

  /// Get the accumulated cut level - the maximum cut number for which all
  /// lower cut numbers were passed.
  //  Int_t GetAccumCutLevel(Int_t branch=0) const{return _accum_cut_level[branch];}
  Int_t GetAccumCutLevel(Int_t branch=0) const{return GetPreviousToyBox().AccumLevel[branch];}

  /// Reset all cuts to "not passed", and set the accumulated cut level to 0.
  void InitializeCutLevels();

  /// Get the maximum cut level for this event and the last processed toy
  Int_t GetMaxAccumCutLevel() const {return GetPreviousToyBox().MaxAccumLevel;}

  /// Get the maximum cut level for this event and all toys
  Int_t GetMaxAccumCutLevelForAllToys() const {return _maxAccumLevelForAllToys;}

  /// Is the preselection passed in any of the branches
  bool PreSelectionPassed(const AnaEventC& event);

  /// Set the pre-selection accum level
  void SetPreSelectionAccumLevel(Int_t presel){_presel_accum_cut_level=presel;}

  /// Set the pre-selection accum level
  Int_t GetPreSelectionAccumLevel() const{return _presel_accum_cut_level;}

  /// Set the Selection index
  void SetEnabledIndex(Int_t index) {_selEnabledIndex=index;}

  /// Get the Selection index
  Int_t GetEnabledIndex() const {return _selEnabledIndex;}

  /// Set the detector in which the Fiducial Volume is defined 
  void SetDetectorFV(SubDetId_h det, Int_t ibranch=-1);

  /// Get the detector in which the Fiducial Volume is defined 
  SubDetId_h GetDetectorFV(Int_t ibranch=0) const;

  /// Returns the Id of the EventBox to be used in this selection
  EventBoxId_h GetEventBoxId() const { return _eventBoxId;}

  //-----------------------------------------------------  

  /// Check the existence of branch 
  bool HasBranch(Int_t ibranch, const std::string& fromMethod="HasBranch") const;

  /// Return the number of branches
  UInt_t GetNBranches() const{return _branchSequence.size();}

  /// Return all the names of the steps, in the order they were added.
  std::vector<std::string> GetStepNames(Int_t ibranch=0) const;

  /// Return all the names of the cuts, in the order they were added.
  std::vector<std::string> GetCutNames(Int_t ibranch=0) const;

  /// Return an step provided the title and the Branch ID
  StepBase* GetStep(const std::string& title, Int_t ID=0) const;

  /// Return the cut number (starting at 0) corresponding to an step with a given title
  Int_t GetCutNumber(const std::string& title, Int_t ID=0) const;

  /// Return the step number (starting at 0) corresponding to an step with a given title
  Int_t GetStepNumber(const std::string& title, Int_t ID=0) const;

  /// Return the number of steps in a given branch
//  UInt_t GetNSteps(Int_t branch);

  /// Return the number of cuts that have been added.
  UInt_t GetNMaxCuts() const;

  /// Return the number of cuts in a given branch
  UInt_t GetNCuts(Int_t branch) const;

  /// By default EventSummary class is created and filled when the selection is passed. But this might be necessary also for events not passing the selection
  /// The user can force the EventSummary to be filled always. 
  void SetForceFillEventSummary(bool force){_forceFillEventSummary= force;}

  /// Returns the flag to fill the EventSummary even when the selection is not passed
  bool GetForceFillEventSummary() const { return _forceFillEventSummary;}

  /// Dumps the list of branches
  void DumpBranches() const;

  /// Print out the index, name and title of each step for a given branch (no argument for all branches)
  void DumpSteps(const std::string& branch="", bool onlycuts=false) const;
  void DumpSteps(Int_t ID, bool onlycuts=false) const;

  /// Print out the index, name and title of each cut for a given branch (no argument for all branches)
  void DumpCuts(Int_t branch=-1) const {DumpSteps(branch,true);}

  /// Set the Run periods (defined in AnalysisUtils) for which this selection is valid
  /// By default all run periods are valid
  void SetValidRunPeriods(std::string runPeriods);

  /// Method to see whether this selection should be applied to the given run period
  bool IsValidRun(int runPeriod) const {return _validRunPeriods[runPeriod];}

  /// Get the ToyBox of the last processed toy for a particular event
  const ToyBoxB& GetPreviousToyBox(const AnaEventC& event) const;

  /// Get the ToyBox of the last processed toy (only for no MULTITHREAD)
  const ToyBoxB& GetPreviousToyBox() const;

  //-----------------------------------------------------  

  ClassDef(SelectionBase, 1);
    
protected:

  /// Add a branch unique ID to all steps in that branch
  void AddBranchUniqueID(std::vector<StepBase*>& steps, UInt_t ibranch);

  /// Apply the step. This is the
  /// important function that calls Apply() in the relevant step.
  bool ApplyStep(AnaEventC& event, ToyBoxB& box, const StepBase& step, Int_t branch=-1);

  /// Apply all steps recursively provided the first step in a threadsafe way
  bool ApplyStepRecursive(AnaEventC& event, ToyBoxB& box, const StepBase& step, Int_t firstStepToApply);

  /// Set whether the current cut was passed, and increment the current cut
  /// counter. Return value is the same as the input "passed" value. Calls
  /// the other CutPassed interface if "passed" is true.
  bool CutPassed(bool passed, Int_t branch, ToyBoxB& box);

  /// Check whetehr a cut is passed and set the relevant all_cuts_passed field if it is not passed
  /// THis is the threadsafe version of the selection
  bool CutPassed(bool ok, const StepBase& step, ToyBoxB& box);


protected:

  /// The index of the selection in the selection manager (for enabled selections)
  Int_t _selEnabledIndex; //!

  // The detector FV in which the selection is performed
  SubDetId_h _detectorFV[NMAXBRANCHES]; //!

  /// The name of the selection
  std::string _name;

  /// the nice name of the selection
  std::string _title;

  /// is this selection enabled ?
  bool _enabled;
  
  /// The preselection accum level
  Int_t _presel_accum_cut_level;

  /// The cuts that were passed.
  bool _cut_passed[NMAXBRANCHES][NMAXSTEPS]; //!

  /// The current cut level.
  Int_t _current_cut[NMAXBRANCHES];  //!

  /// The top level steps in the trunk
  std::vector<StepBase*> _firstSteps;

  /// Association between a branch unique ID (entry in vector) and branch alias
  std::vector<std::string> _branchAlias;  

  /// Association between a branch unique ID (entry in vector) and branch sequence
  std::vector< std::vector<UInt_t> > _branchSequence; 

  /// Number of cuts in each branch
  std::vector<UInt_t> _nCutsInBranch; 

  /// Used for statistics
  std::vector<UInt_t > _nEventsPassed;    //!
  int _cut_passed_stats[NMAXBRANCHES][NMAXSTEPS]; //!

  /// When using the framework by a fitter (i.e. BANFF) we want to break the step sequence when a cut is not passed, that is 
  /// we are interested about events that pass all cuts. In this case _forceBreak should be true. On the other hand it should be false 
  /// in highland since we might be interested about events that didn't pass a specific cut. 
  bool _forceBreak;

  /// By default EventSummary class is created and filled when the selection is passed. But this might be necessary also for events not passing the selection
  /// The user can force the EventSummary to be filled always. 
  bool _forceFillEventSummary;    //!

  /// Boolean array to store valid run periods for this selection
  /// Should use enumeration in future to set size of this
  bool _validRunPeriods[10]; //!

  /// Array of pointers to the PreviousToyBox (for each event)
  ToyBoxB** PreviousToyBox; //!

  /// This is the maximum accum level of any of the branches in all toys
  Int_t _maxAccumLevelForAllToys; //!

  /// The ID of the EventBox to be used
  EventBoxId_h _eventBoxId; //!

  /// Is this selection initialized ?
  bool _initialized; //!
};


#endif
