#include "Event.hh"

//********************************************
void Event::Clear(Option_t*)
//********************************************
{
  fNHits = 0;
  fHits->Clear("C");
  std::fill_n(fdEdzXZ, 48, 0);
  std::fill_n(fdEdzZY, 48, 0);
  std::fill_n(fdEdz, 48, 0);
}

//********************************************
Hit* Event::AddHit()
//********************************************
{
    TClonesArray &hits = *fHits;
    Hit *hit = new(hits[fNHits++]) Hit();
    return hit;
}

//********************************************
Double_t Event::FindMaxCharge()
//********************************************
{
  Double_t En = 0;
  for (Int_t ihit=0; ihit<fNHits ; ihit++){
    Hit *hit = (Hit*)fHits->At(ihit);
    if (hit->GetPE() > En && hit->GetView() != 0) En = hit->GetPE();
  }
  return En;
}
