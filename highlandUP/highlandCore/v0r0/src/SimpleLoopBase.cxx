#include "SimpleLoopBase.hxx"
#include "Parameters.hxx"
#include "Header.hxx"
#include <sys/time.h>

// save all trees every 5000 entries
const int NENTRIES_TREESAVE_SIMPLE=5000;

//********************************************************************
SimpleLoopBase::SimpleLoopBase(int argc, char *argv[]){
//********************************************************************
  std::string programName = argv[0];
  std::string paramFile = "";
  _nmax = 0;
  _imin = 0;
  _outputFileName = "";
  _inputFileName = "";
  _cosmicMode = false;
  _versionCheck = true;
  bool logMemory = false;

  for (;;) {
    int c = getopt(argc, argv, "cvmn:o:p:s:");
    if (c < 0)
      break;
    switch (c) {
      case 'c': {
        _cosmicMode = true;
        break;
      }
      case 'v': {
        _versionCheck = false;
        break;
      }
      case 'm': {
        logMemory = true;
        break;
      }
      case 'n': {
        std::istringstream tmp(optarg);
        tmp >> _nmax;
        break;
      }
      case 'o': {
        _outputFileName = optarg;
        break;
      }
      case 'p': {
        paramFile = optarg;
        break;
      }
      case 's': {
        std::istringstream tmp(optarg);
        tmp >> _imin;
        break;
      }
      default: {
        PrintUsage(programName);
      }
    }
  }

  // Check that an output file was specified
  if (_outputFileName == "") {
    std::cerr << "ERROR: No output file" << std::endl << std::endl;
    PrintUsage(programName);
  }

  // Check that there is an input file.
  if (argc <= optind) {
    std::cerr << "ERROR: No input file" << std::endl << std::endl;
    PrintUsage(programName);
  } else if (argc != optind + 1) {
    std::cerr << "ERROR: Only one input file may be specified. To process multiple ROOT files, " << std::endl
              << "       list them in a text file, and specify that text file as the input." << std::endl << std::endl;
    PrintUsage(programName);
  }

  _inputFileName = argv[optind++];

  // Set up the parameter override file if it was specified
  if (paramFile != "") {
    ND::params().ReadParamOverrideFile(paramFile);
  }

  // Make sure no parameters have been accessed yet
  ND::params().SetReadParamOverrideFilePointPassed();

  _memory.Enable(logMemory);

  /*
  // Add package versions to be saved in config tree
  ND::versioning().AddPackage("psycheCore",          anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHECOREROOT")));
  ND::versioning().AddPackage("psycheUtils",         anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEUTILSROOT")));
  ND::versioning().AddPackage("highlandTools",       anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDTOOLSROOT")));
  ND::versioning().AddPackage("highlandCorrections", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDCORRECTIONSROOT")));
  ND::versioning().AddPackage("highlandIO",          anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDIOROOT")));
  */
  SetDocStringManager(&_docStrings);
}

//********************************************************************
bool SimpleLoopBase::Initialize(){
//********************************************************************

  // Initialize the InputManager by specifying the input type and the input file
  if (!input().Initialize(_inputFileName,_inputFileType,_cosmicMode)) return false;

  // Read corrections from the input file

  // When running over a FlatTree corrections may already exist in it.
  // It that case corrections are read from the input file (config tree) and added to the CorrectionManager. 
  // In this case each correction is stored as 
  // "appliedInInput" and "disabled", such that it is not applied twice. 
  if (!_input.InputIsOriginalTree()){
    _corrections.ReadCorrections(_inputFileName, true);
  }

  // Initialize the tree manager
  if (!OutputManager::Initialize()) return false;

  // Open the output file
  if (!OpenOutputFile(_outputFileName)) return false;

  // Initialize the top level algorithm
  if (!Initialize()) return false;

  // Print on the screen the corrections
  _corrections.DumpCorrections();

  //  if (input().GetChain() == 0) return;

  // Define the output tree
  SimpleLoopBase::DefineOutputTree();

  // Create and fill the "config" tree with a single entry
  FillConfigTree();

  return true;
}

//********************************************************************
bool SimpleLoopBase::InitializeSpill(){
//********************************************************************

  // Initialize the tree manager
  OutputManager::InitializeEntry();

  // Fill the spill structure for the current spill
  // The previous succesfully read spill is deleted internally if the current spill is OK
  if (!input().LoadSpill(_entry)) return false;

  // Apply corrections
  _corrections.ApplyCorrections(input().GetCorrectedSpill());

  // Create a clone of the Corrected Spill
  input().MakeFinalSpill();

  // Record the POT for this spill
  input().IncrementPOTBySpill();

  // initialize the MyAnalysis spill
  if (!InitializeSpill()) return false;

  return true;
}

//********************************************************************
void SimpleLoopBase::FinalizeSpill(){
//********************************************************************

  // Finalize the derived class
  FinalizeSpill();

  // Save all trees every NENTRIES_TREESAVE_SIMPLE entries
  if (_entry%NENTRIES_TREESAVE_SIMPLE == 0){
    std::vector< TTree* >::iterator it;
    for (it=GetTrees().begin();it!=GetTrees().end();it++){
      (*it)->AutoSave("SaveSelf");
    }
  }

  // Log memory usage
  _memory.LogMemory();

  // Finalize the input manager (delete Spill and RawSpill)
  //  input().FinalizeSpill();  
}

//********************************************************************
void SimpleLoopBase::Finalize(){
//********************************************************************

  // Finalize the derived class
  Finalize();

  // Create and fill the header tree with a single entry
  AddTreeWithName(OutputManager::header,"header");  
  Header* headerp = &_input.header();
  GetTree(OutputManager::header)->Branch("POTInfo","Header",&headerp,32000,0);  
  GetTree(OutputManager::header)->Fill();

  // Save all trees after looping over all events
  std::vector< TTree* >::iterator it;
  for (it=GetTrees().begin();it!=GetTrees().end();it++){
    (*it)->AutoSave("SaveSelf");
  }

  // Finalize the base class
  OutputManager::Finalize();

  // Clean up
  input().RemoveConverters();

  // Write out the memory used.
  _memory.Write();

  // Close the output file
  CloseOutputFile();
}

//********************************************************************
void SimpleLoopBase::DefineOutputTree(){
//********************************************************************

  DefineOutputTree();
}
  
//********************************************************************
void SimpleLoopBase::Loop(int nmax,int imin){
//********************************************************************
  
  if (!SimpleLoopBase::Initialize()) return;
  
  // Get the number of entries in the tree
  Long64_t nentries = _input.GetEntries();  

  if (imin>nentries){
    std::cout << "SimpleLoopBase::Loop(). input tree has " << nentries << " entries. You cannot start from entry " << imin << std::endl;    
    return;
  }

  if (nmax==0 || imin+nmax>nentries) nmax=nentries-imin;

  if (_input.InputIsFlatTree())
    std::cout << "SimpleLoopBase::Loop(). input tree has " << nentries << " entries for " << _input.header().GetPOT() << " good POT" << std::endl;
  else
    std::cout << "SimpleLoopBase::Loop(). input tree has " << nentries << " entries (POT counted on the fly)" << std::endl;
  std::cout << "SimpleLoopBase::Loop(). loop over " << nmax << " entries from entry number "<< imin << std::endl;


  // Initialize clock
  timeval tim;
  gettimeofday(&tim, NULL);
  double t0=tim.tv_sec+(tim.tv_usec/1000000.0);


  //--------- Loop over entries in the tree ----------------------------------
  _entry=imin;

  while (_entry<imin+nmax) {
    if (!SimpleLoopBase::InitializeSpill()) continue;    
    Process();      
    SimpleLoopBase::FinalizeSpill();
  }
  SimpleLoopBase::Finalize();

  gettimeofday(&tim, NULL);
  double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
  
  std::cout << "--------- time profile --------------" << std::endl;
  std::cout << nmax << " entries processed in " << t1-t0 << " seconds" << std::endl;    

}

//********************************************************************
void SimpleLoopBase::FillConfigTree(){
//********************************************************************

  // Add a tree
  AddTreeWithName(OutputManager::config, "config");


  SetFillSingleTree(OutputManager::config);

  // The $CMTPATH
  AddVar(OutputManager::config, SimpleLoopBase::CMTPATH,   "CMTPATH",   "C", "the CMTPATH used to produce the output file");  
  FillVar(SimpleLoopBase::CMTPATH, (std::string)getenv("CMTPATH") );  

  // The Machine name
  AddVar(OutputManager::config, SimpleLoopBase::HOSTNAME,   "HOSTNAME",   "C", "the machine used to produce the output file");  
  if (getenv("HOSTNAME"))
    FillVar(SimpleLoopBase::HOSTNAME, (std::string)getenv("HOSTNAME") );  
  else 
    FillVar(SimpleLoopBase::HOSTNAME, "unknown" );  

  // The input file name
  AddVar(OutputManager::config, SimpleLoopBase::INPUTFILE,   "InputFile",   "C", "the input file used to produce the output file");  
  FillVar(SimpleLoopBase::INPUTFILE, _inputFileName );  

  // The original file (i.e. the recon output file)
  AddVar(OutputManager::config, SimpleLoopBase::OriginalFile,   "OriginalFile",   "C", "the original file used to produce the output file");  

  if (!_input.InputIsOriginalTree()){
    TChain* chain = new TChain("config");
    chain->AddFile(_inputFileName.c_str());    
    char OriginalFile[200]="unknown";
    if (chain->FindLeaf("OriginalFile")){        
      chain->SetBranchAddress("OriginalFile", OriginalFile);
      Long64_t centry = chain->LoadTree(0);
      Int_t nb = chain->GetEntry(0);
      (void) centry;
      (void) nb;
    }
    FillVar(SimpleLoopBase::OriginalFile, OriginalFile );  
  }
  else
    FillVar(SimpleLoopBase::OriginalFile, _inputFileName );  


  SetFillAllTrees();


  // The software version
  ND::versioning().WriteClonesArray(*GetTree(OutputManager::config));

  // docstrings
  _docStrings.WriteClonesArray(*GetTree(OutputManager::config));

  //  DocStringManager* doc= &(_docStrings);
  //  OutputManager::GetTree(OutputManager::config)->Branch("DocMan","DocStringManager",&doc,64000,0);


  // Corrections
  _corrections.WriteClonesArray(*GetTree(OutputManager::config));

  //  CorrectionManager* corr= &(_corrections);
  //  OutputManager::GetTree(OutputManager::config)->Branch("CorrMan","CorrectionManager",&corr,64000,0);


  // Fill the tree
  GetTree(OutputManager::config)->Fill();
  GetTree(OutputManager::config)->AutoSave("SaveSelf");
}

//********************************************************************
void SimpleLoopBase::PrintUsage(const std::string& programName){
//********************************************************************
  std::cout << "usage: " << programName << " [options] [input-file]" << std::endl;
  std::cout << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "    -c                Run in cosmics mode, putting all tracks in one 'bunch'" << std::endl;
  std::cout << "    -n <cnt>          Only read <cnt> events" << std::endl;
  std::cout << "    -o <file>         Set the name of an output file (mandatory)" << std::endl;
  std::cout << "    -s <cnt>          Skip <cnt> events" << std::endl;
  std::cout << "    -p <file>         Set the name of a parameter override file" << std::endl;
  std::cout << "    -v                Don't Check version compatibility between nd280AnalysisTools and oaAnalysis file" << std::endl;
  std::cout << "    -m                Log memory usage (written to histograms in output file)" << std::endl;
  std::exit(1);
}

//********************************************************************
void SimpleLoopBase::Execute(){
//********************************************************************
  Loop(_nmax,_imin);
}
