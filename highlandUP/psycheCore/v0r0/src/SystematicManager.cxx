#include "SystematicManager.hxx"
#include "MultiThread.hxx"


//***********************************************************
SystematicManager::SystematicManager(): HLClonesArray("config","SYST","SystematicBase",NMAXSYSTEMATICS) {
//***********************************************************

    _covariance = new SystematicCovariance();
    _systematics.resize(NMAXSYSTEMATICS);
    _systematics.assign(_systematics.size(),NULL);  

    _nVariationSystematics = 0;
    _nWeightSystematics = 0;
    _nFluxSystematics = 0;
    _nVariationSystematicsEnabled = 0;
    _nWeightSystematicsEnabled = 0;
    _nFluxSystematicsEnabled = 0;

}

//***********************************************************
SystematicManager::~SystematicManager() {
//***********************************************************
/*
    for (int i = 0; i < _nVariationSystematics; ++i) {
        delete _variationSystematics[i];
    }
    for (int i = 0; i < _nWeightSystematics; ++i) {
        delete _weightSystematics[i];
    }
*/
    for (int i = 0; i < _nFluxSystematics; ++i) {
        delete _fluxSystematics[i];
    }
}

//***********************************************************
void SystematicManager::Initialize(SelectionManager& sel, Int_t nevents) {
//***********************************************************

  // For variation systematics the SystBox is the same for all selections and branches
  // Create the SystBox for a single selection and branch, for a given number of events
  for (int j = 0; j < _nVariationSystematics; ++j){
    if (_variationSystematics[j]->IsEnabled())
      _variationSystematics[j]->Initialize(1,0,1, nevents);
  }
  
  //For each systematic, create the SystBox for each selection, with nbranches and for a given number of events
  for (std::vector<SelectionBase*>::iterator sit= sel.GetSelections().begin();sit!=sel.GetSelections().end();sit++){
    SelectionBase* selec = *sit;
    if (!selec->IsEnabled()) continue;                                     
    for (int j = 0; j < _nWeightSystematics; ++j){
      if (_weightSystematics[j]->IsEnabled())
        _weightSystematics[j]->Initialize(sel.GetNEnabledSelections(),selec->GetEnabledIndex(), selec->GetNBranches(), nevents);
    }
  }
    //TODO
}

//***********************************************************
void SystematicManager::FinalizeEventSystematics(AnaEventC& event) {
//***********************************************************
  for (int j = 0; j < _nVariationSystematics; ++j){
    if (_variationSystematics[j]->IsEnabled())
      _variationSystematics[j]->FinalizeEvent(event);
  }

  for (int j = 0; j < _nWeightSystematics; ++j){
    if (_weightSystematics[j]->IsEnabled())
      _weightSystematics[j]->FinalizeEvent(event);
  }
    //TODO
}

//***********************************************************
void SystematicManager::InitializeEventSystematics(SelectionManager& sel, AnaEventC& event) {
//***********************************************************

  // Fill the SysteBox for the enabled systematics

  for (int j = 0; j < _nVariationSystematics; ++j){
    if (!_variationSystematics[j]->IsEnabled()) continue;      
    for (std::vector<SelectionBase*>::iterator sit= sel.GetSelections().begin();sit!=sel.GetSelections().end();sit++){
      SelectionBase* selec = *sit;
      if (!selec->IsEnabled()) continue;                                     
      _variationSystematics[j]->InitializeEvent(event,*selec);
    }
    _variationSystematics[j]->InitializeEvent(event);
  }

#ifdef MULTITHREAD
  for (int j = 0; j < _nWeightSystematics; ++j){
    if (!_weightSystematics[j]->IsEnabled()) continue;      
    for (std::vector<SelectionBase*>::iterator sit= sel.GetSelections().begin();sit!=sel.GetSelections().end();sit++){
      SelectionBase* selec = *sit;
      if (!selec->IsEnabled()) continue;                     
      for (UInt_t ibranch=0;ibranch<selec->GetNBranches();ibranch++){        
        _weightSystematics[j]->InitializeEvent(event,*selec,ibranch);
      }
    }
  }
#endif
}

//********************************************************************
std::vector<SystematicBase*> SystematicManager::GetSystematics(const std::vector<Int_t>& indices) const{
//********************************************************************
  
  std::vector<SystematicBase*> systematics;
  for (UInt_t i=0;i<indices.size();i++)
    systematics.push_back(GetSystematic(indices[i]));
  
  return systematics;
}

//********************************************************************
SystematicBase* SystematicManager::GetSystematic(Int_t index) const{
//********************************************************************

  if (index<0 || (UInt_t)index> NMAXSYSTEMATICS-1) return NULL;
  return _systematics[index];
}

//********************************************************************
SystematicBase* SystematicManager::GetSystematic(const std::string& name) const{
//********************************************************************

    for (UInt_t i = 0; i < NMAXSYSTEMATICS; ++i) {
        if (!_systematics[i]) continue;
        if (_systematics[i]->Name() == name) return _systematics[i];
    }
    return NULL;  
}

//********************************************************************
EventWeightBase* SystematicManager::GetSystematicWeight(Int_t index) const{
//********************************************************************

    SystematicBase* sys = GetSystematic(index);
    if (!sys) return NULL;  
    if (sys->Type() != SystematicBase::kWeight) return NULL;
    return static_cast<EventWeightBase*>(sys);
}



//********************************************************************
EventWeightBase* SystematicManager::GetSystematicWeight(const std::string& name) const{
//********************************************************************

    for (int i = 0; i < _nWeightSystematics; ++i) {
        if (_weightSystematics[i]->Name() == name) return _weightSystematics[i];
    }
    return NULL;  
}

//********************************************************************
EventVariationBase* SystematicManager::GetSystematicVariation(Int_t index) const{
//********************************************************************

    SystematicBase* sys = GetSystematic(index);
    if (!sys) return NULL;  
    if (sys->Type() != SystematicBase::kVariation) return NULL;
    return static_cast<EventVariationBase*>(sys);
}

//********************************************************************
EventWeightBase* SystematicManager::GetSystematicFlux(const std::string& name) const{
//********************************************************************

    for (int i = 0; i < _nFluxSystematics; ++i) {
        if (_fluxSystematics[i]->Name() == name) return _fluxSystematics[i];
    }

    return NULL;  
}

//********************************************************************
EventWeightBase* SystematicManager::GetSystematicFlux(Int_t index) const{
//********************************************************************

    SystematicBase* sys = GetSystematic(index);
    if (!sys) return NULL;  
    if (sys->Type() != SystematicBase::kFlux) return NULL;
    return static_cast<EventWeightBase*>(sys);
}



//********************************************************************
EventVariationBase* SystematicManager::GetSystematicVariation(const std::string& name) const{
//********************************************************************

    for (int i = 0; i < _nVariationSystematics; i++) {
        if (_variationSystematics[i]->Name() == name) return _variationSystematics[i];
    }

    return NULL;
}

//********************************************************************
Int_t SystematicManager::GetSystematicWeightIndex(const std::string& name){
//********************************************************************

    for (int i = 0; i < _nWeightSystematics; i++) {
        if (_weightSystematics[i]->Name() == name) return _weightSystematics[i]->GetIndex();
    }  

    return -1;  
}

//********************************************************************
Int_t SystematicManager::GetSystematicFluxIndex(const std::string& name){
//********************************************************************

    for (int i = 0; i < _nFluxSystematics; i++) {
        if (_fluxSystematics[i]->Name() == name) return i;
    }

    return -1;  
}

//********************************************************************
Int_t SystematicManager::GetSystematicVariationIndex(const std::string& name){
//********************************************************************

    for (int i = 0; i < _nVariationSystematics; i++) {
        if (_variationSystematics[i]->Name() == name) return _variationSystematics[i]->GetIndex();
    }

    return -1;
}

//********************************************************************
bool SystematicManager::HasSystematic(const std::string& name){
//********************************************************************

    for (int i = 0; i < _nVariationSystematics; i++) {
        if (_variationSystematics[i]->Name() == name) return true;
    }
    for (int i = 0; i < _nWeightSystematics; i++) {
        if (_weightSystematics[i]->Name() == name) return true;
    }  
    for (int i = 0; i < _nFluxSystematics; i++) {
        if (_fluxSystematics[i]->Name() == name) return true;
    }

    return false;  
}

//***********************************************************
void SystematicManager::ApplyVariationSystematics(const ToyExperiment& toy, AnaEventC& event) {
//***********************************************************

  for (int j = 0; j < _nVariationSystematicsEnabled; ++j){
    Int_t isyst = _variationSystematicsEnabled[j];
    EventVariationBase* var = static_cast<EventVariationBase*>(_systematics[isyst]);
    // Apply the variation systematic. Modify the input data
	  // It needs as input the variations corresponding to the ith systematic
	  var->Apply(toy, event);
  }
}

//***********************************************************
Weight_h SystematicManager::ApplyWeightSystematics(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox) {
//***********************************************************
    // WeightSystematics are applied at the end of the each toy experiment

    // reset the weight to 1 before applying any systematic
    Weight_h totalWeight(1);

    for (int j = 0; j < _nWeightSystematics; j++) {
        // only apply enabled systematics
        if (_weightSystematics[j]->IsEnabled()){
          // Compute the event normalization weight for this systematic
          totalWeight *= _weightSystematics[j]->ComputeWeight(toy, event, ToyBox);      
        }
    }
    return totalWeight;
}

//***********************************************************
Weight_h SystematicManager::ApplyWeightSystematics(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox, Weight_h* weights) {
//***********************************************************

    // WeightSystematics are applied at the end of the each toy experiment
    // This method returns as argument an array of weights, one for each of the weight systematics enabled

    // reset the weight to 1 before applying any systematic
    Weight_h totalWeight(1);
    Weight_h weight;

    Int_t w=0;
    for (int j = 0; j < _nWeightSystematics; j++) {
        // only apply enabled systematics
        if (_weightSystematics[j]->IsEnabled()){
          // Compute the event normalization weight for this systematic
          weight = _weightSystematics[j]->ComputeWeight(toy, event, ToyBox);      
          // Save in into the vector
          weights[w++]=weight;	    
          // Increment the total weight
          totalWeight*= weight;      
        }
    }

    return totalWeight;

}

//***********************************************************
Weight_h SystematicManager::ApplyWeightSystematics(const SelectionBase& sel, const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox, Weight_h* weights) {
//***********************************************************

    // Apply the relevant weight systematics for a given selection
    // This method returns as argument an array of weights, one for each of the weight systematics enabled

    // WeightSystematics are applied at the end of the each toy experiment

    // reset the weight to 1 before applying any systematic
    Weight_h totalWeight(1);
    Weight_h weight;

    Int_t w=0;
    for (int j = 0; j < _nWeightSystematics; j++) {
      // only apply enabled systematics
      if (!_weightSystematics[j]->IsEnabled())  continue;

      Int_t ibranch = ToyBox.SuccessfulBranch;
      // Check if the systematic is relevant for this selection and branch. If not the weight is 1
      // Any of the branches should be succesful. Otherwise ibranch=-1
      if (ibranch>-1 && sel.IsRelevantSystematic(event, ToyBox, _weightSystematics[j]->GetIndex(), ibranch)){          
#ifndef MULTITHREAD
        // We need to initialize the event (Fill the SystBox). We only do that for the first succesful toy in a given selection and branch
        if (!_weightSystematics[j]->GetSystBox(event,sel.GetEnabledIndex(),ibranch))
          _weightSystematics[j]->InitializeEvent(event,sel,ibranch);
#endif
        // Compute the event normalization weight for this systematic
        weight = _weightSystematics[j]->ComputeWeight(toy, event, ToyBox, sel);      
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
Weight_h  SystematicManager::ApplyFluxSystematics(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox) {
//***********************************************************

    // Flux Systematics are applied at the end of the each virtual analysis

    Weight_h  weight(1);

    for (int j = 0; j < _nFluxSystematics; j++) {
        // only apply enabled systematics
        if (_fluxSystematics[j]->IsEnabled()){
          // Compute the event normalization weight for this systematic
          weight *= _fluxSystematics[j]->ComputeWeight(toy, event, ToyBox);      
        }
    }

    return weight;
}


//***********************************************************
Weight_h SystematicManager::ApplyWeightSystematic(Int_t index, const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& ToyBox) {
//***********************************************************

    // Apply an specific weight systematic, provided its index
    // WeightSystematics are applied at the end of the each virtual analysis

    // Compute the event normalization weight for this systematic
    return _weightSystematics[index]->ComputeWeight(toy, event, ToyBox);      
}

//***********************************************************
bool SystematicManager::UndoVariationSystematics(AnaEventC& event) {
//***********************************************************

    // Returns true when any of the systematics returns true. That means that the full Spill will be reseted

    bool ok=false;  
    for (int i = 0; i < _nVariationSystematics; ++i) {
        if (_variationSystematics[i]->IsEnabled())
            if (_variationSystematics[i]->UndoSystematic(event)) ok=true;
    }
    return ok;
}

//***********************************************************
void SystematicManager::EnableSystematic(Int_t index) {
//***********************************************************

    SystematicBase* syst = GetSystematic(index);
    if (syst){
        syst->SetEnabled(true);
        if (syst->Type() == SystematicBase::kWeight){
            _weightSystematicsEnabled[_nWeightSystematicsEnabled] = index;
            _nWeightSystematicsEnabled++;
        }
        else if (syst->Type() == SystematicBase::kVariation){
            _variationSystematicsEnabled[_nVariationSystematicsEnabled] = index;
            _nVariationSystematicsEnabled++;
        }
        else if (syst->Type() == SystematicBase::kFlux){
            _fluxSystematicsEnabled[_nFluxSystematicsEnabled] = index;
            _nFluxSystematicsEnabled++;
        }
    }
}

//***********************************************************
void SystematicManager::DisableSystematic(Int_t index) {
//***********************************************************

    SystematicBase* syst = GetSystematic(index);
    if (!syst) return;
    syst->SetEnabled(false);
    if (syst->Type() == SystematicBase::kWeight){    
        for (Int_t it = 0; it < _nWeightSystematicsEnabled;it++){
            if (_weightSystematicsEnabled[it] == index){
                for(int j = it; j < _nWeightSystematicsEnabled - 1; ++j){
                    _weightSystematicsEnabled[j] = _weightSystematicsEnabled[j+1];
                }
                _nWeightSystematicsEnabled--;
                break;
            }
        }
    }
    else if (syst->Type() == SystematicBase::kVariation){
        for (Int_t it = 0; it < _nVariationSystematicsEnabled;it++){
            if (_variationSystematicsEnabled[it] == index){
                for(int j = it; j < _nVariationSystematicsEnabled - 1; ++j){
                    _variationSystematicsEnabled[j] = _variationSystematicsEnabled[j+1];
                }
                _nVariationSystematicsEnabled--;
                break;
            }
        }
    }
    else if (syst->Type() == SystematicBase::kFlux){
        for (Int_t it = 0; it < _nFluxSystematicsEnabled;it++){
            if (_fluxSystematicsEnabled[it] == index){
                for(int j = it; j < _nFluxSystematicsEnabled - 1; ++j){
                    _fluxSystematicsEnabled[j] = _fluxSystematicsEnabled[j+1];
                }
                _nFluxSystematicsEnabled--;
                break;
            }
        }
    }
}

//***********************************************************
void SystematicManager::EnableSystematics(const std::vector<Int_t>& indices) {
//***********************************************************

    for (std::vector<Int_t>::const_iterator it=indices.begin();it!=indices.end();it++){
        EnableSystematic(*it);
    }
}

//***********************************************************
void SystematicManager::DisableSystematics(const std::vector<Int_t>& indices) {
//***********************************************************

    for (std::vector<Int_t>::const_iterator it=indices.begin();it!=indices.end();it++){
        DisableSystematic(*it);
    }
}

//***********************************************************
void SystematicManager::EnableAllVariationSystematics() {
//***********************************************************

    _nVariationSystematicsEnabled = 0;
    for (int it = 0; it < _nVariationSystematics; it++) {
        _variationSystematics[it]->SetEnabled(true);
        _variationSystematicsEnabled[_nVariationSystematicsEnabled] = _variationSystematics[it]->GetIndex();
        _nVariationSystematicsEnabled++;
    }
}

//***********************************************************
void SystematicManager::EnableAllWeightSystematics() {
//***********************************************************

    _nWeightSystematicsEnabled = 0;
    for (int it = 0; it < _nWeightSystematics; it++) {
        _weightSystematics[it]->SetEnabled(true);
        _weightSystematicsEnabled[_nWeightSystematicsEnabled] = _weightSystematics[it]->GetIndex();
        _nWeightSystematicsEnabled++;
    }
}


//***********************************************************
void SystematicManager::EnableAllFluxSystematics() {
//***********************************************************

    _nFluxSystematicsEnabled = 0;
    for (int it = 0; it < _nFluxSystematics; it++) {
        _fluxSystematics[it]->SetEnabled(true);
        _fluxSystematicsEnabled[_nFluxSystematicsEnabled] = _fluxSystematics[it]->GetIndex();
        _nFluxSystematicsEnabled++;
    }
}

//***********************************************************
void SystematicManager::EnableAllSystematics() {
//***********************************************************

    EnableAllWeightSystematics();
    EnableAllVariationSystematics();
    EnableAllFluxSystematics();
}

//***********************************************************
void SystematicManager::DisableAllVariationSystematics() {
//***********************************************************

    for (int it = 0; it < _nVariationSystematics; it++) {
        _variationSystematics[it]->SetEnabled(false);
    }
    _nVariationSystematicsEnabled = 0;
}

//***********************************************************
void SystematicManager::DisableAllWeightSystematics() {
//***********************************************************

    for (int it = 0; it < _nWeightSystematics; it++) {
        _weightSystematics[it]->SetEnabled(false);
    }
    _nWeightSystematicsEnabled = 0;
}

//***********************************************************
void SystematicManager::DisableAllFluxSystematics() {
//***********************************************************

    for (int it = 0; it < _nFluxSystematics; it++) {
        _fluxSystematics[it]->SetEnabled(false);
    }
    _nFluxSystematicsEnabled = 0;

}


//***********************************************************
void SystematicManager::DisableAllSystematics() {
//***********************************************************

    DisableAllWeightSystematics();
    DisableAllVariationSystematics();
    DisableAllFluxSystematics();
}

//***********************************************************
void SystematicManager::AddVariationSystematic(Int_t index, const std::string& name, EventVariationBase* sys) {
//***********************************************************

    sys->SetName(name);
    AddVariationSystematic(index,sys);
}

//***********************************************************
void SystematicManager::AddVariationSystematic(Int_t index, EventVariationBase* sys) {
//***********************************************************

    if (GetSystematicVariation(index)){
        std::cout << "SystematicManager::AddVariationSystematic(). '" << index << "' already added !!!" <<std::endl;        
        exit(1);
    }

    sys->SetIndex(index);
    _variationSystematics[_nVariationSystematics] = sys;
    _nVariationSystematics++;
    _systematics[index] = sys;
    (*_objects)[_NObjects++] = sys;
}

//***********************************************************
void SystematicManager::ReplaceVariationSystematic(Int_t index, EventVariationBase* sys) {
//***********************************************************

    std::cout << _NObjects << std::endl;
    DumpSystematics();

    for (Int_t i=0;i<_NObjects;i++){
        SystematicBase* sys2 = (SystematicBase*)(*_objects)[i];    
        std::cout << i << " " << sys2 << std::endl;
        std::cout << sys2->GetNParameters() << std::endl;
        std::cout << sys2->GetIndex() << std::endl;
    }


    bool found=false;
    for (int it = 0; it < _nVariationSystematics; it++) {
        if (_variationSystematics[it]->GetIndex() == index){
            //      sys->SetName(name);
            _variationSystematics[it] = sys; 
            found=true;
            break;
        }
    }
    if (!found){
        std::cout << "SystematicManager::ReplaceVariationSystematic(). '" << index << "' does not exist !!!" <<std::endl;
        exit(1);
    }

    for (Int_t i=0;i<_NObjects;i++){
        SystematicBase* sys2 = (SystematicBase*)(*_objects)[i];    
        std::cout << i << " " << sys2 << std::endl;
        std::cout << sys2->GetNParameters() << std::endl;
        std::cout << sys2->Name() << std::endl;
        if (sys2->GetIndex() == index){
            (*_objects)[i] = sys;
            break;
        }
    }
}

//***********************************************************
void SystematicManager::AddWeightSystematic(Int_t index, const std::string& name, EventWeightBase* sys) {
//***********************************************************

    sys->SetName(name);
    AddWeightSystematic(index,sys);
}

//***********************************************************
void SystematicManager::AddWeightSystematic(Int_t index, EventWeightBase* sys) {
//***********************************************************

    if (GetSystematicWeight(index)){
        std::cout << "SystematicManager::AddWeightSystematic(). '" << index << "' already added !!!" <<std::endl;        
        exit(1);
    }

    //  sys->SetName(name);  
    sys->SetIndex(index);  
    _weightSystematics[_nWeightSystematics++] = sys;
    _systematics[index] = sys;
    (*_objects)[_NObjects++] = sys;
}

//***********************************************************
void SystematicManager::ReplaceWeightSystematic(Int_t index, EventWeightBase* sys) {
//***********************************************************

    bool found=false;
    for (int it = 0; it < _nWeightSystematics; it++) {
        if (_weightSystematics[it]->GetIndex() == index){
            //      sys->SetName(name);
            _weightSystematics[it] = sys; 
            found=true;
            break;
        }
    }
    if (!found){
        std::cout << "SystematicManager::ReplaceWeightSystematic(). '" << index << "' does not exist !!!" <<std::endl;
        exit(1);
    }

    for (Int_t i=0;i<_NObjects;i++){
        SystematicBase* sys2 = (SystematicBase*)(*_objects)[i];    
        if (sys2->GetIndex() == index){
            (*_objects)[i] = sys;
            break;
        }
    }
}

//***********************************************************
void SystematicManager::AddFluxSystematic(Int_t index, EventWeightBase* sys) {
//***********************************************************

    if (GetSystematicFlux(index)){
        std::cout << "SystematicManager::AddFluxSystematic(). '" << index << "' already added !!!" <<std::endl;        
        exit(1);
    }

    sys->SetIndex(index);  
    _fluxSystematics[_nFluxSystematics++] = sys;
    _systematics[index] = sys;
    (*_objects)[_NObjects++] = sys;
}

//***********************************************************
void SystematicManager::AddFluxSystematic(Int_t index, const std::string& name, EventWeightBase* sys) {
//***********************************************************
    sys->SetName(name);
    AddFluxSystematic(index,sys);

}

//***********************************************************
void SystematicManager::ReplaceFluxSystematic(Int_t index, EventWeightBase* sys) {
//***********************************************************

    bool found=false;
    for (int it = 0; it < _nFluxSystematics; it++) {
        if (_fluxSystematics[it]->GetIndex() == index){
            //      sys->SetName(name);
            _fluxSystematics[it] = sys; 
            found=true;
            break;
        }
    }
    if (!found){
        std::cout << "SystematicManager::ReplaceFluxSystematic(). '" << index << "' does not exist !!!" <<std::endl;
        exit(1);
    }

    for (Int_t i=0;i<_NObjects;i++){
        SystematicBase* sys2 = (SystematicBase*)(*_objects)[i];    
        if (sys2->GetIndex() == index){
            (*_objects)[i] = sys;
            break;
        }
    }
}


//********************************************************************
void SystematicManager::ReadSystematics(const std::string& file){
//********************************************************************

    // Reset the vectors
    _nVariationSystematics = 0;
    _nWeightSystematics = 0;
    _nFluxSystematics = 0;
    _nVariationSystematicsEnabled = 0;
    _nWeightSystematicsEnabled = 0;
    _nFluxSystematicsEnabled = 0;

    _systematics.resize(NMAXSYSTEMATICS);
    _systematics.assign(_systematics.size(),NULL);

    ReadClonesArray(file);

    // Add them to the map of categories
    for (Int_t i=0;i<_NObjects;i++){
        SystematicBase* sys = (SystematicBase*)(*_objects)[i];    
        if (sys->Type() == SystematicBase::kVariation){
            _variationSystematics[_nVariationSystematics++] = (EventVariationBase*)sys;
            _variationSystematicsEnabled[_nVariationSystematicsEnabled++] = sys->GetIndex();
        }
        else if (sys->Type() == SystematicBase::kWeight){
            _weightSystematics[_nWeightSystematics++] = (EventWeightBase*)sys; 
            _weightSystematicsEnabled[_nWeightSystematicsEnabled++] = sys->GetIndex();
        }
        else if (sys->Type() == SystematicBase::kFlux){
            _fluxSystematics[_nFluxSystematics++] = (EventWeightBase*)sys;
            _fluxSystematicsEnabled[_nFluxSystematicsEnabled++] = sys->GetIndex();
        }
        _systematics[sys->GetIndex()]= sys;
    }
}

//********************************************************************
void SystematicManager::DumpSystematics(){
//********************************************************************

    std::cout << " -------- List of Systematics  --------------------" << std::endl;
    char out[256];  
    sprintf(out,"%3s: %-25s %-15s  %4s", "#", "name", "type", "NPar");
    std::cout << out <<"\n" << std::endl;

    for (UInt_t i=0;i<_systematics.size();i++){
        SystematicBase* sys = _systematics[i];    
        if (!sys) continue;
        sprintf(out,"%3d: %-25s %-15s %4d", i, sys->Name().c_str(), sys->ConvertType().c_str(), sys->GetNParameters());
        std::cout << out << std::endl;
    }

    std::cout << " --------------------------------------------------" << std::endl;
}


//********************************************************************
void SystematicManager::DumpVariationSystematics(){
//********************************************************************

    std::cout << " -------- List of VariationSystematics ------------" << std::endl;
    char out[256];  
    sprintf(out,"%3s: %-25s %-15s %-4s", "#", "name", "pdf", "NPar");
    std::cout << out << "\n" << std::endl;


    Int_t j=0;
    for (int it = 0; it < _nVariationSystematics; it++) {
        if (_variationSystematics[it]->IsEnabled()){
            sprintf(out,"%3d: %-25s %-15s %-4d", j, _variationSystematics[it]->Name().c_str(), _variationSystematics[it]->ConvertPDF().c_str(), _variationSystematics[it]->GetNParameters());
            std::cout << out << std::endl;
            j++;
        }
    }
    std::cout << " --------------------------------------------------" << std::endl;
}

//********************************************************************
void SystematicManager::DumpWeightSystematics(){
//********************************************************************

    std::cout << " -------- List of WeightSystematics ---------------" << std::endl;
    char out[256];  
    sprintf(out,"%3s: %-25s %-15s %-4s", "#", "name", "pdf", "NPar");
    std::cout << out << "\n" << std::endl;

    Int_t j=0;
    for (int it = 0; it < _nWeightSystematics; it++) {
        if (_weightSystematics[it]->IsEnabled()){
            sprintf(out,"%3d: %-25s %-15s %-4d", j, _weightSystematics[it]->Name().c_str(), _weightSystematics[it]->ConvertPDF().c_str(), _weightSystematics[it]->GetNParameters());
            std::cout << out << std::endl;
            j++;
        }
    }
    std::cout << " --------------------------------------------------" << std::endl;
}

//********************************************************************
void SystematicManager::DumpFluxSystematics(){
//********************************************************************

    std::cout << "-------- List of FluxSystematics -------" << std::endl;
    char out[256];  
    sprintf(out,"%3s: %-25s %-15s %-4s", "#", "name", "pdf", "NPar");
    std::cout << out << std::endl;
    std::cout << " --------------------------------------------------" << std::endl;

    Int_t j=0;
    for (int it = 0; it < _nFluxSystematics; it++) {
        if (_fluxSystematics[it]->IsEnabled()){
            sprintf(out,"%3d: %-25s %-15s %-4d", j, _fluxSystematics[it]->Name().c_str(), _fluxSystematics[it]->ConvertPDF().c_str(), _fluxSystematics[it]->GetNParameters());
            std::cout << out << std::endl;
            j++;
        }
    }
    std::cout << " --------------------------------------------------" << std::endl;
}

//********************************************************************
void SystematicManager::ConstructCovarianceMatrix(){
//********************************************************************
    _covariance->ConstructCovarianceMatrix(this);
}
//********************************************************************
TMatrixT<double>* SystematicManager::GetCovarianceMatrix() const{
//********************************************************************
    return _covariance->GetCovarianceMatrix();
}
