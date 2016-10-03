#ifndef SystematicCovariance_h
#define SystematicCovariance_h

#include "Parameters.hxx"

#include "EventVariationBase.hxx"
#include "EventWeightBase.hxx"
#include "TRandom3.h"
#include "TString.h"
#include "TFile.h"
#include "HLClonesArray.hxx"
#include "SystematicManager.hxx"

class SystematicManager;

///This class constructs and manipulates the covariance of the systematic uncertainties.


struct indexlink
{
      int covarianceindex;
      int inputindex;
      int inputcov;
};

struct covlink
{
      std::vector<double> inputindex;
      std::vector<double> covarianceindex;
};

class SystematicCovariance{
      
   public:

      SystematicCovariance();
      virtual ~SystematicCovariance();
      
      /// Creates the covariance matrix for the enabled parameters    
      void ConstructCovarianceMatrix(SystematicManager* man);
      
      bool IsInList(std::string name, std::vector< std::string > list, int& index);

      /// Get Cov. Matrix
      TMatrixT<double>* GetCovarianceMatrix() const {return _covarianceMatrix;}

      
   protected:
      void FillLinks(SystematicBase* syst);
      void MapIndices();
      
      /// The covariance matrix for the enabled systematics    
      TMatrixT<double>* _covarianceMatrix;
      
      ///list of file names for covariances
      std::vector<std::string> listoffiles;
      
      ///list of covariances
      std::vector<TMatrixT<double>*> covariances;
      
      ///vector of index links 
      std::vector<indexlink> links;

      ///vector of covariance links
      std::vector<covlink> covlinks;
      
};

#endif
