#include "AnalysisAlgorithm.hxx"

// By default don't allow non-exclusive selections
bool _allowNonExclusiveSelections = false;

//********************************************************************
AnalysisAlgorithm::AnalysisAlgorithm(AnalysisAlgorithm* ana){
//********************************************************************
 
  if (ana){
    _inputManager     = &(ana->input());
    _selManager       = &(ana->sel());
    _corrManager      = &(ana->corr());
    _systManager      = &(ana->syst());
    _weightManager    = &(ana->eweight());
    _variationManager = &(ana->evar());
    _outputManager    = &(ana->output());
    _confManager      = &(ana->conf()); 
    _categManager     = &(ana->cat()); 
    _docManager       = &(ana->doc()); 
  }
  else{
    _inputManager     = new HighlandInputManager();
    _selManager       = new SelectionManager();
    _corrManager      = new CorrectionManager();
    _systManager      = new SystematicManager();
    _weightManager    = new EventWeightManager();
    _variationManager = new EventVariationManager();
    _outputManager    = new OutputManager();
    _confManager      = new ConfigurationManager();
    _categManager     = new CategoryManager();
    _docManager       = new DocStringManager();
  }

  _outputManager->SetDocStringManager(_docManager);

  anaUtils::_categ = &cat();

  for (UInt_t i=0;i<NMAXSELECTIONS;i++)
    _box[i] = NULL;

  _selectedSelection=0;

  // Minimum cut level to save the event. By default save everything
  _min_accum_cut_level = 0;
  _min_accum_cut_level_roo = 0;


  // Fill the RooTrackerVtx tree by default
  _saveRooTrackerVtxTree = true;

  // By default save events that don't pass the selection 
  _fillSuccessOnly=false;

  // By default initialize micro-trees at the beginning of each configuration
  _initializeTrees=true;

  /// Boolean parameter to know whether nny of the methods that sets things into used analysis has been called
  _setMethodCalled=false; 


  /// Version check enabled by default
  _versionCheck = true;

}

//********************************************************************
void AnalysisAlgorithm::UseAnalysis(AnalysisAlgorithm* ana){
//********************************************************************

  if (_setMethodCalled){
    std::cout << "ERROR. AnalysisAlgorithm::UseAnalysis() cannot be called after setting some analysis dependent parameters" << std::endl;
    exit(0);
  }

  _usedAnalyses.push_back(ana);
}

//********************************************************************
bool AnalysisAlgorithm::CheckAccumLevelToSave() {
//********************************************************************
 
  // At least one branch of one of the enabled selections should fulfill the condition
  for (std::vector<SelectionBase*>::iterator it=sel().GetSelections().begin();it!=sel().GetSelections().end();it++){
    if ((*it)->IsEnabled() && (*it)->GetMaxAccumCutLevel() >= GetMinAccumCutLevelToSave()) return true;
  }
  return false;

}

//********************************************************************
bool AnalysisAlgorithm::CheckAccumLevelToSaveRooTracker() {
//********************************************************************
 
  // At least one branch of one of the enabled selections should fulfill the condition
  for (std::vector<SelectionBase*>::iterator it=sel().GetSelections().begin();it!=sel().GetSelections().end();it++){
    if ((*it)->IsEnabled() && (*it)->GetMaxAccumCutLevel() >= GetMinAccumCutLevelToSaveRooTracker()) return true;
  }
  return false;

}


//********************************************************************
void AnalysisAlgorithm::SetAnalysisPoint(enumAnalysisPoint point){
//********************************************************************
 
  // Set the analysis point for this algorithm
  _analysisPoint = point; 

  // Set the analysis point for all used algorithms
  for (std::vector<AnalysisAlgorithm*>::iterator it=_usedAnalyses.begin();it!=_usedAnalyses.end();it++){
    (*it)->SetAnalysisPoint(point);
  }

}


//********************************************************************
void AnalysisAlgorithm::SetMinAccumCutLevelToSave(Int_t level){
//********************************************************************

  // Any of the methods that sets things into used analysis has been called
  _setMethodCalled = true;

  _min_accum_cut_level_roo = _min_accum_cut_level = level;

  // Use the same accum cut level for all used analyses
  for (std::vector<AnalysisAlgorithm*>::iterator it=_usedAnalyses.begin();it!=_usedAnalyses.end();it++){
    (*it)->SetMinAccumCutLevelToSave(level);
  }

}

//********************************************************************
void AnalysisAlgorithm::SetEvent(AnaEventC* event){
//********************************************************************
  
  // Any of the methods that sets things into used analysis has been called
  _setMethodCalled = true;

  // Set the event into this algorithm
  _event = event;

  // Use the same event for all used analyses
  for (std::vector<AnalysisAlgorithm*>::iterator it=_usedAnalyses.begin();it!=_usedAnalyses.end();it++){
    (*it)->SetEvent(_event);
  }

}

//********************************************************************
void AnalysisAlgorithm::SetToyBox(const ToyBoxB* box, Int_t isel){
//********************************************************************
  
  // Any of the methods that sets things into used analysis has been called
  _setMethodCalled = true;

  // set the box into this algorithm
  _box[isel] = box;

  // Use the same event for all used analyses
  for (std::vector<AnalysisAlgorithm*>::iterator it=_usedAnalyses.begin();it!=_usedAnalyses.end();it++){
    (*it)->SetToyBox(box, isel);
  }

}

//********************************************************************
const ToyBoxB& AnalysisAlgorithm::boxB(Int_t isel) const{
//********************************************************************
 
  if (_analysisPoint == kFillTruthTree){
    std::cout << "ERROR. AnalysisAlgorithm::boxB(). The ToyBox cannot be accessed from FillTruthTree method !!!" << std::endl;
    exit(1);
  }

  if (isel==-1){
    if (!_box[_selectedSelection]){
      std::cout << "ERROR. AnalysisAlgorithm::boxB(). No ToyBox exist for selection 0. Probably because no selection has been added to the manager !!!" << std::endl;
      exit(1);
    }
    return *_box[_selectedSelection];
  }
  else
    return *_box[isel];

}

//********************************************************************
void AnalysisAlgorithm::SetSelectedSelection(Int_t sel){
//********************************************************************
 
  // Any of the methods that sets things into used analysis has been called
  _setMethodCalled = true;

  // The selected selection in this analysis
  _selectedSelection = sel;

  // Select the selection for each of the used analysis
  for (std::vector<AnalysisAlgorithm*>::iterator it=_usedAnalyses.begin();it!=_usedAnalyses.end();it++){
    (*it)->SetSelectedSelection(sel);
  }

}


//********************************************************************
void AnalysisAlgorithm::FinalizeToyBase(){
//********************************************************************
 
  // This method Sets the appropriate ToyBox to be used when filling the Micro-trees.
  // If any of the selections is succesfull chose the first found 
  // If not chose the one with higher accum_level

  static bool first = true;

  Int_t maxAccumLevel=-1;
  bool found=false;

  for (std::vector<SelectionBase*>::iterator it=sel().GetSelections().begin();it!=sel().GetSelections().end();it++){
    SelectionBase* selec = *it;
    if (!selec->IsEnabled()) continue;

    Int_t isel = selec->GetEnabledIndex();

    // Get the ToyBox of the previous Toy
    const ToyBoxB& box = selec->GetPreviousToyBox();
    
    if (first){  
      // Set the box into this and all used algorithms. Only the first time since the Box is always the same for a given selection
      SetToyBox(&box,isel);
    }

    if (!found){
      if (box.SuccessfulBranch>-1){
        SetSelectedSelection(isel);
        found=true;
      }
      else if (selec->GetMaxAccumCutLevel()> maxAccumLevel){ 
        SetSelectedSelection(isel);
        maxAccumLevel = selec->GetMaxAccumCutLevel();
      }
    }
    else if (box.SuccessfulBranch>-1){
      std::cout << "AnalysisAlgorithm::FinalizeToyBase(). Selections are not mutually exclusive !!! Run/Subrun/Event/Bunch " 
                << _event->GetEventInfoString()  
                << " passing selection '" << selec->Name() << ", was also successfull in selection '" << sel().GetSelection(_selectedSelection)->Name() << "'." 
                << " Results for first selection being passed will be saved in micro-trees for this event. " << std::endl;        
      if (!_allowNonExclusiveSelections){ 
        std::cout << "Non exclusive selections are in general problematic. THE PROGRAM WILL STOP NOW !!!!" << std::endl;
        std::cout << "Set '_allowNonExclusiveSelections=true' at the begining of highlandTools/vXrY/src/AnalysisAlgorithm.cxx to allow non-exclusive selections" << std::endl;
        exit(1);
      }
    }

  }
  first = false;

}


