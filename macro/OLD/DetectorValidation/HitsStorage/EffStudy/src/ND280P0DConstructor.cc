#include <vector>

#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

//#include "ND280Log.hh"

#include "ND280P0DConstructor.hh"
#include "ND280USECalSP0DuleConstructor.hh"
#include "ND280USTargetSP0DuleConstructor.hh"
#include "ND280CTargetSP0DuleConstructor.hh"
#include "ND280CECalSP0DuleConstructor.hh"

class ND280P0DMessenger: public ND280ConstructorMessenger {
private:
    ND280P0DConstructor* fConstructor;
    G4UIcmdWithADoubleAndUnit* fLIClearanceCMD;
    G4UIcmdWithADoubleAndUnit* fBottomClearanceCMD;
    G4UIcmdWithADoubleAndUnit* fUpstreamClearanceCMD;
    G4UIcmdWithADoubleAndUnit* fXRotationCMD;
    G4UIcmdWithADoubleAndUnit* fYRotationCMD;
    G4UIcmdWithADoubleAndUnit* fZRotationCMD;
    G4UIcmdWith3VectorAndUnit* fUSECalTransCMD;
    G4UIcmdWith3VectorAndUnit* fUSTargetTransCMD;
    G4UIcmdWith3VectorAndUnit* fCTargetTransCMD;
    G4UIcmdWith3VectorAndUnit* fCECalTransCMD;

public:
    ND280P0DMessenger(ND280P0DConstructor* c) 
        : ND280ConstructorMessenger(c,"Control the P0D geometry."),
          fConstructor(c) {

        fLIClearanceCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("liClearance"),this);
        fLIClearanceCMD->SetGuidance(
            "Set design clearance for LI and basket side.");
        fLIClearanceCMD->SetParameterName("Clearance",false);
        fLIClearanceCMD->SetUnitCategory("Length");

        fBottomClearanceCMD
            =new G4UIcmdWithADoubleAndUnit(CommandName("bottomClearance"),this);
        fBottomClearanceCMD->SetGuidance(
            "Set design clearance between the P0D bottom and the P0D support.");
        fBottomClearanceCMD->SetParameterName("Clearance",false);
        fBottomClearanceCMD->SetUnitCategory("Length");

        fUpstreamClearanceCMD
            =new G4UIcmdWithADoubleAndUnit(
                CommandName("upstreamClearance"),this);
        fUpstreamClearanceCMD->SetGuidance(
            "Set design clearance between upstream end of the P0D and basket.");
        fUpstreamClearanceCMD->SetParameterName("Clearance",false);
        fUpstreamClearanceCMD->SetUnitCategory("Length");

        fXRotationCMD 
            = new G4UIcmdWithADoubleAndUnit(CommandName("xRotation"),this);
        fXRotationCMD->SetGuidance("Set rotation about the X axis.");
        fXRotationCMD->SetParameterName("Angle",false);
        fXRotationCMD->SetUnitCategory("Angle");

        fYRotationCMD 
            = new G4UIcmdWithADoubleAndUnit(CommandName("yRotation"),this);
        fYRotationCMD->SetGuidance("Set rotation about the Y axis.");
        fYRotationCMD->SetParameterName("Angle",false);
        fYRotationCMD->SetUnitCategory("Angle");

        fZRotationCMD 
            = new G4UIcmdWithADoubleAndUnit(CommandName("zRotation"),this);
        fZRotationCMD->SetGuidance("Set rotation about the Z axis.");
        fZRotationCMD->SetParameterName("Angle",false);
        fZRotationCMD->SetUnitCategory("Angle");

        fUSECalTransCMD = new G4UIcmdWith3VectorAndUnit(
            CommandName("setUSECalTranslation"), this);
        fUSECalTransCMD->SetGuidance(
            "Set the USECal translation from the design position");
        fUSECalTransCMD->SetParameterName("transX","transY","transZ",false);
        fUSECalTransCMD->SetUnitCategory("Length");

        fUSTargetTransCMD = new G4UIcmdWith3VectorAndUnit(
            CommandName("setUSTargetTranslation"), this);
        fUSTargetTransCMD->SetGuidance(
            "Set the upstream target translation from the design position");
        fUSTargetTransCMD->SetParameterName("transX","transY","transZ",false);
        fUSTargetTransCMD->SetUnitCategory("Length");

        fCTargetTransCMD = new G4UIcmdWith3VectorAndUnit(
            CommandName("setCTargetTranslation"), this);
        fCTargetTransCMD->SetGuidance(
            "Set the central target translation from the design position");
        fCTargetTransCMD->SetParameterName("transX","transY","transZ",false);
        fCTargetTransCMD->SetUnitCategory("Length");

        fCECalTransCMD = new G4UIcmdWith3VectorAndUnit(
            CommandName("setCECalTranslation"), this);
        fCECalTransCMD->SetGuidance(
            "Set the CECal translation from the design position");
        fCECalTransCMD->SetParameterName("transX","transY","transZ",false);
        fCECalTransCMD->SetUnitCategory("Length");
    }

    virtual ~ND280P0DMessenger() {
        delete fLIClearanceCMD;
        delete fBottomClearanceCMD;
        delete fUpstreamClearanceCMD;
        delete fXRotationCMD;
        delete fYRotationCMD;
        delete fZRotationCMD;
        delete fUSECalTransCMD;
        delete fUSTargetTransCMD;
        delete fCTargetTransCMD;
        delete fCECalTransCMD;
    }

    void SetNewValue(G4UIcommand *cmd, G4String val) {
	if (cmd == fLIClearanceCMD) {
            fConstructor->
                SetLIClearance(fLIClearanceCMD->GetNewDoubleValue(val));
        }
        else if (cmd == fBottomClearanceCMD) {
            fConstructor->
                SetBottomClearance(
                    fBottomClearanceCMD->GetNewDoubleValue(val));
        }
        else if (cmd == fUpstreamClearanceCMD) {
            fConstructor->
                SetUpstreamClearance(
                    fUpstreamClearanceCMD->GetNewDoubleValue(val));
        }
        else if (cmd == fXRotationCMD) {
            fConstructor->
                SetXRotation(fXRotationCMD->GetNewDoubleValue(val));
        }
        else if (cmd == fYRotationCMD) {
            fConstructor->
                SetYRotation(fYRotationCMD->GetNewDoubleValue(val));
        }
        else if (cmd == fZRotationCMD) {
            fConstructor->
                SetZRotation(fXRotationCMD->GetNewDoubleValue(val));
        }
        else if (cmd == fUSECalTransCMD) {
            fConstructor->SetUSECalTrans(
                fUSECalTransCMD->GetNew3VectorValue(val));
        }
        else if (cmd == fUSTargetTransCMD) {
            fConstructor->SetUSTargetTrans(
                fUSTargetTransCMD->GetNew3VectorValue(val));
        }
        else if (cmd == fCTargetTransCMD) {
            fConstructor->SetCTargetTrans(
                fCTargetTransCMD->GetNew3VectorValue(val));
        }
        else if (cmd == fCECalTransCMD) {
            fConstructor->SetCECalTrans(
                fCECalTransCMD->GetNew3VectorValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    }
};

ND280P0DConstructor::~ND280P0DConstructor() {}

void ND280P0DConstructor::Init(void) {

    // From measurements taken 10/26/09 by Taylor and Rutorburies
    SetUSECalDsLIPosition(323*mm);
    SetUSECalSkewAngle(2.71*mrad);

    // From measurements taken 10/26/09 by Taylor and Rutorburies.  The
    // measured value is 1225mm from the front of the P0D.  The dimensions
    // were measured before the water target was filled, and after swelling
    // the water targets swelled slightly.  The USWT position has been tuned
    // to match the relative change in position.
    SetUSWTDsLIPosition(1226.15*mm);
    SetUSWTSkewAngle(2.03*mrad);

    // From measurements taken 10/26/09 by Taylor and Rutorburies
    SetCWTDsLIPosition(2079*mm);
    SetCWTSkewAngle(3.39*mrad);

    // From measurements taken 10/26/09 by Taylor and Rutorburies
    SetCECalDsLIPosition(2396*mm);
    SetCECalSkewAngle(3.39*mrad);

    // Space to allow for the relative alignment of the  super-P0Dules.
    SetLengthAdjustmentSpace(3*mm);
    SetWidthAdjustmentSpace(10*mm);
    SetHeightAdjustmentSpace(20*mm);

    // From measurements taken on 10/26/09 by Taylor and Rutorburies
    SetUpstreamClearance(18*mm);

#ifdef USE_BASKET_MEASUREMENTS
    // From measurements taken on 8/23/10 by McGrew
    SetBottomClearance(10*mm);
    SetLIClearance(18*mm);
#else
    // From TPC/P0D alignment on 9/7/11 by Gilje.  The center in global
    // coordinates is at -36 mm (X) and -1 mm (Y)
    SetBottomClearance(-3*mm);
    SetLIClearance(14*mm);
#endif

    // Set the rotation angles.
    SetXRotation(0*degree);
    SetYRotation(0*degree);
    SetZRotation(0*degree);

    // Set default translation values.
    SetUSECalTrans(G4ThreeVector(0,0,0));
    SetUSTargetTrans(G4ThreeVector(0,0,0));
    SetCTargetTrans(G4ThreeVector(0,0,0));
    SetCECalTrans(G4ThreeVector(0,0,0));
    
    AddConstructor(new ND280USECalSP0DuleConstructor("USECal", this));
    AddConstructor(new ND280USTargetSP0DuleConstructor("USTarget", this));
    AddConstructor(new ND280CTargetSP0DuleConstructor("CTarget", this));
    AddConstructor(new ND280CECalSP0DuleConstructor("CECal", this));

    SetMessenger(new ND280P0DMessenger(this));
}

// Get the length of the P0D.  This does not include empty space between the
// upstream end of the P0D and the basket (about 18mm).
double ND280P0DConstructor::GetLength(void) {
  // double length = fCECalDsLIPosition;
  // length += std::max(0.0,
  //   Get<ND280CECalSP0DuleConstructor>("CECal").GetECalBraceThickness());
  // length += std::max(0.0,std::abs(fCECalSkewAngle)*GetWidth());
  // length -= fUSECalDsLIPosition;
  // length += GetUSECalLength();
  // length += std::max(0.0,std::abs(fUSECalSkewAngle)*GetWidth());
  // length += GetLengthAdjustmentSpace();
  // return length;

  // NEW P0D
  double length = 0.;
  length += GetUSECalLength();
  length += std::max(0.0,std::abs(fUSECalSkewAngle)*GetWidth());
  length += GetLengthAdjustmentSpace();
  return length;
}

double ND280P0DConstructor::GetWidth(void) {
    double width = 0;
    width = std::max(
		     width,Get<ND280USECalSP0DuleConstructor>("USECal").GetWidth());
    // NEW P0D
    //width = std::max(
    //width,Get<ND280USTargetSP0DuleConstructor>("USTarget").GetWidth());
    //width = std::max(
    //width,Get<ND280CTargetSP0DuleConstructor>("CTarget").GetWidth());
    //width = std::max(
    //width,Get<ND280CECalSP0DuleConstructor>("CECal").GetWidth());
    // Allow a small amount of space for adjustment.
    width += GetWidthAdjustmentSpace();
    return width;
}

double ND280P0DConstructor::GetHeight(void) {
  double height = 0;
  height = std::max(
		    height,Get<ND280USECalSP0DuleConstructor>("USECal").GetHeight());
  // NEW P0D
  //height = std::max(
  //height,Get<ND280USTargetSP0DuleConstructor>("USTarget").GetHeight());
  //height = std::max(
  //height,Get<ND280CTargetSP0DuleConstructor>("CTarget").GetHeight());
  //height = std::max(
  //height,Get<ND280CECalSP0DuleConstructor>("CECal").GetHeight());
  // Allow a small amount of space for adjustment.
  height += GetHeightAdjustmentSpace();
  return height;
}

double ND280P0DConstructor::GetLISpace() const {
    return std::max(0.0, GetLIClearance() - GetWidthAdjustmentSpace()/2);
}

double ND280P0DConstructor::GetBottomSpace() const {
    return GetBottomClearance() - GetHeightAdjustmentSpace()/2;
}

double ND280P0DConstructor::GetUpstreamSpace() const {
    return std::max(0.0, GetUpstreamClearance() - GetLengthAdjustmentSpace()/2);
}

// Get the USECal position.
G4ThreeVector ND280P0DConstructor::GetUSECalPosition() {
    // Center the super-P0Dule with respect to the bottom.
    double xCenter = -GetWidth()/2;
    // Allow space for fine adjustment of the relative super-P0Dule position.
    xCenter += GetWidthAdjustmentSpace()/2;
    // Correct the position back to the center of the constructed volume.
    xCenter += GetUSECalWidth()/2;
    // Center the super-P0Dule with respect to the bottom.
    double yCenter = -GetHeight()/2;
    // Allow space for fine adjustment of the relative super-P0Dule position.
    yCenter += GetHeightAdjustmentSpace()/2;
    // Correct the position back to the center of the constructed volume.
    yCenter += GetUSECalHeight()/2;
    // The skew angle is a couple mrad, so this uses the small angle approx.
    double zCenter = - GetLength()/2;
    // Allow space for fine adjustment of the relative super-P0Dule position.
    zCenter += GetLengthAdjustmentSpace()/2;
    // Add the distance to the downstream edge of the super-P0Dule
    zCenter += fUSECalDsLIPosition;
    // Correct the position back to the center of the constructed volume.
    zCenter -= GetUSECalLength()/2;
    // Correct for the skew angle.  It's a couple mrad, so this uses the small
    // angle approx.
    zCenter += fUSECalSkewAngle*GetWidth()/2;
    // Correct for the offset at the upstream end of the USECal.
    zCenter -= (fUSECalDsLIPosition - GetUSECalLength());
    return G4ThreeVector(xCenter,yCenter,zCenter) + GetUSECalTrans();
}

// Get the USWT position.
G4ThreeVector ND280P0DConstructor::GetUSWTPosition() {
    // Center the super-P0Dule with respect to the bottom.
    double xCenter = -GetWidth()/2;
    // Allow space for fine adjustment of the relative super-P0Dule position.
    xCenter += GetWidthAdjustmentSpace()/2;
    // Correct the position back to the center of the constructed volume.
    xCenter += GetUSTargetWidth()/2;
    // Center the super-P0Dule with respect to the bottom.
    double yCenter = -GetHeight()/2;
    // Allow space for fine adjustment of the relative super-P0Dule position.
    yCenter += GetHeightAdjustmentSpace()/2;
    // Correct the position back to the center of the constructed volume.
    yCenter += GetUSTargetHeight()/2;
    // The skew angle is a couple mrad, so this uses the small angle approx.
    double zCenter = - GetLength()/2;
    // Allow space for fine adjustment of the relative super-P0Dule position.
    zCenter += GetLengthAdjustmentSpace()/2;
    // Add the distance to the downstream edge of the super-P0Dule
    zCenter += fUSWTDsLIPosition;
    // Correct the position back to the center of the constructed volume.
    zCenter -= GetUSTargetLength()/2;
    // Correct for the skew angle.  It's a couple mrad, so this uses the small
    // angle approx.
    zCenter += fUSWTSkewAngle*GetWidth()/2;
    // Correct for the offset at the upstream end of the USECal.
    zCenter -= (fUSECalDsLIPosition - GetUSECalLength());
    return G4ThreeVector(0,0,zCenter) + GetUSTargetTrans();
}

// Get the CWT position.
G4ThreeVector ND280P0DConstructor::GetCWTPosition() {
    // Center the super-P0Dule with respect to the bottom.
    double xCenter = -GetWidth()/2;
    // Allow space for fine adjustment of the relative super-P0Dule position.
    xCenter += GetWidthAdjustmentSpace()/2;
    // Correct the position back to the center of the constructed volume.
    xCenter += GetCTargetWidth()/2;
    // Center the super-P0Dule with respect to the bottom.
    double yCenter = -GetHeight()/2;
    // Allow space for fine adjustment of the relative super-P0Dule position.
    yCenter += GetHeightAdjustmentSpace()/2;
    // Correct the position back to the center of the constructed volume.
    yCenter += GetCTargetHeight()/2;
    // The skew angle is a couple mrad, so this uses the small angle approx.
    double zCenter = - GetLength()/2;
    // Allow space for fine adjustment of the relative super-P0Dule position.
    zCenter += GetLengthAdjustmentSpace()/2;
    // Add the distance to the downstream edge of the super-P0Dule
    zCenter += fCWTDsLIPosition;
    // Correct the position back to the center of the constructed volume.
    zCenter -= GetCTargetLength()/2;
    // Correct for the skew angle.  It's a couple mrad, so this uses the small
    // angle approx.
    zCenter += fCWTSkewAngle*GetWidth()/2;
    // Correct for the offset at the upstream end of the USECal.
    zCenter -= (fUSECalDsLIPosition - GetUSECalLength());
    return G4ThreeVector(0,0,zCenter) + GetCTargetTrans();
}

// Get the CECal position.  This is slightly complicated since the DS LI
// measurement does not include the thickness of the ECal bracing, but the
// bracing is build by the CECal constructor so a correction needs to be
// applied.  The bracing is made of 3/4 inch aluminum.
G4ThreeVector ND280P0DConstructor::GetCECalPosition() {
    // Center the super-P0Dule with respect to the bottom.
    double xCenter = -GetWidth()/2;
    // Allow space for fine adjustment of the relative super-P0Dule position.
    xCenter += GetWidthAdjustmentSpace()/2;
    // Correct the position back to the center of the constructed volume.
    xCenter += GetCECalWidth()/2;
    // Center the super-P0Dule with respect to the bottom.
    double yCenter = -GetHeight()/2;
    // Allow space for fine adjustment of the relative super-P0Dule position.
    yCenter += GetHeightAdjustmentSpace()/2;
    // Correct the position back to the center of the constructed volume.
    yCenter += GetCECalHeight()/2;
    // Get the upstream edge of the P0D.
    double zCenter = - GetLength()/2;
    // Allow space for fine adjustment of the relative super-P0Dule position.
    zCenter += GetLengthAdjustmentSpace()/2;
    // Add the distance to the downstream edge of the super-P0Dule
    zCenter += fCECalDsLIPosition;
    // Add the thickness of the ecal bracing that is added to the CECal in the
    // ND280MC constructor.
    zCenter += std::max(0.0,
        Get<ND280CECalSP0DuleConstructor>("CECal").GetECalBraceThickness());
    // Correct the position back to the center of the constructed volume.
    zCenter -= GetCECalLength()/2;
    // Correct for the skew angle.  It's a couple mrad, so this uses the small
    // angle approx.
    zCenter += fCECalSkewAngle*GetWidth()/2;
    // Correct for the offset at the upstream end of the USECal.
    zCenter -= (fUSECalDsLIPosition - GetUSECalLength());
    return G4ThreeVector(0,0,zCenter) + GetCECalTrans();
}

// Get the USECal rotation.
G4RotationMatrix* ND280P0DConstructor::GetUSECalRotation() const {
    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateY(fUSECalSkewAngle);
    return rot;
}

// Get the USWT rotation.
G4RotationMatrix* ND280P0DConstructor::GetUSWTRotation() const {
    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateY(fUSWTSkewAngle);
    return rot;
}

// Get the CWT rotation.
G4RotationMatrix* ND280P0DConstructor::GetCWTRotation() const {
    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateY(fCWTSkewAngle);
    return rot;
}

// Get the CECal rotation.
G4RotationMatrix* ND280P0DConstructor::GetCECalRotation() const {
    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateY(fCECalSkewAngle);
    return rot;
}

// Get the length of the USECal 
double ND280P0DConstructor::GetUSECalLength(void) {
    return Get<ND280USECalSP0DuleConstructor>("USECal").GetLength();
}

// Get the width of the USECal 
double ND280P0DConstructor::GetUSECalWidth(void) {
    return Get<ND280USECalSP0DuleConstructor>("USECal").GetWidth();
}

// Get the height of the USECal 
double ND280P0DConstructor::GetUSECalHeight(void) {
    return Get<ND280USECalSP0DuleConstructor>("USECal").GetHeight();
}

// Get the length of the USTarget 
double ND280P0DConstructor::GetUSTargetLength(void) {
    return Get<ND280USTargetSP0DuleConstructor>("USTarget").GetLength();
}

// Get the width of the USTarget 
double ND280P0DConstructor::GetUSTargetWidth(void) {
    return Get<ND280USTargetSP0DuleConstructor>("USTarget").GetWidth();
}

// Get the height of the USTarget 
double ND280P0DConstructor::GetUSTargetHeight(void) {
    return Get<ND280USTargetSP0DuleConstructor>("USTarget").GetHeight();
}

// Get the length of the CTarget 
double ND280P0DConstructor::GetCTargetLength(void) {
    return Get<ND280CTargetSP0DuleConstructor>("CTarget").GetLength();
}

// Get the width of the CTarget 
double ND280P0DConstructor::GetCTargetWidth(void) {
    return Get<ND280CTargetSP0DuleConstructor>("CTarget").GetWidth();
}

// Get the height of the CTarget 
double ND280P0DConstructor::GetCTargetHeight(void) {
    return Get<ND280CTargetSP0DuleConstructor>("CTarget").GetHeight();
}

// Get the length of the CECal 
double ND280P0DConstructor::GetCECalLength(void) {
    return Get<ND280CECalSP0DuleConstructor>("CECal").GetLength();
}

// Get the width of the CECal 
double ND280P0DConstructor::GetCECalWidth(void) {
    return Get<ND280CECalSP0DuleConstructor>("CECal").GetWidth();
}

// Get the height of the CECal 
double ND280P0DConstructor::GetCECalHeight(void) {
    return Get<ND280CECalSP0DuleConstructor>("CECal").GetHeight();
}

G4LogicalVolume *ND280P0DConstructor::GetPiece(void) {
    if (GetLength()<0.1*mm) return NULL;

    G4LogicalVolume *logVolume
	= new G4LogicalVolume(new G4Box(GetName(),
                                        GetWidth()/2.0,
                                        GetHeight()/2.0,
					GetLength()/2.0),
                              FindMaterial("Air"),
                              GetName());

    if (GetVisible()) {
        G4VisAttributes *visual = new G4VisAttributes();
        visual->SetColor(0.0,0.8,0.3,1); // bluish green
        visual->SetForceWireframe(true);
        logVolume->SetVisAttributes(visual);
    } 
    else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }
    
    ND280USECalSP0DuleConstructor& usECal
        = Get<ND280USECalSP0DuleConstructor>("USECal");
    new G4PVPlacement(GetUSECalRotation(), // rotation
                      GetUSECalPosition(),     // position 
                      usECal.GetPiece(),       // logical volume
                      usECal.GetName(),   // name
                      logVolume,          // mother  volume
                      false,              // no boolean operations
                      0);                 // no specific field

    
    // NEW P0D

    /*

    ND280USTargetSP0DuleConstructor& usTarget
        = Get<ND280USTargetSP0DuleConstructor>("USTarget");
    new G4PVPlacement(GetUSWTRotation(),    // rotation
                      GetUSWTPosition(),     // position 
                      usTarget.GetPiece(),  // logical volume
                      usTarget.GetName(),   // name
                      logVolume,          // mother  volume
                      false,              // no boolean operations
                      0);                 // no specific field

    ND280CTargetSP0DuleConstructor& cTarget
        = Get<ND280CTargetSP0DuleConstructor>("CTarget");
    new G4PVPlacement(GetCWTRotation(),     // rotation
                      GetCWTPosition(),     // position 
                      cTarget.GetPiece(),  // logical volume
                      cTarget.GetName(),   // name
                      logVolume,          // mother  volume
                      false,              // no boolean operations
                      0);                 // no specific field

    ND280CECalSP0DuleConstructor& cECal
        = Get<ND280CECalSP0DuleConstructor>("CECal");
    new G4PVPlacement(GetCECalRotation(), // rotation
                      GetCECalPosition(), // position 
                      cECal.GetPiece(),       // logical volume
                      cECal.GetName(),   // name
                      logVolume,          // mother  volume
                      false,              // no boolean operations
                      0);                 // no specific field

    */


    // ND280Log("P0D Envelope Length: " << GetLength()/mm << " mm");
    // ND280Log("P0D Envelope Width: " << GetWidth()/mm << " mm");
    // ND280Log("P0D Envelope Height: " << GetHeight()/mm << " mm");
    // ND280Log("    USECal Length: "  
    //          << usECal.GetLength()/mm << " mm"
    //          << "    Y Rotation: " << fUSECalSkewAngle/mrad << " mrad");
    // ND280Log("    USTarget Length: "
    //          << usTarget.GetLength()/mm<< " mm"
    //          << "    Y Rotation: " << fUSWTSkewAngle/mrad << " mrad");
    // ND280Log("    CTarget Length: " 
    //          << cTarget.GetLength()/mm<< " mm"
    //          << "    Y Rotation: " << fCWTSkewAngle/mrad << " mrad");
    // ND280Log("    CECal Length:  "  
    //          << cECal.GetLength()/mm << " mm"
    //          << "    Y Rotation: " << fCECalSkewAngle/mrad << " mrad");
    G4cout << "P0D Envelope Length: " << GetLength()/mm << " mm" << G4endl;
    G4cout << "P0D Envelope Width: " << GetWidth()/mm << " mm" << G4endl;
    G4cout << "P0D Envelope Height: " << GetHeight()/mm << " mm" << G4endl;
    G4cout << "    USECal Length: "  
	   << usECal.GetLength()/mm << " mm"
	   << "    Y Rotation: " << fUSECalSkewAngle/mrad << " mrad" << G4endl;
    // G4cout << "    USTarget Length: "
    // 	   << usTarget.GetLength()/mm<< " mm"
    // 	   << "    Y Rotation: " << fUSWTSkewAngle/mrad << " mrad" << G4endl;
    // G4cout << "    CTarget Length: " 
    // 	   << cTarget.GetLength()/mm<< " mm"
    // 	   << "    Y Rotation: " << fCWTSkewAngle/mrad << " mrad" << G4endl;
    // G4cout << "    CECal Length:  "  
    // 	   << cECal.GetLength()/mm << " mm"
    // 	   << "    Y Rotation: " << fCECalSkewAngle/mrad << " mrad" << G4endl;

    return logVolume;
}
