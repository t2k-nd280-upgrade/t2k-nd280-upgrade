#ifndef VersioningUtils_h
#define VersioningUtils_h

#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>

namespace versionUtils {  
  extern bool prod6_POT;    
  extern bool prod6_bunching;    
  extern bool prod6_corrections;    
  extern bool prod6_systematics;    


  void DumpProductions();

  /// Get the extension to be used in the BinnedParams file (filename = syst_name+extension.dat)
  std::string Extension(bool useProdExt=true);  
  
}


#endif


