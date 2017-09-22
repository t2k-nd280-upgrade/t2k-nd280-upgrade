#include <globals.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>

#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Trap.hh>

#include "ND280ExtrudedScintConstructor.hh"

// used to keep a list of SD logical volumes
#include "G4RegionStore.hh"
#include <G4Region.hh> 
//

class ND280ExtrudedScintMessenger: public ND280ConstructorMessenger {

private:
    
    ND280ExtrudedScintConstructor *fConstructor;

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

  ND280ExtrudedScintMessenger(ND280ExtrudedScintConstructor* c) 
    : ND280ConstructorMessenger(c,"Control the T2K world geometry."),
      fConstructor(c) {
        
        fShapeCMD = new G4UIcmdWithAString(CommandName("shape"),this);
        fShapeCMD->SetGuidance("Set the extruded cross section of the bar.");
        fShapeCMD->SetParameterName("Shape",false);
        fShapeCMD->SetCandidates("triangle rectangle");
        
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
  
    ~ND280ExtrudedScintMessenger() {
        delete fShapeCMD;
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
        else if (cmd == fShapeCMD) {
            if (val == "triangle") {
                fConstructor->SetShape(
                    ND280ExtrudedScintConstructor::eTriangle);
            }
            else if (val == "rectangle") {
                fConstructor->SetShape(
                    ND280ExtrudedScintConstructor::eRectangle);
            }
        }
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

ND280ExtrudedScintConstructor::~ND280ExtrudedScintConstructor() {;}

void ND280ExtrudedScintConstructor::Init() {

    SetShape(eTriangle);
    SetLength(10*CLHEP::cm);
    SetBase(3.0*CLHEP::cm);
    SetHeight(1.5*CLHEP::cm);
    
    SetFiberRadius(0.5*CLHEP::mm);
    SetHoleRadius(0.9*CLHEP::mm);

    SetCoatingThickness(0.25*CLHEP::mm);
    SetCoatingRadius(0.0*CLHEP::mm);
    SetGap(0.0*CLHEP::mm);

    SetSensitiveDetector(NULL);
    SetVisibility(false);

    SetCoatingMaterial("ScintillatorCoating");
    SetScintillatorMaterial("Scintillator");
    SetFiberMaterial("FiberCore");

    SetMessenger(new ND280ExtrudedScintMessenger(this));
}

G4LogicalVolume* ND280ExtrudedScintConstructor::GetPiece(void) {
  
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
    
    if ( fShape == eRectangle ) {

        extrusion = new G4Box(GetName(),
                              GetBase()/2,
                              GetHeight()/2,
                              GetLength()/2);
        scintillator = new G4Box(GetName()+"/Core",
                              GetBase()/2-GetCoatingThickness()
                                         -GetCoatingRadius(),
                              GetHeight()/2-GetCoatingThickness()
                                           -GetCoatingRadius(),
                              GetLength()/2);
        if (GetCoatingRadius() > 0.*CLHEP::mm) {
           scintside = new G4Box(GetName()+"/SideOfBar",
                                 GetBase()/2-GetCoatingThickness()
                                            -GetCoatingRadius(),
                                 GetCoatingRadius()/2,
                                 GetLength()/2);

           scintcrnr = new G4Tubs(GetName()+"/CrnrOfBar",
                                  0.0*CLHEP::cm,
                                  GetCoatingRadius(),
                                  GetLength()/2,
                                  0.*CLHEP::deg,
                                  90.*CLHEP::deg);
        }

        coreOffset = 0.0;
    }
    else {

        extrusion = new G4Trap(GetName(),
                               GetLength()/2, 0, 0,
                               GetHeight()/2, GetBase()/2, GetTop()/2, 0,
                               GetHeight()/2, GetBase()/2, GetTop()/2, 0);
        
        // Find the length of a diagonal side.
        double sideLength = std::sqrt(4*GetHeight()*GetHeight() 
                                      + GetBase()*GetBase());
        // Find the inset of the active scintillator from the peak and bottom
        // of the bar.
        double bottomInset = GetCoatingThickness();
        double topInset = GetCoatingThickness()*sideLength/GetBase();
        double heightInset = 0.5*(topInset+bottomInset);
        coreOffset = 0.5*(bottomInset - topInset);
        
        // Find the inset of the active scintillator from the sides points of
        // the bar.
        double baseInset = GetCoatingThickness()*sideLength/(2*GetHeight());
        
        scintillator = new G4Trap(GetName()+"/Core",
                                  GetLength()/2, 0, 0,
                                  GetHeight()/2-heightInset,
                                  GetBase()/2-baseInset,
                                  GetTop()/2, 0,
                                  GetHeight()/2-heightInset,
                                  GetBase()/2-baseInset,
                                  GetTop()/2, 0);
    }

    G4LogicalVolume* logVolume
        = new G4LogicalVolume(extrusion,
                              FindMaterial(GetCoatingMaterial()),
                              GetName());

    G4LogicalVolume *scintVolume;

    scintVolume = new G4LogicalVolume(scintillator,
                                      FindMaterial(GetScintillatorMaterial()),
                                      GetName()+"/Core");

    scintVolume->SetVisAttributes(G4VisAttributes::Invisible);
    scintVolume->SetSensitiveDetector(GetSensitiveDetector());
    
    //if(SDRegion) SDRegion->AddRootLogicalVolume(scintVolume); // NEW ND280 UPGRADE
	

    new G4PVPlacement(0,                   // no rotation
                      G4ThreeVector(0,coreOffset,0),     // position
                      scintVolume,         // its logical volume
                      GetName()+"/Core",   // its name
                      logVolume,           // its mother  volume
                      false,               // no boolean operations
                      0);                  // copy number

    if (fShape == eRectangle && GetCoatingRadius() > 0.*CLHEP::mm) {
        
        G4LogicalVolume *scintSide;
        scintSide = new G4LogicalVolume(scintside,
                                        FindMaterial(GetScintillatorMaterial()),
                                        GetName()+"/SideOfBar");
        G4LogicalVolume *scintCrnr;
        scintCrnr = new G4LogicalVolume(scintcrnr,
                                        FindMaterial(GetScintillatorMaterial()),
                                        GetName()+"/CrnrOfBar");
        
        scintSide->SetVisAttributes(G4VisAttributes::Invisible);
        scintSide->SetSensitiveDetector(GetSensitiveDetector());
        scintCrnr->SetVisAttributes(G4VisAttributes::Invisible);
        scintCrnr->SetSensitiveDetector(GetSensitiveDetector());

	//if(SDRegion) SDRegion->AddRootLogicalVolume(scintSide); // NEW ND280 UPGRADE
	//if(SDRegion) SDRegion->AddRootLogicalVolume(scintCrnr); // NEW ND280 UPGRADE

        
        double x = GetBase()/2-GetCoatingThickness()-GetCoatingRadius()/2;
        double y = GetHeight()/2-GetCoatingThickness()-GetCoatingRadius()/2;
        
        new G4PVPlacement(0,                   // no rotation
                          G4ThreeVector(0,coreOffset-y,0),     // position
                          scintSide,           // its logical volume
                          GetName()+"/SideOfBar",   // its name
                          logVolume,           // its mother  volume
                          false,               // no boolean operations
                          0);                  // copy number
        new G4PVPlacement(0,                   // no rotation
                          G4ThreeVector(0,coreOffset+y,0),     // position
                          scintSide,           // its logical volume
                          GetName()+"/SideOfBar",   // its name
                          logVolume,           // its mother  volume
                          false,               // no boolean operations
                          1);                  // copy number
        
        G4RotationMatrix* g4rot = new G4RotationMatrix();
        *g4rot = stringToRotationMatrix("Z90");
        *g4rot = g4rot->inverse();
        if (*g4rot == G4RotationMatrix()) g4rot = NULL;
        
        new G4PVPlacement(g4rot,               // 90 deg rotation about Z
                          G4ThreeVector(x,0,0),      // position
                          scintSide,           // its logical volume
                          GetName()+"/SideOfBar",   // its name
                          logVolume,           // its mother  volume
                          false,               // no boolean operations
                          2);                  // copy number
        new G4PVPlacement(g4rot,               // 90 deg rotation about Z
                          G4ThreeVector(-x,0,0),     // position
                          scintSide,           // its logical volume
                          GetName()+"/SideOfBar",   // its name
                          logVolume,           // its mother  volume
                          false,               // no boolean operations
                          3);                  // copy number
        
        x = GetBase()/2-GetCoatingThickness()-GetCoatingRadius();
        y = GetHeight()/2-GetCoatingThickness()-GetCoatingRadius();
        
        new G4PVPlacement(0,                   // no rotation
                          G4ThreeVector(x,y,0),      // position
                          scintCrnr,           // its logical volume
                          GetName()+"/CrnrOfBar",   // its name
                          logVolume,           // its mother  volume
                          false,               // no boolean operations
                          0);                  // copy number
        
        new G4PVPlacement(g4rot,               // 90 deg rotation about Z
                          G4ThreeVector(-x,y,0),     // position
                          scintCrnr,           // its logical volume
                          GetName()+"/CrnrOfBar",   // its name
                          logVolume,           // its mother  volume
                          false,               // no boolean operations
                          1);                  // copy number
        
        g4rot = new G4RotationMatrix();
        *g4rot = stringToRotationMatrix("Z180");
        *g4rot = g4rot->inverse();
        if (*g4rot == G4RotationMatrix()) g4rot = NULL;
        
        new G4PVPlacement(g4rot,               // 180 deg rotation about Z
                          G4ThreeVector(-x,-y,0),    // position
                          scintCrnr,           // its logical volume
                          GetName()+"/CrnrOfBar",   // its name
                          logVolume,           // its mother  volume
                          false,               // no boolean operations
                          2);                  // copy number
        
        g4rot = new G4RotationMatrix();
        *g4rot = stringToRotationMatrix("Z270");
        *g4rot = g4rot->inverse();
        if (*g4rot == G4RotationMatrix()) g4rot = NULL;
        
        new G4PVPlacement(g4rot,               // 270 deg rotation about Z
                          G4ThreeVector(x,-y,0),     // position
                          scintCrnr,           // its logical volume
                          GetName()+"/CrnrOfBar",   // its name
                          logVolume,           // its mother  volume
                          false,               // no boolean operations
                          3);                  // copy number
    }
    
    if (GetFiberRadius()<GetHoleRadius()) {
        G4VSolid* fiberHole = new G4Tubs(GetName()+"/Core/Hole",
                                         0.0*CLHEP::cm,
                                         GetHoleRadius(),
                                         GetLength()/2,
                                         0.*CLHEP::deg,
                                         360.*CLHEP::deg);
        
        G4VSolid* fiber = new G4Tubs(GetName()+"/Core/Hole/Fiber",
                                     0.0*CLHEP::cm,
                                     GetFiberRadius(),
                                     GetLength()/2,
                                     0.*CLHEP::deg,
                                     360.*CLHEP::deg);
        
        G4LogicalVolume *holeVolume
            = new G4LogicalVolume(fiberHole,
                                  FindMaterial("Air"),
                                  GetName()+"/Core/Hole");
        holeVolume ->SetVisAttributes(G4VisAttributes::Invisible);
        
        G4LogicalVolume *fiberVolume
            = new G4LogicalVolume(fiber,
                                  FindMaterial(GetFiberMaterial()),
                                  GetName()+"/Core/Hole/Fiber");
        fiberVolume->SetVisAttributes(G4VisAttributes::Invisible);
        
        new G4PVPlacement(0,                   // no rotation
                          G4ThreeVector(),     // position at (0,0,0)
                          holeVolume,          // its logical volume
                          GetName()+"/Core/Hole",   // its name
                          scintVolume,         // its mother  volume
                          false,               // no boolean operations
                          0);                  // copy number
        
        new G4PVPlacement(0,                   // no rotation
                          G4ThreeVector(),     // position at (0,0,0)
                          fiberVolume,         // its logical volume
                          GetName()+"/Core/Hole/Fiber",  // its name
                          holeVolume,          // its mother  volume
                          false,               // no boolean operations
                          0);                  // copy number
    }
    
    //
    // not commented in nd280mc
    //
    if (GetVisible()) {
      G4VisAttributes *visAtt = new G4VisAttributes();
      visAtt->SetColor(0,1,1,1);
      visAtt->SetForceSolid(true);
      logVolume->SetVisAttributes(visAtt);
    }
    else {
      logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }

    return logVolume;
}

double ND280ExtrudedScintConstructor::GetTop(void) {
    double top = 0.001*CLHEP::mm;
    if (fShape == eRectangle) top = fBase;
    return top;
}

double ND280ExtrudedScintConstructor::GetCenterToCenter(void) {
    return GetBase()/2 + GetTop()/2 + GetGap();
}

void ND280ExtrudedScintConstructor::SetBase(double base) {
    fBase = base;
}

void ND280ExtrudedScintConstructor::SetHeight(double height) {
    fHeight = height;
}

void ND280ExtrudedScintConstructor::SetSensitiveDetector(
    G4VSensitiveDetector* s) {
    fSensitiveDetector = s;
}
