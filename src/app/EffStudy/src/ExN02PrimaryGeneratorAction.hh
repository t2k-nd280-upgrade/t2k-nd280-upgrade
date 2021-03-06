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
// $Id: B4PrimaryGeneratorAction.hh 68058 2013-03-13 14:47:43Z gcosmo $
// 
/// \file B4PrimaryGeneratorAction.hh
/// \brief Definition of the B4PrimaryGeneratorAction class

#ifndef ExN02PrimaryGeneratorAction_h
#define ExN02PrimaryGeneratorAction_h 1

#include "ExN02RooTrackerKinematicsGenerator.hh"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class ExN02PrimaryGeneratorMessenger;

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the calorimeter 
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class 
/// (see the macros provided with this example).

class ExN02PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  ExN02PrimaryGeneratorAction();    
  virtual ~ExN02PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* event);
  
  // set methods
  void SetRandomFlag(G4bool value);
  void SetGeneratorType(G4String name){fGeneratorType=name;};
  void SetTypeDirection(G4String name){fTypeDirection=name;};
  void SetTypeMomentum(G4String name){fTypeMomentum=name;};
  void SetTypeGun(G4String name){fTypeGun=name;};
  void SetTypePosition(G4String name){fTypePosition=name;};

private:
  G4ParticleGun*  fParticleGun; // G4 particle gun

  ExN02RooTrackerKinematicsGenerator RooTrackerNEUT; // NEUT input tree

  // Variables of the messenger
  ExN02PrimaryGeneratorMessenger *fPrimaryGeneratorMessenger;
  G4String fGeneratorType;
  G4String fTypeDirection;
  G4String fTypeMomentum;
  G4String fTypeGun;
  G4String fTypePosition;

  ExN02ND280XML *inxml;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


