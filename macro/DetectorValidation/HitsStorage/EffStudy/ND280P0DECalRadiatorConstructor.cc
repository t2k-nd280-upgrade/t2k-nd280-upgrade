////////////////////////////////////////////////////////////
// $Id: ND280P0DECalRadiatorConstructor.cc,v 1.3 2010/04/29 20:45:27 mcgrew Exp $
//

#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>

#include "p0d/ND280P0DECalRadiatorConstructor.hh"

/// Control the parameters of the P0D radiator layers.  This controls the
/// parameters for the P0D USECal and CECal.
class ND280P0DECalRadiatorMessenger: public ND280ConstructorMessenger {
private:
    ND280P0DECalRadiatorConstructor *fConstructor;
    G4UIcmdWithADoubleAndUnit* fEpoxyThicknessCMD;
    G4UIcmdWithADoubleAndUnit* fLeadThicknessCMD;

public:
    ND280P0DECalRadiatorMessenger(ND280P0DECalRadiatorConstructor* c) 
        : ND280ConstructorMessenger(c,
                                             "Control a gap layer."),
          fConstructor(c) {

        fEpoxyThicknessCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("epoxyThickness"),this);
        fEpoxyThicknessCMD->SetGuidance(
            "Set the average thickness of epoxy in radiator.");
        fEpoxyThicknessCMD->SetParameterName("Thickness",false);
        fEpoxyThicknessCMD->SetUnitCategory("Length");

        fLeadThicknessCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("leadThickness"),this);
        fLeadThicknessCMD->SetGuidance(
            "Set the average thickness of Pb in radiator.");
        fLeadThicknessCMD->SetParameterName("Thickness",false);
        fLeadThicknessCMD->SetUnitCategory("Length");
    };

    ~ND280P0DECalRadiatorMessenger() {
        delete fEpoxyThicknessCMD;
        delete fLeadThicknessCMD;
    };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (fLeadThicknessCMD == cmd) {
            fConstructor->
                SetLeadThickness(fLeadThicknessCMD->GetNewDoubleValue(val));
        }
        else if (fEpoxyThicknessCMD == cmd) {
            fConstructor->
                SetEpoxyThickness(fEpoxyThicknessCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280P0DECalRadiatorConstructor::~ND280P0DECalRadiatorConstructor() {}

const G4VisAttributes* ND280P0DECalRadiatorConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.8,0.8,0.8,1); 
    return visual;
}

void ND280P0DECalRadiatorConstructor::Init() {
    SetLeadThickness(3.45*mm);
    SetEpoxyThickness(0.25*mm);
    SetMessenger(new ND280P0DECalRadiatorMessenger(this));
}

G4LogicalVolume *ND280P0DECalRadiatorConstructor::GetPiece(void) {

    // Create a box to contain the radiator
    G4VSolid *solid = new G4Box(GetName(),
				GetWidth()/2, 
				GetHeight()/2,
                                GetLength()/2);

    // Create the volume around the radiator layer.
    G4LogicalVolume *logVolume
	= new G4LogicalVolume(solid,
                              FindMaterial("P0DuleEpoxy"),
                              GetName());

    // Create the aluminum frames
    G4VSolid *bottomFrame = new G4Box(GetName()+"/BottomFrame",
                                      GetWidth()/2,
                                      GetBottomFrame()/2,
                                      GetLeadThickness()/2);
    G4LogicalVolume *logBottomFrame 
        = new G4LogicalVolume(bottomFrame,
                              FindMaterial("Aluminum"),
                              GetName()+"/BottomFrame");

    G4VSolid *sideFrame = new G4Box(GetName()+"/SideFrame",
                                    GetSideFrame()/2,
                                    GetSideFrameLength()/2,
                                    GetLeadThickness()/2);
    G4LogicalVolume *logSideFrame 
        = new G4LogicalVolume(sideFrame,
                              FindMaterial("Aluminum"),
                              GetName()+"/SideFrame");
    
    G4VSolid *topFrame = new G4Box(GetName()+"/TopFrame",
                                   GetTopFrameLength()/2,
                                   GetTopFrame()/2,
                                   GetLeadThickness()/2);
    G4LogicalVolume *logTopFrame 
        = new G4LogicalVolume(topFrame,
                              FindMaterial("Aluminum"),
                              GetName()+"/TopFrame");
    
    // Create the Stainless Steel Skins.
    G4VSolid *steelSkin = new G4Box(GetName()+"/SteelSkin",
                                    GetSteelWidth()/2,
                                    GetSteelHeight()/2,
                                    GetSteelThickness()/2);
    G4LogicalVolume *logSteelSkin 
        = new G4LogicalVolume(steelSkin,
                              FindMaterial("Steel"),
                              GetName()+"/SteelSkin");

    // Construct the radiator from the parts.
    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0,0,
                                    -GetLength()/2 + GetSteelThickness()/2),
                      logSteelSkin,
                      GetName()+"/SteelSkin",
                      logVolume,
                      false,
                      0);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0,0,
                                    GetLength()/2 - GetSteelThickness()/2),
                      logSteelSkin,
                      GetName()+"/SteelSkin",
                      logVolume,
                      false,
                      0);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0,
                                    -GetHeight()/2 + GetBottomFrame()/2,
                                    0),
                      logBottomFrame,
                      GetName()+"/BottomFrame",
                      logVolume,
                      false,
                      0);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0,
                                    GetHeight()/2 - GetTopFrame()/2,
                                    0),
                      logTopFrame,
                      GetName()+"/TopFrame",
                      logVolume,
                      false,
                      0);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(GetWidth()/2 - GetSideFrame()/2, 
                                    (-GetHeight()/2
                                     +GetBottomFrame()
                                     +GetSideFrameLength()/2),
                                    0),
                      logSideFrame,
                      GetName()+"/LeftFrame",
                      logVolume,
                      false,
                      0);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(-GetWidth()/2 + GetSideFrame()/2, 
                                    (-GetHeight()/2
                                     +GetBottomFrame()
                                     +GetSideFrameLength()/2),
                                    0),
                      logSideFrame,
                      GetName()+"/RightFrame",
                      logVolume,
                      false,
                      0);

    
    // Build all of the plates.
    for (int column = 0; column<5; ++column) {
        double xPos = (column-2)*GetPlateWidth();
        double yPos = -GetHeight()/2+GetBottomFrame();
        for (int row=0;;++row) {
            if (column%2 == 0) {
                if (row == 0) yPos += GetPlateHeight()/2;
                else yPos += GetPlateHeight();
            }
            else {
                if (row == 0) yPos += GetPlateHeight()/4;
                else if (row == 1) yPos += 0.75*GetPlateHeight();
                else if (row == 5) yPos += 0.75*GetPlateHeight();
                else yPos += GetPlateHeight();
            }

            if (column%2==1 && (row == 0 || row == 5)) {
                G4VSolid *smallPlate = new G4Box(GetName()+"/SmallPlate",
                                                 GetPlateWidth()/2,
                                                 0.5*GetPlateHeight()/2,
                                                 GetLeadThickness()/2);
                G4LogicalVolume *logSmallPlate
                    = new G4LogicalVolume(smallPlate,
                                          FindMaterial("Lead"),
                                          GetName()+"/SmallPlate");
                new G4PVPlacement(0,
                                  G4ThreeVector(xPos, yPos, 0),
                                  logSmallPlate,
                                  GetName()+"/SmallPlate",
                                  logVolume,
                                  false,
                                  0);
            }
            else {
                G4VSolid *bigPlate = new G4Box(GetName()+"/BigPlate",
                                               GetPlateWidth()/2,
                                               GetPlateHeight()/2,
                                               GetLeadThickness()/2);
                G4LogicalVolume *logBigPlate
                    = new G4LogicalVolume(bigPlate,
                                          FindMaterial("Lead"),
                                          GetName()+"/BigPlate");
                new G4PVPlacement(0,
                                  G4ThreeVector(xPos, yPos, 0),
                                  logBigPlate,
                                  GetName()+"/BigPlate",
                                  logVolume,
                                  false,
                                  0);
            }
            if (column%2==1 && 5<=row) break;
            else if (column%2==0 && 4 <= row)  break;
        }
    }

    if (GetVisible()) {
        logVolume->SetVisAttributes(GetVisual());
    } 
    else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }

    return logVolume;
}
