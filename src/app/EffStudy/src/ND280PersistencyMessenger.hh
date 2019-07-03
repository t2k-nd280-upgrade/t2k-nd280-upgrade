////////////////////////////////////////////////////////////
// $Id: ND280PersistencyMessenger.hh,v 1.12 2011/09/06 18:58:35 mcgrew Exp $
// 
// class description:
//
//      This is a messenger class for ND280PersistencyManager.
//      Implemented commands are following;
//
//  Commands : 
//    /db/open *            Open the output file
//    /db/close             Close the database
// 

#ifndef ND280PersistencyMessenger_h
#define ND280PersistencyMessenger_h 1

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

class ND280PersistencyManager;

class ND280PersistencyMessenger: public G4UImessenger {
public:
    ND280PersistencyMessenger(ND280PersistencyManager* persistencyMgr);
    virtual ~ND280PersistencyMessenger();
    
    void SetNewValue(G4UIcommand* command,G4String newValues);
    G4String GetCurrentValue(G4UIcommand* command);
    
private:
    ND280PersistencyManager* fPersistencyManager;
    
    G4UIdirectory*             fPersistencyDIR;
    G4UIdirectory*             fPersistencySetDIR;
    G4UIcmdWithAString*        fOpenCMD;   
    G4UIcmdWithoutParameter*   fCloseCMD;
    G4UIcmdWithADoubleAndUnit* fGammaThresholdCMD;
    G4UIcmdWithADoubleAndUnit* fNeutronThresholdCMD;
    G4UIcmdWithADoubleAndUnit* fMomentumThresholdCMD;
    G4UIcmdWithADoubleAndUnit* fRangeThresholdCMD;
    G4UIcmdWithADoubleAndUnit* fLengthThresholdCMD;
    G4UIcmdWithAnInteger*      fMaxTrajectoryStepsCMD;   
    G4UIcmdWithADoubleAndUnit* fMinimumStepLengthCMD;
    G4UIcmdWithADoubleAndUnit* fMinimumNeutralStepCMD;
    G4UIcmdWithABool*          fSaveAllHitsCMD;
    G4UIcmdWithABool*          fSaveAllTrajCMD;
    G4UIcmdWithABool*          fSaveAllPrimaryTrajectoriesCMD;
    G4UIcmdWithADoubleAndUnit* fTrajectoryPointAccuracyCMD;
    G4UIcmdWithAString*        fTrajectoryBoundaryCMD;
    G4UIcmdWithoutParameter*   fClearBoundariesCMD;

};
#endif

