#ifndef CoreDataClasses_hxx
#define CoreDataClasses_hxx

#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>

#include "TVector3.h"
#include "TLorentzVector.h"
#include "BasicTypes.hxx"

/// Classes for high level analysis. All input formats (oaAnalysis, flatTree, etc)
/// are converted into these data classes, which are later used in the actual analysis.
///

///Array printing

#define dump_array4(par) std::cout<< #par << ": \t" << par[0] << " " << par[1] << " " << par[2] << " " << par[3] << std::endl;  
#define dump_array3(par) std::cout<< #par << ": \t" << par[0] << " " << par[1] << " " << par[2] << std::endl; 



const UInt_t NMAXRECOBJECTGROUPS = 20;
const UInt_t NMAXTRUEOBJECTGROUPS = 20;

const UInt_t NMAXEVENTBOXES = 5;

class AnaTrueObjectC{
 public :

  AnaTrueObjectC();
  virtual ~AnaTrueObjectC();

  /// Clone this object.
  virtual AnaTrueObjectC* Clone() {
    return new AnaTrueObjectC(*this);
  }

  /// Dump the object to screen.
  void Print() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrueObjectC(const AnaTrueObjectC& trueObj);

 public:

  /// The ID of the trueObj, which corresponds to the ID of the TTruthParticle that created it
  Int_t ID;
};

class AnaRecObjectC{
 public :

  AnaRecObjectC();
  virtual ~AnaRecObjectC();

  /// Clone this object.
  virtual AnaRecObjectC* Clone() {
    return new AnaRecObjectC(*this);
  }

  /// Dump the object to screen.
  void Print() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaRecObjectC(const AnaRecObjectC& recObj);

 public:

  /// The UniqueID of this reconstructed object.
  Int_t UniqueID;

  /// The Status of the fit of this reconstructed object.
  Int_t Status;

  /// The link to the true oject that most likely generated this reconstructed object
  AnaTrueObjectC* TrueObject;

  /// Bit field describing the detector that the object is in. 
  /// See SubDetId.hxx for the detector enumerations used to 
  /// interrogate this object
  unsigned long Detector;
};

class AnaEventSummaryC{
 public :

  AnaEventSummaryC(){}
  virtual ~AnaEventSummaryC(){}


  /*
  AnaEventSummaryC() {
      for(int i = 0; i < SampleId::kNSamples; ++i){
          LeptonCandidate[i]=NULL;
          RooVertexIndex[i] = -1;
          TrueVertex[i] = NULL;
          for(int j = 0; j < 4; ++j){
              VertexPosition[i][j] = -9999;
          }
      }
      EventSample = SampleId::kUnassigned;
  }
  virtual ~AnaEventSummaryC(){}

  void ResetSummary(){
      for(int i = 0; i < SampleId::kNSamples; ++i){
          LeptonCandidate[i]=NULL;
          RooVertexIndex[i] = -1;
          TrueVertex[i] = NULL;
          for(int j = 0; j < 4; ++j){
              VertexPosition[i][j] = -9999;
          }
      }
      EventSample = SampleId::kUnassigned;
  }

  /// A pointer to the Lepton Candidate
  AnaParticleB* LeptonCandidate[SampleId::kNSamples];

  /// A pointer to the event's true vertex
  AnaTrueVertexB* TrueVertex[SampleId::kNSamples];

  /// Event recon vertex position
  float VertexPosition[SampleId::kNSamples][4];

  /// Event sample - whether it passes the NuMu-CCQE selection, or CC1Pi selection for example
  SampleId::SampleEnum EventSample;

  /// The link to the RooTrackerVtx tree
  UInt_t RooVertexIndex[SampleId::kNSamples];
  */
};


class EventBoxB{
 public :

  EventBoxB();
  virtual ~EventBoxB();

  ///---------- RecObjects and TrueRecObjects used in the selection and systematics ------------------

  /// Different groups of rec objects used for selection and systematics
  Int_t nRecObjectsInGroup[NMAXRECOBJECTGROUPS];
  AnaRecObjectC** RecObjectsInGroup[NMAXRECOBJECTGROUPS];

  /// Different groups of rec objects used for systematics
  Int_t nTrueObjectsInGroup[NMAXTRUEOBJECTGROUPS];
  AnaTrueObjectC** TrueObjectsInGroup[NMAXTRUEOBJECTGROUPS];
};

class AnaBunchC{
 public :

  AnaBunchC();
  virtual ~AnaBunchC();

  /// Clone this object.
  virtual AnaBunchC* Clone() {
    return new AnaBunchC(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaBunchC(const AnaBunchC& bunch);

 public:

  /// The index of this bunch (0-7).
  Int_t Bunch;

  /// The weight to apply to this bunch (nominally 1). An example is the beam flux weight
  Float_t Weight;
};


class AnaSpillC{
 public :

  AnaSpillC();
  virtual ~AnaSpillC();

  /// Clone this object.
  virtual AnaSpillC* Clone() = 0;

  /// Dump the object to screen.
  virtual void Print() const;

  /// Return whether this spill is from Monte Carlo or not.
  virtual bool GetIsMC() const = 0;

  /// Copy the arrays into std vectors, needed for FlatTree creation
  virtual void CopyArraysIntoVectors() = 0;

  /// Copy the vectore into the arrays, needed after FlatTree reading
  virtual void CopyVectorsIntoArrays() = 0;

  /// Redo reco-reco and reco-truth after cloning or reading MiniTree
  virtual void RedoLinks() = 0;


protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaSpillC(const AnaSpillC&);

public:
  /// Is this the original Event or a clone
  bool isClone;

  /// The reconstructed objects, split into timing bunches.
  std::vector<AnaBunchC*> Bunches;

};

class AnaEventC{
 public :

  AnaEventC();
  virtual ~AnaEventC();

  /// Clone this object.
  virtual AnaEventC* Clone() = 0;

  /// Return whether this event is from Monte Carlo or not.
  virtual bool GetIsMC() const = 0;

  /// Copy the entire object
  virtual void Copy(const AnaEventC& event, bool copyBunchInfo = true, bool cloneTruth=true);

  /// Dump the object to screen.
  virtual void Print() const;

  /// User-frienly method to get event info (run,subrun,etc)
  virtual std::string GetEventInfoString() const {return "";}

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaEventC(const AnaEventC& event);

 public:

  /// The event unique ID
  Int_t UniqueID;
  
  /// Is this the original Event or a clone
  bool isClone;

  /// Run, subrun, event, time  stamp, etc
  //  AnaEventInfoB EventInfo;

  /// The ND280 data quality flags for this event.
  //  AnaDataQualityB* DataQuality;

  /// The weight to apply to this event (nominally 1). An example is the beam flux weight
  Float_t Weight;

  //---------------------------------------------------------

  /// A summary of the event with high level quantities
  AnaEventSummaryC* Summary;

  /// The array of boxes where objects that are valid for all toys are stored
  Int_t nEventBoxes;
  EventBoxB* EventBoxes[NMAXEVENTBOXES];
};

class AnaSuperEventB{
 public :

  AnaSuperEventB();
  AnaSuperEventB(AnaEventC* event);
  virtual ~AnaSuperEventB();

  /// Clone this object.
  virtual AnaSuperEventB* Clone() {
    AnaSuperEventB* event = new AnaSuperEventB(*this);
    event->isClone=true;
    return event;
  }

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaSuperEventB(const AnaSuperEventB& event);

 public:

  /// Is this the original Event or a clone
  bool isClone;
  
  /// The Raw event
  AnaEventC* RawEvent;

  /// The modified event
  AnaEventC* Event;

  /// The data/MC POT ratio
  Float_t POTWeight;

};


#endif
