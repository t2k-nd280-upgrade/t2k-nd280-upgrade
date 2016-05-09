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

#define USE_PAI

#include "ExN02PhysicsList.hh"

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

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"
#include "G4SystemOfUnits.hh"

#ifdef USE_PAI
#include "G4PAIModel.hh"
#include "G4PAIPhotonModel.hh"
#include "G4EmConfigurator.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PhysicsList::ExN02PhysicsList() : 
  G4VModularPhysicsList(),
  fEmPhysicsList(0),
  fDecPhysicsList(0),
  fHadronPhys()    
  //fStepMaxProcess(0),
  //fMessenger(0)
{
  G4LossTableManager::Instance();

  //fStepMaxProcess  = 0;

  //fMessenger = new PhysicsListMessenger(this);

  SetVerboseLevel(1);
  
  // EM physics --> EM3
  AddPhysicsList("emstandard_opt3");

  // Deacy physics and all particles
  fDecPhysicsList = new G4DecayPhysics("decays"); // w/o "decays" option is the same
  
  // Set the default hadronic physics.
  AddPhysicsList("QGSP_BERT");
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PhysicsList::~ExN02PhysicsList()
{
  //delete fMessenger;
  delete fEmPhysicsList;
  delete fDecPhysicsList;

  //ClearEMPhysics();
  ClearHadronPhysics();

  for(size_t i=0; i<fHadronPhys.size(); i++) {delete fHadronPhys[i];}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// void ExN02PhysicsList::ClearEMPhysics() {
//     for(PhysicsListVector::iterator p = fEMPhysics->begin();
//         p != fEMPhysics->end();
//         ++p) {
//         delete (*p);
//     }
//     fEMPhysics->clear();
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ClearHadronPhysics() {
  for(std::vector<G4VPhysicsConstructor*>::iterator p = fHadronPhys.begin();
      p != fHadronPhys.end();
      ++p) {
    delete (*p);
  }
  fHadronPhys.clear();
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
  fEmPhysicsList->ConstructProcess();

  // decay physics list
  //
  fDecPhysicsList->ConstructProcess();
  
#ifdef USE_PAI
  AddPAIModel();
  em_config->AddModels();
#endif

  // hadronic physics lists
  for(size_t i=0; i<fHadronPhys.size(); i++) {
    fHadronPhys[i]->ConstructProcess();
  }
  
  // step limitation (as a full process)
  //  
  //AddStepMax();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::AddPhysicsList(const G4String& name)
{
  if (verboseLevel>1) {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
  }

  if (name == fEmName) return;

  // if (name == "local") {
  //   fEmName = name;
  //   delete fEmPhysicsList;
  //   fEmPhysicsList = new PhysListEmStandard(name);
  // } else if (name == "emstandard_opt0") {

  if (name == "emstandard_opt0") {
    
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics(1);

  } else if (name == "emstandard_opt1") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option1();

  } else if (name == "emstandard_opt2") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option2();
    
  } else if (name == "emstandard_opt3") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option3();    
    
  } else if (name == "emstandard_opt4") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option4();    

  // } else if (name == "standardSS") {

  //   fEmName = name;
  //   delete fEmPhysicsList;
  //   fEmPhysicsList = new PhysListEmStandardSS(name);

  // } else if (name == "ionGasModels") {

  //   AddPhysicsList("emstandard_opt0");
  //   fEmName = name;
  //   AddIonGasModels();

  // } else if (name == "standardNR") {

  //   fEmName = name;
  //   delete fEmPhysicsList;
  //   fEmPhysicsList = new PhysListEmStandardNR(name);

  } else if (name == "emlivermore") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmLivermorePhysics();

  } else if (name == "empenelope") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmPenelopePhysics();

  } else if (name == "emlowenergy") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmLowEPPhysics();
    
  } else if (name == "QGSP_BERT") {
    SetBuilderList1();
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_BERT());

  } else {
    
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
           << " is not defined"
           << G4endl;
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
    
    // G4StoppingPhysics not available any longer
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
    
    // G4StoppingPhysics not available any longer
    fHadronPhys.push_back(new G4StoppingPhysics("stopping",verboseLevel));
    
    fHadronPhys.push_back(new G4IonBinaryCascadePhysics("ion",verboseLevel));
    fHadronPhys.push_back(new G4NeutronTrackingCut("nTackingCut",verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void ExN02PhysicsList::SetBuilderList2(G4bool flagHP)
// {
//     ClearHadronPhysics();
//     fHadronPhys.push_back(new G4EmExtraPhysics("extra EM"));
//     fHadronPhys.push_back(new G4HadronElasticPhysics("LElastic",
//                                                         verboseLevel,
//                                                         flagHP));
//     fHadronPhys.push_back(new G4IonBinaryCascadePhysics("ion"));
// }
// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void ExN02PhysicsList::SetBuilderList3(G4bool flagHP)
// {
//     ClearHadronPhysics();
//     fHadronPhys.push_back(new G4EmExtraPhysics("extra EM"));
//     fHadronPhys.push_back(new G4HadronElasticPhysics("LElastic",
//                                                         verboseLevel,
//                                                         flagHP));
//     fHadronPhys.push_back(new  G4QStoppingPhysics("stopping",
//                                                      verboseLevel));
//     fHadronPhys.push_back(new G4IonBinaryCascadePhysics("ion"));
// }
// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void ExN02PhysicsList::SetBuilderList4(G4bool)
// {
//     ClearHadronPhysics();
//     fHadronPhys.push_back(new G4EmExtraPhysics("extra EM"));
//     fHadronPhys.push_back(new G4HadronQElasticPhysics("elastic",
//                                                         verboseLevel));
//     fHadronPhys.push_back(new  G4QStoppingPhysics("stopping",
//                                                      verboseLevel));
//     fHadronPhys.push_back(new G4IonBinaryCascadePhysics("ion"));
//     fHadronPhys.push_back(new G4NeutronTrackingCut("nTackingCut",
//                                                      verboseLevel));
// }
// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void ExN02PhysicsList::SetBuilderList5(G4bool flagHP)
// {
//     ClearHadronPhysics();
//     fHadronPhys.push_back(new G4EmExtraPhysics("extra EM"));
//     fHadronPhys.push_back(new G4HadronDElasticPhysics(verboseLevel,
//                                                           flagHP));
//     fHadronPhys.push_back(new  G4QStoppingPhysics("stopping",
//                                                      verboseLevel));
//     fHadronPhys.push_back(new G4IonBinaryCascadePhysics("ionBIC"));
//     fHadronPhys.push_back(new G4NeutronTrackingCut("nTackingCut",
//                                                      verboseLevel));
// }
// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void ExN02PhysicsList::SetBuilderList6(G4bool flagHP)
// {
//     ClearHadronPhysics();
//     fHadronPhys.push_back(new G4EmExtraPhysics("extra EM"));
//     fHadronPhys.push_back(new G4HadronHElasticPhysics(verboseLevel,
//                                                           flagHP));
//     fHadronPhys.push_back(new  G4QStoppingPhysics("stopping",
//                                                      verboseLevel));
//     fHadronPhys.push_back(new G4IonBinaryCascadePhysics("ionBIC"));
//     fHadronPhys.push_back(new G4NeutronTrackingCut("nTackingCut",
//                                                      verboseLevel));
// }
// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifdef USE_PAI
void ND280PhysicsList::AddPAIModel()
{
    em_config = G4LossTableManager::Instance()->EmConfigurator();
//    em_config->SetVerbose(2);

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
