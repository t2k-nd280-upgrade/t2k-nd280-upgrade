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
// $Id: B4aActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file B4aActionInitialization.cc
/// \brief Implementation of the B4aActionInitialization class

#include "ExN02ActionInitialization.hh"

//#include "ExN02PrimaryGeneratorAction_DetDependency.hh"
#include "ExN02PrimaryGeneratorAction.hh"

#include "ExN02RunAction.hh"
#include "ExN02EventAction.hh"
#include "ExN02SteppingAction.hh"
#include "ExN02DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02ActionInitialization::ExN02ActionInitialization
                            (ExN02DetectorConstruction* detConstruction)
 : G4VUserActionInitialization(),
   fDetConstruction(detConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02ActionInitialization::~ExN02ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02ActionInitialization::BuildForMaster() const
{
  //SetUserAction(new ExN02RunAction);

  ExN02EventAction* eventAction = 0;
  SetUserAction(new ExN02RunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02ActionInitialization::Build() const
{
  //SetUserAction(new ExN02PrimaryGeneratorAction); 
  //SetUserAction(new ExN02RunAction);
  //ExN02EventAction* eventAction = new ExN02EventAction;
  //SetUserAction(eventAction);

  SetUserAction(new ExN02PrimaryGeneratorAction);
  ExN02EventAction* eventAction = new ExN02EventAction;
  SetUserAction(eventAction);
  SetUserAction(new ExN02RunAction(eventAction));
  

  //SetUserAction(new TrackingAction(fDetector));

  SetUserAction(new ExN02SteppingAction(fDetConstruction,eventAction));
}
  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
