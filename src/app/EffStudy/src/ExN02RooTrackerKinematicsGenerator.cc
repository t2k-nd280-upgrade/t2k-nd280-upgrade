
//
// Read and store the NEUT tree containing the vertices informations
//
// 28/4/16 Davide Sgalaberna
//

#include "ExN02RooTrackerKinematicsGenerator.hh"
#include "ExN02Constants.hh"
#include "ExN02VertexInfo.hh"
#include "ExN02ND280XML.hh"

#include "globals.hh"

#include <G4Event.hh>
#include <G4PrimaryVertex.hh>
#include <G4PrimaryParticle.hh>
#include <G4IonTable.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4Tokenizer.hh>
#include <G4UnitsTable.hh>
#include <G4RunManager.hh>
#include <G4EventManager.hh>
#include <G4Run.hh>

ExN02RooTrackerKinematicsGenerator::ExN02RooTrackerKinematicsGenerator()
{
  fTotEntry = 0;
  fNextEntry = 0;
  fCurrEntry = 0;
  fneutfile = 0;
  fneutree = 0;

  G4String inputfile = inxml.GetXMLPathFiles();
  inputfile.append(inxml.GetXMLGenerFileName());

  if(inxml.GetXMLGenerTypeName()=="NEUT"){
    this->ReadNEUT(inputfile);
  }
  else if(inxml.GetXMLGenerTypeName()=="GENIE"){
    this->ReadGENIE(inputfile);
  }
  else{
    G4ExceptionDescription msg;
    msg << "The generator type "<< inxml.GetXMLGenerTypeName() << " is not available";
    G4Exception("ExN02RooTrackerKinematicsGenerator::~ExN02RooTrackerKinematicsGenerator",
                "MyCode0002",FatalException, msg);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02RooTrackerKinematicsGenerator::~ExN02RooTrackerKinematicsGenerator()
{
  fTotEntry = 0;
  fNextEntry = 0;
  fCurrEntry = 0;

  delete fneutree;  fneutree=0;
  delete fneutfile; fneutfile=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RooTrackerKinematicsGenerator::ReadNEUT(G4String filename)
{ 
  fneutfile = TFile::Open(filename,"OLD");
  if (!fneutfile->IsOpen()) {
    const char *msg = "NEUT file is not open!";
    const char *origin = "ExN02RooTrackerKinematicsGenerator::ReadNEUT";
    const char *code = "if (!fneutfile->IsOpen())";
    G4Exception(origin,code,FatalException,msg);
  }

  G4cout << G4endl;
  G4cout << "Open a RooTracker NEUT tree from " << filename << G4endl;

  fneutree = dynamic_cast<TTree*>(fneutfile->Get( inxml.GetXMLGenerTreeName() ));
  if (!fneutree) {
    const char *msg = "NEUT tree is not open!";
    const char *origin = "ExN02RooTrackerKinematicsGenerator::ReadNEUT";
    const char *code = "if (!fneutree->IsOpen())";
    G4Exception(origin,code,FatalException,msg);
  }  
  
  G4cout << "   File has  " << fneutree->GetEntries() << " entries" << G4endl;
  G4cout << G4endl;

  fEvtFlags = NULL;
  fneutree->SetBranchAddress("EvtFlags",       &fEvtFlags);
  fEvtCode = NULL;
  fneutree->SetBranchAddress("EvtCode",        &fEvtCode);
  fneutree->SetBranchAddress("EvtNum",         &fEvtNum);
  fneutree->SetBranchAddress("EvtXSec",        &fEvtXSec);
  fneutree->SetBranchAddress("EvtDXSec",       &fEvtDXSec);
  fneutree->SetBranchAddress("EvtWght",        &fEvtWght);
  fneutree->SetBranchAddress("EvtProb",        &fEvtProb);
  fneutree->SetBranchAddress("EvtVtx",          fEvtVtx);
  fneutree->SetBranchAddress("StdHepN",        &fStdHepN);
  fneutree->SetBranchAddress("StdHepPdg",       fStdHepPdg);
  fneutree->SetBranchAddress("StdHepStatus",    fStdHepStatus);
  fneutree->SetBranchAddress("StdHepX4",        fStdHepX4);
  fneutree->SetBranchAddress("StdHepP4",        fStdHepP4);
  fneutree->SetBranchAddress("StdHepPolz",      fStdHepPolz);
  fneutree->SetBranchAddress("StdHepFd",        fStdHepFd);
  fneutree->SetBranchAddress("StdHepLd",        fStdHepLd);
  fneutree->SetBranchAddress("StdHepFm",        fStdHepFm);
  fneutree->SetBranchAddress("StdHepLm",        fStdHepLm);
  fneutree->SetBranchAddress("NuParentPdg",    &fNuParentPdg);
  fneutree->SetBranchAddress("NuParentDecMode",&fNuParentDecMode);
  fneutree->SetBranchAddress("NuParentDecP4",   fNuParentDecP4);
  fneutree->SetBranchAddress("NuParentDecX4",   fNuParentDecX4);
  fneutree->SetBranchAddress("NuParentProP4",   fNuParentProP4);
  fneutree->SetBranchAddress("NuParentProX4",   fNuParentProX4);
  fneutree->SetBranchAddress("NuParentProNVtx",&fNuParentProNVtx);

  fTotEntry = fneutree->GetEntries();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RooTrackerKinematicsGenerator::ReadGENIE(G4String filename)
{

  //
  // Try to read GENIE file
  // if not read set GENIEflag = false
  // --> GeneratePrimaryVertexGENIE() cannot be used!!!
  //

  fneutfile = TFile::Open(filename,"OLD");
  if (!fneutfile->IsOpen()) {
    const char *msg = "GENIE file is not open!";
    const char *origin = "ExN02RooTrackerKinematicsGenerator::ReadGENIE";
    const char *code = "if (!fgeniefile->IsOpen())";
    G4Exception(origin,code,FatalException,msg);
  }

  G4cout << G4endl;
  G4cout << "Open a RooTracker GENIE tree from " << filename << G4endl;

  fneutree = dynamic_cast<TTree*>(fneutfile->Get( inxml.GetXMLGenerTreeName() ));
  if (!fneutree) {
    const char *msg = "GENIE tree is not open!";
    const char *origin = "ExN02RooTrackerKinematicsGenerator::ReadGENIE";
    const char *code = "if (!fneutree->IsOpen())";
    G4Exception(origin,code,FatalException,msg);
  }  
  
  G4cout << "   File has  " << fneutree->GetEntries() << " entries" << G4endl;
  G4cout << G4endl;
  
  fEvtFlags = NULL;
  fneutree->SetBranchAddress("EvtFlags",       &fEvtFlags);
  fEvtCode = NULL;
  fneutree->SetBranchAddress("EvtCode",        &fEvtCode);
  fneutree->SetBranchAddress("EvtNum",         &fEvtNum);
  fneutree->SetBranchAddress("EvtXSec",        &fEvtXSec);
  fneutree->SetBranchAddress("EvtDXSec",       &fEvtDXSec);
  fneutree->SetBranchAddress("EvtWght",        &fEvtWght);
  fneutree->SetBranchAddress("EvtProb",        &fEvtProb);
  fneutree->SetBranchAddress("EvtVtx",          fEvtVtx);
  fneutree->SetBranchAddress("StdHepN",        &fStdHepN);
  fneutree->SetBranchAddress("StdHepPdg",       fStdHepPdg);
  fneutree->SetBranchAddress("StdHepStatus",    fStdHepStatus);
  fneutree->SetBranchAddress("StdHepX4",        fStdHepX4);
  fneutree->SetBranchAddress("StdHepP4",        fStdHepP4);
  fneutree->SetBranchAddress("StdHepPolz",      fStdHepPolz);
  fneutree->SetBranchAddress("StdHepFd",        fStdHepFd);
  fneutree->SetBranchAddress("StdHepLd",        fStdHepLd);
  fneutree->SetBranchAddress("StdHepFm",        fStdHepFm);
  fneutree->SetBranchAddress("StdHepLm",        fStdHepLm);
  fneutree->SetBranchAddress("NuParentPdg",    &fNuParentPdg);
  fneutree->SetBranchAddress("NuParentDecMode",&fNuParentDecMode);
  fneutree->SetBranchAddress("NuParentDecP4",   fNuParentDecP4);
  fneutree->SetBranchAddress("NuParentDecX4",   fNuParentDecX4);
  fneutree->SetBranchAddress("NuParentProP4",   fNuParentProP4);
  fneutree->SetBranchAddress("NuParentProX4",   fNuParentProX4);
  fneutree->SetBranchAddress("NuParentProNVtx",&fNuParentProNVtx);

  fTotEntry = fneutree->GetEntries();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RooTrackerKinematicsGenerator::GeneratePrimaryVertex(G4Event* anEvent)
{  
  if (!fneutfile) {
    const char *msg = "NEUT file is not open!";
    const char *origin = "ExN02RooTrackerKinematicsGenerator::GeneratePrimaryVertex";
    const char *code = "if (!fneutfile) {";
    G4Exception(origin,code,FatalException,msg);
  }
  
  // // Check to see if the next event is there.
  // if (fNextEntry >= fTotEntry) {
  //   G4cerr << G4endl;
  //   G4cerr << "fNextEntry = " << fNextEntry << G4endl;
  //   G4cerr << "fTotEntry = " << fTotEntry << G4endl;
  //   //G4cerr << "fEntryVector.size() = " << fEntryVector.size() << G4endl;
  //   G4cerr << G4endl;
  //   const char *msg = "The # of events available in NEUT file is exceeded!";
  //   const char *origin = "ExN02RooTrackerKinematicsGenerator::GeneratePrimaryVertex";
  //   //const char *code = "if (fNextEntry >= fEntryVector.size()) {";
  //   const char *code = "if (fNextEntry >= fTotEntry)) {";
  //   G4Exception(origin,code,FatalException,msg);
  // }
  
  fneutfile->cd();
  
  
  // Create a new vertex to add the new particles, and add the vertex to the event
  
  if (!fneutree) {
    const char *msg = "NEUT tree is not open!";
    const char *origin = "ExN02RooTrackerKinematicsGenerator::GeneratePrimaryVertex";
    const char *code = "if (!fneutree) {";
    G4Exception(origin,code,FatalException,msg);
  }  

  // Take event ID and pass to the NEUT entry

  G4int evtID = anEvent->GetEventID();
  G4int treeEvtID = this->GetFirstEvent()+evtID; // tree event to pick up!!!

  G4cout << "--> Generate primary vertex from tree event: " << treeEvtID << G4endl;

  G4cout << "evtID = " << evtID << G4endl;
  G4cout << "treeEvtID = " << treeEvtID << G4endl;
  G4cout << "fTotEntry = " << fTotEntry << G4endl;



  //if(evtID > fTotEntry){
  if(treeEvtID >= fTotEntry){
    const char *msg = "Event ID exceeds the number of NEUT events!";
    const char *origin = "ExN02RooTrackerKinematicsGenerator::GeneratePrimaryVertex";
    const char *code = "if(evtID >= fTotEntry){";
    G4Exception(origin,code,FatalException,msg);
    //G4RunManager::GetRunManager()->AbortRun(true); // don't use it because the last to last event will be stored twice
    //anEvent->SetEventAborted();
  }



  // if(evtID != fCurrEntry){
  //   const char *msg = "Event ID different from current NEUT event!";
  //   const char *origin = "ExN02RooTrackerKinematicsGenerator::GeneratePrimaryVertex";
  //   const char *code = " if(evtID != fCurrEntry){";
  //   G4Exception(origin,code,FatalException,msg);
  // }
  
  //fneutree->GetEntry(evtID); // take tree entry using event ID
  fneutree->GetEntry(treeEvtID); // take tree entry using event ID + first event (for running on bunch of events) 
  
  // Set event ID to the input Tree event ID
  anEvent->SetEventID(treeEvtID);
  G4cout << "Set G4 event ID to input Tree event ID: " << anEvent->GetEventID() << G4endl;
  G4cout << G4endl;

  //
  // NB: The following code to add the vertex is taken from nd280mc code
  //     except a few minor differences (e.g. ND280LOG, the interaction vertex index, different G4 version...)
  //

  // Add primary vertex
  
  G4PrimaryVertex* theVertex 
    = new G4PrimaryVertex(G4ThreeVector(fEvtVtx[0]*m,
   					fEvtVtx[1]*m,
   					fEvtVtx[2]*m),
   			  fEvtVtx[3]*second);
  anEvent->AddPrimaryVertex(theVertex);

  // Add an information field to the vertex.
  ExN02VertexInfo *vertexInfo = new ExN02VertexInfo;
  theVertex->SetUserInformation(vertexInfo);

  // Fill the generator name
  vertexInfo->SetName("NEUT");

  // Fill the information fields for this vertex.
  vertexInfo->SetReaction(std::string(fEvtCode->String().Data()));
  // Set the file name for this event.
  std::ostringstream fs;
  //fs << fneutfile << ":" << entry;
  fs << fneutfile << ":" << evtID;
  vertexInfo->SetFilename(fs.str());
  // Set the interaction number to that of the RooTracker pass-through tree.
  //vertexInfo->SetInteractionNumber(ND280KinemPassThrough::GetInstance()->LastEntryNumber());
  vertexInfo->SetInteractionNumber(evtID);
  vertexInfo->SetCrossSection(fEvtXSec*1E-38*cm2);
  vertexInfo->SetDiffCrossSection(fEvtDXSec*1E-38*cm2);
  vertexInfo->SetWeight(fEvtWght);
  vertexInfo->SetProbability(fEvtProb);  
  
  // Add an informational vertex for storing the incoming 
  // neutrino particle and target nucleus.
  G4PrimaryVertex* theIncomingVertex 
    = new G4PrimaryVertex(G4ThreeVector(fEvtVtx[0]*m,
					fEvtVtx[1]*m,
					fEvtVtx[2]*m),
			  fEvtVtx[3]*second);
  vertexInfo->AddInformationalVertex(theIncomingVertex);
  
  // Add an information field to the vertex.
  ExN02VertexInfo *incomingVertexInfo = new ExN02VertexInfo;
  incomingVertexInfo->SetName("initial-state");
  incomingVertexInfo->SetReaction(std::string(fEvtCode->String().Data()));
  theIncomingVertex->SetUserInformation(incomingVertexInfo);
  
  // Fill the particles to be tracked (status ==1).  These particles are
  // attached to the primary vertex.  Also save the incident neutrino
  // particle and the incident target nucleus; these particles are attached
  // to informational vertex.
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4IonTable     * ionTable      = G4IonTable::GetIonTable();
  for (G4int cnt = 0; cnt < fStdHepN; ++cnt) {
    G4ParticleDefinition* particleDef
      = particleTable->FindParticle(fStdHepPdg[cnt]);
    if (!particleDef) {
      //maybe we have an ion; figure out if it makes any sense
      G4int ionA = (fStdHepPdg[cnt]/10) % 1000;
      G4int ionZ = (fStdHepPdg[cnt]/10000) % 1000;
      G4int type = (fStdHepPdg[cnt]/100000000);
      if (type == 10 && ionZ > 0 && ionA > ionZ) {
	//particleDef = particleTable->GetIon(ionZ, ionA, 0.*MeV);
	particleDef = ionTable->GetIon(ionZ, ionA, 0.*MeV);
      }
      else if (type == 20) {
	// This is a pseudo-particle so skip it.
	continue;
      }
    }
    
    // Determine a name for the particle.
    std::string particleName =
      particleDef ? particleDef->GetParticleName(): "unknown";
    
    // Get the momentum.
    G4LorentzVector momentum(fStdHepP4[cnt][0]*GeV,
			     fStdHepP4[cnt][1]*GeV,
			     fStdHepP4[cnt][2]*GeV,
			     fStdHepP4[cnt][3]*GeV);
    
    if (fStdHepStatus[cnt] != 1) {
      G4cout << "Untracked particle: " << cnt 
	     << " " << particleName
	     << " with " << momentum.e()/MeV 
	     << " MeV " 
	     << " w/ mothers " << fStdHepFm[cnt]
	     << " to " << fStdHepLm[cnt] 
	     << G4endl;        
      
      G4cout << fStdHepStatus[cnt] << G4endl;
    }

    
    // We are only interested in particles to be tracked (status==1)
    // or incident neutrino/target nucleus (status==0).
    if( !(fStdHepStatus[cnt] == 0 || fStdHepStatus[cnt] == 1))
      continue;
        
    if (!particleDef) {
      G4cout << "Particle code: " << fStdHepPdg[cnt] << G4endl;
      const char *msg = "Particle code not recognized (not tracking)";
      const char *origin = "ExN02RooTrackerKinematicsGenerator::GeneratePrimaryVertex";
      const char *code = " if (!particleDef) {";
      G4Exception(origin,code,JustWarning,msg);
      
      continue;
    }
    
    // create the particle.
    G4PrimaryParticle* theParticle
      = new G4PrimaryParticle(particleDef,
			      momentum.px(),
			      momentum.py(),
			      momentum.pz());
    theParticle->SetPolarization(fStdHepPolz[cnt][0],
				 fStdHepPolz[cnt][1],
				 fStdHepPolz[cnt][2]);
    
    if (fStdHepStatus[cnt] == 0) {
      theIncomingVertex->SetPrimary(theParticle);
    }else if (fStdHepStatus[cnt] == 1){
      theVertex->SetPrimary(theParticle);
    }
  }
  
  // Fill the particles at the decay vertex.  These are the first info
  // vertex.
  G4PrimaryVertex* theDecayVertex 
    = new G4PrimaryVertex(G4ThreeVector(fNuParentDecX4[0]*m,
					fNuParentDecX4[1]*m,
					fNuParentDecX4[2]*m),
			  fNuParentDecX4[3]*second);
  vertexInfo->AddInformationalVertex(theDecayVertex);
  
  // Add an information field to the vertex.
  ExN02VertexInfo *decayVertexInfo = new ExN02VertexInfo;
  decayVertexInfo->SetName("beam-particle:Decay");
  {
    std::ostringstream tmp;
    tmp << fNuParentDecMode;
    decayVertexInfo->SetReaction(tmp.str());
  }
  theDecayVertex->SetUserInformation(decayVertexInfo);
  
  G4PrimaryParticle* theDecayParticle
    = new G4PrimaryParticle(fNuParentPdg,
			    fNuParentDecP4[0]*GeV,
			    fNuParentDecP4[1]*GeV,
			    fNuParentDecP4[2]*GeV);
  theDecayVertex->SetPrimary(theDecayParticle);
  
  // Fill the particles at the production vertex.
  G4PrimaryVertex* theProductionVertex 
    = new G4PrimaryVertex(G4ThreeVector(fNuParentProX4[0]*m,
					fNuParentProX4[1]*m,
					fNuParentProX4[2]*m),
			  fNuParentProX4[3]*second);
  decayVertexInfo->AddInformationalVertex(theProductionVertex);
  
  // Add information about the production vertex.
  ExN02VertexInfo *productionVertexInfo = new ExN02VertexInfo;
  productionVertexInfo->SetName("beam-particle:Production");
  productionVertexInfo->SetInteractionNumber(fNuParentProNVtx);
  theProductionVertex->SetUserInformation(productionVertexInfo);
  
  G4PrimaryParticle* theProductionParticle
    = new G4PrimaryParticle(fNuParentPdg,
			    fNuParentProP4[0]*GeV,
			    fNuParentProP4[1]*GeV,
			    fNuParentProP4[2]*GeV);
  theProductionVertex->SetPrimary(theProductionParticle);
  
  fCurrEntry++; // move to next event
  

  // Read vertex inputs from NEUT  
  //G4cout << G4endl;
  //G4cout << "ExN02RooTrackerKinematicsGenerator::GeneratePrimaryVertex" << G4endl;
  //G4cout << "evtID = " << evtID << G4endl;
  //G4cout << "fCurrEntry = " << fCurrEntry << G4endl;
  //G4cout << "fTotEntry = " << fTotEntry << G4endl;
  //G4cout << "fNuParentPdg = " << fNuParentPdg << G4endl;
  //G4cout << "XSec = " << vertexInfo->GetCrossSection() << G4endl;
  //G4cout << "EvtWeight = " << vertexInfo->GetWeight() << G4endl;
  //G4cout << "EvtProb = " << vertexInfo->GetProbability() << G4endl;  
  //G4cout << "Primary Vertex: " << G4endl;
  //G4cout << " - fEvtVtx[0] = " << fEvtVtx[0] << " m" << G4endl;
  //G4cout << " - fEvtVtx[1] = " << fEvtVtx[1] << " m" << G4endl;
  //G4cout << " - fEvtVtx[2] = " << fEvtVtx[2] << " m" << G4endl;
  //G4cout << " - fEvtVtx[3]*second = " << fEvtVtx[3]*second << G4endl;  
  //G4cout << G4endl;
  
}







