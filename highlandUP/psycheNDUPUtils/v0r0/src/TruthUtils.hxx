#ifndef TruthUtils_h
#define TruthUtils_h

#include "AnalysisUtils.hxx"

/// This namespace contains useful functions for analyses. Functions include
/// those related to fiducial volumes, accessing tracks with specific
/// characteristics, and more.
namespace anaUtils{

    /// Get the number of true michel electrons
    Int_t GetNMichelElectrons(const AnaTrueVertexB& trueVertex, SubDetId::SubDetEnum det = SubDetId::kTPCUp1);

    /// Return the true linear length traversed in the Target
    Float_t GetTrueLinearLengthInTarget(const AnaTrueParticleB& trueTrack, Float_t& distz);
      
    /// Whether a true track crosses a Subdet1 so to be able to deposit charge in at least two layers
    bool TrueParticleCrossesTPC(const AnaTrueParticleB*   track, SubDetId::SubDetEnum det = SubDetId::kTPC);
    
    /// Whether a true track crosses a Target so to be able to reconstruct an object 
    bool TrueParticleCrossesTarget(const AnaTrueParticleB*   track, SubDetId::SubDetEnum det = SubDetId::kTarget);

    /// Whether a true track enters a given sub-detector 
    bool TrueParticleEntersDet(const AnaTrueParticleB*   track, SubDetId::SubDetEnum det);
    
    /// TPC detectors crossed
    int GetTPCDetCrossed(const AnaTrueParticleB*       track, SubDetId::SubDetEnum det[]);
        /// Get the momentum value from AnaDetCrossing
    Float_t GetEntranceMomentum(const AnaDetCrossingB& cross);
    
    /// Get the momentum value from AnaDetCrossing
    Float_t GetExitMomentum(const AnaDetCrossingB& cross);



//___________________________________________________________________________ 
bool IsCCQE(int neut_reaction_mode);//___________________________________________________________________________ 
bool IsCC1pi(int neut_reaction_mode);
//___________________________________________________________________________ 
bool IsCCcoh(int neut_reaction_mode);
//___________________________________________________________________________ 
bool Is2p2h(int neut_reaction_mode);//___________________________________________________________________________
bool IsCCoth(int neut_reaction_mode);
//___________________________________________________________________________ 
bool IsNC(int neut_reaction_mode);//___________________________________________________________________________ 
int GetReacAll(int neut_reaction_mode);   
}
#endif
