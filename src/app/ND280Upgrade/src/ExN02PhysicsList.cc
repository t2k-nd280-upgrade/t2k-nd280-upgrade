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

#include "globals.hh"
#include "ExN02PhysicsList.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"

#include "G4LossTableManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PhysicsList::ExN02PhysicsList():  G4VUserPhysicsList()
{
  defaultCutValue = 1.0*cm;
   SetVerboseLevel(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PhysicsList::~ExN02PhysicsList()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4BosonConstructor.hh"
void ExN02PhysicsList::ConstructBosons()
{
  // // pseudo-particles
  // G4Geantino::GeantinoDefinition();
  // G4ChargedGeantino::ChargedGeantinoDefinition();
  
  // // gamma
  // G4Gamma::GammaDefinition();  

  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ConstructLeptons()
{
  // leptons
  //  e+/-
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  // mu+/-
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  // // tau+/-
  // G4TauMinus::TauMinusDefinition();
  // G4TauPlus::TauPlusDefinition();
  // nu_e
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  // nu_mu
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
  // // nu_tau
  // G4NeutrinoTau::NeutrinoTauDefinition();
  // G4AntiNeutrinoTau::AntiNeutrinoTauDefinition();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"

void ExN02PhysicsList::ConstructMesons()
{
  //  mesons
  //    light mesons
  // G4PionPlus::PionPlusDefinition();
  // G4PionMinus::PionMinusDefinition();
  // G4PionZero::PionZeroDefinition();
  // G4Eta::EtaDefinition();
  // G4EtaPrime::EtaPrimeDefinition();
  // G4KaonPlus::KaonPlusDefinition();
  // G4KaonMinus::KaonMinusDefinition();
  // G4KaonZero::KaonZeroDefinition();
  // G4AntiKaonZero::AntiKaonZeroDefinition();
  // G4KaonZeroLong::KaonZeroLongDefinition();
  // G4KaonZeroShort::KaonZeroShortDefinition();
  
  //  mesons
  G4MesonConstructor mConstructor;
  mConstructor.ConstructParticle();
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ConstructBaryons()
{
  // //  barions
  // G4Proton::ProtonDefinition();
  // G4AntiProton::AntiProtonDefinition();

  // G4Neutron::NeutronDefinition();
  // G4AntiNeutron::AntiNeutronDefinition();

  //  baryons
  G4BaryonConstructor bConstructor;
  bConstructor.ConstructParticle();  
}

void ExN02PhysicsList::ConstructIons()
{/*
   G4Deuteron::Deuteron();
   G4Triton::Triton();
   G4He3::He3();
   G4Alpha::Alpha();
   G4GenericIon::GenericIon();
 */
 //  ions
  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle();
}

#include "G4ShortLivedConstructor.hh"

void ExN02PhysicsList::ConstructShortLived(){

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM("EM3"); // TO CHECK
  //ConstructHad("QGSP_BIC_HP");
  //ConstructHad("QBBC_HP");
  //ConstructHad("");
  ConstructGeneral();
  AddStepMax();
}

#include "G4StepLimiter.hh"

void ExN02PhysicsList::AddTransportation() {
  
  G4VUserPhysicsList::AddTransportation();
  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle     = theParticleIterator->value();
    G4ProcessManager*     pmanager     = particle->GetProcessManager();
    G4String              particleName = particle->GetParticleName();
    
    // Step limit applied to all particles:
    if(pmanager){
	pmanager->AddProcess(new G4StepLimiter,-1,-1,1);
	pmanager->SetVerboseLevel(VerboseLevel);
    }    
  }	
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4ionIonisation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ConstructEM()
{
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
     
    if (particleName == "gamma") {
      // gamma         
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);
      pmanager->AddDiscreteProcess(new G4GammaConversion);
      
    } else if (particleName == "e-") {
      //electron
      pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);      

    } else if (particleName == "e+") {
      //positron
      pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation,    0,-1, 4);

    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      //muon  
      pmanager->AddProcess(new G4MuMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung,     -1, 3, 3);
      pmanager->AddProcess(new G4MuPairProduction,     -1, 4, 4);       
             
    } else if( particleName == "proton" ||
               particleName == "pi-" ||
               particleName == "pi+"    ) {
      //proton  
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4hBremsstrahlung,     -1, 3, 3);
      pmanager->AddProcess(new G4hPairProduction,     -1, 4, 4);       
     
    } else if( particleName == "alpha" || 
	       particleName == "He3" )     {
      //alpha 
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4ionIonisation,       -1, 2, 2);
     
    } else if( particleName == "GenericIon" ) { 
      //Ions 
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4ionIonisation,       -1, 2, 2);
                 
    } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino")) {
      //all others charged particles except geantino
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
    }
  }

  //extend binning of PhysicsTables
  //
  G4LossTableManager::Instance()->SetMinEnergy(100*eV);  
  G4LossTableManager::Instance()->SetMaxEnergy(1000*PeV);
  G4LossTableManager::Instance()->SetDEDXBinning(160);
  G4LossTableManager::Instance()->SetLambdaBinning(160);
  G4LossTableManager::Instance()->SetSplineFlag(true);  
  G4LossTableManager::Instance()->SetVerbose(0);

}


// Electromagnetic Processes ---------------------------------------------------
// all charged particles

// gamma
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4LivermoreRayleighModel.hh"

//
// NOT AVAILABLE IN THIS GEANT4 VERSION
//
//#include "G4LowEPComptonModel.hh"

// e-
#include "G4eMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"

#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

// e+
#include "G4eIonisation.hh" 
#include "G4eBremsstrahlung.hh" 
#include "G4eplusAnnihilation.hh"


// alpha and GenericIon and deuterons, triton, He3:
//hIonisation #include "G4hLowEnergyIonisation.hh" -> moved to G4hIonisation
#include "G4EnergyLossTables.hh"
// hLowEnergyIonisation uses Ziegler 1988 as the default


//muon:
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonMinusCaptureAtRest.hh"

//OTHERS:
#include "G4hIonisation.hh" 
#include "G4hMultipleScattering.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"

//em process options to allow msc step-limitation to be switched off
#include "G4EmProcessOptions.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"

//
// NOT AVAILABLE IN THIS GEANT4 VERSION
//
//#include "G4EmStandardPhysics_option4.hh"

void ExN02PhysicsList::ConstructEM(std::string emlist){
  
  if(emlist.find("EM0") != std::string::npos || emlist.find("emstandard") != std::string::npos){
    G4EmStandardPhysics* EmStandardPhysics = new G4EmStandardPhysics();
    EmStandardPhysics->ConstructProcess();
    delete EmStandardPhysics;
  }
  else if(emlist.find("EM1") != std::string::npos || emlist.find("emstandard_opt1") != std::string::npos){
    G4EmStandardPhysics_option1* EmStandardPhysics = new G4EmStandardPhysics_option1();
    EmStandardPhysics->ConstructProcess();
    delete EmStandardPhysics;
  }
  else if(emlist.find("EM2") != std::string::npos || emlist.find("emstandard_opt2") != std::string::npos){
    G4EmStandardPhysics_option2* EmStandardPhysics = new G4EmStandardPhysics_option2();
    EmStandardPhysics->ConstructProcess();
    delete EmStandardPhysics;
  }
  else if(emlist.find("EM3") != std::string::npos || emlist.find("emstandard_opt3") != std::string::npos){
    G4EmStandardPhysics_option3* EmStandardPhysics = new G4EmStandardPhysics_option3();
    EmStandardPhysics->ConstructProcess();
    delete EmStandardPhysics;
  }

  //
  // NOT AVAILABLE IN THIS GEANT4 VERSION
  //
  //else if(emlist.find("EM4") != std::string::npos || emlist.find("emstandard_opt4") != std::string::npos){
  //G4EmStandardPhysics_option4* EmStandardPhysics = new G4EmStandardPhysics_option4();
  //EmStandardPhysics->ConstructProcess();
  //delete EmStandardPhysics;
  //}

  else{
    //set a finer grid of the physic tables in order to improve precision
    //former LowEnergy models have 200 bins up to 100 GeV
    G4EmProcessOptions opt;
    opt.SetMaxEnergy(100*GeV);
    opt.SetDEDXBinning(200);
    opt.SetLambdaBinning(200);
    
    theParticleIterator->reset();
    while( (*theParticleIterator)() ){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      G4String particleName = particle->GetParticleName();
      G4String particleType = particle->GetParticleType();
      G4double charge = particle->GetPDGCharge();
      
      if(pmanager == NULL)continue;
      pmanager->SetVerboseLevel(VerboseLevel);

      if (particleName == "gamma") 
	{
	  //gamma
	  G4RayleighScattering* theRayleigh = new G4RayleighScattering();
	  //
	  // NOT AVAILABLE IN THIS GEANT4 VERSION
	  //
	  //theRayleigh->SetEmModel(new G4LivermoreRayleighModel());  //not strictly necessary
	  //pmanager->AddDiscreteProcess(theRayleigh);
	  
	  G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
	  //
	  // NOT AVAILABLE IN THIS GEANT4 VERSION
	  //
	  //thePhotoElectricEffect->SetEmModel(new G4LivermorePhotoElectricModel());
	  //pmanager->AddDiscreteProcess(thePhotoElectricEffect);
	  
	  G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
	  //
	  // NOT AVAILABLE IN THIS GEANT4 VERSION
	  //
	  //theComptonScattering->SetEmModel(new G4LivermoreComptonModel());
	  
	  //
	  // NOT AVAILABLE IN THIS GEANT4 VERSION (not used)
	  //
	  //theComptonScattering->SetEmModel(new G4LowEPComptonModel());
	  pmanager->AddDiscreteProcess(theComptonScattering);

	  G4GammaConversion* theGammaConversion = new G4GammaConversion();
	  //
	  // NOT AVAILABLE IN THIS GEANT4 VERSION (not used)
	  //
	  //theGammaConversion->SetEmModel(new G4LivermoreGammaConversionModel());
	  //pmanager->AddDiscreteProcess(theGammaConversion);
	  
	  delete theRayleigh;
	  delete thePhotoElectricEffect;
	  delete theComptonScattering;
	  delete theGammaConversion;
	} 
      else if (particleName == "e-") 
	{
	  //electron
	  // process ordering: AddProcess(name, at rest, along step, post step)
	  // Multiple scattering
	  G4eMultipleScattering* msc = new G4eMultipleScattering();
	  //xioni	msc->SetStepLimitType(fUseDistanceToBoundary);
	  //xmsc
	  pmanager->AddProcess(msc,-1, 1, 1);
	  
	  // Ionisation
	  G4eIonisation* eIonisation = new G4eIonisation();
	  //xioni	eIonisation->SetEmModel(new G4LivermoreIonisationModel());
	  //xioni	eIonisation->SetStepFunction(0.2, 100*um); //improved precision in tracking  
	  pmanager->AddProcess(eIonisation,-1, 2, 2);
	  
	  // Bremsstrahlung
	  G4eBremsstrahlung* eBremsstrahlung = new G4eBremsstrahlung();
	  eBremsstrahlung->SetEmModel(new G4LivermoreBremsstrahlungModel());
	  pmanager->AddProcess(eBremsstrahlung, -1,-3, 3);

	  delete msc;
	  delete eIonisation;
	  delete eBremsstrahlung;
	} 
      else if (particleName == "e+") 
	{
	  //positron	
	  G4eMultipleScattering* msc = new G4eMultipleScattering();
	  msc->SetStepLimitType(fUseDistanceToBoundary);
	  pmanager->AddProcess(msc,-1, 1, 1);
	  
	  // Ionisation
	  G4eIonisation* eIonisation = new G4eIonisation();
	  //xioni	eIonisation->SetStepFunction(0.2, 100*um); //     
	  pmanager->AddProcess(eIonisation,                 -1, 2, 2);
	  
	  //Bremsstrahlung (use default, no low-energy available)
	  pmanager->AddProcess(new G4eBremsstrahlung(), -1,-1, 3);
	  
	  //Annihilation
	  pmanager->AddProcess(new G4eplusAnnihilation(),0,-1, 4);

	  delete msc;
	  delete eIonisation; 
	} 
      else if( particleName == "mu+" || 
	       particleName == "mu-"    ) 
	{
	  //muon  
	  pmanager->AddProcess(new G4eMultipleScattering,           -1, 1, 1);
	  pmanager->AddProcess(new G4MuIonisation(),          -1, 2, 2);
	  pmanager->AddProcess(new G4MuBremsstrahlung(),      -1,-1, 3);
	  pmanager->AddProcess(new G4MuPairProduction(),      -1,-1, 4);
	  if( particleName == "mu-" )
	    pmanager->AddProcess(new G4MuonMinusCaptureAtRest(), 0,-1,-1);
	} 
      else if (particleName == "proton" || 
	       particleName == "pi+"    || 
	       particleName == "pi-")
	{
	  //multiple scattering
	  //xmsc
	  pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
	  
	  //ionisation
	  G4hIonisation* hIonisation = new G4hIonisation();
	  //xioni	hIonisation->SetStepFunction(0.2, 50*um);
	  pmanager->AddProcess(hIonisation,                     -1, 2, 2);      
	  
	  //bremmstrahlung
	  pmanager->AddProcess(new G4hBremsstrahlung,     -1,-3, 3);

	  delete hIonisation;

	}
      else if(particleName  == "alpha"      ||
	      particleName == "deuteron"   ||
	      particleName == "triton"     ||
	      particleName == "He3")
	{
	  //multiple scattering
	  //xmsc
	  pmanager->AddProcess(new G4hMultipleScattering,-1,1,1);
	  
	  //ionisation
	  G4ionIonisation* ionIoni = new G4ionIonisation();
	  //xioni	ionIoni->SetStepFunction(0.1, 20*um);
	  pmanager->AddProcess(ionIoni,                   -1, 2, 2);

	  delete ionIoni;
	}
      else if (particleName == "GenericIon")
	{
	  // OBJECT may be dynamically created as either a GenericIon or nucleus
	  // G4Nucleus exists and therefore has particle type nucleus
	  // genericIon:
	  
	  //multiple scattering
	  //xmsc
	  pmanager->AddProcess(new G4hMultipleScattering,-1,1,1);
	  
	  //ionisation
	  G4ionIonisation* ionIoni = new G4ionIonisation();
	  //xioni	ionIoni->SetEmModel(new G4IonParametrisedLossModel());
	  //xioni	ionIoni->SetStepFunction(0.1, 20*um);
	  pmanager->AddProcess(ionIoni,                   -1, 2, 2);

	  delete ionIoni;
	} 
      
      else if ((!particle->IsShortLived()) &&
	       (charge != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino")) 
	{
	  //all others charged particles except geantino
	  G4hMultipleScattering* aMultipleScattering = new G4hMultipleScattering();
	  G4hIonisation* ahadronIon = new G4hIonisation();
	  
	  //multiple scattering
	  //xmsc
	  pmanager->AddProcess(aMultipleScattering,-1,1,1);
	  
	  //ionisation
	  pmanager->AddProcess(ahadronIon,       -1,2,2);      

	  delete aMultipleScattering;
	  delete ahadronIon;
	}
      
    }
  
    // turn off msc step-limitation - especially as electron cut 1nm
    //xmsc
    //xioni  opt.SetMscStepLimitation(fMinimal);
    
  }
}



// Hadronic processes ////////////////////////////////////////////////////////

// Elastic processes:
#include "G4HadronElasticProcess.hh"

// Inelastic processes:
#include "G4PionPlusInelasticProcess.hh"
#include "G4PionMinusInelasticProcess.hh"
#include "G4KaonPlusInelasticProcess.hh"
#include "G4KaonZeroSInelasticProcess.hh"
#include "G4KaonZeroLInelasticProcess.hh"
#include "G4KaonMinusInelasticProcess.hh"
#include "G4ProtonInelasticProcess.hh"
#include "G4AntiProtonInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4AntiNeutronInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"

// Low-energy Models: < 20GeV
#include "G4LElastic.hh"
#include "G4LEPionPlusInelastic.hh"
#include "G4LEPionMinusInelastic.hh"
#include "G4LEKaonPlusInelastic.hh"
#include "G4LEKaonZeroSInelastic.hh"
#include "G4LEKaonZeroLInelastic.hh"
#include "G4LEKaonMinusInelastic.hh"
#include "G4LEProtonInelastic.hh"
#include "G4LEAntiProtonInelastic.hh"
#include "G4LENeutronInelastic.hh"
#include "G4LEAntiNeutronInelastic.hh"
#include "G4LEDeuteronInelastic.hh"
#include "G4LETritonInelastic.hh"
#include "G4LEAlphaInelastic.hh"
#include "G4HadronCaptureProcess.hh"
// High-energy Models: >20 GeV
#include "G4HEPionPlusInelastic.hh"
#include "G4HEPionMinusInelastic.hh"
#include "G4HEKaonPlusInelastic.hh"
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonMinusInelastic.hh"
#include "G4HEProtonInelastic.hh"
#include "G4HEAntiProtonInelastic.hh"
#include "G4HENeutronInelastic.hh"
#include "G4HEAntiNeutronInelastic.hh"

// Neutron high-precision models: <20 MeV
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPInelasticData.hh"
#include "G4LCapture.hh"


// neutron-induced fission (new implementation)
#include "G4HadronFissionProcess.hh"
#include "G4FissionLibrary.hh"
#include "G4FissLib.hh"


// Stopping processes
#include "G4PiMinusAbsorptionAtRest.hh"
#include "G4KaonMinusAbsorptionAtRest.hh"
#include "G4AntiProtonAnnihilationAtRest.hh"
#include "G4AntiNeutronAnnihilationAtRest.hh"

#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysics.hh"
//
// NOT AVAILABLE IN THIS GEANT4 VERSION
//
//#include "G4QStoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4NeutronTrackingCut.hh"
//
// NOT AVAILABLE IN THIS GEANT4 VERSION
//
//#include "HadronPhysicsQGSP_BERT_HP.hh"
#include "HadronPhysicsQGSP_BIC_HP.hh"
//
// NOT AVAILABLE IN THIS GEANT4 VERSION
//
//#include "HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronInelasticQBBC.hh"

// ConstructHad()
// Makes discrete physics processes for the hadrons, at present limited
// to those particles with GHEISHA interactions (INTRC > 0).
// The processes are: Elastic scattering and Inelastic scattering.
// F.W.Jones  09-JUL-1998
void ExN02PhysicsList::ConstructHad(std::string hadlist) 
{
  if(0){}

  //
  // NOT AVAILABLE IN THIS GEANT4 VERSION
  //
  // if(hadlist.find("FTFP_BERT_HP") != std::string::npos){
  //    G4EmExtraPhysics* EmExtraPhysics = new G4EmExtraPhysics("extra EM");
  //    G4HadronElasticPhysics*  HadronElasticPhysics = new G4HadronElasticPhysics("elastic",VerboseLevel ,true);
  //    //G4QStoppingPhysics* QStoppingPhysics =  new  G4QStoppingPhysics("stopping",VerboseLevel);
  //    G4IonBinaryCascadePhysics* IonBinaryCascadePhysics = new G4IonBinaryCascadePhysics("ion");
  //    G4NeutronTrackingCut* NeutronTrackingCut = new G4NeutronTrackingCut("nTackingCut",VerboseLevel);
  //    HadronPhysicsFTFP_BERT_HP* HadronPhysicsBert = new HadronPhysicsFTFP_BERT_HP("hadron",true);
  
  //    EmExtraPhysics->ConstructProcess();
  //    HadronElasticPhysics->ConstructProcess();
  //    //QStoppingPhysics->ConstructProcess();
  //    IonBinaryCascadePhysics->ConstructProcess();
  //    NeutronTrackingCut->ConstructProcess();
  //    HadronPhysicsBert->ConstructProcess();
  
  //    EmExtraPhysics->SetVerboseLevel(VerboseLevel);
  //    HadronElasticPhysics->SetVerboseLevel(VerboseLevel);
  //    IonBinaryCascadePhysics->SetVerboseLevel(VerboseLevel);
  //    NeutronTrackingCut->SetVerboseLevel(VerboseLevel);
  //    HadronPhysicsBert->SetVerboseLevel(VerboseLevel);
  //  }

  //
  // NOT AVAILABLE IN THIS GEANT4 VERSION
  //
  // else if(hadlist.find("QGSP_BERT_HP") != std::string::npos){
  //   G4EmExtraPhysics* EmExtraPhysics = new G4EmExtraPhysics("extra EM");
  //   G4HadronElasticPhysics*  HadronElasticPhysics = new G4HadronElasticPhysics("elastic",VerboseLevel ,true);
  //   //G4QStoppingPhysics* QStoppingPhysics =  new  G4QStoppingPhysics("stopping",VerboseLevel);
  //   G4IonBinaryCascadePhysics* IonBinaryCascadePhysics = new G4IonBinaryCascadePhysics("ion");
  //   G4NeutronTrackingCut* NeutronTrackingCut = new G4NeutronTrackingCut("nTackingCut",VerboseLevel);
  //   HadronPhysicsQGSP_BERT_HP* HadronPhysicsBert = new HadronPhysicsQGSP_BERT_HP("hadron",true);

  //   EmExtraPhysics->ConstructProcess();
  //   HadronElasticPhysics->ConstructProcess();
  //   //QStoppingPhysics->ConstructProcess();
  //   IonBinaryCascadePhysics->ConstructProcess();
  //   NeutronTrackingCut->ConstructProcess();
  //   HadronPhysicsBert->ConstructProcess();

  //   EmExtraPhysics->SetVerboseLevel(VerboseLevel);
  //   HadronElasticPhysics->SetVerboseLevel(VerboseLevel);
  //   IonBinaryCascadePhysics->SetVerboseLevel(VerboseLevel);
  //   NeutronTrackingCut->SetVerboseLevel(VerboseLevel);
  //   HadronPhysicsBert->SetVerboseLevel(VerboseLevel);
  // }
  else if(hadlist.find("QGSP_BIC_HP") != std::string::npos){
    G4EmExtraPhysics* EmExtraPhysics = new G4EmExtraPhysics("extra EM");
    G4HadronElasticPhysics*  HadronElasticPhysics = new G4HadronElasticPhysics("elastic", VerboseLevel,true);
    //G4QStoppingPhysics* QStoppingPhysics =  new  G4QStoppingPhysics("stopping",VerboseLevel);
    G4IonBinaryCascadePhysics* IonBinaryCascadePhysics = new G4IonBinaryCascadePhysics("ionBIC");
    G4NeutronTrackingCut* NeutronTrackingCut = new G4NeutronTrackingCut("nTackingCut",VerboseLevel);
    HadronPhysicsQGSP_BIC_HP*  HadronPhysicsBic = new HadronPhysicsQGSP_BIC_HP("hadron",true);

    EmExtraPhysics->ConstructProcess();
    HadronElasticPhysics->ConstructProcess();
    //QStoppingPhysics->ConstructProcess();
    IonBinaryCascadePhysics->ConstructProcess();
    NeutronTrackingCut->ConstructProcess();
    HadronPhysicsBic->ConstructProcess();

    EmExtraPhysics->SetVerboseLevel(VerboseLevel);
    HadronElasticPhysics->SetVerboseLevel(VerboseLevel);
    IonBinaryCascadePhysics->SetVerboseLevel(VerboseLevel);
    NeutronTrackingCut->SetVerboseLevel(VerboseLevel);
    HadronPhysicsBic->SetVerboseLevel(VerboseLevel);
  }
  else if(hadlist.find("QBBC_HP") != std::string::npos){
 
    G4EmExtraPhysics* EmExtraPhysics = new G4EmExtraPhysics("extra EM");
    G4HadronElasticPhysics*  HadronElasticPhysics = new G4HadronElasticPhysics("elastic", VerboseLevel,true);
    //G4QStoppingPhysics* QStoppingPhysics =  new  G4QStoppingPhysics("stopping",VerboseLevel);
    G4IonBinaryCascadePhysics* IonBinaryCascadePhysics = new G4IonBinaryCascadePhysics("ionBIC");
    G4NeutronTrackingCut* NeutronTrackingCut = new G4NeutronTrackingCut("nTackingCut",VerboseLevel);
    G4HadronInelasticQBBC* HadronInelasticQBBC = new G4HadronInelasticQBBC("QBBC",VerboseLevel,false,false,false,true,true);

    EmExtraPhysics->ConstructProcess();
    
    G4cout << "CIAO 1" << G4endl;
    
    HadronElasticPhysics->ConstructProcess();
    
    G4cout << "CIAO 2" << G4endl;

    //QStoppingPhysics->ConstructProcess();
    
    G4cout << "CIAO 3" << G4endl;

    IonBinaryCascadePhysics->ConstructProcess();
    
    G4cout << "CIAO 4" << G4endl;

    NeutronTrackingCut->ConstructProcess();
    
    G4cout << "CIAO 5" << G4endl;
 
    HadronInelasticQBBC->ConstructProcess();

    G4cout << "CIAO 6" << G4endl;
 
    EmExtraPhysics->SetVerboseLevel(VerboseLevel);
    HadronElasticPhysics->SetVerboseLevel(VerboseLevel);
    IonBinaryCascadePhysics->SetVerboseLevel(VerboseLevel);
    NeutronTrackingCut->SetVerboseLevel(VerboseLevel);
    HadronInelasticQBBC->SetVerboseLevel(VerboseLevel);
  }

  //
  // NOT AVAILABLE IN THIS GEANT4 VERSION
  //
  // else if(hadlist.find("FTFP_BERT_HP") != std::string::npos){
  //   G4EmExtraPhysics* EmExtraPhysics = new G4EmExtraPhysics("extra EM");
  //   G4HadronElasticPhysics*  HadronElasticPhysics = new G4HadronElasticPhysics("elastic", VerboseLevel,true);
  //   //G4QStoppingPhysics* QStoppingPhysics =  new  G4QStoppingPhysics("stopping",VerboseLevel);
  //   G4IonBinaryCascadePhysics* IonBinaryCascadePhysics = new G4IonBinaryCascadePhysics("ionBIC");
  //   G4NeutronTrackingCut* NeutronTrackingCut = new G4NeutronTrackingCut("nTackingCut",VerboseLevel);
  //   HadronPhysicsFTFP_BERT_HP*  HadronPhysicsFtfp = new HadronPhysicsFTFP_BERT_HP("hadron",true);

  //   EmExtraPhysics->ConstructProcess();
  //   HadronElasticPhysics->ConstructProcess();
  //   //QStoppingPhysics->ConstructProcess();
  //   IonBinaryCascadePhysics->ConstructProcess();
  //   NeutronTrackingCut->ConstructProcess();
  //   HadronPhysicsFtfp->ConstructProcess();

  //   EmExtraPhysics->SetVerboseLevel(VerboseLevel);
  //   HadronElasticPhysics->SetVerboseLevel(VerboseLevel);
  //   IonBinaryCascadePhysics->SetVerboseLevel(VerboseLevel);
  //   NeutronTrackingCut->SetVerboseLevel(VerboseLevel);
  //   HadronPhysicsFtfp->SetVerboseLevel(VerboseLevel);

  // }
  else{
    G4HadronElasticProcess* theElasticProcess = new G4HadronElasticProcess;
    G4LElastic* theElasticModel = new G4LElastic;
    theElasticProcess->RegisterMe(theElasticModel);
    theElasticProcess->SetVerboseLevel(VerboseLevel);
    
    theParticleIterator->reset();
    while ((*theParticleIterator)()) 
      {
	G4ParticleDefinition* particle = theParticleIterator->value();
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4String particleName = particle->GetParticleName();
	
	if(pmanager == NULL)continue;
        pmanager->SetVerboseLevel(VerboseLevel);

	if (particleName == "pi+") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4PionPlusInelasticProcess* theInelasticProcess = 
	      new G4PionPlusInelasticProcess("inelastic");
	    G4LEPionPlusInelastic* theLEInelasticModel = 
	      new G4LEPionPlusInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEPionPlusInelastic* theHEInelasticModel = 
	      new G4HEPionPlusInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  } 
	
	else if (particleName == "pi-") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4PionMinusInelasticProcess* theInelasticProcess = 
	      new G4PionMinusInelasticProcess("inelastic");
	    G4LEPionMinusInelastic* theLEInelasticModel = 
	      new G4LEPionMinusInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEPionMinusInelastic* theHEInelasticModel = 
	      new G4HEPionMinusInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	    G4String prcNam;
	    pmanager->AddRestProcess(new G4PiMinusAbsorptionAtRest, ordDefault);
	  }
	
	else if (particleName == "kaon+") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4KaonPlusInelasticProcess* theInelasticProcess = 
	      new G4KaonPlusInelasticProcess("inelastic");
	    G4LEKaonPlusInelastic* theLEInelasticModel = 
	      new G4LEKaonPlusInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEKaonPlusInelastic* theHEInelasticModel = 
	      new G4HEKaonPlusInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "kaon0S") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4KaonZeroSInelasticProcess* theInelasticProcess = 
	      new G4KaonZeroSInelasticProcess("inelastic");
	    G4LEKaonZeroSInelastic* theLEInelasticModel = 
	      new G4LEKaonZeroSInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEKaonZeroInelastic* theHEInelasticModel = 
	      new G4HEKaonZeroInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "kaon0L") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4KaonZeroLInelasticProcess* theInelasticProcess = 
	      new G4KaonZeroLInelasticProcess("inelastic");
	    G4LEKaonZeroLInelastic* theLEInelasticModel = 
	      new G4LEKaonZeroLInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEKaonZeroInelastic* theHEInelasticModel = 
	      new G4HEKaonZeroInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "kaon-") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4KaonMinusInelasticProcess* theInelasticProcess = 
	      new G4KaonMinusInelasticProcess("inelastic");
	    G4LEKaonMinusInelastic* theLEInelasticModel = 
	      new G4LEKaonMinusInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEKaonMinusInelastic* theHEInelasticModel = 
	      new G4HEKaonMinusInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	    pmanager->AddRestProcess(new G4KaonMinusAbsorptionAtRest, ordDefault);
	  }
	
	else if (particleName == "proton") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4ProtonInelasticProcess* theInelasticProcess = 
	      new G4ProtonInelasticProcess("inelastic");
	    G4LEProtonInelastic* theLEInelasticModel = new G4LEProtonInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEProtonInelastic* theHEInelasticModel = new G4HEProtonInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "anti_proton") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4AntiProtonInelasticProcess* theInelasticProcess = 
	      new G4AntiProtonInelasticProcess("inelastic");
	    G4LEAntiProtonInelastic* theLEInelasticModel = 
	      new G4LEAntiProtonInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEAntiProtonInelastic* theHEInelasticModel = 
	      new G4HEAntiProtonInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "neutron") {
	  // elastic scattering
	  G4HadronElasticProcess* theNeutronElasticProcess = 
	    new G4HadronElasticProcess;
	  G4LElastic* theElasticModel1 = new G4LElastic;
	  G4NeutronHPElastic * theElasticNeutron = new G4NeutronHPElastic;
	  theNeutronElasticProcess->RegisterMe(theElasticModel1);
	  theElasticModel1->SetMinEnergy(19*MeV);
	  theNeutronElasticProcess->RegisterMe(theElasticNeutron);
	  G4NeutronHPElasticData * theNeutronData = new G4NeutronHPElasticData;
	  theNeutronElasticProcess->AddDataSet(theNeutronData);
	  pmanager->AddDiscreteProcess(theNeutronElasticProcess);
	  // inelastic scattering
	  G4NeutronInelasticProcess* theInelasticProcess =
	    new G4NeutronInelasticProcess("inelastic");
	  G4LENeutronInelastic* theInelasticModel = new G4LENeutronInelastic;
	  theInelasticModel->SetMinEnergy(19*MeV);
	  theInelasticProcess->RegisterMe(theInelasticModel);
	  G4NeutronHPInelastic * theLENeutronInelasticModel =
	    new G4NeutronHPInelastic;
	  theInelasticProcess->RegisterMe(theLENeutronInelasticModel);
	  G4NeutronHPInelasticData * theNeutronData1 = 
	    new G4NeutronHPInelasticData;
	  theInelasticProcess->AddDataSet(theNeutronData1);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	  // capture
	  G4HadronCaptureProcess* theCaptureProcess =
	    new G4HadronCaptureProcess;
	  G4LCapture* theCaptureModel = new G4LCapture;
	  theCaptureModel->SetMinEnergy(19*MeV);
	  theCaptureProcess->RegisterMe(theCaptureModel);
	  G4NeutronHPCapture * theLENeutronCaptureModel = new G4NeutronHPCapture;
	  theCaptureProcess->RegisterMe(theLENeutronCaptureModel);
	  G4NeutronHPCaptureData * theNeutronData3 = new G4NeutronHPCaptureData;
	  theCaptureProcess->AddDataSet(theNeutronData3);
	  pmanager->AddDiscreteProcess(theCaptureProcess);
	  
	  
	  // Fission library model
	  G4HadronFissionProcess *theFissionProcess = new G4HadronFissionProcess("lll_fission");
	  G4FissLib* theFissionModel = new G4FissLib;
	  theFissionProcess->RegisterMe(theFissionModel);
	  pmanager->AddDiscreteProcess(theFissionProcess);
	  
	  
	  
	  //  G4ProcessManager* pmanager = G4Neutron::Neutron->GetProcessManager();
	  //  pmanager->AddProcess(new G4UserSpecialCuts(),-1,-1,1);
      }
	else if (particleName == "anti_neutron") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4AntiNeutronInelasticProcess* theInelasticProcess = 
	      new G4AntiNeutronInelasticProcess("inelastic");
	    G4LEAntiNeutronInelastic* theLEInelasticModel = 
	      new G4LEAntiNeutronInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEAntiNeutronInelastic* theHEInelasticModel = 
	      new G4HEAntiNeutronInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "deuteron") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4DeuteronInelasticProcess* theInelasticProcess = 
	      new G4DeuteronInelasticProcess("inelastic");
	    G4LEDeuteronInelastic* theLEInelasticModel = 
	      new G4LEDeuteronInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "triton") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4TritonInelasticProcess* theInelasticProcess = 
	      new G4TritonInelasticProcess("inelastic");
	    G4LETritonInelastic* theLEInelasticModel = 
	      new G4LETritonInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "alpha") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4AlphaInelasticProcess* theInelasticProcess = 
	      new G4AlphaInelasticProcess("inelastic");
	    G4LEAlphaInelastic* theLEInelasticModel = 
	      new G4LEAlphaInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
      }	
  }
  G4cout << "FINE" << G4endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Decay.hh"

void ExN02PhysicsList::ConstructGeneral()
{
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) { 
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}
  
bool ExN02PhysicsList::IsDefined(G4int p)
{
    theParticleIterator->reset();
    while( (*theParticleIterator)() ){
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4int pid = particle->GetPDGEncoding();
        if(p==pid) return true;
    }
    return false;
}

#include "G4ParallelWorldScoringProcess.hh"
void ExN02PhysicsList::AddVirtualScoringProcess() {
    // Add parallel world scoring process
    //
  G4ParallelWorldScoringProcess* theParallelWorldScoringProcess
      = new G4ParallelWorldScoringProcess("ParaWorldScoringProc");
  theParallelWorldScoringProcess->SetParallelWorld("ParallelScoringWorld");
  theParallelWorldScoringProcess->SetVerboseLevel(VerboseLevel);

  theParticleIterator->reset();
  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    if (!particle->IsShortLived() && particle->GetParticleName() != "gamma")
    {
      G4ProcessManager* pmanager = particle->GetProcessManager();
      if(pmanager != NULL){
	pmanager->SetVerboseLevel(VerboseLevel);
      	pmanager->AddProcess(theParallelWorldScoringProcess);
      	pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcess, idxAtRest);
      	pmanager->SetProcessOrdering(theParallelWorldScoringProcess, idxAlongStep, 1);
      	pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcess, idxPostStep);
      }
    }
  }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"

void ExN02PhysicsList::AddStepMax()
{
  // Step limitation seen as a process
  G4StepLimiter* stepLimiter = new G4StepLimiter();
  ////G4UserSpecialCuts* userCuts = new G4UserSpecialCuts();
  
  theParticleIterator->reset();
  while ((*theParticleIterator)()){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (particle->GetPDGCharge() != 0.0)
        {
	  pmanager ->AddDiscreteProcess(stepLimiter);
	  ////pmanager ->AddDiscreteProcess(userCuts);
        }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Cuts /////////////////////////////////////////////////////////////////////
// void PhysicsList::SetCuts() 
// {
  
//   if (VerboseLevel >1)
//     G4cout << "PhysicsList::SetCuts:";
  
//   if (VerboseLevel>0){
//     G4cout << "PhysicsList::SetCuts:";
//     G4cout << "CutLength : " 
// 	   << G4BestUnit(defaultCut_,"Length") << G4endl;
//   }

//   //special for low energy physics
//   G4double lowlimit=250*eV;  
//   G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowlimit,100.*GeV);

//   // set cut values for gamma at first and for e- second and next for e+,
//   // because some processes for e+/e- need cut values for gamma 
//   SetCutValue(cutForGamma, "gamma");
//   SetCutValue(cutForElectron, "e-");
//   SetCutValue(cutForPositron, "e+");
//   //hadronic also
//   SetCutValue(cutForNeutron,"neutron");
//   SetCutValue(defaultCutValue,"proton");
  
//   if (VerboseLevel>0) DumpCutValuesTable();
// }

void ExN02PhysicsList::SetCuts()
{
  //G4VUserPhysicsList::SetCutsWithDefault method sets 
  //the default cut value for all particle types 
  //
  SetCutsWithDefault();
     
  if (verboseLevel>0) DumpCutValuesTable();
}

//
// NOT AVAILABLE IN THIS GEANT4 VERSION
//
// // Cards /////////////////////////////////////////////////////////////////////
// void ExN02PhysicsList::loadDataCards() {
//   cards_ = DataCards::getInstance();
//   defaultCut_       = cards_->fetchValueDouble("physicsDefaultCut");
//   gammaCut_         = cards_->fetchValueDouble("gammaCut");
//   electronCut_      = cards_->fetchValueDouble("electronCut");
//   positronCut_      = cards_->fetchValueDouble("positronCut");
//   neutronCut_       = cards_->fetchValueDouble("neutronCut");
//   cutEM_            = cards_->fetchValueDouble("cutEM");
//   //decayList_        = cards_->fetchValueString("decayList");
//   cardsLoaded_      = true;
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

