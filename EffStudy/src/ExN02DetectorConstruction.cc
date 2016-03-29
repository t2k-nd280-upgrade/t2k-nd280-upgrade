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
#include "ExN02MagneticField.hh"
#include "ExN02TrackerSD.hh"

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

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02DetectorConstruction::ExN02DetectorConstruction()
:solidWorld(0),  logicWorld(0),  physiWorld(0),
 solidTarget(0), logicTarget(0), physiTarget(0), 
 solidTracker(0),logicTracker(0),physiTracker(0), 
 solidChamberUp(0),logicChamberUp(0),physiChamberUp(0), 
 solidChamberDown(0),logicChamberDown(0),physiChamberDown(0), 
 TargetMater(0), ChamberMater(0), //chamberParam(0),
 stepLimit(0), fpMagField(0),
 fWorldLength(0.), fWorldWidth(0.), fWorldHeight(0.),
 fTargetLength(0.),fTargetWidth(0.),fTargetHeight(0.),
 fTrackerLength(0.),
 NbOfChambers(0) ,  ChamberWidth(0.),  ChamberSpacing(0.),
 fAbsorberPV(0), // new
 fGapPV(0), // new
 fCheckOverlaps(true) // new
{
  fpMagField = new ExN02MagneticField();
  detectorMessenger = new ExN02DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02DetectorConstruction::~ExN02DetectorConstruction()
{
  delete fpMagField;
  delete stepLimit;
  //delete chamberParam;
  delete detectorMessenger;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* ExN02DetectorConstruction::Construct()
{
  //--------- Define materials ----------
  
  this->DefineMaterials(); // method copied from ND280 software
  
  // G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  // G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  
  //--------- Sizes of the principal geometrical components (solids)  ---------

  G4String NameChamberUp   = "TPC Up";
  G4String NameChamberDown = "TPC Down";

  fTargetLength  = 240.0 * cm;      // Full length of Target 
  fTargetWidth   = 230.0 * cm;      // Full width of Target 
  fTargetHeight  = 30.3 * cm;       // Full length of Target 
  
  NbOfChambers = 2;

  //ChamberWidth = 200*cm;
  ChamberLength = fTargetLength;
  ChamberWidth  = fTargetWidth;
  ChamberHeight = 100*cm;

  fTrackerLength = fTargetLength;                 // Full length of Tracker
  fTrackerWidth  = fTargetWidth;                  // Full width of Tracker
  fTrackerHeight = fTargetHeight+ChamberHeight*2; // Full height of Tracker
  
  WorldMater   = FindMaterial("Air"); //Air;
  TrackerMater = FindMaterial("Air"); //Air;
  //TargetMater  = FindMaterial("FGDScintillator"); //fgd scintillator; 
  TargetMater  = FindMaterial("ActiveWater"); //fgd active water; 
  ChamberMater = FindMaterial("GasMixtureTPC");// gasMixture; // gas mixture of ND280 TPCs

 
  
  
  fWorldLength= 1.2 *(fTrackerLength);
  fWorldWidth = 1.2 *(fTrackerWidth);
  fWorldHeight= 1.2 *(fTrackerHeight);
   
  G4double targetSizeLength  = 0.5*fTargetLength;    // Half length of the Target 
  G4double targetSizeWidth   = 0.5*fTargetWidth;     // Half width of the Target 
  G4double targetSizeHeight  = 0.5*fTargetHeight;    // Half height of the Target 
 
  G4double trackerSizeLenght = 0.5*fTrackerLength;  // Half length of the Tracker
  G4double trackerSizeWidth  = 0.5*fTrackerWidth;   // Half width of the Tracker
  G4double trackerSizeHeight = 0.5*fTrackerHeight;  // Half height of the Tracker
      
  //--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------
  
  //------------------------------ 
  // World
  //------------------------------ 

  G4double HalfWorldLength = 0.5*fWorldLength;
  G4double HalfWorldWidth  = 0.5*fWorldWidth;
  G4double HalfWorldHeight = 0.5*fWorldHeight;

  
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(fWorldLength);
  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  solidWorld= new G4Box("world",HalfWorldWidth,HalfWorldHeight,HalfWorldLength);
  logicWorld= new G4LogicalVolume( solidWorld, WorldMater, "World", 0, 0, 0);
  

  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  physiWorld = new G4PVPlacement(0,               // no rotation
                                 G4ThreeVector(), // at (0,0,0)
                                 logicWorld,      // its logical volume
				 "World",         // its name
                                 0,               // its mother  volume
                                 false,           // no boolean operations
                                 0,              // copy number
				 fCheckOverlaps);  // checking overlaps
 
  G4cout << "World is " 
  	 << fWorldHeight/cm << " x " 
  	 << fWorldWidth/cm  << " x " 
  	 << fWorldLength/cm << " cm^3" 
	 << " of " << WorldMater->GetName()
  	 << G4endl;
				 

  //------------------------------ 
  // Tracker
  //------------------------------
  
  
  G4double HalfTrackerLength = 0.5*(fTrackerLength); 
  G4double HalfTrackerWidth  = 0.5*(fTrackerWidth); 
  G4double HalfTrackerHeight = 0.5*(fTrackerHeight); 

  //G4ThreeVector positionTracker = G4ThreeVector(0,HalfTrackerHeight,0);
  G4ThreeVector positionTracker = G4ThreeVector(0,0,0);

  solidTracker = new G4Box("tracker",HalfTrackerWidth,HalfTrackerHeight,HalfTrackerLength);
  logicTracker = new G4LogicalVolume(solidTracker , TrackerMater, "Tracker",0,0,0);  
  physiTracker = new G4PVPlacement(0,              // no rotation
				   positionTracker, // at (x,y,z)
				   logicTracker,    // its logical volume	 
				   "Tracker",       // its name
				   logicWorld,      // its mother  volume
				   false,           // no boolean operations
				   0,              // copy number 
				   fCheckOverlaps);  // checking overlaps     

  G4cout << "Tracker is " 
  	 << fTrackerWidth/cm  << " x " 
  	 << fTrackerHeight/cm << " x " 
  	 << fTrackerLength/cm << " cm^3"
	 << " of " << TrackerMater->GetName()
  	 << G4endl;
 

  //------------------------------ 
  // Target
  //------------------------------

  G4double HalfTargetHeight  = 0.5 * fTargetHeight;
  
  G4ThreeVector positionTarget = G4ThreeVector(0,0,0);
  
  solidTarget = new G4Box("target",targetSizeWidth,targetSizeHeight,targetSizeLength);
  logicTarget = new G4LogicalVolume(solidTarget,TargetMater,"Target",0,0,0);
  physiTarget = new G4PVPlacement(0,               // no rotation
  				  positionTarget,  // at (x,y,z)
  				  logicTarget,     // its logical volume  	  
  				  "Target",        // its name
  				  logicTracker,      // its mother  volume
  				  //logicWorld,      // its mother  volume
  				  false,           // no boolean operations
  				  0,              // copy number 
				  fCheckOverlaps);  // checking overlaps     

  G4cout << "Target is " 
  	 << fTargetWidth/cm  << " x " 
  	 << fTargetHeight/cm << " x " 
  	 << fTargetLength/cm << " cm^3" 
         << " of " << TargetMater->GetName() 
  	 << G4endl;

  
  //------------------------------ 
  // Tracker segments
  //------------------------------
  // 
  // An example of Parameterised volumes
  // dummy values for G4Box -- modified by parameterised volume
  
  G4double HalfChamberLength = 0.5 * ChamberLength;
  G4double HalfChamberWidth  = 0.5 * ChamberWidth;
  G4double HalfChamberHeight = 0.5 * ChamberHeight;
  
  solidChamberUp = new G4Box("chamberUp", HalfChamberWidth, HalfChamberHeight, HalfChamberLength); 
  logicChamberUp = new G4LogicalVolume(solidChamberUp,ChamberMater,"ChamberUp",0,0,0);

  ChamberSpacing = fTargetHeight;
  
  G4ThreeVector positionChamberUp = G4ThreeVector(0,
						  (HalfChamberHeight+HalfTargetHeight),
						  0);

  physiChamberUp = new G4PVPlacement(0,               // no rotation
				     positionChamberUp, // at (x,y,z)
				     logicChamberUp,    // its logical volume	 
				     NameChamberUp,         // its name
				     logicTracker,     // its mother  volume
				     false,            // no boolean operations
				     0,               // copy number 
				     fCheckOverlaps);  // checking overlaps     
  
  
  solidChamberDown = new G4Box("chamberDown", HalfChamberWidth, HalfChamberHeight, HalfChamberLength); 
  logicChamberDown = new G4LogicalVolume(solidChamberDown,ChamberMater,"ChamberDown",0,0,0);
  
  G4ThreeVector positionChamberDown = G4ThreeVector(0,-(HalfChamberHeight+HalfTargetHeight),0);

  physiChamberDown = new G4PVPlacement(0,              // no rotation
				       positionChamberDown, // at (x,y,z)
				       logicChamberDown,    // its logical volume	 
				       NameChamberDown,       // its name
				       logicTracker,      // its mother  volume
				       false,           // no boolean operations
				       0,              // copy number 
				       fCheckOverlaps);  // checking overlaps     
  
  G4cout << "Chamber is " 
  	 << ChamberWidth/cm  << " x " 
  	 << ChamberHeight/cm << " x " 
  	 << ChamberLength/cm << " cm^3" 
         << " of " << ChamberMater->GetName() 
  	 << G4endl;

  // physiChamber = new G4PVReplica(
  // 				 "Chamber",       // their name
  // 				 logicChamber,    // their logical volume
  // 				 logicTracker,    // Mother logical volume
  // 				 kYAxis,          // Are placed along this axis 
  // 				 NbOfChambers,    // Number of chambers
  // 				 ChamberWidth,    // Width of a chamber
  // 				 0.0); // Spacing between chambers 
  //ChamberSpacing); // Spacing between chambers 
 
  // G4double firstPosition = -trackerSizeLenght + 0.5*ChamberWidth;  
  // G4double firstLength = fTrackerLength/10;
  // G4double lastLength  = fTrackerLength;
  
  //chamberParam = new ExN02ChamberParameterisation(  
  // 			   NbOfChambers,          // NoChambers 
  // 			   firstPosition,         // Z of center of first 
  // 			   ChamberSpacing,        // Z spacing of centers
  // 			   ChamberWidth,          // Width Chamber 
  // 			   firstLength,           // lengthInitial 
  // 			   lastLength);           // lengthFinal
			   
  // // dummy value : kZAxis -- modified by parameterised volume
  // //
  // physiChamber = new G4PVParameterised(
  //                           "Chamber",       // their name
  //                           logicChamber,    // their logical volume
  //                           logicTracker,    // Mother logical volume
  // 			    kZAxis,          // Are placed along this axis 
  //                           NbOfChambers,    // Number of chambers
  //                           chamberParam);   // The parametrisation

  // G4cout << "There are " << NbOfChambers << " chambers in the tracker region. "
  //        << "The chambers are " << ChamberWidth/mm << " mm of " 
  //        << ChamberMater->GetName() << "\n The distance between chamber is "
  // 	 << ChamberSpacing/cm << " cm" << G4endl;
	 


  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  G4String trackerChamberSDname = "ExN02/TrackerChamberSD";
  ExN02TrackerSD* aTrackerSD = new ExN02TrackerSD( trackerChamberSDname );
  SDman->AddNewDetector( aTrackerSD );
  logicChamberUp->SetSensitiveDetector( aTrackerSD );
  logicChamberDown->SetSensitiveDetector( aTrackerSD );
  logicTarget->SetSensitiveDetector( aTrackerSD );
    
  //--------- Visualization attributes -------------------------------

  G4VisAttributes* BoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  logicWorld  ->SetVisAttributes(BoxVisAtt);  
  logicTracker->SetVisAttributes(BoxVisAtt);

  G4VisAttributes* TargetVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  logicTarget ->SetVisAttributes(TargetVisAtt);
  
  G4VisAttributes* ChamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  logicChamberUp->SetVisAttributes(ChamberVisAtt);
  logicChamberDown->SetVisAttributes(ChamberVisAtt);
  




  //--------- example of User Limits -------------------------------

  // below is an example of how to set tracking constraints in a given
  // logical volume(see also in N02PhysicsList how to setup the processes
  // G4StepLimiter or G4UserSpecialCuts).
  
  // Sets a max Step length in the tracker region, with G4StepLimiter
  
  G4double maxStep = 0.5*ChamberWidth;
  stepLimit = new G4UserLimits(maxStep);
  logicTracker->SetUserLimits(stepLimit);
  logicTarget->SetUserLimits(stepLimit);
  


  // Set additional contraints on the track, with G4UserSpecialCuts
  //G4double maxLength = 4*fTrackerLength, maxTime = 10*ns, minEkin = 10*keV;
  //logicTracker->SetUserLimits(new G4UserLimits(maxStep,maxLength,maxTime,minEkin));
  //logicTarget->SetUserLimits(new G4UserLimits(maxStep,maxLength,maxTime,minEkin));



  
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void ExN02DetectorConstruction::setTargetMaterial(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
     {TargetMater = pttoMaterial;
      logicTarget->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The target is " << fTargetLength/cm << " cm of "
             << materialName << G4endl;
     }             
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02DetectorConstruction::setChamberMaterial(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
     {ChamberMater = pttoMaterial;
      logicChamberUp->SetMaterial(pttoMaterial); 
      logicChamberDown->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The chambers are " << ChamberWidth/cm << " cm of "
             << materialName << G4endl;
     }             
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void ExN02DetectorConstruction::SetMagField(G4double fieldValue)
{
  fpMagField->SetMagFieldValue(fieldValue);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02DetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((stepLimit)&&(maxStep>0.)) stepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02DetectorConstruction::DefineMaterials() {

  // //--------- Material definition from default example ---------
  // G4String name, symbol;
  // G4int nel, ncomponents, natoms;
  // G4double fractionmass;
  // G4double a, z;
  // G4double density, temperature, pressure;
  // G4NistManager* nistMan = G4NistManager::Instance();  
  // G4Element* elH = nistMan->FindOrBuildElement(1);
  // G4Element* elC = nistMan->FindOrBuildElement(6);
  // G4Element* elO = nistMan->FindOrBuildElement(8);
  // G4Element* elAr = nistMan->FindOrBuildElement(18);
  // //Air
  // G4Element* N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);
  // G4Element* O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*g/mole);   
  // G4Material* Air = new G4Material("Air", density= 1.29*mg/cm3, nel=2);
  // Air->AddElement(N, 70*perCent);
  // Air->AddElement(O, 30*perCent);
  // //Lead
  // G4Material* Pb = new G4Material("Lead", z=82., a= 207.19*g/mole, density= 11.35*g/cm3);  
  // //Xenon gas
  // G4Material* Xenon = new G4Material("XenonGas", z=54., a=131.29*g/mole, density= 5.458*mg/cm3,
  // 				     kStateGas, temperature= 293.15*kelvin, pressure= 1*atmosphere);  
  // //Carbon
  // G4Material* C = new G4Material("Carbon", z=12., a=131.29*g/mole, density= 5.458*mg/cm3,
  // 				 kStateGas, temperature= 293.15*kelvin, pressure= 1*atmosphere);  
  // //Water                           
  // density = 1.0*g/cm3;
  // G4Material* water = new G4Material(name="Water", density, nel=2);
  // water->AddElement(elH, natoms=2);
  // water->AddElement(elO, natoms=1); 
  // //Argon                                     
  // density     = 1.66*mg/cm3;
  // pressure    = 1*atmosphere;
  // temperature = 293.15*kelvin;
  // G4Material* argon =  new G4Material(name="Ar", density,
  // 				      nel=1,kStateGas,temperature,
  // 				      pressure);
  // argon->AddElement(elAr, natoms=1); 
  // //Methane      
  // density     = 0.667*mg/cm3;
  // pressure    = 1*atmosphere;
  // temperature = 293.15*kelvin;
  // G4Material* methane = new G4Material(name="CH4", density,
  // 				       nel=2,kStateGas,temperature,
  // 				       pressure);
  // methane->AddElement(elC, natoms=1);
  // methane->AddElement(elH, natoms=4);    
  // //Argon + 10% Methane                            
  // density     = 2.33*mg/cm3;
  // G4Material* gasMixture
  //   = new G4Material(name="GasMixture", density, ncomponents=2);
  // gasMixture->AddMaterial(argon, fractionmass = 90*perCent);
  // gasMixture->AddMaterial(methane, fractionmass = 10*perCent);
  // // Print all the materials defined.
  // //



  // Method copied from ND280 software

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

    G4Element* elCl = nistMan->FindOrBuildElement(17);

    G4Element* elAr = nistMan->FindOrBuildElement(18);

    G4Element* elTi = nistMan->FindOrBuildElement(22);

    G4Element* elFe = nistMan->FindOrBuildElement(26);

    G4Element* elCo = nistMan->FindOrBuildElement(27);

    G4Element* elCu = nistMan->FindOrBuildElement(29);

    G4Element* elZn = nistMan->FindOrBuildElement(30);

    G4Element* elSn = nistMan->FindOrBuildElement(50);

    G4Element* elPb = nistMan->FindOrBuildElement(82);

#ifdef Define_Vacuum
    //Vacuum (set as default material)
    density     = universe_mean_density;
    pressure    = 3.e-18*pascal;
    temperature = 2.73*kelvin;
    vacuum = new G4Material(name="Vacuum", z=1., a=1.01*g/mole,
                   density,kStateGas,temperature,pressure);
    //gMan->SetColor(vacuum->GetName(),-1); // ND280 class
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
    //gMan->SetDrawAtt(air,kGray+3); // ND280 class

    //Earth
    density = 2.15*g/cm3;
    G4Material* earth = new G4Material(name="Earth", density, nel=2);
    earth->AddElement(elSi, natoms=1);
    earth->AddElement(elO, natoms=2);
    //gMan->SetDrawAtt(earth,49,0.2); // ND280 class
    
    //Cement
    density = 2.5*g/cm3;
    G4Material* cement = new G4Material(name="Cement", density, nel=2);
    cement->AddElement(elSi, natoms=1);
    cement->AddElement(elO, natoms=2);
    //gMan->SetDrawAtt(cement,14,0.2); // ND280 class

    //Water
    density = 1.0*g/cm3;
    G4Material* water = new G4Material(name="Water", density, nel=2);
    water->AddElement(elH, natoms=2);
    water->AddElement(elO, natoms=1);
    //gMan->SetDrawAtt(water,kBlue); // ND280 class

    //Aluminum
    density = 2.70*g/cm3;
    G4Material* aluminum = new G4Material(name="Aluminum",density, nel=1);
    aluminum->AddElement(elAl,100.*perCent);
    //gMan->SetDrawAtt(aluminum,kYellow-5); // ND280 class

    //Iron.
    density = 7.87*g/cm3;
    G4Material* iron = new G4Material(name="Iron",density, nel=1);
    iron->AddElement(elFe,100.*perCent);
    //gMan->SetDrawAtt(iron,kRed+2,0.3); // ND280 class

    //Copper
    density = 8.94*g/cm3;
    G4Material* copper = new G4Material(name="Copper",density, nel=1);
    copper->AddElement(elCu,100.*perCent);
    //gMan->SetDrawAtt(copper,kRed+1,0.3);// ND280 class

    //Lead
    density = 11.35*g/cm3;
    G4Material* lead = new G4Material(name="Lead",density, nel=1);
    lead->AddElement(elPb,100.*perCent);
    //gMan->SetDrawAtt(lead,kGray+1);// ND280 class

    //Brass -- The density is from simetric.co.uk is 8400 -- 8730 gm/cm3
    density = 8.50*g/cm3;
    G4Material* brass = new G4Material(name="Brass", density, nel=2);
    brass->AddElement(elCu, fractionmass = 90*perCent);
    brass->AddElement(elZn, fractionmass = 10*perCent);
    //gMan->SetDrawAtt(brass,kOrange-2);// ND280 class

    //Bronze -- The density is from simetric.co.uk is 7700 -- 8920 gm/cm3
    density = 8.5*g/cm3;
    G4Material* bronze = new G4Material(name="Bronze", density, nel=2);
    bronze->AddElement(elCu, fractionmass = 60*perCent);
    bronze->AddElement(elSn, fractionmass = 40*perCent);
    //gMan->SetDrawAtt(bronze,kOrange-3);// ND280 class

    //Stainless Steel.  The density is taken from average 304 grade SS.
    density = 8.0*g/cm3;
    G4Material* steel = new G4Material(name="Steel", density, nel=3);
    steel->AddElement(elC,  fractionmass =  4*perCent);
    steel->AddElement(elFe, fractionmass = 88*perCent);
    steel->AddElement(elCo, fractionmass =  8*perCent);
    //gMan->SetDrawAtt(steel,kBlue-10);// ND280 class

    //Argon
    density     = 1.66*mg/cm3;    
    pressure    = 1*atmosphere;
    temperature = 293.15*kelvin;
    G4Material* argon =  new G4Material(name="Ar", density,
                                        nel=1,kStateGas,temperature,
                                        pressure);
    argon->AddElement(elAr, natoms=1);
    //gMan->SetDrawAtt(argon,kBlue-10,0.1);// ND280 class

    //Methane
    density     = 0.667*mg/cm3;
    pressure    = 1*atmosphere;
    temperature = 293.15*kelvin;
    G4Material* methane = new G4Material(name="CH4", density,
                                         nel=2,kStateGas,temperature,
                                         pressure);
    methane->AddElement(elC, natoms=1);
    methane->AddElement(elH, natoms=4);
    //gMan->SetDrawAtt(methane,kBlue-10,0.1);// ND280 class

    //Argon + 10% Methane
    density     = 2.33*mg/cm3;
    G4Material* gasMixture 
        = new G4Material(name="GasMixture", density, ncomponents=2);
    gasMixture->AddMaterial(argon, fractionmass = 90*perCent);
    gasMixture->AddMaterial(methane, fractionmass = 10*perCent);
    //gMan->SetDrawAtt(gasMixture,kBlue-10,0.1);// ND280 class

    // CarbonDioxide
    density     = 1.828*mg/cm3;
    pressure    = 1.*atmosphere;
    temperature = 293.15*kelvin;
    G4Material* CO2 = new G4Material(name="CO2", density,
                                     nel=2,kStateGas,temperature,
                                     pressure);
    CO2->AddElement(elC, natoms=1);
    CO2->AddElement(elO, natoms=2);
    //gMan->SetDrawAtt(CO2,kBlue-10,0.1);// ND280 class

    // CarbonTetrafluoride
    density     = 3.66*mg/cm3;
    pressure    = 1.*atmosphere;
    temperature = 293.15*kelvin;
    G4Material* CF4 = new G4Material(name="CF4", density,
                                     nel=2,kStateGas,temperature,
                                     pressure);
    CF4->AddElement(elC, natoms=1);
    CF4->AddElement(elF, natoms=4);
    //gMan->SetDrawAtt(CF4,kBlue-10,0.1);// ND280 class

    // Isobutane
    density     = 2.47*mg/cm3;
    pressure    = 1.*atmosphere;
    temperature = 293.15*kelvin;
    G4Material* C4H10 = new G4Material(name="C410", density,
                                     nel=2,kStateGas,temperature,
                                     pressure);
    C4H10->AddElement(elC, natoms=4);
    C4H10->AddElement(elH, natoms=10);
    //gMan->SetDrawAtt(C4H10,kBlue-10,0.1);// ND280 class

    // Ar-CO2
    density     = 1.958*mg/cm3;
    G4Material* gasMixtureArCO2
        = new G4Material(name="GasMixtureArCO2", density, ncomponents=2);
    gasMixtureArCO2->AddMaterial(argon, fractionmass = 90*perCent);
    gasMixtureArCO2->AddMaterial(CO2, fractionmass = 10*perCent);
    //gMan->SetDrawAtt(gasMixtureArCO2,kBlue-10,0.1);// ND280 class

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
    //gMan->SetDrawAtt(gasMixtureTPC,kBlue-10,0.1);// ND280 class

    // Water-SSteel-Air Mixture
    density     = 2.646*g/cm3;
    G4Material* waterSystem
        = new G4Material(name="WaterSystem", density, ncomponents=3);
    waterSystem->AddMaterial(water, fractionmass = 18*perCent);
    waterSystem->AddMaterial(steel, fractionmass = 32*perCent);
    waterSystem->AddMaterial(CO2, fractionmass = 50*perCent);
    //gMan->SetDrawAtt(waterSystem,kBlue-7);// ND280 class
    
    // add TPC field cage mixture NB rough guesses !!!!
    density = 0.221*g/cm3; // this gives 1.4 10-2 X0 for 2.2 cm
    a = 16.*g/mole;
    G4Material* tpcFieldCage 
        = new G4Material(name="TPCWallMaterial",8,a,density);
    //gMan->SetDrawAtt(tpcFieldCage,kYellow-7);// ND280 class
    
    // Titanium Dioxide -- Used in coextruded scintillator.
    density     = 4.26*g/cm3;
    G4Material* TIO2 = new G4Material(name="TIO2", density, nel=2);
    TIO2->AddElement(elTi, natoms=1);
    TIO2->AddElement(elO , natoms=2);
    //gMan->SetDrawAtt(TIO2,kWhite);// ND280 class
    
    // Polystyrene -- This is polystyrene defined in the PDG C6H5CH=CH2 (this
    // is a net C8H8)
    density = 1.050*g/cm3; // the density of the "extruded polystyrene"
    G4Material* polystyrene 
        = new G4Material(name="Polystyrene", density, nel=2);
    polystyrene->AddElement(elC, 8);
    polystyrene->AddElement(elH, 8);
    //gMan->SetDrawAtt(polystyrene,kGray+3);// ND280 class
    
    // Scintillator -- This is the average polystyrene plastic scintillator as
    // defined in the PDG C6H5CH=CH2 (this is a net C8H8).  The SMRD and ECal
    // geometries have been tuned to this value.  The 1.05 value represents a
    // typical extruded polystyrene.
    density = 1.050*g/cm3; 
    G4Material* scintillator 
        = new G4Material(name="Scintillator", density, nel=2);
    scintillator->AddElement(elC, 8);
    scintillator->AddElement(elH, 8);
    //gMan->SetDrawAtt(scintillator,kGreen);// ND280 class

    // P0D Scintillator -- This has the average polystyrene plastic
    // scintillator composition as defined in the PDG C6H5CH=CH2 (this is a
    // net C8H8).  The density of the P0D scintillator is taken from the assay
    // of the MINERvA scintillator which is manufactured on the same equipment
    // as the P0D scintillator and is chemically the same.  The measured
    // density of the MINERvA strips are 1.058 +- 0.003, including the Ti02
    // coating.  This corresponds to a scintillator density of 1.0506, which
    // has been rounded to 1.051
    density = 1.051*g/cm3; 
    G4Material* p0dScintillator 
        = new G4Material(name="P0DScintillator", density, nel=2);
    p0dScintillator->AddElement(elC, 8);
    p0dScintillator->AddElement(elH, 8);
    //gMan->SetDrawAtt(p0dScintillator,kGreen);// ND280 class

    // FGD Scintillator -- This is the average polystyrene scintillator for
    // the FGD.  The FGD density is based on our measurements of individual
    // bars, in combination with the measurements of the cross-sectional area
    // of the bars.
    density = 1.032*g/cm3; 
    G4Material* fgdScintillator 
        = new G4Material(name="FGDScintillator", density, nel=2);
    fgdScintillator->AddElement(elC, 8);
    fgdScintillator->AddElement(elH, 8);
    //gMan->SetDrawAtt(fgdScintillator,kGreen);// ND280 class

    // Scintillator coating.  This is the coating that goes around the average
    // plastic scintillator. It is 15% TiO2 and 85% polystyrene by weight.
    // rho = (m_Sc + m_Ti) / (V_Sc + V_Ti)
    //     = (0.85 + 0.15) / ( 0.85/1.032 + 0.15/4.26 )
    //     = 1.164 g/cm^3
    density = 1.164*g/cm3;
    G4Material* scintillatorCoating
        = new G4Material(name="ScintillatorCoating", density, ncomponents=2);
    scintillatorCoating->AddMaterial(TIO2        ,fractionmass = 15*perCent);
    scintillatorCoating->AddMaterial(scintillator,fractionmass = 85*perCent);
    //gMan->SetDrawAtt(scintillatorCoating,kGreen);// ND280 class
    
    // PVC -- Polyvinyl Chloride CH2=CHCl = C3H3Cl
    density = 1.38*g/cm3;
    G4Material* pvc
        = new G4Material(name="PVC", density, nel=3);
    pvc->AddElement(elC, 3);
    pvc->AddElement(elH, 3);
    pvc->AddElement(elCl, 1);
    //gMan->SetDrawAtt(pvc,kGray+1);// ND280 class

    // HDPE -- High Density Polyethylene used in P0D water bag
    density = 0.94*g/cm3;
    G4Material* hdpe
        = new G4Material(name="HDPE", density, nel=2);
    hdpe->AddElement(elC, natoms=1);
    hdpe->AddElement(elH, natoms=2);
    //gMan->SetDrawAtt(hdpe,kCyan-10);// ND280 class
    
    // P0DuleEpoxy -- The Epoxy used to hold the P0Dule together.  Density is
    // from the Henkel Hysol data sheet.
    density = 1.36*g/cm3;
    G4Material* p0duleEpoxy
        = new G4Material(name="P0DuleEpoxy", density, nel=3);
    p0duleEpoxy->AddElement(elC, 3);
    p0duleEpoxy->AddElement(elH, 3);
    p0duleEpoxy->AddElement(elCl, 1);
    //gMan->SetDrawAtt(p0duleEpoxy,kBlue+4);// ND280 class
    
    // FGD Glue
    density = 1.365*g/cm3;
    G4Material* fgdGlue
        = new G4Material(name="FGDGlue", density, nel=4);
    fgdGlue->AddElement(elO,fractionmass=62.5*perCent);
    fgdGlue->AddElement(elC,fractionmass=27.9*perCent);
    fgdGlue->AddElement(elH,fractionmass= 8.4*perCent);
    fgdGlue->AddElement(elN,fractionmass= 1.2*perCent);
    //gMan->SetDrawAtt(fgdGlue,kOrange);// ND280 class
    
    // FGD Water Module Epoxy
    density = 0.6573*g/cm3;
    G4Material* fgdWaterModuleEpoxy
        = new G4Material(name="FGDWaterModuleEpoxy", density, nel=4);
    fgdWaterModuleEpoxy->AddElement(elO,fractionmass=62.5*perCent);
    fgdWaterModuleEpoxy->AddElement(elC,fractionmass=27.9*perCent);
    fgdWaterModuleEpoxy->AddElement(elH,fractionmass= 8.4*perCent);
    fgdWaterModuleEpoxy->AddElement(elN,fractionmass= 1.2*perCent);
    //gMan->SetDrawAtt(fgdWaterModuleEpoxy,kOrange);// ND280 class
     
    
    // Polypropylene
    density = 0.9*g/cm3;
    G4Material* polypropylene
        = new G4Material(name="Polypropylene", density, nel=2);
    polypropylene->AddElement(elC, 3);
    polypropylene->AddElement(elH, 6);
    //gMan->SetDrawAtt(polypropylene,kYellow-5);// ND280 class

    // Polycarbonate
    density = 1.2*g/cm3;
    G4Material* polycarbonate
        = new G4Material(name="Polycarbonate", density, nel=3);
    polycarbonate->AddElement(elH,fractionmass= 5.5491*perCent);
    polycarbonate->AddElement(elC,fractionmass=75.5751*perCent);
    polycarbonate->AddElement(elO,fractionmass=18.8758*perCent);
    //gMan->SetDrawAtt(polycarbonate,kYellow-2);// ND280 class

    //carbon fibre
    // NB : chemical composition may not be completely right but  close 
    density = 1.75*g/cm3;
    G4Material* carbonFibre
        = new G4Material(name="CarbonFibre", density, nel=2);
    carbonFibre->AddElement(elC,6);
    carbonFibre->AddElement(elO,1);
    //gMan->SetDrawAtt(carbonFibre,kGray+3);// ND280 class

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
    //gMan->SetDrawAtt(g10,kGreen+3);// ND280 class

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
    //gMan->SetDrawAtt(g10fgd1,kGreen+3);// ND280 class

    // Diluted G10-FGD2
    density = 0.171*g/cm3;
    G4Material* g10fgd2
        = new G4Material(name="G10FGD2", density, nel=6);
    g10fgd2->AddElement(elH,6.2*perCent);
    g10fgd2->AddElement(elC,36.8*perCent);
    g10fgd2->AddElement(elO,30.7*perCent);
    g10fgd2->AddElement(elSi,21.9*perCent);
    g10fgd2->AddElement(elB,2.2*perCent);
    g10fgd2->AddElement(elNa,2.2*perCent);
    //gMan->SetDrawAtt(g10fgd2,kGreen+3);// ND280 class

    // Rohacell (polymethacrylimide, chemical formula to be confirmed from 
    // "Polymethyl methacrylate" C5O2H8)
    density = 0.052*g/cm3;
    G4Material* rohacell 
        = new G4Material(name="Rohacell", density, nel=3);
    // Rohacell C:H:O=5:8:2
    rohacell->AddElement(elC,5);
    rohacell->AddElement(elH,8);
    rohacell->AddElement(elO,2);
    //gMan->SetDrawAtt(rohacell,kGreen-9);// ND280 class
    
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
    //gMan->SetDrawAtt(alroha,kGreen-9);// ND280 class

    // add Al/Rohacell TPC cage wall material
    // (same as above but only 13.2mm thick)
    density = 0.3569*g/cm3;

    G4Material* alroha2
        = new G4Material(name="AlRoha2", density, ncomponents=2);

    alroha2->AddMaterial(aluminum, fractionmass = 87.11*perCent);
    alroha2->AddMaterial(rohacell, fractionmass = 12.89*perCent);
    //gMan->SetDrawAtt(alroha2,kGreen-9);// ND280 class

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
    //gMan->SetDrawAtt(g10roha,kGreen-9);// ND280 class

    // FGD Electronics card (95mm x 252mm x 8mm) with total mass of 400g. The
    // composition is 50% Al and 50% G10 by weight.  -> density =
    // 400g/191.52cm3
    density = 2.0885*g/cm3;

    G4Material* alg10
        = new G4Material(name="AlG10", density, ncomponents=2);

    alg10->AddMaterial(aluminum, fractionmass = 50.0*perCent);
    alg10->AddMaterial(g10, fractionmass = 50.0*perCent);
    //gMan->SetDrawAtt(alg10,kGreen-9);// ND280 class

    // Material for the fiber core.
    density = 1.05*g/cm3;
    G4Material* fiberCore
        = new G4Material(name="FiberCore", density, nel=2);
    fiberCore->AddElement(elC, 9);
    fiberCore->AddElement(elH, 10);
    //gMan->SetDrawAtt(fiberCore,kCyan-4);// ND280 class
    
    // Material for the fiber cladding
    density = 1.19*g/cm3;
    G4Material* fiberCladding
        = new G4Material(name="FiberCladding", density, nel=3);
    fiberCladding->AddElement(elH,8);
    fiberCladding->AddElement(elC,5);
    fiberCladding->AddElement(elO,2);
    //gMan->SetDrawAtt(fiberCladding,kBlue);// ND280 class

    // Fluorinated fiber cladding
    density = 1.43*g/cm3;
    G4Material* fiberCladdingFluor
        = new G4Material(name="FiberCladdingFluor", density, nel=2);
    fiberCladdingFluor->AddElement(elH,4);
    fiberCladdingFluor->AddElement(elC,2);
    //gMan->SetDrawAtt(fiberCladdingFluor,kBlue);// ND280 class
    
    // ActiveWater for the FGD.
    //
    // It is not necessarily activated, it is just a combination of water and
    // polycarbonate that holds the water in place.  Daniel's document says
    // the panel is 342mg/cm2 of PC (for most of the panels), and 2.2226 g/cm2
    // of water (average of the 6 deployed panels).  water+PC layer by mass is
    // 0.342/(0.342+2.2226)=0.1335 polycarbonate, so rho = 1 / (0.1335/1.2 +
    // 0.8666/1.0) = 1.0226 g/cm^3
    density = 1.0226*g/cm3;
    G4Material* activeWater 
        = new G4Material("ActiveWater", density, ncomponents=2);
    activeWater->AddMaterial(water        ,fractionmass = 86.672*perCent);
    activeWater->AddMaterial(polycarbonate,fractionmass = 13.328*perCent);
    //gMan->SetDrawAtt(activeWater,kAzure+8);// ND280 class
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* ExN02DetectorConstruction::FindMaterial(G4String name) {
    G4Material* material = G4Material::GetMaterial(name,true);
    return material;
}
