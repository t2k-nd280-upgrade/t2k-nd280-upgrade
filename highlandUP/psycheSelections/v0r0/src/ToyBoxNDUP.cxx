#include "ToyBoxNDUP.hxx"
#include <typeinfo>

//********************************************************************
ToyBoxNDUP::ToyBoxNDUP():ToyBoxB(){
//******************************************************************** 
  
  TrueVertex=NULL;
  Vertex=NULL;
  MainTrack=HMtrack=SHMtrack=HMNtrack=SHMNtrack=HMPtrack=SHMPtrack=NULL;
  NegativeTPCtracks = NULL;
  PositiveTPCtracks = NULL;
  nNegativeTPCtracks = 0;
  nPositiveTPCtracks = 0;
  HMT=HMinusPionT=HPlusPionT=HProtonT=NULL;
  TTPCtrack=NULL;
  TTargettrack=NULL;
  LTargetmax=-999;
  LYZmax=-999;


} 

//********************************************************************
void ToyBoxNDUP::Reset(){
//******************************************************************** 

  if (!_ResetCheckDone){
    if( typeid(*this) !=  typeid(ToyBoxNDUP)){
      std::cerr << "ERROR in ToyBoxNDUP::Reset(). Either this mandatory method is not implemented " 
                << "by the derived class '" << typeid(*this).name() << "' "
                << "or ToyBoxNDUP::Reset() is called from the Reset method of the derived class. "
                << "Please correct any of these bugs. " << std::endl;
      
      exit(1);
    }
    _ResetCheckDone=true;
  }
  
}

//********************************************************************
void ToyBoxNDUP::ResetBase(){
//******************************************************************** 

  if (Vertex) delete Vertex; 

  TrueVertex=NULL;
  Vertex=NULL;
HMT=HPlusPionT=HMinusPionT=HProtonT=NULL;//  if (TTPCtrack.size() > 0 ) for (UInt_t i = 0; i < TTPCtrack.size(); i++) delete TTPCtrack[i];
 // if (TTargettrack.size() > 0 ) for (UInt_t i = 0; i < TTargettrack.size(); i++) delete TTargettrack[i];
 //TTPCtrack.clear();
 // TTargettrack.clear();
    if (TTPCtrack) delete [] TTPCtrack;
  if (TTargettrack) delete [] TTargettrack;
  
  if (Vertex) delete Vertex; 
  if (NegativeTPCtracks) delete [] NegativeTPCtracks;
  if (PositiveTPCtracks) delete [] PositiveTPCtracks;
  
  Vertex=NULL;
  MainTrack=HMtrack=SHMtrack=HMNtrack=SHMNtrack=HMPtrack=SHMPtrack=NULL;
  NegativeTPCtracks = NULL;
  PositiveTPCtracks = NULL;
  nNegativeTPCtracks = 0;
  nPositiveTPCtracks = 0;

  nTPC = 0;
  TTPCtrack  = NULL;
  nTarget = 0;
  LTargetmax = -999;
  LYZmax = -999;

  TTargettrack  = NULL;

  SoftReset();
}  
