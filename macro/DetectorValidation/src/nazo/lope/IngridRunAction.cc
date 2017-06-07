#include "G4Run.hh"
#include "IngridRunAction.hh"
#include "CLHEP/Random/Random.h"
#include <time.h>

////////////////////////////
//IngridRunAction::IngridRunAction(char* output,float x0,float y0,double hiteff0)
IngridRunAction::IngridRunAction(char* output)
////////////////////////////
{
  time_t timer;
  strcpy(file,output);

  time(&timer);
  seed = long(timer);
  G4cout << "time: " << timer << "     seed: " << seed << G4endl; 
  x = 0.;
  y = 0.;

	hiteff = 100.;

	evt = new IngridEventSummary();

	//
	CLHEP::HepRandom::setTheSeed(seed);  
}

/////////////////////////////
IngridRunAction::~IngridRunAction()
/////////////////////////////
{
	if(evt) delete evt;
}

//////////////////////////////////////////////////////
void IngridRunAction::BeginOfRunAction(const G4Run* aRun)
//////////////////////////////////////////////////////
{
	//
	f = new TFile(file,"RECREATE");   
	tree = new TTree("tree","Tree");

	//
	TBranch * EvtBr = tree->GetBranch("fDefaultReco.");
	tree->Branch("fDefaultReco.","IngridEventSummary",&evt,64000,99);

}


////////////////////////////////////////////////////
void IngridRunAction::EndOfRunAction(const G4Run* aRun)
////////////////////////////////////////////////////
{ 
    G4cout << " Not Entry to INGRID " << NotEntry << G4endl;

    tree->Write();
	if(tree) delete tree;
    f->Close();
	if(f) delete f;

}

 
