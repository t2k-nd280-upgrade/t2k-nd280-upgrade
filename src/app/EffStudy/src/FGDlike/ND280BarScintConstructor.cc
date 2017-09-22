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

#include "ND280BarScintConstructor.hh"

// used to keep a list of SD logical volumes
#include "G4RegionStore.hh"
#include <G4Region.hh> 
//

class ND280BarScintMessenger: public ND280ConstructorMessenger {

private:
    
  ND280BarScintConstructor *fConstructor;
  
  G4UIcmdWithAString*        fShapeCMD;
  G4UIcmdWithADoubleAndUnit* fLengthCMD;
  G4UIcmdWithADoubleAndUnit* fHeightCMD;
  G4UIcmdWithADoubleAndUnit* fCoatingThicknessCMD;
  G4UIcmdWithADoubleAndUnit* fGapCMD;
  G4UIcmdWithABool*          fVisibleCMD;

public:

  ND280BarScintMessenger(ND280BarScintConstructor* c) 
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
    
    fHeightCMD = new G4UIcmdWithADoubleAndUnit(CommandName("height"),this);
    fHeightCMD->SetGuidance("Set the bar height (default from base).");
    fHeightCMD->SetParameterName("Height",false);
    fHeightCMD->SetUnitCategory("Length");
    
    fCoatingThicknessCMD =
      new G4UIcmdWithADoubleAndUnit(CommandName("coating"),this);
    fCoatingThicknessCMD->
      SetGuidance("Set thickness of the coating on the bars.");
    fCoatingThicknessCMD->SetParameterName("TCoating",false);
    fCoatingThicknessCMD->SetUnitCategory("Length");
    
    fGapCMD = new G4UIcmdWithADoubleAndUnit(CommandName("gap"),this);
    fGapCMD->SetGuidance("Set the gap between the bars.");
    fGapCMD->SetParameterName("Gap",false);
    fGapCMD->SetUnitCategory("Length");
    
    fVisibleCMD = new G4UIcmdWithABool(CommandName("visible"),this);
    fVisibleCMD->SetGuidance("Should the bar be drawn in pictures.");
    fVisibleCMD->SetParameterName("Visible",false);
    
  }
  
  
  ~ND280BarScintMessenger() {
    //delete fShapeCMD;
    delete fLengthCMD;
    delete fHeightCMD;
    delete fCoatingThicknessCMD;
    delete fGapCMD;
    delete fVisibleCMD;
  }
  
  void SetNewValue(G4UIcommand *cmd, G4String val) {
    
    if (cmd == fLengthCMD) {
      fConstructor->SetLength(fLengthCMD->GetNewDoubleValue(val));
    }
    else if (cmd == fHeightCMD) {
      fConstructor->SetHeight(fHeightCMD->GetNewDoubleValue(val));
    }
    else if (cmd == fCoatingThicknessCMD) {
      fConstructor->SetCoatingThickness(fCoatingThicknessCMD
					->GetNewDoubleValue(val));
    }
    else if (cmd == fGapCMD) {
      fConstructor->SetGap(fGapCMD->GetNewDoubleValue(val));
    }
    //else if (cmd == fShapeCMD) {
    //if (val == "triangle") {
    //	fConstructor->SetShape(
    //			       ND280BarScintConstructor::eTriangle);
    //}
    //else if (val == "rectangle") {
    //fConstructor->SetShape(
    //			     ND280BarScintConstructor::eRectangle);
    //}
    //}
    else {
      ND280ConstructorMessenger::SetNewValue(cmd,val);
    }
  }
};
  
ND280BarScintConstructor::~ND280BarScintConstructor() {;}

void ND280BarScintConstructor::Init() {
  
  SetBase(10*CLHEP::mm);
  SetWidth(10*CLHEP::mm);
  SetHeight(10*CLHEP::mm);
  SetLength(10*CLHEP::mm);
  SetCoatingThickness(0.25*CLHEP::mm);
  SetGap(0.0*CLHEP::mm);
  
  //SetSensitiveDetector(NULL);
  SetVisibility(false);

  SetCoatingMaterial("ScintillatorCoating");
  SetScintillatorMaterial("FGDScintillator");  
  SetFiberMaterial("FiberCore");

  SetMessenger(new ND280BarScintMessenger(this));
}

G4LogicalVolume* ND280BarScintConstructor::GetPiece(void) {

  // Get the SD defined in ExN02DetectorConstrucion  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  ExN02TrackerSD *aTrackerSD = (ExN02TrackerSD*)SDman->FindSensitiveDetector("ExN02/Tracker"); 
  //
  
  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = InputPersistencyManager->GetXMLInput();

  G4VSolid* bar = NULL;
  G4VSolid* extrusion = NULL;
  G4VSolid* scintillator = NULL;
  G4VSolid* scintside = NULL;
  G4VSolid* scintcrnr = NULL;
  G4VSolid* fiberHole = NULL;
  G4VSolid* scintWithHole = NULL;
  G4VSolid* extrusionWithHole = NULL;

  // The offset of the core volume from the bar volume.
  double coreOffset = 0.0;
  
  G4VisAttributes *visAtt_Scint = new G4VisAttributes();
  //visAtt_Scint->SetColor(0.5,0.5,0.5,0.); // gray
  visAtt_Scint->SetColor(1.0,1.0,1.0); // white  
  visAtt_Scint->SetForceSolid(true);
  G4VisAttributes *visAtt_Coat = new G4VisAttributes();
  //visAtt_Coat->SetColor(1.0,1.0,1.0); // white
  //visAtt_Coat->SetColor(0.5,0.5,0.5,0.); // gray
  visAtt_Coat->SetColor(0.,1.,0.,0.); // green
  visAtt_Coat->SetForceSolid(true);
  G4VisAttributes *visAtt_Hole = new G4VisAttributes();
  visAtt_Hole->SetColor(0.0,0.0,1.0); // blue
  //visAtt_Coat->SetColor(0.5,0.5,0.5,0.); // gray
  visAtt_Hole->SetForceSolid(true);

  double width=0;
  double height=0;
  double length=0;
 
  double cladThickFrac = 0.02 * 2.; // 2% on both sides
  double scintWidth=0;
  double scintHeight=0;
  double scintLength=0;


  G4RotationMatrix* rotation = new G4RotationMatrix(); 
 
  if(GetBarDir()==conv::kAlongX){
    width  = GetAxisLength();
    height = GetBase();
    length = GetBase();

    scintWidth = GetAxisLength();
    scintHeight = GetBase()-2*GetCoatingThickness();
    scintLength = GetBase()-2*GetCoatingThickness();

    rotation->rotateX(0 *CLHEP::degree); 
    rotation->rotateY(90*CLHEP::degree); 
    rotation->rotateZ(0 *CLHEP::degree); 
  }
  else if(GetBarDir()==conv::kAlongY){
    width  = GetBase();
    height = GetAxisLength();
    length = GetBase();

    scintWidth = GetBase()-2*GetCoatingThickness();
    scintHeight = GetAxisLength();
    scintLength = GetBase()-2*GetCoatingThickness();
    
    rotation->rotateX(90*CLHEP::degree); 
    rotation->rotateY(0 *CLHEP::degree); 
    rotation->rotateZ(0 *CLHEP::degree);      
  }
  else if(GetBarDir()==conv::kAlongZ){
    width  = GetBase();
    height = GetBase();
    length = GetAxisLength();

    scintWidth = GetBase()-2*GetCoatingThickness();
    scintHeight = GetBase()-2*GetCoatingThickness();
    scintLength = GetAxisLength();
 
    rotation->rotateX(0*CLHEP::degree); 
    rotation->rotateY(0*CLHEP::degree); 
    rotation->rotateZ(0*CLHEP::degree);      
  }

  //G4cout << "GetCoatingThickness() = " << GetCoatingThickness() << G4endl;
  //G4cout << "width = " << width << G4endl;
  //G4cout << "height = " << height << G4endl;
  //G4cout << "length = " << length << G4endl;
  //G4cout << "scintWidth = " << scintWidth << G4endl;
  //G4cout << "scintHeight = " << scintHeight << G4endl;
  //G4cout << "scintLength = " << scintLength << G4endl; 

  // Set the dimension in the XYZ frame
  SetWidth(width);
  SetHeight(height);
  SetLength(length);
  
  //G4cout << GetName() << G4endl;
  //G4cout << "width = " << GetWidth() << G4endl;
  //G4cout << "height = " << GetHeight() << G4endl;
  //G4cout << "length = " << GetLength() << G4endl;
  
  // Build G4VSolid the plastic cube with coating and holes for WLS fibers

  extrusion = new G4Box(GetName()+"/Extrusion",
			GetWidth()/2,
			GetHeight()/2,
			GetLength()/2
			);
 
  scintillator = new G4Box(GetName()+"/Extrusion/Core",
			   scintWidth/2,
  			   scintHeight/2,
			   scintLength/2
			   );
  
  
  fiberHole = new G4Tubs(GetName()+"/Extrusion/Core/Hole",
			 0.0*CLHEP::cm,
			 GetHoleRadius(),
			 GetAxisLength()/2.,
			 0.*CLHEP::deg,
			 360.*CLHEP::deg);
 
  
  // Define shifts of the 3 holes wrt each other (WLS fibers must not overlap)
  G4ThreeVector Pos_X = G4ThreeVector(0,0,0); // hole along X
  G4ThreeVector Pos_Y = G4ThreeVector(0,0,0); // hole along Y
  G4ThreeVector Pos_Z = G4ThreeVector(0,0,0); // hole along Z
  
  extrusionWithHole = extrusion;
  scintWithHole = scintillator;

  /*
  // subtract hole along Z
  //extrusionWithHole = new G4SubtractionSolid(GetName()+"/Extrusion",
  //extrusionWithHole,
  //fiberHole,
  //rotation,
  //Pos_Z
  //);
  scintWithHole = new G4SubtractionSolid(GetName()+"/Extrusion/Core",
					 scintWithHole,
					 fiberHole,
					 rotation,
					 Pos_Z
					 );
  */
     
  // logical volumes

  coreOffset = 0.0;
  
  G4LogicalVolume* extrusionVolume
    = new G4LogicalVolume(extrusionWithHole,
			  FindMaterial(GetCoatingMaterial()),
			  GetName()+"/Extrusion");

  G4LogicalVolume *scintVolume
    = new G4LogicalVolume(scintWithHole,
			  FindMaterial(GetScintillatorMaterial()),
			  GetName()+"/Extrusion/Core");

  // Define the volume of plastic scintillator as sensitive detector
  scintVolume->SetSensitiveDetector( aTrackerSD ); 
  
  G4LogicalVolume *fiberHoleVolume
    = new G4LogicalVolume(fiberHole,
			  FindMaterial("Air"),
			  GetName()+"/Extrusion/Core/Hole");
  

  // Place the scintillator inside the extrusion volume
  new G4PVPlacement(0,                   // no rotation
		    G4ThreeVector(0,coreOffset,0),     // position
		    scintVolume,         // its logical volume
		    GetName()+"/Extrusion/Core",   // its name
		    extrusionVolume,     // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number


  // Place the hole inside the scintillator volume
  new G4PVPlacement(rotation,                   // no rotation
		    G4ThreeVector(0,coreOffset,0),     // position
		    fiberHoleVolume,         // its logical volume
		    GetName()+"/Extrusion/Core/Hole",   // its name
		    scintVolume,     // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number
  
  if( GetND280XML()->GetXMLInvisFGDlike() ){
    extrusionVolume->SetVisAttributes(G4VisAttributes::Invisible);
    scintVolume    ->SetVisAttributes(G4VisAttributes::Invisible);
    fiberHoleVolume->SetVisAttributes(G4VisAttributes::Invisible);
  }
  else{
    extrusionVolume->SetVisAttributes(visAtt_Coat); 
    scintVolume    ->SetVisAttributes(visAtt_Scint);
    fiberHoleVolume->SetVisAttributes(visAtt_Hole);
  }

  //extrusionVolume->SetVisAttributes(G4VisAttributes::Invisible);
  //fiberHoleVolume->SetVisAttributes(G4VisAttributes::Invisible);


  //
  // Build the WLS fiber
  // 
  // In order to avoid placement of independent WLS fibers outside of the G4VReplica cubes
  // add many WLS fiber along the cube (same size as the single cube).
  // When the G4VReplica is used in ND280FGDlikeConstructor.cc the cubes will be all attached
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
    
    G4VSolid* fiber = new G4Tubs(GetName()+"/Extrusion/Fiber",
				 0.0*CLHEP::cm,
				 GetFiberRadius(),
				 GetAxisLength()/2,
				 0.*CLHEP::deg,
				 360.*CLHEP::deg);

    G4LogicalVolume *fiberVolume
      = new G4LogicalVolume(fiber,
			    FindMaterial(GetFiberMaterial()),
			    GetName()+"/Extrusion/Fiber");
    
    if( GetND280XML()->GetXMLInvisFGDlike() ){
      fiberVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }
    else{
      fiberVolume->SetVisAttributes(visAtt_Fiber);
    }

    // Place the WLS fibers inside the logVolume
    
    new G4PVPlacement(0,                  // already rotated hole!!!      
		      G4ThreeVector(0,0,0),
		      fiberVolume,         // its logical volume
		      GetName()+"/Extrusion/Fiber",  // its name
		      //extrusionVolume,     // its mother  volume
		      fiberHoleVolume,     // its mother  volume
		      false,               // no boolean operations
		      0);           // copy number
  }

  return extrusionVolume;

}



//void ND280BarScintConstructor::SetSensitiveDetector(
//  G4VSensitiveDetector* s) {
//  fSensitiveDetector = s;
//}
