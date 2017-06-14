#include <globals.hh>

#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

//#include "ND280Log.hh"

#include "p0d/ND280TargetSP0DuleSupportConstructor.hh"
#include "ND280IBeamConstructor.hh"

class ND280TargetSP0DuleSupportMessenger: public ND280ConstructorMessenger {
private:
    ND280TargetSP0DuleSupportConstructor* fConstructor;
    G4UIcmdWithADoubleAndUnit* fWidthCMD;
    G4UIcmdWithADoubleAndUnit* fHeightCMD;
    G4UIcmdWithADoubleAndUnit* fLengthCMD;

public:
    ND280TargetSP0DuleSupportMessenger(ND280TargetSP0DuleSupportConstructor* c)
        : ND280ConstructorMessenger(c,"Control the ECal SP0Dule Support geometry."),
          fConstructor(c) {
        fWidthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("Width"),this);
        fWidthCMD->SetGuidance("Set the width of the support structure.");
        fWidthCMD->SetParameterName("Width",false);
        fWidthCMD->SetUnitCategory("Length");
        
        fHeightCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("Height"),this);
        fHeightCMD->SetGuidance("Set the height of the support structure.");
        fHeightCMD->SetParameterName("Height",false);
        fHeightCMD->SetUnitCategory("Length");
        
        fLengthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("Length"),this);
        fLengthCMD->SetGuidance("Set length of the support structure.");
        fLengthCMD->SetParameterName("Length",false);
        fLengthCMD->SetUnitCategory("Length");
        
    };
    
    virtual ~ND280TargetSP0DuleSupportMessenger() {
        delete fWidthCMD;
        delete fHeightCMD;
        delete fLengthCMD;

    };
    
    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd==fWidthCMD) {
            fConstructor->SetWidth(
                fWidthCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fHeightCMD) {
            fConstructor->SetHeight(
                fHeightCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fLengthCMD) {
            fConstructor->SetLength(
                fLengthCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280TargetSP0DuleSupportConstructor::~ND280TargetSP0DuleSupportConstructor() {}

void ND280TargetSP0DuleSupportConstructor::Init(void) {

    fHeight  = 137*mm;
    fWidth   = 890*mm;
    fLength  = 2320*mm;
    
    fvisual = new G4VisAttributes();
    fvisual->SetColor(0.5,0.5,0.5,1); // Grey
    
    AddConstructor(new ND280IBeamConstructor("Rail",this));

    SetMessenger(new ND280TargetSP0DuleSupportMessenger(this));
}

void ND280TargetSP0DuleSupportConstructor::SetVisAttributes(G4LogicalVolume* logVolume) {
    if (GetVisible()) {
        logVolume->SetVisAttributes(fvisual);
    } else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }
}

G4LogicalVolume *ND280TargetSP0DuleSupportConstructor::GetPiece(void) {

    G4LogicalVolume *logVolume
        = new G4LogicalVolume(new G4Box(GetName(),
                                        GetWidth()/2,
                                        GetHeight()/2,
                                        GetLength()/2),
                              FindMaterial("Air"),
                              GetName());

    SetVisAttributes(logVolume);
    
    //////////////////////////////////////////
    // CONSTRUCT THE SUPPORT STRUCTURE
    //////////////////////////////////////////

    ND280IBeamConstructor& rail 
      = Get<ND280IBeamConstructor>("Rail"); // Default aluminum

    // Construct the side I-beams (1)

    G4double railWidth  = 76.2*mm;
    G4double railHeight = GetHeight();
    G4double railLength = GetLength();
    rail.SetWidth(railWidth);
    rail.SetHeight(railHeight);
    rail.SetLength(railLength);

    rail.SetStemThickness(5*mm);
    rail.SetBaseThickness(8*mm);

    new G4PVPlacement(0,          //rotation
		      G4ThreeVector( -(GetWidth()/2-railWidth/2),
				     0,
				     0),
                      rail.GetPiece(),  // logical volume
                      rail.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    new G4PVPlacement(0,          //rotation
		      G4ThreeVector( (GetWidth()/2-railWidth/2),
				     0,
				     0),
                      rail.GetPiece(),  // logical volume
                      rail.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    //Construct the horizontal support beams (2)

    railWidth  = 63.5*mm;
    railHeight = 65.5*mm;
    railLength = GetWidth() - 2*76.2*mm;
    
    rail.SetWidth(railWidth);
    rail.SetHeight(railHeight);
    rail.SetLength(railLength);

    rail.SetStemThickness(4*mm);
    rail.SetBaseThickness(5*mm);

    G4double railSpacing = (GetLength()-8*railWidth)/7;

    G4RotationMatrix* rotP90Y = new G4RotationMatrix();
    rotP90Y->rotateY(90*degree);

    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-railHeight/2),
				     -(7*railSpacing/2+7*railWidth/2)),
                      rail.GetPiece(),  // logical volume
                      rail.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-railHeight/2),
				     -(5*railSpacing/2+5*railWidth/2)),
                      rail.GetPiece(),  // logical volume
                      rail.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-railHeight/2),
				     -(3*railSpacing/2+3*railWidth/2)),
                      rail.GetPiece(),  // logical volume
                      rail.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-railHeight/2),
				     -(railSpacing/2+railWidth/2)),
                      rail.GetPiece(),  // logical volume
                      rail.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-railHeight/2),
				     (railSpacing/2+railWidth/2)),
                      rail.GetPiece(),  // logical volume
                      rail.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-railHeight/2),
				     (3*railSpacing/2+3*railWidth/2)),
                      rail.GetPiece(),  // logical volume
                      rail.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-railHeight/2),
				     (5*railSpacing/2+5*railWidth/2)),
                      rail.GetPiece(),  // logical volume
                      rail.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-railHeight/2),
				     (7*railSpacing/2+7*railWidth/2)),
                      rail.GetPiece(),  // logical volume
                      rail.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    return logVolume;
}
