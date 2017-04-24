////////////////////////////////////////////////////////////
// $Id: ND280InputPersistencyManager.cc,v 1.93 2012/03/23 14:09:24 mcgrew Exp $
//
 
#include "ND280InputPersistencyManager.hh"

#include <memory>
#include <cmath>
#include <algorithm>

#include <globals.hh>

#include <G4Run.hh>
#include <G4RunManager.hh>

#include "ExN02Constants.hh"

#include <TROOT.h>
#include <TFile.h>

ND280InputPersistencyManager::ND280InputPersistencyManager() 
  : ND280PersistencyManager(),fND280XMLInput(),
    fEventFirst(-99999),fNEvents(-99999) {}

ND280InputPersistencyManager* ND280InputPersistencyManager::GetInstance() {
  ND280InputPersistencyManager *current 
    = dynamic_cast<ND280InputPersistencyManager*>(
    			 G4VPersistencyManager::GetPersistencyManager());
  if (!current) current = new ND280InputPersistencyManager();
  return current;
}

ND280InputPersistencyManager::~ND280InputPersistencyManager() {
  delete fND280XMLInput; fND280XMLInput = NULL;
}

bool ND280InputPersistencyManager::Open(G4String filename) {
  fND280XMLInput = new ExN02ND280XML(filename); 
  return true;
}


bool ND280InputPersistencyManager::Close() {
  if (!fND280XMLInput) {
    G4ExceptionDescription msg;
    msg << "No Output File" << G4endl; 
    G4Exception("ND280InputPersistencyManager::Close",
		"ExN02Code001",FatalException, msg);
    return false;
  }
    
  delete fND280XMLInput; fND280XMLInput=NULL;

  return true;
}
