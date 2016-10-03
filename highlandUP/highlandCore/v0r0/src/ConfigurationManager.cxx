#include "ConfigurationManager.hxx"
#include <DocStringManager.hxx>
#include <sstream>
#include <stdlib.h>


//********************************************************************
ConfigurationManager::ConfigurationManager():HLClonesArray("config","CONF","ConfigurationBase",NMAXCONFIGURATIONS) {
//********************************************************************

  _current_conf=-1;
  _toy_index=0; 
  _confs.resize(NMAXCONFIGURATIONS);

}

//********************************************************************
ConfigurationManager::ConfigurationManager(const std::string& file, double norm, const std::string& cut):HLClonesArray("config","CONF","ConfigurationBase",NMAXCONFIGURATIONS) {
//********************************************************************

  (void)norm;
  (void)cut;
  _current_conf=-1;
  _toy_index=0; 
  ReadConfigurations(file);
}

//***********************************************************
ConfigurationManager::~ConfigurationManager() {
//***********************************************************
  for (std::vector<ConfigurationBase*>::iterator it = _confs_nonull.begin(); it != _confs_nonull.end(); it++) {
    delete *it;
  }

  _confs_nonull.clear();
  _confs.clear();
}

//**************************************************
void ConfigurationManager::AddConfiguration(Int_t index, const std::string& conf_name, UInt_t ntoys, Int_t randomSeed, ToyMaker* toyMaker){
//**************************************************

  if (index >= (Int_t)NMAXCONFIGURATIONS){
    std::cout << "ERROR in ConfigurationManager::AddConfiguration(). The index for configuration '" << conf_name << "' with value " 
              << index << " exceeds the maximum number of allowed configurations, which is " << NMAXCONFIGURATIONS << std::endl;
    exit(1);
  }
  
  // Create the new configuration with a given name
  ConfigurationBase* conf = new ConfigurationBase(index, conf_name, ntoys, randomSeed, toyMaker);

  // Add it to the vector of configurations
  _confs_nonull.push_back(conf);

  // Add it to the vector of configurations
  _confs[index] = conf;

  // Add it to the ClonesArray
  (*_objects)[_NObjects++] = conf;

  //  AddVar(conf,"toy_ref","I");

  // The reference toy is the first one by default
  //  SetRefToyIndex(conf,0);

  //  syst().AddConfiguration(conf);

}

//********************************************************************
void ConfigurationManager::ReadConfigurations(const std::string& file){
//********************************************************************

  // Reset the vectors
  _confs_nonull.clear();
  _confs.clear();
  _confs.resize(NMAXCONFIGURATIONS);

  ReadClonesArray(file);

  // Add them to the map of categories
  for (int i=0;i<_NObjects;i++){
    ConfigurationBase* conf = (ConfigurationBase*)(*_objects)[i];    
    _confs_nonull.push_back(conf);
    if (conf->GetIndex() >= (Int_t)NMAXCONFIGURATIONS){
      std::cout << "WARNING in ConfigurationManager::ReadConfigurations(). The index for configuration '" << conf->Name() << "' with value " 
                << conf->GetIndex() << " exceeds the maximum number of allowed configurations, which is " << NMAXCONFIGURATIONS << std::endl;
      std::cout << "  ---> Configuration not added !!!" << std::endl; 
      continue;
    }
    _confs[conf->GetIndex()] = conf;
  }
}

//********************************************************************
//void ConfigurationManager::ReadConfigurations(const std::string& file){
//********************************************************************
/*
  // Load a tree from a file
  ReadFile(file);

  int NTOYS;
  int toy_ref;
  
  std::map< std::string, TTree* >::iterator cit;
  for (cit= GetTrees().begin();cit!=GetTrees().end();cit++){    
    std::string conf = cit->first;
    
    if (!IsSpecialTree(conf)) {
      // Header and config are special trees, which don't have the same branches
      // as the actual configuration trees.
      GetTree(conf)->SetBranchAddress("NTOYS", &NTOYS);
      GetTree(conf)->SetBranchAddress("toy_ref", &toy_ref);

      if (GetTree()->GetEntries() > 0) {
        GetTree(conf)->GetEntry(0);
      } else {
        std::cout << "Warning: tree " << conf << " has no entries!" << std::endl;
        NTOYS = 1;
        toy_ref = 0;
      }

      // 1 single toy by default
      SetNToys(conf, NTOYS);

      // The reference toy is the first one by default
      SetRefToyIndex(conf, toy_ref);

      // Current configuration is the last one
      SetCurrentConfiguration("default");
    }
  }
}
*/  
//**************************************************
int ConfigurationManager::GetNMaxToys(){
//**************************************************

  int nmax =1;
  for (std::vector<ConfigurationBase* >::iterator it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
    if ((*it)->GetNToys() > nmax) nmax = (*it)->GetNToys();
  }

  return nmax;
}

//**************************************************
void ConfigurationManager::EnableAllConfigurations(){
//**************************************************

  for (std::vector<ConfigurationBase* >::iterator it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
    (*it)->SetEnabled(true);
  }
}

//**************************************************
void ConfigurationManager::DisableAllConfigurations(){
//**************************************************
  
  for (  std::vector<ConfigurationBase* >::iterator it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
    (*it)->SetEnabled(false);
  }
}

//*********************************************************
//void TreeManager::SetNToys(const std::string& tree, int NTOYS){
//*********************************************************
/*
  _conf_ntoys[tree]=NTOYS; 
  _ana_enabled[conf] = std::vector<bool>(); 
  _ana_enabled[conf].resize(NTOYS);
  EnableAllToys(conf);

  if(HasCounter(tree, "NTOYS"))
    ResizeCounter(tree, "NTOYS",NTOYS);

  // Delete the variables that depend on the number of toys
  DeleteVar(tree,"toy_index");
  DeleteVar(tree,"toy_weight");

  // Add the variables again with the correct size
  AddAnalysisVar(tree,"toy_index","I");
  AddAnalysisVar(tree,"toy_weight","D");
}


*/

//**************************************************
Int_t ConfigurationManager::GetConfigurationIndex(const std::string& conf_name){
//**************************************************

  for (std::vector<ConfigurationBase* >::iterator it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
    if ((*it)->Name() == conf_name) return (*it)->GetIndex();
  }

  return -1;
}

//**************************************************
void ConfigurationManager::SetCurrentConfigurationName(const std::string& conf_name){
//**************************************************

  for (std::vector<ConfigurationBase* >::iterator it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
    if ((*it)->Name() == conf_name) SetCurrentConfigurationIndex((*it)->GetIndex());
    break;
  }
}

//********************************************************************
void ConfigurationManager::DumpConfigurations(SystematicManager* syst){
//********************************************************************

  std::cout << " -------- List of Configurations  ----------------------------------" << std::endl;
  char out[256];  
  sprintf(out,"%3s: %-25s %5s %8s %8s %12s", "#", "name", "NToys", "enabled", "NSyst", "RandomSeed");
  std::cout << out <<"\n" << std::endl;

  for (UInt_t i=0;i<GetConfigurations().size();i++){
    ConfigurationBase* conf = GetConfigurations()[i];    
    sprintf(out,"%3d: %-25s %5d %8d %8d %12d", i, conf->Name().c_str(), conf->GetNToys(), (Int_t)conf->IsEnabled(), (Int_t)conf->GetEnabledSystematics().size(), conf->GetToyRandomSeed());
    std::cout << out << std::endl;
  }
  std::cout << " -------------------------------------------------------------------" << std::endl;


  if (!syst) return;

  for (UInt_t i=0;i<GetConfigurations().size();i++){
    ConfigurationBase* conf = GetConfigurations()[i];    
    conf->Dump(*syst);
  }
}

//********************************************************************
void ConfigurationManager::CreateToyExperiments(const SystematicManager& syst){
//********************************************************************

  for (std::vector<ConfigurationBase* >::iterator it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
    (*it)->CreateToyExperiments(syst);
  }

}

