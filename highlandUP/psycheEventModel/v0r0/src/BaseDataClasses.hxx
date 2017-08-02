#ifndef BaseDataClasses_hxx
#define BaseDataClasses_hxx
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>

#include "TString.h"
#include "TVector3.h"
#include "TLorentzVector.h"


#include "CoreDataClasses.hxx"
#include "SampleId.hxx"
#include "SubDetId.hxx"


/// Classes for high level analysis. All input formats (oaAnalysis, flatTree, etc)
/// are converted into these data classes, which are later used in the actual analysis.
///

///Array printing

#define dump_array4(par) std::cout<< #par << ": \t" << par[0] << " " << par[1] << " " << par[2] << " " << par[3] << std::endl;  
#define dump_array3(par) std::cout<< #par << ": \t" << par[0] << " " << par[1] << " " << par[2] << std::endl; 


/// The number of subdetectors (not including the "Tracker" detector)
const UInt_t NDETECTORS=19;

/// The maximum number of objects to be saved in the FlatTree
const UInt_t NMAXPARTICLES = 10000;
const UInt_t NMAXTRUEPARTICLES = 10000;
const UInt_t NMAXTRUEVERTICES = 50; // per spill
const UInt_t NMAXVERTICES = 20;
const UInt_t NMAXCROSSEDDET = 30;

const UInt_t NMAXTARGET  = 2;
const UInt_t NMAXFGD  = 2;
const UInt_t NMAXTOF  = 10;
const UInt_t NMAXTPC  = 7;
const UInt_t NMAXECAL  = 4;

const UInt_t NMAXTRUEVERTICESMATCH = 3;
const UInt_t NMAXPARTICLESINVERTEX = 7;

// forward declarations
class AnaTrueParticleB;


/// Representation of a detector crossing info for a true particle (G4 trajectory).
class AnaDetCrossingB{
 public :

  AnaDetCrossingB();
  virtual ~AnaDetCrossingB();

  /// Clone this object.
  virtual AnaDetCrossingB* Clone() {
    return new AnaDetCrossingB(*this);
  }
  void Print() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaDetCrossingB(const AnaDetCrossingB& cross);

 public:  

  /// for each subdetector tell the entrance position
  Float_t EntrancePosition[4];
  
  /// for each subdetector tell the exit position
  Float_t ExitPosition[4];

  /// for each subdetector tell the entrance momentum
  Float_t EntranceMomentum[3];

  /// for each subdetector tell the exit momentum
  Float_t ExitMomentum[3];
  
  /// If the particle passes through an active part of the subdetector
  bool InActive;

  /// Bit field describing the detector that the object is in. 
  /// See SubDetId.hxx for the detector enumerations used to 
  /// interrogate this object
  unsigned long Detector;
  TString Detector_name; 
  
  /// The number of nodes in the reconstructed object.
  Float_t SegLength;

  Float_t DeltaLYZ;

  Float_t EDeposit;


};

/// Representation of a true Monte Carlo vertex.
class AnaTrueVertexB: public AnaTrueObjectC{
 public :

  AnaTrueVertexB();
  virtual ~AnaTrueVertexB();

  /// Clone this object.
  virtual AnaTrueVertexB* Clone() {
    return new AnaTrueVertexB(*this);
  }

  /// Dump the object to screen.
  void Print() const;
 
 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrueVertexB(const AnaTrueVertexB& vertex);

 public:
  
  /// The PDG code of the incoming neutrino.
  Int_t NuPDG;
  Int_t TargetPDG;

  /// The true energy of the incoming neutrino.
  Float_t NuEnergy;

  /// The true energy of the incoming neutrino.
  Float_t NuMom;

  /// The position the true interaction happened at.
  Float_t Position[4];

  /// The bunch in which this true interaction occurred, based on the time of
  /// the interaction and the bunching used for the reconstructed objects.
  Int_t Bunch;

  /// The true particles associated with this vertex.
  AnaTrueParticleB** TrueParticles; //!
  /// How many true particles are associated with this vertex
  int nTrueParticles; //!

  /// A copy of TrueParticles but using an std::vector, which can be saved into a tree
  std::vector<AnaTrueParticleB*> TrueParticlesVect;

  /// Bit field describing the detector that the object is in. 
  /// See SubDetId.hxx for the detector enumerations used to 
  /// interrogate this object
  unsigned long Detector;

};

/// Representation of a true Monte Carlo trajectory/particle.
class AnaTrueParticleB: public AnaTrueObjectC{
 public :

  AnaTrueParticleB();
  virtual ~AnaTrueParticleB();

  /// Clone this object.
  virtual AnaTrueParticleB* Clone() {
    return new AnaTrueParticleB(*this);
  }

  /// Dump the object to screen.
  void Print() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrueParticleB(const AnaTrueParticleB& truePart);

 public:

  /// The PDG code of this particle.
  Int_t PDG;

  /// The ID of this particle's immediate parent, or 0 if there is no parent.
  Int_t ParentID;

  /// The ID of this particle's grandparent, or 0 if there is no grandparent.
  Int_t GParentID;

  /// The PDG code of this particle's immediate parent, or 0 if there is no parent.
  Int_t ParentPDG;

  /// The PDG code of this particle's grandparent, or 0 if there is no grandparent.
  Int_t GParentPDG;

  /// The initial position of the true particle.
  Float_t Position[4];

  /// The end position of the true particle.
  Float_t PositionEnd[4];

  /// The initial direction of the true particle.
  Float_t Direction[3];

  /// The initial momentum of the true particle.
  Float_t Momentum;
  int stopped;

  /// The true charge of the particle.
  Float_t Charge;

  Float_t EKin;
  /// The TruthVertexID of the AnaTrueVertexB of the interaction that created this AnaTrueParticleB.
  Int_t VertexID;

  Float_t Length;

  Float_t EDeposit;

  Float_t CosTheta;
  /// Pointer to the AnaTrueVertexB of the interaction that created this AnaTrueParticleB.
  AnaTrueVertexB* TrueVertex; 

  /// Vector of detector crossings. These are in the order in which the
  /// detectors were crossed.
  AnaDetCrossingB** DetCrossings; //!
  /// The number of DetCrossing objects
  int nDetCrossings; //!

  /// A copy of DetCrossings but using an std::vector, which can be saved into a tree
  std::vector<AnaDetCrossingB*> DetCrossingsVect;
};

/// Representation of a reconstructed particle (track or shower).
class AnaParticleB: public AnaRecObjectC{
 public :

  AnaParticleB();
  virtual ~AnaParticleB();

  /// Clone this object.
  virtual AnaParticleB* Clone() {
    return new AnaParticleB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Compare the NHits of two particles. Return whether t1 is higher NHts than t2.
  static bool CompareNHits(const AnaParticleB* t1, const AnaParticleB* t2);

  /// Return a casted version of the AnaTrueObject associated
  AnaTrueParticleB* GetTrueParticle() const {return static_cast<AnaTrueParticleB*>(TrueObject);}

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaParticleB(const AnaParticleB& part);

 public:

  Int_t NHits;
  
  /// The reconstructed start direction of the particle.
  Float_t DirectionStart[3];
  /// The reconstructed end direction of the particle.
  Float_t DirectionEnd[3];
  
  /// The reconstructed start position of the particle.
  Float_t PositionStart[4];
  /// The reconstructed end position of the particle.
  Float_t PositionEnd[4];

  Float_t EDeposit;
  Float_t CosTheta;
  unsigned long Detectors;
  
  /// Objects can be manipulated by input variations (which apply corrections
  /// and systematics). There are three copies of each spill and this member
  /// provides links between the different versions of this object. The link
  /// goes as Spill->CorrectedSpill or CorrectedSpill->RawSpill. If this object
  /// is in the RawSpill, then this member is NULL.
  const AnaParticleB* Original; //!
};


/// Extension to AnaParticleB containing momentum and charge info, to be used by aggregation
class AnaParticleMomE{
 public :

  AnaParticleMomE();
  virtual ~AnaParticleMomE();

  /// Dump the object to screen.
  virtual void Print() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaParticleMomE(const AnaParticleMomE& part);

 public:
    /// Error of the momentum at the start of the segment.
  Float_t MomentumError;

  Float_t SmearedMomentum;
  /// The reconstructed charge of the particle.
  Float_t Charge;

  /// The reconstructed momentum of the particle, at the start position.
  Float_t Momentum;  

  /// The reconstructed momentum of the particle, at the end position.
  Float_t MomentumEnd;

};


/// /// Extension to AnaParticleB containing momentum and charge info
class AnaParticleMomB: public AnaParticleB, public AnaParticleMomE{
 public :

  AnaParticleMomB();
  virtual ~AnaParticleMomB();

  /// Clone this object.
  virtual AnaParticleMomB* Clone() {
    return new AnaParticleMomB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Function used to sort momentum in decreasing order
  static bool CompareMomentum(const AnaParticleMomB* t1, const AnaParticleMomB* t2);

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaParticleMomB(const AnaParticleMomB& part);
};

/// Representation of a Subdet1 segment of a global track.
class AnaTargetParticleB:public AnaParticleB {
 public :

  AnaTargetParticleB();
  virtual ~AnaTargetParticleB();

  /// Clone this object.
  virtual AnaTargetParticleB* Clone() {
    return new AnaTargetParticleB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

 protected:
  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTargetParticleB(const AnaTargetParticleB& seg);

 public:
  Float_t DeltaLYZ;
  Float_t SegLength;
  Bool_t IsReconstructed;

};

/// Representation of a Subdet1 segment of a global track.
class AnaFGDParticleB:public AnaParticleB {
 public :

  AnaFGDParticleB();
  virtual ~AnaFGDParticleB();

  /// Clone this object.
  virtual AnaFGDParticleB* Clone() {
    return new AnaFGDParticleB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

 protected:
  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaFGDParticleB(const AnaFGDParticleB& seg);

 public:
  Float_t DeltaLYZ;
  Float_t SegLength;
  Bool_t IsReconstructed;

};

/// Representation of a Subdet1 segment of a global track.
class AnaToFParticleB:public AnaParticleB {
 public :

  AnaToFParticleB();
  virtual ~AnaToFParticleB();

  /// Clone this object.
  virtual AnaToFParticleB* Clone() {
    return new AnaToFParticleB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

 protected:
  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaToFParticleB(const AnaToFParticleB& seg);

 public:
  Float_t DeltaLYZ;
  Float_t SegLength;
  Bool_t IsReconstructed;
  TString Detector_name;

};

/// Representation of a Subdet1 segment of a global track.
class AnaECalParticleB:public AnaParticleB {
 public :

  AnaECalParticleB();
  virtual ~AnaECalParticleB();

  /// Clone this object.
  virtual AnaECalParticleB* Clone() {
    return new AnaECalParticleB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

 protected:
  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaECalParticleB(const AnaECalParticleB& seg);

 public:
  Float_t DeltaLYZ;
  Float_t SegLength;
  Bool_t IsReconstructed;

};


/// Representation of a Subdet2 segment of a global track.
class AnaTPCParticleB:public AnaParticleMomB {
 public :

  AnaTPCParticleB();
  virtual ~AnaTPCParticleB();

  /// Clone this object.
  virtual AnaTPCParticleB* Clone() {
    return new AnaTPCParticleB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

 protected:
  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTPCParticleB(const AnaTPCParticleB& seg);

 public:



  /// Reconstructed momentum with the E-field distortion corrections
  Float_t EFieldRefitMomentum;

  /// dE/dx as measured by the Subdet2.
  Float_t dEdxMeas;

  /// Expected dE/dx for a muon, based on the reconstructed momentum.
  Float_t dEdxexpMuon; 

  /// Expected dE/dx for an electron, based on the reconstructed momentum.
  Float_t dEdxexpEle; 

  /// Expected dE/dx for a pion, based on the reconstructed momentum.
  Float_t dEdxexpPion; 

  /// Expected dE/dx for a proton, based on the reconstructed momentum.
  Float_t dEdxexpProton; 

  /// Expected error on the dE/dx measurement, for the muon hypothesis.
  Float_t dEdxSigmaMuon; 

  /// Expected error on the dE/dx measurement, for the electron hypothesis.
  Float_t dEdxSigmaEle; 

  /// Expected error on the dE/dx measurement, for the pion hypothesis.
  Float_t dEdxSigmaPion; 

  /// Expected error on the dE/dx measurement, for the proton hypothesis.
  Float_t dEdxSigmaProton; 

  Float_t DeltaLYZ;

  Float_t SegLength;
};

class AnaRecTrueMatchB{
 public :

  AnaRecTrueMatchB(){}
  virtual ~AnaRecTrueMatchB(){}

  /// Clone this object.
  virtual AnaRecTrueMatchB* Clone() {
    return new AnaRecTrueMatchB(*this);
  }

 public:
  
  /// The cleanliness of the true-reco matching.
  Float_t Cleanliness;

  /// The completeness of the true-reco matching.
  Float_t Completeness;
};

/// Representation of a global track.
class AnaTrackB: public AnaParticleMomB{
 public :

  AnaTrackB();
  virtual ~AnaTrackB();

  /// Clone this object.
  virtual AnaTrackB* Clone() {
    return new AnaTrackB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Return a casted version of the original AnaParticleB associated
  const AnaTrackB* GetOriginalTrack() const {return static_cast<const AnaTrackB*>(Original);}

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrackB(const AnaTrackB& track);

 public:

  /// The index of this particle track in the vector of particles. TODO: Not sure it is needed (only use in PID utils)
  Int_t Index;


  /// Variables in order to be able to flip a particle based on ToF info
  /// Contains kinematics parameters which results of the particle fit 
  /// with a reverse sense hypothesis

  /// Momentum for the main PID hypothesis and reverse sense
  Float_t MomentumFlip;

  /// Direction at the start for the main PID hypothesis and reverse sense
  Float_t DirectionStartFlip[3];

  /// Whether track passes Subdet2 quality cut.
  Int_t TPCQualityCut;

  AnaTargetParticleB* TargetSegments[NMAXTARGET]; //!
  int nTargetSegments; //!

  AnaFGDParticleB* FGDSegments[NMAXTARGET]; //!
  int nFGDSegments; //!

  AnaToFParticleB* ToFSegments[NMAXTOF]; //!
  int nToFSegments; //!

  Float_t EKin;
  AnaTPCParticleB* TPCSegments[NMAXTPC]; //!
  int nTPCSegments; //!

  AnaECalParticleB* ECalSegments[NMAXECAL]; //!
  int nECalSegments; //!

  /// A copy of the segment arrays but using an std::vector, which can be saved into a tree
  std::vector<AnaTargetParticleB*> TargetSegmentsVect; 
  std::vector<AnaFGDParticleB*>    FGDSegmentsVect; 
  std::vector<AnaToFParticleB*>    ToFSegmentsVect; 
  std::vector<AnaTPCParticleB*>    TPCSegmentsVect; 
  std::vector<AnaECalParticleB*>   ECalSegmentsVect; 

  bool HasFlipped;

  /// ToF related information
  Float_t ToF_reco_time, ToF_true_time, ToF_sigma_time;
  SubDetId::SubDetEnum ToF_firstDet, ToF_secondDet;
  Float_t ToF_reco_length, ToF_true_length, ToF_sigma_length;
  Float_t ToF_reco_mass, ToF_sigma_mass;

};

/// Representation of a global vertex.
class AnaVertexB: public AnaRecObjectC{
 public :

  AnaVertexB();
  virtual ~AnaVertexB();

  /// Clone this object.
  virtual AnaVertexB* Clone() {
    return new AnaVertexB(*this);
  }

  /// Dump the object to screen.
  void Print() const;

  /// Function used to sort PrimaryIndex in increasing order
  static bool ComparePrimaryIndex(const AnaVertexB* t1, const AnaVertexB* t2);

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaVertexB(const AnaVertexB& vertex);

 public:
  
  /// The bunch of the global vertex, based on the Position.T()
  Int_t Bunch;

  /// Index of the global vertex.
  Int_t PrimaryIndex;

  /// The identified position of the global vertex.
  Float_t Position[4];

  /// The particles that are associated to this global vertex.
  /// It would be better to rename this into ReconParticles
  AnaParticleB** Particles; //!
  int nParticles; //!

  /// A copy of Particles but using an std::vector, which can be saved into a tree
  std::vector<AnaParticleB*> ParticlesVect;

  /// The true vertex that is associated with this global vertex.
  /// vertex->GetMainTrueVertex() could replace this info.
  AnaTrueVertexB* TrueVertex;


  /// Objects can be manipulated by corrections
  /// There are three copies of each spill and this member
  /// provides links between the different versions of this object. The link
  /// goes as Spill->CorrectedSpill or CorrectedSpill->RawSpill. If this object
  /// is in the RawSpill, then this member is NULL.
  const AnaVertexB* Original; //!

 public:

  /// The true vertices that is associated with this global vertex.
///  int GetTrueVertices(AnaTrueVertexB** arr);

};

/// Representation of the beam quality and perhaps other beam information as needed.
class AnaBeamB{
 public :

  AnaBeamB();
  virtual ~AnaBeamB(){}

  /// Clone this object.
  virtual AnaBeamB* Clone() {
    return new AnaBeamB(*this);
  }

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaBeamB(const AnaBeamB& beam);

 public:
  
  /// Good spill flag, as defined in Beam Summary Data. 0 is bad.
  Int_t GoodSpill;

  /// The POT since the previously saved spill. For data, this comes from the Beam Summary Data 
  /// Is the single Spill POT for oaAnalysis input and accumulated for MiniTree input
  Double_t POTSincePreviousSavedSpill;

  /// The number of spills since the previously saved spill
  /// Is the 1 for oaAnalysis input and accumulated for MiniTree input
  Int_t SpillsSincePreviousSavedSpill;
};


class AnaDelayedClustersB{
public :

  AnaDelayedClustersB();
  virtual ~AnaDelayedClustersB(){}

  /// Clone this object.
  virtual AnaDelayedClustersB* Clone() {
    return new AnaDelayedClustersB(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaDelayedClustersB(const AnaDelayedClustersB& clusters);

public:

  Float_t      MinTime;       //  Minimum time of a hit in the cluster
  Float_t      MaxTime;       //  Maximum time of a hit in the cluster
  Int_t        NHits;         //  Number of hits in the cluster
  Float_t      RawChargeSum;  //  Sum of the RawCharge of all hits
};

/// An AnaBunch contains all the reconstructed objects in a given time window.
/// The normal bunches are indexed from 0-7.
///
/// The bunch windows for each run period are defined in the text file
/// $HIGHLEVELANALYSISIOROOT/data/BunchPosition.dat, where the run period of -1
/// means Monte Carlo.
///
/// Each bunch has a weighting associated with it, to allow for flux
/// re-weighting.
class AnaBunchB: public AnaBunchC{
 public :

  AnaBunchB();
  virtual ~AnaBunchB();

  /// Clone this object.
  virtual AnaBunchB* Clone() {
    return new AnaBunchB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// The primary reconstructed vertex associated with this bunch.
  AnaVertexB* GetPrimaryVertex();

  /// Returns true if the bunch is completely empty
  virtual bool IsEmpty() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaBunchB(const AnaBunchB& bunch);

 public:

  /// The reconstructed vertices in this bunch after deleting CohOnH, PauliBlocked and fully contained events
  /// See https://bugzilla.nd280.org/show_bug.cgi?id=1174
  std::vector<AnaVertexB*> Vertices; 

  /// The reconstructed particles in this bunch after deleting CohOnH, PauliBlocked and fully contained events
  /// See https://bugzilla.nd280.org/show_bug.cgi?id=1174
  std::vector<AnaParticleB*> Particles;

  /// The delayed cluster groups
  AnaDelayedClustersB** DelayedClusters; //! 
  Int_t nDelayedClusters; //! 
};

class AnaEventInfoB{
 public :

  AnaEventInfoB();
  virtual ~AnaEventInfoB();

  /// Clone this object.
  virtual AnaEventInfoB* Clone() {
   return new AnaEventInfoB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Return whether this spill is from Monte Carlo or not.
  bool GetIsMC() const {return IsMC;}

  /// Return whether this spill is from Sand Monte Carlo or not.
  bool GetIsSandMC() const {return IsSand;}

  /// Set whether this event is from Sand MC
  void SetIsSandMC();

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaEventInfoB(const AnaEventInfoB& event);

 public:

  /// The ND280 run number.
  Int_t Run;

  /// The subrun number.
  Int_t SubRun;

  /// The ND280 event number.
  Int_t Event;
  
  /// Says if the event is MC or data
  bool IsMC;

  /// Is this event from the Sand MC
  bool IsSand;
};


/// The top-level object for representing information from the input files.
/// A single AnaSpillB contains information from an entire ND280 event.
/// The reconstructed objects are separated into bunches based on their time
/// information.
///
/// There are multiple copies of each spill available through the InputManager
/// [ND::input()], showing the raw state of the spill after reading it in from
/// the input files; after applying corrections; and after applying corrections
/// and systematics.
class AnaSpillB: public AnaSpillC{
 public :

  AnaSpillB();
  virtual ~AnaSpillB();

  /// Clone this object.
  virtual AnaSpillB* Clone() {
    AnaSpillB* spill = new AnaSpillB(*this);
    spill->RedoLinks();
    spill->isClone=true;
    return spill;
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Return whether this spill is from Monte Carlo or not.
  bool GetIsMC() const {return EventInfo->GetIsMC();}

  /// Return whether this spill is from Sand Monte Carlo or not.
  bool GetIsSandMC() const {return EventInfo->GetIsSandMC();}

  /// Copy the arrays into std vectors, needed for FlatTree creation
  virtual void CopyArraysIntoVectors();

  /// Copy the vectore into the arrays, needed after FlatTree reading
  virtual void CopyVectorsIntoArrays();

  /// Redo reco-reco and reco-truth after cloning or reading MiniTree
  virtual void RedoLinks();

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaSpillB(const AnaSpillB& spill);

  /// Add to a particle a given associated vertex (not in the base class)
  virtual void associateVertexToParticle(AnaParticleB*, AnaVertexB*) const{}

  /// Add to a true vertex a given associated reconstructed vertex (not in the base class)
  virtual void associateVertexToTrueVertex(AnaVertexB*) const{}

  /// Add to a true particle a given associated reconstructed particle (not in the base class)
  virtual void associateParticleToTrueParticle(AnaParticleB*) const{}

 public:

  /// Is this the original Spill or a clone
  //  bool isClone;

  /// The unique ID of the Geometry for this spill
  UInt_t GeomID;
  
  /// Run, sunrun, event, time  stamp, etc
  AnaEventInfoB* EventInfo;

  /// The beam quality flags for this spill.
  AnaBeamB* Beam;

  /// Total number of true vertices in the oaAnalysis Tree (even those not saved in the flat tree) 
  /// after deleting CohOnH, PauliBlocked and fully contained events (see https://bugzilla.nd280.org/show_bug.cgi?id=1174)
  UInt_t NTotalTrueVertices;

  /// Total number of true particles in the oaAnalysis Tree (even those not saved in the flat tree) 
  /// after deleting CohOnH, PauliBlocked and fully contained events (see https://bugzilla.nd280.org/show_bug.cgi?id=1174)
  UInt_t NTotalTrueParticles;

  /// The true MC vertices used in this spill.
  std::vector<AnaTrueVertexB*> TrueVertices;

  /// The true MC particles used in this spill.
  std::vector<AnaTrueParticleB*> TrueParticles;

  /// Reconstructed objects that didn't fit into one of the timing bunches.
  AnaBunchB* OutOfBunch;

};

/// An AnaEventSummaryB is a class used for storing high level objects and quantities 
/// output of the analysis. For example 
///  - LeptonCandidate
///  - Event vertex
///  - NeutrinoEnergy
/// 

class AnaEventSummaryB:public AnaEventSummaryC{
 public :

  AnaEventSummaryB();
  virtual ~AnaEventSummaryB(){}

  void ResetSummary();

public:

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
};


class AnaEventB: public AnaEventC{
 public :

  AnaEventB();
  AnaEventB(const AnaSpillB& spill, const AnaBunchB& bunch);
  virtual ~AnaEventB();

  /// Copy the entire object
  using AnaEventC::Copy;
  virtual void Copy(const AnaEventC& event, bool copyBunchInfo = true, bool cloneTruth=true);

  /// Clone this object.
  virtual AnaEventB* Clone() {
    AnaEventB* event = new AnaEventB(*this);
    event->isClone=true;
    return event;
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Return whether this event is from Monte Carlo or not.
  bool GetIsMC() const {return EventInfo.GetIsMC();}

  /// Return whether this event is from Sand Monte Carlo or not.
  bool GetIsSandMC() const {return EventInfo.GetIsSandMC();}

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaEventB(const AnaEventB& event);

 public:

  /// Run, sunrun, event, time  stamp, etc
  AnaEventInfoB EventInfo;


  /// The beam quality flags for this event.
  AnaBeamB* Beam;

  /// The index of this bunch (0-7).
  Int_t Bunch;

  /// The reconstructed objects in this bunch.
  AnaParticleB** Particles;
  /// The number of reconstructed particles in the event
  int nParticles;

  /// The reconstructed objects in this bunch.
  AnaVertexB** Vertices;
  int nVertices;

  /// The FGD time bins
  //  AnaFgdTimeBinB** FgdTimeBins;
  //  int nFgdTimeBins;

  /// The delayed cluster groups
  AnaDelayedClustersB** DelayedClusters; //! 
  Int_t nDelayedClusters; //! 

  /// The true MC particles used in this spill.
  AnaTrueParticleB** TrueParticles;
  int nTrueParticles;

  /// The true MC vertices used in this spill.
  AnaTrueVertexB** TrueVertices;
  int nTrueVertices;
};

#endif
