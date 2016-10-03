#include "ToyVariations.hxx"

//*************************************************************************
ToyVariations::ToyVariations(Int_t npar){
//*************************************************************************

    nWeights = npar;
    nVariations = npar;
    Variations = new Float_t[npar];
    Weights = new Float_t[npar];

}

//*************************************************************************
ToyVariations::~ToyVariations(){
//*************************************************************************
    delete [] Variations;
    delete [] Weights;
}

//*************************************************************************
Double_t ToyVariations::GetWeight() const{
//*************************************************************************
  Double_t total_weight=1;
  for (Int_t i=0;i<nWeights;i++)
    total_weight *= Weights[i];
  return total_weight;
}

//*************************************************************************
Double_t ToyVariations::GetWeight(const Int_t* bins, int nBins) const{
//*************************************************************************

  if (nBins==0) return GetWeight();

  Double_t total_weight=1;
  for (Int_t i=0;i<nBins;i++){
    total_weight *= Weights[bins[i]];
  }
  return total_weight;
}
