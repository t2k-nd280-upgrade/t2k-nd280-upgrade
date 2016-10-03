#ifndef AnalysisLoop_h
#define AnalysisLoop_h

#include "MemoryUsage.hxx"
#include "CoreDataClasses.hxx"
#include "AnalysisAlgorithm.hxx"

/// This class is a base class for analyses.
///
/// It specifies the interface that analyses should conform to, although user
/// analyses should inherit from baseAnalysis, not AnalysisLoop directly.
/// baseAnalysis implements all the required functions, so the user doesn't
/// have to implement all the functions themselves.
///
/// The class also  contains the low-level functionality for running the
/// analysis, including the all-important Loop function.
class AnalysisLoop{
 public :
  
  /// Constructor which parses and sanity checks the command line options.
  AnalysisLoop(AnalysisAlgorithm* ana, int argc, char *argv[]);

  virtual ~AnalysisLoop(){}

  /// This is the main looping function that controls the analysis. It
  /// initializes the analysis, then loops over
  /// - Spills in the input file
  ///  - Bunches in each spill
  ///   - Configurations for each bunch (which sets of systematics to apply)
  ///    - toy experiments for each configuration (the MC throws for each systematic)
  ///     - Call the user's Process() function.
  void Loop(int nmax=0, int imin=0);

  /// Simply calls Loop with the appropriate parameters as read from the
  /// command line.
  void Execute();

  void SetEvent(AnaEventC* event){_event= event;}
  
 protected:

  /// Initialize the entire analysis. Calls the Initialize() function in the
  /// user's derived class as part of the initialization.
  bool Initialize();

  /// Called after all processing is complete. Cleans up, then writes to and
  /// closes the output file.
  void Finalize();

  /// Initialize each spill, including reading it from the input file, and
  /// applying any corrections specified by the user. Calls the user's
  /// derived version after applying the corrections.
  bool InitializeSpill();

  /// Called after each spill is finished. Calls the user's derived version
  /// before deleting the current active Spill in the InputManager.
  void FinalizeSpill();

  /// Initialize each bunch of each spill, simply calling the user's derived
  /// version.
  void InitializeBunch();

  /// Called after each bunch is finished, simply calling the user's derived
  /// version.
  void FinalizeBunch();

  /// Initialize each configuration for each bunch. The current active
  /// configuration should already have been changed by Loop().
  void InitializeConfiguration();

  /// Fill the tree for this bunch, including the "truth" tree if it is enabled.
  /// The derived version should fill any variabled in the micro-tree that
  /// don't depend on the exact analysis.
  bool FinalizeConfiguration();

  /// Initialize each toy experiment for each configuration, including
  /// applying any systematic variations. The user's derived version is called
  /// after variations have been applied.
  void InitializeToy();

  /// Called after each toy experiment is complete. 
  bool FinalizeToy();

  void InitializeSelection(const SelectionBase& sel);

  void FinalizeSelection(const SelectionBase& sel);


  /// The main function called to run the analysis. The derived version should
  /// apply any cuts, and return whether the current active bunch passed the
  /// cuts or not.
  bool Process(SelectionBase& selec);

  /// Define the corrections to be applied
  void DefineCorrections();

  /// Define the configurations (micro-trees) and which systematics should be applied in each "configuration".
  void DefineConfigurations();

  /// Define the variables that should be stored in the output micro-trees.
  void DefineMicroTrees();

  /// Define the "truth" tree. See FillTruthTree().
  void DefineTruthTree();

  /// Fill the track categories for color drawing. There is a definition in
  /// baseAnalysis, and the user can specify their own if they have defined
  /// their own custom categories.
  void FillCategories() ;

  /// Fill the truth tree. The truth tree is designed to contain information
  /// about all true signal events, including those that the analysis didn't
  /// select as signal.
  ///
  /// The derived version should fill the relevant variables, and return whether
  /// the tree should be written or not. The derived function is only called
  /// once per spill!
  //  void FillTruthTree(const AnaTrueVertex& vtx);

  /// Fill the standard configurations trees: 
  void FillMicroTrees();

  /// Fill the standard configurations only toy-dependent variables
  void FillToyVarsInMicroTrees();

  ///------------------------------------------------------------------------------

  /// Simple usage statement, for if the user gave bad command line options.
  void PrintUsage(const std::string& programName);

  // Return the current AnalysisAlgorithm
  AnalysisAlgorithm& ana(){return *_ana;}

 protected:

  /// Fill the "header" tree, which includes POT information etc.
  void FillHeaderTree();

  /// Fill the "config" tree, which includes details of the analysis cuts etc.
  void FillConfigTree();

  /// Fill the "truth" tree
  //  void FillTruthTree();

  /// The number of entries we have read in.
  Long64_t _entry;

  /// Path to the input file, as defined by the user.
  std::string _inputFileName;

  /// This was used when the user could specify what type of input file they
  /// had provided. Now this is done automatically.
  std::string _inputFileType;

  /// What to call the output file, as defined by the user.
  std::string _outputFileName;

  /// Whether the "truth" tree should be filled. See FillTruthTree().
  bool _enableTruthTree;

  /// Whether to run in "cosmics" mode, where all the tracks are saved in a
  /// single bunch.
  bool _cosmicMode;

  /// Check version compatibility between nd280AnalysisTools compilation and oaAnalysis file
  bool _versionCheck; 

  /// Whether to fill the trees and create an output file or not
  bool _fillTrees; 

  /// Tools for tracking memory usage
  MemoryUsage _memory;

  AnaEventC* _event;

 private:

  /// The number of events to run over.
  int _nmax;

  /// The event to start from.
  int _imin;
  
  /// The Analysis algorithm to run
  AnalysisAlgorithm* _ana;

  /// The vector of weights for the weight systematics and corrections
  Weight_h* _weightSyst;
  Float_t* _weightCorr;
  Int_t _nWeightSyst;


  /// The current toy experiment weight
  Double_t _toy_weight;

  /// Is the current toy succesful for any of the branches ?
  bool _toy_passed;

  /// Is the current configuration succesful for any of the toys ?
  bool _conf_passed;

public:

};

#endif
