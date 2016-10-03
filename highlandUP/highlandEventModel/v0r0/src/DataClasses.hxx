#ifndef DataClasses_hxx
#define DataClasses_hxx

#include "BaseDataClasses.hxx"
#include "ParticleId.hxx"

const UInt_t NMAXTIMENODES=10;

/// Representation of a true Monte Carlo vertex.
class AnaTrueVertex: public AnaTrueVertexB{
public :

  AnaTrueVertex();
  virtual ~AnaTrueVertex(){}

  /// Clone this object.
  virtual AnaTrueVertex* Clone() {
    return new AnaTrueVertex(*this);
  }

  /// Dump the object to screen.
  void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrueVertex(const AnaTrueVertex& vertex);

public:

  /// Entry in the RooTrackerVtx tree (not set directly)
  Int_t RooVtxEntry;

  /// The index of the associated RooTrackerVtx vertex from its position in the TClonesArray
  Int_t RooVtxIndex;

  /// The NEUT reaction code. Genie files have their code translated to the
  /// Neut equivalent.
  Int_t ReacCode;

  /// The true (unit) direction of the incoming neutrino.
  Float_t NuDir[3];

  /// The PDG code of the target nucleus.
  Int_t TargetPDG;

  /// Neutrino parent PDG code
  Int_t NuParentPDG;

  /// Decay point of the neutrino parent.
  Float_t NuParentDecPoint[4];

  /// Accumulated cut level for all selections and cut branches. Tell us if a true vertex has been selected or not
  std::vector< std::vector<Int_t> > AccumLevel; //!

  /// The Q2 of the true interaction
  Float_t Q2;

  /// The PDG code of the primary outgoing electron/muon.
  Int_t LeptonPDG;

  /// The momentum of the primary outgoing electron/muon.
  Float_t LeptonMom;

  /// The direction of the primary outgoing electron/muon.
  Float_t LeptonDir[3];

  /// The momentum of the primary outgoing protons listed first (likely the interacted one).
  Float_t ProtonMom;

  /// The direction of the primary outgoing protons listed first (likely the interacted one).
  Float_t ProtonDir[3];

  /// The momentum of the primary outgoing pions listed first (likely the interacted one).
  Float_t PionMom;

  /// The direction of the primary outgoing pions listed first (likely the interacted one).
  Float_t PionDir[3];

  /// Array to count the outgoing primary particles of each type (
  Int_t NPrimaryParticles[Int_t(ParticleId::kLast)+1];

  // Vector to store particles pre-FSI info (only available for NEUT by now)
  // TODO: check how much the file size increases
  // Only store particles escaping the nucleus w/o beeing affected by FSI (other than the lepton, for which FSI in not applied)
//  std::vector<TVector3> PreFSIParticles; // shortcut to store PDG, Momentum and CosTheta (all as Float_t)

  /// The total number of primary baryons that were ejected.
  // OBSOLETE, NOT FILLED, TO BE DELETED, use NPrimaryParticles instead
  Int_t NBaryons;

  /// The momentum of the target nucleus.
  Float_t TargetMom;

  /// The direction of the target nucleus.
  Float_t TargetDir[3];

  /// Vector of pointers to AnaParticleB associated with this true vertex
  /// Do we really need this? It makes stuff more complicated
  std::vector<AnaParticleB*> ReconParticles; //!

  /// Vector of pointers to AnaVertexB (global vertices) associated with this true vertex
  std::vector<AnaVertexB*> ReconVertices; //!

  /// Neut keep also 1pi and single gamma Pauli blocked vertices (unphysical, to be removed)
  /// see https://bugzilla.nd280.org/show_bug.cgi?id=1011
  bool IsPauliBlocked;

  // For a Neut bug, unphysical coherent interactions on hydrogen were produced (in prod 5 and 6)
  // see https://bugzilla.nd280.org/show_bug.cgi?id=1056
  bool IsCohOnH;

};

/// Representation of a true Monte Carlo trajectory/particle.
class AnaTrueParticle: public AnaTrueParticleB{
public :

  AnaTrueParticle();
  virtual ~AnaTrueParticle(){}

  /// Clone this object.
  virtual AnaTrueParticle* Clone() {
    return new AnaTrueParticle(*this);
  }

  /// Dump the object to screen.
  void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrueParticle(const AnaTrueParticle& track);

public:

  /// The ID of the primary particle that created this particle. You can use the
  /// anaUtils::GetTrueTrackByID(Int_t ID) or anaUtils::GetPrimaryTrueTrack(AnaTrueParticleB* track)
  /// functions to find the AnaTrueParticleB representing the primary particle.
  Int_t PrimaryID;

  /// The purity with which this particle was matched to a reconstructed object.
  Float_t Purity;

  /// The bunch in which this true interaction occurred, based on the time of
  /// this true particle Start Position and the bunching used for the reconstructed objects.
  Int_t Bunch;

  /// The index of the AnaTrueVertexB of the interaction that created this AnaTrueParticleB. Doesn't
  /// match to any variable in the AnaTrueVertexB class, but is needed to set up
  /// the Vertex pointer, which should be used to access the vertex that
  /// created this particle.
  Int_t VertexIndex;

  /// Vector of pointers to AnaParticle's associated with this true particle
  std::vector<AnaParticleB*> ReconParticles; //!
};

/// Extension to AnaParticleB to be used by aggregation
class AnaParticleE{
public :

  AnaParticleE();
  virtual ~AnaParticleE();

  /// Clone this object.
  virtual AnaParticleE* Clone() {
    return new AnaParticleE(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaParticleE(const AnaParticleE& track);

public:

  Float_t AveragedEdx;

  /// The error on the reconstructed momentum.
  Float_t MomentumError;

  /// The number of degrees of freedom when the track was fitted with a Kalman filter.
  Int_t NDOF;

  /// The chi2 value when the track was fitted using a Kalman filter.
  Float_t Chi2;

  /// Momentum from refitting the track assuming the electron hypothesis.
  Float_t MomentumEle;

  /// Error on momentum from refitting the track assuming the electron hypothesis.
  Float_t MomentumErrorEle;

  /// Momentum from refitting the track assuming the muon hypothesis.
  Float_t MomentumMuon;

  /// Error on momentum from refitting the track assuming the muon hypothesis.
  Float_t MomentumErrorMuon;

  /// Momentum from refitting the track assuming the proton hypothesis.
  Float_t MomentumProton;

  /// Error on momentum from refitting the track assuming the proton hypothesis.
  Float_t MomentumErrorProton;

  /// PDG of the most probable particle hypothesis used at reconstruction level
  Int_t ReconPDG; 
   
  /// The bunch of the track, based on the PositionStart.T()
  Int_t Bunch;


  /// The pointer to the most primary AnaVertexB (global vertex) associated with this track
  AnaVertexB* ReconVertex; //!

  /// Vector of pointers to AnaVertexB (global vertices) associated with this track
  std::vector<AnaVertexB*> ReconVertices; //!

  /// The reconstructed momentum of the track, at the most primary global vertex (if exists).
  Float_t MomentumAtVertex;
  //  std::vector<Float_t> MomentumAtVertices;

  /// The reconstructed direction of the track at the most primary global vertex (if exists).
  Float_t DirectionAtVertex[3];
  //  std::vector<TVector3> DirectionAtVertices;

};

/// AnaParticle
class AnaParticle: public AnaParticleB, public AnaParticleE{
public :

  AnaParticle():AnaParticleB(),AnaParticleE(){}
  virtual ~AnaParticle(){}

  /// Clone this object.
  virtual AnaParticle* Clone() {
    return new AnaParticle(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaParticle(const AnaParticle& part):AnaParticleB(part),AnaParticleE(part){}

};


/// Representation of a global track.
class AnaTrack: public AnaTrackB, public AnaParticleE{
public :

  AnaTrack();
  virtual ~AnaTrack();

  /// Clone this object.
  virtual AnaTrack* Clone() {
    return new AnaTrack(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrack(const AnaTrack& track);

public:

  /// A different representation of the detectors used by this track.
  ///
  /// The systems are numbered as:
  /// 1: TPC1
  /// 2: TPC2
  /// 3: TPC3
  /// 4: FGD1
  /// 5: FGD2
  /// 6: P0D
  /// 7: DSECal
  /// 8: Any SMRD module
  /// 9: Any TECal or PECal module
  ///
  /// If this field then has a value of "235", it means the track used TPC2, TPC3
  /// and FGD2. You will have to do some mathematical manipulation to extract the
  /// information you want, so it is recommended to just use the DetUsed array
  /// instead.
//  long unsigned int Detectors;

  
  /// The length of this global track
    Float_t Length;
  
};

class AnaVertex:public AnaVertexB{
public :

  AnaVertex();
  virtual ~AnaVertex(){}

  /// Clone this object.
  virtual AnaVertex* Clone() {
    return new AnaVertex(*this);
  }

  /// Dump the object to screen.
  void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaVertex(const AnaVertex& vertex);

public:

  /// The variance values of the fit using a Kalman filter.
  Float_t Variance[4];

  /// The chi2 value of the fit using a Kalman filter.
  Float_t Chi2;

  /// The number of degrees of freedom of the fit using a Kalman filter.
  Int_t NDOF;
};

/// Representation of the beam information, including POT and quality.
class AnaBeam: public AnaBeamB{
public :

  AnaBeam();
  virtual ~AnaBeam(){}

  /// Clone this object.
  virtual AnaBeam* Clone() {
    return new AnaBeam(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaBeam(const AnaBeam& beam);

public:

  /// The POT for this spill. For data, this comes from the Beam Summary Data.
  Float_t POT;

  /// Spill number from BeamSummaryDataModule::ND280Spill
  Int_t Spill;

  /// Spill number from BeamSummaryDataModule::BeamSummaryData::SpillNumber
  Int_t SpillNumber;

  /// Beam run, in form RRSSSS, where RR is main ring run (e.g. 44), and SSSS
  /// is the main ring subrun.
  Int_t BeamRunNumber;
};


class AnaEventInfo:public AnaEventInfoB{
public :

  AnaEventInfo();
  virtual ~AnaEventInfo(){}

  /// Clone this object.
  virtual AnaEventInfo* Clone() {
    return new AnaEventInfo(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaEventInfo(const AnaEventInfo& event);

public:

  /// The ND280 subrun number.
  //  Int_t SubRun;

  /// UNIX timestamp of the event, from the MCM (master clock module).
  Int_t EventTime;
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
class AnaBunch: public AnaBunchB{
public :

  AnaBunch();
  virtual ~AnaBunch();

  /// Clone this object.
  virtual AnaBunch* Clone() {
    return new AnaBunch(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaBunch(const AnaBunch& bunch);

public:
};

/// The top-level object for representing information from the input files.
/// A single AnaSpill contains information from an entire ND280 event.
/// The reconstructed objects are separated into bunches based on their time
/// information.
///
/// There are multiple copies of each spill available through the InputManager
/// [ND::input()], showing the raw state of the spill after reading it in from
/// the input files; after applying corrections; and after applying corrections
/// and systematics.
class AnaSpill: public AnaSpillB{
public :

  AnaSpill();
  virtual ~AnaSpill();

  /// Clone this object.
  virtual AnaSpill* Clone() {
    AnaSpill* spill = new AnaSpill(*this);
    spill->RedoLinks();
    spill->isClone=true;
    return spill;
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Redo reco-reco and reco-truth after cloning or reading MiniTree
  virtual void RedoLinks();

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaSpill(const AnaSpill& spill);

  /// Add to a particle a given associated vertex (not in the base class)
  virtual void associateVertexToParticle(AnaParticleB*, AnaVertexB*) const;

  /// Add to a true vertex a given associated reconstructed vertex (not in the base class)
  virtual void associateVertexToTrueVertex(AnaVertexB*) const;

  /// Add to a true particle a given associated reconstructed particle (not in the base class)
  virtual void associateParticleToTrueParticle(AnaParticleB*) const;

public:
};


class AnaEvent: public AnaEventB{
public :

  AnaEvent();
  AnaEvent(const AnaSpill& spill, const AnaBunch& bunch);
  virtual ~AnaEvent();

  /// Clone this object.
  virtual AnaEvent* Clone() {
    AnaEvent* spill = new AnaEvent(*this);
    spill->isClone=true;
    return spill;
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaEvent(const AnaEvent& event);

public:
};


class AnaRecTrueMatch: public AnaRecTrueMatchB{
public :

  AnaRecTrueMatch():AnaRecTrueMatchB(){}
  virtual ~AnaRecTrueMatch(){}

};



#endif
