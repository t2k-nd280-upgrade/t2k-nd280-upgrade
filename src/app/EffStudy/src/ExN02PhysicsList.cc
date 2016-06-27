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
// $Id: ExN02PhysicsList.cc,v 1.27 2009-11-15 14:27:30 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN02PhysicsList.hh"
#include "ExN02PhysicsListMessenger.hh"
#include "ExN02Constants.hh"

#include "ExN02StepMax.hh"

//#include "ExN02ExtraPhysics.hh"
#include <G4StepLimiter.hh>

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLowEPPhysics.hh"

#include "G4DecayPhysics.hh"

#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsFTF_BIC.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_FTFP_BERT.hh"
#include "G4HadronPhysicsQGS_BIC.hh"

#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4ChargeExchangePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4NeutronCrossSectionXS.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4IonPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmProcessOptions.hh"

#include "G4LossTableManager.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"

#include "G4SystemOfUnits.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"

#ifdef USE_PAI
  #include "G4PAIModel.hh"
  #include "G4PAIPhotonModel.hh"
  #include "G4EmConfigurator.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PhysicsList::ExN02PhysicsList() : 
  G4VModularPhysicsList(),
  //fEmPhysicsList(0),
  fDecPhysicsList(0),
  fEMPhys(),   
  fHadronPhys(),   
  fUsePAIModel(0),
  fSetHadronicPhysList(""),
  fPhysicsListMessenger(0),
  fStepMaxProcess(0)
{
  fStepMaxProcess  = 0;

  G4LossTableManager::Instance();

  defaultCutValue = 1.*mm;
  fCutForGamma     = defaultCutValue;
  fCutForElectron  = defaultCutValue;
  fCutForPositron  = defaultCutValue;

  SetVerboseLevel(3);

  fPhysicsListMessenger = new ExN02PhysicsListMessenger(this);
    
  // Deacy physics and all particles
  fDecPhysicsList = new G4DecayPhysics("decays"); // w/o "decays" option is same

  // EM physics --> EM3
  AddPhysicsList("emstandard_opt3");
  
  // Set the default hadronic physics.
  AddPhysicsList("QGSP_BERT");
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PhysicsList::~ExN02PhysicsList()
{
  delete fPhysicsListMessenger;
  delete fDecPhysicsList;

  // delete fEmPhysicsList;
  ClearEMPhysics();
    
  ClearHadronPhysics();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ClearEMPhysics() {

  for(size_t i=0; i<fEMPhys.size(); i++) {
    delete fEMPhys[i];
  }
  fEMPhys.clear();
  
  // for(std::vector<G4VPhysicsConstructor*>::iterator p = fEMPhys.begin();
  //     p != fEMPhysics.end();
  //     ++p) {
  //   delete (*p);
  // }
  // fEMPhys.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ClearHadronPhysics() {

  for(size_t i=0; i<fHadronPhys.size(); i++) {
    delete fHadronPhys[i];
  }
  fHadronPhys.clear();

  // for(std::vector<G4VPhysicsConstructor*>::iterator p = fHadronPhys.begin();
  //     p != fHadronPhys.end();
  //     ++p) {
  //   delete (*p);
  // }
  // fHadronPhys.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ConstructParticle()
{
  fDecPhysicsList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ConstructProcess()
{
  // transportation
  //
  AddTransportation();
  
  // electromagnetic physics list
  //
  //fEmPhysicsList->ConstructProcess();
  for(size_t i=0; i<fEMPhys.size(); i++) {
    fEMPhys[i]->ConstructProcess();
  }

#ifdef USE_PAI
  AddPAIModel();
  em_config->AddModels();
#endif

  // decay physics list
  //
  fDecPhysicsList->ConstructProcess();
  
  // hadronic physics lists
  for(size_t i=0; i<fHadronPhys.size(); i++) {
    fHadronPhys[i]->ConstructProcess();
  }

  //
  // step limitation (as a full process)
  //
  AddStepMax();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::RemoveFromEMPhysicsList(const G4String& name) {

  G4bool success = false;
  for(std::vector<G4VPhysicsConstructor*>::iterator p = fEMPhys.begin();
      p != fEMPhys.end(); ++p) {
    G4VPhysicsConstructor* e = (*p);
    if (e->GetPhysicsName() == name) {
      fEMPhys.erase(p);
      success = true;
      break;
    }
  }
  if (!success) {
    G4ExceptionDescription msg;
    msg << "PhysicsList::RemoveFromEMPhysicsList "<< name << "not found";  
    G4Exception("ExN02PhysicsList::RemoveFromEMPhysicsList",
		"MyCode0002",FatalException, msg);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::RemoveFromHadronPhysicsList(const G4String& name) {

  G4bool success = false;
  for(std::vector<G4VPhysicsConstructor*>::iterator p = fHadronPhys.begin();
      p != fHadronPhys.end(); ++p) {
    G4VPhysicsConstructor* e = (*p);
    if (e->GetPhysicsName() == name) {
      fHadronPhys.erase(p);
      success = true;
      break;
    }
  }
  if (!success) {
    G4ExceptionDescription msg;
    msg << "PhysicsList::RemoveFromHadronPhysicsList "<< name << "not found";  
    G4Exception("ExN02PhysicsList::RemoveFromHadronPhysicsList",
		"MyCode0002",FatalException, msg);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::AddPhysicsList(const G4String& name)
{
  if (verboseLevel>1) {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
  }

  if (name == fEmName) return;

  if (name == "emstandard_opt0") {
    fEmName = name;
    ClearEMPhysics();
    fEMPhys.push_back(new G4EmStandardPhysics());
    //fEMPhys.push_back(new ExN02ExtraPhysics());
    //delete fEmPhysicsList;
    //fEmPhysicsList = new G4EmStandardPhysics();    
  } 
  else if (name == "emstandard_opt1") {
    fEmName = name;
    ClearEMPhysics();
    fEMPhys.push_back(new G4EmStandardPhysics_option1());
    //fEMPhys.push_back(new ExN02ExtraPhysics());
    //delete fEmPhysicsList;
    //fEmPhysicsList = new G4EmStandardPhysics_option1();
  } 
  else if (name == "emstandard_opt2") {
    fEmName = name;
    ClearEMPhysics();
    fEMPhys.push_back(new G4EmStandardPhysics_option2());
    //fEMPhys.push_back(new ExN02ExtraPhysics());
    //delete fEmPhysicsList;
    //fEmPhysicsList = new G4EmStandardPhysics_option2();
  } 
  else if (name == "emstandard_opt3") {
    fEmName = name;
    ClearEMPhysics();
    fEMPhys.push_back(new G4EmStandardPhysics_option3());
    //fEMPhys.push_back(new ExN02ExtraPhysics());
    //delete fEmPhysicsList;
    //fEmPhysicsList = new G4EmStandardPhysics_option3();    
  } 
  else if (name == "emstandard_opt4") {
    fEmName = name;
    ClearEMPhysics();
    fEMPhys.push_back(new G4EmStandardPhysics_option4());
    //fEMPhys.push_back(new ExN02ExtraPhysics());
    //delete fEmPhysicsList;
    //fEmPhysicsList = new G4EmStandardPhysics_option4();    
  } 
  else if (name == "emlivermore") {
    fEmName = name;
    ClearEMPhysics();
    fEMPhys.push_back(new G4EmLivermorePhysics());
    //fEMPhys.push_back(new ExN02ExtraPhysics());
    //delete fEmPhysicsList;
    //fEmPhysicsList = new G4EmLivermorePhysics();
  } 
  else if (name == "empenelope") {
    fEmName = name;
    ClearEMPhysics();
    fEMPhys.push_back(new G4EmPenelopePhysics());
    //fEMPhys.push_back(new ExN02ExtraPhysics());
    //delete fEmPhysicsList;
    //fEmPhysicsList = new G4EmPenelopePhysics();
  }
  else if (name == "emlowenergy") {
    fEmName = name;
    ClearEMPhysics();
    fEMPhys.push_back(new G4EmLowEPPhysics());
    //fEMPhys.push_back(new ExN02ExtraPhysics());
    //delete fEmPhysicsList;
    //fEmPhysicsList = new G4EmLowEPPhysics();
  }
  else if (name == "QGSP_BERT") {
    SetBuilderList1();
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_BERT());
  } 
  else {  
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
           << " is not defined"
           << G4endl;
    G4ExceptionDescription msg;
    msg << "The physics list " << name << " is not available" << G4endl;
    G4Exception("ExN02PhysicsList::AddPhysicsList()",
		"MyCode0002",FatalException, msg);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::SetBuilderList0(G4bool flagHP)
{
    ClearHadronPhysics();
    
    // Used in nd280mc (old G4 version 4.9.6)
    // fHadronPhys.push_back(new G4EmExtraPhysics("extra EM"));
    // fHadronPhys.push_back(new G4HadronElasticPhysics("elastic",
    //                                                     verboseLevel,
    //                                                     flagHP));
    // fHadronPhys.push_back(new  G4QStoppingPhysics("stopping",
    //                                                  verboseLevel));
    // fHadronPhys.push_back(new G4IonBinaryCascadePhysics("ionBIC"));
    // fHadronPhys.push_back(new G4NeutronTrackingCut("nTackingCut",
    //                                                  verboseLevel));

    // default is "EM extra". Method with string option is obsolete (see G4 code)
    fHadronPhys.push_back(new G4EmExtraPhysics(verboseLevel)); 
    if(flagHP) {
      fHadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
    } else {
      fHadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
    }
    
    // G4QStoppingPhysics not available any longer
    fHadronPhys.push_back(new G4StoppingPhysics("stopping",verboseLevel));

    fHadronPhys.push_back(new G4IonBinaryCascadePhysics("ionBIC",verboseLevel));
    fHadronPhys.push_back(new G4NeutronTrackingCut("nTrackingCut",verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::SetBuilderList1(G4bool flagHP)
{
    ClearHadronPhysics();

    // Used in nd280mc (old G4 version 4.9.6)
    // fHadronPhys.push_back(new G4EmExtraPhysics("extra EM"));
    // fHadronPhys.push_back(new G4HadronElasticPhysics("elastic",
    //                                                     verboseLevel,
    //                                                     flagHP));
    // fHadronPhys.push_back(new  G4QStoppingPhysics("stopping",
    //                                                  verboseLevel));
    // fHadronPhys.push_back(new G4IonBinaryCascadePhysics("ion"));
    // fHadronPhys.push_back(new G4NeutronTrackingCut("nTackingCut",
    //                                                  verboseLevel));

    fHadronPhys.push_back(new G4EmExtraPhysics(verboseLevel));
    if(flagHP) {
      fHadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
    } else {
      fHadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
    }
    
    // G4QStoppingPhysics not available any longer
    fHadronPhys.push_back(new G4StoppingPhysics("stopping",verboseLevel));
    
    fHadronPhys.push_back(new G4IonBinaryCascadePhysics("ion",verboseLevel));
    fHadronPhys.push_back(new G4NeutronTrackingCut("nTackingCut",verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




void ExN02PhysicsList::SetCuts() {

  G4cout << " ExN02PhysicsList::SetCuts " 
  	 << G4BestUnit(defaultCutValue,"Length")
  	 << G4endl;
  
  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(fCutForGamma, "gamma","DefaultRegionForTheWorld");
  SetCutValue(fCutForElectron, "e-","DefaultRegionForTheWorld");
  SetCutValue(fCutForPositron, "e+","DefaultRegionForTheWorld");
  
  //
  // Hall region (around the detector) is not defined in the simulation
  //  
  // G4Region* hallRegion = G4RegionStore::GetInstance()->
  //                                GetRegion("hallRegion",false);
  // if (hallRegion) {
  //    G4ProductionCuts* hallCuts = new G4ProductionCuts();
  //    hallCuts->SetProductionCut(10*mm);
  //    hallRegion->SetProductionCuts(hallCuts);
  // } else {
  //    ND280Error("hallRegion does not exist");
  //    G4Exception("ND280PhysicsList::SetCuts() found no hallRegion");
  // }
  //
  
#ifdef USE_PAI
    G4Region* driftRegion = G4RegionStore::GetInstance()->GetRegion("driftRegion",false);
    if (driftRegion) {
       G4ProductionCuts* defaultRegionCuts = G4ProductionCutsTable::
                    GetProductionCutsTable()->GetDefaultProductionCuts();
       G4ProductionCuts* driftRegionCuts = new G4ProductionCuts();
       *driftRegionCuts = *defaultRegionCuts;
       driftRegion->SetProductionCuts(driftRegionCuts);
       //       driftRegion->SetProductionCuts(G4ProductionCutsTable::
       //                    GetProductionCutsTable()->GetDefaultProductionCuts());
    } else {
      G4ExceptionDescription msg;
      msg << "The driftRegion is not found" << G4endl;
      G4Exception("ExN02PhysicsList::SetCuts() found no driftRegion",
    		  "MyCode0002",FatalException, msg); 
    }
#endif
    
    if (verboseLevel>0) DumpCutValuesTable(); // see the content of the physics list
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::SetCutForGamma(G4double cut) {
    fCutForGamma = cut;
    SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double ExN02PhysicsList::GetCutForGamma() const {
    return fCutForGamma;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::SetCutForElectron(G4double cut) {
    fCutForElectron = cut;
    SetParticleCuts(fCutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double ExN02PhysicsList::GetCutForElectron() const {
    return fCutForElectron;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::SetCutForPositron(G4double cut) {
    fCutForPositron = cut;
    SetParticleCuts(fCutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double ExN02PhysicsList::GetCutForPositron() const {
    return fCutForPositron;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifdef USE_PAI
void ExN02PhysicsList::AddPAIModel()
{
  em_config = G4LossTableManager::Instance()->EmConfigurator();
  // em_config->SetVerbose(2);
  
  theParticleIterator->reset();
  while ((*theParticleIterator)())
    {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4String particleName = particle->GetParticleName();
      
      if(particleName == "e-" || particleName == "e+") {
        G4PAIModel* pai = new G4PAIModel(particle,"PAIModel");
        em_config->SetExtraEmModel(particleName,"eIoni",pai,"driftRegion",
				   0.0,100.*GeV,pai);
      } else if (particleName == "mu-" || particleName == "mu+") {
        G4PAIModel* pai = new G4PAIModel(particle,"PAIModel");
        em_config->SetExtraEmModel(particleName,"muIoni",pai,"driftRegion",
                                                           0.0,100.*GeV,pai);
      } else if (particleName == "proton" ||
                 particleName == "pi+" ||
                 particleName == "pi-" ) {
        G4PAIModel* pai = new G4PAIModel(particle,"PAIModel");
        em_config->SetExtraEmModel(particleName,"hIoni",pai,"driftRegion",
                                                           0.0,100.*GeV,pai);
      }
    }
}
#endif

void ExN02PhysicsList::AddStepMax(){
  G4cout << " ExN02PhysicsList::AddStepMax:: Add Step Limiters" << G4endl;
  
  //
  // From example TestEm7 (modified)
  //
  // Step limitation seen as a process
   
  fStepMaxProcess = new ExN02StepMax();
  
  theParticleIterator->reset();
  while ((*theParticleIterator)()){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    G4String particleType = particle->GetParticleType();
    G4double charge = particle->GetPDGCharge();

    if (fStepMaxProcess->IsApplicable(*particle) && pmanager)
      {
	pmanager ->AddDiscreteProcess(fStepMaxProcess);
      }
    else if (!pmanager) {
      G4ExceptionDescription msg;
      msg << "Particle without a Process Manager" << G4endl;
      G4Exception("ExN02ExtraPhysics::ConstructProcess()",
                  "MyCode0002",FatalException, msg);
    }
  }
}
