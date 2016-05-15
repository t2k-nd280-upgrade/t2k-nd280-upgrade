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
// $Id: exampleN02.cc,v 1.16 2009-10-30 14:59:59 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



// #include "Geant4GM/volumes/Factory.h" 
// #include "RootGM/volumes/Factory.h" 
// #include "TGeoManager.h"                               



#include "ExN02DetectorConstruction.hh"
#include "ExN02ActionInitialization.hh"
#include "ExN02Constants.hh"

#include "ExN02PhysicsList.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BERT.hh"

//#include "G4StepLimiter.hh"
#include "G4StepLimiterPhysics.hh"
//#include "G4StepLimiterBuilder.hh"

#include "ExN02PrimaryGeneratorAction.hh"
//#include "ExN02PrimaryGeneratorAction_DetDependency.hh"

#include "ExN02RunAction.hh"
#include "ExN02EventAction.hh"
#include "ExN02SteppingAction.hh"
#include "ExN02SteppingVerbose.hh"

#include "G4BlineTracer.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // User Verbose output class
  //
  G4VSteppingVerbose* verbosity = new ExN02SteppingVerbose;
  G4VSteppingVerbose::SetInstance(verbosity);
  
  // Run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // User Initialization classes (mandatory)
  //
  ExN02DetectorConstruction* detector = new ExN02DetectorConstruction;
  runManager->SetUserInitialization(detector);


  // Set the physics list

#ifndef USE_BLINETRACER
  ExN02PhysicsList* physics = new ExN02PhysicsList;
  runManager->SetUserInitialization(physics);
#else
  G4int verbose = 1;
  QGSP_BERT* physlist = new QGSP_BERT(verbose);
  //physlist->RegisterPhysics(new G4StepLimiterPhysics()); // attach the step limit to each particle 
  //physlist->RegisterPhysics(new G4StepLimiterBuilder()); // attach the step limit to each particle   
  runManager->SetUserInitialization(physlist);

  // Instantiate the G4BlineTracer class
  G4BlineTracer* theBlineTool = new G4BlineTracer();
#endif
  

  // User Action classes (OLD STYLE)
  //
  //G4VUserPrimaryGeneratorAction* gen_action = new ExN02PrimaryGeneratorAction(detector);
  //runManager->SetUserAction(gen_action);
  //G4UserRunAction* run_action = new ExN02RunAction;
  //runManager->SetUserAction(run_action);
  //
  //G4UserEventAction* event_action = new ExN02EventAction;
  //runManager->SetUserAction(event_action);
  //
  //G4UserSteppingAction* stepping_action = new ExN02SteppingAction; // OLD
  // //G4UserSteppingAction* stepping_action = new ExN02SteppingAction(detector,event_action); // NEW
  //runManager->SetUserAction(stepping_action);
                                                                                                
  // Initialize all User Action classes
  ExN02ActionInitialization* actionInitialization = new ExN02ActionInitialization(detector);
  runManager->SetUserInitialization(actionInitialization);

  // Initialize G4 kernel                                                                                                                  
  //                                                                                                                                    
  runManager->Initialize();

  //
      
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif    
  
  // Get the pointer to the User Interface manager
  //
  G4UImanager * UImanager = G4UImanager::GetUIpointer();  

  if (argc!=1)   // batch mode  
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }
  else           // interactive mode : define UI session
    { 
#ifdef G4UI_USE
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");     
#endif
      ui->SessionStart();
      delete ui;
#endif
     
#ifdef G4VIS_USE
      delete visManager;
#endif     

    }
  
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

#ifdef USE_BLINETRACER
  delete theBlineTool;
#endif
  delete runManager;
  delete verbosity;
  
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

