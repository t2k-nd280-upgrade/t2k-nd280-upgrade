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
// $Id: ExN02PhysicsList.hh,v 1.12 2008-09-22 16:41:20 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN02PhysicsList_h
#define ExN02PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include "G4LossTableManager.hh"

//#include "StepMax.hh"

#ifdef USE_PAI
class G4EmConfigurator;
class G4ProductionCuts;
#endif

class G4VPhysicsConstructor;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02PhysicsList: public G4VModularPhysicsList
{
public:
  ExN02PhysicsList();
  ~ExN02PhysicsList();
  
  virtual void ConstructParticle();

  void AddPhysicsList(const G4String& name);
  virtual void ConstructProcess();
    
  //void AddStepMax();       
  //StepMax* GetStepMaxProcess() {return fStepMaxProcess;};

  private:

  // void AddIonGasModels();

  // G4bool   fHelIsRegisted;
  // G4bool   fBicIsRegisted;
  // G4bool   fBiciIsRegisted;
    
  G4String                             fEmName;
  G4VPhysicsConstructor*               fEmPhysicsList;
  G4VPhysicsConstructor*               fDecPhysicsList;
  std::vector<G4VPhysicsConstructor*>  fHadronPhys;    
  //StepMax*                             fStepMaxProcess;

  /// Make sure that the EM physics list is empty.
  void ClearEMPhysics();
  
  /// Make sure that the hadron physics list is empty.
  void ClearHadronPhysics();
    
  //PhysicsListMessenger*  fMessenger;

  void SetBuilderList0(G4bool flagHP = false);
  void SetBuilderList1(G4bool flagHP = false);
  void SetBuilderList2(G4bool flagHP = false);
  void SetBuilderList3(G4bool flagHP = false);
  void SetBuilderList4(G4bool flagHP = false);
  void SetBuilderList5(G4bool flagHP = false);
  void SetBuilderList6(G4bool flagHP = false);

#ifdef USE_PAI
    void AddPAIModel();

    G4EmConfigurator* em_config;
#endif

  protected:
  // these methods Construct physics processes and register them

};


#endif

 
