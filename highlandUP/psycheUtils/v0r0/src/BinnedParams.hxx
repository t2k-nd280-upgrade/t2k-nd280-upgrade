#ifndef BinnedParams_h
#define BinnedParams_h

#include "BaseDataClasses.hxx"
#include "TGraph.h"

/// This class represents a systematic source. 
/// It provides the functionality to read a file containing the systematic source values 
/// and store the values in BinnedParamsBin's. It handles systematic parameters in 1D, 2D and 3D

const UInt_t NMAXSYSTSOURCEBINS = 50;

class BinnedParamsParams{
 public :

  BinnedParamsParams() {}  
  virtual ~BinnedParamsParams(){}

  /// The mean value for each of the systematic parameters of the control sample
  Float_t meanDATA; //!

  /// The sigma value for each of the systematic parameters of the control sample  /// with possibility of asymm errors l=low, h=high
  //  Float_t sigmaDATA; //!
  Float_t sigmaDATAl; //!
  Float_t sigmaDATAh; //!

  /// The mean value for each of the systematic parameters of the control sample
  Float_t meanMC; //!

  /// The sigma value for each of the systematic parameters of the control sample
  /// with possibility of asymm errors l=low, h=high
  //  Float_t sigmaMC; //!
  Float_t sigmaMCl; //!
  Float_t sigmaMCh; //!

  /// The mean value for each of the systematic parameters of the analysis sample
  Float_t meanMCANA; //!

  /// The mean value for each of the systematic parameters of the control sample
  Float_t mean; //!

  /// The sigma value for each of the systematic parameters of the control sample taking errors from data and mc
  Float_t sigma; //!

};


class BinnedParamsGraphBin {

public:
  
  enum BinEnum{
    kMeanDATA = 0,                 
    kSigmaDATAl,     
    kSigmaDATAh,     
    kMeanMC,     
    kSigmaMCl,     
    kSigmaMCh,     
    kMeanMCANA,     
    kMean,           
    kSigma,
    kNGraph
  };

  /// default constructor
  BinnedParamsGraphBin(){
    Initialize();    
  }
  
  /// Constructor for 2D bin
  BinnedParamsGraphBin(Float_t min, Float_t max)
  {_min=min; _max=max; Initialize();}

  /// Constructor for 3D bin
  BinnedParamsGraphBin(Float_t min, Float_t max, Float_t min2, Float_t max2)
  {_min=min; _max=max; _min2=min2; _max2=max2; Initialize();}

  /// default destructor
  ~BinnedParamsGraphBin(){
    for (int i = 0; i < kNGraph; i++){
      if (_graphs[i]) delete _graphs[i];
      _graphs[i] = NULL;
    }
  }
  
  /// Update
  void Update(Float_t min, Float_t max, 
      Float_t meanREAL, 
      Float_t meanMC, 
      Float_t sigmaMCl, 
      Float_t sigmaMCh, 
      Float_t meanDATA, 
      Float_t sigmaDATAl, 
      Float_t sigmaDATAh){
   
    Float_t point = min + max;
    point /= 2;
    
    if (_graphs[kMeanMCANA  ] )    _graphs[kMeanMCANA   ] ->  SetPoint( _graphs[kMeanMCANA  ] ->  GetN(), point, meanREAL   );                
    if (_graphs[kMeanMC     ] )    _graphs[kMeanMC      ] ->  SetPoint( _graphs[kMeanMC     ] ->  GetN(), point, meanMC     ); 
    if (_graphs[kSigmaMCl   ] )    _graphs[kSigmaMCh    ] ->  SetPoint( _graphs[kSigmaMCh   ] ->  GetN(), point, sigmaMCl   );
    if (_graphs[kSigmaMCh   ] )    _graphs[kSigmaMCl    ] ->  SetPoint( _graphs[kSigmaMCl   ] ->  GetN(), point, sigmaMCh   );
    if (_graphs[kMeanDATA   ] )    _graphs[kMeanDATA    ] ->  SetPoint( _graphs[kMeanDATA   ] ->  GetN(), point, meanDATA   );
    if (_graphs[kSigmaDATAl ] )    _graphs[kSigmaDATAl  ] ->  SetPoint( _graphs[kSigmaDATAl ] ->  GetN(), point, sigmaDATAl );
    if (_graphs[kSigmaDATAh ] )    _graphs[kSigmaDATAh  ] ->  SetPoint( _graphs[kSigmaDATAh ] ->  GetN(), point, sigmaDATAh );    
  
  }

  void Update(Float_t min, Float_t max, 
      Float_t mean, 
      Float_t sigma){
    
    Float_t point = min + max;
    point /= 2;
  
    if (_graphs[kMean   ] )    _graphs[kMean  ] ->  SetPoint( _graphs[kMean       ] ->  GetN(), point,  mean);
    if (_graphs[kSigma  ] )    _graphs[kSigma ] ->  SetPoint( _graphs[kSigma      ] ->  GetN(), point,  sigma);
  }
  
  bool GetValue(BinEnum type, Float_t value, Float_t& result){
    if (type == kNGraph) return false;
    
    if (!_graphs[type] || _graphs[type]->GetN()<2)
      return false;
   
    // this is a simple linear interpolation
    result =  (Float_t)(_graphs[type]->Eval(value));
   
    return true;
  }
  
  /// Fill params given a value
  bool GetParams(Float_t value, BinnedParamsParams& params);
  
  /// bin limits (for 2D)
  Float_t _min;
  Float_t _max;

  /// bin limits (for 3D)
  Float_t _min2;
  Float_t _max2;
  
  Int_t _index;
  
protected:
  
  /// Internal graphs for interpolation
  TGraph* _graphs[kNGraph];
  
private:
  /// Initialize
  void Initialize(){
    for (int i = 0; i < kNGraph; i++)
      _graphs[i] = new TGraph();
  }
};


class BinnedParamsBin {

public:

  /// default constructor
  BinnedParamsBin(){};

  /// default destructor
  ~BinnedParamsBin(){};

  /// Constructor for 1D bin
  BinnedParamsBin(Float_t min, Float_t max, Float_t meanREAL,Float_t meanMC, Float_t sigmaMCl,Float_t sigmaMCh,Float_t meanDATA, Float_t sigmaDATAl,Float_t sigmaDATAh)
  {_min=min;_max=max;_meanREAL=meanREAL;_meanMC=meanMC;_sigmaMCl=sigmaMCl;_sigmaMCh=sigmaMCh;_meanDATA=meanDATA;_sigmaDATAl=sigmaDATAl;_sigmaDATAh=sigmaDATAh;}
  

  BinnedParamsBin(Float_t min, Float_t max, Float_t mean,Float_t sigma)
  {_min=min;_max=max;_mean=mean;_sigma=sigma;}
  
  /// Constructor for 2D bin
  BinnedParamsBin(Float_t min, Float_t max, Float_t min2, Float_t max2, Float_t meanREAL, Float_t meanMC, Float_t sigmaMCl,Float_t sigmaMCh,Float_t meanDATA, Float_t sigmaDATAl, Float_t sigmaDATAh)
  {_min=min;_max=max;_min2=min2;_max2=max2;_meanREAL=meanREAL;_meanMC=meanMC;_sigmaMCl=sigmaMCl;_sigmaMCh=sigmaMCh;_meanDATA=meanDATA;_sigmaDATAl=sigmaDATAl;_sigmaDATAh=sigmaDATAh;}

  BinnedParamsBin(Float_t min, Float_t max, Float_t min2, Float_t max2,Float_t mean,Float_t sigma)
  {_min=min;_max=max;_min2=min2;_max2=max2;_mean=mean;_sigma=sigma;}

  /// Constructor for 3D bin
  BinnedParamsBin(Float_t min, Float_t max, Float_t min2, Float_t max2, Float_t min3, Float_t max3, 
      Float_t meanREAL, Float_t meanMC, Float_t sigmaMCl,Float_t sigmaMCh,Float_t meanDATA, Float_t sigmaDATAl, Float_t sigmaDATAh)
  {_min=min; _max=max; _min2=min2; _max2=max2; _min3=min3; _max3=max3; _meanMC=meanMC; _meanREAL=meanREAL; _sigmaMCl=sigmaMCl; _sigmaMCh=sigmaMCh; 
    _meanDATA=meanDATA; _sigmaDATAl=sigmaDATAl; _sigmaDATAh=sigmaDATAh;}

  BinnedParamsBin(Float_t min, Float_t max, Float_t min2, Float_t max2, Float_t min3, Float_t max3, Float_t mean, Float_t sigma) 
  {_min=min; _max=max; _min2=min2; _max2=max2; _min3=min3; _max3=max3; _mean=mean; _sigma=sigma;}


public:  

  /// bin limits (for 1D and 2D)
  Float_t _min;
  Float_t _max;

  /// bin limits (only for 2D)
  Float_t _min2;
  Float_t _max2;

  /// bin limits (only for 3D)
  Float_t _min3;
  Float_t _max3;
  
  /// mean and sigma for this bin
  Float_t _mean;    
  Float_t _sigma;   

  /// mean and sigma for this bin
  Float_t _meanREAL;   /// Efficiency of the MC analysis sample
  Float_t _meanMC;     /// Efficiency of the MC control sample
  Float_t _sigmaMCl;   /// Low  error on Efficiency of the MC control sample
  Float_t _sigmaMCh;   /// high error on Efficiency of the MC control sample
  Float_t _meanDATA;   /// Efficiency of the DATA control sample
  Float_t _sigmaDATAl; /// Low  error on Efficiency of the DATA control sample 
  Float_t _sigmaDATAh; /// high error on Efficiency of the DATA control sample

  /// index for bin
  Int_t _index;

};

class BinnedParams {

public:

  /// Enumerator describing the values that _type member can take
  enum TypeEnum {
    k1D_SYMMETRIC = 0,
    k2D_SYMMETRIC,
    k3D_SYMMETRIC, 
    k1D_SYMMETRIC_NOMEAN, // no mean 
    k2D_SYMMETRIC_NOMEAN, // no mean 
    k3D_SYMMETRIC_NOMEAN, // no mean 
    k1D_EFF_SYMMETRIC,
    k2D_EFF_SYMMETRIC,
    k3D_EFF_SYMMETRIC, 
    k1D_EFF_ASSYMMETRIC,
    k2D_EFF_ASSYMMETRIC,
    k3D_EFF_ASSYMMETRIC, 
    // Simple ones that store one value per bin 
    k1D_DATA,
    k2D_DATA, 
    k3D_DATA, 
    kUnknown
  }; //!
  
  //1D, 2D or 3D
  enum TypeShortEnum {
    k1D = 0, 
    k2D, 
    k3D, 
    kUnknownShort
  };


public:

  BinnedParams();
 
  /// Constructor provided the folder name, name and the type
  BinnedParams(
      const std::string& folder_name, 
      const std::string& name, 
      TypeEnum type, 
      const std::string& extension="", 
      bool useInterpolation = false
      ){
    Initialize(
        folder_name, 
        name, 
        type, 
        extension, 
        useInterpolation
        );
  
  }
  
  
  /// Constructor provided the name and the type,  this is to make current psycheSystematics work, may remove it
  BinnedParams(const std::string& name, 
      TypeEnum type, 
      const std::string& extension="",
      bool useInterpolation = false){
    
    std::string folder_name = std::string(getenv("PSYCHESYSTEMATICSROOT")) + "/data";
    
    Initialize(
        folder_name, 
        name, 
        type, 
        extension, 
        useInterpolation
        );
  }
   
  virtual ~BinnedParams(){
    
    //clear bins
    
    for (Int_t i = 0; i < _nBins; i++){
      if (_bins[i]) delete _bins[i];
      _bins[i] = NULL;
    }
    
    for (Int_t i = 0; i < _nGraphBins; i++){
      if (_graphBins[i]) delete _graphBins[i];
      _graphBins[i] = NULL;
    }
  }
  
  /// Add a new bin for a 1D source
  void AddBin(Float_t min, Float_t max,
      Float_t meanREAL, 
      Float_t meanMC, 
      Float_t sigmaMCl,
      Float_t sigmaMCh,
      Float_t meanDATA, 
      Float_t sigmaDATAl,
      Float_t sigmaDATAh){
    
    BinnedParamsBin* b = new BinnedParamsBin(min, max,
        meanREAL,
        meanMC,
        sigmaMCl,
        sigmaMCh,
        meanDATA,
        sigmaDATAl,
        sigmaDATAh);
    
    _bins[_nBins++] = b;
    
    if (_useInterpolation)
      AddGraphBin(min, max,
          meanREAL,
          meanMC,
          sigmaMCl,
          sigmaMCh,
          meanDATA,
          sigmaDATAl,
          sigmaDATAh);
      
  }
  void AddBin(Float_t min, Float_t max,
      Float_t mean, 
      Float_t sigma){
    BinnedParamsBin* b = new BinnedParamsBin(min, max,
        mean,
        sigma);
    
    _bins[_nBins++] = b;
     
    if (_useInterpolation)
      AddGraphBin(min, max,
          mean,
          sigma); 
  }
 
  /// Add a new bin for a 2D source
  void AddBin(Float_t min, Float_t max, Float_t min2, Float_t max2,
      Float_t meanREAL, 
      Float_t meanMC, 
      Float_t sigmaMCl,
      Float_t sigmaMCh,
      Float_t meanDATA, 
      Float_t sigmaDATAl,
      Float_t sigmaDATAh){    
    
    BinnedParamsBin* b = new BinnedParamsBin(min, max, min2, max2, 
        meanREAL,
        meanMC,
        sigmaMCl,
        sigmaMCh,
        meanDATA,
        sigmaDATAl,
        sigmaDATAh);
    
    _bins[_nBins++] = b;
     
    if (_useInterpolation)
      AddGraphBin(min, max, min2, max2, 
          meanREAL,
          meanMC,
          sigmaMCl,
          sigmaMCh,
          meanDATA,
          sigmaDATAl,
          sigmaDATAh);

  }

  void AddBin(Float_t min, Float_t max, Float_t min2, Float_t max2,
      Float_t mean, 
      Float_t sigma){    
    
    BinnedParamsBin* b = new BinnedParamsBin(min, max, min2, max2, 
        mean, 
        sigma);
    _bins[_nBins++] = b;
     
    if (_useInterpolation)
      AddGraphBin(min, max, min2, max2, 
          mean,
          sigma);  
  }
  
  /// Add a new bin for a 3D source
  void AddBin(Float_t min, Float_t max, Float_t min2, Float_t max2, Float_t min3, Float_t max3, 
      Float_t meanREAL, 
      Float_t meanMC, 
      Float_t sigmaMCl,
      Float_t sigmaMCh,
      Float_t meanDATA, 
      Float_t sigmaDATAl,
      Float_t sigmaDATAh){    
    
    BinnedParamsBin* b = new BinnedParamsBin(min, max, min2, max2, min3, max3, 
        meanREAL, 
        meanMC, 
        sigmaMCl, 
        sigmaMCh, 
        meanDATA, 
        sigmaDATAl, 
        sigmaDATAh);
    _bins[_nBins++] = b;
     
    if (_useInterpolation)
      AddGraphBin(min, max, min2, max2, min3, max3,  
          meanREAL,
          meanMC,
          sigmaMCl,
          sigmaMCh,
          meanDATA,
          sigmaDATAl,
          sigmaDATAh);
  }

  void AddBin(Float_t min, Float_t max, Float_t min2, Float_t max2, Float_t min3, Float_t max3, 
      Float_t mean, 
      Float_t sigma){    
    
    BinnedParamsBin* b = new BinnedParamsBin(min, max, min2, max2, min3, max3, 
        mean, 
        sigma);
    _bins[_nBins++] = b;
     
    if (_useInterpolation)
      AddGraphBin(min, max, min2, max2, min3, max3, 
          mean,
          sigma); 
  }
  
  /// Add a new bin graph for a 1D source
  void AddGraphBin(Float_t min, Float_t max, 
      Float_t meanREAL, 
      Float_t meanMC,
      Float_t sigmaMCl,
      Float_t sigmaMCh,
      Float_t meanDATA,
      Float_t sigmaDATAl,
      Float_t sigmaDATAh);

  void AddGraphBin(Float_t min, Float_t max, 
      Float_t mean, 
      Float_t sigma);
  
  
  /// Add a new bin graph for a 2D source
  void AddGraphBin(Float_t min, Float_t max, Float_t min2, Float_t max2,
      Float_t meanREAL,
      Float_t meanMC,
      Float_t sigmaMCl,
      Float_t sigmaMCh,
      Float_t meanDATA, 
      Float_t sigmaDATAl,
      Float_t sigmaDATAh);
  
  void AddGraphBin(Float_t min, Float_t max, Float_t min2, Float_t max2,
      Float_t mean, 
      Float_t sigma);
  
  /// Add a new bin graph for a 3D source
  void AddGraphBin(Float_t min, Float_t max, Float_t min2, Float_t max2, Float_t min3, Float_t max3, 
      Float_t meanREAL, 
      Float_t meanMC, 
      Float_t sigmaMCl,
      Float_t sigmaMCh,
      Float_t meanDATA, 
      Float_t sigmaDATAl,
      Float_t sigmaDATAh);

  void AddGraphBin(Float_t min, Float_t max, Float_t min2, Float_t max2, Float_t min3, Float_t max3, 
      Float_t mean, 
      Float_t sigma);
 
  /// Gets the bin for a 2D source
  bool GetGraphBin(Float_t value, BinnedParamsGraphBin*& bin);

  /// Gets the bin for a 3D source
  bool GetGraphBin(Float_t value1, Float_t value2, BinnedParamsGraphBin*& bin);


  /// Gets the bin for a 1D source
  bool GetBin(Float_t value, BinnedParamsBin*& bin);

  /// Gets the bin for a 2D source
  bool GetBin(Float_t value1, Float_t value2, BinnedParamsBin*& bin);

  /// Gets the bin for a 3D source
  bool GetBin(Float_t value1, Float_t value2, Float_t value3, BinnedParamsBin*& bin);


  /// Gets the bin index for a 1D source
  bool GetBinIndex(Float_t value, Int_t& index);

  /// Gets the bin values for a source provided the bin index
  bool GetParametersForBin(Int_t index, Float_t& mean, Float_t& sigma);
  bool GetParametersForBin(Int_t index, BinnedParamsParams& params);
  
  /// Get only mean or sigma
  bool GetSigmaValueForBin(Int_t index, Float_t& sigma){
    Float_t mean;
    return GetParametersForBin(index, mean, sigma);
  }
  bool GetMeanValueForBin(Int_t index, Float_t& mean){
    Float_t sigma;
    return GetParametersForBin(index, mean, sigma);
  }


  /// Gets the bin values for a 1D source
  bool GetBinValues(Float_t value, Float_t& mean, Float_t& sigma){
    Int_t index;
    return GetBinValues(value, mean, sigma, index);
  }
  bool GetBinValues(Float_t value, Float_t& mean, Float_t& sigma, Int_t& index);
  
  /// Get only sigma
  bool GetBinSigmaValue(Float_t value, Float_t& sigma){
    Float_t mean;
    Int_t index;
    return GetBinValues(value, mean, sigma, index);
  }

  bool GetBinSigmaValue(Float_t value, Float_t& sigma, Int_t& index){
    Float_t mean;
    return GetBinValues(value, mean, sigma, index);
  } 

  // Get only mean
  bool GetBinMeanValue(Float_t value, Float_t& mean){
    Float_t sigma;
    Int_t index;
    return GetBinValues(value, mean, sigma, index);
  }
  
  bool GetBinMeanValue(Float_t value, Float_t& mean, Int_t& index){
    Float_t sigma;
    return GetBinValues(value, mean, sigma, index);
  }
 
  
  bool GetBinValues(Float_t value, BinnedParamsParams& params){
    Int_t index;
    return GetBinValues(value, params, index);
  }
  bool GetBinValues(Float_t value, BinnedParamsParams& params, Int_t& index);

  /// Gets the bin values for a 2D source
  bool GetBinValues(Float_t value1, Float_t value2, Float_t& mean, Float_t& sigma){
    Int_t index;
    return GetBinValues(value1, value2, mean, sigma, index);
  }
  
  bool GetBinValues(Float_t value1, Float_t value2, Float_t& mean, Float_t& sigma, Int_t& index);
  
  /// Get only sigma
  bool GetBinSigmaValue(Float_t value1, Float_t value2, Float_t& sigma){
    Float_t mean;
    Int_t index;
    return GetBinValues(value1, value2, mean, sigma, index);
  }

  bool GetBinSigmaValue(Float_t value1, Float_t value2, Float_t& sigma, Int_t& index){
    Float_t mean;
    return GetBinValues(value1, value2, mean, sigma, index);
  } 

  // Get only mean
  bool GetBinMeanValue(Float_t value1, Float_t value2, Float_t& mean){
    Float_t sigma;
    Int_t index;
    return GetBinValues(value1, value2, mean, sigma, index);
  }
  
  bool GetBinMeanValue(Float_t value1, Float_t value2, Float_t& mean, Int_t& index){
    Float_t sigma;
    return GetBinValues(value1, value2, mean, sigma, index);
  }
  
  bool GetBinValues(Float_t value1, Float_t value2, BinnedParamsParams& params){
    Int_t index;
    return GetBinValues(value1, value2, params, index);
  }
  
  bool GetBinValues(Float_t value1, Float_t value2, BinnedParamsParams& params, Int_t& index);

  /// Gets the bin values for a 3D source
  bool GetBinValues(Float_t value1, Float_t value2, Float_t value3, Float_t& mean, Float_t& sigma){
    Int_t index;
    return GetBinValues(value1, value2, value3, mean, sigma, index);
  }
  
  bool GetBinValues(Float_t value1, Float_t value2, Float_t value3, Float_t& mean, Float_t& sigma, Int_t& index);
  
  /// Get only sigma
  bool GetBinSigmaValue(Float_t value1, Float_t value2, Float_t value3, Float_t& sigma){
    Float_t mean;
    Int_t index;
    return GetBinValues(value1, value2, value3, mean, sigma, index);
  }

  bool GetBinSigmaValue(Float_t value1, Float_t value2, Float_t value3, Float_t& sigma, Int_t& index){
    Float_t mean;
    return GetBinValues(value1, value2, value3, mean, sigma, index);
  } 

  // Get only mean
  bool GetBinMeanValue(Float_t value1, Float_t value2, Float_t value3, Float_t& mean){
    Float_t sigma;
    Int_t index;
    return GetBinValues(value1, value2, value3, mean, sigma, index);
  }
  
  bool GetBinMeanValue(Float_t value1, Float_t value2, Float_t value3, Float_t& mean, Int_t& index){
    Float_t sigma;
    return GetBinValues(value1, value2, value3, mean, sigma, index);
  }
     
  bool GetBinValues(Float_t value1, Float_t value2, Float_t value3, BinnedParamsParams& params){
    Int_t index;
    return GetBinValues(value1, value2, value3, params, index);
  }
  
  bool GetBinValues(Float_t value1, Float_t value2, Float_t value3, BinnedParamsParams& params, Int_t& index);
  
  
  /// Get values using interpolation
  bool GetInterBinValues(Float_t value1, BinnedParamsParams& params);
  
  /// Gets the bin values for a 2D source
  bool GetInterBinValues(Float_t value1, Float_t value2, BinnedParamsParams& params);

  /// Gets the bin values for a 3D source
  bool GetInterBinValues(Float_t value1, Float_t value2, Float_t value3, BinnedParamsParams& params);
   
  /// Set the type 
  void SetType(TypeEnum type);

  /// Set the name
  void SetName(const std::string& name){_name = name;}

  /// Set the name
  void SetInterpolationUsage(bool use){_useInterpolation = use;}
  
  /// Get the name of the source
  const std::string& GetDataName(){return _name;}

  /// Returns the type of the data
  TypeEnum GetDataType(){return _type;}

  /// Returns the type of the data
  bool GetInterpolationStatus(){return _useInterpolation;}
   
  /// Dump into the screen the systematic source values
  void Print();

  /// Read from a file the systematic source values
  void Read(const std::string& inputDirName, const std::string& extension="");

  /// Get the number of bins
  Int_t GetNBins() {return _nBins;}
  
  /// Update the efficiency variables _ncorrect and _nwrong
  bool UpdateEfficiencyCounter(Int_t index, bool correct);

   /// Initialize counters
  void InitializeEfficiencyCounter(){
    for( UInt_t i=0;i<NMAXSYSTSOURCEBINS;i++){
      _ncorrect[i]=0;
      _nwrong[i]=0;
    }
  }
 
  Int_t GetNCorrectAssoc(Int_t index){
    if((UInt_t)index>=NMAXSYSTSOURCEBINS) {index=NMAXSYSTSOURCEBINS-1;std::cout<<" BinnedParams:: error index>" << NMAXSYSTSOURCEBINS <<std::endl;}
    return _ncorrect[index];
  }
  Int_t GetNWrongAssoc(Int_t index){
    if((UInt_t)index>=NMAXSYSTSOURCEBINS) {index=NMAXSYSTSOURCEBINS;std::cout<<" BinnedParams:: error index>"<< NMAXSYSTSOURCEBINS <<std::endl;}
    return _nwrong[index];
  }
  
private: 
  
  /// Prevent copying,  will add Clone() method if needed
  BinnedParams(const BinnedParams&); // no implementation
  BinnedParams& operator=(const BinnedParams&); // no implementation
  
  /// Initialize the BinnedParams
  void Initialize(
      const std::string& folder_name, 
      const std::string& name, 
      TypeEnum type, 
      const std::string& extension, 
      bool useInterpolation
      );
  
  /// number of correct associations
  Int_t  _ncorrect[NMAXSYSTSOURCEBINS]; //!

  /// number of wrong associations
  Int_t  _nwrong[NMAXSYSTSOURCEBINS]; //!

  /// The name of the systematic source
  std::string _name;

  /// The type of the systematic source
  TypeEnum _type;

  /// The vector of bins
  BinnedParamsBin* _bins[NMAXSYSTSOURCEBINS];

  /// The vector of graphs used to interpolate the systematics
  BinnedParamsGraphBin* _graphBins[NMAXSYSTSOURCEBINS];

  /// Number of bins for the BinnedParams
  Int_t _nBins;
  
  /// Number of graph bins for the BinnedParams
  Int_t _nGraphBins;

  /// The short principle type
  TypeShortEnum _typeShort;

  /// A parameter to check whether to use interpolation
  bool _useInterpolation;
  
  /// Set short type based on the current type
  void SetShortType();
  };

#endif
