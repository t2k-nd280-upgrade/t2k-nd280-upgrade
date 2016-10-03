#include "SystematicBase.hxx"
#include "MultiThread.hxx"

ClassImp(SystematicBase)

//***********************************************************
SystematicBase::SystematicBase() {
//***********************************************************

  _nParameters = 1; // The number of systematic parameters
  _name = "";
  _type = kUnknown;
  //  _PDF = kUnknownPDF;
  _PDF = kGaussian;
  _enabled = false;
  _systBoxes=NULL;
}

//***********************************************************
SystematicBase::SystematicBase(UInt_t npar) {
//***********************************************************

  _nParameters = npar; // The number of systematic parameters
  _name = "";
  _type = kUnknown;
  //  _PDF = kUnknownPDF;
  _PDF = kGaussian;
  _enabled = false;
  _systBoxes=NULL;
}

//***********************************************************
std::string SystematicBase::ConvertType() {
//***********************************************************

  if (_type == kVariation)
    return "variation";
  else if (_type == kWeight)
    return "weight";
  else if (_type == kFlux)
    return "flux";
  else
    return "unknown";
}

//***********************************************************
std::string SystematicBase::ConvertPDF() {
//***********************************************************

  if (_PDF == kGaussian)
    return "gaussian";
  else if (_PDF == kUniform)
    return "uniform";
  else if (_PDF == kBinomial)
    return "binomial";
  else if (_PDF == kMultinomial)
    return "multinomial";
  else
    return "unknown";

}

//********************************************************************
SystBoxB* SystematicBase::GetSystBox(const AnaEventC& event, Int_t isel, Int_t ibranch) const {
//********************************************************************

  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#else
  (void)event;
#endif

  return _systBoxes[isel][ibranch][uniqueID];
}

//********************************************************************
void SystematicBase::Initialize(Int_t nsel, Int_t isel, Int_t nbranches, Int_t nevents){
//********************************************************************

#ifndef MULTITHREAD
  nevents=1;
#endif

  // Create the array of SystBox for nsel selections the first time this is called
  if (!_systBoxes){
    _systBoxes = new SystBoxB***[nsel];    
    _systBoxesNSel=nsel;
    for (Int_t i=0;i<nsel;i++){
      _systBoxes[i]=NULL;
      _systBoxesNBranches[i]=0;
    }
  }

  //Create the subarray of SystBox for the specified selection, with nbranches and for a given number of events
  // Only the first time it is called for a given selection
  if (_systBoxes[isel]) return;

  _systBoxes[isel] = new SystBoxB**[nbranches];
  _systBoxesNBranches[isel]=nbranches;
  for (Int_t j=0;j<nbranches;j++){
    _systBoxes[isel][j] = new SystBoxB*[nevents];
    _systBoxesNEvents[isel][j]=nevents;
    for (Int_t k= 0;k<nevents;k++)
      _systBoxes[isel][j][k]=NULL;
  }
}

//********************************************************************
void SystematicBase::FinalizeEvent(const AnaEventC& event){
//********************************************************************

  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#else
  (void)event;
#endif

  // Delete the SystBox when it exists and create a new one. 
  // TODO: It is probably faster to just reset the arrays

  if(!_systBoxes) return;
  for (Int_t isel=0;isel<_systBoxesNSel;isel++){
    if(!_systBoxes[isel]) continue;
    for (Int_t ibranch=0;ibranch<_systBoxesNBranches[isel];ibranch++){
      if(!_systBoxes[isel][ibranch]) continue;
      if(_systBoxes[isel][ibranch][uniqueID]) delete _systBoxes[isel][ibranch][uniqueID];  
      _systBoxes[isel][ibranch][uniqueID]=NULL;
    }
  }
}
              
