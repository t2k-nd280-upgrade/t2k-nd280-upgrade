#ifndef SystematicUtils_h
#define SystematicUtils_h

#include "BinnedParams.hxx"
#include "ToyExperiment.hxx"
#include "WeightType.hxx"

#define useNewWeights 1

namespace systUtils{

  Weight_h ComputeEffLikeWeight(bool found, Float_t variation_mc, Float_t variation_data, const BinnedParamsParams& params);
  Weight_h ComputeEffLikeWeight(bool found, Float_t variation, const BinnedParamsParams& params);

  Weight_h ComputeEffLikeWeight(bool found, const ToyExperiment& toy, Int_t systIndex, Int_t parIndex, const BinnedParamsParams& params);
}
#endif




