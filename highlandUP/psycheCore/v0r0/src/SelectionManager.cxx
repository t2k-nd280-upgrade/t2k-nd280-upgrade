#include "SelectionManager.hxx"


//********************************************************************
SelectionManager::SelectionManager():HLClonesArray("config","SEL","SelectionBase",NMAXSELECTIONS){
//********************************************************************

  _nMaxCuts=0;
  _nMaxBranches=0;
  _nEnabledSelections=0;
}

//********************************************************************
SelectionBase* SelectionManager::GetSelection(const std::string& name, bool print_error) {
//********************************************************************

  for (std::vector<SelectionBase*>::iterator it=_eventSelections.begin();it!=_eventSelections.end();it++){
    if ((*it)->Name() == name) return *it;
  }
  
  if (print_error)
    std::cout << "SelectionManager::GetSelection(). Selection '" << name << "' does not exist !!!!" << std::endl; 

  return NULL;
}

//********************************************************************
SelectionBase* SelectionManager::GetSelection(Int_t index, bool print_error) {
//********************************************************************

  if (index<0 || index>=(Int_t)_eventSelections.size()){
    if (print_error)
      std::cout << "SelectionManager::GetSelection(). Selection with index " << index << " does not exist !!!!" << std::endl; 
    return NULL;
  }

  return _eventSelections[index];
}

//********************************************************************
void SelectionManager::AddSelection(const std::string& name, const std::string& title, SelectionBase* selection, Int_t presel){
//********************************************************************

  // nothing to do if selection already exists
  if (GetSelection(name,false)) return;

  // Set name and title
  selection->SetName(name);
  selection->SetTitle(title);

  // Define the steps and the DetectorFV for the selection
  selection->Initialize();
  
  // Get the index as enabled selection
  selection->SetEnabledIndex(_nEnabledSelections);

  // Set preselection accum level, but only when it is specified, use otherwise the default for that selection
  if (presel>=0)
    selection->SetPreSelectionAccumLevel(presel);

  // add the selection
  (*_objects)[_NObjects++] = selection;
  _eventSelections.push_back(selection);

  // computes the maximum number of cuts and branches for all added selections
  if (selection->GetNBranches() >_nMaxBranches) _nMaxBranches = selection->GetNBranches();
  if (selection->GetNMaxCuts()  >_nMaxCuts)     _nMaxCuts     = selection->GetNMaxCuts();

  // computes the number of enabled selections
  _nEnabledSelections++;
}

//********************************************************************
void SelectionManager::DisableSelection(const std::string& name){
//********************************************************************

  SelectionBase* sel = GetSelection(name); 
  if (!sel) return;

  // nothing to do when the selection is already disabled
  if (!sel->IsEnabled()) return;

  // recompute the number of enabled selections  
  _nEnabledSelections--;

  // Disable the selection
  sel->Disable();

  Int_t isel=0;
  for (std::vector<SelectionBase*>::iterator it=_eventSelections.begin();it!=_eventSelections.end();it++){
    if ((*it)->IsEnabled()){
      (*it)->SetEnabledIndex(isel++);
    }
  }

}

//********************************************************************
void SelectionManager::EnableSelection(const std::string& name){
//********************************************************************
 
  SelectionBase* sel = GetSelection(name); 
  if (!sel) return;

  // nothing to do when the selection is already enabled
  if (sel->IsEnabled()) return;

  // enable the selection
  sel->Enable();

  // recompute the number of enabled selections
  _nEnabledSelections++;

  // recompute and set the new slection indices
  Int_t isel=0;
  for (std::vector<SelectionBase*>::iterator it=_eventSelections.begin();it!=_eventSelections.end();it++){
    if ((*it)->IsEnabled()){
      (*it)->SetEnabledIndex(isel++);
    }
  }

}

//********************************************************************
bool SelectionManager::ApplySelection(const std::string& name, AnaEventC& event, bool& redo) {
//********************************************************************

  SelectionBase* sel = GetSelection(name); 
  if (!sel) return true;

  // Check if it is enabled
  if (!sel->IsEnabled()) return true;

  // Apply the selection
  return sel->Apply(event, redo);
}

//********************************************************************
void SelectionManager::ReadSelections(const std::string& file){
//********************************************************************

  // Reset the vectors
  _eventSelections.clear();

  _nMaxCuts=0;
  _nMaxBranches=0;
  _nEnabledSelections=0;

  // Read the selections from the clones array in the input file
  ReadClonesArray(file);

  for (int i=0;i<_NObjects;i++){
    SelectionBase* sel = (SelectionBase*)(*_objects)[i];    
    _eventSelections.push_back(sel);
    if (sel->IsEnabled()){

      // computes the maximum number of cuts and branches for all added selections
      if (sel->GetNBranches() >_nMaxBranches) _nMaxBranches = sel->GetNBranches();
      if (sel->GetNMaxCuts()  >_nMaxCuts)     _nMaxCuts     = sel->GetNMaxCuts();

      // recompute the number of enabled selections
      _nEnabledSelections++;
    }
  }

  //  int nselections[NMAXBRANCHES]={0};

}

//********************************************************************
void SelectionManager::DumpSelections() {
//********************************************************************

  std::cout << " -------- List of Selections  -----------------------------------------------------------------------------------------------------" << std::endl;
  char out[256];  
  sprintf(out,"%3s: %-25s %-40s %-10s %-10s %-13s %-20s %-22s", "#", "name", "title", "enabled", "#branches", "force break", "presel accum_level", "index in accum_level");
  std::cout << out <<"\n" << std::endl;

  UInt_t index=0;
  for (int i=0;i<_NObjects;i++){
    SelectionBase* sel = (SelectionBase*)(*_objects)[i];    
    if (sel->IsEnabled()){
      sprintf(out,"%3d: %-25s %-40s %-10d %-10d %-13d %-20d %-22d", i, sel->Name().c_str(),  sel->Title().c_str(), 
              (int)sel->IsEnabled(), sel->GetNBranches(), (int)sel->GetForceBreak(), (int)sel->GetPreSelectionAccumLevel(), index);
      index++;
    }
    else{
      sprintf(out,"%3d: %-25s %-40s %-10d %-10d %-13d %-20d %-22s", i, sel->Name().c_str(),  sel->Title().c_str(), 
              (int)sel->IsEnabled(), sel->GetNBranches(), (int)sel->GetForceBreak(), (int)sel->GetPreSelectionAccumLevel(), "-");

    }
    std::cout << out << std::endl;
  }

  std::cout << " ----------------------------------------------------------------------------------------------------------------------------------" << std::endl;
}


//********************************************************************
void SelectionManager::PrintStatistics() {
//********************************************************************

  /// Loop over selections
  for (std::vector<SelectionBase*>::iterator it = _eventSelections.begin(); it!=_eventSelections.end(); it++){    
    if ((*it)->IsEnabled())
      (*it)->PrintStatistics();
  }
}

// //********************************************************************
// void SelectionManager::CopySteps(const std::string& ssel1, const std::string& sbranch1, UInt_t first, UInt_t last,const std::string& ssel2, const std::string& sbranch2){
// //********************************************************************

//   SelectionBase* sel1 = GetSelection(ssel1);
//   SelectionBase* sel2 = GetSelection(ssel2);

//   if (!sel1 || !sel2) return;

//   sel2->CopySteps(*sel1,sbranch1,first,last,sbranch2);

//   // computes the maximum number of cuts and branches for all added selections
//   if (sel2->GetNBranches() >_nMaxBranches) _nMaxBranches = sel2->GetNBranches();
//   if (sel2->GetNMaxCuts()  >_nMaxCuts)     _nMaxCuts     = sel2->GetNMaxCuts();
// }

// //********************************************************************
// void SelectionManager::CopySteps(const std::string& ssel1, const std::string& sbranch1, const std::string& ssel2, const std::string& sbranch2){
// //********************************************************************

//   // Copy all steps
//   SelectionBase* sel1 = GetSelection(ssel1);
//   if (!sel1) return;

//   std::vector<StepBase*> steps1 = sel1->GetStepsInBranch(sbranch1);
//   CopySteps(ssel1,sbranch1,0,steps1.size()-1,ssel2,sbranch2);
// }

// //********************************************************************
// void SelectionManager::CopyStep(const std::string& ssel1, const std::string& sbranch1, UInt_t istep, const std::string& ssel2, const std::string& sbranch2){
// //********************************************************************

//   // Copy only one step
//   CopySteps(ssel1,sbranch1,istep,istep,ssel2,sbranch2);
// }

//********************************************************************
void SelectionManager::SetForceFillEventSummary(bool force){
//********************************************************************

  for (std::vector<SelectionBase*>::iterator it=_eventSelections.begin();it!=_eventSelections.end();it++){
    (*it)->SetForceFillEventSummary(force);
  }  
}

//********************************************************************
void SelectionManager::SetValidRunPeriods(const std::string& ssel1, const std::string validRunPeriods){
//********************************************************************
    SelectionBase* sel1 = GetSelection(ssel1);
    sel1->SetValidRunPeriods(validRunPeriods);
}

//***********************************************************
bool SelectionManager::PreSelectionPassed(const AnaEventC& event) {
//***********************************************************
  
  for (std::vector<SelectionBase*>::iterator it=_eventSelections.begin();it!=_eventSelections.end();it++){
    if ((*it)->IsEnabled()){
      if ((*it)->PreSelectionPassed(event)) return true;
    }
  }
  return false;      
}

//********************************************************************
void SelectionManager::CreateToyBoxArray(Int_t nevents) {
//********************************************************************

  // Loop over selection
  for (std::vector<SelectionBase*>::iterator it = _eventSelections.begin(); it!=_eventSelections.end(); it++){    
    if ((*it)->IsEnabled()){
      (*it)->CreateToyBoxArray(nevents);
    }
  }

}

//********************************************************************
void SelectionManager::FinalizeEvent(const AnaEventC& event) {
//********************************************************************

  // Loop over selection
  for (std::vector<SelectionBase*>::iterator it = _eventSelections.begin(); it!=_eventSelections.end(); it++){    
    if ((*it)->IsEnabled()){
      (*it)->FinalizeEvent(event);
    }
  }
}

//********************************************************************
void SelectionManager::InitializeEvent(AnaEventC& event) {
//********************************************************************

  // Loop over selection
  for (std::vector<SelectionBase*>::iterator it = _eventSelections.begin(); it!=_eventSelections.end(); it++){    
    if ((*it)->IsEnabled()){
      // Create and Fill the EventBox
      (*it)->InitializeEvent(event);
    }
  }
}
