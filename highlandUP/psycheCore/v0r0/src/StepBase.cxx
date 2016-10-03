#include "StepBase.hxx"

ClassImp(StepBase)

//****************************************************
StepBase::StepBase(){
//****************************************************

  _title="";
  _type=kUnknown;
  _break=false;
  _nextSteps.clear();
  _branchUniqueIDs.clear();
  _disabledInBranch.clear();
}  

//****************************************************
void StepBase::ValidateStepRecursive() const{
//****************************************************

  if (GetBranchUniqueIDs().size()==0){
    std::cout << "\nERROR. StepBase::ValidateStepRecursive(). Step '" << _title 
              << "' does not belong to any branch. You must use SetBranchAlias after adding the steps in your selection" << std::endl;
    exit(1);
  }  

  for (UInt_t i = 0; i < GetNextSteps().size(); ++i){
    StepBase* step2=GetNextSteps()[i];
    if (step2) step2->ValidateStepRecursive();
  }

}

//****************************************************
void StepBase::AddBranchUniqueID(UInt_t branch){
//****************************************************

  _branchUniqueIDs.push_back(branch);
  if (_disabledInBranch.size() < branch+1){ 
    _disabledInBranch.resize(branch+1);
    for (UInt_t i=0;i<_disabledInBranch.size();i++)
      _disabledInBranch[i]=false;
  }
}

//****************************************************
void StepBase::RemoveBranchUniqueID(UInt_t branch){
//****************************************************

  for (std::vector<UInt_t>::iterator it =_branchUniqueIDs.begin(); it!=_branchUniqueIDs.end();it++){
    if (*it == branch){
      _branchUniqueIDs.erase(it);
      break;
    }
  }
}

//***********************************************************
std::string StepBase::ConvertType() const{
//***********************************************************

  if (_type == kCut)
    return "cut";
  else if (_type == kAction)
    return "action";
  else
    return "unknown";
}

//********************************************************************
void StepBase::AddNextStep(StepBase* step, Int_t branch){
//********************************************************************
  
  if (branch==-1 || _nextSteps.size()==0)    
    _nextSteps.push_back(step);
  else{
    if (branch>=0 && branch < (Int_t)_nextSteps.size()){
      _nextSteps[branch] = step;
    }
    else
      std::cout << "StepBase::AddNextStep(). branch " << branch << " not defined for step '" << _title << "'" << std::endl;
  }
}

//********************************************************************
void StepBase::RemoveNextStep(const std::string& title, Int_t branch){
//********************************************************************

  if (branch<0 || branch >= (Int_t)_nextSteps.size()){
    std::cout << "ERROR. StepBase::AddNextStep(). branch " << branch << " not defined for step '" << _title << "'" << std::endl;
    exit(1);
  }

  // This method removes a step with a given title (and branch ID) from the vector of next steps
  if (_nextSteps[branch]->Title() != title){
    std::cout << "ERROR. StepBase::AddNextStep(). Step '" << _title << "' does not have a child step '"  << title << "' in branch " << branch << std::endl;
    exit(1);    
  }

  // if there is only one next step, the vector is emptied, otherwise the corresponding entry in the vector is set to NULL
  // in order to keep the number of branches in the split
  if (_nextSteps.size()==1)
    _nextSteps.clear();
  else
    _nextSteps[branch]=NULL;
}

//********************************************************************
void StepBase::GetNextSteps(std::vector<StepBase*>& steps, bool withSplits){
//********************************************************************

  // Get all Steps with or without splits
  if (!withSplits && _nextSteps.size() != 1) return;

  for (std::vector<StepBase*>::iterator it=_nextSteps.begin();it!=_nextSteps.end();it++){
    // Add The next step
    steps.push_back(*it);
    
    // Go recursively to add the childs of the next step
    (*it)->GetNextSteps(steps,withSplits);
  }
}

//********************************************************************
StepBase::BranchStatus StepBase::GetNextStepsInBranch(std::vector<StepBase*>& steps, int branch){
//********************************************************************

  // This function adds all subsequent steps to the steps vector at a given split, for the specific branch  at this split. 

  // Make sure the requested branch is OK
  if ((branch<0 || branch>=(Int_t)_nextSteps.size()) && branch!=0 && _nextSteps.size() !=0){
    std::cout << "StepBase::GetNextSteps(branch): Branch index " << branch << " does not exist in Step '" << _title << "'" << std::endl;
    return BranchOutOfRange;
  }
  
  // No more steps to add when nextSteps has 0 elements
  if (_nextSteps.size()==0) return NoBranches;

  // TODO: not sure it is needed. Just a protection
  if (!_nextSteps[branch]) return BranchEmpty;

  // Add The next step
  steps.push_back(_nextSteps[branch]);

  // Go recursively to add the doughters of the next step
  _nextSteps[branch]->GetNextSteps(steps);

  return BranchOK;
}

