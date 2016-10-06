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

#include "ExN02DetectorMessenger.hh"

#include "ExN02DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02DetectorMessenger::ExN02DetectorMessenger(ExN02DetectorConstruction* myDet)
:myDetector(myDet)
{ 

  N02Dir = new G4UIdirectory("/N02/");
  N02Dir->SetGuidance("UI commands specific to this example.");
  
  detDir = new G4UIdirectory("/N02/det/");
  detDir->SetGuidance("detector control.");
  
  Targ1MatCmd = new G4UIcmdWithAString("/N02/det/setTargetMate1",this);
  Targ1MatCmd->SetGuidance("Select Material of the Target 1.");
  Targ1MatCmd->SetParameterName("choice",false);
  Targ1MatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  Targ2MatCmd = new G4UIcmdWithAString("/N02/det/setTargetMate2",this);
  Targ2MatCmd->SetGuidance("Select Material of the Target 2.");
  Targ2MatCmd->SetParameterName("choice",false);
  Targ2MatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  FGD1MatCmd = new G4UIcmdWithAString("/N02/det/setFGDMate1",this);
  FGD1MatCmd->SetGuidance("Select Material of the FGD 1.");
  FGD1MatCmd->SetParameterName("choice",false);
  FGD1MatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  FGD2MatCmd = new G4UIcmdWithAString("/N02/det/setFGDMate2",this);
  FGD2MatCmd->SetGuidance("Select Material of the FGD 2.");
  FGD2MatCmd->SetParameterName("choice",false);
  FGD2MatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SideTPCMatCmd = new G4UIcmdWithAString("/N02/det/setSideTPCMate",this);
  SideTPCMatCmd->SetGuidance("Select Material of the Side TPCs.");
  SideTPCMatCmd->SetParameterName("choice",false);
  SideTPCMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  
  
  //FieldCmd = new G4UIcmdWithADoubleAndUnit("/N02/det/setField",this);  
  //FieldCmd->SetGuidance("Define magnetic field.");
  //FieldCmd->SetGuidance("Magnetic field will be in X direction.");
  //FieldCmd->SetParameterName("Bx",false);
  //FieldCmd->SetUnitCategory("Magnetic flux density");
  //FieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
        
  StepMaxCmd = new G4UIcmdWithADoubleAndUnit("/N02/det/stepMax",this);  
  StepMaxCmd->SetGuidance("Define a step max");
  StepMaxCmd->SetParameterName("stepMax",false);
  StepMaxCmd->SetUnitCategory("Length");
  StepMaxCmd->AvailableForStates(G4State_Idle);    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02DetectorMessenger::~ExN02DetectorMessenger()
{
  delete Targ1MatCmd;
  delete Targ2MatCmd;
  delete SideTPCMatCmd;
  //delete FieldCmd;
  delete StepMaxCmd;  
  delete detDir;
  delete N02Dir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == Targ1MatCmd )
    { myDetector->setMaterial_Target1(newValue); }
 
  if( command == Targ2MatCmd )
    { myDetector->setMaterial_Target2(newValue);}

  if( command == FGD1MatCmd )
    { myDetector->setMaterial_FGD1(newValue); }
 
  if( command == FGD2MatCmd )
    { myDetector->setMaterial_FGD2(newValue);}
  
  if( command == SideTPCMatCmd )
    { myDetector->setMaterial_SideTPC(newValue);}  
  
  //if( command == FieldCmd )
  //{ myDetector->SetMagField(FieldCmd->GetNewDoubleValue(newValue));}
  
  if( command == StepMaxCmd )
    { myDetector->SetMaxStep(StepMaxCmd->GetNewDoubleValue(newValue));}   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
