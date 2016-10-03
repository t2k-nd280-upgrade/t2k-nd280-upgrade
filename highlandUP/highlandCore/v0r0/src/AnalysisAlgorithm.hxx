#ifndef AnalysisAlgorithm_h
#define AnalysisAlgorithm_h

#include "HighlandInputManager.hxx"
#include "SelectionManager.hxx"
#include "OutputManager.hxx"
#include "ConfigurationManager.hxx"
#include "CorrectionManager.hxx"
#include "SystematicManager.hxx"
#include "EventWeightManager.hxx"
#include "EventVariationManager.hxx"
#include "CategoryManager.hxx"
#include "DocStringManager.hxx"
#include "Deprecated.hxx"

/// This class provides a base for user analyses. It handles many common
/// functions, and provides a structure so that users don't have to write too
/// much "boiler-plate" code.
///
/// User analyses should inherit from this class.

class AnalysisAlgorithm{

public:
  enum enumAnalysisPoint{
    kInitialize=0,
    kDefineProductions,
    kDefineInputConverters,
    kDefineSelections,
    kDefineCorrections,
    kDefineSystematics,
    kDefineConfigurations,
    kDefineMicroTrees,
    kDefineTruthTree,    
    kFillConfigTree,     
    kInitializeSpill,
    kInitializeBunch,
    kInitializeConfiguration,
    kInitializeToy,
    kInitializeSelection,
    kFinalizeSelection,
    kFinalizeToy,    
    kFillToyVarsInMicroTrees,
    kFinalizeConfiguration,
    kFillCategories,    
    kFillMicroTrees,
    kFinalizeBunch,    
    kFillTruthTree,
    kFinalizeSpill,
    kFinalize
  };


 public:
  /// Constructor, which instantiates the necessary converters for converting
  /// input files to the AnaSpill format.
  AnalysisAlgorithm(AnalysisAlgorithm* ana=NULL);

  virtual ~AnalysisAlgorithm(){}


  //---- Optional Initialize and finalize methods 
  virtual void Finalize(){}

  virtual bool InitializeSpill(){return true;}
  virtual void FinalizeSpill(){}

  virtual void InitializeBunch(){}
  virtual void FinalizeBunch(){}

  virtual void InitializeConfiguration(){}
  virtual bool FinalizeConfiguration(){return true;}

  virtual void InitializeToy(){}
  virtual void FinalizeToy(){}
  
  virtual void InitializeSelection(const SelectionBase&){}
  virtual void FinalizeSelection(const SelectionBase&){}

  virtual void FillCategories(){}

  virtual void FillConfigTree(){}

  /// Create the appropriate event time from an Spill and a Bunch in that spill
  virtual AnaEventC* MakeEvent() = 0;

  //---- These are mandatory functions

  virtual bool Initialize() = 0;

  virtual void DefineProductions() = 0;
  virtual void DefineInputConverters() = 0;
  virtual void DefineSelections() = 0;
  virtual void DefineCorrections() = 0;
  virtual void DefineSystematics() = 0;
  virtual void DefineConfigurations() = 0;
  virtual void DefineMicroTrees(bool addBase=true) = 0;
  virtual void DefineTruthTree() = 0;

  virtual void FillMicroTrees(bool addBase=true) = 0;
  virtual void FillToyVarsInMicroTrees(bool addBase=true) = 0;
  virtual void FillTruthTree() = 0;

  //---------------------------------------------

  /// Finalize toy
  void FinalizeToyBase();

  /// Check if this vertex should be saved in the RooTrackerVtx Tree. This condition is put on top of 
  /// the CheckFillTruthTree condition
  virtual bool CheckFillRooTracker(const AnaTrueObjectC&){return true;}

  /// Fill trees and process weight systematics only when any of the branches is successful
  void SetFillSuccessOnly(bool fill){_fillSuccessOnly= fill;}
  bool GetFillSuccessOnly(){return _fillSuccessOnly;}

  /// Initialize trees at the beginning of each configuration
  void SetInitializeTrees(bool ini){_initializeTrees= ini;}
  bool GetInitializeTrees(){return _initializeTrees;}

  /// Set the minimum accumulated cut level to save an event into the micro-tree. By default use the save level fot RooTracker
  void SetMinAccumCutLevelToSave(Int_t level);

  /// Get the minimum accumulated cut level to save an event into the micro-tree
  Int_t GetMinAccumCutLevelToSave(){return _min_accum_cut_level;}

  /// Check if the condition is fulfilled for at least one branch
  bool CheckAccumLevelToSave();

  /// Set the minimum accumulated cut level to save an event in RooTracker
  DEPRECATED(void SetMinAccumCutLevelToSaveRooTracker(Int_t level)){_min_accum_cut_level_roo = level;}

  /// Get the minimum accumulated cut level to save the RooTrackerVtx info for.
  Int_t GetMinAccumCutLevelToSaveRooTracker(){return _min_accum_cut_level_roo;}

  /// Check if the condition is fulfilled for at least one branch.
  bool CheckAccumLevelToSaveRooTracker();

  /// Set whether to save the RooTrackerVtx tree or not
  void SetSaveRooTracker(bool save){_saveRooTrackerVtxTree = save;}

  /// Get whether to save the RooTrackerVtx tree or not
  bool GetSaveRooTracker(){return _saveRooTrackerVtxTree;}

  /// Set the current event into the algorithm and all used algorithms
  void SetEvent(AnaEventC* event);

  /// Set the current box into the algorithm and all used algorithms. This is useful when one Analysis uses another
  void SetToyBox(const ToyBoxB* box, Int_t isel=0);

  /// Used a given analysis
  void UseAnalysis(AnalysisAlgorithm* ana);

  /// Select one of the selections
  void SetSelectedSelection(Int_t sel);

  /// Set version checking 
  void SetVersionCheck(bool check){_versionCheck=check;}

  /// Set the point of the analysis at which AnalysisLoop is
  void SetAnalysisPoint(enumAnalysisPoint point);

public:

  HighlandInputManager& input()  {return *_inputManager;}
  SelectionManager&     sel()    {return *_selManager;}
  CorrectionManager&    corr()   {return *_corrManager;}
  SystematicManager&    syst()   {return *_systManager;}
  EventWeightManager&   eweight(){return *_weightManager;}
  EventVariationManager&evar()   {return *_variationManager;}
  OutputManager&        output() {return *_outputManager;}
  ConfigurationManager& conf()   {return *_confManager;}
  CategoryManager&      cat()    {return *_categManager;}
  DocStringManager&     doc()    {return *_docManager;}
  
  virtual const ToyBoxB& boxB(Int_t isel=-1) const;

 protected:

  /// Input Manager: access to the current Event
  HighlandInputManager *_inputManager;

  /// Correction Manager
  CorrectionManager *_corrManager;

  /// Systematics Manager
  SystematicManager *_systManager;

  /// EventWeight Manager
  EventWeightManager *_weightManager;

  /// EventVariation Manager
  EventVariationManager *_variationManager;

  /// Output Manager
  OutputManager* _outputManager;

  /// Configuration Manager
  ConfigurationManager *_confManager;

  // Selection Manager
  SelectionManager *_selManager;

  // TrackCategory Manager
  CategoryManager *_categManager;

  // DocString Manager
  DocStringManager *_docManager;

  /// The analysis box
  const ToyBoxB* _box[NMAXSELECTIONS];

  /// the minimum accumulated cut level to save an event into the micro-tree
  Int_t _min_accum_cut_level;

  /// the minimum accumulated cut level to save an event into the RooTracker tree
  Int_t _min_accum_cut_level_roo;

  /// Fill trees and process weight systematics only when any of the branches is succesful
  bool _fillSuccessOnly;

  /// Initialize trees at the beginning of each configuration
  bool _initializeTrees;

  /// Save RooTracker tree in output file
  bool _saveRooTrackerVtxTree;

  /// The current event
  AnaEventC* _event;

  /// The Vector of used analysis
  std::vector<AnalysisAlgorithm*> _usedAnalyses;

  /// The selected selection
  Int_t _selectedSelection;
  
  /// Boolean parameter to know whether nny of the methods that sets things into used analysis has been called
  bool _setMethodCalled; 

  // Whether to perform or not version checking to make sure the sofware version and the input file match each other
  bool _versionCheck;

  enumAnalysisPoint _analysisPoint;
  
public:

  enum enumStandardMicroTrees_AnalysisAlgorithm {
    firstCategory = OutputManager::enumStandardMicroTreesLast_OutputManager+1,
    firstCategoryCounter = firstCategory+NMAXCATEG, 
    entry = firstCategoryCounter + NMAXCATEGCOUNTERS, 
    toy_ref,
    toy_index,
    toy_par_weight,
    toy_par_var,
    NWEIGHTSYST,
    weight_syst,
    weight_syst_total,
    weight_corr,
    weight_corr_total,
    redo,
    accum_level,
    first_cut,
    last_cut=first_cut+NMAXSTEPS,
    enumStandardMicroTreesLast_AnalysisAlgorithm
  };

  enum enumConfigTree_AnalysisAlgorithm{
    SoftwareVersion=0,
    HOSTNAME,
    CMTPATH,
    INPUTFILE,
    OriginalFile,
    MinAccumLevelToSave,
    enumConfigTreeLast_AnalysisAlgorithm
  };

};

#endif
