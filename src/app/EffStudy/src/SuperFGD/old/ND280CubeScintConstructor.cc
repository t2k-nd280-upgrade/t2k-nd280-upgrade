#include <globals.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>

#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Trap.hh>

#include "ND280CubeScintConstructor.hh"

// used to keep a list of SD logical volumes
#include "G4RegionStore.hh"
#include <G4Region.hh> 
//

class ND280CubeScintMessenger: public ND280ConstructorMessenger {

private:
    
    ND280CubeScintConstructor *fConstructor;

  //G4UIcmdWithAString*        fShapeCMD;
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
    
    fCoatingRadiusCMD =
      new G4UIcmdWithADoubleAndUnit(CommandName("coatrad"),this);
    fCoatingRadiusCMD->
      SetGuidance("Set inner radius of the corner bar coating.");
    fCoatingRadiusCMD->SetParameterName("RCoating",false);
    fCoatingRadiusCMD->SetUnitCategory("Length");
    
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
    delete fCoatingRadiusCMD;
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
    else if (cmd == fCoatingRadiusCMD) {
      fConstructor->SetCoatingRadius(fCoatingRadiusCMD
				     ->GetNewDoubleValue(val));
    }
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
  SetCoatingRadius(0.0*mm);
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
  
  // // NEW ND280UPGRADE
  //G4Region* SDRegion = G4RegionStore::GetInstance()->
  //GetRegion("SDRegion",false);
  //


  G4VSolid* extrusion = NULL;
  G4VSolid* scintillator = NULL;
  G4VSolid* scintside = NULL;
  G4VSolid* scintcrnr = NULL;
  
  // The offset of the core volume from the bar volume.
  double coreOffset = 0.0;
  
  G4cout << "Cube base = " << GetBase() << G4endl;
  G4cout << "Cube height = " << GetHeight() << G4endl;
  G4cout << "Cube length = " << GetLength() << G4endl;
  
  
  G4VisAttributes *visAtt_Scint = new G4VisAttributes();
  visAtt_Scint->SetColor(0.5,0.5,0.5,0.); // gray
  visAtt_Scint->SetForceSolid(true);
  G4VisAttributes *visAtt_Coat = new G4VisAttributes();
  visAtt_Coat->SetColor(1.0,1.0,1.0); // white
  visAtt_Coat->SetForceSolid(true);
  G4VisAttributes *visAtt_FiberHole = new G4VisAttributes();
  visAtt_FiberHole->SetColor(0.0,0.0,0.0); // black
  visAtt_FiberHole->SetForceSolid(true);
  //G4VisAttributes *visAtt_Fiber = new G4VisAttributes();
  //visAtt_Fiber->SetColor(0.0,1.0,0.0); // green
  //visAtt_Fiber->SetForceSolid(true);
  
  extrusion = new G4Box(GetName(),
			GetBase()/2,
			GetHeight()/2,
			GetLength()/2);
  scintillator = new G4Box(GetName()+"/Core",
			   GetBase()/2-GetCoatingThickness(),
			   GetHeight()/2-GetCoatingThickness(),
			   //GetLength()/2-GetCoatingThickness()
			   GetLength()/2
			   );
  
  coreOffset = 0.0;
  
  G4LogicalVolume* logVolume
    = new G4LogicalVolume(extrusion,
			  FindMaterial(GetCoatingMaterial()),
			  GetName());

  logVolume->SetVisAttributes(visAtt_Coat);
  //logVolume->SetVisAttributes(G4VisAttributes::Invisible);

  G4LogicalVolume *scintVolume;
  
  scintVolume = new G4LogicalVolume(scintillator,
				    FindMaterial(GetScintillatorMaterial()),
				    GetName()+"/Core");

  scintVolume->SetVisAttributes(visAtt_Scint);
  
  // NOT COMMENTED IN nd280mc
  //scintVolume->SetVisAttributes(G4VisAttributes::Invisible);
  //scintVolume->SetSensitiveDetector(GetSensitiveDetector());
  
  //if(SDRegion) SDRegion->AddRootLogicalVolume(scintVolume); // NEW ND280 UPGRADE
  
  
  new G4PVPlacement(0,                   // no rotation
		    G4ThreeVector(0,coreOffset,0),     // position
		    scintVolume,         // its logical volume
		    GetName()+"/Core",   // its name
		    logVolume,           // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number
  
  
  //if (GetFiberRadius()>GetHoleRadius()) { 
  //G4Exception("ND280CubeScintConstructor::GetPiece",
  //"MyCode0002",FatalException,
  //"Fiber radius > Hole radius !");
  //}
  //else{
  G4VSolid* fiberHole = new G4Tubs(GetName()+"/Core/Hole",
				   0.0*cm,
				   GetHoleRadius(),
				   GetLength()/2,
				   0.*deg,
				   360.*deg);
  
  //G4VSolid* fiber = new G4Tubs(GetName()+"/Core/Hole/Fiber",
  //0.0*cm,
  //GetFiberRadius(),
  //GetLength()/2,
  //0.*deg,
  //360.*deg);
    
  G4LogicalVolume *holeVolume
    = new G4LogicalVolume(fiberHole,
			  FindMaterial("Air"),
			  GetName()+"/Core/Hole");
  //holeVolume ->SetVisAttributes(G4VisAttributes::Invisible);
  holeVolume->SetVisAttributes(visAtt_FiberHole);
  
  //G4LogicalVolume *fiberVolume
  //= new G4LogicalVolume(fiber,
  //FindMaterial(GetFiberMaterial()),
  //GetName()+"/Core/Hole/Fiber");
    
  // //fiberVolume->SetVisAttributes(G4VisAttributes::Invisible);
  //fiberVolume->SetVisAttributes(visAtt_Fiber);
  
  new G4PVPlacement(0,                   // no rotation
		    G4ThreeVector(),     // position at (0,0,0)
		    holeVolume,          // its logical volume
		    GetName()+"/Core/Hole",   // its name
		    scintVolume,         // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number
  
  //new G4PVPlacement(0,                   // no rotation
  //G4ThreeVector(),     // position at (0,0,0)
  //fiberVolume,         // its logical volume
  //GetName()+"/Core/Hole/Fiber",  // its name
  //holeVolume,          // its mother  volume
  //false,               // no boolean operations
  //0);                  // copy number
  
  //}



  //
  // not commented in nd280mc
  //
  // if (GetVisible()) {
  //   G4VisAttributes *visAtt = new G4VisAttributes();
  //   visAtt->SetColor(0,1,1,1);
  //   visAtt->SetForceSolid(true);
  //   logVolume->SetVisAttributes(visAtt);
  // }
  // else {
  //   logVolume->SetVisAttributes(G4VisAttributes::Invisible);
  // }

  
  return logVolume;
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