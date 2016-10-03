#include "InputManager.hxx"
#include "CoreUtils.hxx"

//********************************************************************
InputManager::InputManager(){
//********************************************************************

  _CorrectedSpill = NULL;
  _Spill          = NULL;
  _prevSEvent     = NULL;
  _selected       = NULL;
  _IsMC           = true;
  _uniqueID       = 0;
}

//********************************************************************
void InputManager::Reset(){
//********************************************************************

  _CorrectedSpill = NULL;
  _Spill          = NULL;
  _prevSEvent     = NULL;
  _IsMC           = true;
  _uniqueID       = 0;

  for (std::vector<InputConverter*>::const_iterator it = _converters.begin(); it != _converters.end(); it++) {
    (*it)->Reset();
  }        

}

//*****************************************************************************
void InputManager::DeleteSpill(){
//*****************************************************************************

  if (_CorrectedSpill)
    delete _CorrectedSpill;
 
  if (_Spill)
    delete _Spill;

  _CorrectedSpill=NULL;
  _Spill=NULL;

}

//********************************************************************
bool InputManager::AddDataFileToConverter(const std::string& infile_name, const std::string& conv, const bool isCosmic, bool reset){
//********************************************************************

  // Check wehther this is a root file
  bool isROOTFile = IsROOTFile(infile_name);

  // Find out the input format (FlatTre, oaAnalysisTree, etc)
  std::string conv2 = conv;
  if (conv2 == "") {
    std::cout << "InputManager::Initialize(). Input type (oaAnalysisTree, FlatTree, ...) not specified. Find it automatically..." << std::endl;    
    std::string firstFile = FindFirstFile(infile_name, isROOTFile);
    for (std::vector<InputConverter*>::const_iterator it = _converters.begin(); it != _converters.end(); it++) {
      if ((*it)->IsCorrectType(firstFile)) {
        conv2 = (*it)->Name();
        break;
      }
    }
  }

  if (conv2 == "") {
    std::cerr << "Unable to find an input converter for your input files - check that your input files are valid!" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (!HasConverter(conv2)) return false;

  std::cout << "InputManager::Initialize(). Input type is: " << conv2 << std::endl;

  // Select the specified converter
  SelectConverter(conv2);

  // Reset the trees in the TChain and the number of entries
  if (reset) GetConverter().Reset();

  // Initialize the converter. Define tree branches, etc
  if (!GetConverter().Initialize()) return false;
  GetConverter().SetCosmicMode(isCosmic);

  // Read the input file
  if (!ReadFile(infile_name, isROOTFile)) return false;

  // Get the number of entries in the tree
  Long64_t nentries = GetEntries();  
  std::cout << "InputManager::input tree has " << nentries << " entries" << std::endl;

  return true;
}

//********************************************************************
bool InputManager::Initialize(const std::string& infile_name, const std::string& conv, const bool isCosmic){
//********************************************************************

  if (!AddDataFileToConverter(infile_name, conv, isCosmic, true)) return false;

  // Check whether is data or MC
  if(GetIsMC())
    std::cout << "InputManager:: MC mode" << std::endl;
  else
    std::cout << "InputManager:: Data mode" << std::endl;
  
  // SoftwareVersion
  std::cout << "InputManager:: SoftwareVersion for original file is " << GetSoftwareVersion() << std::endl;

  return true;
}

//*****************************************************************************
bool InputManager::IsROOTFile(const std::string& infile_name){
//*****************************************************************************

  std::cout << "InputManager::IsROOTFile(). If this is not a ROOT file an error will appear in the next two lines. Ignore it !!!! " << std::endl;

  TString infileName(infile_name);  
  //ifstream inputFile(infileName.Data(), std::ios::in);
    
  //if(!inputFile){
  //std::cerr << "Cannot open input file '" << infileName.Data() << "'. Exit!" << std::endl;
  //return false;
  //}
  
  TFile* isROOTFile = TFile::Open(infile_name.c_str());
  bool isroo=true;
  if (!isROOTFile){
    isroo=false;
    std::cout << "InputManager::IsROOTFile(). This is not a ROOT file. Try as a list of ROOT files. " << std::endl;
    return isroo;
  }
  isROOTFile->Close();
  std::cout << "InputManager::IsROOTFile(). This is a ROOT file. " << std::endl;
  return isroo;//isROOTFile;
}

//*****************************************************************************
bool InputManager::ReadFile(const std::string& infile_name, bool isROOTFile){
//*****************************************************************************

  // Load data files     
  TString infileName(infile_name);  
  ifstream inputFile(infileName.Data(), std::ios::in);
    
  //if(!inputFile){
  //std::cerr << "Cannot open input file '" << infileName.Data() << "'. Exit!" << std::endl;
  //return false;
  //}
  
  //Loop over the input files
  std::string inputString=infile_name;   
  
  if (!isROOTFile){ 
    while (inputFile >> inputString) {
      if (!GetConverter().AddFileToTChain(inputString)) return false;      
    }    
  }
  else{
    if (!GetConverter().AddFileToTChain(inputString)) return false;
  }

  return true;
}

//*****************************************************************************
std::string InputManager::FindFirstFile(const std::string& infile_name, bool isROOTFile){
//*****************************************************************************

  TString infileName(infile_name);  
  ifstream inputFile(infileName.Data(), std::ios::in);
    
  //if(!inputFile){
  //std::cerr << "Cannot open input file '" << infileName.Data() << "'. Exit!" << std::endl;
  //std::string fal = "false";
  //return fal;
  //}
  
  std::string inputString=infile_name;   
  
  if (!isROOTFile)
    inputFile >> inputString;

  return inputString;
}

//*****************************************************************************
bool InputManager::LoadSpill(Long64_t& entry){
//*****************************************************************************

  Long64_t ientry = LoadTree(entry);
  if (ientry < 0) return false;
  
  // The spill to be read
  AnaSpillC* SpillToRead = NULL;

  // Fill the spill structure from the input tree. The SpillToRead instance is created internally by the converter
  Int_t nb = GetConverter().GetSpill(entry,SpillToRead);
  if (nb <=0 || !SpillToRead){
    return false;
  }

  // If the spill is succesfully read the previous successfully read spill must be deleted
  DeleteSpill();

  // We can now associate the spill just read with the _CorrectedSpill
  _CorrectedSpill = SpillToRead;

  return true;
}

//*****************************************************************************
bool InputManager::LoadEvent(Long64_t& entry, bool delPrevious){
//*****************************************************************************

  Long64_t ientry = LoadTree(entry);
  if (ientry < 0) return false;

  // Empty pointer filled inside GetEvent method
  AnaEventC* RawEvent = NULL;

  // Fill the event structure from the input tree
  Int_t nb = GetConverter().GetEvent(entry,RawEvent);
  if (nb <=0 ) return false;

  // Set the event unique ID
  RawEvent->UniqueID    = _uniqueID++;

  // Create a AnaSuperEvent
  _sevent = new AnaSuperEventB(RawEvent);

  // Delete the previous event
  if (_prevSEvent && delPrevious){
    delete _prevSEvent;
  }

  _prevSEvent = _sevent;

  return true;
}

//*****************************************************************************
Long64_t InputManager::LoadTree(Long64_t entry){
//*****************************************************************************

  return GetConverter().LoadTree(entry);
}

//*****************************************************************************
Long64_t InputManager::GetEntries(){
//*****************************************************************************

  // return the number of entries
  return GetConverter().GetEntries();
}

//********************************************************************
void InputManager::SelectConverter(const std::string& name) {
//********************************************************************

  for (std::vector<InputConverter*>::iterator it = _converters.begin(); it != _converters.end(); it++) {
    if ((*it)->Name() == name){
      _selected = *it;
      break;
    }
  }
}

//********************************************************************
bool InputManager::HasConverter(const std::string& conv, bool err_meaasage) {
//********************************************************************

  for (std::vector<InputConverter*>::iterator it = _converters.begin(); it != _converters.end(); it++) {
    if ((*it)->Name() == conv){
      return true;
    }
  }
  if (err_meaasage)
    std::cout << "Input Converter '" << conv << "' does not exist" << std::endl;

  return false;
}

//********************************************************************
void InputManager::AddConverter(const std::string& name, InputConverter* conv) {
//********************************************************************

  if (HasConverter(name, false)){
    std::cout << "InputConverter " << name << " already exists. If you want to replace it please use ReplaceConverter method" << std::endl;
    return;
  }
  
  // Add a new converter
  conv->SetName(name);
  _converters.push_back(conv);
}

//********************************************************************
void InputManager::ReplaceConverter(const std::string& name, InputConverter* conv) {
//********************************************************************

  for (std::vector<InputConverter*>::iterator it = _converters.begin(); it != _converters.end(); it++) {
    if ((*it)->Name() == name){
      std::cout << "Overriding " << name << " input converter" << std::endl;
      
      // Replace an existing converter with the same name                                                                                                                                                                                   
      delete *it;
      conv->SetName(name);
      *it = conv;
      return;
    }
  }
  
  std::cout << "InputConverter " << name << " does not exist. Cannot replace it !!!!!" << std::endl;
  
}

//********************************************************************
void InputManager::RemoveConverters() {
//********************************************************************
  for (std::vector<InputConverter*>::iterator it = _converters.begin(); it != _converters.end(); it++) {
    if (*it) delete *it;
  }
  _converters.clear();
}

//********************************************************************
void InputManager::IncrementPOTBySpill() {
//********************************************************************

  GetConverter().IncrementPOTBySpill();
}

//*****************************************************************************
void InputManager::MakeFinalSpill(){
//*****************************************************************************

  _Spill = _CorrectedSpill->Clone();
}

//*****************************************************************************
void InputManager::ResetSpillToCorrected() {
//*****************************************************************************
  if (_Spill) {
    delete _Spill;
    _Spill = _CorrectedSpill->Clone();
  }
}

//*****************************************************************************
//AnaEventC*  InputManager::MakeEventFromFinalSpill() {
//*****************************************************************************

//  return new AnaEventC(GetSpill(),GetBunch());
//}

//*****************************************************************************
//AnaEventC*  InputManager::MakeEvent() {
//*****************************************************************************

//  return new AnaEventC(GetCorrectedSpill(),GetCorrectedBunch());
//}

//*****************************************************************************
//AnaSuperEventB*  InputManager::MakeSuperEvent() {
//*****************************************************************************
/*
  AnaSuperEventB* sevent = new AnaSuperEventB();

  sevent->RawEvent = MakeEvent();
  sevent->Event = sevent->RawEvent->Clone();

  return sevent;
}


*/
