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
// $Id: ND280PhysicalVolumeInformation.cc,v 1.10 2006-06-29 17:48:24 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ND280PhysicalVolumeInformation.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <fstream>
#include "Const.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ND280PhysicalVolumeInformation::ND280PhysicalVolumeInformation(G4int dID0, G4String dName0, G4ThreeVector mppcPosition) 
{
  fDetID = dID0;
  fDetName = dName0;
  fMPPCPosition = mppcPosition;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ND280PhysicalVolumeInformation::ND280PhysicalVolumeInformation() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ND280PhysicalVolumeInformation::~ND280PhysicalVolumeInformation() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ND280PhysicalVolumeInformation::ND280PhysicalVolumeInformation(const ND280PhysicalVolumeInformation& right)
{
    fDetID = right.fDetID;
  fDetName = right.fDetName;
  fMPPCPosition = right.fMPPCPosition;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const ND280PhysicalVolumeInformation& ND280PhysicalVolumeInformation::operator=(const ND280PhysicalVolumeInformation& right)
{
  fDetID = right.fDetID;
  fDetName = right.fDetName;
  fMPPCPosition = right.fMPPCPosition;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void ND280PhysicalVolumeInformation::Print()
{

  G4cout.precision(4);
  
  //G4cout << " Mod:" << mod 
  G4cout << " detID:" << fDetID
	 << " detName: " << fNameDet
	 G4endl;

}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
