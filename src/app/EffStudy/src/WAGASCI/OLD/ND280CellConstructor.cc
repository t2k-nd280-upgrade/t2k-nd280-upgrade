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

#include "ND280CellConstructor.hh"

// used to keep a list of SD logical volumes
#include "G4RegionStore.hh"
#include <G4Region.hh> 
//

class ND280CellMessenger: public ND280ConstructorMessenger {

private:
    
  ND280CellConstructor *fConstructor;
  
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

  ND280CellMessenger(ND280CellConstructor* c) 
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
  
  
  ~ND280CellMessenger() {
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
    //			       ND280CellConstructor::eTriangle);
    //}
    //else if (val == "rectangle") {
    //fConstructor->SetShape(
    //			     ND280CellConstructor::eRectangle);
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
  
  ND280CellConstructor::~ND280CellConstructor() {;};

void ND280CellConstructor::Init() {
  
  //SetShape(eTriangle);
  SetLength(10*cm);
  SetBase(10*cm);
  SetHeight(10*cm);
    
  SetFiberRadius(0.5*mm);
  SetHoleRadius(0.9*mm);
  
  SetCoatingThickness(0.25*mm);
  //SetCoatingRadius(0.0*mm);
  SetGap(0.0*mm);
  
  SetScintThickness(3.0*mm);

  //SetSensitiveDetector(NULL);
  SetVisibility(false);

  SetCoatingMaterial("ScintillatorCoating");
  //SetScintillatorMaterial("Scintillator");
  SetScintillatorMaterial("WAGASCIScintillator");  
  SetFiberMaterial("FiberCore");

  SetMessenger(new ND280CellMessenger(this));
}

G4LogicalVolume* ND280CellConstructor::GetPiece(void) {
  
  // NEW ND280UPGRADE
  G4Region* SDRegion = G4RegionStore::GetInstance()->
    GetRegion("SDRegion",false);
  //

  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = InputPersistencyManager->GetXMLInput();

  G4VSolid* cell = NULL;
  G4VSolid* scintAlongX = NULL;
  G4VSolid* scintAlongZ = NULL;
  G4VSolid* scintHoriz = NULL;
  G4VSolid* slitAlongX = NULL;
  G4VSolid* slitAlongZ = NULL;

  G4VSolid* fiberHole = NULL;
  G4VSolid* scintWithHole = NULL;
  G4VSolid* extrusionWithHole = NULL;

  // The offset of the core volume from the bar volume.
  double OffsetX = 0.0;
  double OffsetY = 0.0;
  double OffsetZ = 0.0;
  
  G4VisAttributes *visAtt_Cell = new G4VisAttributes();
  visAtt_Cell->SetColor(0.0,0.0,1.0); // blue
  //visAtt_Cell->SetColor(0.5,0.5,0.5,0.); // gray
  visAtt_Cell->SetForceSolid(true);

   G4VisAttributes *visAtt_Scint = new G4VisAttributes();
  visAtt_Scint->SetColor(1.0,1.0,1.0); // white
  //visAtt_Scint->SetColor(0.5,0.5,0.5,0.); // gray
  visAtt_Scint->SetForceSolid(true);
  G4VisAttributes *visAtt_Coat = new G4VisAttributes();
  visAtt_Coat->SetColor(1.0,1.0,1.0); // white
  //visAtt_Coat->SetColor(0.5,0.5,0.5,0.); // gray
  visAtt_Coat->SetForceSolid(true);


  // Build G4VSolid the plastic cell with coating and holes for WLS fibers

  cell = new G4Box(GetName(),
			GetBase()/2,
			GetHeight()/2,
			GetLength()/2
			);
  

			 
  // Define shifts of the 3 holes wrt each other (WLS fibers must not overlap)
  G4ThreeVector Pos_X = GetHolePosition_X(); // hole along X
  G4ThreeVector Pos_Y = GetHolePosition_Y(); // hole along Y
  G4ThreeVector Pos_Z = GetHolePosition_Z(); // hole along Z
  
  
  
  // logical volumes
  
  G4LogicalVolume* logVolume
    = new G4LogicalVolume(cell,
			  FindMaterial("Air"),
			  GetName());


  //
  // Scintillator bar is along X in the XZ plane
  // --> thickness along Z axis
  //
  
  double ScintWidth = GetBase(); 
  double ScintHeight = GetHeight() - GetScintThickness() - GetSpaceLayerY();
  double ScintLength = GetScintThickness(); 
  scintAlongX = new G4Box(GetName()+"/ScintAlongX",
			  ScintWidth/2,
			  ScintHeight/2,
			  ScintLength/2	
			  );

  double SlitWidth  = GetScintThickness();
  double SlitHeight = ScintHeight/2;
  double SlitLength = ScintLength*1.2; 
  slitAlongX = new G4Box("slitAlongX",
			 SlitWidth/2,
			 SlitHeight/2,
			 SlitLength/2			
			 );

  double SlitPosX = ScintWidth/2 - SlitWidth/2;
  double SlitPosY = -SlitHeight/2;  
  double SlitPosZ = 0;    
  scintAlongX = new G4SubtractionSolid(GetName()+"/ScintAlongX",
   				       scintAlongX,
   				       slitAlongX,
   				       0,
   				       G4ThreeVector(SlitPosX,SlitPosY,SlitPosZ));

  G4LogicalVolume *scintAlongXVolume;
  scintAlongXVolume = new G4LogicalVolume(scintAlongX,
					  FindMaterial(GetScintillatorMaterial()),
					  GetName()+"/ScintAlongX");
  
  OffsetX = 0;
  // leave space for horizontal bar size and the space between them 
  OffsetY = GetScintThickness()/2 + GetSpaceLayerY()/2;  
  // extreme position of the cell along Z
  OffsetZ = GetLength()/2 - GetScintThickness()/2;  

  new G4PVPlacement(0,                   // no rotation
		    G4ThreeVector(OffsetX,OffsetY,OffsetZ), // position
		    scintAlongXVolume,         // its logical volume
		    GetName()+"/ScintAlongX",   // its name
		    logVolume,     // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number
    

  //
  // Scintillator bar is along Z in the XZ plane
  // --> thickness along X axis
  //
  
  ScintWidth = GetScintThickness();
  ScintHeight = GetHeight() - GetScintThickness() - GetSpaceLayerY();
  ScintLength = GetLength(); 
  scintAlongZ = new G4Box(GetName()+"/ScintAlongZ",
			  ScintWidth/2,
			  ScintHeight/2,
			  ScintLength/2	
			  );
  
  SlitWidth  = ScintWidth*1.2;
  SlitHeight = ScintHeight/2;
  SlitLength = GetScintThickness();
  slitAlongZ = new G4Box("slitAlongZ",
			 SlitWidth/2,
			 SlitHeight/2,
			 SlitLength/2			
			 );
  
  SlitPosX = 0;
  SlitPosY = SlitHeight/2;  
  SlitPosZ = ScintLength/2 - SlitLength/2;     // z is always 75% of the scintillator width (not applied...)
  scintAlongZ = new G4SubtractionSolid(GetName()+"/ScintAlongZ",
   				       scintAlongZ,
   				       slitAlongZ,
   				       0,
   				       G4ThreeVector(SlitPosX,SlitPosY,SlitPosZ));

  G4LogicalVolume *scintAlongZVolume;
  scintAlongZVolume = new G4LogicalVolume(scintAlongZ,
					  FindMaterial(GetScintillatorMaterial()),
					  GetName()+"/ScintAlongZ");

  // extreme position of the cell along X
  OffsetX = GetBase()/2 - GetScintThickness()/2;  
  // leave space for horizontal bar size and the space between them 
  OffsetY = GetScintThickness()/2 + GetSpaceLayerY()/2;  
  OffsetZ = 0; 
  
  new G4PVPlacement(0,                   // no rotation
		    G4ThreeVector(OffsetX,OffsetY,OffsetZ), // position
		    scintAlongZVolume,         // its logical volume
		    GetName()+"/ScintAlongZ",   // its name
		    logVolume,     // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number
   

  //
  // Scintillator bar Horizontal (along Z) on bottom of the XZ grid
  // --> thickness along Y axis
  //
  
  //It should be 25% to be at the start of the scintillator, and therefore, overlaps with the 75% of horizontal scintillators   

  scintHoriz = new G4Box(GetName()+"/ScintHoriz",
			 GetBase()/2,
			 GetScintThickness()/2,
			 GetLength()/2 
			 );

  G4LogicalVolume *scintHorizVolume;
  scintHorizVolume = new G4LogicalVolume(scintHoriz,
					 FindMaterial(GetScintillatorMaterial()),
					 GetName()+"/ScintHoriz");
  
  OffsetX = 0;  
  // extreme position of the cell along Y
  OffsetY = -GetHeight()/2 + GetScintThickness()/2; 
  OffsetZ = 0;  

 
  new G4PVPlacement(0,                   // no rotation
		    G4ThreeVector(OffsetX,OffsetY,OffsetZ), // position
		    scintHorizVolume,         // its logical volume
		    GetName()+"/ScintHoriz",   // its name
		    logVolume,     // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number
  






  if( GetND280XML()->GetXMLInvisSuperFGD() ){
    scintAlongXVolume->SetVisAttributes(G4VisAttributes::Invisible);
    logVolume        ->SetVisAttributes(G4VisAttributes::Invisible);
 }
  else{
    scintAlongXVolume->SetVisAttributes(visAtt_Scint);
    logVolume        ->SetVisAttributes(visAtt_Cell);
  }
  //logVolume        ->SetVisAttributes(G4VisAttributes::Invisible);




  return logVolume;
}


  
//  double ND280CellConstructor::GetTop(void) {
//  double top = 0.001*mm;
//if (fShape == eRectangle) top = fBase;
//    return top;
//}

//double ND280CellConstructor::GetCenterToCenter(void) {
//  return GetBase()/2 + GetTop()/2 + GetGap();
//}

void ND280CellConstructor::SetBase(double base) {
    fBase = base;
}

void ND280CellConstructor::SetHeight(double height) {
    fHeight = height;
}

//void ND280CellConstructor::SetSensitiveDetector(
//  G4VSensitiveDetector* s) {
//  fSensitiveDetector = s;
//}
