#include "VersionManager.hxx"
#include "BasicTypes.hxx"
#include <stdio.h>
#include <iostream>

 /// The static member pointer to the singleton.
Versioning* Versioning::_versioning = NULL;

ClassImp(PackageVersion)

//***********************************************************
PackageVersion::PackageVersion(){
//***********************************************************

  _name = "";
  _version = "";
}

//***********************************************************
PackageVersion::PackageVersion(const std::string& name, const std::string& version){
//***********************************************************

  _name = name;
  _version = version;
}

//********************************************************************
Versioning&  ND::versioning(){
//********************************************************************

  return Versioning::Get();
}

//*****************************************************************************
Versioning& Versioning::Get(void) {
  //*****************************************************************************
  
  if (!_versioning) {
    _versioning = new Versioning;
  }
  
  return *_versioning;
}


//********************************************************************
Versioning::Versioning(): HLClonesArray("config","VERSION","PackageVersion",NMAXPACKAGES){
//********************************************************************

  _packages.clear();
}

//********************************************************************
void Versioning::AddProduction(ProdId_h prodId, const std::string& prodName, const std::string& prodLowVersion, const std::string& prodHighVersion){
//********************************************************************

  _prodId.push_back(prodId);
  _prodName.push_back(prodName);
  _prodLowVersion.push_back(prodLowVersion);
  _prodHighVersion.push_back(prodHighVersion);
}

//********************************************************************
ProdId_h Versioning::GetProduction(const std::string softVersion){
//********************************************************************

  for (UInt_t i=0;i<_prodId.size();i++){
    if( !CheckBeforeVersion(softVersion, _prodLowVersion[i]) && CheckBeforeVersion(softVersion, _prodHighVersion[i]))
      return _prodId[i];
  }    

  return UNKNOWN_PRODUCTION;
}

//********************************************************************
std::string Versioning::GetProductionName(const std::string softVersion){
//********************************************************************

  return ConvertProduction(GetProduction(softVersion));
}

//********************************************************************
std::string Versioning::ConvertProduction(ProdId_h prod){
//********************************************************************

  for (UInt_t i=0;i<_prodId.size();i++){
    if (prod == _prodId[i]) return _prodName[i];
  }    
  return "unknown";
}

//********************************************************************
bool Versioning::GetSoftwareVersionRangeForProduction(ProdId_h prodId, std::string& lowVersion, std::string& highVersion){
//********************************************************************

  for (UInt_t i=0;i<_prodId.size();i++){
    if (_prodId[i] == prodId){
      lowVersion = _prodLowVersion[i];
      highVersion= _prodHighVersion[i];
      return true;
    }
  }    

  return false;
}


//********************************************************************
bool Versioning::CheckVersionCompatibility(const std::string& version, ProdId_h prodND280){
//********************************************************************

  bool ok = true;
  std::string prodLow, prodHigh;
    
  // Get the version ranges for a given production
  GetSoftwareVersionRangeForProduction(prodND280, prodLow, prodHigh);

  // Get the production name from the software version of the input file
  ProdId_h prodInputFile = GetProduction(version);


  std::cerr << "nd280AnalysisTools compiled with " << ConvertProduction(prodND280) << " (software version: " << prodLow << " - " << prodHigh << ")  file. " << std::endl;
  std::cerr << "Checking for compatibility version with current file  (software version: " << version << ")" << std::endl;
  if (prodInputFile != prodND280){
    ok = false;
    std::cerr << "*************************************************" << std::endl;
    std::cerr << "*************************************************" << std::endl;
    std::cerr << "WARNING: Running over a " << ConvertProduction(prodInputFile) << "  file !!!!!" << std::endl;
    std::cerr << "oaAnalysis data classes definition could be incompatible !!!!" << std::endl;
    std::cerr << "You should compile nd280AnalysisTools with the proper file and then recompile highland2 from scratch !!!" << std::endl;
    std::cerr << "You could also disable version check with command line option -v" << std::endl;
    std::cerr << "*************************************************" << std::endl;
    std::cerr << "*************************************************" << std::endl;
  }
  else
    std::cout << "---> OK. Running over a " << ConvertProduction(prodND280) << " file" << std::endl;    

  return ok;

}

//********************************************************************
bool Versioning::CheckBeforeVersion(const std::string& version, const std::string& version0){
//********************************************************************

  int v,r,p;
  int v0,r0,p0;
  ParseSoftwareVersion(version,  v, r, p);
  ParseSoftwareVersion(version0, v0,r0,p0);

  if (v<v0) return true;
  else if (v==v0){
    if (r<r0) return true;
    else if (r==r0){
      if (p<p0) return true;
    }
  }
    
  return false;
}


//********************************************************************
void Versioning::ParseSoftwareVersion(const std::string& version, int& v, int& r, int& p) {
//********************************************************************
 
  int vloc = version.find("v");
  int rloc = version.find("r");
  int ploc = version.find("p");
  int size = version.length();

  if ((size_t)vloc == std::string::npos) {
    std::cerr << "*************************************************" << std::endl;
    std::cerr << "*************************************************" << std::endl;
    std::cerr << "WARNING: SoftwareVersion '" << version << "' is not a valid format. This is probably and OLD highland FlatTree !" << std::endl;
    std::cerr << "You could disable version check with command line option -v" << std::endl;
    std::cerr << "*************************************************" << std::endl;
    std::cerr << "*************************************************" << std::endl;
    exit(3);
  }

  if ((size_t)rloc == std::string::npos) {
    // vX
    v = std::atoi(version.substr(vloc+1, size - vloc-1).c_str());
    r = 0;
    p = 0;
  } else if ((size_t)ploc == std::string::npos) {
    // vXrY
    v = std::atoi(version.substr(vloc+1, rloc - vloc-1).c_str());
    r = std::atoi(version.substr(rloc+1, size - rloc-1).c_str());
    p = 0;
  } else {
    // vXrYpZ
    v = std::atoi(version.substr(vloc+1, rloc - vloc-1).c_str());
    r = std::atoi(version.substr(rloc+1, ploc - rloc-1).c_str());
    p = std::atoi(version.substr(ploc+1, size - ploc-1).c_str());
  }
}

//***********************************************************
void Versioning::AddPackage(const std::string& name, const std::string& version){
//***********************************************************

  for (UInt_t i=0;i<_packages.size();i++){
    if (_packages[i]->Name() == name){
      if (_packages[i]->Version() != version){
	std::cerr << "*************************************************" << std::endl;
	std::cerr << "*************************************************" << std::endl;
	std::cerr << "ERROR: Package '" << name << "' has been added twice with two different versions "<< _packages[i]->Version() << " and " << version << " !!!!" << std::endl;
	std::cerr << "*************************************************" << std::endl;
	std::cerr << "*************************************************" << std::endl;
	exit(3);
      }
      else return;
    }
  }

  PackageVersion* pack = new PackageVersion(name, version);
  _packages.push_back(pack);
  (*_objects)[_NObjects++] = pack;
}

//********************************************************************
void Versioning::ReadVersions(const std::string& file){
//********************************************************************

  // Reset the vectors
  _packages.clear();

  ReadClonesArray(file);

  // Add them to the map of categories
  for (int i=0;i<_NObjects;i++){
    PackageVersion* pack = (PackageVersion*)(*_objects)[i];    
    _packages.push_back(pack);
  }
}

//********************************************************************
void Versioning::DumpVersions(){
//********************************************************************

  std::cout << " ------ List of highland2 package Versions  ---------" << std::endl;
  char out[256];  
  sprintf(out,"%3s: %-30s %-15s", "#", "package", "version");
  std::cout << out << "\n" << std::endl;

  for (UInt_t i=0;i<_packages.size();i++){
    PackageVersion* pack = _packages[i];    
    sprintf(out,"%3d: %-30s %-15s", i, pack->Name().c_str(), pack->Version().c_str());
    std::cout << out << std::endl;
  }
  std::cout << " ----------------------------------------------------" << std::endl;
}

