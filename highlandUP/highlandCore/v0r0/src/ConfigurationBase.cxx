#include "ConfigurationBase.hxx"

//********************************************************************
ConfigurationBase::ConfigurationBase(){
//********************************************************************

  _name="";
  _ntoys=1;
  _enabled = false;
  _toyEnabled.resize(1);
  _systematicsEnabled.clear();
  _weightsEnabled.clear();
  _variationsEnabled.clear();
  _toy_randomSeed=-1;    
  _toyMaker=NULL;
}

//********************************************************************
ConfigurationBase::ConfigurationBase(Int_t index, const std::string& name, UInt_t ntoys, Int_t randomSeed, ToyMaker* toyMaker){
//********************************************************************
  
  _conf_index = index;
  _name=name;
  _ntoys=ntoys;
  _toy_ref=0;
  _enabled = true;
  _toyEnabled.resize(ntoys);
  _systematicsEnabled.clear();
  _weightsEnabled.clear();
  _variationsEnabled.clear();
  _toy_randomSeed=randomSeed;
  _toyMaker=toyMaker;
}

//********************************************************************
void ConfigurationBase::Dump(SystematicManager& syst){
//********************************************************************

  std::cout << "*********** Configuration: " << Name() << " *************" << std::endl;

  std::cout << " enabled:     " << (Int_t)IsEnabled() << std::endl;
  std::cout << " NVariations: " << GetEnabledEventVariations().size() << std::endl;
  std::cout << " NWeights:    " << GetEnabledEventWeights().size() << std::endl;
  std::cout << " NSyst:       " << GetEnabledSystematics().size() << std::endl;
  std::cout << " NToys:       " << GetNToys() << std::endl;
  std::cout << " Random seed: " << GetToyRandomSeed() << std::endl;

  if (GetEnabledSystematics().size()==0) return;

  // Dump the systematics for this configuration
  syst.DisableAllSystematics();
  syst.EnableSystematics(GetEnabledSystematics());
  syst.DumpVariationSystematics();
  syst.DumpWeightSystematics();
  syst.EnableAllSystematics();

}

//********************************************************************
void ConfigurationBase::EnableSystematics(const std::vector<Int_t>& indices){
//********************************************************************
  
  // use a set such that the same systematic is not added twice
  std::set<Int_t> enabled;

  // Insert all systematics that are already enabled
  for (std::vector<Int_t>::iterator it = _systematicsEnabled.begin();it!=_systematicsEnabled.end();it++)
    enabled.insert(*it);

  // Insert the systematics to enable
  for (std::vector<Int_t>::const_iterator it = indices.begin();it!=indices.end();it++)
    enabled.insert(*it);

  // Copy the sent into the vector of _systematicsEnabled
  _systematicsEnabled.clear();
  for (std::set<Int_t>::iterator it = enabled.begin();it!=enabled.end();it++)
    _systematicsEnabled.push_back(*it);
}

//********************************************************************
void ConfigurationBase::CreateToyExperiments(const SystematicManager& syst){
//********************************************************************

  // Creat the toy experiments
  if (!_toyMaker) return;
  _toyMaker->CreateToyExperiments(_ntoys,syst.GetSystematics(_systematicsEnabled));

  // Fill the structure to allow saving variations in the config tree
  const std::vector<ToyExperiment*>& toys = _toyMaker->GetToyExperiments();
  for (std::vector<ToyExperiment*>::const_iterator it=toys.begin();it!=toys.end();it++){
    ToyExperiment& toy = **it;    
    ToyVariationWrite wtoy;
    for (UInt_t i = 0; i<NMAXSYSTEMATICS;i++){
      if (!toy.GetToyVariations(i)) continue;
      for(UInt_t j = 0; j < toy.GetToyVariations(i)->GetNParameters(); ++j){ 
        if (wtoy.npar>=NMAXPARAMETERS) return;
        wtoy.variations[wtoy.npar] = toy.GetToyVariations(i)->Variations[j];
        wtoy.weights[wtoy.npar]    = toy.GetToyVariations(i)->Weights[j];
        wtoy.par_index[wtoy.npar]  = wtoy.npar;
        wtoy.npar++;
      }
    }
    for (UInt_t ipar2 = wtoy.npar;ipar2<NMAXPARAMETERS;ipar2++){
      wtoy.variations[ipar2] = 0;
      wtoy.weights[ipar2]    = 0;
      wtoy.par_index[ipar2]  = -1;
    }
    _toys.push_back(wtoy);
  }

}

