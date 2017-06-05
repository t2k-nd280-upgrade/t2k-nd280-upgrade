#include "G4RunManager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "QGSP.hh"

#include "IngridDetectorConstruction.hh"
//#include "ND280mPhysicsList.hh"
#include "IngridPrimaryGeneratorAction.hh"
#include "IngridRunAction.hh"
#include "IngridEventAction.hh"
#include "IngridTrackingAction.hh"
#include "Neut.hh"


#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#define PROTON 2
#define HORIZONTAL 3
#define VERTICAL 4 

// ====================================================================
//     main
// ====================================================================
int main(int argc, char** argv) 
{
	char neutfile[300];
	char output[300];
	char cmd[300];

	int nd = 0;
	int batch = 0;
  int flav = 0;
  // 1:numu, 2:numubar, 3:nue, 4:nuebar

	int NumberOfEvent = 0;

	int c = -1;
	while ((c = getopt(argc, argv, "ho:i:m:b:f:")) != -1) {
    switch(c){
			case 'o':
				sprintf(output,"%s",optarg);
				break;
			case 'i':
				sprintf(neutfile,"%s",optarg);
				break;
			case 'm':
				nd = atoi(optarg);
				break;
			case 'f':
				flav = atoi(optarg);
				break;
			case 'b':
				batch = 1;
				sprintf(cmd,"%s",optarg);
				break;
			case 'h':
				std::cerr << "o:output root file name" << std::endl;
				std::cerr << "i:input neut file" << std::endl;
				std::cerr << "m:2(nd2) or 3(nd3) or 4(nd4)" << std::endl;
				std::cerr << "b:batch command" << std::endl;
				exit(1);
		}
	}

  if( nd==0 ) {
    G4cout << "Select horizontal or vertical or proton module" << G4endl;
    exit(1);
  }
  if( flav==0 ) {
    G4cout << "Select neutrino flavor" << G4endl;
    exit(1);
  }

  // run manager
  G4RunManager* runManager= new G4RunManager;  //G4cout << G4endl;

	// Neut initialization
	Neut *neut = new Neut;
	NumberOfEvent = neut->NtupleReadInit(neutfile);
	G4cout << "NumberOfEvent :" << NumberOfEvent << G4endl;

  // set mandatory user initialization classes...

  // detector setup
  runManager-> SetUserInitialization(new IngridDetectorConstruction(nd));
  //runManager-> SetUserInitialization(new IngridDetectorConstruction());
  G4cout << "Detector Init OK" << G4endl;

  // particles and physics processes
  //runManager-> SetUserInitialization(new ND280mPhysicsList);
  runManager-> SetUserInitialization(new QGSP);
  G4cout << "PhysicsList Init OK" << G4endl;

	IngridRunAction * rac = new IngridRunAction(output);
	G4cout << "RunAction init OK" << G4endl;

	IngridEventAction * evt = new IngridEventAction(rac);
	G4cout << "EventAction init OK" << G4endl;

	IngridTrackingAction * tra = new IngridTrackingAction(rac, evt);
	runManager->SetUserAction(tra);
	G4cout << "TrackingAction init OK" << G4endl;

	runManager-> SetUserAction(new IngridPrimaryGeneratorAction(neut, rac, evt, nd, flav));
	G4cout << "PrimaryGenerator init OK" << G4endl;
    
  // user action classes... (optional)
	runManager-> SetUserAction(rac);
	runManager-> SetUserAction(evt);

#ifdef G4VIS_USE
  // initialize visualization package
  G4VisManager* visManager= new G4VisExecutive;
  visManager-> Initialize();
  G4cout <<"visualization init OK" << G4endl;
#endif

  // Initialize G4 kernel
  runManager-> Initialize();
    
	// get the pointer to the UI manager and set verbosities
	G4UImanager* UI= G4UImanager::GetUIpointer();

	if(batch==1)
	// Define (G)UI terminal for interactive mode  
	{ 
			// G4UIterminal is a (dumb) terminal.
			G4UIsession * session = 0;
			G4String command = "/control/execute ";
			G4String macro = cmd;
			session = new G4UIterminal(new G4UItcsh);
			UI->ApplyCommand(command+macro);
			session->SessionStart();
			delete session;
	}

	else { // batch mode
			runManager->BeamOn(NumberOfEvent);
	}

  // terminating...
#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;  //G4cout << G4endl;

  return 0;

}

