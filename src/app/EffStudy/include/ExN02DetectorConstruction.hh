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
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4VisAttributes.hh"

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

  G4double GetBasketFullLength() {return fBasketLength;};
  G4double GetBasketFullWidth()  {return fBasketWidth;};
  G4double GetBasketFullHeight() {return fBasketHeight;};

  G4double GetTrackerFullLength() {return fTrackerLength;};
  G4double GetTrackerFullWidth()  {return fTrackerWidth;};
  G4double GetTrackerFullHeight() {return fTrackerHeight;};

  G4double GetTargetFullLength()  {return fTargetLength;};
  G4double GetTargetFullWidth()   {return fTargetWidth;};
  G4double GetTargetFullHeight()  {return fTargetHeight;};

  G4double GetWorldFullLength()   {return fWorldLength;}; 
  G4double GetWorldFullWidth()    {return fWorldWidth;}; 
  G4double GetWorldFullHeight()   {return fWorldHeight;}; 
     
  void setTargetMaterial (G4String);
  void setTargetMaterial1 (G4String);
  void setTargetMaterial2 (G4String);
  void setChamberMaterial(G4String);
  void SetMagField(G4double);
  void SetMaxStep (G4double);     
  
  void DefineMaterials();
                         
  const G4VPhysicalVolume* GetAbsorberPV() const; // new
  const G4VPhysicalVolume* GetGapPV() const; // new

  G4LogicalVolume* GetPieceTPC(void);
  const G4VisAttributes* GetVisual(void) const;
 
protected:
  
  G4Material* FindMaterial(G4String m);

private:

  G4Cache<ExN02FieldSetup*>    fEmFieldSetup;

  G4Box*             solidWorld;    // pointer to the solid envelope 
  G4LogicalVolume*   logicWorld;    // pointer to the logical envelope
  G4VPhysicalVolume* physiWorld;    // pointer to the physical envelope
     
  G4Box*             solidBasket;  // pointer to the solid Basket
  G4LogicalVolume*   logicBasket;  // pointer to the logical Basket
  G4VPhysicalVolume* physiBasket;  // pointer to the physical Basket

  G4Box*             solidTarget;   // pointer to the solid Target
  G4LogicalVolume*   logicTarget;   // pointer to the logical Target
  G4VPhysicalVolume* physiTarget;   // pointer to the physical Target
               
  G4Box*             solidTarget1;   // pointer to the solid Target                                                                                                                                  
  G4LogicalVolume*   logicTarget1;   // pointer to the logical Target                                                                                                                                 
  G4VPhysicalVolume* physiTarget1;   // pointer to the physical Target

  G4Box*             solidTarget2;   // pointer to the solid Target                                                                                                                
  G4LogicalVolume*   logicTarget2;   // pointer to the logical Target                                                                                                                            
  G4VPhysicalVolume* physiTarget2;   // pointer to the physical Target 

  G4Box*             solidTracker;  // pointer to the solid Tracker
  G4LogicalVolume*   logicTracker;  // pointer to the logical Tracker
  G4VPhysicalVolume* physiTracker;  // pointer to the physical Tracker
     
  G4Box*             solidChamberUp;  // pointer to the solid Chamber
  G4LogicalVolume*   logicChamberUp;  // pointer to the logical Chamber
  G4VPhysicalVolume* physiChamberUp;  // pointer to the physical Chamber
     
  G4Box*             solidChamberDown;  // pointer to the solid Chamber
  G4LogicalVolume*   logicChamberDown;  // pointer to the logical Chamber
  G4VPhysicalVolume* physiChamberDown;  // pointer to the physical Chamber

  G4Material* WorldMater;  // pointer to the world  material
  G4Material* BasketMater;  // pointer to the basket  material
  G4Material* TrackerMater;  // pointer to the tracker  material
  G4Material* TargetMater;  // pointer to the target  material
  G4Material* TargetMater1;  // pointer to the target  material
  G4Material* TargetMater2;  // pointer to the target  material
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

  G4double fBasketLength;           // Full length of Basket
  G4double fBasketWidth;            // Full Width of Basket
  G4double fBasketHeight;           // Full Height of Basket
  
  G4double fTargetLength;           // Full length of Target
  G4double fTargetWidth;            // Full Width of Target
  G4double fTargetHeight;           // Full Height of Target
 
  G4double fTrackerLength;          // Full length of Tracker
  G4double fTrackerWidth;           // Full width of Tracker
  G4double fTrackerHeight;          // Full height of Tracker

  G4int    fNbOfChambers;            // Nb of chambers in the tracker region
  G4double fChamberSpacing;         // distance between chambers 
  G4double fChamberLength;           // length of the chambers
  G4double fChamberWidth;            // width of the chambers
  G4double fChamberHeight;           // height of the chambers
 
  G4VPhysicalVolume*   fAbsorberPV; // the absorber physical volume
  G4VPhysicalVolume*   fGapPV;      // the gap physical volume
  G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps

  // Visualization
  G4VisAttributes* BoxVisAtt;
  G4VisAttributes* TargetVisAtt;
  G4VisAttributes* ChamberVisAtt;
  G4VisAttributes* ChamberCO2;
  G4VisAttributes* ChamberDeadMat;





  ////////////////////////////////
  //                            //
  //        FROM nd280mc        //
  //                            //
  ////////////////////////////////

  /// Variables to transform TPC MM modules
  G4ThreeVector tpcMMTrans[2][12];
  G4RotationMatrix* tpcMMRot[2][12];
  G4ThreeVector tpcMMRotAxis;

  G4ThreeVector GetRotOffset(G4ThreeVector axis, double angle);
 
  void SetLength(double d){fLength = d;};
  void SetWidth(double d){fWidth = d;};
  void SetHeight(double d){fHeight = d;};
  double GetLength(){return fLength;};
  double GetWidth(){return fWidth;};
  double GetHeight(){return fHeight;};
  double fLength;
  double fWidth;
  double fHeight;

  /// Set the width (x) dimension of the TPC drift volume.
  void SetDriftWidth(double d) {fDriftWidth = d;}
  /// Get the width (x) dimension of the TPC drift volume.
  double GetDriftWidth(void) {return fDriftWidth;}
  /// Set the height (y) dimension of the TPC drift volume.
  void SetDriftHeight(double d) {fDriftHeight = d;}
  /// Get the height (y) dimension of the TPC drift volume.
  double GetDriftHeight(void) {return fDriftHeight;}
  /// Set the length (z) dimension of the TPC drift volume.
  void SetDriftLength(double d) {fDriftLength = d;}
  /// Get the length (z) dimension of the TPC drift volume.
  double GetDriftLength(void) {return fDriftLength;}
  /// Set the central cathode thickness of the TPC.
  void SetCathodeThickness(double d) {fCathodeThickness = d;}
  /// Get the central cathode thickness of the TPC.
  double GetCathodeThickness(void) {return fCathodeThickness;}
  /// Set the amount of CO2 gap on the top of the TPC.
  void SetCO2Top(double d) {fCO2Top = d;}
  /// Get the amount of CO2 gap on the top of the TPC.
  double GetCO2Top(void) {return fCO2Top;}
  /// Set the amount of CO2 gap on the sides of the TPC.
  void SetCO2Sides(double d) {fCO2Sides = d;}
  /// Get the amount of CO2 gap on the sides of the TPC.
  double GetCO2Sides(void) {return fCO2Sides;}
  /// Set the amount of CO2 gap on the bottom of the TPC.
  void SetCO2Bottom(double d) {fCO2Bottom = d;}
  /// Get the amount of CO2 gap on the bottom of the TPC.
  double GetCO2Bottom(void) {return fCO2Bottom;}
  /// Set the inner box wall thickness.
  void SetInnerBoxWall(double d) {fInnerBoxWall = d;}
  /// Get the inner box wall thickness.
  double GetInnerBoxWall(void) {return fInnerBoxWall;}
  /// Set the outer box wall thickness.
  void SetOuterBoxWall(double d) {fOuterBoxWall = d;}
  /// Get the outer box wall thickness.
  double GetOuterBoxWall(void) {return fOuterBoxWall;}
  /// Get the vertical offset required for the FGD to be
  /// centered on the TPC MM pattern (inner cradle)
  double GetActiveTPCVerticalOffset(void) {return fActiveTPCVerticalOffset;}
  /// Set the step length limit for charged particles inside of the TPC.
  /// The step length limit should be short enough that the curvature of
  /// tracks is properly saved.  In general, it should probably be about the
  /// same as the minimum pad dimension.
  void SetSteppingLimit(double d) {fSteppingLimit = d;}
  /// Get the step length limit for charged particles inside of the TPC.
  double GetSteppingLimit(void) {return fSteppingLimit;}
  /// Set the flag to show the outer volume.
  void SetShowOuterVolume(bool flag) {fShowOuterVolume = flag;}
  /// Get the flag to show the outer volume.
  bool GetShowOuterVolume(void) {return fShowOuterVolume;}    
  // Set transformation parameters for TPC MM modules.
  //void SetMMTranslation(int rp, int mm, G4ThreeVector trans); 
  //void SetMMRotation(int rp, int mm, double angle);  
  /// The width (x) dimension of the TPC drift volume.
  double fDriftWidth;
  /// The height (y) dimension of the TPC drift volume.
  double fDriftHeight;
  /// The length (z) dimension of the TPC drift volume.
  double fDriftLength;
  /// The central cathode thickness of the TPC.
  double fCathodeThickness;
  /// The amount of CO2 gap on the top of the TPC.
  double fCO2Top;
  /// The amount of CO2 gap on the sides of the TPC.
  double fCO2Sides;
  /// The amount of CO2 gap on the bottom of the TPC.
  double fCO2Bottom;
  /// The inner box wall thickness.
  double fInnerBoxWall;
  /// The outer box wall thickness.
  double fOuterBoxWall;
  /// The vertical offset required for the FGD to be
  /// centered on the TPC MM pattern (inner cradle)
  double fActiveTPCVerticalOffset;
  /// Flag to show the envelope volume instead of the drift volume.
  bool fShowOuterVolume;
  /// The stepping limit used for the TPC drift volume.
  double fSteppingLimit;
  void Init(void);
  /// Method to build the TPC Central Cathode.
  void BuildTPCCentralCathode(G4LogicalVolume* v);
  /// Method to build the TPC Inner and Outer cages.
  void BuildTPCCages(G4LogicalVolume* v);

  /// Flag to turn on extra debug printouts 
  bool DebugTPCMass;

  //
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
