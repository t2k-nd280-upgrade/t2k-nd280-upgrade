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
//
// $Id: ExN02DetectorMessenger.cc,v 1.12 2008-09-22 16:41:20 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN02PrimaryGeneratorMessenger.hh"
#include "ExN02PrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PrimaryGeneratorMessenger::ExN02PrimaryGeneratorMessenger(ExN02PrimaryGeneratorAction* myPrimGen)
  : G4UImessenger(),
    myPrimaryGenerator(myPrimGen),
    fPrimgenDir(0),
    fGeneratorCmd(0),
    fTypeDirCmd(0),
    fTypeGunCmd(0)
{   
  fPrimgenDir = new G4UIdirectory("/generator/");
  fPrimgenDir->SetGuidance("Primary Generator");
  
  fGeneratorCmd = new G4UIcmdWithAString("/generator/type",this);
  fGeneratorCmd->SetGuidance("Choose the generator: Generator (don't need to specify NEUT or GENIE) and ParticleGun");
  fGeneratorCmd->SetParameterName("generator type",false);
  fGeneratorCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 

  fTypeDirCmd = new G4UIcmdWithAString("/generator/typedir",this);
  fTypeDirCmd->SetGuidance("Choose how to generated the momentum direction: fixed or uniformly randomized");
  fTypeDirCmd->SetParameterName("Type Direction",false);
  fTypeDirCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 

  fTypeMomCmd = new G4UIcmdWithAString("/generator/typemom",this);
  fTypeMomCmd->SetGuidance("Choose how to generated the momentum: fixed or uniformly randomized");
  fTypeMomCmd->SetParameterName("Type Momentum",false);
  fTypeMomCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fTypeGunCmd = new G4UIcmdWithAString("/generator/typegun",this);
  fTypeGunCmd->SetGuidance("Choose a specific configuration of pgun for HATPC FC studies");
  fTypeGunCmd->SetParameterName("Type FC Gun",false);
  fTypeGunCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 

  fTypePosCmd = new G4UIcmdWithAString("/generator/typepos",this);
  fTypePosCmd->SetGuidance("Choose how to generated the position: fixed or gaussian randomized");
  fTypePosCmd->SetParameterName("Type Position",false);
  fTypePosCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PrimaryGeneratorMessenger::~ExN02PrimaryGeneratorMessenger()
{
  delete fGeneratorCmd;
  delete fTypeDirCmd;
  delete fTypeMomCmd;
  delete fTypeGunCmd;
  delete fPrimgenDir;
  delete fTypePosCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == fGeneratorCmd ){
    myPrimaryGenerator->SetGeneratorType(newValue);
  } 
  else if( command == fTypeDirCmd ){
    myPrimaryGenerator->SetTypeDirection(newValue);
  }  
  else if( command == fTypeMomCmd ){
    myPrimaryGenerator->SetTypeMomentum(newValue);
  }
  else if( command == fTypeGunCmd ){
    myPrimaryGenerator->SetTypeGun(newValue);
  }
  else if( command == fTypePosCmd ){
    myPrimaryGenerator->SetTypePosition(newValue);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
