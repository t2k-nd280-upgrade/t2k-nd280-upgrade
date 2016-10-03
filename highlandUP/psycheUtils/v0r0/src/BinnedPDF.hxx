#ifndef BinnedPDF_h
#define BinnedPDF_h

#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
#include <TMath.h>

/// This class defines a binned probability density function
class BinnedPDF{

public:

  BinnedPDF(){_binWeight.clear();_binCenter.clear();_params.clear();}

  virtual ~BinnedPDF(){}

  /// returns the number of bins
  unsigned int GetNBins() const{return _binCenter.size();}

  /// returns the weight for a bin
  Float_t GetBinWeight(int ibin) const {return _binWeight[ibin];}

  /// returns the bin center for a bin
  Float_t GetBinCenter(int ibin) const {return _binCenter[ibin];}

  /// returns vector of parameters
  const std::vector<Float_t>& GetParams() const {return _params;}
  
protected:

  /// vector of weights
  std::vector<Float_t> _binWeight;

  /// vector of bin centers
  std::vector<Float_t> _binCenter;

  /// vector of parameters
  std::vector<Float_t> _params;
};



/// This class defines a symmetric binned gaussian
class BinnedGaussian: public BinnedPDF{

public:  
  ///Define the integral of a gaussian normalized to 1, inside each of the nbins
  BinnedGaussian(int nbins, Float_t mean, Float_t sigma);
  virtual ~BinnedGaussian(){}
};


#endif
