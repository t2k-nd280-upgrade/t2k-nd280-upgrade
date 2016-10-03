#include "InputConverter.hxx"

//*****************************************************************************
InputConverter::InputConverter(const std::string& name){
//*****************************************************************************
  fChain = NULL;
  _treeName = name;
}

//*****************************************************************************
Long64_t InputConverter::LoadTree(Long64_t entry){
//*****************************************************************************

  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //     Notify();
  }
  return centry;
}

//*****************************************************************************
Long64_t InputConverter::GetEntries(){
//*****************************************************************************

  _nentries = GetChain()->GetEntries();

  return _nentries;
}


//*****************************************************************************
Int_t InputConverter::GetNEvents(Int_t entries){
//*****************************************************************************

  if (entries==-1) return GetEntries();
  else return entries;
}

//****************************************************************************
bool InputConverter::IsCorrectType(const std::string& inputString){
//****************************************************************************
  
  bool correct = true;

  TFile *f = TFile::Open(inputString.c_str());
  f->cd();
  if (!f->Get(_treeName.c_str())) correct = false;
  f->Close();

  return correct;
}

