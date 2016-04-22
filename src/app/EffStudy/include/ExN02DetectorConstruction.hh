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
// $Id: ExN02DetectorConstruction.hh,v 1.10 2008-09-22 16:41:20 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN02DetectorConstruction_h
#define ExN02DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"

//#include "ExN02MagneticField.hh" // OLD

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VPVParameterisation;
class G4UserLimits;
class G4UniformMagField; // needed for magnetic field

class ExN02DetectorMessenger;
class ExN02FieldSetup;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  ExN02DetectorConstruction();
  ~ExN02DetectorConstruction();

public:
  
  G4VPhysicalVolume* Construct();
     
  const 
  G4VPhysicalVolume* GetTracker() {return physiTracker;};

  G4double GetTrackerFullLength() {return fTrackerLength;};
  G4double GetTrackerFullWidth()  {return fTrackerWidth;};
  G4double GetTrackerFullHeight() {return fTrackerHeight;};

  G4double GetTargetFullLenght()  {return fTargetLength;};
  G4double GetTargetFullWidth()   {return fTargetWidth;};
  G4double GetTargetFullHeight()  {return fTargetHeight;};

  G4double GetWorldFullLength()   {return fWorldLength;}; 
  G4double GetWorldFullWidth()    {return fWorldWidth;}; 
  G4double GetWorldFullHeight()   {return fWorldHeight;}; 
     
  void setTargetMaterial (G4String);
  void setChamberMaterial(G4String);
  void SetMagField(G4double);
  void SetMaxStep (G4double);     
  
  void DefineMaterials();
                         
  const G4VPhysicalVolume* GetAbsorberPV() const; // new
  const G4VPhysicalVolume* GetGapPV() const; // new
  
protected:
  
  G4Material* FindMaterial(G4String m);

private:

  G4Cache<ExN02FieldSetup*>    fEmFieldSetup;

  G4Box*             solidWorld;    // pointer to the solid envelope 
  G4LogicalVolume*   logicWorld;    // pointer to the logical envelope
  G4VPhysicalVolume* physiWorld;    // pointer to the physical envelope
     
  G4Box*             solidTarget;   // pointer to the solid Target
  G4LogicalVolume*   logicTarget;   // pointer to the logical Target
  G4VPhysicalVolume* physiTarget;   // pointer to the physical Target
               
  G4Box*             solidTracker;  // pointer to the solid Tracker
  G4LogicalVolume*   logicTracker;  // pointer to the logical Tracker
  G4VPhysicalVolume* physiTracker;  // pointer to the physical Tracker
     
  G4Box*             solidChamberUp;  // pointer to the solid Chamber
  G4LogicalVolume*   logicChamberUp;  // pointer to the logical Chamber
  G4VPhysicalVolume* physiChamberUp;  // pointer to the physical Chamber
     
  G4Box*             solidChamberDown;  // pointer to the solid Chamber
  G4LogicalVolume*   logicChamberDown;  // pointer to the logical Chamber
  G4VPhysicalVolume* physiChamberDown;  // pointer to the physical Chamber

  G4Material* WorldMater;  // pointer to the target  material
  G4Material* TrackerMater;  // pointer to the target  material
  G4Material* TargetMater;  // pointer to the target  material
  G4Material* ChamberMater; // pointer to the chamber material                  
  G4Material* fDefaultMaterial;   /// The default material.       

  //G4VPVParameterisation* chamberParam; // pointer to chamber parameterisation
  //G4PVReplica* chamberParam; // pointer to chamber replica

  G4UserLimits* stepLimit;             // pointer to user step limits

  //ExN02MagneticField* fpMagField;   // pointer to the magnetic field 
     
  ExN02DetectorMessenger* detectorMessenger;  // pointer to the Messenger
       
  G4double fWorldLength;            // Full length of the world volume
  G4double fWorldWidth;             // Full Width of the world volume
  G4double fWorldHeight;            // Full Height of the world volume
  
  G4double fTargetLength;           // Full length of Target
  G4double fTargetWidth;            // Full Width of Target
  G4double fTargetHeight;           // Full Height of Target
 
  G4double fTrackerLength;          // Full length of Tracker
  G4double fTrackerWidth;           // Full width of Tracker
  G4double fTrackerHeight;          // Full height of Tracker

  G4int    NbOfChambers;            // Nb of chambers in the tracker region
  G4double ChamberLength;           // length of the chambers
  G4double ChamberWidth;            // width of the chambers
  G4double ChamberHeight;           // height of the chambers
  G4double ChamberSpacing;	    // distance between chambers

  G4VPhysicalVolume*   fAbsorberPV; // the absorber physical volume                                                                                                                                       
  G4VPhysicalVolume*   fGapPV;      // the gap physical volume                                                                                                                                            

  G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps                                                                                                                              

};

// In line functions

inline const G4VPhysicalVolume* ExN02DetectorConstruction::GetAbsorberPV() const {
  return fAbsorberPV;
}

inline const G4VPhysicalVolume* ExN02DetectorConstruction::GetGapPV() const  {
  return fGapPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

#endif
