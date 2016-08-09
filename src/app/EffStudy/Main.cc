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
#include "ExN02RunAction.hh"
#include "ExN02EventAction.hh"
#include "ExN02SteppingAction.hh"
#include "ExN02SteppingVerbose.hh"
#include "ExN02ND280XML.hh"
#include "G4BlineTracer.hh"
#include "ND280PersistencyManager.hh"
#include "ND280RootPersistencyManager.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include <iostream>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//
// argv[1] --> input GEANT4 macro file
// argv[2] --> input xml configuration file
// argv[3] --> First Event of Generator tree
// argv[4] --> # of events to run
// 

int main(int argc,char** argv)
{
  // User Verbose output class
  //
  G4VSteppingVerbose* verbosity = new ExN02SteppingVerbose;
  G4VSteppingVerbose::SetInstance(verbosity);
  
  // Run manager
  //
  G4RunManager * runManager = new G4RunManager;

 
  // Create ROOT the persistency manager.                                              
  ND280RootPersistencyManager* persistencyManager
    = ND280RootPersistencyManager::GetInstance();
    
  std::string rootfilename = "ND280upgrade"; 
  persistencyManager->Open(rootfilename); 
  if(persistencyManager->IsOpen()){
    G4cout << "The output ROOT file is open" << G4endl;
  }
  else{
    G4Exception("ExN02DetectorConstruction",
		"if(persistencyManager->IsOpen()",
		FatalException,
		"The file is not open");    
  }  


  //
  // Set the input configuration file
  //
  G4String xmlfilename = argv[2];
  persistencyManager->OpenXML(xmlfilename);
  
  ExN02ND280XML *ND280XMLInput = persistencyManager->GetXMLInput();
  
  G4cout << "File name: " << ND280XMLInput->GetXMLFileName() << G4endl;
  G4cout << "Generator: " << ND280XMLInput->GetXMLGenerTypeName() << G4endl;
  G4cout << "Path to files: " << ND280XMLInput->GetXMLPathFiles() << G4endl;
  G4cout << "Tree name: " << ND280XMLInput->GetXMLGenerTreeName() << G4endl;
  G4cout << "File name: " << ND280XMLInput->GetXMLGenerFileName() << G4endl;
  
  G4String inputfile = ND280XMLInput->GetXMLPathFiles();
  inputfile.append(ND280XMLInput->GetXMLGenerFileName());

  TFile *myfile = new TFile(inputfile,"READ"); 
  if (!myfile->IsOpen()) {
    const char *msg = "NEUT file is not open!";
    const char *origin = "Main function";
    const char *code = "if (!myfile->IsOpen())";
    G4Exception(origin,code,FatalException,msg);
  }
  TTree *mytree = (TTree*)myfile->Get(ND280XMLInput->GetXMLGenerTreeName());
  if (!mytree) {
    const char *msg = "NEUT tree is not open!";
    const char *origin = "Main function";
    const char *code = "if (!mytree->IsOpen())";
    G4Exception(origin,code,FatalException,msg);
  }    

  
  // Set BeamOn to total # of events in the tree - first event
  
  G4int MyFirstEvent = atoi(argv[3]);
  G4int MyStepEvent  = atoi(argv[4]);  
  
  persistencyManager->SetEventFirst(MyFirstEvent);
  persistencyManager->SetNEvents(MyStepEvent);
  G4cout << "# of events to process: " << persistencyManager->GetNEvents() << G4endl;
  G4cout << "# of first event: " << persistencyManager->GetEventFirst() << G4endl;

  G4int NEvtStep = MyStepEvent;
  G4int NEvtTot = MyFirstEvent+NEvtStep;

  if(MyFirstEvent > (mytree->GetEntries()-1)){ // MyFirstEvent starts from 0
    G4cout << G4endl;
    G4cout << "MyFirstEvent = " << MyFirstEvent << G4endl;
    G4cout << "mytree->GetEntries() = " << mytree->GetEntries() << G4endl;
    const char *msg = "First event Id exceeds the last tree event ID!";
    const char *origin = "Main function";
    const char *code = " if(MyFirstEvent>mytree->GetEntries())";
    G4Exception(origin,code,FatalException,msg);
  }
  
  if(NEvtTot > mytree->GetEntries()){
    G4cout << G4endl;
    G4cout << "MyFirstEvent = " << MyFirstEvent << G4endl;
    G4cout << "NEvtTot = " << NEvtTot << G4endl;
    G4cout << "mytree->GetEntries() = " << mytree->GetEntries() << G4endl;
    const char *msg = "Tot # of simulated events > tree->GetEntries()!";
    const char *origin = "Main function";
    const char *code = " if(NEvtTot>mytree->GetEntries())";
    //G4Exception(origin,code,FatalException,msg);
    G4Exception(origin,code,JustWarning,msg);

    NEvtStep = NEvtTot - mytree->GetEntries(); 

    G4cout << "Set tot # of simul events to :" << NEvtStep << G4endl;
    G4cout << G4endl;
  }
  
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

      //
      // Set number of BeamOn (events to simulate)
      //
      //runManager->BeamOn(NEvtTot); // reset all the UImanager commands!!!
      G4String RunBeamOn = Form("/run/beamOn %i",NEvtStep);
      //G4cout << RunBeamOn << G4endl;
      
      UImanager->ApplyCommand(RunBeamOn);
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

  if(persistencyManager->IsOpen()){
    persistencyManager->Close(); 
  }
  delete persistencyManager;

  delete runManager;
  delete verbosity;
  
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

