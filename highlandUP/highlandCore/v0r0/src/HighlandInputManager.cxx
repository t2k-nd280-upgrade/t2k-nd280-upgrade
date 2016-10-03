#include "HighlandInputManager.hxx"
//#include "BasicUtils.hxx"

//********************************************************************
HighlandInputManager::HighlandInputManager():InputManager(){
//********************************************************************

  _RawSpill       = NULL;

}

//********************************************************************
void HighlandInputManager::Reset(){
//********************************************************************

  InputManager::Reset();
  _RawSpill       = NULL;
}

//*****************************************************************************
void HighlandInputManager::DeleteSpill(){
//*****************************************************************************

  InputManager::DeleteSpill();

  if (_RawSpill)
    delete _RawSpill;

  _RawSpill=NULL;
}

//*****************************************************************************
bool HighlandInputManager::LoadSpill(Long64_t& entry){
//*****************************************************************************

  Long64_t ientry = LoadTree(entry);
  if (ientry < 0) return false;
  
  // The spill to be read
  AnaSpillC* SpillToRead = NULL;

  // Fill the spill structure from the input tree. The SpillToRead instance is created internally by the converter
  Int_t nb = GetConverter().GetSpill(entry,SpillToRead);
  if (nb <=0 || !SpillToRead){
    return false;
  }

  // If the spill is succesfully read the previous successfully read spill must be deleted
  DeleteSpill();

  // We can now associate the spill just read with the _RawSpill
  _RawSpill = SpillToRead;

  // Make a copy of the spill
  _CorrectedSpill = _RawSpill->Clone();

  return true;
}

//*****************************************************************************
void HighlandInputManager::ResetSpillToRaw() {
//*****************************************************************************
  if (_Spill) {
    delete _Spill;
    _Spill = _RawSpill->Clone();
  }
}

//*****************************************************************************
//AnaEventC*  HighlandInputManager::MakeEvent() {
//*****************************************************************************

//  return new AnaEventC(GetRawSpill(),GetRawBunch());
//}



