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

#include "ExN02ND280XML.hh"
#include "ExN02TrackerSD.hh"

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



///// AS IN nd280mc /////
#include "ExN02DetectorMessenger.hh"
class ND280Constructor; 

#include "ND280Constructor.hh"
/////////////////////////



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  ExN02DetectorConstruction();
  ~ExN02DetectorConstruction();

public:
  
  G4VPhysicalVolume* Construct();

  // Initialize the XML parser
  ExN02ND280XML *ND280XMLInput;

  const 
  G4VPhysicalVolume* GetTracker() {return physiTracker;};
  
  ExN02TrackerSD *GetSensitiveDetector() {return aTrackerSD;};
  
  /// Return the detector construction messenger
  virtual ExN02DetectorMessenger* GetMessenger(void) {
    return detectorMessenger;
  };

  void DefineDimensions();

  void SetBasketFullLength(G4double length) {fBasketLength = length;};
  void SetBasketFullWidth(G4double width)  {fBasketWidth = width;};
  void SetBasketFullHeight(G4double height) {fBasketHeight = height;};

  void SetTrackerFullLength(G4double length) {fTrackerLength = length;};
  void SetTrackerFullWidth(G4double width)  {fTrackerWidth = width;};
  void SetTrackerFullHeight(G4double height) {fTrackerHeight = height;};

  void SetTargetFullLength1(G4double length)  {fTargetLength1 = length;};
  void SetTargetFullWidth1(G4double width)   {fTargetWidth1 = width;};
  void SetTargetFullHeight1(G4double height)  {fTargetHeight1 = height;};
  void SetTargetPos1(G4double x,G4double y,G4double z) {fTargetPos1 = G4ThreeVector(x,y,z);};

  void SetTargetFullLength2(G4double length)  {fTargetLength2 = length;};
  void SetTargetFullWidth2(G4double width)   {fTargetWidth2 = width;};
  void SetTargetFullHeight2(G4double height)  {fTargetHeight2 = height;};
  void SetTargetPos2(G4double x,G4double y,G4double z) {fTargetPos2 = G4ThreeVector(x,y,z);};

  void SetFGDFullLength1(G4double length)  {fFGDLength1 = length;};
  void SetFGDFullWidth1(G4double width)   {fFGDWidth1 = width;};
  void SetFGDFullHeight1(G4double height)  {fFGDHeight1 = height;};
  void SetFGDPos1(G4double x,G4double y,G4double z) {fFGDPos1 = G4ThreeVector(x,y,z);};

  void SetFGDFullLength2(G4double length)  {fFGDLength2 = length;};
  void SetFGDFullWidth2(G4double width)   {fFGDWidth2 = width;};
  void SetFGDFullHeight2(G4double height)  {fFGDHeight2 = height;};
  void SetFGDPos2(G4double x,G4double y,G4double z) {fFGDPos2 = G4ThreeVector(x,y,z);};

  void SetForwTPCPos1(G4double x,G4double y,G4double z) {fForwTPCPos1 = G4ThreeVector(x,y,z);};
  void SetForwTPCPos2(G4double x,G4double y,G4double z) {fForwTPCPos2 = G4ThreeVector(x,y,z);};
  void SetForwTPCPos3(G4double x,G4double y,G4double z) {fForwTPCPos3 = G4ThreeVector(x,y,z);};

  void SetSideTPCFullLength1(G4double length)  {fSideTPCLength1 = length;};
  void SetSideTPCFullWidth1(G4double width)   {fSideTPCWidth1 = width;};
  void SetSideTPCFullHeight1(G4double height)  {fSideTPCHeight1 = height;};
  void SetSideTPCUpPos1(G4double x,G4double y,G4double z) {fSideTPCUpPos1 = G4ThreeVector(x,y,z);};
  void SetSideTPCDownPos1(G4double x,G4double y,G4double z) {fSideTPCDownPos1 = G4ThreeVector(x,y,z);};

  void SetSideTPCFullLength2(G4double length)  {fSideTPCLength2 = length;};
  void SetSideTPCFullWidth2(G4double width)   {fSideTPCWidth2 = width;};
  void SetSideTPCFullHeight2(G4double height)  {fSideTPCHeight2 = height;};
  void SetSideTPCUpPos2(G4double x,G4double y,G4double z) {fSideTPCUpPos2 = G4ThreeVector(x,y,z);};
  void SetSideTPCDownPos2(G4double x,G4double y,G4double z) {fSideTPCDownPos2 = G4ThreeVector(x,y,z);};

  // ToF Downstream

  void SetToFFullLength_TopDown(G4double length)  {fToFLength_TopDown = length;};
  void SetToFFullWidth_TopDown(G4double width)   {fToFWidth_TopDown = width;};
  void SetToFFullHeight_TopDown(G4double height)  {fToFHeight_TopDown = height;};
  void SetToFPos_TopDown(G4double x,G4double y,G4double z) {fToFPos_TopDown = G4ThreeVector(x,y,z);};

  void SetToFFullLength_BotDown(G4double length)  {fToFLength_BotDown = length;};
  void SetToFFullWidth_BotDown(G4double width)   {fToFWidth_BotDown = width;};
  void SetToFFullHeight_BotDown(G4double height)  {fToFHeight_BotDown = height;};
  void SetToFPos_BotDown(G4double x,G4double y,G4double z) {fToFPos_BotDown = G4ThreeVector(x,y,z);};

  void SetToFFullLength_RightDown(G4double length)  {fToFLength_RightDown = length;};
  void SetToFFullWidth_RightDown(G4double width)   {fToFWidth_RightDown = width;};
  void SetToFFullHeight_RightDown(G4double height)  {fToFHeight_RightDown = height;};
  void SetToFPos_RightDown(G4double x,G4double y,G4double z) {fToFPos_RightDown = G4ThreeVector(x,y,z);};

  void SetToFFullLength_LeftDown(G4double length)  {fToFLength_LeftDown = length;};
  void SetToFFullWidth_LeftDown(G4double width)   {fToFWidth_LeftDown = width;};
  void SetToFFullHeight_LeftDown(G4double height)  {fToFHeight_LeftDown = height;};
  void SetToFPos_LeftDown(G4double x,G4double y,G4double z) {fToFPos_LeftDown = G4ThreeVector(x,y,z);};

  void SetToFFullLength_BackDown(G4double length)  {fToFLength_BackDown = length;};
  void SetToFFullWidth_BackDown(G4double width)   {fToFWidth_BackDown = width;};
  void SetToFFullHeight_BackDown(G4double height)  {fToFHeight_BackDown = height;};
  void SetToFPos_BackDown(G4double x,G4double y,G4double z) {fToFPos_BackDown = G4ThreeVector(x,y,z);};

  void SetToFFullLength_FrontDown(G4double length)  {fToFLength_FrontDown = length;};
  void SetToFFullWidth_FrontDown(G4double width)   {fToFWidth_FrontDown = width;};
  void SetToFFullHeight_FrontDown(G4double height)  {fToFHeight_FrontDown = height;};
  void SetToFPos_FrontDown(G4double x,G4double y,G4double z) {fToFPos_FrontDown = G4ThreeVector(x,y,z);};

  void SetToFFullLength_ECalP0D(G4double length)  {fToFLength_ECalP0D = length;};
  void SetToFFullWidth_ECalP0D(G4double width)   {fToFWidth_ECalP0D = width;};
  void SetToFFullHeight_ECalP0D(G4double height)  {fToFHeight_ECalP0D = height;};
  void SetToFPos_ECalP0D(G4double x,G4double y,G4double z) {fToFPos_ECalP0D = G4ThreeVector(x,y,z);};

  // ToF Upstream

  void SetToFFullLength_TopUp(G4double length)  {fToFLength_TopUp = length;};
  void SetToFFullWidth_TopUp(G4double width)   {fToFWidth_TopUp = width;};
  void SetToFFullHeight_TopUp(G4double height)  {fToFHeight_TopUp = height;};
  void SetToFPos_TopUp(G4double x,G4double y,G4double z) {fToFPos_TopUp = G4ThreeVector(x,y,z);};

  void SetToFFullLength_BotUp(G4double length)  {fToFLength_BotUp = length;};
  void SetToFFullWidth_BotUp(G4double width)   {fToFWidth_BotUp = width;};
  void SetToFFullHeight_BotUp(G4double height)  {fToFHeight_BotUp = height;};
  void SetToFPos_BotUp(G4double x,G4double y,G4double z) {fToFPos_BotUp = G4ThreeVector(x,y,z);};

  void SetToFFullLength_RightUp(G4double length)  {fToFLength_RightUp = length;};
  void SetToFFullWidth_RightUp(G4double width)   {fToFWidth_RightUp = width;};
  void SetToFFullHeight_RightUp(G4double height)  {fToFHeight_RightUp = height;};
  void SetToFPos_RightUp(G4double x,G4double y,G4double z) {fToFPos_RightUp = G4ThreeVector(x,y,z);};

  void SetToFFullLength_LeftUp(G4double length)  {fToFLength_LeftUp = length;};
  void SetToFFullWidth_LeftUp(G4double width)   {fToFWidth_LeftUp = width;};
  void SetToFFullHeight_LeftUp(G4double height)  {fToFHeight_LeftUp = height;};
  void SetToFPos_LeftUp(G4double x,G4double y,G4double z) {fToFPos_LeftUp = G4ThreeVector(x,y,z);};

  void SetToFFullLength_BackUp(G4double length)  {fToFLength_BackUp = length;};
  void SetToFFullWidth_BackUp(G4double width)   {fToFWidth_BackUp = width;};
  void SetToFFullHeight_BackUp(G4double height)  {fToFHeight_BackUp = height;};
  void SetToFPos_BackUp(G4double x,G4double y,G4double z) {fToFPos_BackUp = G4ThreeVector(x,y,z);};

  void SetToFFullLength_FrontUp(G4double length)  {fToFLength_FrontUp = length;};
  void SetToFFullWidth_FrontUp(G4double width)   {fToFWidth_FrontUp = width;};
  void SetToFFullHeight_FrontUp(G4double height)  {fToFHeight_FrontUp = height;};
  void SetToFPos_FrontUp(G4double x,G4double y,G4double z) {fToFPos_FrontUp = G4ThreeVector(x,y,z);};

  //
  
  void SetWorldFullLength(G4double length)   {fWorldLength = length;}; 
  void SetWorldFullWidth(G4double width)    {fWorldWidth = width;}; 
  void SetWorldFullHeight(G4double height)   {fWorldHeight = height;}; 

  G4double GetBasketFullLength() {return fBasketLength;};
  G4double GetBasketFullWidth()  {return fBasketWidth;};
  G4double GetBasketFullHeight() {return fBasketHeight;};

  G4double GetTrackerFullLength() {return fTrackerLength;};
  G4double GetTrackerFullWidth()  {return fTrackerWidth;};
  G4double GetTrackerFullHeight() {return fTrackerHeight;};

  G4double GetTargetFullLength1()  {return fTargetLength1;};
  G4double GetTargetFullWidth1()   {return fTargetWidth1;};
  G4double GetTargetFullHeight1()  {return fTargetHeight1;};
  G4ThreeVector GetTargetPos1()    {return fTargetPos1;};

  G4double GetTargetFullLength2()  {return fTargetLength2;};
  G4double GetTargetFullWidth2()   {return fTargetWidth2;};
  G4double GetTargetFullHeight2()  {return fTargetHeight2;};
  G4ThreeVector GetTargetPos2()    {return fTargetPos2;};


  G4double GetFGDFullLength1()  {return fFGDLength1;};
  G4double GetFGDFullWidth1()   {return fFGDWidth1;};
  G4double GetFGDFullHeight1()  {return fFGDHeight1;};
  G4ThreeVector GetFGDPos1()    {return fFGDPos1;};

  G4double GetFGDFullLength2()  {return fFGDLength2;};
  G4double GetFGDFullWidth2()   {return fFGDWidth2;};
  G4double GetFGDFullHeight2()  {return fFGDHeight2;};
  G4ThreeVector GetFGDPos2()    {return fFGDPos2;};



  G4ThreeVector GetForwTPCPos1()  {return fForwTPCPos1;};
  G4ThreeVector GetForwTPCPos2()  {return fForwTPCPos2;};
  G4ThreeVector GetForwTPCPos3()  {return fForwTPCPos3;};

  G4double GetSideTPCFullLength1()  {return fSideTPCLength1;};
  G4double GetSideTPCFullWidth1()   {return fSideTPCWidth1;};
  G4double GetSideTPCFullHeight1()  {return fSideTPCHeight1;};
  G4ThreeVector GetSideTPCUpPos1()  {return fSideTPCUpPos1;};
  G4ThreeVector GetSideTPCDownPos1(){return fSideTPCDownPos1;};

  G4double GetSideTPCFullLength2()  {return fSideTPCLength2;};
  G4double GetSideTPCFullWidth2()   {return fSideTPCWidth2;};
  G4double GetSideTPCFullHeight2()  {return fSideTPCHeight2;};
  G4ThreeVector GetSideTPCUpPos2()  {return fSideTPCUpPos2;};
  G4ThreeVector GetSideTPCDownPos2(){return fSideTPCDownPos2;};
  
  // ToF Downstream

  G4double GetToFFullLength_TopDown()  {return fToFLength_TopDown;};
  G4double GetToFFullWidth_TopDown()   {return fToFWidth_TopDown;};
  G4double GetToFFullHeight_TopDown()  {return fToFHeight_TopDown;};
  G4ThreeVector GetToFPos_TopDown()  {return fToFPos_TopDown;};

  G4double GetToFFullLength_BotDown()  {return fToFLength_BotDown;};
  G4double GetToFFullWidth_BotDown()   {return fToFWidth_BotDown;};
  G4double GetToFFullHeight_BotDown()  {return fToFHeight_BotDown;};
  G4ThreeVector GetToFPos_BotDown()  {return fToFPos_BotDown;};

  G4double GetToFFullLength_RightDown()  {return fToFLength_RightDown;};
  G4double GetToFFullWidth_RightDown()   {return fToFWidth_RightDown;};
  G4double GetToFFullHeight_RightDown()  {return fToFHeight_RightDown;};
  G4ThreeVector GetToFPos_RightDown()  {return fToFPos_RightDown;};

  G4double GetToFFullLength_LeftDown()  {return fToFLength_LeftDown;};
  G4double GetToFFullWidth_LeftDown()   {return fToFWidth_LeftDown;};
  G4double GetToFFullHeight_LeftDown()  {return fToFHeight_LeftDown;};
  G4ThreeVector GetToFPos_LeftDown()  {return fToFPos_LeftDown;};

  G4double GetToFFullLength_BackDown()  {return fToFLength_BackDown;};
  G4double GetToFFullWidth_BackDown()   {return fToFWidth_BackDown;};
  G4double GetToFFullHeight_BackDown()  {return fToFHeight_BackDown;};
  G4ThreeVector GetToFPos_BackDown()  {return fToFPos_BackDown;};

  G4double GetToFFullLength_FrontDown()  {return fToFLength_FrontDown;};
  G4double GetToFFullWidth_FrontDown()   {return fToFWidth_FrontDown;};
  G4double GetToFFullHeight_FrontDown()  {return fToFHeight_FrontDown;};
  G4ThreeVector GetToFPos_FrontDown()  {return fToFPos_FrontDown;};

  G4double GetToFFullLength_ECalP0D()  {return fToFLength_ECalP0D;};
  G4double GetToFFullWidth_ECalP0D()   {return fToFWidth_ECalP0D;};
  G4double GetToFFullHeight_ECalP0D()  {return fToFHeight_ECalP0D;};
  G4ThreeVector GetToFPos_ECalP0D()  {return fToFPos_ECalP0D;};

  // ToF Upstream

  G4double GetToFFullLength_TopUp()  {return fToFLength_TopUp;};
  G4double GetToFFullWidth_TopUp()   {return fToFWidth_TopUp;};
  G4double GetToFFullHeight_TopUp()  {return fToFHeight_TopUp;};
  G4ThreeVector GetToFPos_TopUp()  {return fToFPos_TopUp;};

  G4double GetToFFullLength_BotUp()  {return fToFLength_BotUp;};
  G4double GetToFFullWidth_BotUp()   {return fToFWidth_BotUp;};
  G4double GetToFFullHeight_BotUp()  {return fToFHeight_BotUp;};
  G4ThreeVector GetToFPos_BotUp()  {return fToFPos_BotUp;};

  G4double GetToFFullLength_RightUp()  {return fToFLength_RightUp;};
  G4double GetToFFullWidth_RightUp()   {return fToFWidth_RightUp;};
  G4double GetToFFullHeight_RightUp()  {return fToFHeight_RightUp;};
  G4ThreeVector GetToFPos_RightUp()  {return fToFPos_RightUp;};

  G4double GetToFFullLength_LeftUp()  {return fToFLength_LeftUp;};
  G4double GetToFFullWidth_LeftUp()   {return fToFWidth_LeftUp;};
  G4double GetToFFullHeight_LeftUp()  {return fToFHeight_LeftUp;};
  G4ThreeVector GetToFPos_LeftUp()  {return fToFPos_LeftUp;};

  G4double GetToFFullLength_BackUp()  {return fToFLength_BackUp;};
  G4double GetToFFullWidth_BackUp()   {return fToFWidth_BackUp;};
  G4double GetToFFullHeight_BackUp()  {return fToFHeight_BackUp;};
  G4ThreeVector GetToFPos_BackUp()  {return fToFPos_BackUp;};

  G4double GetToFFullLength_FrontUp()  {return fToFLength_FrontUp;};
  G4double GetToFFullWidth_FrontUp()   {return fToFWidth_FrontUp;};
  G4double GetToFFullHeight_FrontUp()  {return fToFHeight_FrontUp;};
  G4ThreeVector GetToFPos_FrontUp()  {return fToFPos_FrontUp;};
  
  //
  
  G4double GetWorldFullLength()   {return fWorldLength;}; 
  G4double GetWorldFullWidth()    {return fWorldWidth;}; 
  G4double GetWorldFullHeight()   {return fWorldHeight;}; 
     
  void setMaterial_Target (G4String);
  void setMaterial_Target1 (G4String);
  void setMaterial_Target2 (G4String);
  void setMaterial_FGD1 (G4String);
  void setMaterial_FGD2 (G4String);
  void setMaterial_SideTPC(G4String);
  void SetMagField(G4double);
  void SetMaxStep (G4double);     
  
  void DefineMaterials();
                         
  const G4VPhysicalVolume* GetAbsorberPV() const; // new
  const G4VPhysicalVolume* GetGapPV() const; // new

  G4LogicalVolume* GetPieceTPC(G4String name,G4String parentname);
  
  //G4LogicalVolume* GetPieceDSECal(G4String name,G4String parentname);

  const G4VisAttributes* GetVisual(void) const;

  bool SDLoopOverDaughter(const G4LogicalVolume* SDLog, const G4VPhysicalVolume *theG4PhysVol);
    
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

  // G4Box*             solidTarget;   // pointer to the solid Target
  // G4LogicalVolume*   logicTarget;   // pointer to the logical Target
  // G4VPhysicalVolume* physiTarget;   // pointer to the physical Target
               
  G4Box*             solidTarget1;   // pointer to the solid Target       
  G4LogicalVolume*   logicTarget1;   // pointer to the logical Target      
  G4VPhysicalVolume* physiTarget1;   // pointer to the physical Target

  G4Box*             solidTarget2;   // pointer to the solid Target 
  G4LogicalVolume*   logicTarget2;   // pointer to the logical Target 
  G4VPhysicalVolume* physiTarget2;   // pointer to the physical Target 

  G4Box*             solidFGD1;   // pointer to the solid FGD       
  G4LogicalVolume*   logicFGD1;   // pointer to the logical FGD      
  G4VPhysicalVolume* physiFGD1;   // pointer to the physical FGD

  G4Box*             solidFGD2;   // pointer to the solid FGD 
  G4LogicalVolume*   logicFGD2;   // pointer to the logical FGD 
  G4VPhysicalVolume* physiFGD2;   // pointer to the physical FGD 

  G4LogicalVolume*   logicSuperFGD1;
  G4VPhysicalVolume* physiSuperFGD1; 

  G4LogicalVolume*   logicSuperFGD2;
  G4VPhysicalVolume* physiSuperFGD2;

  G4Box*             solidTracker;  // pointer to the solid Tracker
  G4LogicalVolume*   logicTracker;  // pointer to the logical Tracker
  G4VPhysicalVolume* physiTracker;  // pointer to the physical Tracker
     
  G4Box*             solidSideTPCUp1;  // pointer to the solid Side TPC Up 1
  G4LogicalVolume*   logicSideTPCUp1;  // pointer to the logical Side TPC Up 1
  G4VPhysicalVolume* physiSideTPCUp1;  // pointer to the physical Side TPC Up 1

  G4Box*             solidSideTPCUp2;  // pointer to the solid Side TPC Up 2
  G4LogicalVolume*   logicSideTPCUp2;  // pointer to the logical Side TPC Up 2
  G4VPhysicalVolume* physiSideTPCUp2;  // pointer to the physical Side TPC Up 2
     
  G4Box*             solidSideTPCDown1;  // pointer to the solid Side TPC Down 1
  G4LogicalVolume*   logicSideTPCDown1;  // pointer to the logical Side TPC Down 1
  G4VPhysicalVolume* physiSideTPCDown1;  // pointer to the physical Side TPC Down 1

  G4Box*             solidSideTPCDown2;  // pointer to the solid Side TPC Down 2
  G4LogicalVolume*   logicSideTPCDown2;  // pointer to the logical Side TPC Down 2
  G4VPhysicalVolume* physiSideTPCDown2;  // pointer to the physical Side TPC Down 2

  //G4Box*             solidToF_TopDown;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_TopDown; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_TopDown; // pointer to the physical ToF 

  //G4Box*             solidToF_BotDown;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_BotDown; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_BotDown; // pointer to the physical ToF 

  //G4Box*             solidToF_RightDown;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_RightDown; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_RightDown; // pointer to the physical ToF 

  //G4Box*             solidToF_LeftDown;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_LeftDown; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_LeftDown; // pointer to the physical ToF 

  //G4Box*             solidToF_BackDown;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_BackDown; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_BackDown; // pointer to the physical ToF 

  //G4Box*             solidToF_FrontDown;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_FrontDown; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_FrontDown; // pointer to the physical ToF 

  //G4Box*             solidToF_ECalP0D;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_ECalP0D; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_ECalP0D; // pointer to the physical ToF 

  //G4Box*             solidToF_TopUp;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_TopUp; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_TopUp; // pointer to the physical ToF 

  //G4Box*             solidToF_BotUp;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_BotUp; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_BotUp; // pointer to the physical ToF 

  //G4Box*             solidToF_RightUp;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_RightUp; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_RightUp; // pointer to the physical ToF 

  //G4Box*             solidToF_LeftUp;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_LeftUp; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_LeftUp; // pointer to the physical ToF 

  //G4Box*             solidToF_BackUp;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_BackUp; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_BackUp; // pointer to the physical ToF 

  //G4Box*             solidToF_FrontUp;  // pointer to the solid ToF
  G4LogicalVolume*   logicToF_FrontUp; // pointer to the logical ToF           
  G4VPhysicalVolume* physiToF_FrontUp; // pointer to the physical ToF 

  G4Material* WorldMater;  // pointer to the world  material
  G4Material* BasketMater;  // pointer to the basket  material
  G4Material* TrackerMater;  // pointer to the tracker  material
  G4Material* TargetMater1;  // pointer to the target  material
  G4Material* TargetMater2;  // pointer to the target  material
  G4Material* FGDMater1;  // pointer to the target  material
  G4Material* FGDMater2;  // pointer to the target  material

  G4Material* SideTPCMater; // pointer to the side tpc material                  
  G4Material* fDefaultMaterial;   /// The default material.       

  ExN02TrackerSD* aTrackerSD; // sensitive detector
  
  G4UserLimits* stepLimit;             // pointer to user step limits

  //ExN02MagneticField* fpMagField;   // pointer to the magnetic field 
     
  ExN02DetectorMessenger* detectorMessenger;  // pointer to the Messenger
       
  G4double fWorldLength;            // Full length of the world volume
  G4double fWorldWidth;             // Full Width of the world volume
  G4double fWorldHeight;            // Full Height of the world volume

  G4double fBasketLength;           // Full length of Basket
  G4double fBasketWidth;            // Full Width of Basket
  G4double fBasketHeight;           // Full Height of Basket
  
  G4double fTargetLength1;           // Full length of Target 1
  G4double fTargetWidth1;            // Full Width of Target 1
  G4double fTargetHeight1;           // Full Height of Target 1
  G4ThreeVector fTargetPos1;         // Position of Target 1

  G4double fTargetLength2;           // Full length of Target 2
  G4double fTargetWidth2;            // Full Width of Target 2
  G4double fTargetHeight2;           // Full Height of Target 2
  G4ThreeVector fTargetPos2;         // Position of Target 2

  G4double fFGDLength1;              // Full length of FGD 1
  G4double fFGDWidth1;               // Full Width of FGD 1
  G4double fFGDHeight1;              // Full Height of FGD 1
  G4ThreeVector fFGDPos1;            // Position of FGD 1

  G4double fFGDLength2;              // Full length of FGD 2
  G4double fFGDWidth2;               // Full Width of FGD 2
  G4double fFGDHeight2;              // Full Height of FGD 2
  G4ThreeVector fFGDPos2;            // Position of FGD 2
 
  G4double fTrackerLength;          // Full length of Tracker
  G4double fTrackerWidth;           // Full width of Tracker
  G4double fTrackerHeight;          // Full height of Tracker

  G4ThreeVector fForwTPCPos1;       // Position of Forward TPC 1
  G4ThreeVector fForwTPCPos2;       // Position of Forward TPC 2
  G4ThreeVector fForwTPCPos3;       // Position of Forward TPC 3

  G4double fSideTPCLength1;           // length of the side tpcs 1
  G4double fSideTPCWidth1;            // width of the side tpcs 1
  G4double fSideTPCHeight1;           // height of the side tpcs 1
  G4ThreeVector fSideTPCUpPos1;       // Position of side tpc Up 1
  G4ThreeVector fSideTPCDownPos1;     // Position of side tpc Down 1

  G4double fSideTPCLength2;           // length of the side tpcs 2
  G4double fSideTPCWidth2;            // width of the side tpcs 2
  G4double fSideTPCHeight2;           // height of the side tpcs 2
  G4ThreeVector fSideTPCUpPos2;       // Position of side tpcs Up 2
  G4ThreeVector fSideTPCDownPos2;     // Position of side tpcs Down 2

  G4double fToFLength_TopDown;           // length of the tof
  G4double fToFWidth_TopDown;            // width of the tof                                       
  G4double fToFHeight_TopDown;           // height of the tof                                      
  G4ThreeVector fToFPos_TopDown;       // Position of side tof

  G4double fToFLength_BotDown;           // length of the tof
  G4double fToFWidth_BotDown;            // width of the tof                                       
  G4double fToFHeight_BotDown;           // height of the tof                                      
  G4ThreeVector fToFPos_BotDown;       // Position of side tof

  G4double fToFLength_RightDown;           // length of the tof
  G4double fToFWidth_RightDown;            // width of the tof                                       
  G4double fToFHeight_RightDown;           // height of the tof                                      
  G4ThreeVector fToFPos_RightDown;       // Position of side tof

  G4double fToFLength_LeftDown;           // length of the tof
  G4double fToFWidth_LeftDown;            // width of the tof                                       
  G4double fToFHeight_LeftDown;           // height of the tof                                      
  G4ThreeVector fToFPos_LeftDown;       // Position of side tof

  G4double fToFLength_BackDown;           // length of the tof
  G4double fToFWidth_BackDown;            // width of the tof                                       
  G4double fToFHeight_BackDown;           // height of the tof                                      
  G4ThreeVector fToFPos_BackDown;       // Position of side tof

  G4double fToFLength_FrontDown;           // length of the tof
  G4double fToFWidth_FrontDown;            // width of the tof                                       
  G4double fToFHeight_FrontDown;           // height of the tof                                      
  G4ThreeVector fToFPos_FrontDown;       // Position of side tof

 G4double fToFLength_ECalP0D;           // length of the tof
  G4double fToFWidth_ECalP0D;            // width of the tof                                       
  G4double fToFHeight_ECalP0D;           // height of the tof                                      
  G4ThreeVector fToFPos_ECalP0D;       // Position of side tof

  G4double fToFLength_TopUp;           // length of the tof
  G4double fToFWidth_TopUp;            // width of the tof                                       
  G4double fToFHeight_TopUp;           // height of the tof                                      
  G4ThreeVector fToFPos_TopUp;       // Position of side tof

  G4double fToFLength_BotUp;           // length of the tof
  G4double fToFWidth_BotUp;            // width of the tof                                       
  G4double fToFHeight_BotUp;           // height of the tof                                      
  G4ThreeVector fToFPos_BotUp;       // Position of side tof

  G4double fToFLength_RightUp;           // length of the tof
  G4double fToFWidth_RightUp;            // width of the tof                                       
  G4double fToFHeight_RightUp;           // height of the tof                                      
  G4ThreeVector fToFPos_RightUp;       // Position of side tof

  G4double fToFLength_LeftUp;           // length of the tof
  G4double fToFWidth_LeftUp;            // width of the tof                                       
  G4double fToFHeight_LeftUp;           // height of the tof                                      
  G4ThreeVector fToFPos_LeftUp;       // Position of side tof

  G4double fToFLength_BackUp;           // length of the tof
  G4double fToFWidth_BackUp;            // width of the tof                                       
  G4double fToFHeight_BackUp;           // height of the tof                                      
  G4ThreeVector fToFPos_BackUp;       // Position of side tof

  G4double fToFLength_FrontUp;           // length of the tof
  G4double fToFWidth_FrontUp;            // width of the tof                                       
  G4double fToFHeight_FrontUp;           // height of the tof                                      
  G4ThreeVector fToFPos_FrontUp;       // Position of side tof

  G4VPhysicalVolume*   fAbsorberPV; // the absorber physical volume
  G4VPhysicalVolume*   fGapPV;      // the gap physical volume
  G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps

  // Visualization
  G4VisAttributes* BoxVisAtt;
  G4VisAttributes* TargetWaterVisAtt;
  G4VisAttributes* TargetScintVisAtt;
  G4VisAttributes* FGDWaterVisAtt;
  G4VisAttributes* FGDScintVisAtt;
  G4VisAttributes* SuperFGDScintVisAtt;
  G4VisAttributes* TPCVisAtt;
  G4VisAttributes* TPCCO2;
  G4VisAttributes* TPCDeadMat;

  // DsECal (not from nd280mc
  ND280Constructor *fND280MCConstructor;

  ////////////////////////////////
  //                            //
  //        FROM nd280mc        //
  //                            //
  ////////////////////////////////
  
  // DsECal
  
  /// The position of the DSECal along the Z axis of the interior part of
  /// the UA1 Magnet.
  G4double fDsECalPosition;
  
  /// The position of the DSECal relative to the centerline of the basket.
  G4double fDsECalVerticalPosition;
  
  /// DsECal transformation variables
  G4ThreeVector fDsECalTrans;
  G4ThreeVector fDsECalRotAxis;
  double fDsECalRotAng;





  /// Variables to transform TPC MM modules
  G4ThreeVector tpcMMTrans[2][12];
  G4RotationMatrix* tpcMMRot[2][12];
  G4ThreeVector tpcMMRotAxis;

  G4ThreeVector GetRotOffset(G4ThreeVector axis, double angle);
 
  void SetLength(double d){fLengthForwTPC = d;};
  void SetWidth(double d){fWidthForwTPC = d;};
  void SetHeight(double d){fHeightForwTPC = d;};
  double GetLengthForwTPC(){return fLengthForwTPC;};
  double GetWidthForwTPC(){return fWidthForwTPC;};
  double GetHeightForwTPC(){return fHeightForwTPC;};
  double fLengthForwTPC;
  double fWidthForwTPC;
  double fHeightForwTPC;

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
