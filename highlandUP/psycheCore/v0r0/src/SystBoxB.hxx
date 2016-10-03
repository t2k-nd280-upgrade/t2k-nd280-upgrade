#ifndef SystBoxB_h
#define SystBoxB_h

#include "CoreDataClasses.hxx"
#include <vector>
#include <string>
#include <sstream>
#include <set>

class SystBoxB{
 public :

  SystBoxB();

  virtual ~SystBoxB();

  ///---------- Relevant rec objects and true objects for each systematic ------------------

  /// Array of Relevant RecObjects for each systematic
  Int_t nRelevantRecObjects;
  AnaRecObjectC** RelevantRecObjects;

  /// Array of Relevant True RecObjects for each systematic
  Int_t nRelevantTrueObjects;
  AnaTrueObjectC** RelevantTrueObjects;

  /// Array of Relevant True RecObjects with Reconstructed association for each systematic
  /// With size nRelevantTrueObjects
  AnaRecObjectC** RelevantTrueObjectsReco;

  /// The Set of relevant tracks for variation systematics.   
  /// A set is use such that we ensure no repetition, since each variation systematic adds its own relevant objects
  std::set<AnaRecObjectC*> RelevantRecObjectsSet;
};


#endif
