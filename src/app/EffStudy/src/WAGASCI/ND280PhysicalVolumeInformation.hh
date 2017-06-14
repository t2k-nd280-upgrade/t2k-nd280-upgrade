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
// $Id: ND280PhysicalVolumeInformation.hh,v 1.8 2006-06-29 17:47:53 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ND280PhysicalVolumeInformation_h
#define ND280PhysicalVolumeInformation_h 1

#include "G4ThreeVector.hh"
#include "G4String.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ND280PhysicalVolumeInformation
{
public:

  ND280PhysicalVolumeInformation(G4int, G4String, G4ThreeVector);
  ND280PhysicalVolumeInformation();
  ~ND280PhysicalVolumeInformation();
  ND280PhysicalVolumeInformation(const ND280PhysicalVolumeInformation&);
  const ND280PhysicalVolumeInformation& operator=(const ND280PhysicalVolumeInformation&);
  G4int operator==(const ND280PhysicalVolumeInformation&) const;
  //void Print();

private:
  // My new
  G4String fDetName;
  G4int fDetID;
  G4ThreeVector fMPPCPosition;


public:

  inline void SetDetName     (G4String name)    { fDetName = name; };
  inline G4String GetDetName()    { return fDetName; }; 

  inline void SetDetID     (G4int detid)      {fDetID = detid; };
  inline G4int GetDetID() { return fDetID; }// ID of scintillator

  //inline void SetMPPCPosition (G4ThreeVector mppcpos) {for(int i=0;i<3;i++) fMPPCPosition[i] = mppcpos[i]; };
  inline void SetMPPCPosition (G4ThreeVector mppcpos) {fMPPCPosition = mppcpos; };
  inline G4ThreeVector GetMPPCPosition() { return fMPPCPosition; }; 
 };


#endif
