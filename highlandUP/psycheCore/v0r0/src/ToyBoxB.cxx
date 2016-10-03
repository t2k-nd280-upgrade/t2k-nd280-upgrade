#include "ToyBoxB.hxx"
#include <typeinfo>

//********************************************************************
ToyBoxB::ToyBoxB(){
//******************************************************************** 
  
  AccumLevel = NULL;
  MaxAccumLevel=-1;
  BranchPassed=NULL;
  SelectionEnabledIndex=-1;
  SuccessfulBranch = -1;
  DetectorFV = UNASSIGNEDID;
  nBranches=0;

  // The pressence of the Reset method in the derived class was not checked yet
  _ResetCheckDone=false;  
} 

//********************************************************************
ToyBoxB::~ToyBoxB(){
//******************************************************************** 

  if (AccumLevel)        delete [] AccumLevel;
  if (BranchPassed)      delete [] BranchPassed;
} 

//********************************************************************
void ToyBoxB::Reset(){
//******************************************************************** 

  if (!_ResetCheckDone){
    if( typeid(*this) !=  typeid(ToyBoxB)){
      std::cerr << "ERROR in ToyBoxB::Reset(). Either this mandatory method is not implemented " 
                << "by the derived class '" << typeid(*this).name() << "' "
                << "or ToyBoxB::Reset() is called from the Reset method of the derived class. "
                << "Please correct any of these bugs. " << std::endl;
      
      exit(1);
    }
    _ResetCheckDone=true;
  }
  
}

//********************************************************************
void ToyBoxB::ResetBase(){
//******************************************************************** 

  SoftReset();
}  


//********************************************************************
void ToyBoxB::SoftReset(){
//******************************************************************** 

  MaxAccumLevel=0;
  for (UInt_t i=0;i<nBranches;i++){      
    AccumLevel[i]=0;
    BranchPassed[i]=true;
  }
  SuccessfulBranch = -1;
}  
