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
            else if (kPart==kAntiK0)      return kAntiKaonZeroPDG;
            else if (kPart==kKaonPlus)    return kKaonPlusPDG;
            else if (kPart==kKaonMinus)   return kKaonMinusPDG;
            else if (kPart==kPhoton)      return kPhotonPDG;
            else if (kPart==kEta)         return kEtaPDG;
            else if (kPart==kRhoZero)     return kRhoZeroPDG;
			else if (kPart==kK0S)         return kK0SPDG;
			else if (kPart==kK0L)         return kK0LPDG;
			else if (kPart==kLambda)      return kLambdaPDG;
			else if (kPart==kSigmaPlus)   return kSigmaPlusPDG;
			else if (kPart==kSigmaZero)   return kSigmaZeroPDG;
			else if (kPart==kSigmaMinus)  return kSigmaMinusPDG;
			else if (kPart==kLambdaCPlus)      return kLambdaCPlusPDG;
			else if (kPart==kSigmaCPlusPlus)   return kSigmaCPlusPlusPDG;
			else if (kPart==kSigmaCPlus)       return kSigmaCPlusPDG;
			else if (kPart==kSigmaCZero)       return kSigmaCZeroPDG;
			
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
            else if (kPart==kAntiK0PDG)      return kAntiKaonZero;
            else if (kPart==kKaonPlusPDG)    return kKaonPlus;
            else if (kPart==kKaonMinusPDG)   return kKaonMinus;
            else if (kPart==kPhotonPDG)      return kPhoton;
            else if (kPart==kEtaPDG)         return kEta;
            else if (kPart==kRhoZeroPDG)     return kRhoZero;
			else if (kPart==kK0SPDG)         return kK0S;
			else if (kPart==kK0LPDG)         return kK0L;
			else if (kPart==kLambdaPDG)      return kLambda;
			else if (kPart==kSigmaPlusPDG)   return kSigmaPlus;
			else if (kPart==kSigmaZeroPDG)   return kSigmaZero;
			else if (kPart==kSigmaMinusPDG)  return kSigmaMinus;
			else if (kPart==kLambdaCPlusPDG)      return kLambdaCPlus;
			else if (kPart==kSigmaCPlusPlusPDG)   return kSigmaCPlusPlus;
			else if (kPart==kSigmaCPlusPDG)       return kSigmaCPlus;
			else if (kPart==kSigmaCZeroPDG)       return kSigmaCZero;
            else if (kPart==+12 || kPart==+14 || kPart==+16) return kNeutrinos;
            else if (kPart==-12 || kPart==-14 || kPart==-16) return kAntiNeutrinos;
            else if (kPart>1000000000 && kPart<2000000000) return kNuclei;
            else if (kPart>2000000000) return kPseudoParticles;

            if (warn) std::cout << "PDG " << kPart << " is not associated to any particle in ParticleId.hxx (you could updated it!)" << std::endl;
            return ParticleId::kOthers;
};
