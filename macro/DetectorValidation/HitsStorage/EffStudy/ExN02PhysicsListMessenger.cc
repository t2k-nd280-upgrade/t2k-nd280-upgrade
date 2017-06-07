//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file electromagnetic/TestEm10/src/Em10PhysicsListMessenger.cc
/// \brief Implementation of the Em10PhysicsListMessenger class
//
//
// $Id: Em10PhysicsListMessenger.cc 67268 2013-02-13 11:38:40Z ihrivnac $
//
// 
//
///////////////////////////////////////////////////////////////////////



#include "ExN02PhysicsListMessenger.hh"

#include "ExN02PhysicsList.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"


//////////////////////////////////////////////////////////////////////////////
//
//

ExN02PhysicsListMessenger::ExN02PhysicsListMessenger(ExN02PhysicsList* List)
:G4UImessenger(),ExN02List(List)
{
  // cutGCmd = new G4UIcmdWithADoubleAndUnit("/calor/cutG",this);
  // cutGCmd->SetGuidance("Set cut values by RANGE for Gamma.");
  // cutGCmd->SetParameterName("range",true);
  // cutGCmd->SetDefaultValue(1.);
  // cutGCmd->SetDefaultUnit("mm");
  // cutGCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // cutECmd = new G4UIcmdWithADoubleAndUnit("/calor/cutE",this);
  // cutECmd->SetGuidance("Set cut values by RANGE for e- e+.");
  // cutECmd->SetParameterName("range",true);
  // cutECmd->SetDefaultValue(1.);
  // cutECmd->SetDefaultUnit("mm");
  // cutECmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // setMaxStepCmd = new G4UIcmdWithADoubleAndUnit("/step/setMaxStep",this);
  // setMaxStepCmd->SetGuidance("Set max. step length in the detector");
  // setMaxStepCmd->SetParameterName("mxStep",true);
  // setMaxStepCmd->SetDefaultUnit("mm");


  // ElectronCutCmd = new G4UIcmdWithADoubleAndUnit("/physlist/setElectronCut",this);
  // ElectronCutCmd->SetGuidance("Set electron cut in mm for vertex region");
  // ElectronCutCmd->SetParameterName("ElectronCut",false,false);
  // ElectronCutCmd->SetDefaultUnit("mm");
  // ElectronCutCmd->SetRange("ElectronCut>0.");
  // ElectronCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


  // PositronCutCmd = new G4UIcmdWithADoubleAndUnit("/physlist/setPositronCut",this);
  // PositronCutCmd->SetGuidance("Set positron cut in mm for vertex region");
  // PositronCutCmd->SetParameterName("PositronCut",false,false);
  // PositronCutCmd->SetDefaultUnit("mm");
  // PositronCutCmd->SetRange("PositronCut>0.");
  // PositronCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


  // GammaCutCmd = new G4UIcmdWithADoubleAndUnit("/physlist/setGammaCut",this);
  // GammaCutCmd->SetGuidance("Set gamma cut in mm for vertex region");
  // GammaCutCmd->SetParameterName("GammaCut",false,false);
  // GammaCutCmd->SetDefaultUnit("mm");
  // GammaCutCmd->SetRange("GammaCut>0.");
  // GammaCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // RadiatorCutCmd = new G4UIcmdWithADoubleAndUnit("/physlist/setRadiatorCuts",this);
  // RadiatorCutCmd->SetGuidance("Set radiator cut in mm for vertex region");
  // RadiatorCutCmd->SetParameterName("RadiatorCuts",false,false);
  // RadiatorCutCmd->SetDefaultUnit("mm");
  // RadiatorCutCmd->SetRange("RadiatorCuts > 0.");
  // RadiatorCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // DetectorCutCmd = new G4UIcmdWithADoubleAndUnit("/physlist/setDetectorCuts",this);
  // DetectorCutCmd->SetGuidance("Set radiator cut in mm for vertex region");
  // DetectorCutCmd->SetParameterName("DetectorCuts",false,false);
  // DetectorCutCmd->SetDefaultUnit("mm");
  // DetectorCutCmd->SetRange("DetectorCuts > 0.");
  // DetectorCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // XTRModelCmd = new G4UIcmdWithAString("/physlist/setXTRModel",this);
  // XTRModelCmd->SetGuidance("Set XTR model");
  // XTRModelCmd->SetParameterName("XTRModel",false);
  // XTRModelCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fPhysDir = new G4UIdirectory("/physlist/");
  fPhysDir->SetGuidance("physics list commands");

  UsePAIModelCmd = new G4UIcmdWithABool("/physlist/usePAIModel",this);
  UsePAIModelCmd->SetGuidance("Set PAI model");
  UsePAIModelCmd->SetParameterName("UsePAIModel",false);
  UsePAIModelCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SetHadronicPhysListCmd = new G4UIcmdWithAString("/physlist/setHadronicPhysList",this);
  SetHadronicPhysListCmd->SetGuidance("Set Hadronic physics list");
  SetHadronicPhysListCmd->SetParameterName("HadronicPhysList",false);
  SetHadronicPhysListCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

/////////////////////////////////////////////////////////////////////////////

ExN02PhysicsListMessenger::~ExN02PhysicsListMessenger()
{
  // delete cutGCmd;
  // delete cutECmd;
  // delete setMaxStepCmd;
  // delete ElectronCutCmd;
  // delete PositronCutCmd;
  // delete GammaCutCmd;
  // delete XTRModelCmd;

  delete fPhysDir;
  delete UsePAIModelCmd;
  delete SetHadronicPhysListCmd;
}

////////////////////////////////////////////////////////////////////////////////

void ExN02PhysicsListMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  // if(command == cutGCmd)
  //   { ExN02List->SetGammaCut(cutGCmd->GetNewDoubleValue(newValue));}
  // if(command == cutECmd)
  //   { ExN02List->SetElectronCut(eCmd->GetNewDoubleValue(newValue));}
  // if(command == setMaxStepCmd)
  //   { ExN02List->SetMaxStep(setMaxStepCmd->GetNewDoubleValue(newValue));}

  // if( command == ElectronCutCmd )
  // {
  //   ExN02List->SetRegElectronCut(ElectronCutCmd->GetNewDoubleValue(newValue));
  // }
  // if( command == PositronCutCmd )
  // {
  //   ExN02List->SetRegPositronCut(PositronCutCmd->GetNewDoubleValue(newValue));
  // }
  // if( command == GammaCutCmd )
  // {
  //   ExN02List->SetRegGammaCut(GammaCutCmd->GetNewDoubleValue(newValue));
  // }
  // if( command == RadiatorCutCmd )
  // {
  //   ExN02List->SetRadiatorCuts();
  // }
  // if( command == DetectorCutCmd )
  // {
  //   ExN02List->SetDetectorCuts();
  // }
  // if( command == XTRModelCmd )
  // { 
  //   ExN02List->SetXTRModel(newValue);
  // }

  if( command == UsePAIModelCmd )
    {
      //ExN02List->SetUsePAIModel(newValue);
      //ExN02List->AddPhysicsList(newValue);
    }
  if( command == SetHadronicPhysListCmd )
    {
      //G4cout << newValue << G4endl;
      ExN02List->AddPhysicsList(newValue);
      //ExN02List->SetHadronicPhysList(newValue);
    }
  
  //G4cout << "ExN02PhysicsListMessenger" << G4endl;
  //exit(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

