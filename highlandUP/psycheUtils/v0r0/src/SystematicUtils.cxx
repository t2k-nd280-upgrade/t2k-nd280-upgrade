#include "SystematicUtils.hxx"

#include <algorithm>
#include <stdio.h>
#include <math.h>

//********************************************************************
Weight_h systUtils::ComputeEffLikeWeight(bool found, Float_t variation_mc,  Float_t variation_data, const BinnedParamsParams& params){
    //********************************************************************

    Weight_h weight = 1;
    // P_DATA_ANA=meanDATA/meanMC*meanMCANA
    // weight = P_DATA_ANA/P_MC_ANA
    // weight = meanDATA/meanMC*meanMCANA/meanMCANA
    // weight = meanDATA/meanMC
    // the effweight are indpendent of the efficiency of the analysis sample
    // this is NOT the case for inefficiency weights!

    Float_t PvarDATA=1, PvarMC=1;
    if(variation_data>0) PvarDATA=std::min(params.meanDATA+params.sigmaDATAh*variation_data,(Float_t) 1.);
    else                 PvarDATA=std::max(params.meanDATA+params.sigmaDATAl*variation_data,(Float_t) 0.);
    
    if(variation_mc>0)   PvarMC=std::min(params.meanMC+params.sigmaMCh*variation_mc,(Float_t) 1.);
    else                 PvarMC=std::max(params.meanMC+params.sigmaMCl*variation_mc,(Float_t) 0.);

    if (found){
        if(PvarMC!=0)
            weight.Systematic = PvarDATA/PvarMC;        
        else 
            weight.Systematic = 1.;
        if(params.meanMC!=0)
            weight.Correction = params.meanDATA/params.meanMC;
        else
            weight.Correction = 1.;
    }
    else{
        Float_t Pvar=PvarDATA/PvarMC*params.meanMCANA;
        Float_t Pnom=params.meanDATA/params.meanMC*params.meanMCANA;
        if(Pvar>1) Pvar=1.;
        if((1-params.meanMCANA)!=0){
            weight.Systematic = (1-Pvar)/(1-params.meanMCANA);
            weight.Correction = (1-Pnom)/(1-params.meanMCANA);
        }
        else 
            weight =1;

    }

    return weight;
}


//********************************************************************
Weight_h systUtils::ComputeEffLikeWeight(bool found, Float_t variation, const BinnedParamsParams& params){
  //********************************************************************

    Weight_h weight = 1;
    // P_DATA_ANA=meanDATA/meanMC*meanMCANA
    // weight = P_DATA_ANA/P_MC_ANA
    // weight = meanDATA/meanMC*meanMCANA/meanMCANA
    // weight = meanDATA/meanMC
    // the effweight are indpendent of the efficiency of the analysis sample
    // this is NOT the case for inefficiency weights!

    Float_t PDATA=params.meanDATA, PMC=params.meanMC;
    double ratio = 1;
    if(params.meanMC!=0)
      ratio = params.meanDATA/params.meanMC;

    // this way we take the biggest stat error from error bars.
    double PMC_error    = std::max(params.sigmaMCh,   params.sigmaMCl);
    double PDATA_error  = std::max(params.sigmaDATAh, params.sigmaDATAl);

    //stat err of MC are independant from the stat error of the data...
    double errstat_ratio_2 = ratio*ratio*((PDATA_error*PDATA_error)/(PDATA*PDATA) + (PMC_error*PMC_error)/(PMC*PMC));

    double errsyst_ratio = ratio-1; 
    double errsyst_ratio_2 = errsyst_ratio*errsyst_ratio; 
    double err_ratio     = sqrt(errstat_ratio_2+errsyst_ratio_2);
    // to cross-check that it gives the same error than the other method
    // double err_ratio     = sqrt(pow(errstat_ratio,2));
    
    double Pvar_ratio=ratio+variation*err_ratio;

    // Also not sure about this, shouldn't it be:
    // double Pvar_ratio = variation*err_ratio + 1;
    //
    // Otherwise we are adding the data-MC difference in to the weight twice
    // MR: it depends what you want to do... if you want to compare to the nominal you will count it twice and in this case I agree that it is 1 that should be used
    //     or we decide, to correct the MC so that it looks like the data (as we do for the variation in fact...)
    if (found){
      weight.Systematic = Pvar_ratio;
      weight.Correction = ratio;
    }
    else{
      Float_t Pvar=Pvar_ratio*params.meanMCANA;
      Float_t Pnom=ratio     *params.meanMCANA;
      if(Pvar>1) Pvar=1.;
      if((1-params.meanMCANA)!=0){
        weight.Systematic = (1-Pvar)/(1-params.meanMCANA);
        weight.Correction = (1-Pnom)/(1-params.meanMCANA);
      }
      else 
        weight =1;
      
    }

    return weight;
}



//********************************************************************
Weight_h systUtils::ComputeEffLikeWeight(bool found, const ToyExperiment& toy, Int_t systIndex, Int_t parIndex, const BinnedParamsParams& params){
//********************************************************************
  
#if useNewWeights 
      return systUtils::ComputeEffLikeWeight(found, toy.GetToyVariations(systIndex)->Variations[parIndex], params);// new way including data-mc diff
#else
      return systUtils::ComputeEffLikeWeight(found, toy.GetToyVariations(systIndex)->Variations[2*parIndex],toy.Variations[2*parIndex+1], params);
#endif

}
