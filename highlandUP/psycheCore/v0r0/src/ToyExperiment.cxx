#include "ToyExperiment.hxx"


//******************************************************************
ToyExperiment::ToyExperiment(){
//******************************************************************

  _nToyVariations=0;

  for(UInt_t i = 0; i < NMAXSYSTEMATICS; ++i){
      _toyVariations[i] = NULL;
  }

}

//******************************************************************
ToyExperiment::ToyExperiment(const ToyExperiment& toy){
//******************************************************************

  _nToyVariations       = toy.GetNToyVariations();
  
  for(UInt_t i = 0; i < NMAXSYSTEMATICS; ++i){ 
    if(i<_nToyVariations)
      _toyVariations[i] = new ToyVariations(toy.GetToyVariations(i)->GetNParameters());
    else
      _toyVariations[i] = NULL;
  }
}

//******************************************************************
ToyExperiment::~ToyExperiment(){
//******************************************************************
  
  for(UInt_t i = 0; i < NMAXSYSTEMATICS; ++i){
    if (_toyVariations[i]) delete _toyVariations[i];
  }

}

//******************************************************************
void ToyExperiment::AddToyVariation(Int_t index, UInt_t npar){
//******************************************************************

  ToyVariations* toyVar = new ToyVariations(npar);
  _toyVariations[index] = toyVar;
  _nToyVariations++;
}

