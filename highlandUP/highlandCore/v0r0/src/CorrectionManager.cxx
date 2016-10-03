#include "CorrectionManager.hxx"
#include <cassert>

//***********************************************************
CorrectionManager::CorrectionManager():HLClonesArray("config","CORR","CorrectionBase", NMAXCORRECTIONS) {
  //***********************************************************

  // By default corrections applied in the input file are not applied again
  _forceApplyCorrections = false;

  //Reset();
  _corrections.clear();

}

//***********************************************************
CorrectionManager::~CorrectionManager() {
  //***********************************************************
  for (std::vector<CorrectionBase*>::iterator it = _corrections.begin(); it != _corrections.end(); it++) {
    delete *it;
    *it = NULL;
  }

  _corrections.clear();

}

//***********************************************************
void CorrectionManager::ApplyCorrections(AnaSpillC& spill) {
  //***********************************************************

  for (std::vector<CorrectionBase*>::iterator it = _corrections.begin(); it != _corrections.end(); it++) {
    if (!(*it)) continue;
    if ((*it)->IsEnabled()){
      (*it)->Apply(spill);
    }
  }
}


//***********************************************************
void CorrectionManager::DisableAllCorrections() {
  //***********************************************************

  for (std::vector<CorrectionBase*>::iterator it = _corrections.begin(); it != _corrections.end(); it++) {
    if (!(*it)) continue;
    (*it)->Disable();
  }
}


//***********************************************************
void CorrectionManager::AddCorrection(Int_t index, const std::string& name, CorrectionBase* corr) {
  //***********************************************************

  // When running over a FlatTree corrections may already exist in it.
  // It that case the correction is already present in the manager 
  // (corrections are first read from the input file) and it has to be set as 
  // "appliedInInput" and "disabled", such that it is not applied twice. 
  // Some corrections may exit in the input file but as disabled. This case
  // is also taken into account: the correction is not added again but it is stored 
  // with the proper settings
  corr->SetName(name);
  corr->SetIndex(index);
  CorrectionBase* corr2 = NULL;
  //CorrectionBase* corr2 = GetCorrection(index); 
  
#pragma message("CorrectionManager::AddCorrection Name usage should be removed once approved!!!")
  
  // TMP: try name for bwd compatibility
  if (!corr2) 
    corr2 = GetCorrection(name);
  
  if (corr2 && !_forceApplyCorrections){
    if(corr2->IsAppliedInInput())
      corr2->Disable();
    else{ 
      if (corr->IsEnabled()) corr2->Enable();
      else                   corr2->Disable();
    }
  }
  else{
    _corrections.push_back(corr);
    CorrectionBase* corr2 = new((*_objects)[_NObjects++]) CorrectionBase(*corr);
    (void)corr2;
  }
}

//***********************************************************
CorrectionBase* CorrectionManager::GetCorrection(const std::string& name) {
  //***********************************************************

  for (std::vector<CorrectionBase*>::iterator it = _corrections.begin(); it != _corrections.end(); it++) {
    if (!(*it)) continue;
    if ((*it)->Name() == name) return *it;
  }
  return NULL;
}


//***********************************************************
bool CorrectionManager::IsEnabled(const std::string& name) {
  //***********************************************************

  CorrectionBase* corr = GetCorrection(name);
  if (corr) return corr->IsEnabled();
  else return false;

}

//***********************************************************
void CorrectionManager::EnableCorrection(const std::string& name) {
  //***********************************************************

  CorrectionBase* corr = GetCorrection(name);
  if (corr) corr->Enable();
}

//***********************************************************
void CorrectionManager::DisableCorrection(const std::string& name) {
  //***********************************************************

  CorrectionBase* corr = GetCorrection(name);
  if (corr) corr->Disable();
}


//***********************************************************
CorrectionBase* CorrectionManager::GetCorrection(Int_t index) {
  //***********************************************************

  if (index < 0 || (UInt_t)index > NMAXCORRECTIONS - 1) return NULL;
  return _corrections[index];

}

//***********************************************************
bool CorrectionManager::IsEnabled(Int_t index) {
  //***********************************************************

  CorrectionBase* corr = GetCorrection(index);
  if (corr) return corr->IsEnabled();
  else return false;
}

//***********************************************************
void CorrectionManager::EnableCorrection(Int_t index) {
  //***********************************************************

  CorrectionBase* corr = GetCorrection(index);
  if (corr) corr->Enable();
}

//***********************************************************
void CorrectionManager::DisableCorrection(Int_t index) {
  //***********************************************************

  CorrectionBase* corr = GetCorrection(index);
  if (corr) corr->Disable();
}


//********************************************************************
void CorrectionManager::ReadCorrections(const std::string& file, bool input){
  //********************************************************************

//  Reset();
  _corrections.clear();
  ReadClonesArray(file);

  // Add them to the map of categories
  for (int i = 0; i < _NObjects; i++){
    CorrectionBase* corr = (CorrectionBase*)(*_objects)[i];
    assert(_corrections.size() < NMAXCORRECTIONS);
    _corrections.push_back(corr);

    // When this method is called From the AnalysisLoop corrections are read from the input file
    // In that case the correction has to be disabled and mark as applied in input
    if (input) {
      if (corr->IsEnabled()){
        corr->Disable();
        corr->SetAppliedInInput(true);
      }
    }      
  }
}

//********************************************************************
void CorrectionManager::Reset(){
  //********************************************************************

  // Reset the vectors
  _corrections.clear();

  _corrections.resize(NMAXCORRECTIONS);
  _corrections.assign(_corrections.size(),NULL);  

}

//********************************************************************
void CorrectionManager::DumpCorrections(){
  //********************************************************************

  std::cout << " -------- List of Corrections  -----------------------------------" << std::endl;
  char out[256];  
  sprintf(out,"%3s: %-3s %-25s %-15s %-20s", "#", "index", "name", "enabled", "applied in input");
  std::cout << out << "\n" << std::endl;

  for (int i = 0; i < _NObjects; i++){
    CorrectionBase* corr = _corrections[i];   
    if (!corr) continue;
    sprintf(out,"%3d: %-3d %-25s %-15d %-20d", i, corr->GetIndex(), corr->Name().c_str(), (Int_t)corr->IsEnabled(), (Int_t)corr->IsAppliedInInput());
    std::cout << out << std::endl;
  }
  std::cout << " -----------------------------------------------------------------" << std::endl;
}

