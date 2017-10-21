////////////////////////////////////////////////////////////
// $Id: ND280P0DuleConstructor.cc,v 1.13 2011/11/28 20:32:50 mcgrew Exp $
//

#include <cmath>
#include <algorithm>

#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>

#include "p0d/ND280P0DuleConstructor.hh"
#include "p0d/ND280P0DulePlanksConstructor.hh"
#include "ND280GapConstructor.hh"

class ND280P0DuleMessenger: public ND280ConstructorMessenger {
private:
    ND280P0DuleConstructor *fConstructor;
    G4UIcmdWithADoubleAndUnit* fEpoxyCMD;
    
public:
    ND280P0DuleMessenger(ND280P0DuleConstructor* c) 
        : ND280ConstructorMessenger(c,"Control a scintilator layer."),
          fConstructor(c) {

        fEpoxyCMD = new G4UIcmdWithADoubleAndUnit(CommandName("epoxyThickness"),
                                                    this);
        fEpoxyCMD->SetGuidance("Set the epoxy thickness which "
                                   "is added to the total P0Dule length.");
        fEpoxyCMD->SetParameterName("thickness",false);
        fEpoxyCMD->SetUnitCategory("Length");
    };

    ~ND280P0DuleMessenger() {
        delete fEpoxyCMD;
    };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd == fEpoxyCMD) {
            fConstructor->SetEpoxyThickness(
                fEpoxyCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280P0DuleConstructor::~ND280P0DuleConstructor() {}

void ND280P0DuleConstructor::Init() {
    fVisual = new G4VisAttributes();
    fVisual->SetColor(0.5,0.5,0.5,1); // gray

    // Estimated thickness based on T2K-TN-73 and US-B280-DocDB-808.  The
    // thickness is set to the nominal value.  The range of possible values is
    // between 0.20 mm and 0.27*mm.  This range was estimated based on the
    // amount of epoxy used during construction.
    SetEpoxyThickness(0.25*CLHEP::mm);

    AddConstructor(new ND280GapConstructor("LightTight",this));
    Get<ND280GapConstructor>("LightTight").SetMaterialName("Polystyrene");
    Get<ND280GapConstructor>("LightTight").SetWidth(GetLightTightWidth());
    Get<ND280GapConstructor>("LightTight").SetHeight(GetLightTightHeight());
    Get<ND280GapConstructor>("LightTight").SetLength(GetLightTightThickness());

    AddConstructor(new ND280P0DuleXPlanksConstructor("X",this));
    AddConstructor(new ND280P0DuleYPlanksConstructor("Y",this));

    SetMessenger(new ND280P0DuleMessenger(this));
}

double ND280P0DuleConstructor::GetLightTightWidth() const {
    return GetP0DuleWidth() - GetSideMPPCFrameWidth() - GetSideLIFrameWidth();
}

double ND280P0DuleConstructor::GetLightTightHeight() const {
    return GetP0DuleHeight() - GetTopMPPCFrameWidth() - GetBottomLIFrameWidth();
}

double ND280P0DuleConstructor::GetLaminationThickness() {
    double centralLength = 0.0;
    
    centralLength += Get<ND280GapConstructor>("LightTight").GetLength();
    centralLength += GetEpoxyThickness();
    centralLength += Get<ND280P0DuleXPlanksConstructor>("X").GetLength();
    centralLength += GetEpoxyThickness();
    centralLength += Get<ND280P0DuleYPlanksConstructor>("Y").GetLength();
    centralLength += GetEpoxyThickness();
    centralLength += Get<ND280GapConstructor>("LightTight").GetLength();

    return centralLength;
}

double ND280P0DuleConstructor::GetLength(void) {

    double len = GetLaminationThickness();
    len = std::max(len, GetSideLIFrameThickness()-2*GetP0DuleFrameInset());
    len = std::max(len, GetSideMPPCFrameThickness()-2*GetP0DuleFrameInset());
    len = std::max(len, GetBottomLIFrameThickness()-2*GetP0DuleFrameInset());
    len = std::max(len, GetTopMPPCFrameThickness()-2*GetP0DuleFrameInset());

    return len;
}

G4VisAttributes* ND280P0DuleConstructor::GetVisual(void) const {
    return fVisual;
}

namespace {
    bool gFirstPiece=true;
}

G4LogicalVolume *ND280P0DuleConstructor::GetPiece(void) {

    ND280P0DuleXPlanksConstructor& x 
        =  Get<ND280P0DuleXPlanksConstructor>("X");
        
    ND280P0DuleYPlanksConstructor& y 
        =  Get<ND280P0DuleYPlanksConstructor>("Y");

    ND280GapConstructor& lightTight
        = Get<ND280GapConstructor>("LightTight");
    
    G4VSolid *solid = new G4Box(GetName(),
                                GetWidth()/2, 
                                GetHeight()/2,
                                GetLength()/2);

    // Create the volume around the P0Dule.
    G4LogicalVolume *logVolume
	= new G4LogicalVolume(solid,
                              FindMaterial("Air"),
                              GetName());
    logVolume->SetVisAttributes(G4VisAttributes::Invisible);

    /// Calculate the center of the glued portion of the P0Dule.
    double planeXCenter = -GetWidth()/2 
        + GetLICoverThickness() + GetP0DuleWidth()/2;

    double planeYCenter = -GetHeight()/2 
        + GetLICoverThickness() + GetP0DuleHeight()/2;

    ////////////////////////////////////////////////
    // Build the Frame
    ////////////////////////////////////////////////

    // Create the side MPPC frame.  This uses the minimum of the actual
    // thickness and the internal thickness of the P0Dule so that the
    // super-P0Dule stacks to the right total length.
    double sideMPPCThickness = std::min(GetSideMPPCFrameThickness(), 
                                        GetLength());
    G4LogicalVolume *logSideMPPC
	= new G4LogicalVolume(new G4Box(GetName()+"/SideMPPCFrame",
                                        GetSideMPPCFrameWidth()/2, 
                                        GetSideMPPCFrameLength()/2,
                                        sideMPPCThickness/2),
                              FindMaterial("PVC"),
                              GetName()+"/SideMPPCFrame");

    if (GetVisible()) logSideMPPC->SetVisAttributes(GetVisual());
    else logSideMPPC->SetVisAttributes(G4VisAttributes::Invisible);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector((planeXCenter 
                                     + GetP0DuleWidth()/2 
                                     - GetSideMPPCFrameWidth()/2),
                                    planeYCenter,
                                    0),// position 
                      logSideMPPC, // logical volume
                      GetName()+"/SideMPPCFrame", // name
                      logVolume, // mother  volume
                      false, // no boolean operations
                      0); // no specific field

    // Create the top MPPC frame.  This uses the minimum of the actual
    // thickness and the internal thickness of the P0Dule so that the
    // super-P0Dule stacks to the right total length.
    double topMPPCThickness = std::min(GetTopMPPCFrameThickness(), 
                                       GetLength());
    G4LogicalVolume *logTopMPPC
	= new G4LogicalVolume(new G4Box(GetName()+"/TopMPPCFrame",
                                        GetTopMPPCFrameLength()/2,
                                        GetTopMPPCFrameWidth()/2, 
                                        topMPPCThickness/2),
                              FindMaterial("PVC"),
                              GetName()+"/TopMPPCFrame");

    if (GetVisible()) logTopMPPC->SetVisAttributes(GetVisual());
    else logTopMPPC->SetVisAttributes(G4VisAttributes::Invisible);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(planeXCenter,
                                    (planeYCenter + GetP0DuleHeight()/2
                                     - GetTopMPPCFrameWidth()/2),
                                    0),// position 
                      logTopMPPC, // logical volume
                      GetName()+"/TopMPPCFrame", // name
                      logVolume, // mother  volume
                      false, // no boolean operations
                      0); // no specific field

    // Create the side LI frame.  This uses the minimum of the actual
    // thickness and the internal thickness of the P0Dule so that the
    // super-P0Dule stacks to the right total length.
    double sideLIThickness = std::min(GetSideLIFrameThickness(), 
                                      GetLength());
    G4LogicalVolume *logSideLI
	= new G4LogicalVolume(new G4Box(GetName()+"/SideLIFrame",
                                        GetSideLIFrameWidth()/2,
                                        GetSideLIFrameLength()/2,
                                        sideLIThickness/2),
                              FindMaterial("PVC"),
                              GetName()+"/SideLIFrame");

    if (GetVisible()) logSideLI->SetVisAttributes(GetVisual());
    else logSideLI->SetVisAttributes(G4VisAttributes::Invisible);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector((planeXCenter 
                                     - GetP0DuleWidth()/2
                                     + GetSideLIFrameWidth()/2),
                                    planeYCenter,
                                    0),// position 
                      logSideLI, // logical volume
                      GetName()+"/SideLIFrame", // name
                      logVolume, // mother  volume
                      false, // no boolean operations
                      0); // no specific field

    // Create the bottom LI frame. This uses the minimum of the actual
    // thickness and the internal thickness of the P0Dule so that the
    // super-P0Dule stacks to the right total length.
    double botLIThickness = std::min(GetBottomLIFrameThickness(), 
                                     GetLength());
    G4LogicalVolume *logBottomLI
	= new G4LogicalVolume(new G4Box(GetName()+"/BottomLIFrame",
                                        GetBottomLIFrameLength()/2,
                                        GetBottomLIFrameWidth()/2,
                                        botLIThickness/2),
                              FindMaterial("PVC"),
                              GetName()+"/BottomLIFrame");

    if (GetVisible()) logBottomLI->SetVisAttributes(GetVisual());
    else logBottomLI->SetVisAttributes(G4VisAttributes::Invisible);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(planeXCenter,
                                    (planeYCenter
                                     - GetP0DuleHeight()/2
                                     + GetBottomLIFrameWidth()/2),
                                    0),// position 
                      logBottomLI, // logical volume
                      GetName()+"/BottomLIFrame", // name
                      logVolume, // mother  volume
                      false, // no boolean operations
                      0); // no specific field

    // Create the side LI Cover.
    G4LogicalVolume *logSideLICover
	= new G4LogicalVolume(new G4Box(GetName()+"/SideLICover",
                                        GetLICoverThickness()/2,
                                        (GetSideLIFrameLength()/2
                                         + GetTopMPPCFrameWidth()/2
                                         + GetBottomLIFrameWidth()/2
                                         + GetLICoverThickness()/2),
                                        sideLIThickness/2),
                              FindMaterial("PVC"),
                              GetName()+"/SideLICover");

    if (GetVisible()) logSideLICover->SetVisAttributes(GetVisual());
    else logSideLICover->SetVisAttributes(G4VisAttributes::Invisible);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector((planeXCenter 
                                     - GetP0DuleWidth()/2
                                     - GetLICoverThickness()/2),
                                    planeYCenter + GetLICoverThickness()/2,
                                    0),// position 
                      logSideLICover, // logical volume
                      GetName()+"/SideLICover", // name
                      logVolume, // mother  volume
                      false, // no boolean operations
                      0); // no specific field

    // Create the bottom LI Cover.
    G4LogicalVolume *logBottomLICover
	= new G4LogicalVolume(new G4Box(GetName()+"/BottomLICover",
                                        GetBottomLIFrameLength()/2,
                                        GetLICoverThickness()/2,
                                        botLIThickness/2),
                              FindMaterial("PVC"),
                              GetName()+"/BottomLICover");

    if (GetVisible()) logBottomLICover->SetVisAttributes(GetVisual());
    else logBottomLICover->SetVisAttributes(G4VisAttributes::Invisible);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(planeXCenter,
                                    (planeYCenter 
                                     - GetP0DuleHeight()/2
                                     - GetLICoverThickness()/2),
                                    0),// position 
                      logBottomLICover, // logical volume
                      GetName()+"/BottomLICover", // name
                      logVolume, // mother  volume
                      false, // no boolean operations
                      0); // no specific field
    
    //////////////////////////////////////////////
    /// Build the central region.
    //////////////////////////////////////////////
    double currentZCenter = -GetLaminationThickness()/2;
    
    double epoxyWidth = std::max(lightTight.GetWidth(),
                                 std::max(x.GetWidth(), y.GetWidth()));
    
    double epoxyHeight = std::max(lightTight.GetHeight(),
                                  std::max(x.GetHeight(), y.GetHeight()));
    
    G4LogicalVolume *epoxyVolume
	= new G4LogicalVolume(new G4Box(GetName()+"/Epoxy",
                                        epoxyWidth/2, 
                                        epoxyHeight/2,
                                        GetLaminationThickness()/2),
                              FindMaterial("P0DuleEpoxy"),
                              GetName() + "/Epoxy");
    epoxyVolume->SetVisAttributes(G4VisAttributes::Invisible);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(planeXCenter,
                                    planeYCenter,
                                    0.0), // position 
                      epoxyVolume, // logical volume
                      GetName()+"/Epoxy", // name
                      logVolume, // mother  volume
                      false, // no boolean operations
                      0); // no specific field

    // place the upstream lightTight
    G4LogicalVolume* frontLightTightVolume = lightTight.GetPiece();
    currentZCenter += lightTight.GetLength()/2;
    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0,0,currentZCenter), // position 
                      frontLightTightVolume, // logical volume
                      lightTight.GetName()+"/UpstreamCover", // name
                      epoxyVolume, // mother  volume
                      false, // no boolean operations
                      0); // no specific field
    currentZCenter += lightTight.GetLength()/2;
    currentZCenter += GetEpoxyThickness();

    // Create and place the X plane.
    if (GetVisible()) x.SetVisible(true);
    currentZCenter += x.GetLength()/2;
    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0,0,currentZCenter), // position 
                      x.GetPiece(), // logical volume
                      x.GetName(), // name
                      epoxyVolume, // mother  volume
                      false, // no boolean operations
                      0); // no specific field
    currentZCenter += x.GetLength()/2;
    currentZCenter += GetEpoxyThickness();

    // place the Y plane.
    if (GetVisible()) y.SetVisible(true);
    currentZCenter += y.GetLength()/2;
    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0,0,currentZCenter), // position 
                      y.GetPiece(), // logical volume
                      y.GetName(), // name
                      epoxyVolume, // mother  volume
                      false, // no boolean operations
                      0); // no specific field
    currentZCenter += y.GetLength()/2;
    currentZCenter += GetEpoxyThickness();

    // place the downstream lightTight
    G4LogicalVolume* backLightTightVolume = lightTight.GetPiece();
    if (backLightTightVolume) {
        currentZCenter += lightTight.GetLength()/2;
        new G4PVPlacement(0, // no rotation
                          G4ThreeVector(0,0,currentZCenter), // position 
                          backLightTightVolume, // logical volume
                          lightTight.GetName()+"/DownstreamCover", // name
                          epoxyVolume, // mother  volume
                          false, // no boolean operations
                          0); // no specific field
        currentZCenter += lightTight.GetLength()/2;
    }

    // Make sure that the Z length of the P0Dule adds up to the correct value.
    if (std::abs(2*currentZCenter - GetLaminationThickness()) > 0.01*CLHEP::mm) {
        // ND280Error("Error in P0Dule Lamination Thickness Calculation");
        // ND280Error("     Calculated Length " << 2*currentZCenter);
        // ND280Error("     Expected Length " << GetLaminationThickness());
      G4cerr << "Error in P0Dule Lamination Thickness Calculation" << G4endl;
      G4cerr << "     Calculated Length " << 2*currentZCenter << G4endl;
      G4cerr << "     Expected Length " << GetLaminationThickness() << G4endl;
        
      //G4Exception("P0Dule geometry error");
      G4Exception("ND280P0DuleConstructor::GetPiece",
		  "MyCode0002",FatalException,
		  "P0Dule geometry error");
    }

    // Make sure that the Z length of the P0Dule isn't too big.
    if (GetLaminationThickness() > GetLength()) {
        // ND280Error("Error in P0Dule Length Calculation");
        // ND280Error("     Lamination Thickness " << GetLaminationThickness());
        // ND280Error("     Total Length " << GetLength());
      G4cerr << "Error in P0Dule Length Calculation" << G4endl;
      G4cerr << "     Lamination Thickness " << GetLaminationThickness() << G4endl;
      G4cerr << "     Total Length " << GetLength() << G4endl;

        //G4Exception("P0Dule geometry error");
	G4Exception("ND280P0DuleConstructor::GetPiece",
		    "MyCode0002",FatalException,
		    "P0Dule geometry error");
    }

    if (gFirstPiece) {
        gFirstPiece = false;
        // ND280Log("P0Dule Thickness:   " << GetLength()/mm << " mm");
        // ND280Log("   Lamination Thickness: " << GetLaminationThickness()/mm << " mm");
        // ND280Log("   Skin Thickness: " << lightTight.GetLength()/mm << " mm");
        // ND280Log("   Epoxy Thickness: " << GetEpoxyThickness()/mm << " mm");
        // ND280Log("   X Plank Thickness: " << x.GetLength()/mm << " mm");
        // ND280Log("   Y Plank Thickness: " << y.GetLength()/mm << " mm");

        G4cout << "P0Dule Thickness:   " << GetLength()/CLHEP::mm << " mm" << G4endl;
	G4cout << "   Lamination Thickness: " << GetLaminationThickness()/CLHEP::mm << " mm" << G4endl;
	G4cout << "   Skin Thickness: " << lightTight.GetLength()/CLHEP::mm << " mm" << G4endl;
	G4cout << "   Epoxy Thickness: " << GetEpoxyThickness()/CLHEP::mm << " mm" << G4endl;
	G4cout << "   X Plank Thickness: " << x.GetLength()/CLHEP::mm << " mm" << G4endl;
	G4cout << "   Y Plank Thickness: " << y.GetLength()/CLHEP::mm << " mm" << G4endl;
    }

    return logVolume;
}
