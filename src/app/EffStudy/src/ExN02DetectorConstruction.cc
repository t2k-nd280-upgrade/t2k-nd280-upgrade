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
// $Id: ExN02DetectorConstruction.cc,v 1.22 2010-01-22 11:57:03 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#include "ExN02DetectorConstruction.hh"
#include "ExN02DetectorMessenger.hh"
#include "ExN02ChamberParameterisation.hh"
//#include "ExN02MagneticField.hh" // OLD
#include "ExN02TrackerSD.hh"
#include "ExN02Constants.hh"
#include "ND280BeamConstructor.hh"
#include "ExN02ND280XML.hh"

//#include "ND280InputPersistencyManager.hh"
#include "ND280RootPersistencyManager.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4PVReplica.hh"
#include "G4NistManager.hh"
#include "G4UserLimits.hh"
//#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4AutoDelete.hh"

//#ifdef USE_PAI
#include <G4Region.hh> // used also to keep a list of SD logical volumes
//#endif  
#include "G4RegionStore.hh"

// Magnetic field
#include "ExN02FieldSetup.hh"
//

// VGM demo
#include "Geant4GM/volumes/Factory.h"
#include "RootGM/volumes/Factory.h"
#include "TGeoManager.h"
// end VGM demo

#include "ND280RootGeometryManager.hh"

//ROOT
//#include "TROOT.h"
//#include "TFile.h"
//#include "TSystem.h"
//#include "Cintex/Cintex.h"

//#include "ExN02GeoTree.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02DetectorConstruction::ExN02DetectorConstruction()
 :solidWorld(0),  logicWorld(0),  physiWorld(0),
  solidBasket(0), logicBasket(0), physiBasket(0), 
  solidTarget(0), logicTarget(0), physiTarget(0), 
  solidTracker(0),logicTracker(0),physiTracker(0), 
  solidSideTPCUp1(0),logicSideTPCUp1(0),physiSideTPCUp1(0), 
  solidSideTPCUp2(0),logicSideTPCUp2(0),physiSideTPCUp2(0), 
  solidSideTPCDown1(0),logicSideTPCDown1(0),physiSideTPCDown1(0), 
  solidSideTPCDown2(0),logicSideTPCDown2(0),physiSideTPCDown2(0), 
  WorldMater(0), BasketMater(0), TargetMater1(0), TargetMater2(0), SideTPCMater(0), fDefaultMaterial(0), 
  stepLimit(0), 
  //fpMagField(0), // OLD
  fWorldLength(0.), fWorldWidth(0.), fWorldHeight(0.),
  fBasketLength(0.), fBasketWidth(0.), fBasketHeight(0.),
  fTargetLength1(0.),fTargetWidth1(0.),fTargetHeight1(0.),fTargetPos1(0),
  fTargetLength2(0.),fTargetWidth2(0.),fTargetHeight2(0.),fTargetPos2(0),
  fTrackerLength(0.),fTrackerWidth(0.),fTrackerHeight(0.),
  fForwTPCPos1(0.),fForwTPCPos2(0.),fForwTPCPos3(0.),
  fSideTPCLength1(0.),fSideTPCWidth1(0.),fSideTPCHeight1(0.),fSideTPCUpPos1(0.),fSideTPCDownPos1(0.),
  fSideTPCLength2(0.),fSideTPCWidth2(0.),fSideTPCHeight2(0.),fSideTPCUpPos2(0.),fSideTPCDownPos2(0.),
  fAbsorberPV(0), 
  fGapPV(0), 
  fCheckOverlaps(true),
  //tpcMMTrans(0.),tpcMMRotAxis(0.),
  fLengthForwTPC(0.),fWidthForwTPC(0.),fHeightForwTPC(0.),
  fDriftWidth(0.),fDriftHeight(0.),fDriftLength(0.),
  fCathodeThickness(0.),fCO2Top(0.),fCO2Sides(0.),
  fCO2Bottom(0.),fInnerBoxWall(0.),fOuterBoxWall(0.),fActiveTPCVerticalOffset(0.),
  fShowOuterVolume(false),fSteppingLimit(0.),DebugTPCMass(true)
{
  //fpMagField = new ExN02MagneticField(); // OLD
  detectorMessenger = new ExN02DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02DetectorConstruction::~ExN02DetectorConstruction()
{
  //delete fpMagField; // OLD
  delete stepLimit;
  delete detectorMessenger;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* ExN02DetectorConstruction::Construct()
{

  // Take inputs
  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  ND280XMLInput = InputPersistencyManager->GetXMLInput();

  //
  
  BoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  TargetWaterVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  TargetScintVisAtt = new G4VisAttributes(G4Color(0.,0.7,0.));
  TPCVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  TPCCO2 = new G4VisAttributes(G4Colour::Grey); //G4Colour(1.0,1.0,1.0));   
  TPCDeadMat = new G4VisAttributes(G4Colour::Magenta);//G4Colour(1.0,0.0,1.0)); 
  
  TargetWaterVisAtt->SetForceWireframe(true); // TO BE USED

#ifdef USE_PAI
  // Create a region for the PAI Model.
  G4RegionStore::GetInstance()->FindOrCreateRegion("driftRegion");
#endif
  
  // Create a region to get track of all the SD
  G4RegionStore::GetInstance()->FindOrCreateRegion("SDRegion");


  //--------- Define materials ----------
  
  this->DefineMaterials(); // method copied from ND280 software
  
  // G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  // G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  this->DefineDimensions();

  //------------------------------------------------ 
  // Initialize sensitive detectors
  //------------------------------------------------ 
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String trackername = "ExN02/Tracker";
  aTrackerSD = new ExN02TrackerSD( trackername );
  SDman->AddNewDetector( aTrackerSD );
  
  //------------------------------ 
  // World
  //------------------------------ 

  const G4String cNameSolidWorld = "/world";
  const G4String cNameLogicWorld = "/World";
  const G4String cNamePhysiWorld = "/World";
  const G4String cWorldMater = "Air";

  WorldMater   = FindMaterial(cWorldMater); 

  G4double HalfWorldLength = 0.5 * GetWorldFullLength();
  G4double HalfWorldWidth  = 0.5 * GetWorldFullWidth();
  G4double HalfWorldHeight = 0.5 * GetWorldFullHeight();

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(fWorldLength);
  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  solidWorld= new G4Box(cNameSolidWorld,HalfWorldWidth,HalfWorldHeight,HalfWorldLength);
  logicWorld= new G4LogicalVolume(solidWorld,WorldMater,cNameLogicWorld,0,0,0);
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  physiWorld = new G4PVPlacement(0,               // no rotation
                                 G4ThreeVector(), // at (0,0,0)
                                 logicWorld,      // its logical volume
				 cNamePhysiWorld, // its name
                                 0,               // its mother  volume
                                 false,           // no boolean operations
                                 0               // copy number
				 );
  //fCheckOverlaps); // checking overlaps
 
  G4cout << "World is "
  	 << fWorldWidth/mm  << " (width) x " 
  	 << fWorldHeight/mm << " (height) x " 
  	 << fWorldLength/mm << " (length) mm^3" 
	 << " of " << WorldMater->GetName() << G4endl;
  if (DebugTPCMass) G4cout << " mass="<<logicWorld->GetMass()/kg   <<" kg";
  G4cout << G4endl;
  
    
  //------------------------------ 
  // Basket
  //------------------------------

  const G4String cParentNameBasket    = logicWorld->GetName();

  const G4String cNameSolidBasket      = cParentNameBasket+"/basket";
  const G4String cNameLogicBasket      = cParentNameBasket+"/Basket"; // match NEUT requirements
  const G4String cNamePhysiBasket      = cParentNameBasket+"/Basket";
  
  const G4String cBasketMater  = "Air";
  
  BasketMater  = FindMaterial(cBasketMater); 

  G4double HalfBasketLength  = 0.5 * GetBasketFullLength(); 
  G4double HalfBasketWidth   = 0.5 * GetBasketFullWidth(); 
  G4double HalfBasketHeight  = 0.5 * GetBasketFullHeight(); 

  if( (HalfBasketLength>HalfWorldLength) || 
      (HalfBasketWidth>HalfWorldWidth) || 
      (HalfBasketHeight>HalfWorldHeight)  
      )
    {
      G4cerr << G4endl;
      G4cerr << "The Basket is bigger than the World!!!" << G4endl;
      G4cerr << G4endl;
      exit(1);
    }

  G4ThreeVector positionBasket = G4ThreeVector(0,0,0);
  
  solidBasket = new G4Box(cNameSolidBasket,HalfBasketWidth,HalfBasketHeight,HalfBasketLength);
  logicBasket = new G4LogicalVolume(solidBasket,BasketMater,cNameLogicBasket,0,0,0);  
  physiBasket = new G4PVPlacement(0,                 // no rotation
  				  positionBasket,   // at (x,y,z)
  				  logicBasket,      // its logical volume	 
  				  cNamePhysiBasket, // its name
  				  logicWorld,        // its mother  volume
  				  false,             // no boolean operations
  				  0                  // copy number 
  				  );
  //fCheckOverlaps);   // checking overlaps     				 
  
  G4cout << "Basket is " 
  	 << fBasketWidth/mm  << " (width) x " 
  	 << fBasketHeight/mm << " (height) x " 
  	 << fBasketLength/mm << " (length) mm^3"
  	 << " of " << BasketMater->GetName() << G4endl;
  if (DebugTPCMass) G4cout << " mass="<<logicBasket->GetMass()/kg   <<" kg" << G4endl;
  G4cout << G4endl;

  
  //------------------------------ 
  // Tracker
  //------------------------------

  const G4String cParentNameTracker    = logicBasket->GetName();
  
  const G4String cNameSolidTracker     = cParentNameTracker+"/tracker";
  const G4String cNameLogicTracker     = cParentNameTracker+"/Tracker";
  const G4String cNamePhysiTracker     = cParentNameTracker+"/Tracker";

  const G4String cTrackerMater = "Air";

  TrackerMater = FindMaterial(cTrackerMater);  

  G4double HalfTrackerLength = 0.5 * GetTrackerFullLength(); 
  G4double HalfTrackerWidth  = 0.5 * GetTrackerFullWidth(); 
  G4double HalfTrackerHeight = 0.5 * GetTrackerFullHeight(); 

  G4ThreeVector positionTracker = G4ThreeVector(0,0,0);

  solidTracker = new G4Box(cNameSolidTracker,HalfTrackerWidth,HalfTrackerHeight,HalfTrackerLength);
  logicTracker = new G4LogicalVolume(solidTracker,TrackerMater,cNameLogicTracker,0,0,0);  
  physiTracker = new G4PVPlacement(0,                 // no rotation
  				   positionTracker,   // at (x,y,z)
  				   logicTracker,      // its logical volume	 
  				   cNamePhysiTracker, // its name
  				   logicBasket,        // its mother  volume
  				   false,             // no boolean operations
  				   0);                 // copy number 
 
  G4cout << "Tracker is " 
  	 << fTrackerWidth/mm  << " (width) x " 
  	 << fTrackerHeight/mm << " (height) x " 
  	 << fTrackerLength/mm << " (length) mm^3"
  	 << " of " << TrackerMater->GetName() << G4endl;
  if (DebugTPCMass) G4cout << " mass="<<logicTracker->GetMass()/kg   <<" kg" << G4endl;
  G4cout << G4endl;


  //------------------------------ 
  // Forward TPC 1
  //------------------------------

  G4cout << "Forward TPC 1: " << endl;

  G4double currentZ = 0.;

  G4String cParentNameTPC = logicTracker->GetName();

  G4LogicalVolume *tpc1Volume = this->GetPieceTPC("ForwTPC1",cParentNameTPC);
  
  if( ND280XMLInput->GetXMLForwTPCdefault() ){ // default
    double upstreamedge_z = +solidTracker->GetZHalfLength();
    currentZ = upstreamedge_z - GetLengthForwTPC()/2.; 
    SetForwTPCPos1(0,0,currentZ);
  }
  else { // from XML file
    G4double x = ND280XMLInput->GetXMLForwTPCPos1_X();
    G4double y = ND280XMLInput->GetXMLForwTPCPos1_Y();
    G4double z = ND280XMLInput->GetXMLForwTPCPos1_Z();
    SetForwTPCPos1(x,y,z);
  }
  
  new G4PVPlacement(0,
		    GetForwTPCPos1(),
  		    tpc1Volume,
  		    cParentNameTPC+"/ForwTPC1",
  		    logicTracker,
  		    false,
  		    0);

  G4cout << " - position: ( " 
	 << GetForwTPCPos1().x()/mm << ", "
	 << GetForwTPCPos1().y()/mm << ", "
	 << GetForwTPCPos1().z()/mm << " ) mm"  
	 << G4endl << G4endl;


  //------------------------------ 
  // Forward TPC 2
  //------------------------------

  G4cout << "Forward TPC 2: " << endl;

  G4LogicalVolume *tpc2Volume = this->GetPieceTPC("ForwTPC2",cParentNameTPC);
  
  if( ND280XMLInput->GetXMLForwTPCdefault() ){ // default
    currentZ = 0.;
    SetForwTPCPos2(0,0,currentZ);
  }
  else { // from XML file
    G4double x = ND280XMLInput->GetXMLForwTPCPos2_X();
    G4double y = ND280XMLInput->GetXMLForwTPCPos2_Y();
    G4double z = ND280XMLInput->GetXMLForwTPCPos2_Z();
    SetForwTPCPos2(x,y,z);
  }
  
  new G4PVPlacement(0,
		    GetForwTPCPos2(),
  		    tpc2Volume,
  		    cParentNameTPC+"/ForwTPC2",
  		    logicTracker,
  		    false,
  		    0);

  G4cout << " - position: ( " 
	 << GetForwTPCPos2().x()/mm << ", "
	 << GetForwTPCPos2().y()/mm << ", "
	 << GetForwTPCPos2().z()/mm << " ) mm"  
	 << G4endl << G4endl;
  
  
  //------------------------------ 
  // Forward TPC 3
  //------------------------------

  G4cout << "Forward TPC 3: " << endl;

  G4LogicalVolume *tpc3Volume = this->GetPieceTPC("ForwTPC3",cParentNameTPC);
  
  if( ND280XMLInput->GetXMLForwTPCdefault() ){ // default
    double downstreamedge_z = -solidTracker->GetZHalfLength();
    currentZ = downstreamedge_z + GetLengthForwTPC()/2.; 
    SetForwTPCPos3(0,0,currentZ);
  }
  else { // from XML file
    G4double x = ND280XMLInput->GetXMLForwTPCPos3_X();
    G4double y = ND280XMLInput->GetXMLForwTPCPos3_Y();
    G4double z = ND280XMLInput->GetXMLForwTPCPos3_Z();
    SetForwTPCPos3(x,y,z);
  }
  
  new G4PVPlacement(0,
		    GetForwTPCPos3(),
  		    tpc3Volume,
  		    cParentNameTPC+"/ForwTPC3",
  		    logicTracker,
  		    false,
  		    0);

  G4cout << " - position: ( " 
	 << GetForwTPCPos3().x()/mm << ", "
	 << GetForwTPCPos3().y()/mm << ", "
	 << GetForwTPCPos3().z()/mm << " ) mm"  
	 << G4endl << G4endl;


  //------------------------------ 
  // Target 1 - Upstream - Carbon 
  //------------------------------
  
  G4String cParentNameTarget = physiTracker->GetName();

  const G4String cNameSolidTarget1     = cParentNameTarget+"/target1";
  const G4String cNameLogicTarget1     = cParentNameTarget+"/Target1";
  const G4String cNamePhysiTarget1     = cParentNameTarget+"/Target1";
  
  //const G4String cTargetMater1  = "ActiveWater";     //fgd active water  
  const G4String cTargetMater1 = "Water"; // WAGASHI water
  TargetMater1  = FindMaterial(cTargetMater1);  

  G4double targetSizeLength1  = 0.5 * GetTargetFullLength1();    // Half length of the Target 1
  G4double targetSizeWidth1   = 0.5 * GetTargetFullWidth1();     // Half width of the Target 1
  G4double targetSizeHeight1  = 0.5 * GetTargetFullHeight1();    // Half height of the Target 1

  G4double HalfTargetHeight1  = 0.5 * GetTargetFullHeight1();
    
  if( ND280XMLInput->GetXMLTargetdefault1() ){ // default
    G4double Target1_Z = GetLengthForwTPC()/2. + GetTargetFullLength1()/2.;
    SetTargetPos1(0,0,Target1_Z);
  }
  else { // from XML file
    G4double x = ND280XMLInput->GetXMLTargetPos1_X();
    G4double y = ND280XMLInput->GetXMLTargetPos1_Y();
    G4double z = ND280XMLInput->GetXMLTargetPos1_Z();
    SetTargetPos1(x,y,z);
  }

  solidTarget1 = new G4Box(cNameSolidTarget1,targetSizeWidth1,targetSizeHeight1,targetSizeLength1);
  logicTarget1 = new G4LogicalVolume(solidTarget1,TargetMater1,cNameLogicTarget1,0,0,0);
  physiTarget1 = new G4PVPlacement(0,                 // no rotation
				   GetTargetPos1(),    // at (x,y,z)
				   logicTarget1,       // its logical volume  	  
				   cNamePhysiTarget1,  // its name
				   logicTracker,      // its mother  volume
				   //logicWorld,      // its mother  volume
				   false,             // no boolean operations
				   0);                 // copy number 
  //fCheckOverlaps);   

  G4cout << "Target 1: " << endl
	 << " - dimensions: "
	 << GetTargetFullWidth1()/mm  << " (width) x " 
  	 << GetTargetFullHeight1()/mm << " (height) x " 
  	 << GetTargetFullLength1()/mm << " (length) mm^3" 
         << " of " << logicTarget1->GetMaterial()->GetName() << G4endl; 
  if (DebugTPCMass) G4cout << " mass="<<logicTarget1->GetMass()/kg   <<" kg" << G4endl;  	 
  G4cout << " - position: ( " 
	 << GetTargetPos1().x()/mm << ", "
	 << GetTargetPos1().y()/mm << ", "
	 << GetTargetPos1().z()/mm << " ) mm"  
	 << G4endl << G4endl;
  
  //------------------------------ 
  // Target 2 - Upstream - Water 
  //------------------------------

  const G4String cNameSolidTarget2     = cParentNameTarget+"/target2";
  const G4String cNameLogicTarget2     = cParentNameTarget+"/Target2";
  const G4String cNamePhysiTarget2     = cParentNameTarget+"/Target2";
  
  //const G4String cTargetMater2  = "FGDScintillator";  
  const G4String cTargetMater2 = "WAGASHIScintillatorEmpty";  
  TargetMater2  = FindMaterial(cTargetMater2);  

  G4double targetSizeLength2  = 0.5 * GetTargetFullLength2();    // Half length of the Target 1
  G4double targetSizeWidth2   = 0.5 * GetTargetFullWidth2();     // Half width of the Target 1
  G4double targetSizeHeight2  = 0.5 * GetTargetFullHeight2();    // Half height of the Target 1

  G4double HalfTargetHeight2  = 0.5 * GetTargetFullHeight2();
  
  if( ND280XMLInput->GetXMLTargetdefault2() ){ // default
    G4double Target2_Z = -GetLengthForwTPC()/2. - GetTargetFullLength2()/2.;
    SetTargetPos2(0,0,Target2_Z);
  }
  else { // from XML file
    G4double x = ND280XMLInput->GetXMLTargetPos2_X();
    G4double y = ND280XMLInput->GetXMLTargetPos2_Y();
    G4double z = ND280XMLInput->GetXMLTargetPos2_Z();
    SetTargetPos2(x,y,z);
  }
  
  solidTarget2 = new G4Box(cNameSolidTarget2,targetSizeWidth2,targetSizeHeight2,targetSizeLength2);
  logicTarget2 = new G4LogicalVolume(solidTarget2,TargetMater2,cNameLogicTarget2,0,0,0);
  physiTarget2 = new G4PVPlacement(0,                 // no rotation
				   GetTargetPos2(),    // at (x,y,z)
				   logicTarget2,       // its logical volume  	  
				   cNamePhysiTarget2,  // its name
				   logicTracker,      // its mother  volume
				   //logicWorld,      // its mother  volume
				   false,             // no boolean operations
				   0);                 // copy number 
    //fCheckOverlaps);   

  G4cout << "Target 2: " << endl
	 << " - dimensions: "
	 << GetTargetFullWidth2()/mm  << " (width) x " 
  	 << GetTargetFullHeight2()/mm << " (height) x " 
  	 << GetTargetFullLength2()/mm << " (length) mm^3" 
         << " of " << logicTarget2->GetMaterial()->GetName() << G4endl; 
  if (DebugTPCMass) G4cout << " mass="<<logicTarget2->GetMass()/kg   <<" kg" << G4endl;
  G4cout << " - position: ( " 
	 << GetTargetPos2().x()/mm << ", "
	 << GetTargetPos2().y()/mm << ", "
	 << GetTargetPos2().z()/mm << " ) mm"  
	 << G4endl << G4endl;

  
  //
  //------------------------------ 
  // Side TPCs 
  //------------------------------
  // 

  const G4String cSideTPCMater = "GasMixtureTPC"; // gas mixture of ND280 TPCs
  SideTPCMater = FindMaterial(cSideTPCMater);
  
  G4double HalfSideTPCLength1 = 0.5 * GetSideTPCFullLength1(); 
  G4double HalfSideTPCWidth1  = 0.5 * GetSideTPCFullWidth1(); 
  G4double HalfSideTPCHeight1 = 0.5 * GetSideTPCFullHeight1(); 

  G4double HalfSideTPCLength2 = 0.5 * GetSideTPCFullLength2(); 
  G4double HalfSideTPCWidth2  = 0.5 * GetSideTPCFullWidth2(); 
  G4double HalfSideTPCHeight2 = 0.5 * GetSideTPCFullHeight2(); 
  
  // TPC Up 1

  const G4String cNameSolidSideTPCUp1   = cParentNameTPC+"/tpcup1";
  const G4String cNameLogicSideTPCUp1   = cParentNameTPC+"/TPCUp1";
  const G4String cNamePhysiSideTPCUp1   = cParentNameTPC+"/TPCUp1";

  solidSideTPCUp1 = new G4Box(cNameSolidSideTPCUp1, HalfSideTPCWidth1, HalfSideTPCHeight1, HalfSideTPCLength1); 
  logicSideTPCUp1 = new G4LogicalVolume(solidSideTPCUp1,SideTPCMater,cNameLogicSideTPCUp1,0,0,0);
  
  if( ND280XMLInput->GetXMLSideTPCdefault1() ){ // default
    SetSideTPCUpPos1(0,
		     HalfSideTPCHeight1 + HalfTargetHeight1,
		     GetTargetPos1().z()
		     );
  }
  else { // from XML file
    G4double x = ND280XMLInput->GetXMLSideTPCUpPos1_X();
    G4double y = ND280XMLInput->GetXMLSideTPCUpPos1_Y();
    G4double z = ND280XMLInput->GetXMLSideTPCUpPos1_Z();
    SetSideTPCUpPos1(x,y,z);
  }
  
  physiSideTPCUp1 = new G4PVPlacement(0,                   // no rotation
				      GetSideTPCUpPos1(),   // at (x,y,z)
				      logicSideTPCUp1,      // its logical volume 
				      cNamePhysiSideTPCUp1, // its name
				      logicTracker,        // its mother  volume
				      false,               // no boolean operations
				      0);                   // copy number 
  
  G4cout << "Side TPC Up 1: " << endl
	 << " - dimensions: "
	 << GetSideTPCFullWidth1()/mm  << " (width) x " 
  	 << GetSideTPCFullHeight1()/mm << " (height) x " 
  	 << GetSideTPCFullLength1()/mm << " (length) mm^3" 
         << " of " << logicSideTPCUp1->GetMaterial()->GetName() << G4endl; 
  if (DebugTPCMass) G4cout << " mass="<<logicSideTPCUp1->GetMass()/kg   <<" kg" << G4endl;
  G4cout << " - position: ( " 
	 << GetSideTPCUpPos1().x()/mm << ", "
	 << GetSideTPCUpPos1().y()/mm << ", "
	 << GetSideTPCUpPos1().z()/mm << " ) mm"  
	 << G4endl << G4endl;

  
  // TPC Down 1
  
  const G4String cNameSolidSideTPCDown1 = cParentNameTPC+"/tpcdown1";
  const G4String cNameLogicSideTPCDown1 = cParentNameTPC+"/TPCDown1";
  const G4String cNamePhysiSideTPCDown1 = cParentNameTPC+"/TPCDown1";
  
  solidSideTPCDown1 = new G4Box(cNameSolidSideTPCDown1,HalfSideTPCWidth1,HalfSideTPCHeight1,HalfSideTPCLength1); 
  logicSideTPCDown1 = new G4LogicalVolume(solidSideTPCDown1,SideTPCMater,cNameLogicSideTPCDown1,0,0,0);
  
  if( ND280XMLInput->GetXMLSideTPCdefault1() ){ // default
    SetSideTPCDownPos1(0,
		       -(HalfSideTPCHeight1 + HalfTargetHeight1),
		       GetTargetPos1().z()
		       );
  }
  else { // from XML file
    G4double x = ND280XMLInput->GetXMLSideTPCDownPos1_X();
    G4double y = ND280XMLInput->GetXMLSideTPCDownPos1_Y();
    G4double z = ND280XMLInput->GetXMLSideTPCDownPos1_Z();
    SetSideTPCDownPos1(x,y,z);
  }
  
  physiSideTPCDown1 = new G4PVPlacement(0,              // no rotation
					GetSideTPCDownPos1(),   // at (x,y,z)
					logicSideTPCDown1,      // its logical volume
					cNamePhysiSideTPCDown1, // its name
					logicTracker,      // its mother  volume
					false,           // no boolean operations
					0);              // copy number 

  G4cout << "Side TPC Down 1: " << endl
	 << " - dimensions: "
	 << GetSideTPCFullWidth1()/mm  << " (width) x " 
  	 << GetSideTPCFullHeight1()/mm << " (height) x " 
  	 << GetSideTPCFullLength1()/mm << " (length) mm^3" 
         << " of " << logicSideTPCDown1->GetMaterial()->GetName() << G4endl;
  if (DebugTPCMass) G4cout << " mass="<<logicSideTPCDown1->GetMass()/kg   <<" kg" << G4endl;
  G4cout << " - position: ( " 
	 << GetSideTPCDownPos1().x()/mm << ", "
	 << GetSideTPCDownPos1().y()/mm << ", "
	 << GetSideTPCDownPos1().z()/mm << " ) mm"  
	 << G4endl << G4endl;
  

  // TPC Up 2
  
  const G4String cNameSolidSideTPCUp2   = cParentNameTPC+"/tpcup2";
  const G4String cNameLogicSideTPCUp2   = cParentNameTPC+"/TPCUp2";
  const G4String cNamePhysiSideTPCUp2   = cParentNameTPC+"/TPCUp2";
  
  solidSideTPCUp2 = new G4Box(cNameSolidSideTPCUp2, HalfSideTPCWidth2, HalfSideTPCHeight2, HalfSideTPCLength2); 
  logicSideTPCUp2 = new G4LogicalVolume(solidSideTPCUp2,SideTPCMater,cNameLogicSideTPCUp2,0,0,0);
  
  if( ND280XMLInput->GetXMLSideTPCdefault2() ){ // default
    SetSideTPCUpPos2(0,
		     HalfSideTPCHeight2 + HalfTargetHeight2,
		     GetTargetPos2().z()
		     );
  }
  else { // from XML file
    G4double x = ND280XMLInput->GetXMLSideTPCUpPos2_X();
    G4double y = ND280XMLInput->GetXMLSideTPCUpPos2_Y();
    G4double z = ND280XMLInput->GetXMLSideTPCUpPos2_Z();
    SetSideTPCUpPos2(x,y,z);
  }

  physiSideTPCUp2 = new G4PVPlacement(0,                   // no rotation
				      GetSideTPCUpPos2(),   // at (x,y,z)
				      logicSideTPCUp2,      // its logical volume
				      cNamePhysiSideTPCUp2, // its name
				      logicTracker,        // its mother  volume
				      false,               // no boolean operations
				      0);                   // copy number 

  G4cout << "Side TPC Up 2: " << endl
	 << " - dimensions: "
	 << GetSideTPCFullWidth2()/mm  << " (width) x " 
  	 << GetSideTPCFullHeight2()/mm << " (height) x " 
  	 << GetSideTPCFullLength2()/mm << " (length) mm^3" 
         << " of " << logicSideTPCUp2->GetMaterial()->GetName() << G4endl; 
  if (DebugTPCMass) G4cout << " mass="<<logicSideTPCUp2->GetMass()/kg   <<" kg" << G4endl;
  G4cout << " - position: ( " 
	 << GetSideTPCUpPos2().x()/mm << ", "
	 << GetSideTPCUpPos2().y()/mm << ", "
	 << GetSideTPCUpPos2().z()/mm << " ) mm"  
	 << G4endl << G4endl;

  // TPC Down 2
  
  const G4String cNameSolidSideTPCDown2 = "/tpcdown2";
  const G4String cNameLogicSideTPCDown2 = "/TPCDown2";
  const G4String cNamePhysiSideTPCDown2 = "/TPCDown2";

  solidSideTPCDown2 = new G4Box(cNameSolidSideTPCDown2,HalfSideTPCWidth2,HalfSideTPCHeight2,HalfSideTPCLength2); 
  logicSideTPCDown2 = new G4LogicalVolume(solidSideTPCDown2,SideTPCMater,cNameLogicSideTPCDown2,0,0,0);
  
  if( ND280XMLInput->GetXMLSideTPCdefault2() ){ // default
    SetSideTPCDownPos2(0,
		       -(HalfSideTPCHeight2 + HalfTargetHeight2),
		       GetTargetPos2().z()
		       );
  }
  else { // from XML file
    G4double x = ND280XMLInput->GetXMLSideTPCDownPos2_X();
    G4double y = ND280XMLInput->GetXMLSideTPCDownPos2_Y();
    G4double z = ND280XMLInput->GetXMLSideTPCDownPos2_Z();
    SetSideTPCDownPos2(x,y,z);
  }

  physiSideTPCDown2 = new G4PVPlacement(0,              // no rotation
					GetSideTPCDownPos2(), // at (x,y,z)
					logicSideTPCDown2,    // its logical volume
					cNamePhysiSideTPCDown2,       // its name
					logicTracker,      // its mother  volume
					false,           // no boolean operations
					0);              // copy number 
  
  G4cout << "Side TPC Down 2: " << endl
	 << " - dimensions: "
	 << GetSideTPCFullWidth2()/mm  << " (width) x " 
  	 << GetSideTPCFullHeight2()/mm << " (height) x " 
  	 << GetSideTPCFullLength2()/mm << " (length) mm^3" 
         << " of " << logicSideTPCDown2->GetMaterial()->GetName() << G4endl;
  if (DebugTPCMass) G4cout << " mass="<<logicSideTPCDown2->GetMass()/kg   <<" kg" << G4endl;
  G4cout << " - position: ( " 
	 << GetSideTPCDownPos2().x()/mm << ", "
	 << GetSideTPCDownPos2().y()/mm << ", "
	 << GetSideTPCDownPos2().z()/mm << " ) mm"  
	 << G4endl << G4endl;

	 
  //------------------------------------------------ 
  // Set regions
  //------------------------------------------------ 

#ifdef USE_PAI
  G4Region* driftRegion = G4RegionStore::GetInstance()->
    GetRegion("driftRegion",false);
  if (driftRegion) {
    driftRegion->AddRootLogicalVolume(logicSideTPCUp1);
    driftRegion->AddRootLogicalVolume(logicSideTPCUp2);
    driftRegion->AddRootLogicalVolume(logicSideTPCDown1);
    driftRegion->AddRootLogicalVolume(logicSideTPCDown2);
  } else {
    G4ExceptionDescription msg;
    msg << "The drift region does not exist" << G4endl;
    G4Exception("ExN02DetectorConstruction::Construct",
		"MyCode0002",FatalException, msg);
  }
#endif
  
  G4Region* SDRegion = G4RegionStore::GetInstance()->
    GetRegion("SDRegion",false);
  if (SDRegion) {
    SDRegion->AddRootLogicalVolume(logicSideTPCUp1);
    SDRegion->AddRootLogicalVolume(logicSideTPCUp2);
    SDRegion->AddRootLogicalVolume(logicSideTPCDown1);
    SDRegion->AddRootLogicalVolume(logicSideTPCDown2);
    SDRegion->AddRootLogicalVolume(logicTarget1);
    SDRegion->AddRootLogicalVolume(logicTarget2);
  } else {
    G4ExceptionDescription msg;
    msg << "The SD region does not exist" << G4endl;
    G4Exception("ExN02DetectorConstruction::Construct",
		"MyCode0002",FatalException, msg);
  }
  

  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 
  
  logicSideTPCUp1->SetSensitiveDetector( GetSensitiveDetector() );
  logicSideTPCDown1->SetSensitiveDetector( GetSensitiveDetector() );
  logicSideTPCUp2->SetSensitiveDetector( GetSensitiveDetector() );
  logicSideTPCDown2->SetSensitiveDetector( GetSensitiveDetector() );
  logicTarget1->SetSensitiveDetector( GetSensitiveDetector() );
  logicTarget2->SetSensitiveDetector( GetSensitiveDetector() );
  
  // Construct the field creator - this will register the field it creates
  if (!fEmFieldSetup.Get()) {
    ExN02FieldSetup* fieldSetup
      = new ExN02FieldSetup(G4ThreeVector( cMagFieldX, 0.0, 0.0 ) );
    G4AutoDelete::Register(fieldSetup); // Kernel will delete the ExN02FieldSetup
    fEmFieldSetup.Put(fieldSetup);
  }
  
  //--------- Visualization attributes ------------------------------- 

  logicWorld  ->SetVisAttributes(BoxVisAtt);  
  logicBasket ->SetVisAttributes(BoxVisAtt);
  logicTracker->SetVisAttributes(BoxVisAtt);

  logicTarget1 ->SetVisAttributes(TargetScintVisAtt);
  logicTarget2 ->SetVisAttributes(TargetWaterVisAtt);


  logicSideTPCUp1->SetVisAttributes(TPCVisAtt);
  logicSideTPCUp2->SetVisAttributes(TPCVisAtt);
  logicSideTPCDown1->SetVisAttributes(TPCVisAtt);
  logicSideTPCDown2->SetVisAttributes(TPCVisAtt);


  // //--------- Set Step Limiter -------------------------------
  // //
  // // NO EFFECT BECAUSE NOT USED IN THE MAIN 
  // //
  // // Sets a max Step length in the drift region, with G4StepLimiter 
  // // Each step limiter must be attached to each physics list in ExN02PhysicsList
  // //logicSideTPCUp  ->SetUserLimits(new G4UserLimits(cStepLimit));
  // //logicSideTPCDown->SetUserLimits(new G4UserLimits(cStepLimit));



  // //------------------------------------------------------------------
  // // // writing the geometry to a ROOT file
  // // // initialize ROOT
  // // TSystem ts;
  // // gSystem->Load("libExN02ClassesDict");  
  // // //  ROOT::Cintex::Cintex::SetDebug(2);
  // // ROOT::Cintex::Cintex::Enable();
  // // //  gDebug = 1;
  // // const G4ElementTable* eltab = G4Element::GetElementTable();
  // // const G4MaterialTable* mattab = G4Material::GetMaterialTable();
  // // ExN02GeoTree* geotree = new ExN02GeoTree(fExperimentalHall_phys, eltab, mattab);
  // // TFile fo("geo.root","RECREATE");
  // // fo.WriteObject(geotree, "my_geo");  
  // //--------------------------

  
  if(ND280XMLInput->GetXMLStoreGeometry()){
    
    // // ---------------------------------------------------------------------------
    // // VGM demo 
    // // Export geometry in Root and save it in a file
    // // TGeoManager::Import("geometry.root");
    // // gGeoManager->GetTopVolume()->Draw();
    // // Import Geant4 geometry to VGM
    // Geant4GM::Factory g4Factory;
    // g4Factory.SetDebug(0);
    // g4Factory.Import(physiWorld);
    // // 
    // // Export VGM geometry to Root
    // RootGM::Factory rtFactory;
    // rtFactory.SetDebug(0);
    // g4Factory.Export(&rtFactory);
    // gGeoManager->CloseGeometry();
    // gGeoManager->SetName("ND280Geometry"); // TGeoManager object name must be "ND280Geometry" to be read by NEUT
    // gGeoManager->Export("geometry_vgm.root");
    // G4cout << "The geometry has been exported to: ";
    // G4cout << gGeoManager->GetName() << G4endl;    
    // // end VGM demo
    // // ---------------------------------------------------------------------------
    
    ND280RootGeometryManager::Get()->Update(physiWorld,true);
    ND280RootGeometryManager::Get()->Export("geometry.root");
  }

  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void ExN02DetectorConstruction::setMaterial_Target1(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
    {TargetMater1 = pttoMaterial;
      logicTarget1->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The target 1 is " << GetTargetFullLength1()/mm << " mm of "
             << materialName << G4endl;
    }             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02DetectorConstruction::setMaterial_Target2(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
    {TargetMater2 = pttoMaterial;
      logicTarget2->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The target 2 is " << GetTargetFullLength2()/mm << " mm of "
             << materialName << G4endl;
    }             
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02DetectorConstruction::setMaterial_SideTPC(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
    {SideTPCMater = pttoMaterial;
      logicSideTPCUp1->SetMaterial(pttoMaterial); 
      logicSideTPCUp2->SetMaterial(pttoMaterial); 
      logicSideTPCDown1->SetMaterial(pttoMaterial); 
      logicSideTPCDown2->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The side TPCs are " << GetSideTPCFullWidth1()/mm << " mm of "
             << materialName << G4endl;
      G4cout << "\n---->              and " << GetSideTPCFullWidth2()/mm << " mm of "
             << materialName << G4endl;
    }             
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
//void ExN02DetectorConstruction::SetMagField(G4double fieldValue) // OLD
//{
//fpMagField->SetMagFieldValue(fieldValue);  
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02DetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((stepLimit)&&(maxStep>0.)) stepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02DetectorConstruction::DefineMaterials() {

  // Method copied from ND280 software

  ND280RootGeometryManager* gMan = ND280RootGeometryManager::Get();
  
  G4double a, density;
  G4String name, symbol;
  G4double temperature, pressure;
  G4int nel, ncomponents, natoms;
  G4double fractionmass;

  G4NistManager* nistMan = G4NistManager::Instance();

  G4Element* elH = nistMan->FindOrBuildElement(1);
  G4Element* elB = nistMan->FindOrBuildElement(5);
  G4Element* elC = nistMan->FindOrBuildElement(6);
  G4Element* elN = nistMan->FindOrBuildElement(7);
  G4Element* elO = nistMan->FindOrBuildElement(8);
  G4Element* elF = nistMan->FindOrBuildElement(9);
  G4Element* elNa = nistMan->FindOrBuildElement(11);
  G4Element* elAl = nistMan->FindOrBuildElement(13);
  G4Element* elSi = nistMan->FindOrBuildElement(14);
  //G4Element* elCl = nistMan->FindOrBuildElement(17);
  G4Element* elAr = nistMan->FindOrBuildElement(18);
  //G4Element* elTi = nistMan->FindOrBuildElement(22);
  G4Element* elFe = nistMan->FindOrBuildElement(26);
  G4Element* elCo = nistMan->FindOrBuildElement(27);
  G4Element* elCu = nistMan->FindOrBuildElement(29);
  //G4Element* elZn = nistMan->FindOrBuildElement(30);
  //G4Element* elSn = nistMan->FindOrBuildElement(50);
  //G4Element* elPb = nistMan->FindOrBuildElement(82);

#ifdef Define_Vacuum
  //Vacuum (set as default material)
  density     = universe_mean_density;
  pressure    = 3.e-18*pascal;
  temperature = 2.73*kelvin;
  vacuum = new G4Material(name="Vacuum", z=1., a=1.01*g/mole,
			  density,kStateGas,temperature,pressure);
  gMan->SetColor(vacuum->GetName(),-1); // ND280 class
#endif

  //Air
  density = 1.29*mg/cm3;
  pressure    = 1*atmosphere;
  temperature = 293.15*kelvin;
  G4Material* air =  new G4Material(name="Air", density,
				    nel=2,kStateGas,temperature,
				    pressure);
  air->AddElement(elN, fractionmass = 70*perCent);
  air->AddElement(elO, fractionmass = 30*perCent);
  fDefaultMaterial = air;
  gMan->SetDrawAtt(air,kGray+3); // ND280 class

  // //Earth
  // density = 2.15*g/cm3;
  // G4Material* earth = new G4Material(name="Earth", density, nel=2);
  // earth->AddElement(elSi, natoms=1);
  // earth->AddElement(elO, natoms=2);
  // gMan->SetDrawAtt(earth,49,0.2); // ND280 class
    
  // //Cement
  // density = 2.5*g/cm3;
  // G4Material* cement = new G4Material(name="Cement", density, nel=2);
  // cement->AddElement(elSi, natoms=1);
  // cement->AddElement(elO, natoms=2);
  // gMan->SetDrawAtt(cement,14,0.2); // ND280 class

  //Water
  density = 1.0*g/cm3;
  G4Material* water = new G4Material(name="Water", density, nel=2);
  water->AddElement(elH, natoms=2);
  water->AddElement(elO, natoms=1);
  gMan->SetDrawAtt(water,kBlue); // ND280 class

  //Aluminum
  density = 2.70*g/cm3;
  G4Material* aluminum = new G4Material(name="Aluminum",density, nel=1);
  aluminum->AddElement(elAl,100.*perCent);
  gMan->SetDrawAtt(aluminum,kYellow-5); // ND280 class

  // //Iron.
  // density = 7.87*g/cm3;
  // G4Material* iron = new G4Material(name="Iron",density, nel=1);
  // iron->AddElement(elFe,100.*perCent);
  // gMan->SetDrawAtt(iron,kRed+2,0.3); // ND280 class

  //Copper
  density = 8.94*g/cm3;
  G4Material* copper = new G4Material(name="Copper",density, nel=1);
  copper->AddElement(elCu,100.*perCent);
  gMan->SetDrawAtt(copper,kRed+1,0.3);// ND280 class

  // //Lead
  // density = 11.35*g/cm3;
  // G4Material* lead = new G4Material(name="Lead",density, nel=1);
  // lead->AddElement(elPb,100.*perCent);
  // gMan->SetDrawAtt(lead,kGray+1);// ND280 class

  // //Brass -- The density is from simetric.co.uk is 8400 -- 8730 gm/cm3
  // density = 8.50*g/cm3;
  // G4Material* brass = new G4Material(name="Brass", density, nel=2);
  // brass->AddElement(elCu, fractionmass = 90*perCent);
  // brass->AddElement(elZn, fractionmass = 10*perCent);
  // gMan->SetDrawAtt(brass,kOrange-2);// ND280 class

  // //Bronze -- The density is from simetric.co.uk is 7700 -- 8920 gm/cm3
  // density = 8.5*g/cm3;
  // G4Material* bronze = new G4Material(name="Bronze", density, nel=2);
  // bronze->AddElement(elCu, fractionmass = 60*perCent);
  // bronze->AddElement(elSn, fractionmass = 40*perCent);
  // gMan->SetDrawAtt(bronze,kOrange-3);// ND280 class

  //Stainless Steel.  The density is taken from average 304 grade SS.
  density = 8.0*g/cm3;
  G4Material* steel = new G4Material(name="Steel", density, nel=3);
  steel->AddElement(elC,  fractionmass =  4*perCent);
  steel->AddElement(elFe, fractionmass = 88*perCent);
  steel->AddElement(elCo, fractionmass =  8*perCent);
  gMan->SetDrawAtt(steel,kBlue-10);// ND280 class

  //Argon
  density     = 1.66*mg/cm3;    
  pressure    = 1*atmosphere;
  temperature = 293.15*kelvin;
  G4Material* argon =  new G4Material(name="Ar", density,
				      nel=1,kStateGas,temperature,
				      pressure);
  argon->AddElement(elAr, natoms=1);
  gMan->SetDrawAtt(argon,kBlue-10,0.1);// ND280 class

  //Methane
  density     = 0.667*mg/cm3;
  pressure    = 1*atmosphere;
  temperature = 293.15*kelvin;
  G4Material* methane = new G4Material(name="CH4", density,
				       nel=2,kStateGas,temperature,
				       pressure);
  methane->AddElement(elC, natoms=1);
  methane->AddElement(elH, natoms=4);
  gMan->SetDrawAtt(methane,kBlue-10,0.1);// ND280 class

  //Argon + 10% Methane
  density     = 2.33*mg/cm3;
  G4Material* gasMixture 
    = new G4Material(name="GasMixture", density, ncomponents=2);
  gasMixture->AddMaterial(argon, fractionmass = 90*perCent);
  gasMixture->AddMaterial(methane, fractionmass = 10*perCent);
  gMan->SetDrawAtt(gasMixture,kBlue-10,0.1);// ND280 class

  // CarbonDioxide
  density     = 1.828*mg/cm3;
  pressure    = 1.*atmosphere;
  temperature = 293.15*kelvin;
  G4Material* CO2 = new G4Material(name="CO2", density,
				   nel=2,kStateGas,temperature,
				   pressure);
  CO2->AddElement(elC, natoms=1);
  CO2->AddElement(elO, natoms=2);
  gMan->SetDrawAtt(CO2,kBlue-10,0.1);// ND280 class

  // CarbonTetrafluoride
  density     = 3.66*mg/cm3;
  pressure    = 1.*atmosphere;
  temperature = 293.15*kelvin;
  G4Material* CF4 = new G4Material(name="CF4", density,
				   nel=2,kStateGas,temperature,
				   pressure);
  CF4->AddElement(elC, natoms=1);
  CF4->AddElement(elF, natoms=4);
  gMan->SetDrawAtt(CF4,kBlue-10,0.1);// ND280 class

  // Isobutane
  density     = 2.47*mg/cm3;
  pressure    = 1.*atmosphere;
  temperature = 293.15*kelvin;
  G4Material* C4H10 = new G4Material(name="C410", density,
                                     nel=2,kStateGas,temperature,
                                     pressure);
  C4H10->AddElement(elC, natoms=4);
  C4H10->AddElement(elH, natoms=10);
  gMan->SetDrawAtt(C4H10,kBlue-10,0.1);// ND280 class

  // // Ar-CO2
  // density     = 1.958*mg/cm3;
  // G4Material* gasMixtureArCO2
  //   = new G4Material(name="GasMixtureArCO2", density, ncomponents=2);
  // gasMixtureArCO2->AddMaterial(argon, fractionmass = 90*perCent);
  // gasMixtureArCO2->AddMaterial(CO2, fractionmass = 10*perCent);
  // gMan->SetDrawAtt(gasMixtureArCO2,kBlue-10,0.1);// ND280 class

  // TPC Gas
#ifdef OLD_TPC_DENSITY
  density     = 1.485*mg/cm3;
#else
  // Gas density calculated by R. Openshaw [See bug 402 for a full
  // discussion].  The density is calculated for a Ar/CF4/iBu 95:03:02
  // mixture at Normal Pressure and Temperature (NTP).
  density     = 1.738*mg/cm3;
#endif
  G4Material* gasMixtureTPC
    = new G4Material(name="GasMixtureTPC", density, ncomponents=3);
  gasMixtureTPC->AddMaterial(argon, fractionmass = 95*perCent);
  gasMixtureTPC->AddMaterial(CF4, fractionmass = 3*perCent);
  gasMixtureTPC->AddMaterial(C4H10, fractionmass = 2*perCent);
  gMan->SetDrawAtt(gasMixtureTPC,kBlue-10,0.1);// ND280 class

  // Water-SSteel-Air Mixture
  density     = 2.646*g/cm3;
  G4Material* waterSystem
    = new G4Material(name="WaterSystem", density, ncomponents=3);
  waterSystem->AddMaterial(water, fractionmass = 18*perCent);
  waterSystem->AddMaterial(steel, fractionmass = 32*perCent);
  waterSystem->AddMaterial(CO2, fractionmass = 50*perCent);
  gMan->SetDrawAtt(waterSystem,kBlue-7);// ND280 class
    
  // add TPC field cage mixture NB rough guesses !!!!
  density = 0.221*g/cm3; // this gives 1.4 10-2 X0 for 2.2 cm
  a = 16.*g/mole;
  G4Material* tpcFieldCage 
    = new G4Material(name="TPCWallMaterial",8,a,density);
  gMan->SetDrawAtt(tpcFieldCage,kYellow-7);// ND280 class
    
  // // Titanium Dioxide -- Used in coextruded scintillator.
  // density     = 4.26*g/cm3;
  // G4Material* TIO2 = new G4Material(name="TIO2", density, nel=2);
  // TIO2->AddElement(elTi, natoms=1);
  // TIO2->AddElement(elO , natoms=2);
  // gMan->SetDrawAtt(TIO2,kWhite);// ND280 class
    
  // Polystyrene -- This is polystyrene defined in the PDG C6H5CH=CH2 (this
  // is a net C8H8)
  density = 1.050*g/cm3; // the density of the "extruded polystyrene"
  G4Material* polystyrene 
    = new G4Material(name="Polystyrene", density, nel=2);
  polystyrene->AddElement(elC, 8);
  polystyrene->AddElement(elH, 8);
  gMan->SetDrawAtt(polystyrene,kGray+3);// ND280 class
    
  // Scintillator -- This is the average polystyrene plastic scintillator as
  // defined in the PDG C6H5CH=CH2 (this is a net C8H8).  The SMRD and ECal
  // geometries have been tuned to this value.  The 1.05 value represents a
  // typical extruded polystyrene.
  density = 1.050*g/cm3; 
  G4Material* scintillator 
    = new G4Material(name="Scintillator", density, nel=2);
  scintillator->AddElement(elC, 8);
  scintillator->AddElement(elH, 8);
  gMan->SetDrawAtt(scintillator,kGreen);// ND280 class

  // // P0D Scintillator -- This has the average polystyrene plastic
  // // scintillator composition as defined in the PDG C6H5CH=CH2 (this is a
  // // net C8H8).  The density of the P0D scintillator is taken from the assay
  // // of the MINERvA scintillator which is manufactured on the same equipment
  // // as the P0D scintillator and is chemically the same.  The measured
  // // density of the MINERvA strips are 1.058 +- 0.003, including the Ti02
  // // coating.  This corresponds to a scintillator density of 1.0506, which
  // // has been rounded to 1.051
  // density = 1.051*g/cm3; 
  // G4Material* p0dScintillator 
  //   = new G4Material(name="P0DScintillator", density, nel=2);
  // p0dScintillator->AddElement(elC, 8);
  // p0dScintillator->AddElement(elH, 8);
  // gMan->SetDrawAtt(p0dScintillator,kGreen);// ND280 class

  // // FGD Scintillator -- This is the average polystyrene scintillator for
  // // the FGD.  The FGD density is based on our measurements of individual
  // // bars, in combination with the measurements of the cross-sectional area
  // // of the bars.
  // density = 1.032*g/cm3; 
  // G4Material* fgdScintillator 
  //   = new G4Material(name="FGDScintillator", density, nel=2);
  // fgdScintillator->AddElement(elC, 8);
  // fgdScintillator->AddElement(elH, 8);
  // gMan->SetDrawAtt(fgdScintillator,kGreen);// ND280 class

  // // Scintillator coating.  This is the coating that goes around the average
  // // plastic scintillator. It is 15% TiO2 and 85% polystyrene by weight.
  // // rho = (m_Sc + m_Ti) / (V_Sc + V_Ti)
  // //     = (0.85 + 0.15) / ( 0.85/1.032 + 0.15/4.26 )
  // //     = 1.164 g/cm^3
  // density = 1.164*g/cm3;
  // G4Material* scintillatorCoating
  //   = new G4Material(name="ScintillatorCoating", density, ncomponents=2);
  // scintillatorCoating->AddMaterial(TIO2        ,fractionmass = 15*perCent);
  // scintillatorCoating->AddMaterial(scintillator,fractionmass = 85*perCent);
  // gMan->SetDrawAtt(scintillatorCoating,kGreen);// ND280 class
    
  // // PVC -- Polyvinyl Chloride CH2=CHCl = C3H3Cl
  // density = 1.38*g/cm3;
  // G4Material* pvc
  //   = new G4Material(name="PVC", density, nel=3);
  // pvc->AddElement(elC, 3);
  // pvc->AddElement(elH, 3);
  // pvc->AddElement(elCl, 1);
  // gMan->SetDrawAtt(pvc,kGray+1);// ND280 class

  // // HDPE -- High Density Polyethylene used in P0D water bag
  // density = 0.94*g/cm3;
  // G4Material* hdpe
  //   = new G4Material(name="HDPE", density, nel=2);
  // hdpe->AddElement(elC, natoms=1);
  // hdpe->AddElement(elH, natoms=2);
  // gMan->SetDrawAtt(hdpe,kCyan-10);// ND280 class
    
  // // P0DuleEpoxy -- The Epoxy used to hold the P0Dule together.  Density is
  // // from the Henkel Hysol data sheet.
  // density = 1.36*g/cm3;
  // G4Material* p0duleEpoxy
  //   = new G4Material(name="P0DuleEpoxy", density, nel=3);
  // p0duleEpoxy->AddElement(elC, 3);
  // p0duleEpoxy->AddElement(elH, 3);
  // p0duleEpoxy->AddElement(elCl, 1);
  // gMan->SetDrawAtt(p0duleEpoxy,kBlue+4);// ND280 class
    
  // // FGD Glue
  // density = 1.365*g/cm3;
  // G4Material* fgdGlue
  //   = new G4Material(name="FGDGlue", density, nel=4);
  // fgdGlue->AddElement(elO,fractionmass=62.5*perCent);
  // fgdGlue->AddElement(elC,fractionmass=27.9*perCent);
  // fgdGlue->AddElement(elH,fractionmass= 8.4*perCent);
  // fgdGlue->AddElement(elN,fractionmass= 1.2*perCent);
  // gMan->SetDrawAtt(fgdGlue,kOrange);// ND280 class
    
  // // FGD Water Module Epoxy
  // density = 0.6573*g/cm3;
  // G4Material* fgdWaterModuleEpoxy
  //   = new G4Material(name="FGDWaterModuleEpoxy", density, nel=4);
  // fgdWaterModuleEpoxy->AddElement(elO,fractionmass=62.5*perCent);
  // fgdWaterModuleEpoxy->AddElement(elC,fractionmass=27.9*perCent);
  // fgdWaterModuleEpoxy->AddElement(elH,fractionmass= 8.4*perCent);
  // fgdWaterModuleEpoxy->AddElement(elN,fractionmass= 1.2*perCent);
  // gMan->SetDrawAtt(fgdWaterModuleEpoxy,kOrange);// ND280 class
     
    
  // // Polypropylene
  // density = 0.9*g/cm3;
  // G4Material* polypropylene
  //   = new G4Material(name="Polypropylene", density, nel=2);
  // polypropylene->AddElement(elC, 3);
  // polypropylene->AddElement(elH, 6);
  // gMan->SetDrawAtt(polypropylene,kYellow-5);// ND280 class

  // Polycarbonate
  density = 1.2*g/cm3;
  G4Material* polycarbonate
    = new G4Material(name="Polycarbonate", density, nel=3);
  polycarbonate->AddElement(elH,fractionmass= 5.5491*perCent);
  polycarbonate->AddElement(elC,fractionmass=75.5751*perCent);
  polycarbonate->AddElement(elO,fractionmass=18.8758*perCent);
  gMan->SetDrawAtt(polycarbonate,kYellow-2);// ND280 class

  // //carbon fibre
  // // NB : chemical composition may not be completely right but  close 
  // density = 1.75*g/cm3;
  // G4Material* carbonFibre
  //   = new G4Material(name="CarbonFibre", density, nel=2);
  // carbonFibre->AddElement(elC,6);
  // carbonFibre->AddElement(elO,1);
  // gMan->SetDrawAtt(carbonFibre,kGray+3);// ND280 class

  // G10 - by volume 57% glass, 43% epoxy (CH2)
  density = 1.70*g/cm3;
  G4Material* g10
    = new G4Material(name="G10", density, nel=6);
  g10->AddElement(elH,6.2*perCent);
  g10->AddElement(elC,36.8*perCent);
  g10->AddElement(elO,30.7*perCent);
  g10->AddElement(elSi,21.9*perCent);
  g10->AddElement(elB,2.2*perCent);
  g10->AddElement(elNa,2.2*perCent);
  gMan->SetDrawAtt(g10,kGreen+3);// ND280 class

  // Diluted G10-FGD1
  density = 0.365*g/cm3;
  G4Material* g10fgd1
    = new G4Material(name="G10FGD1", density, nel=6);
  g10fgd1->AddElement(elH,6.2*perCent);
  g10fgd1->AddElement(elC,36.8*perCent);
  g10fgd1->AddElement(elO,30.7*perCent);
  g10fgd1->AddElement(elSi,21.9*perCent);
  g10fgd1->AddElement(elB,2.2*perCent);
  g10fgd1->AddElement(elNa,2.2*perCent);
  gMan->SetDrawAtt(g10fgd1,kGreen+3);// ND280 class

  // // Diluted G10-FGD2
  // density = 0.171*g/cm3;
  // G4Material* g10fgd2
  //   = new G4Material(name="G10FGD2", density, nel=6);
  // g10fgd2->AddElement(elH,6.2*perCent);
  // g10fgd2->AddElement(elC,36.8*perCent);
  // g10fgd2->AddElement(elO,30.7*perCent);
  // g10fgd2->AddElement(elSi,21.9*perCent);
  // g10fgd2->AddElement(elB,2.2*perCent);
  // g10fgd2->AddElement(elNa,2.2*perCent);
  // gMan->SetDrawAtt(g10fgd2,kGreen+3);// ND280 class

  // Rohacell (polymethacrylimide, chemical formula to be confirmed from 
  // "Polymethyl methacrylate" C5O2H8)
  density = 0.052*g/cm3;
  G4Material* rohacell 
    = new G4Material(name="Rohacell", density, nel=3);
  // Rohacell C:H:O=5:8:2
  rohacell->AddElement(elC,5);
  rohacell->AddElement(elH,8);
  rohacell->AddElement(elO,2);
  gMan->SetDrawAtt(rohacell,kGreen-9);// ND280 class
    
  // add Al/Rohacell TPC cage wall material
  // Outer box 15.2mm Al / Roha Panels (2 x 0.76mm Al, and 13.68mm Rohacell)
  // rho = (m_Al + m_Roha)/(V_Al + V_Roha)
  //     = (rho_Al * A*1.52mm + rho_Roha * A*13.68 mm)/(A*1.52mm + A*13.68mm)
  //     = (2.7g/cm3 * 1.52mm + 0.052g/cm3 * 13.68 mm) / 15.2mm
  //     = 0.3168 g/cm3

  density = 0.3168*g/cm3;

  G4Material* alroha
    = new G4Material(name="AlRoha", density, ncomponents=2);

  // Fraction by weight:
  //  w_Al = m_Al / m_total
  //       = (rho_Al*A*1.52mm) / (rho*A*15.2mm)
  //       = (2.7g/cm3*1.52mm) / (0.3168g/cm3*15.2mm)
  //       = 0.8523
  //  w_Ro = m_Roha / m_total
  //       = (rho_Roha*A*1.52mm) / (rho*A*15.2mm)
  //       = (0.052g/cm3*13.68mm) / (0.3168g/cm3*15.2mm)
  //       = 0.1477
  alroha->AddMaterial(aluminum, fractionmass = 85.23*perCent);
  alroha->AddMaterial(rohacell, fractionmass = 14.77*perCent);
  gMan->SetDrawAtt(alroha,kGreen-9);// ND280 class

  // add Al/Rohacell TPC cage wall material
  // (same as above but only 13.2mm thick)
  density = 0.3569*g/cm3;

  G4Material* alroha2
    = new G4Material(name="AlRoha2", density, ncomponents=2);

  alroha2->AddMaterial(aluminum, fractionmass = 87.11*perCent);
  alroha2->AddMaterial(rohacell, fractionmass = 12.89*perCent);
  gMan->SetDrawAtt(alroha2,kGreen-9);// ND280 class

  // G10/Roha/Cu TPC cage wall material
  //
  // Inner box 13.2mm G10 / Roha / Cu Panels (2 x 0.76mm G10, 11.68mm
  // Rohacell, and 2* 0.035mm Cu)
  //
  // Density:
  // rho = ( m_G10 + m_Roha + m_Cu ) / ( V_G10 + V_Roha +V_Cu )
  //    = ( rho_G10*A*1.52mm + rho_Roha*A*11.68mm + rho_Cu*A*0.07mm ) 
  //           / ( A*1.52mm +A*11.68mm+A*0.07mm )
  //    = ( 1.7g/cm^3*1.52mm + 0.052g/cm^3*11.68mm + 8.94g/cm^3*0.07mm) 
  //                    / 13.27mm
  //    = 0.2877 g/cm^3
  density = 0.2877*g/cm3;
  G4Material* g10roha
    = new G4Material(name="G10Roha", density, ncomponents=3);

  //Fraction by weight:
  //  w_G10 = m_G10 / m_total
  //       = (rho_G10*A*1.52mm) / (rho*A*13.27mm)
  //       = (1.7g/cm^3*1.52mm) / (0.2877g/cm^3*13.27mm)
  //       = 0.6769
  //  w_Ro = m_Roha / m_total
  //       = (rho_Roha*A*11.68mm) / (rho*A*13.27mm)
  //       = (0.052g/cm^3*11.68mm) / (0.2877g/cm^3*13.27mm)
  //       = 0.1591
  //  w_Cu = m_Cu / m_total
  //       = (rho_Cu*A*0.07mm) / (rho*A*13.27mm)
  //       = (8.94g/cm^3*0.07mm) / (0.2877g/cm^3*13.27mm)
  //       = 0.1640
  g10roha->AddMaterial(g10, fractionmass = 67.69*perCent);  
  g10roha->AddMaterial(rohacell, fractionmass = 15.91*perCent);
  g10roha->AddMaterial(copper, fractionmass = 16.40*perCent);
  gMan->SetDrawAtt(g10roha,kGreen-9);// ND280 class

  // FGD Electronics card (95mm x 252mm x 8mm) with total mass of 400g. The
  // composition is 50% Al and 50% G10 by weight.  -> density =
  // 400g/191.52cm3
  density = 2.0885*g/cm3;

  G4Material* alg10
    = new G4Material(name="AlG10", density, ncomponents=2);

  alg10->AddMaterial(aluminum, fractionmass = 50.0*perCent);
  alg10->AddMaterial(g10, fractionmass = 50.0*perCent);
  gMan->SetDrawAtt(alg10,kGreen-9);// ND280 class

  // Material for the fiber core.
  density = 1.05*g/cm3;
  G4Material* fiberCore
    = new G4Material(name="FiberCore", density, nel=2);
  fiberCore->AddElement(elC, 9);
  fiberCore->AddElement(elH, 10);
  gMan->SetDrawAtt(fiberCore,kCyan-4);// ND280 class
    
  // Material for the fiber cladding
  density = 1.19*g/cm3;
  G4Material* fiberCladding
    = new G4Material(name="FiberCladding", density, nel=3);
  fiberCladding->AddElement(elH,8);
  fiberCladding->AddElement(elC,5);
  fiberCladding->AddElement(elO,2);
  gMan->SetDrawAtt(fiberCladding,kBlue);// ND280 class

  // Fluorinated fiber cladding
  density = 1.43*g/cm3;
  G4Material* fiberCladdingFluor
    = new G4Material(name="FiberCladdingFluor", density, nel=2);
  fiberCladdingFluor->AddElement(elH,4);
  fiberCladdingFluor->AddElement(elC,2);
  gMan->SetDrawAtt(fiberCladdingFluor,kBlue);// ND280 class
    
  // // ActiveWater for the FGD.
  // //
  // // It is not necessarily activated, it is just a combination of water and
  // // polycarbonate that holds the water in place.  Daniel's document says
  // // the panel is 342mg/cm2 of PC (for most of the panels), and 2.2226 g/cm2
  // // of water (average of the 6 deployed panels).  water+PC layer by mass is
  // // 0.342/(0.342+2.2226)=0.1335 polycarbonate, so rho = 1 / (0.1335/1.2 +
  // // 0.8666/1.0) = 1.0226 g/cm^3
  // density = 1.0226*g/cm3;
  // G4Material* activeWater 
  //   = new G4Material("ActiveWater", density, ncomponents=2);
  // activeWater->AddMaterial(water        ,fractionmass = 86.672*perCent);
  // activeWater->AddMaterial(polycarbonate,fractionmass = 13.328*perCent);
  // gMan->SetDrawAtt(activeWater,kAzure+8);// ND280 class

  
  // WAGASHI
  
  //Scintillator
  density = 1.032*g/cm3;
  G4Material *WAGASHIScint 
    = new G4Material(name="WAGASHIScintillator", density, nel=2);
  WAGASHIScint->AddElement(elC, 9);
  WAGASHIScint->AddElement(elH, 10);
  gMan->SetDrawAtt(WAGASHIScint,kAzure+8);//ND280 class

  //Scintillator empty --> average density ~40% of normal one
  density = 1.032*g/cm3 * 0.4;
  G4Material *WAGASHIScint_Empty 
    = new G4Material(name="WAGASHIScintillatorEmpty", density,nel=2);
  WAGASHIScint_Empty->AddElement(elC, 9);
  WAGASHIScint_Empty->AddElement(elH, 10);
  gMan->SetDrawAtt(WAGASHIScint_Empty,kGreen);//ND280 class
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* ExN02DetectorConstruction::FindMaterial(G4String name) {
  G4Material* material = G4Material::GetMaterial(name,true);
  return material;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* ExN02DetectorConstruction::GetPieceTPC(G4String name,G4String parentname) {

  //
  // FROM INIT nd280mc
  //
  
  SetWidth(2300*mm);
  SetHeight(2400*mm);

  DebugTPCMass = true ; //set to true to get extra printouts on component masses

  //SetMaximumHitLength(10.*mm);
  //SetMaximumHitSagitta(0.5*mm);

  SetDriftWidth(1807.2*mm);                         
  SetDriftHeight(2148.0*mm); 
  SetDriftLength(772.0*mm);  
  SetCathodeThickness(13.2*mm);  
  SetCO2Top(67.8*mm);        
  SetCO2Sides(67.8*mm);     
  SetCO2Bottom(117.8*mm);   
  SetInnerBoxWall(13.2*mm); 
  SetOuterBoxWall(15.2*mm);  
  
  SetShowOuterVolume(false);
  SetSteppingLimit(1.*mm);

  fActiveTPCVerticalOffset = 25*mm;
  
  // Initialize all translations and rotations to 0.
  for (int rp = 0; rp < 2; rp++)
    {
      for (int imm = 0; imm < 12; imm++)
	{
	  tpcMMTrans[rp][imm] = G4ThreeVector(0., 0., 0.);
	  tpcMMRot[rp][imm] = new G4RotationMatrix (G4ThreeVector(0, 0, 1), 0);
	}
    }
  // Define the rotation axis to +x in TPC coordinate system (+z in the interface).
  tpcMMRotAxis = G4ThreeVector(0, 0, 1);
  
  ////////////////////



  /// Create the inner TPC drift volume
  
  //G4cout << "Create the inner TPC drift volume" << G4endl;

  double width  = GetDriftWidth()/2.;
  double height = GetDriftHeight()/2.;
  double length = GetDriftLength()/2.;
  
  //double DriftWidth = 1807.2*mm;
  //double DriftHeight = 2148.0*mm;
  //double DriftLength = 772.0*mm;
  //double width = DriftWidth / 2.;
  //double height = DriftHeight / 2.;
  //double length = DriftLength / 2.;

  G4VSolid* solidDrift = new G4Box("InnerBox",width,height,length);
  
  G4LogicalVolume* logDrift
    = new G4LogicalVolume(solidDrift,
			  FindMaterial("GasMixtureTPC"),
			  //GetName()+"/Drift");
			  //"TPC1/Drift");
			  parentname+"/"+name+"/Drift");
  
  logDrift->SetVisAttributes(TPCVisAtt);
  //logDrift->SetVisAttributes(G4VisAttributes::Invisible);


  //if (GetVisible() && !GetShowOuterVolume()) {
  //logDrift->SetVisAttributes(GetVisual());
  //}
  //else {
  //logDrift->SetVisAttributes(G4VisAttributes::Invisible);
  //}
  
  if (GetSensitiveDetector()) {
    logDrift->SetSensitiveDetector(GetSensitiveDetector());
  }
  
  //logDrift->SetUserLimits(new G4UserLimits(GetSteppingLimit()));
  


  // CO2 space

  //G4cout << "CO2 space" << G4endl;

  width  = 1150.0*mm;
  height = (1149.5*mm + 30.5*mm + 45.0*mm + 1130.0*mm + 45.0*mm)/2.;
  length = 442.0*mm + 45.0*mm;
  
  G4VSolid* solidCO2 = new G4Box("CO2Space",width,height,length);
  
  G4LogicalVolume* logGasGap
    = new G4LogicalVolume(solidCO2,
			  FindMaterial("CO2"),
			  //GetName()+"/GasGap");
			  //"TPC1/GasGap");
			  parentname+"/"+name+"/GasGap");

  logGasGap->SetVisAttributes(TPCCO2);
  logGasGap->SetVisAttributes(G4VisAttributes::Invisible);

  //if (GetVisible() && GetShowOuterVolume()) {
  //logGasGap->SetVisAttributes(GetVisual());
  //}
  //else {
  //logGasGap->SetVisAttributes(G4VisAttributes::Invisible);
  //}
  
  
  /// Create the inactive volume around the TPC.
  
  //G4cout << "Create the inactive volume around the TPC" << G4endl;

  SetLength(2*(442.0*mm+45.0*mm));
  //double length_inact = (2*(442.0*mm+45.0*mm));
  
  G4LogicalVolume* logVolume
    = new G4LogicalVolume(new G4Box(
				    //GetName(),
				    //"TPC1",
				    parentname+"/"+name,
				    GetWidthForwTPC()/2.,
				    GetHeightForwTPC()/2.,
				    GetLengthForwTPC()/2.),
			  //width/2.,
			  //height/2.,
			  //length_inact/2.),
			  FindMaterial("Air"),
			  //GetName()
			  //"TPC1"
			  parentname+"/"+name
			  );
  
  logVolume->SetVisAttributes(TPCDeadMat);
  logVolume->SetVisAttributes(G4VisAttributes::Invisible);

  //if (GetVisible() && GetShowOuterVolume()) {
  //logVolume->SetVisAttributes(GetVisual());
  //}
  //else {
  //logVolume->SetVisAttributes(G4VisAttributes::Invisible);
  //}
  
  // Now do all envelope volume placements
  
  //G4cout << "Now do all envelope volume placements" << G4endl;

  double CO2Top = 67.8*mm;
  double CO2Bottom = 117.8*mm;
  
  fActiveTPCVerticalOffset = (GetCO2Bottom() - GetCO2Top())/2;
  //fActiveTPCVerticalOffset = (CO2Bottom - CO2Top)/2;
  
  new G4PVPlacement(0,
		    G4ThreeVector(0,fActiveTPCVerticalOffset,0),
		    logDrift,
		    //GetName()+"/Drift",
		    //"TPC1/Drift",
		    parentname+"/"+name+"/Drift",
		    logGasGap,
		    false,
		    0);
  //fCheckOverlaps);

  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),
		    logGasGap,
		    //GetName()+"/GasGap",
		    //"TPC1/GasGap",
		    parentname+"/"+name+"/GasGap",
		    logVolume,
		    false,
		    0);
  //fCheckOverlaps);

  
  // Divide drift volume into two halves (readout planes)
  // Make 2 separate halves (instead of 2 copies), so that each of 24 MM can be modified.
    
  width  = GetDriftWidth()/2.;
  height = GetDriftHeight()/2.;
  length = GetDriftLength()/2.;
  //width  = DriftWidth/2.;
  //height = DriftHeight/2.;
  //length = DriftLength/2.;
  
    width -= GetCathodeThickness()/2.;
    //double CathodeThickness = 13.2*mm;
    //width -= CathodeThickness/2.;
    width /= 2.;

    G4VSolid* innerHalf= new G4Box("InnerHalf",length,height,width);
     
   
    G4LogicalVolume* logHalf0
        = new G4LogicalVolume(innerHalf,
                              FindMaterial("GasMixtureTPC"),
                              //GetName()+"/Half");
                              //"TPC1/Half");
			      parentname+"/"+name+"/Half");
    G4LogicalVolume* logHalf1
        = new G4LogicalVolume(innerHalf,
                              FindMaterial("GasMixtureTPC"),
                              //GetName()+"/Half");
                              //"TPC1/Half");
			      parentname+"/"+name+"/Half");
    logHalf0->SetVisAttributes(TPCVisAtt); //TPCDeadMat);
    logHalf1->SetVisAttributes(TPCVisAtt); //TPCDeadMat);
    //logHalf0->SetVisAttributes(G4VisAttributes::Invisible);
    //logHalf1->SetVisAttributes(G4VisAttributes::Invisible);   


    // if (GetVisible() && !GetShowOuterVolume()) {
    //    logHalf0->SetVisAttributes(GetVisual());
    //    logHalf1->SetVisAttributes(GetVisual());
    // }
    // else {
    //    logHalf0->SetVisAttributes(G4VisAttributes::Invisible);
    //    logHalf1->SetVisAttributes(G4VisAttributes::Invisible);
    // }

    if (GetSensitiveDetector()) {
      logHalf0->SetSensitiveDetector(GetSensitiveDetector());
      logHalf1->SetSensitiveDetector(GetSensitiveDetector());

      G4Region* SDRegion = G4RegionStore::GetInstance()->
	GetRegion("SDRegion",false);
      if (SDRegion) {
	SDRegion->AddRootLogicalVolume(logHalf0);
	SDRegion->AddRootLogicalVolume(logHalf1);
      } else {
	G4ExceptionDescription msg;
	msg << "The SD region does not exist" << G4endl;
	G4Exception("ExN02DetectorConstruction::GetPieceTPC",
		    "MyCode0002",FatalException, msg);
      }

#ifdef USE_PAI
      G4Region* driftRegion = G4RegionStore::GetInstance()->
	GetRegion("driftRegion",false);
      if (driftRegion) {
	driftRegion->AddRootLogicalVolume(logHalf0);
	driftRegion->AddRootLogicalVolume(logHalf1);
      } else {
	G4ExceptionDescription msg;
	msg << "The drift region does not exist" << G4endl;
	G4Exception("ExN02DetectorConstruction::GetPieceTPC",
		    "MyCode0002",FatalException, msg);
      }
#endif
    }

    
    //logHalf0->SetUserLimits(new G4UserLimits(GetSteppingLimit()));
    //logHalf1->SetUserLimits(new G4UserLimits(GetSteppingLimit()));




    double delta = width + GetCathodeThickness()/2.;
    //double delta = width + CathodeThickness/2.;
    
    G4RotationMatrix rm;
    rm.rotateY(90.0*deg);

    new G4PVPlacement(G4Transform3D(rm,G4ThreeVector(-delta,0,0)),
                      logHalf0,
                      //GetName()+"/Half",
                      //"TPC1/Half",
		      parentname+"/"+name+"/Half",
                      logDrift,
                      false,
                      0);
    //fCheckOverlaps);

    rm.rotateY(180.0*deg);    
 
    new G4PVPlacement(G4Transform3D(rm,G4ThreeVector(delta,0,0)),
                      logHalf1,
                      //GetName()+"/Half",
                      //"TPC1/Half",
		      parentname+"/"+name+"/Half",
                      logDrift,
                      false,
                      1);
    //fCheckOverlaps);

    // add 12 Micromegas Modules to readout-plane

    //G4cout << "add 12 Micromegas Modules to readout-plane" << G4endl;

    double mmWidth  = 35.29*cm;
    double mmHeight = 33.61*cm;
    double mmLength = 2.*width;

    G4LogicalVolume* logMM
      = new G4LogicalVolume(new G4Box("MicromegasModule",
				      mmWidth/2.,
				      mmHeight/2.,
				      mmLength/2.),
			    FindMaterial("GasMixtureTPC"),
			    //GetName()+"/MM");
			    //"TPC1/MM");
			    parentname+"/"+name+"/MM");
    
    //if (GetVisible() && !GetShowOuterVolume()) {
    G4VisAttributes* visual = new G4VisAttributes();
    visual->SetColor(0.1,0.6,0.1,1);
    logMM->SetVisAttributes(visual);
    //}
    //else {
    //logMM->SetVisAttributes(G4VisAttributes::Invisible);
    //}

    //if (GetSensitiveDetector()) {
    //logMM->SetSensitiveDetector(GetSensitiveDetector());
    //}

    // //logMM->SetUserLimits(new G4UserLimits(GetSteppingLimit()));
    
    int nmod = 12;

     double xmod[12] = {  189.65*mm ,  189.65*mm ,  189.65*mm ,
                          189.65*mm ,  189.65*mm ,  189.65*mm ,
                         -189.65*mm , -189.65*mm , -189.65*mm ,
                         -189.65*mm , -189.65*mm , -189.65*mm  };

     double ymod[12] = {  881.65*mm ,  538.05*mm ,  194.45*mm ,
                         -149.15*mm , -492.75*mm , -836.35*mm ,
                          836.35*mm ,  492.75*mm ,  149.15*mm ,
                         -194.45*mm , -538.05*mm , -881.65*mm  };

     // Translations and rotations get applied in this version
     // Placement in first readout plane
     for (int imod=0; imod<nmod; imod++ ) {
      
       if ( imod < 6 ) {
     	new G4PVPlacement(tpcMMRot[0][imod],
     			  G4ThreeVector(xmod[imod]-tpcMMTrans[0][imod].z(),ymod[imod]+tpcMMTrans[0][imod].y(), 0.),
     			  logMM,
     			  //GetName()+"/MM",
     			  //"TPC1/MM",
			  parentname+"/"+name+"/MM",
     			  logHalf0,
     			  false,
     			  imod);
       }
       else {
     	tpcMMRot[0][imod]->rotateZ(180.0*deg);
     	new G4PVPlacement(tpcMMRot[0][imod],
     			  G4ThreeVector(xmod[imod]-tpcMMTrans[0][imod].z(),ymod[imod]+tpcMMTrans[0][imod].y(),0.),
     			  logMM,
     			  //GetName()+"/MM",
     			  //"TPC1/MM",
			  parentname+"/"+name+"/MM",
     			  logHalf0,
     			  false,
     			  imod);
       }
     }
     // Placement in second readout plane
     for (int imod=0; imod<nmod; imod++ ) {
      
       if ( imod < 6 ) {
	 new G4PVPlacement(tpcMMRot[1][imod],
			   G4ThreeVector(xmod[imod]+tpcMMTrans[1][imod].z(),ymod[imod]+tpcMMTrans[1][imod].y(),0.),
			   logMM,
			   //GetName()+"/MM",
			   //"TPC1/MM",
			   parentname+"/"+name+"/MM",
			   logHalf1,
			   false,
			   imod);
       }
       else {
     	tpcMMRot[1][imod]->rotateZ(180.0*deg);
     	new G4PVPlacement(tpcMMRot[1][imod],
     			  G4ThreeVector(xmod[imod]+tpcMMTrans[1][imod].z(),ymod[imod]+tpcMMTrans[1][imod].y(),0.),
     			  logMM,
     			  //GetName()+"/MM",
     			  //"TPC1/MM",
			  parentname+"/"+name+"/MM",
     			  logHalf1,
     			  false,
     			  imod);
       }
     }

     if (DebugTPCMass){
       G4cout << "DriftGas (2) mass="<<logHalf0->GetMass()/kg<<" kg" << G4endl;
       G4cout << "CO2 Gap (1) mass="<<logGasGap->GetMass()/kg<<" kg" << G4endl;
       G4cout << "MM (12) mass="<<logMM->GetMass()/kg<<" kg" << G4endl;
       G4cout << "Sum of gases and MM :"<< (2*logHalf0->GetMass()+logGasGap->GetMass()+12*logMM->GetMass())/kg << " kg" << G4endl;
     }


     BuildTPCCentralCathode(logDrift);
     BuildTPCCages(logGasGap);

    return logVolume;
}



void ExN02DetectorConstruction::BuildTPCCentralCathode(G4LogicalVolume* logVolume) {

    double y, z;

    G4String parentname = logVolume->GetName();

    // (red) G10/Roha composite middle plane (central cathode)

    //ND280BeamConstructor& middleG10RoPlate = 
    //Get<ND280BeamConstructor>("CentralCathode");
    ND280BeamConstructor middleG10RoPlate(parentname+"/CentralCathode");
    middleG10RoPlate.SetWidth(GetCathodeThickness());
    middleG10RoPlate.SetHeight(2*1061.0*mm);
    middleG10RoPlate.SetLength(2*373.0*mm);

    //middleG10RoPlate.SetMaterialName("G10Roha");
    middleG10RoPlate.SetMaterial(FindMaterial("G10Roha"));

    double width  = middleG10RoPlate.GetWidth();
    double height = middleG10RoPlate.GetHeight();
    double length = middleG10RoPlate.GetLength();

    G4LogicalVolume* log_middleG10RoPlate = middleG10RoPlate.GetPiece();
    
    new G4PVPlacement(0,
                      G4ThreeVector(0,0,0),
                      log_middleG10RoPlate,
                      middleG10RoPlate.GetName(),
                      logVolume,
                      false,
                      0);

    

    if (middleG10RoPlate.GetVisible()){ // && GetShowOuterVolume()) {
      log_middleG10RoPlate->SetVisAttributes(GetVisual());
    }
    else {
      log_middleG10RoPlate->SetVisAttributes(G4VisAttributes::Invisible);
    }
    
   

    // (green) solid G10 connector

    //ND280BeamConstructor& frbaCon1 =
    //Get<ND280BeamConstructor>("FrBaCon1");

    ND280BeamConstructor frbaCon1(parentname+"/FrBaCon1");
    frbaCon1.SetWidth(GetCathodeThickness());
    frbaCon1.SetHeight(2*1074.0*mm);
    frbaCon1.SetLength(386.0*mm - 373.0*mm);

    //frbaCon1.SetMaterialName("G10");
    frbaCon1.SetMaterial(FindMaterial("G10"));

    width  = frbaCon1.GetWidth();
    height = frbaCon1.GetHeight();
    length = frbaCon1.GetLength();


    G4LogicalVolume* log_frbaCon1 = frbaCon1.GetPiece();

    int k = 0;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        z = d * (373.0*mm + length/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(0,0,z),
                          log_frbaCon1,
                          frbaCon1.GetName(),
                          logVolume,
                          false,
                          k);
         k++;
    }

    if (frbaCon1.GetVisible()) { // && GetShowOuterVolume()) {
      log_frbaCon1->SetVisAttributes(GetVisual());
    }
    else {
      log_frbaCon1->SetVisAttributes(G4VisAttributes::Invisible);
    }


    //ND280BeamConstructor& toboCon1 =
    //Get<ND280BeamConstructor>("ToBoCon1");

    ND280BeamConstructor toboCon1(parentname+"/ToBoCon1");

    toboCon1.SetWidth(GetCathodeThickness());
    toboCon1.SetHeight(386.0*mm - 373.0*mm);
    toboCon1.SetLength(2*373.0*mm);

    //toboCon1.SetMaterialName("G10");
    toboCon1.SetMaterial(FindMaterial("G10"));

    width  = toboCon1.GetWidth();
    height = toboCon1.GetHeight();
    length = toboCon1.GetLength();

    G4LogicalVolume* log_toboCon1 = toboCon1.GetPiece();

    k = 0;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        y = d * (1074.0*mm - height/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(0,y,0),
                          log_toboCon1,
                          toboCon1.GetName(),
                          logVolume,
                          false,
                          k);
	k++;
    }
    
    if (toboCon1.GetVisible()) { // && GetShowOuterVolume()) {
      log_toboCon1->SetVisAttributes(GetVisual());
    }
    else {
      log_toboCon1->SetVisAttributes(G4VisAttributes::Invisible);
    }

    if (DebugTPCMass){
      G4cout << "CentralCathode (1) mass="<<log_middleG10RoPlate->GetMass()/kg<<" kg" << G4endl;
      G4cout << "FrBaCon1 (2) mass="<<log_frbaCon1->GetMass()/kg<<" kg" << G4endl;
      G4cout << "ToBoCon1 (2) mass="<<log_toboCon1->GetMass()/kg<<" kg" << G4endl;
      G4cout << "Sum of central cathode parts: "<<(log_middleG10RoPlate->GetMass()+2*log_frbaCon1->GetMass()+2*log_toboCon1->GetMass())/kg<<" kg" << G4endl;
    }

}






void ExN02DetectorConstruction::BuildTPCCages(G4LogicalVolume* logVolume) {
  
    double x, y, z;

    G4String parentname = logVolume->GetName();

    // (green) solid G10 side plates

    //ND280BeamConstructor& sideG10Plate = 
    //Get<ND280BeamConstructor>("SideG10Plate");
    ND280BeamConstructor sideG10Plate(parentname+"/SideG10Plate");
    
    // was sideG10Plate.SetWidth(27.0*mm) - change suggested by Doug Storey
    sideG10Plate.SetWidth(47.29*mm);
    sideG10Plate.SetHeight(2*1085.6*mm);
    sideG10Plate.SetLength(2*397.6*mm);

    //sideG10Plate.SetMaterialName("G10");
    sideG10Plate.SetMaterial(FindMaterial("G10"));

    double width  = sideG10Plate.GetWidth();
    double height = sideG10Plate.GetHeight();
    double length = sideG10Plate.GetLength();

    G4LogicalVolume* log_sideG10Plate = sideG10Plate.GetPiece();

    int k = 0;
    y = GetActiveTPCVerticalOffset();
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (869.1*mm + 35.0*mm + width/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_sideG10Plate,
                          sideG10Plate.GetName(),
                          logVolume,
                          false,
                          k);
        k++;
    }

    if (sideG10Plate.GetVisible()) { // && GetShowOuterVolume()) {
      log_sideG10Plate->SetVisAttributes(GetVisual());
    }
    else {
      log_sideG10Plate->SetVisAttributes(G4VisAttributes::Invisible);
    }



   

    // (green) solid G10 frame pieces

    //ND280BeamConstructor& verticalG10Frame =
    //Get<ND280BeamConstructor>("VerticalG10Frame");
    ND280BeamConstructor verticalG10Frame(parentname+"/VerticalG10Frame");

    verticalG10Frame.SetWidth(35.0*mm);
    verticalG10Frame.SetHeight(2*(1074.0*mm + 47.6*mm));
    verticalG10Frame.SetLength(47.6*mm);

    //verticalG10Frame.SetMaterialName("G10");
    verticalG10Frame.SetMaterial(FindMaterial("G10"));

    width  = verticalG10Frame.GetWidth();
    height = verticalG10Frame.GetHeight();
    length = verticalG10Frame.GetLength();

    G4LogicalVolume* log_verticalG10Frame = verticalG10Frame.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (869.1*mm + width/2.);
        for (int j=0; j<2; j++) {
            double e = pow(-1,j);
            z = e * (386.0*mm + length/2.);
            new G4PVPlacement(0,
                              G4ThreeVector(x,y,z),
                              log_verticalG10Frame,
                              verticalG10Frame.GetName(),
                              logVolume,
                              false,
                              k);
            k++;
        }
    }

    if (verticalG10Frame.GetVisible()) { // && GetShowOuterVolume()) {
      log_verticalG10Frame->SetVisAttributes(GetVisual());
    }
    else {
      log_verticalG10Frame->SetVisAttributes(G4VisAttributes::Invisible);
    }


   

    //ND280BeamConstructor& horizontalG10Frame =
    //Get<ND280BeamConstructor>("HorizontalG10Frame");

    ND280BeamConstructor horizontalG10Frame(parentname+"/HorizontalG10Frame");
    horizontalG10Frame.SetWidth(35.0*mm);
    horizontalG10Frame.SetHeight(47.6*mm);
    horizontalG10Frame.SetLength(2*386.0*mm);

    //horizontalG10Frame.SetMaterialName("G10");
    horizontalG10Frame.SetMaterial(FindMaterial("G10"));

    width  = horizontalG10Frame.GetWidth();
    height = horizontalG10Frame.GetHeight();
    length = horizontalG10Frame.GetLength();

    G4LogicalVolume* log_horizontalG10Frame = horizontalG10Frame.GetPiece();

    k = 0;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (869.1*mm + width/2.);
        for (int j=0; j<2; j++) {
            double e = pow(-1,j);
            y = e * (1074.0*mm + height/2.) + GetActiveTPCVerticalOffset();
            new G4PVPlacement(0,
                              G4ThreeVector(x,y,0),
                              log_horizontalG10Frame,
                              horizontalG10Frame.GetName(),
                              logVolume,
                              false,
                              k);
            k++;
        }
    }

    if (horizontalG10Frame.GetVisible()) { // && GetShowOuterVolume()) {
      log_horizontalG10Frame->SetVisAttributes(GetVisual());
    }
    else {
      log_horizontalG10Frame->SetVisAttributes(G4VisAttributes::Invisible);
    }


    // (green) solid G10 vertical plates

    //ND280BeamConstructor& verticalG10Plate =
    //Get<ND280BeamConstructor>("VerticalG10Plate");

    ND280BeamConstructor verticalG10Plate(parentname+"/VerticalG10Plate"); 

    verticalG10Plate.SetWidth(160.0*mm);
    verticalG10Plate.SetHeight(2*1085.6*mm);
    verticalG10Plate.SetLength(12.7*mm);

    //verticalG10Plate.SetMaterialName("G10");
    verticalG10Plate.SetMaterial(FindMaterial("G10"));

    width  = verticalG10Plate.GetWidth();
    height = verticalG10Plate.GetHeight();
    length = verticalG10Plate.GetLength();

    G4LogicalVolume* log_verticalG10Plate = verticalG10Plate.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (961.8*mm + width/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_verticalG10Plate,
                          verticalG10Plate.GetName(),
                          logVolume,
                          false,
                          k);
        k++;
    }
    
    if (verticalG10Plate.GetVisible()) { // && GetShowOuterVolume()) {
      log_verticalG10Plate->SetVisAttributes(GetVisual());
    }
    else {
      log_verticalG10Plate->SetVisAttributes(G4VisAttributes::Invisible);
    }


    // (black) solid aluminum side plates

    //ND280BeamConstructor& sideAlPlate =
    //Get<ND280BeamConstructor>("SideAlPlate");
    ND280BeamConstructor sideAlPlate(parentname+"/SideAlPlate");

    // was sideAlPlate.SetWidth(20.0*mm) change suggested by Doug Storey
    sideAlPlate.SetWidth(4.84*mm);
    sideAlPlate.SetHeight(2*1020.9*mm);
    sideAlPlate.SetLength(375.2*mm - 21.4*mm);

    width  = sideAlPlate.GetWidth();
    height = sideAlPlate.GetHeight();
    length = sideAlPlate.GetLength();

    sideAlPlate.SetMaterial(FindMaterial("Aluminum"));

    G4LogicalVolume* log_sideAlPlate = sideAlPlate.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (1007.2*mm + width/2.);
        for (int j=0; j<2; j++) {
            double e = pow(-1,j);
            z = e * (21.4*mm + length/2.);
            new G4PVPlacement(0,
                              G4ThreeVector(x,y,z),
                              log_sideAlPlate,
                              sideAlPlate.GetName(),
                              logVolume,
                              false,
                              k);
            k++;
        }
    }

    if (sideAlPlate.GetVisible()) { // && GetShowOuterVolume()) {
      log_sideAlPlate->SetVisAttributes(GetVisual());
    }
    else {
      log_sideAlPlate->SetVisAttributes(G4VisAttributes::Invisible);
    }

    // (yellow) Water System Posts

    //ND280BeamConstructor& post = Get<ND280BeamConstructor>("Post");
    ND280BeamConstructor post(parentname+"/Post");

    post.SetWidth(50.0*mm);
    post.SetHeight(1068.2*mm + 1149.5*mm);
    post.SetLength(50.0*mm);

    //post.SetMaterialName("WaterSystem");
    post.SetMaterial(FindMaterial("WaterSystem"));

    width  = post.GetWidth();
    height = post.GetHeight();
    length = post.GetLength();

    G4LogicalVolume* log_post = post.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += -1149.5*mm + height/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (1060.4*mm + width/2.);
        for (int j=0; j<2; j++) {
            double e = pow(-1,j);
            z = e * (401.7*mm + length/2.);
            new G4PVPlacement(0,
                              G4ThreeVector(x,y,z),
                              log_post,
                              post.GetName(),
                              logVolume,
                              false,
                              k);
            k++;
        }
    }

    if (post.GetVisible()) { // && GetShowOuterVolume()) {
      log_post->SetVisAttributes(GetVisual());
    }
    else {
      log_post->SetVisAttributes(G4VisAttributes::Invisible);
    }
    
    // (light blue) Alum/Roha composite side plates

    //ND280BeamConstructor& sideAlRoPlate =
    //Get<ND280BeamConstructor>("SideAlRoPlate");
    ND280BeamConstructor sideAlRoPlate(parentname+"/SideAlRoPlate");

    sideAlRoPlate.SetWidth(13.2*mm);
    sideAlRoPlate.SetHeight(1146.8*mm + 1149.5*mm + 47.3*mm);
    sideAlRoPlate.SetLength(2*421.0*mm);

    //sideAlRoPlate.SetMaterialName("AlRoha2");
    sideAlRoPlate.SetMaterial(FindMaterial("AlRoha2"));

    width  = sideAlRoPlate.GetWidth();
    height = sideAlRoPlate.GetHeight();
    length = sideAlRoPlate.GetLength();

    G4LogicalVolume* log_sideAlRoPlate = sideAlRoPlate.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += (1146.8*mm - (1149.5*mm + 47.3*mm))/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (1150.0*mm - width/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_sideAlRoPlate,
                          sideAlRoPlate.GetName(),
                          logVolume,
                          false,
                          k);
        k++;
    }

    if (sideAlRoPlate.GetVisible()) { // && GetShowOuterVolume()) {
      log_sideAlRoPlate->SetVisAttributes(GetVisual());
    }
    else {
      log_sideAlRoPlate->SetVisAttributes(G4VisAttributes::Invisible);
    }

    // (light blue) Alum/Roha composite front/back plates
    
    //ND280BeamConstructor& frbaAlRoPlate =
    //Get<ND280BeamConstructor>("FrBaAlRoPlate");
    ND280BeamConstructor frbaAlRoPlate(parentname+"/FrBaAlRoPlate");

    frbaAlRoPlate.SetWidth(895.3*mm-18.0*mm);
    frbaAlRoPlate.SetHeight(1155.0*mm + 15.2*mm + 1205.0*mm + 15.2*mm);
    frbaAlRoPlate.SetLength(15.2*mm);

    //frbaAlRoPlate.SetMaterialName("AlRoha");
    frbaAlRoPlate.SetMaterial(FindMaterial("AlRoha"));

    width  = frbaAlRoPlate.GetWidth();
    height = frbaAlRoPlate.GetHeight();
    length = frbaAlRoPlate.GetLength();

    G4LogicalVolume* log_frbaAlRoPlate = frbaAlRoPlate.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += ((1155.0*mm + 15.2*mm) - (1205.0*mm + 15.2*mm))/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        z = d * (467.0*mm + length/2.);
        for (int j=0; j<2; j++) {
            double e = pow(-1,j);
            x = e * (18.0*mm + width/2.);
            new G4PVPlacement(0,
                              G4ThreeVector(x,y,z),
                              log_frbaAlRoPlate,
                              frbaAlRoPlate.GetName(),
                              logVolume,
                              false,
                              k);
            k++;
        }
    }

    if (frbaAlRoPlate.GetVisible()) { // && GetShowOuterVolume()) {
       log_frbaAlRoPlate->SetVisAttributes(GetVisual());
    }
    else {
       log_frbaAlRoPlate->SetVisAttributes(G4VisAttributes::Invisible);
    }
    
    
      
    // (light blue) Alum/Roha composite top/bottom plates
    
    //ND280BeamConstructor& toboAlRoPlate =
    //Get<ND280BeamConstructor>("ToBoAlRoPlate");
    ND280BeamConstructor toboAlRoPlate(parentname+"/ToBoAlRoPlate");

    toboAlRoPlate.SetWidth(895.3*mm - 18.0*mm);
    toboAlRoPlate.SetHeight(15.2*mm);
    toboAlRoPlate.SetLength(2*467.0*mm);

    //toboAlRoPlate.SetMaterialName("AlRoha");
    toboAlRoPlate.SetMaterial(FindMaterial("AlRoha"));

    width  = toboAlRoPlate.GetWidth();
    height = toboAlRoPlate.GetHeight();
    length = toboAlRoPlate.GetLength();

    G4LogicalVolume* log_toboAlRoPlate = toboAlRoPlate.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += 1155.0*mm + height/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (18.0*mm + width/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_toboAlRoPlate,
                          toboAlRoPlate.GetName(),
                          logVolume,
                          false,
                          k);
         k++;
    }
    y = GetActiveTPCVerticalOffset();
    y += -1205.0 - height/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (18.0*mm + width/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_toboAlRoPlate,
                          toboAlRoPlate.GetName(),
                          logVolume,
                          false,
                          k);
         k++;
    }

    if (toboAlRoPlate.GetVisible()) { // && GetShowOuterVolume()) {
       log_toboAlRoPlate->SetVisAttributes(GetVisual());
    }
    else {
       log_toboAlRoPlate->SetVisAttributes(G4VisAttributes::Invisible);
    }

    // (red) G10/Roha composite front/back plates

    //ND280BeamConstructor& frbaG10RoPlate =
    //Get<ND280BeamConstructor>("FrBaG10RoPlate");
    ND280BeamConstructor frbaG10RoPlate(parentname+"/FrBaG10RoPlate");
    
    frbaG10RoPlate.SetWidth(869.1*mm - 18.1*mm);
    frbaG10RoPlate.SetHeight(2*(1061.0*mm + 13.2*mm));
    frbaG10RoPlate.SetLength(13.2*mm);

    //frbaG10RoPlate.SetMaterialName("G10Roha");
    frbaG10RoPlate.SetMaterial(FindMaterial("G10Roha"));

    width  = frbaG10RoPlate.GetWidth();
    height = frbaG10RoPlate.GetHeight();
    length = frbaG10RoPlate.GetLength();

    G4LogicalVolume* log_frbaG10RoPlate = frbaG10RoPlate.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        z = d * (386.0*mm + length/2.);
        for (int j=0; j<2; j++) {
            double e = pow(-1,j);
            x = e * (18.1*mm + width/2.);
            new G4PVPlacement(0,
                              G4ThreeVector(x,y,z),
                              log_frbaG10RoPlate,
                              frbaG10RoPlate.GetName(),
                              logVolume,
                              false,
                              k);
            k++;
        }
    }

    if (frbaG10RoPlate.GetVisible()) { // && GetShowOuterVolume()) {
      log_frbaG10RoPlate->SetVisAttributes(GetVisual());
    }
    else {
      log_frbaG10RoPlate->SetVisAttributes(G4VisAttributes::Invisible);
    }

    // (red) G10/Roha composite top/bottom plates

    //ND280BeamConstructor& toboG10RoPlate =
    //Get<ND280BeamConstructor>("ToBoG10RoPlate");
    ND280BeamConstructor toboG10RoPlate(parentname+"/ToBoG10RoPlate");

    toboG10RoPlate.SetWidth(869.1*mm - 18.1*mm);
    toboG10RoPlate.SetHeight(13.2*mm);
    toboG10RoPlate.SetLength(2*386.0*mm);

    //toboG10RoPlate.SetMaterialName("G10Roha");
    toboG10RoPlate.SetMaterial(FindMaterial("G10Roha"));

    width  = toboG10RoPlate.GetWidth();
    height = toboG10RoPlate.GetHeight();
    length = toboG10RoPlate.GetLength();

    G4LogicalVolume* log_toboG10RoPlate = toboG10RoPlate.GetPiece();

    k = 0;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        y = d * (1074.0*mm + height/2.) + GetActiveTPCVerticalOffset();
        for (int j=0; j<2; j++) {
            double e = pow(-1,j);
            x = e * (18.1*mm + width/2.);
            new G4PVPlacement(0,
                              G4ThreeVector(x,y,0),
                              log_toboG10RoPlate,
                              toboG10RoPlate.GetName(),
                              logVolume,
                              false,
                              k);
            k++;
        }
    }

    if (toboG10RoPlate.GetVisible()) { // && GetShowOuterVolume()) {
      log_toboG10RoPlate->SetVisAttributes(GetVisual());
    }
    else {
      log_toboG10RoPlate->SetVisAttributes(G4VisAttributes::Invisible);
    }

    // (black) Aluminum front/back courner pieces

    //ND280BeamConstructor& frbaPiece1 =
    //Get<ND280BeamConstructor>("FrBaPiece1");
    ND280BeamConstructor frbaPiece1(parentname+"/FrBaPiece1");

    frbaPiece1.SetWidth(40.7*mm);
    frbaPiece1.SetHeight(1130.0*mm + 45.0*mm + 1149.5*mm + 30.5*mm + 45.0*mm);
    frbaPiece1.SetLength(45.0*mm);

    // In nd280mc code it's Aluminum by default
    frbaPiece1.SetMaterial(FindMaterial("Aluminum"));

    width  = frbaPiece1.GetWidth();
    height = frbaPiece1.GetHeight();
    length = frbaPiece1.GetLength();

    G4LogicalVolume* log_frbaPiece1 = frbaPiece1.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += ((1130.0*mm + 45.0*mm) - (1149.5*mm + 30.5*mm + 45.0*mm))/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        z = d * (442.0*mm + length/2.);
        for (int j=0; j<2; j++) {
            double e = pow(-1,j);
            x = e * (895.3*mm + width/2.);
            new G4PVPlacement(0,
                              G4ThreeVector(x,y,z),
                              log_frbaPiece1,
                              frbaPiece1.GetName(),
                              logVolume,
                              false,
                              k);
            k++;
        }
    }

    if (frbaPiece1.GetVisible()) { // && GetShowOuterVolume()) {
      log_frbaPiece1->SetVisAttributes(GetVisual());
    }
    else {
      log_frbaPiece1->SetVisAttributes(G4VisAttributes::Invisible);
    }


    //ND280BeamConstructor& frbaPiece2 =
    //Get<ND280BeamConstructor>("FrBaPiece2");
    ND280BeamConstructor frbaPiece2(parentname+"/FrBaPiece2");

    frbaPiece2.SetWidth(29.7*mm);
    frbaPiece2.SetHeight(1170*mm + 1220.0*mm);
    frbaPiece2.SetLength(45.8*mm);

    width  = frbaPiece2.GetWidth();
    height = frbaPiece2.GetHeight();
    length = frbaPiece2.GetLength();

    // In nd280mc code it's Aluminum by default
    frbaPiece2.SetMaterial(FindMaterial("Aluminum"));

    G4LogicalVolume* log_frbaPiece2 = frbaPiece2.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += (1170.0*mm - 1220.0*mm)/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        z = d * (421.0*mm + length/2.);
        for (int j=0; j<2; j++) {
            double e = pow(-1,j);
            x = e * (1150.0*mm - width/2.);
            new G4PVPlacement(0,
                              G4ThreeVector(x,y,z),
                              log_frbaPiece2,
                              frbaPiece2.GetName(),
                              logVolume,
                              false,
                              k);
            k++;
        }
    }

    if (frbaPiece2.GetVisible()) { // && GetShowOuterVolume()) {
      log_frbaPiece2->SetVisAttributes(GetVisual());
    }
    else {
      log_frbaPiece2->SetVisAttributes(G4VisAttributes::Invisible);
    }
    
    
    //ND280BeamConstructor& frbaPiece3 =
    //Get<ND280BeamConstructor>("FrBaPiece3");
    ND280BeamConstructor frbaPiece3(parentname+"/FrBaPiece3");

    frbaPiece3.SetWidth((1150.0*mm-29.7*mm) - (895.3*mm+40.7*mm));
    frbaPiece3.SetHeight(1146.8*mm + 7.9*mm + 1149.5*mm + 7.9*mm);
    frbaPiece3.SetLength(7.9*mm);

    width  = frbaPiece3.GetWidth();
    height = frbaPiece3.GetHeight();
    length = frbaPiece3.GetLength();

    // In nd280mc code it's Aluminum by default
    frbaPiece3.SetMaterial(FindMaterial("Aluminum"));

    G4LogicalVolume* log_frbaPiece3 = frbaPiece3.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += ((1146.8*mm + 7.9*mm) - (1149.5*mm + 7.9*mm))/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        z = d * (421.0*mm + 45.8*mm - length/2.);
        for (int j=0; j<2; j++) {
            double e = pow(-1,j);
            x = e * (895.3*mm + 40.7*mm + width/2.);
            new G4PVPlacement(0,
                              G4ThreeVector(x,y,z),
                              log_frbaPiece3,
                              frbaPiece3.GetName(),
                              logVolume,
                              false,
                              k);
            k++;
        }
    }

    if (frbaPiece3.GetVisible()) { // && GetShowOuterVolume()) {
      log_frbaPiece3->SetVisAttributes(GetVisual());
    }
    else {
      log_frbaPiece3->SetVisAttributes(G4VisAttributes::Invisible);
    }

    // (black) Aluminum bottom courner pieces

    //ND280BeamConstructor& boPiece1 =
    //Get<ND280BeamConstructor>("BoPiece1");
    ND280BeamConstructor boPiece1(parentname+"/BoPiece1");
    
    boPiece1.SetWidth(40.7*mm);
    boPiece1.SetHeight(45.0*mm);
    boPiece1.SetLength(2*442.0*mm);

    width  = boPiece1.GetWidth();
    height = boPiece1.GetHeight();
    length = boPiece1.GetLength();

    // In nd280mc code it's Aluminum by default
    boPiece1.SetMaterial(FindMaterial("Aluminum"));

    G4LogicalVolume* log_boPiece1 = boPiece1.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += -1149.5*mm - 30.5*mm - height/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (895.3*mm + width/2);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_boPiece1,
                          boPiece1.GetName(),
                          logVolume,
                          false,
                          k);
        k++;
    }
    
    if (boPiece1.GetVisible()) { // && GetShowOuterVolume()) {
      log_boPiece1->SetVisAttributes(GetVisual());
    }
    else {
      log_boPiece1->SetVisAttributes(G4VisAttributes::Invisible);
    }

    //ND280BeamConstructor& boPiece2 =
    //Get<ND280BeamConstructor>("BoPiece2");
    ND280BeamConstructor boPiece2(parentname+"/BoPiece2");
    
    boPiece2.SetWidth(29.7*mm);
    boPiece2.SetHeight(23.2*mm);
    boPiece2.SetLength(2*421.0*mm);

    width  = boPiece2.GetWidth();
    height = boPiece2.GetHeight();
    length = boPiece2.GetLength();

    // In nd280mc code it's Aluminum by default
    boPiece2.SetMaterial(FindMaterial("Aluminum"));

    G4LogicalVolume* log_boPiece2 = boPiece2.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += -1149.5*mm - 47.3*mm - height/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (1150.0*mm - width/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_boPiece2,
                          boPiece2.GetName(),
                          logVolume,
                          false,
                          k);
        k++;
    }
 
    if (boPiece2.GetVisible()) { // && GetShowOuterVolume()) {
      log_boPiece2->SetVisAttributes(GetVisual());
    }
    else {
      log_boPiece2->SetVisAttributes(G4VisAttributes::Invisible);
    }

    //ND280BeamConstructor& boPiece3 =
    //Get<ND280BeamConstructor>("BoPiece3");
    ND280BeamConstructor boPiece3(parentname+"/BoPiece3");

    boPiece3.SetWidth((1150.0*mm-29.7*mm) - (895.3*mm+40.7*mm));
    boPiece3.SetHeight(7.9*mm);
    boPiece3.SetLength(2*(421.0*mm + 45.8*mm - 7.9*mm));

    width  = boPiece3.GetWidth();
    height = boPiece3.GetHeight();
    length = boPiece3.GetLength();

    // In nd280mc code it's Aluminum by default
    boPiece3.SetMaterial(FindMaterial("Aluminum"));

    G4LogicalVolume* log_boPiece3 = boPiece3.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += -1149.5*mm - height/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (895.3*mm + 40.7*mm + width/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_boPiece3,
                          boPiece3.GetName(),
                          logVolume,
                          false,
                          k);
        k++;
    }

    if (boPiece3.GetVisible()) { // && GetShowOuterVolume()) {
      log_boPiece3->SetVisAttributes(GetVisual());
    }
    else {
      log_boPiece3->SetVisAttributes(G4VisAttributes::Invisible);
    }

    //ND280BeamConstructor& boPiece4 =
    //Get<ND280BeamConstructor>("BoPiece4");
    ND280BeamConstructor boPiece4(parentname+"/BoPiece4");

    boPiece4.SetWidth(9.5*mm);
    boPiece4.SetHeight(30.5*mm);
    boPiece4.SetLength(2*421.0*mm);

    width  = boPiece4.GetWidth();
    height = boPiece4.GetHeight();
    length = boPiece4.GetLength();

    // In nd280mc code it's Aluminum by default
    boPiece4.SetMaterial(FindMaterial("Aluminum"));

    G4LogicalVolume* log_boPiece4 = boPiece4.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += -1149.5*mm - height/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (895.3*mm + 40.7*mm - width/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_boPiece4,
                          boPiece4.GetName(),
                          logVolume,
                          false,
                          k);
        k++;
    }

    if (boPiece4.GetVisible()) { // && GetShowOuterVolume()) {
      log_boPiece4->SetVisAttributes(GetVisual());
    }
    else {
      log_boPiece4->SetVisAttributes(G4VisAttributes::Invisible);
    }

    //ND280BeamConstructor& boPiece5 =
    //Get<ND280BeamConstructor>("BoPiece5");
    ND280BeamConstructor boPiece5(parentname+"/BoPiece5");

    boPiece5.SetWidth(16.5*mm);
    boPiece5.SetHeight(47.3*mm);
    boPiece5.SetLength(2*421.0*mm);

    width  = boPiece5.GetWidth();
    height = boPiece5.GetHeight();
    length = boPiece5.GetLength();

    // In nd280mc code it's Aluminum by default
    boPiece5.SetMaterial(FindMaterial("Aluminum"));
    
    G4LogicalVolume* log_boPiece5 = boPiece5.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += -1149.5 - height/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (1150.0*mm - 29.7*mm + width/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_boPiece5,
                          boPiece5.GetName(),
                          logVolume,
                          false,
                          k);
        k++;
    }

    if (boPiece5.GetVisible()) { // && GetShowOuterVolume()) {
      log_boPiece5->SetVisAttributes(GetVisual());
    }
    else {
      log_boPiece5->SetVisAttributes(G4VisAttributes::Invisible);
    }

    // (black) Aluminum top courner pieces

    //ND280BeamConstructor& toPiece1 =
    //Get<ND280BeamConstructor>("ToPiece1");
    ND280BeamConstructor toPiece1(parentname+"/ToPiece");

    toPiece1.SetWidth(40.7*mm);
    toPiece1.SetHeight(45.0*mm);
    toPiece1.SetLength(2*442.0*mm);

    width  = toPiece1.GetWidth();
    height = toPiece1.GetHeight();
    length = toPiece1.GetLength();

    // In nd280mc code it's Aluminum by default
    toPiece1.SetMaterial(FindMaterial("Aluminum"));

    G4LogicalVolume* log_toPiece1 = toPiece1.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += 1130.0*mm + height/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (895.3*mm + width/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_toPiece1,
                          toPiece1.GetName(),
                          logVolume,
                          false,
                          k);
        k++;
    }

    if (toPiece1.GetVisible()) { // && GetShowOuterVolume()) {
      log_toPiece1->SetVisAttributes(GetVisual());
    }
    else {
      log_toPiece1->SetVisAttributes(G4VisAttributes::Invisible);
    }

    //ND280BeamConstructor& toPiece2 =
    //Get<ND280BeamConstructor>("ToPiece2");
    ND280BeamConstructor toPiece2(parentname+"/ToPiece2");
    
    toPiece2.SetWidth(29.7*mm);
    toPiece2.SetHeight(1170.0*mm - 1146.8*mm);
    toPiece2.SetLength(2*421.0*mm);

    width  = toPiece2.GetWidth();
    height = toPiece2.GetHeight();
    length = toPiece2.GetLength();

    // In nd280mc code it's Aluminum by default
    toPiece2.SetMaterial(FindMaterial("Aluminum"));

    G4LogicalVolume* log_toPiece2 = toPiece2.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += 1146.8*mm + height/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (1150.0*mm - width/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_toPiece2,
                          toPiece2.GetName(),
                          logVolume,
                          false,
                          k);
        k++;
    }

    if (toPiece2.GetVisible()) { // && GetShowOuterVolume()) {
       log_toPiece2->SetVisAttributes(GetVisual());
    }
    else {
       log_toPiece2->SetVisAttributes(G4VisAttributes::Invisible);
    }

    //ND280BeamConstructor& toPiece3 =
    //Get<ND280BeamConstructor>("ToPiece3");
    ND280BeamConstructor toPiece3(parentname+"/ToPiece3");

    toPiece3.SetWidth((1150.0*mm-29.7*mm) - (895.3*mm+40.7*mm));
    toPiece3.SetHeight(7.9*mm);
    toPiece3.SetLength(2*(421.0*mm + 45.8*mm - 7.9*mm));

    width  = toPiece3.GetWidth();
    height = toPiece3.GetHeight();
    length = toPiece3.GetLength();

    // In nd280mc code it's Aluminum by default
    toPiece3.SetMaterial(FindMaterial("Aluminum"));

    G4LogicalVolume* log_toPiece3 = toPiece3.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += 1146.8*mm + height/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        x = d * (895.3*mm + 40.7*mm + width/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(x,y,0),
                          log_toPiece3,
                          toPiece3.GetName(),
                          logVolume,
                          false,
                          k);
        k++;
    }

    if (toPiece3.GetVisible()) { // && GetShowOuterVolume()) {
      log_toPiece3->SetVisAttributes(GetVisual());
    }
    else {
      log_toPiece3->SetVisAttributes(G4VisAttributes::Invisible);
    }

    // (green) solid G10 connector

    //ND280BeamConstructor& frbaCon2 =
    //Get<ND280BeamConstructor>("FrBaCon2");
    ND280BeamConstructor frbaCon2(parentname+"/FrBaCon2");

    frbaCon2.SetWidth(2*18.1*mm);
    frbaCon2.SetHeight(2*(1074.0*mm + 13.2*mm));
    frbaCon2.SetLength(13.2*mm);

    //frbaCon2.SetMaterialName("G10");
    frbaCon2.SetMaterial(FindMaterial("G10"));

    width  = frbaCon2.GetWidth();
    height = frbaCon2.GetHeight();
    length = frbaCon2.GetLength();

    G4LogicalVolume* log_frbaCon2 = frbaCon2.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        z = d * (386.0*mm + length/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(0,y,z),
                          log_frbaCon2,
                          frbaCon2.GetName(),
                          logVolume,
                          false,
                          k);
         k++;
    }

    if (frbaCon2.GetVisible()) { // && GetShowOuterVolume()) {
      log_frbaCon2->SetVisAttributes(GetVisual());
    }
    else {
      log_frbaCon2->SetVisAttributes(G4VisAttributes::Invisible);
    }
    
    //ND280BeamConstructor& toboCon2 =
    //Get<ND280BeamConstructor>("ToBoCon2");
    ND280BeamConstructor toboCon2(parentname+"/ToBoCon2");
    
    toboCon2.SetWidth(2*18.1*mm);
    toboCon2.SetHeight(13.2*mm);
    toboCon2.SetLength(2*(373.0*mm +(386.0*mm - 373.0*mm)));

    //toboCon2.SetMaterialName("G10");
    toboCon2.SetMaterial(FindMaterial("G10"));

    width  = toboCon2.GetWidth();
    height = toboCon2.GetHeight();
    length = toboCon2.GetLength();

    G4LogicalVolume* log_toboCon2 = toboCon2.GetPiece();

    k = 0;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        y = d * (1074.0*mm + height/2.) + GetActiveTPCVerticalOffset();
        new G4PVPlacement(0,
                          G4ThreeVector(0,y,0),
                          log_toboCon2,
                          toboCon2.GetName(),
                          logVolume,
                          false,
                          k);
         k++;
    }
 
    if (toboCon2.GetVisible()) { // && GetShowOuterVolume()) {
      log_toboCon2->SetVisAttributes(GetVisual());
    }
    else {
      log_toboCon2->SetVisAttributes(G4VisAttributes::Invisible);
    }
    
    // (black) solid aluminum connector
    
    //ND280BeamConstructor& frbaCon =
    //Get<ND280BeamConstructor>("FrBaCon");
    ND280BeamConstructor frbaCon(parentname+"/FrBaCon");

    frbaCon.SetWidth(2*18.0*mm);
    frbaCon.SetHeight(1155.0*mm + 15.2*mm + 1205.0*mm + 15.2*mm);
    frbaCon.SetLength(15.2*mm);

    width  = frbaCon.GetWidth();
    height = frbaCon.GetHeight();
    length = frbaCon.GetLength();

    // In nd280mc code it's Aluminum by default
    frbaCon.SetMaterial(FindMaterial("Aluminum"));

    G4LogicalVolume* log_frbaCon = frbaCon.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += ((1155.0*mm + 15.2*mm) - (1205.0*mm + 15.2*mm))/2.;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        z = d * (467.0*mm + length/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(0,y,z),
                          log_frbaCon,
                          frbaCon.GetName(),
                          logVolume,
                          false,
                          k);
         k++;
    }
 
    if (frbaCon.GetVisible()) { // && GetShowOuterVolume()) {
      log_frbaCon->SetVisAttributes(GetVisual());
    }
    else {
      log_frbaCon->SetVisAttributes(G4VisAttributes::Invisible);
    }

    //ND280BeamConstructor& toboCon =
    //Get<ND280BeamConstructor>("ToBoCon");
    ND280BeamConstructor toboCon(parentname+"/ToBoCon");

    toboCon.SetWidth(2*18.0*mm);
    toboCon.SetHeight(15.2*mm);
    toboCon.SetLength(2*467.0);

    width  = toboCon.GetWidth();
    height = toboCon.GetHeight();
    length = toboCon.GetLength();

    // In nd280mc code it's Aluminum by default
    toboCon.SetMaterial(FindMaterial("Aluminum"));

    G4LogicalVolume* log_toboCon = toboCon.GetPiece();

    k = 0;
    y = GetActiveTPCVerticalOffset();
    y += 1155.0 + height/2.;
    new G4PVPlacement(0,
                      G4ThreeVector(0,y,0),
                      log_toboCon,
                      toboCon.GetName(),
                      logVolume,
                      false,
                      k);

    k++;

    y = GetActiveTPCVerticalOffset();
    y += -1205.0 - height/2.;
    new G4PVPlacement(0,
                      G4ThreeVector(0,y,0),
                      log_toboCon,
                      toboCon.GetName(),
                      logVolume,
                      false,
                      k);

    if (toboCon.GetVisible()) { // && GetShowOuterVolume()) {
      log_toboCon->SetVisAttributes(GetVisual());
    }
    else {
      log_toboCon->SetVisAttributes(G4VisAttributes::Invisible);
    }
    
    if (DebugTPCMass){
      G4cout << "SideG10Plate        (2) mass="<<log_sideG10Plate->GetMass()/kg       <<" kg" << G4endl;
      G4cout << "VerticalG10Frame    (4) mass="<<log_verticalG10Frame->GetMass()/kg   <<" kg" << G4endl;
      G4cout << "HorizontallG10Frame (4) mass="<<log_horizontalG10Frame->GetMass()/kg <<" kg" << G4endl;
      G4cout << "VerticalG10Plate    (2) mass="<<log_verticalG10Plate->GetMass()/kg   <<" kg" << G4endl;
      G4cout << "SideAlPlate         (4) mass="<<log_sideAlPlate->GetMass()/kg        <<" kg" << G4endl;
      G4cout << "Water Post          (4) mass="<<log_post->GetMass()/kg               <<" kg" << G4endl;
      G4cout << "SideAlRoPlate       (2) mass="<<log_sideAlRoPlate->GetMass()/kg      <<" kg" << G4endl;
      G4cout << "FrBaAlRoPlate       (4) mass="<<log_frbaAlRoPlate->GetMass()/kg      <<" kg" << G4endl;
      G4cout << "ToBoAlRoPlate       (4) mass="<<log_toboAlRoPlate->GetMass()/kg      <<" kg" << G4endl;
      G4cout << "FrBaG10RoPlate      (4) mass="<<log_frbaG10RoPlate->GetMass()/kg     <<" kg" << G4endl;
      G4cout << "ToBoG10RoPlate      (4) mass="<<log_toboG10RoPlate->GetMass()/kg     <<" kg" << G4endl;
      G4cout << "FrBaPiece1          (4) mass="<<log_frbaPiece1->GetMass()/kg         <<" kg" << G4endl;
      G4cout << "FrBaPiece2          (4) mass="<<log_frbaPiece2->GetMass()/kg         <<" kg" << G4endl;
      G4cout << "FrBaPiece3          (4) mass="<<log_frbaPiece3->GetMass()/kg         <<" kg" << G4endl;
      G4cout << "BoPiece1            (2) mass="<<log_boPiece1->GetMass()/kg           <<" kg" << G4endl;
      G4cout << "BoPiece2            (2) mass="<<log_boPiece2->GetMass()/kg           <<" kg" << G4endl;
      G4cout << "BoPiece3            (2) mass="<<log_boPiece3->GetMass()/kg           <<" kg" << G4endl;
      G4cout << "BoPiece4            (2) mass="<<log_boPiece4->GetMass()/kg           <<" kg" << G4endl;
      G4cout << "BoPiece5            (2) mass="<<log_boPiece5->GetMass()/kg           <<" kg" << G4endl;
      G4cout << "ToPiece1            (2) mass="<<log_toPiece1->GetMass()/kg           <<" kg" << G4endl;
      G4cout << "ToPiece2            (2) mass="<<log_toPiece2->GetMass()/kg           <<" kg" << G4endl;
      G4cout << "ToPiece3            (2) mass="<<log_toPiece3->GetMass()/kg           <<" kg" << G4endl;
      G4cout << "FrBaCon2            (2) mass="<<log_frbaCon2->GetMass()/kg           <<" kg" << G4endl;
      G4cout << "ToBoCon2            (2) mass="<<log_toboCon2->GetMass()/kg           <<" kg" << G4endl;
      G4cout << "FrBaCon             (2) mass="<<log_frbaCon->GetMass()/kg            <<" kg" << G4endl;
      G4cout << "ToBoCon             (2) mass="<<log_toboCon->GetMass()/kg            <<" kg" << G4endl;
      
      G4cout << "Total TPC cage mass : "<<
	2*log_sideG10Plate->GetMass()/kg       +
	4*log_verticalG10Frame->GetMass()/kg   +
	4*log_horizontalG10Frame->GetMass()/kg +
	2*log_verticalG10Plate->GetMass()/kg   +
	4*log_sideAlPlate->GetMass()/kg        +
	4*log_post->GetMass()/kg               +
	2*log_sideAlRoPlate->GetMass()/kg      +
	4*log_frbaAlRoPlate->GetMass()/kg      +
	4*log_toboAlRoPlate->GetMass()/kg      +
	4*log_frbaG10RoPlate->GetMass()/kg     +
	4*log_toboG10RoPlate->GetMass()/kg     +
	4*log_frbaPiece1->GetMass()/kg         +
	4*log_frbaPiece2->GetMass()/kg         +
	4*log_frbaPiece3->GetMass()/kg         +
	2*log_boPiece1->GetMass()/kg           +
	2*log_boPiece2->GetMass()/kg           +
	2*log_boPiece3->GetMass()/kg           +
	2*log_boPiece4->GetMass()/kg           +
	2*log_boPiece5->GetMass()/kg           +
	2*log_toPiece1->GetMass()/kg           +
	2*log_toPiece2->GetMass()/kg           +
	2*log_toPiece3->GetMass()/kg           +
	2*log_frbaCon2->GetMass()/kg           +
	2*log_toboCon2->GetMass()/kg           +
	2*log_frbaCon->GetMass()/kg            +
	2*log_toboCon->GetMass()/kg            
	     <<" kg" << G4endl;
    }
    
}






/// Need to compensate the fact that active volumes of FGD and TPC have centers that are
/// 25mm above (+25mm in y) the envelope volumes' centers
/// "Active volume" refers to the active materials of the detector, such as TPC micromegas, 
/// or FGD scintillator bars.
G4ThreeVector ExN02DetectorConstruction::GetRotOffset(G4ThreeVector axis, double angle)
{
  // Resulting rotation matrix from an axis and angle
  // Could have used Geant4 implementation, but this is more precise with small angles.
  double trackerRotMatrix[9];
  trackerRotMatrix[0] = 2*pow(sin(angle/2),2)*pow(axis.x(),2) + cos(angle);
  trackerRotMatrix[1] = 2*pow(sin(angle/2),2)*axis.x()*axis.y() - sin(angle)*axis.z();
  trackerRotMatrix[2] = 2*pow(sin(angle/2),2)*axis.x()*axis.z() + sin(angle)*axis.y();
  trackerRotMatrix[3] = 2*pow(sin(angle/2),2)*axis.x()*axis.y() + sin(angle)*axis.z();
  trackerRotMatrix[4] = 2*pow(sin(angle/2),2)*pow(axis.y(),2) + cos(angle);
  trackerRotMatrix[5] = 2*pow(sin(angle/2),2)*axis.y()*axis.z() - sin(angle)*axis.x();
  trackerRotMatrix[6] = 2*pow(sin(angle/2),2)*axis.x()*axis.z() - sin(angle)*axis.y();
  trackerRotMatrix[7] = 2*pow(sin(angle/2),2)*axis.y()*axis.z() + sin(angle)*axis.x();
  trackerRotMatrix[8] = 2*pow(sin(angle/2),2)*pow(axis.z(),2) + cos(angle);

  // Quantities to offset the 25mm shift in Y for trackers
  G4ThreeVector offset(trackerRotMatrix[1]*-25*mm, (trackerRotMatrix[4]-1)*-25*mm, trackerRotMatrix[7]*-25*mm);

  return offset;
}

const G4VisAttributes* ExN02DetectorConstruction::GetVisual(void) const {
    G4VisAttributes* visual = new G4VisAttributes();
    visual->SetColor(0.5,0.5,1.0,0); // blue-green       
    return visual;
}


void ExN02DetectorConstruction::DefineDimensions(){
    
  ////////////////////////////
  //                        //
  // Dimension of detectors //
  //                        //
  ////////////////////////////
  
  // Basket 
  // Follow nd280mc convention in order to match NEUT requirements: 
  // - don't include the P0D
  // - use same size variables 
  
  G4double cBasketInnerWidth    = 2320.0 * mm;
  G4double cBasketSideThickness = 100.0  * mm;
  G4double cBasketOuterWidth    = 
    cBasketInnerWidth + 
    cBasketSideThickness * 2.;
  
  G4double cBasketOuterHeight   = 2480.0 * mm;
  
  G4double cBasketInnerLength         = 6650.0 * mm;
  G4double cBasketUpStreamThickness   = 140.0  * mm;
  G4double cBasketDownStreamThickness = 140.0  * mm;
  G4double cBasketOuterLength   = 
    cBasketInnerLength + 
    cBasketUpStreamThickness + 
    cBasketDownStreamThickness;
  
  G4double cBasketLength = cBasketOuterLength;
  G4double cBasketWidth  = cBasketOuterWidth;
  G4double cBasketHeight = cBasketOuterHeight;

  SetBasketFullLength(cBasketLength);
  SetBasketFullWidth(cBasketWidth);
  SetBasketFullHeight(cBasketHeight);

  // World

  G4double cWorldLength = 1.2 *(cBasketLength);
  G4double cWorldWidth  = 1.2 *(cBasketWidth);
  G4double cWorldHeight = 1.2 *(cBasketHeight);

  SetWorldFullLength(cWorldLength);
  SetWorldFullWidth(cWorldWidth);
  SetWorldFullHeight(cWorldHeight);



  // Tracker 

  G4double cTrackerLength = cBasketLength;    // Full length of Tracker
  G4double cTrackerWidth  = 2320. * mm;  // Full width of Tracker
  G4double cTrackerHeight = cBasketHeight; //2414. * mm;  // Full height of Tracker
  
  SetTrackerFullLength(cTrackerLength);
  SetTrackerFullWidth(cTrackerWidth);
  SetTrackerFullHeight(cTrackerHeight);
  
  // Target 1 (plastic scintillator)
  
  G4double targetlength1 = ND280XMLInput->GetXMLTargetlength1() * mm;
  G4double targetwidth1  = ND280XMLInput->GetXMLTargetwidth1() * mm;
  G4double targetheight1 = ND280XMLInput->GetXMLTargetheight1() * mm;

  G4double cTargetLength1 = 2000.0 * mm; //2400.0 * mm; //1560 * mm;
  G4double cTargetWidth1  = 2300.0 * mm; 
  G4double cTargetHeight1 = 303.0  * mm;   
  
  if( !ND280XMLInput->GetXMLTargetdefault1() ){
    SetTargetFullLength1(targetlength1);
    SetTargetFullWidth1(targetwidth1);
    SetTargetFullHeight1(targetheight1);
  }
  else{
    SetTargetFullLength1(cTargetLength1);
    SetTargetFullWidth1(cTargetWidth1);
    SetTargetFullHeight1(cTargetHeight1);
  }

  
  // Target 2 (water)
  
  G4double targetlength2 = ND280XMLInput->GetXMLTargetlength2() * mm;
  G4double targetwidth2  = ND280XMLInput->GetXMLTargetwidth2() * mm;
  G4double targetheight2 = ND280XMLInput->GetXMLTargetheight2() * mm;

  G4double cTargetLength2 = 2000.0 * mm; //2400.0 * mm; //1560 * mm;
  G4double cTargetWidth2  = 2300.0 * mm; 
  G4double cTargetHeight2 = 303.0  * mm;   
  
  if( !ND280XMLInput->GetXMLTargetdefault2() ){
    SetTargetFullLength2(targetlength2);
    SetTargetFullWidth2(targetwidth2);
    SetTargetFullHeight2(targetheight2);
  }
  else{
    SetTargetFullLength2(cTargetLength2);
    SetTargetFullWidth2(cTargetWidth2);
    SetTargetFullHeight2(cTargetHeight2);
  }
  

  // Side TPCs 1
  
  G4double sidetpclength1 = ND280XMLInput->GetXMLSideTPClength1() * mm;
  G4double sidetpcwidth1  = ND280XMLInput->GetXMLSideTPCwidth1() * mm;
  G4double sidetpcheight1 = ND280XMLInput->GetXMLSideTPCheight1() * mm;

  G4double cSideTPCLength1 = cTargetLength1;// mm; 
  G4double cSideTPCWidth1  = cTargetWidth1; // mm;
  G4double cSideTPCHeight1 = 1000.0 * mm;  

  if(!ND280XMLInput->GetXMLSideTPCdefault1()){
    SetSideTPCFullLength1(sidetpclength1);
    SetSideTPCFullWidth1(sidetpcwidth1);
    SetSideTPCFullHeight1(sidetpcheight1);
  }
  else{
    SetSideTPCFullLength1(cSideTPCLength1);
    SetSideTPCFullWidth1(cSideTPCWidth1);
    SetSideTPCFullHeight1(cSideTPCHeight1);
  }
 
  // Side TPCs 2
  
  G4double sidetpclength2 = ND280XMLInput->GetXMLSideTPClength2() * mm;
  G4double sidetpcwidth2  = ND280XMLInput->GetXMLSideTPCwidth2() * mm;
  G4double sidetpcheight2 = ND280XMLInput->GetXMLSideTPCheight2() * mm;

  G4double cSideTPCLength2 = cTargetLength2;// mm; 
  G4double cSideTPCWidth2  = cTargetWidth2; // mm;
  G4double cSideTPCHeight2 = 1000.0 * mm;  

  if(!ND280XMLInput->GetXMLSideTPCdefault2()){
    SetSideTPCFullLength2(sidetpclength2);
    SetSideTPCFullWidth2(sidetpcwidth2);
    SetSideTPCFullHeight2(sidetpcheight2);
  }
  else{ // default
    SetSideTPCFullLength2(cSideTPCLength2);
    SetSideTPCFullWidth2(cSideTPCWidth2);
    SetSideTPCFullHeight2(cSideTPCHeight2);
  }

}
