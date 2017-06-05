#include <globals.hh>

#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

//#include "ND280Log.hh"

#include "p0d/ND280P0DSupportConstructor.hh"
#include "p0d/ND280ECalSP0DuleSupportConstructor.hh"
#include "p0d/ND280TargetSP0DuleSupportConstructor.hh"

class ND280P0DSupportMessenger: public ND280ConstructorMessenger {
private:
    ND280P0DSupportConstructor* fConstructor;
    G4UIcmdWithADoubleAndUnit* fWidthCMD;

public:
    ND280P0DSupportMessenger(ND280P0DSupportConstructor* c)
        : ND280ConstructorMessenger(c,"Control the ECal SP0Dule Support geometry."),
          fConstructor(c) {
        fWidthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("Width"),this);
        fWidthCMD->SetGuidance("Set the width of the support structure.");
        fWidthCMD->SetParameterName("Width",false);
        fWidthCMD->SetUnitCategory("Length");
        
    };
    
    virtual ~ND280P0DSupportMessenger() {
        delete fWidthCMD;
    };
    
    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd==fWidthCMD) {
            fConstructor->SetWidth(
                fWidthCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280P0DSupportConstructor::~ND280P0DSupportConstructor() {}

void ND280P0DSupportConstructor::Init(void) {

    fUSECalSupportLength = 305*mm;
    fUSTargetSupportLength = 886*mm;
    fCTargetSupportLength = 886*mm;
    fCECalSupportLength = 305*mm;
    fSupportSpace = 4*mm;
    fWidth   = 2320*mm;
    
    fvisual = new G4VisAttributes();
    fvisual->SetColor(0.5,0.5,0.5,1); // Grey
    
    AddConstructor(new ND280ECalSP0DuleSupportConstructor("ECalSupport",this));
    AddConstructor(new ND280TargetSP0DuleSupportConstructor("TargetSupport",this));

    SetMessenger(new ND280P0DSupportMessenger(this));
}

G4double ND280P0DSupportConstructor::GetLength(void) {
    // return fUSECalSupportLength + fSupportSpace + fUSTargetSupportLength
    //     + fSupportSpace + fCTargetSupportLength + fSupportSpace 
    //     + fCECalSupportLength;
  // NEW P0D
  return fUSECalSupportLength + fSupportSpace;
}

void ND280P0DSupportConstructor::SetVisAttributes(G4LogicalVolume* logVolume) {
    if (GetVisible()) {
        logVolume->SetVisAttributes(fvisual);
    } else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }
}

G4LogicalVolume *ND280P0DSupportConstructor::GetPiece(void) {

    G4LogicalVolume *logVolume
        = new G4LogicalVolume(new G4Box(GetName(),
                                        GetWidth()/2,
                                        GetHeight()/2,
                                        GetLength()/2),
                              FindMaterial("Aluminum"),
                              GetName());

    SetVisAttributes(logVolume);

    //////////////////////////////////////////
    // CONSTRUCT THE SUPPORT STRUCTURE
    //////////////////////////////////////////

    G4ThreeVector currentCenter(0, 0, -GetLength()/2);

    /// Place the USECal

    ND280ECalSP0DuleSupportConstructor& eCalSupport 
      = Get<ND280ECalSP0DuleSupportConstructor>("ECalSupport"); // Default aluminum

    eCalSupport.SetHeight(GetHeight());
    eCalSupport.SetWidth(GetUSECalSupportLength());
    eCalSupport.SetLength(GetWidth());

    currentCenter = currentCenter + G4ThreeVector(0, 0, eCalSupport.GetWidth()/2);

    G4RotationMatrix* rotP90Y = new G4RotationMatrix();
    rotP90Y->rotateY(90*degree);

    new G4PVPlacement(rotP90Y,          //rotation
		      currentCenter,
                      eCalSupport.GetPiece(),  // logical volume
                      eCalSupport.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    currentCenter = currentCenter + G4ThreeVector(0, 0, eCalSupport.GetWidth()/2 + GetSupportSpace());


    // NEW P0D

    /*

    ///Place the USTarget

    ND280TargetSP0DuleSupportConstructor& targetSupport
      = Get<ND280TargetSP0DuleSupportConstructor>("TargetSupport");

    targetSupport.SetHeight(GetHeight());
    targetSupport.SetWidth(GetUSTargetSupportLength());
    targetSupport.SetLength(GetWidth());

    currentCenter = currentCenter + G4ThreeVector(0, 0, targetSupport.GetWidth()/2);

    new G4PVPlacement(rotP90Y,          //rotation
		      currentCenter,
                      targetSupport.GetPiece(),  // logical volume
                      targetSupport.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    currentCenter = currentCenter + G4ThreeVector(0, 0, targetSupport.GetWidth()/2 + GetSupportSpace());

    ///Place the CTarget

    targetSupport.SetWidth(GetCTargetSupportLength());

    currentCenter = currentCenter + G4ThreeVector(0, 0, targetSupport.GetWidth()/2);

    new G4PVPlacement(rotP90Y,          //rotation
		      currentCenter,
                      targetSupport.GetPiece(),  // logical volume
                      targetSupport.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    currentCenter = currentCenter + G4ThreeVector(0, 0, targetSupport.GetWidth()/2 + GetSupportSpace());


    eCalSupport.SetWidth(GetCECalSupportLength());

    currentCenter = currentCenter + G4ThreeVector(0, 0, eCalSupport.GetWidth()/2);

    new G4PVPlacement(rotP90Y,          //rotation
		      currentCenter,
                      eCalSupport.GetPiece(),  // logical volume
                      eCalSupport.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    */

    return logVolume;
}
