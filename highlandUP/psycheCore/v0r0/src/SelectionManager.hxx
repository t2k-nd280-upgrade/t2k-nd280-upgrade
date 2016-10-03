#ifndef SelectionManager_h
#define SelectionManager_h

#include "SelectionBase.hxx"


/// This class holds a collection of SelectionBase. It allows us to have multiple selections
///

class SelectionManager: public HLClonesArray{
 public :

  SelectionManager();
  virtual ~SelectionManager(){}

  /// Return the selection that was registered with the given name. NULL if it does not exist
  SelectionBase* GetSelection(const std::string& name, bool print_error=true);

  /// Return the selection that was registered with the given index. NULL if it does not exist
  SelectionBase* GetSelection(Int_t, bool print_error=true);

  /// Add a user selection to the selection manager. 
  void AddSelection(const std::string& name, const std::string& title, SelectionBase* sel, Int_t presel=-1);

  /// Apply the selection that was registered with the given name
  bool ApplySelection(const std::string& name, AnaEventC& event, bool& redo);

  /// Return the map of selections
  std::vector<SelectionBase*>& GetSelections(){return _eventSelections;}

  /// Disable a selection
  void DisableSelection(const std::string& sel);

  /// Enable a selection
  void EnableSelection(const std::string& sel);

  /// Read the "config" tree from the given file, to read the selections that were
  /// used when running the analysis. This is used so we can access the selections in
  /// the DrawingTools.
  void ReadSelections(const std::string& file);

  /// Print out the index, name and title of each selection for a given branch (no argument for all branches)
  void DumpSelections();

  /// Print #events passing cuts
  void PrintStatistics();
  /*  NOT NEEDED ANYMORE, WE SHOULD USE THE METHODS IN SelectionBase 

  /// Copy a clone of the steps in range first-last from branch sbranch1 in selection ssel1 to sbranch2 in selection ssel2
  void CopySteps(const std::string& ssel1, const std::string& sbranch1, UInt_t first, UInt_t last,const std::string& ssel2, const std::string& sbranch2="trunk");

  void CopySteps(const std::string& ssel1, UInt_t first, UInt_t last,const std::string& ssel2, const std::string& sbranch2="trunk"){
    CopySteps(ssel1,"trunk",first,last,ssel2,sbranch2);
  }

  /// Copy a clone of all the steps from branch sbranch1 in selection ssel1 to sbranch2 in selection ssel2
  void CopySteps(const std::string& ssel1, const std::string& sbranch1, const std::string& ssel2, const std::string& sbranch2="trunk");

  void CopySteps(const std::string& ssel1, const std::string& ssel2, const std::string& sbranch2="trunk"){
    CopySteps(ssel1,"trunk",ssel2,sbranch2);
  }


  /// Copy a clone of the steps with number istep from branch sbranch1 in selection ssel1 to sbranch2 in selection ssel2
  void CopyStep(const std::string& ssel1, const std::string& sbranch1, UInt_t istep, const std::string& ssel2, const std::string& sbranch2="trunk");

  void CopyStep(const std::string& ssel1, UInt_t istep, const std::string& ssel2, const std::string& sbranch2="trunk"){
    CopyStep(ssel1,"trunk",istep,ssel2,sbranch2);
  }
  */
  /// Returns the number of enabled selections
  UInt_t GetNEnabledSelections(){return _nEnabledSelections;}

  /// Returns the maximum number of branches in all selections
  UInt_t GetNMaxBranches(){return _nMaxBranches;}

  /// Returns the maximum number of cuts in all selections
  UInt_t GetNMaxCuts(){return _nMaxCuts;}

  /// By default EventSummary class is created and filled when the selection is passed. But this might be necessary also for events not passing the selection
  /// The user can force the EventSummary to be filled always. This function enables/disables this option for all selections in the manager
  void SetForceFillEventSummary(bool force);

  /// Method to set the valid run periods for this selection (e.g. Anti-neutrino selections should only be valid when looking at anti-neutrino data or MC)
  void SetValidRunPeriods(const std::string& ssel1, const std::string validRunPeriods);

  /// Chek if preselection cuts have been passed;
  bool PreSelectionPassed(const AnaEventC& event);

  /// Initialize the EventBox for all enabled selections
  void InitializeEvent(AnaEventC& event);

  /// Create the array of PreviousToyBox for all enabled selections
  void CreateToyBoxArray(Int_t nevents);

  /// Delete the PreviousToyBox pointer for the last toy of the event for all enabled selections
  void FinalizeEvent(const AnaEventC& event);

protected:

  /// Internal storage fo the selections that were added.
  std::vector<SelectionBase*> _eventSelections;

  /// maximum number of branches
  UInt_t _nMaxBranches;   //!

  /// maximum number of cuts 
  UInt_t _nMaxCuts;  //!

  /// number of enabled selections
  UInt_t _nEnabledSelections;  //!

};

#endif
