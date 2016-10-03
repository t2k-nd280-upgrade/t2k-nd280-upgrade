#include "ToyMaker.hxx"

//******************************************************************
ToyMaker::ToyMaker(){
//******************************************************************

  _toys.clear();

  _nSystematics=0;

  for (UInt_t isyst = 0; isyst<NMAXSYSTEMATICS;isyst++)
    _systematics[isyst] = NULL;
}

//******************************************************************
ToyMaker::~ToyMaker(){
//******************************************************************
  
  for (std::vector<ToyExperiment*>::iterator it=_toys.begin();it!=_toys.end();it++){
    if (*it) delete *it;
  }
  _toys.clear();
}

//***********************************************************
void ToyMaker::AddSystematic(SystematicBase* sys) {
//***********************************************************
  
  _systematics[sys->GetIndex()] = sys;
  _nSystematics++;
}

//********************************************************************
void ToyMaker::CreateToyExperiments(Int_t ntoys, const std::vector<SystematicBase*>& systematicsEnabled){
//********************************************************************

  if (systematicsEnabled.size()==0) return;

  // Add the enabled systematics
  for (UInt_t i=0;i<systematicsEnabled.size();i++){
    // Make sure the systematic with a given index exist, since the position in the vector is the Index of the systematics in SystId and not the order in which they are added 
    // (It depends on how systematicsEnabled was filled)
    if (systematicsEnabled[i])
      AddSystematic(systematicsEnabled[i]);
  }
  
  // Create the variations for ntoys
  for (Int_t itoy= 0; itoy<ntoys; itoy++){
    
    // Copy the structure of the template toy
    ToyExperiment* toy = new ToyExperiment();
    
    // Fill the ToyExperiment using the user defined ToyMaker
    FillToyExperiment(*toy);
    
    // put it into the vector of toys
    _toys.push_back(toy);
  }

}



