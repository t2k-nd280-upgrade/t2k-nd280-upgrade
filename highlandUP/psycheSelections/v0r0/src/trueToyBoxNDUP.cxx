#include "trueToyBoxNDUP.hxx"
#include <typeinfo>

//********************************************************************
trueToyBoxNDUP::trueToyBoxNDUP():ToyBoxB(){
//******************************************************************** 
  
  TrueVertex=NULL;
  Vertex=NULL;
  MainTrack=NULL;
  HMT=HMinusPionT=HPlusPionT=HProtonT=NULL;
  TTPCtrack=NULL;
  TTargettrack=NULL;
  LTargetmax=-999;
  LYZmax=-999;


} 

//********************************************************************
void trueToyBoxNDUP::Reset(){
//******************************************************************** 

  if (!_ResetCheckDone){
    if( typeid(*this) !=  typeid(trueToyBoxNDUP)){
      std::cerr << "ERROR in trueToyBoxNDUP::Reset(). Either this mandatory method is not implemented " 
                << "by the derived class '" << typeid(*this).name() << "' "
                << "or trueToyBoxNDUP::Reset() is called from the Reset method of the derived class. "
                << "Please correct any of these bugs. " << std::endl;
      
      exit(1);
    }
    _ResetCheckDone=true;
  }
  
}

//********************************************************************
void trueToyBoxNDUP::ResetBase(){
//******************************************************************** 

  if (Vertex) delete Vertex; 

  TrueVertex=NULL;
  Vertex=NULL;
  MainTrack=NULL;
HMT=HPlusPionT=HMinusPionT=HProtonT=NULL;//  if (TTPCtrack.size() > 0 ) for (UInt_t i = 0; i < TTPCtrack.size(); i++) delete TTPCtrack[i];
 // if (TTargettrack.size() > 0 ) for (UInt_t i = 0; i < TTargettrack.size(); i++) delete TTargettrack[i];
 //TTPCtrack.clear();
 // TTargettrack.clear();
    if (TTPCtrack) delete [] TTPCtrack;
  if (TTargettrack) delete [] TTargettrack;
  

  nTPC = 0;
  TTPCtrack  = NULL;
  nTarget = 0;
  LTargetmax = -999;
  LYZmax = -999;

  TTargettrack  = NULL;

  SoftReset();
}  
