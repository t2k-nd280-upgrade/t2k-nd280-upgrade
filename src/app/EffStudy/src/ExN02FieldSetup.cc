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
/// \file field/field01/src/F01FieldSetup.cc
/// \brief Implementation of the F01FieldSetup class
//
//
// $Id: F01FieldSetup.cc 77115 2013-11-21 15:06:37Z gcosmo $
//
//   User Field setup class implementation.
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN02FieldSetup.hh"
#include "ExN02FieldMessenger.hh"
#include "ExN02Constants.hh"

#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"

#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"

#include "G4PhysicalConstants.hh"


//#include "G4SystemOfUnits.hh" // NEW GLOBAL
#include <CLHEP/Units/SystemOfUnits.h>



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//  Constructors:

ExN02FieldSetup::ExN02FieldSetup(G4ThreeVector fieldVector)
 : fFieldManager(0),
   fChordFinder(0),
   fEquation(0),
   fMagneticField(new G4UniformMagField(fieldVector)),
   fStepper(0),
   fStepperType(0),
   fMinStep(0.),
   fFieldMessenger(0)
{
  G4cout << "[ExN02FieldSetup] Magnetic field set to Uniform( "
         << fieldVector / CLHEP::tesla << " ) T" << G4endl;
  InitialiseAll();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02FieldSetup::ExN02FieldSetup()
 : fFieldManager(0),
   fChordFinder(0),
   fEquation(0),
   fMagneticField(new G4UniformMagField(G4ThreeVector())),
   fStepper(0),
   fStepperType(0),
   fMinStep(0.),
   fFieldMessenger(0)
{
  G4cout << "[ExN02FieldSetup] Magnetic field set to Uniform( 0.0, 0, 0 ) "
         << G4endl;
  InitialiseAll();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02FieldSetup::InitialiseAll()
{
  fFieldMessenger = new ExN02FieldMessenger(this);
 
  fEquation = new G4Mag_UsualEqRhs(fMagneticField);
 
  fMinStep     = cMinStep;
  fStepperType = cStepperType;     

  fFieldManager = G4TransportationManager::GetTransportationManager()
                    ->GetFieldManager();
  CreateStepperAndChordFinder();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02FieldSetup::~ExN02FieldSetup()
{
  delete fMagneticField;
  delete fChordFinder;
  delete fStepper;
  delete fFieldMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02FieldSetup::CreateStepperAndChordFinder()
{
  // Update field

  SetStepper();
  G4cout<<"[ExN02FieldSetup] The minimal step is equal to "<<fMinStep/CLHEP::mm<<" mm"<<G4endl;
  
  fFieldManager->SetDetectorField(fMagneticField );

  if (fChordFinder) delete fChordFinder;

  fChordFinder = new G4ChordFinder( fMagneticField, fMinStep,fStepper );

  fFieldManager->SetChordFinder( fChordFinder );
  
  G4double fieldValue[6],  position[4];
  position[0] = position[1] = position[2] = position[3] = 0.0;
  this->GetGlobalFieldManager()->GetDetectorField()->GetFieldValue( position, fieldValue);
  G4ThreeVector fieldVec(fieldValue[0], fieldValue[1], fieldValue[2]);
  G4cout << "[ExN02FieldSetup] Magnetic field vector is "
	 << fieldVec / CLHEP::tesla << " T " << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02FieldSetup::SetStepper()
{
  // Set stepper according to the stepper type

  if (fStepper) delete fStepper;

  switch ( fStepperType )
  {
    case 0:
      fStepper = new G4ExplicitEuler( fEquation );
      G4cout<<"[ExN02FieldSetup] G4ExplicitEuler is calledS"<<G4endl;
      break;
    case 1:
      fStepper = new G4ImplicitEuler( fEquation );
      G4cout<<"[ExN02FieldSetup] G4ImplicitEuler is called"<<G4endl;
      break;
    case 2:
      fStepper = new G4SimpleRunge( fEquation );
      G4cout<<"[ExN02FieldSetup] G4SimpleRunge is called"<<G4endl;
      break;
    case 3:
      fStepper = new G4SimpleHeum( fEquation );
      G4cout<<"[ExN02FieldSetup] G4SimpleHeum is called"<<G4endl;
      break;
    case 4:
      fStepper = new G4ClassicalRK4( fEquation );
      G4cout<<"[ExN02FieldSetup] G4ClassicalRK4 (default) is called"<<G4endl;
      break;
    case 5:
      fStepper = new G4HelixExplicitEuler( fEquation );
      G4cout<<"[ExN02FieldSetup] G4HelixExplicitEuler is called"<<G4endl;
      break;
    case 6:
      fStepper = new G4HelixImplicitEuler( fEquation );
      G4cout<<"[ExN02FieldSetup] G4HelixImplicitEuler is called"<<G4endl;
      break;
    case 7:
      fStepper = new G4HelixSimpleRunge( fEquation );
      G4cout<<"[ExN02FieldSetup] G4HelixSimpleRunge is called"<<G4endl;
      break;
    case 8:
      fStepper = new G4CashKarpRKF45( fEquation );
      G4cout<<"[ExN02FieldSetup] G4CashKarpRKF45 is called"<<G4endl;
      break;
    case 9:
      fStepper = new G4RKG3_Stepper( fEquation );
      G4cout<<"[ExN02FieldSetup] G4RKG3_Stepper is called"<<G4endl;
      break;
    default: fStepper = 0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02FieldSetup::SetFieldValue(G4double fieldStrength)
{
  // Set the value of the Global Field to fieldValue along X

#ifdef G4VERBOSE
  G4cout << "[ExN02FieldSetup] Setting Field strength to "
         << fieldStrength / CLHEP::tesla  << " Tesla."; // << G4endl;
#endif

  G4ThreeVector fieldSetVec(fieldStrength, 0.0, 0.0);
  this->SetFieldValue( fieldSetVec );

#ifdef G4VERBOSE
  G4double fieldValue[6],  position[4];
  position[0] = position[1] = position[2] = position[3] = 0.0;
  if ( fieldStrength != 0.0 ) {
    fMagneticField->GetFieldValue( position, fieldValue);
    G4ThreeVector fieldVec(fieldValue[0], fieldValue[1], fieldValue[2]);
    // G4cout << " fMagneticField is now " << fMagneticField
    G4cout << "[ExN02FieldSetup] Magnetic field vector is "
           << fieldVec / CLHEP::tesla << " T " << G4endl;
  } else {
    if ( fMagneticField == 0 )
      G4cout << "[ExN02FieldSetup] Magnetic field pointer is null." << G4endl;
    else
      G4Exception("ExN02FieldSetup::SetFieldValue(G4double)",
                  "IncorrectForZeroField",
                  FatalException,
                  "fMagneticField ptr should be set to 0 for no field.");
  }
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02FieldSetup::SetFieldValue(G4ThreeVector fieldVector)
{
  // Set the value of the Global Field

  G4cout << "[ExN02FieldSetup] Magnetic field set to Uniform( "
         << fieldVector / CLHEP::tesla << " ) T" << G4endl;

  if (fMagneticField) delete fMagneticField;
 
  if (fieldVector != G4ThreeVector(0.,0.,0.))
  {
    fMagneticField = new G4UniformMagField(fieldVector);
  }
  else
  {
    // If the new field's value is Zero, signal it as below
    // so that it is not used for propagation.
    fMagneticField = 0;
  }

  // Set this as the field of the global Field Manager
  GetGlobalFieldManager()->SetDetectorField(fMagneticField);

  // Now notify equation of new field
  fEquation->SetFieldObj( fMagneticField );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4FieldManager* ExN02FieldSetup::GetGlobalFieldManager()
{
  //  Utility method

  return G4TransportationManager::GetTransportationManager()
           ->GetFieldManager();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
