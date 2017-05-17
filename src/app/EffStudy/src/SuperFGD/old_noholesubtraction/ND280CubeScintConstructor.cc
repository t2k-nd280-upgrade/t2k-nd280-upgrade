#include <globals.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>

#include <G4RotationMatrix.hh>

#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Trap.hh>
#include <G4SubtractionSolid.hh>

#include "G4SDManager.hh"
#include "ExN02TrackerSD.hh"

#include "ND280CubeScintConstructor.hh"

// used to keep a list of SD logical volumes
#include "G4RegionStore.hh"
#include <G4Region.hh> 
//

class ND280CubeScintMessenger: public ND280ConstructorMessenger {

private:
    
  ND280CubeScintConstructor *fConstructor;
  
  G4UIcmdWithAString*        fShapeCMD;
  G4UIcmdWithADoubleAndUnit* fLengthCMD;
  G4UIcmdWithADoubleAndUnit* fBaseCMD;
  G4UIcmdWithADoubleAndUnit* fHeightCMD;
  G4UIcmdWithADoubleAndUnit* fFiberRadiusCMD;
  G4UIcmdWithADoubleAndUnit* fHoleRadiusCMD;
  G4UIcmdWithADoubleAndUnit* fCoatingThicknessCMD;
  G4UIcmdWithADoubleAndUnit* fCoatingRadiusCMD;
  G4UIcmdWithADoubleAndUnit* fGapCMD;
  G4UIcmdWithABool*          fVisibleCMD;

public:

  ND280CubeScintMessenger(ND280CubeScintConstructor* c) 
    : ND280ConstructorMessenger(c,"Control the T2K world geometry."),
      fConstructor(c) {
    
    //fShapeCMD = new G4UIcmdWithAString(CommandName("shape"),this);
    //fShapeCMD->SetGuidance("Set the extruded cross section of the bar.");
    //fShapeCMD->SetParameterName("Shape",false);
    //fShapeCMD->SetCandidates("triangle rectangle");
    
    fLengthCMD = new G4UIcmdWithADoubleAndUnit(CommandName("length"),this);
    fLengthCMD->SetGuidance("Set the length of a bar.");
    fLengthCMD->SetParameterName("Length",false);
    fLengthCMD->SetUnitCategory("Length");
    
    fBaseCMD = new G4UIcmdWithADoubleAndUnit(CommandName("base"),this);
    fBaseCMD->SetGuidance("Set base dimension of a bar.");
    fBaseCMD->SetParameterName("Base",false);
    fBaseCMD->SetUnitCategory("Length");
    
    fHeightCMD = new G4UIcmdWithADoubleAndUnit(CommandName("height"),this);
    fHeightCMD->SetGuidance("Set the bar height (default from base).");
    fHeightCMD->SetParameterName("Height",false);
    fHeightCMD->SetUnitCategory("Length");
    
    fFiberRadiusCMD 
    = new G4UIcmdWithADoubleAndUnit(CommandName("fiberRadius"),this);
    fFiberRadiusCMD->SetGuidance("Set the radius of the WLS fiber.");
    fFiberRadiusCMD->SetParameterName("Radius",false);
    fFiberRadiusCMD->SetUnitCategory("Length");
    
    fHoleRadiusCMD 
      = new G4UIcmdWithADoubleAndUnit(CommandName("holeRadius"),this);
    fHoleRadiusCMD->SetGuidance("Set the radius of the fiber hole.");
    fHoleRadiusCMD->SetParameterName("RHole",false);
    fHoleRadiusCMD->SetUnitCategory("Length");
    
    fCoatingThicknessCMD =
      new G4UIcmdWithADoubleAndUnit(CommandName("coating"),this);
    fCoatingThicknessCMD->
      SetGuidance("Set thickness of the coating on the bars.");
    fCoatingThicknessCMD->SetParameterName("TCoating",false);
    fCoatingThicknessCMD->SetUnitCategory("Length");
    
    //fCoatingRadiusCMD =
    //new G4UIcmdWithADoubleAndUnit(CommandName("coatrad"),this);
    //fCoatingRadiusCMD->
    //SetGuidance("Set inner radius of the corner bar coating.");
    //fCoatingRadiusCMD->SetParameterName("RCoating",false);
    //fCoatingRadiusCMD->SetUnitCategory("Length");
    
    fGapCMD = new G4UIcmdWithADoubleAndUnit(CommandName("gap"),this);
    fGapCMD->SetGuidance("Set the gap between the bars.");
    fGapCMD->SetParameterName("Gap",false);
    fGapCMD->SetUnitCategory("Length");
    
    fVisibleCMD = new G4UIcmdWithABool(CommandName("visible"),this);
    fVisibleCMD->SetGuidance("Should the bar be drawn in pictures.");
    fVisibleCMD->SetParameterName("Visible",false);
    
  }
  
  
  ~ND280CubeScintMessenger() {
    //delete fShapeCMD;
    delete fLengthCMD;
    delete fBaseCMD;
    delete fHeightCMD;
    delete fFiberRadiusCMD;
    delete fHoleRadiusCMD;
    delete fCoatingThicknessCMD;
    //delete fCoatingRadiusCMD;
    delete fGapCMD;
    delete fVisibleCMD;
  }
  
  void SetNewValue(G4UIcommand *cmd, G4String val) {
    
    if (cmd == fLengthCMD) {
      fConstructor->SetLength(fLengthCMD->GetNewDoubleValue(val));
    }
    else if (cmd == fBaseCMD) {
      fConstructor->SetBase(fBaseCMD->GetNewDoubleValue(val));
    }
    else if (cmd == fHeightCMD) {
      fConstructor->SetHeight(fHeightCMD->GetNewDoubleValue(val));
    }
    else if (cmd == fCoatingThicknessCMD) {
      fConstructor->SetCoatingThickness(fCoatingThicknessCMD
					->GetNewDoubleValue(val));
    }
    //else if (cmd == fCoatingRadiusCMD) {
    //fConstructor->SetCoatingRadius(fCoatingRadiusCMD
    //				     ->GetNewDoubleValue(val));
    //}
    else if (cmd == fGapCMD) {
      fConstructor->SetGap(fGapCMD->GetNewDoubleValue(val));
    }
    //else if (cmd == fShapeCMD) {
    //if (val == "triangle") {
    //	fConstructor->SetShape(
    //			       ND280CubeScintConstructor::eTriangle);
    //}
    //else if (val == "rectangle") {
    //fConstructor->SetShape(
    //			     ND280CubeScintConstructor::eRectangle);
    //}
    //}
    else if (cmd == fFiberRadiusCMD) {
      fConstructor->SetFiberRadius(
				   fFiberRadiusCMD->GetNewDoubleValue(val));
    }
    else if (cmd == fHoleRadiusCMD) {
      fConstructor->SetHoleRadius(
				  fHoleRadiusCMD->GetNewDoubleValue(val));
    }
    else {
      ND280ConstructorMessenger::SetNewValue(cmd,val);
    }
  }
};
  
  ND280CubeScintConstructor::~ND280CubeScintConstructor() {;};

void ND280CubeScintConstructor::Init() {
  
  //SetShape(eTriangle);
  SetLength(10*cm);
  SetBase(10*cm);
  SetHeight(10*cm);
    
  SetFiberRadius(0.5*mm);
  SetHoleRadius(0.9*mm);
  
  SetCoatingThickness(0.25*mm);
  //SetCoatingRadius(0.0*mm);
  SetGap(0.0*mm);
  
  //SetSensitiveDetector(NULL);
  SetVisibility(false);

  SetCoatingMaterial("ScintillatorCoating");
  //SetScintillatorMaterial("Scintillator");
  SetScintillatorMaterial("FGDScintillator");  
  SetFiberMaterial("FiberCore");

  SetMessenger(new ND280CubeScintMessenger(this));
}

G4LogicalVolume* ND280CubeScintConstructor::GetPiece(void) {

  // Get the SD defined in ExN02DetectorConstrucion  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  ExN02TrackerSD *aTrackerSD = (ExN02TrackerSD*)SDman->FindSensitiveDetector("ExN02/Tracker"); 
  //
  
  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = InputPersistencyManager->GetXMLInput();

  G4VSolid* cube = NULL;
  G4VSolid* extrusion = NULL;
  G4VSolid* scintillator = NULL;
  G4VSolid* scintside = NULL;
  G4VSolid* scintcrnr = NULL;
  G4VSolid* fiberHole = NULL;
  G4VSolid* scintWithHole = NULL;
  G4VSolid* extrusionWithHole = NULL;
  
  // The offset of the core volume from the bar volume.
  double coreOffset = 0.0;
  
  //G4cout << "SuperFGD Cube base = " << GetBase() << G4endl;
  //G4cout << "SuperFGD Cube height = " << GetHeight() << G4endl;
  //G4cout << "SuperFGD Cube length = " << GetLength() << G4endl;
  //G4cout << "SuperFGD Hole radius = " << GetHoleRadius() << G4endl;
  
  G4VisAttributes *visAtt_Scint = new G4VisAttributes();
  visAtt_Scint->SetColor(0.5,0.5,0.5,0.); // gray
  visAtt_Scint->SetForceSolid(true);
  G4VisAttributes *visAtt_Coat = new G4VisAttributes();
  visAtt_Coat->SetColor(1.0,1.0,1.0); // white
  //visAtt_Coat->SetColor(0.5,0.5,0.5,0.); // gray
  visAtt_Coat->SetForceSolid(true);
  G4VisAttributes *visAtt_Hole = new G4VisAttributes();
  visAtt_Hole->SetColor(0.0,0.0,1.0,0.); // blue
  visAtt_Hole->SetForceSolid(true);

  // Build G4VSolid the plastic cube with coating and holes for WLS fibers

  extrusion = new G4Box(GetName()+"/Extrusion",
			GetBase()/2,
			GetHeight()/2,
			GetLength()/2
			);
  
  scintillator = new G4Box(GetName()+"/Extrusion/Core",
   			   GetBase()/2-GetCoatingThickness(),
  			   GetHeight()/2-GetCoatingThickness(),
  			   GetLength()/2-GetCoatingThickness()
			   );

  /*
  fiberHole = new G4Tubs(GetName()+"/Extrusion/Core/Hole",
			 0.0*cm,
			 GetHoleRadius(),
			 //GetLength()/2*2.,
			 GetLength()/2.,
			 0.*deg,
			 360.*deg);
  */

  // Define shifts of the 3 holes wrt each other (WLS fibers must not overlap)
  G4ThreeVector Pos_X = GetHolePosition_X(); // hole along X
  G4ThreeVector Pos_Y = GetHolePosition_Y(); // hole along Y
  G4ThreeVector Pos_Z = GetHolePosition_Z(); // hole along Z
  
  extrusionWithHole = extrusion;
  scintWithHole = scintillator;

  /*
  // subtract hole along Z
  //extrusionWithHole = new G4SubtractionSolid(GetName()+"/Extrusion",
  //extrusionWithHole,
  //fiberHole,
  //0,
  //Pos_Z
  //);
  scintWithHole = new G4SubtractionSolid(GetName()+"/Extrusion/Core",
					 scintWithHole,
					 fiberHole,
					 0,
					 Pos_Z
					 );
  */
  
  // subtract hole along X

  G4RotationMatrix* rotationAlongX = new G4RotationMatrix();
  rotationAlongX->rotateX(0 *degree); 
  rotationAlongX->rotateY(90*degree); 
  rotationAlongX->rotateZ(0 *degree);      

  /*
  //extrusionWithHole = new G4SubtractionSolid(GetName()+"/Extrusion",
  //extrusionWithHole,
  //fiberHole,
  //rotationAlongX,
  //Pos_X
  //);
  scintWithHole = new G4SubtractionSolid(GetName()+"/Extrusion/Core",
					 scintWithHole,
					 fiberHole,
					 rotationAlongX,
					 Pos_X
					 );
  */
  
  // subtract hole along Y

  G4RotationMatrix* rotationAlongY = new G4RotationMatrix();
  rotationAlongY->rotateX(90 *degree); 
  rotationAlongY->rotateY(0 *degree); 
  rotationAlongY->rotateZ(0 *degree);      

  /*
  //extrusionWithHole = new G4SubtractionSolid(GetName()+"/Extrusion",
  //extrusionWithHole,
  //fiberHole,
  //rotationAlongY,
  //Pos_Y
  //);
  scintWithHole = new G4SubtractionSolid(GetName()+"/Extrusion/Core",
					 scintWithHole,
					 fiberHole,
					 rotationAlongY,
					 Pos_Y
					 );   
  */
  
  // logical volumes

  coreOffset = 0.0;
  
  G4LogicalVolume* extrusionVolume
    = new G4LogicalVolume(extrusionWithHole,
			  FindMaterial(GetCoatingMaterial()),
			  GetName()+"/Extrusion");

  G4LogicalVolume *scintVolume;
  scintVolume = new G4LogicalVolume(scintWithHole,
				    FindMaterial(GetScintillatorMaterial()),
				    GetName()+"/Extrusion/Core");

  // Define the volume of plastic scintillator as sensitive detector
  scintVolume->SetSensitiveDetector( aTrackerSD ); 

  
  // Place the scintillator inside the extrusion volume
  new G4PVPlacement(0,                   // no rotation
		    G4ThreeVector(0,coreOffset,0),     // position
		    scintVolume,         // its logical volume
		    GetName()+"/Extrusion/Core",   // its name
		    extrusionVolume,     // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number
  
  if( GetND280XML()->GetXMLInvisSuperFGD() ){
    extrusionVolume->SetVisAttributes(G4VisAttributes::Invisible);
    scintVolume    ->SetVisAttributes(G4VisAttributes::Invisible);
  }
  else{
    extrusionVolume->SetVisAttributes(visAtt_Coat); 
    scintVolume    ->SetVisAttributes(visAtt_Scint);
  }
  
  //extrusionVolume->SetVisAttributes(G4VisAttributes::Invisible);
  //scintVolume    ->SetVisAttributes(visAtt_Coat);
  //scintVolume    ->SetVisAttributes(G4VisAttributes::Invisible);


  
  //
  // Build the WLS fiber
  // 
  // In order to avoid placement of independent WLS fibers outside of the G4VReplica cubes
  // add many WLS fiber along the cube (same size as the single cube).
  // When the G4VReplica is used in ND280SuperFGDConstructor.cc the cubes will be all attached
  // between each other and the same will happen among the WLS fiber segments.
  //

  G4VisAttributes *visAtt_Fiber = new G4VisAttributes();
  visAtt_Fiber->SetColor(0.0,1.0,0.0); // green
  visAtt_Fiber->SetForceSolid(true);
  
  if (GetFiberRadius()>GetHoleRadius()) { 
    G4Exception("ND280CubeScintConstructor::GetPiece",
  		"MyCode0002",FatalException,
  		"Fiber radius > Hole radius !");
  }
  else{

    G4VSolid* fiberHoleAlongX = new G4Tubs(GetName()+"/Extrusion/Hole/",
					   0.0*cm,
					   GetHoleRadius(),
					   GetBase()/2,
					   0.*deg,
					   360.*deg);
    G4VSolid* fiberAlongX = new G4Tubs(GetName()+"/Extrusion/Hole/FiberX",
				       0.0*cm,
				       GetFiberRadius(),
				       GetBase()/2,
				       0.*deg,
				       360.*deg);

    G4VSolid* fiberHoleAlongY = new G4Tubs(GetName()+"/Extrusion/Hole/",
					   0.0*cm,
					   GetHoleRadius(),
					   GetBase()/2,
					   0.*deg,
					   360.*deg);
    G4VSolid* fiberAlongY = new G4Tubs(GetName()+"/Extrusion/Hole/FiberY",
				       0.0*cm,
				       GetFiberRadius(),
				       GetHeight()/2,
				       0.*deg,
				       360.*deg);
    
    G4VSolid* fiberHoleAlongZ = new G4Tubs(GetName()+"/Extrusion/Hole/",
					   0.0*cm,
					   GetHoleRadius(),
					   GetBase()/2,
					   0.*deg,
					   360.*deg);
    G4VSolid* fiberAlongZ = new G4Tubs(GetName()+"/Extrusion/Hole/FiberZ",
				       0.0*cm,
				       GetFiberRadius(),
				       GetLength()/2,
				       0.*deg,
				       360.*deg);
    
    G4LogicalVolume *fiberHoleAlongXVolume
      = new G4LogicalVolume(fiberHoleAlongX,
			    FindMaterial("Air"),
			    GetName()+"/Extrusion/Hole");
    G4LogicalVolume *fiberAlongXVolume
      = new G4LogicalVolume(fiberAlongX,
			    FindMaterial(GetFiberMaterial()),
			    GetName()+"/Extrusion/Hole/FiberX");
    
    G4LogicalVolume *fiberHoleAlongYVolume
      = new G4LogicalVolume(fiberHoleAlongY,
			    FindMaterial("Air"),
			    GetName()+"/Extrusion/Hole");
    G4LogicalVolume *fiberAlongYVolume
      = new G4LogicalVolume(fiberAlongY,
			    FindMaterial(GetFiberMaterial()),
			    GetName()+"/Extrusion/FiberY");
    
    G4LogicalVolume *fiberHoleAlongZVolume
      = new G4LogicalVolume(fiberHoleAlongZ,
			    FindMaterial("Air"),
			    GetName()+"/Extrusion/Hole");
    G4LogicalVolume *fiberAlongZVolume
      = new G4LogicalVolume(fiberAlongZ,
			    FindMaterial(GetFiberMaterial()),
			    GetName()+"/Extrusion/Hole/FiberZ");
    
    if( GetND280XML()->GetXMLInvisSuperFGD() ){
      fiberAlongXVolume->SetVisAttributes(G4VisAttributes::Invisible);
      fiberAlongYVolume->SetVisAttributes(G4VisAttributes::Invisible);
      fiberAlongZVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }
    else{
      fiberHoleAlongXVolume->SetVisAttributes(visAtt_Hole);
      fiberHoleAlongYVolume->SetVisAttributes(visAtt_Hole);
      fiberHoleAlongZVolume->SetVisAttributes(visAtt_Hole);

      fiberAlongXVolume->SetVisAttributes(visAtt_Fiber);
      fiberAlongYVolume->SetVisAttributes(visAtt_Fiber);
      fiberAlongZVolume->SetVisAttributes(visAtt_Fiber);
    }

    
    // Place the WLS fibers inside the logVolume

    new G4PVPlacement(rotationAlongX,                  // no rotation         
		      GetHolePosition_X(),
		      fiberHoleAlongXVolume,         // its logical volume
		      GetName()+"/Extrusion/Hole",  // its name
		      scintVolume,     // its mother  volume
		      false,               // no boolean operations
		      0);           // copy number
    
    new G4PVPlacement(0,                  // no rotation         
		      G4ThreeVector(0,0,0), // no translation
		      fiberAlongXVolume,         // its logical volume
		      GetName()+"/Extrusion/Hole/FiberX",  // its name
		      fiberHoleAlongXVolume,     // its mother  volume
		      false,               // no boolean operations
		      0);           // copy number
    
    /*
    new G4PVPlacement(rotationAlongY,                  // no rotation         
		      GetHolePosition_Y(),
		      fiberAlongYVolume,         // its logical volume
		      GetName()+"/Extrusion/FiberY",  // its name
		      extrusionVolume,     // its mother  volume
		      false,               // no boolean operations
		      0);           // copy number

    new G4PVPlacement(0,                  // no rotation         
		      GetHolePosition_Z(),
		      fiberAlongZVolume,         // its logical volume
		      GetName()+"/Extrusion/FiberZ",  // its name
		      extrusionVolume,     // its mother  volume
		      false,               // no boolean operations
		      0);           // copy number
    */
  }

  return extrusionVolume;
}


  
//  double ND280CubeScintConstructor::GetTop(void) {
//  double top = 0.001*mm;
//if (fShape == eRectangle) top = fBase;
//    return top;
//}

//double ND280CubeScintConstructor::GetCenterToCenter(void) {
//  return GetBase()/2 + GetTop()/2 + GetGap();
//}

void ND280CubeScintConstructor::SetBase(double base) {
    fBase = base;
}

void ND280CubeScintConstructor::SetHeight(double height) {
    fHeight = height;
}

//void ND280CubeScintConstructor::SetSensitiveDetector(
//  G4VSensitiveDetector* s) {
//  fSensitiveDetector = s;
//}
