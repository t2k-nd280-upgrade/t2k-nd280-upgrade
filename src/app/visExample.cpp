#include <iostream>

#include "globals.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "ND280CreateRunManager.hh"
#include "ND280PersistencyManager.hh"
#include "ND280RootPersistencyManager.hh"

// The physics lists that can be used.
#include "ND280PhysicsList.hh"

#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif



using namespace std;

int main(int argc, char **argv) {

  cout << "T2K test\n";

  ND::TND280Log::Configure();

  G4String outputFilename;
  G4String physicsList = "QGSP_BERT";

  // Set the mandatory initialization classes
  // Construct the default run manager
  G4RunManager* runManager = ND280CreateRunManager(physicsList);
    
#ifdef G4VIS_USE
  // Visualization.
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // Create the persistency manager.
  ND280PersistencyManager* persistencyManager 
        = ND280RootPersistencyManager::GetInstance();

  // Get the pointer to the UI manager
  G4UImanager* UI = G4UImanager::GetUIpointer();

  // Set the defaults for the simulation.
  UI->ApplyCommand("/t2k/control nd280mc-defaults 1.0");

  if (persistencyManager) {
    G4String command = "/db/open ";
    outputFilename = "t2k-nd280";
    UI->ApplyCommand(command+outputFilename);
  }
     G4String macro(argv[1]);
     G4String command = "/control/execute ";
     UI->ApplyCommand(command+macro); 
  // job G4String
#ifdef G4VIS_USE
  delete visManager;
#endif

  if (persistencyManager) {
    persistencyManager->Close();
    delete persistencyManager;
  }
  delete runManager;

  return 0;
}
