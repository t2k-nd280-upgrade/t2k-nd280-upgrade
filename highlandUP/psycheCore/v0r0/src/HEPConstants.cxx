#include "HEPConstants.hxx"
#include "TDatabasePDG.h"

namespace units{
  TDatabasePDG* pdgBase = TDatabasePDG::Instance();  
  
  // Fill the variables using the TDatabasePDG
  // nuclons
  const double mass_proton       = pdgBase->GetParticle(2212)->Mass()*units::GeV;
  const double mass_neutron      = pdgBase->GetParticle(2112)->Mass()*units::GeV;
            
  // leptons
  const double mass_electron     = pdgBase->GetParticle(11)->Mass()*units::GeV;
  const double mass_muon         = pdgBase->GetParticle(13)->Mass()*units::GeV;
            
  // mesons
  const double mass_pion_zero    = pdgBase->GetParticle(111)->Mass()*units::GeV;
  const double mass_pion_charged = pdgBase->GetParticle(211)->Mass()*units::GeV;
  const double mass_kaon_zero    = pdgBase->GetParticle(311)->Mass()*units::GeV;
  const double mass_kaon_charged = pdgBase->GetParticle(321)->Mass()*units::GeV;

}
