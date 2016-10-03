#include "EventVariationManager.hxx"
#include "MultiThread.hxx"


//***********************************************************
EventVariationManager::EventVariationManager(){
//***********************************************************

    _nEventVariationsEnabled = 0;

    _eventVariationsWithNull.resize(NMAXEVENTVARIATIONS);
    _eventVariationsWithNull.assign(_eventVariationsWithNull.size(),NULL);  
    
    _eventVariations.clear();
}

//***********************************************************
EventVariationManager::~EventVariationManager() {
//***********************************************************

    for (UInt_t i = 0; i < _eventVariations.size(); ++i) {
        delete _eventVariations[i];
    }
}

//***********************************************************
void EventVariationManager::Initialize(Int_t nevents) {
//***********************************************************

  // For variation systematics the SystBox is the same for all selections and branches
  // Create the SystBox for a single selection and branch, for a given number of events
  for (UInt_t j = 0; j < _eventVariations.size(); ++j){
    if (_eventVariations[j]->IsEnabled())
      _eventVariations[j]->Initialize(1,0,1, nevents);
  }
}

//***********************************************************
void EventVariationManager::FinalizeEvent(AnaEventC& event) {
//***********************************************************

  for (UInt_t j = 0; j < _eventVariations.size(); ++j){
    if (_eventVariations[j]->IsEnabled())
      _eventVariations[j]->FinalizeEvent(event);
  }
    //TODO
}

//***********************************************************
void EventVariationManager::InitializeEvent(SelectionManager& sel, AnaEventC& event) {
//***********************************************************

  // Fill the SystBox for the enabled systematics


  for (UInt_t j = 0; j < _eventVariations.size(); ++j){
    if (!_eventVariations[j]->IsEnabled()) continue;      
    for (std::vector<SelectionBase*>::iterator sit= sel.GetSelections().begin();sit!=sel.GetSelections().end();sit++){
      SelectionBase* selec = *sit;
      if (!selec->IsEnabled()) continue;                                     
      _eventVariations[j]->InitializeEvent(event,*selec);
    }
    _eventVariations[j]->InitializeEvent(event);
  }
}

//********************************************************************
std::vector<EventVariationBase*> EventVariationManager::GetEventVariations(const std::vector<Int_t>& indices) const{
//********************************************************************
  
  std::vector<EventVariationBase*> systematics;
  for (UInt_t i=0;i<indices.size();i++)
    systematics.push_back(GetEventVariation(indices[i]));
  
  return systematics;
}

//********************************************************************
EventVariationBase* EventVariationManager::GetEventVariation(Int_t index) const{
//********************************************************************

  if (index<0 || (UInt_t)index> NMAXEVENTVARIATIONS-1) return NULL;
  return _eventVariationsWithNull[index];
}

//********************************************************************
EventVariationBase* EventVariationManager::GetEventVariation(const std::string& name) const{
//********************************************************************

    for (UInt_t i = 0; i < _eventVariations.size(); ++i) {
        if (_eventVariations[i]->Name() == name) return _eventVariations[i];
    }
    return NULL;  
}

//********************************************************************
Int_t EventVariationManager::GetEventVariationIndex(const std::string& name){
//********************************************************************

    for (UInt_t i = 0; i < _eventVariations.size(); i++) {
        if (_eventVariations[i]->Name() == name) return _eventVariations[i]->GetIndex();
    }  

    return -1;  
}

//********************************************************************
bool EventVariationManager::HasEventVariation(const std::string& name){
//********************************************************************

  return (GetEventVariation(name)!=NULL);
}

//***********************************************************
void EventVariationManager::ApplyEventVariations(const ToyExperiment& toy, AnaEventC& event) {
//***********************************************************

  for (int j = 0; j < _nEventVariationsEnabled; ++j){
    Int_t isyst = _eventVariationsEnabled[j];
    _eventVariationsWithNull[isyst]->Apply(toy, event);
  }
}

//***********************************************************
bool EventVariationManager::UndoEventVariations(AnaEventC& event) {
//***********************************************************

    // Returns true when any of the systematics returns true. That means that the full Spill will be reseted

    bool ok=false;  
    for (UInt_t i = 0; i < _eventVariations.size(); ++i) {
        if (_eventVariations[i]->IsEnabled())
            if (_eventVariations[i]->UndoSystematic(event)) ok=true;
    }
    return ok;
}


//***********************************************************
void EventVariationManager::EnableEventVariation(Int_t index) {
//***********************************************************

    EventVariationBase* syst = GetEventVariation(index);
    if (syst){
        syst->SetEnabled(true);
        if (syst->Type() == EventVariationBase::kVariation){
            _eventVariationsEnabled[_nEventVariationsEnabled] = index;
            _nEventVariationsEnabled++;
        }
    }
}

//***********************************************************
void EventVariationManager::DisableEventVariation(Int_t index) {
//***********************************************************

    EventVariationBase* syst = GetEventVariation(index);
    if (!syst) return;
    syst->SetEnabled(false);
    if (syst->Type() == EventVariationBase::kVariation){    
        for (Int_t it = 0; it < _nEventVariationsEnabled;it++){
            if (_eventVariationsEnabled[it] == index){
                for(int j = it; j < _nEventVariationsEnabled - 1; ++j){
                    _eventVariationsEnabled[j] = _eventVariationsEnabled[j+1];
                }
                _nEventVariationsEnabled--;
                break;
            }
        }
    }
}

//***********************************************************
void EventVariationManager::EnableEventVariations(const std::vector<Int_t>& indices) {
//***********************************************************

    for (std::vector<Int_t>::const_iterator it=indices.begin();it!=indices.end();it++){
        EnableEventVariation(*it);
    }
}

//***********************************************************
void EventVariationManager::DisableEventVariations(const std::vector<Int_t>& indices) {
//***********************************************************

    for (std::vector<Int_t>::const_iterator it=indices.begin();it!=indices.end();it++){
        DisableEventVariation(*it);
    }
}

//***********************************************************
void EventVariationManager::EnableAllEventVariations() {
//***********************************************************

    _nEventVariationsEnabled = 0;
    for (UInt_t it = 0; it < _eventVariations.size(); it++) {
        _eventVariations[it]->SetEnabled(true);
        _eventVariationsEnabled[_nEventVariationsEnabled] = _eventVariations[it]->GetIndex();
        _nEventVariationsEnabled++;
    }
}

//***********************************************************
void EventVariationManager::DisableAllEventVariations() {
//***********************************************************

    for (UInt_t it = 0; it < _eventVariations.size(); it++) {
        _eventVariations[it]->SetEnabled(false);
    }
    _nEventVariationsEnabled = 0;
}

//***********************************************************
void EventVariationManager::AddEventVariation(Int_t index, const std::string& name, EventVariationBase* sys) {
//***********************************************************

    sys->SetName(name);
    AddEventVariation(index,sys);
}

//***********************************************************
void EventVariationManager::AddEventVariation(Int_t index, EventVariationBase* sys) {
//***********************************************************

    if (GetEventVariation(index)){
        std::cout << "EventVariationManager::AddEventVariation(). '" << index << "' already added !!!" <<std::endl;        
        exit(1);
    }

    //  sys->SetName(name);  
    sys->SetIndex(index);  
    _eventVariationsWithNull[index] = sys;
    _eventVariations.push_back(sys);
}

//***********************************************************
void EventVariationManager::ReplaceEventVariation(Int_t index, EventVariationBase* sys) {
//***********************************************************

    bool found=false;
    for (UInt_t it = 0; it < _eventVariations.size(); it++) {
        if (_eventVariations[it]->GetIndex() == index){
            //      sys->SetName(name);
            _eventVariations[it] = sys; 
            found=true;
            break;
        }
    }
    if (!found){
        std::cout << "EventVariationManager::ReplaceEventVariation(). '" << index << "' does not exist !!!" <<std::endl;
        exit(1);
    }

}


//********************************************************************
void EventVariationManager::DumpEventVariations(){
//********************************************************************

    std::cout << " -------- List of EventVariations ---------------" << std::endl;
    char out[256];  
    sprintf(out,"%3s: %-25s %-15s %-4s", "#", "name", "pdf", "NPar");
    std::cout << out << "\n" << std::endl;

    Int_t j=0;
    for (UInt_t it = 0; it < _eventVariations.size(); it++) {
        if (_eventVariations[it]->IsEnabled()){
            sprintf(out,"%3d: %-25s %-15s %-4d", j, _eventVariations[it]->Name().c_str(), _eventVariations[it]->ConvertPDF().c_str(), _eventVariations[it]->GetNParameters());
            std::cout << out << std::endl;
            j++;
        }
    }
    std::cout << " --------------------------------------------------" << std::endl;
}

