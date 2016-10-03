#ifndef EventWeightBase_h
#define EventWeightBase_h

#include "SystematicBase.hxx"
#include "ToyExperiment.hxx"
#include "WeightType.hxx"

/// This is the base class that all weight systematics should inherit from. 
/// - Weight: A weight for the event is calculated, which depends on the event properties, but no variation on the event is 
///   performed. Selection does not have to be redone
///
class EventWeightBase: public SystematicBase{
  
public:
    
  /// Create the systematic, with one parameter
  EventWeightBase(){
    _type = kWeight;
  }

  /// Create the systematic, specifying the number of systematic parameters
  EventWeightBase(UInt_t npar):SystematicBase(npar){
    _type = kWeight;
  }
  
  /// Everyone should have a destructor.
  virtual ~EventWeightBase() {}

  /// Initialize the SystBox for this event
  virtual void InitializeEvent(const AnaEventC& event, const SelectionBase& sel, Int_t ibranch);
  
  //----------------------- MANDATORY METHODS -----------------------------------------

  /// This is now the actual method called by SystematicManager, which allows further selection tunning of relevant objects once the ToyBox is filled
  virtual Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel){
    (void)sel; return ComputeWeight(toy, event, box);
  }

  /// This is now the actual method called by SystematicManager, which allows further selection tunning of relevant objects once the ToyBox is filled
  /// This MUST be overridden in the derived class.
  virtual Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box) = 0;
   
protected:

  //------------------------ OPTIONAL METHODS ----------------------------------------

  /// Get the IDs of the RecObject groups that are relevant for this systematic in a given selection
  virtual Int_t GetRelevantRecObjectGroups(const SelectionBase&,     Int_t ibranch, Int_t* IDs) const {(void)IDs;(void)ibranch;return 0;}

  /// Get the IDs of the TrueObject groups that are relevant for this systematic in a given selection
  virtual Int_t GetRelevantTrueObjectGroups(const SelectionBase&, Int_t ibranch, Int_t* IDs) const {(void)IDs;(void)ibranch;return 0;}

  /// Check whether a AnaRecObject is relevant for this systematic or not
  virtual bool IsRelevantRecObject(const AnaEventC&, const AnaRecObjectC&) const {return true;}

  /// Check whether a AnaTrueObject is relevant for this systematic or not
  virtual bool IsRelevantTrueObject(const AnaEventC&, const AnaTrueObjectC&) const {return true;}

  /// Fills the SystBox
  virtual void FillSystBox(const AnaEventC& event, const SelectionBase& sel, Int_t ibranch);

  /// Criteria for true-reco association for this systematic
  virtual bool CheckTrueRecoAssociation(const AnaTrueObjectC& trueRecObject, const AnaRecObjectC& track) const;

  //------------------------ INTERNAL METHODS ----------------------------------------

  /// Fll the true-reco association array
  void FillTrueRecoAssociation(SystBoxB& box);
  
};

#endif
