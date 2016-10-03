#ifndef ConfigurationManager_h
#define ConfigurationManager_h

#include <stdio.h>
#include <iostream>
#include <ConfigurationBase.hxx>
#include <HLClonesArray.hxx>


/// The maximum number of configurations that is supported.
const int NMAXCONFIGURATIONS=50;

class ConfigurationManager: public HLClonesArray {
public :

  ConfigurationManager();
  ConfigurationManager(const std::string& file, double norm, const std::string& cut);
  virtual ~ConfigurationManager();
  

  //----------- Functions to control configurations ------------------

  /// return the vector of configurations
  std::vector<ConfigurationBase*>& GetConfigurations(){return _confs_nonull;}

  /// return the name of the current configuration
  const std::string& GetCurrentConfigurationName(){return _confs[_current_conf]->Name();}

  /// return the current configuration
  ConfigurationBase* GetCurrentConfiguration() const{return _confs[_current_conf];}

  /// return the configuration with a given index
  ConfigurationBase* GetConfiguration(Int_t index) const{
    if (index==-1) return GetCurrentConfiguration();
    else return _confs[index];
  }

  /// return the configuration with a given index
  ConfigurationBase* GetConfiguration(const std::string& conf){
    return GetConfiguration(GetConfigurationIndex(conf));
  }

  /// return the index of the current configuration
  Int_t GetCurrentConfigurationIndex() const{return _current_conf;}

  /// Set the name of the current configuration
  void SetCurrentConfigurationName(const std::string& conf);

  /// Set the index of the current configuration
  void SetCurrentConfigurationIndex(Int_t conf){_current_conf=conf;}

  /// Returs the index of a configuration with name
  Int_t GetConfigurationIndex(const std::string& conf_name);

  /// Returs the name of a configuration with index
  const std::string& GetConfigurationName(Int_t index){return _confs[index]->Name();}

  /// Add a new configuration
  void AddConfiguration(Int_t index, const std::string& conf, UInt_t ntoys=1, Int_t randomSeed=-1, ToyMaker* toyMaker=NULL);

  /// Check if a configuration is enabled
  bool ConfigurationEnabled(const std::string& conf){return _confs[GetConfigurationIndex(conf)]->IsEnabled();}

  /// Check if a configuration is enabled
  bool ConfigurationEnabled(Int_t conf){return _confs[conf]->IsEnabled();}

  /// Enable/Disable configurations
  void EnableConfiguration(const std::string& conf) {_confs[GetConfigurationIndex(conf)]->SetEnabled(true);}
  void DisableConfiguration(const std::string& conf){_confs[GetConfigurationIndex(conf)]->SetEnabled(false);}
  void EnableConfiguration(Int_t conf) {_confs[conf]->SetEnabled(true);}
  void DisableConfiguration(Int_t conf){_confs[conf]->SetEnabled(false);}
  void DisableAllConfigurations();
  void EnableAllConfigurations();

  /// Read configurations from a file
  void ReadConfigurations(const std::string& file);


  /// Enable/disable toys
  bool GetToyEnabled(const std::string& conf, int toy){return _confs[GetConfigurationIndex(conf)]->GetToyEnabled(toy);}
  void EnableToy(const std::string& conf, int toy) {_confs[GetConfigurationIndex(conf)]->SetToyEnabled(toy, true);}
  void DisableToy(const std::string& conf, int toy){_confs[GetConfigurationIndex(conf)]->SetToyEnabled(toy, false);}
  void DisableAllToys(const std::string& conf){for (int i=0;i<GetNToys(conf);i++) DisableToy(conf,i);}
  void EnableAllToys(const std::string& conf) {for (int i=0;i<GetNToys(conf);i++) EnableToy(conf,i);}

  int GetNToys(const std::string& conf){return _confs[GetConfigurationIndex(conf)]->GetNToys();}
  int GetNToys(){return GetCurrentConfiguration()->GetNToys();}

  /// Sets the number of toys to a configuration with name
  void SetNToys(const std::string& conf, int ntoy){_confs[GetConfigurationIndex(conf)]->SetNToys(ntoy);}

  /// Sets the number of toys to a configuration with index
  void SetNToys(Int_t index, int ntoy){_confs[index]->SetNToys(ntoy);}

  /// Sets the Toy random seed to a configuration with index
  void SetToyRandomSeed(Int_t index, Int_t seed){_confs[index]->SetToyRandomSeed(seed);}

  /// Get the maximum number of toys in all configurations
  int GetNMaxToys();

  /// Set and gets the index of the current toy
  void SetToyIndex(int index){_toy_index=index;}
  int GetToyIndex(){return _toy_index;}


  /// Enable the systematic registered with the given index
  void EnableSystematic(Int_t syst, Int_t conf=-1){GetConfiguration(conf)->EnableSystematic(syst);}

  /// Enable the systematics registered with the given indices
  void EnableSystematics(const std::vector<Int_t>& systs, Int_t conf=-1){GetConfiguration(conf)->EnableSystematics(systs);}
  
  /// Disable the systematic registered with the given index
  void DisableSystematic(Int_t index){(void)index;}
  
  /// Disable the systematic registered with the given index
  void DisableAllSystematic(Int_t conf=-1){GetConfiguration(conf)->DisableAllSystematics();}

  /// Get the systematics that are enabled for a given configuration
  const std::vector<Int_t>& GetEnabledSystematics(Int_t conf=-1){return GetConfiguration(conf)->GetEnabledSystematics();}


  /// Enable the systematic registered with the given index
  void EnableEventWeight(Int_t syst, Int_t conf=-1){GetConfiguration(conf)->EnableEventWeight(syst);}

  /// Enable the systematics registered with the given indices
  void EnableEventWeights(const std::vector<Int_t>& systs, Int_t conf=-1){GetConfiguration(conf)->EnableEventWeights(systs);}
  
  /// Disable the systematic registered with the given index
  void DisableEventWeight(Int_t index){(void)index;}
  
  /// Disable the systematic registered with the given index
  void DisableAllEventWeight(Int_t conf=-1){GetConfiguration(conf)->DisableAllEventWeights();}

  /// Get the systematics that are enabled for a given configuration
  const std::vector<Int_t>& GetEnabledEventWeights(Int_t conf=-1){return GetConfiguration(conf)->GetEnabledEventWeights();}


  /// Enable the systematic registered with the given index
  void EnableEventVariation(Int_t syst, Int_t conf=-1){GetConfiguration(conf)->EnableEventVariation(syst);}

  /// Enable the systematics registered with the given indices
  void EnableEventVariations(const std::vector<Int_t>& systs, Int_t conf=-1){GetConfiguration(conf)->EnableEventVariations(systs);}
  
  /// Disable the systematic registered with the given index
  void DisableEventVariation(Int_t index){(void)index;}
  
  /// Disable the systematic registered with the given index
  void DisableAllEventVariation(Int_t conf=-1){GetConfiguration(conf)->DisableAllEventVariations();}

  /// Get the systematics that are enabled for a given configuration
  const std::vector<Int_t>& GetEnabledEventVariations(Int_t conf=-1){return GetConfiguration(conf)->GetEnabledEventVariations();}


  /// Dump summary info about all configurations
  void DumpConfigurations(SystematicManager* syst=NULL);

  /// Create the ToyExperiments using the ToyMaker and the SystematicManager
  void CreateToyExperiments(const SystematicManager& syst);

  enum enumConf_ConfigurationManager{
    default_conf=0,
    enumConfLast_ConfigurationManager    
  };


 protected:

  /// current toy index
  int _toy_index; 

  /// Current configuration index
  Int_t _current_conf;

  /// configurations
  std::vector<ConfigurationBase* > _confs;

  /// no NULL configurations
  std::vector<ConfigurationBase* > _confs_nonull;

};

#endif


