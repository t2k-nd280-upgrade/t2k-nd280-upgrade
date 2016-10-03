#ifndef StepBase_h
#define StepBase_h

#include <TObject.h>
#include <stdio.h>
#include <iostream>
#include <map>

#include "ToyBoxB.hxx"
#include "CoreDataClasses.hxx"

/// Base class for an event selection step. All user steps should inherit from
/// this class, and implement the Apply() function. The type and title are
/// set when the step is added to the step manager using StepManager::AddStep().
/// Steps can be of type Cut or Action. 
/// Cuts would have to return a meaninful boolean while for actions the boolean will be ignored

class StepBase: public TObject{
 public :

  /// Enumerator describing the values that _type member can take.
  enum TypeEnum {
    kCut=0,
    kAction,
    kUnknown
  }; //!

  enum BranchStatus{
    BranchOK=0,
    BranchEmpty,
    NoBranches,
    BranchOutOfRange       
  }; //!

  StepBase();
  virtual ~StepBase(){}

  //-----------------------------------------------------
  /// MANDATORY FUNCTIONS !!! 

  // Clones this step. Keeps only Type, Title and Break
  virtual StepBase* MakeClone(){std::cout << "ERROR: StepBase::MakeClone() must be implemented in your Cut!!!" << std::endl;exit(1);return NULL;}

  /// The return value should specify whether the step was passed.
  /// For actions the return value is ignored
  virtual bool Apply(AnaEventC& event, ToyBoxB& box) const {(void)event;(void)box;return true;std::cout << "ERROR: StepBase::Apply() must be implemented in your Cut!!!" << std::endl;exit(1);}
  //------------------------------------------------------

  /// Set the title of this step, which is the "nice" version of the step name,
  /// used when plotting the step.
  void SetTitle(const std::string& title){_title=title;}

  /// Set the type of this step.
  void SetType(TypeEnum type) {_type = type;}

  /// Set whether the cut sequence should be stopped when this cut fails
  void SetBreak(bool b){_break=b;}

  /// Set the index of this step.
  void SetIndex(Int_t index) {_index = index;}

  /// Return the title of this step.
  const std::string& Title() const{return _title;}

  /// Return the type of this step.
  TypeEnum Type() const{return _type;}

  /// Return true if the cut sequence should be stopped when a cut fails
  bool Break() const{return _break;}

  /// Return the index of this step
  Int_t Index() const{return _index;}

  /// Convert into a string the type of this step
  std::string ConvertType() const;

  /// Disable step in a given branch
  void DisableInBranch(Int_t branch) {_disabledInBranch[branch] = true;}

  /// Enable step in a given branch
  void EnableInBranch(Int_t branch) {_disabledInBranch[branch] = false;}

  /// Returns true if the step is disabled in the specified branch
  bool IsDisabledInBranch(Int_t branch) const {return _disabledInBranch[branch];}

  /// Returns the vector of next steps
  const std::vector<StepBase*>& GetNextSteps() const{return _nextSteps;}

  /// Remove step with a given title (and branch ID) from next steps
  void RemoveNextStep(const std::string& title, Int_t ID=0);

  /// Add a branch unique ID to this step
  /// Although steps are unique and can be shared by different branches we need to store
  /// the branches in which this step is used
  void AddBranchUniqueID(UInt_t ibranch);

  /// Remove a branch unique ID from this step
  void RemoveBranchUniqueID(UInt_t branch);

  /// Returns the vector of branche unique IDs associated to this step
  const std::vector<UInt_t>& GetBranchUniqueIDs() const {return _branchUniqueIDs;}

  /// Add a next step
  /// If now second argument is specified this step will have a unique next step (no split)
  /// If this step contains a split we need to specify the branch to which the next step is added
  void AddNextStep(StepBase* step, Int_t branch=-1);

  /// Get the vector of next steps
  void GetNextSteps(std::vector<StepBase*>& steps, bool withSplits=false);

  /// Get the vector of next steps in a given branch, returns a status code
  BranchStatus GetNextStepsInBranch(std::vector<StepBase*>& steps, Int_t branch);

  /// check that the step and all its suns are correct (it was branch unique IDs defined)
  void ValidateStepRecursive() const;

  ClassDef(StepBase, 1);

protected:

  /// The type of this step: cut or action
  TypeEnum _type;

  /// true if the cut sequence should be stopped when a cut fails
  bool _break;

  /// The title of this step, which is used by the DrawingTools.
  std::string _title;

  /// Is the step disabled in a given branch
  std::vector<bool> _disabledInBranch;

  /// The vector of next steps
  std::vector<StepBase*> _nextSteps;

  /// Branch unique IDs associated to this step
  std::vector<UInt_t> _branchUniqueIDs;

  /// Index of the step in the selection
  Int_t _index; //!
};

#endif
