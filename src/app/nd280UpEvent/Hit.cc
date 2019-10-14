#include "Hit.hh"

//********************************************
void Hit::SetAll(Hit* in){
//********************************************
  fview             = in->GetView();
  fX                = in->GetX();
  fY                = in->GetY();
  fZ                = in->GetZ();
  fPE               = in->GetPE();
  fHG_pe            = in->GetHG_pe();
  fLG_pe            = in->GetLG_pe();
  fToT_pe           = in->GetToT_pe();
  fHG_ADC           = in->GetHG_ADC();
  fLG_ADC           = in->GetLG_ADC();
  fRE               = in->GetRE();
  fFE               = in->GetFE();
  fToT              = in->GetToT();
  fDt               = in->GetDt();
  fSpillTag         = in->GetSpillTag();
  fFEB              = in->GetFEB();
  fCh               = in->GetCh();
  fTfromSpill       = in->GetTfromSpill();
  fGTrigTag         = in->GetGTrigTag();
  fGTrigTime        = in->GetGTrigTime();
  fSpillTime        = in->GetSpillTime();
  fSpillTrailTime   = in->GetSpillTrailTime();
}
