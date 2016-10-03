#ifndef ParticleId_hxx_seen
#define ParticleId_hxx_seen

namespace ParticleId {

        /// PDG enumeration of all particles
        enum PdgEnum {
             kInvalidPDG = 0,
             kProtonPDG = 2212,
             kAntiProtonPDG = -2212,
             kNeutronPDG = 2112,
             kAntiNeutronPDG = -2112,
             kElectronPDG = 11,
             kPositronPDG = -11, kAntiElectronPDG = kPositronPDG,
             kMuonPDG = 13, kMuNegPDG = kMuonPDG, kMuMinusPDG = kMuonPDG, kMuonNegPDG = kMuonPDG, kMuonMinusPDG = kMuonPDG,
             kAntiMuonPDG = -13, kMuPosPDG = kAntiMuonPDG, kMuPlusPDG = kAntiMuonPDG, kMuonPosPDG = kAntiMuonPDG, kMuonPlusPDG = kAntiMuonPDG,
             kTauPDG = 15, kTauNegPDG = kTauPDG, kTauMinusPDG = kTauPDG,
             kAntiTauPDG = -15, kTauPosPDG = kAntiTauPDG, kTauPlusPDG = kAntiTauPDG,
             kPi0PDG = 111, kPion0PDG = kPi0PDG, kPiZeroPDG = kPi0PDG, kPionZeroPDG = kPi0PDG,
             kPiPosPDG = 211, kPiPlusPDG = kPiPosPDG, kPionPosPDG = kPiPosPDG, kPionPlusPDG = kPiPosPDG,
             kPiNegPDG = -211, kPiMinusPDG = kPiNegPDG, kPionNegPDG = kPiNegPDG, kPionMinusPDG = kPiNegPDG,
             kK0PDG = 311, kKaon0PDG = kK0PDG, kKZeroPDG = kK0PDG, kKaonZeroPDG = kK0PDG,
             kAntiK0PDG = -311, kAntiKaon0PDG = kAntiK0PDG, kAntiKZeroPDG = kAntiK0PDG, kAntiKaonZeroPDG = kAntiK0PDG,
             kK0LPDG = 130, kKaonZeroLongPDG = kK0LPDG, kK0LongPDG = kK0LPDG, kKaon0LongPDG = kK0LPDG, kKZeroLongPDG = kK0LPDG, kKaonZeroLPDG = kK0LPDG, kKaon0LPDG = kK0LPDG, kKZeroLPDG = kK0LPDG,
             kK0SPDG = 310, kKaonZeroShortPDG = kK0SPDG, kK0ShortPDG = kK0SPDG, kKaon0ShortPDG = kK0SPDG, kKZeroShortPDG = kK0SPDG, kKaonZeroSPDG = kK0SPDG, kKaon0SPDG = kK0SPDG, kKZeroSPDG = kK0SPDG,
             kKPosPDG = 321, kKPlusPDG = kKPosPDG, kKaonPosPDG = kKPosPDG, kKaonPlusPDG = kKPosPDG,
             kKNegPDG = -321, kKMinusPDG = kKNegPDG, kKaonNegPDG = kKNegPDG, kKaonMinusPDG = kKNegPDG,
             kPhotonPDG = 22,
             kEtaPDG = 221, kEtaZeroPDG = kEtaPDG, kEta0PDG = kEtaPDG,
             kRho0PDG = 113, kRhoZeroPDG = kRho0PDG
        };


        /// Enumeration of all particles
        enum ParticleEnum {
             kBaryons,
             kMesons,
             kLeptons,
             kNeutrinos,
             kAntiNeutrinos,
             kPions,
             kKaons,
             kNuclei,          // Genie lists also the remnant nuclei in StdHep# variables
             kPseudoParticles, // Genie *pseudo-particles", such as 'hadron blob' etc, see $GENIE/data/evgen/catalogues/pdg/genie_pdg_table.txt
             kOthers,

             kProton,
             kAntiProton,
             kNeutron,
             kAntiNeutron,
             kElectron,
             kPositron, kAntiElectron = kPositron,
             kMuon, kMuNeg = kMuon, kMuMinus = kMuon, kMuonNeg = kMuon, kMuonMinus = kMuon,
             kAntiMuon, kMuPos = kAntiMuon, kMuPlus = kAntiMuon, kMuonPos = kAntiMuon, kMuonPlus = kAntiMuon,
             kTau, kTauNeg = kTau, kTauMinus = kTau,
             kAntiTau, kTauPos = kAntiTau, kTauPlus = kAntiTau,
             kPi0, kPion0 = kPi0, kPiZero = kPi0, kPionZero = kPi0,
             kPiPos, kPiPlus = kPiPos, kPionPos = kPiPos, kPionPlus = kPiPos,
             kPiNeg, kPiMinus = kPiNeg, kPionNeg = kPiNeg, kPionMinus = kPiNeg,
             kK0, kKaon0 = kK0, kKZero = kK0, kKaonZero = kK0,
             kAntiK0, kAntiKaon0 = kAntiK0, kAntiKZero = kAntiK0, kAntiKaonZero = kAntiK0,
             kK0L, kKaonZeroLong = kK0L, kK0Long = kK0L, kKaon0Long = kK0L, kKZeroLong = kK0L, kKaonZeroL = kK0L, kKaon0L = kK0L, kKZeroL = kK0L,
             kK0S, kKaonZeroShort = kK0S, kK0Short = kK0S, kKaon0Short = kK0S, kKZeroShort = kK0S, kKaonZeroS = kK0S, kKaon0S = kK0S, kKZeroS = kK0S,
             kKPos, kKPlus = kKPos, kKaonPos = kKPos, kKaonPlus = kKPos,
             kKNeg, kKMinus = kKNeg, kKaonNeg = kKNeg, kKaonMinus = kKNeg,
             kEta, kEtaZero = kEta, kEta0 = kEta,
             kRho0, kRhoZero = kRho0,
             kPhoton,
             kLast // used to give the size to AnaTrueVertex::NPrimaryParticle array and to loop over this enum
        };


        // Get the PDG from the ParticleEnum index
        PdgEnum GetParticlePDG(int kPart, bool warn=true);

        // Get the ParticleEnum index from the PDG
        ParticleEnum GetParticle(int kPart, bool warn=true);

}
#endif

