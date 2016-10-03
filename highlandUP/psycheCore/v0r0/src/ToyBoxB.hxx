#ifndef ToyBoxB_h
#define ToyBoxB_h

#include "BasicTypes.hxx"
//#include "SubDetId.hxx"

/// This is the container used to pass information from one stem to another in the selection. 
/// It contains internal info needed by the selection as the number of branches and the accum_level of each of 
/// each of the branches. The SoftReset method resets this internal information
/// The box should be extended by the selections to hold specific information needed


class ToyBoxB{
public :

  // Create the Toy Box
  ToyBoxB();
  virtual ~ToyBoxB();
  
  /// Mandatory method to be implemented by the derived class. Reset the top level derived box
  virtual void Reset() = 0;
  
  /// Mandatory method to be implemented by the derived class. Reset the base class for the top level box
  virtual void ResetBase() = 0;

  /// Reset the ToyBoxB
  void SoftReset();
  
public:

  /// The number of branches in the selection this ToyBox belongs to
  UInt_t nBranches;

  /// Accum level for each branch in this toy in the selection this ToyBox belongs to
  Int_t* AccumLevel;

  /// Max Accum level among all branches for this toy in the selection this ToyBox belongs to. TODO: could be computed 
  /// from the previous array but in this way is should be faster
  Int_t MaxAccumLevel;

  /// Array telling us which branches were successful. TODO: If we assume branches are mutually exclusive we dont need this variable, 
  /// all we need is SuccsessfulBranch
  bool* BranchPassed;

  /// The branch that is successful for this toy in the selection this ToyBox belongs to
  Int_t SuccessfulBranch;

  /// The enabled index of this selection this ToyBox belongs to
  Int_t SelectionEnabledIndex;

  /// Indicate the FV we are interested in
  SubDetId_h DetectorFV;

protected:
  bool _ResetCheckDone;
};

#endif
