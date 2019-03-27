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
// $Id: ExN02RunAction.cc,v 1.9 2006-06-29 17:48:16 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#include <ctime>
#include <sys/time.h>


#include "ExN02RunAction.hh"
#include "ExN02EventAction.hh"
#include "ExN02Analysis.hh" // TODO

#include "ND280PersistencyManager.hh"
#include "ND280RootPersistencyManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

//#include "G4SystemOfUnits.hh" // NEW GLOBAL
#include <CLHEP/Units/SystemOfUnits.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//ExN02RunAction::ExN02RunAction()
//  : G4UserRunAction()
ExN02RunAction::ExN02RunAction(ExN02EventAction *eventAction)
  : G4UserRunAction(),
    fEventAction(eventAction)
{
  //set the seed engine
  //CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine); // NEW SEED

  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  // Create analysis manager                                        
  // The choice of analysis technology is done via selectin of a namespace       
  // in B4Analysis.hh                                                              
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);

  // Book histograms, ntuple
  //

  // // Creating histograms 
  //analysisManager->CreateH1("Edep_abs_tpcup","Edep in TPC Up", 100, 0., 800*CLHEP::MeV);
#ifdef NEUTRON_FILE
  analysisManager->CreateH1("np", "np", 140, 0., 700.); // 1
  analysisManager->CreateH1("nC", "nC", 140, 0., 700.); // 2
  analysisManager->CreateH1("nInelastic", "nInelastic", 140, 0., 700.); // 3
  analysisManager->CreateH1("B", "B", 140, 0., 700.); // 4
  analysisManager->CreateH1("Li", "Li", 140, 0., 700.); // 5
  analysisManager->CreateH1("3alpha", "3alpha", 140, 0., 700.); // 6
  analysisManager->CreateH1("Be", "Be", 140, 0., 700.); // 7

  analysisManager->CreateH1("0p0aE", "0p0aE", 140, 0., 700.); // 8
  analysisManager->CreateH1("0p0aIE", "0p0aIE", 140, 0., 700.); // 9
  analysisManager->CreateH1("0p1a", "0p1a", 140, 0., 700.); // 10
  analysisManager->CreateH1("0pNa", "0pNa", 140, 0., 700.); // 11
  analysisManager->CreateH1("1p0a", "1p0a", 140, 0., 700.); // 12
  analysisManager->CreateH1("1p1a", "1p1a", 140, 0., 700.); // 13
  analysisManager->CreateH1("1pNa", "1pNa", 140, 0., 700.); // 14
  analysisManager->CreateH1("NpNa", "NpNa", 140, 0., 700.); // 15

  analysisManager->CreateH1("XsecNorm", "XsecNorm", 140, 0., 700.); // 16
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02RunAction::~ExN02RunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  // Create ROOT the persistency manager.                                                                                                                                                            
  ND280RootPersistencyManager* persistencyManager
    = ND280RootPersistencyManager::GetInstance();
  ExN02ND280XML *ND280XMLInput = persistencyManager->GetXMLInput();
  bool IsRandomSeed = ND280XMLInput->GetXMLIsRandomSeed();

  // set the Seed
  if(IsRandomSeed) SetSeed();




  //inform the runManager to save random number seed  
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);      
  // // Store the seed
  //G4RunManager::GetRunManager()->rndmSaveThisEvent();



#ifdef NEUTRON_FILE  
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  // Open an output file     
  //
  G4String fileName = "EffStudy_" + G4UIcommand::ConvertToString(aRun->GetRunID());
  analysisManager->OpenFile(fileName);
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::EndOfRunAction(const G4Run*) {
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
#ifdef NEUTRON_FILE  
  analysisManager->Write();
  analysisManager->CloseFile();
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::SetSeed() {

  //set random seed with system time
  
  //G4long seed = time(NULL); 
  //if (seed<0) seed = -seed;
  //G4cout << "### Random seed number set to: " << seed<< G4endl;
  //CLHEP::HepRandom::setTheSeed(seed);
  
  // As used in nd280mc
  long seed = 0.;
  struct timeval buffer;
  gettimeofday(&buffer,NULL);
  //ND280Log("### Set seed from local time: " << ctime(&buffer.tv_sec));
  //ND280Log("###      Micro-second offset: " << buffer.tv_usec);
  seed = long(buffer.tv_sec + buffer.tv_usec);
  SetSeed(long(seed));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

void ExN02RunAction::SetSeed(long seed) {
  if (seed<0) seed = -seed;
  G4cout << "### Random seed number set to: "<< seed << G4endl;
  CLHEP::HepRandom::setTheSeed(seed);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

long ExN02RunAction::GetSeed(void) const {
  return CLHEP::HepRandom::getTheSeed();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
