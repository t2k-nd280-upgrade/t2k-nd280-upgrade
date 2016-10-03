#include "BinnedParams.hxx"

//*********************************************************
BinnedParams::BinnedParams(){
  //*********************************************************

  _name             = "";
  _type             = kUnknown;
  _typeShort        = kUnknownShort;
  _nBins            = 0;
  _nGraphBins       = 0;
  _useInterpolation = false;
}

//*********************************************************
void BinnedParams::Initialize(
    const std::string& folder_name, 
    const std::string& name, 
    TypeEnum type, 
    const std::string& extension, 
    bool useInterpolation){
//*********************************************************

  _name = name;
  _type = type;
  _useInterpolation = useInterpolation;
  SetShortType();

  _nBins = 0;
  _nGraphBins = 0;
  
  BinnedParams::Read(folder_name, extension);
}

//*********************************************************
void BinnedParams::SetType(TypeEnum type){
  //*********************************************************
  _type = type;
  SetShortType(); 
}

//*********************************************************
void BinnedParams::SetShortType(){
  //*********************************************************
  if (_type == k1D_SYMMETRIC ||
      _type == k1D_SYMMETRIC_NOMEAN || 
      _type == k1D_DATA ||
      _type == k1D_EFF_SYMMETRIC ||
      _type == k1D_EFF_ASSYMMETRIC){
    _typeShort = k1D;
  }
  
  else if ( _type == k2D_SYMMETRIC ||
      _type == k2D_SYMMETRIC_NOMEAN || 
      _type == k2D_DATA ||
      _type == k2D_EFF_SYMMETRIC ||
      _type == k2D_EFF_ASSYMMETRIC){ 
    _typeShort = k2D;
  }
  else
    _typeShort = k3D;
}


//*********************************************************
void BinnedParams::Print(){
  //*********************************************************

  std::cout << "-------- BinnedParams: " << _name << "--------------------" << std::endl;
  std::cout << std::endl;
  Int_t it = 0;
  Int_t i=0;
  if (_type==k1D_SYMMETRIC){
    std::cout << "#: \t min \t max \t mean \t sigma" <<  std::endl;
    for (it=0;it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_mean << "\t " << _bins[it]->_sigma << std::endl;
  }
  else if (_type==k2D_SYMMETRIC){
    std::cout << "#: \t min1 \t max1 \t min2 \t max2 \t mean \t sigma" <<  std::endl;
    for (it=0; it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_min2 << "\t " 
        << _bins[it]->_max2 << "\t " << _bins[it]->_mean << "\t " << _bins[it]->_sigma << std::endl;
  }
  else if (_type==k3D_SYMMETRIC){
    std::cout << "#: \t min1 \t max1 \t min2 \t max2 \t min3 \t max3 \t mean \t sigma" <<  std::endl;
    for (it=0; it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_min2 << "\t " << _bins[it]->_max2 
        << "\t"  << _bins[it]->_min3 << "\t " << _bins[it]->_max3 
        << "\t " << _bins[it]->_mean << "\t " << _bins[it]->_sigma << std::endl;
  }
  
  else if (_type==k1D_SYMMETRIC_NOMEAN){
    std::cout << "#: \t min \t max \t sigma" <<  std::endl;
    for (it=0;it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_sigma << std::endl;
  }
  else if (_type==k2D_SYMMETRIC_NOMEAN){
    std::cout << "#: \t min1 \t max1 \t min2 \t max2 \t sigma" <<  std::endl;
    for (it=0; it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_min2 << "\t " 
        << _bins[it]->_max2 << "\t " << _bins[it]->_sigma << std::endl;
  }
  else if (_type==k3D_SYMMETRIC_NOMEAN){
    std::cout << "#: \t min1 \t max1 \t min2 \t max2 \t min3 \t max3 \t sigma" <<  std::endl;
    for (it=0; it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_min2 << "\t " << _bins[it]->_max2 
        << "\t"  << _bins[it]->_min3 << "\t " << _bins[it]->_max3 << "\t " << _bins[it]->_sigma << std::endl;
  }
  
  else if (_type==k1D_DATA){
    std::cout << "#: \t min \t max \t value" <<  std::endl;
    for (it=0;it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_mean << std::endl;
  }
  else if (_type==k2D_DATA){
    std::cout << "#: \t min1 \t max1 \t min2 \t max2 \t data" <<  std::endl;
    for (it=0; it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_min2 << "\t " 
        << _bins[it]->_max2 << "\t " << _bins[it]->_mean << std::endl;
  }
  else if (_type==k3D_DATA){
    std::cout << "#: \t min1 \t max1 \t min2 \t max2 \t min3 \t max3 \t data" <<  std::endl;
    for (it=0; it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_min2 << "\t " << _bins[it]->_max2 
        << "\t"  << _bins[it]->_min3 << "\t " << _bins[it]->_max3 << "\t " << _bins[it]->_mean << std::endl;
  }
  
  else if (_type==k1D_EFF_SYMMETRIC){
    std::cout << "#: \t min \t max \t real \t meanmc \t sigmamc \t meandata \t sigmadata" <<  std::endl;
    for (it=0; it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_meanREAL << "\t " 
        << _bins[it]->_meanMC << "\t " << _bins[it]->_sigmaMCl<< _bins[it]->_meanDATA << "\t " << _bins[it]->_sigmaDATAl << std::endl;
  }
  else if (_type==k2D_EFF_SYMMETRIC){
    std::cout << "#: \t min1 \t max1 \t min2 \t max2 \t real \t meanmc \t sigmamc \t meandata \t sigmadata" <<  std::endl;
    for (it=0; it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_min2 << "\t " << _bins[it]->_max2 << "\t " 
        << _bins[it]->_meanREAL << "\t "<< _bins[it]->_meanMC << "\t " << _bins[it]->_sigmaMCl<< _bins[it]->_meanDATA << "\t " << _bins[it]->_sigmaDATAl << std::endl;
  }

  else if (_type==k3D_EFF_SYMMETRIC){
    std::cout << "#: \t min1 \t max1 \t min2 \t max2 \t min3 \t max3 \t real \t meanmc \t sigmamc \t meandata \t sigmadata" <<  std::endl;
    for (it=0; it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_min2 << "\t " << _bins[it]->_max2 
        << "\t"  << _bins[it]->_min3 << "\t " << _bins[it]->_max3
        << "\t"  << _bins[it]->_meanREAL << "\t "<< _bins[it]->_meanMC << "\t " << _bins[it]->_sigmaMCl<< _bins[it]->_meanDATA << "\t " << _bins[it]->_sigmaDATAl << std::endl;
  }

  else if (_type==k1D_EFF_ASSYMMETRIC){
    std::cout << "#: \t min \t max \t real \t meanmc \t sigmamcl \t sigmamch \t meandata \t sigmadatal  \t sigmadatah" <<  std::endl;
    for (it=0; it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_meanREAL << "\t "<< _bins[it]->_meanMC << "\t " 
        << _bins[it]->_sigmaMCl <<"\t " << _bins[it]->_sigmaMCh << _bins[it]->_meanDATA << "\t " << _bins[it]->_sigmaDATAl<<" \t " 
        << _bins[it]->_sigmaDATAh << std::endl;
  }
  else if (_type==k2D_EFF_ASSYMMETRIC){
    std::cout << "#: \t min1 \t max1 \t min2 \t max2 \t real \t meanmc \t sigmamcl  \t sigmamch  \t meandata \t sigmadatal  \t sigmadatah" <<  std::endl;
    for (it=0; it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min << "\t " << _bins[it]->_max << "\t " << _bins[it]->_min2 << "\t " << _bins[it]->_max2 << "\t " 
        << _bins[it]->_meanREAL << "\t "<< _bins[it]->_meanMC << "\t " << _bins[it]->_sigmaMCl <<"\t " << _bins[it]->_sigmaMCh<<"\t " 
        << _bins[it]->_meanDATA << "\t " << _bins[it]->_sigmaDATAl<< "\t " << _bins[it]->_sigmaDATAh << std::endl;
  }

  else if (_type==k3D_EFF_ASSYMMETRIC){
    std::cout << "#: \t min1 \t max1 \t min2 \t max2 \t min3 \t max3 \t real \t meanmc \t sigmamcl  \t sigmamch  \t meandata \t sigmadatal  \t sigmadatah" <<  std::endl;
    for (it=0; it<_nBins;it++, i++)
      std::cout << i << ": \t " << _bins[it]->_min  << "\t " << _bins[it]->_max << "\t " << _bins[it]->_min2 << "\t " << _bins[it]->_max2
        << "\t " << _bins[it]->_min2      << "\t "  << _bins[it]->_max2     
        << "\t"  << _bins[it]->_meanREAL  << "\t "  << _bins[it]->_meanMC     << "\t " << _bins[it]->_sigmaMCl    <<  "\t " << _bins[it]->_sigmaMCh 
        << "\t"  << _bins[it]->_meanDATA  << "\t "  << _bins[it]->_sigmaDATAl << "\t " << _bins[it]->_sigmaDATAh  << std::endl;
  }

}

//********************************************************************
void BinnedParams::Read(const std::string& inputDirName, const std::string& extension){
  //********************************************************************

  // The file should have the same name as the BinnedParams
  std::string inputFileName = inputDirName+"/"+_name+extension+".dat";

  ifstream inputFile(inputFileName.c_str(), std::ios::in);

  if (!inputFile) {
    std::cerr << "Cannot open input file '" << inputFileName << "'. Exit!" << std::endl;
    exit(0);
  }

  std::string start, end, start2, end2, start3, end3, mean, sigma; 
  std::string mean_REAL, mean_CS_MC, sig_CS_MCl, sig_CS_MCh, mean_CS_DATA, sig_CS_DATAl, sig_CS_DATAh ; 
  char* pEnd;
  if(_type==k1D_SYMMETRIC){
    while (inputFile >> start >> end  >> mean >> sigma ) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(mean.c_str(),         &pEnd),
          strtod(sigma.c_str(),        &pEnd));
    }
  }
  else if(_type==k2D_SYMMETRIC){
    while (inputFile >> start >> end >>  start2 >> end2 >> mean >> sigma ) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(start2.c_str(),       &pEnd),
          strtod(end2.c_str(),         &pEnd),
          strtod(mean.c_str(),         &pEnd),
          strtod(sigma.c_str(),        &pEnd));
    }
  }

  else if(_type==k3D_SYMMETRIC){
    while (inputFile >> start >> end >>  start2 >> end2 >> start3 >> end3 >> mean >> sigma ) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(start2.c_str(),       &pEnd),
          strtod(end2.c_str(),         &pEnd),
          strtod(start3.c_str(),       &pEnd),
          strtod(end3.c_str(),         &pEnd),
          strtod(mean.c_str(),         &pEnd),
          strtod(sigma.c_str(),        &pEnd));
    }
  }
  // Only sigma values
  if(_type==k1D_SYMMETRIC_NOMEAN){
    while (inputFile >> start >> end  >> sigma) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          0., // no mean
          strtod(sigma.c_str(),        &pEnd));
    }
  }
  else if(_type==k2D_SYMMETRIC_NOMEAN){
    while (inputFile >> start >> end >>  start2 >> end2 >> sigma) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(start2.c_str(),       &pEnd),
          strtod(end2.c_str(),         &pEnd),
          0., // no mean
          strtod(sigma.c_str(),        &pEnd));
    }
  }

  else if(_type==k3D_SYMMETRIC_NOMEAN){
    while (inputFile >> start >> end >>  start2 >> end2 >> start3 >> end3 >> sigma ) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(start2.c_str(),       &pEnd),
          strtod(end2.c_str(),         &pEnd),
          strtod(start3.c_str(),       &pEnd),
          strtod(end3.c_str(),         &pEnd),
          0., // no mean
          strtod(sigma.c_str(),        &pEnd));
    }
  }
  
  if(_type==k1D_DATA){
    while (inputFile >> start >> end  >> mean) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(mean.c_str(),         &pEnd),
          0.); // no sigma
    }
  }
  else if(_type==k2D_DATA){
    while (inputFile >> start >> end >>  start2 >> end2 >> mean) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(start2.c_str(),       &pEnd),
          strtod(end2.c_str(),         &pEnd),
          strtod(mean.c_str(),         &pEnd),
          0.); // no sigma
    }
  }

  else if(_type==k3D_DATA){
    while (inputFile >> start >> end >>  start2 >> end2 >> start3 >> end3 >> mean) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(start2.c_str(),       &pEnd),
          strtod(end2.c_str(),         &pEnd),
          strtod(start3.c_str(),       &pEnd),
          strtod(end3.c_str(),         &pEnd),
          strtod(mean.c_str(),         &pEnd),
          0.); // no sigma
    }
  }
  
  else if(_type==k1D_EFF_SYMMETRIC){
    while (inputFile >> start >> end >> mean_REAL >> mean_CS_MC >> sig_CS_MCl >> mean_CS_DATA >> sig_CS_DATAl) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(mean_REAL.c_str(),    &pEnd),
          strtod(mean_CS_MC.c_str(),   &pEnd),
          strtod(sig_CS_MCl.c_str(),   &pEnd),
          strtod(sig_CS_MCl.c_str(),   &pEnd),
          strtod(mean_CS_DATA.c_str(), &pEnd),
          strtod(sig_CS_DATAl.c_str(), &pEnd),      
          strtod(sig_CS_DATAl.c_str(), &pEnd));
    }
  }
  else if(_type==k2D_EFF_SYMMETRIC){
    while (inputFile >> start >> end >>  start2 >> end2 >> mean_REAL >> mean_CS_MC >> sig_CS_MCl >> mean_CS_DATA >> sig_CS_DATAl) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(start2.c_str(),       &pEnd),
          strtod(end2.c_str(),         &pEnd),
          strtod(mean_REAL.c_str(),    &pEnd),
          strtod(mean_CS_MC.c_str(),   &pEnd),
          strtod(sig_CS_MCl.c_str(),   &pEnd),
          strtod(sig_CS_MCl.c_str(),   &pEnd),
          strtod(mean_CS_DATA.c_str(), &pEnd),
          strtod(sig_CS_DATAl.c_str(), &pEnd),      
          strtod(sig_CS_DATAl.c_str(), &pEnd));      
    }
  }
  else if(_type==k3D_EFF_SYMMETRIC){
    while (inputFile >> start >> end >>  start2 >> end2 >> start3 >> end3 >> mean_REAL >> mean_CS_MC >> sig_CS_MCl >> mean_CS_DATA >> sig_CS_DATAl) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(start2.c_str(),       &pEnd),
          strtod(end2.c_str(),         &pEnd),
          strtod(start3.c_str(),       &pEnd),
          strtod(end3.c_str(),         &pEnd),
          strtod(mean_REAL.c_str(),    &pEnd),
          strtod(mean_CS_MC.c_str(),   &pEnd),
          strtod(sig_CS_MCl.c_str(),   &pEnd),
          strtod(sig_CS_MCl.c_str(),   &pEnd),
          strtod(mean_CS_DATA.c_str(), &pEnd),
          strtod(sig_CS_DATAl.c_str(), &pEnd),      
          strtod(sig_CS_DATAl.c_str(), &pEnd));      
    }
  }

  else if(_type==k1D_EFF_ASSYMMETRIC){
    while (inputFile >> start >> end >> mean_REAL >> mean_CS_MC >> sig_CS_MCl >> sig_CS_MCh  >> mean_CS_DATA >> sig_CS_DATAl >> sig_CS_DATAh ) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(mean_REAL.c_str(),    &pEnd),
          strtod(mean_CS_MC.c_str(),   &pEnd),
          strtod(sig_CS_MCl.c_str(),   &pEnd),
          strtod(sig_CS_MCh.c_str(),   &pEnd),
          strtod(mean_CS_DATA.c_str(), &pEnd),
          strtod(sig_CS_DATAl.c_str(), &pEnd),      
          strtod(sig_CS_DATAh.c_str(), &pEnd));
    }
  }
  else if(_type==k2D_EFF_ASSYMMETRIC){
    while (inputFile >> start >> end >>  start2 >> end2 >> mean_REAL >> mean_CS_MC >> sig_CS_MCl >> sig_CS_MCh  >> mean_CS_DATA >> sig_CS_DATAl >> sig_CS_DATAh ) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(start2.c_str(),       &pEnd),
          strtod(end2.c_str(),         &pEnd),
          strtod(mean_REAL.c_str(),    &pEnd),
          strtod(mean_CS_MC.c_str(),   &pEnd),
          strtod(sig_CS_MCl.c_str(),   &pEnd),
          strtod(sig_CS_MCh.c_str(),   &pEnd),
          strtod(mean_CS_DATA.c_str(), &pEnd),
          strtod(sig_CS_DATAl.c_str(), &pEnd),      
          strtod(sig_CS_DATAh.c_str(), &pEnd));

    }
  }
  else if(_type==k3D_EFF_ASSYMMETRIC){
    while (inputFile >> start >> end >>  start2 >> end2 >> start3 >> end3
        >> mean_REAL>> mean_CS_MC >> sig_CS_MCl >> sig_CS_MCh  >> mean_CS_DATA >> sig_CS_DATAl >> sig_CS_DATAh ) {
      AddBin(strtod(start.c_str(),  &pEnd),
          strtod(end.c_str(),          &pEnd),
          strtod(start2.c_str(),       &pEnd),
          strtod(end2.c_str(),         &pEnd),
          strtod(start3.c_str(),       &pEnd),
          strtod(end3.c_str(),         &pEnd),
          strtod(mean_REAL.c_str(),    &pEnd),
          strtod(mean_CS_MC.c_str(),   &pEnd),
          strtod(sig_CS_MCl.c_str(),   &pEnd),
          strtod(sig_CS_MCh.c_str(),   &pEnd),
          strtod(mean_CS_DATA.c_str(), &pEnd),
          strtod(sig_CS_DATAl.c_str(), &pEnd),      
          strtod(sig_CS_DATAh.c_str(), &pEnd));

    }
  }


  inputFile.close(); 
}

//********************************************************************
bool BinnedParams::GetBin(Float_t value, BinnedParamsBin*& bin){
  //********************************************************************

  if (_typeShort!=k1D) return false;

  Int_t idx=0;
  Int_t it = 0;
  for (it=0; it<_nBins;it++){
    if (value>=_bins[it]->_min && value<_bins[it]->_max){
      bin = _bins[it];
      bin->_index=idx;
      return true;
    }
    idx++;
  }

  return false;
}


//********************************************************************
bool BinnedParams::GetBin(Float_t value1, Float_t value2, BinnedParamsBin*& bin){
  //********************************************************************

  if (_typeShort!=k2D) return false;

  Int_t idx=0;
  for (Int_t it=0; it<_nBins;it++){
    if (value1>=_bins[it]->_min  && value1<_bins[it]->_max && value2>= _bins[it]->_min2 && value2 < _bins[it]->_max2 ){
      bin = _bins[it];
      bin->_index=idx;
      return true;
    }
    idx++;
  }

  return false;
}

//********************************************************************
bool BinnedParams::GetBin(Float_t value1, Float_t value2, Float_t value3, BinnedParamsBin*& bin){
  //********************************************************************

  if (_typeShort!=k3D) return false;

  Int_t idx=0;
  for (Int_t it=0; it<_nBins;it++){
    if (value1>=_bins[it]->_min  && value1<_bins[it]->_max 
        && value2>= _bins[it]->_min2 && value2 < _bins[it]->_max2
        && value3>= _bins[it]->_min3 && value3 < _bins[it]->_max3 ){
      bin = _bins[it];
      bin->_index=idx;
      return true;
    }
    idx++;
  }

  return false;
}


//********************************************************************
bool BinnedParams::GetBinIndex(Float_t value, int& index){
  //********************************************************************

  if (_typeShort!=k1D) return false;

  Int_t it = 0;
  Int_t idx=0;
  for (it=0; it<_nBins;it++){
    if (value>=_bins[it]->_min && value<_bins[it]->_max){
      index=idx;
      return true;
    }
    idx++;
  }

  return false;
}

//********************************************************************
bool BinnedParams::GetParametersForBin(Int_t index, Float_t& mean, Float_t& sigma){
  //********************************************************************

  Int_t it  = 0;
  Int_t idx = 0;
  for (it=0; it<_nBins;it++){
    if(index==idx){
      mean  =_bins[it]->_mean;
      sigma =_bins[it]->_sigma;
      return true;
    }
    idx++;
  }

  return false;
}


//********************************************************************
bool BinnedParams::GetParametersForBin(Int_t index, BinnedParamsParams& params){
  //********************************************************************

  Int_t it  = 0;
  Int_t idx = 0;
  for (it=0; it<_nBins;it++){
    if(index==idx){
      const BinnedParamsBin* bin = _bins[it];
      params.mean       = bin->_mean;
      params.sigma      = bin->_sigma;
      params.meanMCANA  = bin->_meanREAL;
      params.meanMC     = bin->_meanMC;
      params.sigmaMCl   = bin->_sigmaMCl;
      params.sigmaMCh   = bin->_sigmaMCh;
      params.meanDATA   = bin->_meanDATA;
      params.sigmaDATAl = bin->_sigmaDATAl;
      params.sigmaDATAh = bin->_sigmaDATAh;
      return true;
    }
    idx++;
  }

  return false;
}


//********************************************************************
bool BinnedParams::GetBinValues(Float_t value, Float_t& mean, Float_t& sigma, Int_t& index){
  //********************************************************************

  BinnedParamsBin* bin = NULL;
  if (!GetBin(value, bin)) return false;

  mean  = bin->_mean;
  sigma = bin->_sigma;
  index = bin->_index;

  return true;
}


//********************************************************************
bool BinnedParams::GetBinValues(Float_t value, BinnedParamsParams& params, Int_t& index){
  //********************************************************************

  BinnedParamsBin* bin = NULL;
  if (!GetBin(value, bin)) return false;

  params.mean       = bin->_mean;
  params.sigma      = bin->_sigma;
  params.meanMCANA  = bin->_meanREAL;
  params.meanMC     = bin->_meanMC;
  params.sigmaMCl   = bin->_sigmaMCl;
  params.sigmaMCh   = bin->_sigmaMCh;
  params.meanDATA   = bin->_meanDATA;
  params.sigmaDATAl = bin->_sigmaDATAl;
  params.sigmaDATAh = bin->_sigmaDATAh;

  index = bin->_index;

  return true;
}


//********************************************************************
bool BinnedParams::GetBinValues(Float_t value1, Float_t value2, Float_t& mean, Float_t& sigma, Int_t& index){
  //********************************************************************

  BinnedParamsBin* bin = NULL;
  if (!GetBin(value1, value2, bin)) return false;

  mean  = bin->_mean;
  sigma = bin->_sigma;
  index = bin->_index;

  return true;
}

//********************************************************************
bool BinnedParams::GetBinValues(Float_t value1, Float_t value2, BinnedParamsParams& params, Int_t& index){
  //********************************************************************

  BinnedParamsBin* bin = NULL;
  if (!GetBin(value1, value2, bin)) return false;

  params.mean       = bin->_mean;
  params.sigma      = bin->_sigma;
  params.meanMCANA  = bin->_meanREAL;
  params.meanMC     = bin->_meanMC;
  params.sigmaMCl   = bin->_sigmaMCl;
  params.sigmaMCh   = bin->_sigmaMCh;
  params.meanDATA   = bin->_meanDATA;
  params.sigmaDATAl = bin->_sigmaDATAl;
  params.sigmaDATAh = bin->_sigmaDATAh;

  index = bin->_index;

  return true;
}

//********************************************************************
bool BinnedParams::GetBinValues(Float_t value1, Float_t value2, Float_t value3, Float_t& mean, Float_t& sigma, Int_t& index){
  //********************************************************************

  BinnedParamsBin* bin = NULL;
  if (!GetBin(value1, value2, value3,  bin)) return false;

  mean  = bin->_mean;
  sigma = bin->_sigma;
  index = bin->_index;

  return true;
}

//********************************************************************
bool BinnedParams::GetBinValues(Float_t value1, Float_t value2, Float_t value3, BinnedParamsParams& params, Int_t& index){
  //********************************************************************

  BinnedParamsBin* bin = NULL;
  if (!GetBin(value1, value2, value3, bin)) return false;

  params.mean       = bin->_mean;
  params.sigma      = bin->_sigma;
  params.meanMCANA  = bin->_meanREAL;
  params.meanMC     = bin->_meanMC;
  params.sigmaMCl   = bin->_sigmaMCl;
  params.sigmaMCh   = bin->_sigmaMCh;
  params.meanDATA   = bin->_meanDATA;
  params.sigmaDATAl = bin->_sigmaDATAl;
  params.sigmaDATAh = bin->_sigmaDATAh;

  index = bin->_index;

  return true;
}

//********************************************************************
bool BinnedParams::UpdateEfficiencyCounter(Int_t index, bool correct){
  //********************************************************************
  if((UInt_t)index>=NMAXSYSTSOURCEBINS) {
    std::cout<<" problem size of the array is " << NMAXSYSTSOURCEBINS << ", bin index is "<<index<<std::endl;
    return false;
  }
  if (correct) _ncorrect[index]++;
  else         _nwrong[index]++;

  return true;
}

//********************************************************************
void BinnedParams::AddGraphBin(Float_t min, Float_t max, 
    Float_t mean, 
    Float_t sigma){
  //********************************************************************

  if (_nGraphBins == 0)
    _graphBins[_nGraphBins++] = new BinnedParamsGraphBin();

  _graphBins[0]->Update(min, max, mean, sigma);
  return; 

}

//********************************************************************
void BinnedParams::AddGraphBin(Float_t max, Float_t min, 
    Float_t meanREAL, 
    Float_t meanMC, 
    Float_t sigmaMCl,
    Float_t sigmaMCh,
    Float_t meanDATA, 
    Float_t sigmaDATAl,
    Float_t sigmaDATAh){
  //********************************************************************
  if (_nGraphBins == 0)
    _graphBins[_nGraphBins++] = new BinnedParamsGraphBin();

  _graphBins[0]->Update(min, max, 
      meanREAL, 
      meanMC, 
      sigmaMCl, 
      sigmaMCh, 
      meanDATA, 
      sigmaDATAl, 
      sigmaDATAh);
  return; 
}



//********************************************************************
void BinnedParams::AddGraphBin(Float_t min, Float_t max, Float_t min2, Float_t max2, 
    Float_t mean, 
    Float_t sigma){
  //********************************************************************

  //check whether a bin already exists
  BinnedParamsGraphBin* bin = NULL;
  Float_t value = (max + min)/2;

  if (!GetGraphBin(value, bin)){
    bin = new BinnedParamsGraphBin(min, max);
    _graphBins[_nGraphBins++] = bin;
  }

  bin->Update(min2, max2, 
      mean, 
      sigma); 

}

//********************************************************************
void BinnedParams::AddGraphBin(Float_t min, Float_t max, Float_t min2, Float_t max2,
    Float_t meanREAL, 
    Float_t meanMC, 
    Float_t sigmaMCl,
    Float_t sigmaMCh,
    Float_t meanDATA, 
    Float_t sigmaDATAl,
    Float_t sigmaDATAh){
  //********************************************************************

  //check whether a bin already exists
  BinnedParamsGraphBin* bin = NULL;
  Float_t value = (max + min)/2;

  if (!GetGraphBin(value, bin)){
    bin = new BinnedParamsGraphBin(min, max);
    _graphBins[_nGraphBins++] = bin;
  }

  bin->Update(min2, max2, 
      meanREAL,
      meanMC, 
      sigmaMCl,   
      sigmaMCh,   
      meanDATA,   
      sigmaDATAl, 
      sigmaDATAh); 
}

//********************************************************************
void BinnedParams::AddGraphBin(Float_t min, Float_t max, Float_t min2, Float_t max2, Float_t min3, Float_t max3,
    Float_t mean,
    Float_t sigma){
  //********************************************************************

  //check whether a bin already exists
  BinnedParamsGraphBin* bin = NULL;
  Float_t value1 = (max + min)/2;

  Float_t value2 = (max2 + min2)/2;


  if (!GetGraphBin(value1, value2,  bin)){
    bin = new BinnedParamsGraphBin(min, max, min2, max2);
    _graphBins[_nGraphBins++] = bin;
  }

  bin->Update(min3, max3, 
      mean, 
      sigma); 

}

//********************************************************************
void BinnedParams::AddGraphBin(Float_t min, Float_t max, Float_t min2, Float_t max2, Float_t min3, Float_t max3,
    Float_t meanREAL, 
    Float_t meanMC, 
    Float_t sigmaMCl,
    Float_t sigmaMCh,
    Float_t meanDATA, 
    Float_t sigmaDATAl,
    Float_t sigmaDATAh){
  //********************************************************************


  //check whether a bin already exists
  BinnedParamsGraphBin* bin = NULL;
  Float_t value1 = (max + min)/2;

  Float_t value2 = (max2 + min2)/2;


  if (!GetGraphBin(value1, value2,  bin)){
    bin = new BinnedParamsGraphBin(min, max, min2, max2);
    _graphBins[_nGraphBins++] = bin;
  }

  bin->Update(min3, max3, 
      meanREAL,
      meanMC, 
      sigmaMCl,   
      sigmaMCh,   
      meanDATA,   
      sigmaDATAl, 
      sigmaDATAh); 


}


//********************************************************************
bool BinnedParams::GetGraphBin(Float_t value1, BinnedParamsGraphBin*& bin){
  //********************************************************************

  if (_typeShort!=k2D) return false;

  Int_t idx=0;
  for (Int_t it=0; it<_nGraphBins;it++){
    if (value1>=_graphBins[it]->_min  && value1<_graphBins[it]->_max){
      bin = _graphBins[it];
      bin->_index=idx;
      return true;
    }
    idx++;
  }

  return false;
}

//********************************************************************
bool BinnedParams::GetGraphBin(Float_t value1, Float_t value2, BinnedParamsGraphBin*& bin){
  //********************************************************************

  if (_typeShort!=k3D) return false;

  Int_t idx=0;
  for (Int_t it=0; it<_nGraphBins;it++){
    if (value1>=_graphBins[it]->_min  && value1<_graphBins[it]->_max 
        && value2>= _graphBins[it]->_min2 && value2 < _graphBins[it]->_max2){
      bin = _graphBins[it];
      bin->_index=idx;
      return true;
    }
    idx++;
  }

  return false;
}

//********************************************************************
bool BinnedParams::GetInterBinValues(Float_t value1, BinnedParamsParams& params){
  //********************************************************************
  //check whether a bin already exists
  if (_nGraphBins == 0) return false;
  BinnedParamsGraphBin* bin = _graphBins[0];

  if (!bin) return false;

  return bin->GetParams(value1, params);
}

//********************************************************************
bool BinnedParams::GetInterBinValues(Float_t value1, Float_t value2, BinnedParamsParams& params){
  //********************************************************************

  //check whether a bin exists
  BinnedParamsGraphBin* bin = NULL; 

  if (!GetGraphBin(value1, bin)) return false;

  return bin->GetParams(value2, params);
}

//********************************************************************
bool BinnedParams::GetInterBinValues(Float_t value1, Float_t value2, Float_t value3, BinnedParamsParams& params){
  //********************************************************************

  //check whether a bin exists
  BinnedParamsGraphBin* bin = NULL; 

  if (!GetGraphBin(value1, value2, bin)) return false;

  return bin->GetParams(value3, params);

}

//********************************************************************
bool BinnedParamsGraphBin::GetParams(Float_t value, BinnedParamsParams& params){
  //********************************************************************

  if (!GetValue(  BinnedParamsGraphBin::kMean,       value, params.mean       ))
    return false;
  if (!GetValue(  BinnedParamsGraphBin::kSigma,      value, params.sigma      )) 
    return false;
  if (!GetValue(  BinnedParamsGraphBin::kMeanMCANA,  value, params.meanMCANA  ))
    return false;
  if (!GetValue(  BinnedParamsGraphBin::kMeanMC,     value, params.meanMC     )) 
    return false;
  if (!GetValue(  BinnedParamsGraphBin::kSigmaMCl,   value, params.sigmaMCl   ))
    return false;
  if (!GetValue(  BinnedParamsGraphBin::kSigmaMCh,   value, params.sigmaMCh   ))
    return false;
  if (!GetValue(  BinnedParamsGraphBin::kMeanDATA,   value, params.meanDATA   ))
    return false;
  if (!GetValue(  BinnedParamsGraphBin::kSigmaDATAl, value, params.sigmaDATAl )) 
    return false;
  if (!GetValue(  BinnedParamsGraphBin::kSigmaDATAh, value, params.sigmaDATAh ))
    return false;

  return true;

}

