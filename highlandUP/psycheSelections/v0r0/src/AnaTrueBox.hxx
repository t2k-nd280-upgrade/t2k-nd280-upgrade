#ifndef AnaTrueBox_h
#define AnaTrueBox_h

#include "ToyBoxB.hxx"
#include "DataClasses.hxx"
#include "TRandom.h"
#include "TFile.h"
#include "TRandom3.h"

class AnaTrueBox:public ToyBoxB{
public :
  
  AnaTrueBox();
  virtual ~AnaTrueBox(){}

  /// This method should be implemented by the derived class. If so it does nothing here 
  virtual void Reset();

  /// Reset this base class
  virtual void ResetBase();
  
public:
    // Lepton track
    AnaTrueParticleB* Leptontrack;
    AnaTrueParticleB* HMNtrack;
    AnaTrueVertexB* Vertex;
    Int_t npion;
    Int_t npion_pos;
    AnaTrueParticleB* PositivePionTPCtracks;
    Float_t flag_mom;
    Float_t flag_HMN_mom;
    TRandom3 *random_true;
    

};

#endif
