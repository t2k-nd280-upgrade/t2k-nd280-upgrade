#include "BinnedPDF.hxx"

///Define the integral of a gaussian normalized to 1, inside each of the nbins
//********************************************************************
BinnedGaussian::BinnedGaussian(int nbins, Float_t mean, Float_t sigma){
//********************************************************************

  _params.clear();
  _params.push_back(mean);
  _params.push_back(sigma);

  Float_t bin_width = (6*sigma/((Float_t)(nbins-1)));
  Float_t xmin=mean-3*sigma;
  Float_t wt=0;
  //  std::cout<<" mean "<<mean<<" xmin "<<xmin<<" ?= "<<mean-nbins*0.5*bin_width<<" binwidth "<<bin_width<<std::endl;
  for (Int_t i=0;i<nbins;i++){
    //this is the interval where we want to calculate the integral of N(mean,sigma) =1/sqrt(2pi)sigma exp(-1/2 (t'-mean)^2/sigma^2) 
    Float_t x = xmin + bin_width*(i-1./2); 
    Float_t x1 = xmin + bin_width*(i+1./2);
    //this is the corresponding interval for the erf function = 2/sqrt(pi) int_0^x exp(-t^2) dt 
    Float_t x_norm=(x-mean)/(sqrt(2)*sigma);
    Float_t x1_norm=(x1-mean)/(sqrt(2)*sigma);
    //because erf(infinity)=1 and erf(infinity) is defined as the integral from 0 to infinity,
    //we must devide by 2 to get back to the chosen normal distribution.
    Float_t w_erf=(TMath::Erf(x1_norm)-TMath::Erf(x_norm))/2;
    w_erf /=0.999140440; // since we are considering only 3sigma we need to normalize to 1
    if(sigma==0) w_erf=1;
    wt +=w_erf;
    //std::cout  <<"i "<< i << " bincenter " << xmin+bin_width*i << " weight " << w_erf << " sumofweight " << wt << std::endl;
    _binWeight.push_back(w_erf);
    _binCenter.push_back(xmin+bin_width*i);
  }

}
