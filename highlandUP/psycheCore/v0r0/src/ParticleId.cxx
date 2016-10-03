#include "ParticleId.hxx"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>


//***************************************************
ParticleId::PdgEnum ParticleId::GetParticlePDG(int kPart, bool warn) {
//***************************************************

                 if (kPart==kProton)      return kProtonPDG;
            else if (kPart==kAntiProton)  return kAntiProtonPDG;
            else if (kPart==kNeutron)     return kNeutronPDG;
            else if (kPart==kAntiNeutron) return kAntiNeutronPDG;
            else if (kPart==kElectron)    return kElectronPDG;
            else if (kPart==kPositron)    return kPositronPDG;
            else if (kPart==kMuon)        return kMuonPDG;
            else if (kPart==kAntiMuon)    return kAntiMuonPDG;
            else if (kPart==kTau)         return kTauPDG;
            else if (kPart==kAntiTau)     return kAntiTauPDG;
            else if (kPart==kPionZero)    return kPionZeroPDG;
            else if (kPart==kPionPlus)    return kPionPlusPDG;
            else if (kPart==kPionMinus)   return kPionMinusPDG;
            else if (kPart==kKaonZero)    return kKaonZeroPDG;
            else if (kPart==kKaonPlus)    return kKaonPlusPDG;
            else if (kPart==kKaonMinus)   return kKaonMinusPDG;
            else if (kPart==kPhoton)      return kPhotonPDG;
            else if (kPart==kEta)         return kEtaPDG;
            else if (kPart==kRhoZero)     return kRhoZeroPDG;

            if (warn) std::cout << "PDG code not set for this particle index (you could update ParticleId.hxx!)" << std::endl;
            return ParticleId::kInvalidPDG;
};




//***************************************************
ParticleId::ParticleEnum ParticleId::GetParticle(int kPart, bool warn) {
//***************************************************

                 if (kPart==kProtonPDG)      return kProton;
            else if (kPart==kAntiProtonPDG)  return kAntiProton;
            else if (kPart==kNeutronPDG)     return kNeutron;
            else if (kPart==kAntiNeutronPDG) return kAntiNeutron;
            else if (kPart==kElectronPDG)    return kElectron;
            else if (kPart==kPositronPDG)    return kPositron;
            else if (kPart==kMuonPDG)        return kMuon;
            else if (kPart==kAntiMuonPDG)    return kAntiMuon;
            else if (kPart==kTauPDG)         return kTau;
            else if (kPart==kAntiTauPDG)     return kAntiTau;
            else if (kPart==kPionZeroPDG)    return kPionZero;
            else if (kPart==kPionPlusPDG)    return kPionPlus;
            else if (kPart==kPionMinusPDG)   return kPionMinus;
            else if (kPart==kKaonZeroPDG)    return kKaonZero;
            else if (kPart==kKaonPlusPDG)    return kKaonPlus;
            else if (kPart==kKaonMinusPDG)   return kKaonMinus;
            else if (kPart==kPhotonPDG)      return kPhoton;
            else if (kPart==kEtaPDG)         return kEta;
            else if (kPart==kRhoZeroPDG)     return kRhoZero;
            else if (kPart==+12 || kPart==+14 || kPart==+16) return kNeutrinos;
            else if (kPart==-12 || kPart==-14 || kPart==-16) return kAntiNeutrinos;
            else if (kPart>1000000000 && kPart<2000000000) return kNuclei;
            else if (kPart>2000000000) return kPseudoParticles;

            if (warn) std::cout << "PDG " << kPart << " is not associated to any particle in ParticleId.hxx (you could updated it!)" << std::endl;
            return ParticleId::kOthers;
};
