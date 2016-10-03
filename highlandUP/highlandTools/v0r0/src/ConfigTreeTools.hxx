#ifndef ConfigTreeTools_h
#define ConfigTreeTools_h

#include <stdio.h>
#include <iostream>
#include "SystematicManager.hxx"
#include "ConfigurationManager.hxx"

class ConfigTreeTools{
 public :

  ConfigTreeTools(){}
  ConfigTreeTools(SystematicManager& syst, ConfigurationManager& conf);

  // Some features need those managers
  void Initialize(SystematicManager& syst, ConfigurationManager& conf);

  virtual ~ConfigTreeTools(){}
  
  /// Get the Index of a given weight for a given configuration in the micro-trees. 
  /// To compute that it uses the config tree
  Int_t GetWeightIndex(const std::string& conf, const std::string& name) const;
  Int_t GetWeightIndex(Int_t conf, Int_t weight) const;

protected:

  SystematicManager* _syst;
  ConfigurationManager* _conf;

};



#endif


