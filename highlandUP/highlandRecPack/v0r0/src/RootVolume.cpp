#include "RootVolume.h"


//*****************************************************************************
bool RootVolume::is_in_name(const Key& name){
//*****************************************************************************

  bool ok = true;
  int n = conditions().size();
  
  for (int i=0;i<n; i++){
    //    std::cout << i << " " << conditions()[i] << std::endl;
    if (name.find(conditions()[i]) == std::string::npos){
      ok = false;
      break;
    }
  }

  //  std::cout << n-1 << " " << conditions()[n-1] << std::endl;
  if (name.find(conditions()[n-1]+"/") != std::string::npos) ok = false;

  
  //      if (name.find(name1) != std::string::npos &&
  //	  name.find(name1+"/") == std::string::npos ) {

  return ok;

}
