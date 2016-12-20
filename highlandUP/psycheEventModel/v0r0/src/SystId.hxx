#ifndef SystId_hxx_seen
#define SystId_hxx_seen


namespace SystId{

  enum SystEnum {
   
        kBFieldDist=0,
    // Momentum related systematics
    kMomScale,      
    kMomResol,
    kMomRangeResol,
    kMomBiasFromVertexMigration, 
    // Charge and PID identification 
    kTpcPid,        
    kTargetPid,        
    kChargeIDEff,    
    // Efficiencies for TPC tracking
    kTpcClusterEff,     
    kTpcTrackEff,
    // TPC matching
    kTpcTargetMatchEff,
    kTpcECalMatchEff, 
    kTpcP0dMatchEff, 
    // Target reconstruction efficiencies
    kTargetTrackEff,       
    kTargetHybridTrackEff, 
    kMichelEleEff,          
    // Pile-up, tracker
    kPileUp,
    // Detector mass, normalization
    kTargetMass,
    // OOFV
    kOOFV, 
    // GEANT 4 model
    kSIPion,
    kSIProton,
    // Effects from sand-muons, tracker
    kSandMu,
    // ECal
    kECalEMResol,
    kECalEMScale,
    kECalTrackEff,
    kECalPID, 
    // High-angle matching
    kTargetECalMatchEff, 
    kTargetECalSmrdMatchEff,
    // ToF
    kToFResol, 
    // Flux
    kFluxWeightNu,      
    kFluxWeightAntiNu,
    // NuE analysis specific
    kECalEmHipPID,
    kTarget2Shower,
    kNuETPCPileUp,
    kNuEP0DPileUp,
    kNuEECalPileUp,
    kNuEOOFV,
    // P0D+Tracker specific
    kP0DELossScale,
    kP0DELossResol,
    kP0DVeto,

  };
}

#endif
