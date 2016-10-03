#include "KinematicsUtils.hxx"


//********************************************************************
Float_t anaUtils::GetParticleMass(ParticleId::ParticleEnum partID ) {
//********************************************************************

  Float_t mass = -1;
  
  if (partID == ParticleId::kElectron        || partID == ParticleId::kPositron) {
    mass = units::mass_electron;
  } else if (partID == ParticleId::kPionPlus || partID == ParticleId::kPionMinus) {
    mass = units::mass_pion_charged;
  } else if (partID == ParticleId::kMuon     || partID == ParticleId::kAntiMuon) {
    mass = units::mass_muon;
  } else if (partID == ParticleId::kProton   || partID == ParticleId::kAntiProton) {
    mass = units::mass_proton;
  } else if (partID == ParticleId::kKaonPlus || partID == ParticleId::kKaonMinus) {
    mass = units::mass_kaon_charged;
  } else {
    //    std::cout << "Tried to compute mass for invalid particle hypothesis" << std::endl;
    return -1;
  }
  return mass;
}

//********************************************************************
Float_t anaUtils::ComputeBetaGamma(Float_t mom, ParticleId::ParticleEnum partID ) {
//********************************************************************

  Float_t mass = GetParticleMass(partID);
  if (mass<0) return -1;
  
  Float_t bg = mom / mass;
  return bg;
}

//********************************************************************
Float_t anaUtils::ComputeBeta(Float_t mom, ParticleId::ParticleEnum partID ) {
//********************************************************************

  Float_t mass = GetParticleMass(partID);
  if (mass<0) return -1;
  
  Float_t bg = mom / mass;
  Float_t beta = bg / sqrt(1. + bg * bg);
  
  return beta;
}


//********************************************************************
Float_t anaUtils::ComputeInvariantMass(const AnaParticleMomB &part1, const AnaParticleMomB &part2, Float_t mass1, Float_t mass2) {
//********************************************************************
  
  if (mass1 < 0. || mass2 < 0.)
    return -999.;
  
  TVector3 vect = anaUtils::ArrayToTVector3(part1.DirectionStart);
  vect *= part1.Momentum;

  Float_t energy = sqrt(mass1*mass1 + pow(part1.Momentum, 2));

  TLorentzVector lvect1(vect, energy);
  
  vect = anaUtils::ArrayToTVector3(part2.DirectionStart);
  vect *= part2.Momentum;

  energy = sqrt(mass2*mass2 + pow(part2.Momentum, 2));
  
  TLorentzVector lvect2(vect, energy);
  
  return (lvect1 + lvect2).M(); 
  
}

//********************************************************************
Float_t anaUtils::ComputeInversePT(const AnaParticleMomB &part) {
//********************************************************************

  Float_t u_t = sqrt(1-pow(part.DirectionStart[0],2));
  Float_t pt = u_t*(part.Momentum);
  if (pt!=0)
    return 1/pt;
  else
    return -999;
}


//********************************************************************
Float_t anaUtils::ComputeInversePT(const AnaParticleB &part, Float_t mom) {
//********************************************************************

  Float_t u_t = sqrt(1-pow(part.DirectionStart[0],2));
  Float_t pt = u_t*mom;
  if (pt!=0)
    return 1/pt;
  else
    return -999;
}

//********************************************************************
Float_t anaUtils::ComputeMomentumFromInversePT(const AnaParticleB &part, Float_t PTinv) {
//********************************************************************

  if (PTinv==0) return -999;
  Float_t pt = 1/PTinv;  
  Float_t u_t = sqrt(1-pow(part.DirectionStart[0],2));
  if (u_t==0) return -999;

  Float_t p = pt/u_t;
  return p;
}

//********************************************************************
Float_t anaUtils::ComputeInversePT(const AnaTrueParticleB &part) {
//********************************************************************

  Float_t u_t = sqrt(1-pow(part.Direction[0],2));
  Float_t pt = u_t*(part.Momentum);
  if (pt!=0)
    return 1/pt;
  else
    return -999;
}

//********************************************************************
Float_t anaUtils::NeutrinoERecCCQE(Float_t mom_lepton, Float_t mass_lepton, Float_t costheta_lepton) {
//********************************************************************
  Float_t mass_proton = 938.272;
  Float_t ene_lepton = sqrt(mom_lepton * mom_lepton + mass_lepton * mass_lepton);
  return 0.5 * (2 * mass_proton * ene_lepton - mass_lepton * mass_lepton) /
               (mass_proton - ene_lepton + mom_lepton * costheta_lepton);

}

//********************************************************************
Float_t anaUtils::ComputeRecNuEnergyCCQE(Float_t mom_lepton, Float_t mass_lepton, Float_t costheta_lepton, Float_t bindingEnergy) {
//********************************************************************

  // computes the reconstructed neutrino energy using only muon kinematics
  // all masses in MeV
  // bindingEnergy = - nuclear potential = 25 MeV for Carbon (default)

  Float_t reduced_nMass = units::mass_neutron - bindingEnergy;

  // muon energy
  Float_t Emu = sqrt( (mass_lepton*mass_lepton) + (mom_lepton*mom_lepton) );

  // reconstructed neutrino energy
  Float_t eNuRec =  ( (units::mass_proton*units::mass_proton)
                      - (mass_lepton*mass_lepton)
                      - (reduced_nMass*reduced_nMass)
                      + (2 * Emu * reduced_nMass)
                    ) / (
                      2*(reduced_nMass + (mom_lepton * costheta_lepton) - Emu)
                    );

  return eNuRec;
}

//********************************************************************
Float_t anaUtils::ComputeRecQ2CCQE(Float_t mom_lepton, Float_t mass_lepton, Float_t costheta_lepton) {
//********************************************************************

  // computes the reconstructed Q2 using reconstructed quantities

  // reconstructed neutrino energy using only the kinematic of the muon
  Float_t eNuRec = ComputeRecNuEnergyCCQE(mom_lepton, mass_lepton, costheta_lepton);

  // Q2 (using reconstructed neutrino energy). In GeV^2
  Float_t Q2 = ComputeQ2(mom_lepton, mass_lepton,costheta_lepton,eNuRec);

  return Q2;
}

//********************************************************************
Float_t anaUtils::ComputeQ2(Float_t mom_lepton, Float_t mass_lepton, Float_t costheta_lepton, Float_t e_neutrino) {
//********************************************************************

  // computes the Q2

  // reconstructed muon energy
  Float_t Emu = sqrt( pow(mass_lepton,2) + pow(mom_lepton,2) );
  
  // Q2. In GeV^2
  Float_t Q2 = (2 * e_neutrino * (Emu - (mom_lepton * costheta_lepton)) - pow(mass_lepton,2)) / 1e6;

  return Q2;  
}

//********************************************************************
Float_t* anaUtils::GetSLineDir(Float_t* start, Float_t* end){
  //********************************************************************
  static Float_t uf[3] = {0, 0, 0};
  TVector3 u(end[0]-start[0], end[1]-start[1], end[2]-start[2]);
  if (u.Mag()!=0){
    u *= 1/(u.Mag());
    uf[0] = u.X();
    uf[1] = u.Y();
    uf[2] = u.Z();
  }
  return uf;
}

//********************************************************************
Float_t anaUtils::ComputeKineticEnergy(Float_t mom, Float_t mass){
  //******************************************************************** 
  return sqrt(pow(mom, 2) + pow(mass, 2)) - mass;
}


//***************************************************************
TVector3 anaUtils::ProjectTransverse(TVector3& nudir, TVector3& thisvec){
//***************************************************************
  return thisvec - thisvec.Dot(nudir)/nudir.Mag2() * nudir;
}

//***************************************************************
Float_t anaUtils::GetDPhi(const Float_t* nudir, const Float_t* mudir, const Float_t* pdir) {
//***************************************************************
  TVector3 nudirv = TVector3(nudir);
  TVector3 mudirv = TVector3(mudir);
  TVector3 pdirv  = TVector3(pdir);
  return TMath::Pi()-ProjectTransverse(nudirv, mudirv).Angle(ProjectTransverse(nudirv, pdirv));
}

//***************************************************************
Float_t anaUtils::GetTransverseMom(const Float_t* nudir, const Float_t* thisdir, Float_t thismom){
//***************************************************************
  TVector3 nudirv     = TVector3(nudir);
  TVector3 thisdirv   = TVector3(thisdir);
  TVector3 projectedv = anaUtils::ProjectTransverse(nudirv, thisdirv);
  return (thismom * projectedv).Mag();
}

