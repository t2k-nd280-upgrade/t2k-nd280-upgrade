#ifndef KinematicsUtils_h
#define KinematicsUtils_h

#include "BaseDataClasses.hxx"
#include "BasicUtils.hxx"
#include "HEPConstants.hxx"
#include "ParticleId.hxx"

/// This namespace contains useful functions for analyses related to kinematics 
namespace anaUtils{

    //----- Utility functions -------------

    Float_t GetParticleMass(ParticleId::ParticleEnum id );
    Float_t ComputeBetaGamma(Float_t mom, ParticleId::ParticleEnum id );
    Float_t ComputeBeta(Float_t mom, ParticleId::ParticleEnum id );

    /// compute the inverse transverse momentum given an AnaParticleMomB
    Float_t ComputeInversePT(const AnaParticleMomB &part);

    /// compute the inverse transverse momentum given an AnaParticleB and the momentum
    Float_t ComputeInversePT(const AnaParticleB &part, Float_t mom);

    /// compute the inverse transverse momentum for a true part
    Float_t ComputeInversePT(const AnaTrueParticleB &part);

    /// compute the total momentum given the part and the inverse transverse momentum
    Float_t ComputeMomentumFromInversePT(const AnaParticleB &part, Float_t PTinv);

    /// Compute the reconstructed neutrino energy assuming CCQE kinematics. Uses the "basic"
    /// formula which doesn't consider neutron/proton mass difference or binding energy.
    ///
    /// Formula is E_rec = 0.5 * (2 * M_p * E_l - m_l^2) / (M_p - E_l + p_l * costheta_l),
    /// where M_ is mass, E_ is energy, p_ is momentum, p is proton, l is lepton.
    Float_t NeutrinoERecCCQE(Float_t mom_lepton, Float_t mass_lepton, Float_t costheta_lepton);

    /// Get invariant mass given two AnaParticleB's and corresponding masses
    Float_t ComputeInvariantMass(const AnaParticleMomB& part1, const AnaParticleMomB& part2, Float_t mass1, Float_t mass2);

    /// Compute the reconstructed neutrino energy assuming CCQE kinematics.
    /// It considers neutron/proton mass difference and binding energy.
    Float_t ComputeRecNuEnergyCCQE(Float_t mom_lepton, Float_t mass_lepton, Float_t costheta_lepton, Float_t bindingEnergy = 25.);

    /// Compute the reconstructed Q2 assuming CCQE kinematics to compute neutrino energy.
    /// It considers neutron/proton mass difference and binding energy.
    Float_t ComputeRecQ2CCQE(Float_t mom_lepton, Float_t mass_lepton, Float_t costheta_lepton);

    /// Compute the Q2 using muon variables and neutrino energy
    /// It considers neutron/proton mass difference and binding energy.
    Float_t ComputeQ2(Float_t mom_lepton, Float_t mass_lepton, Float_t costheta_lepton, Float_t energy_neutrino);
    
    /// Compute kinetic energy given momentum and mass
    Float_t ComputeKineticEnergy(Float_t mom, Float_t mass);

    /// Direction assuming straight line between given start and end points
    Float_t* GetSLineDir(Float_t* start, Float_t* end);

    /// ------ Transverse plane (i.e   w.r.t. to neutrino direction) calculations ----------
    /// Returns the projection of thisvec onto the transverse nu plane
    TVector3 ProjectTransverse(TVector3& nudir, TVector3& thisvec);

    /// Returns pi-(the angle between muon and proton). Should be delta-fn in ideal back2back
    Float_t GetDPhi(const Float_t* nudir, const Float_t* mudir, const Float_t* pdir);

    /// Get the transverse momentum
    Float_t GetTransverseMom(const Float_t* nudir, const Float_t* thisdir, Float_t thismom);  
}
#endif
