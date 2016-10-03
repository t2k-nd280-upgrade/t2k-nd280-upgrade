#include "EventWeightManager.hxx"
#include "MultiThread.hxx"


//***********************************************************
EventWeightManager::EventWeightManager(){
//***********************************************************

    _nEventWeightsEnabled = 0;

    _eventWeightsWithNull.resize(NMAXEVENTWEIGHTS);
    _eventWeightsWithNull.assign(_eventWeightsWithNull.size(),NULL);  
    
    _eventWeights.clear();
}

//***********************************************************
EventWeightManager::~EventWeightManager() {
//***********************************************************

    for (UInt_t i = 0; i < _eventWeights.size(); ++i) {
        delete _eventWeights[i];
    }
}

//***********************************************************
void EventWeightManager::Initialize(SelectionManager& sel, Int_t nevents) {
//***********************************************************

  //For each enabled EventWeight, create the SystBox for each selection, with nbranches and for a given number of events
  for (std::vector<SelectionBase*>::iterator sit= sel.GetSelections().begin();sit!=sel.GetSelections().end();sit++){
    SelectionBase* selec = *sit;
    if (!selec->IsEnabled()) continue;                                     
    for (UInt_t j = 0; j < _eventWeights.size(); ++j){
      if (_eventWeights[j]->IsEnabled())
        _eventWeights[j]->Initialize(sel.GetNEnabledSelections(),selec->GetEnabledIndex(), selec->GetNBranches(), nevents);
    }
  }
    //TODO
}

//***********************************************************
void EventWeightManager::FinalizeEvent(AnaEventC& event) {
//***********************************************************

  for (UInt_t j = 0; j < _eventWeights.size(); ++j){
    if (_eventWeights[j]->IsEnabled())
      _eventWeights[j]->FinalizeEvent(event);
  }
    //TODO
}

//***********************************************************
void EventWeightManager::InitializeEvent(SelectionManager& sel, AnaEventC& event) {
//***********************************************************

  // Fill the SysteBox for the enabled EventWeights

#ifdef MULTITHREAD
  for (UInt_t j = 0; j < _eventWeights.size(); ++j){
    if (!_eventWeights[j]->IsEnabled()) continue;      
    for (std::vector<SelectionBase*>::iterator sit= sel.GetSelections().begin();sit!=sel.GetSelections().end();sit++){
      SelectionBase* selec = *sit;
      if (!selec->IsEnabled()) continue;                     
      for (UInt_t ibranch=0;ibranch<selec->GetNBranches();ibranch++){        
        _eventWeights[j]->InitializeEvent(event,*selec,ibranch);
      }
    }
  }
#else
  (void)sel;
  (void)event;
#endif
}

//********************************************************************
std::vector<EventWeightBase*> EventWeightManager::GetEventWeights(const std::vector<Int_t>& indices) const{
//********************************************************************
  
  std::vector<EventWeightBase*> weights;
  for (UInt_t i=0;i<indices.size();i++)
    weights.push_back(GetEventWeight(indices[i]));
  
  return weights;
}

//********************************************************************
EventWeightBase* EventWeightManager::GetEventWeight(Int_t index) const{
//********************************************************************

  if (index<0 || (UInt_t)index> NMAXEVENTWEIGHTS-1) return NULL;
  return _eventWeightsWithNull[index];
}

//********************************************************************
EventWeightBase* EventWeightManager::GetEventWeight(const std::string& name) const{
//********************************************************************

    for (UInt_t i = 0; i < _eventWeights.size(); ++i) {
          if (_eventWeights[i]->Name() == name) return _eventWeights[i];
    }
    return NULL;  
}

//********************************************************************
Int_t EventWeightManager::GetEventWeightIndex(const std::string& name){
//********************************************************************

    for (UInt_t i = 0; i < _eventWeights.size(); i++) {
        if (_eventWeights[i]->Name() == name) return _eventWeights[i]->GetIndex();
    }  

    return -1;  
}

//********************************************************************
bool EventWeightManager::HasEventWeight(const std::string& name){
//********************************************************************

  return (GetEventWeight(name)!=NULL);
}

//***********************************************************
Weight_h EventWeightManager::ComputeEventWeights(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox) {
//***********************************************************
    // EventWeights are applied at the end of the each toy experiment

    // reset the weight to 1 before applying any systematic
    Weight_h totalWeight(1);

    for (UInt_t j = 0; j < _eventWeights.size(); j++) {
        // only apply enabled systematics
        if (_eventWeights[j]->IsEnabled()){
            // Compute the event normalization weight for this systematic
          //          totalWeight *= _eventWeights[j]->ComputeWeight(*(toy.GetVariations( _eventWeights[j]->GetIndex())), event, ToyBox);      
          totalWeight *= _eventWeights[j]->ComputeWeight(toy, event, ToyBox);      
        }
    }
    return totalWeight;
}

//***********************************************************
Weight_h EventWeightManager::ComputeEventWeights(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox, Weight_h* weights) {
//***********************************************************

    // EventWeights are applied at the end of the each toy experiment
    // This method returns as argument an array of weights, one for each of the weight systematics enabled

    // reset the weight to 1 before applying any systematic
    Weight_h totalWeight(1);
    Weight_h weight;

    Int_t w=0;
    for (UInt_t j = 0; j < _eventWeights.size(); j++) {
        // only apply enabled systematics
        if (_eventWeights[j]->IsEnabled()){
          // Compute the event normalization weight for this systematic
          //          weight = _eventWeights[j]->ComputeWeight(*(toy.GetVariations(_eventWeights[j]->GetIndex())), event, ToyBox);      
          weight = _eventWeights[j]->ComputeWeight(toy, event, ToyBox);      
          // Save in into the vector
          weights[w++]=weight;	    
          // Increment the total weight
          totalWeight*= weight;      
        }
    }

    return totalWeight;

}

//***********************************************************
Weight_h EventWeightManager::ComputeEventWeights(const SelectionBase& sel, const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox, Weight_h* weights) {
//***********************************************************

    // Compute the relevant weight systematics for a given selection
    // This method returns as argument an array of weights, one for each of the weight systematics enabled

    // EventWeights are applied at the end of the each toy experiment

    // reset the weight to 1 before applying any systematic
    Weight_h totalWeight(1);
    Weight_h weight;

    Int_t w=0;
    for (UInt_t j = 0; j < _eventWeights.size(); j++) {
      // only apply enabled systematics
      if (!_eventWeights[j]->IsEnabled())  continue;

      Int_t ibranch = ToyBox.SuccessfulBranch;
      // Check if the systematic is relevant for this selection and branch. If not the weight is 1
      // Any of the branches should be succesful. Otherwise ibranch=-1
      if (ibranch>-1 && sel.IsRelevantSystematic(event, ToyBox, _eventWeights[j]->GetIndex(), ibranch)){          
#ifndef MULTITHREAD
        // We need to initialize the event (Fill the SystBox). We only do that for the first succesful toy in a given selection and branch
        if (!_eventWeights[j]->GetSystBox(event,sel.GetEnabledIndex(),ibranch))
          _eventWeights[j]->InitializeEvent(event,sel,ibranch);
#endif
        // Compute the event normalization weight for this systematic
        //        weight = _eventWeights[j]->ComputeWeight(*(toy.GetVariations(_eventWeights[j]->GetIndex())), event, ToyBox, sel);
        weight = _eventWeights[j]->ComputeWeight(toy, event, ToyBox, sel);      
      }
      else
        weight=1;
      
      // Save it into the double array
      weights[w++]=weight;	    
      // Increment the total weight
      totalWeight*= weight;              
    }

    return totalWeight;
}

//***********************************************************
Weight_h EventWeightManager::ComputeEventWeight(Int_t index, const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox) {
//***********************************************************

    // Compute an specific weight systematic, provided its index
    // EventWeights are applied at the end of the each virtual analysis

    // Compute the event normalization weight for this systematic
  //    return _eventWeights[index]->ComputeWeight(*(toy.GetVariations(index)), event, ToyBox);      
    return _eventWeightsWithNull[index]->ComputeWeight(toy, event, ToyBox);      
}

//***********************************************************
void EventWeightManager::EnableEventWeight(Int_t index) {
//***********************************************************

    EventWeightBase* syst = GetEventWeight(index);
    if (syst){
        syst->SetEnabled(true);
        if (syst->Type() == EventWeightBase::kWeight){
            _eventWeightsEnabled[_nEventWeightsEnabled] = index;
            _nEventWeightsEnabled++;
        }
    }
}

//***********************************************************
void EventWeightManager::DisableEventWeight(Int_t index) {
//***********************************************************

    EventWeightBase* syst = GetEventWeight(index);
    if (!syst) return;
    syst->SetEnabled(false);
    if (syst->Type() == EventWeightBase::kWeight){    
        for (Int_t it = 0; it < _nEventWeightsEnabled;it++){
            if (_eventWeightsEnabled[it] == index){
                for(int j = it; j < _nEventWeightsEnabled - 1; ++j){
                    _eventWeightsEnabled[j] = _eventWeightsEnabled[j+1];
                }
                _nEventWeightsEnabled--;
                break;
            }
        }
    }
}

//***********************************************************
void EventWeightManager::EnableEventWeights(const std::vector<Int_t>& indices) {
//***********************************************************

    for (std::vector<Int_t>::const_iterator it=indices.begin();it!=indices.end();it++){
        EnableEventWeight(*it);
    }
}

//***********************************************************
void EventWeightManager::DisableEventWeights(const std::vector<Int_t>& indices) {
//***********************************************************

    for (std::vector<Int_t>::const_iterator it=indices.begin();it!=indices.end();it++){
        DisableEventWeight(*it);
    }
}

//***********************************************************
void EventWeightManager::EnableAllEventWeights() {
//***********************************************************

    _nEventWeightsEnabled = 0;
    for (UInt_t it = 0; it < _eventWeights.size(); it++) {
        _eventWeights[it]->SetEnabled(true);
        _eventWeightsEnabled[_nEventWeightsEnabled] = _eventWeights[it]->GetIndex();
        _nEventWeightsEnabled++;
    }
}

//***********************************************************
void EventWeightManager::DisableAllEventWeights() {
//***********************************************************

    for (UInt_t it = 0; it < _eventWeights.size(); it++) {
        _eventWeights[it]->SetEnabled(false);
    }
    _nEventWeightsEnabled = 0;
}

//***********************************************************
void EventWeightManager::AddEventWeight(Int_t index, const std::string& name, EventWeightBase* sys) {
//***********************************************************

    sys->SetName(name);
    AddEventWeight(index,sys);
}

//***********************************************************
void EventWeightManager::AddEventWeight(Int_t index, EventWeightBase* sys) {
//***********************************************************

    if (GetEventWeight(index)){
        std::cout << "EventWeightManager::AddEventWeight(). '" << index << "' already added !!!" <<std::endl;        
        exit(1);
    }

    //  sys->SetName(name);  
    sys->SetIndex(index);  
    _eventWeightsWithNull[index] = sys;
    _eventWeights.push_back(sys);
}

//***********************************************************
void EventWeightManager::ReplaceEventWeight(Int_t index, EventWeightBase* sys) {
//***********************************************************

    bool found=false;
    for (UInt_t it = 0; it < _eventWeights.size(); it++) {
        if (_eventWeights[it]->GetIndex() == index){
            //      sys->SetName(name);
            _eventWeights[it] = sys; 
            found=true;
            break;
        }
    }
    if (!found){
        std::cout << "EventWeightManager::ReplaceEventWeight(). '" << index << "' does not exist !!!" <<std::endl;
        exit(1);
    }
}

//********************************************************************
void EventWeightManager::DumpEventWeights(){
//********************************************************************

    std::cout << " -------- List of EventWeights ---------------" << std::endl;
    char out[256];  
    sprintf(out,"%3s: %-25s %-15s %-4s", "#", "name", "pdf", "NPar");
    std::cout << out << "\n" << std::endl;

    Int_t j=0;
    for (UInt_t it = 0; it < _eventWeights.size(); it++) {
        if (_eventWeights[it]->IsEnabled()){
            sprintf(out,"%3d: %-25s %-15s %-4d", j, _eventWeights[it]->Name().c_str(), _eventWeights[it]->ConvertPDF().c_str(), _eventWeights[it]->GetNParameters());
            std::cout << out << std::endl;
            j++;
        }
    }
    std::cout << " --------------------------------------------------" << std::endl;
}

