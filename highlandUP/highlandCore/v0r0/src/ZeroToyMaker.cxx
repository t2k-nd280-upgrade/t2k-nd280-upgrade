#include "ZeroToyMaker.hxx"
#include "Parameters.hxx"

//******************************************************************
ZeroToyMaker::ZeroToyMaker():ToyMaker(){
//******************************************************************
}

//******************************************************************
void ZeroToyMaker::FillToyExperiment(ToyExperiment& toy){
//******************************************************************
  
  // Set the same weight (1) for al toys. This will be later normalized to the number of toys
  Float_t weight = 1.;


  for (UInt_t isyst = 0; isyst<NMAXSYSTEMATICS;isyst++){
    SystematicBase* syst = _systematics[isyst];
    if (!syst) continue;

    // Create the proper structure for the ToyExperiment adding each of the systematics
    toy.AddToyVariation(syst->GetIndex(), syst->GetNParameters());

    // Loop over parameters for this systematic
    for (UInt_t ipar = 0;ipar<syst->GetNParameters();ipar++){
      Float_t var = 0;
      toy.SetToyVariation(isyst,ipar,var,weight);
    }
  }
}

