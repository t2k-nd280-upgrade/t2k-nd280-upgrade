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

#include "G4VUserPhysicsList.hh"
#include "globals.hh"
#include "G4ParallelWorldScoringProcess.hh"

//
// NOT AVAILABLE IN THIS GEANT4 VERSION
//
//#include "DataCards.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02PhysicsList: public G4VUserPhysicsList
{
  public:
    ExN02PhysicsList();
   ~ExN02PhysicsList();

  bool IsDefined(G4int pid);
  
  virtual void SetCuts();
  void loadDataCards();

  protected:
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess();
 
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
    void ConstructIons();
    void ConstructShortLived();

    void ConstructEM();
    void ConstructEM(std::string emlist);
    void ConstructHad(std::string hadlist);

    void AddTransportation();
    void AddVirtualScoringProcess();
  
  protected:
  // these methods Construct physics processes and register them
    void ConstructGeneral();
    void AddStepMax();

    G4int VerboseLevel;
    G4int OpVerbLevel;
  
    //
    // NOT AVAILABLE IN THIS GEANT4 VERSION
    //
    //DataCards*			cards_;
    //G4bool      		cardsLoaded_;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

 
