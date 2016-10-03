#include "VersioningUtils.hxx"

namespace versionUtils{

  bool prod6_POT        =false;
  bool prod6_bunching   =false;
  bool prod6_corrections=false;
  bool prod6_systematics=false;
}

//***************************************************
void versionUtils::DumpProductions(){
//***************************************************

  if (versionUtils::prod6_POT)    
    std::cout << "Using Prod6 for POT" << std::endl;
  else
    std::cout << "Using Prod5 for POT" << std::endl;

  if (versionUtils::prod6_bunching)    
    std::cout << "Using Prod6 for bunching" << std::endl;
  else
    std::cout << "Using Prod5 for bunching" << std::endl;

  if (versionUtils::prod6_corrections)    
    std::cout << "Using Prod6 for corrections" << std::endl;
  else
    std::cout << "Using Prod5 for corrections" << std::endl;

  if (versionUtils::prod6_systematics)    
    std::cout << "Using Prod6 for systematics" << std::endl;
  else
    std::cout << "Using Prod5 for systematics" << std::endl;

}

//**************************************************
std::string versionUtils::Extension(bool useProdExt){
//**************************************************


  // Extension base of the production 
  std::string prodExt="";
  if (useProdExt){
    if (versionUtils::prod6_systematics)
      prodExt="_p6B";
    else
      prodExt="_p5F";
  }

  return prodExt;
}

