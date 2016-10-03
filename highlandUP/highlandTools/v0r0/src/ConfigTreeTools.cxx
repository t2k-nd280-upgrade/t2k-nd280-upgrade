#include "ConfigTreeTools.hxx"

//********************************************************************
ConfigTreeTools::ConfigTreeTools(SystematicManager& syst, ConfigurationManager& conf){
//********************************************************************

  Initialize(syst,conf);
}

//********************************************************************
void ConfigTreeTools::Initialize(SystematicManager& syst, ConfigurationManager& conf){
//********************************************************************

  _syst = &syst;
  _conf = &conf;
}

//********************************************************************
Int_t ConfigTreeTools::GetWeightIndex(const std::string& conf, const std::string& name) const{
//********************************************************************

  if (!_conf->GetConfiguration(conf)) return -1;
  if (_conf->GetConfiguration(conf)->GetEnabledSystematics().size()==0) return -1;

  _syst->DisableAllSystematics();
  _syst->EnableSystematics(_conf->GetConfiguration(conf)->GetEnabledSystematics());

  Int_t nSyst;
  EventWeightBase** systs = _syst->GetWeightSystematics(nSyst);
  Int_t j=0;
  for (int it = 0; it < nSyst; it++) {
    if (systs[it]->IsEnabled()){
      if (systs[it]->Name() == name) return j;
      j++;
    }
  }
  return -1;
}

//********************************************************************
Int_t ConfigTreeTools::GetWeightIndex(Int_t conf, Int_t weight) const{
//********************************************************************

  if (!_conf->GetConfiguration(conf)) return -1;
  if (_conf->GetConfiguration(conf)->GetEnabledSystematics().size()==0) return -1;

  _syst->DisableAllSystematics();
  _syst->EnableSystematics(_conf->GetConfiguration(conf)->GetEnabledSystematics());

  Int_t nSyst;
  EventWeightBase** systs = _syst->GetWeightSystematics(nSyst);
  Int_t j=0;
  for (int it = 0; it < nSyst; it++) {
    if (systs[it]->IsEnabled()){
      if (systs[it]->GetIndex() == weight) return j;
      j++;
    }
  }
  return -1;
}
