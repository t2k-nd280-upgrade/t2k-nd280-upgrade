////////////////////////////////////////////////////////////
// $Id: ND280PersistencyMessenger.cc,v 1.13 2011/09/06 18:58:35 mcgrew Exp $
//

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcommand.hh"
#include "G4ios.hh"

#include "ND280PersistencyMessenger.hh"
#include "ND280PersistencyManager.hh"

ND280PersistencyMessenger::ND280PersistencyMessenger(
    ND280PersistencyManager* persistencyMgr)
    : fPersistencyManager(persistencyMgr) {
    fPersistencyDIR = new G4UIdirectory("/db/");
    fPersistencyDIR->SetGuidance("Output file  control commands.");
    
    fOpenCMD = new G4UIcmdWithAString("/db/open",this);
    fOpenCMD->SetGuidance("Set the name of the output file and open it.");
    fOpenCMD->SetParameterName("filename",true);
    fOpenCMD->SetDefaultValue("t2k-nd280.root");
    fOpenCMD->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fCloseCMD = new G4UIcmdWithoutParameter("/db/close",this);
    fCloseCMD->SetGuidance("Close the output file.");

    fPersistencySetDIR = new G4UIdirectory("/db/set/");
    fPersistencySetDIR->SetGuidance("Set various parameters");
    
    fGammaThresholdCMD 
        = new G4UIcmdWithADoubleAndUnit("/db/set/gammaThreshold", this);
    fGammaThresholdCMD->SetGuidance(
        "Set momentum threshold for writing out gamma-ray trajectories");
    fGammaThresholdCMD->SetParameterName("momentum", false, false);
    fGammaThresholdCMD->SetUnitCategory("Energy");

    fNeutronThresholdCMD 
        = new G4UIcmdWithADoubleAndUnit("/db/set/neutronThreshold", this);
    fNeutronThresholdCMD->SetGuidance(
        "Set momentum threshold for writing out neutron trajectories");
    fNeutronThresholdCMD->SetParameterName("momentum", false, false);
    fNeutronThresholdCMD->SetUnitCategory("Energy");

    fMomentumThresholdCMD 
        = new G4UIcmdWithADoubleAndUnit("/db/set/momentumThreshold", this);
    fMomentumThresholdCMD->SetGuidance(
        "[DEPRECATED] Set momentum threshold for writing out particle trajectories");
    fMomentumThresholdCMD->SetParameterName("momentum", false, false);
    fMomentumThresholdCMD->SetUnitCategory("Energy");

    fRangeThresholdCMD 
        = new G4UIcmdWithADoubleAndUnit("/db/set/rangeThreshold", this);
    fRangeThresholdCMD->SetGuidance(
        "[DEPRECATED] Set range threshold for writing out particle trajectories");
    fRangeThresholdCMD->SetParameterName("range", false, false);
    fRangeThresholdCMD->SetUnitCategory("Length");

    fLengthThresholdCMD 
        = new G4UIcmdWithADoubleAndUnit("/db/set/lengthThreshold", this);
    fLengthThresholdCMD->SetGuidance(
        "Set length of track in an SD for writing out particle trajectories");
    fLengthThresholdCMD->SetParameterName("length", false, false);
    fLengthThresholdCMD->SetUnitCategory("Length");

    fMaxTrajectoryStepsCMD 
        = new G4UIcmdWithAnInteger("/db/set/maxTrajectorySteps", this);
    fMaxTrajectoryStepsCMD->SetGuidance(
        "[DEPRECATED] Set maximum number of trajectory points to be saved.");
    fMaxTrajectoryStepsCMD->SetParameterName("steps", false, false);

    fMinimumStepLengthCMD 
        = new G4UIcmdWithADoubleAndUnit("/db/set/minimumStepLength", this);
    fMinimumStepLengthCMD->SetGuidance(
        "[DEPRECATED] Set the minimum step length between saved trajectory points.");
    fMinimumStepLengthCMD->SetParameterName("length", false, false);
    fMinimumStepLengthCMD->SetUnitCategory("Length");

    fMinimumNeutralStepCMD 
        = new G4UIcmdWithADoubleAndUnit("/db/set/minimumNeutralStep", this);
    fMinimumNeutralStepCMD->SetGuidance(
        "[DEPRECATED] Set the minimum distance between neutral interaction points.");
    fMinimumNeutralStepCMD->SetParameterName("length", false, false);
    fMinimumNeutralStepCMD->SetUnitCategory("Length");

    fSaveAllPrimaryTrajectoriesCMD 
        = new G4UIcmdWithABool("/db/set/saveAllPrimTraj", this);
    fSaveAllPrimaryTrajectoriesCMD->SetGuidance(
        "Control which primaries have saved trajectories.\n"
        "  True: Save all prim. part. trajectories.\n"
        "  False: Save prim. that ultimately deposit energy in SD.");

    fSaveAllTrajCMD 
        = new G4UIcmdWithABool("/db/set/saveAllTraj", this);
    fSaveAllTrajCMD->SetGuidance(
        "Control to save all the trajectories.\n"
        "  True: Save all trajectories.\n"
        "  False: Save prim. traj (see saveAllPrimTraj)");

    fSaveReconCMD 
        = new G4UIcmdWithABool("/db/set/saveRecon", this);
    fSaveReconCMD->SetGuidance(
        "Control to save trajectories with some specific cuts to study reconstruction.\n"
        "  True: Save trajectories satisfying some cuts.\n"
        "  False: Save prim. traj (see saveAllPrimTraj)");

    fSaveAllHitsCMD 
        = new G4UIcmdWithABool("/db/set/saveAllHits", this);
    fSaveAllHitsCMD->SetGuidance(
        "Control to save all hits.\n"
        "  True: Save all hits from all the trajs.\n"
        "  False: Save hits only from prim. trajs");

    fTrajectoryPointAccuracyCMD 
        = new G4UIcmdWithADoubleAndUnit("/db/set/trajectoryAccuracy", this);
    fTrajectoryPointAccuracyCMD->SetGuidance(
        "Set the minimum accuracy of the trajectory.");
    fTrajectoryPointAccuracyCMD->SetParameterName("length", false, false);
    fTrajectoryPointAccuracyCMD->SetUnitCategory("Length");

    fTrajectoryBoundaryCMD 
        = new G4UIcmdWithAString("/db/set/trajectoryBoundary",this);
    fTrajectoryBoundaryCMD->SetGuidance(
        "Add a Perl RegExp for a phys. vol. boundary where a\n"
        "    trajectory point is saved. The expression is compared to a\n"
        "    string constructed \":particle:charge:volume:\" where particle\n"
        "    is the particle name, charge is \"charged\" or \"neutral\" and\n"
        "    volume is the physical volume name.");
    fTrajectoryBoundaryCMD->SetParameterName("boundary",true);
    fTrajectoryBoundaryCMD->AvailableForStates(G4State_PreInit,G4State_Idle);

    fClearBoundariesCMD 
        = new G4UIcmdWithoutParameter("/db/set/clearBoundaries",this);
    fClearBoundariesCMD->SetGuidance("Remove all of the boundaries for "
                                     "trajectory points.");
    
}

ND280PersistencyMessenger::~ND280PersistencyMessenger() {
    delete fOpenCMD;
    delete fCloseCMD;
    delete fGammaThresholdCMD;
    delete fNeutronThresholdCMD;
    delete fMomentumThresholdCMD;
    delete fRangeThresholdCMD;
    delete fLengthThresholdCMD;
    delete fMaxTrajectoryStepsCMD;
    delete fMinimumStepLengthCMD;
    delete fMinimumNeutralStepCMD;
    delete fSaveAllHitsCMD;
    delete fSaveAllTrajCMD;
    delete fSaveAllPrimaryTrajectoriesCMD;
    delete fTrajectoryPointAccuracyCMD;
    delete fTrajectoryBoundaryCMD;
    delete fClearBoundariesCMD;
    delete fPersistencyDIR;
    delete fPersistencySetDIR;
}


void ND280PersistencyMessenger::SetNewValue(G4UIcommand* command,
                                            G4String newValue) {
    if (command==fOpenCMD) { 
        fPersistencyManager->Open(newValue); 
    }
    else if (command == fCloseCMD) {
        fPersistencyManager->Close(); 
    }
    else if (command == fGammaThresholdCMD) {
        fPersistencyManager->SetGammaThreshold(
            fGammaThresholdCMD->GetNewDoubleValue(newValue));
    }
    else if (command == fNeutronThresholdCMD) {
        fPersistencyManager->SetNeutronThreshold(
            fNeutronThresholdCMD->GetNewDoubleValue(newValue));
    }
    else if (command == fMomentumThresholdCMD) {
        std::cout << "# WARNING -- Using a deprecated command"
                  << std::endl;
    }
    else if (command == fRangeThresholdCMD) {
        std::cout << "# WARNING -- Using a deprecated command"
                  << std::endl;
    }
    else if (command == fLengthThresholdCMD) {
        fPersistencyManager->SetLengthThreshold(
            fLengthThresholdCMD->GetNewDoubleValue(newValue));
    }
    else if (command == fMaxTrajectoryStepsCMD) {
        std::cout << "# WARNING -- Using a deprecated command" << std::endl;
    }
    else if (command == fMinimumStepLengthCMD) {
        std::cout << "# WARNING -- Using a deprecated command" << std::endl;
    }
    else if (command == fMinimumNeutralStepCMD) {
        std::cout << "# WARNING -- Using a deprecated command" << std::endl;
    }
    else if (command == fSaveAllPrimaryTrajectoriesCMD) {
        fPersistencyManager->SetSaveAllPrimaryTrajectories(
            fSaveAllPrimaryTrajectoriesCMD->GetNewBoolValue(newValue));
    }
    else if (command == fSaveAllTrajCMD) {
        fPersistencyManager->SetSaveAllTraj(
            fSaveAllTrajCMD->GetNewBoolValue(newValue));
    }
    else if (command == fSaveReconCMD) {
        fPersistencyManager->SetSaveRecon(
            fSaveReconCMD->GetNewBoolValue(newValue));
    }
    else if (command == fSaveAllHitsCMD) {
        fPersistencyManager->SetSaveAllHits(
            fSaveAllHitsCMD->GetNewBoolValue(newValue));
    }
    else if (command == fTrajectoryPointAccuracyCMD) {
        fPersistencyManager->SetTrajectoryPointAccuracy(
            fTrajectoryPointAccuracyCMD->GetNewDoubleValue(newValue));
    }
    else if (command == fTrajectoryBoundaryCMD) {
        fPersistencyManager->AddTrajectoryBoundary(newValue);
    }
    else if (command == fClearBoundariesCMD) {
        fPersistencyManager->ClearTrajectoryBoundaries();
    }
}


G4String ND280PersistencyMessenger::GetCurrentValue(G4UIcommand * command) {
    G4String currentValue;
    
    if (command==fOpenCMD) {
        currentValue = fPersistencyManager->GetFilename();
    }
    else if (command==fGammaThresholdCMD) {
        currentValue = fGammaThresholdCMD->ConvertToString(
            fPersistencyManager->GetGammaThreshold());
    }
    else if (command==fNeutronThresholdCMD) {
        currentValue = fNeutronThresholdCMD->ConvertToString(
            fPersistencyManager->GetNeutronThreshold());
    }
    else if (command==fLengthThresholdCMD) {
        currentValue = fLengthThresholdCMD->ConvertToString(
            fPersistencyManager->GetLengthThreshold());
    }
    else if (command==fSaveAllPrimaryTrajectoriesCMD) {
        currentValue = fSaveAllPrimaryTrajectoriesCMD->ConvertToString(
            fPersistencyManager->GetSaveAllPrimaryTrajectories());
    }
    else if (command==fSaveAllTrajCMD) {
        currentValue = fSaveAllTrajCMD->ConvertToString(
            fPersistencyManager->GetSaveAllTraj());
    }
    else if (command==fSaveReconCMD) {
        currentValue = fSaveReconCMD->ConvertToString(
            fPersistencyManager->GetSaveAllTraj());
    }
    else if (command==fSaveAllHitsCMD) {
        currentValue = fSaveAllHitsCMD->ConvertToString(
            fPersistencyManager->GetSaveAllHits());
    }
    else if (command==fTrajectoryPointAccuracyCMD) {
        currentValue = fTrajectoryPointAccuracyCMD->ConvertToString(
            fPersistencyManager->GetTrajectoryPointAccuracy());
    }

    return currentValue;
}

