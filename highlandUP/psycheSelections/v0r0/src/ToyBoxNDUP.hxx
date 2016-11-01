#ifndef ToyBoxNDUP_h
#define ToyBoxNDUP_h

#include "ToyBoxB.hxx"
#include "DataClasses.hxx"

class ToyBoxNDUP:public ToyBoxB{
public :
  
  ToyBoxNDUP();
  virtual ~ToyBoxNDUP(){}

  /// This method should be implemented by the derived class. If so it does nothing here 
  virtual void Reset();

  /// Reset this base class
  virtual void ResetBase();
  
public:
  /// For storing the reconstructed vertex.
  AnaVertexB* Vertex;

  /// For storing the Main Track (The lepton candidate in geranal: HMN or HMP track)
  AnaTrackB* MainTrack;

  /// For storing the highest momentum track.
  AnaTrackB* HMtrack;

  /// For storing the highest momentum negative track.
  AnaTrackB* HMNtrack;

  /// For storing the highest momentum positive track.
  AnaTrackB* HMPtrack;

  /// For storing the second highest momentum track.
  AnaTrackB* SHMtrack;

  /// For storing the second highest momentum negative track.
  AnaTrackB* SHMNtrack;

  /// For storing the second highest momentum positive track.
  AnaTrackB* SHMPtrack;

  /// For storing all negative tracks in the event with TPC information.
  AnaTrackB** NegativeTPCtracks;
  int nNegativeTPCtracks;
  /// For storing all positive tracks in the event with TPC information.
  AnaTrackB** PositiveTPCtracks;
  int nPositiveTPCtracks;

  /// For storing the true vertex, for analyses with no reconstructed primary vertex
  AnaTrueVertexB* TrueVertex;


  /// Longest particle in the event
  AnaTrackB* HMT;

  /// Second Longest particle in the event
  AnaTrackB* HPlusPionT;
 /// Second Longest particle in the event
  AnaTrackB* HMinusPionT;

  /// Third Longest particle in the event
  AnaTrackB* HElectronT;

  AnaTrackB* HProtonT;
  AnaTrackB** TTPCtrack;
  AnaTrackB** TTargettrack;
   int nTPC;
  int nTarget;
  Float_t LTargetmax;
  Float_t LYZmax;

};

#endif
