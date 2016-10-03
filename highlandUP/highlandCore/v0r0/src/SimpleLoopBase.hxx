#ifndef SimpleLoopBase_h
#define SimpleLoopBase_h

#include "OutputManager.hxx"
#include "HighlandInputManager.hxx"
#include "CorrectionManager.hxx"
#include "DocStringManager.hxx"
#include "VersionManager.hxx"
#include "MemoryUsage.hxx"

/// This class is a base class for simple event loops (Flat tree creation, etc).
/// It handles the opening and reading of input files, and converting each
/// spill to the AnaSpill format. The derived Process() function is called for
/// each spill.
class SimpleLoopBase: public OutputManager {
 public :
  
  SimpleLoopBase(int argc, char *argv[]);
  virtual ~SimpleLoopBase(){}

  /// The main event loop, which loops over all the spills, and calls Process()
  /// for each.
  void Loop(int nmax=0,int imin=0);

  /// Entry point to the class, where argc and argv are the command line
  /// parameters.
  void Execute();
  
 protected:

  /// Print the program's usage statement.
  void PrintUsage(const std::string& programName);

  /// Initialize the input manager etc, and calls the derived version of the
  /// function. Called before any spills are read.
  virtual bool Initialize() = 0;

  /// Initialize each spill, including reading it from the input file. Called
  /// before Process().
  virtual bool InitializeSpill() = 0;

  /// Tidy up, including closing the output file. Called after all spills have
  /// beem read.
  virtual void Finalize() = 0;

  /// Finalize each spill, including cleaning up data read from the input file.
  virtual void FinalizeSpill() = 0;

  /// The main function that should be overridden in the derived class. Called
  /// once for each spill, after it has been read in from the input file. The
  /// current spill can be accessed using ND::input().GetSpill().
  virtual bool Process() = 0;

  /// Define the tree that should be written to the output file.
  virtual void DefineOutputTree() = 0;

  // Return the Input Manager
  HighlandInputManager& input(){return _input;}

 protected:

  /// Fill the "config" tree, which includes details of the analysis cuts etc.
  void FillConfigTree();

  /// The current entry in the file.
  Long64_t _entry;

  /// Input file name, as specified by the user.
  std::string _inputFileName;

  /// Input file type. The file type is now found automatically, so this is
  /// deprecated.
  std::string _inputFileType;

  /// The output file name, as specified by the user.
  std::string _outputFileName;

  /// Whether to run in "cosmics" mode, where all the tracks are saved in a
  /// single bunch.
  bool _cosmicMode;

  /// Check version compatibility between nd280AnalysisTools compilation and oaAnalysis file
  bool _versionCheck;

  /// Memory logging.
  MemoryUsage _memory;

  /// Correction manager
  CorrectionManager _corrections;

  /// DocStrings manager
  DocStringManager _docStrings;

 private:

  /// The number of events to run over.
  int _nmax;

  /// The event to start from.
  int _imin;

  HighlandInputManager _input;

  enum enumConfigTree_SimpleLoopBase{
    SoftwareVersion=0,
    HOSTNAME,
    CMTPATH,
    INPUTFILE,
    OriginalFile,
    enumConfigTreeLast_SimpleLoopBase
  };

};

#endif
