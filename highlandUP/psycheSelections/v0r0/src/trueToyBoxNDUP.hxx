#ifndef trueToyBoxNDUP_h
#define trueToyBoxNDUP_h

#include "ToyBoxB.hxx"
#include "DataClasses.hxx"

class trueToyBoxNDUP:public ToyBoxB{
public :
  
  trueToyBoxNDUP();
  virtual ~trueToyBoxNDUP(){}

  /// This method should be implemented by the derived class. If so it does nothing here 
  virtual void Reset();

  /// Reset this base class
  virtual void ResetBase();
  
public:

  /// For storing the true vertex, for analyses with no reconstructed primary vertex
  AnaTrueVertexB* TrueVertex;

  /// The reconstructed EventVertex
  AnaVertexB* Vertex;

  /// The MainTrack, defining the event vertex
  AnaTrackB* MainTrack;

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
