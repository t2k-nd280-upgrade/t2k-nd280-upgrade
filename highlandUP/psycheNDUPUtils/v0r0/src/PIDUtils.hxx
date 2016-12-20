#ifndef PIDUtils_h
#define PIDUtils_h

#include "ConstituentsUtils.hxx"
#include "DataClasses.hxx"
/// This namespace contains useful functions for analyses. Functions include
/// those related to fiducial volumes, accessing tracks with specific
/// characteristics, and more.
namespace anaUtils{

    //----- Utility functions -------------

    /// Function to recompute the pull for a TPC track segment
    Float_t ComputeTPCPull(const AnaTPCParticleB &track, const std::string& particle);

    /// Function to recompute the pull for a TPC track segment for all hypotheses
    void ComputeTPCPull( AnaTPCParticleB &track, Float_t* pulls);
    void ComputeTPCPull( AnaTPCParticleB &track, const AnaTrackB& track_);
    void ComputeTPCPull( AnaDetCrossingB &track, const AnaTrueParticleB& track_,Float_t* pulls);
    Float_t ComputedEdxSigma(AnaDetCrossingB &track, int PDG);
    Float_t RecondEdx( AnaDetCrossingB &track, const AnaTrueParticleB& track_,double momentum) ;
    double SmearMomentum(  AnaDetCrossingB &track, int pdg,double& sigma);
    double Sigma( AnaDetCrossingB &track,int pdg);

    /// Function to recompute all the pull for a TPC track segment and save them into the segment
    void RecomputeTPCPulls(AnaTPCParticleB &track);

    /// Compute the expected dEdx for a particle
    Float_t ExpecteddEdx(const AnaParticleMomB &part, ParticleId::ParticleEnum partID);

    /// Compute the expected dEdx for a given momentum and particle type
    Float_t ExpecteddEdx(Float_t mom, ParticleId::ParticleEnum partID);

    Float_t RecondEdx( AnaTPCParticleB & track,  const AnaTrackB & track_);
    
    Double_t TrackMomError(Double_t momentum, Double_t momerr, ParticleId::ParticleEnum partID);

    /// The likelihood of a track being a given particle hypothesis, based on the
    /// pull values of all TPC segments in the track.
    ///
    /// hypo is one of:
    /// * 0 - muon
    /// * 1 - electron
    /// * 2 - proton
    /// * 3 - pion
    Float_t GetPIDLikelihood(const AnaTrackB& track, Int_t hypo, bool prod5Cut = 0);

    /// Get all likelihoods
    void GetPIDLikelihood(const AnaTrackB&, Float_t* hypo, bool prod5Cut = 0);

    /// Get the likelihood for MIP: (like_mu+like_pi)/(1-like_p)
    Float_t GetPIDLikelihoodMIP(const AnaTrackB &track);

    /// A function that is not currently used, and will be documented when it is.
    Float_t GetPIDPrior(const AnaTrackB& track, Int_t hypo);
    Float_t ComputedEdxSigma(AnaTPCParticleB &track, int pdg);
//    void Fill_Tracks_Recon_From_True(AnaTrueParticleB* trueParticle, AnaTrackB* reconParticle);

}
#endif
